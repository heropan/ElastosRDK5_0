
#include "elastos/droid/packages/launcher2/PagedViewCellLayout.h"
#include "Elastos.Droid.Service.h"
#include "R.h"

namespace Elastos {
namespace Droid {
namespace Packages {
namespace Launcher2 {

PagedViewCellLayout::LayoutParams::LayoutParams()
    : MarginLayoutParams(IViewGroupLayoutParams::MATCH_PARENT, IViewGroupLayoutParams::MATCH_PARENT)
    , mCellX(0)
    , mCellY(0)
    , mCellHSpan(1)
    , mCellVSpan(1)
    , mIsDragging(FALSE)
    , mX(0)
    , mY(0)
{
}

PagedViewCellLayout::LayoutParams::LayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : MarginLayoutParams(c, attrs)
    , mCellX(0)
    , mCellY(0)
    , mCellHSpan(1)
    , mCellVSpan(1)
    , mIsDragging(FALSE)
    , mX(0)
    , mY(0)
{
}

PagedViewCellLayout::LayoutParams::LayoutParams(
    /* [in] */ IViewGroupLayoutParams* source)
    : MarginLayoutParams(source)
    , mCellX(0)
    , mCellY(0)
    , mCellHSpan(1)
    , mCellVSpan(1)
    , mIsDragging(FALSE)
    , mX(0)
    , mY(0)
{
}

PagedViewCellLayout::LayoutParams::LayoutParams(
    /* [in] */ LayoutParams* source)
    : MarginLayoutParams(source);
    , mCellX(source->mCellX)
    , mCellY(source->mCellY)
    , mCellHSpan(source->mCellHSpan)
    , mCellVSpan(source->mCellVSpan)
    , mIsDragging(FALSE)
    , mX(0)
    , mY(0)
{
}

PagedViewCellLayout::LayoutParams::LayoutParams(
    /* [in] */ Int32 cellX,
    /* [in] */ Int32 cellY,
    /* [in] */ Int32 cellHSpan,
    /* [in] */ Int32 cellVSpan)
    : MarginLayoutParams(IViewGroupLayoutParams::MATCH_PARENT, IViewGroupLayoutParams::MATCH_PARENT);
    , mCellX(cellX)
    , mCellY(cellY)
    , mCellHSpan(cellHSpan)
    , mCellVSpan(cellVSpan)
    , mIsDragging(FALSE)
    , mX(0)
    , mY(0)
{
}

ECode PagedViewCellLayout::LayoutParams::Setup(
    /* [in] */ Int32 cellWidth,
    /* [in] */ Int32 cellHeight,
    /* [in] */ Int32 widthGap,
    /* [in] */ Int32 heightGap,
    /* [in] */ Int32 hStartPadding,
    /* [in] */ Int32 vStartPadding)
{
    const Int32 myCellHSpan = cellHSpan;
    const Int32 myCellVSpan = cellVSpan;
    const Int32 myCellX = cellX;
    const Int32 myCellY = cellY;

    mWidth = myCellHSpan * cellWidth + ((myCellHSpan - 1) * widthGap) -
            leftMargin - rightMargin;
    mHeight = myCellVSpan * cellHeight + ((myCellVSpan - 1) * heightGap) -
            topMargin - bottomMargin;

    Boolean res;
    LauncherApplication::IsScreenLarge(&res);
    if (res) {
        mX = hStartPadding + myCellX * (cellWidth + widthGap) + leftMargin;
        mY = vStartPadding + myCellY * (cellHeight + heightGap) + topMargin;
    }
    else {
        mX = myCellX * (cellWidth + widthGap) + leftMargin;
        mY = myCellY * (cellHeight + heightGap) + topMargin;
    }
    return NOERROR;
}

ECode PagedViewCellLayout::LayoutParams::GetTag(
    /* [out] */ IInterface** tag)
{
    VALIDATE_NOT_NULL(tag);

    *tag = mTag;
    REFCOUNT_ADD(*tag);
    return NOERROR;
}

ECode PagedViewCellLayout::LayoutParams::SetTag(
    /* [in] */ IInterface* tag)
{
    mTag = tag;
    return NOERROR;
}

ECode PagedViewCellLayout::LayoutParams::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(tag);

    StringBuilder sb;
    sb += "(";
    sb += mCellX;
    sb += ", ";
    sb += mCellY;
    sb += ", ";
    sb += mCellHSpan;
    sb += ", ";
    sb += mCellVSpan;
    sb += ")";
    return sb.ToString(str);
}

const String PagedViewCellLayout::TAG("PagedViewCellLayout");

CAR_INTERFACE_IMPL_2(PagedViewCellLayout, ViewGroup, IPagedViewCellLayout, IPage);

PagedViewCellLayout::PagedViewCellLayout(
    /* [in] */ IContext* context)
{
    PagedViewCellLayout(context, NULL);
}

PagedViewCellLayout::PagedViewCellLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    PagedViewCellLayout(context, attrs, 0);
}

PagedViewCellLayout::PagedViewCellLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : ViewGroup(context, attrs, defStyle)
{
    SetAlwaysDrawnWithCacheEnabled(FALSE);

    // setup default cell parameters
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    mOriginalCellWidth = mCellWidth =
    resources->GetDimensionPixelSize(
            Elastos::Droid::Packages::Launcher2::R::dimen::apps_customize_cell_width,
            &mCellWidth);
    mOriginalCellWidth = mCellWidth;
    resources->GetDimensionPixelSize(
            Elastos::Droid::Packages::Launcher2::R::dimen::apps_customize_cell_height,
            &mCellHeight);
    mOriginalCellHeight = mCellHeight;
    LauncherModel::GetCellCountX(&mCellCountX);
    LauncherModel::GetCellCountY(&mCellCountY);
    mOriginalWidthGap = mOriginalHeightGap = mWidthGap = mHeightGap = -1;
    resources->GetDimensionPixelSize(
        Elastos::Droid::Packages::Launcher2::R::dimen::apps_customize_max_gap,
        &mMaxGap);

    CPagedViewCellLayoutChildren::New(context, (IPagedViewCellLayoutChildren**)&mChildren);
    mChildren->SetCellDimensions(mCellWidth, mCellHeight);
    mChildren->SetGap(mWidthGap, mHeightGap);

    AddView(mChildren);
}

PagedViewCellLayout::GetCellWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mCellWidth;
    return NOERROR;
}

ECode PagedViewCellLayout::GetCellHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = mCellHeight;
    return NOERROR;
}

ECode PagedViewCellLayout::CancelLongPress()
{
    ViewGroup::CancelLongPress();

    // Cancel long press for all children
    Int32 count;
    GetChildCount(&count);
    for (Int32 i = 0; i < count; i++) {
        AUtoPtr<IView> child;
        GetChildAt(i, (IView**)&child);
        child->CancelLongPress();
    }
    return NOERROR;
}

ECode PagedViewCellLayout::AddViewToCellLayout(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ Int32 childId,
    /* [in] */ PagedViewCellLayout::LayoutParams* params,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AUtoPtr<PagedViewCellLayout::LayoutParams> lp = params;

    // Generate an id for each view, this assumes we have at most 256x256 cells
    // per workspace screen
    if (lp->mCellX >= 0 && lp->mCellX <= (mCellCountX - 1) &&
            lp->mCellY >= 0 && (lp->mCellY <= mCellCountY - 1)) {
        // If the horizontal or vertical span is set to -1, it is taken to
        // mean that it spans the extent of the CellLayout
        if (lp->mCellHSpan < 0) lp->mCellHSpan = mCellCountX;
        if (lp->mCellVSpan < 0) lp->mCellVSpan = mCellCountY;

        child->SetId(childId);
        mChildren->AddView(child, index, lp);

        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode PagedViewCellLayout::RemoveAllViewsOnPage()
{
    mChildren->RemoveAllViews();
    return SetLayerType(LAYER_TYPE_NONE, NULL);
}

ECode PagedViewCellLayout::RemoveViewOnPageAt(
    /* [in] */ Int32 index)
{
    return mChildren->RemoveViewAt(index);
}

ECode PagedViewCellLayout::ResetChildrenOnKeyListeners()
{
    Int32 childCount;
    mChildren->GetChildCount(&childCount);
    for (Int32 j = 0; j < childCount; ++j) {
        AutoPtr<IView> view = mChildren->GetChildAt(j);
        view->SetOnKeyListener(NULL);
    }
    return NOERROR;
}

ECode PagedViewCellLayout::GetPageChildCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    return mChildren->GetChildCount(count);
}

ECode PagedViewCellLayout::GetChildrenLayout(
    /* [out] */ IPagedViewCellLayoutChildren** children)
{
    VALIDATE_NOT_NULL(children);

    *children = mChildren;
    REFCOUNT_ADD(*children);
    return NOERROR;
}

ECode PagedViewCellLayout::GetChildOnPageAt(
    /* [in] */ Int32 i,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    return mChildren->GetChildAt(i, view);
}

ECode PagedViewCellLayout::IndexOfChildOnPage(
    /* [in] */ IView* v,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    return mChildren->IndexOfChild(v, index);
}

ECode PagedViewCellLayout::GetCellCountX(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = mCellCountX;
    return NOERROR;
}

ECode PagedViewCellLayout::GetCellCountY(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = mCellCountY;
    return NOERROR;
}

ECode PagedViewCellLayout::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 widthSpecMode;
    View::MeasureSpec::GetMode(widthMeasureSpec, &widthSpecMode);
    Int32 widthSpecSize;
    View::MeasureSpec->GetSize(widthMeasureSpec, &widthSpecSize);

    Int32 heightSpecMode;
    View::MeasureSpec->GetMode(heightMeasureSpec, &heightSpecMode);
    Int32 heightSpecSize;
    View::MeasureSpec->GetSize(heightMeasureSpec, &heightSpecSize);

    if (widthSpecMode == View::MeasureSpec::UNSPECIFIED ||
            heightSpecMode == View::MeasureSpec::UNSPECIFIED) {
        //throw new RuntimeException("CellLayout cannot have UNSPECIFIED dimensions");
        Slogger::E(TAG, "CellLayout cannot have UNSPECIFIED dimensions");
        return E_RUNTIME_EXCEPTION;
    }

    Int32 numWidthGaps = mCellCountX - 1;
    Int32 numHeightGaps = mCellCountY - 1;

    if (mOriginalWidthGap < 0 || mOriginalHeightGap < 0) {
        Int32 left;
        GetPaddingLeft(&left);
        Int32 right;
        GetPaddingRight(&right);
        Int32 hSpace = widthSpecSize - left - right;
        Int32 top;
        GetPaddingTop(&top);
        Int32 bottom;
        GetPaddingBottom(&bottom);
        Int32 vSpace = heightSpecSize - top - bottom;
        Int32 hFreeSpace = hSpace - (mCellCountX * mOriginalCellWidth);
        Int32 vFreeSpace = vSpace - (mCellCountY * mOriginalCellHeight);
        mWidthGap = Math::Min(mMaxGap, numWidthGaps > 0 ? (hFreeSpace / numWidthGaps) : 0);
        mHeightGap = Math::Min(mMaxGap,numHeightGaps > 0 ? (vFreeSpace / numHeightGaps) : 0);

        mChildren->SetGap(mWidthGap, mHeightGap);
    }
    else {
        mWidthGap = mOriginalWidthGap;
        mHeightGap = mOriginalHeightGap;
    }

    // Initial values correspond to widthSpecMode == MeasureSpec.EXACTLY
    Int32 newWidth = widthSpecSize;
    Int32 newHeight = heightSpecSize;
    if (widthSpecMode == View::MeasureSpec::AT_MOST) {
        Int32 left;
        GetPaddingLeft(&left);
        Int32 right;
        GetPaddingRight(&right);
        newWidth = left + right + (mCellCountX * mCellWidth) +
            ((mCellCountX - 1) * mWidthGap);
        Int32 top;
        GetPaddingTop(&top);
        Int32 bottom;
        GetPaddingBottom(&bottom);
        newHeight = top + bottom + (mCellCountY * mCellHeight) +
            ((mCellCountY - 1) * mHeightGap);
        SetMeasuredDimension(newWidth, newHeight);
    }

    Int32 count;
    GetChildCount(&count);
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child;
        GetChildAt(i, (IView**)&child);
        Int32 left;
        GetPaddingLeft(&left);
        Int32 right;
        GetPaddingRight(&right);
        Int32 childWidthMeasureSpec =
                View::MeasureSpec::makeMeasureSpec(newWidth - left -
                right, View::MeasureSpec::EXACTLY);
        Int32 top;
        GetPaddingTop(&top);
        Int32 bottom;
        GetPaddingBottom(&bottom);
        Int32 childheightMeasureSpec =
                View::MeasureSpec::makeMeasureSpec(newHeight - top -
                bottom, View::MeasureSpec::EXACTLY);
        child->Measure(childWidthMeasureSpec, childheightMeasureSpec);
    }

    return SetMeasuredDimension(newWidth, newHeight);
}

ECode PagedViewCellLayout::GetContentWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    Int32 layout;
    GetWidthBeforeFirstLayout(&layout);
    Int32 left;
    GetPaddingLeft(&left);
    Int32 right;
    GetPaddingRight(&right);
    *width = layout + left + right;
    return NOERROR;
}

ECode PagedViewCellLayout::GetContentHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    if (mCellCountY > 0) {
        *height = mCellCountY * mCellHeight + (mCellCountY - 1) * Math::Max(0, mHeightGap);
        return NOERROR;
    }
    *height = 0;
    return NOERROR;
}

ECode PagedViewCellLayout::GetWidthBeforeFirstLayout(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    if (mCellCountX > 0) {
        *width = mCellCountX * mCellWidth + (mCellCountX - 1) * Math::Max(0, mWidthGap);
        return NOERROR;
    }
    *width = 0;
    return NOERROR;
}

ECode PagedViewCellLayout::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    Int32 count;
    GetChildCount(&count);
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child;
        GetChildAt(i, (IView**)&child);
        Int32 left;
        GetPaddingLeft(&left);
        Int32 top;
        GetPaddingTop(&top);
        Int32 right;
        GetPaddingRight(&right);
        Int32 bottom;
        GetPaddingBottom(&bottom);
        child->Layout(left, top, r - l - right, b - t - bottom);
    }
    return NOERROR;
}

ECode PagedViewCellLayout::OnTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Boolean result;
    ViewGroup::OnTouchEvent(event, &result);
    Int32 count;
    GetPageChildCount(&count);
    if (count > 0) {
        // We only intercept the touch if we are tapping in empty space after the final row
        AutoPtr<IView> child;
        GetChildOnPageAt(count - 1, (IView**)&child);
        Int32 bottom;
        child->GetBottom(&bottom);
        Int32 count;
        GetPageChildCount(&count);
        Int32 countX;
        GetCellCountX(&countX);
        Int32 numRows = (Int32)Math::Ceil((Float)count / countX);
        Int32 countY;
        GetCellCountY(&countY);
        if (numRows < countY) {
            // Add a little bit of buffer if there is room for another row
            bottom += mCellHeight / 2;
        }
        Int32 y;
        event->GetY(&y);
        result = result || (y < bottom);
    }
    *result = result;
    return NOERROR;
}

ECode PagedViewCellLayout::EnableCenteredContent(
    /* [in] */ Boolean enabled)
{
    return mChildren->EnableCenteredContent(enabled);
}

ECode PagedViewCellLayout::SetChildrenDrawingCacheEnabled(
    /* [in] */ Boolean enabled)
{
    return mChildren->SetChildrenDrawingCacheEnabled(enabled);
}

ECode PagedViewCellLayout::SetCellCount(
    /* [in] */ Int32 xCount,
    /* [in] */ Int32 yCount)
{
    mCellCountX = xCount;
    mCellCountY = yCount;
    return RequestLayout();
}

ECode PagedViewCellLayout::SetGap(
    /* [in] */ Int32 widthGap,
    /* [in] */ Int32 heightGap)
{
    mOriginalWidthGap = mWidthGap = widthGap;
    mOriginalHeightGap = mHeightGap = heightGap;
    return mChildren->SetGap(widthGap, heightGap);
}

ECode PagedViewCellLayout::GetCellCountForDimensions(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [out, callee] */ ArrayOf<Int32>** dimensions)
{
    VALIDATE_NOT_NULL(dimensions);

    // Always assume we're working with the smallest span to make sure we
    // reserve enough space in both orientations
    Int32 smallerSize = Math::Min(mCellWidth, mCellHeight);

    // Always round up to next largest cell
    Int32 spanX = (width + smallerSize) / smallerSize;
    Int32 spanY = (height + smallerSize) / smallerSize;

    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(2);
    (*array)[0] = spanX;
    (*array)[1] = spanY;
    *dimensions = array;
    REFCOUNT_ADD(*dimensions);
    return NOERROR;
}

