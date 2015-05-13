
#include "net/CRouteInfo.h"
#include "net/CLinkAddress.h"
#include "net/NetworkUtils.h"
#include <Elastos.Core.h>

using Elastos::Net::IInetAddressHelper;
using Elastos::Net::CInetAddressHelper;
using Elastos::Net::IInet4Address;
using Elastos::Net::CInet4Address;
using Elastos::Net::IInet4AddressHelper;
using Elastos::Net::CInet4AddressHelper;
using Elastos::Net::IInet6Address;
using Elastos::Net::CInet6Address;
using Elastos::Net::IInet6AddressHelper;
using Elastos::Net::CInet6AddressHelper;

namespace Elastos {
namespace Droid {
namespace Net {

CRouteInfo::CRouteInfo()
    : mIsDefault(FALSE)
    , mIsHost(FALSE)
{
}

CRouteInfo::~CRouteInfo()
{
}

ECode CRouteInfo::constructor()
{
    return NOERROR;
}

ECode CRouteInfo::constructor(
    /* [in] */ ILinkAddress* _destination,
    /* [in] */ IInetAddress* gateway)
{
    AutoPtr<ILinkAddress> destination = _destination;
    if (destination == NULL) {
        if (gateway != NULL) {
            if (IInet4Address::Probe(gateway) != NULL) {
                AutoPtr<IInet4AddressHelper> net4Helper;
                CInet4AddressHelper::AcquireSingleton((IInet4AddressHelper**)&net4Helper);
                AutoPtr<IInetAddress> inet4ANY;
                net4Helper->GetANY((IInetAddress**)&inet4ANY);
                CLinkAddress::New(inet4ANY, 0, (ILinkAddress**)&destination);
            }
            else {
                AutoPtr<IInet6AddressHelper> net6Helper;
                CInet6AddressHelper::AcquireSingleton((IInet6AddressHelper**)&net6Helper);
                AutoPtr<IInetAddress> inet6ANY;
                net6Helper->GetANY((IInetAddress**)&inet6ANY);
                CLinkAddress::New(inet6ANY, 0, (ILinkAddress**)&destination);
            }
        }
        else {
            // no destination, no gateway. invalid.
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    AutoPtr<IInetAddress> address;
    destination->GetAddress((IInetAddress**)&address);

    if (gateway == NULL) {
        if (IInet4Address::Probe(address) != NULL) {
            AutoPtr<IInet4AddressHelper> net4Helper;
            CInet4AddressHelper::AcquireSingleton((IInet4AddressHelper**)&net4Helper);
            AutoPtr<IInetAddress> inet4ANY;
            net4Helper->GetANY((IInetAddress**)&inet4ANY);
            gateway = inet4ANY;
        }
        else {
            AutoPtr<IInet6AddressHelper> net6Helper;
            CInet6AddressHelper::AcquireSingleton((IInet6AddressHelper**)&net6Helper);
            AutoPtr<IInetAddress> inet6ANY;
            net6Helper->GetANY((IInetAddress**)&inet6ANY);
            gateway = inet6ANY;
        }
    }

    Int32 length;
    destination->GetNetworkPrefixLength(&length);
    AutoPtr<IInetAddress> host;
    NetworkUtils::GetNetworkPart(address, length, (IInetAddress**)&host);
    CLinkAddress::New(host, length, (ILinkAddress**)&mDestination);

    mGateway = gateway;
    mIsDefault = IsDefault();
    mIsHost = IsHost();
    return NOERROR;
}

ECode CRouteInfo::constructor(
    /* [in] */ IInetAddress* gateway)
{
    return constructor(NULL, gateway);
}

Boolean CRouteInfo::IsHost()
{
    AutoPtr<IInet4AddressHelper> net4Helper;
    CInet4AddressHelper::AcquireSingleton((IInet4AddressHelper**)&net4Helper);
    AutoPtr<IInetAddress> inet4ANY;
    net4Helper->GetANY((IInetAddress**)&inet4ANY);
    Boolean isInet4addr;
    mGateway->Equals(inet4ANY, &isInet4addr);

    AutoPtr<IInet6AddressHelper> net6Helper;
    CInet6AddressHelper::AcquireSingleton((IInet6AddressHelper**)&net6Helper);
    AutoPtr<IInetAddress> inet6ANY;
    net6Helper->GetANY((IInetAddress**)&inet6ANY);
    Boolean isInet6addr;
    mGateway->Equals(inet6ANY, &isInet6addr);
    return (isInet4addr || isInet6addr);
}

Boolean CRouteInfo::IsDefault()
{
    Boolean val = FALSE;
    Int32 length;
    if (mGateway != NULL) {
        if (IInet4Address::Probe(mGateway)) {
            mDestination->GetNetworkPrefixLength(&length);
            val = (mDestination == NULL || length == 0);
        }
        else {
            mDestination->GetNetworkPrefixLength(&length);// same as above...why??
            val = (mDestination == NULL || length == 0);
        }
    }
    return val;
}

ECode CRouteInfo::MakeHostRoute(
    /* [in] */ IInetAddress* host,
    /* [out] */ IRouteInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return MakeHostRouteEx(host, NULL, info);
}

ECode CRouteInfo::MakeHostRouteEx(
    /* [in] */ IInetAddress* host,
    /* [in] */ IInetAddress* gateway,
    /* [out] */ IRouteInfo** info)
{
    VALIDATE_NOT_NULL(info);

    if (host == NULL) {
        *info = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ILinkAddress> address;
    if (IInet4Address::Probe(host) != NULL) {
        CLinkAddress::New(host, 32, (ILinkAddress**)&address);
        CRouteInfo::New(address, gateway, info);
    } else {
        CLinkAddress::New(host, 128, (ILinkAddress**)&address);
        CRouteInfo::New(address, gateway, info);
    }

    return NOERROR;
}

ECode CRouteInfo::GetDestination(
    /* [out] */ ILinkAddress** address)
{
    VALIDATE_NOT_NULL(address);
    *address = mDestination;
    INTERFACE_ADDREF(*address);
    return NOERROR;
}

ECode CRouteInfo::GetGateway(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);
    *address = mGateway;
    INTERFACE_ADDREF(*address);
    return NOERROR;
}

ECode CRouteInfo::IsDefaultRoute(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mIsDefault;
    return NOERROR;
}

ECode CRouteInfo::IsHostRoute(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mIsHost;
    return NOERROR;
}

ECode CRouteInfo::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    String val("");
    if (mDestination != NULL) {
       mDestination->ToString(&val);
    }
    if (mGateway != NULL) {
        String val2;
        mGateway->GetHostAddress(&val2);
        val += String(" -> ") + val2;
    }
    *str = val;
    return NOERROR;
}

ECode CRouteInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    if (mDestination == NULL) {
        dest->WriteByte(0);
    }
    else {
        dest->WriteByte(1);
        AutoPtr<IInetAddress> netaddress;
        mDestination->GetAddress((IInetAddress**)&netaddress);
        AutoPtr< ArrayOf<Byte> > addr;
        netaddress->GetAddress((ArrayOf<Byte>**)&addr);
        dest->WriteArrayOf((Handle32)addr.Get());
        Int32 prefixLength;
        mDestination->GetNetworkPrefixLength(&prefixLength);
        dest->WriteInt32(prefixLength);
    }

    if (mGateway == NULL) {
        dest->WriteByte(0);
    }
    else {
        dest->WriteByte(1);
        AutoPtr< ArrayOf<Byte> > addr;
        mGateway->GetAddress((ArrayOf<Byte>**)&addr);
        dest->WriteArrayOf((Handle32)addr.Get());
    }
    return NOERROR;
}

ECode CRouteInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    AutoPtr<IInetAddressHelper> addressHelper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&addressHelper);

    Byte byte1;
    source->ReadByte(&byte1);
    if (byte1 == 1)
    {
        AutoPtr<IInetAddress> destAddr;
        AutoPtr< ArrayOf<Byte> > addr;
        source->ReadArrayOf((Handle32*)&addr);
        addressHelper->GetByAddress(addr, (IInetAddress**)&destAddr);
        Int32 prefixLength = 0;
        source->ReadInt32(&prefixLength);

        AutoPtr<ILinkAddress> dest;
        if (destAddr != NULL) {
            CLinkAddress::New(destAddr, prefixLength, (ILinkAddress**)&mDestination);
        }
    }
    Byte byte2;
    source->ReadByte(&byte2);
    if (byte2 == 1)
    {
        AutoPtr< ArrayOf<Byte> > addr;
        source->ReadArrayOf((Handle32*)&addr);
        addressHelper->GetByAddress(addr, (IInetAddress**)&mGateway);
    }
    return NOERROR;
}

ECode CRouteInfo::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result )
{
    VALIDATE_NOT_NULL(result);
    return EqualsEx(IRouteInfo::Probe(obj), result);
}

//@Override
ECode CRouteInfo::EqualsEx(
    /* [in] */ IRouteInfo* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (this == obj) {
        *result = TRUE;
        return NOERROR;
    }

    if (IRouteInfo::Probe(obj) == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    AutoPtr<IRouteInfo> target = obj;

    AutoPtr<ILinkAddress> linkAddr;
    target->GetDestination((ILinkAddress**)&linkAddr);
    Boolean equalDest;
    mDestination->Equals(linkAddr, &equalDest);

    Boolean sameDestination = ( mDestination == NULL) ? (linkAddr == NULL) : equalDest;

    AutoPtr<IInetAddress> inetAddr;
    target->GetGateway((IInetAddress**)&inetAddr);
    Boolean equalGtwy;
    mGateway->Equals(inetAddr, &equalGtwy);
    Boolean sameAddress = (mGateway == NULL) ? (inetAddr == NULL) : equalGtwy;
    Boolean isDefault;
    target->IsDefaultRoute(&isDefault);

    *result = sameDestination && sameAddress && mIsDefault == isDefault;
    return NOERROR;

}

//@Override
ECode CRouteInfo::GetHashCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code);

    Int32 value1;
    mDestination->GetHashCode(&value1);
    Int32 value2;
    mGateway->GetHashCode(&value2);

    *code = (mDestination == NULL ? 0 : value1)
            + (mGateway == NULL ? 0 : value2)
            + (mIsDefault ? 3 : 7);
    return NOERROR;
}

