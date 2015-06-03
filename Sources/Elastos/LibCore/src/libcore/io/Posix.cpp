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
#include "droid/system/CStructPasswd.h"
#include "droid/system/CStructStat.h"
#include "droid/system/CStructLinger.h"
#include "droid/system/CStructTimeval.h"
#include "droid/system/CStructUcred.h"
#include "droid/system/CStructStatVfs.h"
#include "droid/system/CStructUtsname.h"
#include "Portability.h"
#include "UniquePtr.h"

using Elastos::Core::Object;
using Elastos::IO::CFileDescriptor;
using Elastos::Droid::System::CStructPasswd;
using Elastos::Droid::System::CStructStat;
using Elastos::Droid::System::CStructLinger;
using Elastos::Droid::System::CStructTimeval;
using Elastos::Droid::System::CStructUcred;
using Elastos::Droid::System::CStructStatVfs;
using Elastos::Droid::System::CStructUtsname;

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
        return_value = E_IO_EXCEPTION; \
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
static rc_t ErrorIfMinusOne(const char* name, rc_t rc, ECode* ec) {
    *ec = NOERROR;
    if (rc == rc_t(-1)) {
        ALOGE("System-call error: %s", name);
        *ec = E_ERRNO_EXCEPTION;
    }
    return rc;
}

