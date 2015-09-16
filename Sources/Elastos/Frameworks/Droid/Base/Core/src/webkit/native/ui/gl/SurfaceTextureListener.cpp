// wuweizuo automatic build .cpp file from .java file.

#include "SurfaceTextureListener.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Gl {

//=====================================================================
//                        SurfaceTextureListener
//=====================================================================
SurfaceTextureListener::SurfaceTextureListener(
    /* [in] */ Int64 nativeSurfaceTextureListener)
{
    // ==================before translated======================
    // assert nativeSurfaceTextureListener != 0;
    // mNativeSurfaceTextureListener = nativeSurfaceTextureListener;
}

ECode SurfaceTextureListener::OnFrameAvailable(
    /* [in] */ ISurfaceTexture* surfaceTexture)
{
    VALIDATE_NOT_NULL(surfaceTexture);
    // ==================before translated======================
    // nativeFrameAvailable(mNativeSurfaceTextureListener);
    assert(0);
    return NOERROR;
}

ECode SurfaceTextureListener::Finalize()
{
    // ==================before translated======================
    // try {
    //     nativeDestroy(mNativeSurfaceTextureListener);
    // } finally {
    //     super.finalize();
    // }
    assert(0);
    return NOERROR;
}

ECode SurfaceTextureListener::NativeFrameAvailable(
    /* [in] */ Int64 nativeSurfaceTextureListener)
{
    assert(0);
    return NOERROR;
}

ECode SurfaceTextureListener::NativeDestroy(
    /* [in] */ Int64 nativeSurfaceTextureListener)
{
    assert(0);
    return NOERROR;
}

} // namespace Gl
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


