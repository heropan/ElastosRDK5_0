
#ifndef __ELASTOS_DROID_NET_NETWORKREQUEST_H__
#define __ELASTOS_DROID_NET_NETWORKREQUEST_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * Defines a request for a network, made through {@link NetworkRequest.Builder} and used
 * to request a network via {@link ConnectivityManager#requestNetwork} or listen for changes
 * via {@link ConnectivityManager#registerNetworkCallback}.
 */
class NetworkRequest
    : public Object
    , public IParcelable
    , public INetworkRequest
{
public:
    CAR_INTERFACE_DECL()

    /**
     * @hide
     */
    CARAPI constructor(
        /* [in] */ INetworkCapabilities* nc,
        /* [in] */ Int32 legacyType,
        /* [in] */ Int32 rId);

    /**
     * @hide
     */
    CARAPI constructor(
        /* [in] */ INetworkRequest* that);

    // implement the Parcelable interface
    CARAPI DescribeContents(
        /* [out] */ Int32* result);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest,
        /* [in] */ Int32 flags);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI Equals(
        /* [in] */ IObject* obj,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* result);

    /**
     * The {@link NetworkCapabilities} that define this request.
     * @hide
     */
    CARAPI GetNetworkCapabilities(
        /* [out] */ INetworkCapabilities** result);

    /**
     * Identifies the request.  NetworkRequests should only be constructed by
     * the Framework and given out to applications as tokens to be used to identify
     * the request.
     * @hide
     */
    CARAPI GetRequestId(
        /* [out] */ Int32* result);

    /**
     * Set for legacy requests and the default.  Set to TYPE_NONE for none.
     * Causes CONNECTIVITY_ACTION broadcasts to be sent.
     * @hide
     */
    CARAPI GetLegacyType(
        /* [out] */ Int32* result);

private:
    /**
     * The {@link NetworkCapabilities} that define this request.
     * @hide
     */
    AutoPtr<INetworkCapabilities> mNetworkCapabilities;

    /**
     * Identifies the request.  NetworkRequests should only be constructed by
     * the Framework and given out to applications as tokens to be used to identify
     * the request.
     * @hide
     */
    Int32 mRequestId;

    /**
     * Set for legacy requests and the default.  Set to TYPE_NONE for none.
     * Causes CONNECTIVITY_ACTION broadcasts to be sent.
     * @hide
     */
    Int32 mLegacyType;

};

} // namespace Net
} // namespace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_NET_NETWORKREQUEST_H__
