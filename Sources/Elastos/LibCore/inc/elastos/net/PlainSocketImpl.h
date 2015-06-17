#ifndef __ELASTOS_NET_PLAINSOCKETIMPL_H__
#define __ELASTOS_NET_PLAINSOCKETIMPL_H__

#include "SocketImpl.h"

using Elastos::Core::ICloseGuard;

namespace Elastos {
namespace Net {

extern "C" const InterfaceID EIID_PlainSocketImpl;

class PlainSocketImpl
    : public SocketImpl
    , public IPlainSocketImpl
{
    friend class CSocketInputStream;
    friend class CSocketOutputStream;
public:
    CAR_INTERFACE_DECL()

    PlainSocketImpl();

    CARAPI constructor(
        /* [in] */ IFileDescriptor* fd);

    CARAPI constructor(
        /* [in] */ IProxy* proxy);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 localport,
        /* [in] */ IInetAddress* addr,
        /* [in] */ Int32 port);

    CARAPI InitLocalPort(
        /* [in] */ Int32 localPort);

    CARAPI InitRemoteAddressAndPort(
        /* [in] */ IInetAddress* remoteAddress,
        /* [in] */ Int32 remotePort);

    CARAPI GetOption(
        /* [in] */ Int32 option,
        /* [out] */ IInterface** res);

    CARAPI SetOption(
        /* [in] */ Int32 option,
        /* [in] */ IInterface* value);

    CARAPI SocksAccept();

    // CARAPI GetFD(
    //      [out]  IFileDescriptor** ppFd);

    CARAPI Read(
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [out] */ Int32* value);

    CARAPI Write(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* value);

//    void PrintAddr();
protected:
    CARAPI Accept(
        /* [in] */ ISocketImpl* newImpl);

    CARAPI Available(
        /* [out] */ Int32* value);

    CARAPI Bind(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port);

    CARAPI Close();

    CARAPI Connect(
        /* [in] */ const String& aHost,
        /* [in] */ Int32 aPort);

    CARAPI Connect(
        /* [in] */ IInetAddress* anAddr,
        /* [in] */ Int32 aPort);

    CARAPI Create(
        /* [in] */ Boolean streaming);

    CARAPI Finalize();

    CARAPI GetInputStream(
        /* [out] */ IInputStream** in);

    CARAPI GetOutputStream(
        /* [out] */ IOutputStream** out);

    CARAPI Listen(
        /* [in] */ Int32 backlog);

    CARAPI ShutdownInput();

    CARAPI ShutdownOutput();

    CARAPI Connect(
        /* [in] */ ISocketAddress* remoteAddr,
        /* [in] */ Int32 timeout);

    CARAPI SupportsUrgentData(
        /* [out] */ Boolean* value);

    CARAPI SendUrgentData(
        /* [in] */ Int32 value);

//    CARAPI Set(
//        /* [in] */ IFileDescriptor* fd,
//        /* [in] */ Int32 localport,
//        /* [in] */ IInetAddress* addr,
//        /* [in] */ Int32 port)

private:
    Boolean UsingSocks();

//    Field getSocketImplField(final String fieldName);

    CARAPI CheckNotClosed();

    CARAPI Connect(
        /* [in] */ IInetAddress* anAddr,
        /* [in] */ Int32 aPort,
        /* [in] */ Int32 timeout);

    Int32 SocksGetServerPort();

    AutoPtr<IInetAddress> SocksGetServerAddress();

    CARAPI SocksConnect(
        /* [in] */ IInetAddress* applicationServerAddress,
        /* [in] */ Int32 applicationServerPort,
        /* [in] */ Int32 timeout);

    CARAPI SocksRequestConnection(
        /* [in] */ IInetAddress* applicationServerAddress,
        /* [in] */ Int32 applicationServerPort);

    CARAPI SocksBind();

    // static CARAPI IntToBytes(
    //     /* [in] */ Int32 value,
    //     /* [in] */ ArrayOf<Byte>* bytes,
    //     /* [in] */ Int32 start);

    CARAPI SocksSendRequest(
        /* [in] */ Int32 command,
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port);

    CARAPI SocksReadReply(
        /* [out] */ ISocks4Message** ppMessage);

private:
    static AutoPtr<IInetAddress> sLastConnectedAddress;
    static Int32 sLastConnectedPort;
    Boolean mStreaming;
    Boolean mShutdownInput;
    AutoPtr<IProxy> mProxy;
    AutoPtr<ICloseGuard> mGuard;

    //Mutex mLock;

//    Int32 mIfd;
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_PLAINSOCKETIMPL_H__
