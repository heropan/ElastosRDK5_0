#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <pwd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#ifndef __APPLE__
#include <sys/prctl.h>
#endif
#include <sys/socket.h>
#include <sys/stat.h>
#ifdef __APPLE__
#include <sys/statvfs.h>
#endif
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <cutils/log.h>

#include <stdio.h>
#include <string.h>
#include <sys/un.h>

#include "Posix.h"
#include "AsynchronousCloseMonitorNative.h"
#include "io/CFileDescriptor.h"
#include "NetworkUtilities.h"

using Elastos::Core::Object;
using Elastos::IO::CFileDescriptor;

#define NET_FAILURE_RETRY(return_type, return_value, syscall_name, fd, ...) ({ \
    return_type _rc = -1; \
    return_value = NOERROR; \
    do { \
        Boolean _wasSignaled; \
        Int32 _syscallErrno; \
        { \
            Int32 _fd; \
            fd->GetDescriptor(&_fd); \
            AsynchronousCloseMonitorNative _monitor(_fd); \
            _rc = syscall_name(_fd, __VA_ARGS__); \
            _syscallErrno = errno; \
            _wasSignaled = _monitor.WasSignaled(); \
        } \
        if (_wasSignaled) { \
            ALOGE("Libcore::IO::Posix: Socket closed"); \
            return_value = E_SOCKET_EXCEPTION; \
            _rc = -1; \
            break; \
        } \
        if (_rc == -1 && _syscallErrno != EINTR) { \
            /* TODO: with a format string we could show the arguments too, like strace(1). */ \
            ALOGE("Libcore::IO::Posix: " # syscall_name "error"); \
            return_value = E_SOCKET_EXCEPTION; \
            break; \
        } \
    } while (_rc == -1); /* _syscallErrno == EINTR && !_wasSignaled */ \
    _rc; })

#define IO_FAILURE_RETRY(return_type, return_value, syscall_name, fd, ...) ({ \
    return_type _rc = -1; \
    return_value = NOERROR; \
    Int32 _fd; \
    fd->GetDescriptor(&_fd); \
    if (_fd == -1) { \
        return_value = E_IO_Exception; \
        ALOGE("Libcore::IO::Posix: File descriptor closed"); \
    } else { \
        do { \
            Boolean _wasSignaled; \
            Int32 _syscallErrno; \
            { \
                AsynchronousCloseMonitorNative _monitor(_fd); \
                _rc = syscall_name(_fd, __VA_ARGS__); \
                _syscallErrno = errno; \
                _wasSignaled = _monitor.WasSignaled(); \
            } \
            if (_wasSignaled) { \
                ALOGE("Libcore::IO::Posix: " # syscall_name "interrupted"); \
                return_value = E_IO_EXCEPTION; \
                _rc = -1; \
                break; \
            } \
            if (_rc == -1 && _syscallErrno != EINTR) { \
                /* TODO: with a format string we could show the arguments too, like strace(1). */ \
                ALOGE("Libcore::IO::Posix: " # syscall_name "error"); \
                return_value = E_IO_EXCEPTION; \
                break; \
            } \
        } while (_rc == -1); /* && _syscallErrno == EINTR && !_wasSignaled */ \
    } \
    _rc; })

struct addrinfo_deleter {
    void operator()(addrinfo* p) const {
        if (p != NULL) { // bionic's freeaddrinfo(3) crashes when passed NULL.
            freeaddrinfo(p);
        }
    }
};

template <typename rc_t>
static ECode ErrorIfMinusOne(const char* name, rc_t rc) {
    if (rc == rc_t(-1)) {
        ALOGE("System-call error: %s", name);
        return E_ERRNO_EXCEPTION;
    }
    return NOERROR;
}

namespace Libcore {
namespace IO {



static Boolean FillInetSocketAddress(
    /* [in] */ Int32 rc,
    /* [in] */ IInetSocketAddress* socketAddress,
    /* [in] */ const sockaddr_storage& ss) {
    if (rc == -1 || socketAddress == NULL) {
        return TRUE;
    }
    // Fill out the passed-in InetSocketAddress with the sender's IP address and port number.
    Int32 port;
    AutoPtr<IInetAddress> sender = SockaddrToInetAddress(ss, &port);
    if (sender == NULL) {
        return FALSE;
    }

    socketAddress->SetAddress(sender);
    socketAddress->SetPort(port);
    return TRUE;
}


ECode Posix::Accept(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IInetSocketAddress* peerAddress,
    /* [out] */ IFileDescriptor** retFd)
{
    *retFd = NULL;
    sockaddr_storage ss;
    socklen_t sl = sizeof(ss);
    memset(&ss, 0, sizeof(ss));
    sockaddr* peer = (peerAddress != NULL) ? reinterpret_cast<sockaddr*>(&ss) : NULL;
    socklen_t* peerLength = (peerAddress != NULL) ? &sl : 0;
    ECode ec;
    Int32 clientFd = NET_FAILURE_RETRY(Int32, ec, accept, fd, peer, peerLength);
    if (clientFd == -1 || !FillInetSocketAddress(clientFd, peerAddress, ss)) {
        close(clientFd);
    } else if (clientFd != -1) {
        CFileDescriptor::New(retFd);
        (*retFd)->SetDescriptor(clientFd);
    }
    return ec;
}

ECode Posix::Access(
    /* [in] */ String path,
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* succeed)
{
    *succeed = FALSE;
    if (path == NULL) {
        return NOERROR;
    }
    Int32 rc = TEMP_FAILURE_RETRY(access(path, mode));
    if (rc == -1) {
        ALOGE("System-call : access Error, errno = %d", errno);
        return E_ERRNO_EXCEPTION;
    }
    *succeed = (rc == 0);
    return NOERROR;
}

ECode Posix::Elastos_getaddrinfo(
    /* [in] */ String node,
    /* [in] */ IStructAddrinfo* _hints,
    /* [in] */ Int32 netId,
    /* [out, callee] */ ArrayOf<IInetAddress*>** info)
{
    *info = NULL;
    if (node == NULL) {
        return NOERROR;
    }

    // static jfieldID flagsFid = env->GetFieldID(JniConstants::structAddrinfoClass, "ai_flags", "I");
    // static jfieldID familyFid = env->GetFieldID(JniConstants::structAddrinfoClass, "ai_family", "I");
    // static jfieldID socktypeFid = env->GetFieldID(JniConstants::structAddrinfoClass, "ai_socktype", "I");
    // static jfieldID protocolFid = env->GetFieldID(JniConstants::structAddrinfoClass, "ai_protocol", "I");

    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    Int32 valueTmp;
    _hints->GetFlags(&valueTmp);
    hints.ai_flags = valueTmp;
    _hints->GetFamily(&valueTmp);
    hints.ai_family = valueTmp;
    _hints->GetSocktype(&valueTmp);
    hints.ai_socktype = valueTmp;
    _hints->GetProtocol(&valueTmp);
    hints.ai_protocol = valueTmp;

    addrinfo* addressList = NULL;
    errno = 0;
    Int32 rc;// = android_getaddrinfofornet(node.c_str(), NULL, &hints, netId, 0, &addressList);
    if (rc != 0) {
        // throwGaiException(env, "android_getaddrinfo", rc);
        ALOGE("GaiException: Elastos_getaddrinfo");
        return E_GAI_EXCEPTION;
    }

    // Count results so we know how to size the output array.
    Int32 addressCount = 0;
    for (addrinfo* ai = addressList; ai != NULL; ai = ai->ai_next) {
        if (ai->ai_family == AF_INET || ai->ai_family == AF_INET6) {
            ++addressCount;
        } else {
            ALOGE("android_getaddrinfo unexpected ai_family %i", ai->ai_family);
        }
    }
    if (addressCount == 0) {
        return NOERROR;
    }

    // Prepare output array.
    AutoPtr<ArrayOf<IInetAddress*> > result = ArrayOf<IInetAddress*>::Alloc(addressCount);

    // Examine returned addresses one by one, save them in the output array.
    int index = 0;
    for (addrinfo* ai = addressList; ai != NULL; ai = ai->ai_next) {
        if (ai->ai_family != AF_INET && ai->ai_family != AF_INET6) {
            // Unknown address family. Skip this address.
            ALOGE("android_getaddrinfo unexpected ai_family %i", ai->ai_family);
            continue;
        }

        // Convert each IP address into a Java byte array.
        sockaddr_storage& address = *reinterpret_cast<sockaddr_storage*>(ai->ai_addr);
        AutoPtr<IInetAddress> inetAddress = SockaddrToInetAddress(address, NULL);
        if (inetAddress == NULL) {
            return NOERROR;
        }
        result->Set(index, inetAddress);
        ++index;
    }
    *info = result;
    REFCOUNT_ADD(*info)
    addrinfo_deleter()(addressList);
    return NOERROR;
}

ECode Posix::Bind(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port)
{
    sockaddr_storage ss;
    socklen_t sa_len;
    if (!InetAddressToSockaddr(address, port, ss, sa_len)) {
        return NOERROR;
    }
    const sockaddr* sa = reinterpret_cast<const sockaddr*>(&ss);
    // We don't need the return value because we'll already have thrown.
    ECode ec;
    (void) NET_FAILURE_RETRY(int, ec, bind, fd, sa, sa_len);
    return ec;
}

ECode Posix::Chmod(
    /* [in] */ String path,
    /* [in] */ Int32 mode)
{
    if (path == NULL) {
        return NOERROR;
    }
    return ErrorIfMinusOne("chmod", TEMP_FAILURE_RETRY(chmod(path, mode)));
}

ECode Posix::Chown(
    /* [in] */ String path,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    if (path == NULL) {
        return NOERROR;
    }
    return ErrorIfMinusOne("chown", TEMP_FAILURE_RETRY(chown(path, uid, gid)));
}

ECode Posix::Close(
    /* [in] */ IFileDescriptor* fd)
{
    // Get the FileDescriptor's 'fd' field and clear it.
    // We need to do this before we can throw an IOException (http://b/3222087).
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    fd->SetDescriptor(-1);

    // Even if close(2) fails with EINTR, the fd will have been closed.
    // Using TEMP_FAILURE_RETRY will either lead to EBADF or closing someone else's fd.
    // http://lkml.indiana.edu/hypermail/linux/kernel/0509.1/0877.html
    return ErrorIfMinusOne("close", close(_fd));
}

ECode Posix::Connect(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port)
{
    sockaddr_storage ss;
    socklen_t sa_len;
    if (!InetAddressToSockaddr(address, port, ss, sa_len)) {
        return NOERROR;
    }
    const sockaddr* sa = reinterpret_cast<const sockaddr*>(&ss);
    // We don't need the return value because we'll already have thrown.
    ECode ec;
    (void) NET_FAILURE_RETRY(Int32, ec, connect, fd, sa, sa_len);
    return ec;
}

ECode Posix::Dup(
    /* [in] */ Int32 oldFd,
    /* [out] */ Int32* retFd)
{
    return NOERROR;
}

ECode Posix::Dup2(
    /* [in] */ Int32 oldFd,
    /* [in] */ Int32 newFd,
    /* [out] */ Int32* retFd)
{
    return NOERROR;
}

ECode Posix::Environ(
    /* [out, callee] */ ArrayOf<String>* env)
{
    return NOERROR;
}

ECode Posix::Execv(
    /* [in] */ String filename,
    /* [in] */ ArrayOf<String>* argv)
{
    return NOERROR;
}

ECode Posix::Execve(
    /* [in] */ String filename,
    /* [in] */ ArrayOf<String>* argv,
    /* [in] */ ArrayOf<String>* envp)
{
    return NOERROR;
}

ECode Posix::Fchmod(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 mode)
{
    return NOERROR;
}

ECode Posix::Fchown(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    return NOERROR;
}

ECode Posix::FcntlVoid(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 cmd,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

ECode Posix::FcntlInt64(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 cmd,
    /* [in] */ Int64 arg,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

ECode Posix::FcntlFlock(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 cmd,
    /* [in] */ IStructFlock* arg,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

ECode Posix::Fdatasync(
    /* [in] */ IFileDescriptor* fd)
{
    return NOERROR;
}

ECode Posix::Fstat(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ IStructStat** stat)
{
    return NOERROR;
}

ECode Posix::Fstatvfs(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ IStructStatVfs** statFs)
{
    return NOERROR;
}

ECode Posix::Fsync(
    /* [in] */ IFileDescriptor* fd)
{
    return NOERROR;
}

ECode Posix::Ftruncate(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 length)
{
    return NOERROR;
}

ECode Posix::Gai_strerror(
    /* [in] */ Int32 error,
    /* [out] */ String* strerror)
{
    return NOERROR;
}

ECode Posix::Getegid(
    /* [out] */ Int32* egid)
{
    return NOERROR;
}

ECode Posix::Geteuid(
    /* [out] */ Int32* euid)
{
    return NOERROR;
}

ECode Posix::Getgid(
    /* [out] */ Int32* gid)
{
    return NOERROR;
}

ECode Posix::Getenv(
    /* [in] */ String name,
    /* [out] */ String* env)
{
    return NOERROR;
}

    /* TODO: break into getnameinfoHost and getnameinfoService? */
ECode Posix::Getnameinfo(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 flags,
    /* [out] */ String* nameinfo)
{
    return NOERROR;
}

ECode Posix::Getpeername(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ ISocketAddress** peername)
{
    return NOERROR;
}

ECode Posix::Getpid(
    /* [out] */ Int32* pid)
{
    return NOERROR;
}

ECode Posix::Getppid(
    /* [out] */ Int32* ppid)
{
    return NOERROR;
}

ECode Posix::Getpwnam(
    /* [in] */ String name,
    /* [out] */ IStructPasswd** pwnam)
{
    return NOERROR;
}

ECode Posix::Getpwuid(
    /* [in] */ Int32 uid,
    /* [out] */ IStructPasswd** pwuid)
{
    return NOERROR;
}

ECode Posix::Getsockname(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ ISocketAddress** sockname)
{
    return NOERROR;
}

ECode Posix::GetsockoptByte(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ Int32* sockopt)
{
    return NOERROR;
}

ECode Posix::GetsockoptInAddr(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IInetAddress** addr)
{
    return NOERROR;
}

ECode Posix::GetsockoptInt(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ Int32* sockopt)
{
    return NOERROR;
}

ECode Posix::GetsockoptLinger(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IStructLinger** linger)
{
    return NOERROR;
}

ECode Posix::GetsockoptTimeval(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IStructTimeval** timeval)
{
    return NOERROR;
}

ECode Posix::GetsockoptUcred(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IStructUcred** ucred)
{
    return NOERROR;
}

ECode Posix::Gettid(
    /* [out] */ Int32* tid)
{
    return NOERROR;
}

ECode Posix::Getuid(
    /* [out] */ Int32* uid)
{
    return NOERROR;
}

ECode Posix::If_indextoname(
    /* [in] */ Int32 index,
    /* [out] */ String* name)
{
    return NOERROR;
}

ECode Posix::Inet_pton(
    /* [in] */ Int32 family,
    /* [in] */ String address,
    /* [out] */ IInetAddress** addr)
{
    return NOERROR;
}

ECode Posix::IoctlInetAddress(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 cmd,
    /* [in] */ String interfaceName,
    /* [out] */ IInetAddress** addr)
{
    return NOERROR;
}

ECode Posix::IoctlInt(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 cmd,
    /* [in] */ Int32* arg,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

ECode Posix::Isatty(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ Boolean* isatty)
{
    return NOERROR;
}

ECode Posix::Kill(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 signal)
{
    return NOERROR;
}

ECode Posix::Lchown(
    /* [in] */ String path,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    return NOERROR;
}

ECode Posix::Link(
    /* [in] */ String oldPath,
    /* [in] */ String newPath)
{
    return NOERROR;
}

ECode Posix::Listen(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 backlog)
{
    return NOERROR;
}

ECode Posix::Lseek(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int32 whence,
    /* [out] */ Int64* result)
{
    return NOERROR;
}

ECode Posix::Lstat(
    /* [in] */ String path,
    /* [out] */ IStructStat** stat)
{
    return NOERROR;
}

ECode Posix::Mincore(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount,
    /* [in] */ ArrayOf<Byte>* vector)
{
    return NOERROR;
}

ECode Posix::Mkdir(
    /* [in] */ String path,
    /* [in] */ Int32 mode)
{
    return NOERROR;
}

ECode Posix::Mlock(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount)
{
    return NOERROR;
}

ECode Posix::Mkfifo(
    /* [in] */ String path,
    /* [in] */ Int32 mode)
{
    return NOERROR;
}

ECode Posix::Mmap(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount,
    /* [in] */ Int32 prot,
    /* [in] */ Int32 flags,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [out] */ Int64* result)
{
    return NOERROR;
}

ECode Posix::Msync(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount,
    /* [in] */ Int32 flags)
{
    return NOERROR;
}

ECode Posix::Munlock(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount)
{
    return NOERROR;
}

ECode Posix::Munmap(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount)
{
    return NOERROR;
}

ECode Posix::Open(
    /* [in] */ String path,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mode,
    /* [out] */ IFileDescriptor** fd)
{
    return NOERROR;
}

ECode Posix::Pipe(
    /* [out, callee] */ ArrayOf<Int32>* fds)
{
    return NOERROR;
}

    /* TODO: if we used the non-standard ppoll(2) behind the scenes, we could take a long timeout. */
ECode Posix::Poll(
    /* [in] */ ArrayOf<IStructPollfd*>* fds,
    /* [in] */ Int32 timeoutMs,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

ECode Posix::Posix_fallocate(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length)
{
    return NOERROR;
}

ECode Posix::Prctl(
    /* [in] */ Int32 option,
    /* [in] */ Int64 arg2,
    /* [in] */ Int64 arg3,
    /* [in] */ Int64 arg4,
    /* [in] */ Int64 arg5,
    /* [out] */ Int32* prctl)
{
    return NOERROR;
}

ECode Posix::Pread(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num)
{
    return NOERROR;
}

ECode Posix::Pread(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num)
{
    return NOERROR;
}

ECode Posix::Pwrite(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num)
{
    return NOERROR;
}

ECode Posix::Pwrite(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num)
{
    return NOERROR;
}

ECode Posix::Read(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* num)
{
    return NOERROR;
}

ECode Posix::Read(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IByteBuffer* buffer,
    /* [out] */ Int32* num)
{
    return NOERROR;
}

ECode Posix::Readlink(
    /* [in] */ String path,
    /* [out] */ String* link)
{
    return NOERROR;
}

ECode Posix::Readv(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<IInterface*>* buffers,
    /* [in] */ ArrayOf<Int32>* offsets,
    /* [in] */ ArrayOf<Int32>* byteCounts,
    /* [out] */ Int32* num)
{
    return NOERROR;
}

ECode Posix::Recvfrom(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int32 flags,
    /* [in] */ IInetSocketAddress* srcAddress,
    /* [out] */ Int32* num)
{
    return NOERROR;
}

ECode Posix::Recvfrom(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int32 flags,
    /* [in] */ IInetSocketAddress* srcAddress,
    /* [out] */ Int32* num)
{
    return NOERROR;
}

ECode Posix::Remove(
    /* [in] */ String path)
{
    return NOERROR;
}

ECode Posix::Rename(
    /* [in] */ String oldPath,
    /* [in] */ String newPath)
{
    return NOERROR;
}

ECode Posix::Sendto(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int32 flags,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

ECode Posix::Sendto(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int32 flags,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

ECode Posix::Sendfile(
    /* [in] */ Int32 outFd,
    /* [in] */ Int32 inFd,
    /* [out] */ Int64* inOffset,
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* result)
{
    return NOERROR;
}

ECode Posix::Setegid(
    /* [in] */ Int32 egid)
{
    return NOERROR;
}

ECode Posix::Setenv(
    /* [in] */ String name,
    /* [in] */ String value,
    /* [in] */ Boolean overwrite)
{
    return NOERROR;
}

ECode Posix::Seteuid(
    /* [in] */ Int32 euid)
{
    return NOERROR;
}

ECode Posix::Setgid(
    /* [in] */ Int32 gid)
{
    return NOERROR;
}

ECode Posix::Setsid(
    /* [out] */ Int32* sid)
{
    return NOERROR;
}

ECode Posix::SetsockoptByte(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ Int32 value)
{
    return NOERROR;
}

ECode Posix::SetsockoptIfreq(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ String interfaceName)
{
    return NOERROR;
}

ECode Posix::SetsockoptInt(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ Int32 value)
{
    return NOERROR;
}

ECode Posix::SetsockoptIpMreqn(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ Int32 value)
{
    return NOERROR;
}

ECode Posix::SetsockoptGroupReq(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ IStructGroupReq* value)
{
    return NOERROR;
}

ECode Posix::SetsockoptGroupSourceReq(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ IStructGroupSourceReq* value)
{
    return NOERROR;
}

ECode Posix::SetsockoptLinger(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ IStructLinger* value)
{
    return NOERROR;
}

ECode Posix::SetsockoptTimeval(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ IStructTimeval* value)
{
    return NOERROR;
}

ECode Posix::Setuid(
    /* [in] */ Int32 uid)
{
    return NOERROR;
}

ECode Posix::Shutdown(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 how)
{
    return NOERROR;
}

ECode Posix::Socket(
    /* [in] */ Int32 socketDomain,
    /* [in] */ Int32 type,
    /* [in] */ Int32 protocol,
    /* [out] */ IFileDescriptor** fd)
{
    return NOERROR;
}

ECode Posix::Socketpair(
    /* [in] */ Int32 socketDomain,
    /* [in] */ Int32 type,
    /* [in] */ Int32 protocol,
    /* [out] */ IFileDescriptor** fd1,
    /* [out] */ IFileDescriptor** fd2)
{
    return NOERROR;
}

ECode Posix::Stat(
    /* [in] */ String path,
    /* [out] */ IStructStat** stat)
{
    return NOERROR;
}

    /* TODO: replace statfs with statvfs. */
ECode Posix::StatVfs(
    /* [in] */ String path,
    /* [out] */ IStructStatVfs** statfs)
{
    return NOERROR;
}

ECode Posix::Strerror(
    /* [in] */ Int32 errno,
    /* [out] */ String* strerr)
{
    return NOERROR;
}

ECode Posix::Symlink(
    /* [in] */ String oldPath,
    /* [in] */ String newPath)
{
    return NOERROR;
}

ECode Posix::Sysconf(
    /* [in] */ Int32 name,
    /* [out] */ Int64* result)
{
    return NOERROR;
}

ECode Posix::Tcdrain(
    /* [in] */ IFileDescriptor* fd)
{
    return NOERROR;
}

ECode Posix::Umask(
    /* [in] */ Int32 mask,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

ECode Posix::Uname(
    /* [out] */ IStructUtsname** uname)
{
    return NOERROR;
}

ECode Posix::Waitpid(
    /* [in] */ Int32 pid,
    /* [out] */ Int32* status,
    /* [in] */ Int32 options,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

ECode Posix::Write(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* num)
{
    return NOERROR;
}

ECode Posix::Write(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IByteBuffer* buffer,
    /* [out] */ Int32* num)
{
    return NOERROR;
}

ECode Posix::Writev(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<IInterface*>* buffers,
    /* [in] */ ArrayOf<Int32>* offsets,
    /* [in] */ ArrayOf<Int32>* byteCounts,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

} // namespace IO
} // namespace Libcore
