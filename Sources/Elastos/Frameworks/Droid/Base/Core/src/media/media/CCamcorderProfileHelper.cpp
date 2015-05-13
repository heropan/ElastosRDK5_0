
#include "media/CCamcorderProfileHelper.h"
#include "media/CCamcorderProfile.h"

namespace Elastos {
namespace Droid {
namespace Media {

/*static*/
ECode CCamcorderProfileHelper::Get(
    /* [in] */ Int32 quality,
    /* [out] */ ICamcorderProfile** result)
{
    return CCamcorderProfile::Get(quality, result);
}

/*static*/
ECode CCamcorderProfileHelper::GetEx(
    /* [in] */ Int32 cameraId,
    /* [in] */ Int32 quality,
    /* [out] */ ICamcorderProfile** result)
{
    return CCamcorderProfile::GetEx(cameraId, quality, result);
}

/*static*/
ECode CCamcorderProfileHelper::HasProfile(
    /* [in] */ Int32 quality,
    /* [out] */ Boolean* result)
{
    return CCamcorderProfile::HasProfile(quality, result);
}

/*static*/
ECode CCamcorderProfileHelper::HasProfileEx(
    /* [in] */ Int32 cameraId,
    /* [in] */ Int32 quality,
    /* [out] */ Boolean* result)
{
    return CCamcorderProfile::HasProfileEx(cameraId, quality, result);
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
