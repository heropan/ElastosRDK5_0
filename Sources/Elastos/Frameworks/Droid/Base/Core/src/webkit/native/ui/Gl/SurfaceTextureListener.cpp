// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

SurfaceTextureListener::SurfaceTextureListener(
	/* in */ Int64 nativeSurfaceTextureListener)
{
	assert (nativeSurfaceTextureListener != 0);
    mNativeSurfaceTextureListener = nativeSurfaceTextureListener;
}

ECode SurfaceTextureListener::OnFrameAvailable(
    /* in */ ISurfaceTexture* surfaceTexture)
{
	VALIDATE_NOT_NULL(surfaceTexture);
    NativeFrameAvailable(mNativeSurfaceTextureListener);
    return NOERROR;
}

ECode SurfaceTextureListener::Finalize()
{
    //try {
        NativeDestroy(mNativeSurfaceTextureListener);
    //}
    // finally {
        SurfaceTexture::OnFrameAvailableListener::Finalize();
    //}
    return NOERROR;
}

ECode SurfaceTextureListener::NativeFrameAvailable(
    /* in */ Int64 nativeSurfaceTextureListener)
{
    return NOERROR;
}

ECode SurfaceTextureListener::NativeDestroy(
    /* in */ Int64 nativeSurfaceTextureListener)
{
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

