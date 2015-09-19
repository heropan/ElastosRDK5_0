
#include "graphics/CComposePathEffect.h"
#include <SkPathEffect.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

CAR_OBJECT_IMPL(CComposePathEffect);
CAR_INTERFACE_IMPL(CComposePathEffect, PathEffect, IComposePathEffect);
ECode CComposePathEffect::constructor(
    /* [in] */ IPathEffect* outerpe,
    /* [in] */ IPathEffect* innerpe)
{
    mNativeInstance = NativeCreate(
            ((PathEffect*)outerpe->Probe(EIID_PathEffect))->mNativeInstance,
            ((PathEffect*)innerpe->Probe(EIID_PathEffect))->mNativeInstance);
    return NOERROR;
}

PInterface CComposePathEffect::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_PathEffect) {
        return reinterpret_cast<PInterface>((PathEffect*)this);
    }

    return _CComposePathEffect::Probe(riid);
}

Int32 CComposePathEffect::NativeCreate(
    /* [in] */ Int32 outerpe,
    /* [in] */ Int32 innerpe)
{
    return reinterpret_cast<Int32>(new SkComposePathEffect(
            reinterpret_cast<SkPathEffect*>(outerpe),
            reinterpret_cast<SkPathEffect*>(innerpe)));
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
