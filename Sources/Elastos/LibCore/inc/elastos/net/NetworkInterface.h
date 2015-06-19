#ifndef __ELASTOS_NET_NETWORKINTERFACE_H__
#define __ELASTOS_NET_NETWORKINTERFACE_H__

#include <elastos/core/Object.h>

using Elastos::Utility::IList;
using Elastos::Utility::IEnumeration;

namespace Elastos {
namespace Net {

class NetworkInterface
    : public Object
    , public INetworkInterface
{
public:
    CAR_INTERFACE_DECL()

    NetworkInterface(
        /* [in] */ const String& name,
        /* [in] */ Int32 interfaceIndex,
        /* [in] */ IList * addresses,
        /* [in] */ IList * interfaceAddresses);

    ~NetworkInterface();

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ Int32 interfaceIndex,
        /* [in] */ IList * addresses,
        /* [in] */ IList * interfaceAddresses);

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

protected:
    NetworkInterface();

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

#endif //__ELASTOS_NET_NETWORKINTERFACE_H__
