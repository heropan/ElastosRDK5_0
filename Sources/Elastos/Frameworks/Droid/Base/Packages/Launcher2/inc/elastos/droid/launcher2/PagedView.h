
#ifndef  __ELASTOS_DROID_LAUNCHER2_PAGEDVIEW_H__
#define  __ELASTOS_DROID_LAUNCHER2_PAGEDVIEW_H__

#include <elastos/droid/animation/AnimatorListenerAdapter.h>
#include <elastos/droid/os/Runnable.h>
#include <elastos/droid/view/ViewGroup.h>

using Elastos::Droid::Animation::IAnimator;
using Elastos::Droid::Animation::AnimatorListenerAdapter;
using Elastos::Droid::Animation::ITimeInterpolator;
using Elastos::Droid::Animation::IValueAnimator;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IVelocityTracker;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroupOnHierarchyChangeListener;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
using Elastos::Droid::View::Animation::IInterpolator;
using Elastos::Droid::Widget::IScroller;
using Elastos::Utility::IArrayList;

namespace Elastos {
namespace Droid {
namespace Launcher2 {

/**
 * An abstraction of the original Workspace which supports browsing through a
 * sequential list of "pages"
 */
class PagedView
    : public ViewGroup
    , public IPagedView
{
public:
    class SavedState : public BaseSavedState
    {
    public:
        SavedState();

        CARAPI constructor(
            /* [in] */ IParcelable* superState);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

    public:
        Int32 mCurrentPage;
    };

private:
    class OnHierarchyChangeListener
        : public Object
        , public IViewGroupOnHierarchyChangeListener
    {
    public:
        OnHierarchyChangeListener(
            /* [in] */ PagedView* host)
            : mHost(host)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnChildViewAdded(
            /* [in] */ IView* parent,
            /* [in] */ IView* child);

        CARAPI OnChildViewRemoved(
            /* [in] */ IView* parent,
            /* [in] */ IView* child);

    private:
        PagedView* mHost;
    };

    class ScrollInterpolator
        : public Object
        , public IInterpolator
        , public ITimeInterpolator
    {
    public:
        ScrollInterpolator();

        CAR_INTERFACE_DECL()

        CARAPI GetInterpolation(
            /* [in] */ Float t,
            /* [out] */ Float* res);

        CARAPI HasNativeInterpolator(
            /* [out] */ Boolean* res);
    };

    class HideScrollingIndicatorRunnable : public Runnable
    {
    public:
        HideScrollingIndicatorRunnable(
            /* [in] */ PagedView* mHost);

        // @Override
        CARAPI Run();

    private:
        PagedView* mHost;
    };

    class MyAnimatorListenerAdapter : public AnimatorListenerAdapter
    {
    public:
        MyAnimatorListenerAdapter(
            /* [in] */ PagedView* host);

        // @Override
        CARAPI OnAnimationCancel(
            /* [in] */ IAnimator* animation);

        // @Override
        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);

    private:
        Boolean mCancelled;
        PagedView* mHost;
    };

public:
    PagedView();

    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    virtual CARAPI SetPageSwitchListener(
        /* [in] */ IPageSwitchListener* pageSwitchListener);

    /**
     * Note: this is a reimplementation of IView::IsLayoutRtl() since that is currently hidden api.
     */
    virtual CARAPI_(Boolean) IsLayoutRtl();

    /**
     * Returns the index of the currently displayed page.
     *
     * @return The index of the currently displayed page.
     */
    virtual CARAPI_(Int32) GetCurrentPage();

    virtual CARAPI_(Int32) GetNextPage();

    virtual CARAPI_(Int32) GetPageCount();

    virtual CARAPI_(AutoPtr<IView>) GetPageAt(
        /* [in] */ Int32 index);

    /**
     * Called during AllApps/Home transitions to avoid Unnecessary work. When that other animati
     n
     * ends, {@link #resumeScrolling()} should be called, along with
     * {@link #UpdateCurrentPageScroll()} to correctly set the final state and re-enable scrolling.
     */
    virtual CARAPI PauseScrolling();

    /**
     * Enables scrolling again.
     * @see #pauseScrolling()
     */
    virtual CARAPI ResumeScrolling();

    /**
     * Sets the current page.
     */
    virtual CARAPI SetCurrentPage(
        /* [in] */ Int32 currentPage);

