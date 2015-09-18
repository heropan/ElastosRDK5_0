
#include "graphics/CBlurMaskFilter.h"
#include <skia/effects/SkBlurMaskFilter.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

CAR_OBJECT_IMPL(CBlurMaskFilter);
CAR_INTERFACE_IMPL(CBlurMaskFilter, MaskFilter, IBlurMaskFilter);
ECode CBlurMaskFilter::constructor(
    /* [in] */ Float radius,
    /* [in] */ Int32 style)
{
    mNativeInstance = NativeConstructor(radius, style);
    return NOERROR;
}

PInterface CBlurMaskFilter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_MaskFilter) {
        return reinterpret_cast<PInterface>((MaskFilter*)this);
    }
    return _CBlurMaskFilter::Probe(riid);
}

Int32 CBlurMaskFilter::NativeConstructor(
    /* [in] */ Float radius,
    /* [in] */ Int32 style)
{
    SkMaskFilter* filter = SkBlurMaskFilter::Create(
            SkFloatToScalar(radius), (SkBlurMaskFilter::BlurStyle)style);
    assert(filter != NULL);
    return reinterpret_cast<Int32>(filter);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
