
#include "elastos/droid/webkit/native/ui/gl/SurfaceTexturePlatformWrapper.h"
#include "elastos/droid/graphics/CSurfaceTexture.h"
#include "elastos/droid/os/Build.h"
#include "elastos/droid/webkit/native/ui/gl/SurfaceTextureListener.h"

using Elastos::Droid::Graphics::ISurfaceTexture;
using Elastos::Droid::Graphics::CSurfaceTexture;
using Elastos::Droid::Os::Build;
using Elastos::Droid::View::ISurfaceTextureListener;
using Elastos::Droid::Webkit::Ui::Gl::SurfaceTextureListener;

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

    AutoPtr<ISurfaceTexture> surfaceTexture;
    CSurfaceTexture::New(textureId, (ISurfaceTexture**)&surfaceTexture);
    return surfaceTexture;
}

AutoPtr<ISurfaceTexture> SurfaceTexturePlatformWrapper::CreateSingleBuffered(
    /* [in] */ Int32 textureId)
{
    // ==================before translated======================
    // assert Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT;
    // return new SurfaceTexture(textureId, true);

    assert (Build::VERSION::SDK_INT >= Build::VERSION_CODES::KITKAT);
    AutoPtr<ISurfaceTexture> surfaceTexture;
    CSurfaceTexture::New(textureId, TRUE, (ISurfaceTexture**)&surfaceTexture);
    return surfaceTexture;
}

ECode SurfaceTexturePlatformWrapper::Destroy(
    /* [in] */ ISurfaceTexture* surfaceTexture)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    // ==================before translated======================
    // surfaceTexture.setOnFrameAvailableListener(null);
    // surfaceTexture.release();

    surfaceTexture->SetOnFrameAvailableListener(NULL);
    surfaceTexture->Release();
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

    AutoPtr<SurfaceTextureListener> surfaceTextureListener = new SurfaceTextureListener(nativeSurfaceTextureListener);
    surfaceTexture->SetOnFrameAvailableListener((IOnFrameAvailableListener*)surfaceTextureListener);
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

    //try {
        surfaceTexture->UpdateTexImage();
    //}
    // catch (RuntimeException e) {
    //    Log.e(TAG, "Error calling updateTexImage", e);
    //}
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::ReleaseTexImage(
    /* [in] */ ISurfaceTexture* surfaceTexture)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    // ==================before translated======================
    // assert Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT;
    // surfaceTexture.releaseTexImage();

    assert (Build::VERSION::SDK_INT >= Build::VERSION_CODES::KITKAT);
    surfaceTexture->ReleaseTexImage();
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

    surfaceTexture->SetDefaultBufferSize(width, height);
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

    surfaceTexture->GetTransformMatrix(matrix);
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

    assert (Build::VERSION::SDK_INT >= Build::VERSION_CODES::JELLY_BEAN);
    surfaceTexture->AttachToGLContext(texName);
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::DetachFromGLContext(
    /* [in] */ ISurfaceTexture* surfaceTexture)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    // ==================before translated======================
    // assert Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN;
    // surfaceTexture.detachFromGLContext();

    assert (Build::VERSION::SDK_INT >= Build::VERSION_CODES::JELLY_BEAN);
    surfaceTexture->DetachFromGLContext();
    return NOERROR;
}

} // namespace Gl
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


