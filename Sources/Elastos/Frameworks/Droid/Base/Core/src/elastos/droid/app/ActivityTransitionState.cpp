
#include "elastos/droid/app/ActivityTransitionState.h"

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


CAR_INTERFACE_IMPL(ActivityTransitionState, Object, IActivityTransitionState)

ActivityTransitionState::ActivityTransitionState();

ActivityTransitionState::~ActivityTransitionState();

ECode ActivityTransitionState::constructor();

ECode ActivityTransitionState::AddExitTransitionCoordinator(
    /* [in] */ IExitTransitionCoordinator* exitTransitionCoordinator,
    /* [out] */ Int32* result)
{
    if (mExitTransitionCoordinators == null) {
        mExitTransitionCoordinators =
                new SparseArray<WeakReference<ExitTransitionCoordinator>>();
    }
    WeakReference<ExitTransitionCoordinator> ref = new WeakReference(exitTransitionCoordinator);
    // clean up old references:
    for (int i = mExitTransitionCoordinators.size() - 1; i >= 0; i--) {
        WeakReference<ExitTransitionCoordinator> oldRef
                = mExitTransitionCoordinators.valueAt(i);
        if (oldRef.get() == null) {
            mExitTransitionCoordinators.removeAt(i);
        }
    }
    int newKey = mExitTransitionCoordinatorsKey++;
    mExitTransitionCoordinators.append(newKey, ref);
    return newKey;
}

ECode ActivityTransitionState::ReadState(
    /* [in] */ IBundle* bundle);
{
    if (bundle != null) {
        if (mEnterTransitionCoordinator == null || mEnterTransitionCoordinator.isReturning()) {
            mEnteringNames = bundle.getStringArrayList(ENTERING_SHARED_ELEMENTS);
        }
        if (mEnterTransitionCoordinator == null) {
            mExitingFrom = bundle.getStringArrayList(EXITING_MAPPED_FROM);
            mExitingTo = bundle.getStringArrayList(EXITING_MAPPED_TO);
        }
    }
}

ECode ActivityTransitionState::SaveState(
    /* [in] */ IBundle* bundle);
{
    if (mEnteringNames != null) {
        bundle.putStringArrayList(ENTERING_SHARED_ELEMENTS, mEnteringNames);
    }
    if (mExitingFrom != null) {
        bundle.putStringArrayList(EXITING_MAPPED_FROM, mExitingFrom);
        bundle.putStringArrayList(EXITING_MAPPED_TO, mExitingTo);
    }
}

ECode ActivityTransitionState::SetEnterActivityOptions(
    /* [in] */ IActivity* activity,
    /* [in] */ IActivityOptions* options);
{
    if (activity.getWindow().hasFeature(Window.FEATURE_ACTIVITY_TRANSITIONS)
            && options != null && mEnterActivityOptions == null
            && mEnterTransitionCoordinator == null
            && options.getAnimationType() == ActivityOptions.ANIM_SCENE_TRANSITION) {
        mEnterActivityOptions = options;
        mIsEnterTriggered = false;
        if (mEnterActivityOptions.isReturning()) {
            restoreExitedViews();
            int result = mEnterActivityOptions.getResultCode();
            if (result != 0) {
                activity.onActivityReenter(result, mEnterActivityOptions.getResultData());
            }
        }
    }
}

ECode ActivityTransitionState::EnterReady(
    /* [in] */ IActivity* activity);
{
    if (mEnterActivityOptions == null || mIsEnterTriggered) {
        return;
    }
    mIsEnterTriggered = true;
    mHasExited = false;
    ArrayList<String> sharedElementNames = mEnterActivityOptions.getSharedElementNames();
    ResultReceiver resultReceiver = mEnterActivityOptions.getResultReceiver();
    if (mEnterActivityOptions.isReturning()) {
        restoreExitedViews();
        activity.getWindow().getDecorView().setVisibility(View.VISIBLE);
    }
    mEnterTransitionCoordinator = new EnterTransitionCoordinator(activity,
            resultReceiver, sharedElementNames, mEnterActivityOptions.isReturning());

    if (!mIsEnterPostponed) {
        startEnter();
    }
}

ECode ActivityTransitionState::PostponeEnterTransition()
{
    mIsEnterPostponed = true;
}

ECode ActivityTransitionState::StartPostponedEnterTransition()
{
    if (mIsEnterPostponed) {
        mIsEnterPostponed = false;
        if (mEnterTransitionCoordinator != null) {
            startEnter();
        }
    }
}

void ActivityTransitionState::StartEnter()
{
    if (mEnterActivityOptions.isReturning()) {
        if (mExitingToView != null) {
            mEnterTransitionCoordinator.viewInstancesReady(mExitingFrom, mExitingTo,
                    mExitingToView);
        } else {
            mEnterTransitionCoordinator.namedViewsReady(mExitingFrom, mExitingTo);
        }
    } else {
        mEnterTransitionCoordinator.namedViewsReady(null, null);
        mEnteringNames = mEnterTransitionCoordinator.getAllSharedElementNames();
    }

    mExitingFrom = null;
    mExitingTo = null;
    mExitingToView = null;
    mEnterActivityOptions = null;
}

ECode ActivityTransitionState::OnStop()
{
    restoreExitedViews();
    if (mEnterTransitionCoordinator != null) {
        mEnterTransitionCoordinator.stop();
        mEnterTransitionCoordinator = null;
    }
    if (mReturnExitCoordinator != null) {
        mReturnExitCoordinator.stop();
        mReturnExitCoordinator = null;
    }
}

ECode ActivityTransitionState::OnResume()
{
    restoreExitedViews();
}

ECode ActivityTransitionState::Clear()
{
    mEnteringNames = null;
    mExitingFrom = null;
    mExitingTo = null;
    mExitingToView = null;
    mCalledExitCoordinator = null;
    mEnterTransitionCoordinator = null;
    mEnterActivityOptions = null;
    mExitTransitionCoordinators = null;
}

void ActivityTransitionState::RestoreExitedViews() {
    if (mCalledExitCoordinator != null) {
        mCalledExitCoordinator.resetViews();
        mCalledExitCoordinator = null;
    }
}

ECode ActivityTransitionState::StartExitBackTransition(
    /* [in] */ IActivity* activity,
    /* [out] */ Boolean* result)
{
    if (mEnteringNames == null) {
        return false;
    } else {
        if (!mHasExited) {
            mHasExited = true;
            Transition enterViewsTransition = null;
            ViewGroup decor = null;
            if (mEnterTransitionCoordinator != null) {
                enterViewsTransition = mEnterTransitionCoordinator.getEnterViewsTransition();
                decor = mEnterTransitionCoordinator.getDecor();
                mEnterTransitionCoordinator.cancelEnter();
                mEnterTransitionCoordinator = null;
                if (enterViewsTransition != null && decor != null) {
                    enterViewsTransition.pause(decor);
                }
            }

            mReturnExitCoordinator =
                    new ExitTransitionCoordinator(activity, mEnteringNames, null, null, true);
            if (enterViewsTransition != null && decor != null) {
                enterViewsTransition.resume(decor);
            }
            mReturnExitCoordinator.startExit(activity.mResultCode, activity.mResultData);
        }
        return true;
    }
}

ECode ActivityTransitionState::StartExitOutTransition(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* options)
{
    if (!activity.getWindow().hasFeature(Window.FEATURE_ACTIVITY_TRANSITIONS)) {
        return;
    }
    ActivityOptions activityOptions = new ActivityOptions(options);
    mEnterTransitionCoordinator = null;
    if (activityOptions.getAnimationType() == ActivityOptions.ANIM_SCENE_TRANSITION) {
        int key = activityOptions.getExitCoordinatorKey();
        int index = mExitTransitionCoordinators.indexOfKey(key);
        if (index >= 0) {
            mCalledExitCoordinator = mExitTransitionCoordinators.valueAt(index).get();
            mExitTransitionCoordinators.removeAt(index);
            if (mCalledExitCoordinator != null) {
                mExitingFrom = mCalledExitCoordinator.getAcceptedNames();
                mExitingTo = mCalledExitCoordinator.getMappedNames();
                mExitingToView = mCalledExitCoordinator.copyMappedViews();
                mCalledExitCoordinator.startExit();
            }
        }
    }
}



} // namespace App
} // namespace Droid
} // namespace Elastos
