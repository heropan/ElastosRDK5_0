
#ifndef __CPLAINSERVERSOCKETIMPL_H__
#define __CPLAINSERVERSOCKETIMPL_H__

#include "_CPlainServerSocketImpl.h"
#include "PlainServerSocketImpl.h"

namespace Elastos {
namespace Net {

CarClass(CPlainServerSocketImpl) , public PlainServerSocketImpl
{
public:
    CARAPI InitLocalPort(
        /* [in] */ Int32 localPort);

    CARAPI InitRemoteAddressAndPort(
        /* [in] */ IInetAddress* remoteAddress,
        /* [in] */ Int32 remotePort);

    CARAPI SocksAccept();

    CARAPI GetOption(
        /* [in] */ Int32 optID,
        /* [out] */ IInterface ** ppOptVal);

    CARAPI SetOption(
        /* [in] */ Int32 optID,
        /* [in] */ IInterface * pOptVal);

    CARAPI Accept(
        /* [in] */ ISocketImpl * pNewSocket);

    CARAPI Available(
        /* [out] */ Int32 * pNum);

    CARAPI Bind(
        /* [in] */ IInetAddress * pAddress,
        /* [in] */ Int32 port);

    CARAPI Close();

    CARAPI Connect(
        /* [in] */ const String& host,
        /* [in] */ Int32 port);

    CARAPI ConnectEx(
        /* [in] */ IInetAddress * pAddress,
        /* [in] */ Int32 port);

    CARAPI Create(
        /* [in] */ Boolean isStreaming);

    CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor ** ppFileDescriptor);

    CARAPI GetInetAddress(
        /* [out] */ IInetAddress ** ppNetAddress);

    CARAPI GetInputStream(
        /* [out] */ IInputStream ** ppInputStream);

    CARAPI GetLocalPort(
        /* [out] */ Int32 * pNum);

    CARAPI GetOutputStream(
        /* [out] */ IOutputStream ** ppOutputStream);

    CARAPI GetPort(
        /* [out] */ Int32 * pPort);

    CARAPI Listen(
        /* [in] */ Int32 backlog);

    CARAPI ShutdownInput();

    CARAPI ShutdownOutput();

    CARAPI ConnectEx2(
        /* [in] */ ISocketAddress * pRemoteAddr,
        /* [in] */ Int32 timeout);

    CARAPI SupportsUrgentData(
        /* [out] */ Boolean * pIsSupports);

    CARAPI SendUrgentData(
        /* [in] */ Int32 value);

    CARAPI SetPerformancePreferences(
        /* [in] */ Int32 connectionTime,
        /* [in] */ Int32 latency,
        /* [in] */ Int32 bandwidth);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IFileDescriptor * pFd);
};

} // namespace Net
} // namespace Elastos

#endif // __CPLAINSERVERSOCKETIMPL_H__
