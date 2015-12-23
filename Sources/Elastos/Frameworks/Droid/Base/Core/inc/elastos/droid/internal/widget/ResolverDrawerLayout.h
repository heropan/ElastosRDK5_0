
#ifndef __ELASTOS_DROID_INTERNAL_WIDGET_RESOLVERDRAWERLAYOUT_H__
#define __ELASTOS_DROID_INTERNAL_WIDGET_RESOLVERDRAWERLAYOUT_H__

#include "elastos/droid/view/ViewGroup.h"

using Elastos::Droid::View::ViewGroup;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IVelocityTracker;
using Elastos::Droid::Widget::IOverScroller;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Widget {

class ResolverDrawerLayout
    : public ViewGroup
    , public IResolverDrawerLayout
{
public:
    class ResolverDrawerLayoutLayoutParams
        : public ViewGroup::MarginLayoutParams
        , public IResolverDrawerLayoutLayoutParams
    {
    public:
        CAR_INTERFACE_DECL()

        ResolverDrawerLayoutLayoutParams(
            /* [in] */ IContext* c,
            /* [in] */ IAttributeSet* attrs);

        ResolverDrawerLayoutLayoutParams(
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        ResolverDrawerLayoutLayoutParams(
            /* [in] */ IResolverDrawerLayoutLayoutParams* source);

        ResolverDrawerLayoutLayoutParams(
            /* [in] */ IViewGroupMarginLayoutParams* source);

        ResolverDrawerLayoutLayoutParams(
            /* [in] */ IViewGroupLayoutParams* source);

    public:
        Boolean mAlwaysShow;
        Boolean mIgnoreOffset;
    };

    class SavedState
        : public BaseSavedState
    {
    public:
        SavedState(
            /* [in] */ IParcelable* superState);

        CARAPI ReadFromParcel(
            /* [in] */ IParcel* in);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* out);

    public:
        Boolean mOpen;
    };

public:
    CAR_INTERFACE_DECL()

    ResolverDrawerLayout();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr);

    CARAPI SetSmallCollapsed(
        /* [in] */ Boolean smallCollapsed);

    CARAPI IsSmallCollapsed(
        /* [out] */ Boolean* result);

    CARAPI IsCollapsed(
        /* [out] */ Boolean* result);

    CARAPI SetOnClickOutsideListener(
        /* [in] */ IViewOnClickListener* listener);

    CARAPI OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* result);

    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* result);

    CARAPI ComputeScroll();

    CARAPI RequestChildFocus(
        /* [in] */ IView* child,
        /* [in] */ IView* focused);

    CARAPI OnStartNestedScroll(
        /* [in] */ IView* child,
        /* [in] */ IView* target,
        /* [in] */ Int32 nestedScrollAxes,
        /* [out] */ Boolean* result);

    CARAPI OnNestedScrollAccepted(
        /* [in] */ IView* child,
        /* [in] */ IView* target,
        /* [in] */ Int32 axes);

    CARAPI OnStopNestedScroll(
        /* [in] */ IView* child);

    CARAPI OnNestedScroll(
        /* [in] */ IView* target,
        /* [in] */ Int32 dxConsumed,
        /* [in] */ Int32 dyConsumed,
        /* [in] */ Int32 dxUnconsumed,
        /* [in] */ Int32 dyUnconsumed);

    CARAPI OnNestedPreScroll(
        /* [in] */ IView* target,
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy,
        /* [in] */ ArrayOf<Int32>* consumed);

    CARAPI OnNestedPreFling(
        /* [in] */ View target,
        /* [in] */ Float velocityX,
        /* [in] */ Float velocityY,
        /* [out] */ Boolean* result);

    CARAPI OnNestedFling(
        /* [in] */ IView* target,
        /* [in] */ Float velocityX,
        /* [in] */ Float velocityY,
        /* [in] */ Boolean consumed,
        /* [out] */ Boolean* result);

    CARAPI GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IViewGroupLayoutParams** result);

protected:
    CARAPI OnAttachedToWindow();

    CARAPI OnDetachedFromWindow();

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI GenerateLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p,
        /* [out] */ IViewGroupLayoutParams** result);

    CARAPI GenerateDefaultLayoutParams(
        /* [out] */ IViewGroupLayoutParams** result);

    CARAPI OnSaveInstanceState(
        /* [out] */ IParcelable** result);

    CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

private:
    CARAPI_(Boolean) IsMoving();

    CARAPI_(Int32) GetMaxCollapsedHeight();

    CARAPI_(void) OnSecondaryPointerUp(
        /* [in] */ IMotionEvent* ev);

    CARAPI_(void) ResetTouch();

    CARAPI_(Float) PerformDrag(
        /* [in] */ Float dy);

    CARAPI_(void) SmoothScrollTo(
        /* [in] */ Int32 yOffset,
        /* [in] */ Float velocity);

    CARAPI_(Float) DistanceInfluenceForSnapDuration(
        /* [in] */ Float f);

    /**
     * Note: this method doesn't take Z into account for overlapping views
     * since it is only used in contexts where this doesn't affect the outcome.
     */
    CARAPI_(AutoPtr<IView>) FindChildUnder(
        /* [in] */ Float x,
        /* [in] */ Float y);

    static CARAPI_(AutoPtr<IView>) FindChildUnder(
        /* [in] */ IViewGroup* parent,
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI_(AutoPtr<IView>) FindListChildUnder(
        /* [in] */ Float x,
        /* [in] */ Float y);

    /**
     * This only checks clipping along the bottom edge.
     */
    CARAPI_(Boolean) IsListChildUnderClipped(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI_(Boolean) IsDescendantClipped(
        /* [in] */ IView* child);

    static CARAPI_(Boolean) IsChildUnder(
        /* [in] */ IView* child,
        /* [in] */ Float x,
        /* [in] */ Float y);

private:
    static const String TAG;

    /**
     * Max width of the whole drawer layout
     */
    Int32 mMaxWidth;

    /**
     * Max total visible height of views not marked always-show when in the closed/initial state
     */
    Int32 mMaxCollapsedHeight;

    /**
     * Max total visible height of views not marked always-show when in the closed/initial state
     * when a default option is present
     */
    Int32 mMaxCollapsedHeightSmall;

    Boolean mSmallCollapsed;

    /**
     * Move views down from the top by this much in px
     */
    Float mCollapseOffset;

    Int32 mCollapsibleHeight;

    Int32 mTopOffset;

    Boolean mIsDragging;
    Boolean mOpenOnClick;
    Boolean mOpenOnLayout;
    Int32 mTouchSlop;
    Float mMinFlingVelocity;
    AutoPtr<IOverScroller> mScroller;
    AutoPtr<IVelocityTracker> mVelocityTracker;

    AutoPtr<IViewOnClickListener> mClickOutsideListener;
    Float mInitialTouchX;
    Float mInitialTouchY;
    Float mLastTouchY;
    Int32 mActivePointerId;

    AutoPtr<IRect> mTempRect;

    // ViewTreeObserver.OnTouchModeChangeListener mTouchModeChangeListener =
    //     new ViewTreeObserver.OnTouchModeChangeListener() {
    //         @Override
    //         public void onTouchModeChanged(Boolean isInTouchMode) {
    //             if (!isInTouchMode && hasFocus() && isDescendantClipped(getFocusedChild())) {
    //                 smoothScrollTo(0, 0);
    //             }
    //         }
    //     };
};

}// namespace Widget
}// namespace Internal
}// namespace Droid
}// namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_WIDGET_RESOLVERDRAWERLAYOUT_H__
