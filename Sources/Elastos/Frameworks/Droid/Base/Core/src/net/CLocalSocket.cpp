
#include "ext/frameworkdef.h"
#include "net/CLocalSocket.h"

using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Net::ISocketOptions;

namespace Elastos {
namespace Droid {
namespace Net {

CLocalSocket::CLocalSocket()
    : mImplCreated(FALSE)
    , mIsBound(FALSE)
    , mIsConnected(FALSE)
{}

/**
 * Creates a AF_LOCAL/UNIX domain stream socket.
 */
ECode CLocalSocket::constructor()
{
    AutoPtr<LocalSocketImpl> socketImpl = new LocalSocketImpl();
    constructor((Handle32)socketImpl.Get());
    mIsBound = FALSE;
    mIsConnected = FALSE;
    return NOERROR;
}

/**
 * Creates a AF_LOCAL/UNIX domain stream socket with FileDescriptor.
 * @hide
 */
ECode CLocalSocket::constructor(
    /* [in] */ IFileDescriptor* fd)
{
    AutoPtr<LocalSocketImpl> socketImpl = new LocalSocketImpl(fd);
    constructor((Handle32)socketImpl.Get());
    mIsBound = TRUE;
    mIsConnected = TRUE;
    return NOERROR;
}

/**
 * for use with AndroidServerSocket
 * @param impl a SocketImpl
 */
ECode CLocalSocket::constructor(
    /* [in] */ Handle32 impl)
{
    mImpl = (LocalSocketImpl*)impl;
    mIsConnected = FALSE;
    mIsBound = FALSE;
    return NOERROR;
}

/** {@inheritDoc} */
ECode CLocalSocket::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
//    return super.toString() + " impl:" + mImpl;
    assert(0);
    return NOERROR;
}

/**
 * It's difficult to discern from the spec when impl.create() should be
 * called, but it seems like a reasonable rule is "as soon as possible,
 * but not in a context where IOException cannot be thrown"
 *
 * @throws IOException from SocketImpl.create()
 */
void CLocalSocket::ImplCreateIfNeeded()
{
    if (!mImplCreated) {
        AutoLock lock(mLock);

        if (!mImplCreated) {
//            try {
            mImpl->Create(TRUE);
//            } finally {
            mImplCreated = TRUE;
//            }
        }
    }
}

/**
 * Connects this socket to an endpoint. May only be called on an instance
 * that has not yet been connected.
 *
 * @param endpoint endpoint address
 * @throws IOException if socket is in invalid state or the address does
 * not exist.
 */
ECode CLocalSocket::Connect(
    /* [in] */ ILocalSocketAddress* endpoint)
{
    AutoLock lock(mLock);

    if (mIsConnected) {
//        throw new IOException("already connected");
        return E_IO_EXCEPTION;
    }

    ImplCreateIfNeeded();
    mImpl->Connect(endpoint, 0);
    mIsConnected = TRUE;
    mIsBound = TRUE;
    return NOERROR;
}

/**
 * Binds this socket to an endpoint name. May only be called on an instance
 * that has not yet been bound.
 *
 * @param bindpoint endpoint address
 * @throws IOException
 */
ECode CLocalSocket::Bind(
    /* [in] */ ILocalSocketAddress* bindpoint)
{
    ImplCreateIfNeeded();

    {
        AutoLock lock(mLock);

        if (mIsBound) {
//            throw new IOException("already bound");
            return E_IO_EXCEPTION;
        }

        mLocalAddress = bindpoint;
        mImpl->Bind(mLocalAddress);
        mIsBound = TRUE;
    }
    return NOERROR;
}

/**
 * Retrieves the name that this socket is bound to, if any.
 *
 * @return Local address or null if anonymous
 */
ECode CLocalSocket::GetLocalSocketAddress(
    /* [out] */ ILocalSocketAddress** address)
{
    VALIDATE_NOT_NULL(address);
    *address = mLocalAddress;
    REFCOUNT_ADD(*address)
    return NOERROR;
}

/**
 * Retrieves the input stream for this instance.
 *
 * @return input stream
 * @throws IOException if socket has been closed or cannot be created.
 */
ECode CLocalSocket::GetInputStream(
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is);
    ImplCreateIfNeeded();
    return mImpl->GetInputStream(is);
}

/**
 * Retrieves the output stream for this instance.
 *
 * @return output stream
 * @throws IOException if socket has been closed or cannot be created.
 */
ECode CLocalSocket::GetOutputStream(
    /* [out] */ IOutputStream** os)
{
    VALIDATE_NOT_NULL(os);
    ImplCreateIfNeeded();
    return mImpl->GetOutputStream(os);
}

/**
 * Closes the socket.
 *
 * @throws IOException
 */
ECode CLocalSocket::Close()
{
    ImplCreateIfNeeded();
    return mImpl->Close();
}

/**
 * Shuts down the input side of the socket.
 *
 * @throws IOException
 */
ECode CLocalSocket::ShutdownInput()
{
    ImplCreateIfNeeded();
    return mImpl->ShutdownInput();
}

/**
 * Shuts down the output side of the socket.
 *
 * @throws IOException
 */
ECode CLocalSocket::ShutdownOutput()
{
    ImplCreateIfNeeded();
    return mImpl->ShutdownOutput();
}

ECode CLocalSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    AutoPtr<IInteger32> sizeObj;
    CInteger32::New(size, (IInteger32**)&sizeObj);
    return mImpl->SetOption(ISocketOptions::SO_RCVBUF, sizeObj);
}

