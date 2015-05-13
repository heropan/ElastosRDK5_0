
#include "widget/TableRow.h"
#include "widget/CTableRowLayoutParams.h"
#include "view/CGravity.h"
#include <elastos/Math.h>

using Elastos::Utility::HashMap;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::CGravity;
using Elastos::Droid::View::EIID_IViewGroupOnHierarchyChangeListener;
using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Widget {

//==============================================================================
//              TableRow::ChildrenTracker
//==============================================================================

CAR_INTERFACE_IMPL(TableRow::ChildrenTracker, IViewGroupOnHierarchyChangeListener)

TableRow::ChildrenTracker::ChildrenTracker(
    /* [in] */ TableRow* owner)
    : mOwner(owner)
{
}

ECode TableRow::ChildrenTracker::SetOnHierarchyChangeListener(
    /* [in] */ IViewGroupOnHierarchyChangeListener* listener)
{
    mListener = listener;
    return NOERROR;
}

ECode TableRow::ChildrenTracker::OnChildViewAdded(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    // dirties the index to column map
    mOwner->mColumnToChildIndex = NULL;

    if (mListener != NULL) {
        mListener->OnChildViewAdded(parent, child);
    }

    return NOERROR;
}

ECode TableRow::ChildrenTracker::OnChildViewRemoved(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    // dirties the index to column map
    mOwner->mColumnToChildIndex = NULL;

    if (mListener != NULL) {
        mListener->OnChildViewRemoved(parent, child);
    }

    return NOERROR;
}

//==============================================================================
//              TableRow
//==============================================================================

// {D6AA0095-3075-4C69-814B-1FE58D273B07}
extern "C" const InterfaceID EIID_TableRow =
{ 0xd6aa0095, 0x3075, 0x4c69, { 0x81, 0x4b, 0x1f, 0xe5, 0x8d, 0x27, 0x3b, 0x7 } };


TableRow::TableRow()
    : mNumColumns(0)
{
}

/**
 * <p>Creates a new TableRow for the given context.</p>
 *
 * @param context the application environment
 */
TableRow::TableRow(
    /* [in] */ IContext* context)
    : LinearLayout(context)
    , mNumColumns(0)
{
    InitTableRow();
}

TableRow::~TableRow()
{
    mColumnToChildIndex = NULL;
}

/**
 * <p>Creates a new TableRow for the given context and with the
 * specified set attributes.</p>
 *
 * @param context the application environment
 * @param attrs a collection of attributes
 */
TableRow::TableRow(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : LinearLayout(context, attrs)
    , mNumColumns(0)
{
    InitTableRow();
}

void TableRow::InitTableRow()
{
    AutoPtr<IViewGroupOnHierarchyChangeListener> oldListener = mOnHierarchyChangeListener;
    mChildrenTracker = new ChildrenTracker(this);
    if (oldListener != NULL) {
        mChildrenTracker->SetOnHierarchyChangeListener(oldListener);
    }
    LinearLayout::SetOnHierarchyChangeListener(mChildrenTracker);
}

/**
 * {@inheritDoc}
 */
ECode TableRow::SetOnHierarchyChangeListener(
    /* [in] */ IViewGroupOnHierarchyChangeListener* listener)
{
    mChildrenTracker->SetOnHierarchyChangeListener(listener);

    return NOERROR;
}

/**
 * <p>Collapses or restores a given column.</p>
 *
 * @param columnIndex the index of the column
 * @param collapsed true if the column must be collapsed, false otherwise
 * {@hide}
 */
ECode TableRow::SetColumnCollapsed(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Boolean collapsed)
{
    AutoPtr<IView> child = GetVirtualChildAt(columnIndex);
    if (child != NULL) {
        child->SetVisibility(collapsed ? IView::GONE : IView::VISIBLE);
    }

    return NOERROR;
}

/**
 * {@inheritDoc}
 */
void TableRow::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    // enforce horizontal layout
    MeasureHorizontal(widthMeasureSpec, heightMeasureSpec);
}

/**
 * {@inheritDoc}
 */
void TableRow::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    // enforce horizontal layout
    LayoutHorizontal();
}

/**
 * {@inheritDoc}
 */
AutoPtr<IView> TableRow::GetVirtualChildAt(
    /* [in] */ Int32 i)
{
    if (mColumnToChildIndex == NULL) {
        MapIndexAndColumns();
    }

    Int32 deflectedIndex = -1;
    HashMap<Int32, Int32>::Iterator it = mColumnToChildIndex->Find(i);
    if (it != mColumnToChildIndex->End()) {
        deflectedIndex = it->mSecond;
    }
    if (deflectedIndex != -1) {
        return GetChildAt(deflectedIndex);
    }

    return NULL;
}

/**
 * {@inheritDoc}
 */
Int32 TableRow::GetVirtualChildCount()
{
    if (mColumnToChildIndex == NULL) {
        MapIndexAndColumns();
    }
    return mNumColumns;
}

void TableRow::MapIndexAndColumns()
{
    if (mColumnToChildIndex == NULL) {
        Int32 virtualCount = 0;
        Int32 count = GetChildCount();

        mColumnToChildIndex = new HashMap<Int32, Int32>(5);

        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> child = GetChildAt(i);
            AutoPtr<ITableRowLayoutParams> layoutParams;
            child->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
            Int32 column, span;
            layoutParams->GetColumn(&column);
            layoutParams->GetSpan(&span);

            if (column >= virtualCount) {
                virtualCount = column;
            }

            for (Int32 j = 0; j < span; j++) {
                (*mColumnToChildIndex)[virtualCount++] = i;
            }
        }

        mNumColumns = virtualCount;
    }
}

/**
 * {@inheritDoc}
 */
Int32 TableRow::MeasureNullChild(
    /* [in] */ Int32 childIndex)
{
    return (*mConstrainedColumnWidths)[childIndex];
}

/**
 * {@inheritDoc}
 */
void TableRow::MeasureChildBeforeLayout(
    /* [in] */ IView* child,
    /* [in] */ Int32 childIndex,
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 totalWidth,
    /* [in] */ Int32 heightMeasureSpec,
    /* [in] */ Int32 totalHeight)
{
    if (mConstrainedColumnWidths != NULL) {
        AutoPtr<ITableRowLayoutParams> _lp;
        child->GetLayoutParams((IViewGroupLayoutParams**)&_lp);
        TableRowLayoutParams* lp = reinterpret_cast<TableRowLayoutParams*>(_lp->Probe(EIID_TableRowLayoutParams));
        assert(lp != 0);
        Int32 measureMode = MeasureSpec::EXACTLY;
        Int32 columnWidth = 0;

        Int32 span = lp->mSpan;
        ArrayOf<Int32>* constrainedColumnWidths = mConstrainedColumnWidths;
        for (Int32 i = 0; i < span; i++) {
            columnWidth += (*constrainedColumnWidths)[childIndex + i];
        }

        Int32 gravity = lp->mGravity;
        Boolean isHorizontalGravity;
        AutoPtr<IGravity> g;
        CGravity::AcquireSingleton((IGravity**)&g);
        g->IsHorizontal(gravity, &isHorizontalGravity);

        if (isHorizontalGravity) {
            measureMode = MeasureSpec::AT_MOST;
        }

        // no need to care about padding here,
        // ViewGroup.getChildMeasureSpec() would get rid of it anyway
        // because of the EXACTLY measure spec we use
        Int32 childWidthMeasureSpec = MeasureSpec::MakeMeasureSpec(
            Elastos::Core::Math::Max(0, columnWidth - lp->mLeftMargin - lp->mRightMargin), measureMode);
        Int32 childHeightMeasureSpec = GetChildMeasureSpec(heightMeasureSpec,
                mPaddingTop + mPaddingBottom + lp->mTopMargin +
                lp->mBottomMargin + totalHeight, lp->mHeight);

        child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);

        if (isHorizontalGravity) {
            Int32 childWidth;
            child->GetMeasuredWidth(&childWidth);
            lp->mOffset[TableRowLayoutParams::LOCATION_NEXT] = columnWidth - childWidth;

            Int32 layoutDirection = GetLayoutDirection();
            Int32 absoluteGravity;
            g->GetAbsoluteGravity(gravity, layoutDirection, &absoluteGravity);
            switch (absoluteGravity & IGravity::HORIZONTAL_GRAVITY_MASK) {
                case IGravity::LEFT:
                    // don't offset on X axis
                    break;
                case IGravity::RIGHT:
                    lp->mOffset[TableRowLayoutParams::LOCATION] = lp->mOffset[TableRowLayoutParams::LOCATION_NEXT];
                    break;
                case IGravity::CENTER_HORIZONTAL:
                    lp->mOffset[TableRowLayoutParams::LOCATION] = lp->mOffset[TableRowLayoutParams::LOCATION_NEXT] / 2;
                    break;
            }
        }
        else {
            lp->mOffset[TableRowLayoutParams::LOCATION] = lp->mOffset[TableRowLayoutParams::LOCATION_NEXT] = 0;
        }
    }
    else {
        // fail silently when column widths are not available
        LinearLayout::MeasureChildBeforeLayout(child, childIndex, widthMeasureSpec,
                totalWidth, heightMeasureSpec, totalHeight);
    }
}

/**
 * {@inheritDoc}
 */
Int32 TableRow::GetChildrenSkipCount(
    /* [in] */ IView* child,
    /* [in] */ Int32 index)
{
    AutoPtr<ITableRowLayoutParams> layoutParams;
    child->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
    Int32 span;
    layoutParams->GetSpan(&span);

    // when the span is 1 (default), we need to skip 0 child
    return span - 1;
}

/**
 * {@inheritDoc}
 */
Int32 TableRow::GetLocationOffset(
    /* [in] */ IView* child)
{
    AutoPtr<ITableRowLayoutParams> layoutParams;
    child->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);

    return ((CTableRowLayoutParams*)layoutParams.Get())->mOffset[TableRowLayoutParams::LOCATION];
}

/**
 * {@inheritDoc}
 */
Int32 TableRow::GetNextLocationOffset(
    /* [in] */ IView* child)
{
    AutoPtr<ITableRowLayoutParams> layoutParams;
    child->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);

    return ((CTableRowLayoutParams*)layoutParams.Get())->mOffset[TableRowLayoutParams::LOCATION_NEXT];
}

/**
 * <p>Measures the preferred width of each child, including its margins.</p>
 *
 * @param widthMeasureSpec the width constraint imposed by our parent
 *
 * @return an array of integers corresponding to the width of each cell, or
 *         column, in this row
 * {@hide}
 */
AutoPtr<ArrayOf<Int32> > TableRow::GetColumnsWidths(
    /* [in] */ Int32 widthMeasureSpec)
{
    Int32 numColumns = GetVirtualChildCount();
    if (mColumnWidths == NULL || numColumns != mColumnWidths->GetLength()) {
        mColumnWidths = ArrayOf<Int32>::Alloc(numColumns);
    }

    AutoPtr<ArrayOf<Int32> > columnWidths = mColumnWidths;

    for (Int32 i = 0; i < numColumns; i++) {
        AutoPtr<IView> child = GetVirtualChildAt(i);
        Int32 visible;
        if (child != NULL && (child->GetVisibility(&visible), visible != IView::GONE)) {
            AutoPtr<ITableRowLayoutParams> layoutParams;
            child->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
            CTableRowLayoutParams* lp = (CTableRowLayoutParams*)layoutParams.Get();

            if (lp->mSpan == 1) {
                Int32 spec;
                switch (lp->mWidth) {
                    case IViewGroupLayoutParams::WRAP_CONTENT:
                        spec = GetChildMeasureSpec(widthMeasureSpec, 0, IViewGroupLayoutParams::WRAP_CONTENT);
                        break;
                    case IViewGroupLayoutParams::MATCH_PARENT:
                        spec = MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED);
                        break;
                    default:
                        spec = MeasureSpec::MakeMeasureSpec(lp->mWidth, MeasureSpec::EXACTLY);
                }
                child->Measure(spec, spec);

                Int32 width;
                child->GetMeasuredWidth(&width);

                (*columnWidths)[i] = width + lp->mLeftMargin + lp->mRightMargin;
            }
            else {
                (*columnWidths)[i] = 0;
            }
        } else {
            (*columnWidths)[i] = 0;
        }
    }

    return columnWidths;
}

/**
 * <p>Sets the width of all of the columns in this row. At layout time,
 * this row sets a fixed width, as defined by <code>columnWidths</code>,
 * on each child (or cell, or column.)</p>
 *
 * @param columnWidths the fixed width of each column that this row must
 *                     honor
 * @throws IllegalArgumentException when columnWidths' length is smaller
 *         than the number of children in this row
 * {@hide}
 */
ECode TableRow::SetColumnsWidthConstraints(
    /* [in] */ ArrayOf<Int32>* columnWidths)
{
    if (columnWidths == NULL || columnWidths->GetLength() < GetVirtualChildCount()) {
//        throw new IllegalArgumentException(
//                "columnWidths should be >= getVirtualChildCount()");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mConstrainedColumnWidths = columnWidths;

    return NOERROR;
}

/**
 * {@inheritDoc}
 */
ECode TableRow::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<ITableRowLayoutParams> lp;
    FAIL_RETURN(CTableRowLayoutParams::New(GetContext(), attrs, (ITableRowLayoutParams**)&lp));
    *params = IViewGroupLayoutParams::Probe(lp);
    INTERFACE_ADDREF(*params);
    return NOERROR;
}

/**
 * Returns a set of layout parameters with a width of
 * {@link android.view.ViewGroup.LayoutParams#MATCH_PARENT},
 * a height of {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT} and no spanning.
 */
AutoPtr<ILinearLayoutLayoutParams> TableRow::GenerateDefaultLayoutParams()
{
    AutoPtr<ITableRowLayoutParams> lp;
    CTableRowLayoutParams::New((ITableRowLayoutParams**)&lp);
    return (ILinearLayoutLayoutParams*)lp.Get();
}

/**
 * {@inheritDoc}
 */
Boolean TableRow::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return ITableRowLayoutParams::Probe(p) != NULL;
}

/**
 * {@inheritDoc}
 */
AutoPtr<IViewGroupLayoutParams> TableRow::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    CTableRowLayoutParams::New((ITableRowLayoutParams**)&lp);
    return lp;
}

ECode TableRow::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    LinearLayout::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CTableRow"), (ICharSequence**)&seq);
    return event->SetClassName(seq);
}


ECode TableRow::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    LinearLayout::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CTableRow"), (ICharSequence**)&seq);
    return info->SetClassName(seq);
}

ECode TableRow::Init(
        /* [in] */ IContext* context)
{
    FAIL_RETURN(LinearLayout::Init(context));
    InitTableRow();
    return NOERROR;
}

/**
 * <p>Creates a new TableRow for the given context and with the
 * specified set attributes.</p>
 *
 * @param context the application environment
 * @param attrs a collection of attributes
 */
ECode TableRow::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(LinearLayout::Init(context, attrs));
    InitTableRow();
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
