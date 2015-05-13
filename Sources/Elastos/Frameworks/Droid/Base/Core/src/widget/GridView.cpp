
#include "widget/GridView.h"
#include <elastos/Math.h>
#include "widget/CAbsListViewLayoutParams.h"
#include "view/SoundEffectConstants.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/animation/GridLayoutAnimationController.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::View::SoundEffectConstants;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::Animation::GridLayoutAnimationController;
using Elastos::Droid::View::Animation::IGridLayoutAnimationParameters;
using Elastos::Droid::Widget::CAbsListViewLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {

const String GridView::GRIDVIEW_NAME = String("GridView");
const Int32 GridView::NO_STRETCH;
const Int32 GridView::STRETCH_SPACING;
const Int32 GridView::STRETCH_COLUMN_WIDTH;
const Int32 GridView::STRETCH_SPACING_UNIFORM;
const Int32 GridView::AUTO_FIT;

GridView::GridView()
    : mNumColumns(AUTO_FIT)
    , mHorizontalSpacing(0)
    , mRequestedHorizontalSpacing(0)
    , mVerticalSpacing(0)
    , mStretchMode(STRETCH_COLUMN_WIDTH)
    , mColumnWidth(0)
    , mRequestedColumnWidth(0)
    , mRequestedNumColumns(0)
    , mGravity(IGravity::START)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));
}

ECode GridView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    assert(context != NULL);
    AbsListView::Init(context, attrs, defStyle);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::GridView),
            ARRAY_SIZE(R::styleable::GridView));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(
            attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

    Int32 hSpacing;
    a->GetDimensionPixelOffset(
        R::styleable::GridView_horizontalSpacing, 0, &hSpacing);
    SetHorizontalSpacing(hSpacing);

    Int32 vSpacing;
    a->GetDimensionPixelOffset(
        R::styleable::GridView_verticalSpacing, 0, &vSpacing);
    SetVerticalSpacing(vSpacing);

    Int32 index;
    a->GetInt32(R::styleable::GridView_stretchMode, STRETCH_COLUMN_WIDTH, &index);
    if (index >= 0) {
        SetStretchMode(index);
    }

    Int32 columnWidth;
    a->GetDimensionPixelOffset(R::styleable::GridView_columnWidth, -1, &columnWidth);
    if (columnWidth > 0) {
        SetColumnWidth(columnWidth);
    }

    Int32 numColumns;
    a->GetInt32(R::styleable::GridView_numColumns, 1, &numColumns);
    SetNumColumns(numColumns);

    a->GetInt32(R::styleable::GridView_gravity, -1, &index);
    if (index >= 0) {
        SetGravity(index);
    }

    a->Recycle();
    return NOERROR;
}

ECode GridView::SetRemoteViewsAdapter(
        /* [in] */ IIntent* intent)
{
    return AbsListView::SetRemoteViewsAdapter(intent);
}

AutoPtr<IAdapter> GridView::GetAdapter()
{
    return mAdapter;
}

ECode GridView::SetAdapter(
    /* [in] */ IAdapter* adapter)
{
    if (mAdapter != NULL && mDataSetObserver != NULL) {
        mAdapter->UnregisterDataSetObserver(mDataSetObserver);
    }

    ResetList();
    mRecycler->Clear();
    mAdapter = IListAdapter::Probe(adapter);

    mOldSelectedPosition = IAdapterView::INVALID_POSITION;
    mOldSelectedRowId = IAdapterView::INVALID_ROW_ID;

    if (mAdapter != NULL) {
        mOldItemCount = mItemCount;
        mAdapter->GetCount(&mItemCount);
        mDataChanged = TRUE;
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
        CheckSelectionChanged();
    } else {
        CheckFocus();
        CheckSelectionChanged();
    }

    RequestLayout();

    return NOERROR;
}

Int32 GridView::LookForSelectablePosition(
    /* [in] */ Int32 position,
    /* [in] */ Boolean lookDown)
{
    AutoPtr<IListAdapter> adapter = mAdapter;
    if (adapter == NULL || IsInTouchMode()) {
        return IAdapterView::INVALID_POSITION;
    }

    if (position < 0 || position >= mItemCount) {
        return IAdapterView::INVALID_POSITION;
    }
    return position;
}

ECode GridView::FillGap(
    /* [in] */ Boolean down)
{
    Int32 numColumns = mNumColumns;
    Int32 verticalSpacing = mVerticalSpacing;

    Int32 count = GetChildCount();

    if (down) {
        Int32 bottom;
        GetChildAt(count - 1)->GetBottom(&bottom);
        Int32 startOffset = count > 0 ?
                bottom + verticalSpacing : GetListPaddingTop();
        Int32 position = mFirstPosition + count;
        if (mStackFromBottom) {
            position += numColumns - 1;
        }
        FillDown(position, startOffset);
        CorrectTooHigh(numColumns, verticalSpacing, GetChildCount());
    } else {
        Int32 top;
        GetChildAt(0)->GetTop(&top);

        Int32 startOffset = count > 0 ?
                top - verticalSpacing : GetHeight() - GetListPaddingBottom();
        Int32 position = mFirstPosition;
        if (!mStackFromBottom) {
            position -= numColumns;
        } else {
            position--;
        }
        FillUp(position, startOffset);
        CorrectTooLow(numColumns, verticalSpacing, GetChildCount());
    }
    return NOERROR;
}

AutoPtr<IView> GridView::FillDown(
    /* [in] */ Int32 pos,
    /* [in] */ Int32 nextTop)
{
    AutoPtr<IView> selectedView;

    Int32 end = (mBottom - mTop) - mListPadding->mBottom;

    while (nextTop < end && pos < mItemCount) {
        AutoPtr<IView> temp = MakeRow(pos, nextTop, TRUE);
        if (temp != NULL) {
            selectedView = temp;
        }

        mReferenceView->GetBottom(&nextTop);
        nextTop += mVerticalSpacing;

        pos += mNumColumns;
    }

    return selectedView;
}

AutoPtr<IView> GridView::MakeRow(
    /* [in] */ Int32 startPos,
    /* [in] */ Int32 y,
    /* [in] */ Boolean flow)
{
    Int32 columnWidth = mColumnWidth;
    Int32 horizontalSpacing = mHorizontalSpacing;

    Int32 last;
    Int32 nextLeft = mListPadding->mLeft +
            ((mStretchMode == STRETCH_SPACING_UNIFORM) ? horizontalSpacing : 0);

    if (!mStackFromBottom) {
        last = Elastos::Core::Math::Min(startPos + mNumColumns, mItemCount);
    } else {
        last = startPos + 1;
        startPos = Elastos::Core::Math::Max(0, startPos - mNumColumns + 1);

        if (last - startPos < mNumColumns) {
            nextLeft += (mNumColumns - (last - startPos)) * (columnWidth + horizontalSpacing);
        }
    }

    AutoPtr<IView> selectedView = NULL;

    Boolean hasFocus = ShouldShowSelector();
    Boolean inClick = TouchModeDrawsInPressedState();
    Int32 selectedPosition = mSelectedPosition;

    AutoPtr<IView> child = NULL;
    for (Int32 pos = startPos; pos < last; pos++) {
        Boolean selected = pos == selectedPosition;

        Int32 where = flow ? -1 : pos - startPos;
        child = MakeAndAddView(pos, y, flow, nextLeft, selected, where);

        nextLeft += columnWidth;
        if (pos < last - 1) {
            nextLeft += horizontalSpacing;
        }

        if (selected && (hasFocus || inClick)) {
            selectedView = child;
        }
    }

    mReferenceView = child;

    if (selectedView != NULL) {
        mReferenceViewInSelectedRow = mReferenceView;
    }

    return selectedView;
}

