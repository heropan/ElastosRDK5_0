
#include "display/WifiDisplayController.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringUtils.h>
#include <os/Handler.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::IEnumeration;
using Elastos::Utility::Logging::Slogger;
using Elastos::Net::INetworkInterfaceHelper;
using Elastos::Net::CNetworkInterfaceHelper;
using Elastos::Net::INetworkInterface;
using Elastos::Droid::Wifi::IWpsInfo;
using Elastos::Droid::Wifi::CWpsInfo;
using Elastos::Droid::Wifi::P2p::CWifiP2pDevice;
using Elastos::Droid::Wifi::P2p::IWifiP2pConfig;
using Elastos::Droid::Wifi::P2p::CWifiP2pConfig;
using Elastos::Droid::Wifi::P2p::IWifiP2pWfdInfo;
using Elastos::Droid::Wifi::P2p::CWifiP2pWfdInfo;
using Elastos::Droid::Wifi::P2p::EIID_IWifiP2pManagerActionListener;
using Elastos::Droid::Wifi::P2p::EIID_IWifiP2pManagerPeerListListener;
using Elastos::Droid::Wifi::P2p::EIID_IWifiP2pManagerGroupInfoListener;
using Elastos::Droid::Media::IRemoteDisplayHelper;
using Elastos::Droid::Media::CRemoteDisplayHelper;
using Elastos::Droid::Media::EIID_IRemoteDisplayListener;
using Elastos::Droid::Database::IContentObserver;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Provider::ISettingsGlobal;
using Elastos::Droid::Provider::CSettingsGlobal;
using Elastos::Droid::Hardware::Display::IWifiDisplayStatus;
using Elastos::Droid::Hardware::Display::CWifiDisplay;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

const String WifiDisplayController::TAG("WifiDisplayController");
const Boolean WifiDisplayController::DEBUG = FALSE;
const Int32 WifiDisplayController::DEFAULT_CONTROL_PORT;
const Int32 WifiDisplayController::MAX_THROUGHPUT;
const Int32 WifiDisplayController::CONNECTION_TIMEOUT_SECONDS;
const Int32 WifiDisplayController::RTSP_TIMEOUT_SECONDS;
const Int32 WifiDisplayController::DISCOVER_PEERS_MAX_RETRIES;
const Int32 WifiDisplayController::DISCOVER_PEERS_RETRY_DELAY_MILLIS;
const Int32 WifiDisplayController::CONNECT_MAX_RETRIES;
const Int32 WifiDisplayController::CONNECT_RETRY_DELAY_MILLIS;
const Int32 WifiDisplayController::REMOTE_SUBMIX_ADDRESS;


//==============================================================================
// WifiDisplayController::MyContentObserver
//==============================================================================
WifiDisplayController::MyContentObserver::MyContentObserver(
    /* [in] */ WifiDisplayController* owner,
    /* [in] */ IHandler* handler)
    : ContentObserver(handler)
    , mHost(owner)
{}

ECode WifiDisplayController::MyContentObserver::OnChange(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    mHost->UpdateSettings();

    return NOERROR;
}

//==============================================================================
// WifiDisplayController::WFDInfoActionListener
//==============================================================================
CAR_INTERFACE_IMPL(WifiDisplayController::WFDInfoActionListener, IWifiP2pManagerActionListener);

WifiDisplayController::WFDInfoActionListener::WFDInfoActionListener(
    /* [in] */ WifiDisplayController* owner)
    : mHost(owner)
{}

ECode WifiDisplayController::WFDInfoActionListener::OnSuccess()
{
    if (WifiDisplayController::DEBUG) {
        Slogger::D(WifiDisplayController::TAG, "Successfully set WFD info.");
    }

    if (mHost->mWfdEnabling) {
        mHost->mWfdEnabling = FALSE;
        mHost->mWfdEnabled = TRUE;
        mHost->ReportFeatureState();
    }

    return NOERROR;
}

ECode WifiDisplayController::WFDInfoActionListener::OnFailure(
    /* [in] */ Int32 reason)
{
    if (WifiDisplayController::DEBUG) {
        Slogger::D(WifiDisplayController::TAG, "Failed to set WFD info with reason %d.", reason);
    }
    mHost->mWfdEnabling = FALSE;

    return NOERROR;
}

//==============================================================================
// WifiDisplayController::DiscoverPeersActionListener
//==============================================================================
CAR_INTERFACE_IMPL(WifiDisplayController::DiscoverPeersActionListener, IWifiP2pManagerActionListener);

WifiDisplayController::DiscoverPeersActionListener::DiscoverPeersActionListener(
    /* [in] */ WifiDisplayController* owner)
    : mHost(owner)
{}

ECode WifiDisplayController::DiscoverPeersActionListener::OnSuccess()
{
    if (WifiDisplayController::DEBUG) {
        Slogger::D(WifiDisplayController::TAG, "Discover peers succeeded.  Requesting peers now.");
    }

    mHost->mDiscoverPeersInProgress = FALSE;
    mHost->RequestPeers();

    return NOERROR;
}

ECode WifiDisplayController::DiscoverPeersActionListener::OnFailure(
    /* [in] */ Int32 reason)
{
    if (WifiDisplayController::DEBUG) {
        Slogger::D(WifiDisplayController::TAG, "Discover peers failed with reason %d", reason);
    }

    if (mHost->mDiscoverPeersInProgress) {
        if (reason == 0 && mHost->mDiscoverPeersRetriesLeft > 0
            && mHost->mWfdEnabled) {
            AutoPtr<IRunnable> runnable = new DiscoverPeersRunnable(reason, mHost);
            Boolean result;
            mHost->mHandler->PostDelayed(runnable, DISCOVER_PEERS_RETRY_DELAY_MILLIS, &result);
        }
        else {
            mHost->HandleScanFinished();
            mHost->mDiscoverPeersInProgress = FALSE;
        }
    }

    return NOERROR;
}

//==============================================================================
// WifiDisplayController::RequestPeersPeerListListener
//==============================================================================
CAR_INTERFACE_IMPL(WifiDisplayController::RequestPeersPeerListListener, IWifiP2pManagerPeerListListener);

WifiDisplayController::RequestPeersPeerListListener::RequestPeersPeerListListener(
    /* [in] */ WifiDisplayController* owner)
    : mHost(owner)
{}

ECode WifiDisplayController::RequestPeersPeerListListener::OnPeersAvailable(
    /* [in] */ IWifiP2pDeviceList* peers)
{
    mHost->mAvailableWifiDisplayPeers.Clear();

    AutoPtr<ArrayOf<IWifiP2pDevice*> > array;
    peers->GetDeviceList((ArrayOf<IWifiP2pDevice*>**)&array);

    if (WifiDisplayController::DEBUG) {
        Slogger::D(WifiDisplayController::TAG, "OnPeersAvailable, peer's count: %d.",
            array ? array->GetLength() : 0);
    }

    if (array) {
        for (Int32 i = 0; i < array->GetLength(); ++i) {
            AutoPtr<IWifiP2pDevice> device = (*array)[i];
            if (WifiDisplayController::DEBUG) {
                Slogger::D(WifiDisplayController::TAG, "OnPeersAvailable: peer %d: %s",
                    i, mHost->DescribeWifiP2pDevice(device).string());
            }

            if (mHost->IsWifiDisplay(device)) {
                mHost->mAvailableWifiDisplayPeers.PushBack(device);
            }
        }
    }

    if (WifiDisplayController::DEBUG) {
        Slogger::D(WifiDisplayController::TAG, "OnPeersAvailable, Wifi display peer's count: %d.",
            mHost->mAvailableWifiDisplayPeers.GetSize());
    }

    mHost->HandleScanFinished();

    return NOERROR;
}

//==============================================================================
// WifiDisplayController::WifiP2pReceiver
//==============================================================================
CAR_INTERFACE_IMPL(WifiDisplayController::MyActionListenerEx2, IWifiP2pManagerActionListener);

WifiDisplayController::MyActionListenerEx2::MyActionListener(
    /* [in] */ WifiDisplayController* owner,
    /* [in] */ IWifiP2pDevice* oldDevice,
    /* [in] */ Boolean isDisconnecting)
    : mHost(owner)
    , mOldDevice(oldDevice)
    , mIsDisconnecting(isDisconnecting)
{}

ECode WifiDisplayController::MyActionListenerEx2::OnSuccess()
{
    String deviceName;
    mOldDevice->GetDeviceName(&deviceName);
    if (mIsDisconnecting)
        Slogger::I(TAG, "Disconnected from Wifi display: %s", deviceName.string());
    else
        Slogger::I(TAG, "Canceling connection to Wifi display: %s", deviceName.string());
    Next();

    return NOERROR;
}

ECode WifiDisplayController::MyActionListenerEx2::OnFailure(
    /* [in] */ Int32 reason)
{
    String deviceName;
    mOldDevice->GetDeviceName(&deviceName);
    if (mIsDisconnecting)
        Slogger::I(TAG, "Failed to disconnect from Wifi display: %s, reason=%s",
            deviceName.string(), reason);
    else
        Slogger::I(TAG, "Failed to cancel connection to Wifi display: %s, reason=%s",
            deviceName.string(), reason);
    Next();

    return NOERROR;
}

void WifiDisplayController::MyActionListenerEx2::Next()
{
    if (mHost->mConnectingDevice == mOldDevice) {
        mHost->mConnectingDevice = NULL;
        mHost->UpdateConnection();
    }
}

//==============================================================================
// WifiDisplayController::ConnectDeviceActionListener
//==============================================================================
CAR_INTERFACE_IMPL(WifiDisplayController::ConnectDeviceActionListener, IWifiP2pManagerActionListener);

WifiDisplayController::ConnectDeviceActionListener::ConnectDeviceActionListener(
    /* [in] */ WifiDisplayController* owner,
    /* [in] */ IWifiP2pDevice* newDevice)
    : mHost(owner)
    , mNewDevice(newDevice)
{}

ECode WifiDisplayController::ConnectDeviceActionListener::OnSuccess()
{
    // The connection may not yet be established.  We still need to wait
    // for WIFI_P2P_CONNECTION_CHANGED_ACTION.  However, we might never
    // get that broadcast, so we register a timeout.
    String deviceName;
    mNewDevice->GetDeviceName(&deviceName);
    Slogger::I(WifiDisplayController::TAG, "Initiated connection to Wifi display: %s", deviceName.string());

    Boolean result;
    mHost->mHandler->PostDelayed(mHost->mConnectionTimeout, CONNECTION_TIMEOUT_SECONDS * 1000, &result);
    return NOERROR;
}

ECode WifiDisplayController::ConnectDeviceActionListener::OnFailure(
    /* [in] */ Int32 reason)
{
    if (mHost->mConnectingDevice == mNewDevice) {
        String deviceName;
        mNewDevice->GetDeviceName(&deviceName);
        Slogger::I(WifiDisplayController::TAG, "Failed to initiate connection to Wifi display: %s, reason=%d",
            deviceName.string(), reason);
        mHost->mConnectingDevice = NULL;
        mHost->HandleConnectionFailure(FALSE);
    }

    return NOERROR;
}

//==============================================================================
// WifiDisplayController::DisconnectingDeviceActionListener
//==============================================================================
CAR_INTERFACE_IMPL(WifiDisplayController::DisconnectingDeviceActionListener, IWifiP2pManagerActionListener);

WifiDisplayController::DisconnectingDeviceActionListener::DisconnectingDeviceActionListener(
    /* [in] */ WifiDisplayController* owner,
    /* [in] */ IWifiP2pDevice* oldDevice)
    : mHost(owner)
    , mOldDevice(oldDevice)
{}

ECode WifiDisplayController::DisconnectingDeviceActionListener::OnSuccess()
{
    String deviceName;
    mOldDevice->GetDeviceName(&deviceName);
    Slogger::I(WifiDisplayController::TAG, "Disconnected from Wifi display: %s", deviceName.string());

    Next();

    return NOERROR;
}

ECode WifiDisplayController::DisconnectingDeviceActionListener::OnFailure(
    /* [in] */ Int32 reason)
{
    String deviceName;
    mOldDevice->GetDeviceName(&deviceName);
    Slogger::I(WifiDisplayController::TAG, "Failed to disconnect from Wifi display: %s, reason=%s",
        deviceName.string(), reason);

    Next();

    return NOERROR;
}

void WifiDisplayController::DisconnectingDeviceActionListener::Next()
{
    if (mHost->mDisconnectingDevice == mOldDevice) {
        mHost->mDisconnectingDevice = NULL;
        mHost->UpdateConnection();
    }
}

//==============================================================================
// WifiDisplayController::WifiP2pReceiver
//==============================================================================
CAR_INTERFACE_IMPL(WifiDisplayController::CancelingDeviceActionListener, IWifiP2pManagerActionListener);

WifiDisplayController::CancelingDeviceActionListener::CancelingDeviceActionListener(
    /* [in] */ WifiDisplayController* owner,
    /* [in] */ IWifiP2pDevice* oldDevice)
    : mHost(owner)
    , mOldDevice(oldDevice)
{}

ECode WifiDisplayController::CancelingDeviceActionListener::OnSuccess()
{
    String deviceName;
    mOldDevice->GetDeviceName(&deviceName);
    Slogger::I(WifiDisplayController::TAG, "Canceling connection to Wifi display: %s", deviceName.string());

    Next();

    return NOERROR;
}

