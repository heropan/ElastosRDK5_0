
#include "graphics/CSumPathEffect.h"
#include <SkPathEffect.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CSumPathEffect::constructor(
    /* [in] */ IPathEffect* first,
    /* [in] */ IPathEffect* second)
{
    mNativeInstance = NativeCreate((
            (PathEffect*)first->Probe(EIID_PathEffect))->mNativeInstance,
            ((PathEffect*)second->Probe(EIID_PathEffect))->mNativeInstance);
    return NOERROR;
}

PInterface CSumPathEffect::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_PathEffect) {
        return reinterpret_cast<PInterface>((PathEffect*)this);
    }
    return _CSumPathEffect::Probe(riid);
}

Int32 CSumPathEffect::NativeCreate(
    /* [in] */ Int32 first,
    /* [in] */ Int32 second)
{
    SkPathEffect* skFirst = reinterpret_cast<SkPathEffect*>(first);
    SkPathEffect* skSecond = reinterpret_cast<SkPathEffect*>(second);
    return reinterpret_cast<Int32>(new SkSumPathEffect(skFirst, skSecond));
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
