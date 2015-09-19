
#ifndef __ELASTOS_DROID_WIDGET_ABSLISTVIEW_H__
#define __ELASTOS_DROID_WIDGET_ABSLISTVIEW_H__

#include <ext/frameworkext.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>
#include <elastos/IntegralToString.h>
#include "widget/AdapterView.h"
#include "widget/EdgeEffect.h"
#include "view/VelocityTracker.h"
#include "view/View.h"
#include "view/CViewConfigurationHelper.h"
#include "content/CIntentFilterComparison.h"
#include "view/CKeyEventHelper.h"
#include "view/IInputConnectionWrapper.h"
#include "graphics/CRect.h"
#include <Elastos.CoreLibrary.h>
#include "R.h"

namespace Elastos{
namespace Droid{
namespace Widget{

class CAbsListViewSavedState;

using Elastos::Core::IRunnable;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IOnGlobalLayoutListener;
using Elastos::Droid::View::EIID_IOnGlobalLayoutListener;
using Elastos::Droid::View::IOnTouchModeChangeListener;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IStrictModeSpan;
using Elastos::Droid::Widget::IFilterListener;
using Elastos::Droid::Widget::IAbsListViewSavedState;
using Elastos::Droid::Widget::IMultiChoiceModeListener;
using Elastos::Droid::Text::ITextWatcher;
using Elastos::Droid::Text::EIID_ITextWatcher;
using Elastos::Droid::Text::IEditable;
using Elastos::Droid::Text::EIID_IEditable;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::View::View;
using Elastos::Droid::Widget::IPopupWindow;
//using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IActionMode;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::IMenuItem;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::Text::IEditable;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::View::IInputConnectionWrapper;
using Elastos::Droid::View::VelocityTracker;
using Elastos::Droid::View::Animation::IInterpolator;
using Elastos::Droid::Widget::CAbsListViewSavedState;
using Elastos::Droid::R;

class OverScroller;
class FastScroller;

typedef List<AutoPtr<IView> > ViewList;
typedef typename ViewList::Iterator ViewListIterator;
typedef typename ViewList::ReverseIterator ViewListReverseIterator;
typedef HashMap<Int32, AutoPtr<IView> > ViewMap;
typedef typename ViewMap::Iterator ViewMapIterator;

class AbsListView
     : public AdapterView
{
    friend class FastScroller;
protected:
    class ListItemAccessibilityDelegate
        : public View::AccessibilityDelegate
    {
    public:
        ListItemAccessibilityDelegate(
            /* [in] */ AbsListView* host);

        virtual CARAPI OnInitializeAccessibilityNodeInfo(
            /* [in] */ IView* host,
            /* [in] */ IAccessibilityNodeInfo* info);

        CARAPI PerformAccessibilityAction(
            /* [in] */ IView* host,
            /* [in] */ Int32 action,
            /* [in] */ IBundle* arguments,
            /* [out] */ Boolean* result);
    private:
        AbsListView* mHost;
    };


/**
 * A base class for Runnables that will check that their view is still attached to
 * the original window as when the Runnable was created.
 *
 */
private:
    class WindowRunnnable : public Runnable
    {
    public:
        WindowRunnnable(
            /* [in] */ AbsListView* host);

        CARAPI_(void) RememberWindowAttachCount();

        CARAPI_(Boolean) SameWindow();

        virtual CARAPI Run() { return NOERROR; }

    protected:
        AbsListView* mHost;

    private:
        Int32 mOriginalAttachCount;
    };

private:
    class AbsListViewPerformClick
        : public WindowRunnnable
    {
        friend class AbsListView;
    public:

        AbsListViewPerformClick (
            /* [in] */ AbsListView* host);

        virtual CARAPI Run();

    private:
        //AutoPtr<IView> mChild;
        Int32 mClickMotionPosition;
    };

private:
    class CheckForLongPress
        : public WindowRunnnable
    {
    public:

        CheckForLongPress (
            /* [in] */ AbsListView* host);

        virtual CARAPI Run();
    };

private:
    class CheckForKeyLongPress
        : public WindowRunnnable
    {
    public:

        CheckForKeyLongPress (
            /* [in] */ AbsListView* host);

        CARAPI Run();
    };

protected:
    class CheckForTap
        : public Runnable
    {
    public:
        CheckForTap(
            /* [in] */ AbsListView* host);

        CARAPI Run();

    private:
        AbsListView* mHost;
    };

private:
/*
 * The member mCheckFlywheel in FlingRunnable is a sub-Runnable
 * so you need implement it here
 */
    class FlingRunnable;
    class FlingRunnableInner
            : public Runnable
    {
    public:

        FlingRunnableInner(
            /* [in] */ FlingRunnable* host,
            /* [in] */ AbsListView* mainHost);

        CARAPI Run();

    private:

        FlingRunnable* mHost;
        AbsListView* mMainHost;
    };

private:
/**
 * Responsible for fling behavior. Use {@link #start(Int32)} to
 * initiate a fling. Each frame of the fling is handled in {@link #run()}.
 * A FlingRunnable will keep re-posting itself until the fling is done.
 *
 */
    class FlingRunnable
            : public Runnable
    {
    public:
        FlingRunnable(
            /* [in] */ AbsListView* host);

        CARAPI Run();

        CARAPI_(void) Start(
            /* [in] */ Int32 initialVelocity);

        CARAPI_(void) StartSpringback();

        CARAPI_(void) StartOverfling(
            /* [in] */ Int32 initialVelocity);

        CARAPI_(void) EdgeReached(
            /* [in] */ Int32 delta);

        CARAPI_(void) StartScroll(
            /* [in] */ Int32 distance,
            /* [in] */ Int32 duration,
            /* [in] */ Boolean linear);

        CARAPI_(void) EndFling();

        CARAPI_(void) FlywheelTouch();

    private:

        static const Int32 FLYWHEEL_TIMEOUT = 40; // milliseconds

    private:
        /**
         * Tracks the decay of a fling scroll
         */
        AutoPtr<OverScroller> mScroller;

        /**
         * Y value reported by mScroller on the previous fling
         */
        Int32 mLastFlingY;

        AbsListView* mHost;

        AutoPtr<FlingRunnableInner> mCheckFlywheel;

        friend class FlingRunnableInner;
        friend class AbsListView;
    };

private:
// This class is create for the runnable in PositionScroller::Start functions
    class PositionScroller;
    class PositionScrollerStartRunnable
            : public Runnable
    {
    public:

        PositionScrollerStartRunnable(
            /* [in] */ Int32 paramsCount,
            /* [in] */ Int32 param1,
            /* [in] */ Int32 param2,
            /* [in] */ Int32 param3,
            /* [in] */ PositionScroller* host);

        CARAPI Run();
    private:

        const Int32 mParamsCount;
        Int32 mParam1;
        Int32 mParam2;
        Int32 mParam3;
        PositionScroller* mHost;
    };

private:
    class PositionScroller
            : public Runnable
    {
    public:
        PositionScroller(
            /* [in] */ AbsListView* host);

        CARAPI_(void) Start(
            /* [in] */ const Int32 position);

        CARAPI_(void) Start(
            /* [in] */ const Int32 position,
            /* [in] */ const Int32 boundPosition);

        CARAPI_(void) StartWithOffset(
            /* [in] */ Int32 position,
            /* [in] */ Int32 offset);

        CARAPI_(void) StartWithOffset(
            /* [in] */ Int32 position,
            /* [in] */ Int32 offset,
            /* [in] */ const Int32 duration);

        CARAPI_(void) ScrollToVisible(
            /* [in] */ Int32 targetPos,
            /* [in] */ Int32 boundPos,
            /* [in] */ Int32 duration);

        CARAPI_(void) Stop();

        virtual CARAPI Run();

    private:
        static const Int32 SCROLL_DURATION = 200;

        static const Int32 MOVE_DOWN_POS = 1;
        static const Int32 MOVE_UP_POS = 2;
        static const Int32 MOVE_DOWN_BOUND = 3;
        static const Int32 MOVE_UP_BOUND = 4;
        static const Int32 MOVE_OFFSET = 5;

    private:
        AbsListView* mHost;
        Int32 mMode;
        Int32 mTargetPos;
        Int32 mBoundPos;
        Int32 mLastSeenPos;
        Int32 mScrollDuration;
        Int32 mExtraScroll; //const
        Int32 mOffsetFromTop;
        friend class AbsListView;
    };



protected:
    class AdapterDataSetObserver
            : public AdapterView::AdapterDataSetObserver
    {
    public:

        AdapterDataSetObserver(
            /* [in] */ AbsListView* host);

        //@Override
        CARAPI OnChanged();

        //@Override
        CARAPI OnInvalidated();

    private:
        AbsListView* mOwner;
    };

protected:
    class MultiChoiceModeWrapper
            : public IMultiChoiceModeListener
            , public ElRefBase
    {
    public:

    CAR_INTERFACE_DECL()

    private:
        AutoPtr<IMultiChoiceModeListener> mWrapped;

        AbsListView* mHost;
    public:

        MultiChoiceModeWrapper(
            /* [in] */ AbsListView* host);

        CARAPI SetWrapped(
            /* [in] */ IMultiChoiceModeListener* wrapped);

        CARAPI HasWrappedCallback(
            /* [out] */ Boolean* result);

        CARAPI OnCreateActionMode(
            /* [in] */ IActionMode* mode,
            /* [in] */ IMenu* menu,
            /* [out] */ Boolean* result);

        CARAPI OnPrepareActionMode(
            /* [in] */ IActionMode* mode,
            /* [in] */ IMenu* menu,
            /* [out] */ Boolean* result);

        CARAPI OnActionItemClicked(
            /* [in] */ IActionMode* mode,
            /* [in] */ IMenuItem* item,
            /* [out] */ Boolean* result);

        CARAPI OnDestroyActionMode(
            /* [in] */ IActionMode* mode);

        CARAPI OnItemCheckedStateChanged(
            /* [in] */ IActionMode* mode,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id,
            /* [in] */ Boolean checked);
    };

protected:
/**
 * The RecycleBin facilitates reuse of views across layouts. The RecycleBin has two levels of
 * storage: ActiveViews and ScrapViews. ActiveViews are those views which were onscreen at the
 * start of a layout. By construction, they are displaying current information. At the end of
 * layout, all views in ActiveViews are demoted to ScrapViews. ScrapViews are old views that
 * could potentially be used by the adapter to avoid allocating views unnecessarily.
 *
 * @see android.widget.AbsListView#setRecyclerListener(android.widget.AbsListView.RecyclerListener)
 * @see android.widget.AbsListView.RecyclerListener
 */
    class RecycleBin : public ElRefBase
    {
        friend class AbsListView;
    public:
        RecycleBin(
            /* [in] */ AbsListView* host);

        ~RecycleBin();

        CARAPI SetViewTypeCount(
            /* [in] */ Int32 viewTypeCount);

        CARAPI_(void) MarkChildrenDirty();

        CARAPI_(Boolean) ShouldRecycleViewType(
            /* [in] */ Int32 viewType);

        CARAPI_(void) Clear();

        CARAPI_(void) AddScrapView(
            /* [in ]*/ IView* scrap,
            /* [in] */ Int32 position);

        CARAPI_(void) FillActiveViews(
            /* [in] */ Int32 childCount,
            /* [in] */ Int32 firstActivePosition);

        CARAPI_(AutoPtr<IView>) GetActiveView(
            /* [in] */ Int32 position);

        CARAPI_(AutoPtr<IView>) GetTransientStateView(
            /* [in] */ Int32 position);

        /**
         * Dump any currently saved views with transient state.
         */
        CARAPI_(void) ClearTransientStateViews();

        CARAPI_(AutoPtr<IView>) GetScrapView(
            /* [in] */ Int32 position);

        CARAPI_(void) ScrapActiveViews();

        CARAPI_(void) RemoveSkippedScrap();

        CARAPI_(void) ReclaimScrapViews(
            /* [in] */ IObjectContainer* views);

        CARAPI_(void) SetCacheColorHint(
            /* [in] */ Int32 color);

    private:
        CARAPI_(void) PruneScrapViews();

    private:
        AutoPtr<IRecyclerListener> mRecyclerListener;

        /**
         * The position of the first view stored in mActiveViews.
         */
        Int32 mFirstActivePosition;

        /**
         * Views that were on screen at the start of layout. This array is populated at the start of
         * layout, and at the end of layout all view in mActiveViews are moved to mScrapViews.
         * Views in mActiveViews represent a contiguous range of Views, with position of the first
         * view store in mFirstActivePosition.
         */
        AutoPtr<ArrayOf<IView*> > mActiveViews;

        /**
         * Unsorted views that can be used by the adapter as a convert view.
         */
        AutoPtr<ArrayOf<ViewList*> > mScrapViews;

        Int32 mViewTypeCount;

        AutoPtr<ViewList> mCurrentScrap;

        AutoPtr<ViewList> mSkippedScrap;

        AutoPtr< HashMap<Int32, AutoPtr<IView> > > mTransientStateViews;

        AbsListView* mHost;
    };

private:
    // The sub-Runnable in AbsListView::ClearScrollingCache
    class ClearScrollingCacheRunnable
            : public Runnable
    {
    public:
        ClearScrollingCacheRunnable(
            /* [in] */ AbsListView* host);

        CARAPI Run();

    private:
        AbsListView* mHost;
    };

    // The sub-Runnable for AbsListView::mTouchModeReset
    class TouchModeResetRunnable
            : public Runnable
    {
    public:
        TouchModeResetRunnable(
            /* [in] */ IRunnable* r,
            /* [in] */ AbsListView* host,
            /* [in] */ IView* v);

        CARAPI Run();
    private:
        AutoPtr<IRunnable> mRunnable;
        AbsListView* mHost;
        AutoPtr<IView> mView;

    };

protected:

    AbsListView();

public:
    AbsListView(
        /* [in] */ IContext* context);

    AbsListView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = R::attr::absListViewStyle/* com.android.internal.R.attr.absListViewStyle */);

    ~AbsListView();

    //@Override
    CARAPI SetOverScrollMode(
        /* [in] */ Int32 mode);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI SetAdapter(
        /* [in] */ IAdapter* adapter);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    /**
     * Returns the number of items currently selected. This will only be valid
     * if the choice mode is not {@link #CHOICE_MODE_NONE} (default).
     *
     * <p>To determine the specific items that are currently selected, use one of
     * the <code>getChecked*</code> methods.
     *
     * @return The number of items currently selected
     *
     * @see #getCheckedItemPosition()
     * @see #getCheckedItemPositions()
     * @see #getCheckedItemIds()
     */
    virtual CARAPI_(Int32) GetCheckedItemCount();

    /**
     * Returns the checked state of the specified position. The result is only
     * valid if the choice mode has been set to {@link #CHOICE_MODE_SINGLE}
     * or {@link #CHOICE_MODE_MULTIPLE}.
     *
     * @param position The item whose checked state to return
     * @return The item's checked state or <code>false</code> if choice mode
     *         is invalid
     *
     * @see #setChoiceMode(int)
     */
    virtual CARAPI IsItemChecked(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* result);

    /**
     * Returns the currently checked item. The result is only valid if the choice
     * mode has been set to {@link #CHOICE_MODE_SINGLE}.
     *
     * @return The position of the currently checked item or
     *         {@link #INVALID_POSITION} if nothing is selected
     *
     * @see #setChoiceMode(int)
     */
    virtual CARAPI GetCheckedItemPosition(
            /* [out] */ Int32* result);

