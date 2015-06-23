
#ifndef  __CHARDWARE_CCAMERAHELPER_H__
#define  __CHARDWARE_CCAMERAHELPER_H__


#include "_CHardwareCameraHelper.h"


namespace Elastos {
namespace Droid {
namespace Hardware {


CarClass(CHardwareCameraHelper)
{
public:
    CARAPI GetCameraInfo(
        /* [in] */ Int32 cameraId,
        /* [in] */ ICameraInfo* cameraInfo);

    CARAPI Open(
        /* [in] */ Int32 cameraId,
        /* [out] */ IHardwareCamera** camera);

    CARAPI Open(
        /* [out] */ IHardwareCamera** camera);

    CARAPI GetEmptyParameters(
        /* [out] */ IParameters** para);

    /**
     * Returns the number of physical cameras available on this device.
     */
    /*public native*/
    CARAPI GetNumberOfCameras(
        /* [out] */ Int32* num);

    CARAPI IsPlayingCamera(
        /* [out] */ Boolean* para);

private:
    /*native*/ static void _getCameraInfo(
        /* [in] */ Int32 cameraId,
        /* [in] */ ICameraInfo* cameraInfo);
};

} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif //__CHARDWARE_CCAMERAHELPER_H__