ECode CLocalSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    AutoPtr<IInteger32> sizeObj;
    FAIL_RETURN(mImpl->GetOption(ISocketOptions::SO_RCVBUF, (IInterface**)&sizeObj));
    return sizeObj->GetValue(size);
}

ECode CLocalSocket::SetSoTimeout(
    /* [in] */ Int32 n)
{
    AutoPtr<IInteger32> nObj;
    CInteger32::New(n, (IInteger32**)&nObj);
    return mImpl->SetOption(ISocketOptions::SO_TIMEOUT, nObj);
}

ECode CLocalSocket::GetSoTimeout(
  /* [out] */ Int32* n)
{
    VALIDATE_NOT_NULL(n);
    AutoPtr<IInteger32> nObj;
    FAIL_RETURN(mImpl->GetOption(ISocketOptions::SO_TIMEOUT, (IInterface**)&nObj));
    return nObj->GetValue(n);
}

ECode CLocalSocket::SetSendBufferSize(
    /* [in] */ Int32 size)
{
    AutoPtr<IInteger32> sizeObj;
    CInteger32::New(size, (IInteger32**)&sizeObj);
    return mImpl->SetOption(ISocketOptions::SO_SNDBUF, sizeObj);
}

ECode CLocalSocket::GetSendBufferSize(
  /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    AutoPtr<IInteger32> sizeObj;
    FAIL_RETURN(mImpl->GetOption(ISocketOptions::SO_SNDBUF, (IInterface**)&sizeObj));
    return sizeObj->GetValue(size);
}

//???SEC
ECode CLocalSocket::GetRemoteSocketAddress(
  /* [out] */ ILocalSocketAddress** result)
{
    VALIDATE_NOT_NULL(result);
//    throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//???SEC
ECode CLocalSocket::IsConnected(
  /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(mLock);
    *result = mIsConnected;
    return NOERROR;
}

//???SEC
ECode CLocalSocket::IsClosed(
  /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
//    throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//???SEC
ECode CLocalSocket::IsBound(
  /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(mLock);
    *result = mIsBound;
    return NOERROR;
}

//???SEC
ECode CLocalSocket::IsOutputShutdown(
  /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
//    throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//???SEC
ECode CLocalSocket::IsInputShutdown(
  /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
//    throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//???SEC
ECode CLocalSocket::Connect(
  /* [in] */ ILocalSocketAddress* endpoint,
  /* [in] */ Int32 timeout)
{
//    throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

/**
 * Enqueues a set of file descriptors to send to the peer. The queue
 * is one deep. The file descriptors will be sent with the next write
 * of normal data, and will be delivered in a single ancillary message.
 * See "man 7 unix" SCM_RIGHTS on a desktop Linux machine.
 *
 * @param fds non-null; file descriptors to send.
 */
ECode CLocalSocket::SetFileDescriptorsForSend(
  /* [in] */ ArrayOf<IFileDescriptor*>* fds)
{
    mImpl->SetFileDescriptorsForSend(fds);
    return NOERROR;
}

/**
 * Retrieves a set of file descriptors that a peer has sent through
 * an ancillary message. This method retrieves the most recent set sent,
 * and then returns null until a new set arrives.
 * File descriptors may only be passed along with regular data, so this
 * method can only return a non-null after a read operation.
 *
 * @return null or file descriptor array
 * @throws IOException
 */
ECode CLocalSocket::GetAncillaryFileDescriptors(
    /* [out, callee] */ ArrayOf<IFileDescriptor*>** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr< ArrayOf<IFileDescriptor*> > fds = mImpl->GetAncillaryFileDescriptors();
    *result = fds;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Retrieves the credentials of this socket's peer. Only valid on
 * connected sockets.
 *
 * @return non-null; peer credentials
 * @throws IOException
 */
ECode CLocalSocket::GetPeerCredentials(
    /* [out] */ ICredentials** result)
{
    VALIDATE_NOT_NULL(result);
    return mImpl->GetPeerCredentials(result);
}

/**
 * Returns file descriptor or null if not yet open/already closed
 *
 * @return fd or null
 */
ECode CLocalSocket::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);
    AutoPtr<IFileDescriptor> _fd = mImpl->GetFileDescriptor();
    *fd = _fd;
    REFCOUNT_ADD(*fd);
    return NOERROR;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
