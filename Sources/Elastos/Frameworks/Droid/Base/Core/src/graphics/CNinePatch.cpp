
#include "ext/frameworkext.h"
#include "graphics/CNinePatch.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CNinePatch::constructor(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ const String& srcName)
{
    return NinePatch::Init(bitmap, chunk, srcName);
}

ECode CNinePatch::constructor(
    /* [in] */ INinePatch* patch)
{
    return NinePatch::Init((NinePatch*)(CNinePatch*)patch);
}

ECode CNinePatch::SetPaint(
    /* [in] */ IPaint* p)
{
    return NinePatch::SetPaint(p);
}

ECode CNinePatch::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRectF* location)
{
    return NinePatch::Draw(canvas, location);
}

ECode CNinePatch::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* location)
{
    return NinePatch::Draw(canvas, location);
}

ECode CNinePatch::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* location,
    /* [in] */ IPaint* paint)
{
    return NinePatch::Draw(canvas, location, paint);
}

ECode CNinePatch::GetDensity(
    /* [out] */ Int32* density)
{
    VALIDATE_NOT_NULL(density);
    return NinePatch::GetDensity(density);
}

ECode CNinePatch::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    return NinePatch::GetWidth(width);
}

ECode CNinePatch::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    return NinePatch::GetHeight(height);;
}

ECode CNinePatch::HasAlpha(
    /* [out] */ Boolean* hasAlpha)
{
    VALIDATE_NOT_NULL(hasAlpha);
    return NinePatch::HasAlpha(hasAlpha);
}

ECode CNinePatch::GetTransparentRegion(
    /* [in] */ IRect* location,
    /* [out] */ IRegion** region)
{
    VALIDATE_NOT_NULL(region);
    return NinePatch::GetTransparentRegion(location, region);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
