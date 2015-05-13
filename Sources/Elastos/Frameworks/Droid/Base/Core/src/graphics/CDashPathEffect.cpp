
#include "graphics/CDashPathEffect.h"
#include <SkDashPathEffect.h>
#include <SkTemplates.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CDashPathEffect::constructor(
    /* [in] */ const ArrayOf<Float>& intervals,
    /* [in] */ Float phase)
{
    if (intervals.GetLength() < 2) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    mNativeInstance = NativeCreate(intervals, phase);
    return NOERROR;
}

PInterface CDashPathEffect::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_PathEffect) {
        return reinterpret_cast<PInterface>((PathEffect*)this);
    }
    return _CDashPathEffect::Probe(riid);
}

Int32 CDashPathEffect::NativeCreate(
    /* [in] */ const ArrayOf<Float>& intervals,
    /* [in] */ Float phase)
{
    Int32 count = intervals.GetLength() & ~1;  // even number
    Float* values = intervals.GetPayload();

    SkAutoSTMalloc<32, SkScalar> storage(count);
    SkScalar* nativeIntervals = storage.get();
    for (int i = 0; i < count; i++) {
        nativeIntervals[i] = SkFloatToScalar(values[i]);
    }
    return reinterpret_cast<Int32>(new SkDashPathEffect(
            nativeIntervals, count, SkFloatToScalar(phase)));
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
