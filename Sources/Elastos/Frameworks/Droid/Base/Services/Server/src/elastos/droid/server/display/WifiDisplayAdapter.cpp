
#include "elastos/droid/server/display/WifiDisplayAdapter.h"
#include "elastos/droid/server/display/DisplayDeviceInfo.h"
#include "elastos/droid/server/display/PersistentDataStore.h"
#include "elastos/droid/server/display/WifiDisplayController.h"
#include "elastos/droid/R.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::R;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Hardware::Display::CWifiDisplay;
using Elastos::Droid::Hardware::Display::CWifiDisplayStatus;
using Elastos::Droid::Hardware::Display::IDisplayManager;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::ISurfaceHelper;
using Elastos::Droid::View::CSurfaceHelper;
using Elastos::Droid::Media::IRemoteDisplay;
using Elastos::Droid::Provider::ISettings;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

const String WifiDisplayAdapter::TAG("WifiDisplayAdapter");
const Boolean WifiDisplayAdapter::DEBUG = FALSE;
const Int32 WifiDisplayAdapter::MSG_SEND_STATUS_CHANGE_BROADCAST;
const Int32 WifiDisplayAdapter::MSG_UPDATE_NOTIFICATION;
const String WifiDisplayAdapter::ACTION_DISCONNECT("android.server.display.wfd.DISCONNECT");

//==============================================================================
//                  WifiDisplayAdapter::MyBroadcastReceiver
//==============================================================================
WifiDisplayAdapter::MyBroadcastReceiver::MyBroadcastReceiver(
   /* [in] */ WifiDisplayAdapter* owner)
   : mHost(owner)
{}

ECode WifiDisplayAdapter::MyBroadcastReceiver::OnReceive(
   /* [in] */ IContext* context,
   /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);

    if (action.Equals(WifiDisplayAdapter::ACTION_DISCONNECT)) {
        if (WifiDisplayAdapter::DEBUG) {
            Slogger::D(WifiDisplayAdapter::TAG, "OnReceive: %s", action.string());
        }

        AutoLock lock(mHost->GetSyncRoot());
        mHost->RequestDisconnectLocked();
    }

    return NOERROR;
}

//==============================================================================
//                  WifiDisplayAdapter::MyWifiDisplayControllerListener
//==============================================================================
CAR_INTERFACE_IMPL(WifiDisplayAdapter::MyWifiDisplayControllerListener, Object, IWifiDisplayControllerListener)

WifiDisplayAdapter::MyWifiDisplayControllerListener::MyWifiDisplayControllerListener(
    /* [in] */ WifiDisplayAdapter* owner)
    : mHost(owner)
{}

ECode WifiDisplayAdapter::MyWifiDisplayControllerListener::OnFeatureStateChanged(
    /* [in] */ Int32 featureState)
{
    AutoLock lock(mHost->GetSyncRoot());
    if (mHost->mFeatureState != featureState) {
        mHost->mFeatureState = featureState;
        mHost->ScheduleStatusChangedBroadcastLocked();
    }
    return NOERROR;
}

ECode WifiDisplayAdapter::MyWifiDisplayControllerListener::OnScanStarted()
{
    if (WifiDisplayAdapter::DEBUG) {
        Slogger::D(WifiDisplayAdapter::TAG, "OnScanStarted");
    }

    AutoLock lock(mHost->GetSyncRoot());
    if (mHost->mScanState != IWifiDisplayStatus::SCAN_STATE_SCANNING) {
        mHost->mScanState = IWifiDisplayStatus::SCAN_STATE_SCANNING;
        mHost->ScheduleStatusChangedBroadcastLocked();
    }
    return NOERROR;
}

Boolean WifiDisplayAdapter::MyWifiDisplayControllerListener::Equals(
   /* [in] */ ArrayOf<IWifiDisplay*>* array1,
   /* [in] */ ArrayOf<IWifiDisplay*>* array2)
{
    if (array1 == array2) {
        return TRUE;
    }

    if (array1 == NULL || array2 == NULL ||
        array1->GetLength() != array2->GetLength()) {
        return FALSE;
    }

    for (Int32 i = 0; i < array1->GetLength(); i++) {
        Boolean res;
        (*array1)[i]->Equals((*array2)[i], &res);
        if (!res) {
            return FALSE;
        }
    }
    return TRUE;
}

ECode WifiDisplayAdapter::MyWifiDisplayControllerListener::OnScanFinished(
    /* [in] */ ArrayOf<IWifiDisplay*>* availableDisplays)
{
    if (WifiDisplayAdapter::DEBUG) {
        Slogger::D(WifiDisplayAdapter::TAG, "OnScanFinished");
    }

    AutoLock lock(mHost->GetSyncRoot());
    AutoPtr<ArrayOf<IWifiDisplay*> > displays =
       mHost->mPersistentDataStore->ApplyWifiDisplayAliases(availableDisplays);

    if (mHost->mScanState != IWifiDisplayStatus::SCAN_STATE_NOT_SCANNING
        || !Equals(mHost->mAvailableDisplays, displays)) {
        mHost->mScanState = IWifiDisplayStatus::SCAN_STATE_NOT_SCANNING;
        mHost->mAvailableDisplays = displays;
        mHost->FixRememberedDisplayNamesFromAvailableDisplaysLocked();
        mHost->ScheduleStatusChangedBroadcastLocked();
    }
   return NOERROR;
}

ECode WifiDisplayAdapter::MyWifiDisplayControllerListener::OnDisplayConnecting(
    /* [in] */ IWifiDisplay* display)
{
    if (WifiDisplayAdapter::DEBUG) {
        String info;
        display->ToString(&info);
        Slogger::D(WifiDisplayAdapter::TAG, "OnDisplayConnecting: %s", info.string());
    }

    AutoLock lock(mHost->GetSyncRoot());
    AutoPtr<IWifiDisplay> newDisplay =
       mHost->mPersistentDataStore->ApplyWifiDisplayAlias(display);

    Boolean res;
    if (mHost->mActiveDisplayState != IWifiDisplayStatus::DISPLAY_STATE_CONNECTING
       || mHost->mActiveDisplay == NULL
       || !(mHost->mActiveDisplay->Equals(newDisplay, &res), res)) {
        mHost->mActiveDisplayState = IWifiDisplayStatus::DISPLAY_STATE_CONNECTING;
        mHost->mActiveDisplay = newDisplay;
        mHost->ScheduleStatusChangedBroadcastLocked();
    }
    return NOERROR;
}

ECode WifiDisplayAdapter::MyWifiDisplayControllerListener::OnDisplayConnectionFailed()
{
    if (WifiDisplayAdapter::DEBUG) {
        Slogger::D(WifiDisplayAdapter::TAG, "OnDisplayConnectionFailed");
    }

    AutoLock lock(mHost->GetSyncRoot());
    if (mHost->mActiveDisplayState != IWifiDisplayStatus::DISPLAY_STATE_NOT_CONNECTED
        || mHost->mActiveDisplay != NULL) {
        mHost->mActiveDisplayState = IWifiDisplayStatus::DISPLAY_STATE_NOT_CONNECTED;
        mHost->mActiveDisplay = NULL;
        mHost->ScheduleStatusChangedBroadcastLocked();
    }
    return NOERROR;
}

ECode WifiDisplayAdapter::MyWifiDisplayControllerListener::OnDisplayConnected(
    /* [in] */ IWifiDisplay* display,
    /* [in] */ ISurface* surface,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 flags)
{
    if (WifiDisplayAdapter::DEBUG) {
        String info;
        display->ToString(&info);
        Slogger::D(WifiDisplayAdapter::TAG, "OnDisplayConnected: %s, w:h - %d:%d",
            info.string(), width, height);
    }

    AutoLock lock(mHost->GetSyncRoot());
    AutoPtr<IWifiDisplay> newDisplay =
        mHost->mPersistentDataStore->ApplyWifiDisplayAlias(display);
    mHost->AddDisplayDeviceLocked(newDisplay, surface, width, height, flags);

    Boolean res;
    if (mHost->mActiveDisplayState != IWifiDisplayStatus::DISPLAY_STATE_CONNECTED
        || mHost->mActiveDisplay == NULL
        || !(mHost->mActiveDisplay->Equals(newDisplay, &res), res)) {
        mHost->mActiveDisplayState = IWifiDisplayStatus::DISPLAY_STATE_CONNECTED;
        mHost->mActiveDisplay = newDisplay;
        mHost->ScheduleStatusChangedBroadcastLocked();
    }
    return NOERROR;
}

ECode WifiDisplayAdapter::MyWifiDisplayControllerListener::OnDisplayChanged(
    /* [in] */ IWifiDisplay* display)
{
    if (WifiDisplayAdapter::DEBUG) {
        String info;
        display->ToString(&info);
        Slogger::D(WifiDisplayAdapter::TAG, "OnDisplayChanged: %s", info.string());
    }

    AutoLock lock(mHost->GetSyncRoot());
    AutoPtr<IWifiDisplay> newDisplay =
        mHost->mPersistentDataStore->ApplyWifiDisplayAlias(display);
    Boolean hasSameAddr, equal;
    if (mHost->mActiveDisplay != NULL
        && (mHost->mActiveDisplay->HasSameAddress(newDisplay, &hasSameAddr), hasSameAddr)
        && (mHost->mActiveDisplay->Equals(newDisplay, &equal), !equal)) {
        mHost->mActiveDisplay = newDisplay;
        String name;
        newDisplay->GetFriendlyDisplayName(&name);
        mHost->RenameDisplayDeviceLocked(name);
        mHost->ScheduleStatusChangedBroadcastLocked();
    }

    return NOERROR;
}

ECode WifiDisplayAdapter::MyWifiDisplayControllerListener::OnDisplayDisconnected()
{
    if (WifiDisplayAdapter::DEBUG) {
        Slogger::D(WifiDisplayAdapter::TAG, "OnDisplayDisconnected");
    }

    // Stop listening.
    AutoLock lock(mHost->GetSyncRoot());
    mHost->RemoveDisplayDeviceLocked();

    if (mHost->mActiveDisplayState != IWifiDisplayStatus::DISPLAY_STATE_NOT_CONNECTED
        || mHost->mActiveDisplay != NULL) {
        mHost->mActiveDisplayState = IWifiDisplayStatus::DISPLAY_STATE_NOT_CONNECTED;
        mHost->mActiveDisplay = NULL;
        mHost->ScheduleStatusChangedBroadcastLocked();
    }
    return NOERROR;
}

//==============================================================================
//                  WifiDisplayAdapter::WifiDisplayDevice
//==============================================================================
WifiDisplayAdapter::WifiDisplayDevice::WifiDisplayDevice(
    /* [in] */ WifiDisplayAdapter* owner,
    /* [in] */ IBinder* displayToken,
    /* [in] */ const String& name,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Float refreshRate,
    /* [in] */ Int32 flags,
    /* [in] */ const String& address,
    /* [in] */ ISurface* surface)
    : DisplayDevice(owner, displayToken)
    , mName(name)
    , mWidth(width)
    , mHeight(height)
    , mRefreshRate(refreshRate)
    , mFlags(flags)
    , mAddress(address)
    , mSurface(surface)
    , mHost(owner)
{}

void WifiDisplayAdapter::WifiDisplayDevice::ClearSurfaceLocked()
{
    mSurface = NULL;
    mHost->SendTraversalRequestLocked();
}

void WifiDisplayAdapter::WifiDisplayDevice::SetNameLocked(
    /* [in] */ const String& name)
{
    mName = name;
    mInfo = NULL;
}

void WifiDisplayAdapter::WifiDisplayDevice::PerformTraversalInTransactionLocked()
{
    SetSurfaceInTransactionLocked(mSurface);
}

AutoPtr<DisplayDeviceInfo> WifiDisplayAdapter::WifiDisplayDevice::GetDisplayDeviceInfoLocked()
{
    if (mInfo == NULL) {
        mInfo = new DisplayDeviceInfo();
        mInfo->mName = mName;
        mInfo->mWidth = mWidth;
        mInfo->mHeight = mHeight;
        mInfo->mRefreshRate = mRefreshRate;
        mInfo->mFlags = mFlags;
        mInfo->mType = IDisplay::TYPE_WIFI;
        mInfo->mAddress = mAddress;
        mInfo->mTouch = DisplayDeviceInfo::TOUCH_EXTERNAL;
        mInfo->SetAssumedDensityForExternalDisplay(mWidth, mHeight);
    }
    return mInfo;
}

//==============================================================================
//                  WifiDisplayAdapter::WifiDisplayHandler
//==============================================================================

WifiDisplayAdapter::WifiDisplayHandler::WifiDisplayHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ WifiDisplayAdapter* host)
    : Handler(looper, NULL, TRUE /*async*/)
    , mHost(host)
{
}

