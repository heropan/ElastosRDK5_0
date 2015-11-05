
#ifndef __ELASTOS_DROID_APP_ACTIVITY_TRANSITION_STATE_H__
#define __ELASTOS_DROID_APP_ACTIVITY_TRANSITION_STATE_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

namespace Elastos {
namespace Droid {
namespace App {

// using Elastos::Droid::os.Bundle;
// using Elastos::Droid::os.ResultReceiver;
// using Elastos::Droid::transition.Transition;
// using Elastos::Droid::util.ArrayMap;
// using Elastos::Droid::util.SparseArray;
// using Elastos::Droid::view.View;
// using Elastos::Droid::view.ViewGroup;
// using Elastos::Droid::view.Window;

// import java.lang.ref.WeakReference;
// import java.util.ArrayList;

/**
 * This class contains all persistence-related functionality for Activity Transitions.
 * Activities start exit and enter Activity Transitions through this class.
 */
class ActivityTransitionState
    : public Object
    , public IActivityTransitionState
{
public:
    CAR_INTERFACE_DECL()

    ActivityTransitionState();

    virtual ~ActivityTransitionState();

    CARAPI constructor();

    CARAPI AddExitTransitionCoordinator(
        /* [in] */ IExitTransitionCoordinator* exitTransitionCoordinator,
        /* [out] */ Int32* result);

    CARAPI ReadState(
        /* [in] */ IBundle* bundle);

    CARAPI SaveState(
        /* [in] */ IBundle* bundle);

    CARAPI SetEnterActivityOptions(
        /* [in] */ IActivity* activity,
        /* [in] */ IActivityOptions* options);

    CARAPI EnterReady(
        /* [in] */ IActivity* activity);

    CARAPI PostponeEnterTransition();

    CARAPI StartPostponedEnterTransition();

    CARAPI OnStop();

    CARAPI OnResume();

    CARAPI Clear();

    CARAPI StartExitBackTransition(
        /* [in] */ IActivity* activity,
        /* [out] */ Boolean* result);

    CARAPI StartExitOutTransition(
        /* [in] */ IActivity* activity,
        /* [in] */ IBundle* options);

private:
    CARAPI_(void) StartEnter();

    CARAPI_(void) RestoreExitedViews();

private:

    static const String ENTERING_SHARED_ELEMENTS = "android:enteringSharedElements";

    static const String EXITING_MAPPED_FROM = "android:exitingMappedFrom";

    static const String EXITING_MAPPED_TO = "android:exitingMappedTo";

    /**
     * The shared elements that the calling Activity has said that they transferred to this
     * Activity.
     */
    ArrayList<String> mEnteringNames;

    /**
     * The names of shared elements that were shared to the called Activity.
     */
    ArrayList<String> mExitingFrom;

    /**
     * The names of local Views that were shared out, mapped to those elements in mExitingFrom.
     */
    ArrayList<String> mExitingTo;

    /**
     * The local Views that were shared out, mapped to those elements in mExitingFrom.
     */
    ArrayList<View> mExitingToView;

    /**
     * The ExitTransitionCoordinator used to start an Activity. Used to make the elements restore
     * Visibility of exited Views.
     */
    AutoPtr<IExitTransitionCoordinator> mCalledExitCoordinator;

    /**
     * The ExitTransitionCoordinator used to return to a previous Activity when called with
     * {@link android.app.Activity#finishAfterTransition()}.
     */
    AutoPtr<IExitTransitionCoordinator> mReturnExitCoordinator;

    /**
     * We must be able to cancel entering transitions to stop changing the Window to
     * opaque when we exit before making the Window opaque.
     */
    AutoPtr<IEnterTransitionCoordinator> mEnterTransitionCoordinator;

    /**
     * ActivityOptions used on entering this Activity.
     */
    AutoPtr<IActivityOptions> mEnterActivityOptions;

    /**
     * Has an exit transition been started? If so, we don't want to double-exit.
     */
    Boolean mHasExited;

    /**
     * Postpone painting and starting the enter transition until this is false.
     */
    Boolean mIsEnterPostponed;

    /**
     * Potential exit transition coordinators.
     */
    SparseArray<WeakReference<ExitTransitionCoordinator>> mExitTransitionCoordinators;

    /**
     * Next key for mExitTransitionCoordinator.
     */
    Int32 mExitTransitionCoordinatorsKey = 1;

    Boolean mIsEnterTriggered;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_ACTIVITY_TRANSITION_STATE_H__
