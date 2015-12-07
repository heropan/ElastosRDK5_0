
#ifndef __ELASTOS_DROID_SERVER_DISPLAY_WIFIDISPLAYCONTROLLER_H__
#define __ELASTOS_DROID_SERVER_DISPLAY_WIFIDISPLAYCONTROLLER_H__

#include "Elastos.Droid.Server_server.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/content/BroadcastReceiver.h"
#include "elastos/droid/database/ContentObserver.h"
#include <elastos/utility/etl/List.h>

using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Media::IAudioManager;
using Elastos::Droid::Media::IRemoteDisplay;
using Elastos::Droid::Media::IRemoteDisplayListener;
using Elastos::Droid::Net::INetworkInfo;
using Elastos::Droid::Wifi::P2p::IWifiP2pManagerChannel;
using Elastos::Droid::Wifi::P2p::IWifiP2pManagerActionListener;
using Elastos::Droid::Wifi::P2p::IWifiP2pManagerPeerListListener;
using Elastos::Droid::Wifi::P2p::IWifiP2pManagerGroupInfoListener;
using Elastos::Droid::Wifi::P2p::IWifiP2pManager;
using Elastos::Droid::Wifi::P2p::IWifiP2pDevice;
using Elastos::Droid::Wifi::P2p::IWifiP2pGroup;
using Elastos::Droid::Wifi::P2p::IWifiP2pDeviceList;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Hardware::Display::IWifiDisplay;
using Elastos::Droid::Database::ContentObserver;
using Elastos::Droid::View::ISurface;

using Elastos::Utility::Etl::List;
using Elastos::Net::IInet4Address;
using Elastos::IO::IPrintWriter;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

/**
 * Manages all of the various asynchronous interactions with the {@link WifiP2pManager}
 * on behalf of {@link WifiDisplayAdapter}.
 * <p>
 * This code is isolated from {@link WifiDisplayAdapter} so that we can avoid
 * accidentally introducing any deadlocks due to the display manager calling
 * outside of itself while holding its lock.  It's also way easier to write this
 * asynchronous code if we can assume that it is single-threaded.
 * </p><p>
 * The controller must be instantiated on the handler thread.
 * </p>
 */
class WifiDisplayController : public Object
{
private:
    class WifiP2pReceiver : public BroadcastReceiver
    {
    public:
        WifiP2pReceiver(
            /* [in] */ WifiDisplayController* host);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("WifiDisplayController::WifiP2pReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        WifiDisplayController* mHost;
    };

    class MyContentObserver : public ContentObserver
    {
    public:
        MyContentObserver(
            /* [in] */ WifiDisplayController* host,
            /* [in] */ IHandler* handler);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange,
            /* [in] */ IUri* uri);

    private:
        WifiDisplayController* mHost;
    };

    class WFDInfoActionListener
        : public Object
        , public IWifiP2pManagerActionListener
    {
    public:
        CAR_INTERFACE_DECL();

        WFDInfoActionListener(
            /* [in] */ WifiDisplayController* host);

        CARAPI OnSuccess();

        CARAPI OnFailure(
            /* [in] */ Int32 reason);

    private:
        WifiDisplayController* mHost;
    };

    class DiscoverPeersActionListener
        : public Object
        , public IWifiP2pManagerActionListener
    {
    public:
        CAR_INTERFACE_DECL();

        DiscoverPeersActionListener(
            /* [in] */ WifiDisplayController* host);

        CARAPI OnSuccess();

        CARAPI OnFailure(
            /* [in] */ Int32 reason);

    private:
        WifiDisplayController* mHost;
    };

    class RequestPeersPeerListListener
        : public Object
        , public IWifiP2pManagerPeerListListener
    {
    public:
        CAR_INTERFACE_DECL();

        RequestPeersPeerListListener(
            /* [in] */ WifiDisplayController* host);

        CARAPI OnPeersAvailable(
            /* [in] */ IWifiP2pDeviceList* peers);

    private:
        WifiDisplayController* mHost;
    };

