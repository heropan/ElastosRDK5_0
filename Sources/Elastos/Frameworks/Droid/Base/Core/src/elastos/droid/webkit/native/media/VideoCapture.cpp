
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

//===============================================================
//                 VideoCapture::CaptureFormat
//===============================================================

VideoCapture::CaptureFormat::CaptureFormat(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 framerate,
    /* [in] */ Int32 pixelformat)
    : mWidth(width)
    , mHeight(height)
    , mFramerate(framerate)
    , mPixelFormat(pixelformat)
{
}

Int32 VideoCapture::CaptureFormat::GetWidth()
{
    return mWidth;
}

Int32 VideoCapture::CaptureFormat::GetHeight()
{
    return mHeight;
}

Int32 VideoCapture::CaptureFormat::GetFramerate()
{
    return mFramerate;
}

Int32 VideoCapture::CaptureFormat::GetPixelFormat()
{
    return mPixelFormat;
}

//===============================================================
//                         VideoCapture
//===============================================================

const Int32 VideoCapture::GL_TEXTURE_EXTERNAL_OES;
const String VideoCapture::TAG("VideoCapture");

// Lock to mutually exclude execution of OnPreviewFrame {start/stop}Capture.

VideoCapture::VideoCapture(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [in] */ Int64 nativeVideoCaptureDeviceAndroid)
    : mContext(context)
    , mId(id)
    , mNativeVideoCaptureDeviceAndroid(nativeVideoCaptureDeviceAndroid)
    , mIsRunning(FALSE)
    , mCameraOrientation(0)
    , mCameraFacing(0)
    , mDeviceOrientation(0)
{

    CReentrantLock::New((IReentrantLock**)&mPreviewBufferLock);
}

