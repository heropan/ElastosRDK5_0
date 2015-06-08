#include "PlainSocketImpl.h"
#include "InetAddress.h"
#include "CSocketInputStream.h"
#include "CSocketOutputStream.h"
#include "CFileDescriptor.h"
#include "CInetSocketAddress.h"
#include "CInteger32.h"
#include "CIoBridge.h"
#include "CInetAddressHelper.h"
#include "CSocks4Message.h"
#include "Arrays.h"
#include "CCloseGuard.h"
#include "CLibcore.h"
#include "COsConstants.h"
#include "Memory.h"

using Elastos::IO::CFileDescriptor;
using Elastos::IO::Memory;
using Elastos::IO::EIID_IInputStream;
using Elastos::IO::EIID_IOutputStream;
using Elastos::Net::CInetSocketAddress;
using Elastos::Net::ProxyType;
using Elastos::Net::CInetAddressHelper;
using Elastos::Net::Socks4Message;
using Elastos::Net::CSocks4Message;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::CCloseGuard;
using Elastos::Utility::Arrays;
using Libcore::IO::IIoBridge;
using Libcore::IO::CIoBridge;
using Libcore::IO::CLibcore;
using Libcore::IO::COsConstants;

namespace Elastos {
namespace Net {

// 1d3d148a-53bb-4c22-8c4b-f53cf5305f65
extern "C" const InterfaceID EIID_PlainSocketImpl =
    { 0x1d3d148a, 0x53bb, 0x4c22, { 0x8c, 0x4b, 0xf5, 0x3c, 0xf5, 0x30, 0x5f, 0x65 } };

AutoPtr<IInetAddress> PlainSocketImpl::sLastConnectedAddress = NULL;
Int32 PlainSocketImpl::sLastConnectedPort = 0;

PlainSocketImpl::PlainSocketImpl()
    :SocketImpl()
    , mStreaming(TRUE)
    , mShutdownInput(FALSE)
{
    mGuard = CCloseGuard::Get();
}

ECode PlainSocketImpl::Init(
    /* [in] */ IFileDescriptor* fd)
{
    mFd = fd;
    Boolean bValid;
    fd->Valid(&bValid);
    if (bValid) {
       mGuard->Open(String("close"));
    }
    return NOERROR;
}

ECode PlainSocketImpl::Init(
    /* [in] */ IProxy* proxy)
{
    AutoPtr<IFileDescriptor> fd;
    CFileDescriptor::New((IFileDescriptor**)&fd);
    Init(fd);
    mProxy = proxy;
    return NOERROR;
}

ECode PlainSocketImpl::Init()
{
    AutoPtr<IFileDescriptor> fd;
    CFileDescriptor::New((IFileDescriptor**)&fd);
    Init(fd);
    return NOERROR;
}

ECode PlainSocketImpl::Init(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 localport,
    /* [in] */ IInetAddress* addr,
    /* [in] */ Int32 port)
{
    mFd = fd;
    mLocalport = localport;
    mAddress = addr;
    mPort = port;
    Boolean bValid;
    fd->Valid(&bValid);
    if (bValid) {
       mGuard->Open(String("close"));
    }
    return NOERROR;
}

ECode PlainSocketImpl::Accept(
    /* [in] */ ISocketImpl* newImpl)
{
    if (UsingSocks()) {
        PlainSocketImpl* p = (PlainSocketImpl*) newImpl->Probe(EIID_PlainSocketImpl);
        if (p != NULL) {
            p->SocksBind();
            p->SocksAccept();
            return NOERROR;
        }
    }

    AutoPtr<IInetSocketAddress> peerAddress;
    CInetSocketAddress::New((IInetSocketAddress**)&peerAddress);
    Int32 outfd;
    Int32 fd = 0;
    mFd->GetDescriptor(&fd);
    CLibcore::sOs->Accept(fd, peerAddress, &outfd);
    AutoPtr<IFileDescriptor> clientFd;
    CFileDescriptor::New((IFileDescriptor**)&clientFd);
    clientFd->SetDescriptor(outfd);

    // TODO: we can't just set newImpl.fd to clientFd because a nio SocketChannel may
    // be sharing the FileDescriptor. http://b//4452981.
    Int32 id;
    clientFd->GetDescriptor(&id);
    PlainSocketImpl* p = (PlainSocketImpl*) newImpl->Probe(EIID_PlainSocketImpl);
    if (p == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    p->mFd->SetDescriptor(id);

    peerAddress->GetAddress((IInetAddress**)&(p->mAddress));
    peerAddress->GetPort(&(p->mPort));

    // Reset the client's inherited read timeout to the Java-specified default of 0.
    AutoPtr<IInteger32> val;
    CInteger32::New(0, (IInteger32**)&val);
    newImpl->SetOption(ISocketOptions::SO_TIMEOUT, val);
    p->mFd->GetDescriptor(&fd);
    CIoBridge::_GetSocketLocalPort(fd, &(p->mLocalport));
    return NOERROR;
}

Boolean PlainSocketImpl::UsingSocks()
{
    if (mProxy != NULL) {
        ProxyType type;
        mProxy->GetType(&type);
        if (type == ProxyType_SOCKS) {
            return TRUE;
        }
    }
    return FALSE;
}

ECode PlainSocketImpl::InitLocalPort(
    /* [in] */ Int32 localPort)
{
    mLocalport = localPort;
    return NOERROR;
}

ECode PlainSocketImpl::InitRemoteAddressAndPort(
    /* [in] */ IInetAddress* remoteAddress,
    /* [in] */ Int32 remotePort)
{
    mAddress = remoteAddress;
    mPort = remotePort;
    return NOERROR;
}

ECode PlainSocketImpl::CheckNotClosed()
{
    Boolean value  = FALSE;
    mFd->Valid(&value);
    if (!value) {
        return E_SOCKET_CLOSE_EXCEPTION;
    }
    return NOERROR;
}

ECode PlainSocketImpl::Available(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    ECode ec = CheckNotClosed();
    // we need to check if the input has been shutdown. If so
    // we should return that there is no data to be read
    if (mShutdownInput) {
        *value = 0;
        return NOERROR;
    }
    Int32 fd = 0;
    mFd->GetDescriptor(&fd);
    return CIoBridge::_Available(fd, value);
}

ECode PlainSocketImpl::Bind(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port)
{
    Int32 fd = 0;
    mFd->GetDescriptor(&fd);
    FAIL_RETURN(CIoBridge::_Bind(fd, address, port));
    mAddress = address;
    if (port != 0) {
        mLocalport = port;
        return NOERROR;
    }
    else {
       return CIoBridge::_GetSocketLocalPort(fd, &mLocalport);
    }
}

ECode PlainSocketImpl::Close()
{
    mGuard->Close();
    Int32 fd = 0;
    mFd->GetDescriptor(&fd);
    return CIoBridge::_CloseSocket(fd);
}

ECode PlainSocketImpl::Connect(
    /* [in] */ const String& aHost,
    /* [in] */ Int32 aPort)
{
    AutoPtr<IInetAddressHelper> helper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&helper);
    AutoPtr<IInetAddress> addr;
    helper->GetByName(aHost, (IInetAddress**)&addr);
    return Connect(addr, aPort);
}

ECode PlainSocketImpl::Connect(
    /* [in] */ IInetAddress* anAddr,
    /* [in] */ Int32 aPort)
{
    return Connect(anAddr, aPort, 0);
}

ECode PlainSocketImpl::Connect(
    /* [in] */ IInetAddress* anAddr,
    /* [in] */ Int32 aPort,
    /* [in] */ Int32 timeout)
{
    AutoPtr<IInetAddress> normalAddr;
    Boolean isAny;
    anAddr->IsAnyLocalAddress(&isAny);
    if (isAny) {
        AutoPtr<IInetAddressHelper> helper;
        CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&helper);
        helper->GetLocalHost((IInetAddress**)&normalAddr);
    }
    else {
        normalAddr = anAddr;
    }

    ECode ec;
    if (mStreaming && UsingSocks()) {
        ec = SocksConnect(anAddr, aPort, 0);
    }
    else {
        Int32 fd = 0;
        mFd->GetDescriptor(&fd);
        Boolean isflag = FALSE;
        ec = CIoBridge::_Connect(fd, normalAddr, aPort, timeout, &isflag);
    }

    mAddress = normalAddr;
    mPort = aPort;
    return ec;
}

ECode PlainSocketImpl::Create(
    /* [in] */ Boolean streaming)
{
    mStreaming = streaming;
    Int32 outfd = 0;
    FAIL_RETURN(CIoBridge::_Socket(streaming, &outfd));
    mFd->SetDescriptor(outfd);
    return NOERROR;
}

ECode PlainSocketImpl::Finalize()
{
    if (mGuard != NULL) {
        mGuard->WarnIfOpen();
    }
    ECode ec = Close();
    // SocketImpl::Finalize();
    return ec;
}

ECode PlainSocketImpl::GetInputStream(
    /* [out] */ IInputStream** in)
{
    VALIDATE_NOT_NULL(in)

    FAIL_RETURN(CheckNotClosed());
    AutoPtr<CSocketInputStream> inS;
    FAIL_RETURN(CSocketInputStream::NewByFriend((ISocketImpl*)this, (CSocketInputStream**)&inS));
    *in = (IInputStream*) inS->Probe(EIID_IInputStream);
    REFCOUNT_ADD(*in)
    return NOERROR;
}

ECode PlainSocketImpl::GetOption(
    /* [in] */ Int32 option,
    /* [out] */ IInterface** res)
{
    VALIDATE_NOT_NULL(res)

    AutoPtr<IInterface> out;
    Int32 fd = 0;
    mFd->GetDescriptor(&fd);
    CIoBridge::_GetSocketOption(fd, option, (IInterface**)&out);
    *res = out;
    REFCOUNT_ADD(*res)
    return NOERROR;
}

ECode PlainSocketImpl::GetOutputStream(
    /* [out] */ IOutputStream** out)
{
    VALIDATE_NOT_NULL(out)

    FAIL_RETURN(CheckNotClosed());
    AutoPtr<CSocketOutputStream> res;
    CSocketOutputStream::NewByFriend((ISocketImpl*)this, (CSocketOutputStream**)&res);
    *out = (IOutputStream*) res->Probe(EIID_IOutputStream);
    REFCOUNT_ADD(*out)
    return NOERROR;
}

ECode PlainSocketImpl::Listen(
    /* [in] */ Int32 backlog)
{
    if (UsingSocks()) {
        // Do nothing for a SOCKS connection. The listen occurs on the
        // server during the bind.
        return E_SOCKET_EXCEPTION;
    }
    Int32 fd = 0;
    mFd->GetDescriptor(&fd);
    return CLibcore::sOs->Listen(fd, backlog);
}

ECode PlainSocketImpl::SetOption(
    /* [in] */ Int32 option,
    /* [in] */ IInterface* value)
{
    Int32 fd = 0;
    mFd->GetDescriptor(&fd);
    return CIoBridge::_SetSocketOption(fd, option, value);
}

Int32 PlainSocketImpl::SocksGetServerPort()
{
    AutoPtr<IInetSocketAddress> addr;
    mProxy->GetAddress((ISocketAddress**)&addr);
    Int32 port;
    addr->GetPort(&port);
    return port;
}

AutoPtr<IInetAddress> PlainSocketImpl::SocksGetServerAddress()
{
    String proxyName;
    // get socks server address from proxy. It is unnecessary to check
    // "socksProxyHost" property, since all proxy setting should be
    // determined by ProxySelector.
    AutoPtr<IInetSocketAddress> addr;
    mProxy->GetAddress((ISocketAddress**)&addr);
    addr->GetHostName(&proxyName);
    if (proxyName.IsNull()) {
        AutoPtr<IInetAddress> iaddr;
        addr->GetAddress((IInetAddress**)&iaddr);
        iaddr->GetHostAddress(&proxyName);
    }

    AutoPtr<IInetAddressHelper> helper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&helper);
    AutoPtr<IInetAddress> retAddr;
    helper->GetByName(proxyName, (IInetAddress**)&retAddr);
    return retAddr;
}

ECode PlainSocketImpl::SocksConnect(
    /* [in] */ IInetAddress* applicationServerAddress,
    /* [in] */ Int32 applicationServerPort,
    /* [in] */ Int32 timeout)
{
    AutoPtr<IInetAddress> outadd = SocksGetServerAddress();
    Int32 fd = 0;
    mFd->GetDescriptor(&fd);
    CIoBridge::_Connect(fd, outadd, SocksGetServerPort(), timeout);
    SocksRequestConnection(applicationServerAddress, applicationServerPort);
    sLastConnectedAddress = applicationServerAddress;
    sLastConnectedPort = applicationServerPort;
    return NOERROR;
}

ECode PlainSocketImpl::SocksRequestConnection(
    /* [in] */ IInetAddress* applicationServerAddress,
    /* [in] */ Int32 applicationServerPort)
{
    SocksSendRequest(ISocks4Message::COMMAND_CONNECT,
            applicationServerAddress, applicationServerPort);

    AutoPtr<ISocks4Message> reply;
    SocksReadReply((ISocks4Message**)&reply);
    if (reply != NULL) {
        Int32 res;
        reply->GetCommandOrResult(&res);
        if (res != ISocks4Message::RETURN_SUCCESS) {
            return E_SOCKET_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode PlainSocketImpl::SocksAccept()
{
    AutoPtr<ISocks4Message> reply;
    SocksReadReply((ISocks4Message**)&reply);
    if (reply != NULL) {
        Int32 res;
        reply->GetCommandOrResult(&res);
        if (res != ISocks4Message::RETURN_SUCCESS) {
            return E_SOCKET_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode PlainSocketImpl::ShutdownInput()
{
    mShutdownInput = TRUE;
    Int32 fd = 0;
    mFd->GetDescriptor(&fd);
    return CLibcore::sOs->Shutdown(fd, COsConstants::sSHUT_RD);
}

ECode PlainSocketImpl::ShutdownOutput()
{
    Int32 fd = 0;
    mFd->GetDescriptor(&fd);
    return CLibcore::sOs->Shutdown(fd, COsConstants::sSHUT_WR);
}

ECode PlainSocketImpl::SocksBind()
{
    AutoPtr<IInetAddress> addr = SocksGetServerAddress();
    AutoPtr<ArrayOf<Byte> > array;
    addr->GetAddress((ArrayOf<Byte>**)&array);

    Boolean isflag = FALSE;
    Int32 fd = 0;
    mFd->GetDescriptor(&fd);
    FAIL_RETURN(CIoBridge::_Connect(fd, addr, SocksGetServerPort(), &isflag));

    // There must be a connection to an application host for the bind to
    // work.
    if (sLastConnectedAddress == NULL) {
        return E_INVALID_SOCKET_EXCEPTION;
    }

    // Use the last connected address and port in the bind request.
    SocksSendRequest(ISocks4Message::COMMAND_BIND, sLastConnectedAddress,
            sLastConnectedPort);

    AutoPtr<ISocks4Message> reply;
    SocksReadReply((ISocks4Message**)&reply);

    if (reply == NULL) {
        return E_SOCKET_EXCEPTION;
    }
    Int32 res;
    reply->GetCommandOrResult(&res);
    if (res != ISocks4Message::RETURN_SUCCESS) {
        return E_SOCKET_EXCEPTION;
    }

    mAddress = NULL;
    // A peculiarity of socks 4 - if the address returned is 0, use the
    // original socks server address.
    Int32 ip;
    reply->GetIP(&ip);
    if (ip == 0) {
        mAddress = SocksGetServerAddress();
    }
    else {
        // IPv6 support not yet required as
        // currently the Socks4Message.getIP() only returns int,
        // so only works with IPv4 4byte addresses
        AutoPtr<ArrayOf<Byte> > replyBytes = ArrayOf<Byte>::Alloc(4);
        Int32 replyip = 0;
        reply->GetIP(&replyip);
        Memory::PokeInt32(replyBytes, 0, replyip, Elastos::IO::ByteOrder_BIG_ENDIAN);
        AutoPtr<IInetAddressHelper> helper;
        CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&helper);
        helper->GetByAddress(replyBytes, (IInetAddress**)&mAddress);
    }

    reply->GetPort(&mLocalport);
    return NOERROR;
}

ECode PlainSocketImpl::SocksSendRequest(
    /* [in] */ Int32 command,
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port)
{
    Socks4Message request;
    request.SetCommandOrResult(command);
    request.SetPort(port);
    AutoPtr<ArrayOf<Byte> > array;
    mAddress->GetAddress((ArrayOf<Byte>**)&array);
    request.SetIP(*array);
    request.SetUserId(String("default"));

    AutoPtr<IOutputStream> out;
    GetOutputStream((IOutputStream**)&out);
    return out->WriteBytes(*(request.GetBytes()), 0, request.GetLength());;
}

ECode PlainSocketImpl::SocksReadReply(
    /* [out] */ ISocks4Message** ppMessage)
{
    AutoPtr<ISocks4Message> reply;
    CSocks4Message::New((ISocks4Message**)&reply);
    Int32 bytesRead = 0;
    while (bytesRead < ISocks4Message::REPLY_LENGTH) {
        Int32 count;
        AutoPtr<IInputStream> in;
        GetInputStream((IInputStream**)&in);
        AutoPtr<ArrayOf<Byte> > bytes;
        reply->GetBytes((ArrayOf<Byte>**)&bytes);
        in->ReadBytes(bytes, bytesRead, Socks4Message::REPLY_LENGTH - bytesRead, &count);
        if (-1 == count) {
            break;
        }
        bytesRead += count;
    }
    if (ISocks4Message::REPLY_LENGTH != bytesRead) {
        return E_MALFORMED_REPLY_EXCEPTION;
    }

    *ppMessage = reply;
    REFCOUNT_ADD(*ppMessage)
    return NOERROR;
}

ECode PlainSocketImpl::Connect(
    /* [in] */ ISocketAddress* remoteAddr,
    /* [in] */ Int32 timeout)
{
    AutoPtr<IInetSocketAddress> inetAddr = IInetSocketAddress::Probe(remoteAddr);
    AutoPtr<IInetAddress> addr;
    inetAddr->GetAddress((IInetAddress**)&addr);
    Int32 port;
    inetAddr->GetPort(&port);
    return Connect(addr, port, timeout);
}

ECode PlainSocketImpl::SupportsUrgentData(
    /* [out] */ Boolean* value)
{
    *value = TRUE;
    return NOERROR;
}

ECode PlainSocketImpl::SendUrgentData(
    /* [in] */ Int32 value)
{
    AutoPtr< ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(4);
    memcpy(buffer->GetPayload(), &value, 4);
    Int32 outvalue = 0;
    Int32 fd = 0;
    mFd->GetDescriptor(&fd);
    return CLibcore::sOs->Sendto(fd, *buffer, 0, 1, COsConstants::sMSG_OOB, NULL, 0, &outvalue);
}

ECode PlainSocketImpl::Read(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* value)
{
    if (byteCount == 0) {
        *value = 0;
        return NOERROR;
    }
    Arrays::CheckOffsetAndCount(buffer->GetLength(), offset, byteCount);
    if (mShutdownInput) {
        *value = -1;
        return NOERROR;
    }

    Int32 readCount;
    Int32 fd = 0;
    mFd->GetDescriptor(&fd);
    CIoBridge::_Recvfrom(TRUE, fd, buffer, offset, byteCount, 0, NULL, FALSE, &readCount);
    // Return of zero bytes for a blocking socket means a timeout occurred
    if (readCount == 0) {
        // throw new SocketTimeoutException();
        return E_SOCKET_TIMEOUT_EXCEPTION;
    }
    // Return of -1 indicates the peer was closed
    if (readCount == -1) {
        mShutdownInput = TRUE;
    }
    *value = readCount;
    return NOERROR;
}

ECode PlainSocketImpl::Write(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* value)
{
    Arrays::CheckOffsetAndCount(buffer.GetLength(), offset, byteCount);
    Int32 fd = 0;
    mFd->GetDescriptor(&fd);
    if (mStreaming) {
        while (byteCount > 0) {
            Int32 bytesWritten;
            FAIL_RETURN(CIoBridge::_Sendto(fd, buffer, offset, byteCount, 0, NULL, 0, &bytesWritten));
            byteCount -= bytesWritten;
            offset += bytesWritten;
        }
    }
    else {
        // Unlike writes to a streaming socket, writes to a datagram
        // socket are all-or-nothing, so we don't need a loop here.
        // http://code.google.com/p/android/issues/detail?id=15304
        FAIL_RETURN(CIoBridge::_Sendto(fd, buffer, offset, byteCount, 0, mAddress, mPort, value));
    }
    *value = byteCount;
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
