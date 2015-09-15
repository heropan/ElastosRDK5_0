// wuweizuo automatic build .cpp file from .java file.

#include "SurfaceTexturePlatformWrapper.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Gl {

//=====================================================================
//                    SurfaceTexturePlatformWrapper
//=====================================================================
const String SurfaceTexturePlatformWrapper::TAG("SurfaceTexturePlatformWrapper");

AutoPtr<ISurfaceTexture> SurfaceTexturePlatformWrapper::Create(
    /* [in] */ Int32 textureId)
{
    // ==================before translated======================
    // return new SurfaceTexture(textureId);
    assert(0);
    AutoPtr<ISurfaceTexture> empty;
    return empty;
}

AutoPtr<ISurfaceTexture> SurfaceTexturePlatformWrapper::CreateSingleBuffered(
    /* [in] */ Int32 textureId)
{
    // ==================before translated======================
    // assert Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT;
    // return new SurfaceTexture(textureId, true);
    assert(0);
    AutoPtr<ISurfaceTexture> empty;
    return empty;
}

ECode SurfaceTexturePlatformWrapper::Destroy(
    /* [in] */ ISurfaceTexture* surfaceTexture)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    // ==================before translated======================
    // surfaceTexture.setOnFrameAvailableListener(null);
    // surfaceTexture.release();
    assert(0);
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::SetFrameAvailableCallback(
    /* [in] */ ISurfaceTexture* surfaceTexture,
    /* [in] */ Int64 nativeSurfaceTextureListener)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    // ==================before translated======================
    // surfaceTexture.setOnFrameAvailableListener(
    //         new SurfaceTextureListener(nativeSurfaceTextureListener));
    assert(0);
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::UpdateTexImage(
    /* [in] */ ISurfaceTexture* surfaceTexture)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    // ==================before translated======================
    // try {
    //     surfaceTexture.updateTexImage();
    // } catch (RuntimeException e) {
    //     Log.e(TAG, "Error calling updateTexImage", e);
    // }
    assert(0);
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::ReleaseTexImage(
    /* [in] */ ISurfaceTexture* surfaceTexture)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    // ==================before translated======================
    // assert Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT;
    // surfaceTexture.releaseTexImage();
    assert(0);
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::SetDefaultBufferSize(
    /* [in] */ ISurfaceTexture* surfaceTexture,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    // ==================before translated======================
    // surfaceTexture.setDefaultBufferSize(width, height);
    assert(0);
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::GetTransformMatrix(
    /* [in] */ ISurfaceTexture* surfaceTexture,
    /* [in] */ ArrayOf<Float>* matrix)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    VALIDATE_NOT_NULL(matrix);
    // ==================before translated======================
    // surfaceTexture.getTransformMatrix(matrix);
    assert(0);
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::AttachToGLContext(
    /* [in] */ ISurfaceTexture* surfaceTexture,
    /* [in] */ Int32 texName)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    // ==================before translated======================
    // assert Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN;
    // surfaceTexture.attachToGLContext(texName);
    assert(0);
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::DetachFromGLContext(
    /* [in] */ ISurfaceTexture* surfaceTexture)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    // ==================before translated======================
    // assert Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN;
    // surfaceTexture.detachFromGLContext();
    assert(0);
    return NOERROR;
}

} // namespace Gl
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