    /**
     * Registers the specified listener on each page contained in this workspace.
     *
     * @param l The listener used to respond to long clicks.
     */
    // @Override
    CARAPI SetOnLongClickListener(
        /* [in] */ IViewOnLongClickListener* l);

    // @Override
    CARAPI ScrollBy(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    // @Override
    CARAPI ScrollTo(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    // @Override
    CARAPI ComputeScroll();

    // A layout scale of 1.0f assumes that the pages, in their unshrunken state, have a
    // scale of 1.0f. A layout scale of 0.8f assumes the pages have a scale of 0.8f, and
    // tightens the layout accordingly
    virtual CARAPI SetLayoutScale(
        /* [in] */ Float childrenScale);

    virtual CARAPI SetPageSpacing(
        /* [in] */ Int32 pageSpacing);

    // @Override
    CARAPI OnChildViewAdded(
        /* [in] */ IView* parent,
        /* [in] */ IView* child);

    // @Override
    CARAPI OnChildViewRemoved(
        /* [in] */ IView* parent,
        /* [in] */ IView* child);

    // @Override
    CARAPI RequestChildRectangleOnScreen(
        /* [in] */ IView* child,
        /* [in] */ IRect* rectangle,
        /* [in] */ Boolean immediate,
        /* [out] */ Boolean* res);

    // @Override
    CARAPI DispatchUnhandledMove(
        /* [in] */ IView* focused,
        /* [in] */ Int32 direction,
        /* [out] */ Boolean* res);

    // @Override
    CARAPI AddFocusables(
        /* [in] */ IArrayList* views,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 focusableMode);

    /**
     * If one of our descendant views decides that it could be focused now, only
     * pass that along if it's on the current page.
     *
     * This happens when live folders requery, and if they're off page, they
     * end up calling requestFocus, which pulls it on page.
     */
    // @Override
    CARAPI FocusableViewAvailable(
        /* [in] */ IView* focused);

    /**
     * {@inheritDoc}
     */
    // @Override
    CARAPI RequestDisallowInterceptTouchEvent(
        /* [in] */ Boolean disallowIntercept);

    // @Override
    CARAPI OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* res);

    // @Override
    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* res);

    // @Override
    CARAPI OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* res);

    // @Override
    CARAPI RequestChildFocus(
        /* [in] */ IView* child,
        /* [in] */ IView* focused);

    virtual CARAPI_(Int32) GetPageNearestToCenterOfScreen();

    // We want the duration of the page snap animation to be influenced by the distance that
    // the screen has to travel, however, we don't want this duration to be effected in a
    // purely linear fashion. Instead, we use this method to moderate the effect that the distance
    // of travel has on the overall snap duration.
    virtual CARAPI_(Float) DistanceInfluenceForSnapDuration(
        /* [in] */ Float f);

    virtual CARAPI ScrollLeft();

    virtual CARAPI ScrollRight();

    virtual CARAPI_(Int32) GetPageForView(
        /* [in] */ IView* v);

    /**
     * @return True is long presses are still allowed for the current touch
     */
    virtual CARAPI_(Boolean) AllowLongPress();

    /**
     * Set TRUE to allow long-press events to be triggered, usually checked by
     * {@link Launcher} to accept or block dpad-initiated long-presses.
     */
    virtual CARAPI SetAllowLongPress(
        /* [in] */ Boolean allowLongPress);

    /**
     * This method is called ONLY to synchronize the number of pages that the paged view has.
     * To actually fill the pages with information, implement syncPageItems() below.  It is
     * guaranteed that syncPageItems() will be called for a particular page before it is shown,
     * and therefore, individual page items do not need to be updated in this method.
     */
    virtual CARAPI SyncPages() = 0;

    /**
     * This method is called to synchronize the items that are on a particular page.  If views on
     * the page can be reused, then they should be updated within this method.
     */
    virtual CARAPI SyncPageItems(
        /* [in] */ Int32 page,
        /* [in] */ Boolean immediate) = 0;

    virtual CARAPI ShowScrollIndicatorTrack();

    virtual CARAPI HideScrollIndicatorTrack();

    /* Accessibility */
    // @Override
    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    // @Override
    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    // @Override
    CARAPI PerformAccessibilityAction(
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments,
        /* [out] */ Boolean* res);

    // @Override
    CARAPI OnHoverEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* res);

protected:
    /**
     * Initializes various states for this workspace.
     */
    virtual CARAPI Init();

    /**
     * Called by subclasses to mark that data is ready, and that we can begin loading and laying
     * out pages.
     */
    virtual CARAPI SetDataIsReady();

    virtual CARAPI_(Boolean) IsDataReady();

    virtual CARAPI_(Int32) IndexToPage(
        /* [in] */ Int32 index);

    /**
     * Updates the scroll of the current page immediately to its final scroll position.  We use this
     * in CustomizePagedView to allow tabs to share the same PagedView while resetting the scroll of
     * the previous tab page.
     */
    virtual CARAPI UpdateCurrentPageScroll();

    virtual CARAPI NotifyPageSwitchListener();

    virtual CARAPI PageBeginMoving();

    virtual CARAPI PageEndMoving();

    virtual CARAPI_(Boolean) IsPageMoving();

    // a method that subclasses can override to add behavior
    virtual CARAPI OnPageBeginMoving();

    // a method that subclasses can override to add behavior
    virtual CARAPI OnPageEndMoving();

    // we moved this functionality to a helper function so SmoothPagedView can reuse it
    virtual CARAPI_(Boolean) ComputeScrollHelper();

    // @Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI ScrollToNewPageWithoutMovingPages(
        /* [in] */ Int32 newCurrentPage);

    // @Override
    CARAPI OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI ScreenScrolled(
        /* [in] */ Int32 screenCenter);

    virtual CARAPI InvalidateCachedOffsets();

    virtual CARAPI_(Int32) GetChildOffset(
        /* [in] */ Int32 index);

    virtual CARAPI_(Int32) GetRelativeChildOffset(
        /* [in] */ Int32 index);

    virtual CARAPI_(Int32) GetScaledMeasuredWidth(
        /* [in] */ IView* child);

    virtual CARAPI GetVisiblePages(
        /* [in] */ ArrayOf<Int32>* range);

    virtual CARAPI_(Boolean) ShouldDrawChild(
        /* [in] */ IView* child);

    // @Override
    CARAPI_(void) DispatchDraw(
        /* [in] */ ICanvas* canvas);

    // @Override
    CARAPI_(Boolean) OnRequestFocusInDescendants(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    /**
     * Return TRUE if a tap at (x, y) should trigger a flip to the previous page.
     */
    virtual CARAPI_(Boolean) HitsPreviousPage(
        /* [in] */ Float x,
        /* [in] */ Float y);

    /**
     * Return TRUE if a tap at (x, y) should trigger a flip to the next page.
     */
    virtual CARAPI_(Boolean) HitsNextPage(
        /* [in] */ Float x,
        /* [in] */ Float y);

    virtual CARAPI DetermineScrollingStart(
        /* [in] */ IMotionEvent* ev);

    /*
     * Determines if we should change the touch state to start scrolling after the
     * user moves their touch point too far.
     */
    virtual CARAPI DetermineScrollingStart(
        /* [in] */ IMotionEvent* ev,
        /* [in] */ Float touchSlopScale);

    virtual CARAPI CancelCurrentPageLongPress();

    virtual CARAPI_(Float) GetScrollProgress(
        /* [in] */ Int32 screenCenter,
        /* [in] */ IView* v,
        /* [in] */ Int32 page);

    virtual CARAPI AcceleratedOverScroll(
        /* [in] */ Float amount);

    virtual CARAPI DampedOverScroll(
        /* [in] */ Float amount);

    virtual CARAPI OverScroll(
        /* [in] */ Float amount);

    virtual CARAPI_(Float) MaxOverScroll();

    virtual CARAPI OnUnhandledTap(
        /* [in] */ IMotionEvent* ev);

    virtual CARAPI_(Int32) GetChildIndexForRelativeOffset(
        /* [in] */ Int32 relativeOffset);

    virtual CARAPI_(Int32) GetChildWidth(
        /* [in] */ Int32 index);

    virtual CARAPI SnapToDestination();

    virtual CARAPI SnapToPageWithVelocity(
        /* [in] */ Int32 whichPage,
        /* [in] */ Int32 velocity);

    virtual CARAPI SnapToPage(
        /* [in] */ Int32 whichPage);

    virtual CARAPI SnapToPage(
        /* [in] */ Int32 whichPage,
        /* [in] */ Int32 duration);

    virtual CARAPI SnapToPage(
        /* [in] */ Int32 whichPage,
        /* [in] */ Int32 delta,
        /* [in] */ Int32 duration);

    virtual CARAPI LoadAssociatedPages(
        /* [in] */ Int32 page);

    virtual CARAPI LoadAssociatedPages(
        /* [in] */ Int32 page,
        /* [in] */ Boolean immediateAndOnly);

    virtual CARAPI_(Int32) GetAssociatedLowerPageBound(
        /* [in] */ Int32 page);

    virtual CARAPI_(Int32) GetAssociatedUpperPageBound(
        /* [in] */ Int32 page);

    virtual CARAPI InvalidatePageData();

    virtual CARAPI InvalidatePageData(
        /* [in] */ Int32 currentPage);

    virtual CARAPI InvalidatePageData(
        /* [in] */ Int32 currentPage,
        /* [in] */ Boolean immediateAndOnly);

    virtual CARAPI_(AutoPtr<IView>) GetScrollingIndicator();

    virtual CARAPI_(Boolean) IsScrollingIndicatorEnabled();

    virtual CARAPI FlashScrollingIndicator(
        /* [in] */ Boolean animated);

    virtual CARAPI ShowScrollingIndicator(
        /* [in] */ Boolean immediately);

    virtual CARAPI CancelScrollingIndicatorAnimations();

    virtual CARAPI HideScrollingIndicator(
        /* [in] */ Boolean immediately);

    /**
     * To be overridden by subclasses to determine whether the scroll indicator should stretch to
     * fill its space on the track or not.
     */
    virtual CARAPI_(Boolean) HasElasticScrollIndicator();

    virtual CARAPI_(String) GetCurrentPageDescription();

private:
    // This curve determines how the effect of scrolling over the limits of the page dimishes
    // as the user pulls further and further from the bounds
    CARAPI_(Float) OverScrollInfluenceCurve(
        /* [in] */ Float f);

    CARAPI_(void) AcquireVelocityTrackerAndAddMovement(
        /* [in] */ IMotionEvent* ev);

    CARAPI_(void) ReleaseVelocityTracker();

    CARAPI_(void) OnSecondaryPointerUp(
        /* [in] */ IMotionEvent* ev);

    CARAPI_(void) UpdateScrollingIndicator();

    CARAPI_(void) UpdateScrollingIndicatorPosition();

public:
    static const Int32 AUTOMATIC_PAGE_SPACING = -1;

protected:
    static const Int32 INVALID_PAGE = -1;

    static const Int32 PAGE_SNAP_ANIMATION_DURATION = 550;
    static const Int32 MAX_PAGE_SNAP_DURATION = 750;
    static const Int32 SLOW_PAGE_SNAP_ANIMATION_DURATION = 950;
    static const Float NANOTIME_DIV = 1000000000.0f;

    static const Int32 TOUCH_STATE_REST = 0;
    static const Int32 TOUCH_STATE_SCROLLING = 1;
    static const Int32 TOUCH_STATE_PREV_PAGE = 2;
    static const Int32 TOUCH_STATE_NEXT_PAGE = 3;
    static const Float ALPHA_QUANTIZE_LEVEL = 0.0001f;

    static const Int32 INVALID_POINTER = -1;

    static const Int32 sScrollIndicatorFadeInDuration = 150;
    static const Int32 sScrollIndicatorFadeOutDuration = 650;
    static const Int32 sScrollIndicatorFlashDuration = 650;

    Int32 mFlingThresholdVelocity;
    Int32 mMinFlingVelocity;
    Int32 mMinSnapVelocity;

    Float mDensity;
    Float mSmoothingTime;
    Float mTouchX;

    Boolean mFirstLayout;

    Int32 mCurrentPage;
    Int32 mNextPage;
    Int32 mMaxScrollX;
    AutoPtr<IScroller> mScroller;
    Float mLastMotionX;
    Float mLastMotionXRemainder;
    Float mLastMotionY;
    Float mTotalMotionX;

    Int32 mTouchState;
    Boolean mForceScreenScrolled;

    AutoPtr<IViewOnLongClickListener> mLongClickListener;

    Boolean mAllowLongPress;

    Int32 mTouchSlop;
    Int32 mPageSpacing;
    Int32 mPageLayoutPaddingTop;
    Int32 mPageLayoutPaddingBottom;
    Int32 mPageLayoutPaddingLeft;
    Int32 mPageLayoutPaddingRight;
    Int32 mPageLayoutWidthGap;
    Int32 mPageLayoutHeightGap;
    Int32 mCellCountX;
    Int32 mCellCountY;
    Boolean mCenterPagesVertically;
    Boolean mAllowOverScroll;
    Int32 mUnboundedScrollX;
    AutoPtr<ArrayOf<Int32> > mTempVisiblePagesRange;
    Boolean mForceDrawAllChildrenNextFrame;

    // mOverScrollX is equal to getScrollX() when we're within the normal scroll range. Otherwise
    // it is equal to the scaled overscroll position. We use a separate value so as to prevent
    // the screens from continuing to translate beyond the normal bounds.
    Int32 mOverScrollX;

    // parameter that adjusts the layout to be optimized for pages with that scale factor
    Float mLayoutScale;

    Int32 mActivePointerId = INVALID_POINTER;

    AutoPtr<IArrayList> mDirtyPageContent; // item is Boolean

    // If TRUE, syncPages and syncPageItems will be called to refresh pages
    Boolean mContentIsRefreshable;

    // If TRUE, modify alpha of neighboring pages as user scrolls left/right
    Boolean mFadeInAdjacentScreens;

    // It TRUE, use a different slop parameter (pagingTouchSlop = 2 * touchSlop) for deciding
    // to switch to a new page
    Boolean mUsePagingTouchSlop;

    // If TRUE, the subclass should directly update scrollX itself in its computeScroll method
    // (SmoothPagedView does this)
    Boolean mDeferScrollUpdate;

    Boolean mIsPageMoving;

    // All syncs and layout passes are deferred until data is ready.
    Boolean mIsDataReady;

private:
    static const String TAG;
    static const Boolean DEBUG = FALSE;

    // the min drag distance for a fling to register, to prevent random page shifts
    static const Int32 MIN_LENGTH_FOR_FLING = 25;

    static const Float OVERSCROLL_ACCELERATE_FACTOR = 2;
    static const Float OVERSCROLL_DAMP_FACTOR = 0.14f;

    static const Float RETURN_TO_ORIGINAL_PAGE_THRESHOLD = 0.33f;
    // The page is moved more than halfway, automatically move to the next page on touch up.
    static const Float SIGNIFICANT_MOVE_THRESHOLD = 0.4f;

    // The following constants need to be scaled based on density. The scaled versions will be
    // assigned to the corresponding member variables below.
    static const Int32 FLING_THRESHOLD_VELOCITY = 500;
    static const Int32 MIN_SNAP_VELOCITY = 1500;
    static const Int32 MIN_FLING_VELOCITY = 250;

    AutoPtr<IVelocityTracker> mVelocityTracker;

    Float mDownMotionX;
    Int32 mLastScreenCenter;
    AutoPtr<ArrayOf<Int32> > mChildOffsets;
    AutoPtr<ArrayOf<Int32> > mChildRelativeOffsets;
    AutoPtr<ArrayOf<Int32> > mChildOffsetsWithLayoutScale;

    Int32 mPagingTouchSlop;
    Int32 mMaximumVelocity;
    Int32 mMinimumWidth;
    AutoPtr<IPageSwitchListener> mPageSwitchListener;

    // Scrolling indicator
    AutoPtr<IValueAnimator> mScrollIndicatorAnimator;
    AutoPtr<IView> mScrollIndicator;
    Int32 mScrollIndicatorPaddingLeft;
    Int32 mScrollIndicatorPaddingRight;
    Boolean mHasScrollIndicator;
    Boolean mShouldShowScrollIndicator;
    Boolean mShouldShowScrollIndicatorImmediately;
    Boolean mScrollingPaused;

    // If set, will defer loading associated pages until the scrolling settles
    Boolean mDeferLoadAssociatedPagesUntilScrollCompletes;

    AutoPtr<Runnable> mHideScrollingIndicatorRunnable;
};

} // namespace Launcher2
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_LAUNCHER2_PAGEDVIEW_H__
