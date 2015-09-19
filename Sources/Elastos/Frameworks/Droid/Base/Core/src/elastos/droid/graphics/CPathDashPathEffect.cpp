#include "graphics/CPathDashPathEffect.h"
#include "graphics/CPath.h"
#include <skia/effects/Sk1DPathEffect.h>
#include <skia/core/SkPath.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CPathDashPathEffect::constructor(
    /* [in] */ IPath* shape,
    /* [in] */ Float advance,
    /* [in] */ Float phase,
    /* [in] */ PathDashPathEffectStyle style)
{
    mNativeInstance = NativeCreate(
                        ((CPath*)shape)->Ni(),
                        advance,
                        phase,
                        style);
    return NOERROR;
}

PInterface CPathDashPathEffect::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_PathEffect) {
        return reinterpret_cast<PInterface>((PathEffect*)this);
    }
    return _CPathDashPathEffect::Probe(riid);
}

Int32 CPathDashPathEffect::NativeCreate(
    /* [in] */ Int32 nativePath,
    /* [in] */ Float advance,
    /* [in] */ Float phase,
    /* [in] */ Int32 nativeStyle)
{
    SkPath* skPath = reinterpret_cast<SkPath*>(nativePath);
    SkASSERT(skPath != NULL);

    return reinterpret_cast<Int32>(new SkPath1DPathEffect(
                    *skPath,
                    SkFloatToScalar(advance),
                    SkFloatToScalar(phase),
                    (SkPath1DPathEffect::Style)nativeStyle));
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
