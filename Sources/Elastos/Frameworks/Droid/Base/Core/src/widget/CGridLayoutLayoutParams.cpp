#include "ext/frameworkext.h"
#include "widget/CGridLayoutLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEWGROUPLP_METHODS_IMPL(CGridLayoutLayoutParams, GridLayoutLayoutParams)
IVIEWGROUPMARGINLP_METHODS_IMPL(CGridLayoutLayoutParams, GridLayoutLayoutParams)

ECode CGridLayoutLayoutParams::constructor()
{
    return GridLayoutLayoutParams::Init();
}

ECode CGridLayoutLayoutParams::constructor(
    /* [in] */ ISpec* rowSpec,
    /* [in] */ ISpec* columnSpec)
{
    return GridLayoutLayoutParams::Init(rowSpec, columnSpec);
}

ECode CGridLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* params)
{
    return GridLayoutLayoutParams::Init(params);
}

ECode CGridLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* params)
{
    return GridLayoutLayoutParams::Init(params);
}

ECode CGridLayoutLayoutParams::constructor(
    /* [in] */ IGridLayoutLayoutParams* that)
{
    return GridLayoutLayoutParams::Init(that);
}

ECode CGridLayoutLayoutParams::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return GridLayoutLayoutParams::Init(context, attrs);
}

ECode CGridLayoutLayoutParams::SetGravity(
    /* [in] */ Int32 gravity)
{
    return GridLayoutLayoutParams::SetGravity(gravity);
}

ECode CGridLayoutLayoutParams::GetHashCode(
    /* [out] */ Int32* hash)
{
    return GridLayoutLayoutParams::GetHashCode(hash);
}

ECode CGridLayoutLayoutParams::Equals(
    /* [in] */ IInterface* layout,
    /* [out] */ Boolean* result)
{
    return GridLayoutLayoutParams::Equals(layout, result);
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
