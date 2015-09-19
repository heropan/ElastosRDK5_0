
#ifndef __ELASTOS_DROID_WEBKIT_MEDIA_VIDEOCAPTURE_H__
#define __ELASTOS_DROID_WEBKIT_MEDIA_VIDEOCAPTURE_H__

// import android.content.Context;
// import android.graphics.ImageFormat;
// import android.graphics.SurfaceTexture;
// import android.hardware.Camera;
// import android.hardware.Camera.PreviewCallback;
// import android.opengl.GLES20;
// import android.util.Log;
// import android.view.Surface;
// import android.view.WindowManager;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

// import java.io.IOException;
// import java.util.List;
// import java.util.concurrent.locks.ReentrantLock;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

/**
 * Video Capture Device base class to interface to native Chromium.
 **/
//@JNINamespace("media")
class VideoCapture
    : public Object
    , public PreviewCallback
{
protected:
    class CaptureFormat
    {
    public:
        CaptureFormat(
            /* [in] */ Int32 width,
            /* [in] */ Int32 height,
            /* [in] */ Int32 framerate,
            /* [in] */ Int32 pixelformat);

        CARAPI_(Int32) GetWidth();

        CARAPI_(Int32) GetHeight();

        CARAPI_(Int32) GetFramerate();

        CARAPI_(Int32) GetPixelFormat();

    public:
        Int3 mWidth;
        Int32 mHeight;
        const Int32 mFramerate;
        const Int32 mPixelFormat;
    };

public:
    VideoCapture(
        /* [in] */ IContext* context,
        /* [in] */ Int32 id,
        /* [in] */ Int64 nativeVideoCaptureDeviceAndroid);

    //@CalledByNative
    virtual CARAPI_(Boolean) Allocate(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 frameRate);

    //@CalledByNative
    virtual CARAPI_(Int32) StartCapture();

    //@CalledByNative
    virtual CARAPI_(Int32) StopCapture();

    //@CalledByNative
    virtual CARAPI_(void) Deallocate();

    // Local hook to allow derived classes to fill capture format and modify
    // camera parameters as they see fit.
    virtual CARAPI_(void) SetCaptureParameters(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 frameRate,
        /* [in] */ IParameters* cameraParameters) = 0;

    // Local hook to allow derived classes to configure and plug capture
    // buffers if needed.
    virtual CARAPI_(void) AllocateBuffers() = 0;

    // Local method to be overriden with the particular setPreviewCallback to be
    // used in the implementations.
    virtual CARAPI_(void) SetPreviewCallback(
        /* [in] */ IPreviewCallback* cb) = 0;

    //@CalledByNative
    virtual CARAPI_(Int32) QueryWidth();

    //@CalledByNative
    virtual CARAPI_(Int32) QueryHeight();

    //@CalledByNative
    virtual CARAPI_(Int32) QueryFrameRate();

    //@CalledByNative
    virtual CARAPI_(Int32) GetColorspace();

    // Method for VideoCapture implementations to call back native code.
    virtual CARAPI_(void) NativeOnFrameAvailable(
        /* [in] */ Int64 nativeVideoCaptureDeviceAndroid,
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 length,
        /* [in] */ Int32 rotation);

protected:
    CARAPI_(Int32) GetDeviceOrientation();

    static CARAPI_(AutoPtr<IParameters>) GetCameraParameters(
        /* [in] */ ICamera* camera);

protected:
    AutoPtr<ICamera> mCamera;
    AutoPtr<CaptureFormat> mCaptureFormat;

    // Lock to mutually exclude execution of OnPreviewFrame {start/stop}Capture.
    AutoPtr<IReentrantLock> mPreviewBufferLock;
    AutoPtr<IContext> mContext;

    // True when native code has started capture.
    Boolean mIsRunning;

    Int32 mId;
    // Native callback context variable.
    Int64 mNativeVideoCaptureDeviceAndroid;
    AutoPtr< ArrayOf<Int32> > mGlTextures;
    AutoPtr<ISurfaceTexture> mSurfaceTexture;
    static const Int32 GL_TEXTURE_EXTERNAL_OES = 0x8D65;

    Int32 mCameraOrientation;
    Int32 mCameraFacing;
    Int32 mDeviceOrientation;

private:
    static const String TAG;

    CARAPI_(AutoPtr<ICameraInfo>) GetCameraInfo(
        /* [in] */ Int32 id);
};

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_MEDIA_VIDEOCAPTURE_H__