AutoPtr<IView> GridView::FillUp(
    /* [in] */ Int32 pos,
    /* [in] */ Int32 nextBottom)
{
    AutoPtr<IView> selectedView = NULL;

    Int32 end = mListPadding->mTop;

    while (nextBottom > end && pos >= 0) {

        AutoPtr<IView> temp = MakeRow(pos, nextBottom, FALSE);
        if (temp != NULL) {
            selectedView = temp;
        }

        mReferenceView->GetTop(&nextBottom);
        nextBottom -= mVerticalSpacing;

        mFirstPosition = pos;

        pos -= mNumColumns;
    }

    if (mStackFromBottom) {
        mFirstPosition = Elastos::Core::Math::Max(0, pos + 1);
    }

    return selectedView;
}

AutoPtr<IView> GridView::FillFromTop(
    /* [in] */ Int32 nextTop)
{
    mFirstPosition = Elastos::Core::Math::Min(mFirstPosition, mSelectedPosition);
    mFirstPosition = Elastos::Core::Math::Min(mFirstPosition, mItemCount - 1);
    if (mFirstPosition < 0) {
        mFirstPosition = 0;
    }
    mFirstPosition -= mFirstPosition % mNumColumns;
    return FillDown(mFirstPosition, nextTop);
}

AutoPtr<IView> GridView::FillFromBottom(
    /* [in] */ Int32 lastPosition,
    /* [in] */ Int32 nextBottom)
{
    lastPosition = Elastos::Core::Math::Max(lastPosition, mSelectedPosition);
    lastPosition = Elastos::Core::Math::Min(lastPosition, mItemCount - 1);

    Int32 invertedPosition = mItemCount - 1 - lastPosition;
    lastPosition = mItemCount - 1 - (invertedPosition - (invertedPosition % mNumColumns));

    return FillUp(lastPosition, nextBottom);
}

AutoPtr<IView> GridView::FillSelection(
    /* [in] */ Int32 childrenTop,
    /* [in] */ Int32 childrenBottom)
{
    Int32 selectedPosition = ReconcileSelectedPosition();
    Int32 numColumns = mNumColumns;
    Int32 verticalSpacing = mVerticalSpacing;

    Int32 rowStart;
    Int32 rowEnd = -1;

    if (!mStackFromBottom) {
        rowStart = selectedPosition - (selectedPosition % numColumns);
    } else {
        Int32 invertedSelection = mItemCount - 1 - selectedPosition;

        rowEnd = mItemCount - 1 - (invertedSelection - (invertedSelection % numColumns));
        rowStart = Elastos::Core::Math::Max(0, rowEnd - numColumns + 1);
    }

    Int32 fadingEdgeLength = GetVerticalFadingEdgeLength();
    Int32 topSelectionPixel = GetTopSelectionPixel(childrenTop, fadingEdgeLength, rowStart);

    AutoPtr<IView> sel = MakeRow(mStackFromBottom ? rowEnd : rowStart, topSelectionPixel, TRUE);
    mFirstPosition = rowStart;

    AutoPtr<IView> referenceView = mReferenceView;

    Int32 bottom, top;
    if (!mStackFromBottom) {
        referenceView->GetBottom(&bottom);
        FillDown(rowStart + numColumns, bottom + verticalSpacing);
        PinToBottom(childrenBottom);

        referenceView->GetTop(&top);
        FillUp(rowStart - numColumns, top - verticalSpacing);
        AdjustViewsUpOrDown();
    } else {
        Int32 bottomSelectionPixel = GetBottomSelectionPixel(childrenBottom,
                fadingEdgeLength, numColumns, rowStart);
        referenceView->GetBottom(&bottom);
        Int32 offset = bottomSelectionPixel - bottom;
        OffsetChildrenTopAndBottom(offset);

        referenceView->GetTop(&top);
        FillUp(rowStart - 1, top - verticalSpacing);
        PinToTop(childrenTop);
        FillDown(rowEnd + numColumns, bottom + verticalSpacing);
        AdjustViewsUpOrDown();
    }

    return sel;
}

void GridView::PinToTop(
    /* [in] */ Int32 childrenTop)
{
    if (mFirstPosition == 0) {
        Int32 top;
        GetChildAt(0)->GetTop(&top);
        Int32 offset = childrenTop - top;
        if (offset < 0) {
            OffsetChildrenTopAndBottom(offset);
        }
    }
}

void GridView::PinToBottom(
    /* [in] */ Int32 childrenBottom)
{
    Int32 count = GetChildCount();
    if (mFirstPosition + count == mItemCount) {
        Int32 bottom;
        GetChildAt(count - 1)->GetBottom(&bottom);
        Int32 offset = childrenBottom - bottom;
        if (offset > 0) {
            OffsetChildrenTopAndBottom(offset);
        }
    }
}

Int32 GridView::FindMotionRow(
    /* [in] */ Int32 y)
{
    Int32 childCount = GetChildCount();
    if (childCount > 0) {

        Int32 numColumns = mNumColumns;
        if (!mStackFromBottom) {
            for (Int32 i = 0; i < childCount; i += numColumns) {
                Int32 bottom;
                GetChildAt(i)->GetBottom(&bottom);
                if (y <= bottom) {
                    return mFirstPosition + i;
                }
            }
        } else {
            for (Int32 i = childCount - 1; i >= 0; i -= numColumns) {
                Int32 top;
                GetChildAt(i)->GetTop(&top);
                if (y >= top) {
                    return mFirstPosition + i;
                }
            }
        }
    }
    return IAdapterView::INVALID_POSITION;
}

ECode GridView::SmoothScrollToPosition(
    /* [in] */ Int32 position)
{
    return AbsListView::SmoothScrollToPosition(position);
}

ECode GridView::SmoothScrollByOffset(
    /* [in] */ Int32 offset)
{
    return AbsListView::SmoothScrollByOffset(offset);
}

Int32 GridView::GetGravity()
{
    return mGravity;
}

Int32 GridView::GetHorizontalSpacing()
{
    return mHorizontalSpacing;
}

Int32 GridView::GetRequestedHorizontalSpacing()
{
    return mRequestedHorizontalSpacing;
}

Int32 GridView::GetVerticalSpacing()
{
    return mVerticalSpacing;
}

Int32 GridView::GetColumnWidth()
{
    return mColumnWidth;
}

Int32 GridView::GetRequestedColumnWidth()
{
    return mRequestedColumnWidth;
}

Int32 GridView::GetNumColumns()
{
    return mNumColumns;
}

CARAPI GridView::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    AbsListView::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(GRIDVIEW_NAME, (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

CARAPI GridView::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    AbsListView::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(GRIDVIEW_NAME, (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}

AutoPtr<IView> GridView::FillSpecific(
    /* [in] */ Int32 position,
    /* [in] */ Int32 top)
{
    Int32 numColumns = mNumColumns;

    Int32 motionRowStart;
    Int32 motionRowEnd = -1;

    if (!mStackFromBottom) {
        motionRowStart = position - (position % numColumns);
    } else {
        Int32 invertedSelection = mItemCount - 1 - position;

        motionRowEnd = mItemCount - 1 - (invertedSelection - (invertedSelection % numColumns));
        motionRowStart = Elastos::Core::Math::Max(0, motionRowEnd - numColumns + 1);
    }

    AutoPtr<IView> temp = MakeRow(mStackFromBottom ? motionRowEnd : motionRowStart, top, TRUE);

    mFirstPosition = motionRowStart;

    AutoPtr<IView> referenceView = mReferenceView;
    if (referenceView == NULL) {
        return NULL;
    }

    Int32 verticalSpacing = mVerticalSpacing;

    AutoPtr<IView> above;
    AutoPtr<IView> below;

    Int32 t, bottom;
    if (!mStackFromBottom) {

        referenceView->GetTop(&t);
        referenceView->GetBottom(&bottom);

        above = FillUp(motionRowStart - numColumns, t - verticalSpacing);
        AdjustViewsUpOrDown();
        below = FillDown(motionRowStart + numColumns, bottom + verticalSpacing);
        Int32 childCount = GetChildCount();
        if (childCount > 0) {
            CorrectTooHigh(numColumns, verticalSpacing, childCount);
        }
    } else {
        referenceView->GetBottom(&bottom);
        referenceView->GetTop(&t);

        below = FillDown(motionRowEnd + numColumns, bottom + verticalSpacing);
        AdjustViewsUpOrDown();
        above = FillUp(motionRowStart - 1, t - verticalSpacing);
        Int32 childCount = GetChildCount();
        if (childCount > 0) {
            CorrectTooLow(numColumns, verticalSpacing, childCount);
        }
    }

    if (temp != NULL) {
        return temp;
    } else if (above != NULL) {
        return above;
    } else {
        return below;
    }
}

void GridView::CorrectTooHigh(
    /* [in] */ Int32 numColumns,
    /* [in] */ Int32 verticalSpacing,
    /* [in] */ Int32 childCount)
{
    Int32 lastPosition = mFirstPosition + childCount - 1;
    if (lastPosition == mItemCount - 1 && childCount > 0) {
        AutoPtr<IView> lastChild = GetChildAt(childCount - 1);

        Int32 lastBottom;
        lastChild->GetBottom(&lastBottom);
        Int32 end = (mBottom - mTop) - mListPadding->mBottom;

        Int32 bottomOffset = end - lastBottom;

        AutoPtr<IView> firstChild = GetChildAt(0);
        Int32 firstTop;
        firstChild->GetTop(&firstTop);

        if (bottomOffset > 0 && (mFirstPosition > 0 || firstTop < mListPadding->mTop))  {
            if (mFirstPosition == 0) {
                bottomOffset = Elastos::Core::Math::Min(bottomOffset, mListPadding->mTop - firstTop);
            }

            OffsetChildrenTopAndBottom(bottomOffset);
            if (mFirstPosition > 0) {
                Int32 top;
                firstChild->GetTop(&top);
                FillUp(mFirstPosition - (mStackFromBottom ? 1 : numColumns),
                        top - verticalSpacing);
                AdjustViewsUpOrDown();
            }
        }
    }
}

void GridView::CorrectTooLow(
    /* [in] */ Int32 numColumns,
    /* [in] */ Int32 verticalSpacing,
    /* [in] */ Int32 childCount)
{
    if (mFirstPosition == 0 && childCount > 0) {
        AutoPtr<IView> firstChild = GetChildAt(0);

        Int32 firstTop;
        firstChild->GetTop(&firstTop);

        Int32 start = mListPadding->mTop;

        Int32 end = (mBottom - mTop) - mListPadding->mBottom;

        Int32 topOffset = firstTop - start;
        AutoPtr<IView> lastChild = GetChildAt(childCount - 1);
        Int32 lastBottom;
        lastChild->GetBottom(&lastBottom);
        Int32 lastPosition = mFirstPosition + childCount - 1;

        if (topOffset > 0 && (lastPosition < mItemCount - 1 || lastBottom > end))  {
            if (lastPosition == mItemCount - 1 ) {
                topOffset = Elastos::Core::Math::Min(topOffset, lastBottom - end);
            }

            OffsetChildrenTopAndBottom(-topOffset);
            if (lastPosition < mItemCount - 1) {
                Int32 bottom;
                lastChild->GetBottom(&bottom);
                FillDown(lastPosition + (!mStackFromBottom ? 1 : numColumns),
                       bottom + verticalSpacing);
                AdjustViewsUpOrDown();
            }
        }
    }
}

AutoPtr<IView> GridView::FillFromSelection(
    /* [in] */ Int32 selectedTop,
    /* [in] */ Int32 childrenTop,
    /* [in] */ Int32 childrenBottom)
{
    Int32 fadingEdgeLength = GetVerticalFadingEdgeLength();
    Int32 selectedPosition = mSelectedPosition;
    Int32 numColumns = mNumColumns;
    Int32 verticalSpacing = mVerticalSpacing;

    Int32 rowStart;
    Int32 rowEnd = -1;

    if (!mStackFromBottom) {
        rowStart = selectedPosition - (selectedPosition % numColumns);
    } else {
        Int32 invertedSelection = mItemCount - 1 - selectedPosition;

        rowEnd = mItemCount - 1 - (invertedSelection - (invertedSelection % numColumns));
        rowStart = Elastos::Core::Math::Max(0, rowEnd - numColumns + 1);
    }

    AutoPtr<IView> sel;
    AutoPtr<IView> referenceView;

    Int32 topSelectionPixel = GetTopSelectionPixel(childrenTop, fadingEdgeLength, rowStart);
    Int32 bottomSelectionPixel = GetBottomSelectionPixel(childrenBottom, fadingEdgeLength,
            numColumns, rowStart);

    sel = MakeRow(mStackFromBottom ? rowEnd : rowStart, selectedTop, TRUE);
    mFirstPosition = rowStart;

    referenceView = mReferenceView;
    AdjustForTopFadingEdge(referenceView, topSelectionPixel, bottomSelectionPixel);
    AdjustForBottomFadingEdge(referenceView, topSelectionPixel, bottomSelectionPixel);

    Int32 top, bottom;
    referenceView->GetTop(&top);
    referenceView->GetBottom(&bottom);

    if (!mStackFromBottom) {
        FillUp(rowStart - numColumns, top - verticalSpacing);
        AdjustViewsUpOrDown();
        FillDown(rowStart + numColumns, bottom + verticalSpacing);
    } else {
        FillDown(rowEnd + numColumns, bottom + verticalSpacing);
        AdjustViewsUpOrDown();
        FillUp(rowStart - 1, top - verticalSpacing);
    }


    return sel;
}

Int32 GridView::GetBottomSelectionPixel(
    /* [in] */ Int32 childrenBottom,
    /* [in] */ Int32 fadingEdgeLength,
    /* [in] */ Int32 numColumns,
    /* [in] */ Int32 rowStart)
{
    Int32 bottomSelectionPixel = childrenBottom;
    if (rowStart + numColumns - 1 < mItemCount - 1) {
        bottomSelectionPixel -= fadingEdgeLength;
    }
    return bottomSelectionPixel;
}

Int32 GridView::GetTopSelectionPixel(
    /* [in] */ Int32 childrenTop,
    /* [in] */ Int32 fadingEdgeLength,
    /* [in] */ Int32 rowStart)
{
    Int32 topSelectionPixel = childrenTop;
    if (rowStart > 0) {
        topSelectionPixel += fadingEdgeLength;
    }
    return topSelectionPixel;
}

void GridView::AdjustForBottomFadingEdge(
    /* [in] */ IView* childInSelectedRow,
    /* [in] */ Int32 topSelectionPixel,
    /* [in] */ Int32 bottomSelectionPixel)
{
    Int32 bottom, top;
    childInSelectedRow->GetBottom(&bottom);
    childInSelectedRow->GetTop(&top);
    if (bottom > bottomSelectionPixel) {

        Int32 spaceAbove = top - topSelectionPixel;

        Int32 spaceBelow = bottom - bottomSelectionPixel;
        Int32 offset = Elastos::Core::Math::Min(spaceAbove, spaceBelow);

        OffsetChildrenTopAndBottom(-offset);
    }
}

void GridView::AdjustForTopFadingEdge(
    /* [in] */ IView* childInSelectedRow,
    /* [in] */ Int32 topSelectionPixel,
    /* [in] */ Int32 bottomSelectionPixel)
{
    Int32 top, bottom;
    childInSelectedRow->GetTop(&top);
    childInSelectedRow->GetBottom(&bottom);
    if (top < topSelectionPixel) {
        Int32 spaceAbove = topSelectionPixel - top;

        Int32 spaceBelow = bottomSelectionPixel - bottom;
        Int32 offset = Elastos::Core::Math::Min(spaceAbove, spaceBelow);

        OffsetChildrenTopAndBottom(offset);
    }
}

AutoPtr<IView> GridView::MoveSelection(
    /* [in] */ Int32 delta,
    /* [in] */ Int32 childrenTop,
    /* [in] */ Int32 childrenBottom)
{
    Int32 fadingEdgeLength = GetVerticalFadingEdgeLength();
    Int32 selectedPosition = mSelectedPosition;
    Int32 numColumns = mNumColumns;
    Int32 verticalSpacing = mVerticalSpacing;

    Int32 oldRowStart;
    Int32 rowStart;
    Int32 rowEnd = -1;

    if (!mStackFromBottom) {
        oldRowStart = (selectedPosition - delta) - ((selectedPosition - delta) % numColumns);

        rowStart = selectedPosition - (selectedPosition % numColumns);
    } else {
        Int32 invertedSelection = mItemCount - 1 - selectedPosition;

        rowEnd = mItemCount - 1 - (invertedSelection - (invertedSelection % numColumns));
        rowStart = Elastos::Core::Math::Max(0, rowEnd - numColumns + 1);

        invertedSelection = mItemCount - 1 - (selectedPosition - delta);
        oldRowStart = mItemCount - 1 - (invertedSelection - (invertedSelection % numColumns));
        oldRowStart = Elastos::Core::Math::Max(0, oldRowStart - numColumns + 1);
    }

    Int32 rowDelta = rowStart - oldRowStart;

    Int32 topSelectionPixel = GetTopSelectionPixel(childrenTop, fadingEdgeLength, rowStart);
    Int32 bottomSelectionPixel = GetBottomSelectionPixel(childrenBottom, fadingEdgeLength,
            numColumns, rowStart);

    mFirstPosition = rowStart;

    AutoPtr<IView> sel;
    AutoPtr<IView> referenceView;

    if (rowDelta > 0) {
        /*
         * Case 1: Scrolling down.
         */
        Int32 oldBottom = 0;
        if (mReferenceViewInSelectedRow != NULL) {
            mReferenceViewInSelectedRow->GetBottom(&oldBottom);
        }

        sel = MakeRow(mStackFromBottom ? rowEnd : rowStart, oldBottom + verticalSpacing, TRUE);
        referenceView = mReferenceView;

        AdjustForBottomFadingEdge(referenceView, topSelectionPixel, bottomSelectionPixel);
    } else if (rowDelta < 0) {
        /*
         * Case 2: Scrolling up.
         */
        Int32 oldTop = 0;
        if (mReferenceViewInSelectedRow != NULL) {
            mReferenceViewInSelectedRow->GetTop(&oldTop);
        }

        sel = MakeRow(mStackFromBottom ? rowEnd : rowStart, oldTop - verticalSpacing, FALSE);
        referenceView = mReferenceView;

        AdjustForTopFadingEdge(referenceView, topSelectionPixel, bottomSelectionPixel);
    } else {
        /*
         * Keep selection where it was
         */
        Int32 oldTop = 0;
        if (mReferenceViewInSelectedRow != NULL) {
            mReferenceViewInSelectedRow->GetTop(&oldTop);
        }

        sel = MakeRow(mStackFromBottom ? rowEnd : rowStart, oldTop, TRUE);
        referenceView = mReferenceView;
    }

    Int32 bottom = 0, top = 0;
    referenceView->GetTop(&top);
    referenceView->GetBottom(&bottom);

    if (!mStackFromBottom) {
        FillUp(rowStart - numColumns, top - verticalSpacing);
        AdjustViewsUpOrDown();
        FillDown(rowStart + numColumns, bottom + verticalSpacing);
    } else {
        FillDown(rowEnd + numColumns, bottom + verticalSpacing);
        AdjustViewsUpOrDown();
        FillUp(rowStart - 1, top - verticalSpacing);
    }

    return sel;
}

Boolean GridView::DetermineColumns(
    /* [in] */ Int32 availableSpace)
{
    Int32 requestedHorizontalSpacing = mRequestedHorizontalSpacing;
    Int32 stretchMode = mStretchMode;
    Int32 requestedColumnWidth = mRequestedColumnWidth;
    Boolean didNotInitiallyFit = FALSE;

    if (mRequestedNumColumns == AUTO_FIT) {
        if (requestedColumnWidth > 0) {
            mNumColumns = (availableSpace + requestedHorizontalSpacing) /
                    (requestedColumnWidth + requestedHorizontalSpacing);
        } else {
            mNumColumns = 2;
        }
    } else {
        mNumColumns = mRequestedNumColumns;
    }

    if (mNumColumns <= 0) {
        mNumColumns = 1;
    }

    switch (stretchMode) {
    case NO_STRETCH:
        mColumnWidth = requestedColumnWidth;
        mHorizontalSpacing = requestedHorizontalSpacing;
        break;

    default:
        Int32 spaceLeftOver = availableSpace - (mNumColumns * requestedColumnWidth) -
                ((mNumColumns - 1) * requestedHorizontalSpacing);

        if(spaceLeftOver < 0) {
            didNotInitiallyFit = TRUE;
        }

        switch (stretchMode) {
        case STRETCH_COLUMN_WIDTH:
            mColumnWidth = requestedColumnWidth + spaceLeftOver / mNumColumns;
            mHorizontalSpacing = requestedHorizontalSpacing;
            break;

        case STRETCH_SPACING:
            mColumnWidth = requestedColumnWidth;
            if (mNumColumns > 1) {
                mHorizontalSpacing = requestedHorizontalSpacing +
                    spaceLeftOver / (mNumColumns - 1);
            } else {
                mHorizontalSpacing = requestedHorizontalSpacing + spaceLeftOver;
            }
            break;

        case STRETCH_SPACING_UNIFORM:
            mColumnWidth = requestedColumnWidth;
            if (mNumColumns > 1) {
                mHorizontalSpacing = requestedHorizontalSpacing +
                    spaceLeftOver / (mNumColumns + 1);
            } else {
                mHorizontalSpacing = requestedHorizontalSpacing + spaceLeftOver;
            }
            break;
        }

        break;
    }
    return didNotInitiallyFit;
}

void GridView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    AbsListView::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);
    Int32 widthSize = MeasureSpec::GetSize(widthMeasureSpec);
    Int32 heightSize = MeasureSpec::GetSize(heightMeasureSpec);

    if (widthMode == MeasureSpec::UNSPECIFIED) {
        if (mColumnWidth > 0) {
            widthSize = mColumnWidth + mListPadding->mLeft + mListPadding->mRight;
        } else {
            widthSize = mListPadding->mLeft + mListPadding->mRight;
        }
        widthSize += GetVerticalScrollbarWidth();
    }

    Int32 childWidth = widthSize - mListPadding->mLeft - mListPadding->mRight;
    DetermineColumns(childWidth);

    Int32 childHeight = 0;
    mItemCount = 0;
    if (mAdapter != NULL) {
        mAdapter->GetCount(&mItemCount);
    }

    Int32 count = mItemCount;
    if (count > 0) {
        AutoPtr<IView> child = ObtainView(0, mIsScrap);

        AutoPtr<IAbsListViewLayoutParams> p;
        child->GetLayoutParams((IViewGroupLayoutParams**)&p);
        if (p == NULL) {
            CAbsListViewLayoutParams::New(IViewGroupLayoutParams::MATCH_PARENT,
                    IViewGroupLayoutParams::WRAP_CONTENT, 0, (IAbsListViewLayoutParams**)&p);
            child->SetLayoutParams(p);
        }
        mAdapter->GetItemViewType(0, &((CAbsListViewLayoutParams*)p.Get())->mViewType);
        ((CAbsListViewLayoutParams*)p.Get())->mForceAdd = TRUE;

        Int32 childHeightSpec = GetChildMeasureSpec(
            MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED), 0, ((CAbsListViewLayoutParams*)p.Get())->mHeight);
        Int32 childWidthSpec = GetChildMeasureSpec(
            MeasureSpec::MakeMeasureSpec(mColumnWidth, MeasureSpec::EXACTLY), 0, ((CAbsListViewLayoutParams*)p.Get())->mWidth);
        child->Measure(childWidthSpec, childHeightSpec);

        child->GetMeasuredHeight(&childHeight);

        if (mRecycler->ShouldRecycleViewType(((CAbsListViewLayoutParams*)p.Get())->mViewType)) {
            mRecycler->AddScrapView(child, -1);
        }
    }

    if (heightMode == MeasureSpec::UNSPECIFIED) {
        heightSize = mListPadding->mTop + mListPadding->mBottom + childHeight +
                GetVerticalFadingEdgeLength() * 2;
    }

    if (heightMode == MeasureSpec::AT_MOST) {
        Int32 ourSize =  mListPadding->mTop + mListPadding->mBottom;

        Int32 numColumns = mNumColumns;
        for (Int32 i = 0; i < count; i += numColumns) {
            ourSize += childHeight;
            if (i + numColumns < count) {
                ourSize += mVerticalSpacing;
            }
            if (ourSize >= heightSize) {
                ourSize = heightSize;
                break;
            }
        }
        heightSize = ourSize;
    }

    SetMeasuredDimension(widthSize, heightSize);
    mWidthMeasureSpec = widthMeasureSpec;
}

ECode GridView::AttachLayoutAnimationParameters(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* params,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count)
{
    AutoPtr<IAnimationParameters> temp;
    params->GetLayoutAnimationParameters((IAnimationParameters**)&temp);
    AutoPtr<IGridLayoutAnimationParameters> animationParams = IGridLayoutAnimationParameters::Probe(temp);

    if(animationParams == NULL) {
        animationParams = new GridLayoutAnimationController::GridLayoutAnimationParameters();
        params->SetLayoutAnimationParameters(IAnimationParameters::Probe(animationParams));
    }

    animationParams->SetCount(count);
    animationParams->SetIndex(index);
    animationParams->SetColumnsCount(mNumColumns);
    animationParams->SetRowsCount(count / mNumColumns);

    if(!mStackFromBottom) {
        animationParams->SetColumn(index % mNumColumns);
        animationParams->SetRow(index / mNumColumns);
    } else {
        Int32 invertedIndex = count - 1 - index;

        animationParams->SetColumn(mNumColumns - 1 - (invertedIndex & mNumColumns));
        Int32 rowCount = 0;
        animationParams->GetRowsCount(&rowCount);
        animationParams->SetRow(rowCount - 1 - invertedIndex / mNumColumns);
    }

    return NOERROR;
}

void GridView::LayoutChildren()
{
    Boolean blockLayoutRequests = mBlockLayoutRequests;
    if (!blockLayoutRequests) {
        mBlockLayoutRequests = TRUE;
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
    Int32 index;
    Int32 delta = 0;

    AutoPtr<IView> sel;
    AutoPtr<IView> oldSel = NULL;
    AutoPtr<IView> oldFirst = NULL;
    AutoPtr<IView> newSel = NULL;

    switch (mLayoutMode) {
    case LAYOUT_SET_SELECTION:
        index = mNextSelectedPosition - mFirstPosition;
        if (index >= 0 && index < childCount) {
            newSel = GetChildAt(index);
        }
        break;
    case LAYOUT_FORCE_TOP:
    case LAYOUT_FORCE_BOTTOM:
    case LAYOUT_SPECIFIC:
    case LAYOUT_SYNC:
        break;
    case LAYOUT_MOVE_SELECTION:
        if (mNextSelectedPosition >= 0) {
            delta = mNextSelectedPosition - mSelectedPosition;
        }
        break;
    default:
        index = mSelectedPosition - mFirstPosition;
        if (index >= 0 && index < childCount) {
            oldSel = GetChildAt(index);
        }

        oldFirst = GetChildAt(0);
    }

    Boolean dataChanged = mDataChanged;
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
    }

    SetSelectedPositionInt(mNextSelectedPosition);

    Int32 firstPosition = mFirstPosition;
    RecycleBin* recycleBin = mRecycler;

    if (dataChanged) {
        for (Int32 i = 0; i < childCount; i++) {
            recycleBin->AddScrapView(GetChildAt(i), firstPosition + 1);
        }
    } else {
        recycleBin->FillActiveViews(childCount, firstPosition);
    }

    DetachAllViewsFromParent();

    Int32 top = 0;
    switch (mLayoutMode) {
    case LAYOUT_SET_SELECTION:
        if (newSel != NULL) {
            newSel->GetTop(&top);
            sel = FillFromSelection(top, childrenTop, childrenBottom);
        } else {
            sel = FillSelection(childrenTop, childrenBottom);
        }
        break;
    case LAYOUT_FORCE_TOP:
        mFirstPosition = 0;
        sel = FillFromTop(childrenTop);
        AdjustViewsUpOrDown();
        break;
    case LAYOUT_FORCE_BOTTOM:
        sel = FillUp(mItemCount - 1, childrenBottom);
        AdjustViewsUpOrDown();
        break;
    case LAYOUT_SPECIFIC:
        sel = FillSpecific(mSelectedPosition, mSpecificTop);
        break;
    case LAYOUT_SYNC:
        sel = FillSpecific(mSyncPosition, mSpecificTop);
        break;
    case LAYOUT_MOVE_SELECTION:
        // Move the selection relative to its old position
        sel = MoveSelection(delta, childrenTop, childrenBottom);
        break;
    default:
        if (childCount == 0) {
            if (!mStackFromBottom) {
                SetSelectedPositionInt(mAdapter == NULL || IsInTouchMode() ?
                        IAdapterView::INVALID_POSITION : 0);
                sel = FillFromTop(childrenTop);
            } else {
                Int32 last = mItemCount - 1;
                SetSelectedPositionInt(mAdapter == NULL || IsInTouchMode() ?
                        IAdapterView::INVALID_POSITION : last);
                sel = FillFromBottom(last, childrenBottom);
            }
        } else {
            if (mSelectedPosition >= 0 && mSelectedPosition < mItemCount) {
                sel = FillSpecific(mSelectedPosition, oldSel == NULL ?
                        childrenTop : (oldSel->GetTop(&top), top));
            } else if (mFirstPosition < mItemCount)  {
                sel = FillSpecific(mFirstPosition, oldFirst == NULL ?
                        childrenTop : (oldFirst->GetTop(&top), top));
            } else {
                sel = FillSpecific(0, childrenTop);
            }
        }
        break;
    }

    recycleBin->ScrapActiveViews();

    if (sel != NULL) {
       PositionSelector(IAdapterView::INVALID_POSITION, sel);
       sel->GetTop(&mSelectedTop);
    } else if (mTouchMode > TOUCH_MODE_DOWN && mTouchMode < TOUCH_MODE_SCROLL) {
        AutoPtr<IView> child = GetChildAt(mMotionPosition - mFirstPosition);
        if (child != NULL) PositionSelector(mMotionPosition, child);
    } else {
        mSelectedTop = 0;
        mSelectorRect->SetEmpty();
    }

    mLayoutMode = LAYOUT_NORMAL;
    mDataChanged = FALSE;
    mNeedSync = FALSE;
    SetNextSelectedPositionInt(mSelectedPosition);

    UpdateScrollIndicators();

    if (mItemCount > 0) {
        CheckSelectionChanged();
    }

    InvokeOnItemScrollListener();
//} finally {
    if (!blockLayoutRequests) {
        mBlockLayoutRequests = FALSE;
    }
//}
}

AutoPtr<IView> GridView::MakeAndAddView(
    /* [in] */ Int32 position,
    /* [in] */ Int32 y,
    /* [in] */ Boolean flow,
    /* [in] */ Int32 childrenLeft,
    /* [in] */ Boolean selected,
    /* [in] */ Int32 where)
{
    AutoPtr<IView> child;

    if (!mDataChanged) {
        child = mRecycler->GetActiveView(position);
        if (child != NULL) {
            SetupChild(child, position, y, flow, childrenLeft, selected, TRUE, where);
            return child;
        }
    }

    child = ObtainView(position, mIsScrap);

    SetupChild(child, position, y, flow, childrenLeft, selected, (*mIsScrap)[0], where);

    return child;
}

void GridView::SetupChild(
    /* [in] */ IView* child,
    /* [in] */ Int32 position,
    /* [in] */ Int32 y,
    /* [in] */ Boolean flow,
    /* [in] */ Int32 childrenLeft,
    /* [in] */ Boolean selected,
    /* [in] */ Boolean recycled,
    /* [in] */ Int32 where)
{
    Boolean isSelected = selected && ShouldShowSelector();

    Boolean sel;
    child->IsSelected(&sel);

    Boolean updateChildSelected = isSelected != sel;
    Int32 mode = mTouchMode;
    Boolean isPressed = mode > TOUCH_MODE_DOWN && mode < TOUCH_MODE_SCROLL &&
            mMotionPosition == position;

    Boolean press;
    child->IsPressed(&press);
    Boolean updateChildPressed = isPressed != press;

    Boolean layoutRequested;
    child->IsLayoutRequested(&layoutRequested);
    Boolean needToMeasure = !recycled || updateChildSelected || layoutRequested;

    // Respect layout params that are already in the view. Otherwise make
    // some up...
    AutoPtr<IAbsListViewLayoutParams> p;
    child->GetLayoutParams((IViewGroupLayoutParams**)&p);
    if (p == NULL) {
        CAbsListViewLayoutParams::New(IViewGroupLayoutParams::MATCH_PARENT,
                IViewGroupLayoutParams::WRAP_CONTENT, 0, (IAbsListViewLayoutParams**)&p);
    }
    mAdapter->GetItemViewType(position, &((CAbsListViewLayoutParams*)p.Get())->mViewType);

    if (recycled && !((CAbsListViewLayoutParams*)p.Get())->mForceAdd) {
        AttachViewToParent(child, where, p);
    } else {
        ((CAbsListViewLayoutParams*)p.Get())->mForceAdd = FALSE;
        AddViewInLayout(child, where, p, TRUE);
    }

    if (updateChildSelected) {
        child->SetSelected(isSelected);
        if (isSelected) {
            View::RequestFocus();
        }
    }

    if (updateChildPressed) {
        child->SetPressed(isPressed);
    }

    if (needToMeasure) {
        Int32 childHeightSpec = ViewGroup::GetChildMeasureSpec(
            MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED), 0, ((CAbsListViewLayoutParams*)p.Get())->mHeight);

        Int32 childWidthSpec = ViewGroup::GetChildMeasureSpec(
            MeasureSpec::MakeMeasureSpec(mColumnWidth, MeasureSpec::EXACTLY), 0, ((CAbsListViewLayoutParams*)p.Get())->mWidth);
        child->Measure(childWidthSpec, childHeightSpec);
    } else {
        CleanupLayoutState(child);
    }

    Int32 w;
    child->GetMeasuredWidth(&w);
    Int32 h;
    child->GetMeasuredHeight(&h);

    Int32 childLeft;
    Int32 childTop = flow ? y : y - h;

    switch (mGravity & IGravity::HORIZONTAL_GRAVITY_MASK) {
    case IGravity::LEFT:
        childLeft = childrenLeft;
        break;
    case IGravity::CENTER_HORIZONTAL:
        childLeft = childrenLeft + ((mColumnWidth - w) / 2);
        break;
    case IGravity::RIGHT:
        childLeft = childrenLeft + mColumnWidth - w;
        break;
    default:
        childLeft = childrenLeft;
        break;
    }

    if (needToMeasure) {
        Int32 childRight = childLeft + w;
        Int32 childBottom = childTop + h;
        child->Layout(childLeft, childTop, childRight, childBottom);
    } else {
        Int32 left, top;
        child->GetLeft(&left);
        child->GetTop(&top);
        child->OffsetLeftAndRight(childLeft - left);
        child->OffsetTopAndBottom(childTop - top);
    }

    if (mCachingStarted) {
        child->SetDrawingCacheEnabled(TRUE);
    }
}

Boolean GridView::SequenceScroll(
    /* [in] */ Int32 direction)
{
    return FALSE;
}

ECode GridView::SetSelection(
    /* [in] */ Int32 position)
{
    if (!IsInTouchMode()) {
        SetNextSelectedPositionInt(position);
    } else {
        mResurrectToPosition = position;
    }
    mLayoutMode = LAYOUT_SET_SELECTION;
    RequestLayout();

    return NOERROR;
}

ECode GridView::SetSelectionInt(
    /* [in] */ Int32 position)
{
    Int32 previousSelectedPosition = mNextSelectedPosition;

    SetNextSelectedPositionInt(position);
    LayoutChildren();

    Int32 next = mStackFromBottom ? mItemCount - 1  - mNextSelectedPosition :
        mNextSelectedPosition;
    Int32 previous = mStackFromBottom ? mItemCount - 1
            - previousSelectedPosition : previousSelectedPosition;

    Int32 nextRow = next / mNumColumns;
    Int32 previousRow = previous / mNumColumns;

    if (nextRow != previousRow) {
        AwakenScrollBars();
    }
    return NOERROR;
}

Boolean GridView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return CommonKey(keyCode, 1, event);
}

Boolean GridView::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 repeatCount,
    /* [in] */ IKeyEvent* event)
{
    return CommonKey(keyCode, repeatCount, event);
}

Boolean GridView::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return CommonKey(keyCode, 1, event);
}

Boolean GridView::CommonKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ IKeyEvent* event)
{
    if (mAdapter == NULL) {
        return FALSE;
    }

    if (mDataChanged) {
        LayoutChildren();
    }

    Boolean handled = FALSE;
    Int32 action;
    event->GetAction(&action);

    if (action != IKeyEvent::ACTION_UP) {
        if (mSelectedPosition < 0) {
            switch (keyCode) {
                case IKeyEvent::KEYCODE_DPAD_UP:
                case IKeyEvent::KEYCODE_DPAD_DOWN:
                case IKeyEvent::KEYCODE_DPAD_LEFT:
                case IKeyEvent::KEYCODE_DPAD_RIGHT:
                case IKeyEvent::KEYCODE_DPAD_CENTER:
                case IKeyEvent::KEYCODE_SPACE:
                case IKeyEvent::KEYCODE_ENTER:
                    ResurrectSelection();
                    return TRUE;
            }
        }

        Boolean press;
        switch (keyCode) {
            case IKeyEvent::KEYCODE_DPAD_LEFT:
                handled = ArrowScroll(IView::FOCUS_LEFT);
                break;

            case IKeyEvent::KEYCODE_DPAD_RIGHT:
                handled = ArrowScroll(IView::FOCUS_RIGHT);
                break;

            case IKeyEvent::KEYCODE_DPAD_UP:

                event->IsAltPressed(&press);
                if (!press) {
                    handled = ArrowScroll(IView::FOCUS_UP);

                } else {
                    handled = FullScroll(IView::FOCUS_UP);
                }
                break;

            case IKeyEvent::KEYCODE_DPAD_DOWN:
                event->IsAltPressed(&press);
                if (!press) {
                    handled = ArrowScroll(IView::FOCUS_DOWN);
                } else {
                    handled = FullScroll(IView::FOCUS_DOWN);
                }
                break;

            case IKeyEvent::KEYCODE_DPAD_CENTER:
            case IKeyEvent::KEYCODE_ENTER: {
                Int32 count;
                event->GetRepeatCount(&count);
                if (GetChildCount() > 0 && count == 0) {
                    KeyPressed();
                }

                return TRUE;
            }

            case IKeyEvent::KEYCODE_SPACE:
                Boolean show = FALSE;
                if (mPopup == NULL || !(mPopup->IsShowing(&show), show)) {
                    event->IsShiftPressed(&press);
                    if (!press) {
                        handled = PageScroll(IView::FOCUS_DOWN);
                    } else {
                        handled = PageScroll(IView::FOCUS_UP);
                    }
                }
                break;
        }
    }

    if (!handled) {
        handled = SendToTextFilter(keyCode, count, event);
    }

    if (handled) {
        return TRUE;
    } else {
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
    return FALSE;
}

Boolean GridView::PageScroll(
    /* [in] */ Int32 direction)
{
    Int32 nextPage = -1;

    if (direction == IView::FOCUS_UP) {
        nextPage = Elastos::Core::Math::Max(0, mSelectedPosition - GetChildCount() - 1);
    } else if (direction == IView::FOCUS_DOWN) {
        nextPage = Elastos::Core::Math::Min(mItemCount - 1, mSelectedPosition + GetChildCount() - 1);
    }

    if (nextPage >= 0) {
        SetSelectionInt(nextPage);
        InvokeOnItemScrollListener();
        AwakenScrollBars();
        return TRUE;
    }

    return FALSE;
}

Boolean GridView::FullScroll(
    /* [in] */ Int32 direction)
{
    Boolean moved = FALSE;
    if (direction == IView::FOCUS_UP) {
        mLayoutMode = LAYOUT_SET_SELECTION;
        SetSelectionInt(0);
        InvokeOnItemScrollListener();
        moved = TRUE;
    } else if (direction == IView::FOCUS_DOWN) {
        mLayoutMode = LAYOUT_SET_SELECTION;
        SetSelectionInt(mItemCount - 1);
        InvokeOnItemScrollListener();
        moved = TRUE;
    }

    if (moved) {
        AwakenScrollBars();
    }

    return moved;
}

Boolean GridView::ArrowScroll(
    /* [in] */ Int32 direction)
{
    Int32 selectedPosition = mSelectedPosition;
    Int32 numColumns = mNumColumns;

    Int32 startOfRowPos;
    Int32 endOfRowPos;

    Boolean moved = FALSE;

    if (!mStackFromBottom) {
        startOfRowPos = (selectedPosition / numColumns) * numColumns;
        endOfRowPos = Elastos::Core::Math::Min(startOfRowPos + numColumns - 1, mItemCount - 1);
    } else {
        Int32 invertedSelection = mItemCount - 1 - selectedPosition;
        endOfRowPos = mItemCount - 1 - (invertedSelection / numColumns) * numColumns;
        startOfRowPos = Elastos::Core::Math::Max(0, endOfRowPos - numColumns + 1);
    }

    switch (direction) {
        case IView::FOCUS_UP:
            if (startOfRowPos > 0) {
                mLayoutMode = LAYOUT_MOVE_SELECTION;
                SetSelectionInt(Elastos::Core::Math::Max(0, selectedPosition - numColumns));
                moved = TRUE;
            }
            break;
        case IView::FOCUS_DOWN:
            if (endOfRowPos < mItemCount - 1) {
                mLayoutMode = LAYOUT_MOVE_SELECTION;
                SetSelectionInt(Elastos::Core::Math::Min(selectedPosition + numColumns, mItemCount - 1));
                moved = TRUE;
            }
            break;
        case IView::FOCUS_LEFT:
            if (selectedPosition > startOfRowPos) {
                mLayoutMode = LAYOUT_MOVE_SELECTION;
                SetSelectionInt(Elastos::Core::Math::Max(0, selectedPosition - 1));
                moved = TRUE;
            }
            break;
        case IView::FOCUS_RIGHT:
            if (selectedPosition < endOfRowPos) {
                mLayoutMode = LAYOUT_MOVE_SELECTION;
                SetSelectionInt(Elastos::Core::Math::Min(selectedPosition + 1, mItemCount - 1));
                moved = TRUE;
            }
            break;
    }

    if (moved) {
        PlaySoundEffect(SoundEffectConstants::GetContantForFocusDirection(direction));
        InvokeOnItemScrollListener();
    }

    if (moved) {
        AwakenScrollBars();
    }

    return moved;
}

void GridView::OnFocusChanged(
    /* [in] */ Boolean gainFocus,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    AbsListView::OnFocusChanged(gainFocus, direction, previouslyFocusedRect);

    Int32 closestChildIndex = -1;
    if (gainFocus && previouslyFocusedRect != NULL) {
        previouslyFocusedRect->Offset(mScrollX, mScrollY);

        // figure out which item should be selected based on previously
        // focused rect
        AutoPtr<CRect> otherRect = mTempRect;
        Int32 minDistance = Elastos::Core::Math::INT32_MAX_VALUE;
        Int32 childCount = GetChildCount();
        for (Int32 i = 0; i < childCount; i++) {
            // only consider view's on appropriate edge of grid
            if (!IsCandidateSelection(i, direction)) {
                continue;
            }

            AutoPtr<IView> other = GetChildAt(i);
            other->GetDrawingRect(otherRect);
            OffsetDescendantRectToMyCoords(other, otherRect);
            Int32 distance = GetDistance(previouslyFocusedRect, otherRect, direction);

            if (distance < minDistance) {
                minDistance = distance;
                closestChildIndex = i;
            }
        }
    }

    if (closestChildIndex >= 0) {
        SetSelection(closestChildIndex + mFirstPosition);
    } else {
        RequestLayout();
    }
}

Boolean GridView::IsCandidateSelection(
    /* [in] */ Int32 childIndex,
    /* [in] */ Int32 direction)
{
    Int32 count = GetChildCount();
    Int32 invertedIndex = count - 1 - childIndex;

    Int32 rowStart;
    Int32 rowEnd;

    if (!mStackFromBottom) {
        rowStart = childIndex - (childIndex % mNumColumns);
        rowEnd = Elastos::Core::Math::Max(rowStart + mNumColumns - 1, count);
    } else {
        rowEnd = count - 1 - (invertedIndex - (invertedIndex % mNumColumns));
        rowStart = Elastos::Core::Math::Max(0, rowEnd - mNumColumns + 1);
    }

    switch (direction) {
        case IView::FOCUS_RIGHT:
            return childIndex == rowStart;
        case IView::FOCUS_DOWN:
            return rowStart == 0;
        case IView::FOCUS_LEFT:
            return childIndex == rowEnd;
        case IView::FOCUS_UP:
            return rowEnd == count - 1;
        default:
            break;
            /*throw new IllegalArgumentException("direction must be one of "
                    + "{FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");*/
    }
    return FALSE;
}

ECode GridView::SetGravity(
    /* [in] */ Int32 gravity)
{
    if (mGravity != gravity) {
        mGravity = gravity;
        RequestLayoutIfNecessary();
    }

    return NOERROR;
}

ECode GridView::SetHorizontalSpacing(
    /* [in] */ Int32 horizontalSpacing)
{
    if (horizontalSpacing != mRequestedHorizontalSpacing) {
        mRequestedHorizontalSpacing = horizontalSpacing;
        RequestLayoutIfNecessary();
    }

    return NOERROR;
}

ECode GridView::SetVerticalSpacing(
    /* [in] */ Int32 verticalSpacing)
{
    if (verticalSpacing != mVerticalSpacing) {
        mVerticalSpacing = verticalSpacing;
        RequestLayoutIfNecessary();
    }

    return NOERROR;
}

ECode GridView::SetStretchMode(
    /* [in] */ Int32 stretchMode)
{
    if (stretchMode != mStretchMode) {
        mStretchMode = stretchMode;
        RequestLayoutIfNecessary();
    }

    return NOERROR;
}

Int32 GridView::GetStretchMode()
{
    return mStretchMode;
}


ECode GridView::SetColumnWidth(
    /* [in] */ Int32 columnWidth)
{
    if (columnWidth != mRequestedColumnWidth) {
        mRequestedColumnWidth = columnWidth;
        RequestLayoutIfNecessary();
    }

    return NOERROR;
}

ECode GridView::SetNumColumns(Int32 numColumns)
{
    if (numColumns != mRequestedNumColumns) {
        mRequestedNumColumns = numColumns;
        RequestLayoutIfNecessary();
    }

    return NOERROR;
}

ECode GridView::AdjustViewsUpOrDown()
{
    Int32 childCount = GetChildCount();

    if (childCount > 0) {
        Int32 delta;
        AutoPtr<IView> child;

        if (!mStackFromBottom) {
            // Uh-oh -- we came up short. Slide all views up to make them
            // align with the top
            child = GetChildAt(0);
            Int32 top = 0;
            child->GetTop(&top);
            delta = top - mListPadding->mTop;
            if (mFirstPosition != 0) {
                // It's OK to have some space above the first item if it is
                // part of the vertical spacing
                delta -= mVerticalSpacing;
            }
            if (delta < 0) {
                // We only are looking to see if we are too low, not too high
                delta = 0;
            }
        } else {
            // we are too high, slide all views down to align with bottom
            child = GetChildAt(childCount - 1);
            Int32 bottom = 0;
            child->GetBottom(&bottom);
            delta = bottom - (GetHeight() - mListPadding->mBottom);

            if (mFirstPosition + childCount < mItemCount) {
                // It's OK to have some space below the last item if it is
                // part of the vertical spacing
                delta += mVerticalSpacing;
            }

            if (delta > 0) {
                // We only are looking to see if we are too high, not too low
                delta = 0;
            }
        }

        if (delta != 0) {
            OffsetChildrenTopAndBottom(-delta);
        }
    }

    return NOERROR;
}

Int32 GridView::ComputeVerticalScrollExtent()
{
    Int32 count = GetChildCount();
    if (count > 0) {
        Int32 numColumns = mNumColumns;
        Int32 rowCount = (count + numColumns - 1) / numColumns;

        Int32 extent = rowCount * 100;

        AutoPtr<IView> view = GetChildAt(0);
        Int32 top;
        view->GetTop(&top);
        Int32 height;
        view->GetHeight(&height);
        if (height > 0) {
            extent += (top * 100) / height;
        }

        view = GetChildAt(count - 1);
        Int32 bottom;
        view->GetBottom(&bottom);
        view->GetHeight(&height);
        if (height > 0) {
            extent -= ((bottom - GetHeight()) * 100) / height;
        }

        return extent;
    }
    return 0;
}

Int32 GridView::ComputeVerticalScrollOffset()
{
    if (mFirstPosition >= 0 && GetChildCount() > 0) {
        AutoPtr<IView> view = GetChildAt(0);
        Int32 top;
        view->GetTop(&top);
        Int32 height;
        view->GetHeight(&height);
        if (height > 0) {
            Int32 numColumns = mNumColumns;
            Int32 whichRow = mFirstPosition / numColumns;
            Int32 rowCount = (mItemCount + numColumns - 1) / numColumns;
            return Elastos::Core::Math::Max(whichRow * 100 - (top * 100) / height +
                    (Int32) ((Float) mScrollY / GetHeight() * rowCount * 100), 0);
        }
    }
    return 0;
}

Int32 GridView::ComputeVerticalScrollRange()
{
    // TODO: Account for vertical spacing too
    Int32 numColumns = mNumColumns;
    Int32 rowCount = (mItemCount + numColumns - 1) / numColumns;
    Int32 result = Elastos::Core::Math::Max(rowCount * 100, 0);
    if (mScrollY != 0) {
        // Compensate for overscroll
        result += Elastos::Core::Math::Abs((Int32) ((Float) mScrollY / GetHeight() * rowCount * 100));
    }
    return result;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos

