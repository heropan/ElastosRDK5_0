#include "elastos/droid/widget/GridLayoutLayoutParams.h"
#include "elastos/droid/widget/CGridLayoutLayoutParams.h"

using Elastos::Droid::View::IGravity;
using Elastos::Droid::Widget::CGridLayoutLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {

static AutoPtr<GridLayout::Interval> InitDSpan() {
    AutoPtr<GridLayout::Interval> val = new GridLayout::Interval(GridLayout::UNDEFINED, GridLayout::UNDEFINED + 1);
    return val;
}

const Int32 GridLayoutLayoutParams::DEFAULT_MARGIN = Elastos::Core::Math::INT32_MIN_VALUE;
const Int32 GridLayoutLayoutParams::DEFAULT_ROW = Elastos::Core::Math::INT32_MIN_VALUE;
const Int32 GridLayoutLayoutParams::DEFAULT_COLUMN = Elastos::Core::Math::INT32_MIN_VALUE;
const AutoPtr<GridLayout::Interval> GridLayoutLayoutParams::DEFAULT_SPAN = InitDSpan();
const Int32 GridLayoutLayoutParams::DEFAULT_SPAN_SIZE = DEFAULT_SPAN->Size();

CAR_INTERFACE_IMPL(GridLayoutLayoutParams, ViewGroupMarginLayoutParams, IGridLayoutLayoutParams)

void GridLayoutLayoutParams::ReInitSuper(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::ViewGroup_MarginLayout),
            ARRAY_SIZE(R::styleable::ViewGroup_MarginLayout));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);
    //try {
        Int32 margin = 0;
        a->GetDimensionPixelSize(MARGIN, DEFAULT_MARGIN, &margin);

        a->GetDimensionPixelSize(LEFT_MARGIN, margin, &mLeftMargin);
        a->GetDimensionPixelSize(TOP_MARGIN, margin, &mTopMargin);
        a->GetDimensionPixelSize(RIGHT_MARGIN, margin, &mRightMargin);
        a->GetDimensionPixelSize(BOTTOM_MARGIN, margin, &mBottomMargin);
    //} finally {
        a->Recycle();
    //}
}

void GridLayoutLayoutParams::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::GridLayout_Layout),
            ARRAY_SIZE(R::styleable::GridLayout_Layout));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);

//    try {
        Int32 gravity = 0;
        a->GetInt32(GRAVITY, IGravity::NO_GRAVITY, &gravity);

        Int32 column = 0;
        a->GetInt32(COLUMN, DEFAULT_COLUMN, &column);
        Int32 colSpan = 0;
        a->GetInt32(COLUMN_SPAN, DEFAULT_SPAN_SIZE, &colSpan);
        Float colWeight;
        a->GetFloat(COLUMN_WEIGHT, GridLayout::Spec::DEFAULT_WEIGHT, &colWeight);
        mColumnSpec = GridLayout::GetSpec(column, colSpan, GridLayout::GetAlignment(gravity, TRUE), colWeight);

        Int32 row = 0;
        a->GetInt32(ROW, DEFAULT_ROW, &row);
        Int32 rowSpan = 0;
        a->GetInt32(ROW_SPAN, DEFAULT_SPAN_SIZE, &rowSpan);
        mRowSpec = GridLayout::GetSpec(row, rowSpan, GridLayout::GetAlignment(gravity, FALSE), colWeight);
//    } finally {
        a->Recycle();
//    }
}


GridLayoutLayoutParams::GridLayoutLayoutParams()
{}

ECode GridLayoutLayoutParams::SetGravity(
    /* [in] */ Int32 gravity)
{
    AutoPtr<GridLayout::Spec> row = (GridLayout::Spec*)mRowSpec.Get();
    AutoPtr<GridLayout::Spec> col = (GridLayout::Spec*)mColumnSpec.Get();
    mRowSpec = row->CopyWriteAlignment(GridLayout::GetAlignment(gravity, FALSE));
    mColumnSpec = col->CopyWriteAlignment(GridLayout::GetAlignment(gravity, TRUE));
    return NOERROR;
}

ECode GridLayoutLayoutParams::SetRowSpec(
    /* [in] */ IGridLayoutSpec* r)
{
    mRowSpec = r;
    return NOERROR;
}

ECode GridLayoutLayoutParams::SetColumnSpec(
    /* [in] */ IGridLayoutSpec* c)
{
    mColumnSpec = c;
    return NOERROR;
}

ECode GridLayoutLayoutParams::GetRowSpec(
    /* [out] */ IGridLayoutSpec** r)
{
    VALIDATE_NOT_NULL(r)

    *r = mRowSpec;
    REFCOUNT_ADD(*r)
    return NOERROR;
}

ECode GridLayoutLayoutParams::GetColumnSpec(
    /* [out] */ IGridLayoutSpec** c)
{
    VALIDATE_NOT_NULL(c)

    *c = mColumnSpec;
    REFCOUNT_ADD(*c)
    return NOERROR;
}

void GridLayoutLayoutParams::SetRowSpecSpan(
    /* [in] */ GridLayout::Interval* span)
{
    AutoPtr<GridLayout::Spec> row = (GridLayout::Spec*)mRowSpec.Get();
    mRowSpec = row->CopyWriteSpan(span);
}

void GridLayoutLayoutParams::SetColumnSpecSpan(
    /* [in] */ GridLayout::Interval* span)
{
    AutoPtr<GridLayout::Spec> col = (GridLayout::Spec*)mColumnSpec.Get();
    mColumnSpec = col->CopyWriteSpan(span);
}

ECode GridLayoutLayoutParams::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(obj);

    IGridLayoutLayoutParams * glp = IGridLayoutLayoutParams::Probe(obj);
    if (glp == NULL) return NOERROR;

    GridLayoutLayoutParams* that = (GridLayoutLayoutParams*) glp;

    Boolean equals = FALSE;
    IObject::Probe(mColumnSpec)->Equals(that->mColumnSpec, &equals);
    if (!equals) return NOERROR;
    IObject::Probe(mRowSpec)->Equals(that->mRowSpec, &equals);
    if (!equals) return NOERROR;

    *result = TRUE;
    return NOERROR;
}

ECode GridLayoutLayoutParams::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash)

    Int32 result = 0;
    IObject::Probe(mRowSpec)->GetHashCode(&result);
    Int32 col = 0;
    IObject::Probe(mColumnSpec)->GetHashCode(&col);
    result = 31 * result + col;
    *hash = result;
    return NOERROR;
}

ECode GridLayoutLayoutParams::SetBaseAttributes(
    /* [in] */ ITypedArray* attributes,
    /* [in] */ Int32 widthAttr,
    /* [in] */ Int32 heightAttr)
{
    FAIL_RETURN(attributes->GetLayoutDimension(widthAttr, DEFAULT_WIDTH, &mWidth));
    return attributes->GetLayoutDimension(heightAttr, DEFAULT_HEIGHT, &mHeight);
}

ECode GridLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ IGridLayoutSpec* rowSpec,
    /* [in] */ IGridLayoutSpec* columnSpec)
{
    FAIL_RETURN(ViewGroupMarginLayoutParams::constructor(width, height))
    SetMargins(left, top, right, bottom);
    mRowSpec = rowSpec;
    mColumnSpec = columnSpec;
    return NOERROR;
}

ECode GridLayoutLayoutParams::constructor()
{
    return constructor(GridLayout::Spec::UNDEFINED, GridLayout::Spec::UNDEFINED);
}

ECode GridLayoutLayoutParams::constructor(
    /* [in] */ IGridLayoutSpec* rowSpec,
    /* [in] */ IGridLayoutSpec* columnSpec)
{
    return constructor(DEFAULT_WIDTH, DEFAULT_HEIGHT,
        DEFAULT_MARGIN, DEFAULT_MARGIN, DEFAULT_MARGIN, DEFAULT_MARGIN,
        rowSpec, columnSpec);
}

ECode GridLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* params)
{
    return ViewGroupMarginLayoutParams::constructor(params);
}

ECode GridLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* params)
{
    return ViewGroupMarginLayoutParams::constructor(params);
}

ECode GridLayoutLayoutParams::constructor(
    /* [in] */ IGridLayoutLayoutParams* that)
{
    FAIL_RETURN(ViewGroupMarginLayoutParams::constructor(IViewGroupMarginLayoutParams::Probe(that)))
    AutoPtr<GridLayoutLayoutParams> cgl = (GridLayoutLayoutParams*)that;
    mRowSpec = cgl->mRowSpec;
    mColumnSpec = cgl->mColumnSpec;
    return NOERROR;
}

ECode GridLayoutLayoutParams::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(ViewGroupMarginLayoutParams::constructor(context, attrs))
    ReInitSuper(context, attrs);
    Init(context, attrs);
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos























