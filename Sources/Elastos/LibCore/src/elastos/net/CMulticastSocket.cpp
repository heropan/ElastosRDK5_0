
#include "CMulticastSocket.h"
#include "InetAddress.h"
#include "CInet6Address.h"
#include "CInet4Address.h"
#include "NetworkInterface.h"
#include "CInteger32.h"
#include "CBoolean.h"
#include "CPlainDatagramSocketImpl.h"

using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;

namespace Elastos {
namespace Net {

ECode CMulticastSocket::constructor()
{
    FAIL_RETURN(DatagramSocket::Init());
    return SetReuseAddress(TRUE);
}

ECode CMulticastSocket::constructor(
    /* [in] */ Int32 aPort)
{
    FAIL_RETURN(DatagramSocket::Init(aPort));
    return SetReuseAddress(TRUE);
}

ECode CMulticastSocket::constructor(
    /* [in] */ ISocketAddress* localAddr)
{
    FAIL_RETURN(DatagramSocket::Init(localAddr));
    return SetReuseAddress(TRUE);
}

ECode CMulticastSocket::GetInterface(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    FAIL_RETURN(CheckOpen());
    if (mSetAddress == NULL) {
        AutoPtr<IInetAddress> ipvXaddress;
        mImpl->GetOption(ISocketOptions::IP_MULTICAST_IF, (IInterface**)&ipvXaddress);
        Boolean isAnyLocalAddress;
        if (ipvXaddress->IsAnyLocalAddress(&isAnyLocalAddress), isAnyLocalAddress) {
            // the address was not set at the IPV4 level so check the IPV6
            // level
            AutoPtr<INetworkInterface> theInterface;
            GetNetworkInterface((INetworkInterface**)&theInterface);
            if (theInterface != NULL) {
                AutoPtr<IEnumeration> enumerator;
                theInterface->GetInetAddresses((IEnumeration**)&enumerator);
                if (enumerator != NULL) {
                    Boolean hasNext;
                    while(enumerator->HasMoreElements(&hasNext), hasNext) {
                        AutoPtr<IInterface> nextAddress;
                        enumerator->NextElement((IInterface**)&nextAddress);
                        if (nextAddress != NULL &&
                                IInet6Address::Probe(nextAddress.Get()) != NULL) {
                            *address = nextAddress;
                            REFCOUNT_ADD(*address);
                            return NOERROR;
                        }
                    }
                }
            }
        }
        *address = ipvXaddress;
        REFCOUNT_ADD(*address);
        return NOERROR;
    }
    *address = mSetAddress;
    REFCOUNT_ADD(*address);
    return NOERROR;
}

ECode CMulticastSocket::GetNetworkInterface(
    /* [out] */ INetworkInterface** networkInterface)
{
    VALIDATE_NOT_NULL(networkInterface);
    *networkInterface = NULL;

    FAIL_RETURN(CheckOpen());

    // check if it is set at the IPV6 level. If so then use that. Otherwise
    // do it at the IPV4 level
    // Integer theIndex = Integer.valueOf(0);
    AutoPtr<IInteger32> theIndex;
    // try {
    ECode ec = mImpl->GetOption(ISocketOptions::IP_MULTICAST_IF2, (IInterface**)&theIndex);
    if (FAILED(ec)) {
        theIndex = NULL;
        CInteger32::New(0, (IInteger32**)&theIndex);
    }
    // } catch (SocketException e) {
    //     we may get an exception if IPV6 is not enabled.
    // }

    Int32 value;
    if (theIndex->GetValue(&value), value != 0) {
        return NetworkInterface::GetByIndex(value, networkInterface);
    }

    AutoPtr<INetworkInterface> res = NetworkInterface::ForUnboundMulticastSocket();
    *networkInterface = res;
    REFCOUNT_ADD(*networkInterface)
    return NOERROR;
}

ECode CMulticastSocket::GetTimeToLive(
    /* [out] */ Int32* ttl)
{
    VALIDATE_NOT_NULL(ttl);

    FAIL_RETURN(CheckOpen());
    return mImpl->GetTimeToLive(ttl);
}

ECode CMulticastSocket::GetTTL(
    /* [out] */ Byte* ttl)
{
    VALIDATE_NOT_NULL(ttl);

    FAIL_RETURN(CheckOpen());
    return mImpl->GetTTL(ttl);
}

ECode CMulticastSocket::JoinGroup(
    /* [in] */ IInetAddress* groupAddr)
{
    FAIL_RETURN(CheckJoinOrLeave(groupAddr));
    return mImpl->Join(groupAddr);
}

ECode CMulticastSocket::JoinGroup(
    /* [in] */ ISocketAddress* groupAddress,
    /* [in] */ INetworkInterface* netInterface)
{
    FAIL_RETURN(CheckJoinOrLeave(groupAddress, netInterface));
    return mImpl->JoinGroup(groupAddress, netInterface);
}

ECode CMulticastSocket::LeaveGroup(
    /* [in] */ IInetAddress* groupAddr)
{
    FAIL_RETURN(CheckJoinOrLeave(groupAddr));
    return mImpl->Leave(groupAddr);
}

ECode CMulticastSocket::LeaveGroup(
    /* [in] */ ISocketAddress* groupAddress,
    /* [in] */ INetworkInterface* netInterface)
{
    FAIL_RETURN(CheckJoinOrLeave(groupAddress, netInterface));
    return mImpl->LeaveGroup(groupAddress, netInterface);
}

ECode CMulticastSocket::CheckJoinOrLeave(
    /* [in] */ ISocketAddress* groupAddress,
    /* [in] */ INetworkInterface* netInterface)
{
    FAIL_RETURN(CheckOpen());
    if (groupAddress == NULL) {
        // throw new IllegalArgumentException("groupAddress == null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (netInterface != NULL) {
        AutoPtr<IEnumeration> outenum;
        netInterface->GetInetAddresses((IEnumeration**)&outenum);
        Boolean isflag = FALSE;
        outenum->HasMoreElements(&isflag);
        if (!isflag) {
            // throw new SocketException("No address associated with interface: " + netInterface);
            return E_SOCKET_EXCEPTION;
        }
    }

    if (IInetSocketAddress::Probe(groupAddress) == NULL) {
        // throw new IllegalArgumentException("Group address not an InetSocketAddress: " +
        //        groupAddress.getClass());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IInetAddress> groupAddr;
    IInetSocketAddress::Probe(groupAddress)->GetAddress((IInetAddress**)&groupAddr);
    if (groupAddr == NULL) {
        // throw new SocketException("Group address has no address: " + groupAddress);
        return E_SOCKET_EXCEPTION;
    }

    Boolean isMulticastAddress;
    if (groupAddr->IsMulticastAddress(&isMulticastAddress), !isMulticastAddress) {
        // throw new IOException("Not a multicast group: " + groupAddr);
        return E_IO_EXCEPTION;
    }
    // SecurityManager security = System.getSecurityManager();
    // if (security != NULL) {
    //    security.checkMulticast(groupAddr);
    // }

    return NOERROR;
}

ECode CMulticastSocket::CheckJoinOrLeave(
    /* [in] */ IInetAddress* groupAddr)
{
    FAIL_RETURN(CheckOpen());
    Boolean isMulticastAddress;
    if (groupAddr->IsMulticastAddress(&isMulticastAddress), !isMulticastAddress) {
        // throw new IOException("Not a multicast group: " + groupAddr);
        return E_IO_EXCEPTION;
    }
    // SecurityManager security = System.getSecurityManager();
    // if (security != null) {
    //     security.checkMulticast(groupAddr);
    // }

    return NOERROR;
}

ECode CMulticastSocket::Send(
    /* [in] */ IDatagramPacket* pack,
    /* [in] */ Byte ttl)
{
    FAIL_RETURN(CheckOpen());
    AutoPtr<IInetAddress> packAddr;
    pack->GetAddress((IInetAddress**)&packAddr);
//    SecurityManager security = System.getSecurityManager();
    // if (security != NULL) {
    //     Boolean isMulticastAddress;
    //     groupAddr->IsMulticastAddress(&isMulticastAddress);
    //     if (isMulticastAddress) {
    //         security.checkMulticast(packAddr, ttl);
    //     }
    //     else {
    //         String hostName;
    //         Int32 port;
    //         packAddr->GetHostName(&hostName);
    //         pack->GetPort(&port);
    //         security.checkConnect(hostName, port);
    //     }
    // }
    Int32 currTTL;
    GetTimeToLive(&currTTL);
    Boolean isMulticastAddress;
    packAddr->IsMulticastAddress(&isMulticastAddress);
    if (isMulticastAddress && (Byte)currTTL != ttl) {
        // try {
        SetTimeToLive(ttl & 0xff);
        mImpl->Send(pack);
        // } finally {
        SetTimeToLive(currTTL);
        // }
    }
    else {
        mImpl->Send(pack);
    }

    return NOERROR;
}

ECode CMulticastSocket::SetInterface(
    /* [in] */ IInetAddress* addr)
{
    FAIL_RETURN(CheckOpen());
    if (addr == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    Boolean isAnyLocalAddress;
    addr->IsAnyLocalAddress(&isAnyLocalAddress);
    if (isAnyLocalAddress) {
        mImpl->SetOption(ISocketOptions::IP_MULTICAST_IF, CInet4Address::ANY.Get());
    }
    else if (IInet4Address::Probe(addr) != NULL) {
        mImpl->SetOption(ISocketOptions::IP_MULTICAST_IF, addr);
        // keep the address used to do the set as we must return the same
        // value and for IPv6 we may not be able to get it back uniquely
        mSetAddress = addr;
    }

    /*
     * now we should also make sure this works for IPV6 get the network
     * interface for the address and set the interface using its index
     * however if IPV6 is not enabled then we may get an exception. if IPV6
     * is not enabled
     */
    AutoPtr<INetworkInterface> theInterface;
    FAIL_RETURN(NetworkInterface::GetByInetAddress(addr, (INetworkInterface**)&theInterface));
    Int32 indexvalue = 0;
    if ((theInterface != NULL) && ((((NetworkInterface*)theInterface.Get())->GetIndex(&indexvalue), indexvalue) != 0)) {
//        try {
        AutoPtr<IInteger32> index;
        CInteger32::New(indexvalue, (IInteger32**)&index);
        mImpl->SetOption(ISocketOptions::IP_MULTICAST_IF2, index.Get());
//        } catch (SocketException e) {
            // Ignored
//        }
    }
    else if (isAnyLocalAddress) {
//        try {
        AutoPtr<IInteger32> index;
        CInteger32::New(0, (IInteger32**)&index);
        mImpl->SetOption(ISocketOptions::IP_MULTICAST_IF2, index.Get());
//        } catch (SocketException e) {
            // Ignored
//        }
    }
    else if (IInet6Address::Probe(addr) != NULL) {
//        throw new SocketException("Address not associated with an interface: " + addr);
        return E_SOCKET_EXCEPTION;
    }

    return NOERROR;
}

ECode CMulticastSocket::SetNetworkInterface(
    /* [in] */ INetworkInterface* netInterface)
{
    FAIL_RETURN(CheckOpen());

    if (netInterface == NULL) {
        // throw a socket exception indicating that we do not support this
        // throw new SocketException("netInterface == null");
        return E_SOCKET_EXCEPTION;
    }

    Int32 index = 0;
    netInterface->GetIndex(&index);
    AutoPtr<IInteger32> intindex;
    CInteger32::New(intindex, (IInteger32**)&intindex);
    mImpl->SetOption(ISocketOptions::IP_MULTICAST_IF2, intindex);
    mSetAddress = NULL;
    return NOERROR;
}

ECode CMulticastSocket::SetTimeToLive(
    /* [in] */ Int32 ttl)
{
    FAIL_RETURN(CheckOpen());
    if (ttl < 0 || ttl > 255) {
//        throw new IllegalArgumentException("TimeToLive out of bounds: " + ttl);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return mImpl->SetTimeToLive(ttl);
}

ECode CMulticastSocket::SetTTL(
    /* [in] */ Byte ttl)
{
    FAIL_RETURN(CheckOpen());
    return mImpl->SetTTL(ttl);
}

ECode CMulticastSocket::CreateSocket(
    /* [in] */ Int32 aPort,
    /* [in] */ IInetAddress* addr)
{
    Mutex::Autolock lock(_m_syncLock);

    if (mFactory != NULL) {
        mFactory->CreateDatagramSocketImpl((IDatagramSocketImpl**)&mImpl);
    }
    else {
        FAIL_RETURN(CPlainDatagramSocketImpl::New((IDatagramSocketImpl**)&mImpl));
    }

    mImpl->Create();
    // try {
    AutoPtr<IBoolean> value;
    CBoolean::New(TRUE, (IBoolean**)&value);
    mImpl->SetOption(ISocketOptions::SO_REUSEADDR, value);
    ECode ec = mImpl->Bind(aPort, addr);
    if (FAILED(ec)) {
        Close();
        return ec;
    }

    mIsBound = TRUE;
    return NOERROR;
    // } catch (SocketException e) {
    //    close();
    //    throw e;
    // }
}

ECode CMulticastSocket::GetLoopbackMode(
    /* [out] */ Boolean* isDisabled)
{
    VALIDATE_NOT_NULL(isDisabled);

    FAIL_RETURN(CheckOpen());
    AutoPtr<IBoolean> value;
    mImpl->GetOption(ISocketOptions::IP_MULTICAST_LOOP, (IInterface**)&value);
    Boolean bv;
    value->GetValue(&bv);
    *isDisabled = !bv;

    return NOERROR;
}

ECode CMulticastSocket::SetLoopbackMode(
    /* [in] */ Boolean isDisabled)
{
    FAIL_RETURN(CheckOpen());
    AutoPtr<IBoolean> value;
    CBoolean::New(!isDisabled, (IBoolean**)&value);
    mImpl->SetOption(ISocketOptions::IP_MULTICAST_LOOP, value.Get());

    return NOERROR;
}

ECode CMulticastSocket::Close()
{
    return DatagramSocket::Close();
}

ECode CMulticastSocket::Connect(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 aPort)
{
    return DatagramSocket::Connect(address, aPort);
}

ECode CMulticastSocket::Disconnect()
{
    return DatagramSocket::Disconnect();
}

ECode CMulticastSocket::GetInetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);
    return DatagramSocket::GetInetAddress(address);
}

ECode CMulticastSocket::GetLocalAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);
    return DatagramSocket::GetLocalAddress(address);
}

ECode CMulticastSocket::GetLocalPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    return DatagramSocket::GetLocalPort(port);
}

