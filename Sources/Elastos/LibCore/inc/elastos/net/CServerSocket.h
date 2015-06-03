
#ifndef __CSERVERSOCKET_H__
#define __CSERVERSOCKET_H__

#include "_CServerSocket.h"
#include "ServerSocket.h"

namespace Elastos {
namespace Net {

CarClass(CServerSocket) , public ServerSocket
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 aPort);

    CARAPI constructor(
        /* [in] */ Int32 aport,
        /* [in] */ Int32 backlog);

    CARAPI constructor(
        /* [in] */ Int32 aport,
        /* [in] */ Int32 backlog,
        /* [in] */ IInetAddress* localAddr);

    CARAPI GetImpl(
        /* [out] */ ISocketImpl** impl);

    CARAPI Accept(
        /* [out] */ ISocket** socket);

    CARAPI Close();

    CARAPI GetInetAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetLocalPort(
        /* [out] */ Int32* port);

    CARAPI GetSoTimeout(
        /* [out] */ Int32* timeout);

    CARAPI SetSoTimeout(
        /* [in] */ Int32 timeout);

    CARAPI Bind(
        /* [in] */ ISocketAddress* localAddr);

    CARAPI Bind(
        /* [in] */ ISocketAddress* localAddr,
        /* [in] */ Int32 backlog);

    CARAPI GetLocalSocketAddress(
        /* [out] */ ISocketAddress** address);

    CARAPI IsBound(
        /* [out] */ Boolean* isBound);

    CARAPI IsClosed(
        /* [out] */ Boolean* isClosed);

    CARAPI SetReuseAddress(
        /* [in] */ Boolean reuse);

    CARAPI GetReuseAddress(
        /* [out] */ Boolean* reuse);

    CARAPI SetReceiveBufferSize(
        /* [in] */ Int32 size);

    CARAPI GetReceiveBufferSize(
        /* [out] */ Int32* size);

    CARAPI GetChannel(
        /* [out] */ IServerSocketChannel** channel);

    CARAPI SetPerformancePreferences(
        /* [in] */ Int32 connectionTime,
        /* [in] */ Int32 latency,
        /* [in] */ Int32 bandwidth);
};

} // namespace Net
} // namespace Elastos

#endif //__CSERVERSOCKET_H__
