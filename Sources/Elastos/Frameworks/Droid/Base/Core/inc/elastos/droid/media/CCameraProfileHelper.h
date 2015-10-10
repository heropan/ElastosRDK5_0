
#ifndef __ELASTOS_DROID_MEDIA_CCAMERAPROFILEHELPER_H__
#define __ELASTOS_DROID_MEDIA_CCAMERAPROFILEHELPER_H__

#include "_Elastos_Droid_Media_CCameraProfileHelper.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
namespace Elastos {
namespace Droid {
namespace Media {
/**
 * The CameraProfile class is used to retrieve the pre-defined still image
 * capture (jpeg) quality levels (0-100) used for low, medium, and high
 * quality settings in the Camera application.
 *
 */
CarClass(CCameraProfileHelper)
{
public:
    /**
     * Returns a pre-defined still image capture (jpeg) quality level
     * used for the given quality level in the Camera application for
     * the first back-facing camera on the device. If the device has no
     * back-facing camera, this returns 0.
     *
     * @param quality The target quality level
     */
    CARAPI GetJpegEncodingQualityParameter(
        /* [in] */ Int32 quality,
        /* [out] */ Int32* result);

    /**
     * Returns a pre-defined still image capture (jpeg) quality level
     * used for the given quality level in the Camera application for
     * the specified camera.
     *
     * @param cameraId The id of the camera
     * @param quality The target quality level
     */
    CARAPI GetJpegEncodingQualityParameter(
        /* [in] */ Int32 cameraId,
        /* [in] */ Int32 quality,
        /* [out] */ Int32* result);

private:
    CARAPI_(AutoPtr<ArrayOf<Int32> >) GetImageEncodingQualityLevels(
        /* [in] */ Int32 cameraId);

    // Methods implemented by JNI
    CARAPI_(void) NativeInit();

    CARAPI_(Int32) NativeGetNumImageEncodingQualityLevels(
        /* [in] */ Int32 cameraId);

    CARAPI_(Int32) NativeGetImageEncodingQualityLevel(
        /* [in] */ Int32 cameraId,
        /* [in] */ Int32 index);

private:
    /*
     * Cache the Jpeg encoding quality parameters
     */
    HashMap<Int32, AutoPtr<ArrayOf<Int32> > > sCache;
    Object mCacheLock;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif
