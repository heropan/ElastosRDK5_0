
#ifndef __ELASTOS_DROID_NET_CNETWORKQUOTAINFO_H__
#define __ELASTOS_DROID_NET_CNETWORKQUOTAINFO_H__

#include "_CNetworkQuotaInfo.h"
#include <elastos/Mutex.h>

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkQuotaInfo)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int64 estimatedBytes,
        /* [in] */ Int64 softLimitBytes,
        /* [in] */ Int64 hardLimitBytes);

    CARAPI GetEstimatedBytes(
        /* [out] */ Int64* result);

    CARAPI GetSoftLimitBytes(
        /* [out] */ Int64* result);

    CARAPI GetHardLimitBytes(
        /* [out] */ Int64* result);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

private:
    Int64 mEstimatedBytes;
    Int64 mSoftLimitBytes;
    Int64 mHardLimitBytes;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CNETWORKQUOTAINFO_H__