ECode WifiDisplayAdapter::WifiDisplayHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
    case MSG_SEND_STATUS_CHANGE_BROADCAST:
        if (WifiDisplayAdapter::DEBUG) {
            Slogger::D(WifiDisplayAdapter::TAG, "HandleMessage: MSG_SEND_STATUS_CHANGE_BROADCAST");
        }
        mHost->HandleSendStatusChangeBroadcast();
        break;

    case MSG_UPDATE_NOTIFICATION:
        if (WifiDisplayAdapter::DEBUG) {
            Slogger::D(WifiDisplayAdapter::TAG, "HandleMessage: MSG_UPDATE_NOTIFICATION");
        }
        mHost->HandleUpdateNotification();
        break;
    }
    return NOERROR;
}

//==============================================================================
//                  WifiDisplayAdapter::RegisterRunnable
//==============================================================================
WifiDisplayAdapter::RegisterRunnable::RegisterRunnable(
    /* [in] */ WifiDisplayAdapter* host)
    : mHost(host)
{
}

ECode WifiDisplayAdapter::RegisterRunnable::Run()
{
    AutoPtr<IContext> context = mHost->GetContext();
    mHost->mDisplayController = new WifiDisplayController(
        context, mHost->GetHandler(), mHost->mWifiDisplayListener);

    AutoPtr<IUserHandle> all;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetALL((IUserHandle**)&all);

    String nullStr;
    AutoPtr<IIntent> stickyIntent;
    AutoPtr<IIntentFilter> intentFilter;
    CIntentFilter::New(ACTION_DISCONNECT, (IIntentFilter**)&intentFilter);
    context->RegisterReceiverAsUser(mHost->mBroadcastReceiver, all,
        intentFilter, nullStr, mHost->mHandler, (IIntent**)&stickyIntent);
    return NOERROR;
}

//==============================================================================
//                  WifiDisplayAdapter::RequestScanRunnable
//==============================================================================
WifiDisplayAdapter::RequestScanRunnable::RequestScanRunnable(
    /* [in] */ WifiDisplayAdapter* host)
    : mHost(host)
{
}

ECode WifiDisplayAdapter::RequestScanRunnable::Run()
{
    if (mHost->mDisplayController != NULL) {
        mHost->mDisplayController->RequestScan();
    }
    return NOERROR;
}

//==============================================================================
//                  WifiDisplayAdapter::RequestConnectRunnable
//==============================================================================
WifiDisplayAdapter::RequestConnectRunnable::RequestConnectRunnable(
    /* [in] */ const String& address,
    /* [in] */ WifiDisplayAdapter* host)
    : mAddress(address)
    , mHost(host)
{
}

ECode WifiDisplayAdapter::RequestConnectRunnable::Run()
{
    if (mHost->mDisplayController != NULL) {
        mHost->mDisplayController->RequestConnect(mAddress);
    }
    return NOERROR;
}

//==============================================================================
//                  WifiDisplayAdapter::RequestDisconnectRunnable
//==============================================================================
WifiDisplayAdapter::RequestDisconnectRunnable::RequestDisconnectRunnable(
    /* [in] */ WifiDisplayAdapter* host)
    : mHost(host)
{
}

ECode WifiDisplayAdapter::RequestDisconnectRunnable::Run()
{
    if (mHost->mDisplayController != NULL) {
        mHost->mDisplayController->RequestDisconnect();
    }
    return NOERROR;
}

//==============================================================================
//                  WifiDisplayAdapter
//==============================================================================
// Called with SyncRoot lock held.
WifiDisplayAdapter::WifiDisplayAdapter(
    /* [in] */ Object* syncRoot,
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler,
    /* [in] */ IDisplayAdapterListener* listener,
    /* [in] */ PersistentDataStore* persistentDataStore)
    : DisplayAdapter(syncRoot, context, handler, listener, TAG)
    , mPersistentDataStore(persistentDataStore)
    , mFeatureState(0)
    , mScanState(0)
    , mActiveDisplayState(0)
    , mPendingStatusChangeBroadcast(FALSE)
    , mPendingNotificationUpdate(FALSE)
{
    mAvailableDisplays = ArrayOf<IWifiDisplay*>::Alloc(0);//WifiDisplay::EMPTY_ARRAY
    mRememberedDisplays = ArrayOf<IWifiDisplay*>::Alloc(0);//WifiDisplay::EMPTY_ARRAY

    mWifiDisplayListener = new MyWifiDisplayControllerListener(this);
    mBroadcastReceiver = new MyBroadcastReceiver(this);

    assert(handler != NULL);
    AutoPtr<ILooper> looper;
    handler->GetLooper((ILooper**)&looper);
    mHandler = new WifiDisplayHandler(looper, this);

    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    assert(resources != NULL);
    resources->GetBoolean(
        R::bool_::config_wifiDisplaySupportsProtectedBuffers, &mSupportsProtectedBuffers);
    AutoPtr<IInterface> service;
    context->GetSystemService(IContext::NOTIFICATION_SERVICE, (IInterface**)&service);
    mNotificationManager = INotificationManager::Probe(service);
}

