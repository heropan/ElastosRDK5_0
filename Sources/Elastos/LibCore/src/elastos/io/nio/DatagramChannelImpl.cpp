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

namespace Elastos{
namespace IO {

//==========================================================
//       DatagramChannelImpl::DatagramSocketAdapter
//==========================================================

DatagramChannelImpl::DatagramSocketAdapter::DatagramSocketAdapter(
    /* [in] */ IDatagramSocketImpl* socketimpl,
    /* [in] */ DatagramChannelImpl* channelimpl)
    : mChannelImpl(channelimpl)
{
    assert(0 && "TODO");
    // DatagramSocket::constructor(socketimpl);
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
}

ECode DatagramChannelImpl::DatagramSocketAdapter::Close()
{
    AutoLock lock(GetSelfLock());
    Boolean isflag = FALSE;
    if (mChannelImpl->IsOpen(&isflag), isflag) {
        // try {
        mChannelImpl->Close();
        // } catch (IOException e) {
            // Ignore
        // }
    }
    return DatagramSocket::Close();
}

ECode DatagramChannelImpl::DatagramSocketAdapter::Connect(
    /* [in] */ ISocketAddress* peer)
{
    return DatagramSocket::Connect(peer);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::Connect(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 aPort)
{
    return DatagramSocket::Connect(address, aPort);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::Disconnect()
{
    // try {
    mChannelImpl->Disconnect();
    // } catch (IOException e) {
    //     // Ignore
    // }
    return DatagramSocket::Disconnect();
}

ECode DatagramChannelImpl::DatagramSocketAdapter::GetInetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address)
    *address = NULL;

    if (mChannelImpl->mConnectAddress == NULL) {
        return NOERROR;
    }
    return mChannelImpl->mConnectAddress->GetAddress(address);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::GetLocalAddress(
    /* [out] */ IInetAddress** address)
{
    return mChannelImpl->GetLocalAddress(address);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::GetLocalPort(
    /* [out] */ Int32* port)
{
    return DatagramSocket::GetLocalPort(port);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port)

    if (mChannelImpl->mConnectAddress == NULL) {
        *port = -1;
        return NOERROR;
    }
    return mChannelImpl->mConnectAddress->GetPort(port);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    return DatagramSocket::GetReceiveBufferSize(size);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::GetSendBufferSize(
    /* [out] */ Int32* size)
{
    return DatagramSocket::GetSendBufferSize(size);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::GetSoTimeout(
    /* [out] */ Int32* timeout)
{
    return DatagramSocket::GetSoTimeout(timeout);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::Receive(
    /* [in] */ IDatagramPacket* pack)
{
    Boolean isflag = FALSE;
    if (mChannelImpl->IsBlocking(&isflag), !isflag) {
        // throw new IllegalBlockingModeException();
        return E_ILLEGAL_BLOCKING_MODE_EXCEPTION;
    }
    return DatagramSocket::Receive(pack);
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
}

ECode DatagramChannelImpl::DatagramSocketAdapter::SetNetworkInterface(
    /* [in] */ INetworkInterface* netInterface)
{
    return DatagramSocket::SetNetworkInterface(netInterface);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::SetSendBufferSize(
    /* [in] */ Int32 size)
{
    return DatagramSocket::SetSendBufferSize(size);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    return DatagramSocket::SetReceiveBufferSize(size);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::SetSoTimeout(
    /* [in] */ Int32 timeout)
{
    return DatagramSocket::SetSoTimeout(timeout);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
    if (mChannelImpl->IsConnected()) {
        // throw new AlreadyConnectedException();
        return E_ALREADY_CONNECTED_EXCEPTION;
    }
    DatagramSocket::Bind(localAddr);
    mChannelImpl->mIsBound = TRUE;
    return NOERROR;
}

ECode DatagramChannelImpl::DatagramSocketAdapter::IsBound(
    /* [out] */ Boolean* isBound)
{
    VALIDATE_NOT_NULL(isBound)

    *isBound = mChannelImpl->mIsBound;
    return NOERROR;
}

ECode DatagramChannelImpl::DatagramSocketAdapter::IsConnected(
    /* [out] */ Boolean* isConnected)
{
    VALIDATE_NOT_NULL(isConnected)

    *isConnected = mChannelImpl->IsConnected();
    return NOERROR;
}

ECode DatagramChannelImpl::DatagramSocketAdapter::GetRemoteSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    return DatagramSocket::GetRemoteSocketAddress(address);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::GetLocalSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    return DatagramSocket::GetLocalSocketAddress(address);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::SetReuseAddress(
    /* [in] */ Boolean reuse)
{
    return DatagramSocket::SetReuseAddress(reuse);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::GetReuseAddress(
    /* [out] */ Boolean* reuse)
{
    return DatagramSocket::GetReuseAddress(reuse);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::SetBroadcast(
    /* [in] */ Boolean broadcast)
{
    return DatagramSocket::SetBroadcast(broadcast);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::GetBroadcast(
    /* [out] */ Boolean* broadcast)
{
    return DatagramSocket::GetBroadcast(broadcast);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::SetTrafficClass(
    /* [in] */ Int32 value)
{
    return DatagramSocket::SetTrafficClass(value);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::GetTrafficClass(
    /* [out] */ Int32* value)
{
    return DatagramSocket::GetTrafficClass(value);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    return DatagramSocket::IsClosed(isClosed);
}

ECode DatagramChannelImpl::DatagramSocketAdapter::GetChannel(
    /* [out] */ IDatagramChannel** channel)
{
    VALIDATE_NOT_NULL(channel)

    assert(0 && "TODO");
    // *channel = mChannelImpl->Probe(EIID_IDatagramChannel);
    REFCOUNT_ADD(*channel)
    return NOERROR;
}

ECode DatagramChannelImpl::DatagramSocketAdapter::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fd)
{
    return DatagramSocket::GetFileDescriptor(fd);
}

Object* DatagramChannelImpl::DatagramSocketAdapter::GetSelfLock()
{
    return &mMlock;
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
        mSocket = (IDatagramSocket*) new DatagramSocketAdapter(IDatagramSocketImpl::Probe(res), this);
    }

    (*socket) = mSocket;
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
    if(mConnected)
    {
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
