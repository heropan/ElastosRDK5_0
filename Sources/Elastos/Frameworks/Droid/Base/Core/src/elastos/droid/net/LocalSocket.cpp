
#include "elastos/droid/net/LocalSocket.h"

using Elastos::IO::EIID_ICloseable;

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(LocalSocket, Object, ILocalSocket, ICloseable)

const Int32 LocalSocket::SOCKET_UNKNOWN = 0;
const Int32 LocalSocket::SOCKET_DGRAM = 1;
const Int32 LocalSocket::SOCKET_STREAM = 2;
const Int32 LocalSocket::SOCKET_SEQPACKET = 3;

ECode LocalSocket::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(SOCKET_STREAM);

#endif
}

ECode LocalSocket::constructor(
    /* [in] */ Int32 sockType)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(new LocalSocketImpl(), sockType);
        isBound = false;
        isConnected = false;

#endif
}

ECode LocalSocket::constructor(
    /* [in] */ IFileDescriptor* fd)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(new LocalSocketImpl(fd), SOCKET_UNKNOWN);
        isBound = true;
        isConnected = true;

#endif
}

ECode LocalSocket::constructor(
    /* [in] */ ILocalSocketImpl* impl,
    /* [in] */ Int32 sockType)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.impl = impl;
        this.sockType = sockType;
        this.isConnected = false;
        this.isBound = false;

#endif
}

ECode LocalSocket::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return super.toString() + " impl:" + impl;

#endif
}

ECode LocalSocket::ImplCreateIfNeeded()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate before. Need check.
    if (!mImplCreated) {
        AutoLock lock(mLock);

        if (!mImplCreated) {
//            try {
            mImpl->Create(mSocketType);
//            } finally {
            mImplCreated = TRUE;
//            }
        }
    }
#endif
}

ECode LocalSocket::Connect(
    /* [in] */ ILocalSocketAddress* endpoint)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode LocalSocket::Bind(
    /* [in] */ ILocalSocketAddress* bindpoint)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode LocalSocket::GetLocalSocketAddress(
    /* [out] */ ILocalSocketAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(address);
    *address = mLocalAddress;
    REFCOUNT_ADD(*address)
    return NOERROR;
#endif
}

ECode LocalSocket::GetInputStream(
    /* [out] */ IInputStream** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(is);
    ImplCreateIfNeeded();
    return mImpl->GetInputStream(is);
#endif
}

ECode LocalSocket::GetOutputStream(
    /* [out] */ IOutputStream** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(os);
    ImplCreateIfNeeded();
    return mImpl->GetOutputStream(os);
#endif
}

ECode LocalSocket::Close()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    ImplCreateIfNeeded();
    return mImpl->Close();
#endif
}

ECode LocalSocket::ShutdownInput()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    ImplCreateIfNeeded();
    return mImpl->ShutdownInput();
#endif
}

ECode LocalSocket::ShutdownOutput()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    ImplCreateIfNeeded();
    return mImpl->ShutdownOutput();
#endif
}

ECode LocalSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    AutoPtr<IInteger32> sizeObj;
    CInteger32::New(size, (IInteger32**)&sizeObj);
    return mImpl->SetOption(ISocketOptions::SO_RCVBUF, sizeObj);
#endif
}

ECode LocalSocket::GetReceiveBufferSize(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(size);
    AutoPtr<IInteger32> sizeObj;
    FAIL_RETURN(mImpl->GetOption(ISocketOptions::SO_RCVBUF, (IInterface**)&sizeObj));
    return sizeObj->GetValue(size);
#endif
}

ECode LocalSocket::SetSoTimeout(
    /* [in] */ Int32 n)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    AutoPtr<IInteger32> nObj;
    CInteger32::New(n, (IInteger32**)&nObj);
    return mImpl->SetOption(ISocketOptions::SO_TIMEOUT, nObj);
#endif
}

ECode LocalSocket::GetSoTimeout(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(n);
    AutoPtr<IInteger32> nObj;
    FAIL_RETURN(mImpl->GetOption(ISocketOptions::SO_TIMEOUT, (IInterface**)&nObj));
    return nObj->GetValue(n);
#endif
}

ECode LocalSocket::SetSendBufferSize(
    /* [in] */ Int32 n)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    AutoPtr<IInteger32> sizeObj;
    CInteger32::New(size, (IInteger32**)&sizeObj);
    return mImpl->SetOption(ISocketOptions::SO_SNDBUF, sizeObj);
#endif
}

ECode LocalSocket::GetSendBufferSize(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(size);
    AutoPtr<IInteger32> sizeObj;
    FAIL_RETURN(mImpl->GetOption(ISocketOptions::SO_SNDBUF, (IInterface**)&sizeObj));
    return sizeObj->GetValue(size);
#endif
}

ECode LocalSocket::GetRemoteSocketAddress(
    /* [out] */ ILocalSocketAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
//    throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
#endif
}

ECode LocalSocket::IsConnected(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    AutoLock lock(mLock);
    *result = mIsConnected;
    return NOERROR;
#endif
}

ECode LocalSocket::IsClosed(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
//    throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
#endif
}

ECode LocalSocket::IsBound(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    AutoLock lock(mLock);
    *result = mIsBound;
    return NOERROR;
#endif
}

ECode LocalSocket::IsOutputShutdown(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
//    throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
#endif
}

ECode LocalSocket::IsInputShutdown(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
//    throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
#endif
}

ECode LocalSocket::Connect(
    /* [in] */ ILocalSocketAddress* endpoint,
    /* [in] */ Int32 timeout)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
//    throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
#endif
}

ECode LocalSocket::SetFileDescriptorsForSend(
    /* [in] */ ArrayOf<IFileDescriptor*>* fds)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    mImpl->SetFileDescriptorsForSend(fds);
    return NOERROR;
#endif
}

ECode LocalSocket::GetAncillaryFileDescriptors(
    /* [out, callee] */ ArrayOf<IFileDescriptor*>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    AutoPtr< ArrayOf<IFileDescriptor*> > fds = mImpl->GetAncillaryFileDescriptors();
    *result = fds;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

ECode LocalSocket::GetPeerCredentials(
    /* [out] */ ICredentials** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    return mImpl->GetPeerCredentials(result);
#endif
}

ECode LocalSocket::GetFileDescriptor(
    /* [out] */ IFileDescriptor** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(fd);
    AutoPtr<IFileDescriptor> _fd = mImpl->GetFileDescriptor();
    *fd = _fd;
    REFCOUNT_ADD(*fd);
    return NOERROR;
#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
