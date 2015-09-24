
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

//===============================================================
//               VideoCaptureFactory::CamParams
//===============================================================

VideoCaptureFactory::CamParams::CamParams(
    /* [in] */ Int32 id,
    /* [in] */ const String& name,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : mId(id)
    , mName(name)
    , mWidth(width)
    , mHeight(height)
{
}

//===============================================================
//             VideoCaptureFactory::ChromiumCameraInfo
//===============================================================

static AutoPtr< ArrayOf<String> > s_SPECIAL_DEVICE_LIST_Init()
{
    AutoPtr< ArrayOf< AutoPtr< ArrayOf<String> > > > array = ArrayOf< AutoPtr< ArrayOf<String> > >::Alloc(1);
    AutoPtr< ArrayOf<String> > strs = ArrayOf<String>::Alloc(2);

    (*strs)[0] = String("Peanut");
    (*strs)[1] = String("peanut");

    (*array)[0] = strs;

    return array;
}

// Special devices have more cameras than usual. Those devices are
// identified by model & device. Currently only the Tango is supported.
// Note that these devices have no Camera.CameraInfo.
const AutoPtr< ArrayOf< AutoPtr< ArrayOf<String> > > > VideoCaptureFactory::ChromiumCameraInfo::s_SPECIAL_DEVICE_LIST = s_SPECIAL_DEVICE_LIST_Init();
const String VideoCaptureFactory::ChromiumCameraInfo::TAG("ChromiumCameraInfo");
static Int32 VideoCaptureFactory::ChromiumCameraInfo::sNumberOfSystemCameras;

VideoCaptureFactory::ChromiumCameraInfo::ChromiumCameraInfo(
    /* [in] */ Int32 index)
    : mId(index)
    , mCameraInfo(IsSpecialCamera(index) ? NULL : GetCameraInfo(mId))
{
}

Boolean VideoCaptureFactory::ChromiumCameraInfo::IsSpecialDevice()
{
    AutoPtr< ArrayOf<String> > device;
    Int32 length = s_SPECIAL_DEVICE_LIST->GetLength();
    for (Int32 i = 0; i < length; ++i) {
        device = (*s_SPECIAL_DEVICE_LIST[i]);
        if ((*device)[0].ContentEquals(android::os::Build::MODEL) &&
                (*device)[1].ContentEquals(android::os::Build::DEVICE)) {
            return TRUE;
        }
    }

    return FALSE;
}

Boolean VideoCaptureFactory::ChromiumCameraInfo::IsSpecialCamera(
    /* [in] */ Int32 id)
{
    return id >= sNumberOfSystemCameras;
}

Int32 VideoCaptureFactory::ChromiumCameraInfo::ToSpecialCameraId(
    /* [in] */ Int32 id)
{
    assert(IsSpecialCamera(id));
    return id - sNumberOfSystemCameras;
}

//@CalledByNative("ChromiumCameraInfo")
Int32 VideoCaptureFactory::ChromiumCameraInfo::GetNumberOfCameras(
    /* [in] */ IContext* appContext)
{
    // Camera.getNumberOfCammeras() will not fail without permission, but the
    // following operation on camera will do. Without permission isn't fatal
    // error in WebView, specially for those application which has no purpose
    // to use camera, but happens to load page required it.
    // So, we output a warning log and pretend system have no camera at all.
    if (sNumberOfSystemCameras == -1) {
        AutoPtr<IPackageManager> pm;
        appContext->GetPackageManager((IPackageManager**)&pm);
        String packageName;
        appContext->GetPackageName(&packageName);
        Int32 permission;
        pm->CheckPermission(String("android.permission.CAMERA"), packageName, &permission);
        if (IPackageManager::PERMISSION_GRANTED == permission) {
            AutoPtr<IHardwareCameraHelper> helper;
            CHardwareCameraHelper::AcquireSingleton((IHardwareCameraHelper**)&helper);
            helper->GetNumberOfCameras(&sNumberOfSystemCameras);
        }
        else {
            sNumberOfSystemCameras = 0;
            // Log.w(TAG, "Missing android.permission.CAMERA permission, "
            //         + "no system camera available.");
        }
    }

    if (IsSpecialDevice()) {
//        Log.d(TAG, "Special device: " + android.os.Build.MODEL);
        return sNumberOfSystemCameras +
               VideoCaptureTango::NumberOfCameras();
    }
    else {
        return sNumberOfSystemCameras;
    }
}

//@CalledByNative("ChromiumCameraInfo")
AutoPtr<ChromiumCameraInfo> VideoCaptureFactory::ChromiumCameraInfo::GetAt(
    /* [in] */ Int32 index)
{
    AutoPtr<ChromiumCameraInfo> info = new ChromiumCameraInfo(index);
    return info;
}

//@CalledByNative("ChromiumCameraInfo")
Int32 VideoCaptureFactory::ChromiumCameraInfo::GetId()
{
    return mId;
}

//@CalledByNative("ChromiumCameraInfo")
String VideoCaptureFactory::ChromiumCameraInfo::GetDeviceName()
{
    if (IsSpecialCamera(mId)) {
        return VideoCaptureTango::GetCamParams(ToSpecialCameraId(mId))->mName;
    }
    else {
        if (mCameraInfo == NULL) {
            return String("");
        }

        // Log.d(TAG, "Camera enumerated: " + (mCameraInfo.facing ==
        //         Camera.CameraInfo.CAMERA_FACING_FRONT ? "front" :
        //         "back"));
        String str("camera ");
        str += mId;
        str += ", facing ";
        Int32 facing;
        mCameraInfo->GetFacing(&facing);
        str += (facing ==
                ICameraInfo::CAMERA_FACING_FRONT ? "front" :
                "back");

        return str;
    }
}

//@CalledByNative("ChromiumCameraInfo")
Int32 VideoCaptureFactory::ChromiumCameraInfo::GetOrientation()
{
    if (IsSpecialCamera(mId)) {
        return ICameraInfo::CAMERA_FACING_BACK;
    }
    else {
        Int orientation;
        return (mCameraInfo == NULL ? 0 : (mCameraInfo->GetOrientation(&orientation), orientation));
    }
}

AutoPtr<ICameraInfo> VideoCaptureFactory::ChromiumCameraInfo::GetCameraInfo(
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

//===============================================================
//                      VideoCaptureFactory
//===============================================================

// Factory methods.
//@CalledByNative
AutoPtr<VideoCapture> VideoCaptureFactory::CreateVideoCapture(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [in] */ Int64 nativeVideoCaptureDeviceAndroid)
{
  if (ChromiumCameraInfo::IsSpecialCamera(id)) {
      AutoPtr<VideoCapture> vc = new VideoCaptureTango(context, ChromiumCameraInfo::ToSpecialCameraId(id),
              nativeVideoCaptureDeviceAndroid);
      return vc;
  }
  else {
      AutoPtr<VideoCapture> vc = new VideoCaptureAndroid(context, id,
              nativeVideoCaptureDeviceAndroid);
      return vc;
  }
}

//@CalledByNative
AutoPtr< ArrayOf<VideoCapture::CaptureFormat> > VideoCaptureFactory::GetDeviceSupportedFormats(
    /* [in] */ Int32 id)
{
    return ChromiumCameraInfo::IsSpecialCamera(id) ?
            VideoCaptureTango::GetDeviceSupportedFormats(
                    ChromiumCameraInfo::ToSpecialCameraId(id)) :
            VideoCaptureAndroid::GetDeviceSupportedFormats(id);
}

//@CalledByNative
Int32 VideoCaptureFactory::GetCaptureFormatWidth(
    /* [in] */ VideoCapture::CaptureFormat* format)
{
    return format->GetWidth();
}

//@CalledByNative
Int32 VideoCaptureFactory::GetCaptureFormatHeight(
    /* [in] */ VideoCapture::CaptureFormat format)
{
    return format->GetHeight();
}

//@CalledByNative
Int32 VideoCaptureFactory::GetCaptureFormatFramerate(
    /* [in] */ VideoCapture::CaptureFormat* format)
{
    return format->GetFramerate();
}

//@CalledByNative
Int32 VideoCaptureFactory::GetCaptureFormatPixelFormat(
    /* [in] */ VideoCapture::CaptureFormat* format)
{
    return format->GetPixelFormat();
}

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
