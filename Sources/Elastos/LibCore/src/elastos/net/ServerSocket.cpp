
#include "ServerSocket.h"
#include "CInet4Address.h"
#include "CInetSocketAddress.h"
#include <stdio.h>
#include "CPlainServerSocketImpl.h"
#include "CPlainSocketImpl.h"
#include "CInteger32.h"
#include "CBoolean.h"
#include "CSocket.h"
#include "Autolock.h"

using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::IO::EIID_ICloseable;
using Elastos::Net::ISocketImpl;
using Elastos::Net::CSocket;
using Elastos::Net::CInet4Address;
using Elastos::Net::CPlainSocketImpl;
using Elastos::Net::CPlainServerSocketImpl;

namespace Elastos {
namespace Net {

AutoPtr<ISocketImplFactory> ServerSocket::sFactory;

CAR_INTERFACE_IMPL_2(ServerSocket, Object, IServerSocket, ICloseable)

ServerSocket::ServerSocket()
    : mIsCreated(FALSE)
    , mIsBound(FALSE)
    , mIsClosed(FALSE)
{}

ECode ServerSocket::constructor()
{
    if (sFactory != NULL) {
        return sFactory->CreateSocketImpl((ISocketImpl**)&mImpl);
    }
    else {
        AutoPtr<CPlainServerSocketImpl> pss;
        FAIL_RETURN(CPlainServerSocketImpl::NewByFriend((CPlainServerSocketImpl**)&pss));
        mImpl = (ISocketImpl*) pss->Probe(EIID_ISocketImpl);
        return NOERROR;
    }
}

ECode ServerSocket::constructor(
    /* [in] */ ISocketImpl* impl)
{
    mImpl = impl;
    return NOERROR;
}

ECode ServerSocket::constructor(
    /* [in] */ Int32 aPort)
{
    return constructor(aPort, DefaultBacklog(), CInet4Address::ANY);
}

ECode ServerSocket::constructor(
    /* [in] */ Int32 aPort,
    /* [in] */ Int32 backlog)
{
    return constructor(aPort, backlog, CInet4Address::ANY);
}

ECode ServerSocket::constructor(
    /* [in] */ Int32 aPort,
    /* [in] */ Int32 backlog,
    /* [in] */ IInetAddress* localAddr)
{
    FAIL_RETURN(CheckListen(aPort));
    ECode ec = NOERROR;
    if (sFactory != NULL) {
        ec = sFactory->CreateSocketImpl((ISocketImpl**)&mImpl);
        if (FAILED(ec)) {
            return ec;
        }
    }
    else {
        AutoPtr<CPlainServerSocketImpl> pss;
        ec = CPlainServerSocketImpl::NewByFriend((CPlainServerSocketImpl**)&pss);
        if (FAILED(ec)) {
            return ec;
        }
        mImpl = (ISocketImpl*)pss->Probe(EIID_ISocketImpl);
    }

    AutoPtr<IInetAddress> addr = localAddr == NULL ? CInet4Address::ANY.Get() : localAddr;

    synchronized (this) {
        mImpl->Create(TRUE);
        mIsCreated = TRUE;
    //    try {
        ec = mImpl->Bind(addr, aPort);
        if (FAILED(ec)) {
            Close();
            return ec;
        }

        mIsBound = TRUE;
        ec = mImpl->Listen(backlog > 0 ? backlog : DefaultBacklog());
        if (FAILED(ec)) {
            Close();
            return ec;
        }
    }
//    } catch (IOException e) {
//        close();
//        throw e;
//    }

    return NOERROR;
}

ECode ServerSocket::GetImpl(
    /* [out] */ ISocketImpl** impl)
{
    VALIDATE_NOT_NULL(impl);
    *impl = mImpl;
    REFCOUNT_ADD(*impl);
    return NOERROR;
}

ECode ServerSocket::Accept(
    /* [out] */ ISocket** socket)
{
    VALIDATE_NOT_NULL(socket);
    FAIL_RETURN(CheckClosedAndCreate(FALSE));
    Boolean isBound;
    if (IsBound(&isBound), !isBound) {
        // throw new SocketException("Socket is not bound");
        return E_SOCKET_EXCEPTION;
    }

    AutoPtr<ISocket> socketObj;
    FAIL_RETURN(CSocket::New((ISocket**)&socketObj));
    // try {
    ECode ec = ImplAccept(socketObj);
    if (FAILED(ec)) {
        socketObj->Close();
        *socket = NULL;
        return ec;
    }
    // } catch (SecurityException e) {
    //    aSocket.close();
    //    throw e;
    // } catch (IOException e) {
    //    aSocket.close();
    //    throw e;
    // }
    *socket = socketObj;
    REFCOUNT_ADD(*socket);
    return NOERROR;
}

ECode ServerSocket::CheckListen(
    /* [in] */ Int32 aPort)
{
    if (aPort < 0 || aPort > 65535) {
//        throw new IllegalArgumentException("Port out of range: " + aPort);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkListen(aPort);
//    }
    return NOERROR;
}

ECode ServerSocket::Close()
{
    mIsClosed = TRUE;
    return mImpl->Close();
}

Int32 ServerSocket::DefaultBacklog()
{
    return 50;
}

ECode ServerSocket::GetInetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    Boolean isBound;
    if (IsBound(&isBound), !isBound) {
        *address = NULL;
        return NOERROR;
    }
    return mImpl->GetInetAddress(address);
}

ECode ServerSocket::GetLocalPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);

