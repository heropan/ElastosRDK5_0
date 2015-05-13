#include "cmdef.h"
#include "ForwardingOs.h"

namespace Libcore {
namespace IO {

ForwardingOs::ForwardingOs(
    /* [in] */ IOs* os)
    : mOs(os)
{}

CAR_INTERFACE_IMPL(ForwardingOs, IOs);

ECode ForwardingOs::Accept(
    /* [in] */ Int32 fd,
    /* [in] */ IInetSocketAddress* peerAddress,
    /* [out] */ Int32* retFd)
{
    VALIDATE_NOT_NULL(retFd);

    return mOs->Accept(fd, peerAddress, retFd);
}

ECode ForwardingOs::Access(
    /* [in] */ const String& path,
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return mOs->Access(path, mode, succeeded);
}

ECode ForwardingOs::Bind(
    /* [in] */ Int32 fd,
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port)
{
    return mOs->Bind(fd, address, port);
}

ECode ForwardingOs::Chmod(
    /* [in] */ const String& path,
    /* [in] */ Int32 mode)
{
    return mOs->Chmod(path, mode);
}

ECode ForwardingOs::Chown(
    /* [in] */ const String& path,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    return mOs->Chown(path, uid, gid);
}

ECode ForwardingOs::Close(
    /* [in] */ Int32 fd)
{
    return mOs->Close(fd);
}

ECode ForwardingOs::Connect(
    /* [in] */ Int32 fd,
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port)
{
    return mOs->Connect(fd, address, port);
}

ECode ForwardingOs::Dup(
    /* [in] */ Int32 oldFd,
    /* [out] */ Int32* retFd)
{
    VALIDATE_NOT_NULL(retFd);

    return mOs->Dup(oldFd, retFd);
}

ECode ForwardingOs::Dup2(
    /* [in] */ Int32 oldFd,
    /* [in] */ Int32 newFd,
    /* [out] */ Int32* retFd)
{
    VALIDATE_NOT_NULL(retFd);

    return mOs->Dup2(oldFd, newFd, retFd);
}

ECode ForwardingOs::Environ(
    /* [out, callee] */ ArrayOf<String>** env)
{
    return mOs->Environ(env);
}

ECode ForwardingOs::Fchmod(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 mode)
{
    return mOs->Fchmod(fd, mode);
}

ECode ForwardingOs::Fchown(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    return mOs->Fchown(fd, uid, gid);
}

ECode ForwardingOs::FcntlVoid(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 cmd,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return mOs->FcntlVoid(fd, cmd, result);
}

ECode ForwardingOs::FcntlInt64(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 cmd,
    /* [in] */ Int64 arg,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return mOs->FcntlInt64(fd, cmd, arg, result);
}

ECode ForwardingOs::FcntlFlock(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 cmd,
    /* [in] */ IStructFlock* arg,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return mOs->FcntlFlock(fd, cmd, arg, result);
}

ECode ForwardingOs::Fdatasync(
    /* [in] */ Int32 fd)
{
    return mOs->Fdatasync(fd);
}

ECode ForwardingOs::Fstat(
    /* [in] */ Int32 fd,
    /* [out] */ IStructStat** stat)
{
    VALIDATE_NOT_NULL(stat);

    return mOs->Fstat(fd, stat);
}

ECode ForwardingOs::Fstatfs(
    /* [in] */ Int32 fd,
    /* [out] */ IStructStatFs** statFs)
{
    VALIDATE_NOT_NULL(statFs);

    return mOs->Fstatfs(fd, statFs);
}

ECode ForwardingOs::Fsync(
    /* [in] */ Int32 fd)
{
    return mOs->Fsync(fd);
}

ECode ForwardingOs::Ftruncate(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 length)
{
    return mOs->Ftruncate(fd, length);
}

ECode ForwardingOs::Gai_strerror(
    /* [in] */ Int32 error,
    /* [out] */ String* strerror)
{
    VALIDATE_NOT_NULL(strerror);

    return mOs->Gai_strerror(error, strerror);
}

ECode ForwardingOs::Getaddrinfo(
    /* [in] */ const String& node,
    /* [in] */ IStructAddrinfo* hints,
    /* [out, callee] */ ArrayOf<IInetAddress*>** addrinfo)
{
    return mOs->Getaddrinfo(node, hints, addrinfo);
}

ECode ForwardingOs::Getegid(
    /* [out] */ Int32* egid)
{
    VALIDATE_NOT_NULL(egid);

    return mOs->Getegid(egid);
}

ECode ForwardingOs::Geteuid(
    /* [out] */ Int32* eEuid)
{
    VALIDATE_NOT_NULL(eEuid);

    return mOs->Geteuid(eEuid);
}

ECode ForwardingOs::Getgid(
    /* [out] */ Int32* gid)
{
    VALIDATE_NOT_NULL(gid);

    return mOs->Getgid(gid);
}

ECode ForwardingOs::Getenv(
    /* [in] */ const String& name,
    /* [out] */ String* env)
{
    VALIDATE_NOT_NULL(env);

    return mOs->Getenv(name, env);
}

ECode ForwardingOs::Getnameinfo(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 flags,
    /* [out] */ String* nameinfo)
{
    VALIDATE_NOT_NULL(nameinfo);

    return mOs->Getnameinfo(address, flags, nameinfo);
}

ECode ForwardingOs::Getpid(
    /* [out] */ Int32* pid)
{
    VALIDATE_NOT_NULL(pid);

    return mOs->Getpid(pid);
}

ECode ForwardingOs::Getppid(
    /* [out] */ Int32* ppid)
{
    VALIDATE_NOT_NULL(ppid);

    return mOs->Getppid(ppid);
}

ECode ForwardingOs::Getpwnam(
    /* [in] */ const String& name,
    /* [out] */ IStructPasswd** pwnam)
{
    VALIDATE_NOT_NULL(pwnam);

    return mOs->Getpwnam(name, pwnam);
}

ECode ForwardingOs::Getpwuid(
    /* [in] */ Int32 uid,
    /* [out] */ IStructPasswd** pwuid)
{
    VALIDATE_NOT_NULL(pwuid);

    return mOs->Getpwuid(uid, pwuid);
}

ECode ForwardingOs::Getsockname(
    /* [in] */ Int32 fd,
    /* [out] */ ISocketAddress** sockname)
{
    VALIDATE_NOT_NULL(sockname);

    return mOs->Getsockname(fd, sockname);
}

ECode ForwardingOs::GetsockoptByte(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ Int32* sockopt)
{
    VALIDATE_NOT_NULL(sockopt);

    return mOs->GetsockoptByte(fd, level, option, sockopt);
}

ECode ForwardingOs::GetsockoptInAddr(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IInetAddress** addr)
{
    VALIDATE_NOT_NULL(addr);

    return mOs->GetsockoptInAddr(fd, level, option, addr);
}

ECode ForwardingOs::GetsockoptInt(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ Int32* sockopt)
{
    VALIDATE_NOT_NULL(sockopt);

    return mOs->GetsockoptInt(fd, level, option, sockopt);
}

ECode ForwardingOs::GetsockoptLinger(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IStructLinger** linger)
{
    VALIDATE_NOT_NULL(linger);

    return mOs->GetsockoptLinger(fd, level, option, linger);
}

ECode ForwardingOs::GetsockoptTimeval(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IStructTimeval** timeval)
{
    VALIDATE_NOT_NULL(timeval);

    return mOs->GetsockoptTimeval(fd, level, option, timeval);
}

ECode ForwardingOs::Getuid(
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid);

    return mOs->Getuid(uid);
}

ECode ForwardingOs::If_indextoname(
    /* [in] */ Int32 index,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    return mOs->If_indextoname(index, name);
}

ECode ForwardingOs::Inet_pton(
    /* [in] */ Int32 family,
    /* [in] */ const String& address,
    /* [out] */ IInetAddress** addr)
{
    VALIDATE_NOT_NULL(addr);

    return mOs->Inet_pton(family, address, addr);
}

ECode ForwardingOs::IoctlInetAddress(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 cmd,
    /* [in] */ const String& interfaceName,
    /* [out] */ IInetAddress** addr)
{
    VALIDATE_NOT_NULL(addr);

    return mOs->IoctlInetAddress(fd, cmd, interfaceName, addr);
}

ECode ForwardingOs::IoctlInt(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 cmd,
    /* [in] */ Int32* arg,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return mOs->IoctlInt(fd, cmd, arg, result);
}

ECode ForwardingOs::Isatty(
    /* [in] */ Int32 fd,
    /* [out] */ Boolean* isatty)
{
    VALIDATE_NOT_NULL(isatty);

    return mOs->Isatty(fd, isatty);
}

ECode ForwardingOs::Kill(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 signal)
{
    return mOs->Kill(pid, signal);
}

ECode ForwardingOs::Lchown(
    /* [in] */ const String& path,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    return mOs->Lchown(path, uid, gid);
}

ECode ForwardingOs::Listen(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 backlog)
{
    return mOs->Listen(fd, backlog);
}

ECode ForwardingOs::Lseek(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int32 whence,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    return mOs->Lseek(fd, offset, whence, result);
}

ECode ForwardingOs::Lstat(
    /* [in] */ const String& path,
    /* [out] */ IStructStat** stat)
{
    VALIDATE_NOT_NULL(stat);

    return mOs->Lstat(path, stat);
}

ECode ForwardingOs::Mincore(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount,
    /* [in] */ const ArrayOf<Byte>& vector)
{
    return mOs->Mincore(address, byteCount, vector);
}

ECode ForwardingOs::Mkdir(
    /* [in] */ const String& path,
    /* [in] */ Int32 mode)
{
    return mOs->Mkdir(path, mode);
}

ECode ForwardingOs::Mlock(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount)
{
    return mOs->Mlock(address, byteCount);
}

ECode ForwardingOs::Mmap(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount,
    /* [in] */ Int32 prot,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 fd,
    /* [in] */ Int64 offset,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    return mOs->Mmap(address, byteCount, prot, flags, fd, offset, result);
}

ECode ForwardingOs::Msync(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount,
    /* [in] */ Int32 flags)
{
    return mOs->Msync(address, byteCount, flags);
}

ECode ForwardingOs::Munlock(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount)
{
    return mOs->Munlock(address, byteCount);
}

ECode ForwardingOs::Munmap(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount)
{
    return mOs->Munmap(address, byteCount);
}

ECode ForwardingOs::Open(
    /* [in] */ const String& path,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mode,
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);

    return mOs->Open(path, flags, mode, fd);
}

ECode ForwardingOs::Pipe(
    /* [out, callee] */ ArrayOf<Int32>** fds)
{
    return mOs->Pipe(fds);
}

ECode ForwardingOs::Poll(
    /* [in] */ const ArrayOf<IStructPollfd*>& fdStructs,
    /* [in] */ Int32 timeoutMs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return mOs->Poll(fdStructs, timeoutMs, result);
}

ECode ForwardingOs::Pread(
    /* [in] */ Int32 fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(bytes);
    VALIDATE_NOT_NULL(num);

    return mOs->Pread(fd, bytes, byteOffset, byteCount, offset, num);
}

ECode ForwardingOs::Pwrite(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);

    return mOs->Pwrite(fd, bytes, byteOffset, byteCount, offset, num);
}

ECode ForwardingOs::Read(
    /* [in] */ Int32 fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);

    return mOs->Read(fd, bytes, byteOffset, byteCount, num);
}

ECode ForwardingOs::Recvfrom(
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

    return mOs->Recvfrom(fd, bytes, byteOffset, byteCount, flags, srcAddress, num);
}

ECode ForwardingOs::Remove(
    /* [in] */ const String& path)
{
    return mOs->Remove(path);
}

ECode ForwardingOs::Rename(
    /* [in] */ const String& oldPath,
    /* [in] */ const String& newPath)
{
    return mOs->Rename(oldPath, newPath);
}

ECode ForwardingOs::Sendto(
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

    return mOs->Sendto(fd, bytes, byteOffset, byteCount, flags, inetAddress, port, result);
}

ECode ForwardingOs::Sendfile(
    /* [in] */ Int32 outFd,
    /* [in] */ Int32 inFd,
    /* [in, out] */ Int64* inOffset,
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(inOffset);
    VALIDATE_NOT_NULL(result);

    return mOs->Sendfile(outFd, inFd, inOffset, byteCount, result);
}

ECode ForwardingOs::Setegid(
    /* [in] */ Int32 egid)
{
    return mOs->Setegid(egid);
}

ECode ForwardingOs::Seteuid(
    /* [in] */ Int32 euid)
{
    return mOs->Seteuid(euid);
}

ECode ForwardingOs::Setgid(
    /* [in] */ Int32 gid)
{
    return mOs->Setgid(gid);
}

