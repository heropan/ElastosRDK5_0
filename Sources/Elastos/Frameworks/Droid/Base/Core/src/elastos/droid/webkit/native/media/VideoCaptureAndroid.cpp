
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

//===============================================================
//       VideoCaptureAndroid::BuggyDeviceHack::IdAndSizes
//===============================================================

VideoCaptureAndroid::BuggyDeviceHack::IdAndSizes::IdAndSizes(
    /* [in] */ const String& model,
    /* [in] */ const String& device,
    /* [in] */ Int32 minWidth,
    /* [in] */ Int32 minHeight)
    : mModel(model)
    , mDevice(device)
    , mMinWidth(minWidth)
    , mMinHeight(minHeight)
{
}

//===============================================================
//              VideoCaptureAndroid::BuggyDeviceHack
//===============================================================

static AutoPtr< ArrayOf<IdAndSizes> > s_CAPTURESIZE_BUGGY_DEVICE_LIST_Init()
{
    AutoPtr<IdAndSizes> idAndSizes = new IdAndSizes("Nexus 7", "flo", 640, 480);
    AutoPtr< ArrayOf<IdAndSizes> > array = ArrayOf<IdAndSizes>::Alloc(1);
    (*array)[0] = idAndSizes;
    return array;
}

static AutoPtr< ArrayOf<String> > s_COLORSPACE_BUGGY_DEVICE_LIST_Init()
{
    AutoPtr< ArrayOf<String> > array = ArrayOf<String>::Alloc(2);
    (*array)[0] = String("SAMSUNG-SGH-I747");
    (*array)[1] = String("ODROID-U2");
    return array;
}

const AutoPtr< ArrayOf<IdAndSizes> > VideoCaptureAndroid::BuggyDeviceHack::s_CAPTURESIZE_BUGGY_DEVICE_LIST = s_CAPTURESIZE_BUGGY_DEVICE_LIST_Init();
const AutoPtr< ArrayOf<String> > VideoCaptureAndroid::BuggyDeviceHack::s_COLORSPACE_BUGGY_DEVICE_LIST = s_COLORSPACE_BUGGY_DEVICE_LIST_Init();

void VideoCaptureAndroid::BuggyDeviceHack::ApplyMinDimensions(
    /* [in] */ CaptureFormat* format)
{
    // NOTE: this can discard requested aspect ratio considerations.
    Int32 length = s_CAPTURESIZE_BUGGY_DEVICE_LIST->GetLength();
    for (Int32 i = 0; i < length; ++i) {
        AutoPtr<IdAndSizes> buggyDevice = (*s_CAPTURESIZE_BUGGY_DEVICE_LIST)[i];
        if (buggyDevice->mModel.ContentEquals(android::os::Build::MODEL) &&
                buggyDevice::mDevice->ContentEquals(android::os::Build::DEVICE)) {
            format->mWidth = (buggyDevice->mMinWidth > format->mWidth)
                    ? buggyDevice->mMinWidth : format->mWidth;
            format->mHeight = (buggyDevice->mMinHeight > format->mHeight)
                    ? buggyDevice->mMinHeight : format->mHeight;
        }
    }
}

Int32 VideoCaptureAndroid::BuggyDeviceHack::GetImageFormat()
{
    if (android::os::Build::VERSION.SDK_INT < android::os::Build::VERSION_CODES::JELLY_BEAN) {
        return IImageFormat::NV21;
    }

    Int32 length = s_COLORSPACE_BUGGY_DEVICE_LIST->GetLength();
    for (Int32 i = 0; i < length; ++i) {
        String buggyDevice = (*s_COLORSPACE_BUGGY_DEVICE_LIST)[i];
        if (buggyDevice.ContentEquals(android::os::Build::MODEL)) {
            return IImageFormat::NV21;
        }
    }

    return IImageFormat::YV12;
}

//===============================================================
//                    VideoCaptureAndroid
//===============================================================

const Int32 VideoCaptureAndroid::NUM_CAPTURE_BUFFERS;
const String VideoCaptureAndroid::TAG("VideoCaptureAndroid");

VideoCaptureAndroid::VideoCaptureAndroid(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [in] */ Int64 nativeVideoCaptureDeviceAndroid)
    : VideoCapture(context, id, nativeVideoCaptureDeviceAndroid)
    , mExpectedFrameSize(0)
{
}

