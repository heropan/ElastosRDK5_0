
#include "ext/frameworkdef.h"
#include "CMobileLinkQualityInfoHelper.h"
#include "MobileLinkQualityInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(CMobileLinkQualityInfoHelper, Singleton, IMobileLinkQualityInfoHelper)

CAR_SINGLETON_IMPL(CMobileLinkQualityInfoHelper)

ECode CMobileLinkQualityInfoHelper::CreateFromParcelBody(
        /* [in] */ IParcel* parcel,
        /* [out] */ IMobileLinkQualityInfo** result)
{
    return MobileLinkQualityInfo::CreateFromParcelBody(parcel, result);
}

} // namespace Net
} // namespace Droid
} // namespace Elastos

