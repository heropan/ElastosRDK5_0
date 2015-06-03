
#include "BlockGuardOs.h"
#include "CLibcore.h"
#include "BlockGuard.h"
#include "CStructStat.h"
#include "CStructLinger.h"
#include "COsConstants.h"
#include "SocketTagger.h"
#include <fcntl.h>

using Elastos::Core::BlockGuard;
using Elastos::Core::IBlockGuardPolicy;
using Libcore::IO::IStructStat;
using Libcore::IO::CStructStat;
using Libcore::IO::IStructLinger;
using Libcore::IO::CStructLinger;
using Libcore::IO::ISocketTagger;

namespace Libcore {
namespace IO {

BlockGuardOs::BlockGuardOs(
    /* [in] */ IOs* os)
    : ForwardingOs(os)
{}

ECode BlockGuardOs::TagSocket(
    /* [in] */ Int32 fd,
    /* [out] */ Int32* retFd)
{
    VALIDATE_NOT_NULL(retFd);

    //try {
    AutoPtr<ISocketTagger> tagger;
    FAIL_RETURN(SocketTagger::Get((ISocketTagger**)&tagger))
    FAIL_RETURN(tagger->Tag(fd))
    *retFd = fd;
    //} catch (SocketException e) {
        //throw new ErrnoException("socket", EINVAL, e);
    //}

    return NOERROR;
}

ECode BlockGuardOs::UntagSocket(
    /* [in] */ Int32 fd)
{
    //try {
    AutoPtr<ISocketTagger> tagger;
    FAIL_RETURN(SocketTagger::Get((ISocketTagger**)&tagger))
    FAIL_RETURN(tagger->Untag(fd))
    //} catch (SocketException e) {
        //throw new ErrnoException("socket", EINVAL, e);
    //}

    return NOERROR;
}

ECode BlockGuardOs::Accept(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IInetSocketAddress* peerAddress,
    /* [out] */ Int32* retFd)
{
    VALIDATE_NOT_NULL(retFd);

    AutoPtr<IBlockGuardPolicy> policy;
    FAIL_RETURN(BlockGuard::GetThreadPolicy((IBlockGuardPolicy**)&policy))
    FAIL_RETURN(policy->OnNetwork())

    Int32 _retFd;
    FAIL_RETURN(mOs->Accept(fd, peerAddress, &_retFd))
    return TagSocket(_retFd, retFd);
}

ECode BlockGuardOs::Close(
    /* [in] */ Int32 fd)
{
    //try {
    AutoPtr<IStructStat> stat;
    FAIL_RETURN(CLibcore::sOs->Fstat(fd, (IStructStat**)&stat))
    Int32 mode;
    FAIL_RETURN(stat->GetMode(&mode))
    if (COsConstants::_S_ISSOCK(mode)) {
        Boolean ret;
        FAIL_RETURN(IsLingerSocket(fd, &ret));
        if (ret) {
            // If the fd is a socket with SO_LINGER set, we might block indefinitely.
            // We allow non-linger sockets so that apps can close their network
            // connections in methods like onDestroy which will run on the UI thread.
            AutoPtr<IBlockGuardPolicy> policy;
            FAIL_RETURN(BlockGuard::GetThreadPolicy((IBlockGuardPolicy**)&policy))
            FAIL_RETURN(policy->OnNetwork())
        }
        FAIL_RETURN(UntagSocket(fd))
    }
    //} catch (ErrnoException ignored) {
        // We're called via Socket.close (which doesn't ask for us to be called), so we
        // must not throw here, because Socket.close must not throw if asked to close an
        // already-closed socket. Also, the passed-in FileDescriptor isn't necessarily
        // a socket at all.
    //}
    return mOs->Close(fd);
}

ECode BlockGuardOs::IsLingerSocket(
    /* [in] */ Int32 fd,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IStructLinger> struLinger;
    FAIL_RETURN(CLibcore::sOs->GetsockoptLinger(fd, COsConstants::sSOL_SOCKET, COsConstants::sSO_LINGER, (IStructLinger**)&struLinger))
    Boolean isOn;
    FAIL_RETURN(struLinger->IsOn(&isOn))
    Int32 linger;
    FAIL_RETURN(struLinger->GetLinger(&linger))
    *result = isOn && linger > 0;

    return NOERROR;
}

ECode BlockGuardOs::Connect(
    /* [in] */ Int32 fd,
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port)
{
    AutoPtr<IBlockGuardPolicy> policy;
    FAIL_RETURN(BlockGuard::GetThreadPolicy((IBlockGuardPolicy**)&policy))
    FAIL_RETURN(policy->OnNetwork())

    return mOs->Connect(fd, address, port);
}

// TODO: Untag newFd when needed for dup2(FileDescriptor oldFd, int newFd)

ECode BlockGuardOs::Fdatasync(
    /* [in] */ Int32 fd)
{
    AutoPtr<IBlockGuardPolicy> policy;
    FAIL_RETURN(BlockGuard::GetThreadPolicy((IBlockGuardPolicy**)&policy))
    FAIL_RETURN(policy->OnWriteToDisk())

    return mOs->Fdatasync(fd);
}

ECode BlockGuardOs::Fsync(
    /* [in] */ Int32 fd)
{
    AutoPtr<IBlockGuardPolicy> policy;
    FAIL_RETURN(BlockGuard::GetThreadPolicy((IBlockGuardPolicy**)&policy))
    FAIL_RETURN(policy->OnWriteToDisk())

    return mOs->Fsync(fd);
}

ECode BlockGuardOs::Ftruncate(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 length)
{
    AutoPtr<IBlockGuardPolicy> policy;
    FAIL_RETURN(BlockGuard::GetThreadPolicy((IBlockGuardPolicy**)&policy))
    FAIL_RETURN(policy->OnWriteToDisk())

    return mOs->Ftruncate(fd, length);
}

ECode BlockGuardOs::Open(
    /* [in] */ const String& path,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mode,
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);

