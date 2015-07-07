
#include "CDefaultServerSocketFactory.h"
#include "CServerSocket.h"

using Elastos::Net::CServerSocket;

namespace Elastosx {
namespace Net {

CAR_OBJECT_IMPL(CDefaultServerSocketFactory)

ECode CDefaultServerSocketFactory::CreateServerSocket(
    /* [out] */ IServerSocket** sock)
{
    VALIDATE_NOT_NULL(sock)

    return  CServerSocket::New(sock);
}

ECode CDefaultServerSocketFactory::CreateServerSocket(
    /* [in] */ Int32 port,
    /* [out] */ IServerSocket** sock)
{
    VALIDATE_NOT_NULL(sock)

    return  CServerSocket::New(port, sock);
}

ECode CDefaultServerSocketFactory::CreateServerSocket(
    /* [in] */ Int32 port,
    /* [in] */ Int32 backlog,
    /* [out] */ IServerSocket** sock)
{
    VALIDATE_NOT_NULL(sock)

    return  CServerSocket::New(port, backlog, sock);
}

ECode CDefaultServerSocketFactory::CreateServerSocket(
    /* [in] */ Int32 port,
    /* [in] */ Int32 backlog,
    /* [in] */ IInetAddress* iAddress,
    /* [out] */ IServerSocket** sock)
{
    VALIDATE_NOT_NULL(sock)

    return  CServerSocket::New(port, backlog, iAddress, sock);
}

} // namespace Net
} // namespace Elastosx

