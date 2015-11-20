
#include "elastos/droid/widget/FrameLayoutLayoutParams.h"
#include "elastos/droid/R.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CAR_INTERFACE_IMPL(FrameLayoutLayoutParams, ViewGroupMarginLayoutParams, IFrameLayoutLayoutParams);
FrameLayoutLayoutParams::FrameLayoutLayoutParams()
    : mGravity(-1)
{}

ECode FrameLayoutLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(ViewGroupMarginLayoutParams::constructor(c, attrs));

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::FrameLayout_Layout),
            ARRAY_SIZE(R::styleable::FrameLayout_Layout));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(c->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a));
    a->GetInt32(R::styleable::FrameLayout_Layout_layout_gravity, -1, &mGravity);

    a->Recycle();
    return NOERROR;
}

ECode FrameLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ViewGroupMarginLayoutParams::constructor(width, height);
}

ECode FrameLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 gravity)
{
    ViewGroupMarginLayoutParams::constructor(width, height);
    mGravity = gravity;
    return NOERROR;
}

ECode FrameLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return ViewGroupMarginLayoutParams::constructor(source);
}

ECode FrameLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    return ViewGroupMarginLayoutParams::constructor(source);
}

ECode FrameLayoutLayoutParams::constructor(
    /* [in] */ IFrameLayoutLayoutParams* source)
{
    ViewGroupMarginLayoutParams::constructor(IViewGroupMarginLayoutParams::Probe(source));
    mGravity = ((FrameLayoutLayoutParams*)source)->mGravity;
    return NOERROR;
}

ECode FrameLayoutLayoutParams::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);
    *gravity = mGravity;
    return NOERROR;
}

ECode FrameLayoutLayoutParams::SetGravity(
    /* [in] */ Int32 gravity)
{
    mGravity = gravity;
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
