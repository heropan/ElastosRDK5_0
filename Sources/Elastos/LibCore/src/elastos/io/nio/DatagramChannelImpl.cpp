#include "DatagramChannelImpl.h"
#include "CIoBridge.h"
#include "CLibcore.h"
#include "CFileDescriptor.h"
#include "SelectorProvider.h"
#include "CInetSocketAddress.h"
#include "CPlainDatagramSocketImpl.h"
#include "CDatagramPacket.h"
#include "InetAddress.h"
#include "AutoLock.h"

using Elastos::Core::AutoLock;
using Libcore::IO::IIoBridge;
using Libcore::IO::CIoBridge;
using Libcore::IO::ILibcore;
using Libcore::IO::CLibcore;
using Libcore::IO::IOs;
using Elastos::Net::CInetSocketAddress;
using Elastos::Net::EIID_IDatagramSocket;
using Elastos::Net::IPlainDatagramSocketImpl;
using Elastos::Net::CPlainDatagramSocketImpl;
using Elastos::Net::IDatagramPacket;
using Elastos::Net::CDatagramPacket;
using Elastos::Net::InetAddress;
using Elastos::IO::Channels::EIID_IDatagramChannel;

namespace Elastos{
namespace IO {

//==========================================================
//       DatagramChannelImpl::DatagramSocketAdapter
//==========================================================

DatagramChannelImpl::DatagramSocketAdapter::DatagramSocketAdapter()
{}

ECode DatagramChannelImpl::DatagramSocketAdapter::constructor(
    /* [in] */ IDatagramSocketImpl* socketimpl,
    /* [in] */ DatagramChannelImpl* channelimpl)
{
    FAIL_RETURN(DatagramSocket::constructor(socketimpl))

    mChannelImpl = channelimpl;

    // Sync state socket state with the channel it is being created from
    if (mChannelImpl->mIsBound) {
        OnBind(mChannelImpl->mLocalAddress, mChannelImpl->mLocalPort);
    }
    if (mChannelImpl->mConnected) {
        AutoPtr<IInetAddress> addr;
        Int32 numport;
        mChannelImpl->mConnectAddress->GetAddress((IInetAddress**)&addr);
        mChannelImpl->mConnectAddress->GetPort(&numport);
        OnConnect(addr, numport);
    }
    else {
        OnDisconnect();
    }

    Boolean isflag = FALSE;
    if (mChannelImpl->IsOpen(&isflag), !isflag) {
        OnClose();
    }
    return NOERROR;
}

ECode DatagramChannelImpl::DatagramSocketAdapter::GetChannel(
    /* [out] */ IDatagramChannel** channel)
{
    VALIDATE_NOT_NULL(channel)

    *channel = IDatagramChannel::Probe(mChannelImpl);
    REFCOUNT_ADD(*channel)
    return NOERROR;
}

ECode DatagramChannelImpl::DatagramSocketAdapter::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
    if (mChannelImpl->IsConnected()) {
        // throw new AlreadyConnectedException();
        return E_ALREADY_CONNECTED_EXCEPTION;
    }