ECode WifiDisplayController::CancelingDeviceActionListener::OnFailure(
    /* [in] */ Int32 reason)
{
    String deviceName;
    mOldDevice->GetDeviceName(&deviceName);
    Slogger::I(TAG, "Failed to cancel connection to Wifi display: %s, reason=%s",
        deviceName.string(), reason);

    Next();

    return NOERROR;
}

void WifiDisplayController::CancelingDeviceActionListener::Next()
{
    if (mHost->mCancelingDevice == mOldDevice) {
        mHost->mCancelingDevice = NULL;
        mHost->UpdateConnection();
    }
}

//==============================================================================
// WifiDisplayController::MyRemoteDisplayListener
//==============================================================================
CAR_INTERFACE_IMPL(WifiDisplayController::MyRemoteDisplayListener, IRemoteDisplayListener);

WifiDisplayController::MyRemoteDisplayListener::MyRemoteDisplayListener(
    /* [in] */ WifiDisplayController* owner,
    /* [in] */ IWifiP2pDevice* oldDevice)
    : mHost(owner)
    , mOldDevice(oldDevice)
{}

ECode WifiDisplayController::MyRemoteDisplayListener::OnDisplayConnected(
    /* [in] */ ISurface* surface,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 flags)
{
    if (mHost->mConnectedDevice == mOldDevice && !mHost->mRemoteDisplayConnected) {
        String deviceName;
        mHost->mConnectedDevice->GetDeviceName(&deviceName);
        Slogger::I(WifiDisplayController::TAG, "Opened RTSP connection with Wifi display: %d", deviceName.string());

        mHost->mRemoteDisplayConnected = TRUE;
        mHost->mHandler->RemoveCallbacks(mHost->mRtspTimeout);

        AutoPtr<IWifiDisplay> display = CreateWifiDisplay(mHost->mConnectedDevice);
        mHost->AdvertiseDisplay(display, surface, width, height, flags);
    }
    return NOERROR;
}

ECode WifiDisplayController::MyRemoteDisplayListener::OnDisplayDisconnected()
{
    if (mHost->mConnectedDevice == mOldDevice) {
        String deviceName;
        mHost->mConnectedDevice->GetDeviceName(&deviceName);
        Slogger::I(WifiDisplayController::TAG, "Closed RTSP connection with Wifi display: %s", deviceName.string());
        mHost->mHandler->RemoveCallbacks(mHost->mRtspTimeout);
        mHost->Disconnect();
    }
    return NOERROR;
}

ECode WifiDisplayController::MyRemoteDisplayListener::OnDisplayError(
    /* [in] */ Int32 error)
{
    if (mHost->mConnectedDevice == mOldDevice) {
        String deviceName;
        mHost->mConnectedDevice->GetDeviceName(&deviceName);
        Slogger::I(WifiDisplayController::TAG, "Lost RTSP connection with Wifi display due to error %d: %s",
            error, deviceName.string());
        mHost->mHandler->RemoveCallbacks(mHost->mRtspTimeout);
        mHost->HandleConnectionFailure(FALSE);
    }
    return NOERROR;
}

//==============================================================================
// WifiDisplayController::ConnectionChangedGroupInfoListener
//==============================================================================
CAR_INTERFACE_IMPL(WifiDisplayController::ConnectionChangedGroupInfoListener, IWifiP2pManagerGroupInfoListener);

WifiDisplayController::ConnectionChangedGroupInfoListener::ConnectionChangedGroupInfoListener(
    /* [in] */ WifiDisplayController* owner)
    : mHost(owner)
{}

ECode WifiDisplayController::ConnectionChangedGroupInfoListener::OnGroupInfoAvailable(
    /* [in] */ IWifiP2pGroup* info)
{
    if (DEBUG) {
        Slogger::D(WifiDisplayController::TAG, "Received group info: %s",
            DescribeWifiP2pGroup(info).string());
    }

    Boolean isContains;
    if (mHost->mConnectingDevice != NULL &&
        !(info->Contains(mHost->mConnectingDevice, &isContains), isContains)) {
        String deviceName;
        mHost->mConnectingDevice->GetDeviceName(&deviceName);
        Slogger::I(WifiDisplayController::TAG, "Aborting connection to Wifi display because "
            "the current P2P group does not contain the device "
            "we expected to find: %s , group info was: %s",
            deviceName.string(), DescribeWifiP2pGroup(info).string());
        mHost->HandleConnectionFailure(FALSE);
        return NOERROR;
    }

    if (mHost->mDesiredDevice != NULL &&
        !(info->Contains(mHost->mDesiredDevice, &isContains), isContains)) {
        mHost->Disconnect();
        return NOERROR;
    }

    if (mHost->mConnectingDevice != NULL
        && mHost->mConnectingDevice == mHost->mDesiredDevice) {
        String deviceName;
        mHost->mConnectingDevice->GetDeviceName(&deviceName);
        Slogger::I(WifiDisplayController::TAG, "Connected to Wifi display: %s", deviceName.string());

        mHost->mHandler->RemoveCallbacks(mHost->mConnectionTimeout);
        mHost->mConnectedDeviceGroupInfo = info;
        mHost->mConnectedDevice = mHost->mConnectingDevice;
        mHost->mConnectingDevice = NULL;
        mHost->UpdateConnection();
    }
    return NOERROR;
}

//==============================================================================
// WifiDisplayController::ReportFeatureStateRunnable
//==============================================================================
WifiDisplayController::ReportFeatureStateRunnable::ReportFeatureStateRunnable(
    /* [in] */ Int32 featureState,
    /* [in] */ WifiDisplayController* owner)
    : mFeatureState(featureState)
    , mHost(owner)
{}

ECode WifiDisplayController::ReportFeatureStateRunnable::Run()
{
    if (mHost->mListener)
        mHost->mListener->OnFeatureStateChanged(mFeatureState);
    return NOERROR;
}

//==============================================================================
// WifiDisplayController::DiscoverPeersRunnable
//==============================================================================
WifiDisplayController::DiscoverPeersRunnable::DiscoverPeersRunnable(
    /* [in] */ Int32 reason,
    /* [in] */ WifiDisplayController* owner)
    : mReason(reason)
    , mHost(owner)
{}

ECode WifiDisplayController::DiscoverPeersRunnable::Run()
{
    if (mHost->mDiscoverPeersInProgress) {
        if (mHost->mDiscoverPeersRetriesLeft > 0 && mHost->mWfdEnabled) {
            mHost->mDiscoverPeersRetriesLeft -= 1;
            if (WifiDisplayController::DEBUG) {
                Slogger::D(WifiDisplayController::TAG, "Retrying discovery.  Retries left: %d",
                    mHost->mDiscoverPeersRetriesLeft);
            }
            mHost->TryDiscoverPeers();
        }
        else {
            mHost->HandleScanFinished();
            mHost->mDiscoverPeersInProgress = FALSE;
        }
    }
    return NOERROR;
}

//==============================================================================
// WifiDisplayController::ScanStartedRunnable
//==============================================================================
WifiDisplayController::ScanStartedRunnable::ScanStartedRunnable(
    /* [in] */ WifiDisplayController* owner)
    : mHost(owner)
{}

ECode WifiDisplayController::ScanStartedRunnable::Run()
{
    if (mHost->mListener)
        mHost->mListener->OnScanStarted();
    return NOERROR;
}

//==============================================================================
// WifiDisplayController::ScanFinishedRunnable
//==============================================================================
WifiDisplayController::ScanFinishedRunnable::ScanFinishedRunnable(
    /* [in] */ ArrayOf<IWifiDisplay*>* displays,
    /* [in] */ WifiDisplayController* owner)
    : mDisplays(displays)
    , mHost(owner)
{}

ECode WifiDisplayController::ScanFinishedRunnable::Run()
{
    if (mHost->mListener) {
        mHost->mListener->OnScanFinished(mDisplays);
    }
    return NOERROR;
}

//==============================================================================
// WifiDisplayController::ConnectionFailureRunnable
//==============================================================================
WifiDisplayController::ConnectionFailureRunnable::ConnectionFailureRunnable(
    /* [in] */ IWifiP2pDevice* oldDevice,
    /* [in] */ WifiDisplayController* owner)
    : mOldDevice(oldDevice)
    , mHost(owner)
{}

ECode WifiDisplayController::ConnectionFailureRunnable::Run()
{
    if (mHost->mDesiredDevice.Get() == mOldDevice && mHost->mConnectionRetriesLeft > 0) {
        mHost->mConnectionRetriesLeft -= 1;
        Slogger::I(WifiDisplayController::TAG, "Retrying Wifi display connection.  Retries left: %d",
            mHost->mConnectionRetriesLeft);
        mHost->RetryConnection();
    }
    return NOERROR;
}

//==============================================================================
// WifiDisplayController::ConnectionTimeoutRunnable
//==============================================================================
WifiDisplayController::ConnectionTimeoutRunnable::ConnectionTimeoutRunnable(
    /* [in] */ WifiDisplayController* owner)
    : mHost(owner)
{}

ECode WifiDisplayController::ConnectionTimeoutRunnable::Run()
{
    if (mHost->mConnectingDevice != NULL &&
        mHost->mConnectingDevice == mHost->mDesiredDevice) {
        String deviceName;
        mHost->mConnectingDevice->GetDeviceName(&deviceName);
        Slogger::I(TAG, "Timed out waiting for Wifi display connection after %d seconds: %s",
            CONNECTION_TIMEOUT_SECONDS, deviceName.string());

        mHost->HandleConnectionFailure(TRUE);
    }
    return NOERROR;
}

//==============================================================================
// WifiDisplayController::RtspTimeoutRunnable
//==============================================================================
WifiDisplayController::RtspTimeoutRunnable::RtspTimeoutRunnable(
    /* [in] */ WifiDisplayController* owner)
    : mHost(owner)
{}

ECode WifiDisplayController::RtspTimeoutRunnable::Run()
{
    if (mHost->mConnectedDevice != NULL
        && mHost->mRemoteDisplay != NULL && !mHost->mRemoteDisplayConnected) {
        String deviceName;
        mHost->mConnectedDevice->GetDeviceName(&deviceName);
        Slogger::I(TAG, "Timed out waiting for Wifi RTSP connection after %d seconds: %s",
            RTSP_TIMEOUT_SECONDS, deviceName.string());

        mHost->HandleConnectionFailure(TRUE);
    }
    return NOERROR;
}

//==============================================================================
// WifiDisplayController::WifiP2pReceiver
//==============================================================================
WifiDisplayController::WifiP2pReceiver::WifiP2pReceiver(
    /* [in] */ WifiDisplayController* owner)
    : mHost(owner)
{}

ECode WifiDisplayController::WifiP2pReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (action.Equals(IWifiP2pManager::WIFI_P2P_STATE_CHANGED_ACTION)) {
        // This broadcast is sticky so we'll always get the initial Wifi P2P state
        // on startup.
        Int32 value;
        intent->GetInt32Extra(
            IWifiP2pManager::EXTRA_WIFI_STATE,
            IWifiP2pManager::WIFI_P2P_STATE_DISABLED,
            &value);
        Boolean enabled = value == IWifiP2pManager::WIFI_P2P_STATE_ENABLED;
        if (DEBUG) {
            Slogger::D(TAG, "Received WIFI_P2P_STATE_CHANGED_ACTION: enabled=%d", enabled);
        }

        mHost->HandleStateChanged(enabled);
    }
    else if (action.Equals(IWifiP2pManager::WIFI_P2P_PEERS_CHANGED_ACTION)) {
        if (DEBUG) {
            Slogger::D(TAG, "Received WIFI_P2P_PEERS_CHANGED_ACTION.");
        }

        mHost->HandlePeersChanged();
    }
    else if (action.Equals(IWifiP2pManager::WIFI_P2P_CONNECTION_CHANGED_ACTION)) {
        AutoPtr<IParcelable> parcelable;
        intent->GetParcelableExtra(
            IWifiP2pManager::EXTRA_NETWORK_INFO,
            (IParcelable**)&parcelable);
        AutoPtr<INetworkInfo> networkInfo = INetworkInfo::Probe(parcelable.Get());

        if (DEBUG) {
            String ni;
            if (networkInfo) {
                networkInfo->ToString(&ni);
            }
            Slogger::D(TAG, "Received WIFI_P2P_CONNECTION_CHANGED_ACTION: networkInfo=%s",
                ni.string());
        }

        mHost->HandleConnectionChanged(networkInfo);
    }

    return NOERROR;
}

//==============================================================================
// WifiDisplayController
//==============================================================================