void WifiDisplayAdapter::DumpLocked(
    /* [in] */ IPrintWriter* pw)
{
    DisplayAdapter::DumpLocked(pw);

    // pw->PrintStringln(String("mCurrentStatus=") + getWifiDisplayStatusLocked());
    // pw->PrintStringln(String("mFeatureState=") + mFeatureState);
    // pw->PrintStringln(String("mScanState=") + mScanState);
    // pw->PrintStringln(String("mActiveDisplayState=") + mActiveDisplayState);
    // pw->PrintStringln(String("mActiveDisplay=") + mActiveDisplay);
    // pw->PrintStringln(String("mAvailableDisplays=") + Arrays.toString(mAvailableDisplays));
    // pw->PrintStringln(String("mRememberedDisplays=") + Arrays.toString(mRememberedDisplays));
    // pw->PrintStringln(String("mPendingStatusChangeBroadcast=") + mPendingStatusChangeBroadcast);
    // pw->PrintStringln(String("mPendingNotificationUpdate=") + mPendingNotificationUpdate);
    // pw->PrintStringln(String("mSupportsProtectedBuffers=") + mSupportsProtectedBuffers);

    // // Try to dump the controller state.
    // if (mDisplayController == NULL) {
    //     pw->PrintStringln("mDisplayController=NULL");
    // } else {
    //     pw->PrintStringln("mDisplayController:");
    //     final IndentingPrintWriter ipw = new IndentingPrintWriter(pw, "  ");
    //     ipw->increaseIndent();
    //     DumpUtils.dumpAsync(getHandler(), mDisplayController, ipw, 200);
    // }
    assert(0);
}

void WifiDisplayAdapter::RegisterLocked()
{
    if (DEBUG) {
        Slogger::D(TAG, "RegisterLocked");
    }

    DisplayAdapter::RegisterLocked();

    UpdateRememberedDisplaysLocked();

    AutoPtr<IRunnable> runnable = new RegisterRunnable(this);
    Boolean result;
    GetHandler()->Post(runnable, &result);
}

void WifiDisplayAdapter::RequestScanLocked()
{
    if (DEBUG) {
        Slogger::D(TAG, "requestScanLocked");
    }

    AutoPtr<IRunnable> runnable = new RequestScanRunnable(this);
    Boolean result;
    GetHandler()->Post(runnable, &result);
}

void WifiDisplayAdapter::RequestConnectLocked(
    /* [in] */ const String& address,
    /* [in] */ Boolean trusted)
{
    if (DEBUG) {
       Slogger::D(TAG, "requestConnectLocked: address=%s, trusted=%d", address.string(), trusted);
    }

    if (!trusted) {
        AutoLock lock(GetSyncRoot());

        if (!IsRememberedDisplayLocked(address)) {
           Slogger::W(TAG, "Ignoring request by an untrusted client "
               "to connect to an unknown wifi display: %s", address.string());
            return;
        }
    }

    AutoPtr<IRunnable> runnable = new RequestConnectRunnable(address, this);
    Boolean result;
    GetHandler()->Post(runnable, &result);
}

Boolean WifiDisplayAdapter::IsRememberedDisplayLocked(
    /* [in] */ const String& address)
{
    String address2;
    Int32 length = mRememberedDisplays->GetLength();
    for (Int32 i = 0; i < length; i++) {
        (*mRememberedDisplays)[i]->GetDeviceAddress(&address2);
        if (address2.Equals(address)) {
            return TRUE;
        }
    }
    return FALSE;
}

void WifiDisplayAdapter::RequestDisconnectLocked()
{
    if (DEBUG) {
        Slogger::D(TAG, "RequestDisconnectLocked");
    }

    AutoPtr<IRunnable> runnable = new RequestDisconnectRunnable(this);
    Boolean result;
    GetHandler()->Post(runnable, &result);
}

void WifiDisplayAdapter::RequestRenameLocked(
    /* [in] */ const String& address,
    /* [in] */ const String& alias_)
{
    String alias(alias_);
    if (DEBUG) {
        Slogger::D(TAG, "RequestRenameLocked: address=%s, alias=%s",
            address.string(), alias.string());
    }

    if (!alias.IsNull()) {
        alias = alias.Trim();
        if (alias.IsEmpty() || alias.Equals(address)) {
            alias = NULL;
        }
    }

    AutoPtr<IWifiDisplay> display = mPersistentDataStore->GetRememberedWifiDisplay(address);
    if (display != NULL) {
        String deviceAlias;
        display->GetDeviceAlias(&deviceAlias);
        if (deviceAlias == alias) {
            String deviceName;
            display->GetDeviceName(&deviceName);
            display = NULL;
            CWifiDisplay::New(address, deviceName, alias, (IWifiDisplay**)&display);
            if (mPersistentDataStore->RememberWifiDisplay(display)) {
                mPersistentDataStore->SaveIfNeeded();
                UpdateRememberedDisplaysLocked();
                ScheduleStatusChangedBroadcastLocked();
            }
        }
    }

    String address2;
    if (mActiveDisplay != NULL && (mActiveDisplay->GetDeviceAddress(&address2),
        address2.Equals(address)) && mDisplayDevice != NULL) {
        String name;
        mActiveDisplay->GetFriendlyDisplayName(&name);
        RenameDisplayDeviceLocked(name);
    }
}

void WifiDisplayAdapter::RequestForgetLocked(
    /* [in] */ const String& address)
{
    if (DEBUG) {
        Slogger::D(TAG, "RequestForgetLocked: address=%s", address.string());
    }

    if (mPersistentDataStore->ForgetWifiDisplay(address)) {
        mPersistentDataStore->SaveIfNeeded();
        UpdateRememberedDisplaysLocked();
        ScheduleStatusChangedBroadcastLocked();
    }

    String address2;
    if (mActiveDisplay != NULL && (mActiveDisplay->GetDeviceAddress(&address2),
        address2.Equals(address))) {
        RequestDisconnectLocked();
    }
}

AutoPtr<IWifiDisplayStatus> WifiDisplayAdapter::GetWifiDisplayStatusLocked()
{
    if (mCurrentStatus == NULL) {
        CWifiDisplayStatus::New(
            mFeatureState, mScanState, mActiveDisplayState,
            mActiveDisplay, mAvailableDisplays, mRememberedDisplays,
            (IWifiDisplayStatus**)&mCurrentStatus);
    }

    if (DEBUG) {
        String info;
        mCurrentStatus->ToString(&info);
        Slogger::D(TAG, "GetWifiDisplayStatus: [%s]", info.string());
    }
    return mCurrentStatus;
}

void WifiDisplayAdapter::UpdateRememberedDisplaysLocked()
{
    mRememberedDisplays = mPersistentDataStore->GetRememberedWifiDisplays();
    mActiveDisplay = mPersistentDataStore->ApplyWifiDisplayAlias(mActiveDisplay);
    mAvailableDisplays = mPersistentDataStore->ApplyWifiDisplayAliases(mAvailableDisplays);
}

void WifiDisplayAdapter::FixRememberedDisplayNamesFromAvailableDisplaysLocked()
{
    // It may happen that a display name has changed since it was remembered.
    // Consult the list of available displays and update the name if needed.
    // We don't do anything special for the active display here.  The display
    // controller will send a separate event when it needs to be updates.
    Boolean changed = FALSE;
    Boolean bval;
    String address;
    for (Int32 i = 0; i < mRememberedDisplays->GetLength(); i++) {
        AutoPtr<IWifiDisplay> rememberedDisplay = (*mRememberedDisplays)[i];
        rememberedDisplay->GetDeviceAddress(&address);
        AutoPtr<IWifiDisplay> availableDisplay = FindAvailableDisplayLocked(address);
        if (availableDisplay != NULL && (rememberedDisplay->Equals(availableDisplay, &bval), !bval)) {
            if (DEBUG) {
                Slogger::D(TAG, "FixRememberedDisplayNamesFromAvailableDisplaysLocked: "
                    "updating remembered display to %p", availableDisplay.Get());
            }
            mRememberedDisplays->Set(i, availableDisplay);
            changed |= mPersistentDataStore->RememberWifiDisplay(availableDisplay);
        }
    }
    if (changed) {
        mPersistentDataStore->SaveIfNeeded();
    }
}

AutoPtr<IWifiDisplay> WifiDisplayAdapter::FindAvailableDisplayLocked(
    /* [in] */ const String& address)
{
    String addr;
    for (Int32 i = 0; i < mAvailableDisplays->GetLength(); ++i) {
        AutoPtr<IWifiDisplay> display = (*mAvailableDisplays)[i];
        if (display) {
            display->GetDeviceAddress(&addr);
            if (addr.Equals(address))
                return display;
        }
    }

    return NULL;
}

void WifiDisplayAdapter::AddDisplayDeviceLocked(
    /* [in] */ IWifiDisplay* display,
    /* [in] */ ISurface* surface,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 flags)
{
    Slogger::D(TAG, "===================WifiDisplayAdapter::AddDisplayDeviceLocked===========");

    RemoveDisplayDeviceLocked();

    if (mPersistentDataStore->RememberWifiDisplay(display)) {
        mPersistentDataStore->SaveIfNeeded();
        UpdateRememberedDisplaysLocked();
        ScheduleStatusChangedBroadcastLocked();
    }

    Boolean secure = (flags & IRemoteDisplay::DISPLAY_FLAG_SECURE) != 0;
    Int32 deviceFlags = 0;
    if (secure) {
        deviceFlags |= DisplayDeviceInfo::FLAG_SECURE;
        if (mSupportsProtectedBuffers) {
            deviceFlags |= DisplayDeviceInfo::FLAG_SUPPORTS_PROTECTED_BUFFERS;
        }
    }

    Float refreshRate = 60.0f; // TODO: get this for real

    String name, address;
    display->GetFriendlyDisplayName(&name);
    display->GetDeviceAddress(&address);

    AutoPtr<ISurfaceHelper> helper;
    CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&helper);
    AutoPtr<IBinder> displayToken;
    helper->CreateDisplay(name, secure, (IBinder**)&displayToken);
    mDisplayDevice = new WifiDisplayDevice(this, displayToken, name, width, height,
        refreshRate, deviceFlags, address, surface);

    SendDisplayDeviceEventLocked(mDisplayDevice, DISPLAY_DEVICE_EVENT_ADDED);
    ScheduleUpdateNotificationLocked();
}

void WifiDisplayAdapter::RemoveDisplayDeviceLocked()
{
    if (mDisplayDevice != NULL) {
        mDisplayDevice->ClearSurfaceLocked();
        SendDisplayDeviceEventLocked(mDisplayDevice, DISPLAY_DEVICE_EVENT_REMOVED);
        mDisplayDevice = NULL;

        ScheduleUpdateNotificationLocked();
    }
}

void WifiDisplayAdapter::RenameDisplayDeviceLocked(
    /* [in] */ const String& name)
{
    if (mDisplayDevice != NULL) {
        String displayName = mDisplayDevice->GetNameLocked();
        if (!displayName.Equals(name)) {
            mDisplayDevice->SetNameLocked(name);
            SendDisplayDeviceEventLocked(mDisplayDevice, DISPLAY_DEVICE_EVENT_CHANGED);
        }
    }
}

void WifiDisplayAdapter::ScheduleStatusChangedBroadcastLocked()
{
    mCurrentStatus = NULL;
    if (!mPendingStatusChangeBroadcast) {
        mPendingStatusChangeBroadcast = TRUE;
        Boolean result;
        mHandler->SendEmptyMessage(MSG_SEND_STATUS_CHANGE_BROADCAST, &result);
    }
}

void WifiDisplayAdapter::ScheduleUpdateNotificationLocked()
{
    if (!mPendingNotificationUpdate) {
        mPendingNotificationUpdate = TRUE;
        Boolean result;
        mHandler->SendEmptyMessage(MSG_UPDATE_NOTIFICATION, &result);
    }
}

// Runs on the handler.
void WifiDisplayAdapter::HandleSendStatusChangeBroadcast()
{
    AutoPtr<IIntent> intent;
    {
        AutoLock lock(GetSyncRoot());
        if (!mPendingStatusChangeBroadcast) {
            return;
        }

        mPendingStatusChangeBroadcast = FALSE;
        CIntent::New(
            IDisplayManager::ACTION_WIFI_DISPLAY_STATUS_CHANGED,
            (IIntent**)&intent);
        intent->AddFlags(IIntent::FLAG_RECEIVER_REGISTERED_ONLY);
        AutoPtr<IWifiDisplayStatus> status = GetWifiDisplayStatusLocked();
        intent->PutExtra(IDisplayManager::EXTRA_WIFI_DISPLAY_STATUS,
            status != NULL ? IParcelable::Probe(status) : NULL);
    }

    AutoPtr<IUserHandle> all;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetALL((IUserHandle**)&all);

    // Send protected broadcast about wifi display status to registered receivers.
    GetContext()->SendBroadcastAsUser(intent, all);
}

// Runs on the handler.
void WifiDisplayAdapter::HandleUpdateNotification()
{
    Boolean isConnected;
    {
        AutoLock lock(GetSyncRoot());

        if (!mPendingNotificationUpdate) {
            return;
        }

        mPendingNotificationUpdate = FALSE;
        isConnected = (mDisplayDevice != NULL);
    }

    AutoPtr<IUserHandle> all;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetALL((IUserHandle**)&all);

    // Cancel the old notification if there is one.
    String nullStr;
    mNotificationManager->CancelAsUser(nullStr,
        R::string::wifi_display_notification_title, all);

    if (isConnected) {
        AutoPtr<IContext> context = GetContext();

        AutoPtr<IPendingIntentHelper> piHelper;
        CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&piHelper);

        AutoPtr<IUserHandle> current;
        helper->GetCURRENT((IUserHandle**)&current);

        // Initialize pending intents for the notification outside of the lock because
        // creating a pending intent requires a call into the activity manager.
        if (mSettingsPendingIntent == NULL) {
            AutoPtr<IIntent> settingsIntent;
            CIntent::New(ISettings::ACTION_WIFI_DISPLAY_SETTINGS, (IIntent**)&settingsIntent);
            settingsIntent->SetFlags(IIntent::FLAG_ACTIVITY_NEW_TASK
               | IIntent::FLAG_ACTIVITY_RESET_TASK_IF_NEEDED
               | IIntent::FLAG_ACTIVITY_CLEAR_TOP);
            piHelper->GetActivityAsUser(
               context, 0, settingsIntent, 0, NULL, current,
               (IPendingIntent**)&mSettingsPendingIntent);
        }

        if (mDisconnectPendingIntent == NULL) {
            AutoPtr<IIntent> disconnectIntent;
            CIntent::New(ACTION_DISCONNECT, (IIntent**)&disconnectIntent);
            piHelper->GetBroadcastAsUser(
                context, 0, disconnectIntent, 0, current,
                (IPendingIntent**)&mDisconnectPendingIntent);
        }

        // Post the notification.
        AutoPtr<IResources> r;
        context->GetResources((IResources**)&r);
        AutoPtr<INotification> notification;
        AutoPtr<INotificationBuilder> builder;
        CNotificationBuilder::New(context, (INotificationBuilder**)&builder);
        String str;
        r->GetString(R::string::wifi_display_notification_title, &str);
        AutoPtr<ICharSequence> seq;
        CStringWrapper::New(str, (ICharSequence**)&seq);
        builder->SetContentTitle(seq);
        r->GetString(R::string::wifi_display_notification_message, &str);
        seq = NULL;
        CStringWrapper::New(str, (ICharSequence**)&seq);
        builder->SetContentText(seq);
        builder->SetContentIntent(mSettingsPendingIntent);
        builder->SetSmallIcon(R::drawable::ic_notify_wifidisplay);
        builder->SetOngoing(TRUE);
        r->GetString(R::string::wifi_display_notification_disconnect, &str);
        seq = NULL;
        CStringWrapper::New(str, (ICharSequence**)&seq);
        builder->AddAction(R::drawable::ic_menu_close_clear_cancel,
            seq, mDisconnectPendingIntent);
        builder->Build((INotification**)&notification);
        mNotificationManager->NotifyAsUser(nullStr,
            R::string::wifi_display_notification_title,
            notification, all);
    }
}


} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos
