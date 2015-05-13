
#include "graphics/CCornerPathEffect.h"
#include <skia/effects/SkCornerPathEffect.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CCornerPathEffect::constructor(
    /* [in] */ Float radius)
{
    mNativeInstance = NativeCreate(radius);
    return NOERROR;
}

PInterface CCornerPathEffect::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_PathEffect) {
        return reinterpret_cast<PInterface>((PathEffect*)this);
    }
    return _CCornerPathEffect::Probe(riid);
}

Int32 CCornerPathEffect::NativeCreate(
    /* [in] */ Float radius)
{
    return reinterpret_cast<Int32>(new SkCornerPathEffect(
            SkFloatToScalar(radius)));
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