AutoPtr< ArrayOf<CaptureFormat> > VideoCaptureAndroid::GetDeviceSupportedFormats(
    /* [in] */ Int32 id)
{
    AutoPtr<ICamera> camera;
    // try {
        AutoPtr<IHardwareCameraHelper> helper;
        CHardwareCameraHelper::AcquireSingleton((IHardwareCameraHelper**)&helper);
        helper->Open(id, (ICamera**)&camera);
    // } catch (RuntimeException ex) {
    //     Log.e(TAG, "Camera.open: " + ex);
    //     return null;
    // }

    AutoPtr<ICameraParameters> parameters = GetCameraParameters(camera);
    if (parameters == NULL) {
        return NULL;
    }

    ArrayList<CaptureFormat> formatList = new ArrayList<CaptureFormat>();
    // getSupportedPreview{Formats,FpsRange,PreviewSizes}() returns Lists
    // with at least one element, but when the camera is in bad state, they
    // can return null pointers; in that case we use a 0 entry, so we can
    // retrieve as much information as possible.
    List<Integer> pixelFormats = parameters.getSupportedPreviewFormats();
    if (pixelFormats == null) {
        pixelFormats = new ArrayList<Integer>();
    }
    if (pixelFormats.size() == 0) {
        pixelFormats.add(ImageFormat.UNKNOWN);
    }
    for (Integer previewFormat : pixelFormats) {
        int pixelFormat =
                AndroidImageFormatList.ANDROID_IMAGEFORMAT_UNKNOWN;
        if (previewFormat == ImageFormat.YV12) {
            pixelFormat = AndroidImageFormatList.ANDROID_IMAGEFORMAT_YV12;
        } else if (previewFormat == ImageFormat.NV21) {
            continue;
        }

        List<int[]> listFpsRange = parameters.getSupportedPreviewFpsRange();
        if (listFpsRange == null) {
            listFpsRange = new ArrayList<int[]>();
        }
        if (listFpsRange.size() == 0) {
            listFpsRange.add(new int[] {0, 0});
        }
        for (int[] fpsRange : listFpsRange) {
            List<Camera.Size> supportedSizes =
                    parameters.getSupportedPreviewSizes();
            if (supportedSizes == null) {
                supportedSizes = new ArrayList<Camera.Size>();
            }
            if (supportedSizes.size() == 0) {
                supportedSizes.add(camera.new Size(0, 0));
            }
            for (Camera.Size size : supportedSizes) {
                formatList.add(new CaptureFormat(size.width,
                                                 size.height,
                                                 (fpsRange[0] + 999) / 1000,
                                                 pixelFormat));
            }
        }
    }

    camera->Release();

    return formatList.toArray(new CaptureFormat[formatList.size()]);
}


//@Override
void VideoCaptureAndroid::SetCaptureParameters(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 frameRate,
    /* [in] */ ICameraParameters* cameraParameters)
{
    mCaptureFormat = new CaptureFormat(
            width, height, frameRate, BuggyDeviceHack::getImageFormat());
    // Hack to avoid certain capture resolutions under a minimum one,
    // see http://crbug.com/305294.
    BuggyDeviceHack::ApplyMinDimensions(mCaptureFormat);
}

//@Override
void VideoCaptureAndroid::AllocateBuffers()
{
    AutoPtr<IImageFormat> format;
    CImageFormat::AcquireSingleton((IImageFormat**)&format);
    Int32 bits;
    bits->GetBitsPerPixel(mCaptureFormat->mPixelFormat, &bits);
    mExpectedFrameSize = mCaptureFormat.mWidth * mCaptureFormat.mHeight *
            bits / 8;

    for (Int32 i = 0; i < NUM_CAPTURE_BUFFERS; i++) {
        AutoPtr< ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(mExpectedFrameSize);
        mCamera->AddCallbackBuffer(buffer);
    }
}

//@Override
void VideoCaptureAndroid::SetPreviewCallback(
    /* [in] */ ICameraPreviewCallback* cb)
{
    mCamera->SetPreviewCallbackWithBuffer(cb);
}

//@Override
void VideoCaptureAndroid::OnPreviewFrame(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ ICamera* camera)
{
    mPreviewBufferLock.lock();
    // try {
        if (!mIsRunning) {
            return;
        }

        if (data->GetLength() == mExpectedFrameSize) {
            Int32 rotation = GetDeviceOrientation();
            if (rotation != mDeviceOrientation) {
                mDeviceOrientation = rotation;
            }
            if (mCameraFacing == ICameraCameraInfo::CAMERA_FACING_BACK) {
                rotation = 360 - rotation;
            }
            rotation = (mCameraOrientation + rotation) % 360;
            NativeOnFrameAvailable(mNativeVideoCaptureDeviceAndroid,
                    data, mExpectedFrameSize, rotation);
        }
    // } finally {
        mPreviewBufferLock.unlock();
        if (camera != NULL) {
            camera->AddCallbackBuffer(data);
        }
    // }
}

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