//@CalledByNative
Boolean VideoCapture::Allocate(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 frameRate)
{
    // Log.d(TAG, "allocate: requested (" + width + "x" + height + ")@" +
    //         frameRate + "fps");
    // try {
        Camera->Open(mId, (ICamera**)&mCamera);
    // } catch (RuntimeException ex) {
    //     Log.e(TAG, "allocate: Camera.open: " + ex);
    //     return false;
    // }

    AutoPtr<ICameraInfo> cameraInfo = GetCameraInfo(mId);
    if (cameraInfo == NULL) {
        mCamera->Release();
        mCamera = NULL;
        return FALSE;
    }

    cameraInfo->GetOrientation(&mCameraOrientation);
    cameraInfo->GetFacing(&mCameraFacing);
    mDeviceOrientation = GetDeviceOrientation();
    // Log.d(TAG, "allocate: orientation dev=" + mDeviceOrientation +
    //           ", cam=" + mCameraOrientation + ", facing=" + mCameraFacing);

    AutoPtr<IParameters> parameters = GetCameraParameters(mCamera);
    if (parameters == NULL) {
        mCamera = NULL;
        return FALSE;
    }

    // getSupportedPreviewFpsRange() returns a List with at least one
    // element, but when camera is in bad state, it can return null pointer.
    List<int[]> listFpsRange = parameters.getSupportedPreviewFpsRange();
    if (listFpsRange == null || listFpsRange.size() == 0) {
//        Log.e(TAG, "allocate: no fps range found");
        return FALSE;
    }

    Int32 frameRateInMs = frameRate * 1000;
    // Use the first range as default.
    AutoPtr< ArrayOf<Int> > fpsMinMax = listFpsRange.get(0);
    Int32 newFrameRate = ((*fpsMinMax)[0] + 999) / 1000;
    for (int[] fpsRange : listFpsRange) {
        if (fpsRange[0] <= frameRateInMs && frameRateInMs <= fpsRange[1]) {
            fpsMinMax = fpsRange;
            newFrameRate = frameRate;
            break;
        }
    }

    frameRate = newFrameRate;
//    Log.d(TAG, "allocate: fps set to " + frameRate);

    // Calculate size.
    AutoPtr< ArrayOf<ICameraSize*> > listCameraSize;
    parameters->GetSupportedPreviewSizes((ArrayOf<ICameraSize*>**)&listCameraSize);
    Int32 minDiff = Integer.MAX_VALUE;
    Int32 matchedWidth = width;
    Int32 matchedHeight = height;

    AutoPtr<ICameraSize> size;
    Int32 length = listCameraSize->GetLength();
    for (Int32 i = 0; i < length; ++i) {
        size = (*listCameraSize)[i];
        Int32 _width, _height;
        size->GetWidth(&_width);
        size->GetHeight(&_height);
        Int32 diff = Math::Abs(_width - width) +
                   Math::Abs(_height - height);
//        Log.d(TAG, "allocate: supported (" +
//              size.width + ", " + size.height + "), diff=" + diff);
        // TODO(wjia): Remove this hack (forcing width to be multiple
        // of 32) by supporting stride in video frame buffer.
        // Right now, VideoCaptureController requires compact YV12
        // (i.e., with no padding).
        if (diff < minDiff && (_width % 32 == 0)) {
            minDiff = diff;
            matchedWidth = _width;
            matchedHeight = _height;
        }
    }

    if (minDiff == Integer.MAX_VALUE) {
//        Log.e(TAG, "allocate: can not find a multiple-of-32 resolution");
        return FALSE;
    }

//    Log.d(TAG, "allocate: matched (" + matchedWidth + "x" + matchedHeight + ")");

    Boolean isVideoStabilizationSupported;
    parameters->IsVideoStabilizationSupported(&isVideoStabilizationSupported);
    if (isVideoStabilizationSupported) {
        // Log.d(TAG, "Image stabilization supported, currently: " +
        //       parameters.getVideoStabilization() + ", setting it.");
        parameters->SetVideoStabilization(TRUE);
    }
    else {
        //Log.d(TAG, "Image stabilization not supported.");
    }

    SetCaptureParameters(matchedWidth, matchedHeight, frameRate, parameters);
    parameters->SetPreviewSize(mCaptureFormat.mWidth,
                              mCaptureFormat.mHeight);
    parameters->SetPreviewFpsRange((*fpsMinMax)[0], (*fpsMinMax)[1]);
    parameters->SetPreviewFormat(mCaptureFormat->mPixelFormat);
    mCamera->SetParameters(parameters);

    // Set SurfaceTexture. Android Capture needs a SurfaceTexture even if
    // it is not going to be used.
    mGlTextures = ArrayOf<Int32>::Alloc(1);

    // Generate one texture pointer and bind it as an external texture.
    AutoPtr<IGLES20> gles20;
    CGLES20::AcquireSingleton((IGLES20**)&gles20);
    gles20->GlGenTextures(1, mGlTextures, 0);
    gles20->GlBindTexture(GL_TEXTURE_EXTERNAL_OES, (*mGlTextures)[0]);
    // No mip-mapping with camera source.
    gles20->GlTexParameterf(GL_TEXTURE_EXTERNAL_OES,
            IGLES20::GL_TEXTURE_MIN_FILTER, IGLES20::GL_LINEAR);
    gles20->GlTexParameterf(GL_TEXTURE_EXTERNAL_OES,
            IGLES20::GL_TEXTURE_MAG_FILTER, IGLES20::GL_LINEAR);
    // Clamp to edge is only option.
    gles20->GlTexParameteri(GL_TEXTURE_EXTERNAL_OES,
            IGLES20::GL_TEXTURE_WRAP_S, IGLES20::GL_CLAMP_TO_EDGE);
    gles20->GlTexParameteri(GL_TEXTURE_EXTERNAL_OES,
            IGLES20::GL_TEXTURE_WRAP_T, IGLES20::GL_CLAMP_TO_EDGE);

    CSurfaceTexture::New((*mGlTextures[0]), (ISurfaceTexture**)&mSurfaceTexture);
    mSurfaceTexture->SetOnFrameAvailableListener(NULL);
    // try {
        mCamera->SetPreviewTexture(mSurfaceTexture);
    // } catch (IOException ex) {
    //     Log.e(TAG, "allocate: " + ex);
    //     return false;
    // }

    AllocateBuffers();

    return TRUE;
}

//@CalledByNative
Int32 VideoCapture::StartCapture()
{
    if (mCamera == NULL) {
//        Log.e(TAG, "startCapture: camera is null");
        return -1;
    }

    mPreviewBufferLock->Lock();
    //try {
        if (mIsRunning) {
            return 0;
        }
        mIsRunning = TRUE;
    //} finally {
        mPreviewBufferLock->Unlock();
    //}
    SetPreviewCallback(this);
    // try {
        mCamera->StartPreview();
    // } catch (RuntimeException ex) {
    //     Log.e(TAG, "startCapture: Camera.startPreview: " + ex);
    //     return -1;
    // }

    return 0;
}

//@CalledByNative
Int32 VideoCapture::StopCapture()
{
    if (mCamera == NULL) {
//        Log.e(TAG, "stopCapture: camera is null");
        return 0;
    }

    mPreviewBufferLock->Lock();
    //try {
        if (!mIsRunning) {
            return 0;
        }
        mIsRunning = FALSE;
    //} finally {
        mPreviewBufferLock->Unlock();
    //}

    mCamera->StopPreview();
    SetPreviewCallback(NULL);

    return 0;
}

//@CalledByNative
void VideoCapture::Deallocate()
{
    if (mCamera == NULL)
        return;

    StopCapture();
    // try {
        mCamera->SetPreviewTexture(NULL);
        if (mGlTextures != NULL) {
            AutoPtr<IGLES20> gles20;
            CGLES20::AcquireSingleton((IGLES20**)&gles20);
            gles20->GlDeleteTextures(1, mGlTextures, 0);
        }
        mCaptureFormat = NULL;
        mCamera->Release();
        mCamera = NULL;
    // } catch (IOException ex) {
    //     Log.e(TAG, "deallocate: failed to deallocate camera, " + ex);
    //     return;
    // }
}

//@CalledByNative
Int32 VideoCapture::QueryWidth()
{
    return mCaptureFormat->mWidth;
}

//@CalledByNative
Int32 VideoCapture::QueryHeight()
{
    return mCaptureFormat->mHeight;
}

//@CalledByNative
Int32 VideoCapture::QueryFrameRate()
{
    return mCaptureFormat->mFramerate;
}

//@CalledByNative
Int3 VideoCapture::GetColorspace()
{
    switch (mCaptureFormat->mPixelFormat) {
        case IImageFormat::YV12:
            return AndroidImageFormatList::ANDROID_IMAGEFORMAT_YV12;
        case IImageFormat::NV21:
            return AndroidImageFormatList::ANDROID_IMAGEFORMAT_NV21;
        case IImageFormat::UNKNOWN:
        default:
            return AndroidImageFormatList::ANDROID_IMAGEFORMAT_UNKNOWN;
    }
}

Int32 VideoCapture::GetDeviceOrientation()
{
    Int32 orientation = 0;
    if (mContext != NULL) {
        AutoPtr<IWindowManager> wm;
        mContext->GetSystemService(
                IContext::WINDOW_SERVICE,
                (IWindowManager**)&wm);

        AutoPtr<IDisplay> display;
        wm->GetDefaultDisplay((IDisplay**)&display);
        Int32 rotation;
        display->GetRotation(&rotation);
        switch(rotation) {
            case ISurface::ROTATION_90:
                orientation = 90;
                break;
            case ISurface::ROTATION_180:
                orientation = 180;
                break;
            case ISurface::ROTATION_270:
                orientation = 270;
                break;
            case ISurface::ROTATION_0:
            default:
                orientation = 0;
                break;
        }
    }

    return orientation;
}

// Method for VideoCapture implementations to call back native code.
void VideoCapture::NativeOnFrameAvailable(
    /* [in] */ Int64 nativeVideoCaptureDeviceAndroid,
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 length,
    /* [in] */ Int32 rotation)
{
}

AutoPtr<IParameters> VideoCapture::GetCameraParameters(
    /* [in] */ ICamera* camera)
{
    AutoPtr<IParameters> parameters;
    // try {
        camera->GetParameters((IParameters**)&parameters);
    // } catch (RuntimeException ex) {
    //     Log.e(TAG, "getCameraParameters: Camera.getParameters: " + ex);
    //     camera.release();
    //     return null;
    // }

    return parameters;
}

AutoPtr<ICameraInfo> VideoCapture::GetCameraInfo(
    /* [in] */ Int32 id)
{
    AutoPtr<ICameraInfo> cameraInfo;
    CCameraInfo::New((ICameraInfo**)&cameraInfo);
    // try {
        AutoPtr<IHardwareCameraHelper> helper;
        CHardwareCameraHelper::AcquireSingleton((IHardwareCameraHelper**)&helper);
        helper->GetCameraInfo(id, cameraInfo);
    // } catch (RuntimeException ex) {
    //     Log.e(TAG, "getCameraInfo: Camera.getCameraInfo: " + ex);
    //     return null;
    // }

    return cameraInfo;
}

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
