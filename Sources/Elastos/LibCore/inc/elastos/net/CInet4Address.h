
#ifndef __CINET4ADDRESS_H__
#define __CINET4ADDRESS_H__

#include "_CInet4Address.h"
#include "InetAddress.h"

namespace Elastos {
namespace Net {

CarClass(CInet4Address) , public InetAddress
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetAddress(
        /* [out, callee] */ ArrayOf<Byte>** address);

    CARAPI GetHostAddress(
        /* [out] */ String* hostAddress);

    CARAPI GetHostName(
        /* [out] */ String* hostName);

    CARAPI GetCanonicalHostName(
        /* [out] */ String* hostName);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI IsMulticastAddress(
        /* [out] */ Boolean* isMulticastAddress);

    CARAPI IsLoopbackAddress(
        /* [out] */ Boolean* isLoopbackAddress);

    CARAPI IsLinkLocalAddress(
        /* [out] */ Boolean* isLinkLocalAddress);

    CARAPI IsSiteLocalAddress(
        /* [out] */ Boolean* isSiteLocalAddress);

    CARAPI IsMCGlobal(
        /* [out] */ Boolean* isMCGlobal);

    CARAPI IsMCNodeLocal(
        /* [out] */ Boolean* isMCNodeLocal);

    CARAPI IsMCLinkLocal(
        /* [out] */ Boolean* isMCLinkLocal);

    CARAPI IsMCSiteLocal(
        /* [out] */ Boolean* isMCSiteLocal);

    CARAPI IsMCOrgLocal(
        /* [out] */ Boolean* isMCOrgLocal);

    CARAPI IsAnyLocalAddress(
        /* [out] */ Boolean* isAnyLocalAddress);

    CARAPI IsReachable(
        /* [in] */ Int32 timeout,
        /* [out] */ Boolean* isReachable);

    CARAPI IsReachableEx(
        /* [in] */ INetworkInterface* networkInterface,
        /* [in] */ Int32 ttl,
        /* [in] */ Int32 timeout,
        /* [out] */ Boolean* isReachable);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* address,
        /* [in] */ const String& name);

    CARAPI Equals(
        /*[in] */ IInterface* obj,
        /* [out] */ Boolean* result);

    CARAPI ToString(
        /* [out] */ String* result);

    static CARAPI GetANY(
        /* [out] */ IInetAddress** result);

    static CARAPI GetLOOPBACK(
        /* [out] */ IInetAddress** result);

    static CARAPI GetALL(
        /* [out] */ IInetAddress** result);

public:
    static AutoPtr<IInetAddress> ANY;
    static AutoPtr<IInetAddress> LOOPBACK;
    static AutoPtr<IInetAddress> ALL;

private:
    static const Int32 AF_INET = 2;
};

} // namespace Net
} // namespace Elastos

#endif //__CINET4ADDRESS_H__
