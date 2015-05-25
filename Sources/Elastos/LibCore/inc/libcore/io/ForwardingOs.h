
#ifndef __FORWARDINGOS_H__
#define __FORWARDINGOS_H__

#include "Elastos.CoreLibrary_server.h"

using Libcore::IO::IOs;
using Elastos::Net::IInetSocketAddress;
using Elastos::Net::IInetAddress;
using Elastos::Net::ISocketAddress;

namespace Libcore {
namespace IO {

/**
 * Subclass this if you want to override some {@link Os} methods but otherwise delegate.
 */
class ForwardingOs
    : public ElRefBase
    , public IOs
{
public:
    ForwardingOs(
        /* [in] */ IOs* os);

    CAR_INTERFACE_DECL();

    CARAPI Accept(
        /* [in] */ Int32 fd,
        /* [in] */ IInetSocketAddress* peerAddress,
        /* [out] */ Int32* retFd);

    CARAPI Access(
        /* [in] */ const String& path,
        /* [in] */ Int32 mode,
        /* [out] */ Boolean* succeeded);

    CARAPI Bind(
        /* [in] */ Int32 fd,
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port);

    CARAPI Chmod(
        /* [in] */ const String& path,
        /* [in] */ Int32 mode);

    CARAPI Chown(
        /* [in] */ const String& path,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    CARAPI Close(
        /* [in] */ Int32 fd);

    CARAPI Connect(
        /* [in] */ Int32 fd,
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port);

    CARAPI Dup(
        /* [in] */ Int32 oldFd,
        /* [out] */ Int32* retFd);

    CARAPI Dup2(
        /* [in] */ Int32 oldFd,
        /* [in] */ Int32 newFd,
        /* [out] */ Int32* retFd);

    CARAPI Environ(
        /* [out, callee] */ ArrayOf<String>** env);

    CARAPI Fchmod(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 mode);

    CARAPI Fchown(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    CARAPI FcntlVoid(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 cmd,
        /* [out] */ Int32* result);

    CARAPI FcntlInt64(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 cmd,
        /* [in] */ Int64 arg,
        /* [out] */ Int32* result);

    CARAPI FcntlFlock(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 cmd,
        /* [in] */ IStructFlock* arg,
        /* [out] */ Int32* result);

    CARAPI Fdatasync(
        /* [in] */ Int32 fd);

    CARAPI Fstat(
        /* [in] */ Int32 fd,
        /* [out] */ IStructStat** stat);

    CARAPI Fstatfs(
        /* [in] */ Int32 fd,
        /* [out] */ IStructStatFs** statFs);

    CARAPI Fsync(
        /* [in] */ Int32 fd);

    CARAPI Ftruncate(
        /* [in] */ Int32 fd,
        /* [in] */ Int64 length);

    CARAPI Gai_strerror(
        /* [in] */ Int32 error,
        /* [out] */ String* strerror);

    CARAPI Getaddrinfo(
        /* [in] */ const String& node,
        /* [in] */ IStructAddrinfo* hints,
        /* [out, callee] */ ArrayOf<IInetAddress*>** addrinfo);

    CARAPI Getegid(
        /* [out] */ Int32* egid);

    CARAPI Geteuid(
        /* [out] */ Int32* eEuid);

    CARAPI Getgid(
        /* [out] */ Int32* gid);

    CARAPI Getenv(
        /* [in] */ const String& name,
        /* [out] */ String* env);

    CARAPI Getnameinfo(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 flags,
        /* [out] */ String* nameinfo);

    CARAPI Getpid(
        /* [out] */ Int32* pid);

    CARAPI Getppid(
        /* [out] */ Int32* ppid);

    CARAPI Getpwnam(
        /* [in] */ const String& name,
        /* [out] */ IStructPasswd** pwnam);

    CARAPI Getpwuid(
        /* [in] */ Int32 uid,
        /* [out] */ IStructPasswd** pwuid);

    CARAPI Getsockname(
        /* [in] */ Int32 fd,
        /* [out] */ ISocketAddress** sockname);

    CARAPI GetsockoptByte(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [out] */ Int32* sockopt);

    CARAPI GetsockoptInAddr(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [out] */ IInetAddress** addr);

    CARAPI GetsockoptInt(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [out] */ Int32* sockopt);

    CARAPI GetsockoptLinger(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [out] */ IStructLinger ** linger);

    CARAPI GetsockoptTimeval(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [out] */ IStructTimeval ** timeval);

    CARAPI Getuid(
        /* [out] */ Int32* uid);

    CARAPI If_indextoname(
        /* [in] */ Int32 index,
        /* [out] */ String* name);

    CARAPI Inet_pton(
        /* [in] */ Int32 family,
        /* [in] */ const String& address,
        /* [out] */ IInetAddress** addr);

    CARAPI IoctlInetAddress(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 cmd,
        /* [in] */ const String& interfaceName,
        /* [out] */ IInetAddress** addr);

    CARAPI IoctlInt(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 cmd,
        /* [in] */ Int32* arg,
        /* [out] */ Int32* result);

    CARAPI Isatty(
        /* [in] */ Int32 fd,
        /* [out] */ Boolean* isatty);

    CARAPI Kill(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 signal);

    CARAPI Lchown(
        /* [in] */ const String& path,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    CARAPI Listen(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 backlog);

    CARAPI Lseek(
        /* [in] */ Int32 fd,
        /* [in] */ Int64 offset,
        /* [in] */ Int32 whence,
        /* [out] */ Int64* result);

    CARAPI Lstat(
        /* [in] */ const String& path,
        /* [out] */ IStructStat** stat);

    CARAPI Mincore(
        /* [in] */ Int64 address,
        /* [in] */ Int64 byteCount,
        /* [in] */ const ArrayOf<Byte>& vector);

    CARAPI Mkdir(
        /* [in] */ const String& path,
        /* [in] */ Int32 mode);

    CARAPI Mlock(
        /* [in] */ Int64 address,
        /* [in] */ Int64 byteCount);

    CARAPI Mmap(
        /* [in] */ Int64 address,
        /* [in] */ Int64 byteCount,
        /* [in] */ Int32 prot,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 fd,
        /* [in] */ Int64 offset,
        /* [out] */ Int64* result);

    CARAPI Msync(
        /* [in] */ Int64 address,
        /* [in] */ Int64 byteCount,
        /* [in] */ Int32 flags);

    CARAPI Munlock(
        /* [in] */ Int64 address,
        /* [in] */ Int64 byteCount);

    CARAPI Munmap(
        /* [in] */ Int64 address,
        /* [in] */ Int64 byteCount);

    CARAPI Open(
        /* [in] */ const String& path,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 mode,
        /* [out] */ Int32* fd);

    CARAPI Pipe(
        /* [out, callee] */ ArrayOf<Int32>** fds);

    CARAPI Poll(
        /* [in] */ const ArrayOf<IStructPollfd*>& fdStructs,
        /* [in] */ Int32 timeoutMs,
        /* [out] */ Int32* result);

    CARAPI Pread(
        /* [in] */ Int32 fd,
        /* [out] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int64 offset,
        /* [out] */ Int32* num);

    CARAPI Pwrite(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int64 offset,
        /* [out] */ Int32* num);

    CARAPI Read(
        /* [in] */ Int32 fd,
        /* [out] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* num);

    CARAPI Recvfrom(
        /* [in] */ Int32 fd,
        /* [out] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int32 flags,
        /* [in] */ IInetSocketAddress* srcAddress,
        /* [out] */ Int32* num);

    CARAPI Remove(
        /* [in] */ const String& path);

    CARAPI Rename(
        /* [in] */ const String& oldPath,
        /* [in] */ const String& newPath);

    CARAPI Sendto(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int32 flags,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [out] */ Int32* result);

    CARAPI Sendfile(
        /* [in] */ Int32 outFd,
        /* [in] */ Int32 inFd,
        /* [in, out] */ Int64* inOffset,
        /* [in] */ Int64 byteCount,
        /* [out] */ Int64* result);

    CARAPI Setegid(
        /* [in] */ Int32 egid);

    CARAPI Seteuid(
        /* [in] */ Int32 euid);

    CARAPI Setgid(
        /* [in] */ Int32 gid);

    CARAPI Setsid(
        /* [out] */ Int32* sid);

    CARAPI SetsockoptByte(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [in] */ Int32 value);

    CARAPI SetsockoptIfreq(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [in] */ const String& interfaceName);

    CARAPI SetsockoptInt(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [in] */ Int32 value);

    CARAPI SetsockoptIpMreqn(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [in] */ Int32 value);

    CARAPI SetsockoptGroupReq(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [in] */ IStructGroupReq* value);

    CARAPI SetsockoptLinger(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [in] */ IStructLinger* value);

    CARAPI SetsockoptTimeval(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [in] */ IStructTimeval* value);

    CARAPI Setuid(
        /* [in] */ Int32 uid);

    CARAPI Shutdown(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 how);

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

    CARAPI Stat(
        /* [in] */ const String& path,
        /* [out] */ IStructStat** stat);

    CARAPI Statfs(
        /* [in] */ const String& path,
        /* [out] */ IStructStatFs** statfs);

    CARAPI Strerror(
        /* [in] */ Int32 errnum,
        /* [out] */ String* strerr);

    CARAPI Symlink(
        /* [in] */ const String& oldPath,
        /* [in] */ const String& newPath);

    CARAPI Sysconf(
        /* [in] */ Int32 name,
        /* [out] */ Int64* result);

    CARAPI Tcdrain(
        /* [in] */ Int32 fd);

    CARAPI Umask(
        /* [in] */ Int32 mask,
        /* [out] */ Int32* result);

    CARAPI Uname(
        /* [out] */ IStructUtsname** utsname);

    CARAPI Waitpid(
        /* [in] */ Int32 pid,
        /* [in, out] */ Int32* _status,
        /* [in] */ Int32 options,
        /* [out] */ Int32* result);

    CARAPI Write(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* num);

protected:
    AutoPtr<IOs> mOs;

};

} // namespace IO
} // namespace Libcore

#endif // __FORWARDINGOS_H__
