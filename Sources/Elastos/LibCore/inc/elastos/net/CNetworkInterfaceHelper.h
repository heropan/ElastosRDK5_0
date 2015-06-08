#ifndef __ELASTOS_NET_CNETWORKINTERFACEHELPER_H__
#define __ELASTOS_NET_CNETWORKINTERFACEHELPER_H__

#include <_CNetworkInterfaceHelper.h>
#include <NetworkInterface.h>
#include <elastos/core/Singleton.h>

using Elastos::Core::Singleton;

namespace Elastos {
namespace Net {

CarClass(CNetworkInterfaceHelper)
    , public Singleton
    , public INetworkInterfaceHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI GetByName(
        /* [in] */ const String& interfaceName,
        /* [out] */ INetworkInterface** networkInterface);

    CARAPI GetByInetAddress(
        /* [in] */ IInetAddress* address,
        /* [out] */ INetworkInterface** networkInterface);

    CARAPI GetByIndex(
        /* [in] */ Int32 index,
        /* [out] */ INetworkInterface** networkInterface);

    CARAPI GetNetworkInterfaces(
        /* [out] */ IEnumeration** networkInterfaceList);
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_CNETWORKINTERFACEHELPER_H__
