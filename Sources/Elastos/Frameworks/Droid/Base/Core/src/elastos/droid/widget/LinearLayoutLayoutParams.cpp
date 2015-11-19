
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/LinearLayoutLayoutParams.h"
#include "elastos/droid/R.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_INTERFACE_IMPL(LinearLayoutLayoutParams, ViewGroupMarginLayoutParams, ILinearLayoutLayoutParams);
LinearLayoutLayoutParams::LinearLayoutLayoutParams()
    : mWeight(0)
    , mGravity(-1)
{}

ECode LinearLayoutLayoutParams::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(ViewGroupMarginLayoutParams::constructor(context, attrs));

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::LinearLayout_Layout),
            ARRAY_SIZE(R::styleable::LinearLayout_Layout));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributes(
            attrs, attrIds, (ITypedArray**)&a));

    a->GetFloat(R::styleable::LinearLayout_Layout_layout_weight,
            0, &mWeight);
    a->GetInt32(R::styleable::LinearLayout_Layout_layout_gravity,
            -1, &mGravity);

    a->Recycle();
    return NOERROR;
}

ECode LinearLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    ViewGroupMarginLayoutParams::constructor(width, height);
    mWeight = 0;
    return NOERROR;
}

ECode LinearLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Float weight)
{
    ViewGroupMarginLayoutParams::constructor(width, height);
    mWeight = weight;
    return NOERROR;
}

ECode LinearLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    ViewGroupMarginLayoutParams::constructor(source);
    return NOERROR;
}

ECode LinearLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    ViewGroupMarginLayoutParams::constructor(source);
    return NOERROR;
}

ECode LinearLayoutLayoutParams::constructor(
    /* [in] */ ILinearLayoutLayoutParams* source)
{
    ViewGroupMarginLayoutParams::constructor(IViewGroupMarginLayoutParams::Probe(source));
    mWeight = ((LinearLayoutLayoutParams*)source)->mWeight;
    mGravity = ((LinearLayoutLayoutParams*)source)->mGravity;
    return NOERROR;
}

ECode LinearLayoutLayoutParams::SetWeight(
    /* [in] */ Float weight)
{
    mWeight = weight;
    return NOERROR;
}

ECode LinearLayoutLayoutParams::GetWeight(
    /* [out] */ Float* weight)
{
    VALIDATE_NOT_NULL(weight);
    *weight = mWeight;
    return NOERROR;
}

ECode LinearLayoutLayoutParams::SetGravity(
    /* [in] */ Int32 gravity)
{
    mGravity = gravity;
    return NOERROR;
}

ECode LinearLayoutLayoutParams::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);
    *gravity = mGravity;
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
