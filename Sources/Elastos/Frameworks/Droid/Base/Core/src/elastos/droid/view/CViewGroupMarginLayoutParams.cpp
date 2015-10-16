
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/CViewGroupLayoutParams.h"
#include "elastos/droid/view/CViewGroupMarginLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace View {

IVIEWGROUPLP_METHODS_IMPL(CViewGroupMarginLayoutParams, ViewGroupMarginLayoutParams);

IVIEWGROUPMARGINLP_METHODS_IMPL(CViewGroupMarginLayoutParams, ViewGroupMarginLayoutParams);

ECode CViewGroupMarginLayoutParams::ToString(
    /* [out] */ String* description)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CViewGroupMarginLayoutParams::constructor()
{
    return NOERROR;
}

ECode CViewGroupMarginLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return ViewGroupMarginLayoutParams::Init(c, attrs);
}

ECode CViewGroupMarginLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ViewGroupMarginLayoutParams::Init(width, height);
}

ECode CViewGroupMarginLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    return ViewGroupMarginLayoutParams::Init(source);
}

} // namespace View
} // namespace Droid
} // namespace Elastos
