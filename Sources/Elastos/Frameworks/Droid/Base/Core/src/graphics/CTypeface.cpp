
#include "ext/frameworkext.h"
#include "graphics/CTypeface.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CTypeface::constructor(
    /* [in] */ Int32 ni)
{
    return Typeface::Init(ni);
}

PInterface CTypeface::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Typeface) {
        return reinterpret_cast<PInterface>((Typeface*)this);
    }
    return _CTypeface::Probe(riid);
}

ECode CTypeface::GetStyle(
    /* [out] */ Int32* style)
{
    VALIDATE_NOT_NULL(style);
    *style = Typeface::GetStyle();
    return NOERROR;
}

ECode CTypeface::IsBold(
    /* [out] */ Boolean* isBold)
{
    VALIDATE_NOT_NULL(isBold);
    *isBold = Typeface::IsBold();
    return NOERROR;
}

ECode CTypeface::IsItalic(
    /* [out] */ Boolean* isItalic)
{
    VALIDATE_NOT_NULL(isItalic);
    *isItalic = Typeface::IsItalic();
    return NOERROR;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