    Boolean isBound;
    if (IsBound(&isBound), !isBound) {
        *port = -1;
        return NOERROR;
    }
    return mImpl->GetLocalPort(port);
}

ECode ServerSocket::GetSoTimeout(
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout);

    if (!mIsCreated) {
        //Mutex::Autolock lock(&mLock);

        if (!mIsCreated) {
//            try {
            ECode ec = mImpl->Create(TRUE);
            if (ec == E_SOCKET_EXCEPTION) {
                return ec;
            }
            else if (ec == E_IO_EXCEPTION) {
                return E_SOCKET_EXCEPTION;
            }
//            } catch (SocketException e) {
//                throw e;
//            } catch (IOException e) {
//                throw new SocketException(e.toString());
//            }
            mIsCreated = TRUE;
        }
    }
    AutoPtr<IInteger32> optVal;
    ISocketOptions* option = (ISocketOptions*)mImpl->Probe(EIID_ISocketOptions);
    FAIL_RETURN(option->GetOption(ISocketOptions::_SO_TIMEOUT, (IInterface**)&optVal));
    return optVal->GetValue(timeout);
}

ECode ServerSocket::ImplAccept(
    /* [in] */ ISocket* aSocket)
{
    {
        //Mutex::Autolock lock(&mLock);
        mImpl->Accept(((CSocket*)aSocket)->mImpl);
        ((CSocket*)aSocket)->Accepted();
    }

    return NOERROR;

//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkAccept(aSocket.getInetAddress().getHostAddress(),
//                aSocket.getPort());
//    }
}

ECode ServerSocket::SetSocketFactory(
    /* [in] */ ISocketImplFactory* aFactory)
{
    //Mutex::Autolock lock(&sLock);

//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkSetFactory();
//    }
    if (sFactory != NULL) {
//        throw new SocketException("Factory already set");
        return E_SOCKET_EXCEPTION;
    }
    sFactory = aFactory;
    return NOERROR;
}

ECode ServerSocket::SetSoTimeout(
    /* [in] */ Int32 timeout)
{
    //Mutex::Autolock lock(&mLock);

    FAIL_RETURN(CheckClosedAndCreate(TRUE));
    if (timeout < 0) {
//        throw new IllegalArgumentException("timeout < 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IInteger32> optVal;
    CInteger32::New(timeout, (IInteger32**)&optVal);
    ISocketOptions* option = (ISocketOptions*)mImpl->Probe(EIID_ISocketOptions);
    return option->SetOption(ISocketOptions::_SO_TIMEOUT, optVal);
}

ECode ServerSocket::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
    return Bind(localAddr, DefaultBacklog());
}

ECode ServerSocket::Bind(
    /* [in] */ ISocketAddress* localAddr,
    /* [in] */ Int32 backlog)
{
    FAIL_RETURN(CheckClosedAndCreate(TRUE));
    Boolean isBound;
    if (IsBound(&isBound), isBound) {
//        throw new BindException("Socket is already bound");
        return E_BIND_EXCEPTION;
    }
    Int32 port = 0;
    AutoPtr<IInetAddress> addr = CInet4Address::ANY;
    if (localAddr != NULL) {
        if (IInetSocketAddress::Probe(localAddr) == NULL) {
//            throw new IllegalArgumentException("Local address not an InetSocketAddress: " +
//                    localAddr.getClass());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        IInetSocketAddress* inetAddr = IInetSocketAddress::Probe(localAddr);
        addr = NULL;
        inetAddr->GetAddress((IInetAddress**)&addr);
        if (addr == NULL) {
//            throw new SocketException("Host is unresolved: " + inetAddr.getHostName());
            return E_SOCKET_EXCEPTION;
        }
        inetAddr->GetPort(&port);
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkListen(port);
//    }

    //Mutex::Autolock lock(mLock);
//    try {
    ECode ec = mImpl->Bind(addr, port);
    if (FAILED(ec)) {
        Close();
        return ec;
    }
    mIsBound = TRUE;
    ec = mImpl->Listen(backlog > 0 ? backlog : DefaultBacklog());
    if (FAILED(ec)) {
        Close();
        return ec;
    }
    // } catch (IOException e) {
    //     close();
    //     throw e;
    // }

    return NOERROR;
}

ECode ServerSocket::GetLocalSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);

    Boolean isBound;
    if (IsBound(&isBound), !isBound) {
        *address = NULL;
        return NOERROR;
    }

    AutoPtr<IInetAddress> addr;
    FAIL_RETURN(GetInetAddress((IInetAddress**)&addr));
    Int32 port;
    FAIL_RETURN(GetLocalPort(&port));
    return CInetSocketAddress::New(addr, port, (IInetSocketAddress**)address);
}

ECode ServerSocket::IsBound(
    /* [out] */ Boolean* isBound)
{
    VALIDATE_NOT_NULL(isBound);
    *isBound = mIsBound;
    return NOERROR;
}

ECode ServerSocket::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    VALIDATE_NOT_NULL(isClosed);
    *isClosed = mIsClosed;
    return NOERROR;
}

ECode ServerSocket::CheckClosedAndCreate(
    /* [in] */ Boolean create)
{
    Boolean isClosed;
    if (IsClosed(&isClosed), isClosed) {
//        throw new SocketException("Socket is closed");
        return E_SOCKET_EXCEPTION;
    }

    if (!create || mIsCreated) {
        return NOERROR;
    }

    //Mutex::Autolock lock(mLock);

    if (mIsCreated) {
        return NOERROR;
    }
//    try {
    ECode ec = mImpl->Create(TRUE);
    if (FAILED(ec)) {
        return E_SOCKET_EXCEPTION;
    }
    // } catch (SocketException e) {
    //     throw e;
    // } catch (IOException e) {
    //     throw new SocketException(e.toString());
    // }
    mIsCreated = TRUE;

    return NOERROR;
}

ECode ServerSocket::SetReuseAddress(
    /* [in] */ Boolean reuse)
{
    CheckClosedAndCreate(TRUE);
    AutoPtr<IBoolean> optVal;
    CBoolean::New(reuse, (IBoolean**)&optVal);
    ISocketOptions* option = (ISocketOptions*)mImpl->Probe(EIID_ISocketOptions);
    return option->SetOption(ISocketOptions::_SO_REUSEADDR, optVal);
}

ECode ServerSocket::GetReuseAddress(
    /* [out] */ Boolean* reuse)
{
    VALIDATE_NOT_NULL(reuse);

    FAIL_RETURN(CheckClosedAndCreate(TRUE));
    AutoPtr<IBoolean> optVal;
    ISocketOptions* option = (ISocketOptions*)mImpl->Probe(EIID_ISocketOptions);
    FAIL_RETURN(option->GetOption(ISocketOptions::_SO_REUSEADDR, (IInterface**)&optVal));
    return optVal->GetValue(reuse);
}

ECode ServerSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    FAIL_RETURN(CheckClosedAndCreate(TRUE));
    if (size < 1) {
//        throw new IllegalArgumentException("size < 1");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IInteger32> optVal;
    CInteger32::New(size, (IInteger32**)&optVal);
    ISocketOptions* option = (ISocketOptions*)mImpl->Probe(EIID_ISocketOptions);
    return option->SetOption(ISocketOptions::_SO_RCVBUF, optVal);
}

ECode ServerSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    FAIL_RETURN(CheckClosedAndCreate(TRUE));
    AutoPtr<IInteger32> optVal;
    ISocketOptions* option = (ISocketOptions*)mImpl->Probe(EIID_ISocketOptions);
    FAIL_RETURN(option->GetOption(ISocketOptions::_SO_RCVBUF, (IInterface**)&optVal));
    return optVal->GetValue(size);
}

ECode ServerSocket::GetChannel(
    /* [out] */ IServerSocketChannel** channel)
{
    VALIDATE_NOT_NULL(channel);

    *channel = NULL;
    return NOERROR;
}

ECode ServerSocket::SetPerformancePreferences(
    /* [in] */ Int32 connectionTime,
    /* [in] */ Int32 latency,
    /* [in] */ Int32 bandwidth)
{
    // Our socket implementation only provide one protocol: TCP/IP, so
    // we do nothing for this method
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