WifiDisplayController::WifiDisplayController(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler,
    /* [in] */ IWifiDisplayControllerListener* listener)
    : mWifiP2pEnabled(FALSE)
    , mWfdEnabled(FALSE)
    , mWfdEnabling(FALSE)
    , mWifiDisplayOnSetting(FALSE)
    , mDiscoverPeersInProgress(FALSE)
    , mDiscoverPeersRetriesLeft(0)
    , mConnectionRetriesLeft(0)
    , mRemoteDisplayConnected(FALSE)
    , mRemoteSubmixOn(FALSE)
    , mAdvertisedDisplayWidth(0)
    , mAdvertisedDisplayHeight(0)
    , mAdvertisedDisplayFlags(0)
{
    mConnectionTimeout = new ConnectionTimeoutRunnable(this);
    mRtspTimeout = new RtspTimeoutRunnable(this);
    mWifiP2pReceiver = new WifiP2pReceiver(this);

    mContext = context;
    mHandler = handler;
    mListener = listener;

    assert(mHandler != NULL);
    AutoPtr<ILooper> looper;
    mHandler->GetLooper((ILooper**)&looper);
    AutoPtr<IInterface> service;
    context->GetSystemService(IContext::WIFI_P2P_SERVICE, (IInterface**)&service);
    mWifiP2pManager = IWifiP2pManager::Probe(service);
    if (mWifiP2pManager) {
        mWifiP2pManager->Initialize(context, looper, NULL, (IWifiP2pManagerChannel**)&mWifiP2pChannel);
    }
    else {
        Slogger::E(TAG, "WifiP2pManager not ready!");
    }

    service = NULL;
    context->GetSystemService(IContext::AUDIO_SERVICE, (IInterface**)&service);
    mAudioManager = IAudioManager::Probe(service);

    String nullStr;
    AutoPtr<IIntent> stickyIntent;
    AutoPtr<IIntentFilter> intentFilter;
    CIntentFilter::New((IIntentFilter**)&intentFilter);
    intentFilter->AddAction(IWifiP2pManager::WIFI_P2P_STATE_CHANGED_ACTION);
    intentFilter->AddAction(IWifiP2pManager::WIFI_P2P_PEERS_CHANGED_ACTION);
    intentFilter->AddAction(IWifiP2pManager::WIFI_P2P_CONNECTION_CHANGED_ACTION);
    context->RegisterReceiver(mWifiP2pReceiver, intentFilter, nullStr, mHandler,
        (IIntent**)&stickyIntent);

    AutoPtr<IContentObserver> settingsObserver = new MyContentObserver(this, mHandler);

    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<ISettingsGlobal> sg;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sg);
    AutoPtr<IUri> uri;
    sg->GetUriFor(ISettingsGlobal::WIFI_DISPLAY_ON, (IUri**)&uri);
    resolver->RegisterContentObserver(uri, FALSE, settingsObserver);
    UpdateSettings();
}

void WifiDisplayController::UpdateSettings()
{
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<ISettingsGlobal> sg;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sg);
    Int32 value;
    sg->GetInt32(resolver, ISettingsGlobal::WIFI_DISPLAY_ON, 0, &value);
    // TODO:
    mWifiDisplayOnSetting = TRUE; //value != 0;

    UpdateWfdEnableState();
}

void WifiDisplayController::Dump(
    /* [in] */ IPrintWriter* pw)
{
    // pw->PrintStringln(String("mWifiDisplayOnSetting=") + mWifiDisplayOnSetting);
    // pw->PrintStringln(String("mWifiP2pEnabled=") + mWifiP2pEnabled);
    // pw->PrintStringln(String("mWfdEnabled=") + mWfdEnabled);
    // pw->PrintStringln(String("mWfdEnabling=") + mWfdEnabling);
    // pw->PrintStringln(String("mNetworkInfo=") + mNetworkInfo);
    // pw->PrintStringln(String("mDiscoverPeersInProgress=") + mDiscoverPeersInProgress);
    // pw->PrintStringln(String("mDiscoverPeersRetriesLeft=") + mDiscoverPeersRetriesLeft);
    // pw->PrintStringln(String("mDesiredDevice=") + DescribeWifiP2pDevice(mDesiredDevice));
    // pw->PrintStringln(String("mConnectingDisplay=") + DescribeWifiP2pDevice(mConnectingDevice));
    // pw->PrintStringln(String("mConnectedDevice=") + DescribeWifiP2pDevice(mConnectedDevice));
    // pw->PrintStringln(String("mConnectionRetriesLeft=") + mConnectionRetriesLeft);
    // pw->PrintStringln(String("mRemoteDisplay=") + mRemoteDisplay);
    // pw->PrintStringln(String("mRemoteDisplayInterface=") + mRemoteDisplayInterface);
    // pw->PrintStringln(String("mRemoteDisplayConnected=") + mRemoteDisplayConnected);
    // pw->PrintStringln(String("mRemoteSubmixOn=") + mRemoteSubmixOn);
    // pw->PrintStringln(String("mAdvertisedDisplay=") + mAdvertisedDisplay);
    // pw->PrintStringln(String("mAdvertisedDisplaySurface=") + mAdvertisedDisplaySurface);
    // pw->PrintStringln(String("mAdvertisedDisplayWidth=") + mAdvertisedDisplayWidth);
    // pw->PrintStringln(String("mAdvertisedDisplayHeight=") + mAdvertisedDisplayHeight);
    // pw->PrintStringln(String("mAdvertisedDisplayFlags=") + mAdvertisedDisplayFlags);

    // pw->PrintStringln(String("mAvailableWifiDisplayPeers: size=") + mAvailableWifiDisplayPeers.size());
    // for (WifiP2pDevice device : mAvailableWifiDisplayPeers) {
    //     pw->PrintStringln(String("  ") + DescribeWifiP2pDevice(device));
    // }
}

void WifiDisplayController::RequestScan()
{
    DiscoverPeers();
}

void WifiDisplayController::RequestConnect(
    /* [in] */ const String& address)
{
    String deviceAddress;
    List<AutoPtr<IWifiP2pDevice> >::Iterator iter = mAvailableWifiDisplayPeers.Begin();
    for (; iter != mAvailableWifiDisplayPeers.End(); ++iter) {
        (*iter)->GetDeviceAddress(&deviceAddress);
        if (deviceAddress.Equals(address)) {
            Connect(iter->Get());
        }
    }
}

void WifiDisplayController::RequestDisconnect()
{
    Disconnect();
}

void WifiDisplayController::UpdateWfdEnableState()
{
    if (mWifiDisplayOnSetting && mWifiP2pEnabled) {
        // WFD should be enabled.
        if (!mWfdEnabled && !mWfdEnabling) {
            mWfdEnabling = TRUE;

            AutoPtr<IWifiP2pWfdInfo> wfdInfo;
            // TODO: delete
            if (DEBUG && FALSE) {
                CWifiP2pWfdInfo::New(337, 0, 300, (IWifiP2pWfdInfo**)&wfdInfo);
            }
            else {
                CWifiP2pWfdInfo::New((IWifiP2pWfdInfo**)&wfdInfo);
                wfdInfo->SetWfdEnabled(TRUE);
                Boolean result;
                wfdInfo->SetDeviceType(IWifiP2pWfdInfo::WFD_SOURCE, &result);
                wfdInfo->SetSessionAvailable(TRUE);
                wfdInfo->SetControlPort(DEFAULT_CONTROL_PORT);
                wfdInfo->SetMaxThroughput(MAX_THROUGHPUT);
            }

            AutoPtr<IWifiP2pManagerActionListener> l = new WFDInfoActionListener(this);
            if (mWifiP2pManager) {
                mWifiP2pManager->SetWFDInfo(mWifiP2pChannel, wfdInfo, l);
            }
        }
    }
    else {
        // WFD should be disabled.
        mWfdEnabling = FALSE;
        mWfdEnabled = FALSE;
        ReportFeatureState();
        Disconnect();
    }
}

