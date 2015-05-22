
#ifndef __PROXYSELECTOR_H__
#define __PROXYSELECTOR_H__

#include "Elastos.CoreLibrary_server.h"

using Elastos::Net::INetPermission;

namespace Elastos {
namespace Net {

class ProxySelector
{
public:
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

#endif //__ProxySelector_H__
