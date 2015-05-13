#ifndef __CNETWORKINTERFACEHELPER_H__
#define __CNETWORKINTERFACEHELPER_H__

#include <_CNetworkInterfaceHelper.h>
#include <NetworkInterface.h>

namespace Elastos {
namespace Net {

CarClass(CNetworkInterfaceHelper)
{
public:
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

#endif //__CNETWORKINTERFACEHELPER_H__
