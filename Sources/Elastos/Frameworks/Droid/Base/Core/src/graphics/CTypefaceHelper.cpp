
#include "ext/frameworkext.h"
#include "graphics/CTypefaceHelper.h"
#include "graphics/CTypeface.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CTypefaceHelper::Create(
    /* [in] */ const String& familyName,
    /* [in] */ Int32 style,
    /* [out] */ ITypeface** typeface)
{
    VALIDATE_NOT_NULL(typeface);
    return Typeface::Create(familyName, style, typeface);
}

ECode CTypefaceHelper::Create(
    /* [in] */ ITypeface* family,
    /* [in] */ Int32 style,
    /* [out] */ ITypeface** typeface)
{
    VALIDATE_NOT_NULL(typeface);
    return Typeface::Create(family, style, typeface);
}

ECode CTypefaceHelper::DefaultFromStyle(
    /* [in] */ Int32 style,
    /* [out] */ ITypeface** typeface)
{
    VALIDATE_NOT_NULL(typeface);
    return Typeface::DefaultFromStyle(style, typeface);
}

ECode CTypefaceHelper::CreateFromAsset(
    /* [in] */ IAssetManager* mgr,
    /* [in] */ const String& path,
    /* [out] */ ITypeface** typeface)
{
    VALIDATE_NOT_NULL(typeface);
    return Typeface::CreateFromAsset(mgr, path, typeface);
}

ECode CTypefaceHelper::CreateFromFile(
    /* [in] */ IFile* path,
    /* [out] */ ITypeface** typeface)
{
    VALIDATE_NOT_NULL(typeface);
    return Typeface::CreateFromFile(path, typeface);
}

ECode CTypefaceHelper::CreateFromFile(
    /* [in]*/ const String& path,
    /* [out] */ ITypeface** typeface)
{
    VALIDATE_NOT_NULL(typeface);
    return Typeface::CreateFromFile(path, typeface);
}

ECode CTypefaceHelper::SetGammaForText(
    /* [in] */ Float blackGamma,
    /* [in] */ Float whiteGamma)
{
    Typeface::SetGammaForText(blackGamma, whiteGamma);
    return NOERROR;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
