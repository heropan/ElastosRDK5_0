#ifndef __CEXPANDHELPER_H__
#define __CEXPANDHELPER_H__

#include "_CExpandHelper.h"
#include "systemui/CViewScaller.h"
#include "view/SimpleOnScaleGestureListener.h"
#include "animation/AnimatorListenerAdapter.h"

using Elastos::Droid::Os::IVibrator;
using Elastos::Droid::Animation::IAnimatorSet;
using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::Animation::AnimatorListenerAdapter;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IScaleGestureDetector;
using Elastos::Droid::View::IOnScaleGestureListener;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::SimpleOnScaleGestureListener;
using Elastos::Droid::SystemUI::IExpandHelperCallback;

namespace Elastos {
namespace Droid {
namespace SystemUI {

CarClass(CExpandHelper)
{
private:
    class AnimatorListener : public AnimatorListenerAdapter
    {
    public:
        virtual CARAPI OnAnimationStart(
            /* [in] */ IAnimator* animation);

        virtual CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);
    };

    class ExpandScaleGestureListener
        : public SimpleOnScaleGestureListener
    {
    public:
        ExpandScaleGestureListener(
            /* [in] */ CExpandHelper* host);

        virtual CARAPI OnScale(
            /* [in ]*/ IScaleGestureDetector* detector,
            /* [out] */ Boolean* res);

        virtual CARAPI OnScaleBegin(
            /* [in ]*/ IScaleGestureDetector* detector,
            /* [out] */ Boolean* res);

        virtual CARAPI OnScaleEnd(
            /* [in ]*/ IScaleGestureDetector* detector);
    private:
        CExpandHelper* mHost;
    };

public:
    CExpandHelper();

    /**
     * Handle expansion gestures to expand and contract children of the callback.
     *
     * @param context application context
     * @param callback the container that holds the items to be manipulated
     * @param small the smallest allowable size for the manuipulated items.
     * @param large the largest allowable size for the manuipulated items.
     * @param scoller if non-null also manipulate the scroll position to obey the gravity.
     */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IExpandHelperCallback* callback,
        /* [in] */ Int32 small,
        /* [in] */ Int32 large);

    CARAPI SetEventSource(
        /* [in] */ IView* eventSource);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetScrollView(
        /* [in] */ IView* scrollView);

    CARAPI SetGlow(
        /* [in] */ Float glow);

    CARAPI HandleGlowVisibility();

    CARAPI OnClick(
        /* [in] */ IView* v);

    CARAPI OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* result);

    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* result);

    /**
     * Use this to abort any pending expansions in progress.
     */
     CARAPI Cancel();

protected:
    Float CalculateGlow(
        /* [in] */ Float target,
        /* [in] */ Float actual);

private:
    void UpdateExpansion();

    Float Clamp(
        /* [in] */ Float target);

    AutoPtr<IView> FindView(
        /* [in] */ Float x,
        /* [in] */ Float y);

    Boolean IsInside(
        /* [in] */ IView* v,
        /* [in] */ Float x,
        /* [in] */ Float y);

    void StartExpanding(
        /* [in] */ IView* v,
        /* [in] */ Int32 expandType);

    void FinishExpanding(
        /* [in] */ Boolean force);

    void ClearView();

    void SetView(
        /* [in] */ IView* v);

    /**
     * Triggers haptic feedback.
     */
    void Vibrate(
         /* [in] */ Int64 duration);

protected:
    static const Boolean DEBUG;// = FALSE;
    static const Boolean DEBUG_SCALE;// = FALSE;
    static const Boolean DEBUG_GLOW;// = FALSE;

private:
    static const String TAG;// = "ExpandHelper";
    static const Int64 EXPAND_DURATION;// = 250;
    static const Int64 GLOW_DURATION;// = 150;

    // Set to FALSE to disable focus-based gestures (spread-finger vertical pull).
    static const Boolean USE_DRAG;// = TRUE;
    // Set to FALSE to disable scale-based gestures (both horizontal and vertical).
    static const Boolean USE_SPAN;// = TRUE;
    // Both gestures types may be active at the same time.
    // At least one gesture type should be active.
    // A variant of the screwdriver gesture will emerge from either gesture type.

    // amount of overstretch for maximum brightness expressed in U
    // 2f: maximum brightness is stretching a 1U to 3U, or a 4U to 6U
    static const Float STRETCH_INTERVAL;// = 2.0f;

    // level of glow for a touch, without overstretch
    // overstretch fills the range (GLOW_BASE, 1.0]
    static const Float GLOW_BASE;// = 0.5f;

    // @SuppressWarnings("unused")
    AutoPtr<IContext> mContext;

    Boolean mExpanding;
    static const Int32 NONE;//    = 0;
    static const Int32 BLINDS;//  = 1<<0;
    static const Int32 PULL;//    = 1<<1;
    static const Int32 STRETCH;// = 1<<2;
    Int32 mExpansionStyle;// = NONE;
    Boolean mWatchingForPull;
    Boolean mHasPopped;
    AutoPtr<IView> mEventSource;
    AutoPtr<IView> mCurrView;
    AutoPtr<IView> mCurrViewTopGlow;
    AutoPtr<IView> mCurrViewBottomGlow;
    Float mOldHeight;
    Float mNaturalHeight;
    Float mInitialTouchFocusY;
    Float mInitialTouchY;
    Float mInitialTouchSpan;
    Float mLastFocusY;
    Float mLastSpanY;
    Int32 mTouchSlop;
    Int32 mLastMotionY;
    Float mPopLimit;
    Int32 mPopDuration;
    Float mPullGestureMinXSpan;
    AutoPtr<IExpandHelperCallback> mCallback;
    AutoPtr<IScaleGestureDetector> mSGD;
    AutoPtr<ExpandScaleGestureListener> mScaleGestureListener;
    AutoPtr<IViewScaller> mScaler;
    AutoPtr<IObjectAnimator> mScaleAnimation;
    AutoPtr<IAnimatorSet> mGlowAnimationSet;
    AutoPtr<IObjectAnimator> mGlowTopAnimation;
    AutoPtr<IObjectAnimator> mGlowBottomAnimation;
    AutoPtr<IVibrator> mVibrator;

    Int32 mSmallSize;
    Int32 mLargeSize;
    Float mMaximumStretch;

    Int32 mGravity;

    AutoPtr<IView> mScrollView;

    Object mVibrateLock;
};

}// namespace SystemUI
}// namespace Droid
}// namespace Elastos


#endif //__CEXPANDHELPER_H__
