
#ifndef __ELASTOS_DROID_NET_CLINKADDRESS_H__
#define __ELASTOS_DROID_NET_CLINKADDRESS_H__

#include "_CLinkAddress.h"

using Elastos::Net::IInetAddress;
using Elastos::Net::IInterfaceAddress;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CLinkAddress)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 prefixLength);

    CARAPI constructor(
        /* [in] */ IInterfaceAddress* interfaceAddress);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI EqualsEx(
        /* [in] */ ILinkAddress* obj,
        /* [out] */ Boolean* result);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* result);

    CARAPI GetAddress(
        /* [out] */ IInetAddress** result);

    CARAPI GetNetworkPrefixLength(
        /* [out] */ Int32* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    /**
     * IPv4 or IPv6 address.
     */
    AutoPtr<IInetAddress> mAddress;

    /**
     * Network prefix length
     */
    Int32 mPrefixLength;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_NET_CLINKADDRESS_H__
