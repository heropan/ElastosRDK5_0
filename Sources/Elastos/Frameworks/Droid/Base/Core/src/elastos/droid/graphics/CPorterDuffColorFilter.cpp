
#include "graphics/CPorterDuffColorFilter.h"
#include <skia/core/SkColorFilter.h>
#include <skia/effects/SkPorterDuff.h>
#include <hwui/SkiaColorFilter.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

PInterface CPorterDuffColorFilter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ColorFilter) {
        return reinterpret_cast<PInterface>((ColorFilter*)this);
    }
    return _CPorterDuffColorFilter::Probe(riid);
}

ECode CPorterDuffColorFilter::constructor(
    /* [in] */ Int32 srcColor,
    /* [in] */ PorterDuffMode mode)
{
    mNativeInstance = NativeCreatePorterDuffFilter(srcColor, mode);
    mNativeColorFilter = NCreatePorterDuffFilter(mNativeInstance, srcColor, mode);
    return NOERROR;
}

Int32 CPorterDuffColorFilter::NativeCreatePorterDuffFilter(
    /* [in] */ Int32 srcColor,
    /* [in] */ PorterDuffMode mode)
{
    return (Int32)SkColorFilter::CreateModeFilter(srcColor,
            SkPorterDuff::ToXfermodeMode((SkPorterDuff::Mode)mode));
}

Int32 CPorterDuffColorFilter::NCreatePorterDuffFilter(
    /* [in] */ Int32 nativeFilter,
    /* [in] */ Int32 srcColor,
    /* [in] */ PorterDuffMode mode)
{
    SkColorFilter* skFilter = (SkColorFilter*)nativeFilter;
    return (Int32)new android::uirenderer::SkiaBlendFilter(skFilter, srcColor,
            SkPorterDuff::ToXfermodeMode((SkPorterDuff::Mode)mode));
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