    AutoPtr<IBlockGuardPolicy> policy;
    FAIL_RETURN(BlockGuard::GetThreadPolicy((IBlockGuardPolicy**)&policy))
    FAIL_RETURN(policy->OnReadFromDisk())
    if ((mode & COsConstants::sO_ACCMODE) != O_RDONLY) {
        FAIL_RETURN(policy->OnWriteToDisk())
    }
    return mOs->Open(path, flags, mode, fd);
}

ECode BlockGuardOs::Poll(
    /* [in] */ const ArrayOf<IStructPollfd*>& fdStructs,
    /* [in] */ Int32 timeoutMs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    // Greater than 0 is a timeout in milliseconds and -1 means "block forever",
    // but 0 means "poll and return immediately", which shouldn't be subject to BlockGuard.
    if (timeoutMs != 0) {
        AutoPtr<IBlockGuardPolicy> policy;
        FAIL_RETURN(BlockGuard::GetThreadPolicy((IBlockGuardPolicy**)&policy))
        FAIL_RETURN(policy->OnNetwork())
    }
    return mOs->Poll(fdStructs, timeoutMs, result);
}

//ECode BlockGuardOs::int pread(FileDescriptor fd, ByteBuffer buffer, long offset)

ECode BlockGuardOs::Pread(
    /* [in] */ Int32 fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(bytes);
    VALIDATE_NOT_NULL(num);

    AutoPtr<IBlockGuardPolicy> policy;
    FAIL_RETURN(BlockGuard::GetThreadPolicy((IBlockGuardPolicy**)&policy))
    FAIL_RETURN(policy->OnReadFromDisk())

    return mOs->Pread(fd, bytes, byteOffset, byteCount, offset, num);
}

//ECode BlockGuardOs::int pwrite(FileDescriptor fd, ByteBuffer buffer, long offset)

ECode BlockGuardOs::Pwrite(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);

    AutoPtr<IBlockGuardPolicy> policy;
    FAIL_RETURN(BlockGuard::GetThreadPolicy((IBlockGuardPolicy**)&policy))
    FAIL_RETURN(policy->OnWriteToDisk())

    return mOs->Pwrite(fd, bytes, byteOffset, byteCount, offset, num);
}

//ECode BlockGuardOs::int read(FileDescriptor fd, ByteBuffer buffer)

ECode BlockGuardOs::Read(
    /* [in] */ Int32 fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(bytes);
    VALIDATE_NOT_NULL(num);

    AutoPtr<IBlockGuardPolicy> policy;
    FAIL_RETURN(BlockGuard::GetThreadPolicy((IBlockGuardPolicy**)&policy))
    FAIL_RETURN(policy->OnReadFromDisk())

    return mOs->Read(fd, bytes, byteOffset, byteCount, num);
}

//ECode BlockGuardOs::int readv(FileDescriptor fd, Object[] buffers, int[] offsets, int[] byteCounts)

//ECode BlockGuardOs::int recvfrom(FileDescriptor fd, ByteBuffer buffer, int flags, InetSocketAddress srcAddress)

ECode BlockGuardOs::Recvfrom(
    /* [in] */ Int32 fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int32 flags,
    /* [in] */ IInetSocketAddress* srcAddress,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(bytes);
    VALIDATE_NOT_NULL(num);

    AutoPtr<IBlockGuardPolicy> policy;
    FAIL_RETURN(BlockGuard::GetThreadPolicy((IBlockGuardPolicy**)&policy))
    FAIL_RETURN(policy->OnNetwork())

    return mOs->Recvfrom(fd, bytes, byteOffset, byteCount, flags, srcAddress, num);
}

//ECode BlockGuardOs::int sendto(FileDescriptor fd, ByteBuffer buffer, int flags, InetAddress inetAddress, int port)

ECode BlockGuardOs::Sendto(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int32 flags,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    // We permit datagrams without hostname lookups.
    if (inetAddress != NULL) {
        AutoPtr<IBlockGuardPolicy> policy;
        FAIL_RETURN(BlockGuard::GetThreadPolicy((IBlockGuardPolicy**)&policy))
        FAIL_RETURN(policy->OnNetwork())
    }
    return mOs->Sendto(fd, bytes, byteOffset, byteCount, flags, inetAddress, port, result);
}

ECode BlockGuardOs::Socket(
    /* [in] */ Int32 socketDomain,
    /* [in] */ Int32 type,
    /* [in] */ Int32 protocol,
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);

    Int32 _fd;
    FAIL_RETURN(mOs->Socket(socketDomain, type, protocol, &_fd))
    return TagSocket(_fd, fd);
}

ECode BlockGuardOs::Socketpair(
    /* [in] */ Int32 socketDomain,
    /* [in] */ Int32 type,
    /* [in] */ Int32 protocol,
    /* [out] */ Int32* fd1,
    /* [out] */ Int32* fd2)
{
    VALIDATE_NOT_NULL(fd1);
    VALIDATE_NOT_NULL(fd2);

    Int32 _fd;
    FAIL_RETURN(mOs->Socketpair(socketDomain, type, protocol, fd1, fd2))
    FAIL_RETURN(TagSocket(*fd1, &_fd))
    return TagSocket(*fd2, &_fd);
}

//ECode BlockGuardOs::int write(FileDescriptor fd, ByteBuffer buffer)

ECode BlockGuardOs::Write(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);

    AutoPtr<IBlockGuardPolicy> policy;
    FAIL_RETURN(BlockGuard::GetThreadPolicy((IBlockGuardPolicy**)&policy))
    FAIL_RETURN(policy->OnWriteToDisk())

    return mOs->Write(fd, bytes, byteOffset, byteCount, num);
}

//ECode BlockGuardOs::int writev(FileDescriptor fd, Object[] buffers, int[] offsets, int[] byteCounts)

} // namespace IO
} // namespace Libcore