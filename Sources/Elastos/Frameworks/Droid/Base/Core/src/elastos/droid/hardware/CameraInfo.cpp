
#include "elastos/droid/hardware/CameraInfo.h"

namespace Elastos {
namespace Droid {
namespace Hardware {

CAR_INTERFACE_IMPL(CameraInfo, Object, IParcelable)

CameraInfo::CameraInfo()
{
    info = new HardwareCamera::CameraInfo();
}

ECode CameraInfo::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);

    *contents = 0;
    return NOERROR;
}

ECode CameraInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteInt32(info->mFacing))
    FAIL_RETURN(dest->WriteInt32(info->mOrientation))
    return NOERROR;
}

ECode CameraInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(source->ReadInt32(&(info->mFacing)))
    FAIL_RETURN(source->ReadInt32(&(info->mOrientation)))
    return NOERROR;
}

} // namespace Hardware
} // namespace Droid
} // namespace Elastos

