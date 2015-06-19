#ifndef __ELASTOS_NET_CPROXYHELPER_H__
#define __ELASTOS_NET_CPROXYHELPER_H__

#include <_CProxyHelper.h>
#include "Singleton.h"


namespace Elastos {
namespace Net {

CarClass(CProxyHelper)
    , public Singleton
    , public IProxyHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    GetNO_PROXY(
        /* [out] */ IProxy** proxy);
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_CPROXYHELPER_H__
