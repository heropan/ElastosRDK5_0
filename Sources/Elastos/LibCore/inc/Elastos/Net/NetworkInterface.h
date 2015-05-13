#ifndef __NETWORKINTERFACE_H__
#define __NETWORKINTERFACE_H__

#include "Elastos.Core_server.h"
#include <elautoptr.h>
#include <elrefbase.h>

using Elastos::Utility::IList;
using Elastos::Utility::IEnumeration;

namespace Elastos {
namespace Net {

class NetworkInterface
    : public ElRefBase
    , public INetworkInterface
{
public:
    NetworkInterface(
        /* [in] */ const String& name,
        /* [in] */ Int32 displayName,
        /* [in] */ AutoPtr<IList> addresses,
        /* [in] */ AutoPtr<IList> interfaceAddresses);

    ~NetworkInterface();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetIndex(
        /* [out] */ Int32* index);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetInetAddresses(
        /* [out] */ IEnumeration** addresses);

    CARAPI GetDisplayName(
        /* [out] */ String* name);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* result);

    CARAPI ToString(
        /*  [out] */ String* str);

    static CARAPI  GetByName(
        /* [in] */ const String& interfaceName,
        /* [out] */ INetworkInterface** networkInterface);

    static CARAPI GetByInetAddress(
        /* [in] */ IInetAddress* address,
        /* [out] */ INetworkInterface** networkInterface);

    static CARAPI GetNetworkInterfaces(
            /* [out] */IEnumeration** enumerator);

    static CARAPI GetByIndex(
         /* [in] */ Int32 index,
         /* [out] */ INetworkInterface** networkInterface);

    static AutoPtr<INetworkInterface> ForUnboundMulticastSocket();

    CARAPI GetInterfaceAddresses(
        /* [out] */ IList** addresses);

    CARAPI GetSubInterfaces(
        /* [out] */ IEnumeration** subInterfaces);

    CARAPI GetParent(
        /* [out] */ INetworkInterface** parent);

    CARAPI IsUp(
        /* [out] */ Boolean* isUp);

    CARAPI IsLoopback(
        /* [out] */ Boolean* isLoopback);

    CARAPI IsPointToPoint(
        /* [out] */ Boolean* isPointToPoint);

    CARAPI SupportsMulticast(
        /* [out] */ Boolean* multicast);

    CARAPI GetHardwareAddress(
        /* [out, callee] */ ArrayOf<Byte>** address);

    CARAPI GetMTU(
        /* [out] */ Int32* mtu);

    CARAPI IsVirtual(
        /* [out] */ Boolean* isvirtual);

    CARAPI FindAddress(
        /* [in] */ IInetAddress* address,
        /* [out] */ Boolean* found);

    CARAPI GetChildren(
        /* [out] */ IList** children);

    CARAPI GetAddresses(
        /* [out] */ IList** addresses);

    static AutoPtr<IList> GetNetworkInterfacesList();

private:
    static CARAPI CollectIpv6Addresses(
        /* [in] */ const String& interfaceName,
        /* [in] */ Int32 interfaceIndex,
        /* [in] */ IList* addresses,
        /* [in] */ IList* interfaceAddresses);

    static CARAPI CollectIpv4Addresses(
        /* [in] */ const String& interfaceName,
        /* [in] */ IList* addresses,
        /* [in] */ IList* interfaceAddresses);

    static CARAPI_(Boolean) IsValidInterfaceName(
        /* [in] */ const String& interfaceName);

    static CARAPI_(Int32) ReadIntFile(
        /* [in] */ const String& path);

    CARAPI_(Boolean) HasFlag(
        /* [in] */ Int32 mask);

public:
    static const Int32 NO_INTERFACE_INDEX = 0;
    static const Int32 UNSET_INTERFACE_INDEX = -1;

private:
    static const Int32 CHECK_CONNECT_NO_PORT = -1;

    String mName;

    String mDisplayName;

    Int32 mInterfaceIndex;

    AutoPtr<IList> mInterfaceAddresses;

    AutoPtr<IList> mAddresses;

    AutoPtr<INetworkInterface> mParent;

    AutoPtr<IList> mChildren;
};

} // namespace Net
} // namespace Elastos

#endif //__NETWORKINTERFACE_H__
