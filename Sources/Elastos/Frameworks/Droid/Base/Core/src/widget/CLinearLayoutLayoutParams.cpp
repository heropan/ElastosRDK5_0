#include "ext/frameworkext.h"
#include "widget/CLinearLayoutLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEWGROUPLP_METHODS_IMPL(CLinearLayoutLayoutParams, LinearLayoutLayoutParams)
IVIEWGROUPMARGINLP_METHODS_IMPL(CLinearLayoutLayoutParams, LinearLayoutLayoutParams)

ECode CLinearLayoutLayoutParams::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return LinearLayoutLayoutParams::Init(context, attrs);
}

ECode CLinearLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return LinearLayoutLayoutParams::Init(width, height);
}

ECode CLinearLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Float weight)
{
    return LinearLayoutLayoutParams::Init(width, height, weight);
}

ECode CLinearLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return LinearLayoutLayoutParams::Init(source);
}

ECode CLinearLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    return LinearLayoutLayoutParams::Init(source);
}

ECode CLinearLayoutLayoutParams::GetWeight(
    /* [out] */ Float* weight)
{
    VALIDATE_NOT_NULL(weight);
    *weight = LinearLayoutLayoutParams::GetWeight();
    return NOERROR;
}

ECode CLinearLayoutLayoutParams::SetWeight(
    /* [in] */ Float weight)
{
    return LinearLayoutLayoutParams::SetWeight(weight);
}

ECode CLinearLayoutLayoutParams::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);
    *gravity = LinearLayoutLayoutParams::GetGravity();
    return NOERROR;
}

ECode CLinearLayoutLayoutParams::SetGravity(
    /* [in] */ Int32 gravity)
{
    return LinearLayoutLayoutParams::SetGravity(gravity);
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
