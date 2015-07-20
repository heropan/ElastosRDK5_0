
#ifndef __SHUTDOWNTHREAD_H__
#define __SHUTDOWNTHREAD_H__

#include "Elastos.Droid.Server_server.h"
#include "ext/frameworkdef.h"
#include "content/BroadcastReceiver.h"
#include "os/HandlerBase.h"
#include <elastos/Core/Object.h>


using Elastos::Core::Object;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IDialogInterfaceOnDismissListener;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::IDialogInterfaceOnMultiChoiceClickListener;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::App::IDialog;
using Elastos::Droid::App::IAlertDialog;
using Elastos::Droid::App::IProgressDialog;
using Elastos::Droid::Os::IPowerManager;
using Elastos::Droid::Os::IPowerManagerWakeLock;
using Elastos::Droid::View::IWindowManagerPolicy;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

class ShutdownThread
    : public ThreadBase
{
private:
    class CloseDialogReceiver
        : public BroadcastReceiver
        , public IDialogInterfaceOnDismissListener
    {
    public:
        CloseDialogReceiver(
            /* [in] */IContext* context);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [out] */ InterfaceID* pIID);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

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

    class DialogInterfaceOnClickListener
        : public ElRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        DialogInterfaceOnClickListener(
            /* [in] */ IContext* context);

        CAR_INTERFACE_DECL();

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        AutoPtr<IContext> mContext;
    };

    class DialogInterfaceOnMultiChoiceClickListener
        : public ElRefBase
        , public IDialogInterfaceOnMultiChoiceClickListener
    {
    public:
        DialogInterfaceOnMultiChoiceClickListener(
            /* [in] */ IContext* context);

        CAR_INTERFACE_DECL();

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which,
            /* [in] */ Boolean isChecked);

    private:
        AutoPtr<IContext> mContext;
    };

    class DialogInterfaceOnClickListener2
        : public ElRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        DialogInterfaceOnClickListener2(
            /* [in] */ IContext* context,
            /* [in] */ IWindowManagerPolicy* policy);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        AutoPtr<IContext> mContext;
        AutoPtr<IWindowManagerPolicy> mPolicy;
    };

    class ActionDoneBroadcastReceiver : public BroadcastReceiver
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

    class ShutdownRadiosThread
        : public ThreadBase
    {
    public:
        ShutdownRadiosThread(
            /* [in] */ ArrayOf<Boolean>* done,
            /* [in] */ Int64 endTime);

        virtual CARAPI Run();

    private:
        AutoPtr< ArrayOf<Boolean> > mDone;
        Int64 mEndTime;
    };

    class ShutdownThreadHandler
        : public HandlerBase
    {
    public:
        ShutdownThreadHandler(
            /* [in] */ IProgressDialog* pd);

        virtual CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        AutoPtr<IProgressDialog> mPd;
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
        /* [in] */ Boolean confirm,
        /* [in] */ IWindowManagerPolicy* policy);

    static CARAPI_(void) ShutdownInner(
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

    CARAPI_(void) ActionDone();

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

private:
    ShutdownThread();

    ~ShutdownThread();

    CARAPI_(void) SaveAirplaneModeState(
        /* [in] */ Boolean enabled);

    CARAPI_(void) EnableAirplaneModeState();

    CARAPI_(void) SetAirplaneModeState(
        /* [in] */ Boolean enabled);

    static CARAPI_(void) BeginShutdownSequence(
        /* [in] */ IContext* context);

    CARAPI_(void) KillRemoveActivity(
        /* [in] */ IContext* context);

    CARAPI_(void) KillRemoveService(
        /* [in] */ IContext* context);

    CARAPI_(void) ShutdownRadios(
        /* [in] */ Int32 timeout);

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

    static const Int32 CLOSE_PROCESS_DIALOG;

    static const Int32 MAX_SERVICES;

    static const Int32 MAX_ACTIVITYS;

    static const Int32 BOOTFAST_WAIT_TIME;

    // state tracking
    static Object sIsStartedGuard;
    static Boolean sIsStarted;

    static Boolean mReboot;
    static Boolean mRebootSafeMode;
    static AutoPtr<IWindowManagerPolicy> mPolicy;
    static String mRebootReason;

    // static instance of this thread
    static AutoPtr<ShutdownThread> sInstance;

    Object mActionDoneSync;
    Boolean mActionDone;
    AutoPtr<IContext> mContext;
    AutoPtr<IPowerManager> mPowerManager;
    AutoPtr<IPowerManagerWakeLock> mCpuWakeLock;
    AutoPtr<IPowerManagerWakeLock> mScreenWakeLock;
    AutoPtr<ShutdownThreadHandler> mHandler;

    static Boolean mBootFastEnable;

    static AutoPtr<IAlertDialog> sConfirmDialog;
};

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __SHUTDOWNTHREAD_H__
