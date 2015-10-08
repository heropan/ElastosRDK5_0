
#include "graphics/CLayerRasterizer.h"
#include "graphics/Paint.h"
#include <skia/effects/SkLayerRasterizer.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

CAR_OBJECT_IMPL(CLayerRasterizer);
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
    else if (riid == EIID_ILayerRasterizer) {
        return (ILayerRasterizer*)this;
    }
    return Rasterizer::Probe(riid);
}

UInt32 CLayerRasterizer::AddRef()
{
    return Rasterizer::AddRef();
}

UInt32 CLayerRasterizer::Release()
{
    return Rasterizer::Release();
}

ECode CLayerRasterizer::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    return Rasterizer::GetInterfaceID(object, iid);
}

ECode CLayerRasterizer::AddLayer(
    /* [in] */ IPaint* paint,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    NativeAddLayer(mNativeInstance, ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint, dx, dy);
    return NOERROR;
}

ECode CLayerRasterizer::AddLayer(
    /* [in] */ IPaint* paint)
{
    NativeAddLayer(mNativeInstance, ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint, 0, 0);
    return NOERROR;
}

Int64 CLayerRasterizer::NativeConstructor()
{
    assert(0 && "TODO");
    // return reinterpret_cast<jlong>(new NativeLayerRasterizer);
    return 0;
}

void CLayerRasterizer::NativeAddLayer(
    /* [in] */ Int64 nativeLayer,
    /* [in] */ Int64 nativePaint,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    assert(0 && "TODO");
    // NativeLayerRasterizer* nr = reinterpret_cast<NativeLayerRasterizer *>(layerHandle);
    // const Paint* paint = reinterpret_cast<Paint *>(paintHandle);
    // SkASSERT(nr);
    // SkASSERT(paint);
    // nr->fBuilder.addLayer(*paint, dx, dy);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
