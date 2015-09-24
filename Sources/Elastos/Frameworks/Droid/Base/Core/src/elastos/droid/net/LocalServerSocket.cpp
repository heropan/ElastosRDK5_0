
#include "elastos/droid/net/LocalServerSocket.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(LocalServerSocket, Object, ILocalServerSocket)

const Int32 LocalServerSocket::sLISTEN_BACKLOG = 50;

ECode LocalServerSocket::constructor(
    /* [in] */ const String& name)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    mImpl = new LocalSocketImpl();

    mImpl->Create(ILocalSocket::SOCKET_STREAM);

    FAIL_RETURN(CLocalSocketAddress::New(name, (ILocalSocketAddress**)&mLocalAddress));
    FAIL_RETURN(mImpl->Bind(mLocalAddress));
    return mImpl->Listen(LISTEN_BACKLOG);
#endif
}

ECode LocalServerSocket::constructor(
    /* [in] */ IFileDescriptor* fd)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    mImpl = new LocalSocketImpl(fd);
    FAIL_RETURN(mImpl->Listen(LISTEN_BACKLOG));
    return mImpl->GetSockAddress((ILocalSocketAddress**)&mLocalAddress);
#endif
}

ECode LocalServerSocket::GetLocalSocketAddress(
    /* [out] */ ILocalSocketAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mLocalAddress;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

ECode LocalServerSocket::Accept(
    /* [out] */ ILocalSocket** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);

    LocalSocketImpl* acceptedImpl = new LocalSocketImpl();

    mImpl->Accept(acceptedImpl);

    AutoPtr<ILocalSocket> lsocket;
    CLocalSocket::New((Handle32)acceptedImpl, ILocalSocket::SOCKET_UNKNOWN, (ILocalSocket**)&lsocket);
    *result = lsocket;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

ECode LocalServerSocket::GetFileDescriptor(
    /* [out] */ IFileDescriptor** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    AutoPtr<IFileDescriptor> fd;
    fd = mImpl->GetFileDescriptor();
    *result = fd;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

ECode LocalServerSocket::Close()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    return mImpl->Close();
#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
