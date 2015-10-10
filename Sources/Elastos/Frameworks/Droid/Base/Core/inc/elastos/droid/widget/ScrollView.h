
#ifndef __ELASTOS_DROID_WIDGET_SCROLLVIEW_H__
#define __ELASTOS_DROID_WIDGET_SCROLLVIEW_H__

#include "elastos/droid/ext/frameworkext.h"
#include "widget/FrameLayout.h"
#include "view/VelocityTracker.h"
#include "graphics/CRect.h"

using Elastos::Droid::View::VelocityTracker;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::Os::IStrictModeSpan;

namespace Elastos {
namespace Droid {
namespace Widget {

class OverScroller;

class ScrollView : public FrameLayout
{
public:
    ScrollView();

    ScrollView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    ScrollView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    ~ScrollView();

    virtual CARAPI_(Int32) GetMaxScrollAmount();

    virtual CARAPI_(Boolean) IsFillViewport();

    virtual CARAPI SetFillViewport(
        /* [in] */ Boolean fillViewport);

    virtual CARAPI_(Boolean) IsSmoothScrollingEnabled();

    virtual CARAPI SetSmoothScrollingEnabled(
        /* [in] */ Boolean smoothScrollingEnabled);

    virtual CARAPI_(Boolean) ExecuteKeyEvent(
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) PageScroll(
        /* [in] */ Int32 direction);

    virtual CARAPI_(Boolean) FullScroll(
        /* [in] */ Int32 direction);

    virtual CARAPI_(Boolean) ArrowScroll(
        /* [in] */ Int32 direction);

    virtual CARAPI SmoothScrollBy(
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    virtual CARAPI SmoothScrollTo(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    virtual CARAPI Fling(
        /* [in] */ Int32 velocityY);

    virtual CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    virtual CARAPI ComputeScroll();

    virtual CARAPI RequestChildFocus(
        /* [in] */ IView* child,
        /* [in] */ IView* focused);

    virtual CARAPI_(Boolean) RequestChildRectangleOnScreen(
        /* [in] */ IView* child,
        /* [in] */ IRect* rectangle,
        /* [in] */ Boolean immediate);

    virtual CARAPI RequestLayout();

    virtual CARAPI ScrollTo(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    virtual CARAPI SetOverScrollMode(
        /* [in] */ Int32 overScrollMode);

    virtual CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI AddView(
        /* [in] */ IView* child);

    virtual CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ Int32 index);

    using FrameLayout::AddView;

    virtual CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ IViewGroupLayoutParams* params);

    virtual CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ Int32 index,
        /* [in] */ IViewGroupLayoutParams* params);

    virtual CARAPI_(Boolean) ShouldDelayChildPressedState();

    virtual CARAPI RequestDisallowInterceptTouchEvent(
        /* [in] */ Boolean disallowIntercept);

    virtual CARAPI_(Boolean) OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* ev);

    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* ev);

