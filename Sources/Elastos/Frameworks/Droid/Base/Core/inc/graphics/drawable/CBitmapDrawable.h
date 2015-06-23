
#ifndef __CBITMAPDRAWABLE_H__
#define __CBITMAPDRAWABLE_H__

#include "_CBitmapDrawable.h"
#include "graphics/drawable/BitmapDrawable.h"


namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CBitmapDrawable), public BitmapDrawable
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IResources* res);

    CARAPI constructor(
        /* [in] */ IBitmap* bitmap);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bitmap);

    CARAPI constructor(
        /* [in] */ const String& filepath);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ const String& filepath);

    CARAPI constructor(
        /* [in] */ IInputStream* is);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ IInputStream* is);

    CARAPI constructor(
        /* [in] */ Handle32 state,
        /* [in] */ IResources* res);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IDRAWABLE_METHODS_DECL()

    CARAPI GetPaint(
        /* [out] */ IPaint** paint);

    CARAPI GetBitmap(
        /* [out] */ IBitmap** bitmap);

    CARAPI SetTargetDensity(
        /* [in] */ ICanvas* canvas);

    CARAPI SetTargetDensity(
        /* [in] */ IDisplayMetrics* metrics);

    CARAPI SetTargetDensity(
        /* [in] */ Int32 density);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetAntiAlias(
        /* [in] */ Boolean aa);

    CARAPI GetTileModeX(
        /* [out] */ ShaderTileMode* tileModeX);

    CARAPI GetTileModeY(
        /* [out] */ ShaderTileMode* tileModeY);

    CARAPI SetTileModeX(
        /* [in] */ ShaderTileMode mode);

    CARAPI SetTileModeY(
        /* [in] */ ShaderTileMode mode);

    CARAPI SetTileModeXY(
        /* [in] */ ShaderTileMode xmode,
        /* [in] */ ShaderTileMode ymode);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__CBITMAPDRAWABLE_H__
