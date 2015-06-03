
#include "InetAddress.h"
#include "CInetAddress.h"
#include "CInet4Address.h"
#include "CInet6Address.h"
#include "CIoBridge.h"
#include "CBlockGuard.h"
#include "NetworkUtilities.h"
#include "CStructAddrinfo.h"
#include "CLibcore.h"
#include "CArrays.h"
#include "CCountDownLatch.h"
#include "CAtomicBoolean.h"
#include "CCollections.h"
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

using Libcore::IO::IStructAddrinfo;
using Libcore::IO::CStructAddrinfo;
using Libcore::IO::ILibcore;
using Libcore::IO::CLibcore;
using Libcore::IO::IIoBridge;
using Libcore::IO::CIoBridge;
using Libcore::IO::IStructUtsname;
using Elastos::Core::IBlockGuard;
using Elastos::Core::CBlockGuard;
using Elastos::Core::IBlockGuardPolicy;
using Elastos::Utility::IArrays;
using Elastos::Utility::CArrays;
using Elastos::Utility::IEnumeration;
using Elastos::Utility::ICollections;
using Elastos::Utility::CCollections;
using Elastos::Utility::Concurrent::CCountDownLatch;
using Elastos::Utility::Concurrent::Atomic::CAtomicBoolean;

namespace Elastos {
namespace Net {

// {6E01A794-9ED6-4D1C-BBD5-A9D8855C9D42}
const InterfaceID EIID_InetAddress =
    { 0x6e01a794, 0x9ed6, 0x4d1c, { 0xbb, 0xd5, 0xa9, 0xd8, 0x85, 0x5c, 0x9d, 0x42 } };

AutoPtr<AddressCache> InetAddress::ADDRESS_CACHE = new AddressCache();
AutoPtr<IOs> InetAddress::sOs = CLibcore::sOs;
AutoPtr<IInetAddress> GetInit()
{
    AutoPtr<CInetAddress> res;
    CInetAddress::NewByFriend(AF_UNSPEC, NULL, String(NULL), (CInetAddress**)&res);
    return (IInetAddress*)res.Get();
}

AutoPtr<IInetAddress> InetAddress::UNSPECIFIED;

InetAddress::InetAddress(
    /* [in] */ Int32 family,
    /* [in] */ ArrayOf<Byte>* ipaddress,
    /* [in] */ const String& hostName)
    : mHostName(hostName)
    , mFamily(family)
    , mIpAddress(ipaddress)
{}

ECode InetAddress::Init(
        /* [in] */ Int32 family,
        /* [in] */ ArrayOf<Byte>* ipaddress,
        /* [in] */ const String& hostName)
{
    mHostName = hostName;
    mFamily = family;
    mIpAddress = ipaddress;
    return NOERROR;
}

ECode InetAddress::GetAddress(
    /* [out, callee] */ ArrayOf<Byte>** address)
{
    VALIDATE_NOT_NULL(address);

    if (mIpAddress) {
        *address = mIpAddress->Clone();
        REFCOUNT_ADD(*address);
    }
    return NOERROR;
}

ECode InetAddress::GetAllByName(
    /* [in] */ const String& host,
    /* [out, callee] */ ArrayOf<IInetAddress*>** addresses)
{
    VALIDATE_NOT_NULL(addresses)

    AutoPtr< ArrayOf<IInetAddress*> > addrs;
    FAIL_RETURN(GetAllByNameImpl(host, (ArrayOf<IInetAddress*>**)&addrs));
    *addresses = addrs->Clone();
    REFCOUNT_ADD(*addresses);
    return NOERROR;
}

ECode InetAddress::GetAllByNameImpl(
    /* [in] */ const String& host,
    /* [out, callee] */ ArrayOf<IInetAddress*>** addresses)
{
    VALIDATE_NOT_NULL(addresses)

    if (host.IsNullOrEmpty()) {
        return LoopbackAddresses(addresses);
    }

    //Is it a numeric address?
    AutoPtr<IInetAddress> result = ParseNumericAddressNoThrow(host);
    if (result != NULL) {
        result = DisallowDeprecatedFormats(host, result);
        if (result == NULL) {
            return E_UNKNOWN_HOST_EXCEPTION;
        }
        AutoPtr< ArrayOf<IInetAddress*> > addrs = ArrayOf<IInetAddress*>::Alloc(1);
        addrs->Set(0, result);
        *addresses = addrs;
        REFCOUNT_ADD(*addresses);
        return NOERROR;
    }

    AutoPtr< ArrayOf<IInetAddress*> > addrs;
    FAIL_RETURN(LookupHostByName(host, (ArrayOf<IInetAddress*>**)&addrs));
    *addresses = addrs->Clone();
    REFCOUNT_ADD(*addresses);
    return NOERROR;
}

ECode InetAddress::GetByName(
    /* [in] */ const String& host,
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    AutoPtr< ArrayOf<IInetAddress*> > addresses;
    FAIL_RETURN(GetAllByNameImpl(host, (ArrayOf<IInetAddress*>**)&addresses));
    *address = (*addresses)[0];
    REFCOUNT_ADD(*address);
    return NOERROR;
}

ECode InetAddress::GetHostAddress(
    /* [out] */ String* address)
{
    VALIDATE_NOT_NULL(address);

    return sOs->Getnameinfo((IInetAddress*)this->Probe(EIID_IInetAddress), NI_NUMERICHOST, address);
}

ECode InetAddress::GetHostName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    if(mHostName.IsNull()) {
        AutoPtr<IInetAddress> addr;
        if(SUCCEEDED(GetHostByAddrImpl(this, (IInetAddress**)&addr))) {
            InetAddress* inetAddress = reinterpret_cast<InetAddress*>(addr->Probe(EIID_InetAddress));
            mHostName = inetAddress->mHostName;
        }
        else{
            GetHostAddress(&mHostName);
        }
    }
    *name = mHostName;
    return NOERROR;
}

ECode InetAddress::GetCanonicalHostName(
    /* [out] */ String* canonicalName)
{
    VALIDATE_NOT_NULL(canonicalName);

    AutoPtr<IInetAddress> addr;
    if(SUCCEEDED(GetHostByAddrImpl(this, (IInetAddress**)&addr))) {
        InetAddress* inetAddress = reinterpret_cast<InetAddress*>(addr->Probe(EIID_InetAddress));
        *canonicalName = inetAddress->mHostName;
    }
    else{
        GetHostAddress(canonicalName);
    }
    return NOERROR;
}

ECode InetAddress::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)

    AutoPtr<IArrays> arrays;
    CArrays::AcquireSingleton((IArrays**)&arrays);
    return arrays->HashCodeByte(mIpAddress, hashCode);
}

