
#ifndef __ELASTOS_DROID_SERVER_DISPLAY_WIFIDISPLAYADAPTER_H__
#define __ELASTOS_DROID_SERVER_DISPLAY_WIFIDISPLAYADAPTER_H__

#include "display/DisplayAdapter.h"
#include "display/DisplayDevice.h"
#include "elastos/droid/content/BroadcastReceiver.h"
#include "elastos/droid/os/HandlerBase.h"
#include "elastos/droid/os/Runnable.h"

using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::App::INotificationManager;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Hardware::Display::IWifiDisplayStatus;
using Elastos::Droid::Hardware::Display::IWifiDisplay;
using Elastos::Droid::View::ISurface;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

class WifiDisplayController;
class DisplayDeviceInfo;
class PersistentDataStore;

/**
 * Connects to Wifi displays that implement the Miracast protocol.
 * <p>
 * The Wifi display protocol relies on Wifi direct for discovering and pairing
 * with the display.  Once connected, the Media Server opens an RTSP socket and accepts
 * a connection from the display.  After session negotiation, the Media Server
 * streams encoded buffers to the display.
 * </p><p>
 * This class is responsible for connecting to Wifi displays and mediating
 * the interactions between Media Server, Surface Flinger and the Display Manager Service.
 * </p><p>
 * Display adapters are guarded by the {@link DisplayManagerService.SyncRoot} lock.
 * </p>
 */
class WifiDisplayAdapter : public DisplayAdapter
{
private:
    class MyBroadcastReceiver
        : public BroadcastReceiver
    {
    public:
        MyBroadcastReceiver(
            /* [in] */ WifiDisplayAdapter* owner);

        //@Override
        virtual CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("WifiDisplayAdapter::MyBroadcastReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        WifiDisplayAdapter* mHost;
    };

    class MyWifiDisplayControllerListener
        : public ElRefBase
        , public IWifiDisplayControllerListener
    {
    public:
        CAR_INTERFACE_DECL()

        MyWifiDisplayControllerListener(
           /* [in] */ WifiDisplayAdapter* owner);

        //@Override
        CARAPI OnFeatureStateChanged(
           /* [in] */ Int32 featureState);

        //@Override
        CARAPI OnScanStarted();

        //@Override
        CARAPI OnScanFinished(
           /* [in] */ ArrayOf<IWifiDisplay*>* availableDisplays);

        //@Override
        CARAPI OnDisplayConnecting(
           /* [in] */ IWifiDisplay* display);

        //@Override
        CARAPI OnDisplayConnectionFailed();

        //@Override
        CARAPI OnDisplayConnected(
           /* [in] */ IWifiDisplay* display,
           /* [in] */ ISurface* surface,
           /* [in] */ Int32 width,
           /* [in] */ Int32 height,
           /* [in] */ Int32 flags);

        //@Override
        CARAPI OnDisplayChanged(
            /* [in] */ IWifiDisplay* display);

        //@Override
        CARAPI OnDisplayDisconnected();

    private:
        static Boolean Equals(
            /* [in] */ ArrayOf<IWifiDisplay*>* array1,
            /* [in] */ ArrayOf<IWifiDisplay*>* array2);

    private:
        WifiDisplayAdapter* mHost;
    };

    class WifiDisplayDevice : public DisplayDevice
    {
    public:
        WifiDisplayDevice(
            /* [in] */ WifiDisplayAdapter* owner,
            /* [in] */ IBinder* displayToken,
            /* [in] */ const String& name,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height,
            /* [in] */ Float refreshRate,
            /* [in] */ Int32 flags,
            /* [in] */ const String& address,
            /* [in] */ ISurface* surface);

        CARAPI_(void) ClearSurfaceLocked();

        CARAPI_(void) SetNameLocked(
            /* [in] */ const String& name);

        //@Override
        CARAPI_(void) PerformTraversalInTransactionLocked();

        //@Override
        CARAPI_(AutoPtr<DisplayDeviceInfo>) GetDisplayDeviceInfoLocked();

    private:
        String mName;
        const Int32 mWidth;
        const Int32 mHeight;
        const Float mRefreshRate;
        const Int32 mFlags;
        const String mAddress;

        AutoPtr<ISurface> mSurface;
        AutoPtr<DisplayDeviceInfo> mInfo;

        WifiDisplayAdapter* mHost;
    };

    class WifiDisplayHandler
        : public HandlerBase
    {
    public:
        WifiDisplayHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ WifiDisplayAdapter* host);

        virtual CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        WifiDisplayAdapter* mHost;
    };

    class RegisterRunnable
        : public Runnable
    {
    public:
        RegisterRunnable(
            /* [in] */ WifiDisplayAdapter* host);

        virtual CARAPI Run();

    private:
        WifiDisplayAdapter* mHost;
    };

    class RequestScanRunnable
        : public Runnable
    {
    public:
        RequestScanRunnable(
            /* [in] */ WifiDisplayAdapter* host);

        virtual CARAPI Run();

    private:
        WifiDisplayAdapter* mHost;
    };

    class RequestConnectRunnable
        : public Runnable
    {
    public:
        RequestConnectRunnable(
            /* [in] */ const String& address,
            /* [in] */ WifiDisplayAdapter* host);

        virtual CARAPI Run();

    private:
        String mAddress;
        WifiDisplayAdapter* mHost;
    };

    class RequestDisconnectRunnable
        : public Runnable
    {
    public:
        RequestDisconnectRunnable(
            /* [in] */ WifiDisplayAdapter* host);

        virtual CARAPI Run();

    private:
        WifiDisplayAdapter* mHost;
    };

public:
    // Called with SyncRoot lock held.
    WifiDisplayAdapter(
        /* [in] */ Object* syncRoot,
        /* [in] */ IContext* context,
        /* [in] */ IHandler* handler,
        /* [in] */ IDisplayAdapterListener* listener,
        /* [in] */ PersistentDataStore* persistentDataStore);

    //@Override
    CARAPI_(void) DumpLocked(
        /* [in] */ IPrintWriter* pw);

    //@Override
    CARAPI_(void) RegisterLocked();

    CARAPI_(void) RequestScanLocked();

    CARAPI_(void) RequestConnectLocked(
        /* [in] */ const String& address,
        /* [in] */ Boolean trusted);

    CARAPI_(void) RequestDisconnectLocked();

    CARAPI_(void) RequestRenameLocked(
        /* [in] */ const String& address,
        /* [in] */ const String& alias);

    CARAPI_(void) RequestForgetLocked(
        /* [in] */ const String& address);

    CARAPI_(AutoPtr<IWifiDisplayStatus>) GetWifiDisplayStatusLocked();

private:
    CARAPI_(Boolean) IsRememberedDisplayLocked(
        /* [in] */ const String& address);

    CARAPI_(void) UpdateRememberedDisplaysLocked();

    CARAPI_(void) FixRememberedDisplayNamesFromAvailableDisplaysLocked();

    CARAPI_(AutoPtr<IWifiDisplay>) FindAvailableDisplayLocked(
        /* [in] */ const String& address);

    CARAPI_(void) AddDisplayDeviceLocked(
        /* [in] */ IWifiDisplay* display,
        /* [in] */ ISurface* surface,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 flags);

    CARAPI_(void) RemoveDisplayDeviceLocked();

    CARAPI_(void) RenameDisplayDeviceLocked(
        /* [in] */ const String& name);

    CARAPI_(void) ScheduleStatusChangedBroadcastLocked();

    CARAPI_(void) ScheduleUpdateNotificationLocked();

    // Runs on the handler.
    CARAPI_(void) HandleSendStatusChangeBroadcast();

    // Runs on the handler.
    CARAPI_(void) HandleUpdateNotification();

    CARAPI_(void) HandleRegister();

    CARAPI_(void) HandleRequestScan();

    CARAPI_(void) HandleRequestConnect(
        /* [in] */ const String& address);

    CARAPI_(void) HandleRequestDisconnect();

private:
    static const String TAG;

    static const Boolean DEBUG;// = FALSE;

    static const Int32 MSG_SEND_STATUS_CHANGE_BROADCAST = 1;
    static const Int32 MSG_UPDATE_NOTIFICATION = 2;

    static const String ACTION_DISCONNECT;

    AutoPtr<WifiDisplayHandler> mHandler;
    AutoPtr<PersistentDataStore> mPersistentDataStore;
    Boolean mSupportsProtectedBuffers;
    AutoPtr<INotificationManager> mNotificationManager;

    AutoPtr<IPendingIntent> mSettingsPendingIntent;
    AutoPtr<IPendingIntent> mDisconnectPendingIntent;

    AutoPtr<WifiDisplayController> mDisplayController;
    AutoPtr<WifiDisplayDevice> mDisplayDevice;

    AutoPtr<IWifiDisplayStatus> mCurrentStatus;
    Int32 mFeatureState;
    Int32 mScanState;
    Int32 mActiveDisplayState;
    AutoPtr<IWifiDisplay> mActiveDisplay;
    AutoPtr< ArrayOf<IWifiDisplay*> > mAvailableDisplays;
    AutoPtr< ArrayOf<IWifiDisplay*> > mRememberedDisplays;

    Boolean mPendingStatusChangeBroadcast;
    Boolean mPendingNotificationUpdate;

    AutoPtr<IBroadcastReceiver> mBroadcastReceiver;
    AutoPtr<IWifiDisplayControllerListener> mWifiDisplayListener;
};

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_DISPLAY_WIFIDISPLAYADAPTER_H__
