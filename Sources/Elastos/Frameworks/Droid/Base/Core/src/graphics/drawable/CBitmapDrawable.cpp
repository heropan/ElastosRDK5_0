
#include "graphics/drawable/CBitmapDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

IDRAWABLE_METHODS_IMPL(CBitmapDrawable, BitmapDrawable)

ECode CBitmapDrawable::constructor()
{
    return BitmapDrawable::Init();
}

ECode CBitmapDrawable::constructor(
        /* [in] */ IResources* res)
{
    return BitmapDrawable::Init(res);
}

ECode CBitmapDrawable::constructor(
        /* [in] */ IBitmap* bitmap)
{
    return BitmapDrawable::Init(bitmap);
}

ECode CBitmapDrawable::constructor(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap)
{
    return BitmapDrawable::Init(res, bitmap);
}

ECode CBitmapDrawable::constructor(
    /* [in] */ const String& filepath)
{
    return BitmapDrawable::Init(filepath);
}

ECode CBitmapDrawable::constructor(
    /* [in] */ IResources* res,
    /* [in] */ const String& filepath)
{
    return BitmapDrawable::Init(res, filepath);
}

ECode CBitmapDrawable::constructor(
    /* [in] */ IInputStream* is)
{
    return BitmapDrawable::Init(is);
}

ECode CBitmapDrawable::constructor(
    /* [in] */ IResources* res,
    /* [in] */ IInputStream* is)
{
    return BitmapDrawable::Init(res, is);
}

ECode CBitmapDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return BitmapDrawable::Init((BitmapState*)state, res);
}

PInterface CBitmapDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CBitmapDrawable::Probe(riid);
}

ECode CBitmapDrawable::GetPaint(
    /* [out] */ IPaint** paint)
{
    VALIDATE_NOT_NULL(paint);
    AutoPtr<IPaint> temp = BitmapDrawable::GetPaint();
    *paint = temp;
    REFCOUNT_ADD(*paint);

    return NOERROR;
}

ECode CBitmapDrawable::GetBitmap(
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    AutoPtr<IBitmap> temp = BitmapDrawable::GetBitmap();
    *bitmap = temp;
    REFCOUNT_ADD(*bitmap);

    return NOERROR;
}

ECode CBitmapDrawable::SetTargetDensity(
    /* [in] */ ICanvas* canvas)
{
    return BitmapDrawable::SetTargetDensity(canvas);
}

ECode CBitmapDrawable::SetTargetDensity(
    /* [in] */ IDisplayMetrics* metrics)
{
    return BitmapDrawable::SetTargetDensity(metrics);
}

ECode CBitmapDrawable::SetTargetDensity(
    /* [in] */ Int32 density)
{
    return BitmapDrawable::SetTargetDensity(density);
}

ECode CBitmapDrawable::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);
    *gravity = BitmapDrawable::GetGravity();

    return NOERROR;
}

ECode CBitmapDrawable::SetGravity(
    /* [in] */ Int32 gravity)
{
    return BitmapDrawable::SetGravity(gravity);
}

ECode CBitmapDrawable::SetAntiAlias(
    /* [in] */ Boolean aa)
{
    return BitmapDrawable::SetAntiAlias(aa);
}

ECode CBitmapDrawable::GetTileModeX(
    /* [out] */ ShaderTileMode* tileModeX)
{
    VALIDATE_NOT_NULL(tileModeX);
    *tileModeX = BitmapDrawable::GetTileModeX();

    return NOERROR;
}

ECode CBitmapDrawable::GetTileModeY(
    /* [out] */ ShaderTileMode* tileModeY)
{
    VALIDATE_NOT_NULL(tileModeY);
    *tileModeY = BitmapDrawable::GetTileModeY();

    return NOERROR;
}

ECode CBitmapDrawable::SetTileModeX(
    /* [in] */ ShaderTileMode mode)
{
    return BitmapDrawable::SetTileModeX(mode);
}

ECode CBitmapDrawable::SetTileModeY(
    /* [in] */ ShaderTileMode mode)
{
    return BitmapDrawable::SetTileModeY(mode);
}

ECode CBitmapDrawable::SetTileModeXY(
    /* [in] */ ShaderTileMode xmode,
    /* [in] */ ShaderTileMode ymode)
{
    return BitmapDrawable::SetTileModeXY(xmode, ymode);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
