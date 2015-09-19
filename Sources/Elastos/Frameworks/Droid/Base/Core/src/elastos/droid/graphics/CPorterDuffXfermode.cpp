
#include "ext/frameworkext.h"
#include "graphics/CPorterDuffXfermode.h"
#include <skia/effects/SkPorterDuff.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CPorterDuffXfermode::constructor(
    /* [in] */ PorterDuffMode mode)
{
    this->mMode = mode;
    mNativeInstance = NativeCreateXfermode(mode);
    return NOERROR;
}

PInterface CPorterDuffXfermode::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Xfermode) {
        return reinterpret_cast<PInterface>((Xfermode*)this);
    }
    return _CPorterDuffXfermode::Probe(riid);
}

ECode CPorterDuffXfermode::GetMode(
    /* [out] */ PorterDuffMode* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = mMode;
    return NOERROR;
}

Int32 CPorterDuffXfermode::NativeCreateXfermode(
    /* [in] */ PorterDuffMode mode)
{
    return (Int32)SkPorterDuff::CreateXfermode((SkPorterDuff::Mode)mode);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
