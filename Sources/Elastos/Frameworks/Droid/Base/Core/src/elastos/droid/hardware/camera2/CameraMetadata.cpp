
#include "elastos/droid/hardware/camera2/CameraMetadata.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Camera2 {

const String CameraMetadata::TAG("CameraMetadataAb");
const Boolean CameraMetadata::VERBOSE = FALSE;//Log.isLoggable(TAG, Log.VERBOSE);

CAR_INTERFACE_IMPL(CameraMetadata, Object, ICameraMetadata)

CameraMetadata::CameraMetadata()
{
}

ECode CameraMetadata::GetKeys(
    /* [out] */ IList** outlist)
{
    VALIDATE_NOT_NULL(outlist);

    assert(0);
    return NOERROR;
}

} // namespace Camera2
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos