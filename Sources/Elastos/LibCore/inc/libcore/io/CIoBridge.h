
#ifndef __CIOBRIDGE_H__
#define __CIOBRIDGE_H__

#include "_CIoBridge.h"

using Elastos::IO::IByteBuffer;
using Elastos::Net::IInetAddress;
using Elastos::Net::IDatagramPacket;
using Elastos::Net::IInetSocketAddress;

namespace Libcore {
namespace IO {

CarClass(CIoBridge)
{
public:
    CARAPI Available(
        /* [in] */ Int32 fd,
        /* [out] */ Int32* avail);

    CARAPI Bind(
        /* [in] */ Int32 fd,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port);

    CARAPI Connect(
        /* [in] */ Int32 fd,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [out] */ Boolean* result);

    CARAPI ConnectEx(
        /* [in] */ Int32 fd,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [in] */ Int32 timeoutMs,
        /* [out] */ Boolean* result);

    CARAPI CloseSocket(
        /* [in] */ Int32 fd);

    CARAPI IsConnected(
        /* [in] */ Int32 fd,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [in] */ Int32 timeoutMs,
        /* [in] */ Int32 remainingTimeoutMs,
        /* [out] */ Boolean* isConnected);

    CARAPI GetSocketOption(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 option,
        /* [out] */ IInterface** value);

    CARAPI SetSocketOption(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 option,
        /* [in] */ IInterface* value);

    CARAPI Open(
        /* [in] */ const String& path,
        /* [in] */ Int32 flags,
        /* [out] */ Int32* fd);

    CARAPI Read(
        /* [in] */ Int32 fd,
        /* [out] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* count);

    CARAPI Write(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount);

    CARAPI Sendto(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int32 flags,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [out] */ Int32* result);

    CARAPI SendtoEx(
        /* [in] */ Int32 fd,
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int32 flags,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [out] */ Int32* result);

    CARAPI Recvfrom(
        /* [in] */ Boolean isRead,
        /* [in] */ Int32 fd,
        /* [out] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int32 flags,
        /* [in] */ IDatagramPacket* packet,
        /* [in] */ Boolean isConnected,
        /* [out] */ Int32* result);

    CARAPI RecvfromEx(
        /* [in] */ Boolean isRead,
        /* [in] */ Int32 fd,
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int32 flags,
        /* [in] */ IDatagramPacket* packet,
        /* [in] */ Boolean isConnected,
        /* [out] */ Int32* result);

    CARAPI Socket(
        /* [in] */ Boolean stream,
        /* [out] */ Int32* fd);

    CARAPI GetSocketLocalAddress(
        /* [in] */ Int32 fd,
        /* [out] */ IInetAddress** address);

    CARAPI GetSocketLocalPort(
        /* [in] */ Int32 fd,
        /* [out] */ Int32* port);

public:
    static CARAPI _Available(
        /* [in] */ Int32 fd,
        /* [out] */ Int32* avail);

    static CARAPI _Bind(
        /* [in] */ Int32 fd,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port);

    static CARAPI _Connect(
        /* [in] */ Int32 fd,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [out] */ Boolean* result);

    static CARAPI _Connect(
        /* [in] */ Int32 fd,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [in] */ Int32 timeoutMs,
        /* [out] */ Boolean* result);

    static CARAPI _CloseSocket(
        /* [in] */ Int32 fd);

    static CARAPI _IsConnected(
        /* [in] */ Int32 fd,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [in] */ Int32 timeoutMs,
        /* [in] */ Int32 remainingTimeoutMs,
        /* [out] */ Boolean* isConnected);

    static CARAPI _GetSocketOption(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 option,
        /* [out] */ IInterface** value);

    static CARAPI _SetSocketOption(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 option,
        /* [in] */ IInterface* value);

    static CARAPI _Open(
        /* [in] */ const String& path,
        /* [in] */ Int32 flags,
        /* [out] */ Int32* fd);

    static CARAPI _Read(
        /* [in] */ Int32 fd,
        /* [out] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* count);

    static CARAPI _Write(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount);

    static CARAPI _Sendto(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int32 flags,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [out] */ Int32* result);

    static CARAPI _Sendto(
        /* [in] */ Int32 fd,
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int32 flags,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [out] */ Int32* result);

    static CARAPI _Recvfrom(
        /* [in] */ Boolean isRead,
        /* [in] */ Int32 fd,
        /* [out] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int32 flags,
        /* [in] */ IDatagramPacket* packet,
        /* [in] */ Boolean isConnected,
        /* [out] */ Int32* result);

    static CARAPI _Recvfrom(
        /* [in] */ Boolean isRead,
        /* [in] */ Int32 fd,
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int32 flags,
        /* [in] */ IDatagramPacket* packet,
        /* [in] */ Boolean isConnected,
        /* [out] */ Int32* result);

    static CARAPI _Socket(
        /* [in] */ Boolean stream,
        /* [out] */ Int32* fd);

    static CARAPI _GetSocketLocalAddress(
        /* [in] */ Int32 fd,
        /* [out] */ IInetAddress** address);

    static CARAPI _GetSocketLocalPort(
        /* [in] */ Int32 fd,
        /* [out] */ Int32* port);

private:
    static ConnectErrno(
        /* [in] */ Int32 fd,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [in] */ Int32 timeoutMs,
        /* [out] */ Boolean* result);

    static GetSocketOptionErrno(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 option,
        /* [out] */ IInterface** value);

    static CARAPI_(Boolean) BooleanFromInt(
        /* [in] */ Int32 i);

    static CARAPI_(Int32) BooleanToInt(
        /* [in] */ Boolean b);

    static CARAPI SetSocketOptionErrno(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 option,
        /* [in] */ IInterface* value);

    static CARAPI_(Int32) PostRecvfrom(
        /* [in] */ Boolean isRead,
        /* [in] */ IDatagramPacket* packet,
        /* [in] */ Boolean isConnected,
        /* [in] */ IInetSocketAddress* srcAddress,
        /* [in] */ Int32 byteCount);

public:
    // Socket options used by java.net but not exposed in SocketOptions.
    static const Int32 MCAST_JOIN_GROUP = 19;
    static const Int32 MCAST_LEAVE_GROUP = 20;
    static const Int32 IP_MULTICAST_TTL = 17;
};

} // namespace IO
} // namespace Libcore

#endif // __CIOBRIDGE_H__
