
#ifndef __ELASTOS_NET_PROXYSELECTOR_H__
#define __ELASTOS_NET_PROXYSELECTOR_H__

#include <elastos/core/Object.h>

using Elastos::Net::INetPermission;

namespace Elastos {
namespace Net {

class ProxySelect
    : public Object
    , public IProxySelect
{
public:
    CAR_INTERFACE_DECL()

    static CARAPI GetDefault(
        /* [out] */ IProxySelector** defaultSelector);

    static CARAPI SetDefault(
        /* [in] */ IProxySelector* selector);

    virtual CARAPI Select(
        /* [in] */ IURI* uri,
        /* [out] */ IObjectContainer** container) = 0;

    virtual CARAPI ConnectFailed(
        /* [in] */ IURI* uri,
        /* [in] */ ISocketAddress* sa,
        /* [in] */ ECode ec) = 0;

private:
    static AutoPtr<IProxySelector> sDefaultSelector;

    static AutoPtr<INetPermission> sGetProxySelectorPermission;

    static AutoPtr<INetPermission> sSetProxySelectorPermission;
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_ProxySelector_H__
