
#include "ext/frameworkext.h"
#include "view/CViewGroupLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace View {

IVIEWGROUPLP_METHODS_IMPL(CViewGroupLayoutParams, ViewGroupLayoutParams);

ECode CViewGroupLayoutParams::constructor()
{
    return NOERROR;
}

ECode CViewGroupLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return ViewGroupLayoutParams::Init(c, attrs);
}

ECode CViewGroupLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ViewGroupLayoutParams::Init(width, height);
}

ECode CViewGroupLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return ViewGroupLayoutParams::Init(source);
}

ECode CViewGroupLayoutParams::ToString(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
