#include "ext/frameworkext.h"
#include "widget/CRelativeLayoutLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEWGROUPLP_METHODS_IMPL(CRelativeLayoutLayoutParams, RelativeLayoutLayoutParams)
IVIEWGROUPMARGINLP_METHODS_IMPL(CRelativeLayoutLayoutParams, RelativeLayoutLayoutParams)

ECode CRelativeLayoutLayoutParams::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return RelativeLayoutLayoutParams::Init(context, attrs);
}

ECode CRelativeLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return RelativeLayoutLayoutParams::Init(width, height);
}

ECode CRelativeLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return RelativeLayoutLayoutParams::Init(source);
}

ECode CRelativeLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    return RelativeLayoutLayoutParams::Init(source);
}

ECode CRelativeLayoutLayoutParams::AddRule(
    /* [in] */ Int32 verb)
{
    return RelativeLayoutLayoutParams::AddRule(verb);
}

ECode CRelativeLayoutLayoutParams::AddRuleEx(
    /* [in] */ Int32 verb,
    /* [in] */ Int32 anchor)
{
    return RelativeLayoutLayoutParams::AddRule(verb, anchor);
}

 ECode CRelativeLayoutLayoutParams::RemoveRule(
     /* [in] */ Int32 verb)
 {
     return RelativeLayoutLayoutParams::RemoveRule(verb);
 }

 ECode CRelativeLayoutLayoutParams::GetRules(
    /* [out, callee] */ ArrayOf<Int32>** rules)
 {
     VALIDATE_NOT_NULL(rules);
     *rules = NULL;

     AutoPtr<ArrayOf<Int32> > r = RelativeLayoutLayoutParams::GetRules();
     if (r != NULL) {
         *rules = r;
         INTERFACE_ADDREF(*rules);
     }
     return NOERROR;
 }

ECode CRelativeLayoutLayoutParams::GetRulesEx(
    /* [in] */ Int32 layoutDirection,
    /* [out, callee] */ ArrayOf<Int32>** rules)
{
    VALIDATE_NOT_NULL(rules);
    AutoPtr<ArrayOf<Int32> > r = RelativeLayoutLayoutParams::GetRules(layoutDirection);
    if (r != NULL) {
        *rules = r;
        INTERFACE_ADDREF(*rules);
    }
    return NOERROR;
}

ECode CRelativeLayoutLayoutParams::SetAlignWithParent(
    /* [in] */ Boolean align)
{
    return RelativeLayoutLayoutParams::SetAlignWithParent(align);
}

ECode CRelativeLayoutLayoutParams::GetAlignWithParent(
    /* [out] */ Boolean* align)
{
    VALIDATE_NOT_NULL(align);
    *align = RelativeLayoutLayoutParams::GetAlignWithParent();
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
