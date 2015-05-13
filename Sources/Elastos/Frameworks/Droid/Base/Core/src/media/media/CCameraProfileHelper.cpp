
#include "media/CCameraProfileHelper.h"
#include "util/ArrayUtils.h"
#include "media/MediaProfiles.h"
#include "hardware/CHardwareCameraHelper.h"
#include "hardware/CHardwareCamera.h"

using Elastos::Droid::Internal::Utility::ArrayUtils;
using Elastos::Droid::Hardware::IHardwareCameraHelper;
using Elastos::Droid::Hardware::CHardwareCameraHelper;
using Elastos::Droid::Hardware::CHardwareCamera;
using Elastos::Droid::Hardware::ICameraInfo;

namespace Elastos {
namespace Droid {
namespace Media {
/**
 * Returns a pre-defined still image capture (jpeg) quality level
 * used for the given quality level in the Camera application for
 * the first back-facing camera on the device. If the device has no
 * back-facing camera, this returns 0.
 *
 * @param quality The target quality level
 */
ECode CCameraProfileHelper::GetJpegEncodingQualityParameter(
    /* [in] */ Int32 quality,
    /* [out] */ Int32* result)
{
    AutoPtr<IHardwareCameraHelper> hardwareCameraHelper;
    CHardwareCameraHelper::AcquireSingleton((IHardwareCameraHelper**)&hardwareCameraHelper);
    Int32 numberOfCameras;
    hardwareCameraHelper->GetNumberOfCameras(&numberOfCameras);

    AutoPtr<ICameraInfo> cameraInfo = new CHardwareCamera::CameraInfo();

    for (Int32 i = 0; i < numberOfCameras; i++) {
        hardwareCameraHelper->GetCameraInfo(i, cameraInfo);
        Int32 facing;
        cameraInfo->GetFacing(&facing);
        if (facing == ICameraInfo::CAMERA_FACING_BACK) {
            GetJpegEncodingQualityParameterEx(i, quality, result);
            return NOERROR;
        }
    }
    *result = 0;
    return NOERROR;
}

/**
 * Returns a pre-defined still image capture (jpeg) quality level
 * used for the given quality level in the Camera application for
 * the specified camera.
 *
 * @param cameraId The id of the camera
 * @param quality The target quality level
 */
ECode CCameraProfileHelper::GetJpegEncodingQualityParameterEx(
    /* [in] */ Int32 cameraId,
    /* [in] */ Int32 quality,
    /* [out] */ Int32* result)
{
    if (quality < ICameraProfile::QUALITY_LOW || quality > ICameraProfile::QUALITY_HIGH) {
        //throw new IllegalArgumentException("Unsupported quality level: " + quality);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(mCacheLock);
    AutoPtr<ArrayOf<Int32> > levels = sCache[cameraId];
    if (levels == NULL) {
        levels = GetImageEncodingQualityLevels(cameraId);
        sCache[cameraId] = levels;
    }
    *result = (*levels)[quality];
    return NOERROR;
}

AutoPtr<ArrayOf<Int32> > CCameraProfileHelper::GetImageEncodingQualityLevels(
    /* [in] */ Int32 cameraId)
{
    Int32 nLevels = NativeGetNumImageEncodingQualityLevels(cameraId);
    if (nLevels != ICameraProfile::QUALITY_HIGH + 1) {
        //throw new RuntimeException("Unexpected Jpeg encoding quality levels " + nLevels);
        return NULL;
    }

    AutoPtr<ArrayOf<Int32> > levels = ArrayOf<Int32>::Alloc(nLevels);
    for (Int32 i = 0; i < nLevels; ++i) {
        (*levels)[i] = NativeGetImageEncodingQualityLevel(cameraId, i);
    }
    ArrayUtils::Sort(levels);  // Lower quality level ALWAYS comes before higher one
    return levels;
}

static Mutex sLock;
android::MediaProfiles* sProfiles = NULL;

// Methods implemented by JNI
void CCameraProfileHelper::NativeInit()
{
    ALOGV("native_init");
    Mutex::Autolock lock(sLock);

    if (sProfiles == NULL) {
        sProfiles = android::MediaProfiles::getInstance();
    }
    return;
}

Int32 CCameraProfileHelper::NativeGetNumImageEncodingQualityLevels(
    /* [in] */ Int32 cameraId)
{
    ALOGV("NativeGetNumImageEncodingQualityLevels");
    return sProfiles->getImageEncodingQualityLevels(cameraId).size();
}

Int32 CCameraProfileHelper::NativeGetImageEncodingQualityLevel(
    /* [in] */ Int32 cameraId,
    /* [in] */ Int32 index)
{
    ALOGV("NativeGetImageEncodingQualityLevel");
    android::Vector<int> levels = sProfiles->getImageEncodingQualityLevels(cameraId);
    if (index < 0 || index >= levels.size()) {
        // jniThrowException(env, "java/lang/IllegalArgumentException", "out of array boundary");
        return -1;
    }
    return levels[index];
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