void WifiDisplayController::ReportFeatureState()
{
    Int32 featureState = ComputeFeatureState();

    AutoPtr<IRunnable> runnable = new ReportFeatureStateRunnable(featureState, this);
    Boolean result;
    mHandler->Post(runnable, &result);
}

Int32 WifiDisplayController::ComputeFeatureState()
{
    if (!mWifiP2pEnabled) {
        return IWifiDisplayStatus::FEATURE_STATE_DISABLED;
    }
    return mWifiDisplayOnSetting ? IWifiDisplayStatus::FEATURE_STATE_ON :
        IWifiDisplayStatus::FEATURE_STATE_OFF;
}

void WifiDisplayController::DiscoverPeers()
{
    if (!mDiscoverPeersInProgress) {
        mDiscoverPeersInProgress = TRUE;
        mDiscoverPeersRetriesLeft = DISCOVER_PEERS_MAX_RETRIES;
        HandleScanStarted();
        TryDiscoverPeers();
    }
}

void WifiDisplayController::TryDiscoverPeers()
{
    if (mWifiP2pManager) {
        AutoPtr<DiscoverPeersActionListener> l = new DiscoverPeersActionListener(this);
        mWifiP2pManager->DiscoverPeers(mWifiP2pChannel, l.Get());
    }
}

void WifiDisplayController::RequestPeers()
{
    if (mWifiP2pManager) {
        AutoPtr<IWifiP2pManagerPeerListListener> l = new RequestPeersPeerListListener(this);
        mWifiP2pManager->RequestPeers(mWifiP2pChannel, l.Get());
    }
}

void WifiDisplayController::HandleScanStarted()
{
    AutoPtr<IRunnable> runnable = new ScanStartedRunnable(this);
    Boolean result;
    mHandler->Post(runnable, &result);
}

void WifiDisplayController::HandleScanFinished()
{
    Int32 count = mAvailableWifiDisplayPeers.GetSize();
    AutoPtr<ArrayOf<IWifiDisplay*> > displays = ArrayOf<IWifiDisplay*>::Alloc(count);

    List<AutoPtr<IWifiP2pDevice> >::Iterator iter = mAvailableWifiDisplayPeers.Begin();
    for (Int32 i = 0; iter != mAvailableWifiDisplayPeers.End(); ++iter, ++i) {
        AutoPtr<IWifiP2pDevice> device = *iter;
        AutoPtr<IWifiDisplay> display = CreateWifiDisplay(device);
        displays->Set(i, display);
        UpdateDesiredDevice(device);
    }

    AutoPtr<IRunnable> runnable = new ScanFinishedRunnable(displays, this);
    Boolean result;
    mHandler->Post(runnable, &result);
}

void WifiDisplayController::UpdateDesiredDevice(
    /* [in] */ IWifiP2pDevice* device)
{
    assert(NULL != device);
    // Handle the case where the device to which we are connecting or connected
    // may have been renamed or reported different properties in the latest scan.
    String address;
    device->GetDeviceAddress(&address);
    if (mDesiredDevice != NULL) {
        String addr;
        mDesiredDevice->GetDeviceAddress(&addr);

        if (addr.Equals(address)) {
            if (DEBUG) {
                Slogger::D(TAG, "updateDesiredDevice: new information %s",
                    DescribeWifiP2pDevice(device).string());
            }
            mDesiredDevice->Update(device);
            if (mAdvertisedDisplay != NULL) {
                mAdvertisedDisplay->GetDeviceAddress(&addr);
                if (addr.Equals(address)) {
                    AutoPtr<IWifiDisplay> display = CreateWifiDisplay(mDesiredDevice);
                    ReadvertiseDisplay(display);
                }
            }
        }
    }
}

void WifiDisplayController::Connect(
    /* [in] */ IWifiP2pDevice* device)
{
    assert(device != NULL);

    String address1, address2;
    device->GetDeviceAddress(&address1);
    if (mDesiredDevice != NULL &&
        !(mDesiredDevice->GetDeviceAddress(&address2), address2.Equals(address1))) {
        if (DEBUG) {
            Slogger::D(TAG, "connect: nothing to do, already connecting to %s",
                DescribeWifiP2pDevice(device).string());
        }
        return;
    }

    if (mConnectedDevice != NULL
        && !(mConnectedDevice->GetDeviceAddress(&address2), address2.Equals(address1))
        && mDesiredDevice == NULL) {
        if (DEBUG) {
            Slogger::D(TAG, "connect: nothing to do, already connected to %s"
                " and not part way through connecting to a different device.",
                DescribeWifiP2pDevice(device).string());
        }
        return;
    }

    mDesiredDevice = device;
    mConnectionRetriesLeft = CONNECT_MAX_RETRIES;
    UpdateConnection();
}

void WifiDisplayController::Disconnect()
{
    mDesiredDevice = NULL;
    UpdateConnection();
}

void WifiDisplayController::RetryConnection()
{
    // Cheap hack.  Make a new instance of the device object so that we
    // can distinguish it from the previous connection attempt.
    // This will cause us to tear everything down before we try again.
    AutoPtr<IWifiP2pDevice> temp;
    CWifiP2pDevice::New(mDesiredDevice, (IWifiP2pDevice**)&temp);
    mDesiredDevice = temp;
    UpdateConnection();
}

