
#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "Elastos.CoreLibrary_server.h"
#include <elastos/Mutex.h>

using Elastos::Core::Threading::Mutex;
using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;
using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Net {

class Socket
{
public:
    CARAPI Close();

    CARAPI GetInetAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetInputStream(
        /* [out] */ IInputStream** is);

    CARAPI GetKeepAlive(
        /* [out] */ Boolean* keepAlive);

    CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetLocalPort(
        /* [out] */ Int32* localPort);

    CARAPI GetOutputStream(
        /* [out] */ IOutputStream** os);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    CARAPI GetSoLinger(
        /* [out] */ Int32* value);

    CARAPI GetReceiveBufferSize(
        /* [out] */ Int32* size);

    CARAPI GetSendBufferSize(
        /* [out] */ Int32* size);

    CARAPI GetSoTimeout(
        /* [out] */ Int32* timeout);

    CARAPI GetTcpNoDelay(
        /* [out] */ Boolean* noDelay);

    CARAPI SetKeepAlive(
        /* [in] */ Boolean keepAlive);

    static CARAPI SetSocketImplFactory(
        /* [in] */ ISocketImplFactory* fac);

    CARAPI SetSendBufferSize(
        /* [in] */ Int32 size);

    CARAPI SetReceiveBufferSize(
        /* [in] */ Int32 size);

    CARAPI SetSoLinger(
        /* [in] */ Boolean on,
        /* [in] */ Int32 timeout);

    CARAPI SetSoTimeout(
        /* [in] */ Int32 timeout);

    CARAPI SetTcpNoDelay(
        /* [in] */ Boolean on);

    CARAPI ShutdownInput();

    CARAPI ShutdownOutput();

    CARAPI GetLocalSocketAddress(
        /* [out] */ ISocketAddress** address);

    CARAPI GetRemoteSocketAddress(
        /* [out] */ ISocketAddress** address);

    CARAPI IsBound(
        /* [out] */ Boolean* isBound);

    CARAPI IsConnected(
        /* [out] */ Boolean* isConnected);

    CARAPI IsClosed(
        /* [out] */ Boolean* isClosed);

    CARAPI Bind(
        /* [in] */ ISocketAddress* localAddr);

    CARAPI Connect(
        /* [in] */ ISocketAddress* remoteAddr);

    CARAPI ConnectEx(
        /* [in] */ ISocketAddress* remoteAddr,
        /* [in] */ Int32 timeout);

    CARAPI IsInputShutdown(
        /* [out] */ Boolean* isInputShutdown);

    CARAPI IsOutputShutdown(
        /* [out] */ Boolean* isOutputShutdown);

    CARAPI SetReuseAddress(
        /* [in] */ Boolean reuse);

    CARAPI GetReuseAddress(
        /* [out] */ Boolean* reuse);

    CARAPI SetOOBInline(
        /* [in] */ Boolean oobinline);

    CARAPI GetOOBInline(
        /* [out] */ Boolean* oobinline);

    CARAPI SetTrafficClass(
        /* [in] */ Int32 value);

    CARAPI GetTrafficClass(
        /* [out] */ Int32* value);

    CARAPI SendUrgentData(
        /* [in] */ Int32 value);

    CARAPI_(void) Accepted();

    CARAPI GetChannel(
        /* [out] */ ISocketChannel** channel);

    CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor** fd);

    CARAPI SetPerformancePreferences(
        /* [in] */ Int32 connectionTime,
        /* [in] */ Int32 latency,
        /* [in] */ Int32 bandwidth);

protected:
    Socket();

    CARAPI Init();

    CARAPI Init(
        /* [in] */ IProxy* proxy);

    CARAPI Init(
        /* [in] */ const String& dstName,
        /* [in] */ Int32 dstPort);

    CARAPI Init(
        /* [in] */ const String& dstName,
        /* [in] */ Int32 dstPort,
        /* [in] */ IInetAddress* localAddress,
        /* [in] */ Int32 localPort);

    CARAPI Init(
        /* [in] */ const String& hostName,
        /* [in] */ Int32 port,
        /* [in] */ Boolean streaming);

    CARAPI Init(
        /* [in] */ IInetAddress* dstAddress,
        /* [in] */ Int32 dstPort);

    CARAPI Init(
        /* [in] */ IInetAddress* dstAddress,
        /* [in] */ Int32 dstPort,
        /* [in] */ IInetAddress* localAddress,
        /* [in] */ Int32 localPort);

    CARAPI Init(
        /* [in] */ IInetAddress* addr,
        /* [in] */ Int32 port,
        /* [in] */ Boolean streaming);

    virtual CARAPI_(Mutex*) GetSelfLock() = 0;

private:
    CARAPI TryAllAddresses(
        /* [in] */ const String& dstName,
        /* [in] */ Int32 dstPort,
        /* [in] */ IInetAddress* localAddress,
        /* [in] */ Int32 localPort,
        /* [in] */ Boolean streaming);

    CARAPI CheckDestination(
        /* [in] */ IInetAddress* destAddr,
        /* [in] */ Int32 dstPort);

    CARAPI CheckConnectPermission(
        /* [in] */ const String& hostname,
        /* [in] */ Int32 dstPort);

    CARAPI StartupSocket(
        /* [in] */ IInetAddress* dstAddress,
        /* [in] */ Int32 dstPort,
        /* [in] */ IInetAddress* localAddress,
        /* [in] */ Int32 localPort,
        /* [in] */ Boolean streaming);

    CARAPI_(Boolean) UsingSocks();

    CARAPI CheckOpenAndCreate(
        /* [in] */ Boolean create);

    CARAPI_(void) CacheLocalAddress();

public:
    AutoPtr<ISocketImpl> mImpl;

private:
    static AutoPtr<ISocketImplFactory> sFactory;
    static Mutex sLock;

    AutoPtr<IProxy> mProxy;

    Boolean mIsCreated;
    Boolean mIsBound;
    Boolean mIsConnected;
    Boolean mIsClosed;
    Boolean mIsInputShutdown;
    Boolean mIsOutputShutdown;

    AutoPtr<IInetAddress> mLocalAddress;
    Mutex mConnectLock;
};

} // namespace Net
} // namespace Elastos

#endif //__SOCKET_H__
