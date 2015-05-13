
#ifndef __LISTVIEW_H__
#define __LISTVIEW_H__

#include "ext/frameworkext.h"
#include <elastos/Vector.h>
#include "widget/AbsListView.h"

namespace Elastos {
namespace Droid {
namespace Widget {

using Elastos::Utility::Vector;
using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IView;
using Elastos::Droid::Widget::IListAdapter;

/**
 * A view that shows items in a vertically scrolling list. The items
 * come from the {@link ListAdapter} associated with this view.
 *
 * <p>See the <a href="{@docRoot}resources/tutorials/views/hello-listview.html">List View
 * tutorial</a>.</p>
 *
 * @attr ref android.R.styleable#ListView_entries
 * @attr ref android.R.styleable#ListView_divider
 * @attr ref android.R.styleable#ListView_dividerHeight
 * @attr ref android.R.styleable#ListView_choiceMode
 * @attr ref android.R.styleable#ListView_headerDividersEnabled
 * @attr ref android.R.styleable#ListView_footerDividersEnabled
 */
class ListView : public AbsListView
{

protected:
    /**
    * Used to indicate a no preference for a position type.
    */
    static const Int32 NO_POSITION = -1;

private:
    /**
     * When arrow scrolling, ListView will never scroll more than this factor
     * times the height of the list.
     */
    static const float MAX_SCROLL_FACTOR = 0.33f;

    /**
     * When arrow scrolling, need a certain amount of pixels to preview next
     * items.  This is usually the fading edge, but if that is small enough,
     * we want to make sure we preview at least this many pixels.
     */
    static const Int32 MIN_SCROLL_PREVIEW_PIXELS = 2;

    static const String LISTVIEW_NAME;

public:
    /**
     * A class that represents a fixed view in a list, for example a header at the top
     * or a footer at the bottom.
     */
    class FixedViewInfo
        : public ElRefBase
        , public IFixedViewInfo
    {
    public:
        CAR_INTERFACE_DECL()

        FixedViewInfo();

        CARAPI SetView(
            /* [in] */ IView* view);

        CARAPI GetView(
            /* [out] */ IView** view);

        CARAPI SetData(
            /* [in] */ IInterface* data);

        CARAPI GetData(
            /* [out] */ IInterface** data);

        CARAPI SetSelectable(
            /* [in] */ Boolean selectable);

        CARAPI GetSelectable(
            /* [out] */ Boolean* selectable);

    public:
        /** The view to add to the list */
        AutoPtr<IView> mView;
        /** The data backing the view. This is returned from {@link ListAdapter#GetItem(Int32)}. */
        AutoPtr<IInterface> mData;
        /** <code>TRUE</code> if the fixed view should be selectable in the list */
        Boolean mIsSelectable;
    };

private:
    class FocusSelector
        : public ElRefBase
        , public IRunnable
    {
    public:
        CAR_INTERFACE_DECL()
    public:
        FocusSelector(
            /* [in] */ ListView* host);

        CARAPI_(AutoPtr<FocusSelector>) Setup(
            /* [in] */ Int32 position,
            /* [in] */ Int32 top);

        CARAPI Run();

    private:
        Int32 mPosition;
        Int32 mPositionTop;
        ListView* mHost;
    };

    /**
     * Holds results of focus aware arrow scrolling.
     */
    class ArrowScrollFocusResult
        : public ElRefBase
    {
        friend class ListView;
    public:
        CARAPI_(Int32) GetSelectedPosition();

        CARAPI_(Int32) GetAmountToScroll();

    protected:
        CARAPI_(void) Populate(
            /* [in] */ Int32 selectedPosition,
            /* [in] */ Int32 amountToScroll);

    private:
        Int32 mSelectedPosition;
        Int32 mAmountToScroll;
    };

public:
    ListView();

    ListView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::listViewStyle);

    ~ListView();

    virtual CARAPI_(Int32) GetMaxScrollAmount();

    virtual CARAPI AddHeaderView(
        /* [in] */ IView* v,
        /* [in] */ IInterface* data,
        /* [in] */ Boolean isSelectable);

    virtual CARAPI AddHeaderView(
        /* [in] */ IView* v);

    //@Override
    virtual CARAPI_(Int32) GetHeaderViewsCount();

    virtual CARAPI_(Boolean) RemoveHeaderView(
        /* [in] */ IView* v);

    virtual CARAPI AddFooterView(
        /* [in] */ IView* v,
        /* [in] */ IInterface* data,
        /* [in] */ Boolean isSelectable);

    virtual CARAPI AddFooterView(
        /* [in] */ IView* v);

    //@Override
    virtual CARAPI_(Int32) GetFooterViewsCount();

    virtual CARAPI_(Boolean) RemoveFooterView(
        /* [in] */ IView* v);

    //@Override
    virtual CARAPI_(AutoPtr<IAdapter>) GetAdapter();

    //@Override
    virtual CARAPI SetAdapter(
        /* [in] */ IAdapter* adapter);

    //@Override
    virtual CARAPI_(Boolean) RequestChildRectangleOnScreen(
        /* [in] */ IView* childView,
        /* [in] */ IRect* r,
        /* [in] */ Boolean immediate);

    //@Override
    virtual CARAPI SetSelection(
        /* [in] */ Int32 position);

    virtual CARAPI SetSelectionFromTop(
        /* [in] */ Int32 position,
        /* [in] */ Int32 y);

    virtual CARAPI SetSelectionAfterHeaderView();

    //@Override
    virtual CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    //@Override
    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    //@Override
    virtual CARAPI_(Boolean) OnKeyMultiple(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 repeatCount,
        /* [in] */ IKeyEvent* event);

    //@Override
    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI SetItemsCanFocus(
        /* [in] */ Boolean itemsCanFocus);

    virtual CARAPI_(Boolean) GetItemsCanFocus();

    //@Override
    virtual CARAPI_(Boolean) IsOpaque();

    //@Override
    CARAPI SetCacheColorHint(
        /* [in] */ Int32 color);

    virtual CARAPI_(AutoPtr<IDrawable>) GetDivider();

    virtual CARAPI SetDivider(
        /* [in] */ IDrawable* divider);

    /**
     * @return Returns the height of the divider that will be drawn between each item in the list.
     */
    CARAPI_(Int32) GetDividerHeight();

    virtual CARAPI SetDividerHeight(
        /* [in] */ Int32 height);

    virtual CARAPI SetHeaderDividersEnabled(
        /* [in] */ Boolean headerDividersEnabled);

    virtual CARAPI SetFooterDividersEnabled(
        /* [in] */ Boolean footerDividersEnabled);

    virtual CARAPI SetOverscrollHeader(
        /* [in] */ IDrawable* header);

    virtual CARAPI_(AutoPtr<IDrawable>) GetOverscrollHeader();

    virtual CARAPI SetOverscrollFooter(
        /* [in] */ IDrawable* footer);

    virtual CARAPI_(AutoPtr<IDrawable>) GetOverscrollFooter();

     /**
     * Sets up this AbsListView to use a remote views adapter which connects to a RemoteViewsService
     * through the specified intent.
     * @param intent the intent used to identify the RemoteViewsService for the adapter to connect to.
     */
    virtual CARAPI SetRemoteViewsAdapter(
        /* [in] */ IIntent* intent);

    /**
     * Smoothly scroll to the specified adapter position. The view will
     * scroll such that the indicated position is displayed.
     * @param position Scroll to this adapter position.
     */
    virtual CARAPI SmoothScrollToPosition(
        /* [in] */ Int32 position);

    using AbsListView::SmoothScrollToPosition;

    /**
     * Smoothly scroll to the specified adapter position offset. The view will
     * scroll such that the indicated position is displayed.
     * @param offset The amount to offset from the adapter position to scroll to.
     */
    virtual CARAPI SmoothScrollByOffset(
        /* [in] */ Int32 offset);

    CARAPI_(AutoPtr<ArrayOf<Int64> >) GetCheckItemIds();

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

protected:
    //@Override
    virtual CARAPI_(void) ResetList();

    //@Override
    virtual CARAPI FillGap(
        /* [in] */ Boolean down);

    //@Override
    virtual CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    //@Override
    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI_(Boolean) RecycleOnMeasure();

    virtual CARAPI_(Int32) MeasureHeightOfChildren(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 startPosition,
        /* [in] */ Int32 endPosition,
        /* [in] */ Int32 maxHeight,
        /* [in] */ Int32 disallowPartialChildPosition);

    //@Override
    virtual CARAPI_(Int32) FindMotionRow(
        /* [in] */ Int32 y);

    //@Override
    virtual CARAPI_(void) LayoutChildren();

    //@Override
    virtual CARAPI_(Boolean) CanAnimate();

    //@Override
    virtual CARAPI SetSelectionInt(
        /* [in] */ Int32 position);

    //@Override
    virtual CARAPI_(Int32) LookForSelectablePosition(
        /* [in] */ Int32 position,
        /* [in] */ Boolean lookDown);

    virtual CARAPI_(Boolean) PageScroll(
        /* [in] */ Int32 direction);

    virtual CARAPI_(Boolean) FullScroll(
        /* [in] */ Int32 direction);

    virtual CARAPI_(Boolean) ArrowScroll(
        /* [in] */ Int32 direction);

    virtual CARAPI_(void) DrawOverscrollHeader(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IDrawable* drawable,
        /* [in] */ IRect* bounds);

    virtual CARAPI_(void) DrawOverscrollFooter(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IDrawable* drawable,
        /* [in] */ IRect* bounds);

    //@Override
    virtual CARAPI_(void) DispatchDraw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI_(void) DrawDivider(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRect* bounds,
        /* [in] */ Int32 childIndex);

    //@Override
    virtual CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean gainFocus,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    //@Override
    virtual CARAPI OnFinishInflate();

    //@Override
    virtual CARAPI_(AutoPtr<IView>) FindViewTraversal(
        /* [in] */ Int32 id);

    virtual CARAPI_(AutoPtr<IView>) FindViewInHeadersOrFooters(
        /* [in] */ Vector<AutoPtr<FixedViewInfo> >& where,
        /* [in] */ Int32 id);

    //@Override
    virtual CARAPI_(AutoPtr<IView>) FindViewWithTagTraversal(
        /* [in] */ IInterface* tag);

    virtual CARAPI_(AutoPtr<IView>) FindViewWithTagInHeadersOrFooters(
        /* [in] */ Vector<AutoPtr<FixedViewInfo> >& where,
        /* [in] */ IInterface* tag);

    virtual CARAPI_(Boolean) DrawChild(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IView* child,
        /* [in] */ Int64 drawingTime);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::listViewStyle);

    virtual CARAPI_(AutoPtr<IView>) FindViewByPredicateTraversal(
        /* [in] */ IPredicate* predicate,
        /* [in] */ IView* childToSkip);

    CARAPI_(AutoPtr<IView>) FindViewByPredicateInHeadersOrFooters(
        /* [in] */ Vector<AutoPtr<FixedViewInfo> >& where,
        /* [in] */ IPredicate* predicate,
        /* [in] */ IView* childToSkip);

private:
    CARAPI_(void) AdjustViewsUpOrDown();

    CARAPI_(void) RemoveFixedViewInfo(
        /* [in] */ IView* v,
        /* [in] */ Vector<AutoPtr<FixedViewInfo> >& where);

    CARAPI_(void) ClearRecycledState(
        /* [in] */ Vector<AutoPtr<FixedViewInfo> >& infos);

    CARAPI_(Boolean) ShowingTopFadingEdge();

    CARAPI_(Boolean) ShowingBottomFadingEdge();

    CARAPI_(AutoPtr<IView>) FillDown(
        /* [in] */ Int32 pos,
        /* [in] */ Int32 nextTop);

    CARAPI_(AutoPtr<IView>) FillUp(
        /* [in] */ Int32 pos,
        /* [in] */ Int32 nextBottom);

    CARAPI_(AutoPtr<IView>) FillFromTop(
        /* [in] */ Int32 nextTop);

    CARAPI_(AutoPtr<IView>) FillFromMiddle(
        /* [in] */ Int32 childrenTop,
        /* [in] */ Int32 childrenBottom);

    CARAPI_(void) FillAboveAndBelow(
        /* [in] */ IView* sel,
        /* [in] */ Int32 position);

    CARAPI_(AutoPtr<IView>) FillFromSelection(
        /* [in] */ Int32 selectedTop,
        /* [in] */ Int32 childrenTop,
        /* [in] */ Int32 childrenBottom);

    CARAPI_(Int32) GetBottomSelectionPixel(
        /* [in] */ Int32 childrenBottom,
        /* [in] */ Int32 fadingEdgeLength,
        /* [in] */ Int32 selectedPosition);

    CARAPI_(Int32) GetTopSelectionPixel(
        /* [in] */ Int32 childrenTop,
        /* [in] */ Int32 fadingEdgeLength,
        /* [in] */ Int32 selectedPosition);

    CARAPI_(AutoPtr<IView>) MoveSelection(
        /* [in] */ IView* oldSel,
        /* [in] */ IView* newSel,
        /* [in] */ Int32 delta,
        /* [in] */ Int32 childrenTop,
        /* [in] */ Int32 childrenBottom);

    CARAPI_(void) MeasureScrapChild(
        /* [in] */ IView* child,
        /* [in] */ Int32 position,
        /* [in] */ Int32 widthMeasureSpec);

    CARAPI_(AutoPtr<IView>) FillSpecific(
        /* [in] */ Int32 position,
        /* [in] */ Int32 top);

    CARAPI_(void) CorrectTooHigh(
        /* [in] */ Int32 childCount);

    CARAPI_(void) CorrectTooLow(
        /* [in] */ Int32 childCount);

    CARAPI_(Boolean) IsDirectChildHeaderOrFooter(
        /* [in] */ IView* child);

    CARAPI_(AutoPtr<IView>) MakeAndAddView(
        /* [in] */ Int32 position,
        /* [in] */ Int32 y,
        /* [in] */ Boolean flow,
        /* [in] */ Int32 childrenLeft,
        /* [in] */ Boolean selected);

    CARAPI_(void) SetupChild(
        /* [in] */ IView* child,
        /* [in] */ Int32 position,
        /* [in] */ Int32 y,
        /* [in] */ Boolean flowDown,
        /* [in] */ Int32 childrenLeft,
        /* [in] */ Boolean selected,
        /* [in] */ Boolean recycled);

    CARAPI_(Boolean) CommonKey(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 count,
        /* [in] */ IKeyEvent* event);

    CARAPI HandleHorizontalFocusWithinListItem(
        /* [in] */ Int32 direction,
        /* [out] */ Boolean* rst);

    CARAPI_(Boolean) ArrowScrollImpl(
        /* [in] */ Int32 direction);

    CARAPI HandleNewSelectionChange(
        /* [in] */ IView* selectedView,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 newSelectedPosition,
        /* [in] */ Boolean newFocusAssigned);

    CARAPI_(void) MeasureAndAdjustDown(
        /* [in] */ IView* child,
        /* [in] */ Int32 childIndex,
        /* [in] */ Int32 numChildren);

    CARAPI_(void) MeasureItem(
        /* [in] */ IView* child);

    CARAPI_(void) RelayoutMeasuredItem(
        /* [in] */ IView* child);

    CARAPI_(Int32) GetArrowScrollPreviewLength();

    CARAPI_(Int32) AmountToScroll(
        /* [in] */ Int32 direction,
        /* [in] */ Int32 nextSelectedPosition);

    CARAPI_(Int32) LookForSelectablePositionOnScreen(
        /* [in] */ Int32 direction);

    CARAPI_(AutoPtr<ArrowScrollFocusResult>) ArrowScrollFocused(
        /* [in] */ Int32 direction);

    CARAPI_(Int32) PositionOfNewFocus(
        /* [in] */ IView* newFocus);

    CARAPI_(Boolean) IsViewAncestorOf(
        /* [in] */ IView* child,
        /* [in] */ IView* parent);

    CARAPI_(Int32) AmountToScrollToNewFocus(
        /* [in] */ Int32 direction,
        /* [in] */ IView* newFocus,
        /* [in] */ Int32 positionOfNewFocus);

    CARAPI_(Int32) DistanceToView(
        /* [in] */ IView* descendant);

    CARAPI_(void) ScrollListItemsBy(
        /* [in] */ Int32 amount);

    CARAPI_(AutoPtr<IView>) AddViewAbove(
        /* [in] */ IView* theView,
        /* [in] */ Int32 position);

    CARAPI_(AutoPtr<IView>) AddViewBelow(
        /* [in] */ IView* theView,
        /* [in] */ Int32 position);

    CARAPI_(AutoPtr<IView>) FindAccessibilityFocusedChild(
        /* [in] */ IView* focusedView);

protected:
    AutoPtr<IDrawable> mDivider;
    Int32 mDividerHeight;

    AutoPtr<IDrawable> mOverScrollHeader;
    AutoPtr<IDrawable> mOverScrollFooter;

private:
    Vector<AutoPtr<FixedViewInfo> > mHeaderViewInfos;
    Vector<AutoPtr<FixedViewInfo> > mFooterViewInfos;

    Boolean mIsCacheColorOpaque;
    Boolean mDividerIsOpaque;


    Boolean mHeaderDividersEnabled;
    Boolean mFooterDividersEnabled;

    Boolean mAreAllItemsSelectable;

    Boolean mItemsCanFocus;

    // used for temporary calculations.
    AutoPtr<CRect> mTempRect;
    AutoPtr<IPaint> mDividerPaint;

    // the single allocated result per list view; kinda cheesey but avoids
    // allocating these thingies too often.
    AutoPtr<ArrowScrollFocusResult> mArrowScrollFocusResult;

    // Keeps focused children visible through resizes
    AutoPtr<FocusSelector> mFocusSelector;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif//__LISTVIEW_H__