    class MyActionListenerEx2
        : public Object
        , public IWifiP2pManagerActionListener
    {
    public:
        CAR_INTERFACE_DECL();

        MyActionListener(
            /* [in] */ WifiDisplayController* host,
            /* [in] */ IWifiP2pDevice* oldDevice,
            /* [in] */ Boolean isDisconnecting);

        CARAPI OnSuccess();

        CARAPI OnFailure(
            /* [in] */ Int32 reason);

    private:
        CARAPI_(void) Next();

    private:
        WifiDisplayController* mHost;
        AutoPtr<IWifiP2pDevice> mOldDevice;
        Boolean mIsDisconnecting;
    };

    class ConnectDeviceActionListener
        : public Object
        , public IWifiP2pManagerActionListener
    {
    public:
        CAR_INTERFACE_DECL();

        ConnectDeviceActionListener(
            /* [in] */ WifiDisplayController* host,
            /* [in] */ IWifiP2pDevice* newDevice);

        CARAPI OnSuccess();

        CARAPI OnFailure(
            /* [in] */ Int32 reason);

    private:
        WifiDisplayController* mHost;
        AutoPtr<IWifiP2pDevice> mNewDevice;
    };

    class DisconnectingDeviceActionListener
        : public Object
        , public IWifiP2pManagerActionListener
    {
    public:
        CAR_INTERFACE_DECL();

        DisconnectingDeviceActionListener(
            /* [in] */ WifiDisplayController* host,
            /* [in] */ IWifiP2pDevice* oldDevice);

        CARAPI OnSuccess();

        CARAPI OnFailure(
            /* [in] */ Int32 reason);

    private:
        CARAPI_(void) Next();

    private:
        WifiDisplayController* mHost;
        AutoPtr<IWifiP2pDevice> mOldDevice;
    };

    class CancelingDeviceActionListener
        : public Object
        , public IWifiP2pManagerActionListener
    {
    public:
        CAR_INTERFACE_DECL();

        CancelingDeviceActionListener(
            /* [in] */ WifiDisplayController* host,
            /* [in] */ IWifiP2pDevice* oldDevice);

        CARAPI OnSuccess();

        CARAPI OnFailure(
            /* [in] */ Int32 reason);

    private:
        CARAPI_(void) Next();

    private:
        WifiDisplayController* mHost;
        AutoPtr<IWifiP2pDevice> mOldDevice;
    };

    class MyRemoteDisplayListener
        : public Object
        , public IRemoteDisplayListener
    {
    public:
        CAR_INTERFACE_DECL();

        MyRemoteDisplayListener(
            /* [in] */ WifiDisplayController* host,
            /* [in] */ IWifiP2pDevice* oldDevice);

        CARAPI OnDisplayConnected(
            /* [in] */ ISurface* surface,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height,
            /* [in] */ Int32 flags);

        CARAPI OnDisplayDisconnected();

        CARAPI OnDisplayError(
            /* [in] */ Int32 error);

    private:
        WifiDisplayController* mHost;
        AutoPtr<IWifiP2pDevice> mOldDevice;
    };

    class ConnectionChangedGroupInfoListener
        : public Object
        , public IWifiP2pManagerGroupInfoListener
    {
    public:
        CAR_INTERFACE_DECL();

        ConnectionChangedGroupInfoListener(
            /* [in] */ WifiDisplayController* host);

        CARAPI OnGroupInfoAvailable(
            /* [in] */ IWifiP2pGroup* info);

    private:
        WifiDisplayController* mHost;
    };

    class AdvertiseDisplayRunnable
        : public Runnable
    {
    public:
        AdvertiseDisplayRunnable(
            /* [in] */ IWifiDisplay* display,
            /* [in] */ ISurface* surface,
            /* [in] */ WifiDisplayController* host)
            : mOldDisplay(display)
            , mOldSurface(surface)
            , mHost(host)
        {}

        CARAPI Run();

    private:
        AutoPtr<IWifiDisplay> mOldDisplay;
        AutoPtr<ISurface> mOldSurface;
        WifiDisplayController* mHost;
    };

    class ReportFeatureStateRunnable
        : public Runnable
    {
    public:
        ReportFeatureStateRunnable(
            /* [in] */ Int32 featureState,
            /* [in] */ WifiDisplayController* host);

        CARAPI Run();

    private:
        Int32 mFeatureState;
        WifiDisplayController* mHost;
    };

    class DiscoverPeersRunnable
        : public Runnable
    {
    public:
        DiscoverPeersRunnable(
            /* [in] */ Int32 reason,
            /* [in] */ WifiDisplayController* host);

        CARAPI Run();

    private:
        Int32 mReason;
        WifiDisplayController* mHost;
    };

    class ScanStartedRunnable
        : public Runnable
    {
    public:
        ScanStartedRunnable(
            /* [in] */ WifiDisplayController* host);

        CARAPI Run();

    private:
        WifiDisplayController* mHost;
    };

    class ScanFinishedRunnable
        : public Runnable
    {
    public:
        ScanFinishedRunnable(
            /* [in] */ ArrayOf<IWifiDisplay*>* displays,
            /* [in] */ WifiDisplayController* host);

        CARAPI Run();

    private:
        AutoPtr<ArrayOf<IWifiDisplay*> > mDisplays;
        WifiDisplayController* mHost;
    };

    class ConnectionFailureRunnable
        : public Runnable
    {
    public:
        ConnectionFailureRunnable(
            /* [in] */ IWifiP2pDevice* oldDevice,
            /* [in] */ WifiDisplayController* host);

        CARAPI Run();

    private:
        AutoPtr<IWifiP2pDevice> mOldDevice;
        WifiDisplayController* mHost;
    };

    class ConnectionTimeoutRunnable
        : public Runnable
    {
    public:
        ConnectionTimeoutRunnable(
            /* [in] */ WifiDisplayController* host);

        CARAPI Run();

    private:
        WifiDisplayController* mHost;
    };

    class RtspTimeoutRunnable
        : public Runnable
    {
    public:
        RtspTimeoutRunnable(
            /* [in] */ WifiDisplayController* host);

        CARAPI Run();

    private:
        WifiDisplayController* mHost;
    };

public:
    WifiDisplayController(
        /* [in] */ IContext* context,
        /* [in] */ IHandler* handler,
        /* [in] */ IWifiDisplayControllerListener* listener);

    CARAPI_(void) Dump(
        /* [in] */ IPrintWriter* pw);

    CARAPI_(void) RequestScan();

    CARAPI_(void) RequestConnect(
        /* [in] */ const String& address);

    CARAPI_(void) RequestDisconnect();

private:

    CARAPI_(void) UpdateSettings();

    CARAPI_(void) UpdateWfdEnableState();

    CARAPI_(void) ReportFeatureState();

    CARAPI_(Int32) ComputeFeatureState();

    CARAPI_(void) DiscoverPeers();

    CARAPI_(void) TryDiscoverPeers();

    CARAPI_(void) RequestPeers();

    CARAPI_(void) HandleScanStarted();

    CARAPI_(void) HandleScanFinished();

    CARAPI_(void) UpdateDesiredDevice(
        /* [in] */ IWifiP2pDevice* device);

    CARAPI_(void) Connect(
        /* [in] */ IWifiP2pDevice* device);

    CARAPI_(void) Disconnect();

    CARAPI_(void) RetryConnection();

    /**
     * This function is called repeatedly after each asynchronous operation
     * until all preconditions for the connection have been satisfied and the
     * connection is established (or not).
     */
    CARAPI_(void) UpdateConnection();

    CARAPI_(void) SetRemoteSubmixOn(
        /* [in] */ Boolean on);

    CARAPI_(void) HandleStateChanged(
        /* [in] */ Boolean enabled);

    CARAPI_(void) HandlePeersChanged();

    CARAPI_(void) HandleConnectionChanged(
        /* [in] */ INetworkInfo* networkInfo);

    CARAPI_(void) HandleConnectionFailure(
        /* [in] */ Boolean timeoutOccurred);

    CARAPI_(void) AdvertiseDisplay(
        /* [in] */ IWifiDisplay* display,
        /* [in] */ ISurface* surface,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 flags);

    CARAPI_(void) UnadvertiseDisplay();

    CARAPI_(void) ReadvertiseDisplay(
        /* [in] */ IWifiDisplay* display);
private:
    static CARAPI_(AutoPtr<IInet4Address>) GetInterfaceAddress(
        /* [in] */ IWifiP2pGroup* info);

    static CARAPI_(Int32) GetPortNumber(
        /* [in] */ IWifiP2pDevice* device);

    static CARAPI_(Boolean) IsWifiDisplay(
        /* [in] */ IWifiP2pDevice* device);

    static CARAPI_(Boolean) IsPrimarySinkDeviceType(
        /* [in] */ Int32 deviceType);

    static CARAPI_(String) DescribeWifiP2pDevice(
        /* [in] */ IWifiP2pDevice* device);

    static CARAPI_(String) DescribeWifiP2pGroup(
        /* [in] */ IWifiP2pGroup* group);

    static CARAPI_(AutoPtr<IWifiDisplay>) CreateWifiDisplay(
        /* [in] */ IWifiP2pDevice* device);

private:
    static const String TAG ;
    static const Boolean DEBUG;

    static const Int32 DEFAULT_CONTROL_PORT = 7236;
    static const Int32 MAX_THROUGHPUT = 50;
    static const Int32 CONNECTION_TIMEOUT_SECONDS = 60;
    static const Int32 RTSP_TIMEOUT_SECONDS = 15;

    static const Int32 DISCOVER_PEERS_MAX_RETRIES = 10;
    static const Int32 DISCOVER_PEERS_RETRY_DELAY_MILLIS = 500;

    static const Int32 CONNECT_MAX_RETRIES = 3;
    static const Int32 CONNECT_RETRY_DELAY_MILLIS = 500;

    // A unique token to identify the remote submix that is managed by Wifi display.
    // It must match what the media server uses when it starts recording the submix
    // for transmission.  We use 0 although the actual value is currently ignored.
    static const Int32 REMOTE_SUBMIX_ADDRESS = 0;

    AutoPtr<IContext> mContext;
    AutoPtr<IHandler> mHandler;
    AutoPtr<IWifiDisplayControllerListener> mListener;

    AutoPtr<IWifiP2pManager> mWifiP2pManager;
    AutoPtr<IWifiP2pManagerChannel> mWifiP2pChannel;

    AutoPtr<IAudioManager> mAudioManager;

    Boolean mWifiP2pEnabled;
    Boolean mWfdEnabled;
    Boolean mWfdEnabling;
    AutoPtr<INetworkInfo> mNetworkInfo;

    List<AutoPtr<IWifiP2pDevice> > mAvailableWifiDisplayPeers;

    // True if Wifi display is enabled by the user.
    Boolean mWifiDisplayOnSetting;

    // True if there is a call to discoverPeers in progress.
    Boolean mDiscoverPeersInProgress;

    // Number of discover peers retries remaining.
    Int32 mDiscoverPeersRetriesLeft;

    // The device to which we want to connect, or NULL if we want to be disconnected.
    AutoPtr<IWifiP2pDevice> mDesiredDevice;

    // The device to which we are currently connecting, or NULL if we have already connected
    // or are not trying to connect.
    AutoPtr<IWifiP2pDevice> mConnectingDevice;

    // The device from which we are currently disconnecting.
    AutoPtr<IWifiP2pDevice> mDisconnectingDevice;

    // The device to which we were previously trying to connect and are now canceling.
    AutoPtr<IWifiP2pDevice> mCancelingDevice;

    // The device to which we are currently connected, which means we have an active P2P group.
    AutoPtr<IWifiP2pDevice> mConnectedDevice;

    // The group info obtained after connecting.
    AutoPtr<IWifiP2pGroup> mConnectedDeviceGroupInfo;

    // Number of connection retries remaining.
    Int32 mConnectionRetriesLeft;

    // The remote display that is listening on the connection.
    // Created after the Wifi P2P network is connected.
    AutoPtr<IRemoteDisplay> mRemoteDisplay;

    // The remote display interface.
    String mRemoteDisplayInterface;

    // True if RTSP has connected.
    Boolean mRemoteDisplayConnected;

    // True if the remote submix is enabled.
    Boolean mRemoteSubmixOn;

    // The information we have most recently told WifiDisplayAdapter about.
    AutoPtr<IWifiDisplay> mAdvertisedDisplay;
    AutoPtr<ISurface> mAdvertisedDisplaySurface;
    Int32 mAdvertisedDisplayWidth;
    Int32 mAdvertisedDisplayHeight;
    Int32 mAdvertisedDisplayFlags;

    AutoPtr<IRunnable> mConnectionTimeout;
    AutoPtr<IRunnable> mRtspTimeout;
    AutoPtr<IBroadcastReceiver> mWifiP2pReceiver;
};

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_DISPLAY_WIFIDISPLAYCONTROLLER_H__
