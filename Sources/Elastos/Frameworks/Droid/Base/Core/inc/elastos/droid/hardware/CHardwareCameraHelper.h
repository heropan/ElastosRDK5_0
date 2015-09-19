
#ifndef  __ELASTOS_DROID_HARDWARE_CCAMERAHELPER_H__
#define  __ELASTOS_DROID_HARDWARE_CCAMERAHELPER_H__

#include "_Elastos_Droid_Hardware_CHardwareCameraHelper.h"
#include <elastos/core/Singleton.h>

namespace Elastos {
namespace Droid {
namespace Hardware {

CarClass(CHardwareCameraHelper)
    , public Singleton
    , public IHardwareCameraHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

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

#endif //__ELASTOS_DROID_HARDWARE_CCAMERAHELPER_H__
