
#ifndef __ELASTOS_NET_DATAGRAMSOCKETIMPL_H__
#define __ELASTOS_NET_DATAGRAMSOCKETIMPL_H__

#include "Object.h"

using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Net {

class DatagramSocketImpl
    : public Object
    , public IDatagramSocketImpl
    , public ISocketOptions
{
public:
    CAR_INTERFACE_DECL()

    DatagramSocketImpl();

    virtual CARAPI Bind(
        /* [in] */ Int32 port,
        /* [in] */ IInetAddress* addr) = 0;

    virtual CARAPI Close() = 0;

    virtual CARAPI Create() = 0;

    virtual CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor** fileDescriptor);

    virtual CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** address);

    virtual CARAPI GetLocalPort(
        /* [out] */ Int32* port);

    virtual CARAPI GetTTL(
        /* [out] */ Byte* ttl) = 0;

    virtual CARAPI GetTimeToLive(
        /* [out] */ Int32* option) = 0;

    virtual CARAPI Join(
        /* [in] */ IInetAddress* addr) = 0;

    virtual CARAPI JoinGroup(
        /* [in] */ ISocketAddress* addr,
        /* [in] */ INetworkInterface* netInterface) = 0;

    virtual CARAPI Leave(
        /* [in] */ IInetAddress* addr) = 0;

    virtual CARAPI LeaveGroup(
        /* [in] */ ISocketAddress* addr,
        /* [in] */ INetworkInterface* netInterface) = 0;

    virtual CARAPI Peek(
        /* [in] */ IInetAddress* sender,
        /* [out] */ Int32* addr) = 0;

    virtual CARAPI Receive(
        /* [in] */ IDatagramPacket* pack) = 0;

    virtual CARAPI Send(
        /* [in] */ IDatagramPacket* pack) = 0;

    virtual CARAPI SetTimeToLive(
        /* [in] */ Int32 ttl) = 0;

    virtual CARAPI SetTTL(
        /* [in] */ Byte ttl) = 0;

    virtual CARAPI Connect(
        /* [in] */ IInetAddress* inetAddr,
        /* [in] */ Int32 port);

    virtual CARAPI Disconnect();

    virtual CARAPI PeekData(
        /* [in] */ IDatagramPacket* pack,
        /* [out] */ Int32* port) = 0;

protected:
    AutoPtr<IFileDescriptor> mFd;

    Int32 mLocalPort;
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_DATAGRAMSOCKETIMPL_H__
