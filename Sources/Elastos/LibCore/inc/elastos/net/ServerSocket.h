
#ifndef __ELASTOS_NET_SERVERSOCKET_H__
#define __ELASTOS_NET_SERVERSOCKET_H__

#include "CSocket.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;
using Elastos::IO::ICloseable;
using Elastos::IO::Channels::IServerSocketChannel;

namespace Elastos {
namespace Net {

class ServerSocket
    : public Object
    , public IServerSocket
    , public ICloseable
{
public:
    CAR_INTERFACE_DECL()

    CARAPI GetImpl(
        /* [out] */ ISocketImpl** impl);

    virtual CARAPI Accept(
        /* [out] */ ISocket** socket);

    CARAPI CheckListen(
        /* [in] */ Int32 aPort);

    virtual CARAPI Close();

    static CARAPI_(Int32) DefaultBacklog();

    virtual CARAPI GetInetAddress(
        /* [out] */ IInetAddress** address);

    virtual CARAPI GetLocalPort(
        /* [out] */ Int32* port);

    virtual CARAPI GetSoTimeout(
        /* [out] */ Int32* timeout);

    static CARAPI SetSocketFactory(
        /* [in] */ ISocketImplFactory* aFactory);

    virtual CARAPI SetSoTimeout(
        /* [in] */ Int32 timeout);

    virtual CARAPI Bind(
        /* [in] */ ISocketAddress* localAddr);

    virtual CARAPI Bind(
        /* [in] */ ISocketAddress* localAddr,
        /* [in] */ Int32 backlog);

    virtual CARAPI GetLocalSocketAddress(
        /* [out] */ ISocketAddress** address);

    virtual CARAPI IsBound(
        /* [out] */ Boolean* isBound);

    virtual CARAPI IsClosed(
        /* [out] */ Boolean* isClosed);

    virtual CARAPI SetReuseAddress(
        /* [in] */ Boolean reuse);

    virtual CARAPI GetReuseAddress(
        /* [out] */ Boolean* reuse);

    virtual CARAPI SetReceiveBufferSize(
        /* [in] */ Int32 size);

    virtual CARAPI GetReceiveBufferSize(
        /* [out] */ Int32* size);

    virtual CARAPI GetChannel(
        /* [out] */ IServerSocketChannel** channel);

    virtual CARAPI SetPerformancePreferences(
        /* [in] */ Int32 connectionTime,
        /* [in] */ Int32 latency,
        /* [in] */ Int32 bandwidth);

protected:
    ServerSocket();

    CARAPI constructor(
        /* [in] */ ISocketImpl* impl);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 aPort);

    CARAPI constructor(
        /* [in] */ Int32 aPort,
        /* [in] */ Int32 backlog);

    CARAPI constructor(
        /* [in] */ Int32 aPort,
        /* [in] */ Int32 backlog,
        /* [in] */ IInetAddress* localAddr);

    CARAPI ImplAccept(
        /* [in] */ ISocket* aSocket);

private:
    CARAPI CheckClosedAndCreate(
        /* [in] */ Boolean create);

public:
    AutoPtr<ISocketImpl> mImpl;

    static AutoPtr<ISocketImplFactory> sFactory;

private:
    Boolean mIsCreated;

    Boolean mIsBound;

    Boolean mIsClosed;

    //Mutex mLock;
    //static Mutex sLock;
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_SERVERSOCKET_H__
