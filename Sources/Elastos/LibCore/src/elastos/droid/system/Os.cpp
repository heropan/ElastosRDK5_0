#include "Os.h"

namespace Elastos {
namespace Droid {
namespace System {

ECode Os::Accept(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IInetSocketAddress* peerAddress,
    /* [out] */ Int32* retFd);

ECode Os::Access(
    /* [in] */ const String& path,
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* succeed);

ECode Os::Elastos_getaddrinfo(
    /* [in] */ const String& node,
    /* [in] */ IStructAddrinfo* hints,
    /* [in] */ Int32 netId,
    /* [out, callee] */ ArrayOf<IInetAddress*>** info);

ECode Os::Bind(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port);

ECode Os::Chmod(
    /* [in] */ const String& path,
    /* [in] */ Int32 mode);

ECode Os::Chown(
    /* [in] */ const String& path,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid);

ECode Os::Close(
    /* [in] */ IFileDescriptor* fd);

ECode Os::Connect(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port);

ECode Os::Dup(
    /* [in] */ Int32 oldFd,
    /* [out] */ Int32* retFd);

ECode Os::Dup2(
    /* [in] */ Int32 oldFd,
    /* [in] */ Int32 newFd,
    /* [out] */ Int32* retFd);

ECode Os::Environ(
    /* [out, callee] */ ArrayOf<String>** env);

ECode Os::Execv(
    /* [in] */ const String& filename,
    /* [in] */ ArrayOf<String>* argv);

ECode Os::Execve(
    /* [in] */ const String& filename,
    /* [in] */ ArrayOf<String>* argv,
    /* [in] */ ArrayOf<String>* envp);

ECode Os::Fchmod(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 mode);

ECode Os::Fchown(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid);

ECode Os::FcntlVoid(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 cmd,
    /* [out] */ Int32* result);

ECode Os::FcntlInt64(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 cmd,
    /* [in] */ Int64 arg,
    /* [out] */ Int32* result);

ECode Os::FcntlFlock(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 cmd,
    /* [in] */ IStructFlock* arg,
    /* [out] */ Int32* result);

ECode Os::Fdatasync(
    /* [in] */ IFileDescriptor* fd);

ECode Os::Fstat(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ IStructStat** stat);

ECode Os::Fstatvfs(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ IStructStatVfs** statFs);

ECode Os::Fsync(
    /* [in] */ IFileDescriptor* fd);

ECode Os::Ftruncate(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 length);

ECode Os::Gai_strerror(
    /* [in] */ Int32 error,
    /* [out] */ String* strerror);

ECode Os::Getegid(
    /* [out] */ Int32* egid);

ECode Os::Geteuid(
    /* [out] */ Int32* euid);

ECode Os::Getgid(
    /* [out] */ Int32* gid);

ECode Os::Getenv(
    /* [in] */ const String& name,
    /* [out] */ String* env);

/* TODO: break into getnameinfoHost and getnameinfoService? */
ECode Os::Getnameinfo(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 flags,
    /* [out] */ String* nameinfo);

ECode Os::Getpeername(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ ISocketAddress** peername);

ECode Os::Getpid(
    /* [out] */ Int32* pid);

ECode Os::Getppid(
    /* [out] */ Int32* ppid);

ECode Os::Getpwnam(
    /* [in] */ const String& name,
    /* [out] */ IStructPasswd** pwnam);

ECode Os::Getpwuid(
    /* [in] */ Int32 uid,
    /* [out] */ IStructPasswd** pwuid);

ECode Os::Getsockname(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ ISocketAddress** sockname);

ECode Os::GetsockoptByte(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ Int32* sockopt);

ECode Os::GetsockoptInAddr(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IInetAddress** addr);

ECode Os::GetsockoptInt(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ Int32* sockopt);

ECode Os::GetsockoptLinger(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IStructLinger** linger);

ECode Os::GetsockoptTimeval(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IStructTimeval** timeval);

ECode Os::GetsockoptUcred(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IStructUcred** ucred);

ECode Os::Gettid(
    /* [out] */ Int32* tid);

ECode Os::Getuid(
    /* [out] */ Int32* uid);

ECode Os::If_indextoname(
    /* [in] */ Int32 index,
    /* [out] */ String* name);

ECode Os::Inet_pton(
    /* [in] */ Int32 family,
    /* [in] */ const String& address,
    /* [out] */ IInetAddress** addr);

ECode Os::IoctlInetAddress(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 cmd,
    /* [in] */ const String& interfaceName,
    /* [out] */ IInetAddress** addr);

ECode Os::IoctlInt(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 cmd,
    /* [in] */ Int32* arg,
    /* [out] */ Int32* result);

ECode Os::Isatty(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ Boolean* isatty);

ECode Os::Kill(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 signal);

ECode Os::Lchown(
    /* [in] */ const String& path,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid);

ECode Os::Link(
    /* [in] */ const String& oldPath,
    /* [in] */ const String& newPath);

ECode Os::Listen(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 backlog);

ECode Os::Lseek(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int32 whence,
    /* [out] */ Int64* result);

ECode Os::Lstat(
    /* [in] */ const String& path,
    /* [out] */ IStructStat** stat);

ECode Os::Mincore(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount,
    /* [in] */ ArrayOf<Byte>* vector);

ECode Os::Mkdir(
    /* [in] */ const String& path,
    /* [in] */ Int32 mode);

ECode Os::Mlock(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount);

ECode Os::Mkfifo(
    /* [in] */ const String& path,
    /* [in] */ Int32 mode);

ECode Os::Mmap(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount,
    /* [in] */ Int32 prot,
    /* [in] */ Int32 flags,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [out] */ Int64* result);

ECode Os::Msync(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount,
    /* [in] */ Int32 flags);

ECode Os::Munlock(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount);

ECode Os::Munmap(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount);

ECode Os::Open(
    /* [in] */ const String& path,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mode,
    /* [out] */ IFileDescriptor** fd);

ECode Os::Pipe(
    /* [out, callee] */ ArrayOf<Int32>** fds);

/* TODO: if we used the non-standard ppoll(2) behind the scenes, we could take a long timeout. */
ECode Os::Poll(
    /* [in] */ ArrayOf<IStructPollfd*>* fds,
    /* [in] */ Int32 timeoutMs,
    /* [out] */ Int32* result);

ECode Os::Posix_fallocate(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length);

ECode Os::Prctl(
    /* [in] */ Int32 option,
    /* [in] */ Int64 arg2,
    /* [in] */ Int64 arg3,
    /* [in] */ Int64 arg4,
    /* [in] */ Int64 arg5,
    /* [out] */ Int32* prctl);

ECode Os::Pread(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num);

ECode Os::Pread(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num);

ECode Os::Pwrite(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num);

ECode Os::Pwrite(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num);

ECode Os::Read(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* num);

ECode Os::Read(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IByteBuffer* buffer,
    /* [out] */ Int32* num);

ECode Os::Readlink(
    /* [in] */ const String& path,
    /* [out] */ String* link);

ECode Os::Readv(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<IInterface*>* buffers,
    /* [in] */ ArrayOf<Int32>* offsets,
    /* [in] */ ArrayOf<Int32>* byteCounts,
    /* [out] */ Int32* num);

ECode Os::Recvfrom(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int32 flags,
    /* [in] */ IInetSocketAddress* srcAddress,
    /* [out] */ Int32* num);

ECode Os::Recvfrom(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int32 flags,
    /* [in] */ IInetSocketAddress* srcAddress,
    /* [out] */ Int32* num);

ECode Os::Remove(
    /* [in] */ const String& path);

ECode Os::Rename(
    /* [in] */ const String& oldPath,
    /* [in] */ const String& newPath);

ECode Os::Sendto(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int32 flags,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [out] */ Int32* result);

ECode Os::Sendto(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int32 flags,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [out] */ Int32* result);

ECode Os::Sendfile(
    /* [in] */ Int32 outFd,
    /* [in] */ Int32 inFd,
    [in/*, out */] Int64* inOffset,
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* result);

ECode Os::Setegid(
    /* [in] */ Int32 egid);

ECode Os::Setenv(
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [in] */ Boolean overwrite);

ECode Os::Seteuid(
    /* [in] */ Int32 euid);

ECode Os::Setgid(
    /* [in] */ Int32 gid);

ECode Os::Setsid(
    /* [out] */ Int32* sid);

ECode Os::SetsockoptByte(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ Int32 value);

ECode Os::SetsockoptIfreq(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ const String& interfaceName);

ECode Os::SetsockoptInt(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ Int32 value);

ECode Os::SetsockoptIpMreqn(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ Int32 value);

ECode Os::SetsockoptGroupReq(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ IStructGroupReq* value);

ECode Os::SetsockoptGroupSourceReq(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ IStructGroupSourceReq* value);

ECode Os::SetsockoptLinger(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ IStructLinger* value);

ECode Os::SetsockoptTimeval(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ IStructTimeval* value);

ECode Os::Setuid(
    /* [in] */ Int32 uid);

ECode Os::Shutdown(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 how);

ECode Os::Socket(
    /* [in] */ Int32 socketDomain,
    /* [in] */ Int32 type,
    /* [in] */ Int32 protocol,
    /* [out] */ IFileDescriptor** fd);

ECode Os::Socketpair(
    /* [in] */ Int32 socketDomain,
    /* [in] */ Int32 type,
    /* [in] */ Int32 protocol,
    /* [out] */ IFileDescriptor** fd1,
    /* [out] */ IFileDescriptor** fd2);

ECode Os::Stat(
    /* [in] */ const String& path,
    /* [out] */ IStructStat** stat);

/* TODO: replace statfs with statvfs. */
ECode Os::StatVfs(
    /* [in] */ const String& path,
    /* [out] */ IStructStatVfs** statfs);

ECode Os::Strerror(
    /* [in] */ Int32 errno,
    /* [out] */ String* strerr);

ECode Os::Symlink(
    /* [in] */ const String& oldPath,
    /* [in] */ const String& newPath);

ECode Os::Sysconf(
    /* [in] */ Int32 name,
    /* [out] */ Int64* result);

ECode Os::Tcdrain(
    /* [in] */ IFileDescriptor* fd);

ECode Os::Umask(
    /* [in] */ Int32 mask,
    /* [out] */ Int32* result);

ECode Os::Uname(
    /* [out] */ IStructUtsname** uname);

ECode Os::Waitpid(
    /* [in] */ Int32 pid,
    [in/*, out */] Int32* status,
    /* [in] */ Int32 options,
    /* [out] */ Int32* result);

ECode Os::Write(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* num);

ECode Os::Write(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IByteBuffer* buffer,
    /* [out] */ Int32* num);

ECode Os::Writev(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<IInterface*>* buffers,
    /* [in] */ ArrayOf<Int32>* offsets,
    /* [in] */ ArrayOf<Int32>* byteCounts,
    /* [out] */ Int32* result);

}// namespace System
}// namespace Droid
}// namespace Elastos