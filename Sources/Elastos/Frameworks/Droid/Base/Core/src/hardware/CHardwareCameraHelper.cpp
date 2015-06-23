
#include "hardware/CHardwareCameraHelper.h"
#include "hardware/CHardwareCamera.h"
#include "os/ServiceManager.h"
#include <cutils/properties.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Media::IIAudioService;

namespace Elastos {
namespace Droid {
namespace Hardware {

ECode CHardwareCameraHelper::GetCameraInfo(
    /* [in] */ Int32 cameraId,
    /* [in] */ ICameraInfo* cameraInfo)
{
    _getCameraInfo(cameraId, cameraInfo);

    AutoPtr<IInterface> tmpObj = ServiceManager::GetService(IContext::AUDIO_SERVICE);
    AutoPtr<IIAudioService> audioService = IIAudioService::Probe(tmpObj.Get());

    // try {
    Boolean state = FALSE;
    if (audioService->IsCameraSoundForced(&state), state) {
        // Only set this when sound is forced; otherwise let native code
        // decide.
        ((CHardwareCamera::CameraInfo*)cameraInfo)->mCanDisableShutterSound = FALSE;
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Audio service is unavailable for queries");
    // }
    return NOERROR;
}

ECode CHardwareCameraHelper::Open(
    /* [in] */ Int32 cameraId,
    /* [out] */ IHardwareCamera** camera)
{
    return CHardwareCamera::New(cameraId, camera);
}

ECode CHardwareCameraHelper::Open(
    /* [out] */ IHardwareCamera** camera)
{
    assert(camera != NULL);
    *camera = NULL;

    Int32 numberOfCameras = 0;
    GetNumberOfCameras(&numberOfCameras);
    AutoPtr<ICameraInfo> cameraInfo = new CHardwareCamera::CameraInfo();
    for (Int32 i = 0; i < numberOfCameras; i++) {
        GetCameraInfo(i, cameraInfo);
        if (((CHardwareCamera::CameraInfo*)cameraInfo.Get())->mFacing == ICameraInfo::CAMERA_FACING_BACK) {
            return CHardwareCamera::New(i, camera);
        }
    }

    return NOERROR;
}

ECode CHardwareCameraHelper::GetEmptyParameters(
    /* [out] */ IParameters** para)
{
    VALIDATE_NOT_NULL(para);
    AutoPtr<IHardwareCamera> camera;
    CHardwareCamera::New((IHardwareCamera**)&camera);

    *para = new CHardwareCamera::Parameters();
    REFCOUNT_ADD(*para);
    return NOERROR;
}

ECode CHardwareCameraHelper::GetNumberOfCameras(
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);
    *num = android::Camera::getNumberOfCameras();
    return NOERROR;
}

ECode CHardwareCameraHelper::IsPlayingCamera(
        /* [out] */ Boolean* para)
{
    VALIDATE_NOT_NULL(para);
    bool value;
    android::Camera::isPlayingCamera(&value);
    *para = value;
    return NOERROR;
}

void CHardwareCameraHelper::_getCameraInfo(
    /* [in] */ Int32 cameraId,
    /* [in] */ ICameraInfo* info_obj)
{
    android::CameraInfo cameraInfo;
    ECode rc = android::Camera::getCameraInfo(cameraId, &cameraInfo);
    if (rc != NOERROR) {
        assert(0);
        // jniThrowRuntimeException(env, "Fail to get camera info");
        return;
    }

    // env->SetIntField(info_obj, fields.facing, cameraInfo.facing);
    // env->SetIntField(info_obj, fields.orientation, cameraInfo.orientation);
    ((CHardwareCamera::CameraInfo*)info_obj)->mFacing = cameraInfo.facing;
    ((CHardwareCamera::CameraInfo*)info_obj)->mOrientation = cameraInfo.orientation;

    char value[PROPERTY_VALUE_MAX];
    property_get("ro.camera.sound.forced", value, "0");
    Boolean canDisableShutterSound = (strncmp(value, "0", 2) == 0);
    // env->SetBooleanField(info_obj, fields.canDisableShutterSound,
    //         canDisableShutterSound);

    ((CHardwareCamera::CameraInfo*)info_obj)->mCanDisableShutterSound = canDisableShutterSound;
}


} // namespace Hardware
} // namespace Droid
} // namespace Elastos
