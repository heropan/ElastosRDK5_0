
#ifndef __ELASTOS_DROID_HARDWARE_CAMERAINFO_H__
#define __ELASTOS_DROID_HARDWARE_CAMERAINFO_H__

#include "Elastos.Droid.Core_server.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/hardware/HardwareCamera.h"
#include <elastos/core/Object.h>
#include <camera/Camera.h>

using Elastos::Droid::Content::IContext;
using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Hardware {

class CameraInfo
    : public Object
    , public IParcelable
{
public:
    CAR_INTERFACE_DECL()

    CameraInfo();

    ~CameraInfo();

    //@Override
    CARAPI DescribeContents(
        /* [out] */ Int32* contents);

    //@Override
    CARAPI WriteToParcel(
    /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
    /* [in] */ IParcel* source);


    // public static final Parcelable.Creator<CameraInfo> CREATOR =
    //         new Parcelable.Creator<CameraInfo>() {
    //     @Override
    //     public CameraInfo createFromParcel(Parcel in) {
    //         CameraInfo info = new CameraInfo();
    //         info.readFromParcel(in);

    //         return info;
    //     }

    //     @Override
    //     public CameraInfo[] newArray(int size) {
    //         return new CameraInfo[size];
    //     }
    // };

public:
// Can't parcel nested classes, so make this a top level class that composes
    // CameraInfo.
    //AutoPtr<Camera::CameraInfo> info;
    AutoPtr<HardwareCamera::CameraInfo> info;
};

} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_HARDWARE_CAMERAINFO_H__
