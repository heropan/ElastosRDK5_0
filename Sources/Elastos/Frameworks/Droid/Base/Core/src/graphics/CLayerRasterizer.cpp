
#include "graphics/CLayerRasterizer.h"
#include "graphics/Paint.h"
#include <skia/effects/SkLayerRasterizer.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CLayerRasterizer::constructor()
{
    mNativeInstance = NativeConstructor();
    return NOERROR;
}

PInterface CLayerRasterizer::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_Rasterizer) {
        return reinterpret_cast<PInterface>((Rasterizer*)this);
    }
    return _CLayerRasterizer::Probe(riid);
}

ECode CLayerRasterizer::AddLayer(
    /* [in] */ IPaint* paint,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    NativeAddLayer(mNativeInstance, ((Paint*)paint->Probe(EIID_Paint))->mNativePaint, dx, dy);
    return NOERROR;
}

ECode CLayerRasterizer::AddLayerEx(
    /* [in] */ IPaint* paint)
{
    NativeAddLayer(mNativeInstance, ((Paint*)paint->Probe(EIID_Paint))->mNativePaint, 0, 0);
    return NOERROR;
}

Int32 CLayerRasterizer::NativeConstructor()
{
    return reinterpret_cast<Int32>(new SkLayerRasterizer());
}

void CLayerRasterizer::NativeAddLayer(
    /* [in] */ Int32 nativeLayer,
    /* [in] */ Int32 nativePaint,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkLayerRasterizer* skLayer = reinterpret_cast<SkLayerRasterizer*>(nativeLayer);
    SkPaint* skPaint = reinterpret_cast<SkPaint*>(nativePaint);
    SkASSERT(skLayer);
    SkASSERT(skPaint);
    skLayer->addLayer(*skPaint, SkFloatToScalar(dx), SkFloatToScalar(dy));
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
