
#ifndef __ELASTOS_NET_PROXYSELECTOR_H__
#define __ELASTOS_NET_PROXYSELECTOR_H__

#include "Object.h"
#include <elastos/utility/etl/List.h>

using Elastos::Core::Object;
using Elastos::Utility::IList;
using Elastos::Utility::Etl::List;
using Elastos::Net::INetPermission;

namespace Elastos {
namespace Net {

class ProxySelector
    : public Object
    , public IProxySelector
{
public:
    CAR_INTERFACE_DECL()

    static CARAPI GetDefault(
        /* [out] */ IProxySelector** defaultSelector);

    static CARAPI SetDefault(
        /* [in] */ IProxySelector* selector);

    virtual CARAPI Select(
        /* [in] */ IURI* uri,
        /* [out] */ IList** container) = 0;

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
