
#include "ext/frameworkext.h"
#include "graphics/CBitmapShader.h"
#include "graphics/CBitmap.h"
#include <skia/core/SkShader.h>
#include <skia/core/SkBitmap.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

CAR_OBJECT_IMPL(CBitmapShader);
CAR_INTERFACE_IMPL(CBitmapShader, Shader, IBitmapShader);
ECode CBitmapShader::constructor(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ShaderTileMode tileX,
    /* [in] */ ShaderTileMode tileY)
{
    mBitmap = bitmap;
    Int32 b = ((CBitmap*)bitmap)->Ni();
    mNativeInstance = NativeCreate(b, tileX, tileY);
    mNativeShader = NativePostCreate(mNativeInstance, b, tileX, tileY);
    return NOERROR;
}

PInterface CBitmapShader::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Shader) {
        return reinterpret_cast<PInterface>((Shader*)this);
    }
    return _CBitmapShader::Probe(riid);
}

ECode CBitmapShader::GetLocalMatrix(
    /* [in ,out] */ IMatrix* localM,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Shader::GetLocalMatrix(localM);
    return NOERROR;
}

ECode CBitmapShader::SetLocalMatrix(
    /* [in] */ IMatrix* localM)
{
    Shader::SetLocalMatrix(localM);
    return NOERROR;
}

Int32 CBitmapShader::NativeCreate(
    /* [in] */ Int32 nBitmap,
    /* [in] */ Int32 tileModeX,
    /* [in] */ Int32 tileModeY)
{
    SkShader* s = SkShader::CreateBitmapShader(*(SkBitmap*)nBitmap,
            (SkShader::TileMode)tileModeX,
            (SkShader::TileMode)tileModeY);
    return (Int32)s;
}

Int32 CBitmapShader::NativePostCreate(
        /* [in] */ Int32 shader,
        /* [in] */ Int32 bitmap,
        /* [in] */ Int32 shaderTileModeX,
        /* [in] */ Int32 shaderTileModeY)
{
#ifdef USE_OPENGL_RENDERER
    SkiaShader* skiaShader = new SkiaBitmapShader((SkBitmap*)bitmap, (SkShader*)shader,
            static_cast<SkShader::TileMode>(tileModeX), static_cast<SkShader::TileMode>(tileModeY),
            NULL, (((SkShader*)shader)->getFlags() & SkShader::kOpaqueAlpha_Flag) == 0);
    return (Int32)skiaShader;
#else
    return 0;
#endif
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos