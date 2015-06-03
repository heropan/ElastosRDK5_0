
#ifndef __CINET6ADDRESS_H__
#define __CINET6ADDRESS_H__

#include "_CInet6Address.h"
#include "InetAddress.h"

namespace Elastos {
namespace Net {

CarClass(CInet6Address) , public InetAddress
{
public:
    CInet6Address();

    CInet6Address(
        /* [in] */ ArrayOf<Byte>* address,
        /* [in] */ const String& name,
        /* [in] */ Int32 scopeId);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    static CARAPI GetByAddress(
        /* [in] */ const String& host,
        /* [in] */ ArrayOf<Byte>* addr,
        /* [in] */ Int32 scopeId,
        /* [out] */ IInet6Address** address);

    static CARAPI GetByAddress(
        /* [in] */ const String& host,
        /* [in] */ ArrayOf<Byte>* addr,
        /* [in] */ INetworkInterface* nif,
        /* [out] */ IInet6Address** address);

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

    CARAPI IsReachable(
        /* [in] */ INetworkInterface* networkInterface,
        /* [in] */ Int32 ttl,
        /* [in] */ Int32 timeout,
        /* [out] */ Boolean* isReachable);

    CARAPI GetScopeId(
        /* [out] */ Int32* scopeId);

    CARAPI GetScopedInterface(
        /* [out] */ INetworkInterface** scopedInterface);

    CARAPI IsIPv4CompatibleAddress(
        /* [out] */ Boolean* isIPv4Compatible);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* address,
        /* [in] */ const String& name,
        /* [in] */ Int32 scopeId);

    CARAPI Equals(
        /*[in] */ IInterface* obj,
        /* [out] */ Boolean* result);

    CARAPI ToString(
        /* [out] */ String* result);

    static CARAPI GetANY(
        /* [out] */ IInetAddress** result);

    static CARAPI GetLOOPBACK(
        /* [out] */ IInetAddress** result);

private:
    CARAPI_(Boolean) CompareLocalType(
        /* [in] */ IInet6Address* ia);

public:
    static AutoPtr<IInetAddress> ANY;
    static AutoPtr<IInetAddress> LOOPBACK;

    Int32 mScopeId;

    Boolean mScopeIdSet;

    Boolean mScopeIfnameSet;

    String mIfname;

    AutoPtr<INetworkInterface> mScopedIf;

private:
    static const Int32 AF_INET6 = 10;

//    private static final ObjectStreamField[] serialPersistentFields = {
//            new ObjectStreamField("ipaddress", new byte[0].getClass()),
//            new ObjectStreamField("scope_id", Integer.TYPE),
//            new ObjectStreamField("scope_id_set", Boolean.TYPE),
//            new ObjectStreamField("scope_ifname_set", Boolean.TYPE),
//            new ObjectStreamField("ifname", String.class), };
};

} // namespace Net
} // namespace Elastos

#endif //__CINET6ADDRESS_H__
