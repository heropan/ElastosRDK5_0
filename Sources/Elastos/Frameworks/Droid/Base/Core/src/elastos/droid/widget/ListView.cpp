#include "elastos/droid/widget/ListView.h"
#include <elastos/core/Math.h>
#include "elastos/droid/os/Build.h"
#include "elastos/droid/content/pm/CApplicationInfo.h"
#include "elastos/droid/view/ViewRootImpl.h"
#include "elastos/droid/view/FocusFinder.h"
#include "elastos/droid/view/CViewGroupLayoutParams.h"
#include "elastos/droid/view/SoundEffectConstants.h"
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/widget/CArrayAdapter.h"
#include "elastos/droid/widget/CAbsListViewLayoutParams.h"
#include "elastos/droid/widget/CHeaderViewListAdapter.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::Etl::HashMap;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Content::Pm::EIID_IApplicationInfo;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::CApplicationInfo;
using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::View::ViewRootImpl;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_IViewParent;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::FocusFinder;
using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::SoundEffectConstants;
using Elastos::Droid::Widget::CArrayAdapter;
using Elastos::Droid::Widget::CAbsListViewLayoutParams;
using Elastos::Droid::Widget::CHeaderViewListAdapter;
using Elastos::Droid::View::EIID_IViewGroupLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {

const String ListView::LISTVIEW_NAME = String("ListView");

//==============================================================================
//            ListView::FixedViewInfo
//==============================================================================

CAR_INTERFACE_IMPL(ListView::FixedViewInfo, IFixedViewInfo)

ListView::FixedViewInfo::FixedViewInfo()
    : mIsSelectable(FALSE)
{
}

ECode ListView::FixedViewInfo::SetView(
    /* [in] */ IView* view)
{
    mView = view;
    return NOERROR;
}

ECode ListView::FixedViewInfo::GetView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = mView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode ListView::FixedViewInfo::SetData(
    /* [in] */ IInterface* data)
{
    mData = data;
    return NOERROR;
}

ECode ListView::FixedViewInfo::GetData(
    /* [out] */ IInterface** data)
{
    VALIDATE_NOT_NULL(data);
    *data = mData;
    REFCOUNT_ADD(*data);
    return NOERROR;
}

ECode ListView::FixedViewInfo::SetSelectable(
    /* [in] */ Boolean selectable)
{
    mIsSelectable = selectable;
    return NOERROR;
}

ECode ListView::FixedViewInfo::GetSelectable(
    /* [out] */ Boolean* selectable)
{
    VALIDATE_NOT_NULL(selectable);
    *selectable = mIsSelectable;
    return NOERROR;
}

//==============================================================================
//            ListView::FocusSelector
//==============================================================================
CAR_INTERFACE_IMPL(ListView::FocusSelector, IRunnable)

ListView::FocusSelector::FocusSelector(
    /* [in] */ ListView* host) :
    mHost(host)
{
}

AutoPtr<ListView::FocusSelector> ListView::FocusSelector::Setup(
    /* [in] */ Int32 position,
    /* [in] */ Int32 top)
{
    mPosition = position;
    mPositionTop = top;
    return this;
}

ECode ListView::FocusSelector::Run()
{
    mHost->SetSelectionFromTop(mPosition, mPositionTop);
    return NOERROR;
}

//==============================================================================
//            ListView::ArrowScrollFocusResult
//==============================================================================
Int32 ListView::ArrowScrollFocusResult::GetSelectedPosition()
{
    return mSelectedPosition;
}

Int32 ListView::ArrowScrollFocusResult::GetAmountToScroll()
{
    return mAmountToScroll;
}

void ListView::ArrowScrollFocusResult::Populate(
    /* [in] */ Int32 selectedPosition,
    /* [in] */ Int32 amountToScroll)
{
    mSelectedPosition = selectedPosition;
    mAmountToScroll = amountToScroll;
}

//==============================================================================
//            ListView
//==============================================================================

ListView::ListView()
    : mDivider(NULL)
    , mDividerHeight(0)
    , mOverScrollHeader(NULL)
    , mOverScrollFooter(NULL)
    , mIsCacheColorOpaque(FALSE)
    , mDividerIsOpaque(FALSE)
    , mHeaderDividersEnabled(FALSE)
    , mFooterDividersEnabled(FALSE)
    , mAreAllItemsSelectable(TRUE)
    , mItemsCanFocus(FALSE)
    , mArrowScrollFocusResult(NULL)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));
    mArrowScrollFocusResult = new ArrowScrollFocusResult();
}

ListView::ListView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : mDivider(NULL)
    , mDividerHeight(0)
    , mOverScrollHeader(NULL)
    , mOverScrollFooter(NULL)
    , mIsCacheColorOpaque(FALSE)
    , mDividerIsOpaque(FALSE)
    , mHeaderDividersEnabled(FALSE)
    , mFooterDividersEnabled(FALSE)
    , mAreAllItemsSelectable(TRUE)
    , mItemsCanFocus(FALSE)
    , mArrowScrollFocusResult(NULL)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));
    mArrowScrollFocusResult = new ArrowScrollFocusResult();
    Init(context, attrs, defStyle);
}

ListView::~ListView()
{
    if (mHeaderViewInfos.GetSize()) mHeaderViewInfos.Clear();
    if (mFooterViewInfos.GetSize()) mFooterViewInfos.Clear();
}

ECode ListView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AbsListView::Init(context, attrs, defStyle);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::ListView),
        ARRAY_SIZE(R::styleable::ListView));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributes(attrs, attrIds, defStyle, 0, (ITypedArray**)&a));

    AutoPtr<ArrayOf<ICharSequence*> > entries;
    a->GetTextArray(R::styleable::ListView_entries, (ArrayOf<ICharSequence*>**)&entries);
    if (entries != NULL) {
        AutoPtr<IObjectContainer> container;
        CObjectContainer::New((IObjectContainer**)&container);

        for (Int32 i = 0; i < entries->GetLength(); ++i) {
            if ((*entries)[i] != NULL) {
                container->Add((*entries)[i]);
            }
        }

        AutoPtr<IArrayAdapter> a;

        FAIL_RETURN(CArrayAdapter::New(context, R::layout::simple_list_item_1, container, (IArrayAdapter**)&a));
        SetAdapter((IAdapter*)a->Probe(EIID_IAdapter));
    }

    AutoPtr<IDrawable> d;
    a->GetDrawable(R::styleable::ListView_divider, (IDrawable**)&d);

    if (d != NULL) {
        SetDivider(d);
    }

    AutoPtr<IDrawable> osHeader;
    a->GetDrawable(R::styleable::ListView_overScrollHeader, (IDrawable**)&osHeader);
    if (osHeader != NULL) {
        SetOverscrollHeader(osHeader);
    }

    AutoPtr<IDrawable> osFooter;
    a->GetDrawable(R::styleable::ListView_overScrollFooter, (IDrawable**)&osFooter);
    if (osFooter != NULL) {
        SetOverscrollFooter(osFooter);
    }

    Int32 dividerHeight;
    a->GetDimensionPixelSize(R::styleable::ListView_dividerHeight, 0, &dividerHeight);
    if (dividerHeight != 0) {
        SetDividerHeight(dividerHeight);
    }

    a->GetBoolean(R::styleable::ListView_headerDividersEnabled, TRUE, &mHeaderDividersEnabled);
    a->GetBoolean(R::styleable::ListView_footerDividersEnabled, TRUE, &mFooterDividersEnabled);

    a->Recycle();

    return NOERROR;
}

Int32 ListView::GetMaxScrollAmount()
{
    return (Int32)(MAX_SCROLL_FACTOR * (mBottom - mTop));
}

void ListView::AdjustViewsUpOrDown()
{
    Int32 childCount = GetChildCount();
    Int32 delta;

    if (childCount > 0) {
        AutoPtr<IView> child;
        if (!mStackFromBottom) {
            child = GetChildAt(0);
            Int32 top;
            child->GetTop(&top);
            delta = top - mListPadding->mTop;
            if (mFirstPosition != 0) {
                delta -= mDividerHeight;
            }
            if (delta < 0) {
                delta = 0;
            }
        } else {
            child = GetChildAt(childCount - 1);
            Int32 bottom;
            child->GetBottom(&bottom);
            delta = bottom - (GetHeight() - mListPadding->mBottom);

            if (mFirstPosition + childCount < mItemCount) {
                delta += mDividerHeight;
            }

            if (delta > 0) {
                delta = 0;
            }
        }
        if (delta != 0) {
            OffsetChildrenTopAndBottom(-delta);
        }
    }
}

ECode ListView::AddHeaderView(
    /* [in] */ IView* v,
    /* [in] */ IInterface* data,
    /* [in] */ Boolean isSelectable)
{
    AutoPtr<IHeaderViewListAdapter> listAdapter = IHeaderViewListAdapter::Probe(mAdapter);

    if (mAdapter != NULL && listAdapter == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<FixedViewInfo> info = new FixedViewInfo();
    info->mView = v;
    info->mData = data;
    info->mIsSelectable = isSelectable;
    mHeaderViewInfos.PushBack(info);
    if (mAdapter != NULL && mDataSetObserver != NULL) {
        mDataSetObserver->OnChanged();
    }
    return NOERROR;
}

ECode ListView::AddHeaderView(
    /* [in] */ IView* v)
{
    return AddHeaderView(v, NULL, TRUE);
}

Int32 ListView::GetHeaderViewsCount()
{
    return mHeaderViewInfos.GetSize();
}

Boolean ListView::RemoveHeaderView(
    /* [in] */ IView* v)
{
    if (mHeaderViewInfos.IsEmpty() == FALSE) {
        Boolean result = FALSE;
        Boolean isRemoved;
        IHeaderViewListAdapter::Probe(mAdapter)->RemoveHeader(v, &isRemoved);
        if (mAdapter != NULL && isRemoved) {
            if (mDataSetObserver != NULL) {
                mDataSetObserver->OnChanged();
            }
            result = TRUE;
        }
        RemoveFixedViewInfo(v, mHeaderViewInfos);
        return result;
    }
    return FALSE;
}

void ListView::RemoveFixedViewInfo(
    /* [in] */ IView* v,
    /* [in] */ Vector<AutoPtr<FixedViewInfo> >& where)
{
    Vector<AutoPtr< FixedViewInfo> >::Iterator iter;
    for(iter = where.Begin(); iter != where.End(); ++iter) {
        if ((*iter)->mView.Get() == v) {
            where.Erase(iter);
            break;
        }
    }
}

ECode ListView::AddFooterView(
    /* [in] */ IView* v,
    /* [in] */ IInterface* data,
    /* [in] */ Boolean isSelectable)
{
    AutoPtr<FixedViewInfo> info = new FixedViewInfo();
    info->mView = v;
    info->mData = data;
    info->mIsSelectable = isSelectable;
    mFooterViewInfos.PushBack(info);
    if (mAdapter != NULL && mDataSetObserver != NULL) {
        mDataSetObserver->OnChanged();
    }
    return NOERROR;
}

ECode ListView::AddFooterView(
    /* [in] */ IView* v)
{
    return AddFooterView(v, NULL, TRUE);
}

Int32 ListView::GetFooterViewsCount()
{
    return mFooterViewInfos.GetSize();
}

Boolean ListView::RemoveFooterView(
    /* [in] */ IView* v)
{
    if (mFooterViewInfos.IsEmpty() == FALSE) {
        Boolean result = FALSE;
        Boolean isRemoved;
        IHeaderViewListAdapter::Probe(mAdapter)->RemoveFooter(v, &isRemoved);
        if (mAdapter != NULL && isRemoved) {
            if (mDataSetObserver != NULL) {
                mDataSetObserver->OnChanged();
            }
            result = TRUE;
        }
        RemoveFixedViewInfo(v, mFooterViewInfos);
        return result;
    }
    return FALSE;
}

AutoPtr<IAdapter> ListView::GetAdapter()
{
    return mAdapter;
}

ECode ListView::SetRemoteViewsAdapter(
    /* [in] */ IIntent* intent)
{
    AbsListView::SetRemoteViewsAdapter(intent);
    return NOERROR;
}

ECode ListView::SetAdapter(
    /* [in] */ IAdapter* adapter)
{
    if (mAdapter != NULL && mDataSetObserver != NULL) {
        mAdapter->UnregisterDataSetObserver(mDataSetObserver);
    }
    ResetList();
    mRecycler->Clear();

    if (mHeaderViewInfos.IsEmpty() == FALSE || mFooterViewInfos.IsEmpty() == FALSE) {
        AutoPtr< ArrayOf<IFixedViewInfo*> > first = ArrayOf<IFixedViewInfo*>::Alloc(mHeaderViewInfos.GetSize());
        for(Int32 i = 0; i < mHeaderViewInfos.GetSize(); ++i) {
            first->Set(i, (IFixedViewInfo*)(mHeaderViewInfos.At(i)));
        }
        AutoPtr< ArrayOf<IFixedViewInfo*> > second = ArrayOf<IFixedViewInfo*>::Alloc(mFooterViewInfos.GetSize());
        for(Int32 i = 0; i < mFooterViewInfos.GetSize(); ++i) {
            second->Set(i, (IFixedViewInfo*)(mFooterViewInfos.At(i)));
        }
        AutoPtr<IHeaderViewListAdapter> temp;
        CHeaderViewListAdapter::New(first, second, IListAdapter::Probe(adapter), (IHeaderViewListAdapter**)&temp);
        mAdapter = IListAdapter::Probe(temp);
    } else {
        mAdapter = IListAdapter::Probe(adapter);
    }

    mOldSelectedPosition = IAdapterView::INVALID_POSITION;
    mOldSelectedRowId = IAdapterView::INVALID_ROW_ID;

    AbsListView::SetAdapter(adapter);

    if (mAdapter != NULL) {
        mAdapter->AreAllItemsEnabled(&mAreAllItemsSelectable);
        mOldItemCount = mItemCount;
        mAdapter->GetCount(&mItemCount);
        CheckFocus();

        mDataSetObserver = new AdapterDataSetObserver(this);
        mAdapter->RegisterDataSetObserver(mDataSetObserver);

        Int32 count;
        mAdapter->GetViewTypeCount(&count);
        mRecycler->SetViewTypeCount(count);

        Int32 position;
        if (mStackFromBottom) {
            position = LookForSelectablePosition(mItemCount - 1, FALSE);
        } else {
            position = LookForSelectablePosition(0, TRUE);
        }

        SetSelectedPositionInt(position);
        SetNextSelectedPositionInt(position);

        if (mItemCount == 0) {
            CheckSelectionChanged();
        }
    } else {
        mAreAllItemsSelectable = TRUE;
        CheckFocus();
        CheckSelectionChanged();
    }
    RequestLayout();

    return NOERROR;
}

void ListView::ResetList()
{
    ClearRecycledState(mHeaderViewInfos);
    ClearRecycledState(mFooterViewInfos);

    AbsListView::ResetList();

    mLayoutMode = AbsListView::LAYOUT_NORMAL;
}

void ListView::ClearRecycledState(
    /* [in] */ Vector<AutoPtr<FixedViewInfo> >& infos)
{
    if (infos.IsEmpty()) {
        for(Vector<AutoPtr<FixedViewInfo> >::Iterator iter = infos.Begin();
            iter != infos.End(); ++iter) {
            AutoPtr<IView> child = (*iter)->mView;
            AutoPtr<IViewGroupLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
            AutoPtr<CAbsListViewLayoutParams> p =
                (CAbsListViewLayoutParams*)IAbsListViewLayoutParams::Probe(lp);

            if (p != NULL) {
                p->mRecycledHeaderFooter = FALSE;
            }
        }
    }
}

Boolean ListView::ShowingTopFadingEdge()
{
    Int32 listTop = mScrollY + mListPadding->mTop;
    Int32 top;
    GetChildAt(0)->GetTop(&top);
    return (mFirstPosition > 0) || (top > listTop);
}

Boolean ListView::ShowingBottomFadingEdge()
{
    Int32 childCount = GetChildCount();
    Int32 bottomOfBottomChild;
    GetChildAt(childCount - 1)->GetBottom(&bottomOfBottomChild);
    Int32 lastVisiblePosition = mFirstPosition + childCount - 1;
    Int32 listBottom = mScrollY + GetHeight() - mListPadding->mBottom;

    return (lastVisiblePosition < mItemCount - 1) || (bottomOfBottomChild < listBottom);
}

Boolean ListView::RequestChildRectangleOnScreen(
    /* [in] */ IView* childView,
    /* [in] */ IRect* r,
    /* [in] */ Boolean immediate)
{
    Int32 rectTopWithinChild;
    AutoPtr<CRect> rect = (CRect*)r;
    r->GetTop(&rectTopWithinChild);
    Int32 left, top, scrollX, scrollY;
    childView->GetLeft(&left);
    childView->GetTop(&top);
    childView->GetScrollX(&scrollX);
    childView->GetScrollY(&scrollY);

    Int32 height = GetHeight();
    Int32 listUnfadedTop = GetScrollY();
    Int32 listUnfadedBottom = listUnfadedTop + height;
    Int32 fadingEdge = GetVerticalFadingEdgeLength();

    if (ShowingBottomFadingEdge()) {
        if ((mSelectedPosition > 0) || (rectTopWithinChild > fadingEdge)) {
            listUnfadedTop += fadingEdge;
        }
    }

    Int32 childCount = GetChildCount();
    Int32 bottomOfBottomChild;
    GetChildAt(childCount - 1)->GetBottom(&bottomOfBottomChild);

    if (ShowingBottomFadingEdge()) {
        if ((mSelectedPosition < mItemCount -1)
            || (rect->mBottom < (bottomOfBottomChild - fadingEdge))) {
            listUnfadedBottom -= fadingEdge;
        }
    }

    Int32 scrollYDelta = 0;
    Int32 rectGetHeight;
    rect->GetHeight(&rectGetHeight);
    if (rect->mBottom > listUnfadedBottom && rect->mTop > listUnfadedTop) {
        if (rectGetHeight > height) {
            scrollYDelta += (rect->mTop - listUnfadedTop);
        } else {
            scrollYDelta += (rect->mBottom - listUnfadedBottom);
        }
        Int32 distanceToBottom = bottomOfBottomChild - listUnfadedBottom;
        scrollYDelta = Elastos::Core::Math::Min(scrollYDelta, distanceToBottom);
    } else if (rect->mTop < listUnfadedTop && rect->mBottom < listUnfadedBottom) {
        if (rectGetHeight > height) {
            scrollYDelta -= (listUnfadedBottom - rect->mBottom);
        } else {
            scrollYDelta -= (listUnfadedTop - rect->mTop);
        }

        Int32 scrollTop;
        GetChildAt(0)->GetTop(&scrollTop);
        Int32 deltaToTop = scrollTop - listUnfadedTop;
        scrollYDelta = Elastos::Core::Math::Max(scrollYDelta, deltaToTop);
    }

    Boolean scroll = scrollYDelta != 0;
    if (scroll) {
        ScrollListItemsBy(-scrollYDelta);
        PositionSelector(IAdapterView::INVALID_POSITION, childView);
        childView->GetTop(&mSelectedTop);
        Invalidate();
    }
    return scroll;
    return FALSE;
}


ECode ListView::FillGap(
    /* [in] */ Boolean down)
{
    Int32 count = GetChildCount();
    if (down) {
        Int32 paddingTop = 0;
        if ((mGroupFlags & ViewGroup::CLIP_TO_PADDING_MASK) == ViewGroup::CLIP_TO_PADDING_MASK) {
            paddingTop = GetListPaddingTop();
        }
        Int32 startOffSet;
        if (count > 0) {
            GetChildAt(count - 1)->GetBottom(&startOffSet);
            startOffSet += mDividerHeight;
        } else {
            startOffSet = paddingTop;
        }
        FillDown(mFirstPosition + count, startOffSet);
        CorrectTooHigh(GetChildCount());
    } else {
        Int32 paddingBottom = 0;
        if ((mGroupFlags & ViewGroup::CLIP_TO_PADDING_MASK) == ViewGroup::CLIP_TO_PADDING_MASK) {
            paddingBottom = GetListPaddingBottom();
        }
        Int32 startOffSet;
        if (count > 0) {
            GetChildAt(0)->GetTop(&startOffSet);
            startOffSet -= mDividerHeight;
        } else {
            startOffSet = GetHeight() - paddingBottom;
        }
        FillUp(mFirstPosition - 1, startOffSet);
        CorrectTooLow(GetChildCount());
    }
    return NOERROR;
}

AutoPtr<IView> ListView::FillDown(
    /* [in] */ Int32 pos,
    /* [in] */ Int32 nextTop)
{
    AutoPtr<IView> selectedView;
    Int32 end = mBottom - mTop;
    if ((mGroupFlags & ViewGroup::CLIP_TO_PADDING_MASK) == ViewGroup::CLIP_TO_PADDING_MASK) {
        end -= mListPadding->mBottom;
    }
    while(nextTop < end && pos < mItemCount) {
        Boolean selected = pos == mSelectedPosition;
        AutoPtr<IView> child = MakeAndAddView(pos , nextTop, TRUE, mListPadding->mLeft, selected);
        child->GetBottom(&nextTop);
        nextTop += mDividerHeight;
        if (selected) {
            selectedView = child;
        }
        pos++;
    }
    SetVisibleRangeHint(mFirstPosition, mFirstPosition + GetChildCount() - 1);
    return selectedView;
}

AutoPtr<IView> ListView::FillUp(
    /* [in] */ Int32 pos,
    /* [in] */ Int32 nextBottom)
{
    AutoPtr<IView> selectedView = NULL;

    Int32 end = 0;
    if ((mGroupFlags & ViewGroup::CLIP_TO_PADDING_MASK) == ViewGroup::CLIP_TO_PADDING_MASK) {
        end = mListPadding->mTop;
    }

    while(nextBottom > end && pos >= 0) {
        Boolean selected = pos == mSelectedPosition;
        AutoPtr<IView> child = MakeAndAddView(pos , nextBottom, FALSE, mListPadding->mLeft, selected);
        child->GetTop(&nextBottom);
        nextBottom -= mDividerHeight;
        if (selected) {
            selectedView = child;
        }
        pos--;
    }
    mFirstPosition = pos + 1;
    SetVisibleRangeHint(mFirstPosition, mFirstPosition + GetChildCount() - 1);
    return selectedView;
}

AutoPtr<IView> ListView::FillFromTop(
    /* [in] */ Int32 nextTop)
{
    mFirstPosition = Elastos::Core::Math::Min(mFirstPosition, mSelectedPosition);
    mFirstPosition = Elastos::Core::Math::Min(mFirstPosition, mItemCount - 1);
    if (mFirstPosition < 0) {
        mFirstPosition = 0;
    }
    return FillDown(mFirstPosition, nextTop);
}

AutoPtr<IView> ListView::FillFromMiddle(
    /* [in] */ Int32 childrenTop,
    /* [in] */ Int32 childrenBottom)
{
    Int32 height = childrenBottom - childrenTop;
    Int32 position = ReconcileSelectedPosition();
    AutoPtr<IView> sel = MakeAndAddView(position, childrenTop, TRUE, mListPadding->mLeft, TRUE);
    mFirstPosition = position;
    Int32 selHeight;
    sel->GetMeasuredHeight(&selHeight);
    if (selHeight <= height) {
        sel->OffsetTopAndBottom((height - selHeight) / 2);
    }
    FillAboveAndBelow(sel, position);

    if (!mStackFromBottom) {
        CorrectTooHigh(GetChildCount());
    } else {
        CorrectTooLow(GetChildCount());
    }
    return sel;
}

void ListView::FillAboveAndBelow(
    /* [in] */ IView* sel,
    /* [in] */ Int32 position)
{
    Int32 dividerHeight = mDividerHeight;
    Int32 top, bottom;
    sel->GetTop(&top);
    sel->GetBottom(&bottom);
    if (!mStackFromBottom) {
        FillUp(position - 1, top - dividerHeight);
        AdjustViewsUpOrDown();
        FillDown(position + 1, bottom + dividerHeight);
    } else {
        FillDown(position + 1, bottom + dividerHeight);
        AdjustViewsUpOrDown();
        FillUp(position - 1, top - dividerHeight);
    }
}

AutoPtr<IView> ListView::FillFromSelection(
    /* [in] */ Int32 selectedTop,
    /* [in] */ Int32 childrenTop,
    /* [in] */ Int32 childrenBottom)
{
    Int32 fadingEdgeLength = GetVerticalFadingEdgeLength();
    Int32 selectedPosition = mSelectedPosition;
    AutoPtr<IView> sel = NULL;
    Int32 topSelectionPixel = GetTopSelectionPixel(childrenTop, fadingEdgeLength, selectedPosition);
    Int32 bottomSelectionPixel = GetBottomSelectionPixel(childrenBottom, fadingEdgeLength, selectedPosition);
    sel = MakeAndAddView(selectedPosition, selectedTop, TRUE, mListPadding->mLeft, TRUE);
    Int32 top, bottom;
    sel->GetTop(&top);
    sel->GetBottom(&bottom);
    if (bottom > bottomSelectionPixel) {
        Int32 spaceAbove = top - topSelectionPixel;
        Int32 spaceBelow = bottom - bottomSelectionPixel;
        Int32 offset = Elastos::Core::Math::Min(spaceAbove, spaceBelow);
        sel->OffsetTopAndBottom(-offset);
    } else if (top < topSelectionPixel) {
        Int32 spaceAbove = topSelectionPixel - top;
        Int32 spaceBelow = bottomSelectionPixel - bottom;
        Int32 offset = Elastos::Core::Math::Min(spaceAbove, spaceBelow);
        sel->OffsetTopAndBottom(offset);
    }
    FillAboveAndBelow(sel, selectedPosition);
    if (!mStackFromBottom) {
        CorrectTooHigh(GetChildCount());
    } else {
        CorrectTooLow(GetChildCount());
    }
    return sel;
}

Int32 ListView::GetBottomSelectionPixel(
    /* [in] */ Int32 childrenBottom,
    /* [in] */ Int32 fadingEdgeLength,
    /* [in] */ Int32 selectedPosition)
{
    Int32 bottomSelectionPixel = childrenBottom;
    if (selectedPosition != mItemCount - 1) {
        bottomSelectionPixel -= fadingEdgeLength;
    }
    return bottomSelectionPixel;
}

Int32 ListView::GetTopSelectionPixel(
    /* [in] */ Int32 childrenTop,
    /* [in] */ Int32 fadingEdgeLength,
    /* [in] */ Int32 selectedPosition)
{
    Int32 topSelectionPixel = childrenTop;
    if (selectedPosition > 0) {
        topSelectionPixel += fadingEdgeLength;
    }
    return topSelectionPixel;
}

ECode ListView::SmoothScrollToPosition(
    /* [in] */ Int32 position)
{
    AbsListView::SmoothScrollToPosition(position);
    return NOERROR;
}

ECode ListView::SmoothScrollByOffset(
        /* [in] */ Int32 offset)
{
    AbsListView::SmoothScrollByOffset(offset);
    return NOERROR;
}

AutoPtr<IView> ListView::MoveSelection(
    /* [in] */ IView* oldSel,
    /* [in] */ IView* newSel,
    /* [in] */ Int32 delta,
    /* [in] */ Int32 childrenTop,
    /* [in] */ Int32 childrenBottom)
{
    Int32 fadingEdgeLength = GetVerticalFadingEdgeLength();
    Int32 selectedPosition = mSelectedPosition;
    AutoPtr<IView> sel = NULL;
    Int32 topSelectionPixel = GetTopSelectionPixel(childrenTop, fadingEdgeLength, selectedPosition);
    Int32 bottomSelectionPixel = GetBottomSelectionPixel(childrenTop, fadingEdgeLength, selectedPosition);
    Int32 oldTop, oldBottom, selTop, selBottom, newTop, newBottom;
    oldSel->GetTop(&oldTop);
    oldSel->GetBottom(&oldBottom);
    sel->GetTop(&selTop);
    sel->GetBottom(&selBottom);
    newSel->GetTop(&newTop);
    newSel->GetBottom(&newBottom);
    if (delta > 0) {
        oldSel = MakeAndAddView(selectedPosition - 1, oldTop, TRUE, mListPadding->mLeft, FALSE);
        Int32 dividerHeight = mDividerHeight;
        sel = MakeAndAddView(selectedPosition, oldBottom + dividerHeight, TRUE, mListPadding->mLeft, TRUE);
        if (selBottom > bottomSelectionPixel) {
            Int32 spaceAbove, spaceBelow;
            sel->GetTop(&spaceAbove);
            spaceAbove -= topSelectionPixel;
            sel->GetBottom(&spaceBelow);
            spaceBelow -= bottomSelectionPixel;
            Int32 halfVerticalSpace = (childrenBottom - childrenTop) / 2;
            Int32 offset = Elastos::Core::Math::Min(spaceAbove, spaceBelow);
            offset = Elastos::Core::Math::Min(offset, halfVerticalSpace);
            oldSel->OffsetTopAndBottom(-offset);
            sel->OffsetTopAndBottom(-offset);
        }

        if (!mStackFromBottom) {
            FillUp(mSelectedPosition - 2, selTop - dividerHeight);
            AdjustViewsUpOrDown();
            FillDown(mSelectedPosition + 1, selBottom + dividerHeight);
        } else {
            FillDown(mSelectedPosition + 1, selBottom + dividerHeight);
            AdjustViewsUpOrDown();
            FillUp(mSelectedPosition - 2, selTop - dividerHeight);
        }
    } else if (delta < 0) {
        if (newSel != NULL) {
            sel = MakeAndAddView(selectedPosition, newTop, TRUE, mListPadding->mLeft, TRUE);
        } else {
            sel = MakeAndAddView(selectedPosition, oldTop, FALSE, mListPadding->mLeft, TRUE);
        }
        if (selTop < topSelectionPixel) {
            Int32 spaceAbove = topSelectionPixel - selTop;
            Int32 spaceBelow = bottomSelectionPixel - selBottom;
            Int32 halfVerticalSpace = (childrenBottom - childrenTop) / 2;
            Int32 offset = Elastos::Core::Math::Min(spaceAbove, spaceBelow);
            offset = Elastos::Core::Math::Min(offset, halfVerticalSpace);
            sel->OffsetTopAndBottom(offset);
        }
        FillAboveAndBelow(sel, selectedPosition);
    } else {
        sel = MakeAndAddView(selectedPosition, oldTop, TRUE, mListPadding->mLeft, TRUE);
        if (oldTop < childrenTop) {
            if (selBottom < childrenTop + 20) {
                sel->OffsetTopAndBottom(childrenTop - selTop);
            }
        }
        FillAboveAndBelow(sel, selectedPosition);
    }
    return sel;
}

void ListView::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    if (GetChildCount() > 0) {
        AutoPtr<IView> focusedView = GetFocusedChild();
        if (focusedView != NULL) {
            Int32 childPosition = mFirstPosition + IndexOfChild(focusedView);
            Int32 childBottom;
            focusedView->GetBottom(&childBottom);
            Int32 offset = Elastos::Core::Math::Max(0, childBottom - (h - mPaddingTop));
            Int32 top;
            focusedView->GetTop(&top);
            top -= offset;
            if (mFocusSelector == NULL) {
                mFocusSelector = new FocusSelector(this);
            }
            Post(mFocusSelector->Setup(childPosition, top));
        }
    }
    AbsListView::OnSizeChanged(w, h, oldw, oldh);
}

void ListView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    AbsListView::OnMeasure(widthMeasureSpec, heightMeasureSpec);
    Int32 widthMode = View::MeasureSpec::GetMode(widthMeasureSpec);
    Int32 heightMode = View::MeasureSpec::GetMode(heightMeasureSpec);
    Int32 widthSize = View::MeasureSpec::GetSize(widthMeasureSpec);
    Int32 heightSize = View::MeasureSpec::GetSize(heightMeasureSpec);

    Int32 childWidth, childHeight, childState;
    childWidth = childHeight = childState = 0;
    if (mAdapter == NULL) {
        mItemCount = 0;
    } else {
        mAdapter->GetCount(&mItemCount);
    }

    if (mItemCount > 0 && (widthMode == View::MeasureSpec::UNSPECIFIED ||
        heightMode == View::MeasureSpec::UNSPECIFIED)) {

        AutoPtr<IView> child = ObtainView(0, mIsScrap.Get());
        MeasureScrapChild(child, 0, widthMeasureSpec);
        child->GetMeasuredWidth(&childWidth);
        child->GetMeasuredHeight(&childHeight);
        Int32 measuredState;
        child->GetMeasuredState(&measuredState);
        childState = CombineMeasuredStates(childState, measuredState);

        AutoPtr<IViewGroupLayoutParams> params;
        child->GetLayoutParams((IViewGroupLayoutParams**)&params);
        AutoPtr<CAbsListViewLayoutParams> p =
            (CAbsListViewLayoutParams*)IAbsListViewLayoutParams::Probe(params);
        if (RecycleOnMeasure() && mRecycler->ShouldRecycleViewType(p->mViewType)) {
            mRecycler->AddScrapView(child, -1);
        }
    }

    if (widthMode == View::MeasureSpec::UNSPECIFIED) {
        widthSize = mListPadding->mLeft + mListPadding->mRight + childWidth + GetVerticalScrollbarWidth();
    } else {
        widthSize |= (childState & IView::MEASURED_STATE_MASK);
    }

    if (heightMode == View::MeasureSpec::UNSPECIFIED) {
        heightSize = mListPadding->mTop + mListPadding->mBottom + childHeight +
            GetVerticalFadingEdgeLength() * 2;
    }

    if (heightMode == View::MeasureSpec::AT_MOST) {
        heightSize = MeasureHeightOfChildren(widthMeasureSpec, 0, NO_POSITION, heightSize, -1);
    }

    SetMeasuredDimension(widthSize, heightSize);
    mWidthMeasureSpec = widthMeasureSpec;
}

void ListView::MeasureScrapChild(
    /* [in] */ IView* child,
    /* [in] */ Int32 position,
    /* [in] */ Int32 widthMeasureSpec)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    AutoPtr<IAbsListViewLayoutParams> p;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    if(lp != NULL)
    {
         p = (IAbsListViewLayoutParams*)(lp->Probe(EIID_IAbsListViewLayoutParams));
    }
    if (p == NULL || lp == NULL){
        lp = NULL;
        GenerateDefaultLayoutParams((IViewGroupLayoutParams**)&lp);
        p = (IAbsListViewLayoutParams*)(lp->Probe(EIID_IAbsListViewLayoutParams));
        child->SetLayoutParams(lp);
    }

    Int32 viewType;
    mAdapter->GetItemViewType(position, &viewType);
    p->SetViewType(viewType);
    p->SetForceAdd(TRUE);

    Int32 pWidth;
    p->GetWidth(&pWidth);
    Int32 childWidthSpec = ViewGroup::GetChildMeasureSpec(widthMeasureSpec,
        mListPadding->mLeft + mListPadding->mRight, pWidth);

    Int32 lpHeight;
    p->GetHeight(&lpHeight);
    Int32 childHeightSpec;
    if (lpHeight > 0) {
        childHeightSpec = View::MeasureSpec::MakeMeasureSpec(lpHeight, View::MeasureSpec::EXACTLY);
    } else {
        childHeightSpec = View::MeasureSpec::MakeMeasureSpec(0, View::MeasureSpec::UNSPECIFIED);
    }
    child->Measure(childWidthSpec, childHeightSpec);
}

Boolean ListView::RecycleOnMeasure()
{
    return TRUE;
}

Int32 ListView::MeasureHeightOfChildren(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 startPosition,
    /* [in] */ Int32 endPosition,
    /* [in] */ Int32 maxHeight,
    /* [in] */ Int32 disallowPartialChildPosition)
{
    AutoPtr<IListAdapter> adapter = mAdapter;
    if (adapter == NULL) {
        return mListPadding->mTop + mListPadding->mBottom;
    }
    Int32 returnedHeight = mListPadding->mTop + mListPadding->mBottom;
    Int32 dividerHeight;
    if ((mDividerHeight > 0) && mDivider != NULL) {
        dividerHeight = mDividerHeight;
    } else {
        dividerHeight = 0;
    }
    Int32 prevHeightWithoutPartialChild = 0;
    AutoPtr<IView> child;
    Int32 count;
    adapter->GetCount(&count);
    endPosition = (endPosition == NO_POSITION) ? count - 1 : endPosition;
    Boolean recyle = RecycleOnMeasure();

    for(Int32 i = startPosition; i <= endPosition; ++i) {
        child = ObtainView(i, mIsScrap);
        MeasureScrapChild(child, i, widthMeasureSpec);

        if (i > 0) {
            returnedHeight += dividerHeight;
        }
        AutoPtr<IViewGroupLayoutParams> lp;
        child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

        if (recyle && mRecycler->ShouldRecycleViewType(((CAbsListViewLayoutParams*)lp.Get())->mViewType)) {
            mRecycler->AddScrapView(child, -1);
        }

        Int32 getChildHeight;
        child->GetMeasuredHeight(&getChildHeight);
        returnedHeight += getChildHeight;

        if (returnedHeight >= maxHeight) {
            return (disallowPartialChildPosition >= 0)
                && (i > disallowPartialChildPosition)
                && (prevHeightWithoutPartialChild > 0)
                && (returnedHeight != maxHeight) ? prevHeightWithoutPartialChild :maxHeight;
        }
        if ((disallowPartialChildPosition >= 0) && (i >= disallowPartialChildPosition)) {
            prevHeightWithoutPartialChild = returnedHeight;
        }
    }
    return returnedHeight;
}

Int32 ListView::FindMotionRow(
    /* [in] */ Int32 y)
{
    Int32 childCount = GetChildCount();
    if (childCount > 0) {
        if (!mStackFromBottom) {
            for(Int32 i = 0; i < childCount; i++) {
                AutoPtr<IView> v = GetChildAt(i);
                Int32 bottom;
                v->GetBottom(&bottom);
                if (y <= bottom) {
                    return mFirstPosition + i;
                }
            }
        } else {
            for(Int32 i = childCount - 1; i >= 0; i--) {
                 AutoPtr<IView> v = GetChildAt(i);
                 Int32 top;
                 v->GetTop(&top);
                 if (y >= top) {
                    return mFirstPosition + i;
                 }
            }
        }
    }
    return IAdapterView::INVALID_POSITION;
}

AutoPtr<IView> ListView::FillSpecific(
    /* [in] */ Int32 position,
    /* [in] */ Int32 top)
{
    Boolean tempIsSelected = position == mSelectedPosition;
    AutoPtr<IView> temp = MakeAndAddView(position, top, TRUE, mListPadding->mLeft, tempIsSelected);
    mFirstPosition = position;

    AutoPtr<IView> above;
    AutoPtr<IView> below;
    Int32 dividerHeight = mDividerHeight;
    Int32 tempTop, tempBottom;
    temp->GetTop(&tempTop);
    temp->GetBottom(&tempBottom);
    if (!mStackFromBottom) {
        above = FillUp(position - 1, tempTop - dividerHeight);
        AdjustViewsUpOrDown();
        below = FillDown(position + 1, tempBottom + dividerHeight);
        Int32 childCount = GetChildCount();
        if (childCount > 0) {
            CorrectTooHigh(childCount);
        }
    } else {
        below = FillDown(position + 1, tempBottom + dividerHeight);
        AdjustViewsUpOrDown();
        above = FillUp(position - 1, tempTop - dividerHeight);
        Int32 childCount = GetChildCount();
        if (childCount > 0) {
            CorrectTooLow(childCount);
        }
    }

    if (tempIsSelected) {
        return temp;
    } else if (above != NULL) {
        return above;
    } else {
        return below;
    }
}

void ListView::CorrectTooHigh(
    /* [in] */ Int32 childCount)
{
    Int32 lastPosition = mFirstPosition + childCount - 1;
    if (lastPosition == mItemCount - 1 && childCount > 0) {
        AutoPtr<IView> lastChild = GetChildAt(childCount - 1);
        Int32 lastBottom;
        lastChild->GetBottom(&lastBottom);
        Int32 end = (mBottom - mTop) - mListPadding->mBottom;
        Int32 bottomOffSet = end - lastBottom;
        AutoPtr<IView> firstChild = GetChildAt(0);
        Int32 firstTop;
        firstChild->GetTop(&firstTop);

        if (bottomOffSet > 0 && (mFirstPosition > 0 || firstTop < mListPadding->mTop)) {
            if (mFirstPosition == 0) {
                bottomOffSet = Elastos::Core::Math::Min(bottomOffSet, mListPadding->mTop - firstTop);
            }

            OffsetChildrenTopAndBottom(bottomOffSet);
            Int32 firstChildTop;
            firstChild->GetTop(&firstChildTop);
            if (mFirstPosition > 0) {
                FillUp(mFirstPosition - 1, firstChildTop - mDividerHeight);
                AdjustViewsUpOrDown();
            }
        }
    }
}

void ListView::CorrectTooLow(
    /* [in] */ Int32 childCount)
{
    if (mFirstPosition == 0 && childCount > 0) {
        AutoPtr<IView> firstChild = GetChildAt(0);
        Int32 firstTop;
        firstChild->GetTop(&firstTop);
        Int32 end = (mBottom - mTop) - mListPadding->mBottom;
        Int32 topOffSet = firstTop - mListPadding->mTop;
        AutoPtr<IView> lastChild = GetChildAt(childCount - 1);
        Int32 lastBottom;
        lastChild->GetBottom(&lastBottom);
        Int32 lastPosition = mFirstPosition + childCount - 1;

        if (topOffSet > 0) {
            if (lastPosition < mItemCount - 1 || lastBottom > end) {
                if (lastPosition == mItemCount - 1) {
                    topOffSet = Elastos::Core::Math::Min(topOffSet, lastBottom - end);
                }
                if (lastPosition < mItemCount - 1) {
                    Int32 lastChildBottom;
                    lastChild->GetBottom(&lastChildBottom);
                    FillDown(lastPosition + 1, lastChildBottom + mDividerHeight);
                    AdjustViewsUpOrDown();
                }
            }  else if (lastPosition == mItemCount - 1) {
                AdjustViewsUpOrDown();
            }
        }
    }
}