    /**
     * Returns the set of checked items in the list. The result is only valid if
     * the choice mode has not been set to {@link #CHOICE_MODE_NONE}.
     *
     * @return  A SparseBooleanArray which will return true for each call to
     *          get(int position) where position is a position in the list,
     *          or <code>null</code> if the choice mode is set to
     *          {@link #CHOICE_MODE_NONE}.
     */
//    virtual AutoPtr<HashMap<Int32, Boolean> > GetCheckedItemPositions();

    virtual AutoPtr<IObjectInt32Map> GetCheckedItemPositions();

    /**
     * Returns the set of checked items ids. The result is only valid if the
     * choice mode has not been set to {@link #CHOICE_MODE_NONE} and the adapter
     * has stable IDs. ({@link ListAdapter#hasStableIds()} == {@code true})
     *
     * @return A new array which contains the id of each checked item in the
     *         list.
     */
    virtual CARAPI GetCheckedItemIds(
            /* [out] */ ArrayOf<Int64>** result);

    /**
     * Clear any choices previously set
     */
    virtual CARAPI ClearChoices();

    /**
     * Sets the checked state of the specified position. The is only valid if
     * the choice mode has been set to {@link #CHOICE_MODE_SINGLE} or
     * {@link #CHOICE_MODE_MULTIPLE}.
     *
     * @param position The item whose checked state is to be checked
     * @param value The new checked state for the item
     */
    virtual CARAPI SetItemChecked(
        /* [in] */ Int32 position,
        /* [in] */ Boolean value);

    virtual CARAPI_(Boolean) PerformItemClick(
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    /**
     * @see #setChoiceMode(int)
     *
     * @return The current choice mode
     */
    virtual CARAPI GetChoiceMode(
        /* [out] */ Int32* result);

    /**
     * Defines the choice behavior for the List. By default, Lists do not have any choice behavior
     * ({@link #CHOICE_MODE_NONE}). By setting the choiceMode to {@link #CHOICE_MODE_SINGLE}, the
     * List allows up to one item to  be in a chosen state. By setting the choiceMode to
     * {@link #CHOICE_MODE_MULTIPLE}, the list allows any number of items to be chosen.
     *
     * @param choiceMode One of {@link #CHOICE_MODE_NONE}, {@link #CHOICE_MODE_SINGLE}, or
     * {@link #CHOICE_MODE_MULTIPLE}
     */
    virtual CARAPI SetChoiceMode(
        /* [in] */ Int32 choiceMode);

    /**
     * Set a {@link MultiChoiceModeListener} that will manage the lifecycle of the
     * selection {@link ActionMode}. Only used when the choice mode is set to
     * {@link #CHOICE_MODE_MULTIPLE_MODAL}.
     *
     * @param listener Listener that will manage the selection mode
     *
     * @see #setChoiceMode(int)
     */
    virtual CARAPI SetMultiChoiceModeListener(
        /* [in] */ IMultiChoiceModeListener* listener);


    /**
     * Enables fast scrolling by letting the user quickly scroll through lists by
     * dragging the fast scroll thumb. The adapter attached to the list may want
     * to implement {@link SectionIndexer} if it wishes to display alphabet preview and
     * jump between sections of the list.
     * @see SectionIndexer
     * @see #isFastScrollEnabled()
     * @param enabled whether or not to enable fast scrolling
     */
    virtual CARAPI SetFastScrollEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Set whether or not the fast scroller should always be shown in place of the
     * standard scrollbars. Fast scrollers shown in this way will not fade out and will
     * be a permanent fixture within the list. Best combined with an inset scroll bar style
     * that will ensure enough padding. This will enable fast scrolling if it is not
     * already enabled.
     *
     * @param alwaysShow true if the fast scroller should always be displayed.
     * @see #setScrollBarStyle(int)
     * @see #setFastScrollEnabled(boolean)
     */
    virtual CARAPI SetFastScrollAlwaysVisible(
        /* [in] */ Boolean alwaysShow);

    /**
     * Returns true if the fast scroller is set to always show on this view rather than
     * fade out when not in use.
     *
     * @return true if the fast scroller will always show.
     * @see #setFastScrollAlwaysVisible(boolean)
     */
    virtual CARAPI_(Boolean) IsFastScrollAlwaysVisible();

    //@Override
    virtual CARAPI_(Int32) GetVerticalScrollbarWidth();

    /**
     * Returns the current state of the fast scroll feature.
     * @see #setFastScrollEnabled(Boolean)
     * @return TRUE if fast scroll is enabled, FALSE otherwise
     */
    virtual CARAPI_(Boolean) IsFastScrollEnabled();

    //@Override
    virtual CARAPI SetVerticalScrollbarPosition(
            /* [in] */ Int32 result);

    /**
     * When smooth scrollbar is enabled, the position and size of the scrollbar thumb
     * is computed based on the number of visible pixels in the visible items. This
     * however assumes that all list items have the same height. If you use a list in
     * which items have different heights, the scrollbar will change appearance as the
     * user scrolls through the list. To avoid this issue, you need to disable this
     * property.
     *
     * When smooth scrollbar is disabled, the position and size of the scrollbar thumb
     * is based solely on the number of items in the adapter and the position of the
     * visible items inside the adapter. This provides a stable scrollbar as the user
     * navigates through a list of items with varying heights.
     *
     * @param enabled Whether or not to enable smooth scrollbar.
     *
     * @see #setSmoothScrollbarEnabled(Boolean)
     * @attr ref android.R.styleable#AbsListView_smoothScrollbar
     */
    virtual CARAPI SetSmoothScrollbarEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Returns the current state of the fast scroll feature.
     *
     * @return True if smooth scrollbar is enabled is enabled, FALSE otherwise.
     *
     * @see #setSmoothScrollbarEnabled(Boolean)
     */
    //@ViewDebug.ExportedProperty
    virtual CARAPI_(Boolean) IsSmoothScrollbarEnabled();

    /**
     * Set the listener that will receive notifications every time the list scrolls.
     *
     * @param l the scroll listener
     */
    virtual CARAPI SetOnScrollListener(
        /* [in] */ IAbsListViewOnScrollListener* l);

    /**
     * Indicates whether the children's drawing cache is used during a scroll.
     * By default, the drawing cache is enabled but this will consume more memory.
     *
     * @return TRUE if the scrolling cache is enabled, FALSE otherwise
     *
     * @see #setScrollingCacheEnabled(Boolean)
     * @see View#setDrawingCacheEnabled(Boolean)
     */
    //@ViewDebug.ExportedProperty
    virtual CARAPI_(Boolean) IsScrollingCacheEnabled();

    /**
     * Enables or disables the children's drawing cache during a scroll.
     * By default, the drawing cache is enabled but this will use more memory.
     *
     * When the scrolling cache is enabled, the caches are kept after the
     * first scrolling. You can manually clear the cache by calling
     * {@link android.view.ViewGroup#setChildrenDrawingCacheEnabled(Boolean)}.
     *
     * @param enabled TRUE to enable the scroll cache, FALSE otherwise
     *
     * @see #isScrollingCacheEnabled()
     * @see View#setDrawingCacheEnabled(Boolean)
     */
    virtual CARAPI SetScrollingCacheEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Enables or disables the type filter window. If enabled, typing when
     * this view has focus will filter the children to match the users input.
     * Note that the {@link Adapter} used by this view must implement the
     * {@link Filterable} interface.
     *
     * @param textFilterEnabled TRUE to enable type filtering, FALSE otherwise
     *
     * @see Filterable
     */
    virtual CARAPI SetTextFilterEnabled(
        /* [in] */ Boolean textFilterEnabled);

    /**
     * Indicates whether type filtering is enabled for this view
     *
     * @return TRUE if type filtering is enabled, FALSE otherwise
     *
     * @see #setTextFilterEnabled(Boolean)
     * @see Filterable
     */
    //@ViewDebug.ExportedProperty
    virtual CARAPI_(Boolean) IsTextFilterEnabled();

    //@Override
    CARAPI GetFocusedRect(
        /* [in, out] */ IRect* r);

    /**
     * Indicates whether the content of this view is pinned to, or stacked from,
     * the bottom edge.
     *
     * @return TRUE if the content is stacked from the bottom edge, FALSE otherwise
     */
    //@ViewDebug.ExportedProperty
    virtual CARAPI_(Boolean) IsStackFromBottom();

    /**
     * When stack from bottom is set to TRUE, the list fills its content starting from
     * the bottom of the view.
     *
     * @param stackFromBottom TRUE to pin the view's content to the bottom edge,
     *        FALSE to pin the view's content to the top edge
     */
    virtual CARAPI SetStackFromBottom(
        /* [in] */ Boolean stackFromBottom);

    //@Override
    CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    //@Override
    CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

    /**
     * Sets the initial value for the text filter.
     * @param filterText The text to use for the filter.
     *
     * @see #setTextFilterEnabled
     */
    virtual CARAPI SetFilterText(
        /* [in] */ const String& filterText);

    /**
     * Returns the list's text filter, if available.
     * @return the list's text filter or NULL if filtering isn't enabled
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) GetTextFilter();

    //@Override
    //@ViewDebug.ExportedProperty
    CARAPI_(AutoPtr<IView>) GetSelectedView();

    /**
     * List padding is the maximum of the normal view's padding and the padding of the selector.
     *
     * @see android.view.View#getPaddingTop()
     * @see #getSelector()
     *
     * @return The top list padding.
     */
    virtual CARAPI_(Int32) GetListPaddingTop();

    /**
     * List padding is the maximum of the normal view's padding and the padding of the selector.
     *
     * @see android.view.View#getPaddingBottom()
     * @see #getSelector()
     *
     * @return The bottom list padding.
     */
    virtual CARAPI_(Int32) GetListPaddingBottom();

    /**
     * List padding is the maximum of the normal view's padding and the padding of the selector.
     *
     * @see android.view.View#getPaddingLeft()
     * @see #getSelector()
     *
     * @return The left list padding.
     */
    virtual CARAPI_(Int32) GetListPaddingLeft();

    /**
     * List padding is the maximum of the normal view's padding and the padding of the selector.
     *
     * @see android.view.View#getPaddingRight()
     * @see #getSelector()
     *
     * @return The right list padding.
     */
    virtual CARAPI_(Int32) GetListPaddingRight();

    /**
     * Controls whether the selection highlight drawable should be drawn on top of the item or
     * behind it.
     *
     * @param onTop If TRUE, the selector will be drawn on the item it is highlighting. The default
     *        is FALSE.
     *
     * @attr ref android.R.styleable#AbsListView_drawSelectorOnTop
     */
    virtual CARAPI SetDrawSelectorOnTop(
        /* [in] */ Boolean onTop);

    /**
     * Set a Drawable that should be used to highlight the currently selected item.
     *
     * @param resID A Drawable resource to use as the selection highlight.
     *
     * @attr ref android.R.styleable#AbsListView_listSelector
     */
    virtual CARAPI SetSelector(
        /* [in] */ Int32 resID);

    virtual CARAPI SetSelector(
        /* [in] */ IDrawable* sel);

    /**
     * Returns the selector {@link android.graphics.drawable.Drawable} that is used to draw the
     * selection in the list.
     *
     * @return the drawable used to display the selector
     */
    virtual CARAPI_(AutoPtr<IDrawable>) GetSelector();

    virtual CARAPI SetScrollIndicators(
        /* [in] */ IView* up,
        /* [in] */ IView* down);

    //@Override
    CARAPI_(Boolean) VerifyDrawable(
        /* [in] */ IDrawable* dr);

    //@Override
    CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus);

    //@Override
    CARAPI_(Boolean) ShowContextMenuForChild(
        /* [in] */ IView* originalView);

    //@Override
    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    //@Override
    CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    /**
     * Maps a point to a position in the list.
     *
     * @param x X in local coordinate
     * @param y Y in local coordinate
     * @return The position of the item which contains the specified point, or
     *         {@link #AdapterView_INVALID_POSITION} if the point does not intersect an item.
     */
    virtual CARAPI_(Int32) PointToPosition(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    /**
     * Maps a point to a the rowId of the item which intersects that point.
     *
     * @param x X in local coordinate
     * @param y Y in local coordinate
     * @return The rowId of the item which contains the specified point, or {@link #AdapterView_INVALID_ROW_ID}
     *         if the point does not intersect an item.
     */
    virtual CARAPI_(Int64) PointToRowId(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    virtual CARAPI OnTouchModeChanged(
        /* [in] */ Boolean isInTouchMode);

    //@Override
    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* ev);

    using AdapterView::Draw;
    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI_(Boolean) OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* ev);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI AddTouchables(
        /* [in] */ IObjectContainer* views);

    /**
     * Smoothly scroll to the specified adapter position. The view will
     * scroll such that the indicated position is displayed.
     * @param position Scroll to this adapter position.
     */
    virtual CARAPI SmoothScrollToPosition(
        /* [in] */ Int32 position);

    /**
     * Smoothly scroll to the specified adapter position. The view will
     * scroll such that the indicated position is displayed, but it will
     * stop early if scrolling further would scroll boundPosition out of
     * view.
     * @param position Scroll to this adapter position.
     * @param boundPosition Do not scroll if it would move this adapter
     *          position out of view.
     */
    virtual CARAPI SmoothScrollToPosition(
        /* [in] */ Int32 position,
        /* [in] */ Int32 boundPosition);

    /**
     * Smoothly scroll by distance pixels over duration milliseconds.
     * @param distance Distance to scroll in pixels.
     * @param duration Duration of the scroll animation in milliseconds.
     */
    virtual CARAPI SmoothScrollBy(
        /* [in] */ Int32 distance,
        /* [in] */ Int32 duration);

    /**
     * Causes all the views to be rebuilt and redrawn.
     */
    virtual CARAPI InvalidateViews();

    /**
     * Return an InputConnection for editing of the filter text.
     */

    //zhangjingcheng , this is not complete
    //@Override
    CARAPI_(AutoPtr<IInputConnection>) OnCreateInputConnection(
        /* [in] */ IEditorInfo* outAttrs);

    /**
     * For filtering we proxy an input connection to an internal text editor,
     * and this allows the proxying to happen.
     */
    //@Override
    CARAPI_(Boolean) CheckInputConnectionProxy(
        /* [in] */ IView* view);

    /**
     * Clear the text filter.
     */
    virtual CARAPI ClearTextFilter();

    /**
     * Returns if the ListView currently has a text filter.
     */
    virtual CARAPI_(Boolean) HasTextFilter();

    virtual CARAPI OnGlobalLayout();

    /**
     * For our text watcher that is associated with the text filter.  Does
     * nothing.
     */
    virtual CARAPI BeforeTextChanged(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [in] */ Int32 after);

    /**
     * For our text watcher that is associated with the text filter. Performs
     * the actual filtering as the text changes, and takes care of hiding and
     * showing the popup displaying the currently entered filter text.
     */
    virtual CARAPI OnTextChanged(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 count);

    virtual CARAPI AfterTextChanged(
        /* [in] */ IEditable* s);

    virtual CARAPI OnFilterComplete(
        /* [in] */ Int32 count);

    //@Override
    CARAPI GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IViewGroupLayoutParams** params);

    /**
     * Puts the list or grid into transcript mode. In this mode the list or grid will always scroll
     * to the bottom to show new items.
     *
     * @param mode the transcript mode to set
     *
     * @see #AbsListView_TRANSCRIPT_MODE_DISABLED
     * @see #AbsListView_TRANSCRIPT_MODE_NORMAL
     * @see #AbsListView_TRANSCRIPT_MODE_ALWAYS_SCROLL
     */
    virtual CARAPI SetTranscriptMode(
        /* [in] */ Int32 mode);

    /**
     * Returns the current transcript mode.
     *
     * @return {@link #AbsListView_TRANSCRIPT_MODE_DISABLED}, {@link #AbsListView_TRANSCRIPT_MODE_NORMAL} or
     *         {@link #AbsListView_TRANSCRIPT_MODE_ALWAYS_SCROLL}
     */
    virtual CARAPI_(Int32) GetTranscriptMode();

    //@Override
    CARAPI_(Int32) GetSolidColor();

    /**
     * When set to a non-zero value, the cache color hint indicates that this list is always drawn
     * on top of a solid, single-color, opaque background
     *
     * @param color The background color
     */
    virtual CARAPI SetCacheColorHint(
        /* [in] */ Int32 color);

    /**
     * When set to a non-zero value, the cache color hint indicates that this list is always drawn
     * on top of a solid, single-color, opaque background
     *
     * @return The cache color hint
     */
    virtual CARAPI_(Int32) GetCacheColorHint();

    /**
     * Move all views (excluding headers and footers) held by this AbsListView into the supplied
     * List. This includes views displayed on the screen as well as views stored in AbsListView's
     * internal view recycler.
     *
     * @param views A list into which to put the reclaimed views
     */
    virtual CARAPI ReclaimViews(
        /* [in] */ IObjectContainer* views);

    /**
     * Sets the recycler listener to be notified whenever a View is set aside in
     * the recycler for later reuse. This listener can be used to free resources
     * associated to the View::
     *
     * @param listener The recycler listener to be notified of views set aside
     *        in the recycler.
     *
     * @see android.widget.AbsListView.RecycleBin
     * @see android.widget.AbsListView.RecyclerListener
     */
    virtual CARAPI SetRecyclerListener(
        /* [in] */ IRecyclerListener* listener);


    CARAPI SendAccessibilityEvent(
        /* [in] */ Int32 eventType);

    /**
     * @see #onInitializeAccessibilityEvent(AccessibilityEvent)
     *
     * Note: Called from the default {@link AccessibilityDelegate}.
     */
    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);


    CARAPI_(Boolean) PerformAccessibilityAction(
            /* [in] */ Int32 action,
            /* [in] */ IBundle* arguments);

    virtual CARAPI JumpDrawablesToCurrentState();

    // You must override all ShowContextMenu function in super class
    // to avoid hiding for C++；
    // The function will invoke the one in super class directly；
    CARAPI_(Boolean) ShowContextMenu();

    CARAPI_(Boolean) ShowContextMenu(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 metaState);

    CARAPI_(Boolean) OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI SetOverScrollEffectPadding(
        /* [in] */ Int32 leftPadding,
        /* [in] */ Int32 rightPadding);

    CARAPI RequestDisallowInterceptTouchEvent(
        /* [in] */ Boolean disallowIntercept);

     /**
     * The amount of friction applied to flings. The default value
     * is {@link ViewConfiguration#getScrollFriction}.
     *
     * @return A scalar dimensionless value representing the coefficient of
     *         friction.
     */
    virtual CARAPI SetFriction(
        /* [in] */ Float friction);

    /**
     * Sets a scale factor for the fling velocity. The initial scale
     * factor is 1.0.
     *
     * @param scale The scale factor to multiply the velocity by.
     */
    virtual CARAPI SetVelocityScale(
        /* [in] */ Float scale);

     /**
     * Smoothly scroll to the specified adapter position. The view will scroll
     * such that the indicated position is displayed <code>offset</code> pixels from
     * the top edge of the view. If this is impossible, (e.g. the offset would scroll
     * the first or last item beyond the boundaries of the list) it will get as close
     * as possible. The scroll will take <code>duration</code> milliseconds to complete.
     *
     * @param position Position to scroll to
     * @param offset Desired distance in pixels of <code>position</code> from the top
     *               of the view when scrolling is finished
     * @param duration Number of milliseconds to use for the scroll
     */
    virtual CARAPI SmoothScrollToPositionFromTop(
        /* [in] */ Int32 position,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 duration);

    /**
     * Smoothly scroll to the specified adapter position. The view will scroll
     * such that the indicated position is displayed <code>offset</code> pixels from
     * the top edge of the view. If this is impossible, (e.g. the offset would scroll
     * the first or last item beyond the boundaries of the list) it will get as close
     * as possible.
     *
     * @param position Position to scroll to
     * @param offset Desired distance in pixels of <code>position</code> from the top
     *               of the view when scrolling is finished
     */
    virtual CARAPI SmoothScrollToPositionFromTop(
        /* [in] */ Int32 position,
        /* [in] */ Int32 offset);

    /**
     * Sets up this AbsListView to use a remote views adapter which connects to a RemoteViewsService
     * through the specified intent.
     * @param intent the intent used to identify the RemoteViewsService for the adapter to connect to.
     */
    virtual CARAPI SetRemoteViewsAdapter(
        /* [in] */ IIntent* intent);

    /**
     * Sets up the onClickHandler to be used by the RemoteViewsAdapter when inflating RemoteViews
     *
     * @param handler The OnClickHandler to use when inflating RemoteViews.
     *
     * @hide
     */
    virtual CARAPI SetRemoteViewsOnClickHandler(
        /* [in] */ IRemoteViewsOnClickHandler* handler);

    /**
     * This defers a notifyDataSetChanged on the pending RemoteViewsAdapter if it has not
     * connected yet.
     */
    virtual CARAPI DeferNotifyDataSetChanged();

