
#include "Elastos.CoreLibrary.Net.h"
#include "elastos/droid/net/RouteInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(RouteInfo, Object, IParcelable, IRouteInfo)

ECode RouteInfo::constructor(
    /* [in] */ IIpPrefix* destination,
    /* [in] */ IInetAddress* gateway,
    /* [in] */ const String& iface,
    /* [in] */ Int32 type)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        switch (type) {
            case RTN_UNICAST:
            case RTN_UNREACHABLE:
            case RTN_THROW:
                // TODO: It would be nice to ensure that route types that don't have nexthops or
                // interfaces, such as unreachable or throw, can't be created if an interface or
                // a gateway is specified. This is a bit too complicated to do at the moment
                // because:
                //
                // - LinkProperties sets the interface on routes added to it, and modifies the
                //   interfaces of all the routes when its interface name changes.
                // - Even when the gateway is null, we store a non-null gateway here.
                //
                // For now, we just rely on the code that sets routes to do things properly.
                break;
            default:
                throw new IllegalArgumentException("Unknown route type " + type);
        }
        if (destination == NULL) {
            if (gateway != NULL) {
                if (IInet4Address::Probe(gateway) != NULL) {
                    destination = new IpPrefix(Inet4Address.ANY, 0);
                } else {
                    destination = new IpPrefix(Inet6Address.ANY, 0);
                }
            } else {
                // no destination, no gateway. invalid.
                throw new IllegalArgumentException("Invalid arguments passed in: " + gateway + "," +
                                                   destination);
            }
        }
        // TODO: set mGateway to null if there is no gateway. This is more correct, saves space, and
        // matches the documented behaviour. Before we can do this we need to fix all callers (e.g.,
        // ConnectivityService) to stop doing things like r.getGateway().equals(), ... .
        if (gateway == NULL) {
            if (destination.getAddress() instanceof Inet4Address) {
                gateway = Inet4Address.ANY;
            } else {
                gateway = Inet6Address.ANY;
            }
        }
        mHasGateway = (!gateway.isAnyLocalAddress());
        if ((destination.getAddress() instanceof Inet4Address &&
                 ((IInet4Address::Probe(gateway) != NULL) == FALSE)) ||
                (destination.getAddress() instanceof Inet6Address &&
                 ((IInet6Address::Probe(gateway) != NULL) == FALSE))) {
            throw new IllegalArgumentException("address family mismatch in RouteInfo constructor");
        }
        mDestination = destination;  // IpPrefix objects are immutable.
        mGateway = gateway;          // InetAddress objects are immutable.
        mInterface = iface;          // Strings are immutable.
        mType = type;
        mIsHost = isHost();
#endif
}

ECode RouteInfo::constructor(
    /* [in] */ IIpPrefix* destination,
    /* [in] */ IInetAddress* gateway,
    /* [in] */ const String& iface)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(destination, gateway, iface, RTN_UNICAST);
#endif
}

ECode RouteInfo::constructor(
    /* [in] */ ILinkAddress* destination,
    /* [in] */ IInetAddress* gateway,
    /* [in] */ const String& iface)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(destination == NULL ? NULL :
                new IpPrefix(destination.getAddress(), destination.getPrefixLength()),
                gateway, iface);
#endif
}

ECode RouteInfo::constructor(
    /* [in] */ IIpPrefix* destination,
    /* [in] */ IInetAddress* gateway)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(destination, gateway, NULL);
#endif
}

ECode RouteInfo::constructor(
    /* [in] */ ILinkAddress* destination,
    /* [in] */ IInetAddress* gateway)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(destination, gateway, NULL);
#endif
}

ECode RouteInfo::constructor(
    /* [in] */ IInetAddress* gateway)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this((IpPrefix) NULL, gateway, NULL);
#endif
}

ECode RouteInfo::constructor(
    /* [in] */ IIpPrefix* destination)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(destination, NULL, NULL);
#endif
}

ECode RouteInfo::constructor(
    /* [in] */ ILinkAddress* destination)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(destination, NULL, NULL);
#endif
}

ECode RouteInfo::constructor(
    /* [in] */ IIpPrefix* destination,
    /* [in] */ Int32 type)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(destination, NULL, NULL, type);
#endif
}

ECode RouteInfo::MakeHostRoute(
    /* [in] */ IInetAddress* host,
    /* [in] */ const String& iface,
    /* [out] */ IRouteInfo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return makeHostRoute(host, NULL, iface);
#endif
}

ECode RouteInfo::MakeHostRoute(
    /* [in] */ IInetAddress* host,
    /* [in] */ IInetAddress* gateway,
    /* [in] */ const String& iface,
    /* [out] */ IRouteInfo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (host == NULL) return NULL;
        if (IInet4Address::Probe(host) != NULL) {
            return new RouteInfo(new IpPrefix(host, 32), gateway, iface);
        } else {
            return new RouteInfo(new IpPrefix(host, 128), gateway, iface);
        }
#endif
}

ECode RouteInfo::IsHost(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return (mDestination.getAddress() instanceof Inet4Address &&
                mDestination.getPrefixLength() == 32) ||
               (mDestination.getAddress() instanceof Inet6Address &&
                mDestination.getPrefixLength() == 128);
#endif
}

ECode RouteInfo::GetDestination(
    /* [out] */ IIpPrefix** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mDestination;
#endif
}

ECode RouteInfo::GetDestinationLinkAddress(
    /* [out] */ ILinkAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return new LinkAddress(mDestination.getAddress(), mDestination.getPrefixLength());
#endif
}

ECode RouteInfo::GetGateway(
    /* [out] */ IInetAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mGateway;
#endif
}

ECode RouteInfo::GetInterface(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mInterface;
#endif
}

ECode RouteInfo::GetType(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mType;
#endif
}

ECode RouteInfo::IsDefaultRoute(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mType == RTN_UNICAST && mDestination.getPrefixLength() == 0;
#endif
}

ECode RouteInfo::IsIPv4Default(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return isDefaultRoute() && mDestination.getAddress() instanceof Inet4Address;
#endif
}

ECode RouteInfo::IsIPv6Default(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return isDefaultRoute() && mDestination.getAddress() instanceof Inet6Address;
#endif
}

ECode RouteInfo::IsHostRoute(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mIsHost;
#endif
}

ECode RouteInfo::HasGateway(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mHasGateway;
#endif
}

ECode RouteInfo::Matches(
    /* [in] */ IInetAddress* destination,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (destination == NULL) return FALSE;
        // match the route destination and destination with prefix length
        InetAddress dstNet = NetworkUtils.getNetworkPart(destination,
                mDestination.getPrefixLength());
        return mDestination.getAddress().equals(dstNet);
#endif
}

ECode RouteInfo::SelectBestRoute(
    /* [in] */ ICollection* routes,
    /* [in] */ IInetAddress* dest,
    /* [out] */ IRouteInfo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if ((routes == NULL) || (dest == NULL)) return NULL;
        RouteInfo bestRoute = NULL;
        // pick a longest prefix match under same address type
        for (RouteInfo route : routes) {
            if (NetworkUtils.addressTypeMatches(route.mDestination.getAddress(), dest)) {
                if ((bestRoute != NULL) &&
                        (bestRoute.mDestination.getPrefixLength() >=
                        route.mDestination.getPrefixLength())) {
                    continue;
                }
                if (route.matches(dest)) bestRoute = route;
            }
        }
        return bestRoute;
#endif
}

ECode RouteInfo::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String val = "";
        if (mDestination != NULL) val = mDestination.toString();
        if (mType == RTN_UNREACHABLE) {
            val += " unreachable";
        } else if (mType == RTN_THROW) {
            val += " throw";
        } else {
            val += " ->";
            if (mGateway != NULL) val += " " + mGateway.getHostAddress();
            if (mInterface != NULL) val += " " + mInterface;
            if (mType != RTN_UNICAST) {
                val += " unknown type " + mType;
            }
        }
        return val;
#endif
}

ECode RouteInfo::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (this == obj) return TRUE;
        if (!(IRouteInfo::Probe(obj) != NULL)) return FALSE;
        RouteInfo target = (RouteInfo) obj;
        return Objects.equals(mDestination, target.getDestination()) &&
                Objects.equals(mGateway, target.getGateway()) &&
                Objects.equals(mInterface, target.getInterface()) &&
                mType == target.getType();
#endif
}

ECode RouteInfo::GetHashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return (mDestination.hashCode() * 41)
                + (mGateway == NULL ? 0 :mGateway.hashCode() * 47)
                + (mInterface == NULL ? 0 :mInterface.hashCode() * 67)
                + (mType * 71);
#endif
}

ECode RouteInfo::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        public RouteInfo createFromParcel(Parcel in) {
            IpPrefix dest = in.readParcelable(NULL);
            InetAddress gateway = NULL;
            byte[] addr = in.createByteArray();
            try {
                gateway = InetAddress.getByAddress(addr);
            } catch (UnknownHostException e) {}
            String iface = in.readString();
            Int32 type = in.readInt();
            return new RouteInfo(dest, gateway, iface, type);
        }
        public RouteInfo[] newArray(Int32 size) {
            return new RouteInfo[size];
        }
#endif
}

ECode RouteInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        public RouteInfo createFromParcel(Parcel in) {
            IpPrefix dest = in.readParcelable(NULL);
            InetAddress gateway = NULL;
            byte[] addr = in.createByteArray();
            try {
                gateway = InetAddress.getByAddress(addr);
            } catch (UnknownHostException e) {}
            String iface = in.readString();
            Int32 type = in.readInt();
            return new RouteInfo(dest, gateway, iface, type);
        }
        public RouteInfo[] newArray(Int32 size) {
            return new RouteInfo[size];
        }
#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos

#if 0 // old CRouteInfo.cpp
#include "elastos/droid/net/CRouteInfo.h"
#include "elastos/droid/net/CLinkAddress.h"
#include "elastos/droid/net/NetworkUtils.h"
#include <Elastos.CoreLibrary.h>

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
    return MakeHostRoute(host, NULL, info);
}

ECode CRouteInfo::MakeHostRoute(
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
    REFCOUNT_ADD(*address);
    return NOERROR;
}

ECode CRouteInfo::GetGateway(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);
    *address = mGateway;
    REFCOUNT_ADD(*address);
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
    return Equals(IRouteInfo::Probe(obj), result);
}

//@Override
ECode CRouteInfo::Equals(
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
    REFCOUNT_ADD(*info);
    return NOERROR;
}

} //namespace Net
} //namespace Droid
} //namespace Elastos
#endif