void ListView::LayoutChildren()
{
    Boolean blockLayoutRequests = mBlockLayoutRequests;
    if (!blockLayoutRequests) {
        mBlockLayoutRequests = TRUE;
    } else {
        return;
    }

    //try {
    AbsListView::LayoutChildren();
    Invalidate();
    if (mAdapter == NULL) {
        ResetList();
        InvokeOnItemScrollListener();
        if (!blockLayoutRequests) {
            mBlockLayoutRequests = FALSE;
        }
        return;
    }
    Int32 childrenTop = mListPadding->mTop;
    Int32 childrenBottom = mBottom - mTop - mListPadding->mBottom;
    Int32 childCount = GetChildCount();
    Int32 index = 0, delta = 0;
    AutoPtr<IView> sel, oldSel, oldFirst, newSel;
    AutoPtr<IView> focusLayoutRestoreView;
    AutoPtr<IAccessibilityNodeInfo> accessibilityFocusLayoutRestoreNode;
    AutoPtr<IView> accessibilityFocusLayoutRestoreView;
    Int32 accessibilityFocusPosition = IAdapterView::INVALID_POSITION;
    switch(mLayoutMode) {
    case AbsListView::LAYOUT_SET_SELECTION:
        index = mNextSelectedPosition - mFirstPosition;
        if (index >= 0 && index < childCount) {
            newSel = GetChildAt(index);
        }
        break;
    case AbsListView::LAYOUT_FORCE_TOP:
    case AbsListView::LAYOUT_FORCE_BOTTOM:
    case AbsListView::LAYOUT_SPECIFIC:
    case AbsListView::LAYOUT_SYNC:
        break;
    case AbsListView::LAYOUT_MOVE_SELECTION:
    default:
        index = mSelectedPosition - mFirstPosition;
        if (index >= 0 && index < childCount) {
            oldSel = GetChildAt(index);
        }
        oldFirst = GetChildAt(0);
        if (mNextSelectedPosition >= 0) {
            delta = mNextSelectedPosition - mSelectedPosition;
        }
        newSel = GetChildAt(index + delta);
    }

    Boolean dataChanged = mDataChanged;
    Int32 adaCount;
    mAdapter->GetCount(&adaCount);
    if (dataChanged) {
        HandleDataChanged();
    }
    if (mItemCount == 0) {
        ResetList();
        InvokeOnItemScrollListener();
        if (!blockLayoutRequests) {
            mBlockLayoutRequests = FALSE;
        }
        return;
    } else if ( mItemCount != adaCount) {
        Slogger::E("ListView", "IllegalStateException: The content of the adapter has changed but " \
            "ListView did not receive a notification. Make sure the content of " \
            "your adapter is not modified from a background thread, but only " \
            "from the UI thread. [in ListView(0x%08x), with Adapter(%p)]", mID, mAdapter.Get());

        if (!blockLayoutRequests) {
            mBlockLayoutRequests = FALSE;
        }
        return;
        //return E_ILLEGAL_STATE_EXCEPTION;
    }
    SetSelectedPositionInt(mNextSelectedPosition);
    Int32 firstPosition = mFirstPosition;
    AutoPtr<IView> focusLayoutRestoreDirectChild;
    if (dataChanged) {
        for (Int32 i = 0; i < childCount; i++) {
            mRecycler->AddScrapView(GetChildAt(i), firstPosition + i);
        }
    } else {
        mRecycler->FillActiveViews(childCount, firstPosition);
    }
    AutoPtr<IView> focusedChild = GetFocusedChild();
    if (focusedChild != NULL) {
        if (!dataChanged || IsDirectChildHeaderOrFooter(focusedChild)) {
            focusLayoutRestoreDirectChild = focusedChild;
            focusLayoutRestoreView = FindFocus();
            if (focusLayoutRestoreView != NULL) {
                focusLayoutRestoreView->OnStartTemporaryDetach();
            }
        }
        View::RequestFocus();
    }
    AutoPtr<ViewRootImpl> viewRootImpl = GetViewRootImpl();
    if (viewRootImpl != NULL) {
        AutoPtr<IView> accessFocusedView = viewRootImpl->GetAccessibilityFocusedHost();
        if (accessFocusedView != NULL) {
            AutoPtr<IView> accessFocusedChild = FindAccessibilityFocusedChild(accessFocusedView);
            if (accessFocusedChild != NULL) {
                if (!dataChanged || IsDirectChildHeaderOrFooter(accessFocusedChild)) {
                    accessibilityFocusLayoutRestoreView = accessFocusedView;
                    accessibilityFocusLayoutRestoreNode = viewRootImpl->GetAccessibilityFocusedVirtualView();
                }else {
                    accessibilityFocusPosition = GetPositionForView(accessFocusedChild);
                }
            }
        }
    }
    DetachAllViewsFromParent();
    mRecycler->RemoveSkippedScrap();
    switch (mLayoutMode) {
    case AbsListView::LAYOUT_SET_SELECTION:
        if (newSel != NULL) {
            Int32 newSelTop;
            newSel->GetTop(&newSelTop);
            sel = FillFromSelection(newSelTop, childrenTop, childrenBottom);
        } else {
            sel = FillFromMiddle(childrenTop, childrenBottom);
        }
        break;
    case AbsListView::LAYOUT_SYNC:
        sel = FillSpecific(mSyncPosition, mSpecificTop);
        break;
    case AbsListView::LAYOUT_FORCE_BOTTOM:
        sel = FillUp(mItemCount - 1, childrenBottom);
        AdjustViewsUpOrDown();
        break;
    case AbsListView::LAYOUT_FORCE_TOP:
        mFirstPosition = 0;
        sel = FillFromTop(childrenTop);
        AdjustViewsUpOrDown();
        break;
    case AbsListView::LAYOUT_SPECIFIC:
        sel = FillSpecific(ReconcileSelectedPosition(), mSpecificTop);
        break;
    case AbsListView::LAYOUT_MOVE_SELECTION:
        sel = MoveSelection(oldSel, newSel, delta, childrenTop, childrenBottom);
        break;
    default:
        if (childCount == 0) {
            if (!mStackFromBottom) {
                Int32 position = LookForSelectablePosition(0, TRUE);
                SetSelectedPositionInt(position);
                sel = FillFromTop(childrenTop);
            } else {
                Int32 position = LookForSelectablePosition(mItemCount - 1, FALSE);
                SetSelectedPositionInt(position);
                sel = FillUp(mItemCount - 1, childrenBottom);
            }
        } else {
            Int32 top = childrenTop;
            if (mSelectedPosition >= 0 && mSelectedPosition < mItemCount) {
                if (oldSel != NULL) {
                    oldSel->GetTop(&top);
                }
                sel = FillSpecific(mSelectedPosition, top);
            } else if (mFirstPosition < mItemCount) {
                if (oldFirst != NULL) {
                    oldFirst->GetTop(&top);
                }
                sel = FillSpecific(mFirstPosition, top);
            } else {
                sel = FillSpecific(0, childrenTop);
            }
        }
        break;
    }
    mRecycler->ScrapActiveViews();
    if (sel != NULL) {
        Boolean IsHasFocus;
        sel->HasFocus(&IsHasFocus);
        if (mItemsCanFocus && HasFocus() && IsHasFocus) {
            Boolean fLRVRequestFocus = FALSE, selRequestFocus = FALSE;
            if (focusLayoutRestoreView != NULL) focusLayoutRestoreView->RequestFocus(&fLRVRequestFocus);
            sel->RequestFocus(&selRequestFocus);

            Boolean focusWasTaken = ((sel == focusLayoutRestoreDirectChild)
                    && fLRVRequestFocus) || selRequestFocus;
            if (!focusWasTaken) {
                AutoPtr<IView> focused = GetFocusedChild();
                if (focused != NULL) {
                    focused->ClearFocus();
                }
                PositionSelector(IAdapterView::INVALID_POSITION, sel);
            } else {
                sel->SetSelected(FALSE);
                mSelectorRect->SetEmpty();
            }
        } else {
            PositionSelector(IAdapterView::INVALID_POSITION, sel);
        }
        sel->GetTop(&mSelectedTop);
    }
    else {
        if (mTouchMode > AbsListView::TOUCH_MODE_DOWN && mTouchMode < TOUCH_MODE_SCROLL) {
            AutoPtr<IView> child = GetChildAt(mMotionPosition - mFirstPosition);
            if (child != NULL) PositionSelector(mMotionPosition, child);
        } else {
            mSelectedTop = 0;
            mSelectorRect->SetEmpty();
        }

        if (HasFocus() && focusLayoutRestoreView != NULL) {
            View* viewFromFocusLayoutRestoreView = VIEW_PROBE(focusLayoutRestoreView);
            viewFromFocusLayoutRestoreView->RequestFocus();
        }
    }
    if (accessibilityFocusLayoutRestoreNode != NULL) {
        Boolean rubish;
        accessibilityFocusLayoutRestoreNode->PerformAction(
            IAccessibilityNodeInfo::ACTION_ACCESSIBILITY_FOCUS, &rubish);
    }
    else if (accessibilityFocusLayoutRestoreView != NULL) {
        View* viewFromAccessibilityFocusLayoutRestoreView = VIEW_PROBE(accessibilityFocusLayoutRestoreView);
        viewFromAccessibilityFocusLayoutRestoreView->RequestAccessibilityFocus();
    }
    else if (accessibilityFocusPosition != IAdapterView::INVALID_POSITION) {
        using Elastos::Core::Math;
        Int32 position = Math::Constrain((accessibilityFocusPosition - mFirstPosition), 0, (GetChildCount() - 1));
        AutoPtr<IView> restoreView = GetChildAt(position);
        if (restoreView != NULL) {
            View* viewFromRestoreView = VIEW_PROBE(restoreView);
            viewFromRestoreView->RequestAccessibilityFocus();
        }
    }
    if (focusLayoutRestoreView != NULL) {
        AutoPtr<IBinder> winToken;
        focusLayoutRestoreView->GetWindowToken((IBinder**)&winToken);
        if (winToken != NULL) {
            focusLayoutRestoreView->OnFinishTemporaryDetach();
        }
    }
    mLayoutMode = AbsListView::LAYOUT_NORMAL;
    mDataChanged = FALSE;
    if (mPositionScrollAfterLayout != NULL) {
        Post(mPositionScrollAfterLayout);
        mPositionScrollAfterLayout = NULL;
    }
    mNeedSync = FALSE;
    SetNextSelectedPositionInt(mSelectedPosition);
    UpdateScrollIndicators();
    if (mItemCount > 0) {
        CheckSelectionChanged();
    }
    InvokeOnItemScrollListener();
    //}  finally{
    if (!blockLayoutRequests) {
        mBlockLayoutRequests = FALSE;
    }
    //}
}

AutoPtr<IView> ListView::FindAccessibilityFocusedChild(
    /* [in] */ IView* focusedView)
{
    AutoPtr<IViewParent> viewParent;
    focusedView->GetParent((IViewParent**)&viewParent);

    while (IView::Probe(viewParent.Get()) && viewParent.Get() != THIS_PROBE(IViewParent)) {
        focusedView = IView::Probe(viewParent);
        viewParent = NULL;
        focusedView->GetParent((IViewParent**)&viewParent);
    }
    if (!IView::Probe(viewParent.Get())) {
        return NULL;
    }
    return focusedView;
}

Boolean ListView::IsDirectChildHeaderOrFooter(
    /* [in] */ IView* child)
{
    Vector<AutoPtr<FixedViewInfo> >::Iterator iter;
    for (iter = mHeaderViewInfos.Begin(); iter != mHeaderViewInfos.End(); ++iter) {
        if (child == (*iter)->mView.Get()) {
            return TRUE;
        }
    }

    for (iter = mFooterViewInfos.Begin(); iter != mFooterViewInfos.End(); ++iter) {
        if (child == (*iter)->mView.Get()){
            return TRUE;
        }
    }
    return FALSE;
}

AutoPtr<IView> ListView::MakeAndAddView(
    /* [in] */ Int32 position,
    /* [in] */ Int32 y,
    /* [in] */ Boolean flow,
    /* [in] */ Int32 childrenLeft,
    /* [in] */ Boolean selected)
{
    AutoPtr<IView> child;
    if (!mDataChanged) {
        child = mRecycler->GetActiveView(position);
        if (child != NULL) {
            SetupChild(child, position, y, flow, childrenLeft, selected, TRUE);
            return child;
        }
    }
    child = ObtainView(position, mIsScrap);
    SetupChild(child, position, y, flow, childrenLeft, selected, (*mIsScrap)[0]);
    return child;
}

void ListView::SetupChild(
    /* [in] */ IView* child,
    /* [in] */ Int32 position,
    /* [in] */ Int32 y,
    /* [in] */ Boolean flowDown,
    /* [in] */ Int32 childrenLeft,
    /* [in] */ Boolean selected,
    /* [in] */ Boolean recycled)
{
    View* childView = VIEW_PROBE(child);
    Boolean isSelected = selected && ShouldShowSelector();
    Boolean updateChildSelected = isSelected != childView->IsSelected();
    Int32 mode = mTouchMode;
    Boolean isPressed = mode > AbsListView::TOUCH_MODE_DOWN && mode < AbsListView::TOUCH_MODE_SCROLL &&
        mMotionPosition == position;
    Boolean updateChildPressed = isPressed != childView->IsPressed();
    Boolean needToMeasure = !recycled || updateChildSelected || childView->IsLayoutRequested();

    AutoPtr<IViewGroupLayoutParams> p = childView->GetLayoutParams().Get();
    AutoPtr<IAbsListViewLayoutParams> lp = IAbsListViewLayoutParams::Probe(p.Get());
    AutoPtr<CAbsListViewLayoutParams> cp = (CAbsListViewLayoutParams*)lp.Get();
    if (p == NULL) {
        GenerateDefaultLayoutParams((IViewGroupLayoutParams**)&p);
        cp = (CAbsListViewLayoutParams*)IAbsListViewLayoutParams::Probe(p.Get());
    }
    mAdapter->GetItemViewType(position, &(cp->mViewType));

    if ((recycled && !cp->mForceAdd) || (cp->mRecycledHeaderFooter &&
        cp->mViewType == IAdapterView::ITEM_VIEW_TYPE_HEADER_OR_FOOTER)) {
        AttachViewToParent(child, flowDown ? -1 : 0, p);
    } else {
        cp->mForceAdd = FALSE;
        if (cp->mViewType == IAdapterView::ITEM_VIEW_TYPE_HEADER_OR_FOOTER) {
            cp->mRecycledHeaderFooter = TRUE;
        }
        AddViewInLayout(child, flowDown ? -1 : 0, p, TRUE);
    }
    if (updateChildSelected) {
        childView->SetSelected(isSelected);
    }

    if (updateChildPressed) {
        childView->SetPressed(isPressed);
    }

    AutoPtr<IApplicationInfo> info;
    GetContext()->GetApplicationInfo((IApplicationInfo**)&info);
    AutoPtr<CApplicationInfo> cInfo = (CApplicationInfo*)info.Get();
    if (mChoiceMode == IAbsListView::CHOICE_MODE_NONE && mCheckStates != NULL) {
        AutoPtr<ICheckable> checkable = (ICheckable*)child->Probe(EIID_ICheckable);
        if (checkable != NULL) {
            checkable->SetChecked((*mCheckStates)[position]);
        } else if (cInfo->mTargetSdkVersion >= Build::VERSION_CODES::HONEYCOMB) {
            childView->SetActivated((*mCheckStates)[position]);
        }
    }

    if (needToMeasure) {
        Int32 childWidthSpec = ViewGroup::GetChildMeasureSpec(mWidthMeasureSpec,
            mListPadding->mLeft + mListPadding->mRight, cp->mWidth);
        Int32 childHeightSpec = 0;
        if (cp->mHeight > 0) {
            childHeightSpec = View::MeasureSpec::MakeMeasureSpec(cp->mHeight,
                View::MeasureSpec::EXACTLY);
        } else {
            childHeightSpec = View::MeasureSpec::MakeMeasureSpec(0,
                View::MeasureSpec::UNSPECIFIED);
        }
        childView->Measure(childWidthSpec, childHeightSpec);
    } else {
        CleanupLayoutState(child);
    }

    Int32 w = childView->GetMeasuredWidth();
    Int32 h = childView->GetMeasuredHeight();
    Int32 childTop = flowDown ? y : y - h;

    if (needToMeasure) {
        Int32 childRight = childrenLeft + w;
        Int32 childBottom = childTop + h;
        childView->Layout(childrenLeft, childTop, childRight, childBottom);
    } else {
        childView->OffsetLeftAndRight(childrenLeft - childView->GetLeft());
        childView->OffsetTopAndBottom(childTop - childView->GetTop());
    }

    if (mCachingStarted && !childView->IsDrawingCacheEnabled()) {
        childView->SetDrawingCacheEnabled(TRUE);
    }

    AutoPtr<IViewGroupLayoutParams> layoutParams = childView->GetLayoutParams();
    AutoPtr<CAbsListViewLayoutParams> params =
        (CAbsListViewLayoutParams*)IAbsListViewLayoutParams::Probe(layoutParams.Get());
    if (recycled && params->mScrappedFromPosition != position) {
        childView->JumpDrawablesToCurrentState();
    }
}

