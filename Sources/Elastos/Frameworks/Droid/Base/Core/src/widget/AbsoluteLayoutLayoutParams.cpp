#include "ext/frameworkext.h"
#include "widget/AbsoluteLayoutLayoutParams.h"
#include <R.h>

namespace Elastos {
namespace Droid {
namespace Widget {


AbsoluteLayoutLayoutParams::AbsoluteLayoutLayoutParams()
    : ViewGroupLayoutParams(0, 0)
    , mX(0)
    , mY(0)
{}

AbsoluteLayoutLayoutParams::AbsoluteLayoutLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
    : ViewGroupLayoutParams(width, height)
    , mX(x)
    , mY(y)
{}

AbsoluteLayoutLayoutParams::AbsoluteLayoutLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroupLayoutParams(c, attrs)
{
    InitFromAttributes(c, attrs);
}

AbsoluteLayoutLayoutParams::AbsoluteLayoutLayoutParams(
    /* [in] */ ViewGroupLayoutParams* source)
    : ViewGroupLayoutParams(source)
{
}

ECode AbsoluteLayoutLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    ViewGroupLayoutParams::Init(width, height);
    mX = x;
    mY = y;
    return NOERROR;
}

ECode AbsoluteLayoutLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    ViewGroupLayoutParams::Init(c, attrs);
    return InitFromAttributes(c, attrs);
}

ECode AbsoluteLayoutLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return ViewGroupLayoutParams::Init(source);
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
    FAIL_RETURN(c->ObtainStyledAttributesEx2(attrs, attrIds, (ITypedArray**)&a));
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

Int32 AbsoluteLayoutLayoutParams::GetX()
{
    return mX;
}

Int32 AbsoluteLayoutLayoutParams::GetY()
{
    return mY;
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos
