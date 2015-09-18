
#ifndef __ELASTOS_DROID_GRAPHICS_CCOMPOSESHADER_H__
#define __ELASTOS_DROID_GRAPHICS_CCOMPOSESHADER_H__

#include "_Elastos_Droid_Graphics_CComposeShader.h"
#include "graphics/Shader.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CComposeShader)
    , public Shader
    , public IComposeShader
{
public:
    CAR_OBJECT_DECL();

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

    /**
     * Return true if the shader has a non-identity local matrix.
     * @param localM If not null, it is set to the shader's local matrix.
     * @return true if the shader has a non-identity local matrix
     */
    CARAPI GetLocalMatrix(
        /* [in] */ IMatrix* localM,
        /* [out] */ Boolean* result);

    /**
     * Set the shader's local matrix. Passing null will reset the shader's
     * matrix to identity
     * @param localM The shader's new local matrix, or null to specify identity
     */
    CARAPI SetLocalMatrix(
        /* [in] */ IMatrix* localM);

    /**
     * Create a new compose shader, given shaders A, B, and a combining mode.
     * When the mode is applied, it will be given the result from shader A as its
     * "dst", and the result from shader B as its "src".
     *
     * @param shaderA  The colors from this shader are seen as the "dst" by the mode
     * @param shaderB  The colors from this shader are seen as the "src" by the mode
     * @param mode     The mode that combines the colors from the two shaders. If mode
     *                 is null, then SRC_OVER is assumed.
     */
    CARAPI constructor(
        /* [in] */ IShader* shaderA,
        /* [in] */ IShader* shaderB,
        /* [in] */ IXfermode* mode);

    /**
     * Create a new compose shader, given shaders A, B, and a combining PorterDuff mode.
     * When the mode is applied, it will be given the result from shader A as its
     * "dst", and the result from shader B as its "src".
     *
     * @param shaderA  The colors from this shader are seen as the "dst" by the mode
     * @param shaderB  The colors from this shader are seen as the "src" by the mode
     * @param mode     The PorterDuff mode that combines the colors from the two shaders.
     */
    CARAPI constructor(
        /* [in] */ IShader* shaderA,
        /* [in] */ IShader* shaderB,
        /* [in] */ PorterDuffMode mode);

private:
    static CARAPI_(Int32) NativeCreate1(
        /* [in] */ Int32 nativeShaderA,
        /* [in] */ Int32 nativeShaderB,
        /* [in] */ Int32 nativeMode);

    static CARAPI_(Int32) NativeCreate2(
        /* [in] */ Int32 nativeShaderA,
        /* [in] */ Int32 nativeShaderB,
        /* [in] */ Int32 porterDuffMode);

    static CARAPI_(Int32) NativePostCreate1(
        /* [in] */ Int32 nativeShader,
        /* [in] */ Int32 nativeSkiaShaderA,
        /* [in] */ Int32 nativeSkiaShaderB,
        /* [in] */ Int32 nativeMode);

    static CARAPI_(Int32) NativePostCreate2(
        /* [in] */ Int32 nativeShader,
        /* [in] */ Int32 nativeSkiaShaderA,
        /* [in] */ Int32 nativeSkiaShaderB,
        /* [in] */ Int32 porterDuffMode);

private:
    /**
     * Hold onto the shaders to avoid GC.
     */
    AutoPtr<IShader> mShaderA;

    AutoPtr<IShader> mShaderB;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_CCOMPOSESHADER_H__