Boolean ListView::CanAnimate()
{
    return AbsListView::CanAnimate() && mItemCount > 0;
}

ECode ListView::SetSelection(
    /* [in] */ Int32 position)
{
    return SetSelectionFromTop(position, 0);
}

ECode ListView::SetSelectionFromTop(
    /* [in] */ Int32 position,
    /* [in] */ Int32 y)
{
    if (mAdapter == NULL) {
        return NOERROR;
    }

    if (!IsInTouchMode()) {
        position = LookForSelectablePosition(position, TRUE);
        if (position > 0) {
            SetNextSelectedPositionInt(position);
        }
    } else {
        mResurrectToPosition = position;
    }

    if (position >= 0) {
        mLayoutMode = AbsListView::LAYOUT_SPECIFIC;
        mSpecificTop = mListPadding->mTop + y;
        if (mNeedSync) {
            mSyncPosition = position;
            mAdapter->GetItemId(position, &mSyncRowId);
        }

        if (mPositionScroller != NULL) {
            mPositionScroller->Stop();
        }
        RequestLayout();
    }
    return NOERROR;
}

ECode ListView::SetSelectionInt(
    /* [in] */ Int32 position)
{
    SetNextSelectedPositionInt(position);

    Boolean awakeScrollbars = FALSE;
    if (mSelectedPosition >= 0) {
        if (position == mSelectedPosition - 1) {
            awakeScrollbars = TRUE;
        } else if (position == mSelectedPosition + 1) {
            awakeScrollbars = TRUE;
        }
    }

    if (mPositionScroller != NULL) {
        mPositionScroller->Stop();
    }

    LayoutChildren();

    if (awakeScrollbars) {
        AwakenScrollBars();
    }
    return NOERROR;
}

Int32 ListView::LookForSelectablePosition(
    /* [in] */ Int32 position,
    /* [in] */ Boolean lookDown)
{
    AutoPtr<IListAdapter> adapter = mAdapter;
    if (adapter == NULL || IsInTouchMode()) {
        return IAdapterView::INVALID_POSITION;
    }

    Int32 count;
    adapter->GetCount(&count);
    if (!mAreAllItemsSelectable) {
        Boolean enabled;
        adapter->IsEnabled(position, &enabled);
        if (lookDown) {
            position = Elastos::Core::Math::Max(0, position);
            while(position < count && !enabled) {
                position++;
            }
        } else {
            position = Elastos::Core::Math::Min(position, count - 1);
            while(position >= 0 && !enabled) {
                position--;
            }
        }

        if (position < 0 || position >= count) {
            return IAdapterView::INVALID_POSITION;
        }
        return position;
    } else {
        if (position < 0 || position >= count) {
            return IAdapterView::INVALID_POSITION;
        }
        return position;
    }
}

ECode ListView::SetSelectionAfterHeaderView()
{
    Int32 count = mHeaderViewInfos.GetSize();
    if (count > 0) {
        mNextSelectedPosition = 0;
        return NOERROR;
    }
    if (mAdapter != NULL) {
        SetSelection(count);
    } else {
        mNextSelectedPosition = count;
        mLayoutMode = AbsListView::LAYOUT_SET_SELECTION;
    }
    return NOERROR;
}

Boolean ListView::DispatchKeyEvent(
        /* [in] */ IKeyEvent* event)
{
    Boolean handled = AbsListView::DispatchKeyEvent(event);
    if (!handled) {
        AutoPtr<IView> focused = GetFocusedChild();
        Int32 keyEvent;
        event->GetAction(&keyEvent);
        if (focused != NULL && keyEvent == IKeyEvent::ACTION_DOWN) {
            Int32 keyCode;
            event->GetKeyCode(&keyCode);
            handled = OnKeyDown(keyCode, event);
        }
    }
    return handled;
}

Boolean ListView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return CommonKey(keyCode, 1, event);
}

Boolean ListView::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 repeatCount,
    /* [in] */ IKeyEvent* event)
{
    return CommonKey(keyCode, repeatCount, event);
}

Boolean ListView::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return CommonKey(keyCode, 1, event);
}

Boolean ListView::CommonKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ IKeyEvent* event)
{
    if (mAdapter == NULL || mIsAttached) {
        return FALSE;
    }

    if (mDataChanged) {
        LayoutChildren();
    }

    Boolean handled = FALSE;
    Int32 action;
    event->GetAction(&action);

    if (action != IKeyEvent::ACTION_UP) {
        switch (keyCode) {
        case IKeyEvent::KEYCODE_DPAD_UP:
            Boolean noModifiersUp, hasModifiersUp;
            event->HasNoModifiers(&noModifiersUp);
            event->HasModifiers(IKeyEvent::META_ALT_ON ,&hasModifiersUp);
            if (noModifiersUp) {
                handled = ResurrectSelectionIfNeeded();
                if (!handled) {
                    while(count-- > 0) {
                        if (ArrowScroll(IView::FOCUS_UP)) {
                            handled = TRUE;
                        } else {
                            break;
                        }
                    }
                }
            } else if (hasModifiersUp) {
                handled = ResurrectSelectionIfNeeded() || FullScroll(IView::FOCUS_UP);
            }
            break;

        case IKeyEvent::KEYCODE_DPAD_DOWN:
            Boolean noModifiersDown, hasModifiersDown;
            event->HasNoModifiers(&noModifiersDown);
            event->HasModifiers(IKeyEvent::META_ALT_ON ,&hasModifiersDown);
            if (noModifiersDown) {
                handled = ResurrectSelectionIfNeeded();
                if (!handled) {
                    while (count-- > 0) {
                        if (ArrowScroll(IView::FOCUS_DOWN)) {
                            handled = TRUE;
                        } else {
                            break;
                        }
                    }
                }
            } else if (hasModifiersDown) {
                handled = ResurrectSelectionIfNeeded() || FullScroll(IView::FOCUS_DOWN);
            }
            break;

        case IKeyEvent::KEYCODE_DPAD_LEFT:
            Boolean noModifiersLeft;
            event->HasNoModifiers(&noModifiersLeft);
            if (noModifiersLeft) {
                ASSERT_SUCCEEDED(HandleHorizontalFocusWithinListItem(IView::FOCUS_LEFT, &handled));
            }
            break;

        case IKeyEvent::KEYCODE_DPAD_RIGHT:
            Boolean noModifiersRight;
            event->HasNoModifiers(&noModifiersRight);
            if (noModifiersRight) {
                ASSERT_SUCCEEDED(HandleHorizontalFocusWithinListItem(IView::FOCUS_RIGHT, &handled));
            }
            break;

        case IKeyEvent::KEYCODE_DPAD_CENTER:
        case IKeyEvent::KEYCODE_ENTER:
            Boolean noModifiersEnter;
            event->HasNoModifiers(&noModifiersEnter);
            if (noModifiersEnter) {
                handled = ResurrectSelectionIfNeeded();
                Int32 repeatCount;
                event->GetRepeatCount(&repeatCount);
                if (!handled && repeatCount == 0 && GetChildCount() > 0) {
                    KeyPressed();
                    handled = TRUE;
                }
            }
            break;

        case IKeyEvent::KEYCODE_PAGE_UP:
            Boolean noModifiersPageUp, hasModifiersPageUp;
            event->HasNoModifiers(&noModifiersPageUp);
            event->HasModifiers(IKeyEvent::META_ALT_ON ,&hasModifiersPageUp);
            if (noModifiersPageUp) {
                handled = ResurrectSelectionIfNeeded() || PageScroll(IView::FOCUS_UP);
            } else if (hasModifiersPageUp) {
                handled = ResurrectSelectionIfNeeded() || FullScroll(IView::FOCUS_UP);
            }
            break;

        case IKeyEvent::KEYCODE_PAGE_DOWN:
            Boolean noModifiersPageDown, hasModifiersPageDown;
            event->HasNoModifiers(&noModifiersPageDown);
            event->HasModifiers(IKeyEvent::META_ALT_ON ,&hasModifiersPageDown);
            if (noModifiersPageDown) {
                handled = ResurrectSelectionIfNeeded() || PageScroll(IView::FOCUS_DOWN);
            } else if (hasModifiersPageDown) {
                handled = ResurrectSelectionIfNeeded() || FullScroll(IView::FOCUS_DOWN);
            }
            break;

        case IKeyEvent::KEYCODE_MOVE_HOME:
            Boolean noModifiersMoveHome;
            event->HasNoModifiers(&noModifiersMoveHome);
            if (noModifiersMoveHome) {
                handled = ResurrectSelectionIfNeeded() || FullScroll(IView::FOCUS_UP);
            }
            break;

        case IKeyEvent::KEYCODE_MOVE_END:
            Boolean noModifiersMoveEnd;
            event->HasNoModifiers(&noModifiersMoveEnd);
            if (noModifiersMoveEnd) {
                handled = ResurrectSelectionIfNeeded() || FullScroll(IView::FOCUS_DOWN);
            }
            break;

        case IKeyEvent::KEYCODE_TAB:
        if (FALSE) {
            Boolean noModifiersTab, hasModifiersTab;
            event->HasNoModifiers(&noModifiersTab);
            event->HasModifiers(IKeyEvent::META_ALT_ON ,&hasModifiersTab);
            if (noModifiersTab) {
                handled = ResurrectSelectionIfNeeded() || ArrowScroll(IView::FOCUS_DOWN);
            } else if (hasModifiersTab) {
                handled = ResurrectSelectionIfNeeded() || ArrowScroll(IView::FOCUS_UP);
            }
        }
        break;
        }
    }

    if (handled) {
        return TRUE;
    }

    if (SendToTextFilter(keyCode, count, event)) {
        return TRUE;
    }

    switch (action) {
    case IKeyEvent::ACTION_DOWN:
        return AbsListView::OnKeyDown(keyCode, event);

    case IKeyEvent::ACTION_UP:
        return AbsListView::OnKeyUp(keyCode, event);

    case IKeyEvent::ACTION_MULTIPLE:
        return AbsListView::OnKeyMultiple(keyCode, count, event);

    default:
        return FALSE;
    }
}

Boolean ListView::PageScroll(
    /* [in] */ Int32 direction)
{
    Int32 nextPage = -1;
    Boolean down = FALSE;

    if (direction == IView::FOCUS_UP) {
        nextPage = Elastos::Core::Math::Max(0, mSelectedPosition - GetChildCount() - 1);
    } else if (direction == IView::FOCUS_DOWN) {
        nextPage = Elastos::Core::Math::Min(mItemCount - 1, mSelectedPosition + GetChildCount() - 1);
        down = TRUE;
    }

    if (nextPage >= 0) {
        Int32 position = LookForSelectablePosition(nextPage, down);
        if (position >= 0) {
            mLayoutMode = AbsListView::LAYOUT_SPECIFIC;
            mSpecificTop = mPaddingTop + GetVerticalFadingEdgeLength();

            if (down && position > mItemCount - GetChildCount()) {
                mLayoutMode = AbsListView::LAYOUT_FORCE_TOP;
            }

            SetSelectionInt(position);
            InvokeOnItemScrollListener();
            if (!AwakenScrollBars()) {
                Invalidate();
            }
            return TRUE;
        }
    }
    return FALSE;
}

Boolean ListView::FullScroll(
    /* [in] */ Int32 direction)
{
    Boolean moved = FALSE;
    if (direction == IView::FOCUS_UP) {
        if (mSelectedPosition != 0) {
            Int32 position = LookForSelectablePosition(0, TRUE);
            if (position >= 0) {
                mLayoutMode = AbsListView::LAYOUT_FORCE_TOP;
                SetSelectionInt(position);
                InvokeOnItemScrollListener();
            }
            moved = TRUE;
        }
    } else if (direction == IView::FOCUS_DOWN) {
        if (mSelectedPosition < mItemCount - 1) {
            Int32 position = LookForSelectablePosition(mItemCount - 1, TRUE);
            if (position >= 0) {
                mLayoutMode = AbsListView::LAYOUT_FORCE_BOTTOM;
                SetSelectionInt(position);
                InvokeOnItemScrollListener();
            }
            moved = TRUE;
        }
    }
    if (moved && !AwakenScrollBars()) {
        AwakenScrollBars();
        Invalidate();
    }
    return moved;
}

ECode ListView::HandleHorizontalFocusWithinListItem(
    /* [in] */ Int32 direction,
    /* [out] */ Boolean* rst)
{
    *rst = FALSE;
    if (direction != IView::FOCUS_LEFT && direction != IView::FOCUS_RIGHT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 numChildren = GetChildCount();
    if (mItemsCanFocus && numChildren > 0 && mSelectedPosition != IAdapterView::INVALID_POSITION) {
        AutoPtr<IView> selectedView = GetSelectedView();
        AutoPtr<IViewGroup> theView = (IViewGroup*)theView->Probe(EIID_IViewGroup);
        Boolean hasFocus;
        theView->HasFocus(&hasFocus);
        if (selectedView != NULL && hasFocus && theView != NULL) {
            AutoPtr<IView> currentFocus;
            selectedView->FindFocus((IView**)&currentFocus);
            AutoPtr<IView> nextFocus;
            FocusFinder::GetInstance()->FindNextFocus(
               IViewGroup::Probe(selectedView), currentFocus.Get(), direction, (IView**)&nextFocus);
            if (nextFocus != NULL) {
                currentFocus->GetFocusedRect((IRect*)mTempRect);
                OffsetDescendantRectToMyCoords(currentFocus, mTempRect);
                OffsetRectIntoDescendantCoords(nextFocus, mTempRect);
                Boolean res = FALSE;
                nextFocus->RequestFocus(direction, mTempRect, &res);
                if (res) {
                    *rst = TRUE;
                    return NOERROR;
                }
            }
            AutoPtr<IView> globalNextFocus;
            FocusFinder::GetInstance()->FindNextFocus(
                IViewGroup::Probe(GetRootView()), currentFocus, direction, (IView**)&globalNextFocus);
            if (globalNextFocus != NULL) {
                *rst = IsViewAncestorOf(globalNextFocus, THIS_PROBE(IView));
                return NOERROR;
            }
        }

    }
    *rst = FALSE;
    return NOERROR;
}

Boolean ListView::ArrowScroll(
    /* [in] */ Int32 direction)
{
    //try{
        mInLayout = TRUE;
        Boolean handled = ArrowScrollImpl(direction);
        if (handled) {
            PlaySoundEffect(SoundEffectConstants::GetContantForFocusDirection(direction));
        }
        mInLayout = FALSE;
        return handled;
    //} finally {
    //    mInLayout = FALSE;
    //}
}

Boolean ListView::ArrowScrollImpl(
    /* [in] */ Int32 direction)
{
    if (GetChildCount() < 0) {
    return FALSE;
    }
    AutoPtr<IView> selectedView = GetSelectedView();
    Int32 selectedPos = mSelectedPosition;

    Int32 nextSelectedPosition = LookForSelectablePositionOnScreen(direction);
    Int32 amountToScroll = AmountToScroll(direction, nextSelectedPosition);

    AutoPtr<ListView::ArrowScrollFocusResult> focusResult = mItemsCanFocus ?
        ArrowScrollFocused(direction) : NULL;
    if (focusResult != NULL) {
        nextSelectedPosition = focusResult->GetSelectedPosition();
        amountToScroll = focusResult->GetAmountToScroll();
    }

    Boolean needToRedraw = focusResult != NULL;
    if (nextSelectedPosition != IAdapterView::INVALID_POSITION) {
        HandleNewSelectionChange(selectedView, direction, nextSelectedPosition, needToRedraw);
        SetSelectedPositionInt(nextSelectedPosition);
        SetNextSelectedPositionInt(nextSelectedPosition);
        selectedView = GetSelectedView();
        selectedPos = nextSelectedPosition;
        if (mItemsCanFocus && !needToRedraw) {
            AutoPtr<IView> focused = GetFocusedChild();
            if (focused != NULL) {
                focused->ClearFocus();
            }
        }

        needToRedraw = TRUE;
        CheckSelectionChanged();
    }

    if (amountToScroll > 0) {
        ScrollListItemsBy((direction == IView::FOCUS_UP) ? amountToScroll : -amountToScroll);
        needToRedraw = TRUE;
    }

    Boolean hasFocus;
    selectedView->HasFocus(&hasFocus);
    if (mItemsCanFocus && (focusResult == NULL)
        && selectedView != NULL && hasFocus) {
        AutoPtr<IView> focused;
        selectedView->FindFocus((IView**)&focused);
        if (!IsViewAncestorOf(focused, THIS_PROBE(IView)) || DistanceToView(focused) > 0) {
            focused->ClearFocus();
        }
    }

    if (nextSelectedPosition == IAdapterView::INVALID_POSITION && selectedView != NULL
        && !IsViewAncestorOf(selectedView, THIS_PROBE(IView))) {
        selectedView = NULL;
        HideSelector();
        mResurrectToPosition = IAdapterView::INVALID_POSITION;
    }

    if (needToRedraw) {
        if (selectedView != NULL) {
            PositionSelector(selectedPos, selectedView);
            selectedView->GetTop(&mSelectedTop);
        }
        if (!AwakenScrollBars()) {
            Invalidate();
        }

        InvokeOnItemScrollListener();
        return TRUE;
    }

    return FALSE;
}

ECode ListView::HandleNewSelectionChange(
    /* [in] */ IView* selectedView,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 newSelectedPosition,
    /* [in] */ Boolean newFocusAssigned)
{
    if (newSelectedPosition == IAdapterView::INVALID_POSITION) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IView> topView;
    AutoPtr<IView> bottomView;
    Int32 topViewIndex = 0;
    Int32 bottomViewIndex = 0;
    Boolean topSelected = FALSE;
    Int32 selectedIndex = mSelectedPosition - mFirstPosition;
    Int32 nextSelectedIndex = newSelectedPosition - mFirstPosition;
    if (direction == IView::FOCUS_UP) {
        topViewIndex = nextSelectedIndex;
        bottomViewIndex = selectedIndex;
        topView = GetChildAt(topViewIndex);
        bottomView = selectedView;
        topSelected = TRUE;
    } else {
        topViewIndex = selectedIndex;
        bottomViewIndex = nextSelectedIndex;
        topView = selectedView;
        bottomView = GetChildAt(bottomViewIndex);
    }

    Int32 numChildren = GetChildCount();
    if (topView != NULL) {
        topView->SetSelected(!newFocusAssigned && topSelected);
        MeasureAndAdjustDown(topView, topViewIndex, numChildren);
    }

    if (bottomView != NULL) {
        bottomView->SetSelected(!newFocusAssigned && !topSelected);
        MeasureAndAdjustDown(bottomView, bottomViewIndex, numChildren);
    }
    return NOERROR;
}

void ListView::MeasureAndAdjustDown(
    /* [in] */ IView* child,
    /* [in] */ Int32 childIndex,
    /* [in] */ Int32 numChildren)
{
    Int32 oldHeight, meaHeight;
    child->GetHeight(&oldHeight);
    MeasureItem(child);
    child->GetMeasuredHeight(&meaHeight);
    if (meaHeight != oldHeight) {
        RelayoutMeasuredItem(child);
        Int32 heightDelta;
        child->GetMeasuredHeight(&heightDelta);
        heightDelta -= oldHeight;
        for(Int32 i = childIndex + 1; i < numChildren; i++) {
            GetChildAt(i)->OffsetTopAndBottom(heightDelta);
        }
    }
}

void ListView::MeasureItem(
    /* [in] */ IView* child)
{
    AutoPtr<IViewGroupLayoutParams> p;
    child->GetLayoutParams((IViewGroupLayoutParams**)&p);
    if (p == NULL) {
        CViewGroupLayoutParams::New(IViewGroupLayoutParams::MATCH_PARENT,
            IViewGroupLayoutParams::WRAP_CONTENT, (IViewGroupLayoutParams**)&p);
    }
    Int32 width;
    p->GetWidth(&width);
    Int32 childWidthSpec = ViewGroup::GetChildMeasureSpec(mWidthMeasureSpec,
        mListPadding->mLeft + mListPadding->mRight, width);
    Int32 height;
    p->GetHeight(&height);
    Int32 childHeightSpec;
    if (height > 0) {
        childHeightSpec = View::MeasureSpec::MakeMeasureSpec(height, View::MeasureSpec::EXACTLY);
    }
    else {
        childHeightSpec = View::MeasureSpec::MakeMeasureSpec(0, View::MeasureSpec::UNSPECIFIED);
    }
    child->Measure(childWidthSpec, childHeightSpec);
}

void ListView::RelayoutMeasuredItem(
    /* [in] */ IView* child)
{
    View* view = (View*)IView::Probe(child);
    Int32 w = view->GetMeasuredWidth();
    Int32 h = view->GetMeasuredHeight();
    Int32 childLetf = mListPadding->mLeft;
    Int32 childRight = childLetf + w;
    Int32 childTop = view->GetTop();
    Int32 childBottom = childTop + h;
    view->Layout(childLetf, childTop, childRight, childBottom);
}

Int32 ListView::GetArrowScrollPreviewLength()
{
    return Elastos::Core::Math::Max(MIN_SCROLL_PREVIEW_PIXELS, GetVerticalFadingEdgeLength());
}

Int32 ListView::AmountToScroll(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 nextSelectedPosition)
{
    Int32 listBottom = GetHeight() - mListPadding->mBottom;
    Int32 listTop = mListPadding->mTop;
    Int32 numChildren = GetChildCount();

    if (direction == IView::FOCUS_DOWN) {
        Int32 indexToMakeVisible = numChildren - 1;
        if (nextSelectedPosition != IAdapterView::INVALID_POSITION) {
            indexToMakeVisible = nextSelectedPosition - mFirstPosition;
        }

        Int32 positionToMakeVisible = mFirstPosition + indexToMakeVisible;
        AutoPtr<IView> viewToMakeVisible = GetChildAt(indexToMakeVisible);

        Int32 goalBottom = listBottom;
        if (positionToMakeVisible < mItemCount - 1) {
            goalBottom -= GetArrowScrollPreviewLength();
        }
        Int32 visibleTop, visibleBottom;
        viewToMakeVisible->GetTop(&visibleTop);
        viewToMakeVisible->GetBottom(&visibleBottom);
        if (visibleTop <= goalBottom) {
            return 0;
        }

        if (nextSelectedPosition != IAdapterView::INVALID_POSITION
            &&(goalBottom - visibleTop) >= GetMaxScrollAmount()) {
            return 0;
        }

        Int32 amountToScroll = visibleBottom - goalBottom;

        if ((mFirstPosition) == mItemCount) {
            Int32 max;
            GetChildAt(numChildren - 1)->GetBottom(&max);
            max -= listBottom;
            amountToScroll = Elastos::Core::Math::Min(amountToScroll, max);
        }
        return Elastos::Core::Math::Min(amountToScroll, GetMaxScrollAmount());
    } else {
        Int32 indexToMakeVisible = 0;
        if (nextSelectedPosition != IAdapterView::INVALID_POSITION) {
            indexToMakeVisible = nextSelectedPosition - mFirstPosition;
        }
        Int32 positionToMakeVisible = mFirstPosition + indexToMakeVisible;
        AutoPtr<IView> viewToMakeVisible = GetChildAt(indexToMakeVisible);
        Int32 goalTop = listTop;
        if (positionToMakeVisible > 0) {
            goalTop += GetArrowScrollPreviewLength();
        }
        Int32 visibleTop, visibleBottom;
        viewToMakeVisible->GetTop(&visibleTop);
        viewToMakeVisible->GetBottom(&visibleBottom);
        if (visibleTop >= goalTop) {
            return 0;
        }

        if (nextSelectedPosition != IAdapterView::INVALID_POSITION
            && (visibleBottom - goalTop) >= GetMaxScrollAmount()) {
            return 0;
        }

        Int32 amountToScroll = goalTop - visibleTop;
        if (mFirstPosition == 0) {
            Int32 maxTop = 0;
            GetChildAt(0)->GetTop(&maxTop);
            Int32 max = listTop - maxTop;
            amountToScroll = Elastos::Core::Math::Min(amountToScroll, max);
        }
        return Elastos::Core::Math::Min(amountToScroll, GetMaxScrollAmount());
    }
}

Int32 ListView::LookForSelectablePositionOnScreen(
    /* [in] */ Int32 direction)
{
    Int32 firstPosition = mFirstPosition;
    if (direction == IView::FOCUS_DOWN) {
        Int32 startPos = (mSelectedPosition != IAdapterView::INVALID_POSITION)
            ? mSelectedPosition + 1 : firstPosition;
            Int32 count = 0;
            mAdapter->GetCount(&count);
        if (startPos > count) {
            return IAdapterView::INVALID_POSITION;
        }
        if (startPos < firstPosition) {
            startPos = firstPosition;
        }

        Int32 lastVisiblePosition = GetLastVisiblePosition();
        AutoPtr<IAdapter> temp = GetAdapter();
        AutoPtr<IListAdapter> adapter = IListAdapter::Probe(temp.Get());
        for(Int32 pos = startPos; pos <= lastVisiblePosition; pos++) {
            Boolean res = FALSE;
            adapter->IsEnabled(pos, &res);
            Int32 visible;
            GetChildAt(pos - firstPosition)->GetVisibility(&visible);
            if (res && visible == IView::VISIBLE) {
                return pos;
            }
        }
    } else {
        Int32 last = firstPosition + GetChildCount() - 1;
        Int32 startPos = (mSelectedPosition != IAdapterView::INVALID_POSITION)
            ? mSelectedPosition - 1 : firstPosition + GetChildCount() - 1;
        Int32 count = 0;
        mAdapter->GetCount(&count);
        if (startPos < 0 || startPos >= count) {
            return IAdapterView::INVALID_POSITION;
        }
        if (startPos > last) {
            startPos = last;
        }

        AutoPtr<IAdapter> temp = GetAdapter();
        AutoPtr<IListAdapter> adapter = IListAdapter::Probe(temp.Get());
        for(Int32 pos = startPos; pos <= firstPosition; pos--) {
            Boolean res;
            adapter->IsEnabled(pos, &res);
            Int32 visible;
            GetChildAt(pos - firstPosition)->GetVisibility(&visible);
            if (res && visible == IView::VISIBLE) {
                return pos;
            }
        }
    }
    return IAdapterView::INVALID_POSITION;
}

AutoPtr<ListView::ArrowScrollFocusResult> ListView::ArrowScrollFocused(
    /* [in] */ Int32 direction)
{
    AutoPtr<IView> selectedView = GetSelectedView();
    AutoPtr<IView> newFocus;
    Boolean selectedViewHasFocus = FALSE;
    selectedView->HasFocus(&selectedViewHasFocus);
    if (selectedView != NULL && selectedViewHasFocus) {
        AutoPtr<IView> oldFocus;
        selectedView->FindFocus((IView**)&oldFocus);
        FocusFinder::GetInstance()->FindNextFocus(
                (IViewGroup*)this->Probe(EIID_IViewGroup), oldFocus, direction, (IView**)&newFocus);
    }
    else {
        if (direction == IView::FOCUS_DOWN) {
            Boolean topFadingEdgeShowing = (mFirstPosition > 0);
            Int32 listTop = mListPadding->mTop +
                (topFadingEdgeShowing ? GetArrowScrollPreviewLength() : 0);
            Int32 selectedViewTop = 0;
            selectedView->GetTop(&selectedViewTop);
            Int32 ySearchPoint = (selectedView != NULL && selectedViewTop > listTop) ?
                selectedViewTop : listTop;
            mTempRect->Set(0, ySearchPoint, 0, ySearchPoint);
        }
        else {
            Boolean bottomFadingEdgeShowing = (mFirstPosition + GetChildCount() - 1) < mItemCount;
            Int32 listBottom = GetHeight() - mListPadding->mBottom -
                (bottomFadingEdgeShowing ? GetArrowScrollPreviewLength() : 0);
            Int32 selectedViewBottom = 0;
            selectedView->GetBottom(&selectedViewBottom);
            Int32 ySearchPoint = (selectedView != NULL && selectedViewBottom < listBottom) ?
                selectedViewBottom : listBottom;
            mTempRect->Set(0, ySearchPoint, 0, ySearchPoint);
        }
        FocusFinder::GetInstance()->FindNextFocusFromRect(
                (IViewGroup*)this->Probe(EIID_IViewGroup), mTempRect, direction, (IView**)&newFocus);
    }

    if (newFocus != NULL) {
        Int32 positionOfNewFocus = PositionOfNewFocus(newFocus);
        if (mSelectedPosition != IAdapterView::INVALID_POSITION &&
            positionOfNewFocus != mSelectedPosition) {
            Int32 selectablePosition = LookForSelectablePositionOnScreen(direction);
            if (selectablePosition != IAdapterView::INVALID_POSITION &&
                ((direction == IView::FOCUS_DOWN && selectablePosition < positionOfNewFocus) ||
                (direction == IView::FOCUS_UP && selectablePosition > positionOfNewFocus))) {
                return NULL;
            }
        }

        Int32 focusScroll = AmountToScrollToNewFocus(direction, newFocus, positionOfNewFocus);
        Int32 maxScrollAmount = GetMaxScrollAmount();
        if (focusScroll < maxScrollAmount) {
            Boolean res = FALSE;
            newFocus->RequestFocus(direction, &res);
            mArrowScrollFocusResult->Populate(positionOfNewFocus, focusScroll);
            return mArrowScrollFocusResult;
        } else if (DistanceToView(newFocus) < maxScrollAmount) {
            Boolean res = FALSE;
            newFocus->RequestFocus(direction, &res);
            mArrowScrollFocusResult->Populate(positionOfNewFocus, maxScrollAmount);
            return mArrowScrollFocusResult;
        }
    }
    return NULL;
}

Int32 ListView::PositionOfNewFocus(
    /* [in] */ IView* newFocus)
{
    Int32 numChildren = GetChildCount();
    for(Int32 i = 0; i < numChildren; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        if (IsViewAncestorOf(newFocus, child)) {
            return mFirstPosition + i;
        }
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

Boolean ListView::IsViewAncestorOf(
    /* [in] */ IView* child,
    /* [in] */ IView* parent)
{
    if (child == parent) {
        return TRUE;
    }
    AutoPtr<IViewParent> theParent;
    child->GetParent((IViewParent**)&theParent);
    AutoPtr<IViewGroup> viewGroup = IViewGroup::Probe(theParent);
    AutoPtr<IView> viewParent = IView::Probe(theParent);
    return (viewGroup != NULL && IsViewAncestorOf(viewParent, parent));
}

Int32 ListView::AmountToScrollToNewFocus(
    /* [in] */ Int32 direction,
    /* [in] */ IView* newFocus,
    /* [in] */ Int32 positionOfNewFocus)
{
    Int32 amountToScroll = 0;
    newFocus->GetDrawingRect(mTempRect);
    OffsetDescendantRectToMyCoords(newFocus, mTempRect);
    if (direction == IView::FOCUS_UP) {
        if (mTempRect->mTop < mListPadding->mTop) {
            amountToScroll = mListPadding->mTop - mTempRect->mTop;
            if (positionOfNewFocus > 0) {
                amountToScroll += GetArrowScrollPreviewLength();
            }
        }
    } else {
        Int32 listBottom= GetHeight() - mListPadding->mBottom;
        if (mTempRect->mBottom > listBottom) {
            amountToScroll = mTempRect->mBottom - listBottom;
            if (positionOfNewFocus < mItemCount - 1) {
                amountToScroll += GetArrowScrollPreviewLength();
            }
        }
    }
    return amountToScroll;
}

Int32 ListView::DistanceToView(
    /* [in] */ IView* descendant)
{
    Int32 distance = 0;
    descendant->GetDrawingRect(mTempRect);
    OffsetDescendantRectToMyCoords(descendant, mTempRect);
    Int32 listBottom = mBottom - mTop - mListPadding->mBottom;
    if (mTempRect->mBottom < mListPadding->mTop) {
        distance = mListPadding->mTop - mTempRect->mBottom;
    } else if (mTempRect->mTop > listBottom) {
        distance = mTempRect->mTop - listBottom;
    }
    return distance;
}

void ListView::ScrollListItemsBy(
    /* [in] */ Int32 amount)
{
    OffsetChildrenTopAndBottom(amount);
    Int32 listBottom = GetHeight() - mListPadding->mBottom;
    Int32 listTop = mListPadding->mTop;

    if (amount < 0) {
        Int32 numChildren = GetChildCount();
        AutoPtr<IView> last = GetChildAt(numChildren - 1);
        View* lastView = VIEW_PROBE(last);
        while(lastView->GetBottom() < listBottom) {
            Int32 lastVisiblePosition = mFirstPosition + numChildren - 1;
            if (lastVisiblePosition < mItemCount - 1) {
                last = AddViewAbove(last, lastVisiblePosition);
                numChildren++;
            } else {
                break;
            }
        }

        if (lastView->GetBottom() < listBottom) {
            OffsetChildrenTopAndBottom(listBottom - lastView->GetBottom());
        }
        AutoPtr<IView> first = GetChildAt(0);
        View* firstView = VIEW_PROBE(first);
        while(firstView->GetBottom() < listTop) {
            AutoPtr<IViewGroupLayoutParams> layoutParams;
            first->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
            AutoPtr<CAbsListViewLayoutParams> lp =
                (CAbsListViewLayoutParams*)IAbsListViewLayoutParams::Probe(layoutParams);
            if (mRecycler->ShouldRecycleViewType(lp->mViewType)) {
                DetachViewFromParent(first);
                mRecycler->AddScrapView(first, mFirstPosition);
            } else {
                RemoveViewInLayout(first);
            }
            first = GetChildAt(0);
            mFirstPosition++;
        }
    } else {
        AutoPtr<IView> first = GetChildAt(0);
        View* firstView = VIEW_PROBE(first);
        while((firstView->GetTop() > listTop) && (mFirstPosition > 0)) {
            first = AddViewAbove(first, mFirstPosition);
            mFirstPosition--;
        }

        if (firstView->GetTop() > listTop) {
            OffsetChildrenTopAndBottom(listTop - firstView->GetTop());
        }

        Int32 lastIndex = GetChildCount() - 1;
        AutoPtr<IView> last = GetChildAt(lastIndex);
        View* lastView = VIEW_PROBE(last);

        while(lastView->GetTop() > listBottom) {
            AutoPtr<IViewGroupLayoutParams> layoutParams;
            last->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
            AutoPtr<CAbsListViewLayoutParams> lp =
                (CAbsListViewLayoutParams*)IAbsListViewLayoutParams::Probe(layoutParams);
            if (mRecycler->ShouldRecycleViewType(lp->mViewType)) {
                DetachViewFromParent(last);
                mRecycler->AddScrapView(last, mFirstPosition + lastIndex);
            } else {
                RemoveViewInLayout(last);
            }

            last = GetChildAt(--lastIndex);
        }

    }
}

AutoPtr<IView> ListView::AddViewAbove(
    /* [in] */ IView* theView,
    /* [in] */ Int32 position)
{
    Int32 abovePosition = position - 1;
    AutoPtr<IView> view = ObtainView(abovePosition, mIsScrap);
    Int32 edgeOfNewChild = 0;
    theView->GetTop(&edgeOfNewChild);
    edgeOfNewChild -= mDividerHeight;
    SetupChild(view, abovePosition, edgeOfNewChild, FALSE, mListPadding->mLeft,
        FALSE, (*mIsScrap)[0]);
    return view;
}

AutoPtr<IView> ListView::AddViewBelow(
    /* [in] */ IView* theView,
    /* [in] */ Int32 position)
{
    Int32 belowPosition = position + 1;
    AutoPtr<IView> view = ObtainView(belowPosition, mIsScrap);
    Int32 edgeOfNewChild = 0;
    theView->GetTop(&edgeOfNewChild);
    edgeOfNewChild += mDividerHeight;
    SetupChild(view, belowPosition, edgeOfNewChild, TRUE, mListPadding->mLeft,
        FALSE, (*mIsScrap)[0]);
    return view;
}

ECode ListView::SetItemsCanFocus(
    /* [in] */ Boolean itemsCanFocus)
{
    mItemsCanFocus = itemsCanFocus;
    if (!itemsCanFocus) {
        SetDescendantFocusability(ViewGroup::FOCUS_BLOCK_DESCENDANTS);
    }
    return NOERROR;
}

Boolean ListView::GetItemsCanFocus()
{
    return mItemsCanFocus;
}

Boolean ListView::IsOpaque()
{
    Boolean retValue = (mCachingActive && mIsCacheColorOpaque && mDividerIsOpaque &&
        HasOpaqueScrollbars()) || AbsListView::IsOpaque();
    if (retValue) {
        Int32 listTop = mListPadding != NULL ? mListPadding->mTop : mPaddingTop;
        AutoPtr<IView> first = GetChildAt(0);
        Int32 firstTop = 0;
        if (first == NULL || (first->GetTop(&firstTop), firstTop) > listTop) {
            return FALSE;
        }

        Int32 listBottom = GetHeight() - (mListPadding != NULL ?
            mListPadding->mBottom : mPaddingBottom);
        AutoPtr<IView> last = GetChildAt(GetChildCount() - 1);
        Int32 lastBottom = 0;
        last->GetBottom(&lastBottom);
        if (last == NULL || lastBottom < listBottom) {
            return FALSE;
        }
    }
    return retValue;
}

ECode ListView::SetCacheColorHint(
    /* [in] */ Int32 color)
{
    Boolean opaque = ((color & 0X80000000) | (color >> 24)) == 0XFF;
    mIsCacheColorOpaque = opaque;
    if (opaque) {
        if (mDividerPaint == NULL) {
            FAIL_RETURN(CPaint::New((IPaint**)&mDividerPaint));
        }
        mDividerPaint->SetColor(color);
    }
    AbsListView::SetCacheColorHint(color);
    return NOERROR;
}

void ListView::DrawOverscrollHeader(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IDrawable* drawable,
    /* [in] */ IRect* bounds)
{
    Int32 height = 0;
    drawable->GetMinimumHeight(&height);
    Int32 saveCabvas = 0;
    Boolean clipCanvas = FALSE;
    canvas->Save(&saveCabvas);
    canvas->ClipRect(bounds, &clipCanvas);

    AutoPtr<CRect> rect = (CRect*)IRect::Probe(bounds);
    Int32 span = rect->mTop - rect->mBottom;
    if (span < height) {
        rect->mTop = rect->mBottom - height;
    }

    drawable->SetBounds(bounds);
    drawable->Draw(canvas);

    canvas->Restore();
}

void ListView::DrawOverscrollFooter(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IDrawable* drawable,
    /* [in] */ IRect* bounds)
{
    Int32 height = 0;
    drawable->GetMinimumHeight(&height);
    Int32 saveCabvas = 0;
    Boolean clipCanvas = FALSE;
    canvas->Save(&saveCabvas);
    canvas->ClipRect(bounds, &clipCanvas);

    AutoPtr<CRect> rect = (CRect*)IRect::Probe(bounds);
    Int32 span = rect->mBottom - rect->mTop;
    if (span < height) {
        rect->mBottom = rect->mTop + height;
    }

    drawable->SetBounds(bounds);
    drawable->Draw(canvas);

    canvas->Restore();
}

void ListView::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    if (mCachingStarted) {
        mCachingActive = TRUE;
    }

    Int32 dividerHeight = mDividerHeight;
    AutoPtr<IDrawable> overscrollHeader = mOverScrollHeader;
    AutoPtr<IDrawable> overscrollFooter = mOverScrollFooter;
    Boolean drawOverscrollHeader = overscrollHeader != NULL;
    Boolean drawOverscrollFooter = overscrollFooter != NULL;
    Boolean drawDividers = dividerHeight > 0 && mDivider != NULL;

    if (drawDividers || drawOverscrollHeader || drawOverscrollFooter) {
        AutoPtr<CRect> bounds = mTempRect;
        bounds->mLeft = mPaddingLeft;
        bounds->mRight = mRight - mLeft -mPaddingRight;

        Int32 count = GetChildCount();
        Int32 headerCount = mHeaderViewInfos.GetSize();
        Int32 itemCount = mItemCount;
        Int32 footerLimit = itemCount - mFooterViewInfos.GetSize() - 1;
        Boolean headerDividers = mHeaderDividersEnabled;
        Boolean footerDividers = mFooterDividersEnabled;
        Int32 first = mFirstPosition;
        Boolean areAllItemsSelectable = mAreAllItemsSelectable;
        AutoPtr<IListAdapter> adapter = mAdapter;
        Boolean fillForMissingDividers = IsOpaque() && !AbsListView::IsOpaque();

        if (fillForMissingDividers && mDividerPaint == NULL && mIsCacheColorOpaque) {
            CPaint::New((IPaint**)&mDividerPaint);
            mDividerPaint->SetColor(GetCacheColorHint());
        }
        AutoPtr<IPaint> paint = mDividerPaint;
        Int32 effectivePaddingTop = 0;
        Int32 effectivePaddingBottom = 0;
        if ((mGroupFlags & ViewGroup::CLIP_TO_PADDING_MASK) == ViewGroup::CLIP_TO_PADDING_MASK) {
            effectivePaddingTop = mListPadding->mTop;
            effectivePaddingBottom = mListPadding->mBottom;
        }
        Int32 listBottom = mBottom - mTop - effectivePaddingBottom +mScrollY;
        if (!mStackFromBottom) {
            Int32 bottom = 0;
            Int32 scrollY = mScrollY;
            if (count > 0 && scrollY < 0) {
                if (drawOverscrollHeader) {
                    bounds->mBottom = 0;
                    bounds->mTop = scrollY;
                    DrawOverscrollHeader(canvas, overscrollHeader, bounds);
                } else if (drawDividers) {
                    bounds->mBottom = 0;
                    bounds->mTop = -dividerHeight;
                    DrawDivider(canvas, bounds, -1);
                }
            }

            for(Int32 i = 0; i < count; i++) {
                if ((headerDividers || first + i >= headerCount) &&
                    (footerDividers || first + i < footerLimit)) {
                    AutoPtr<IView> child = GetChildAt(i);
                    child->GetBottom(&bottom);
                    if (drawDividers && (bottom < listBottom &&
                        !(drawOverscrollFooter && i == count -1))) {
                        Boolean enabled = FALSE;
                        Boolean addEnabled = FALSE;
                        adapter->IsEnabled((first + i), &enabled);
                        adapter->IsEnabled((first + i + 1), &addEnabled);
                        if (areAllItemsSelectable || ((enabled &&
                            (i == count - 1)) || addEnabled)) {
                            bounds->mTop = bottom;
                            bounds->mBottom = bottom + dividerHeight;
                            DrawDivider(canvas, bounds, i);
                        } else if (fillForMissingDividers) {
                            bounds->mTop = bottom;
                            bounds->mBottom = bottom + dividerHeight;
                            canvas->DrawRect(bounds, paint);
                        }
                    }
                }
            }

            Int32 overFooterBottom = mBottom + mScrollY;
            if (drawOverscrollFooter && first + count == itemCount &&
                overFooterBottom > bottom) {
                bounds->mTop = bottom;
                bounds->mBottom = overFooterBottom;
                DrawOverscrollFooter(canvas, overscrollFooter, bounds);
            }
        } else {
            Int32 top = 0;
            Int32 scrollY = mScrollY;
            if (count > 0 && drawOverscrollHeader) {
                bounds->mTop = scrollY;
                GetChildAt(0)->GetTop(&(bounds->mBottom));
                DrawOverscrollHeader(canvas, overscrollHeader, bounds);
            }

            Int32 start = drawOverscrollHeader ? 1 : 0;
            for(Int32 i = start; i < count; i++) {
                if ((headerDividers || first + i >= headerCount) &&
                    (footerDividers || first + i < footerLimit)) {
                    AutoPtr<IView> child = GetChildAt(i);
                    child->GetTop(&top);

                    Boolean enabled = FALSE;
                    Boolean addEnabled = FALSE;
                    adapter->IsEnabled(first + i, &enabled);
                    adapter->IsEnabled(first + i + 1, &addEnabled);
                    if (top > effectivePaddingTop) {
                        if (areAllItemsSelectable || (enabled &&
                            (i == count - 1 || addEnabled))) {
                            bounds->mTop = top - dividerHeight;
                            bounds->mBottom = top;
                            DrawDivider(canvas, bounds, i - 1);
                        } else if (fillForMissingDividers) {
                            bounds->mTop = top - dividerHeight;
                            bounds->mBottom = top;
                            canvas->DrawRect(bounds, paint);
                        }
                    }
                }
            }

            if (count > 0 && scrollY > 0) {
                if (drawOverscrollFooter) {
                    Int32 absListBottom = mBottom;
                    bounds->mTop = absListBottom;
                    bounds->mBottom = absListBottom + scrollY;
                    DrawOverscrollFooter(canvas, overscrollFooter, bounds);
                } else if (drawDividers) {
                    bounds->mTop = listBottom;
                    bounds->mBottom = listBottom + dividerHeight;
                    DrawDivider(canvas, bounds, -1);
                }
            }
        }
    }

    AbsListView::DispatchDraw(canvas);
}

Boolean ListView::DrawChild(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IView* child,
    /* [in] */ Int64 drawingTime)
{
    Boolean more = AbsListView::DrawChild(canvas, child, drawingTime);
    View* view = (View*)IView::Probe(child);
    if (mCachingActive && view->mCachingFailed) {
        mCachingActive = FALSE;
    }
    return more;
}

void ListView::DrawDivider(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* bounds,
    /* [in] */ Int32 childIndex)
{
    AutoPtr<IDrawable> divider = mDivider;
    divider->SetBounds(bounds);
    divider->Draw(canvas);
}

AutoPtr<IDrawable> ListView::GetDivider()
{
    return mDivider;
}

ECode ListView::SetDivider(
    /* [in] */ IDrawable* divider)
{
    if (divider != NULL) {
        divider->GetIntrinsicHeight(&mDividerHeight);
    } else {
        mDividerHeight = 0;
    }

    mDivider = divider;
    Int32 opacity = 0;
    if(divider) {
        divider->GetOpacity(&opacity);
        mDividerIsOpaque = opacity == IPixelFormat::OPAQUE;
    } else {
        mDividerIsOpaque = TRUE;
    }
    RequestLayout();
    Invalidate();
    return NOERROR;
}

Int32 ListView::GetDividerHeight()
{
    return mDividerHeight;
}

ECode ListView::SetDividerHeight(
    /* [in] */ Int32 height)
{
    mDividerHeight = height;
    RequestLayout();
    Invalidate();
    return NOERROR;
}

ECode ListView::SetHeaderDividersEnabled(
    /* [in] */ Boolean headerDividersEnabled)
{
    mHeaderDividersEnabled = headerDividersEnabled;
    Invalidate();
    return NOERROR;
}

ECode ListView::SetFooterDividersEnabled(
    /* [in] */ Boolean footerDividersEnabled)
{
    mFooterDividersEnabled = footerDividersEnabled;
    Invalidate();
    return NOERROR;
}

ECode ListView::SetOverscrollHeader(
    /* [in] */ IDrawable* header)
{
    mOverScrollHeader = header;
    if (mScrollY < 0) {
        Invalidate();
    }
    return NOERROR;
}

AutoPtr<IDrawable> ListView::GetOverscrollHeader()
{
    return mOverScrollHeader;
}

ECode ListView::SetOverscrollFooter(
    /* [in] */ IDrawable* footer)
{
    mOverScrollFooter = footer;
    Invalidate();
    return NOERROR;
}

AutoPtr<IDrawable> ListView::GetOverscrollFooter()
{
    return mOverScrollFooter;
}

void ListView::OnFocusChanged(
    /* [in] */ Boolean gainFocus,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    AbsListView::OnFocusChanged(gainFocus, direction, previouslyFocusedRect);
    AutoPtr<IListAdapter> adapter = mAdapter;
    Int32 closetChildIndex = -1;
    Int32 closestChildTop = 0;
    if (adapter != NULL && gainFocus && previouslyFocusedRect != NULL) {
        previouslyFocusedRect->Offset(mScrollX, mScrollY);
        Int32 count = 0;
        adapter->GetCount(&count);
        if (count < GetChildCount() + mFirstPosition) {
            mLayoutMode = AbsListView::LAYOUT_NORMAL;
            LayoutChildren();
        }

        AutoPtr<CRect> otherRect = mTempRect;
        Int32 minDistance = Elastos::Core::Math::INT32_MAX_VALUE;
        Int32 childCount = GetChildCount();
        Int32 firstPosition = mFirstPosition;

        for(Int32 i = 0; i < childCount; i++) {
            Boolean res = FALSE;
            adapter->IsEnabled(firstPosition + i, &res);
            if (res) {
                continue;
            }

            AutoPtr<IView> other = GetChildAt(i);
            other->GetDrawingRect(otherRect);
            OffsetDescendantRectToMyCoords(other, otherRect);
            Int32 distance = GetDistance(previouslyFocusedRect, otherRect, direction);

            if (distance < minDistance) {
                minDistance = distance;
                closetChildIndex = i;
                other->GetTop(&closestChildTop);
            }
        }
    }

    if (closetChildIndex >= 0) {
        SetSelectionFromTop(closetChildIndex + mFirstPosition, closestChildTop);
    } else {
        RequestLayout();
    }
}

ECode ListView::OnFinishInflate()
{
    AbsListView::OnFinishInflate();
    Int32 count = GetChildCount();
    if (count > 0) {
        for(Int32 i = 0; i < count; i++) {
            AddHeaderView(GetChildAt(i));
        }
        RemoveAllViews();
    }
    return NOERROR;
}

AutoPtr<IView> ListView::FindViewTraversal(
    /* [in] */ Int32 id)
{
    AutoPtr<IView> v = NULL;
    v = AbsListView::FindViewTraversal(id);
    if (v == NULL) {
        v = FindViewInHeadersOrFooters(mHeaderViewInfos, id);
        if (v != NULL) {
            return v;
        }
        v = FindViewInHeadersOrFooters(mFooterViewInfos, id);
        if (v != NULL) {
            return v;
        }
    }
    return v;
}

AutoPtr<IView> ListView::FindViewInHeadersOrFooters(
    /* [in] */ Vector<AutoPtr<FixedViewInfo> >& where,
    /* [in] */ Int32 id)
{
    AutoPtr<IView> v, subV;
    for (Vector<AutoPtr<FixedViewInfo> >::Iterator iter = where.Begin();
        iter != where.End(); iter++) {
        v = (*iter)->mView;
        Boolean res = FALSE;
        v->IsRootNamespace(&res);
        if (res) {
            subV = NULL;
            v->FindViewById(id, (IView**)&subV);
            if (subV != NULL) {
                return subV;
            }
        }
    }
    return NULL;
}

AutoPtr<IView> ListView::FindViewWithTagTraversal(
    /* [in] */ IInterface* tag)
{
    AutoPtr<IView> v = NULL;
    v = AbsListView::FindViewWithTagTraversal(tag);
    if (v == NULL) {
        v = FindViewWithTagInHeadersOrFooters(mHeaderViewInfos, tag);
        if (v != NULL) return v;

        v = FindViewWithTagInHeadersOrFooters(mFooterViewInfos, tag);
        if (v != NULL) return v;
    }
    return v;
}

AutoPtr<IView> ListView::FindViewWithTagInHeadersOrFooters(
    /* [in] */ Vector<AutoPtr<FixedViewInfo> >& where,
    /* [in] */ IInterface* tag)
{
    AutoPtr<IView> v = NULL;
    for(Vector<AutoPtr<FixedViewInfo> >::Iterator iter = where.Begin();
        iter != where.End(); iter++) {
        v = (*iter)->mView;

        Boolean res = FALSE;
        v->IsRootNamespace(&res);
        if (res) {
            View* view = (View*)IView::Probe(v);
            v = view->FindViewWithTag(tag);
            if (v != NULL) return v;
        }
    }
    return NULL;
}

AutoPtr<IView> ListView::FindViewByPredicateTraversal(
    /* [in] */ IPredicate* predicate,
    /* [in] */ IView* childToSkip)
{
    AutoPtr<IView> v = NULL;
    v = AbsListView::FindViewByPredicateTraversal(predicate, childToSkip);
    if ( v == NULL) {
        v = FindViewByPredicateInHeadersOrFooters(mHeaderViewInfos, predicate, childToSkip);
        if (v != NULL) return v;

        v = FindViewByPredicateInHeadersOrFooters(mFooterViewInfos, predicate, childToSkip);
        if (v != NULL) return v;
    }
    return v;
}

AutoPtr<IView> ListView::FindViewByPredicateInHeadersOrFooters(
    /* [in] */ Vector<AutoPtr<FixedViewInfo> >& where,
    /* [in] */ IPredicate* predicate,
    /* [in] */ IView* childToSkip)
{
   AutoPtr<IView> v, subV;
   for(Vector<AutoPtr<FixedViewInfo> >::Iterator iter = where.Begin();
        iter != where.End(); iter++) {
        v = (*iter)->mView;
        Boolean res = FALSE;
        v->IsRootNamespace(&res);
        if (v.Get() != childToSkip && !res) {
            subV = NULL;
            v->FindViewByPredicate(predicate, (IView**)&subV);
            if (subV != NULL) return v;
        }
   }
   return NULL;
}

AutoPtr<ArrayOf<Int64> > ListView::GetCheckItemIds()
{
    Boolean res = FALSE;
    AutoPtr<ArrayOf<Int64> > array;
    mAdapter->HasStableIds(&res);
    if (mAdapter != NULL && res) {
        GetCheckedItemIds((ArrayOf<Int64>**)&array);
        return array;
    }

    if (mChoiceMode != IAbsListView::CHOICE_MODE_NONE && mCheckStates != NULL && mAdapter != NULL) {
        AutoPtr<HashMap<Int32, Boolean> > states = mCheckStates;
        Int32 count = states->GetSize();
        AutoPtr<ArrayOf<Int64> > ids = ArrayOf<Int64>::Alloc(count);
        AutoPtr<IListAdapter> adapter = mAdapter;

        Int32 checkedCount = 0;
        for(HashMap<Int32, Boolean>::Iterator iter = states->Begin(); iter != states->End(); iter++) {
            adapter->GetItemId(iter->mFirst, &((*ids)[checkedCount++]));
        }
        if (checkedCount == count) {
            return ids;
        } else {
            AutoPtr<ArrayOf<Int64> > result = ArrayOf<Int64>::Alloc(checkedCount);
            result->Copy(0, ids, checkedCount);
            return result;
        }
    }
    return NULL;
}

ECode ListView::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    AbsListView::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(LISTVIEW_NAME, (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

ECode ListView::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    AbsListView::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(LISTVIEW_NAME, (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}


}// amespace Widget
}// namespace Droid
}// namespace Elastos
