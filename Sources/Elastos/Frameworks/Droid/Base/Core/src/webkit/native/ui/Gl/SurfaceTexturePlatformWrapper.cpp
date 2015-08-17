// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Gl {

const String SurfaceTexturePlatformWrapper::TAG("SurfaceTexturePlatformWrapper");

AutoPtr<ISurfaceTexture> SurfaceTexturePlatformWrapper::Create(
    /* in */ Int32 textureId)
{
    AutoPtr<ISurfaceTexture> surfaceTexture;
    CSurfaceTexture::New(textureId, (ISurfaceTexture**)&surfaceTexture);
    return surfaceTexture;
}

AutoPtr<ISurfaceTexture> SurfaceTexturePlatformWrapper::CreateSingleBuffered(
    /* in */ Int32 textureId)
{
    assert (IBuild::VERSION::SDK_INT >= IBuild::VERSION_CODES::KITKAT);
    AutoPtr<ISurfaceTexture> surfaceTexture;
    CSurfaceTexture::New(textureId, TRUE, (ISurfaceTexture**)&surfaceTexture);
    return surfaceTexture;
}

ECode SurfaceTexturePlatformWrapper::Destroy(
    /* in */ ISurfaceTexture* surfaceTexture)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    surfaceTexture->SetOnFrameAvailableListener(NULL);
    surfaceTexture->Release();
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::SetFrameAvailableCallback(
    /* in */ ISurfaceTexture* surfaceTexture,
    /* in */ Int64 nativeSurfaceTextureListener)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    AutoPtr<ISurfaceTextureListener> surfaceTextureListener;
    CSurfaceTextureListener::New(nativeSurfaceTextureListener, (ISurfaceTextureListener**)&surfaceTextureListener);
    surfaceTexture->SetOnFrameAvailableListener(surfaceTextureListener);
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::UpdateTexImage(
    /* in */ ISurfaceTexture* surfaceTexture)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    //try {
        surfaceTexture->UpdateTexImage();
    //}
    // catch (RuntimeException e) {
    //    Log.e(TAG, "Error calling updateTexImage", e);
    //}
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::ReleaseTexImage(
    /* in */ ISurfaceTexture* surfaceTexture)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    assert (IBuild::VERSION::SDK_INT >= IBuild::VERSION_CODES::KITKAT);
    surfaceTexture->ReleaseTexImage();
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::SetDefaultBufferSize(
    /* in */ ISurfaceTexture* surfaceTexture,
    /* in */ Int32 width,
    /* in */ Int32 height)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    surfaceTexture->SetDefaultBufferSize(width, height);
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::GetTransformMatrix(
    /* in */ ISurfaceTexture* surfaceTexture,
    /* in */ ArrayOf<Float>* matrix)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    VALIDATE_NOT_NULL(matrix);
    surfaceTexture->GetTransformMatrix(matrix);
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::AttachToGLContext(
    /* in */ ISurfaceTexture* surfaceTexture,
    /* in */ Int32 texName)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    assert (IBuild::VERSION::SDK_INT >= IBuild::VERSION_CODES::JELLY_BEAN);
    surfaceTexture->AttachToGLContext(texName);
    return NOERROR;
}

ECode SurfaceTexturePlatformWrapper::DetachFromGLContext(
    /* in */ ISurfaceTexture* surfaceTexture)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    assert (IBuild::VERSION::SDK_INT >= IBuild::VERSION_CODES::JELLY_BEAN);
    surfaceTexture->DetachFromGLContext();
    return NOERROR;
}

} // namespace Gl
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

