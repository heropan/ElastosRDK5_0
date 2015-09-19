
#include "graphics/CLightingColorFilter.h"
#include <skia/core/SkColorFilter.h>
#include <hwui/SkiaColorFilter.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CLightingColorFilter::constructor(
    /* [in] */ Int32 mul,
    /* [in] */ Int32 add)
{
    mNativeInstance = NativeCreateLightingFilter(mul, add);
    mNativeColorFilter = NCreateLightingFilter(mNativeInstance, mul, add);
    return NOERROR;
}

PInterface CLightingColorFilter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ColorFilter) {
        return reinterpret_cast<PInterface>((ColorFilter*)this);
    }
    return _CLightingColorFilter::Probe(riid);
}

Int32 CLightingColorFilter::NativeCreateLightingFilter(
    /* [in] */ Int32 mul,
    /* [in] */ Int32 add)
{
    return (Int32)SkColorFilter::CreateLightingFilter(mul, add);
}

Int32 CLightingColorFilter::NCreateLightingFilter(
    /* [in] */ Int32 skFilter,
    /* [in] */ Int32 mul,
    /* [in] */ Int32 add)
{
#ifdef USE_OPENGL_RENDERER
        return (Int32)new SkiaLightingFilter((SkColorFilter*)skFilter, mul, add);
#else
        return 0;
#endif
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
