#ifndef __BLOCKGUARDOS_H__
#define __BLOCKGUARDOS_H__

#include "ForwardingOs.h"

using Libcore::IO::IOs;
using Elastos::IO::IFileDescriptor;
using Elastos::Net::IInetSocketAddress;
using Elastos::Net::IInetAddress;

namespace Libcore {
namespace IO {

class BlockGuardOs : public ForwardingOs
{
public:
    BlockGuardOs(
        /* [in] */ IOs* os);

    CARAPI Accept(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IInetSocketAddress* peerAddress,
        /* [out] */ Int32* retFd);

    CARAPI Close(
        /* [in] */ Int32 fd);

    CARAPI Connect(
        /* [in] */ Int32 fd,
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port);

    // TODO: Untag newFd when needed for dup2(FileDescriptor oldFd, int newFd)

    CARAPI Fdatasync(
        /* [in] */ Int32 fd);

    CARAPI Fsync(
        /* [in] */ Int32 fd);

    CARAPI Ftruncate(
        /* [in] */ Int32 fd,
        /* [in] */ Int64 length);

    CARAPI Open(
        /* [in] */ const String& path,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 mode,
        /* [out] */ Int32* fd);

    CARAPI Poll(
        /* [in] */ const ArrayOf<IStructPollfd*>& fdStructs,
        /* [in] */ Int32 timeoutMs,
        /* [out] */ Int32* result);

    //CARAPI int pread(FileDescriptor fd, ByteBuffer buffer, long offset);

    CARAPI Pread(
        /* [in] */ Int32 fd,
        /* [out] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int64 offset,
        /* [out] */ Int32* num);

    //CARAPI int pwrite(FileDescriptor fd, ByteBuffer buffer, long offset);

    CARAPI Pwrite(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int64 offset,
        /* [out] */ Int32* num);

    //CARAPI int read(FileDescriptor fd, ByteBuffer buffer);

    CARAPI Read(
        /* [in] */ Int32 fd,
        /* [out] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* num);

    //CARAPI int readv(FileDescriptor fd, Object[] buffers, int[] offsets, int[] byteCounts);

    //CARAPI int recvfrom(FileDescriptor fd, ByteBuffer buffer, int flags, InetSocketAddress srcAddress);

    CARAPI Recvfrom(
        /* [in] */ Int32 fd,
        /* [out] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int32 flags,
        /* [in] */ IInetSocketAddress* srcAddress,
        /* [out] */ Int32* num);

    //CARAPI int sendto(FileDescriptor fd, ByteBuffer buffer, int flags, InetAddress inetAddress, int port);

    CARAPI Sendto(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int32 flags,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [out] */ Int32* result);

    CARAPI Socket(
        /* [in] */ Int32 socketDomain,
        /* [in] */ Int32 type,
        /* [in] */ Int32 protocol,
        /* [out] */ Int32* fd);

    CARAPI Socketpair(
        /* [in] */ Int32 socketDomain,
        /* [in] */ Int32 type,
        /* [in] */ Int32 protocol,
        /* [out] */ Int32* fd1,
        /* [out] */ Int32* fd2);

    //CARAPI int write(FileDescriptor fd, ByteBuffer buffer);

    CARAPI Write(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* num);

    //CARAPI int writev(FileDescriptor fd, Object[] buffers, int[] offsets, int[] byteCounts);

private:
    CARAPI TagSocket(
        /* [in] */ Int32 fd,
        /* [out] */ Int32* retFd);

    CARAPI UntagSocket(
        /* [in] */ Int32 fd);

    static CARAPI IsLingerSocket(
        /* [in] */ Int32 fd,
        /* [out] */ Boolean* result);
};

} // namespace IO
} // namespace Libcore

#endif // __BLOCKGUARDOS_H__