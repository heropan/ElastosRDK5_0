
#include "ext/frameworkext.h"
#include "graphics/CTableMaskFilter.h"
#include <skia/effects/SkTableMaskFilter.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CTableMaskFilter::constructor(
    /* [in] */ const ArrayOf<Byte>& table)
{
    if (table.GetLength() < 256) {
        // throw new RuntimeException("table.length must be >= 256");
        return E_RUNTIME_EXCEPTION;
    }
    mNativeInstance = NativeNewTable(table);
    return NOERROR;
}

ECode CTableMaskFilter::constructor(
    /* [in] */ Int32 ni)
{
    mNativeInstance = ni;
    return NOERROR;
}

PInterface CTableMaskFilter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_MaskFilter) {
        return reinterpret_cast<PInterface>((MaskFilter*)this);
    }
    return _CTableMaskFilter::Probe(riid);
}

ECode CTableMaskFilter::CreateClipTable(
    /* [in] */ Int32 min,
    /* [in] */ Int32 max,
    /* [out] */ ITableMaskFilter** tf)
{
    VALIDATE_NOT_NULL(tf);

    return CTableMaskFilter::New(NativeNewClip(min, max), tf);
}

ECode CTableMaskFilter::CreateGammaTable(
    /* [in] */ Float gamma,
    /* [out] */ ITableMaskFilter** tf)
{
    VALIDATE_NOT_NULL(tf);

    return CTableMaskFilter::New(NativeNewGamma(gamma), tf);
}

Int32 CTableMaskFilter::NativeNewTable(
    /* [in] */ const ArrayOf<Byte>& table)
{
    return (Int32)new SkTableMaskFilter((const uint8_t*)table.GetPayload());
}

Int32 CTableMaskFilter::NativeNewClip(
    /* [in] */ Int32 min,
    /* [in] */ Int32 max)
{
    return (Int32)SkTableMaskFilter::CreateClip(min, max);
}

Int32 CTableMaskFilter::NativeNewGamma(
    /* [in] */ Float gamma)
{
    return (Int32)SkTableMaskFilter::CreateGamma(gamma);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
