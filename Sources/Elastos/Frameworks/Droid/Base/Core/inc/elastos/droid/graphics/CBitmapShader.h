
#ifndef __ELASTOS_DROID_GRAPHICS_CBITMAPSHADER_H__
#define __ELASTOS_DROID_GRAPHICS_CBITMAPSHADER_H__

#include "_Elastos_Droid_Graphics_CBitmapShader.h"
#include "graphics/Shader.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CBitmapShader)
    , public Shader
    , public IBitmapShader
{
public:
    CAR_INTERFACE_DECL();

    CAR_OBJECT_DECL();

    CARAPI constructor(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ ShaderTileMode tileX,
        /* [in] */ ShaderTileMode tileY);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Return true if the shader has a non-identity local matrix.
     * @param localM If not null, it is set to the shader's local matrix.
     * @return true if the shader has a non-identity local matrix
     */
    CARAPI GetLocalMatrix(
        /* [in ,out] */ IMatrix* localM,
        /* [out] */ Boolean* result);

    /**
     * Set the shader's local matrix. Passing null will reset the shader's
     * matrix to identity
     * @param localM The shader's new local matrix, or null to specify identity
     */
    CARAPI SetLocalMatrix(
        /* [in] */ IMatrix* localM);

private:
    static CARAPI_(Int32) NativeCreate(
        /* [in] */ Int32 nBitmap,
        /* [in] */ Int32 tileModeX,
        /* [in] */ Int32 tileModeY);

    static CARAPI_(Int32) NativePostCreate(
        /* [in] */ Int32 native_shader,
        /* [in] */ Int32 native_bitmap,
        /* [in] */ Int32 shaderTileModeX,
        /* [in] */ Int32 shaderTileModeY);

public:
    /**
     * Prevent garbage collection.
     * @hide
     */
    AutoPtr<IBitmap> mBitmap;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_CBITMAPSHADER_H__
