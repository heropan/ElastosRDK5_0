
#include "graphics/CRadialGradient.h"
#include <skia/effects/SkGradientShader.h>
#include <skia/core/SkTemplates.h>
#include <hwui/SkiaShader.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CRadialGradient::constructor(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ ArrayOf<Float>* positions,
    /* [in] */ ShaderTileMode tile)
{
    if (radius <= 0) {
//        throw new IllegalArgumentException("radius must be > 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (colors.GetLength() < 2) {
//        throw new IllegalArgumentException("needs >= 2 number of colors");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (positions != NULL && colors.GetLength() != positions->GetLength()) {
//        throw new IllegalArgumentException("color and position arrays must be of equal length");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mNativeInstance = NativeCreate1(x, y, radius, colors, positions, tile);
    mNativeShader = NativePostCreate1(mNativeInstance, x, y, radius, colors, positions, tile);
    return NOERROR;
}

ECode CRadialGradient::constructor(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1,
    /* [in] */ ShaderTileMode tile)
{
    if (radius <= 0) {
//        throw new IllegalArgumentException("radius must be > 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mNativeInstance = NativeCreate2(x, y, radius, color0, color1, tile);
    mNativeShader = NativePostCreate2(mNativeInstance, x, y, radius, color0, color1, tile);
    return NOERROR;
}

PInterface CRadialGradient::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Shader) {
        return reinterpret_cast<PInterface>((Shader*)this);
    }
    return _CRadialGradient::Probe(riid);
}

ECode CRadialGradient::GetLocalMatrix(
    /* [in ,out] */ IMatrix* localM,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Shader::GetLocalMatrix(localM);
    return NOERROR;
}

ECode CRadialGradient::SetLocalMatrix(
    /* [in] */ IMatrix* localM)
{
    Shader::SetLocalMatrix(localM);
    return NOERROR;
}

Int32 CRadialGradient::NativeCreate1(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ ArrayOf<Float>* positions,
    /* [in] */ ShaderTileMode tile)
{
    SkPoint center;
    center.set(SkFloatToScalar(x), SkFloatToScalar(y));

    size_t count = (size_t)colors.GetLength();
    const Int32* colorValues = colors.GetPayload();

    SkAutoSTMalloc<8, SkScalar> storage(positions ? count : 0);
    SkScalar* pos = NULL;

    if (positions) {
        const Float* posValues = positions->GetPayload();
        pos = (SkScalar*)storage.get();
        for (size_t i = 0; i < count; i++)
            pos[i] = SkFloatToScalar(posValues[i]);
    }

    SkShader* shader = SkGradientShader::CreateRadial(center,
            SkFloatToScalar(radius),
            reinterpret_cast<const SkColor*>(colorValues),
            pos, count,
            static_cast<SkShader::TileMode>(tile));

    assert(shader != NULL);
    return (Int32)shader;
}

Int32 CRadialGradient::NativeCreate2(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1,
    /* [in] */ ShaderTileMode tile)
{
    SkPoint center;
    center.set(SkFloatToScalar(x), SkFloatToScalar(y));

    SkColor colors[2];
    colors[0] = color0;
    colors[1] = color1;

    SkShader* s = SkGradientShader::CreateRadial(center, SkFloatToScalar(radius), colors, NULL,
            2, (SkShader::TileMode)tile);
    assert(s != NULL);
    return (Int32)s;
}

Int32 CRadialGradient::NativePostCreate1(
    /* [in] */ Int32 nativeShader,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ ArrayOf<Float>* positions,
    /* [in] */ ShaderTileMode tileMode)
{
#ifdef USE_OPENGL_RENDERER
    Int32 count = colors.GetLength();
    const Int32* colorValues = colors.GetPayload();

    Float* storedPositions = new Float[count];
    UInt32* storedColors = new UInt32[count];
    for (size_t i = 0; i < count; i++) {
        storedColors[i] = static_cast<UInt32>(colorValues[i]);
    }

    if (positions) {
        assert(positions->GetLength() >= count);
        const float* posValues = positions->GetPayload();
        for (size_t i = 0; i < count; i++) {
            storedPositions[i] = posValues[i];
        }
    }
    else {
        storedPositions[0] = 0.0f;
        const Float step = 1.0f / (count - 1);
        for (size_t i = 1; i < count - 1; i++) {
            storedPositions[i] = step * i;
        }
        storedPositions[count - 1] = 1.0f;
    }

    return (Int32)new SkiaCircularGradientShader(x, y, radius, storedColors,
            storedPositions, count, (SkShader*)nativeShader, (SkShader::TileMode)tileMode,
            NULL, (nativeShader->getFlags() & SkShader::kOpaqueAlpha_Flag) == 0);
#else
    return 0;
#endif
}

Int32 CRadialGradient::NativePostCreate2(
    /* [in] */ Int32 nativeShader,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1,
    /* [in] */ ShaderTileMode tileMode)
{
#ifdef USE_OPENGL_RENDERER
    Float* storedPositions = new Float[2];
    storedPositions[0] = 0.0f;
    storedPositions[1] = 1.0f;

    UInt32* storedColors = new UInt32[2];
    storedColors[0] = static_cast<UInt32>(color0);
    storedColors[1] = static_cast<UInt32>(color1);

    return (Int32)new SkiaCircularGradientShader(x, y, radius, storedColors,
            storedPositions, 2, (SkShader*)nativeShader, (SkShader::TileMode) tileMode,
            NULL, (nativeShader->getFlags() & SkShader::kOpaqueAlpha_Flag) == 0);
#else
    return 0;
#endif
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
