#ifndef __CPROXY_H__
#define __CPROXY_H__

#include "_CProxy.h"

namespace Elastos {
namespace Net {

CarClass(CProxy)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ProxyType type,
        /* [in] */ ISocketAddress* sa);

    CARAPI GetType(
        /* [out] */ ProxyType* type);

    CARAPI GetAddress(
        /* [out] */ ISocketAddress** address);

    CARAPI static GetNO_PROXY(
        /* [out] */ IProxy** proxy);

public:
    static AutoPtr<IProxy> NO_PROXY;// = new Proxy();

private:
    ProxyType mType;

    AutoPtr<ISocketAddress> mAddress;
};

} // namespace Net
} // namespace Elastos

#endif //__CPROXY_H__