    FAIL_RETURN(DatagramSocket::Bind(localAddr))
    assert(0);
    // mChannelImpl->OnBind(FALSE /* updateSocketState */);
    return NOERROR;
}

ECode DatagramChannelImpl::DatagramSocketAdapter::Connect(
    /* [in] */ ISocketAddress* peer)
{
    Boolean bval;
    if (IsConnected(&bval), bval) {
        // RI compatibility: If the socket is already connected this fails.
        // throw new IllegalStateException("Socket is already connected.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    FAIL_RETURN(DatagramSocket::Connect(peer))
    // Connect may have performed an implicit bind(). Sync up here.
    assert(0);
    // mChannelImpl->OnBind(FALSE /* updateSocketState */);

    IInetSocketAddress* inetSocketAddress = IInetSocketAddress::Probe(peer);
    AutoPtr<IInetAddress> address;
    inetSocketAddress->GetAddress((IInetAddress**)&address);
    Int32 port;
    inetSocketAddress->GetPort(&port);
    assert(0);
    // return mChannelImpl->OnConnect(address, port, FALSE /* updateSocketState */);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::Connect(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port)
{
    // To avoid implementing connect() twice call this.connect(SocketAddress) in preference
    // to super.connect().
    // try {
    AutoPtr<ISocketAddress> isa;
    CInetSocketAddress::New(address, port, (ISocketAddress**)&isa);
    Connect(isa);
    // } catch (SocketException e) {
    //     // Ignored - there is nothing we can report here.
    // }
    return NOERROR;
}

ECode DatagramChannelImpl::DatagramSocketAdapter::Receive(
    /* [in] */ IDatagramPacket* pack)
{
    Boolean isflag = FALSE;
    if (mChannelImpl->IsBlocking(&isflag), !isflag) {
        // throw new IllegalBlockingModeException();
        return E_ILLEGAL_BLOCKING_MODE_EXCEPTION;
    }

    Boolean wasBound;
    IsBound(&wasBound);
    FAIL_RETURN(DatagramSocket::Receive(pack))
    if (!wasBound) {
        // DatagramSocket.receive() will implicitly bind if it hasn't been done explicitly.
        // Sync the channel state with the socket.
        assert(0);
        // mChannelImpl->OnBind(FALSE /* updateSocketState */);
    }
    return NOERROR;
}

ECode DatagramChannelImpl::DatagramSocketAdapter::Send(
    /* [in] */ IDatagramPacket* pack)
{
    Boolean isflag = FALSE;
    if (mChannelImpl->IsBlocking(&isflag), !isflag) {
        // throw new IllegalBlockingModeException();
        return E_ILLEGAL_BLOCKING_MODE_EXCEPTION;
    }
    return DatagramSocket::Send(pack);

    // DatagramSocket.send() will implicitly bind if it hasn't been done explicitly. Force
    // bind() here so that the channel state stays in sync with the socket.
    Boolean wasBound;
    IsBound(&wasBound);
    FAIL_RETURN(DatagramSocket::Send(pack))
    if (!wasBound) {
        // DatagramSocket.send() will implicitly bind if it hasn't been done explicitly.
        // Sync the channel state with the socket.
        assert(0);
        //mChannelImpl->OnBind(FALSE /* updateSocketState */);
    }
    return NOERROR;
}

ECode DatagramChannelImpl::DatagramSocketAdapter::Close()
{
    synchronized(mChannelImpl) {
        FAIL_RETURN(DatagramSocket::Close())

        Boolean isflag = FALSE;
        if (mChannelImpl->IsOpen(&isflag), isflag) {
            // try {
            return mChannelImpl->Close();
            // } catch (IOException e) {
                // Ignore
            // }
        }
    }

    return NOERROR;
}

ECode DatagramChannelImpl::DatagramSocketAdapter::Disconnect()
{
    FAIL_RETURN(DatagramSocket::Disconnect())
    assert(0);
    //return mChannelImpl->OnDisconnect(FALSE /* updateSocketState */);
}

//==========================================================
//       DatagramChannelImpl
//==========================================================
CAR_INTERFACE_IMPL(DatagramChannelImpl, DatagramChannel, IFileDescriptorChannel)

DatagramChannelImpl::DatagramChannelImpl(
    /* [in] */ ISelectorProvider* provider)
    : DatagramChannel(provider)
{
    CIoBridge::_Socket(FALSE, (IFileDescriptor**)&mFd);
}

DatagramChannelImpl::DatagramChannelImpl()
    : DatagramChannel((SelectorProvider::GetProvider((ISelectorProvider**)&mSelprovider), mSelprovider))
{
    CFileDescriptor::New((IFileDescriptor**)&mFd);
    CInetSocketAddress::New(0, (IInetSocketAddress**)&mConnectAddress);
}

ECode DatagramChannelImpl::IsOpen(
    /* [out] */ Boolean* value)
{
    return AbstractSelectableChannel::IsOpen(value);
}

ECode DatagramChannelImpl::Close()
{
    return AbstractInterruptibleChannel::Close();
}

ECode DatagramChannelImpl::GetSocket(
    /* [out] */ IDatagramSocket** socket)
{
    VALIDATE_NOT_NULL(socket)
    *socket = NULL;

    if(NULL == mSocket) {
        AutoPtr<IPlainDatagramSocketImpl> res;
        FAIL_RETURN(CPlainDatagramSocketImpl::New(mFd, mLocalPort, (IPlainDatagramSocketImpl**)&res));
        AutoPtr<DatagramSocketAdapter> dsa = new DatagramSocketAdapter();
        FAIL_RETURN(dsa->constructor(IDatagramSocketImpl::Probe(res), this))
        mSocket = IDatagramSocket::Probe(dsa);
    }

    *socket = mSocket;
    REFCOUNT_ADD(*socket);
    return NOERROR;
}

ECode DatagramChannelImpl::GetLocalAddress(
    /* [out] */ IInetAddress** addr)
{
    return CIoBridge::_GetSocketLocalAddress(mFd, addr);
}

Boolean DatagramChannelImpl::IsConnected()
{
    return mConnected;
}

ECode DatagramChannelImpl::Connect(
    /* [in] */ ISocketAddress* address,
    /* [out] */ IDatagramChannel** channel)
{
    VALIDATE_NOT_NULL(channel)
    if (mConnected) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return E_NOT_IMPLEMENTED;
}

ECode DatagramChannelImpl::Disconnect()
{
    Boolean bConnected, bOpen;
    bConnected = IsConnected();
    AbstractInterruptibleChannel::IsOpen(&bOpen);

    if (!bOpen || !bConnected) {
        return NOERROR;
    }

    mConnected = FALSE;
    mConnectAddress = NULL;
    // try {
    FAIL_RETURN(CLibcore::sOs->Connect(mFd, InetAddress::UNSPECIFIED, 0));
    // } catch (ErrnoException errnoException) {
        // throw errnoException.rethrowAsIOException();
    // }
    if (mSocket != NULL) {
        mSocket->Disconnect();
    }
    return NOERROR;
}

ECode DatagramChannelImpl::Receive(
    /* [in] */ IByteBuffer* target,
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address)

    Boolean bRet;
    IBuffer::Probe(target)->IsReadOnly(&bRet);
    if(bRet)
    {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    ECode ecRet;
    if(NOERROR != ecRet)
        return ecRet;

    if(!mIsBound)
    {
        return E_NULL_POINTER_EXCEPTION;
    }

    Begin();

    mReadLock.Lock();
    Boolean loop;
    IsBlocking(&loop);

    Boolean bDirect;
    IBuffer::Probe(target)->IsDirect(&bDirect);
    if(!target)
    {
        ecRet = ReceiveImpl(target, loop, address);
    }

    do
    {
        if (NOERROR != ecRet)
        {
            break;
        }

        REFCOUNT_ADD(*address);
    } while (0);

    End((*address) != NULL);
    return ecRet;
}

ECode DatagramChannelImpl::ReceiveImpl(
    /* [in] */ IByteBuffer* target,
    /* [in] */ Boolean loop,
    /* [out] */ ISocketAddress** addr)
{
    VALIDATE_NOT_NULL(addr)
    *addr = NULL;

    AutoPtr<ISocketAddress> retAddr;
    AutoPtr<IDatagramPacket> receivePacket;
    Int32 oldPosition;
    IBuffer::Probe(target)->GetPosition(&oldPosition);
    Int32 received = 0;

    // TODO: disallow mapped buffers and lose this conditional?
    Boolean isflag = FALSE;
    if (IBuffer::Probe(target)->HasArray(&isflag), isflag) {
        AutoPtr< ArrayOf<Byte> > bytearr;
        target->GetArray((ArrayOf<Byte>**)&bytearr);
        Int32 offset = 0;
        Int32 remain = 0;
        IBuffer::Probe(target)->GetArrayOffset(&offset);
        IBuffer::Probe(target)->GetRemaining(&remain);
        FAIL_RETURN(CDatagramPacket::New(bytearr, oldPosition + offset, remain, (IDatagramPacket**)&receivePacket));
    }
    else {
        Int32 remain = 0;
        IBuffer::Probe(target)->GetRemaining(&remain);
        AutoPtr< ArrayOf<Byte> > bytearr = ArrayOf<Byte>::Alloc(remain);
        FAIL_RETURN(CDatagramPacket::New(bytearr, remain, (IDatagramPacket**)&receivePacket));
    }
    do {
        AutoPtr< ArrayOf<Byte> > bytearr;
        receivePacket->GetData((ArrayOf<Byte>**)&bytearr);
        Int32 offset = 0;
        receivePacket->GetOffset(&offset);
        Int32 length = 0;
        receivePacket->GetLength(&length);
        FAIL_RETURN(CIoBridge::_Recvfrom(FALSE, mFd, bytearr, offset, length, 0, receivePacket, IsConnected(), &received));
        AutoPtr<IInetAddress> outnet;
        receivePacket->GetAddress((IInetAddress**)&outnet);
        if (receivePacket != NULL && outnet != NULL) {
            if (received > 0) {
                if (IBuffer::Probe(target)->HasArray(&isflag), isflag) {
                    IBuffer::Probe(target)->SetPosition(oldPosition + received);
                }
                else {
                    // copy the data of received packet
                    target->Put(bytearr, 0, received);
                }
            }
            receivePacket->GetSocketAddress((ISocketAddress**)&retAddr);
            break;
        }
    } while (loop);
    *addr = retAddr;
    REFCOUNT_ADD(*addr)
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
