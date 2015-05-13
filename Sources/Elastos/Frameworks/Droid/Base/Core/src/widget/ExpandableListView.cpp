
#include "widget/ExpandableListView.h"
#include "widget/ExpandableListPosition.h"
#include "widget/CExpandableListPosition.h"
#include "widget/CExpandableListConnector.h"
#include "widget/PositionMetadata.h"
#include "widget/GroupMetadata.h"
/*#include "widget/ExpandableListViewSavedState.h"
#include "widget/CExpandableListViewSavedState.h"*/
#include "view/SoundEffectConstants.h"
//#include <elastos/Logger.h>

//using Elastos::Utility::Logging::Logger;
using Elastos::Droid::View::SoundEffectConstants;
using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Widget {

const Int32 ExpandableListView::PACKED_POSITION_TYPE_GROUP;
const Int32 ExpandableListView::PACKED_POSITION_TYPE_CHILD;
const Int32 ExpandableListView::PACKED_POSITION_TYPE_NULL;
const Int64 ExpandableListView::PACKED_POSITION_VALUE_NULL;
const Int32 ExpandableListView::CHILD_INDICATOR_INHERIT;

const Int64 ExpandableListView::PACKED_POSITION_MASK_CHILD;
const Int64 ExpandableListView::PACKED_POSITION_MASK_GROUP;
const Int64 ExpandableListView::PACKED_POSITION_MASK_TYPE;
const Int64 ExpandableListView::PACKED_POSITION_SHIFT_GROUP;
const Int64 ExpandableListView::PACKED_POSITION_SHIFT_TYPE;
const Int64 ExpandableListView::PACKED_POSITION_INT_MASK_CHILD;
const Int64 ExpandableListView::PACKED_POSITION_INT_MASK_GROUP;


Int32 ExpandableListView::EMPTY_STATE_SET[] = {};

Int32 ExpandableListView::GROUP_EXPANDED_STATE_SET[] =
        {R::attr::state_expanded};

Int32 ExpandableListView::GROUP_EMPTY_STATE_SET[] =
        {R::attr::state_empty};

Int32 ExpandableListView::GROUP_EXPANDED_EMPTY_STATE_SET[] =
        {R::attr::state_expanded, R::attr::state_empty};

Int32* ExpandableListView::GROUP_STATE_SETS[]= {
     EMPTY_STATE_SET, // 00
     GROUP_EXPANDED_STATE_SET, // 01
     GROUP_EMPTY_STATE_SET, // 10
     GROUP_EXPANDED_EMPTY_STATE_SET // 11
};

Int32 ExpandableListView::CHILD_LAST_STATE_SET[] =
        {R::attr::state_last};


CAR_INTERFACE_IMPL(ExpandableListView::ExpandableListContextMenuInfo, IContextMenuInfo)

ExpandableListView::ExpandableListView() :
    mIndicatorLeft(0),
    mIndicatorRight(0),
    mChildIndicatorLeft(0),
    mChildIndicatorRight(0)
{
}

ExpandableListView::ExpandableListView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle) :
    ListView(context, attrs, defStyle),
    mIndicatorLeft(0),
    mIndicatorRight(0),
    mChildIndicatorLeft(0),
    mChildIndicatorRight(0)
{
    Init(context, attrs, defStyle);
}

ECode ExpandableListView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::ExpandableListView),
            ARRAY_SIZE(R::styleable::ExpandableListView));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(
            attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

    a->GetDrawable(R::styleable::ExpandableListView_groupIndicator, (IDrawable**)&mGroupIndicator);
    a->GetDrawable(R::styleable::ExpandableListView_childIndicator, (IDrawable**)&mChildIndicator);
    a->GetDimensionPixelSize(R::styleable::ExpandableListView_indicatorLeft, 0, &mIndicatorLeft);
    a->GetDimensionPixelSize(R::styleable::ExpandableListView_indicatorRight, 0, &mIndicatorRight);
    if (mIndicatorRight == 0 && mGroupIndicator != NULL) {
        Int32 width = 0;
        mGroupIndicator->GetIntrinsicWidth(&width);
        mIndicatorRight = mIndicatorLeft + width;
    }

    a->GetDimensionPixelSize(R::styleable::ExpandableListView_childIndicatorLeft, CHILD_INDICATOR_INHERIT, &mChildIndicatorLeft);
    a->GetDimensionPixelSize(
            R::styleable::ExpandableListView_childIndicatorRight, CHILD_INDICATOR_INHERIT, &mChildIndicatorRight);
    a->GetDrawable(R::styleable::ExpandableListView_childDivider, (IDrawable**)&mChildDivider);

    a->Recycle();
    return NOERROR;
}

void ExpandableListView::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    ListView::DispatchDraw(canvas);

    if ((mChildIndicator == NULL) && (mGroupIndicator == NULL)) {
        return;
    }

    Int32 saveCount = 0;
    Boolean clipToPadding = (mGroupFlags & ViewGroup::CLIP_TO_PADDING_MASK) == ViewGroup::CLIP_TO_PADDING_MASK;
    if (clipToPadding) {
        canvas->Save(&saveCount);
        Int32 scrollX = mScrollX;
        Int32 scrollY = mScrollY;
        Boolean isNotEmpty = FALSE;
        canvas->ClipRectEx6(scrollX + mPaddingLeft, scrollY + mPaddingTop,
                scrollX + mRight - mLeft - mPaddingRight,
                scrollY + mBottom - mTop - mPaddingBottom, &isNotEmpty);
    }

    Int32 headerViewsCount = GetHeaderViewsCount();
    Int32 lastChildFlPos = mItemCount - GetFooterViewsCount() - headerViewsCount - 1;
    Int32 myB = mBottom;


    AutoPtr<IView> item;
    AutoPtr<IDrawable> indicator;
    Int32 t = 0;
    Int32 b = 0;

    Int32 lastItemType = ~(ExpandableListPosition::CHILD | ExpandableListPosition::GROUP);
    AutoPtr<IRect> indicatorRect = mIndicatorRect;

    Int32 childCount = GetChildCount();
    for (Int32 i = 0, childFlPos = mFirstPosition - headerViewsCount; i < childCount;
         i++, childFlPos++) {

        if (childFlPos < 0) {
            continue;
        } else if (childFlPos > lastChildFlPos) {
            break;
        }

        item = GetChildAt(i);
        item->GetTop(&t);
        item->GetBottom(&b);

        if ((b < 0) || (t > myB)) continue;

        AutoPtr<IPositionMetadata> pos;
        mConnector->GetUnflattenedPos(childFlPos, (IPositionMetadata**)&pos);
        CExpandableListPosition* elpos = (CExpandableListPosition*)(((PositionMetadata*)pos.Get())->mPosition.Get());
        if (elpos->type != lastItemType) {
            if (elpos->type == ExpandableListPosition::CHILD) {
                indicatorRect->SetLeft((mChildIndicatorLeft == CHILD_INDICATOR_INHERIT) ?
                        mIndicatorLeft : mChildIndicatorLeft);
                indicatorRect->SetRight((mChildIndicatorRight == CHILD_INDICATOR_INHERIT) ?
                        mIndicatorRight : mChildIndicatorRight);
            } else {
                indicatorRect->SetLeft(mIndicatorLeft);
                indicatorRect->SetRight(mIndicatorRight);
            }

            ((CRect*)indicatorRect.Get())->mLeft += mPaddingLeft;
            ((CRect*)indicatorRect.Get())->mRight += mPaddingLeft;

            lastItemType = elpos->type;
        }

        if (((CRect*)indicatorRect.Get())->mLeft != ((CRect*)indicatorRect.Get())->mRight) {
            if (mStackFromBottom) {
                ((CRect*)indicatorRect.Get())->mTop = t;
                ((CRect*)indicatorRect.Get())->mBottom = b;
            } else {
                ((CRect*)indicatorRect.Get())->mTop = t;
                ((CRect*)indicatorRect.Get())->mBottom = b;
            }

            indicator = GetIndicator(pos);
            if (indicator != NULL) {
                indicator->SetBoundsEx(indicatorRect);
                indicator->Draw(canvas);
            }
        }

        pos->Recycle();
    }

    if (clipToPadding) {
        canvas->RestoreToCount(saveCount);
    }
}

AutoPtr<IDrawable> ExpandableListView::GetIndicator(
    /* [in] */ IPositionMetadata* pos)
{
    AutoPtr<IDrawable> indicator = NULL;
    CExpandableListPosition* elpos = (CExpandableListPosition*)(((PositionMetadata*)pos)->mPosition.Get());
    if (elpos->type == ExpandableListPosition::GROUP) {
        indicator = mGroupIndicator;

        Boolean stateful = FALSE;
        if (indicator.Get() != NULL && (indicator->IsStateful(&stateful), stateful)) {
            // Empty check based on availability of data.  If the groupMetadata isn't NULL,
            // we do a check on it. Otherwise, the group is collapsed so we consider it
            // empty for performance reasons.
            Boolean isEmpty = (((PositionMetadata*)pos)->mGroupMetadata == NULL) ||
                    (((GroupMetadata*)((PositionMetadata*)pos)->mGroupMetadata.Get())->mLastChildFlPos == ((GroupMetadata*)((PositionMetadata*)pos)->mGroupMetadata.Get())->mFlPos);

            Boolean expanded;
            pos->IsExpanded(&expanded);
            Int32 stateSetIndex =
                (expanded ? 1 : 0) | // Expanded?
                (isEmpty ? 2 : 0); // Empty?

            Boolean res;
            //indicator->SetState(ArrayOf<Int32>(GROUP_STATE_SETS[stateSetIndex]), &res);
        }
    } else {
        indicator = mChildIndicator;

        Boolean stateful = FALSE;
        if (indicator != NULL && (indicator->IsStateful(&stateful), stateful)) {
            // No need for a state sets array for the child since it only has two states
            Int32* stateSet = elpos->flatListPos == ((GroupMetadata*)(((PositionMetadata*)pos)->mGroupMetadata.Get()))->mLastChildFlPos
                    ? CHILD_LAST_STATE_SET
                    : EMPTY_STATE_SET;

            Boolean res;
            //indicator->SetState(ArrayOf<Int32>(stateSet, ), &res);
        }
    }

    return indicator;
}

ECode ExpandableListView::SetChildDivider(
    /* [in] */ IDrawable* childDivider)
{
    mChildDivider = childDivider;

    return NOERROR;
}

void ExpandableListView::DrawDivider(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* bounds,
    /* [in] */ Int32 childIndex)
{
    Int32 flatListPosition = childIndex + mFirstPosition;

    // Only proceed as possible child if the divider isn't above all items (if it is above
    // all items, then the item below it has to be a group)
    if (flatListPosition >= 0) {
        Int32 adjustedPosition = GetFlatPositionForConnector(flatListPosition);
        AutoPtr<IPositionMetadata> pos;
        mConnector->GetUnflattenedPos(adjustedPosition, (IPositionMetadata**)&pos);
        // If this item is a child, or it is a non-empty group that is expanded

        Boolean expanded;
        pos->IsExpanded(&expanded);
        CExpandableListPosition* elpos = (CExpandableListPosition*)(((PositionMetadata*)pos.Get())->mPosition.Get());
        if ((elpos->type == ExpandableListPosition::CHILD) || (expanded &&
                ((GroupMetadata*)((PositionMetadata*)pos.Get())->mGroupMetadata.Get())->mLastChildFlPos != ((GroupMetadata*)((PositionMetadata*)pos.Get())->mGroupMetadata.Get())->mFlPos)) {
            // These are the cases where we draw the child divider
            AutoPtr<IDrawable> divider = mChildDivider;
            divider->SetBoundsEx(bounds);
            divider->Draw(canvas);
            pos->Recycle();
            return;
        }
        pos->Recycle();
    }

    // Otherwise draw the default divider
    ListView::DrawDivider(canvas, bounds, flatListPosition);
}

ECode ExpandableListView::SetAdapter(
    /* [in] */ IAdapter* adapter)
{
    /*throw new RuntimeException(
            "For ExpandableListView, use setAdapter(ExpandableListAdapter) instead of " +
            "setAdapter(ListAdapter)");*/
    return E_RUNTIME_EXCEPTION;
}

AutoPtr<IAdapter> ExpandableListView::GetAdapter()
{
    return ListView::GetAdapter();
}

ECode ExpandableListView::SetOnItemClickListener(
    /* [in] */ IAdapterViewOnItemClickListener* l)
{
    ListView::SetOnItemClickListener(l);
    return NOERROR;
}

ECode ExpandableListView::SetAdapter(
    /* [in] */ IExpandableListAdapter* adapter)
{
    mAdapter = adapter;
    mConnector = NULL;
    if (adapter != NULL) {
        CExpandableListConnector::New((IExpandableListConnector**)&mConnector);
    }

    ListView::SetAdapter((IListAdapter*)mConnector->Probe(EIID_IListAdapter));
    return NOERROR;
}

AutoPtr<IExpandableListAdapter> ExpandableListView::GetExpandableListAdapter()
{
    return mAdapter;
}

Boolean ExpandableListView::IsHeaderOrFooterPosition(
    /* [in] */ Int32 position)
{
    Int32 footerViewsStart = mItemCount - GetFooterViewsCount();
    return (position < GetHeaderViewsCount() || position >= footerViewsStart);
}

Int32 ExpandableListView::GetFlatPositionForConnector(
    /* [in] */ Int32 flatListPosition)
{
    return flatListPosition - GetHeaderViewsCount();
}

Int32 ExpandableListView::GetAbsoluteFlatPosition(
    /* [in] */ Int32 flatListPosition)
{
    return flatListPosition + GetHeaderViewsCount();
}

Boolean ExpandableListView::PerformItemClick(
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    if (IsHeaderOrFooterPosition(position)) {
        return ListView::PerformItemClick(v, position, id);
    }

    Int32 adjustedPosition = GetFlatPositionForConnector(position);
    return HandleItemClick(v, adjustedPosition, id);
}

Boolean ExpandableListView::HandleItemClick(
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    AutoPtr<IPositionMetadata> posMetadata;
    mConnector->GetUnflattenedPos(position, (IPositionMetadata**)&posMetadata);

    id = GetChildOrGroupId(((PositionMetadata*)posMetadata.Get())->mPosition);

    Boolean returnValue = FALSE;
    if (((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->type == ExpandableListPosition::GROUP) {

        if (mOnGroupClickListener.Get() != NULL) {
            Boolean res = FALSE;
            mOnGroupClickListener->OnGroupClick((IExpandableListView*)this->Probe(EIID_IExpandableListView), v,
                ((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos, id, &res);
            if (res) {
                posMetadata->Recycle();
                return TRUE;
            }
        }

        Boolean expanded = FALSE;
        posMetadata->IsExpanded(&expanded);
        if (expanded) {
            Boolean res = FALSE;
            ((CExpandableListConnector*)mConnector.Get())->CollapseGroupEx(posMetadata, &res);

            PlaySoundEffect(SoundEffectConstants::CLICK);

            if (mOnGroupCollapseListener.Get() != NULL) {
                mOnGroupCollapseListener->OnGroupCollapse(((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos);
            }
        } else {
            Boolean rush = FALSE;
            ((CExpandableListConnector*)mConnector.Get())->CollapseGroupEx(posMetadata, &rush);

            PlaySoundEffect(SoundEffectConstants::CLICK);

            if (mOnGroupExpandListener.Get() != NULL) {
                mOnGroupExpandListener->OnGroupExpand(((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos);
            }

            Int32 groupPos = ((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos;
            Int32 groupFlatPos = ((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->flatListPos;

            Int32 shiftedGroupPosition = groupFlatPos + GetHeaderViewsCount();

            Int32 count;
            mAdapter->GetChildrenCount(groupPos, &count);
            SmoothScrollToPosition(shiftedGroupPosition + count,
                    shiftedGroupPosition);
        }

        returnValue = TRUE;
    } else {
        if (mOnChildClickListener.Get() != NULL) {
            PlaySoundEffect(SoundEffectConstants::CLICK);
            mOnChildClickListener->OnChildClick((IExpandableListView*)(this->Probe(EIID_IExpandableListView)), v,
                ((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->groupPos,
                    ((CExpandableListPosition*)((PositionMetadata*)posMetadata.Get())->mPosition.Get())->childPos, id, &returnValue);
            return returnValue;
        }

        returnValue = FALSE;
    }

    posMetadata->Recycle();

    return returnValue;
}

Boolean ExpandableListView::ExpandGroup(
    /* [in] */ Int32 groupPos)
{
    return ExpandGroup(groupPos, FALSE);
}

Boolean ExpandableListView::ExpandGroup(
    /* [in] */ Int32 groupPos,
    /* [in] */ Boolean animate)
{
    AutoPtr<IExpandableListPosition> elGroupPos = ExpandableListPosition::Obtain(
        ExpandableListPosition::GROUP, groupPos, -1, -1);
    AutoPtr<IPositionMetadata> pm;
    mConnector->GetFlattenedPos(elGroupPos, (IPositionMetadata**)&pm);
    elGroupPos->Recycle();

    Boolean retValue = FALSE;
    mConnector->ExpandGroupEx(pm, &retValue);

    if(mOnGroupExpandListener != NULL) {
        mOnGroupExpandListener->OnGroupExpand(groupPos);
    }

    if(animate) {
        Int32 groupFlatPos = ((CExpandableListPosition*)((PositionMetadata*)pm.Get())->mPosition.Get())->flatListPos;
        Int32 shiftedGroupPosition = groupFlatPos + GetHeaderViewsCount();
        Int32 childCount = 0;
        mAdapter->GetChildrenCount(groupPos, &childCount);
        SmoothScrollToPosition(shiftedGroupPosition + childCount, shiftedGroupPosition);
    }

    pm->Recycle();
    return NOERROR;
}

Boolean ExpandableListView::CollapseGroup(
    /* [in] */ Int32 groupPos)
{
    Boolean retValue = FALSE;
    Boolean res = FALSE;
    retValue = ((CExpandableListConnector*)mConnector.Get())->CollapseGroup(groupPos, &res);

    if (mOnGroupCollapseListener.Get() != NULL) {
        mOnGroupCollapseListener->OnGroupCollapse(groupPos);
    }

    return retValue;
}

ECode ExpandableListView::SetOnGroupCollapseListener(
    /* [in] */ IOnGroupCollapseListener* onGroupCollapseListener)
{
    mOnGroupCollapseListener = onGroupCollapseListener;
    return NOERROR;
}

ECode ExpandableListView::SetOnGroupExpandListener(
    /* [in] */ IOnGroupExpandListener* onGroupExpandListener)
{
    mOnGroupExpandListener = onGroupExpandListener;
    return NOERROR;
}

ECode ExpandableListView::SetOnGroupClickListener(
    /* [in] */ IOnGroupClickListener* onGroupClickListener)
{
    mOnGroupClickListener = onGroupClickListener;
    return NOERROR;
}

ECode ExpandableListView::SetOnChildClickListener(
    /* [in] */ IOnChildClickListener* onChildClickListener)
{
    mOnChildClickListener = onChildClickListener;
    return NOERROR;
}

Int64 ExpandableListView::GetExpandableListPosition(
    /* [in] */ Int32 flatListPosition)
{
    if (IsHeaderOrFooterPosition(flatListPosition)) {
        return PACKED_POSITION_VALUE_NULL;
    }

    Int32 adjustedPosition = GetFlatPositionForConnector(flatListPosition);
    AutoPtr<IPositionMetadata> pm;
    mConnector->GetUnflattenedPos(adjustedPosition, (IPositionMetadata**)&pm);
    Int64 packedPos = 0;
    ((CExpandableListPosition*)((PositionMetadata*)pm.Get())->mPosition.Get())->GetPackedPosition(&packedPos);
    pm->Recycle();
    return packedPos;
}

Int32 ExpandableListView::GetFlatListPosition(
    /* [in] */ Int64 packedPosition)
{
    AutoPtr<IExpandableListPosition> elPackedPos =
        ExpandableListPosition::ObtainPosition(packedPosition);
    AutoPtr<IPositionMetadata> pm;
    mConnector->GetFlattenedPos(elPackedPos, (IPositionMetadata**)&pm);
    elPackedPos->Recycle();

    Int32 flatListPosition = ((CExpandableListPosition*)((PositionMetadata*)pm.Get())->mPosition.Get())->flatListPos;
    pm->Recycle();
    return GetAbsoluteFlatPosition(flatListPosition);
}

Int64 ExpandableListView::GetSelectedPosition()
{
    Int32 selectedPos = GetSelectedItemPosition();

    return GetExpandableListPosition(selectedPos);
}

Int64 ExpandableListView::GetSelectedId()
{
    Int64 packedPos = GetSelectedPosition();
    if (packedPos == PACKED_POSITION_VALUE_NULL) return -1;

    Int32 groupPos = GetPackedPositionGroup(packedPos);

    Int64 id = 0;
    if (GetPackedPositionType(packedPos) == PACKED_POSITION_TYPE_GROUP) {
        // It's a group
        mAdapter->GetGroupId(groupPos, &id);
        return id;
    } else {
        // It's a child
        mAdapter->GetChildId(groupPos, GetPackedPositionChild(packedPos), &id);
        return id;
    }
}

ECode ExpandableListView::SetSelectedGroup(
    /* [in] */ Int32 groupPosition)
{
    AutoPtr<IExpandableListPosition> elGroupPos =
        ExpandableListPosition::ObtainGroupPosition(groupPosition);
    AutoPtr<IPositionMetadata> pm = NULL;
    //pm = mConnector->GetFlattenedPos(elGroupPos);
    elGroupPos->Recycle();
    Int32 absoluteFlatPosition = GetAbsoluteFlatPosition(((CExpandableListPosition*)((PositionMetadata*)pm.Get())->mPosition.Get())->flatListPos);
    ListView::SetSelection(absoluteFlatPosition);
    pm->Recycle();
    return NOERROR;
}

Boolean ExpandableListView::SetSelectedChild(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition,
    /* [in] */ Boolean shouldExpandGroup)
{
    AutoPtr<IExpandableListPosition> elChildPos = ExpandableListPosition::ObtainChildPosition(
            groupPosition, childPosition);
    AutoPtr<IPositionMetadata> flatChildPos;
    mConnector->GetFlattenedPos(elChildPos, (IPositionMetadata**)&flatChildPos);

    if (flatChildPos.Get() == NULL) {
        if (!shouldExpandGroup) return FALSE;

        ExpandGroup(groupPosition);

        mConnector->GetFlattenedPos(elChildPos, (IPositionMetadata**)&flatChildPos);

        if (flatChildPos.Get() == NULL) {
            //throw new IllegalStateException("Could not find child");
        }
    }

    Int32 absoluteFlatPosition = GetAbsoluteFlatPosition(((CExpandableListPosition*)((PositionMetadata*)flatChildPos.Get())->mPosition.Get())->flatListPos);
    ListView::SetSelection(absoluteFlatPosition);

    elChildPos->Recycle();
    flatChildPos->Recycle();

    return TRUE;
}

Boolean ExpandableListView::IsGroupExpanded(
    /* [in] */ Int32 groupPosition)
{
    Boolean expanded;
    mConnector->IsGroupExpanded(groupPosition, &expanded);
    return expanded;
}

Int32 ExpandableListView::GetPackedPositionType(
    /* [in] */ Int64 packedPosition)
{
    if (packedPosition == PACKED_POSITION_VALUE_NULL) {
        return PACKED_POSITION_TYPE_NULL;
    }

    return (packedPosition & PACKED_POSITION_MASK_TYPE) == PACKED_POSITION_MASK_TYPE
            ? PACKED_POSITION_TYPE_CHILD
            : PACKED_POSITION_TYPE_GROUP;
}

Int32 ExpandableListView::GetPackedPositionGroup(
    /* [in] */ Int64 packedPosition)
{
    if (packedPosition == PACKED_POSITION_VALUE_NULL) return -1;

    return (Int32) ((packedPosition & PACKED_POSITION_MASK_GROUP) >> PACKED_POSITION_SHIFT_GROUP);
}

Int32 ExpandableListView::GetPackedPositionChild(
    /* [in] */ Int64 packedPosition)
{
    if (packedPosition == PACKED_POSITION_VALUE_NULL) return -1;

    if ((packedPosition & PACKED_POSITION_MASK_TYPE) != PACKED_POSITION_MASK_TYPE) return -1;

    return (Int32) (packedPosition & PACKED_POSITION_MASK_CHILD);
}

Int64 ExpandableListView::GetPackedPositionForChild(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition)
{
    return (((Int64)PACKED_POSITION_TYPE_CHILD) << PACKED_POSITION_SHIFT_TYPE)
            | ((((Int64)groupPosition) & PACKED_POSITION_INT_MASK_GROUP)
                    << PACKED_POSITION_SHIFT_GROUP)
            | (childPosition & PACKED_POSITION_INT_MASK_CHILD);
}

Int64 ExpandableListView::GetPackedPositionForGroup(
    /* [in] */ Int32 groupPosition)
{
    return ((((Int64)groupPosition) & PACKED_POSITION_INT_MASK_GROUP)
                    << PACKED_POSITION_SHIFT_GROUP);
}

AutoPtr<IContextMenuInfo> ExpandableListView::CreateContextMenuInfo(
    /* [in] */ IView* view,
    /* [in] */ Int32 flatListPosition,
    /* [in] */ Int64 id)
{
    if (IsHeaderOrFooterPosition(flatListPosition)) {
        return new AdapterContextMenuInfo(view, flatListPosition, id);
    }

    Int32 adjustedPosition = GetFlatPositionForConnector(flatListPosition);
    AutoPtr<IPositionMetadata> pm;
    mConnector->GetUnflattenedPos(adjustedPosition, (IPositionMetadata**)&pm);
    AutoPtr<IExpandableListPosition> pos = ((PositionMetadata*)pm.Get())->mPosition;

    id = GetChildOrGroupId(pos);
    Int64 packedPosition;
    pos->GetPackedPosition(&packedPosition);
    pm->Recycle();

    return new ExpandableListContextMenuInfo(view, packedPosition, id);
}

Int64 ExpandableListView::GetChildOrGroupId(
    /* [in] */ IExpandableListPosition* position)
{
    Int64 id = 0;
    if (((CExpandableListPosition*)position)->type == ExpandableListPosition::CHILD) {
        mAdapter->GetChildId(((CExpandableListPosition*)position)->groupPos, ((CExpandableListPosition*)position)->childPos, &id);
        return id;
    } else {
        mAdapter->GetGroupId(((CExpandableListPosition*)position)->groupPos, &id);
        return id;
    }
}

ECode ExpandableListView::SetChildIndicator(
    /* [in] */ IDrawable* childIndicator)
{
    mChildIndicator = childIndicator;
    return NOERROR;
}

ECode ExpandableListView::SetChildIndicatorBounds(
    /* [in] */ Int32 left,
    /* [in] */ Int32 right)
{
    mChildIndicatorLeft = left;
    mChildIndicatorRight = right;
    return NOERROR;
}

ECode ExpandableListView::SetGroupIndicator(
    /* [in] */ IDrawable* groupIndicator)
{
    mGroupIndicator = groupIndicator;

    if (mIndicatorRight == 0 && mGroupIndicator != NULL) {
        Int32 res = 0;
        mGroupIndicator->GetIntrinsicWidth(&res);
        mIndicatorRight = mIndicatorLeft + res;
    }
    return NOERROR;
}

ECode ExpandableListView::SetIndicatorBounds(
    /* [in] */ Int32 left,
    /* [in] */ Int32 right)
{
    mIndicatorLeft = left;
    mIndicatorRight = right;
    return NOERROR;
}

ExpandableListView::ExpandableListContextMenuInfo::ExpandableListContextMenuInfo(
    /* [in] */ IView* targetView,
    /* [in] */ Int64 packedPosition,
    /* [in] */ Int64 id)
{
    mTargetView = targetView;
    mPackedPosition = packedPosition;
    mId = id;
}


AutoPtr<IParcelable> ExpandableListView::OnSaveInstanceState()
{
    /*AutoPtr<IParcelable> superState = ListView::OnSaveInstanceState();

    AutoPtr<ArrayOf<IGroupMetadata*> > expandedGroupMetadataList;
    if (mConnector.Get() != NULL) {
        expandedGroupMetadataList = ((CExpandableListConnector*)mConnector.Get())->GetExpandedGroupMetadataList();
    }

    AutoPtr<IExpandableListViewSavedState> ss = NULL;
    CExpandableListViewSavedState::New(superState, *expandedGroupMetadataList, (IExpandableListViewSavedState**)&ss);

    return (IParcelable*)ss->Probe(EIID_IParcelable);*/
    return NULL;
}

void ExpandableListView::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    /*if (!(state->Probe(EIID_IExpandableListViewSavedState))) {
        ListView::OnRestoreInstanceState(state);
    }

    ExpandableListViewSavedState* ss = (ExpandableListViewSavedState*) state;
    ListView::OnRestoreInstanceState(ss->GetSuperState());

    if (mConnector.Get() != NULL && ss->expandedGroupMetadataList != NULL) {
        ((CExpandableListConnector*)mConnector.Get())->SetExpandedGroupMetadataList(ss->expandedGroupMetadataList);
    }*/
}


ECode ExpandableListView::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    ListView::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CExpandableListView"), (ICharSequence**)&seq);
    event->SetClassName(seq);
    return NOERROR;
}

ECode ExpandableListView::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    ListView::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CExpandableListView"), (ICharSequence**)&seq);
    info->SetClassName(seq);
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
