
#ifndef __ELASTOS_DROID_NET_NETWORKQUOTAINFO_H__
#define __ELASTOS_DROID_NET_NETWORKQUOTAINFO_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * Information about quota status on a specific network.
 *
 * @hide
 */
class NetworkQuotaInfo
    : public Object
    , public IParcelable
    , public INetworkQuotaInfo
{
public:
    CAR_INTERFACE_DECL()

    /** {@hide} */
    CARAPI constructor(
        /* [in] */ Int64 estimatedBytes,
        /* [in] */ Int64 softLimitBytes,
        /* [in] */ Int64 hardLimitBytes);

    /** {@hide} */
    CARAPI constructor(
        /* [in] */ IParcel* in);

    CARAPI GetEstimatedBytes(
        /* [out] */ Int64* result);

    CARAPI GetSoftLimitBytes(
        /* [out] */ Int64* result);

    CARAPI GetHardLimitBytes(
        /* [out] */ Int64* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    Int64 mEstimatedBytes;

    Int64 mSoftLimitBytes;

    Int64 mHardLimitBytes;
};

} // namespace Net
} // namespace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_NET_NETWORKQUOTAINFO_H__