void WifiDisplayController::UpdateConnection()
{
    // Step 1. Before we try to connect to a new device, tell the system we
    // have disconnected from the old one.
    if (mRemoteDisplay != NULL && mConnectedDevice != mDesiredDevice) {
        String deviceName;
        mConnectedDevice->GetDeviceName(&deviceName);
        Slogger::I(TAG, "Stopped listening for RTSP connection on %s from Wifi display: %s",
            mRemoteDisplayInterface.string(), deviceName.string());

        mRemoteDisplay->Dispose();
        mRemoteDisplay = NULL;
        mRemoteDisplayInterface = NULL;
        mRemoteDisplayConnected = FALSE;
        mHandler->RemoveCallbacks(mRtspTimeout);

        SetRemoteSubmixOn(FALSE);
        UnadvertiseDisplay();

        // continue to next step
    }

    // Step 2. Before we try to connect to a new device, disconnect from the old one.
    if (mDisconnectingDevice != NULL) {
        return; // wait for asynchronous callback
    }

    if (mConnectedDevice != NULL && mConnectedDevice != mDesiredDevice) {
        String deviceName;
        mConnectedDevice->GetDeviceName(&deviceName);
        Slogger::I(TAG, "Disconnecting from Wifi display: %s", deviceName.string());

        mDisconnectingDevice = mConnectedDevice;
        mConnectedDevice = NULL;

        UnadvertiseDisplay();

        if (mWifiP2pManager) {
            AutoPtr<DisconnectingDeviceActionListener> l = new DisconnectingDeviceActionListener(this, mDisconnectingDevice);
            mWifiP2pManager->RemoveGroup(mWifiP2pChannel, l);
        }
        return; // wait for asynchronous callback
    }

    // Step 3. Before we try to connect to a new device, stop trying to connect
    // to the old one.
    if (mCancelingDevice != NULL) {
        return; // wait for asynchronous callback
    }

    if (mConnectingDevice != NULL && mConnectingDevice != mDesiredDevice) {
        String deviceName;
        mConnectingDevice->GetDeviceName(&deviceName);
        Slogger::I(TAG, "Canceling connection to Wifi display: %s", deviceName.string());

        mCancelingDevice = mConnectingDevice;
        mConnectingDevice = NULL;

        UnadvertiseDisplay();
        mHandler->RemoveCallbacks(mConnectionTimeout);

        AutoPtr<CancelingDeviceActionListener> l = new CancelingDeviceActionListener(this, mCancelingDevice);
        if (mWifiP2pManager)
            mWifiP2pManager->CancelConnect(mWifiP2pChannel, l);
        return; // wait for asynchronous callback
    }

    // Step 4. If we wanted to disconnect, then mission accomplished.
    if (mDesiredDevice == NULL) {
        UnadvertiseDisplay();
        return; // done
    }

    // Step 5. Try to connect.
    if (mConnectedDevice == NULL && mConnectingDevice == NULL) {
        String deviceName;
        mDesiredDevice->GetDeviceName(&deviceName);
        Slogger::I(TAG, "Connecting to Wifi display: %s", deviceName.string());

        mConnectingDevice = mDesiredDevice;
        AutoPtr<IWifiP2pConfig> config;
        CWifiP2pConfig::New((IWifiP2pConfig**)&config);

        AutoPtr<IWpsInfo> wps;
        CWpsInfo::New((IWpsInfo**)&wps);
        Boolean bval;
        if (mConnectingDevice->WpsPbcSupported(&bval), bval) {
            wps->SetSetup(IWpsInfo::PBC);
        }
        else if (mConnectingDevice->WpsDisplaySupported(&bval), bval) {
            // We do keypad if peer does display
            wps->SetSetup(IWpsInfo::KEYPAD);
        }
        else {
            wps->SetSetup(IWpsInfo::DISPLAY);
        }
        config->SetWps(wps);

        String deviceAddress;
        mConnectingDevice->GetDeviceAddress(&deviceAddress);
        config->SetDeviceAddress(deviceAddress);
        // Helps with STA & P2P concurrency
        config->SetGroupOwnerIntent(IWifiP2pConfig::MIN_GROUP_OWNER_INTENT);

        AutoPtr<IWifiDisplay> display = CreateWifiDisplay(mConnectingDevice);
        AdvertiseDisplay(display, NULL, 0, 0, 0);

        AutoPtr<ConnectDeviceActionListener> l = new ConnectDeviceActionListener(this, mDesiredDevice);
        if (mWifiP2pManager) {
            mWifiP2pManager->Connect(mWifiP2pChannel, config, l);
        }
        return; // wait for asynchronous callback
    }

    // Step 6. Listen for incoming connections.
    if (mConnectedDevice != NULL && mRemoteDisplay == NULL) {
        String deviceName;
        mConnectedDevice->GetDeviceName(&deviceName);
        AutoPtr<IInet4Address> addr = GetInterfaceAddress(mConnectedDeviceGroupInfo);
        if (addr == NULL) {
            Slogger::I(TAG, "Failed to get local interface address"
                " for communicating with Wifi display: %s", deviceName.string());
            HandleConnectionFailure(FALSE);
            return; // done
        }

        SetRemoteSubmixOn(TRUE);

        AutoPtr<IWifiP2pDevice> oldDevice = mConnectedDevice;
        Int32 port = GetPortNumber(mConnectedDevice);
        String iface;
        addr->GetHostAddress(&iface);
        iface += ":";
        iface += StringUtils::ToString(port);
        mRemoteDisplayInterface = iface;

        Slogger::I(TAG, "Listening for RTSP connection on %s"
            " from Wifi display: %s", iface.string(), deviceName.string());

        AutoPtr<IRemoteDisplayHelper> remoteDisplayHelper;
        CRemoteDisplayHelper::AcquireSingleton((IRemoteDisplayHelper**)&remoteDisplayHelper);
        AutoPtr<IRemoteDisplayListener> myRD = new MyRemoteDisplayListener(this, oldDevice);
        mRemoteDisplay = NULL;
        remoteDisplayHelper->Listen(iface, myRD, mHandler, (IRemoteDisplay**)&mRemoteDisplay);

        Boolean result;
        mHandler->PostDelayed(mRtspTimeout, RTSP_TIMEOUT_SECONDS * 1000, &result);
    }
}

void WifiDisplayController::SetRemoteSubmixOn(
    /* [in] */ Boolean on)
{
    if (mRemoteSubmixOn != on) {
        mRemoteSubmixOn = on;
        mAudioManager->SetRemoteSubmixOn(on, REMOTE_SUBMIX_ADDRESS);
    }
}

void WifiDisplayController::HandleStateChanged(
    /* [in] */ Boolean enabled)
{
    mWifiP2pEnabled = enabled;
    UpdateWfdEnableState();
}

void WifiDisplayController::HandlePeersChanged()
{
    // Even if wfd is disabled, it is best to get the latest set of peers to
    // keep in sync with the p2p framework
    RequestPeers();
}

void WifiDisplayController::HandleConnectionChanged(
    /* [in] */ INetworkInfo* networkInfo)
{
    mNetworkInfo = networkInfo;
    Boolean isConnected;
    if (mWfdEnabled && (networkInfo->IsConnected(&isConnected), isConnected)) {
        if (DEBUG) {
            Slogger::D(TAG, "HandleConnectionChanged: connected!");
        }

        if (mDesiredDevice != NULL) {
            AutoPtr<ConnectionChangedGroupInfoListener> l = new ConnectionChangedGroupInfoListener(this);
            if (mWifiP2pManager) {
                mWifiP2pManager->RequestGroupInfo(mWifiP2pChannel, l);
            }
        }
    }
    else {
        if (DEBUG) {
            Slogger::D(TAG, "HandleConnectionChanged: Wfd not enabled or not connected!");
        }

        Disconnect();

        // After disconnection for a group, for some reason we have a tendency
        // to get a peer change notification with an empty list of peers.
        // Perform a fresh scan.
        if (mWfdEnabled) {
            RequestPeers();
        }
    }
}

void WifiDisplayController::HandleConnectionFailure(
    /* [in] */ Boolean timeoutOccurred)
{
    Slogger::I(TAG, "Wifi display connection failed!");

    if (mDesiredDevice != NULL) {
        if (mConnectionRetriesLeft > 0) {
            AutoPtr<IWifiP2pDevice> oldDevice = mDesiredDevice;
            AutoPtr<IRunnable> runnable = new ConnectionFailureRunnable(oldDevice, this);
            Boolean result;
            mHandler->PostDelayed(runnable, timeoutOccurred ? 0 : CONNECT_RETRY_DELAY_MILLIS, &result);
        }
        else {
            Disconnect();
        }
    }
}

ECode WifiDisplayController::AdvertiseDisplayRunnable::Run()
{
    Boolean result;
    AutoPtr<ISurface> surface = mHost->mAdvertisedDisplaySurface;
    AutoPtr<IWifiDisplay> display = mHost->mAdvertisedDisplay;

    if (mOldSurface != NULL && surface != mOldSurface) {
        mHost->mListener->OnDisplayDisconnected();
    }
    else if (mOldDisplay != NULL
        && (mOldDisplay->HasSameAddress(display, &result), !result)) {
        mHost->mListener->OnDisplayConnectionFailed();
    }

    if (display != NULL) {
        if (display->HasSameAddress(mOldDisplay, &result), !result) {
            mHost->mListener->OnDisplayConnecting(display);
        }
        else if (display->Equals(mOldDisplay, &result), !result) {
            // The address is the same but some other property such as the
            // name must have changed.
            mHost->mListener->OnDisplayChanged(display);
        }
        if (surface != NULL && surface != mOldSurface) {
            mHost->mListener->OnDisplayConnected(
                display, surface,
                mHost->mAdvertisedDisplayWidth,
                mHost->mAdvertisedDisplayHeight,
                mHost->mAdvertisedDisplayFlags);
        }
    }
    return NOERROR;
}

void WifiDisplayController::AdvertiseDisplay(
    /* [in] */ IWifiDisplay* display,
    /* [in] */ ISurface* surface,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 flags)
{
    Boolean isEquals = (display == mAdvertisedDisplay.Get());
    if (!isEquals && display != NULL) {
        display->Equals(mAdvertisedDisplay, &isEquals);
    }

    if (!isEquals
        || mAdvertisedDisplaySurface.Get() != surface
        || mAdvertisedDisplayWidth != width
        || mAdvertisedDisplayHeight != height
        || mAdvertisedDisplayFlags != flags) {
        AutoPtr<IWifiDisplay> oldDisplay = mAdvertisedDisplay;
        AutoPtr<ISurface> oldSurface = mAdvertisedDisplaySurface;

        mAdvertisedDisplay = display;
        mAdvertisedDisplaySurface = surface;
        mAdvertisedDisplayWidth = width;
        mAdvertisedDisplayHeight = height;
        mAdvertisedDisplayFlags = flags;

        AutoPtr<IRunnable> runnable = new AdvertiseDisplayRunnable(oldDisplay, oldSurface, this);
        Boolean result;
        mHandler->Post(runnable, &result);
    }
}

void WifiDisplayController::UnadvertiseDisplay()
{
    AdvertiseDisplay(NULL, NULL, 0, 0, 0);
}

void WifiDisplayController::ReadvertiseDisplay(
    /* [in] */ IWifiDisplay* display)
{
    AdvertiseDisplay(display, mAdvertisedDisplaySurface,
        mAdvertisedDisplayWidth, mAdvertisedDisplayHeight,
        mAdvertisedDisplayFlags);
}

AutoPtr<IInet4Address> WifiDisplayController::GetInterfaceAddress(
    /* [in] */ IWifiP2pGroup* info)
{
    AutoPtr<INetworkInterfaceHelper> nwiHelper;
    CNetworkInterfaceHelper::AcquireSingleton((INetworkInterfaceHelper**)&nwiHelper);
    String interfaceName;
    info->GetInterface(&interfaceName);
    AutoPtr<INetworkInterface> iface;
    if (FAILED(nwiHelper->GetByName(interfaceName, (INetworkInterface**)&iface))) {
        Slogger::W(TAG, "Could not obtain address of network interface %s",
            interfaceName.string());
        return NULL;
    }

    AutoPtr<IEnumeration> addrs;
    iface->GetInetAddresses((IEnumeration**)&addrs);
    Boolean hasNext = FALSE;
    while (addrs->HasMoreElements(&hasNext), hasNext) {
        AutoPtr<IInterface> addr;
        addrs->NextElement((IInterface**)&addr);
        if (IInet4Address::Probe(addr)) {
            return IInet4Address::Probe(addr);
        }
    }

    Slogger::W(TAG, "Could not obtain address of network interface %s",
        " because it had no IPv4 addresses.", interfaceName.string());
    return NULL;
}

Int32 WifiDisplayController::GetPortNumber(
    /* [in] */ IWifiP2pDevice* device)
{
    String deviceName;
    device->GetDeviceName(&deviceName);
    if (deviceName.StartWith("DIRECT-")
        && deviceName.EndWith("Broadcom")) {
        // These dongles ignore the port we broadcast in our WFD IE.
        return 8554;
    }
    return DEFAULT_CONTROL_PORT;
}

Boolean WifiDisplayController::IsWifiDisplay(
    /* [in] */ IWifiP2pDevice* device)
{
    AutoPtr<IWifiP2pWfdInfo> wfdInfo;
    device->GetWfdInfo((IWifiP2pWfdInfo**)&wfdInfo);
    Boolean isWfdEnabled;
    Int32 type;
    return wfdInfo != NULL
        && (wfdInfo->IsWfdEnabled(&isWfdEnabled), isWfdEnabled)
        && IsPrimarySinkDeviceType((wfdInfo->GetDeviceType(&type), type));
}

Boolean WifiDisplayController::IsPrimarySinkDeviceType(
    /* [in] */ Int32 deviceType)
{
    return deviceType == IWifiP2pWfdInfo::PRIMARY_SINK
        || deviceType == IWifiP2pWfdInfo::SOURCE_OR_PRIMARY_SINK;
}

String WifiDisplayController::DescribeWifiP2pDevice(
    /* [in] */ IWifiP2pDevice* device)
{
    if (device == NULL) {
        return String("NULL");
    }
    else {
        String str;
        device->ToString(&str);
        return str.Replace('\n', ',');
    }
}

String WifiDisplayController::DescribeWifiP2pGroup(
    /* [in] */ IWifiP2pGroup* group)
{
    String str;
    return group != NULL ? (group->ToString(&str), str).Replace('\n', ',') : String("NULL");
}

AutoPtr<IWifiDisplay> WifiDisplayController::CreateWifiDisplay(
    /* [in] */ IWifiP2pDevice* device)
{
    String deviceAddress;
    device->GetDeviceAddress(&deviceAddress);
    String deviceName, nullStr;
    device->GetDeviceName(&deviceName);
    AutoPtr<IWifiDisplay> wifiDisplay;
    CWifiDisplay::New(deviceAddress, deviceName, nullStr, (IWifiDisplay**)&wifiDisplay);

    return wifiDisplay;
}


} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos
