
#include "CProxy.h"
#include <elastos/core/StringBuilder.h>

namespace Elastos {
namespace Net {

AutoPtr<IProxy> StaticInitProxy()
{
    AutoPtr<CProxy> proxy;
    CProxy::NewByFriend((CProxy**)&proxy);
    return (IProxy*)proxy.Get();
}

AutoPtr<IProxy> CProxy::NO_PROXY = StaticInitProxy();

ECode CProxy::constructor(
    /* [in] */ ProxyType type,
    /* [in] */ ISocketAddress* sa)
{
    /*
     * Don't use DIRECT type to construct a proxy instance directly.
     * SocketAddress must NOT be null.
     */
    if (type == ProxyType_DIRECT || sa == NULL) {
//        throw new IllegalArgumentException("Illegal Proxy.Type or SocketAddress argument");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mType = type;
    mAddress = sa;
    return NOERROR;
}

ECode CProxy::constructor()
{
    mType = ProxyType_DIRECT;
    mAddress = NULL;
    return NOERROR;
}

ECode CProxy::GetType(
    /* [out] */ ProxyType* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;
    return NOERROR;
}

ECode CProxy::GetAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);
    *address = mAddress;
    REFCOUNT_ADD(*address);
    return NOERROR;
}

ECode CProxy::GetNO_PROXY(
    /* [out] */ IProxy** proxy)
{
    VALIDATE_NOT_NULL(proxy);
    *proxy = NO_PROXY;
    REFCOUNT_ADD(*proxy);
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
