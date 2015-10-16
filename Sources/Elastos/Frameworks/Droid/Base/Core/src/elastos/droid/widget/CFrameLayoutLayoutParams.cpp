#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/CFrameLayoutLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEWGROUPLP_METHODS_IMPL(CFrameLayoutLayoutParams, FrameLayoutLayoutParams)
IVIEWGROUPMARGINLP_METHODS_IMPL(CFrameLayoutLayoutParams, FrameLayoutLayoutParams)

ECode CFrameLayoutLayoutParams::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return FrameLayoutLayoutParams::Init(context, attrs);
}

ECode CFrameLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return FrameLayoutLayoutParams::Init(width, height);
}

ECode CFrameLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 weight)
{
    return FrameLayoutLayoutParams::Init(width, height, weight);
}

ECode CFrameLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return FrameLayoutLayoutParams::Init(source);
}

ECode CFrameLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    return FrameLayoutLayoutParams::Init(source);
}

ECode CFrameLayoutLayoutParams::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);
    *gravity = FrameLayoutLayoutParams::GetGravity();
    return NOERROR;
}

ECode CFrameLayoutLayoutParams::SetGravity(
    /* [in] */ Int32 gravity)
{
    return FrameLayoutLayoutParams::SetGravity(gravity);
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
