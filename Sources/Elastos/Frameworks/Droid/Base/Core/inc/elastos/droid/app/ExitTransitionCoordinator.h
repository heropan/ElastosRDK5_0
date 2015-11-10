
#ifndef __ELASTOS_DROID_APP_EXIT_TRANSITION_COORDINATOR_H__
#define __ELASTOS_DROID_APP_EXIT_TRANSITION_COORDINATOR_H__

#include "elastos/droid/app/ActivityTransitionCoordinator.h"

using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Transition::ITransition;
using Elastos::Droid::View::IViewGroup;

using Elastos::Utility::IArrayList;

namespace Elastos {
namespace Droid {
namespace App {

/**
 * This ActivityTransitionCoordinator is created in ActivityOptions#makeSceneTransitionAnimation
 * to govern the exit of the Scene and the shared elements when calling an Activity as well as
 * the reentry of the Scene when coming back from the called Activity.
 */
class ExitTransitionCoordinator
    : public ActivityTransitionCoordinator
    , public IExitTransitionCoordinator
{
public:
    CAR_INTERFACE_DECL()

    ExitTransitionCoordinator();

    virtual ~ExitTransitionCoordinator();

    CARAPI constructor(
        /* [in] */ IActivity* activity,
        /* [in] */ IArrayList* names, //ArrayList<String>
        /* [in] */ IArrayList* accepted, //ArrayList<String>
        /* [in] */ IArrayList* mapped,//ArrayList<View>
        /* [in] */ Boolean isReturning);

    CARAPI ResetViews();

    CARAPI StartExit();

    CARAPI StartExit(
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);

    CARAPI Stop();

protected:
    void OnReceiveResult(
        /* [in] */ Int32 resultCode,
        /* [in] */ IBundle* resultData);

    CARAPI_(Boolean) MoveSharedElementWithParent();

    CARAPI_(AutoPtr<ITransition>) GetViewsTransition();

    CARAPI_(AutoPtr<ITransition>) GetSharedElementTransition();

    CARAPI_(Boolean) IsReadyToNotify();

    CARAPI_(void) NotifyComplete();

private:
    static AutoPtr<ISharedElementCallback> GetListener(
        /* [in] */ IActivity* activity,
        /* [in] */ Boolean isReturning);

    CARAPI_(void) StopCancel();

    CARAPI_(void) DelayCancel();

    CARAPI_(void) SharedElementExitBack();

    CARAPI_(void) StartSharedElementExit(
        /* [in] */ IViewGroup* decorView);

    CARAPI_(void) HideSharedElements();

    CARAPI_(void) StartExitTransition();

    CARAPI_(void) FadeOutBackground();

    AutoPtr<ITransition> GetExitTransition();

    AutoPtr<ITransition> GetSharedElementExitTransition();

    CARAPI_(void) BeginTransitions();

    CARAPI_(void) ExitTransitionComplete();

    CARAPI_(void) SharedElementTransitionComplete();

    AutoPtr<IBundle> CaptureExitSharedElementsState();

    CARAPI_(void) FinishIfNecessary();

    CARAPI_(void) Finish();

private:
    static const String TAG;
    static const Int64 MAX_WAIT_MS;

    Boolean mExitComplete;

    AutoPtr<IBundle> mSharedElementBundle;

    Boolean mExitNotified;

    Boolean mSharedElementNotified;

    AutoPtr<IActivity> mActivity;

    Boolean mIsBackgroundReady;

    Boolean mIsCanceled;

    AutoPtr<IHandler> mHandler;

    AutoPtr<IObjectAnimator> mBackgroundAnimator;

    Boolean mIsHidden;

    AutoPtr<IBundle> mExitSharedElementBundle;

    Boolean mIsExitStarted;

    Boolean mSharedElementsHidden;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_EXIT_TRANSITION_COORDINATOR_H__
