
#include "graphics/CDiscretePathEffect.h"
#include <skia/effects/SkDiscretePathEffect.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CDiscretePathEffect::constructor(
    /* [in] */ Float segmentLength,
    /* [in] */ Float deviation)
{
    mNativeInstance = NativeCreate(
                        segmentLength,
                        deviation);
    return NOERROR;
}

PInterface CDiscretePathEffect::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_PathEffect) {
        return reinterpret_cast<PInterface>((PathEffect*)this);
    }
    return _CDiscretePathEffect::Probe(riid);
}

Int32 CDiscretePathEffect::NativeCreate(
    /* [in] */ Float length,
    /* [in] */ Float deviation)
{
    return reinterpret_cast<Int32>(new SkDiscretePathEffect(
            SkFloatToScalar(length), SkFloatToScalar(deviation)));
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
