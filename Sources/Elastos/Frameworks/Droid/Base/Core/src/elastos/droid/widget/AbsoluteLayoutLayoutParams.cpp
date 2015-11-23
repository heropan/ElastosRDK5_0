#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/AbsoluteLayoutLayoutParams.h"
#include "elastos/droid/R.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_INTERFACE_IMPL(AbsoluteLayoutLayoutParams, ViewGroupLayoutParams, IAbsoluteLayoutLayoutParams)

AbsoluteLayoutLayoutParams::AbsoluteLayoutLayoutParams()
    : mX(0)
    , mY(0)
{}

ECode AbsoluteLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    ViewGroupLayoutParams::constructor(width, height);
    mX = x;
    mY = y;
    return NOERROR;
}

ECode AbsoluteLayoutLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    ViewGroupLayoutParams::constructor(c, attrs);
    return InitFromAttributes(c, attrs);
}

ECode AbsoluteLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return ViewGroupLayoutParams::constructor(source);
}

ECode AbsoluteLayoutLayoutParams::InitFromAttributes(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    VALIDATE_NOT_NULL(c);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::AbsoluteLayout_Layout),
            ARRAY_SIZE(R::styleable::AbsoluteLayout_Layout));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(c->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a));
    FAIL_RETURN(a->GetDimensionPixelOffset(
            R::styleable::AbsoluteLayout_Layout_layout_x, 0, &mX));
    FAIL_RETURN(a->GetDimensionPixelOffset(
            R::styleable::AbsoluteLayout_Layout_layout_y, 0, &mY));
    return a->Recycle();
}

ECode AbsoluteLayoutLayoutParams::SetX(
    /* [in] */ Int32 x)
{
    mX = x;
    return NOERROR;
}

ECode AbsoluteLayoutLayoutParams::SetY(
    /* [in] */ Int32 y)
{
    mY = y;
    return NOERROR;
}

ECode AbsoluteLayoutLayoutParams::GetX(
    /* [out] */ Int32* x)
{
    VALIDATE_NOT_NULL(x)
    *x = mX;
    return NOERROR;
}

ECode AbsoluteLayoutLayoutParams::GetY(
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(y)
    *y = mY;
    return NOERROR;
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos
