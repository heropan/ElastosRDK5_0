
#include "graphics/CLinearGradient.h"
#include <skia/effects/SkGradientShader.h>
#include <skia/core/SkTemplates.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CLinearGradient::constructor(
    /* [in] */ Float x0,
    /* [in] */ Float y0,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ const ArrayOf<Float>* positions,
    /* [in] */ ShaderTileMode tile)
{
    if (colors.GetLength() < 2) {
//        throw new IllegalArgumentException("needs >= 2 number of colors");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (positions != NULL && colors.GetLength() != positions->GetLength()) {
//        throw new IllegalArgumentException("color and position arrays must be of equal length");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mNativeInstance = NativeCreate1(x0, y0, x1, y1, colors, positions, tile);
    mNativeShader = NativePostCreate1(mNativeInstance, x0, y0, x1, y1, colors, positions, tile);
    return NOERROR;
}

ECode CLinearGradient::constructor(
    /* [in] */ Float x0,
    /* [in] */ Float y0,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1,
    /* [in] */ ShaderTileMode tile)
{
    mNativeInstance = NativeCreate2(x0, y0, x1, y1, color0, color1, tile);
    mNativeShader = NativePostCreate2(mNativeInstance, x0, y0, x1, y1, color0, color1, tile);
    return NOERROR;
}

PInterface CLinearGradient::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Shader) {
        return reinterpret_cast<PInterface>((Shader*)this);
    }
    return _CLinearGradient::Probe(riid);
}

ECode CLinearGradient::GetLocalMatrix(
    /* [in, out] */ IMatrix* localM,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Shader::GetLocalMatrix(localM);
    return NOERROR;
}

ECode CLinearGradient::SetLocalMatrix(
    /* [in] */ IMatrix* localM)
{
    Shader::SetLocalMatrix(localM);
    return NOERROR;
}

Int32 CLinearGradient::NativeCreate1(
    /* [in] */ Float x0,
    /* [in] */ Float y0,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ const ArrayOf<Float>* positions,
    /* [in] */ ShaderTileMode tileMode)
{
    SkPoint pts[2];
    pts[0].set(SkFloatToScalar(x0), SkFloatToScalar(y0));
    pts[1].set(SkFloatToScalar(x1), SkFloatToScalar(y1));

    size_t count = colors.GetLength();

    SkAutoSTMalloc<8, SkScalar> storage(positions ? count : 0);
    SkScalar* pos = NULL;

    if (positions) {
        assert((size_t)positions->GetLength() >= count);
        const float* posValues = positions->GetPayload();
        pos = (SkScalar*)storage.get();
        for (size_t i = 0; i < count; i++) {
            pos[i] = SkFloatToScalar(posValues[i]);
        }
    }

    SkShader* shader = SkGradientShader::CreateLinear(pts,
            reinterpret_cast<const SkColor*>(colors.GetPayload()),
            pos, count,
            static_cast<SkShader::TileMode>(tileMode));

    assert(shader != NULL);
    return (Int32)shader;
}

Int32 CLinearGradient::NativeCreate2(
    /* [in] */ Float x0,
    /* [in] */ Float y0,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1,
    /* [in] */ ShaderTileMode tileMode)
{
    SkPoint pts[2];
    pts[0].set(SkFloatToScalar(x0), SkFloatToScalar(y0));
    pts[1].set(SkFloatToScalar(x1), SkFloatToScalar(y1));

    SkColor colors[2];
    colors[0] = color0;
    colors[1] = color1;

    SkShader* shader = SkGradientShader::CreateLinear(pts, colors, NULL, 2,
            (SkShader::TileMode)tileMode);

    assert(shader != NULL);
    return (Int32)shader;
}

Int32 CLinearGradient::NativePostCreate1(
    /* [in] */ Int32 shader,
    /* [in] */ Float x0,
    /* [in] */ Float y0,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ const ArrayOf<Float>* positions,
    /* [in] */ ShaderTileMode tileMode)
{
#ifdef USE_OPENGL_RENDERER
    size_t count = colors.GetLength();
    const Int32* colorValues = colors.GetPayload();

    Float* storedBounds = new Float[4];
    storedBounds[0] = x0; storedBounds[1] = y0;
    storedBounds[2] = x1; storedBounds[3] = y1;

    bool missFirst = false;
    bool missLast = false;
    size_t stopCount = count;

    Float* storedPositions = NULL;
    if (positions) {
        const Float* posValues = positions->GetPayload();

        missFirst = posValues[0] != 0.0f;
        missLast = posValues[count - 1] != 1.0f;

        stopCount += missFirst + missLast;
        storedPositions = new Float[stopCount];

        if (missFirst) {
            storedPositions[0] = 0.0f;
        }

        for (size_t i = missFirst; i < count + missFirst; i++) {
            storedPositions[i] = posValues[i - missFirst];
        }

        if (missLast) {
            storedPositions[stopCount - 1] = 1.0f;
        }
    }
    else {
        storedPositions = new Float[count];
        storedPositions[0] = 0.0f;
        const Float step = 1.0f / (count - 1);
        for (size_t i = 1; i < count - 1; i++) {
            storedPositions[i] = step * i;
        }
        storedPositions[count - 1] = 1.0f;
    }

    uint32_t* storedColors = new uint32_t[stopCount];

    if (missFirst) {
        storedColors[0] = static_cast<uint32_t>(colorValues[0]);
    }

    for (size_t i = missFirst; i < count + missFirst; i++) {
        storedColors[i] = static_cast<uint32_t>(colorValues[i - missFirst]);
    }

    if (missLast) {
        storedColors[stopCount - 1] = static_cast<uint32_t>(colorValues[count - 1]);
    }

    SkiaShader* skiaShader = new SkiaLinearGradientShader(storedBounds, storedColors,
            storedPositions, stopCount, (SkShader*)shader, static_cast<SkShader::TileMode>(tileMode), NULL,
            (((SkShader*)shader)->getFlags() & SkShader::kOpaqueAlpha_Flag) == 0);

    return (Int32)skiaShader;
#else
    return 0;
#endif
}

Int32 CLinearGradient::NativePostCreate2(
    /* [in] */ Int32 shader,
    /* [in] */ Float x0,
    /* [in] */ Float y0,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1,
    /* [in] */ ShaderTileMode tileMode)
{
#ifdef USE_OPENGL_RENDERER
    float* storedBounds = new float[4];
    storedBounds[0] = x0; storedBounds[1] = y0;
    storedBounds[2] = x1; storedBounds[3] = y1;

    float* storedPositions = new float[2];
    storedPositions[0] = 0.0f;
    storedPositions[1] = 1.0f;

    uint32_t* storedColors = new uint32_t[2];
    storedColors[0] = static_cast<uint32_t>(color0);
    storedColors[1] = static_cast<uint32_t>(color1);

    SkiaShader* skiaShader = new SkiaLinearGradientShader(storedBounds, storedColors,
            storedPositions, 2, (SkShader*)shader, static_cast<SkShader::TileMode>(tileMode), NULL,
            (((SkShader*)shader)->getFlags() & SkShader::kOpaqueAlpha_Flag) == 0);

    return (Int32)skiaShader;
#else
    return 0;
#endif
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