ECode ForwardingOs::Setsid(
    /* [out] */ Int32* sid)
{
    VALIDATE_NOT_NULL(sid);

    return mOs->Setsid(sid);
}

ECode ForwardingOs::SetsockoptByte(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ Int32 value)
{
    return mOs->SetsockoptByte(fd, level, option, value);
}

ECode ForwardingOs::SetsockoptIfreq(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ const String& interfaceName)
{
    return mOs->SetsockoptIfreq(fd, level, option, interfaceName);
}

ECode ForwardingOs::SetsockoptInt(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ Int32 value)
{
    return mOs->SetsockoptInt(fd, level, option, value);
}

ECode ForwardingOs::SetsockoptIpMreqn(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ Int32 value)
{
    return mOs->SetsockoptIpMreqn(fd, level, option, value);
}

ECode ForwardingOs::SetsockoptGroupReq(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ IStructGroupReq* value)
{
    return mOs->SetsockoptGroupReq(fd, level, option, value);
}

ECode ForwardingOs::SetsockoptLinger(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ IStructLinger* value)
{
    return mOs->SetsockoptLinger(fd, level, option, value);
}

ECode ForwardingOs::SetsockoptTimeval(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ IStructTimeval* value)
{
    return mOs->SetsockoptTimeval(fd, level, option, value);
}

ECode ForwardingOs::Setuid(
    /* [in] */ Int32 uid)
{
    return mOs->Setuid(uid);
}

ECode ForwardingOs::Shutdown(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 how)
{
    return mOs->Shutdown(fd, how);
}

ECode ForwardingOs::Socket(
    /* [in] */ Int32 socketDomain,
    /* [in] */ Int32 type,
    /* [in] */ Int32 protocol,
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);

    return mOs->Socket(socketDomain, type, protocol, fd);
}

ECode ForwardingOs::Socketpair(
    /* [in] */ Int32 socketDomain,
    /* [in] */ Int32 type,
    /* [in] */ Int32 protocol,
    /* [out] */ Int32* fd1,
    /* [out] */ Int32* fd2)
{
    VALIDATE_NOT_NULL(fd1);
    VALIDATE_NOT_NULL(fd2);

    return mOs->Socketpair(socketDomain, type, protocol, fd1, fd2);
}

ECode ForwardingOs::Stat(
    /* [in] */ const String& path,
    /* [out] */ IStructStat** stat)
{
    VALIDATE_NOT_NULL(stat);

    return mOs->Stat(path, stat);
}

ECode ForwardingOs::Statfs(
    /* [in] */ const String& path,
    /* [out] */ IStructStatFs** statfs)
{
    VALIDATE_NOT_NULL(statfs);

    return mOs->Statfs(path, statfs);
}

ECode ForwardingOs::Strerror(
    /* [in] */ Int32 errnum,
    /* [out] */ String* strerr)
{
    VALIDATE_NOT_NULL(strerr);

    return mOs->Strerror(errnum, strerr);
}

ECode ForwardingOs::Symlink(
    /* [in] */ const String& oldPath,
    /* [in] */ const String& newPath)
{
    return mOs->Symlink(oldPath, newPath);
}

ECode ForwardingOs::Sysconf(
    /* [in] */ Int32 name,
    /* [out] */ Int64* result)

{
    VALIDATE_NOT_NULL(result);

    return mOs->Sysconf(name, result);
}

ECode ForwardingOs::Tcdrain(
    /* [in] */ Int32 fd)
{
    return mOs->Tcdrain(fd);
}

ECode ForwardingOs::Umask(
    /* [in] */ Int32 mask,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return mOs->Umask(mask, result);
}

ECode ForwardingOs::Uname(
    /* [out] */ IStructUtsname** utsname)
{
    VALIDATE_NOT_NULL(utsname);

    return mOs->Uname(utsname);
}

ECode ForwardingOs::Waitpid(
    /* [in] */ Int32 pid,
    /* [in, out] */ Int32* _status,
    /* [in] */ Int32 options,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(_status);
    VALIDATE_NOT_NULL(result);

    return mOs->Waitpid(pid, _status, options, result);
}

ECode ForwardingOs::Write(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);

    return mOs->Write(fd, bytes, byteOffset, byteCount, num);
}

} // namespace IO
} // namespace Libcore