ECode CRouteInfo::Matches(
    /* [in] */ IInetAddress* destination,
    /* [out] */ Boolean* isMatch)
{
    if (destination == NULL) {
        *isMatch = FALSE;
        return NOERROR;
    }

    // if the destination is present and the route is default.
    // return true
    if (IsDefault()) {
        *isMatch = TRUE;
        return NOERROR;
    }

    // match the route destination and destination with prefix length
    AutoPtr<IInetAddress> dstNet;
    Int32 length;
    mDestination->GetNetworkPrefixLength(&length);
    NetworkUtils::GetNetworkPart(destination, length, (IInetAddress**)&dstNet);

    AutoPtr<IInetAddress> address;
    mDestination->GetAddress((IInetAddress**)&address);
    return address->Equals(dstNet, isMatch);
}

/**
* Find the route from a Collection of routes that best matches a given address.
* May return null if no routes are applicable.
* @param routes a Collection of RouteInfos to chose from
* @param dest the InetAddress your trying to get to
* @return the RouteInfo from the Collection that best fits the given address
*/
//To-do: confirm the use of Collection<> (that is originally used here) in Elastos and replace List<>
ECode CRouteInfo::SelectBestRoute(
    /* [in] */ IObjectContainer* routes,
    /* [in] */ IInetAddress* dest,
    /* [out] */ IRouteInfo** info)
{
    if ((routes == NULL) || (dest == NULL)) {
        *info = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IRouteInfo> bestRoute = NULL;
    AutoPtr<IObjectEnumerator> enumerator;
    routes->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IRouteInfo> route;
        enumerator->Current((IInterface**)&route);
        AutoPtr<ILinkAddress> linkAddr;
        route->GetDestination((ILinkAddress**)&linkAddr);
        AutoPtr<IInetAddress> netAddr;
        linkAddr->GetAddress((IInetAddress**)&netAddr);

        Boolean isMatch = NetworkUtils::AddressTypeMatches(netAddr, dest);
        if (isMatch == TRUE) {
            if (bestRoute != NULL) {
                AutoPtr<ILinkAddress> linkAddr1;
                bestRoute->GetDestination((ILinkAddress**)&linkAddr1);
                Int32 prefixLenBest;
                linkAddr1->GetNetworkPrefixLength(&prefixLenBest);

                AutoPtr<ILinkAddress> linkAddr2;
                route->GetDestination((ILinkAddress**)&linkAddr2);
                Int32 prefixLen;
                linkAddr2->GetNetworkPrefixLength(&prefixLen);
                if (prefixLenBest >= prefixLen) continue;
            }
            Boolean bol;
            route->Matches(dest, &bol);
            if (bol) {
                bestRoute = route;
            }
        }
    }
    *info = bestRoute;
    INTERFACE_ADDREF(*info);
    return NOERROR;
}

} //namespace Net
} //namespace Droid
} //namespace Elastos