ECode InetAddress::GetLocalHost(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    AutoPtr<IStructUtsname> isuname;
    sOs->Uname((IStructUtsname**)&isuname);
    String host;
    isuname->GetNodename(&host);
    AutoPtr<ArrayOf<IInetAddress*> > addresses;
    FAIL_RETURN(LookupHostByName(host, (ArrayOf<IInetAddress*>**)&addresses));
    *address = (*addresses)[0];
    REFCOUNT_ADD(*address);
    return NOERROR;
}

ECode InetAddress::ClearDnsCache()
{
    ADDRESS_CACHE->Clear();
    return NOERROR;
}

/**
 * Gets the hashcode of the represented IP address.
 *
 * @return the appropriate hashcode value.
 */
//    @Override
//    public int hashCode();

ECode InetAddress::IsMulticastAddress(
    /* [out] */ Boolean* isMulticastAddress)
{
    VALIDATE_NOT_NULL(isMulticastAddress);

    *isMulticastAddress = FALSE;
    return NOERROR;
}

ECode InetAddress::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    String hostAddress;
    GetHostAddress(&hostAddress);
    if (!mHostName.IsNull()) {
        *result = mHostName + String("/") + hostAddress;
    }
    else {
        *result = String("/") +hostAddress;
    }
    return NOERROR;
}

ECode InetAddress::IsNumeric(
    /* [in] */ const String& address,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IInetAddress> inetAddress = ParseNumericAddressNoThrow(address);
    AutoPtr<IInetAddress> formats = DisallowDeprecatedFormats(address, inetAddress);
    *result = (inetAddress != NULL) && formats != NULL;
    return NOERROR;
}

ECode InetAddress::ParseNumericAddress(
    /* [in] */ const String& numericAddress,
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(result);

    if (numericAddress.IsNullOrEmpty()) {
        *result = CInet6Address::LOOPBACK;
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
    AutoPtr<IInetAddress> resultTemp = ParseNumericAddressNoThrow(numericAddress);
    resultTemp = DisallowDeprecatedFormats(numericAddress, resultTemp);
    if (resultTemp == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *result = resultTemp;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode InetAddress::GetLoopbackAddress(
   /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    *address = CInet6Address::LOOPBACK;
    REFCOUNT_ADD(*address)
    return NOERROR;
}

ECode InetAddress::IsLoopbackAddress(
    /* [out] */ Boolean* isLoopbackAddress)
{
    VALIDATE_NOT_NULL(isLoopbackAddress);

    *isLoopbackAddress = FALSE;
    return NOERROR;
}

ECode InetAddress::IsLinkLocalAddress(
    /* [out] */ Boolean* isLinkLocalAddress)
{
    VALIDATE_NOT_NULL(isLinkLocalAddress);

    *isLinkLocalAddress = FALSE;
    return NOERROR;
}

ECode InetAddress::IsSiteLocalAddress(
    /* [out] */ Boolean* isSiteLocalAddress)
{
    VALIDATE_NOT_NULL(isSiteLocalAddress);

    *isSiteLocalAddress = FALSE;
    return NOERROR;
}

ECode InetAddress::IsMCGlobal(
    /* [out] */ Boolean* isMCGlobal)
{
    VALIDATE_NOT_NULL(isMCGlobal);

    *isMCGlobal = FALSE;
    return NOERROR;
}

ECode InetAddress::IsMCNodeLocal(
    /* [out] */ Boolean* isMCNodeLocal)
{
    VALIDATE_NOT_NULL(isMCNodeLocal);

    *isMCNodeLocal = FALSE;
    return NOERROR;
}

ECode InetAddress::IsMCLinkLocal(
    /* [out] */ Boolean* isMCLinkLocal)
{
    VALIDATE_NOT_NULL(isMCLinkLocal);

    *isMCLinkLocal = FALSE;
    return NOERROR;
}

ECode InetAddress::IsMCSiteLocal(
    /* [out] */ Boolean* isMCSiteLocal)
{
    VALIDATE_NOT_NULL(isMCSiteLocal);

    *isMCSiteLocal = FALSE;
    return NOERROR;
}

ECode InetAddress::IsMCOrgLocal(
    /* [out] */ Boolean* isMCOrgLocal)
{
    VALIDATE_NOT_NULL(isMCOrgLocal);

    *isMCOrgLocal = FALSE;
    return NOERROR;
}

ECode InetAddress::IsAnyLocalAddress(
    /* [out] */ Boolean* isAnyLocalAddress)
{
    VALIDATE_NOT_NULL(isAnyLocalAddress);

    *isAnyLocalAddress = FALSE;
    return NOERROR;
}

ECode InetAddress::IsReachable(
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isReachable)
{
    VALIDATE_NOT_NULL(isReachable);

    return IsReachable(NULL, 0, timeout, isReachable);
}

ECode InetAddress::IsReachable(
    /* [in] */ INetworkInterface* networkInterface,
    /* [in] */ Int32 ttl,
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isReachable)
{
    VALIDATE_NOT_NULL(isReachable);


    if (ttl < 0 || timeout < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    //The simple case.
    if (networkInterface == NULL) {
        *isReachable = IsReachable(this, NULL, timeout);
        return NOERROR;
    }

    //Try each NetworkInterface in parallel.
    //Use a thread pool executor?
    AutoPtr<IList> sourceAddresses;
    AutoPtr<IIterator> em;
    AutoPtr<IEnumeration> outenum;
    networkInterface->GetInetAddresses((IEnumeration**)&outenum);
    CCollections::_NewList(outenum, (IList**)&sourceAddresses);
    Int32 count;
    sourceAddresses->GetSize(&count);
    if (count == 0) {
        *isReachable = FALSE;
        return NOERROR;
    }
    AutoPtr<ICountDownLatch> latch;
    CCountDownLatch::New(count, (ICountDownLatch**)&latch);
    AutoPtr<IAtomicBoolean> _isReachable;
    CAtomicBoolean::New(FALSE, (IAtomicBoolean**)&_isReachable);
    sourceAddresses->GetIterator((IIterator**)&em);
    Boolean hasNext = FALSE;
    // TODO: find all inetAddress instance to check is the isReachabel should be TRUE;
    while (em->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> outface;
        em->Next((IInterface**)&outface);
        AutoPtr<IInetAddress> sourceAddress = IInetAddress::Probe(outface);
        AutoPtr<_InetAddressThread> newthread = new _InetAddressThread(this,
                                                    sourceAddress, timeout, latch, _isReachable);
        newthread->Start();
    }
    // try {
    ECode ec = latch->Await();
    // } catch (InterruptedException ignored) {
    if (ec == E_INTERRUPTED_EXCEPTION) {
       AutoPtr<IThread> onthread = Thread::GetCurrentThread();
       onthread->Interrupt(); // Leave the interrupted bit set.
    }
    // }
    return _isReachable->Get(isReachable);
}

ECode InetAddress::GetByAddress(
    /* [in] */ ArrayOf<Byte>* ipAddress,
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    return GetByAddress(String(NULL), ipAddress, 0, address);
}

ECode InetAddress::GetByAddress(
    /* [in] */ const String& hostName,
    /* [in] */ ArrayOf<Byte>* ipAddress,
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    return GetByAddress(hostName, ipAddress, 0, address);
}

ECode InetAddress::BytesToInetAddresses(
    /* [in] */ ArrayOf<ByteArray>* rawAddresses,
    /* [in] */ const String& hostName,
    /* [out, callee] */ ArrayOf<IInetAddress*>** addresses)
{
    VALIDATE_NOT_NULL(addresses)

    AutoPtr<ArrayOf<IInetAddress*> > returnedAddresses =
            ArrayOf<IInetAddress*>::Alloc(rawAddresses->GetLength());
    for(Int32 i=0; i < rawAddresses->GetLength(); i++) {
        AutoPtr<IInetAddress> addr;
        MakeInetAddress((*rawAddresses)[i], hostName, (IInetAddress**)&addr);
        returnedAddresses->Set(i, addr);
    }
    *addresses = returnedAddresses;
    REFCOUNT_ADD(*addresses);
    return NOERROR;
}

ECode InetAddress::GetHostByAddrImpl(
    /* [in] */ InetAddress* address,
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IBlockGuard> helper;
    CBlockGuard::AcquireSingleton((IBlockGuard**)&helper);
    AutoPtr<IBlockGuardPolicy> policy;
    helper->GetThreadPolicy((IBlockGuardPolicy**)&policy);
    policy->OnNetwork();

    String hostname;
    FAIL_RETURN(sOs->Getnameinfo((IInetAddress*) address->Probe(EIID_IInetAddress), NI_NAMEREQD, &hostname));
    AutoPtr<ArrayOf<Byte> > addressClone = address->mIpAddress->Clone();
    if(FAILED(MakeInetAddress(addressClone, hostname, result))) {
        return E_UNKNOWN_HOST_EXCEPTION;
    }
    return NOERROR;
}

ECode InetAddress::GetByAddress(
    /* [in] */ const String& hostName,
    /* [in] */ ArrayOf<Byte>* ipaddress,
    /* [in] */ Int32 scopeId,
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    if(ipaddress == NULL) {
        return E_UNKNOWN_HOST_EXCEPTION;
    }
    AutoPtr<ArrayOf<Byte> > addressClone;
    if(ipaddress->GetLength() == 4) {
        addressClone = ipaddress->Clone();
        CInet4Address::New(addressClone, hostName, (IInet4Address**)address);
        return NOERROR;
    }
    else if(ipaddress->GetLength() == 16){
        if(IsIPv4MappedAddress(ipaddress)) {
            IPv4MappedToIPv4(ipaddress, (ArrayOf<Byte>**)&addressClone);
            return CInet4Address::New(addressClone, hostName, (IInet4Address**)address);
        }
        else{
            addressClone = ipaddress->Clone();
            return CInet6Address::New(addressClone, hostName, scopeId, (IInet6Address**)address);
        }
    }
    else{
        return E_UNKNOWN_HOST_EXCEPTION;
    }
}

Int32 InetAddress::BytesToInt32(
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 start)
{
    return ((bytes[start] & 0xff) << 24) |
            ((bytes[++start] & 0xff) << 16) |
            ((bytes[++start] & 0xff) << 8) |
            ((bytes[++start] & 0xff) << 0) ;
}

ECode InetAddress::LookupHostByName(
    /* [in] */ const String& host,
    /* [out, callee] */ ArrayOf<IInetAddress*>** addresses)
{
    VALIDATE_NOT_NULL(addresses);

    AutoPtr<IBlockGuard> helper;
    CBlockGuard::AcquireSingleton((IBlockGuard**)&helper);
    AutoPtr<IBlockGuardPolicy> policy;
    helper->GetThreadPolicy((IBlockGuardPolicy**)&policy);
    policy->OnNetwork();
    // Do we have a result cached?
    AutoPtr< ArrayOf<IInetAddress*> > cachedResult = ADDRESS_CACHE->Get(host);
    if(cachedResult != NULL) {
        if (cachedResult != AddressCache::UNKNOWN_ADDRESS) {
            *addresses = cachedResult;
            REFCOUNT_ADD(*addresses);
            return NOERROR;
        }
        else {
            // A cached negative result.
            // throw new UnknownHostException((String) cachedResult);
            return E_UNKNOWN_HOST_EXCEPTION;
        }
    }
    AutoPtr<IStructAddrinfo> hints;
    FAIL_RETURN(CStructAddrinfo::New((IStructAddrinfo**)&hints));
    hints->SetFlags(AI_ADDRCONFIG);
    hints->SetFamily(AF_UNSPEC);
    // If we don't specify a socket type, every address will appear twice, once
    // for SOCK_STREAM and one for SOCK_DGRAM. Since we do not return the family
    // anyway, just pick one.
    hints->SetSocktype(SOCK_STREAM);
    AutoPtr<ArrayOf<IInetAddress*> > structAddresses;
    ECode ec = sOs->Getaddrinfo(host, hints, (ArrayOf<IInetAddress*>**)&structAddresses);
    if (FAILED(ec)) {
        ADDRESS_CACHE->PutUnknownHost(host, String("Unable to resolve") + host);
        return ec;
    }
    if (structAddresses != NULL) {
        for (Int32 i = 0; i < structAddresses->GetLength(); i++) {
            InetAddress* inetAddressObj = reinterpret_cast<InetAddress*>((*structAddresses)[i]->Probe(EIID_InetAddress));
            inetAddressObj->mHostName = host;
        }
    }

    ADDRESS_CACHE->Put(host, structAddresses);
    *addresses = structAddresses;
    REFCOUNT_ADD(*addresses);
    return NOERROR;
}

Boolean InetAddress::IsReachable(
    /* [in] */ InetAddress* destination,
    /* [in] */ IInetAddress* source,
    /* [in] */ Int32 timeout)
{
    Int32 fd;
    AutoPtr<IIoBridge> ioBridge;
    CIoBridge::AcquireSingleton((IIoBridge**)&ioBridge);
    ioBridge->Socket(TRUE, &fd);
    Boolean reached = FALSE;
    if(source != NULL) {
        if (FAILED(ioBridge->Bind(fd, source, 0))) {
            return FALSE;
        }
    }

    if (FAILED(ioBridge->Connect(fd, (IInetAddress*)destination->Probe(EIID_IInetAddress), 7, timeout, &reached))) {
        return FALSE;
    }
    return TRUE;
}

Boolean InetAddress::IsIPv4MappedAddress(
    /* [in] */ ArrayOf<Byte>* ipAddress)
{
    if(ipAddress == NULL || ipAddress->GetLength() != 16) {
        return FALSE;
    }
    for(Int32 i = 0; i < 10; i++) {
        if((*ipAddress)[i] != 0) {
            return FALSE;
        }
    }
    if((*ipAddress)[10] != -1 || (*ipAddress)[11] != -1) {
        return FALSE;
    }
    return TRUE;
}

ECode InetAddress::IPv4MappedToIPv4(
    /* [in] */ ArrayOf<Byte>* mappedAddress,
    /* [out, callee] */ ArrayOf<Byte>** ipv4Address)
{
    VALIDATE_NOT_NULL(ipv4Address);

    *ipv4Address = ArrayOf<Byte>::Alloc(4);
    REFCOUNT_ADD(*ipv4Address);
    for(Int32 i=0; i < 4; i++) {
        (**ipv4Address)[i] = (*mappedAddress)[12 + i];
    }
    return NOERROR;
}

//    private void writeObject(ObjectOutputStream stream)

//    private void readObject(ObjectInputStream stream)

ECode InetAddress::MakeInetAddress(
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [in] */ const String& hostName,
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    if(bytes->GetLength() == 4) {
        return CInet4Address::New(bytes, hostName, (IInet4Address**)address);
    }
    else if(bytes->GetLength() == 16) {
        return CInet6Address::New(bytes, hostName, 0, (IInet6Address**)address);
    }
    else{
        return E_UNKNOWN_HOST_EXCEPTION;
    }
}

AutoPtr<IInetAddress> InetAddress::DisallowDeprecatedFormats(
    /* [in] */ const String& address,
    /* [in] */ IInetAddress* inetAddress)
{
    if (IInet4Address::Probe(inetAddress) != NULL ||
            address.IndexOf(':') != -1) {
        return inetAddress;
    }
    // If inet_pton(3) can't parse it, it must have been a deprecated format.
    // We need to return inet_pton(3)'s result to ensure that numbers assumed to be octal
    // by getaddrinfo(3) are reinterpreted by inet_pton(3) as decimal.
    AutoPtr<IInetAddress> isia;
    sOs->Inet_pton(AF_INET, address, (IInetAddress**)&isia);
    return isia;
}

AutoPtr<IInetAddress> InetAddress::ParseNumericAddressNoThrow(
    /* [in] */ const String& _address)
{
    String address = _address;
    if (address.StartWith("[") && address.EndWith("]") && address.IndexOf(':') != -1) {
       address = address.Substring(1, address.GetLength() - 1);
    }
    AutoPtr<IStructAddrinfo> hints;
    CStructAddrinfo::New((IStructAddrinfo**)&hints);
    hints->SetFlags(AI_NUMERICHOST);
    AutoPtr< ArrayOf<IInetAddress*> > info;
    sOs->Getaddrinfo(address, hints, (ArrayOf<IInetAddress*>**)&info);
    if (info == NULL)
        return NULL;

    AutoPtr<IInetAddress> result = (*info)[0];
    return result;
}

ECode InetAddress::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (IInetAddress::Probe(obj) == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    InetAddress* inetAddress = reinterpret_cast<InetAddress*>(obj->Probe(EIID_InetAddress));
    if (inetAddress == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr< ArrayOf<Byte> > objIpaddress = inetAddress->mIpAddress;
    if (mIpAddress->GetLength() != objIpaddress->GetLength()) {
        *result = FALSE;
        return NOERROR;
    }
    *result = TRUE;
    for (Int32 i = 0; i < mIpAddress->GetLength(); i++) {
        if((*objIpaddress)[i] != (*mIpAddress)[i]) {
            *result = FALSE;
            break;
        }
    }
    return NOERROR;
}

ECode InetAddress::LoopbackAddresses(
    /* [out] */ ArrayOf<IInetAddress*>** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr< ArrayOf<IInetAddress*> > addrs = ArrayOf<IInetAddress*>::Alloc(2);
    addrs->Set(0, CInet6Address::LOOPBACK);
    addrs->Set(1, CInet4Address::LOOPBACK);
    *result = addrs;
    REFCOUNT_ADD(*result);
    return NOERROR;
}


//==========================================================
//       InetAddress::_InetAddressThread
//==========================================================
InetAddress::_InetAddressThread::_InetAddressThread(
    /* [in] */ InetAddress* host,
    /* [in] */ IInetAddress* sourceAddress,
    /* [in] */ Int32 timeout,
    /* [in] */ ICountDownLatch* latch,
    /* [in] */ IAtomicBoolean* isReachable)
    : mHost(host)
    , mSourceAddress(sourceAddress)
    , mTimeout(timeout)
    , mLatch(latch)
    , mIsReachable(isReachable)
{
}

ECode InetAddress::_InetAddressThread::Run()
{
    // try {
    if (mHost->IsReachable(mHost, mSourceAddress, mTimeout)) {
         mIsReachable->Set(TRUE);
        // Wake the main thread so it can return success without
        // waiting for any other threads to time out.
        Int64 latchount = 0;
        while ((mLatch->GetCount(&latchount), latchount) > 0) {
            mLatch->CountDown();
        }
    }
    // } catch (IOException ignored) {
    // }
    mLatch->CountDown();
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
