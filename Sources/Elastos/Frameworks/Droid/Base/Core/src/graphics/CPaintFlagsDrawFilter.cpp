
#include "graphics/CPaintFlagsDrawFilter.h"
#include <skia/effects/SkPaintFlagsDrawFilter.h>
#include <skia/core/SkPaint.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CPaintFlagsDrawFilter::constructor(
    /* [in] */ Int32 clearBits,
    /* [in] */ Int32 setBits)
{
    mClearBits = clearBits;
    mSetBits = setBits;
    mNativeInstance = NativeConstructor(clearBits, setBits);
    return NOERROR;
}

PInterface CPaintFlagsDrawFilter::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_DrawFilter) {
        return reinterpret_cast<PInterface>((DrawFilter*)this);
    }
    return _CPaintFlagsDrawFilter::Probe(riid);
}

Int32 CPaintFlagsDrawFilter::NativeConstructor(
    /* [in] */ Int32 clearBits,
    /* [in] */ Int32 setBits)
{
    // trim off any out-of-range bits
    clearBits &= SkPaint::kAllFlags;
    setBits &= SkPaint::kAllFlags;

    if (clearBits | setBits) {
        return (Int32)new SkPaintFlagsDrawFilter(clearBits, setBits);
    }
    else {
        return 0;
    }
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