namespace Libcore {
namespace IO {

static AutoPtr<ISocketAddress> MakeSocketAddress(
    /* [in] */ const sockaddr_storage& ss)
{
    Int32 port;
    AutoPtr<IInetAddress> inetAddress = SockaddrToInetAddress(ss, &port);
    if (inetAddress == NULL) {
        return NULL;
    }
    AutoPtr<ISocketAddress> rst;
    // CInetSocketAddress::New(inetAddress, port, (ISocketAddress**)&rst);
    return rst;
}

static AutoPtr<IStructPasswd> MakeStructPasswd(
    /* [in] */ const struct passwd& pw)
{
    String pw_name = String(pw.pw_name);
    String pw_dir = String(pw.pw_dir);
    String pw_shell = String(pw.pw_shell);

    AutoPtr<IStructPasswd> rst;
    CStructPasswd::New(
        pw_name,
        static_cast<Int32>(pw.pw_uid),
        static_cast<Int32>(pw.pw_gid),
        pw_dir,
        pw_shell,
        (IStructPasswd**)&rst);
    return rst;
}

static AutoPtr<IStructStat> MakeStructStat(
    /* [in] */ const struct stat& sb) {
    AutoPtr<IStructStat> rst;
    CStructStat::New(
        static_cast<Int32>(sb.st_dev), static_cast<Int32>(sb.st_ino),
        static_cast<Int32>(sb.st_mode), static_cast<Int32>(sb.st_nlink),
        static_cast<Int32>(sb.st_uid), static_cast<Int32>(sb.st_gid),
        static_cast<Int32>(sb.st_rdev), static_cast<Int32>(sb.st_size),
        static_cast<Int32>(sb.st_atime), static_cast<Int32>(sb.st_mtime),
        static_cast<Int32>(sb.st_ctime), static_cast<Int32>(sb.st_blksize),
        static_cast<Int32>(sb.st_blocks), (IStructStat**)&rst);
    return rst;
}

static AutoPtr<IStructStatVfs> MakeStructStatVfs(
    /* [in] */ const struct ::statvfs& sb) {
#if defined(__APPLE__)
    // Mac OS has no f_namelen field in struct statfs.
    Int64 max_name_length = 255; // __DARWIN_MAXNAMLEN
#else
    Int64 max_name_length = static_cast<Int64>(sb.f_namemax);
#endif
    AutoPtr<IStructStatVfs> rst;
    CStructStatVfs::New(
        static_cast<Int32>(sb.f_bsize),
        static_cast<Int32>(sb.f_frsize),
        static_cast<Int32>(sb.f_blocks),
        static_cast<Int32>(sb.f_bfree),
        static_cast<Int32>(sb.f_bavail),
        static_cast<Int32>(sb.f_files),
        static_cast<Int32>(sb.f_ffree),
        static_cast<Int32>(sb.f_favail),
        static_cast<Int32>(sb.f_fsid),
        static_cast<Int32>(sb.f_flag),
        max_name_length, (IStructStatVfs**)&rst);
    return rst;
}

static AutoPtr<IStructLinger> MakeStructLinger(
    /* [in] */ const struct linger& l)
{
    AutoPtr<IStructLinger> rst;
    CStructLinger::New(l.l_onoff, l.l_linger, (IStructLinger**)&rst);
    return rst;
}

static AutoPtr<IStructTimeval> MakeStructTimeval(
    /* [in] */ const struct timeval& tv)
{
    AutoPtr<IStructTimeval> rst;
    CStructTimeval::New(static_cast<Int32>(tv.tv_sec), static_cast<Int32>(tv.tv_usec), (IStructTimeval**)&rst);
    return NOERROR;
}

static AutoPtr<IStructUcred> MakeStructUcred(
    /* [in] */ const struct ucred& u __unused)
{
#ifdef __APPLE__
    ALOGE("unimplemented support for ucred on a Mac");
    return NULL;
#else
    AutoPtr<IStructUcred> rst;
    CStructUcred::New(u.pid, u.uid, u.gid, (IStructUcred**)&rst);
    return rst;
#endif
}

static AutoPtr<IStructUtsname> MakeStructUtsname(
    /* [in] */ const struct utsname& buf)
{
    String sysname = String(buf.sysname);
    String nodename = String(buf.nodename);
    String release = String(buf.release);
    String version = String(buf.version);
    String machine = String(buf.machine);

    AutoPtr<IStructUtsname> rst;
    CStructUtsname::New(sysname, nodename, release, version, machine, (IStructUtsname**)&rst);
    return rst;
};


static AutoPtr<ISocketAddress> DoGetSockName(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Boolean is_sockname)
{
  Int32 _fd;
  fd->GetDescriptor(&_fd);
  sockaddr_storage ss;
  sockaddr* sa = reinterpret_cast<sockaddr*>(&ss);
  socklen_t byteCount = sizeof(ss);
  memset(&ss, 0, byteCount);
  Int32 rc = is_sockname ? TEMP_FAILURE_RETRY(getsockname(_fd, sa, &byteCount))
      : TEMP_FAILURE_RETRY(getpeername(_fd, sa, &byteCount));
  if (rc == -1) {
    // throwErrnoException(env, is_sockname ? "getsockname" : "getpeername");
    if (is_sockname) {
        ALOGE("System-call getsockname error, errno = %d",errno);
    } else {
        ALOGE("System-call getpeername error, errno = %d",errno);
    }
    return NULL;
  }
  return MakeSocketAddress(ss);
}

static Boolean FillIfreq(
    /* [in] */ const String& interfaceName,
    /* [in] */ struct ifreq& req) {
    if (interfaceName == NULL) {
        return FALSE;
    }
    memset(&req, 0, sizeof(req));
    strncpy(req.ifr_name, interfaceName, sizeof(req.ifr_name));
    req.ifr_name[sizeof(req.ifr_name) - 1] = '\0';
    return TRUE;
}

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

class Passwd {
public:
    Passwd() : mResult(NULL) {
        mBufferSize = sysconf(_SC_GETPW_R_SIZE_MAX);
        mBuffer.reset(new char[mBufferSize]);
    }

    AutoPtr<IStructPasswd> getpwnam(const char* name) {
        return process("getpwnam_r", getpwnam_r(name, &mPwd, mBuffer.get(), mBufferSize, &mResult));
    }

    AutoPtr<IStructPasswd> getpwuid(uid_t uid) {
        return process("getpwuid_r", getpwuid_r(uid, &mPwd, mBuffer.get(), mBufferSize, &mResult));
    }

    struct passwd* get() {
        return mResult;
    }

private:
    AutoPtr<IStructPasswd> process(const char* syscall, int error) {
        if (mResult == NULL) {
            errno = error;
            // throwErrnoException(mEnv, syscall);
            ALOGE("System-call %s error, errno = %d", syscall, errno);
            return NULL;
        }
        return MakeStructPasswd(*mResult);
    }

    UniquePtr<char[]> mBuffer;
    size_t mBufferSize;
    struct passwd mPwd;
    struct passwd* mResult;
};

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
    UniquePtr<addrinfo, addrinfo_deleter> addressListDeleter(addressList);
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
    ECode ec;
    ErrorIfMinusOne("chmod", TEMP_FAILURE_RETRY(chmod(path, mode)), &ec);
    return ec;
}

ECode Posix::Chown(
    /* [in] */ String path,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    if (path == NULL) {
        return NOERROR;
    }
    ECode ec;
    ErrorIfMinusOne("chown", TEMP_FAILURE_RETRY(chown(path, uid, gid)), &ec);
    return ec;
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
    ECode ec;
    ErrorIfMinusOne("close", close(_fd), &ec);
    return ec;
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
    /* [in] */ IFileDescriptor* oldFd,
    /* [out] */ IFileDescriptor** retFd)
{
    Int32 _oldFd;
    oldFd->GetDescriptor(&_oldFd);
    ECode ec;
    Int32 newFd = ErrorIfMinusOne("dup", TEMP_FAILURE_RETRY(dup(_oldFd)), &ec);
    *retFd = NULL;
    if (newFd != -1) {
        CFileDescriptor::New(retFd);
        (*retFd)->SetDescriptor(newFd);
    }
    return ec;
}

ECode Posix::Dup2(
    /* [in] */ IFileDescriptor* oldFd,
    /* [in] */ Int32 newFd,
    /* [out] */ IFileDescriptor** retFd)
{
    Int32 _oldFd;
    oldFd->GetDescriptor(&_oldFd);
    ECode ec;
    Int32 fd = ErrorIfMinusOne("dup2", TEMP_FAILURE_RETRY(dup2(_oldFd, newFd)), &ec);
    *retFd = NULL;
    if (fd != -1) {
        CFileDescriptor::New(retFd);
        (*retFd)->SetDescriptor(fd);
    }
    return ec;
}

ECode Posix::Environ(
    /* [out, callee] */ ArrayOf<String>** env)
{
    extern char** environ; // Standard, but not in any header file.
    Int32 size = 0;
    while(*environ) size++;
    AutoPtr<ArrayOf<String> > envTmp = ArrayOf<String>::Alloc(size);
    for (Int32 i = 0; i < size; i++) {
        String str = String(environ[i]);
        envTmp->Set(i, str);
    }
    *env = envTmp;
    REFCOUNT_ADD(*env);
    return NOERROR;
}

ECode Posix::Execv(
    /* [in] */ String filename,
    /* [in] */ ArrayOf<String>* argv)
{
    if (filename == NULL) {
        return NOERROR;
    }

    Int32 length = argv->GetLength();
    char const ** array_ = new char const *[length + 1];
    array_[length] = NULL;
    for (Int32 i = 0; i < length; i++) {
        array_[i] = (*argv)[i];
    }
    execv(filename, array_);

    ALOGE("execv returned errno = %d", errno);
    return E_ERRNO_EXCEPTION;
    // throwErrnoException(env, "execv");
}

ECode Posix::Execve(
    /* [in] */ String filename,
    /* [in] */ ArrayOf<String>* argv,
    /* [in] */ ArrayOf<String>* envp)
{
    if (filename == NULL) {
        return NOERROR;
    }

    Int32 argvLength = argv->GetLength();
    char const** argvArray_ = new char const*[argvLength + 1];
    argvArray_[argvLength] = NULL;
    for (Int32 i = 0; i < argvLength; i++) {
        argvArray_[i] = (*argv)[i];
    }

    Int32 envpLength = envp->GetLength();
    char const** envpArray_ = new char const*[envpLength + 1];
    envpArray_[envpLength] = NULL;
    for (Int32 i = 0; i < envpLength; i++) {
        envpArray_[i] = (*envp)[i];
    }

    execve(filename, argvArray_, envpArray_);

    ALOGE("execve returned errno = %d", errno);
    return E_ERRNO_EXCEPTION;
    // return NOERROR;
}

ECode Posix::Fchmod(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 mode)
{
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    ECode ec;
    ErrorIfMinusOne("fchmod", TEMP_FAILURE_RETRY(fchmod(_fd, mode)), &ec);
    return ec;
}

ECode Posix::Fchown(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    ECode ec;
    ErrorIfMinusOne("fchown", TEMP_FAILURE_RETRY(fchown(_fd, uid, gid)), &ec);
    return ec;
}

ECode Posix::FcntlVoid(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 cmd,
    /* [out] */ Int32* result)
{
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    ECode ec;
    ErrorIfMinusOne("fcntl", TEMP_FAILURE_RETRY(fcntl(_fd, cmd)), &ec);
    return ec;
}

ECode Posix::FcntlInt64(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 cmd,
    /* [in] */ Int64 arg,
    /* [out] */ Int32* result)
{
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    ECode ec;
    ErrorIfMinusOne("fcntl", TEMP_FAILURE_RETRY(fcntl(_fd, cmd, arg)), &ec);
    return ec;
}

ECode Posix::FcntlFlock(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 cmd,
    /* [in] */ IStructFlock* flock,
    /* [out] */ Int32* result)
{
    struct flock64 lock;
    memset(&lock, 0, sizeof(lock));
    Int16 typeTmp, whenceTmp;
    Int64 startTmp, lenTmp;
    Int32 pidTmp;
    flock->GetType(&typeTmp);
    flock->GetWhence(&whenceTmp);
    flock->GetStart(&startTmp);
    flock->GetLen(&lenTmp);
    flock->GetPid(&pidTmp);
    lock.l_type = typeTmp;
    lock.l_whence = whenceTmp;
    lock.l_start = startTmp;
    lock.l_len = lenTmp;
    lock.l_pid = pidTmp;

    ECode ec;
    Int32 rc = IO_FAILURE_RETRY(Int32, ec, fcntl, fd, cmd, &lock);
    if (rc != -1) {
        flock->SetType(lock.l_type);
        flock->SetWhence(lock.l_whence);
        flock->SetStart(lock.l_start);
        flock->SetLen(lock.l_len);
        flock->SetPid(lock.l_pid);
    }
    *result = rc;
    return ec;
}

ECode Posix::Fdatasync(
    /* [in] */ IFileDescriptor* fd)
{
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    ECode ec;
    ErrorIfMinusOne("fdatasync", TEMP_FAILURE_RETRY(fdatasync(_fd)), &ec);
    return ec;
}

ECode Posix::Fstat(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ IStructStat** statout)
{
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    struct stat sb;
    Int32 rc = TEMP_FAILURE_RETRY(fstat(_fd, &sb));
    if (rc == -1) {
        // throwErrnoException(env, "fstat");
        ALOGE("System-call fstat error: errno = %d", errno);
        *statout = NULL;
        return E_ERRNO_EXCEPTION;
    }
    *statout = MakeStructStat(sb);
    REFCOUNT_ADD(*statout)
    return NOERROR;
}

ECode Posix::Fstatvfs(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ IStructStatVfs** statFs)
{
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    struct statvfs sb;
    Int32 rc = TEMP_FAILURE_RETRY(fstatvfs(_fd, &sb));
    if (rc == -1) {
        // throwErrnoException(env, "fstatvfs");
        ALOGE("System-call fstatvfs error: errno = %d", errno);
        *statFs = NULL;
        return E_ERRNO_EXCEPTION;
    }
    *statFs = MakeStructStatVfs(sb);
    REFCOUNT_ADD(*statFs)
    return NOERROR;
}

ECode Posix::Fsync(
    /* [in] */ IFileDescriptor* fd)
{
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    ECode ec;
    ErrorIfMinusOne("fsync", TEMP_FAILURE_RETRY(fsync(_fd)), &ec);
    return ec;
}

ECode Posix::Ftruncate(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 length)
{
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    ECode ec;
    ErrorIfMinusOne("ftruncate", TEMP_FAILURE_RETRY(ftruncate64(_fd, length)), &ec);
    return NOERROR;
}

ECode Posix::Gai_strerror(
    /* [in] */ Int32 error,
    /* [out] */ String* strerror)
{
    *strerror = gai_strerror(error);
    return NOERROR;
}

ECode Posix::Getegid(
    /* [out] */ Int32* egid)
{
    *egid = getegid();
    return NOERROR;
}

ECode Posix::Geteuid(
    /* [out] */ Int32* euid)
{
    *euid = geteuid();
    return NOERROR;
}

ECode Posix::Getgid(
    /* [out] */ Int32* gid)
{
    *gid = getgid();
    return NOERROR;
}

ECode Posix::Getenv(
    /* [in] */ String name,
    /* [out] */ String* env)
{
    if (name == NULL) {
        *env = NULL;
        return NOERROR;
    }
    *env = getenv(name);
    return NOERROR;
}

    /* TODO: break into getnameinfoHost and getnameinfoService? */
ECode Posix::Getnameinfo(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 flags,
    /* [out] */ String* nameinfo)
{
    sockaddr_storage ss;
    socklen_t sa_len;
    if (!InetAddressToSockaddrVerbatim(address, 0, ss, sa_len)) {
        *nameinfo = NULL;
        return NOERROR;
    }
    char buf[NI_MAXHOST]; // NI_MAXHOST is longer than INET6_ADDRSTRLEN.
    errno = 0;
    Int32 rc = getnameinfo(reinterpret_cast<sockaddr*>(&ss), sa_len, buf, sizeof(buf), NULL, 0, flags);
    if (rc != 0) {
        // throwGaiException(env, "getnameinfo", rc);
        ALOGE("System-call getnameinfo error , errno = %d", errno);
        *nameinfo = NULL;
        return E_GAI_EXCEPTION;
    }
    *nameinfo = String(buf);
    return NOERROR;
}

ECode Posix::Getpeername(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ ISocketAddress** peername)
{
    *peername = DoGetSockName(fd, FALSE);
    REFCOUNT_ADD(*peername)
    return NOERROR;
}

ECode Posix::Getpid(
    /* [out] */ Int32* pid)
{
    *pid = getpid();
    return NOERROR;
}

ECode Posix::Getppid(
    /* [out] */ Int32* ppid)
{
    *ppid = getppid();
    return NOERROR;
}

ECode Posix::Getpwnam(
    /* [in] */ String name,
    /* [out] */ IStructPasswd** pwnam)
{
    if (name == NULL) {
        *pwnam = NULL;
    }
    *pwnam = Passwd().getpwnam(name);
    REFCOUNT_ADD(*pwnam)
    return NOERROR;
}

ECode Posix::Getpwuid(
    /* [in] */ Int32 uid,
    /* [out] */ IStructPasswd** pwuid)
{
    *pwuid = Passwd().getpwuid(uid);
    REFCOUNT_ADD(*pwuid)
    return NOERROR;
}

ECode Posix::Getsockname(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ ISocketAddress** sockname)
{
    *sockname = DoGetSockName(fd, TRUE);
    REFCOUNT_ADD(*sockname)
    return NOERROR;
}

ECode Posix::GetsockoptByte(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ Int32* sockopt)
{
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    u_char result = 0;
    socklen_t size = sizeof(result);
    ECode ec;
    ErrorIfMinusOne("getsockopt", TEMP_FAILURE_RETRY(getsockopt(_fd, level, option, &result, &size)), &ec);
    *sockopt = result;
    return ec;
}

ECode Posix::GetsockoptInAddr(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IInetAddress** addr)
{
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    sockaddr_storage ss;
    memset(&ss, 0, sizeof(ss));
    ss.ss_family = AF_INET; // This is only for the IPv4-only IP_MULTICAST_IF.
    sockaddr_in* sa = reinterpret_cast<sockaddr_in*>(&ss);
    socklen_t size = sizeof(sa->sin_addr);
    Int32 rc = TEMP_FAILURE_RETRY(getsockopt(_fd, level, option, &sa->sin_addr, &size));
    if (rc == -1) {
        // throwErrnoException(env, "getsockopt");
        ALOGE("System-call getsockopt error, errno = %d", errno);
        *addr = NULL;
        return E_ERRNO_EXCEPTION;
    }
    *addr = SockaddrToInetAddress(ss, NULL);
    REFCOUNT_ADD(*addr)
    return NOERROR;
}

ECode Posix::GetsockoptInt32(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ Int32* sockopt)
{
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    Int32 result = 0;
    socklen_t size = sizeof(result);
    ECode ec;
    ErrorIfMinusOne("getsockopt", TEMP_FAILURE_RETRY(getsockopt(_fd, level, option, &result, &size)), &ec);
    *sockopt = result;
    return NOERROR;
}

ECode Posix::GetsockoptLinger(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IStructLinger** linger)
{
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    struct linger l;
    socklen_t size = sizeof(l);
    memset(&l, 0, size);
    Int32 rc = TEMP_FAILURE_RETRY(getsockopt(_fd, level, option, &l, &size));
    if (rc == -1) {
        // throwErrnoException(env, "getsockopt");
        ALOGE("System-call getsockopt error, errno = %d", errno);
        *linger = NULL;
        return E_ERRNO_EXCEPTION;
    }
    *linger = MakeStructLinger(l);
    REFCOUNT_ADD(*linger)
    return NOERROR;
}

ECode Posix::GetsockoptTimeval(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IStructTimeval** timeval)
{
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    struct timeval tv;
    socklen_t size = sizeof(tv);
    memset(&tv, 0, size);
    Int32 rc = TEMP_FAILURE_RETRY(getsockopt(_fd, level, option, &tv, &size));
    if (rc == -1) {
        // throwErrnoException(env, "getsockopt");
        ALOGE("System-call getsockopt error, errno = %d", errno);
        *timeval = NULL;
        return E_ERRNO_EXCEPTION;
    }
    *timeval = MakeStructTimeval(tv);
    REFCOUNT_ADD(*timeval)
    return NOERROR;
}

ECode Posix::GetsockoptUcred(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IStructUcred** ucred)
{
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    struct ucred u;
    socklen_t size = sizeof(u);
    memset(&u, 0, size);
    Int32 rc = TEMP_FAILURE_RETRY(getsockopt(_fd, level, option, &u, &size));
    if (rc == -1) {
        // throwErrnoException(env, "getsockopt");
        ALOGE("System-call getsockopt error, errno = %d", errno);
        *ucred = NULL;
        return E_ERRNO_EXCEPTION;
    }
    *ucred = MakeStructUcred(u);
    REFCOUNT_ADD(*ucred)
    return NOERROR;
}

ECode Posix::Gettid(
    /* [out] */ Int32* tid)
{
#if defined(__APPLE__)
    uint64_t owner;
    int rc = pthread_threadid_np(NULL, &owner);  // Requires Mac OS 10.6
    if (rc != 0) {
        // throwErrnoException(env, "gettid");
        ALOGE("System-call gettid error, errno = %d", errno);
        *tid = 0;
        return E_ERRNO_EXCEPTION;
    }
    *tid = static_cast<Int32>(owner);
    return NOERROR;
#else
    // Neither bionic nor glibc exposes gettid(2).
    *tid = syscall(__NR_gettid);
    return NOERROR;
#endif
}

ECode Posix::Getuid(
    /* [out] */ Int32* uid)
{
    *uid =getuid();
    return NOERROR;
}

ECode Posix::If_indextoname(
    /* [in] */ Int32 index,
    /* [out] */ String* name)
{
    char buf[IF_NAMESIZE];
    char* _name = if_indextoname(index, buf);
    // if_indextoname(3) returns NULL on failure, which will come out of NewStringUTF unscathed.
    // There's no useful information in errno, so we don't bother throwing. Callers can null-check.
    *name = _name;
    return NOERROR;
}

ECode Posix::Inet_pton(
    /* [in] */ Int32 family,
    /* [in] */ String name,
    /* [out] */ IInetAddress** addr)
{
    if (name == NULL) {
        return NULL;
    }
    sockaddr_storage ss;
    memset(&ss, 0, sizeof(ss));
    // sockaddr_in and sockaddr_in6 are at the same address, so we can use either here.
    void* dst = &reinterpret_cast<sockaddr_in*>(&ss)->sin_addr;
    if (inet_pton(family, name, dst) != 1) {
        *addr = NULL;
        return NOERROR;
    }
    ss.ss_family = family;
    *addr = SockaddrToInetAddress(ss, NULL);
    REFCOUNT_ADD(*addr)
    return NOERROR;
}

ECode Posix::IoctlInetAddress(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 cmd,
    /* [in] */ String interfaceName,
    /* [out] */ IInetAddress** addr)
{
    struct ifreq req;
    if (!FillIfreq(interfaceName, req)) {
        *addr = NULL;
        return NOERROR;
    }
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    ECode ec;
    Int32 rc = ErrorIfMinusOne("ioctl", TEMP_FAILURE_RETRY(ioctl(_fd, cmd, &req)), &ec);
    if (rc == -1) {
        *addr = NULL;
        return ec;
    }
    *addr = SockaddrToInetAddress(reinterpret_cast<sockaddr_storage&>(req.ifr_addr), NULL);
    REFCOUNT_ADD(*addr)
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
    /* [in] */ IFileDescriptor* outFd,
    /* [in] */ IFileDescriptor* inFd,
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