    virtual CARAPI_(Boolean) OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) PerformAccessibilityAction(
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

protected:

    virtual CARAPI_(Float) GetTopFadingEdgeStrength();

    virtual CARAPI_(Float) GetBottomFadingEdgeStrength();

    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI_(void) OnOverScrolled(
        /* [in] */ Int32 scrollX,
        /* [in] */ Int32 scrollY,
        /* [in] */ Boolean clampedX,
        /* [in] */ Boolean clampedY);

    virtual CARAPI_(Int32) ComputeVerticalScrollRange();

    virtual CARAPI_(Int32) ComputeVerticalScrollOffset();

    virtual CARAPI_(void) MeasureChild(
        /* [in] */ IView* child,
        /* [in] */ Int32 parentWidthMeasureSpec,
        /* [in] */ Int32 parentHeightMeasureSpec);

    virtual CARAPI_(void) MeasureChildWithMargins(
        /* [in] */ IView* child,
        /* [in] */ Int32 parentWidthMeasureSpec,
        /* [in] */ Int32 widthUsed,
        /* [in] */ Int32 parentHeightMeasureSpec,
        /* [in] */ Int32 heightUsed);

    /**
     * Compute the amount to scroll in the Y direction in order to get
     * a rectangle completely on the screen (or, if taller than the screen,
     * at least the first screen size chunk of it).
     *
     * @param rect The rect.
     * @return The scroll delta.
     */
    CARAPI_(Int32) ComputeScrollDeltaToGetChildRectOnScreen(
        /* [in] */ IRect* rect);

    /**
     * When looking for focus in children of a scroll view, need to be a little
     * more careful not to give focus to something that is scrolled off screen.
     *
     * This is more expensive than the default {@link android.view.ViewGroup}
     * implementation, otherwise this behavior might have been made the default.
     */
    virtual CARAPI_(Boolean) OnRequestFocusInDescendants(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    virtual CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    virtual CARAPI OnDetachedFromWindow();

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:
    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(void) InitScrollView();

    /**
     * @return Returns true this ScrollView can be scrolled
     */
    CARAPI_(Boolean) CanScroll();

    CARAPI_(Boolean) InChild(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI_(void) OnSecondaryPointerUp(
        /* [in] */ IMotionEvent* ev);

    CARAPI_(Int32) GetScrollRange();

    /**
     * <p>
     * Finds the next focusable component that fits in the specified bounds.
     * </p>
     *
     * @param topFocus look for a candidate is the one at the top of the bounds
     *                 if topFocus is true, or at the bottom of the bounds if topFocus is
     *                 false
     * @param top      the top offset of the bounds in which a focusable must be
     *                 found
     * @param bottom   the bottom offset of the bounds in which a focusable must
     *                 be found
     * @return the next focusable component in the bounds or null if none can
     *         be found
     */
    CARAPI_(AutoPtr<IView>) FindFocusableViewInBounds(
        /* [in] */ Boolean topFocus,
        /* [in] */ Int32 top,
        /* [in] */ Int32 bottom);

    /**
     * <p>Scrolls the view to make the area defined by <code>top</code> and
     * <code>bottom</code> visible. This method attempts to give the focus
     * to a component visible in this area. If no component can be focused in
     * the new visible area, the focus is reclaimed by this scrollview.</p>
     *
     * @param direction the scroll direction: {@link android.view.View#FOCUS_UP}
     *                  to go upward
     *                  {@link android.view.View#FOCUS_DOWN} to downward
     * @param top       the top offset of the new area to be made visible
     * @param bottom    the bottom offset of the new area to be made visible
     * @return true if the key event is consumed by this method, false otherwise
     */
    CARAPI_(Boolean) ScrollAndFocus(
        /* [in] */ Int32 direction,
        /* [in] */ Int32 top,
        /* [in] */ Int32 bottom);

    /**
     * @return whether the descendant of this scroll view is scrolled off
     *  screen.
     */
    CARAPI_(Boolean) IsOffScreen(
        /* [in] */ IView* descendant);

    /**
     * @return whether the descendant of this scroll view is within delta
     *  pixels of being on the screen.
     */
    CARAPI_(Boolean) IsWithinDeltaOfScreen(
        /* [in] */ IView* descendant,
        /* [in] */ Int32 delta,
        /* [in] */ Int32 height);

    /**
     * Smooth scroll by a Y delta
     *
     * @param delta the number of pixels to scroll by on the Y axis
     */
    CARAPI_(void) DoScrollY(
        /* [in] */ Int32 delta);

    /**
     * Scrolls the view to the given child.
     *
     * @param child the View to scroll to
     */
    CARAPI_(void) ScrollToChild(
        /* [in] */ IView* child);

    /**
     * If rect is off screen, scroll just enough to get it (or at least the
     * first screen size chunk of it) on screen.
     *
     * @param rect      The rectangle.
     * @param immediate True to scroll immediately without animation
     * @return true if scrolling was performed
     */
    CARAPI_(Boolean) ScrollToChildRect(
        /* [in] */ IRect* rect,
        /* [in] */ Boolean immediate);

    /**
     * Return true if child is an descendant of parent, (or equal to the parent).
     */
    CARAPI_(Boolean) IsViewDescendantOf(
        /* [in] */ IView* child,
        /* [in] */ IView* parent);

    static CARAPI_(Int32) Clamp(
        /* [in] */ Int32 n,
        /* [in] */ Int32 my,
        /* [in] */ Int32 child);

    CARAPI_(void) InitOrResetVelocityTracker();

    CARAPI_(void) InitVelocityTrackerIfNotExists();

    CARAPI_(void) RecycleVelocityTracker();

    CARAPI_(void) EndDrag();

public:
    static const Int32 ANIMATED_SCROLL_GAP = 250;
    static const Float MAX_SCROLL_FACTOR = 0.5;
    static const String TAG;// = "ScrollView";

private:
     /**
     * Sentinel value for no current active pointer.
     * Used by {@link #mActivePointerId}.
     */
    static const Int32 INVALID_POINTER = -1;

private:
    Int64 mLastScroll;

    AutoPtr<CRect> mTempRect;
    AutoPtr<OverScroller> mScroller;
    AutoPtr<IEdgeEffect> mEdgeGlowTop;
    AutoPtr<IEdgeEffect> mEdgeGlowBottom;

    /**
     * Position of the last motion event.
     */
    Int32 mLastMotionY;

    /**
     * True when the layout has changed but the traversal has not come through yet.
     * Ideally the view hierarchy would keep track of this for us.
     */
    Boolean mIsLayoutDirty;

    /**
     * The child to give focus to in the event that a child has requested focus while the
     * layout is dirty. This prevents the scroll from being wrong if the child has not been
     * laid out before requesting focus.
     */
    AutoPtr<IView> mChildToScrollTo;

    /**
     * True if the user is currently dragging this ScrollView around. This is
     * not the same as 'is being flinged', which can be checked by
     * mScroller.isFinished() (flinging begins when the user lifts his finger).
     */
    Boolean mIsBeingDragged;

    /**
     * Determines speed during touch scrolling
     */
    AutoPtr<VelocityTracker> mVelocityTracker;

    /**
     * When set to true, the scroll view measure its child to make it fill the currently
     * visible area.
     */
    Boolean mFillViewport;

    /**
     * Whether arrow scrolling is animated.
     */
    Boolean mSmoothScrollingEnabled;

    Int32 mTouchSlop;
    Int32 mMinimumVelocity;
    Int32 mMaximumVelocity;

    Int32 mOverscrollDistance;
    Int32 mOverflingDistance;

    /**
     * ID of the active pointer. This is used to retain consistency during
     * drags/flings if multiple pointers are used.
     */
    Int32 mActivePointerId;

    /**
     * The StrictMode "critical time span" objects to catch animation
     * stutters.  Non-null when a time-sensitive animation is
     * in-flight.  Must call finish() on them when done animating.
     * These are no-ops on user builds.
     */
     AutoPtr<IStrictModeSpan> mScrollStrictSpan;
     AutoPtr<IStrictModeSpan> mFlingStrictSpan;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_SCROLLVIEW_H__
