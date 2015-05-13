
#include "ext/frameworkdef.h"
#include "net/CLocalSocket.h"
#include "net/CLocalServerSocket.h"
#include "net/CLocalSocketAddress.h"

using Elastos::Droid::Net::CLocalSocket;
using Elastos::Droid::Net::CLocalSocketAddress;

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * Crewates a new server socket listening at specified name.
 * On the Android platform, the name is created in the Linux
 * abstract namespace (instead of on the filesystem).
 *
 * @param name address for socket
 * @throws IOException
 */
ECode CLocalServerSocket::constructor(
    /* [in] */ const String& name)
{
    mImpl = new LocalSocketImpl();

    mImpl->Create(TRUE);

    FAIL_RETURN(CLocalSocketAddress::New(name, (ILocalSocketAddress**)&mLocalAddress));
    FAIL_RETURN(mImpl->Bind(mLocalAddress));
    return mImpl->Listen(LISTEN_BACKLOG);
}

/**
 * Create a LocalServerSocket from a file descriptor that's already
 * been created and bound. listen() will be called immediately on it.
 * Used for cases where file descriptors are passed in via environment
 * variables
 *
 * @param fd bound file descriptor
 * @throws IOException
 */
ECode CLocalServerSocket::constructor(
    /* [in] */ IFileDescriptor* fd)
{
    mImpl = new LocalSocketImpl(fd);
    FAIL_RETURN(mImpl->Listen(LISTEN_BACKLOG));
    return mImpl->GetSockAddress((ILocalSocketAddress**)&mLocalAddress);
}

/**
 * Obtains the socket's local address
 *
 * @return local address
 */
ECode CLocalServerSocket::GetLocalSocketAddress(
    /* [out] */ ILocalSocketAddress** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mLocalAddress;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/**
 * Accepts a new connection to the socket. Blocks until a new
 * connection arrives.
 *
 * @return a socket representing the new connection.
 * @throws IOException
 */
ECode CLocalServerSocket::Accept(
    /* [out] */ ILocalSocket** result)
{
    VALIDATE_NOT_NULL(result);

    LocalSocketImpl* acceptedImpl = new LocalSocketImpl();

    mImpl->Accept(acceptedImpl);

    AutoPtr<ILocalSocket> lsocket;
    CLocalSocket::New((Handle32)acceptedImpl, (ILocalSocket**)&lsocket);
    *result = lsocket;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/**
 * Returns file descriptor or null if not yet open/already closed
 *
 * @return fd or null
 */
ECode CLocalServerSocket::GetFileDescriptor(
    /* [out] */ IFileDescriptor** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IFileDescriptor> fd;
    fd = mImpl->GetFileDescriptor();
    *result = fd;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/**
 * Closes server socket.
 *
 * @throws IOException
 */
ECode CLocalServerSocket::Close()
{
    return mImpl->Close();
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