ECode CMulticastSocket::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    return DatagramSocket::GetPort(port);
}

ECode CMulticastSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return DatagramSocket::GetReceiveBufferSize(size);
}

ECode CMulticastSocket::GetSendBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return DatagramSocket::GetSendBufferSize(size);
}

ECode CMulticastSocket::GetSoTimeout(
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout);
    return DatagramSocket::GetSoTimeout(timeout);
}

ECode CMulticastSocket::Receive(
    /* [in] */ IDatagramPacket* pack)
{
    return DatagramSocket::Receive(pack);
}

ECode CMulticastSocket::Send(
    /* [in] */ IDatagramPacket* pack)
{
    return DatagramSocket::Send(pack);
}

ECode CMulticastSocket::SetSendBufferSize(
    /* [in] */ Int32 size)
{
    return DatagramSocket::SetSendBufferSize(size);
}

ECode CMulticastSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    return DatagramSocket::SetReceiveBufferSize(size);
}

ECode CMulticastSocket::SetSoTimeout(
    /* [in] */ Int32 timeout)
{
    return DatagramSocket::SetSoTimeout(timeout);
}

ECode CMulticastSocket::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
    return DatagramSocket::Bind(localAddr);
}

ECode CMulticastSocket::Connect(
    /* [in] */ ISocketAddress* remoteAddr)
{
    return DatagramSocket::Connect(remoteAddr);
}

ECode CMulticastSocket::IsBound(
    /* [out] */ Boolean* isBound)
{
    VALIDATE_NOT_NULL(isBound);
    return DatagramSocket::IsBound(isBound);
}

ECode CMulticastSocket::IsConnected(
    /* [out] */ Boolean* isConnected)
{
    VALIDATE_NOT_NULL(isConnected);
    return DatagramSocket::IsConnected(isConnected);
}

ECode CMulticastSocket::GetRemoteSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);
    return DatagramSocket::GetRemoteSocketAddress(address);
}

ECode CMulticastSocket::GetLocalSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);
    return DatagramSocket::GetLocalSocketAddress(address);
}

ECode CMulticastSocket::SetReuseAddress(
    /* [in] */ Boolean reuse)
{
    return DatagramSocket::SetReuseAddress(reuse);
}

ECode CMulticastSocket::GetReuseAddress(
    /* [out] */ Boolean* reuse)
{
    VALIDATE_NOT_NULL(reuse);
    return DatagramSocket::GetReuseAddress(reuse);
}

ECode CMulticastSocket::SetBroadcast(
    /* [in] */ Boolean broadcast)
{
    return DatagramSocket::SetBroadcast(broadcast);
}

ECode CMulticastSocket::GetBroadcast(
    /* [out] */ Boolean* broadcast)
{
    VALIDATE_NOT_NULL(broadcast);
    return DatagramSocket::GetBroadcast(broadcast);
}

ECode CMulticastSocket::SetTrafficClass(
    /* [in] */ Int32 value)
{
    return DatagramSocket::SetTrafficClass(value);
}

ECode CMulticastSocket::GetTrafficClass(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return DatagramSocket::GetTrafficClass(value);
}

ECode CMulticastSocket::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    VALIDATE_NOT_NULL(isClosed);
    return DatagramSocket::IsClosed(isClosed);
}

ECode CMulticastSocket::GetChannel(
    /* [out] */ IDatagramChannel** channel)
{
    VALIDATE_NOT_NULL(channel);
    return DatagramSocket::GetChannel(channel);
}

ECode CMulticastSocket::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);
    return DatagramSocket::GetFileDescriptor(fd);
}

Mutex* CMulticastSocket::GetSelfLock()
{
    return &_m_syncLock;
}

} // namespace Net
} // namespace Elastos
