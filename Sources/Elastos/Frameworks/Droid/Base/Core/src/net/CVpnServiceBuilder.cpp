
#include "net/CVpnServiceBuilder.h"
#include "net/CVpnService.h"

namespace Elastos {
namespace Droid {
namespace Net {

CVpnServiceBuilder::CVpnServiceBuilder()
{}

ECode CVpnServiceBuilder::SetSession(
    /* [in] */ const String& session)
{
    return mConfig->SetSession(session);
}

ECode CVpnServiceBuilder::SetConfigureIntent(
    /* [in] */ IPendingIntent* intent)
{
    return mConfig->SetConfigureIntent(session);
}

ECode CVpnServiceBuilder::SetMtu(
    /* [in] */ Int32 mtu)
{
    if (mtu <= 0) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return mConfig->SetMtu(mtu);
}

ECode CVpnServiceBuilder::AddAddress(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength)
{
    Check(address, prefixLength);

    Boolean isAnyLocalAddress;
    address->IsAnyLocalAddress(&isAnyLocalAddress);

    if (isAnyLocalAddress) {
        Logger::D("VpnService", "Bad address");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    String hostAddress;
    address->GetHostAddress(&hostAddress);

    mAddresses += ' ';
    mAddresses += hostAddress;
    mAddresses += '/';
    mAddresses += prefixLength;

    return NOERROR;
}

ECode CVpnServiceBuilder::AddAddressEx(
    /* [in] */ const String& address,
    /* [in] */ Int32 prefixLength)
{
    AutoPtr<IInetAddressHelper> addressHelper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&addressHelper);

    AutoPtr<IInetAddress> addressObj;
    addressHelper->ParseNumericAddress(address, (IInetAddress**)&addressObj);

    return AddAddress(addressObj, prefixLength);
}

ECode CVpnServiceBuilder::AddRoute(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength)
{
    Check(address, prefixLength);

    Int32 offset = prefixLength / 8;

    AutoPtr< ArrayOf<Byte> > bytes;
    address->GetAddress((ArrayOf<Byte>**)&bytes);

    Int32 length = bytes->GetLength();

    if (offset < length) {
        for ((*bytes)[offset] <<= prefixLength % 8; offset < length; ++offset) {
            if ((*bytes)[offset] != 0) {
                Logger::D("VpnService", "Bad address");
                return E_ILLEGAL_STATE_EXCEPTION;
            }
        }
    }

    String hostAddress;
    address->GetHostAddress(&hostAddress);

    mRoutes += ' ';
    mRoutes += hostAddress;
    mRoutes += '/';
    mRoutes += prefixLength;

    return NOERROR;
}

ECode CVpnServiceBuilder::AddRouteEx(
    /* [in] */ const String& address,
    /* [in] */ Int32 prefixLength)
{
    AutoPtr<IInetAddressHelper> addressHelper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&addressHelper);

    AutoPtr<IInetAddress> addressObj;
    addressHelper->ParseNumericAddress(address, (IInetAddress**)&addressObj);

    return AddRoute(addressObj, prefixLength);
}

ECode CVpnServiceBuilder::AddDnsServer(
    /* [in] */ IInetAddress* address)
{
    Boolean isLoopbackAddress;
    address->IsLoopbackAddress(isLoopbackAddress);

    Boolean isAnyLocalAddress;
    address->IsAnyLocalAddress(&isAnyLocalAddress);

    if (isLoopbackAddress || isAnyLocalAddress) {
        Logger::D("VpnService", "Bad address");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<IObjectContainer> dnsServers;
    mConfig->GetDnsServers((IObjectContainer**)&dnsServers);

    if (dnsServers == NULL) {
        CObjectContainer::New((IObjectContainer**)&dnsServers);
        mConfig->SetDnsServers(dnsServers->Get());
    }

    String hostAddress;
    address->GetHostAddress(&hostAddress);

    AutoPtr<ICharSequence> elmt;
    CStringWrapper::New(hostAddress, (IcharSequence**)&elmt);
    return dnsServers->Add(elmt->Get());
}

ECode CVpnServiceBuilder::AddDnsServerEx(
    /* [in] */ const String& address)
{
    AutoPtr<IInetAddressHelper> addressHelper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&addressHelper);

    AutoPtr<IInetAddress> addressObj;
    addressHelper->ParseNumericAddress(address, (IInetAddress**)&addressObj);

    return AddDnsServer(addressObj, prefixLength);
}

ECode CVpnServiceBuilder::AddSearchDomain(
    /* [in] */ const String& dnsDomain)
{
    AutoPtr<IObjectContainer> searchDomains;
    mConfig->GetSearchDomains((IObjectContainer**)&searchDomains);

    if (searchDomains == NULL) {
        CObjectContainer::New((IObjectContainer**)&searchDomains);
        mConfig->SetSearchDomains(searchDomains->Get());
    }

    AutoPtr<ICharSequence> elmt;
    CStringWrapper::New(dnsDomain, (IcharSequence**)&elmt);
    return searchDomains->Add(elmt->Get());
}

ECode CVpnServiceBuilder::Establish(
    /* [out] */ IParcelFileDescriptor** pfd)
{
    VALIDATE_NOT_NULL(pfd);

    mConfig->SetAddresses((String)mAddresses);
    mConfig->SetRoutes((String)mRoutes);

    ECode ec = CVpnService::GetService()->EstablishVpn(mConfig);
    if (FAILED(ec)) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return NOERROR;
}

ECode CVpnServiceBuilder::Check(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength)
{
    Boolean isLoopbackAddress;
    address->IsLoopbackAddress(isLoopbackAddress);

    if (isLoopbackAddress) {
        Logger::D("VpnService", "Bad address");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (IInet4Address::Probe(address) != NULL) {
        if (prefixLength < 0 || prefixLength > 32) {
            Logger::D("VpnService", "Bad prefixLength");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
    } else if (IInet6Address::Probe(address) != NULL) {
        if (prefixLength < 0 || prefixLength > 128) {
            Logger::D("VpnService", "Bad prefixLength");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
    } else {
        Logger::D("VpnService", "Unsupported family");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
