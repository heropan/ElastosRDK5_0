
#ifndef __ELASTOS_DROID_SERVER_POWER_SHUTDOWNTHREAD_H__
#define __ELASTOS_DROID_SERVER_POWER_SHUTDOWNTHREAD_H__

#include <Elastos.Droid.Media.h>
#include <Elastos.Droid.Os.h>
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/content/BroadcastReceiver.h"
#include <elastos/core/Object.h>
#include <elastos/core/Thread.h>

using Elastos::Droid::App::IDialog;
using Elastos::Droid::App::IAlertDialog;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IDialogInterfaceOnDismissListener;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Media::IAudioAttributes;
using Elastos::Droid::Os::Storage::IIMountShutdownObserver;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IPowerManager;
using Elastos::Droid::Os::IPowerManagerWakeLock;
using Elastos::Core::Thread;
using Elastos::Core::IThread;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

class ShutdownThread
    : public Thread
{
public:
    class MountShutdownObserver
        : public Object
        , public IIMountShutdownObserver
        , public IBinder
    {
    public:
        CAR_INTERFACE_DECL();

        MountShutdownObserver();

        ~MountShutdownObserver();

        CARAPI constructor(
            /* [in] */ IThread* host);

        //@Override
        CARAPI OnShutDownComplete(
            /* [in] */ Int32 statusCode);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("ShutdownThread::MountShutdownObserver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }

    private:
        ShutdownThread* mHost;
    };

private:
    class CloseDialogReceiver
        : public BroadcastReceiver
        , public IDialogInterfaceOnDismissListener
    {
    public:
        CAR_INTERFACE_DECL();

        CloseDialogReceiver(
            /* [in] */IContext* context);

        //@Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        //@Override
        CARAPI OnDismiss(
            /* [in] */ IDialogInterface* unused);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("ShutdownThread::CloseDialogReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }

    public:
        AutoPtr<IDialog> mDialog;

    private:
        AutoPtr<IContext> mContext;
    };

    // used in function ShutdownInner();
    class DialogInterfaceOnClickListener
        : public Object
        , public IDialogInterfaceOnClickListener
    {
    public:
        CAR_INTERFACE_DECL();

        DialogInterfaceOnClickListener(
            /* [in] */ IContext* context);

        //@Override
        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        AutoPtr<IContext> mContext;
    };

    // used in function Run();
    class ActionDoneBroadcastReceiver
        : public BroadcastReceiver
    {
    public:
        ActionDoneBroadcastReceiver(
            /* [in] */ ShutdownThread* host);

        //@Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("ShutdownThread::ActionDoneBroadcastReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }

    private:
        ShutdownThread* mHost;
    };

    // used in function ShutdownRadios();
    class ShutdownRadiosThread
        : public Thread
    {
    public:
        ShutdownRadiosThread(
            /* [in] */ ArrayOf<Boolean>* done,
            /* [in] */ Int64 endTime);

        //@Override
        virtual CARAPI Run();

    private:
        AutoPtr< ArrayOf<Boolean> > mDone;
        Int64 mEndTime;
    };

public:
    /**
     * Request a clean shutdown, waiting for subsystems to clean up their
     * state etc.  Must be called from a Looper thread in which its UI
     * is shown.
     *
     * @param context Context used to display the shutdown progress dialog.
     * @param confirm true if user confirmation is needed before shutting down.
     */
    static CARAPI_(void) Shutdown(
        /* [in] */ IContext* context,
        /* [in] */ Boolean confirm);

    /**
     * Request a clean shutdown, waiting for subsystems to clean up their
     * state etc.  Must be called from a Looper thread in which its UI
     * is shown.
     *
     * @param context Context used to display the shutdown progress dialog.
     * @param reason code to pass to the kernel (e.g. "recovery"), or null.
     * @param confirm true if user confirmation is needed before shutting down.
     */
    static CARAPI_(void) Reboot(
        /* [in] */ IContext* context,
        /* [in] */ const String& reason,
        /* [in] */ Boolean confirm);

    /**
     * Request a reboot into safe mode.  Must be called from a Looper thread in which its UI
     * is shown.
     *
     * @param context Context used to display the shutdown progress dialog.
     * @param confirm true if user confirmation is needed before shutting down.
     */
    static CARAPI_(void) RebootSafeMode(
        /* [in] */ IContext* context,
        /* [in] */ Boolean confirm);

    /**
     * Makes sure we handle the shutdown gracefully.
     * Shuts off power regardless of radio and bluetooth state if the alloted time has passed.
     */
    virtual CARAPI Run();

    /**
     * Do not call this directly. Use {@link #reboot(Context, String, boolean)}
     * or {@link #shutdown(Context, boolean)} instead.
     *
     * @param reboot true to reboot or false to shutdown
     * @param reason reason for reboot
     */
    static CARAPI_(void) RebootOrShutdown(
        /* [in] */ Boolean reboot,
        /* [in] */ const String& reason);

protected:
    static CARAPI_(void) ShutdownInner(
        /* [in] */ IContext* context,
        /* [in] */ Boolean confirm);

    CARAPI_(void) ActionDone();

private:
    ShutdownThread();

    ~ShutdownThread();

    static CARAPI_(void) BeginShutdownSequence(
        /* [in] */ IContext* context);

    CARAPI_(void) ShutdownRadios(
        /* [in] */ Int32 timeout);

    static CARAPI_(AutoPtr<ShutdownThread>) InitsInstance();

    static CARAPI_(AutoPtr<IAudioAttributes>) InitVIBRATION_ATTRIBUTES();

public:
    // Provides shutdown assurance in case the system_server is killed
    static const String SHUTDOWN_ACTION_PROPERTY;

    // Indicates whether we are rebooting into safe mode
    static const String REBOOT_SAFEMODE_PROPERTY;

private:
    // constants
    static const String TAG;
    static const Int32 PHONE_STATE_POLL_SLEEP_MSEC;
    // maximum time we wait for the shutdown broadcast before going on.
    static const Int32 MAX_BROADCAST_TIME;
    static const Int32 MAX_SHUTDOWN_WAIT_TIME;
    static const Int32 MAX_RADIO_WAIT_TIME;

    // length of vibration before shutting down
    static const Int32 SHUTDOWN_VIBRATE_MS;

    // state tracking
    static Object sIsStartedGuard;
    static Boolean sIsStarted;

    static Boolean mReboot;
    static Boolean mRebootSafeMode;
    static String mRebootReason;

    // static instance of this thread
    static AutoPtr<ShutdownThread> sInstance;

    static AutoPtr<IAudioAttributes> VIBRATION_ATTRIBUTES;

    Object mActionDoneSync;
    Boolean mActionDone;
    AutoPtr<IContext> mContext;
    AutoPtr<IPowerManager> mPowerManager;
    AutoPtr<IPowerManagerWakeLock> mCpuWakeLock;
    AutoPtr<IPowerManagerWakeLock> mScreenWakeLock;
    AutoPtr<IHandler> mHandler;

    static AutoPtr<IAlertDialog> sConfirmDialog;
};

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_POWER_SHUTDOWNTHREAD_H__
