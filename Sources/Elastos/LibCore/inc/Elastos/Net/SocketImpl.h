
#ifndef __SOCKETIMPL_H__
#define __SOCKETIMPL_H__

#include "Elastos.Core_server.h"

using Elastos::IO::IFileDescriptor;
using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;

namespace Elastos {
namespace Net {

extern "C" const InterfaceID EIID_SocketImpl;

class SocketImpl
{
public:
    SocketImpl();

    virtual CARAPI GetFD(
        /* [out] */ IFileDescriptor** fileDescriptor);

    virtual ToString(
        /* [out] */ String* str);

protected:
    virtual CARAPI Accept(
        /* [in] */ ISocketImpl* newSocket) = 0;

    virtual CARAPI Available(
        /* [out] */ Int32* num) = 0;

    virtual CARAPI Bind(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port) = 0;

    virtual CARAPI Close() = 0;

    virtual CARAPI Connect(
        /* [in] */ const String& host,
        /* [in] */ Int32 port) = 0;

    virtual CARAPI ConnectEx(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port) = 0;

    virtual CARAPI Create(
        /* [in] */ Boolean isStreaming) = 0;

    virtual CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor** fileDescriptor);

    virtual CARAPI GetInetAddress(
        /* [out] */ IInetAddress** netAddress);

    virtual CARAPI GetInputStream(
        /* [out] */ IInputStream** inputStream) = 0;

    virtual CARAPI GetLocalPort(
        /* [out] */ Int32* num);

    virtual CARAPI GetOutputStream(
        /* [out] */ IOutputStream** outputStream) = 0;

    virtual CARAPI GetPort(
        /* [out] */ Int32* port);

    virtual CARAPI Listen(
        /* [in] */ Int32 backlog) = 0;

    virtual CARAPI ShutdownInput();

    virtual CARAPI ShutdownOutput();

    virtual CARAPI ConnectEx2(
        /* [in] */ ISocketAddress* remoteAddr,
        /* [in] */ Int32 timeout) = 0;

    virtual CARAPI SupportsUrgentData(
        /* [out] */ Boolean* isSupports);

    virtual CARAPI SendUrgentData(
        /* [in] */ Int32 value) = 0;

    virtual CARAPI SetPerformancePreferences(
        /* [in] */ Int32 connectionTime,
        /* [in] */ Int32 latency,
        /* [in] */ Int32 bandwidth);

public:
    Boolean mStreaming;

protected:
    AutoPtr<IInetAddress> mAddress;

    Int32 mPort;

    AutoPtr<IFileDescriptor> mFd;

    Int32 mLocalport;
};

} // namespace Net
} // namespace Elastos

#endif //__SOCKETIMPL_H__
