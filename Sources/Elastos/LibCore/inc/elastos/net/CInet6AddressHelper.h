#ifndef __CINET6ADDRESSHELPER_H__
#define __CINET6ADDRESSHELPER_H__

#include "_CInet6AddressHelper.h"
#include "InetAddress.h"

namespace Elastos {
namespace Net {

CarClass(CInet6AddressHelper)
{
    CARAPI GetByAddress(
        /* [in] */ const String& host,
        /* [in] */ ArrayOf<Byte>* addr,
        /* [in] */ Int32 scope_id,
        /* [out] */ IInet6Address** address);

    CARAPI GetByAddressEx(
        /* [in] */ const String& host,
        /* [in] */ ArrayOf<Byte>* addr,
        /* [in] */ INetworkInterface* nif,
        /* [out] */ IInet6Address** address);

    CARAPI GetANY(
        /* [out] */ IInetAddress** result);

    CARAPI GetLOOPBACK(
        /* [out] */ IInetAddress** result);

};

} // namespace Net
} // namespace Elastos

#endif //__CINET6ADDRESSHELPER_H__
