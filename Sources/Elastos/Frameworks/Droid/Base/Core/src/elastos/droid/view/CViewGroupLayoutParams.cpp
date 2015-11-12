
#include "elastos/droid/view/CViewGroupLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace View {

CAR_OBJECT_IMPL(CViewGroupLayoutParams);
ECode CViewGroupLayoutParams::constructor()
{
    return NOERROR;
}

ECode CViewGroupLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return ViewGroupLayoutParams::constructor(c, attrs);
}

ECode CViewGroupLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ViewGroupLayoutParams::constructor(width, height);
}

ECode CViewGroupLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return ViewGroupLayoutParams::constructor(source);
}

} // namespace View
} // namespace Droid
} // namespace Elastos
