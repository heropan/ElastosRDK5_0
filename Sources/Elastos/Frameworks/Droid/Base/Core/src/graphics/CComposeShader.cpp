
#include "graphics/CComposeShader.h"
#include "graphics/Xfermode.h"
#include <skia/core/SkComposeShader.h>
#include <skia/effects/SkPorterDuff.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CComposeShader::constructor(
    /* [in] */ IShader* shaderA,
    /* [in] */ IShader* shaderB,
    /* [in] */ IXfermode* mode)
{
    mShaderA = shaderA;
    mShaderB = shaderB;
    mNativeInstance = NativeCreate1(
                ((Shader*)shaderA->Probe(EIID_Shader))->mNativeInstance,
                ((Shader*)shaderB->Probe(EIID_Shader))->mNativeInstance,
                (mode != NULL) ? ((Xfermode*)mode->Probe(EIID_Xfermode))->mNativeInstance : 0);
    if (IPorterDuffXfermode::Probe(mode) != NULL) {
        PorterDuffMode pdMode;
        IPorterDuffXfermode::Probe(mode)->GetMode(&pdMode);
        mNativeShader = NativePostCreate2(mNativeInstance,
                ((Shader*)shaderA->Probe(EIID_Shader))->mNativeInstance,
                ((Shader*)shaderB->Probe(EIID_Shader))->mNativeInstance, pdMode != -1 ? pdMode : 0);
        return NOERROR;
    }
    else {
        mNativeShader = NativePostCreate1(mNativeInstance,
                ((Shader*)shaderA->Probe(EIID_Shader))->mNativeInstance,
                ((Shader*)shaderB->Probe(EIID_Shader))->mNativeInstance,
                (mode != NULL) ? ((Xfermode*)mode->Probe(EIID_Xfermode))->mNativeInstance : 0);
        return NOERROR;
    }
}

ECode CComposeShader::constructor(
    /* [in] */ IShader* shaderA,
    /* [in] */ IShader* shaderB,
    /* [in] */ PorterDuffMode mode)
{
    mShaderA = shaderA;
    mShaderB = shaderB;
    mNativeInstance = NativeCreate2(
                ((Shader*)shaderA->Probe(EIID_Shader))->mNativeInstance,
                ((Shader*)shaderB->Probe(EIID_Shader))->mNativeInstance,
                mode);
    mNativeShader = NativePostCreate2(mNativeInstance,
            ((Shader*)shaderA->Probe(EIID_Shader))->mNativeInstance,
            ((Shader*)shaderB->Probe(EIID_Shader))->mNativeInstance,
            mode);
    return NOERROR;
}

PInterface CComposeShader::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_Shader) {
        return reinterpret_cast<PInterface>((Shader*)this);
    }
    return _CComposeShader::Probe(riid);
}

ECode CComposeShader::GetLocalMatrix(
    /* [in] */ IMatrix* localM,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = Shader::GetLocalMatrix(localM);
    return NOERROR;
}

ECode CComposeShader::SetLocalMatrix(
    /* [in] */ IMatrix* localM)
{
    return Shader::SetLocalMatrix(localM);
}

Int32 CComposeShader::NativeCreate1(
    /* [in] */ Int32 nativeShaderA,
    /* [in] */ Int32 nativeShaderB,
    /* [in] */ Int32 nativeMode)
{
    return (Int32)new SkComposeShader(
                (SkShader*)nativeShaderA,
                (SkShader*)nativeShaderB,
                (SkXfermode*)nativeMode);
}

Int32 CComposeShader::NativeCreate2(
    /* [in] */ Int32 nativeShaderA,
    /* [in] */ Int32 nativeShaderB,
    /* [in] */ Int32 porterDuffMode)
{
    SkAutoUnref au(SkPorterDuff::CreateXfermode((SkPorterDuff::Mode)porterDuffMode));
    return (Int32)new SkComposeShader(
                (SkShader*)nativeShaderA,
                (SkShader*)nativeShaderB,
                (SkXfermode*)au.get());
}

Int32 CComposeShader::NativePostCreate1(
    /* [in] */ Int32 nativeShader,
    /* [in] */ Int32 nativeSkiaShaderA,
    /* [in] */ Int32 nativeSkiaShaderB,
    /* [in] */ Int32 nativeMode)
{
#ifdef USE_OPENGL_RENDERER
    SkXfermode::Mode skiaMode;
    if (!SkXfermode::IsMode((SkXfermode*)nativeMode, &skiaMode)) {
        // TODO: Support other modes
        skiaMode = SkXfermode::kSrcOver_Mode;
    }
    return (Int32)new SkiaComposeShader((SkiaShader*)nativeSkiaShaderA,
            (SkiaShader*)nativeSkiaShaderB, skiaMode, (SkShader*)nativeShader);
#else
    return 0;
#endif
}

Int32 CComposeShader::NativePostCreate2(
    /* [in] */ Int32 nativeShader,
    /* [in] */ Int32 nativeSkiaShaderA,
    /* [in] */ Int32 nativeSkiaShaderB,
    /* [in] */ Int32 porterDuffMode)
{
#ifdef USE_OPENGL_RENDERER
    SkXfermode::Mode mode = SkPorterDuff::ToXfermodeMode((SkPorterDuff::Mode)porterDuffMode);
    return (Int32)new SkiaComposeShader((SkiaShader*)nativeSkiaShaderA,
            (SkiaShader*)nativeSkiaShaderB, mode, (SkShader*)nativeShader);
#else
    return 0;
#endif
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
