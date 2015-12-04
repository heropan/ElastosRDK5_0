#ifndef __ELASTOS_DROID_INTERNAL_POLICY_IMPL_RECENTAPPLICATIONSDIALOG_H__
#define __ELASTOS_DROID_INTERNAL_POLICY_IMPL_RECENTAPPLICATIONSDIALOG_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/app/Dialog.h"
#include "elastos/droid/content/BroadcastReceiver.h"

using Elastos::Droid::App::Dialog;
using Elastos::Droid::App::IActivityManagerRecentTaskInfo;
using Elastos::Droid::App::IStatusBarManager;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::Widget::ITextView;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

class RecentApplicationsDialog
    : public Dialog
    , public IViewOnClickListener
{
public:
    class RecentTag
    {
    public:
        AutoPtr<IActivityManagerRecentTaskInfo> info;
        AutoPtr<IIntent> intent;
    };

private:
    class InnerRunnable
        : public Object
        , public IRunnable
    {
    public:
        CAR_INTERFACE_DECL()

        InnerRunnable(
            /* [in] */ RecentApplicationsDialog* owner);

        virtual CARAPI Run();

    private:
        RecentApplicationsDialog* mOwner;
    };

    class InnerBroadcastReceiver
        : public BroadcastReceiver
    {
    public:
        InnerBroadcastReceiver(
            /* [in] */ RecentApplicationsDialog* owner);

        // @Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        RecentApplicationsDialog* mOwner;
    };

public:
    CAR_INTERFACE_DECL()

    RecentApplicationsDialog();

    CARAPI constructor(
        /* [in] */ IContext* context);

    // @Override
    CARAPI OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    /**
      * Dismiss the dialog and switch to the selected application.
      */
    virtual CARAPI DismissAndSwitch();

    /**
      * Handler for user clicks.  If a button was clicked, launch the corresponding activity.
      */
    virtual CARAPI OnClick(
        /* [in] */ IView* v);

    /**
      * Set up and show the recent activities dialog.
      */
    // @Override
    CARAPI OnStart();

    /**
      * Dismiss the recent activities dialog.
      */
    // @Override
    CARAPI OnStop();

protected:
    /**
      * We create the recent applications dialog just once, and it stays around (hidden)
      * until activated by the user.
      *
      * @see PhoneWindowManager#showRecentAppsDialog
      */
    // @Override
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

private:
    CARAPI_(AutoPtr< ArrayOf< AutoPtr<ITextView> > >) MiddleInitMicons();

    CARAPI_(void) SwitchTo(
        /* [in] */ RecentTag* tag);

    /**
      * Reload the 6 buttons with recent activities
      */
    CARAPI_(void) ReloadButtons();

public:
    // allow for some discards
    AutoPtr< ArrayOf<ITextView> > mIcons;
    AutoPtr<IView> mNoAppsText;
    AutoPtr<IIntentFilter> mBroadcastIntentFilter;
    AutoPtr<IHandler> mHandler;
    AutoPtr<Runnable> mCleanup;

private:
    // Elements for debugging support
    //  private static final String LOG_TAG = "RecentApplicationsDialog";
    static const Boolean DBG_FORCE_EMPTY_LIST;
    static AutoPtr<IStatusBarManager> sStatusBar;
    static const Int32 NUM_BUTTONS = 8;
    static const Int32 MAX_RECENT_TASKS = NUM_BUTTONS * 2;
    /**
      * This is the listener for the ACTION_CLOSE_SYSTEM_DIALOGS intent.  It's an indication that
      * we should close ourselves immediately, in order to allow a higher-priority UI to take over
      * (e.g. phone call received).
      */
    AutoPtr<IBroadcastReceiver> mBroadcastReceiver;
};

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_POLICY_IMPL_RECENTAPPLICATIONSDIALOG_H__

