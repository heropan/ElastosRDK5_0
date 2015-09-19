#include "ext/frameworkext.h"
#include "widget/CAbsoluteLayoutLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEWGROUPLP_METHODS_IMPL(CAbsoluteLayoutLayoutParams, AbsoluteLayoutLayoutParams);

ECode CAbsoluteLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return AbsoluteLayoutLayoutParams::Init(width, height, x, y);
}

ECode CAbsoluteLayoutLayoutParams::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return AbsoluteLayoutLayoutParams::Init(context, attrs);
}

ECode CAbsoluteLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return AbsoluteLayoutLayoutParams::Init(source);
}

ECode CAbsoluteLayoutLayoutParams::SetX(
    /* [in] */ Int32 x)
{
    return AbsoluteLayoutLayoutParams::SetX(x);
}

ECode CAbsoluteLayoutLayoutParams::SetY(
    /* [in] */ Int32 y)
{
    return AbsoluteLayoutLayoutParams::SetY(y);
}

ECode CAbsoluteLayoutLayoutParams::GetX(
    /* [out] */ Int32* x)
{
    VALIDATE_NOT_NULL(x);
    *x = AbsoluteLayoutLayoutParams::GetX();
    return NOERROR;
}

ECode CAbsoluteLayoutLayoutParams::GetY(
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(y);
    *y = AbsoluteLayoutLayoutParams::GetY();
    return NOERROR;
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos
