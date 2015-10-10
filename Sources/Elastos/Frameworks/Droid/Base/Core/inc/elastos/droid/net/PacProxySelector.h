
#ifndef __ELASTOS_DROID_NET_PACPROXYSELECTOR_H__
#define __ELASTOS_DROID_NET_PACPROXYSELECTOR_H__

#include "elastos/droid/ext/frameworkext.h"
// #include <elastos/net/ProxySelector.h>

using Elastos::Net::IURI;
using Elastos::Net::ProxyType;
using Elastos::Droid::Net::IIProxyService;
using Elastos::Utility::IList;
using Elastos::Net::ISocketAddress;

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * @hide
 */
class PacProxySelector
#if 0 // TODO: Waiting for outport ProxySelector
    : public ProxySelector
#else
    : public Object
#endif
    , public IPacProxySelector
{
public:
    CAR_INTERFACE_DECL()

    CARAPI constructor();

    // @Override
    CARAPI Select(
        /* [in] */ IURI* uri,
        /* [out] */ IList** result);

    // @Override
    CARAPI ConnectFailed(
        /* [in] */ IURI* uri,
        /* [in] */ ISocketAddress* address,
        /* [in] */ ECode failure);

private:
    static CARAPI ParseResponse(
        /* [in] */ const String& response,
        /* [out] */ IList** result);

    static CARAPI ProxyFromHostPort(
        /* [in] */ ProxyType type,
        /* [in] */ const String& hostPortString,
        /* [out] */ Elastos::Net::IProxy** result);

    static const String TAG;

    static const String SOCKS;

    static const String PROXY;

    IIProxyService* mProxyService;

    IList* mDefaultList;

};

} // namespace Net
} // namespace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_NET_PACPROXYSELECTOR_H__
