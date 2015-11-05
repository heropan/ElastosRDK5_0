
#include "elastos/droid/app/ExitTransitionCoordinator.h"

namespace Elastos {
namespace Droid {
namespace App {


// using Elastos::Droid::animation.Animator;
// using Elastos::Droid::animation.AnimatorListenerAdapter;
// using Elastos::Droid::animation.ObjectAnimator;
// using Elastos::Droid::content.Intent;
// using Elastos::Droid::graphics.Color;
// using Elastos::Droid::graphics.Matrix;
// using Elastos::Droid::graphics.RectF;
// using Elastos::Droid::graphics.drawable.ColorDrawable;
// using Elastos::Droid::graphics.drawable.Drawable;
// using Elastos::Droid::os.Bundle;
// using Elastos::Droid::os.Handler;
// using Elastos::Droid::os.Message;
// using Elastos::Droid::transition.Transition;
// using Elastos::Droid::transition.TransitionManager;
// using Elastos::Droid::view.View;
// using Elastos::Droid::view.ViewGroup;
// using Elastos::Droid::view.ViewTreeObserver;

// import java.util.ArrayList;

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
        /* [in] */ Boolean isReturning)
    {
        super(activity.getWindow(), names, getListener(activity, isReturning), isReturning);
        viewsReady(mapSharedElements(accepted, mapped));
        stripOffscreenViews();
        mIsBackgroundReady = !isReturning;
        mActivity = activity;
    }

    private static AutoPtr<ISharedElementCallback> GetListener(
        /* [in] */ IActivity* activity,
        /* [in] */ Boolean isReturning)
    {
        return isReturning ? activity.mEnterTransitionListener : activity.mExitTransitionListener;
    }

    @Override
    protected void OnReceiveResult(
        /* [in] */ Int32 resultCode,
        /* [in] */ IBundle* resultData)
    {
        switch (resultCode) {
            case MSG_SET_REMOTE_RECEIVER:
                stopCancel();
                mResultReceiver = resultData.getParcelable(KEY_REMOTE_RECEIVER);
                if (mIsCanceled) {
                    mResultReceiver.send(MSG_CANCEL, null);
                    mResultReceiver = null;
                } else {
                    notifyComplete();
                }
                break;
            case MSG_HIDE_SHARED_ELEMENTS:
                stopCancel();
                if (!mIsCanceled) {
                    hideSharedElements();
                }
                break;
            case MSG_START_EXIT_TRANSITION:
                mHandler.removeMessages(MSG_CANCEL);
                startExit();
                break;
            case MSG_SHARED_ELEMENT_DESTINATION:
                mExitSharedElementBundle = resultData;
                sharedElementExitBack();
                break;
        }
    }

    private void StopCancel()
    {
        if (mHandler != null) {
            mHandler.removeMessages(MSG_CANCEL);
        }
    }

    private void DelayCancel()
    {
        if (mHandler != null) {
            mHandler.sendEmptyMessageDelayed(MSG_CANCEL, MAX_WAIT_MS);
        }
    }

    CARAPI ResetViews()
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

    private void SharedElementExitBack()
    {
        final ViewGroup decorView = getDecor();
        if (decorView != null) {
            decorView.suppressLayout(true);
        }
        if (decorView != null && mExitSharedElementBundle != null &&
                !mExitSharedElementBundle.isEmpty() &&
                !mSharedElements.isEmpty() && getSharedElementTransition() != null) {
            startTransition(new Runnable() {
                CARAPI run() {
                    startSharedElementExit(decorView);
                }
            });
        } else {
            sharedElementTransitionComplete();
        }
    }

    private void StartSharedElementExit(
        /* [in] */ IViewGroup* decorView)
    {
        Transition transition = getSharedElementExitTransition();
        transition.addListener(new Transition.TransitionListenerAdapter() {
            @Override
            CARAPI onTransitionEnd(Transition transition) {
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

    private void HideSharedElements()
    {
        moveSharedElementsFromOverlay();
        if (!mIsHidden) {
            hideViews(mSharedElements);
        }
        mSharedElementsHidden = true;
        finishIfNecessary();
    }

    CARAPI StartExit()
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
                CARAPI run() {
                    beginTransitions();
                }
            });
        }
    }

    CARAPI StartExit(
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
                CARAPI handleMessage(Message msg) {
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
                CARAPI onTranslucentConversionComplete(Boolean drawComplete) {
                    if (!mIsCanceled) {
                        fadeOutBackground();
                    }
                }
            }, options);
            startTransition(new Runnable() {
                @Override
                CARAPI run() {
                    startExitTransition();
                }
            });
        }
    }

    CARAPI Stop()
    {
        if (mIsReturning && mActivity != null) {
            // Override the previous ActivityOptions. We don't want the
            // activity to have options since we're essentially canceling the
            // transition and finishing right now.
            mActivity.convertToTranslucent(null, null);
            finish();
        }
    }

    private void StartExitTransition()
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

    private void FadeOutBackground()
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
                    CARAPI onAnimationEnd(Animator animation) {
                        mBackgroundAnimator = null;
                        if (!mIsCanceled) {
                            mIsBackgroundReady = true;
                            notifyComplete();
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

    private AutoPtr<ITransition> GetExitTransition()
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
                CARAPI onTransitionEnd(Transition transition) {
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

    private AutoPtr<ITransition> GetSharedElementExitTransition()
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
                CARAPI onTransitionEnd(Transition transition) {
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

    private void BeginTransitions()
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

    private void ExitTransitionComplete()
    {
        mExitComplete = true;
        notifyComplete();
    }

    protected Boolean IsReadyToNotify()
    {
        return mSharedElementBundle != null && mResultReceiver != null && mIsBackgroundReady;
    }

    private void SharedElementTransitionComplete()
    {
        mSharedElementBundle = mExitSharedElementBundle == null
                ? captureSharedElementState() : captureExitSharedElementsState();
        notifyComplete();
    }

    private AutoPtr<IBundle> CaptureExitSharedElementsState()
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

    protected void NotifyComplete()
    {
        if (isReadyToNotify()) {
            if (!mSharedElementNotified) {
                mSharedElementNotified = true;
                delayCancel();
                mResultReceiver.send(MSG_TAKE_SHARED_ELEMENTS, mSharedElementBundle);
            }
            if (!mExitNotified && mExitComplete) {
                mExitNotified = true;
                mResultReceiver.send(MSG_EXIT_TRANSITION_COMPLETE, null);
                mResultReceiver = null; // done talking
                ViewGroup decorView = getDecor();
                if (!mIsReturning && decorView != null) {
                    decorView.suppressLayout(false);
                }
                finishIfNecessary();
            }
        }
    }

    private void FinishIfNecessary()
    {
        if (mIsReturning && mExitNotified && mActivity != null && (mSharedElements.isEmpty() ||
                mSharedElementsHidden)) {
            finish();
        }
        if (!mIsReturning && mExitNotified) {
            mActivity = null; // don't need it anymore
        }
    }

    private void Finish()
    {
        stopCancel();
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

    @Override
    protected Boolean MoveSharedElementWithParent()
    {
        return !mIsReturning;
    }

    @Override
    protected CARAPI_(AutoPtr<ITransition>) GetViewsTransition()
    {
        if (mIsReturning) {
            return getWindow().getReturnTransition();
        } else {
            return getWindow().getExitTransition();
        }
    }

    protected CARAPI_(AutoPtr<ITransition>) GetSharedElementTransition()
    {
        if (mIsReturning) {
            return getWindow().getSharedElementReturnTransition();
        } else {
            return getWindow().getSharedElementExitTransition();
        }
    }

private:
    private static final String TAG = "ExitTransitionCoordinator";
    private static final long MAX_WAIT_MS = 1000;

    private Boolean mExitComplete;

    private Bundle mSharedElementBundle;

    private Boolean mExitNotified;

    private Boolean mSharedElementNotified;

    private Activity mActivity;

    private Boolean mIsBackgroundReady;

    private Boolean mIsCanceled;

    private Handler mHandler;

    private ObjectAnimator mBackgroundAnimator;

    private Boolean mIsHidden;

    private Bundle mExitSharedElementBundle;

    private Boolean mIsExitStarted;

    private Boolean mSharedElementsHidden;
};

} // namespace App
} // namespace Droid
} // namespace Elastos
