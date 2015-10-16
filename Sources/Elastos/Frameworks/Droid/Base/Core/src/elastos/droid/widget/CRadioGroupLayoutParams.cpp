#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/CRadioGroupLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEWGROUPLP_METHODS_IMPL(CRadioGroupLayoutParams, RadioGroupLayoutParams)
IVIEWGROUPMARGINLP_METHODS_IMPL(CRadioGroupLayoutParams, RadioGroupLayoutParams)
ILINEARLAYOUTLP_METHODS_IMPL(CRadioGroupLayoutParams, RadioGroupLayoutParams)


ECode CRadioGroupLayoutParams::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return RadioGroupLayoutParams::Init(context, attrs);
}

ECode CRadioGroupLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return RadioGroupLayoutParams::Init(width, height);
}

ECode CRadioGroupLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Float weight)
{
    return RadioGroupLayoutParams::Init(width, height, weight);
}

ECode CRadioGroupLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return RadioGroupLayoutParams::Init(source);
}

ECode CRadioGroupLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    return RadioGroupLayoutParams::Init(source);
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos
