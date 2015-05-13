
#include "graphics/CSweepGradient.h"
#include <skia/effects/SkGradientShader.h>
#include <skia/core/SkTemplates.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CSweepGradient::constructor(
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ ArrayOf<Float>* positions)
{
    if (colors.GetLength() < 2) {
//        throw new IllegalArgumentException("needs >= 2 number of colors");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (positions != NULL && colors.GetLength() != positions->GetLength()) {
//        throw new IllegalArgumentException(
//                    "color and position arrays must be of equal length");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mNativeInstance = NativeCreate1(cx, cy, colors, positions);
    mNativeShader = NativePostCreate1(mNativeInstance, cx, cy, colors, positions);
    return NOERROR;
}

ECode CSweepGradient::constructor(
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1)
{
    mNativeInstance = NativeCreate2(cx, cy, color0, color1);
    mNativeShader = NativePostCreate2(mNativeInstance, cx, cy, color0, color1);
    return NOERROR;
}

PInterface CSweepGradient::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Shader) {
        return reinterpret_cast<PInterface>((Shader*)this);
    }
    return _CSweepGradient::Probe(riid);
}

ECode CSweepGradient::GetLocalMatrix(
    /* [in ,out] */ IMatrix* localM,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Shader::GetLocalMatrix(localM);
    return NOERROR;
}

ECode CSweepGradient::SetLocalMatrix(
    /* [in] */ IMatrix* localM)
{
    return Shader::SetLocalMatrix(localM);
}

Int32 CSweepGradient::NativeCreate1(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ ArrayOf<Float>* positions)
{
    size_t count = (size_t)colors.GetLength();
    const Int32* colValues = colors.GetPayload();

    SkAutoSTMalloc<8, SkScalar> storage(positions ? count : 0);
    SkScalar* pos = NULL;

    if (NULL != positions) {
        const Float* posValues = positions->GetPayload();
        pos = (SkScalar*)storage.get();
        for (size_t i = 0; i < count; i++) {
            pos[i] = SkFloatToScalar(posValues[i]);
        }
    }

    SkShader* shader = SkGradientShader::CreateSweep(SkFloatToScalar(x),
            SkFloatToScalar(y),
            reinterpret_cast<const SkColor*>(colValues),
            pos, count);
    assert(shader != NULL);
    return (Int32)shader;
}

Int32 CSweepGradient::NativeCreate2(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1)
{
    SkColor colors[2];
    colors[0] = color0;
    colors[1] = color1;
    SkShader* s = SkGradientShader::CreateSweep(
            SkFloatToScalar(x), SkFloatToScalar(y), colors, NULL, 2);
    assert(s != NULL);
    return (Int32)s;
}

Int32 CSweepGradient::NativePostCreate1(
    /* [in] */ Int32 nativeShader,
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ ArrayOf<Float>* positions)
{
#ifdef USE_OPENGL_RENDERER
    size_t count = colors->GetLength();
    const Int32* colorValues = colors->GetPayload();

    Float* storedPositions = new Float[count];
    uint32_t* storedColors = new uint32_t[count];
    for (size_t i = 0; i < count; i++) {
        storedColors[i] = static_cast<uint32_t>(colorValues[i]);
    }

    if (posArray) {
        const Float* posValues = positions->GetPayload();
        for (size_t i = 0; i < count; i++) {
            storedPositions[i] = posValues[i];
        }
    } else {
        storedPositions[0] = 0.0f;
        const Float step = 1.0f / (count - 1);
        for (size_t i = 1; i < count - 1; i++) {
            storedPositions[i] = step * i;
        }
        storedPositions[count - 1] = 1.0f;
    }

    SkiaShader* skiaShader = new SkiaSweepGradientShader(x, y, storedColors, storedPositions, count,
            shader, NULL, (shader->getFlags() & SkShader::kOpaqueAlpha_Flag) == 0);

    return (Int32)skiaShader;
#else
    return 0;
#endif
}

Int32 CSweepGradient::NativePostCreate2(
    /* [in] */ Int32 nativeShader,
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1)
{
#ifdef USE_OPENGL_RENDERER
    float* storedPositions = new float[2];
    storedPositions[0] = 0.0f;
    storedPositions[1] = 1.0f;

    uint32_t* storedColors = new uint32_t[2];
    storedColors[0] = static_cast<uint32_t>(color0);
    storedColors[1] = static_cast<uint32_t>(color1);

    SkiaShader* skiaShader = new SkiaSweepGradientShader(x, y, storedColors, storedPositions, 2,
            shader, NULL, (shader->getFlags() & SkShader::kOpaqueAlpha_Flag) == 0);

    return (Int32)skiaShader;
#else
    return 0;
#endif
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