    /**
     * Called back when the adapter connects to the RemoteViewsService.
     */
    virtual CARAPI_(Boolean) OnRemoteAdapterConnected();

    /**
     * Called back when the adapter disconnects from the RemoteViewsService.
     */
    virtual CARAPI OnRemoteAdapterDisconnected();


protected:
    static CARAPI_(Int32) GetDistance(
        /* [in] */ IRect* sourceRect,
        /* [in] */ IRect* destRect,
        /* [in] */ Int32 direction);

    static CARAPI_(AutoPtr<IView>) RetrieveFromScrap(
        /* [in] */ List<AutoPtr<IView> >* scrapViews,
        /* [in] */ Int32 position);

protected:

    /**
     * If there is a selection returns false.
     * Otherwise resurrects the selection and returns true if resurrected.
     */
    CARAPI_(Boolean) ResurrectSelectionIfNeeded();

    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    //@Override
    CARAPI_(Boolean) IsVerticalScrollBarHidden();

    virtual CARAPI InvokeOnItemScrollListener();

    virtual CARAPI RequestLayoutIfNecessary();

    //@Override
    CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean gainFocus,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    //@Override
    CARAPI RequestLayout();

    virtual CARAPI_(void) ResetList();

    //@Override
    CARAPI_(Int32) ComputeVerticalScrollExtent();

    //@Override
    CARAPI_(Int32) ComputeVerticalScrollOffset();

    //@Override
    CARAPI_(Int32) ComputeVerticalScrollRange();

    //@Override
    CARAPI_(Float) GetTopFadingEdgeStrength();

    //@Override
    CARAPI_(Float) GetBottomFadingEdgeStrength();

    //@Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    //@Override
    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    //@Override
    CARAPI_(Boolean) SetFrame(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI_(void) LayoutChildren();

    virtual CARAPI_(void) UpdateScrollIndicators();

    virtual CARAPI_(AutoPtr<IView>) ObtainView(
        /* [in] */ Int32 position,
        /* [in] */ ArrayOf<Boolean>* isScrap);

    virtual CARAPI_(void) PositionSelector(
        /* [in] */ Int32 position,
        /* [in] */ IView* sel);

    //@Override
    virtual CARAPI_(void) DispatchDraw(
        /* [in] */ ICanvas* canvas);

    //@Override
    virtual CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    virtual CARAPI_(Boolean) TouchModeDrawsInPressedState();

    virtual CARAPI_(Boolean) ShouldShowSelector();

    virtual CARAPI_(void) KeyPressed();

    //@Override
    virtual CARAPI DrawableStateChanged();

    //@Override
    virtual CARAPI OnCreateDrawableState(
        /* [in] */ Int32 extraSpace,
        /* [out] */ ArrayOf<Int32>** drawableState);

    //@Override
    virtual CARAPI OnAttachedToWindow();

    //@Override
    virtual CARAPI OnDetachedFromWindow();

    virtual CARAPI_(AutoPtr<IContextMenuInfo>) CreateContextMenuInfo(
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    //@Override
    virtual CARAPI_(AutoPtr<IContextMenuInfo>) GetContextMenuInfo();

    //@Override
    virtual CARAPI_(void) DispatchSetPressed(
        /* [in] */ Boolean pressed);

    //@Override
    virtual CARAPI_(void) OnOverScrolled(
        /* [in] */ Int32 scrollX,
        /* [in] */ Int32 scrollY,
        /* [in] */ Boolean clampedX,
        /* [in] */ Boolean clampedY);

    virtual CARAPI_(void) ReportScrollStateChange(
        /* [in] */ Int32 newState);

    virtual CARAPI_(Boolean) TrackMotionScroll(
        /* [in] */ Int32 deltaY,
        /* [in] */ Int32 incrementalDeltaY);

    virtual CARAPI_(Int32) GetHeaderViewsCount();

    virtual CARAPI_(Int32) GetFooterViewsCount();

    virtual CARAPI FillGap(
        /* [in] */ Boolean down) = 0;

    virtual CARAPI_(void) HideSelector();

    virtual CARAPI_(Int32) ReconcileSelectedPosition();

    virtual CARAPI_(Int32) FindMotionRow(
        /* [in] */ Int32 y) = 0;

    virtual CARAPI_(Int32) FindClosestMotionRow(
        /* [in] */ Int32 y);

    virtual CARAPI SetSelectionInt(
        /* [in] */ Int32 position) = 0;

    virtual CARAPI_(Boolean) ResurrectSelection();

    //@Override
    virtual CARAPI_(void) HandleDataChanged();

    //@Override
    virtual CARAPI_(void) OnDisplayHint(
        /* [in] */ Int32 hint);

    //@Override
    virtual CARAPI_(Boolean) IsInFilterMode();

    virtual CARAPI_(Boolean) SendToTextFilter(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 count,
        /* [in] */ IKeyEvent* event);

    //@Override
    virtual CARAPI_(Boolean) CheckLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);


    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);/*com.android.internal.R.attr.absListViewStyle*/

    CARAPI_(Boolean) IsPaddingOffsetRequired();

    CARAPI_(Int32) GetLeftPaddingOffset();

    CARAPI_(Int32) GetTopPaddingOffset();

    CARAPI_(Int32) GetRightPaddingOffset();

    CARAPI_(Int32) GetBottomPaddingOffset();

    CARAPI GenerateDefaultLayoutParams(
        /* [out] */ IViewGroupLayoutParams** result);

    /**
     * Hints the RemoteViewsAdapter, if it exists, about which views are currently
     * being displayed by the AbsListView.
     */
    CARAPI_(void) SetVisibleRangeHint(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Allows RemoteViews to scroll relatively to a position.
     */
    CARAPI SmoothScrollByOffset(
        /* [in] */ Int32 position);


private:
    CARAPI InitAbsListView();

    CARAPI_(Boolean) ContentFits();

    CARAPI UseDefaultSelector();

    CARAPI_(Boolean) AcceptFilter();

    CARAPI_(void) PositionSelector(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI_(void) DrawSelector(
        /* [in] */ ICanvas* canvas);

    CARAPI_(Boolean) PerformLongPress(
        /* [in] */ IView* child,
        /* [in] */ Int32 longPressPosition,
        /* [in] */ Int64 longPressId);

    CARAPI_(Boolean) StartScrollIfNeeded(
        /* [in] */ Int32 y);

    CARAPI_(void) OnSecondaryPointerUp(
        /* [in] */ IMotionEvent* ev);

    CARAPI_(void) CreateScrollingCache();

    CARAPI_(void) ClearScrollingCache();

    CARAPI_(void) DismissPopup();

    CARAPI_(void) ShowPopup();

    CARAPI_(void) PositionPopup();

    CARAPI_(void) CreateTextFilter(
        /* [in] */ Boolean animateEntrance);

    CARAPI_(void) FinishGlows();

    /**
     * Perform a quick, in-place update of the checked or activated state
     * on all visible item views. This should only be called when a valid
     * choice mode is active.
     */
    CARAPI_(void) UpdateOnScreenCheckedViews();

    CARAPI_(void) UpdateSelectorState();

    CARAPI_(void) ScrollIfNeeded(
        /* [in] */ Int32 y);

    CARAPI_(void) InitOrResetVelocityTracker();

    CARAPI_(void) InitVelocityTrackerIfNotExists();

    CARAPI_(void) RecycleVelocityTracker();

    CARAPI_(void) SmoothScrollBy(
        /* [in] */ Int32 distance,
        /* [in] */ Int32 duration,
        /* [in] */ Boolean linear);

    CARAPI_(void) ConfirmCheckedPositionsById();


protected:
    /**
     * Indicates that we are not in the middle of a touch gesture
     */
    static const Int32 TOUCH_MODE_REST = -1;

    /**
     * Indicates we just received the touch event and we are waiting to see if the it is a tap or a
     * scroll gesture.
     */
    static const Int32 TOUCH_MODE_DOWN = 0;

    /**
     * Indicates the touch has been recognized as a tap and we are now waiting to see if the touch
     * is a longpress
     */
    static const Int32 TOUCH_MODE_TAP = 1;

    /**
     * Indicates we have waited for everything we can wait for, but the user's finger is still down
     */
    static const Int32 TOUCH_MODE_DONE_WAITING = 2;

    /**
     * Indicates the touch gesture is a scroll
     */
    static const Int32 TOUCH_MODE_SCROLL = 3;

    /**
     * Indicates the view is in the process of being flung
     */
    static const Int32 TOUCH_MODE_FLING = 4;

    /**
     * Indicates the touch gesture is an overscroll - a scroll beyond the beginning or end.
     */
    static const Int32 TOUCH_MODE_OVERSCROLL = 5;

    /**
     * Indicates the view is being flung outside of normal content bounds
     * and will spring back.
     */
    static const Int32 TOUCH_MODE_OVERFLING = 6;

    /**
     * Regular layout - usually an unsolicited layout from the view system
     */
    static const Int32 LAYOUT_NORMAL = 0;

    /**
     * Show the first item
     */
    static const Int32 LAYOUT_FORCE_TOP = 1;

    /**
     * Force the selected item to be on somewhere on the screen
     */
    static const Int32 LAYOUT_SET_SELECTION = 2;

    /**
     * Show the last item
     */
    static const Int32 LAYOUT_FORCE_BOTTOM = 3;

    /**
     * Make a mSelectedItem appear in a specific location and build the rest of
     * the views from there. The top is specified by mSpecificTop.
     */
    static const Int32 LAYOUT_SPECIFIC = 4;

    /**
     * Layout to sync as a result of a data change. Restore mSyncPosition to have its top
     * at mSpecificTop
     */
    static const Int32 LAYOUT_SYNC = 5;

    /**
     * Layout as a result of using the navigation keys
     */
    static const Int32 LAYOUT_MOVE_SELECTION = 6;

    /**
     * Content height divided by this is the overscroll limit.
     */
    static const Int32 OVERSCROLL_LIMIT_DIVISOR = 3;

    /**
     * How many positions in either direction we will search to try to
     * find a checked item with a stable ID that moved position across
     * a data set change. If the item isn't found it will be unselected.
     */
    static const Int32 CHECK_POSITION_SEARCH_DISTANCE = 20;

    /**
     * Used for smooth scrolling at a consistent rate
     */
    static AutoPtr<IInterpolator> sLinearInterpolator;

private:
    /**
     * Used to request a layout when we changed touch mode
     */
    static const Int32 TOUCH_MODE_UNKNOWN = -1;
    static const Int32 TOUCH_MODE_ON = 0;
    static const Int32 TOUCH_MODE_OFF = 1;
    static const Boolean PROFILE_SCROLLING = FALSE;
    static const Boolean PROFILE_FLINGING = FALSE;
    /**
     * Sentinel value for no current active pointer.
     * Used by {@link #mActivePointerId}.
     */
    static const Int32 INVALID_POINTER = -1;

protected:

    /**
     * Controls if/how the user may choose/check items in the list
     */
    Int32 mChoiceMode;

    /**
     * Controls CHOICE_MODE_MULTIPLE_MODAL. null when inactive.
     */
    AutoPtr<IActionMode> mChoiceActionMode;

    /**
     * Wrapper for the multiple choice mode callback; AbsListView needs to perform
     * a few extra actions around what application code does.
     */
    AutoPtr<MultiChoiceModeWrapper> mMultiChoiceModeCallback;

    /**
     * Running count of how many items are currently checked
     */
    Int32 mCheckedItemCount;

    /**
     * Running state of which positions are currently checked
     */
    AutoPtr< HashMap<Int32, Boolean> > mCheckStates;

    /**
     * Running state of which IDs are currently checked.
     * If there is a value for a given key, the checked state for that ID is true
     * and the value holds the last known position in the adapter for that id.
     */
    AutoPtr< HashMap<Int64, Int32> > mCheckedIdStates;

    /**
     * Controls how the next layout will happen
     */
    Int32 mLayoutMode;

    /**
     * Should be used by subclasses to listen to changes in the dataset
     */
    AutoPtr<AdapterDataSetObserver> mDataSetObserver;

    /**
     * The adapter containing the data to be displayed by this view
     */
    AutoPtr<IListAdapter> mAdapter;

    /**
     * If mAdapter != null, whenever this is true the adapter has stable IDs.
     */
    Boolean mAdapterHasStableIds;

    /**
     * Indicates whether the list selector should be drawn on top of the children or behind
     */
    Boolean mDrawSelectorOnTop;

    /**
     * The drawable used to draw the selector
     */
    AutoPtr<IDrawable> mSelector;

    /**
     * The current position of the selector in the list.
     */
    Int32 mSelectorPosition;

    /**
     * Defines the selector's location and dimension at drawing time
     */
    AutoPtr<IRect> mSelectorRect;

    /**
     * The data set used to store unused views that should be reused during the next layout
     * to avoid creating new ones
     */
    AutoPtr<RecycleBin> mRecycler;

    /**
     * The selection's left padding
     */
    Int32 mSelectionLeftPadding;

    /**
     * The selection's top padding
     */
    Int32 mSelectionTopPadding;

    /**
     * The selection's right padding
     */
    Int32 mSelectionRightPadding;

    /**
     * The selection's bottom padding
     */
    Int32 mSelectionBottomPadding;

    /**
     * This view's padding
     */
    AutoPtr<CRect> mListPadding;

    /**
     * Subclasses must retain their measure spec from onMeasure() into this member
     */
    Int32 mWidthMeasureSpec;

    /**
     * The top scroll indicator
     */
    AutoPtr<IView> mScrollUp;

    /**
     * The down scroll indicator
     */
    AutoPtr<IView> mScrollDown;

    /**
     * When the view is scrolling, this flag is set to TRUE to indicate subclasses that
     * the drawing cache was enabled on the children
     */
    Boolean mCachingStarted;

    Boolean mCachingActive;

    /**
     * The position of the view that received the down motion event
     */
    Int32 mMotionPosition;

    /**
     * The offset to the top of the mMotionPosition view when the down motion event was received
     */
    Int32 mMotionViewOriginalTop;

    /**
     * The desired offset to the top of the mMotionPosition view after a scroll
     */
    Int32 mMotionViewNewTop;

    /**
     * The X value associated with the the down motion event
     */
    Int32 mMotionX;

    /**
     * The Y value associated with the the down motion event
     */
    Int32 mMotionY;

    /**
     * One of TOUCH_MODE_REST, TOUCH_MODE_DOWN, TOUCH_MODE_TAP, TOUCH_MODE_SCROLL, or
     * TOUCH_MODE_DONE_WAITING
     */
    Int32 mTouchMode;

    /**
     * Y value from on the previous motion event (if any)
     */
    Int32 mLastY;

    /**
     * How far the finger moved before we started scrolling
     */
    Int32 mMotionCorrection;

    /**
     * The offset in pixels form the top of the AdapterView to the top
     * of the currently selected view. Used to save and restore state.
     */
    Int32 mSelectedTop;

    /**
     * Indicates whether the list is stacked from the bottom edge or
     * the top edge.
     */
    Boolean mStackFromBottom;

    /**
     * When set to TRUE, the list automatically discards the children's
     * bitmap cache after scrolling.
     */
    Boolean mScrollingCacheEnabled;

    /**
     * Whether or not to enable the fast scroll feature on this list
     */
    Boolean mFastScrollEnabled;

    /**
     * Keeps track of our accessory window
     */
    AutoPtr<IPopupWindow> mPopup;

    /**
     * Used with type filter window
     */
    AutoPtr<IEditText> mTextFilter;

    /**
     * The position to resurrect the selected position to.
     */
    Int32 mResurrectToPosition;

    /**
     * Maximum distance to record overscroll
     */
    Int32 mOverscrollMax;

    AutoPtr<ArrayOf<Boolean> > mIsScrap;

    /**
     * Maximum distance to overscroll by during edge effects
     */
    Int32 mOverscrollDistance;

    /**
     * Maximum distance to overfling during edge effects
     */
    Int32 mOverflingDistance;

    /**
     * Handles scrolling between positions within the list.
     */
    AutoPtr<PositionScroller> mPositionScroller;

    AutoPtr<IRunnable> mPositionScrollAfterLayout;

    /**
     * Track if we are currently attached to a window.
     */
    Boolean mIsAttached;

private:

    /**
     * The remote adapter containing the data to be displayed by this view to be set
     */
    AutoPtr<IRemoteViewsAdapter> mRemoteAdapter;

    /**
     * This flag indicates the a full notify is required when the RemoteViewsAdapter connects
     */
    Boolean mDeferNotifyDataSetChanged;


    /**
     * Optional callback to notify client when scroll position has changed
     */
    AutoPtr<IAbsListViewOnScrollListener> mOnScrollListener;

    /**
     * Determines speed during touch scrolling
     */
    AutoPtr<VelocityTracker> mVelocityTracker;

    /**
     * Handles one frame of a fling
     */
    AutoPtr<FlingRunnable> mFlingRunnable;

    /**
     * Indicates whether to use pixels-based or position-based scrollbar
     * properties.
     */
    Boolean mSmoothScrollbarEnabled;

    /**
     * Indicates that this view supports filtering
     */
    Boolean mTextFilterEnabled;

    /**
     * Indicates that this view is currently displaying a filtered view of the data
     */
    Boolean mFiltered;

    /**
     * Rectangle used for hit testing children
     */
    AutoPtr<IRect> mTouchFrame;

    AutoPtr<IContextMenuInfo> mContextMenuInfo;

    Int32 mLastTouchMode;
    Boolean mScrollProfilingStarted;
    Boolean mFlingProfilingStarted;

    /**
     * The StrictMode "critical time span" objects to catch animation
     * stutters.  Non-null when a time-sensitive animation is
     * in-flight.  Must call finish() on them when done animating.
     * These are no-ops on user builds.
     */
    AutoPtr<IStrictModeSpan> mScrollStrictSpan;

    AutoPtr<IStrictModeSpan> mFlingStrictSpan;

    /**
     * The last CheckForLongPress runnable we posted, if any
     */
    AutoPtr<CheckForLongPress> mPendingCheckForLongPress;

    /**
     * The last CheckForTap runnable we posted, if any
     */
    AutoPtr<IRunnable> mPendingCheckForTap;

    /**
     * The last CheckForKeyLongPress runnable we posted, if any
     */
    AutoPtr<CheckForKeyLongPress> mPendingCheckForKeyLongPress;

    /**
     * Acts upon click
     */
    AutoPtr<AbsListViewPerformClick> mPerformClick;

    /**
     * Delayed action for touch mode.
     */
    AutoPtr<IRunnable> mTouchModeReset;

    /**
     * This view is in transcript mode -- it shows the bottom of the list when the data
     * changes
     */
    Int32 mTranscriptMode;

    /**
     * Indicates that this list is always drawn on top of a solid, single-color, opaque
     * background
     */
    Int32 mCacheColorHint;

    /**
     * The select child's view (from the adapter's getView) is enabled.
     */
    Boolean mIsChildViewEnabled;

    /**
     * The last scroll state reported to clients through {@link OnScrollListener}.
     */
    Int32 mLastScrollState;

    /**
     * Helper object that renders and controls the fast scroll thumb.
     */
    AutoPtr<FastScroller> mFastScroller;

    Boolean mGlobalLayoutListenerAddedFilter;

    Int32 mTouchSlop;
    Float mDensityScale;

    AutoPtr<IInputConnection> mDefInputConnection;
    AutoPtr<IInputConnection> mPublicInputConnection; // InputConnectionWrapper

    AutoPtr<IRunnable> mClearScrollingCache;

    Int32 mMinimumVelocity;
    Int32 mMaximumVelocity;

    Float mVelocityScale;

    // True when the popup should be hidden because of a call to
    // dispatchDisplayHint()
    Boolean mPopupHidden;

    /**
     * ID of the active pointer. This is used to retain consistency during
     * drags/flings if multiple pointers are used.
     */
    Int32 mActivePointerId;

    // These two EdgeGlows are always set and used together.
    // Checking one for NULL is as good as checking both.

    /**
     * Tracks the state of the top edge glow.
     */
    AutoPtr<IEdgeEffect> mEdgeGlowTop;

    /**
     * Tracks the state of the bottom edge glow.
     */
    AutoPtr<IEdgeEffect> mEdgeGlowBottom;

    /**
     * An estimate of how many pixels are between the top of the list and
     * the top of the first position in the adapter, based on the last time
     * we saw it. Used to hint where to draw edge glows.
     */
    Int32 mFirstPositionDistanceGuess;

    /**
     * An estimate of how many pixels are between the bottom of the list and
     * the bottom of the last position in the adapter, based on the last time
     * we saw it. Used to hint where to draw edge glows.
     */
    Int32 mLastPositionDistanceGuess;

    /**
     * Used for determining when to cancel out of overscroll.
     */
    Int32 mDirection;

    /**
     * Tracked on measurement in transcript mode. Makes sure that we can still pin to
     * the bottom correctly on resizes.
     */
    Boolean mForceTranscriptScroll;

    Int32 mGlowPaddingLeft;
    Int32 mGlowPaddingRight;

    /**
     * Used for interacting with list items from an accessibility service.
     */
    AutoPtr<ListItemAccessibilityDelegate> mAccessibilityDelegate;

    Int32 mLastAccessibilityScrollEventFromIndex;
    Int32 mLastAccessibilityScrollEventToIndex;

    /**
     * Track the item count from the last time we handled a data change.
     */
    Int32 mLastHandledItemCount;

    /**
     * The saved state that we will be restoring from when we next sync.
     * Kept here so that if we happen to be asked to save our state before
     * the sync happens, we can return this existing data rather than losing
     * it.
     */
    AutoPtr<CAbsListViewSavedState> mPendingSync;

};

}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif //__ELASTOS_DROID_WIDGET_ABSLISTVIEW_H__
