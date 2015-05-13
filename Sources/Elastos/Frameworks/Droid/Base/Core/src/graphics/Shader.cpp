
#include "graphics/Shader.h"
#include "graphics/CMatrix.h"
#include <skia/core/SkShader.h>
#include <hwui/SkiaShader.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

// {AE795BA0-60FF-4BED-ADF2-9BDE205AB72F}
extern const InterfaceID EIID_Shader =
    { 0xae795ba0, 0x60ff, 0x4bed, { 0xad, 0xf2, 0x9b, 0xde, 0x20, 0x5a, 0xb7, 0x2f } };

Shader::~Shader()
{
    NativeDestructor(mNativeInstance, mNativeShader);
}

Boolean Shader::GetLocalMatrix(
    /* [in, out] */ IMatrix* localM)
{
    if (mLocalMatrix != NULL) {
        localM->Set(mLocalMatrix);
        Boolean result;
        mLocalMatrix->IsIdentity(&result);
        return !result;
    }
    return FALSE;
}

ECode Shader::SetLocalMatrix(
    /* [in] */ IMatrix* localM)
{
    mLocalMatrix = localM;
    NativeSetLocalMatrix(mNativeInstance, mNativeShader,
            localM == NULL ? 0 : ((Matrix*)localM->Probe(EIID_Matrix))->mNativeInstance);
    return NOERROR;
}

void Shader::NativeDestructor(
    /* [in] */ Int32 shader,
    /* [in] */ Int32 skiaShader)
{
    SkSafeUnref((SkShader*)shader);
    // skiaShader == NULL when not !USE_OPENGL_RENDERER, so no need to delete it outside the ifdef
#ifdef USE_OPENGL_RENDERER
    if (android::uirenderer::Caches::hasInstance()) {
        android::uirenderer::Caches::getInstance().resourceCache.destructor((SkiaShader*)skiaShader);
    } else {
        delete (SkiaShader*)skiaShader;
    }
#endif
}

void Shader::NativeSetLocalMatrix(
    /* [in] */ Int32 shader,
    /* [in] */ Int32 skiaShader,
    /* [in] */ Int32 matrix)
{
    if (shader) {
        if (0 == matrix) {
            ((SkShader*)shader)->resetLocalMatrix();
        }
        else {
            ((SkShader*)shader)->setLocalMatrix(*(SkMatrix*)matrix);
        }
#ifdef USE_OPENGL_RENDERER
        ((SkiaShader*)skiaShader)->setMatrix((SkMatrix*)matrix);
#endif
    }
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
