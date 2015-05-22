#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include "Elastos.CoreLibrary_server.h"
#include "AddressCache.h"
#include "ThreadBase.h"

using Libcore::IO::IOs;
using Elastos::Core::Threading::ThreadBase;
using Elastos::Utility::Concurrent::Atomic::IAtomicBoolean;
using Elastos::Utility::Concurrent::ICountDownLatch;

namespace Elastos {
namespace Net {

extern const InterfaceID EIID_InetAddress;

class InetAddress
{
protected:
    class _InetAddressThread : public ThreadBase
    {
    public:
        _InetAddressThread(
            /* [in] */ InetAddress* host,
            /* [in] */ IInetAddress* sourceAddress,
            /* [in] */ Int32 timeout,
            /* [in] */ ICountDownLatch* latch,
            /* [in] */ IAtomicBoolean* isReachable);

        CARAPI Run();

    private:
        InetAddress* mHost;
        AutoPtr<IInetAddress> mSourceAddress;
        AutoPtr<ICountDownLatch> mLatch;
        AutoPtr<IAtomicBoolean> mIsReachable;
        Int32 mTimeout;
    };

public:
    InetAddress(
        /* [in] */ Int32 family,
        /* [in] */ ArrayOf<Byte>* ipaddress,
        /* [in] */ const String& hostName);

    CARAPI Init(
        /* [in] */ Int32 family,
        /* [in] */ ArrayOf<Byte>* ipaddress,
        /* [in] */ const String& hostName);

    virtual CARAPI Equals(
        /* [in] */IInterface* obj,
        /* [out] */Boolean* result);

    virtual CARAPI GetAddress(
        /* [out, callee] */ ArrayOf<Byte>** address);

    static CARAPI GetAllByName(
        /* [in] */ const String& host,
        /* [out, callee] */ ArrayOf<IInetAddress*>** addresses);

    static CARAPI GetAllByNameImpl(
        /* [in] */ const String& host,
        /* [out, callee] */ ArrayOf<IInetAddress*>** addresses);

    static CARAPI GetByName(
        /* [in] */ const String& host,
        /* [out] */ IInetAddress** address);

    virtual CARAPI GetHostAddress(
        /* [out] */ String* address);

    virtual CARAPI GetHostName(
        /* [out] */ String* name);

    virtual CARAPI GetCanonicalHostName(
        /* [out] */ String* canonicalName);

    virtual CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    static CARAPI GetLocalHost(
        /* [out] */ IInetAddress** address);

    static CARAPI ClearDnsCache();

    virtual CARAPI IsMulticastAddress(
        /* [out] */ Boolean* isMulticastAddress);

    CARAPI ToString(
        /* [out] */String* result);

    static CARAPI IsNumeric(
        /* [in] */ const String& address,
        /* [out] */ Boolean* result);

    static CARAPI ParseNumericAddress(
        /* [in] */ const String& numericAddress,
        /* [out] */ IInetAddress** result);

    static CARAPI GetLoopbackAddress(
        /* [out] */ IInetAddress** address);

    virtual CARAPI IsLoopbackAddress(
        /* [out] */ Boolean* isLoopbackAddress);

    virtual CARAPI IsLinkLocalAddress(
        /* [out] */ Boolean* isLinkLocalAddress);

    virtual CARAPI IsSiteLocalAddress(
        /* [out] */ Boolean* isSiteLocalAddress);

    virtual CARAPI IsMCGlobal(
        /* [out] */ Boolean* isMCGlobal);

    virtual CARAPI IsMCNodeLocal(
        /* [out] */ Boolean* isMCNodeLocal);

    virtual CARAPI IsMCLinkLocal(
        /* [out] */ Boolean* isMCLinkLocal);

    virtual CARAPI IsMCSiteLocal(
        /* [out] */ Boolean* isMCSiteLocal);

    virtual CARAPI IsMCOrgLocal(
        /* [out] */ Boolean* isMCOrgLocal);

    virtual CARAPI IsAnyLocalAddress(
        /* [out] */ Boolean* isAnyLocalAddress);

    virtual CARAPI IsReachable(
        /* [in] */ Int32 timeout,
        /* [out] */ Boolean* isReachable);

    virtual CARAPI IsReachableEx(
        /* [in] */ INetworkInterface* networkInterface,
        /* [in] */ Int32 ttl,
        /* [in] */ Int32 timeout,
        /* [out] */ Boolean* isReachable);

    static CARAPI GetByAddress(
        /* [in] */ ArrayOf<Byte>* ipAddress,
        /* [out] */ IInetAddress** address);

    static CARAPI GetByAddressEx(
        /* [in] */ const String& hostName,
        /* [in] */ ArrayOf<Byte>* ipAddress,
        /* [out] */ IInetAddress** address);

    static CARAPI GetByAddress(
        /* [in] */ const String& hostName,
        /* [in] */ ArrayOf<Byte>* ipaddress,
        /* [in] */ Int32 scopeId,
        /* [out] */ IInetAddress** address);

protected:
    InetAddress() : mFamily(0) {}

    static CARAPI BytesToInetAddresses(
        /* [in] */ ArrayOf<ByteArray>* rawAddresses,
        /* [in] */ const String& hostName,
        /* [out, callee] */ ArrayOf<IInetAddress*>** addresses);

    static CARAPI GetHostByAddrImpl(
        /* [in] */ InetAddress* address,
        /* [out] */ IInetAddress** result);

    static CARAPI_(Int32) BytesToInt32(
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 start);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

private:
    static CARAPI LookupHostByName(
        /* [in] */ const String& host,
        /* [out, callee] */ ArrayOf<IInetAddress*>** addresses);

    CARAPI_(Boolean) IsReachable(
        /* [in] */ InetAddress* destination,
        /* [in] */ IInetAddress* source,
        /* [in] */ Int32 timeout);

    static CARAPI_(Boolean) IsIPv4MappedAddress(
        /* [in] */ ArrayOf<Byte>* ipAddress);

    static CARAPI IPv4MappedToIPv4(
        /* [in] */ ArrayOf<Byte>* mappedAddress,
        /* [out, callee] */ ArrayOf<Byte>** ipv4Address);

//    private void writeObject(ObjectOutputStream stream);

//    private void readObject(ObjectInputStream stream);

    static CARAPI MakeInetAddress(
        /* [in] */ ArrayOf<Byte>* bytes,
        /* [in] */ const String& hostName,
        /* [out] */ IInetAddress** address);

    static CARAPI_(AutoPtr<IInetAddress>) DisallowDeprecatedFormats(
        /* [in] */ const String& address,
        /* [in] */ IInetAddress* inetAddress);

    static CARAPI_(AutoPtr<IInetAddress>) ParseNumericAddressNoThrow(
        /* [in] */ const String& address);

    static CARAPI LoopbackAddresses(
        /* [out] */ ArrayOf<IInetAddress*>** result);

public:
    String mHostName;
    Int32 mFamily;
    AutoPtr<ArrayOf<Byte> > mIpAddress;

    static AutoPtr<IInetAddress> UNSPECIFIED;

private:
    /** Our Java-side DNS cache. */
    static AutoPtr<AddressCache> ADDRESS_CACHE;
    static AutoPtr<IOs> sOs;
};

} // namespace Net
} // namespace Elastos

#endif //__INETADDRESS_H__