ECode PagedViewCellLayout::OnDragChild(
    /* [in] */ IView* child)
{
    AutoPtr<IViewGroupLayoutParams> params;
    child->GetLayoutParams((IViewGroupLayoutParams**)&params);
    AutoPtr<PagedViewCellLayout::LayoutParams> lp =
            (PagedViewCellLayout::LayoutParams*)IObject::Probe(params);
    lp->mIsDragging = TRUE;
    return NOERROR;
}

ECode PagedViewCellLayout::EstimateCellHSpan(
    /* [in] */ Int32 width,
    /* [out] */ Int32* span)
{
    VALIDATE_NOT_NULL(dimensions);

    // We don't show the next/previous pages any more, so we use the full width, minus the
    // padding
    Int32 left;
    GetPaddingLeft(&left);
    Int32 right;
    GetPaddingRight(&right);
    Int32 availWidth = width - (left + right);

    // We know that we have to fit N cells with N-1 width gaps, so we just juggle to solve for N
    Int32 n = Math::Max(1, (availWidth + mWidthGap) / (mCellWidth + mWidthGap));

    // We don't do anything fancy to determine if we squeeze another row in.
    *span = n;
    return NOERROR;
}

ECode PagedViewCellLayout::EstimateCellVSpan(
    /* [in] */ Int32 height,
    /* [out] */ Int32* span)
{
    VALIDATE_NOT_NULL(span);

    // The space for a page is the height - top padding (current page) - bottom padding (current
    // page)
    Int32 top;
    GetPaddingTop(&top);
    Int32 bottom;
    GetPaddingBottom(&bottom);
    Int32 availHeight = height - (top + bottom);

    // We know that we have to fit N cells with N-1 height gaps, so we juggle to solve for N
    Int32 n = Math::Max(1, (availHeight + mHeightGap) / (mCellHeight + mHeightGap));

    // We don't do anything fancy to determine if we squeeze another row in.
    *span = n;
    return NOERROR;
}

ECode PagedViewCellLayout::EstimateCellPosition(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out, callee] */ ArrayOf<Int32>** position)
{
    VALIDATE_NOT_NULL(position);

    Int32 left;
    GetPaddingLeft(&left);
    Int32 top;
    GetPaddingTop(&top);

    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(2);
    (*array)[0] = left + (x * mCellWidth) + (x * mWidthGap) + (mCellWidth / 2);
    (*array)[1] = top + (y * mCellHeight) + (y * mHeightGap) + (mCellHeight / 2);
    *position = array;
    REFCOUNT_ADD(*position);
    return NOERROR;
}

ECode PagedViewCellLayout::CalculateCellCount(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 maxCellCountX,
    /* [in] */ Int32 maxCellCountY)
{
    Int32 span;
    EstimateCellHSpan(width, &span);
    mCellCountX = Math::Min(maxCellCountX, span);
    estimateCellVSpan(height, &span)
    mCellCountY = Math::Min(maxCellCountY, span);
    return RequestLayout();
}

ECode PagedViewCellLayout::EstimateCellWidth(
    /* [in] */ Int32 hSpan,
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    // TODO: we need to take widthGap into effect
    *width = hSpan * mCellWidth;
    return NOERROR;
}

ECode PagedViewCellLayout::EstimateCellHeight(
    /* [in] */ Int32 vSpan,
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    // TODO: we need to take heightGap into effect
    *height = vSpan * mCellHeight;
    return NOERROR;
}

ECode PagedViewCellLayout::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** param)
{
    VALIDATE_NOT_NULL(param);

    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IViewGroupLayoutParams> p = new PagedViewCellLayout::LayoutParams(context, attrs);
    *param = p;
    REFCOUNT_ADD(*param);
    return NOERROR;
}

ECode PagedViewCellLayout::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IPagedViewCellLayoutLayoutParams::Probe(p) != NULL;
    return NOERROR;
}

ECode PagedViewCellLayout::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);

    AutoPtr<IViewGroupLayoutParams> _p = new PagedViewCellLayout::LayoutParams(p);
    *params = _p;
    REFCOUNT_ADD(*params);
    return NOERROR;
}

} // namespace Launcher2
} // namespace Packages
} // namespace Droid
} // namespace Elastos