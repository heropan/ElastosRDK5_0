
#include "elastos/droid/app/ExitTransitionCoordinator.h"
#include "elastos/droid/app/Activity.h"

using Elastos::Droid::Animation::IAnimator;
using Elastos::Droid::Animation::IAnimatorListenerAdapter;
using Elastos::Droid::Graphics::IColor;
using Elastos::Droid::Graphics::IMatrix;
using Elastos::Droid::Graphics::IRectF;
using Elastos::Droid::Graphics::Drawable::IColorDrawable;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Transition::ITransition;
using Elastos::Droid::Transition::ITransitionManager;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewTreeObserver;

namespace Elastos {
namespace Droid {
namespace App {

const String ExitTransitionCoordinator::TAG("ExitTransitionCoordinator");
const Int64 ExitTransitionCoordinator::MAX_WAIT_MS = 1000;

CAR_INTERFACE_IMPL(ExitTransitionCoordinator, ActivityTransitionCoordinator, IExitTransitionCoordinator)

ExitTransitionCoordinator::ExitTransitionCoordinator()
    : mExitComplete(FALSE)
    , mExitNotified(FALSE)
    , mSharedElementNotified(FALSE)
    , mIsBackgroundReady(FALSE)
    , mIsCanceled(FALSE)
    , mIsHidden(FALSE)
    , mIsExitStarted(FALSE)
    , mSharedElementsHidden(FALSE)
{}

ExitTransitionCoordinator::~ExitTransitionCoordinator()
{}

ECode ExitTransitionCoordinator::constructor(
    /* [in] */ IActivity* activity,
    /* [in] */ IArrayList* names, //ArrayList<String>
    /* [in] */ IArrayList* accepted, //ArrayList<String>
    /* [in] */ IArrayList* mapped,//ArrayList<View>
    /* [in] */ Boolean isReturning)
{
    AutoPtr<IWindow> window;
    GetWindow((IWindow**)&window);
    FAIL_RETURN(ActivityTransitionCoordinator::(
        window, names, GetListener(activity, isReturning), isReturning);
    ViewsReady(MapSharedElements(accepted, mapped));
    StripOffscreenViews();
    mIsBackgroundReady = !isReturning;
    mActivity = activity;
    return NOERROR;
}

AutoPtr<ISharedElementCallback> ExitTransitionCoordinator::GetListener(
    /* [in] */ IActivity* activity,
    /* [in] */ Boolean isReturning)
{
    Activity* act = (Activity*)activity;
    return isReturning ? act->mEnterTransitionListener : act->mExitTransitionListener;
}

void ExitTransitionCoordinator::OnReceiveResult(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData)
{
    switch (resultCode) {
        case IActivityTransitionCoordinator::MSG_SET_REMOTE_RECEIVER:
            StopCancel();
            mResultReceiver = resultData.getParcelable(KEY_REMOTE_RECEIVER);
            if (mIsCanceled) {
                mResultReceiver.send(IActivityTransitionCoordinator::MSG_CANCEL, null);
                mResultReceiver = null;
            } else {
                NotifyComplete();
            }
            break;
        case IActivityTransitionCoordinator::MSG_HIDE_SHARED_ELEMENTS:
            StopCancel();
            if (!mIsCanceled) {
                hideSharedElements();
            }
            break;
        case IActivityTransitionCoordinator::MSG_START_EXIT_TRANSITION:
            mHandler.removeMessages(IActivityTransitionCoordinator::MSG_CANCEL);
            startExit();
            break;
        case IActivityTransitionCoordinator::MSG_SHARED_ELEMENT_DESTINATION:
            mExitSharedElementBundle = resultData;
            sharedElementExitBack();
            break;
    }
}

void ExitTransitionCoordinator::StopCancel()
{
    if (mHandler != null) {
        mHandler.removeMessages(IActivityTransitionCoordinator::MSG_CANCEL);
    }
}

void ExitTransitionCoordinator::DelayCancel()
{
    if (mHandler != null) {
        mHandler.sendEmptyMessageDelayed(IActivityTransitionCoordinator::MSG_CANCEL, MAX_WAIT_MS);
    }
}

ECode ExitTransitionCoordinator::ResetViews()
{
    if (mTransitioningViews != null) {
        showViews(mTransitioningViews, true);
    }
    showViews(mSharedElements, true);
    mIsHidden = true;
    ViewGroup decorView = getDecor();
    if (!mIsReturning && decorView != null) {
        decorView.suppressLayout(false);
    }
    moveSharedElementsFromOverlay();
    clearState();
}

void ExitTransitionCoordinator::SharedElementExitBack()
{
    final ViewGroup decorView = getDecor();
    if (decorView != null) {
        decorView.suppressLayout(true);
    }
    if (decorView != null && mExitSharedElementBundle != null &&
            !mExitSharedElementBundle.isEmpty() &&
            !mSharedElements.isEmpty() && getSharedElementTransition() != null) {
        startTransition(new Runnable() {
            ECode ExitTransitionCoordinator::run() {
                startSharedElementExit(decorView);
            }
        });
    } else {
        sharedElementTransitionComplete();
    }
}

void ExitTransitionCoordinator::StartSharedElementExit(
    /* [in] */ IViewGroup* decorView)
{
    Transition transition = getSharedElementExitTransition();
    transition.addListener(new Transition.TransitionListenerAdapter() {
        @Override
        ECode ExitTransitionCoordinator::onTransitionEnd(Transition transition) {
            transition.removeListener(this);
            if (mExitComplete) {
                delayCancel();
            }
        }
    });
    final ArrayList<View> sharedElementSnapshots = createSnapshots(mExitSharedElementBundle,
            mSharedElementNames);
    decorView.getViewTreeObserver()
            .addOnPreDrawListener(new ViewTreeObserver.OnPreDrawListener() {
                @Override
                public Boolean onPreDraw() {
                    decorView.getViewTreeObserver().removeOnPreDrawListener(this);
                    setSharedElementState(mExitSharedElementBundle, sharedElementSnapshots);
                    return true;
                }
            });
    setGhostVisibility(View.INVISIBLE);
    scheduleGhostVisibilityChange(View.INVISIBLE);
    if (mListener != null) {
        mListener.onSharedElementEnd(mSharedElementNames, mSharedElements,
                sharedElementSnapshots);
    }
    TransitionManager.beginDelayedTransition(decorView, transition);
    scheduleGhostVisibilityChange(View.VISIBLE);
    setGhostVisibility(View.VISIBLE);
    decorView.invalidate();
}

void ExitTransitionCoordinator::HideSharedElements()
{
    moveSharedElementsFromOverlay();
    if (!mIsHidden) {
        hideViews(mSharedElements);
    }
    mSharedElementsHidden = true;
    finishIfNecessary();
}

ECode ExitTransitionCoordinator::StartExit()
{
    if (!mIsExitStarted) {
        mIsExitStarted = true;
        ViewGroup decorView = getDecor();
        if (decorView != null) {
            decorView.suppressLayout(true);
        }
        moveSharedElementsToOverlay();
        startTransition(new Runnable() {
            @Override
            ECode ExitTransitionCoordinator::run() {
                beginTransitions();
            }
        });
    }
}

ECode ExitTransitionCoordinator::StartExit(
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    if (!mIsExitStarted) {
        mIsExitStarted = true;
        ViewGroup decorView = getDecor();
        if (decorView != null) {
            decorView.suppressLayout(true);
        }
        mHandler = new Handler() {
            @Override
            ECode ExitTransitionCoordinator::handleMessage(Message msg) {
                mIsCanceled = true;
                finish();
            }
        };
        delayCancel();
        moveSharedElementsToOverlay();
        if (decorView != null && decorView.getBackground() == null) {
            getWindow().setBackgroundDrawable(new ColorDrawable(Color.BLACK));
        }
        ActivityOptions options = ActivityOptions.makeSceneTransitionAnimation(mActivity, this,
                mAllSharedElementNames, resultCode, data);
        mActivity.convertToTranslucent(new Activity.TranslucentConversionListener() {
            @Override
            ECode ExitTransitionCoordinator::onTranslucentConversionComplete(Boolean drawComplete) {
                if (!mIsCanceled) {
                    fadeOutBackground();
                }
            }
        }, options);
        startTransition(new Runnable() {
            @Override
            ECode ExitTransitionCoordinator::run() {
                startExitTransition();
            }
        });
    }
}

ECode ExitTransitionCoordinator::Stop()
{
    if (mIsReturning && mActivity != null) {
        // Override the previous ActivityOptions. We don't want the
        // activity to have options since we're essentially canceling the
        // transition and finishing right now.
        mActivity.convertToTranslucent(null, null);
        finish();
    }
}

void ExitTransitionCoordinator::StartExitTransition()
{
    Transition transition = getExitTransition();
    ViewGroup decorView = getDecor();
    if (transition != null && decorView != null && mTransitioningViews != null) {
        TransitionManager.beginDelayedTransition(decorView, transition);
        mTransitioningViews.get(0).invalidate();
    } else {
        transitionStarted();
    }
}

void ExitTransitionCoordinator::FadeOutBackground()
{
    if (mBackgroundAnimator == null) {
        ViewGroup decor = getDecor();
        Drawable background;
        if (decor != null && (background = decor.getBackground()) != null) {
            background = background.mutate();
            getWindow().setBackgroundDrawable(background);
            mBackgroundAnimator = ObjectAnimator.ofInt(background, "alpha", 0);
            mBackgroundAnimator.addListener(new AnimatorListenerAdapter() {
                @Override
                ECode ExitTransitionCoordinator::onAnimationEnd(Animator animation) {
                    mBackgroundAnimator = null;
                    if (!mIsCanceled) {
                        mIsBackgroundReady = true;
                        NotifyComplete();
                    }
                }
            });
            mBackgroundAnimator.setDuration(getFadeDuration());
            mBackgroundAnimator.start();
        } else {
            mIsBackgroundReady = true;
        }
    }
}

AutoPtr<ITransition> ExitTransitionCoordinator::GetExitTransition()
{
    Transition viewsTransition = null;
    if (mTransitioningViews != null && !mTransitioningViews.isEmpty()) {
        viewsTransition = configureTransition(getViewsTransition(), true);
    }
    if (viewsTransition == null) {
        exitTransitionComplete();
    } else {
        final ArrayList<View> transitioningViews = mTransitioningViews;
        viewsTransition.addListener(new ContinueTransitionListener() {
            @Override
            ECode ExitTransitionCoordinator::onTransitionEnd(Transition transition) {
                transition.removeListener(this);
                exitTransitionComplete();
                if (mIsHidden && transitioningViews != null) {
                    showViews(transitioningViews, true);
                }
                if (mSharedElementBundle != null) {
                    delayCancel();
                }
                super.onTransitionEnd(transition);
            }
        });
        viewsTransition.forceVisibility(View.INVISIBLE, false);
    }
    return viewsTransition;
}

AutoPtr<ITransition> ExitTransitionCoordinator::GetSharedElementExitTransition()
{
    Transition sharedElementTransition = null;
    if (!mSharedElements.isEmpty()) {
        sharedElementTransition = configureTransition(getSharedElementTransition(), false);
    }
    if (sharedElementTransition == null) {
        sharedElementTransitionComplete();
    } else {
        sharedElementTransition.addListener(new ContinueTransitionListener() {
            @Override
            ECode ExitTransitionCoordinator::onTransitionEnd(Transition transition) {
                transition.removeListener(this);
                sharedElementTransitionComplete();
                if (mIsHidden) {
                    showViews(mSharedElements, true);
                }
            }
        });
        mSharedElements.get(0).invalidate();
    }
    return sharedElementTransition;
}

void ExitTransitionCoordinator::BeginTransitions()
{
    Transition sharedElementTransition = getSharedElementExitTransition();
    Transition viewsTransition = getExitTransition();

    Transition transition = mergeTransitions(sharedElementTransition, viewsTransition);
    ViewGroup decorView = getDecor();
    if (transition != null && decorView != null) {
        setGhostVisibility(View.INVISIBLE);
        scheduleGhostVisibilityChange(View.INVISIBLE);
        TransitionManager.beginDelayedTransition(decorView, transition);
        scheduleGhostVisibilityChange(View.VISIBLE);
        setGhostVisibility(View.VISIBLE);
        decorView.invalidate();
    } else {
        transitionStarted();
    }
}

void ExitTransitionCoordinator::ExitTransitionComplete()
{
    mExitComplete = true;
    NotifyComplete();
}

Boolean ExitTransitionCoordinator::IsReadyToNotify()
{
    return mSharedElementBundle != null && mResultReceiver != null && mIsBackgroundReady;
}

void ExitTransitionCoordinator::SharedElementTransitionComplete()
{
    mSharedElementBundle = mExitSharedElementBundle == null
            ? captureSharedElementState() : captureExitSharedElementsState();
    NotifyComplete();
}

AutoPtr<IBundle> ExitTransitionCoordinator::CaptureExitSharedElementsState()
{
    Bundle bundle = new Bundle();
    RectF bounds = new RectF();
    Matrix matrix = new Matrix();
    for (int i = 0; i < mSharedElements.size(); i++) {
        String name = mSharedElementNames.get(i);
        Bundle sharedElementState = mExitSharedElementBundle.getBundle(name);
        if (sharedElementState != null) {
            bundle.putBundle(name, sharedElementState);
        } else {
            View view = mSharedElements.get(i);
            captureSharedElementState(view, name, bundle, matrix, bounds);
        }
    }
    return bundle;
}

void ExitTransitionCoordinator::NotifyComplete()
{
    if (isReadyToNotify()) {
        if (!mSharedElementNotified) {
            mSharedElementNotified = true;
            delayCancel();
            mResultReceiver.send(IActivityTransitionCoordinator::MSG_TAKE_SHARED_ELEMENTS, mSharedElementBundle);
        }
        if (!mExitNotified && mExitComplete) {
            mExitNotified = true;
            mResultReceiver.send(IActivityTransitionCoordinator::MSG_EXIT_TRANSITION_COMPLETE, null);
            mResultReceiver = null; // done talking
            ViewGroup decorView = getDecor();
            if (!mIsReturning && decorView != null) {
                decorView.suppressLayout(false);
            }
            finishIfNecessary();
        }
    }
}

void ExitTransitionCoordinator::FinishIfNecessary()
{
    if (mIsReturning && mExitNotified && mActivity != null && (mSharedElements.isEmpty() ||
            mSharedElementsHidden)) {
        finish();
    }
    if (!mIsReturning && mExitNotified) {
        mActivity = null; // don't need it anymore
    }
}

void ExitTransitionCoordinator::Finish()
{
    StopCancel();
    if (mActivity != null) {
        mActivity.mActivityTransitionState.clear();
        mActivity.finish();
        mActivity.overridePendingTransition(0, 0);
        mActivity = null;
    }
    // Clear the state so that we can't hold any references accidentally and leak memory.
    mHandler = null;
    mSharedElementBundle = null;
    if (mBackgroundAnimator != null) {
        mBackgroundAnimator.cancel();
        mBackgroundAnimator = null;
    }
    mExitSharedElementBundle = null;
    clearState();
}

Boolean ExitTransitionCoordinator::MoveSharedElementWithParent()
{
    return !mIsReturning;
}

AutoPtr<ITransition> ExitTransitionCoordinator::GetViewsTransition()
{
    if (mIsReturning) {
        return getWindow().getReturnTransition();
    } else {
        return getWindow().getExitTransition();
    }
}

AutoPtr<ITransition> ExitTransitionCoordinator::GetSharedElementTransition()
{
    if (mIsReturning) {
        return getWindow().getSharedElementReturnTransition();
    } else {
        return getWindow().getSharedElementExitTransition();
    }
}

} // namespace App
} // namespace Droid
} // namespace Elastos
