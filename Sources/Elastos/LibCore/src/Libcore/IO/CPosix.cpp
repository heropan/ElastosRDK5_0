
#include "cmdef.h"
#include "CPosix.h"
#include "AsynchronousSocketCloseMonitor.h"
#include "ToStringArray.h"
#include "CStructLinger.h"
#include "CStructPasswd.h"
#include "CStructStat.h"
#include "CStructStatFs.h"
#include "CStructTimeval.h"
#include "CStructUtsname.h"
#include "InetAddress.h"
#include "CInet4Address.h"
#include "CInet6Address.h"
#include "CInetSocketAddress.h"
#include "NioUtils.h"
#include "elastos/UniquePtr.h"
#include "elastos/Vector.h"

#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <pwd.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <sys/vfs.h> // Bionic doesn't have <sys/statvfs.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

using Elastos::Core::UniquePtr;
using Elastos::Utility::Vector;
using Elastos::Net::InetAddress;
using Elastos::Net::IInet4Address;
using Elastos::Net::IInet6Address;
using Elastos::Net::CInet4Address;
using Elastos::Net::CInet6Address;
using Elastos::Net::CInetSocketAddress;
using Elastos::Net::EIID_InetAddress;
using Elastos::IO::NioUtils;
using Elastos::IO::EIID_HeapByteBuffer;
using Elastos::IO::HeapByteBuffer;
using Elastos::IO::EIID_ByteBuffer;
using Elastos::IO::ByteBuffer;

extern char** environ; // Standard, but not in any header file.

namespace Libcore {
namespace IO {

#define BUFSIZ 1024

/**
 * Similar to C++0x's static_assert. Message argument must be a valid identifier, not a string.
 * Called COMPILE_ASSERT in Google, COMPILE_TIME_ASSERT in other places. This is the usual Google
 * implementation.
 */
#define STATIC_ASSERT(exp, msg) typedef StaticAssert<(Boolean(exp))> msg[Boolean(exp) ? 1 : -1]
template<Boolean> struct StaticAssert {
};

AutoPtr<IInetAddress> SockaddrToInetAddress(
    /* [in] */ const sockaddr_storage* ss,
    /* [in] */ Int32* port)
{
    // Convert IPv4-mapped IPv6 addresses to IPv4 addresses.
    // The RI states "Java will never return an IPv4-mapped address".
    sockaddr_storage tmp;
    memset(&tmp, 0, sizeof(tmp));
    const sockaddr_in6* sin6 = reinterpret_cast<const sockaddr_in6*>(ss);
    if (ss->ss_family == AF_INET6 && IN6_IS_ADDR_V4MAPPED(&sin6->sin6_addr)) {
        // Copy the IPv6 address into the temporary sockaddr_storage.
        memcpy(&tmp, ss, sizeof(sockaddr_in6));
        // Unmap it into an IPv4 address.
        sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(&tmp);
        sin->sin_family = AF_INET;
        sin->sin_port = sin6->sin6_port;
        memcpy(&sin->sin_addr.s_addr, &sin6->sin6_addr.s6_addr[12], 4);
        // Fall through into the regular conversion using the unmapped address.
        ss = &tmp;
    }

    const void* rawAddress;
    size_t addressLength;
    int sin_port;
    int scope_id = 0;
    if (ss->ss_family == AF_INET) {
        const sockaddr_in* sin = reinterpret_cast<const sockaddr_in*>(ss);
        rawAddress = &sin->sin_addr.s_addr;
        addressLength = 4;
        sin_port = ntohs(sin->sin_port);
    }
    else if (ss->ss_family == AF_INET6) {
        const sockaddr_in6* sin6 = reinterpret_cast<const sockaddr_in6*>(ss);
        rawAddress = &sin6->sin6_addr.s6_addr;
        addressLength = 16;
        sin_port = ntohs(sin6->sin6_port);
        scope_id = sin6->sin6_scope_id;
    }
    else {
        //  // We can't throw SocketException. We aren't meant to see bad addresses, so seeing one
        //  // really does imply an internal error.
        // jniThrowExceptionFmt(env, "java/lang/IllegalArgumentException",
        //         "sockaddrToInetAddress bad ss_family: %i", ss->ss_family);
        return NULL;
    }
    if (port != NULL) {
        *port = sin_port;
    }

    AutoPtr<ArrayOf<Byte> > byteArray = ArrayOf<Byte>::Alloc(addressLength);
    if (byteArray.Get() == NULL) {
        return NULL;
    }
    memcpy(byteArray->GetPayload(), reinterpret_cast<const Byte*>(rawAddress),
            addressLength);

    AutoPtr<IInetAddress> inetAddress;
    InetAddress::GetByAddress(String(NULL), byteArray.Get(), scope_id,
            (IInetAddress**)&inetAddress);
    return inetAddress;
}

static Boolean InetAddressToSockaddr(
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [in] */ sockaddr_storage* ss,
    /* [in] */ Boolean map)
{
    memset(ss, 0, sizeof(*ss));

    if (inetAddress == NULL) {
        // jniThrowNullPointerException(env, NULL);
        return FALSE;
    }

    // Get the address family.
    ss->ss_family = ((InetAddress*)inetAddress->Probe(EIID_InetAddress))->mFamily;
    if (ss->ss_family == AF_UNSPEC) {
        return TRUE; // Job done!
    }

    // Check this is an address family we support.
    if (ss->ss_family != AF_INET && ss->ss_family != AF_INET6) {
        // jniThrowExceptionFmt(env, "java/lang/IllegalArgumentException",
        //         "inetAddressToSockaddr bad family: %i", ss->ss_family);
        return FALSE;
    }

    // Get the byte array that stores the IP address bytes in the InetAddress.
    AutoPtr<ArrayOf<Byte> > addressBytes = ((InetAddress*)inetAddress->Probe(EIID_InetAddress))->mIpAddress;
    if (addressBytes.Get() == NULL) {
        // jniThrowNullPointerException(env, NULL);
        return FALSE;
    }

    // We use AF_INET6 sockets, so we want an IPv6 address (which may be a IPv4-mapped address).
    sockaddr_in6* sin6 = reinterpret_cast<sockaddr_in6*>(ss);
    sin6->sin6_port = htons(port);
    if (ss->ss_family == AF_INET6) {
        // IPv6 address. Copy the bytes...
        Byte* dst = reinterpret_cast<Byte*>(&sin6->sin6_addr.s6_addr);
        memcpy(dst, addressBytes->GetPayload(), 16);
        // ...and set the scope id...
        AutoPtr<IInet6Address> res6add = IInet6Address::Probe(inetAddress);
        if (res6add == NULL) {
            return FALSE;
        }
        sin6->sin6_scope_id = ((CInet6Address*)res6add.Get())->mScopeId;
        return TRUE;
    }

    // Deal with Inet4Address instances.
    if (map) {
        // We should represent this Inet4Address as an IPv4-mapped IPv6 sockaddr_in6.
        // Change the family...
        sin6->sin6_family = AF_INET6;
        // Copy the bytes...
        Byte* dst = reinterpret_cast<Byte*>(&sin6->sin6_addr.s6_addr[12]);
        memcpy(dst, addressBytes->GetPayload(), 4);
        // INADDR_ANY and in6addr_any are both all-zeros...
        if (!IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            // ...but all other IPv4-mapped addresses are ::ffff:a.b.c.d, so insert the ffff...
            memset(&(sin6->sin6_addr.s6_addr[10]), 0xff, 2);
        }
    }
    else {
        // We should represent this Inet4Address as an IPv4 sockaddr_in.
        sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(ss);
        sin->sin_port = htons(port);
        Byte* dst = reinterpret_cast<Byte*>(&sin->sin_addr.s_addr);
        memcpy(dst, addressBytes->GetPayload(), 4);
    }
    return TRUE;
}

Boolean InetAddressToSockaddrVerbatim(
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [in] */ sockaddr_storage* ss)
{
    return InetAddressToSockaddr(inetAddress, port, ss, FALSE);
}

Boolean InetAddressToSockaddr(
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [in] */ sockaddr_storage* ss)
{
    return InetAddressToSockaddr(inetAddress, port, ss, TRUE);
}

struct addrinfo_deleter {
    void operator()(addrinfo* p) const {
        if (p != NULL) { // bionic's freeaddrinfo(3) crashes when passed NULL.
            freeaddrinfo(p);
        }
    }
};

/**
 * Used to retry networking system calls that can return EINTR. Unlike TEMP_FAILURE_RETRY,
 * this also handles the case where the reason for failure is that another thread called
 * Socket.close. This macro also throws exceptions on failure.
 *
 * Returns the result of 'exp', though a Java exception will be pending if the result is -1.
 */
#define NET_FAILURE_RETRY(return_type, syscall_name, fd, ...) ({ \
    return_type _rc = -1; \
    do { \
        { \
            AsynchronousSocketCloseMonitor _monitor(fd); \
            _rc = syscall_name(fd, __VA_ARGS__); \
        } \
        if (_rc == -1) { \
            if (fd == -1) { \
                break; \
            } \
            else if (errno != EINTR) { \
                /* TODO: with a format string we could show the arguments too, like strace(1). */ \
                break; \
            } \
        } \
    } while (_rc == -1); \
    _rc; })

const char* StrError(
    /* [in] */ Int32 errnum,
    /* [in] */ char* buf,
    /* [in] */ size_t buflen)
{
    // Note: glibc has a nonstandard strerror_r that returns char* rather than POSIX's int.
    // char *strerror_r(int errnum, char *buf, size_t n);
    char* ret = (char*) strerror_r(errnum, buf, buflen);
    if (((Int32) ret) == 0) {
        // POSIX strerror_r, success
        return buf;
    } else if (((Int32) ret) == -1) {
        // POSIX strerror_r, failure
        // (Strictly, POSIX only guarantees a value other than 0. The safest
        // way to implement this function is to use C++ and overload on the
        // type of strerror_r to accurately distinguish GNU from POSIX. But
        // realistic implementations will always return -1.)
        snprintf(buf, buflen, "errno %d", errnum);
        return buf;
    } else {
        // glibc strerror_r returning a string
        return ret;
    }
}

static AutoPtr<ISocketAddress> MakeSocketAddress(
    /* [in] */ const sockaddr_storage* ss)
{
    // TODO: support AF_UNIX and AF_UNSPEC (and other families?)
    Int32 port;
    AutoPtr<IInetAddress> inetAddress = SockaddrToInetAddress(ss,
            &port);
    if (inetAddress == NULL) {
        return NULL;
    }
    AutoPtr<CInetSocketAddress> netsockaddr;
    CInetSocketAddress::NewByFriend(inetAddress, port,
            (CInetSocketAddress**)&netsockaddr);
    AutoPtr<ISocketAddress> sockaddr = ISocketAddress::Probe(netsockaddr);

    return sockaddr;
}

static AutoPtr<IStructPasswd> MakeStructPasswd(
    /* [in] */ const struct passwd& pw)
{
    String pw_name(pw.pw_name);
    String pw_dir(pw.pw_dir);
    String pw_shell(pw.pw_shell);
    AutoPtr<CStructPasswd> obj;
    CStructPasswd::NewByFriend(pw_name, static_cast<Int32>(pw.pw_uid),
            static_cast<Int32>(pw.pw_gid), pw_dir, pw_shell,
            (CStructPasswd**)&obj);
    return (IStructPasswd*)obj;
}

static AutoPtr<IStructStat> MakeStructStat(
    /* [in] */ const struct stat& sb)
{
    AutoPtr<CStructStat> obj;
    CStructStat::NewByFriend(static_cast<Int64>(sb.st_dev),
            static_cast<Int64>(sb.st_ino), static_cast<Int32>(sb.st_mode),
            static_cast<Int64>(sb.st_nlink), static_cast<Int32>(sb.st_uid),
            static_cast<Int32>(sb.st_gid), static_cast<Int64>(sb.st_rdev),
            static_cast<Int64>(sb.st_size), static_cast<Int64>(sb.st_atime),
            static_cast<Int64>(sb.st_mtime), static_cast<Int64>(sb.st_ctime),
            static_cast<Int64>(sb.st_blksize), static_cast<Int64>(sb.st_blocks),
            (CStructStat**)&obj);
    return (IStructStat*)obj;
}

static AutoPtr<IStructStatFs> MakeStructStatFs(
    /* [in] */ const struct statfs& sb)
{
    STATIC_ASSERT(sizeof(sb.f_bavail) == sizeof(Int64), statfs_not_64_bit);
    STATIC_ASSERT(sizeof(sb.f_bfree) == sizeof(Int64), statfs_not_64_bit);
    STATIC_ASSERT(sizeof(sb.f_blocks) == sizeof(Int64), statfs_not_64_bit);

    AutoPtr<CStructStatFs> obj;
    CStructStatFs::NewByFriend(static_cast<Int64>(sb.f_bsize),
            static_cast<Int64>(sb.f_blocks), static_cast<Int64>(sb.f_bfree),
            static_cast<Int64>(sb.f_bavail), static_cast<Int64>(sb.f_files),
            static_cast<Int64>(sb.f_ffree), static_cast<Int64>(sb.f_namelen),
            static_cast<Int64>(sb.f_frsize), (IStructStatFs**)&obj);
    return (IStructStatFs*)obj;
}

static AutoPtr<IStructLinger> MakeStructLinger(
    /* [in] */ const struct linger& l)
{
    AutoPtr<CStructLinger> linger;
    CStructLinger::NewByFriend(l.l_onoff, l.l_linger, (CStructLinger**)&linger);
    return (IStructLinger*)linger;
}

static AutoPtr<IStructTimeval> MakeStructTimeval(
    /* [in] */ const struct timeval& tv)
{
    AutoPtr<CStructTimeval> timeval;
    CStructTimeval::NewByFriend(static_cast<Int64>(tv.tv_sec),
            static_cast<Int64>(tv.tv_usec), (CStructTimeval**)&timeval);
    return (IStructTimeval*)timeval;
}

static AutoPtr<IStructUtsname> MakeStructUtsname(
    /* [in] */ const struct utsname& buf)
{
    String sysname(buf.sysname);
    String nodename(buf.nodename);
    String release(buf.release);
    String version(buf.version);
    String machine(buf.machine);

    AutoPtr<CStructUtsname> retObj;
    CStructUtsname::NewByFriend(sysname, nodename, release, version, machine,
            (CStructUtsname**)&retObj);
    return (IStructUtsname*)retObj;
}

static Boolean FillIfreq(
    /* [in] */ const String& interfaceName,
    /* [in] */ struct ifreq& req)
{
    if (interfaceName.IsNull()) {
        return FALSE;
    }
    memset(&req, 0, sizeof(req));
    strncpy(req.ifr_name, interfaceName.string(), sizeof(req.ifr_name));
    req.ifr_name[sizeof(req.ifr_name) - 1] = '\0';
    return TRUE;
}

static Boolean FillInetSocketAddress(
    /* [in] */ Int32 rc,
    /* [in] */ IInetSocketAddress* inetSocketAddress,
    /* [in] */ const sockaddr_storage* ss)
{
    if (rc == -1 || inetSocketAddress == NULL) {
        return TRUE;
    }
    // Fill out the passed-in InetSocketAddress with the sender's IP address and port number.
    Int32 port;
    AutoPtr<IInetAddress> sender = SockaddrToInetAddress(ss, &port);
    if (sender == NULL) {
        return FALSE;
    }
    ((CInetSocketAddress*)inetSocketAddress)->mAddr = sender;
    ((CInetSocketAddress*)inetSocketAddress)->mPort = port;
    return TRUE;
}

static AutoPtr<IStructStat> DoStat(
    /* [in] */ const String& path,
    /* [in] */ Boolean isLstat)
{
    if (path.IsNull()) {
        return NULL;
    }
    struct stat sb;
    Int32 rc =
            isLstat ?
                    TEMP_FAILURE_RETRY(lstat(path.string(), &sb)) :
                    TEMP_FAILURE_RETRY(stat(path.string(), &sb));
    if (rc == -1) {
//        throwErrnoException(env, isLstat ? "lstat" : "stat");
        return NULL;
    }
    return MakeStructStat(sb);
}

class Passwd {
public:
    Passwd()
        : mResult(NULL)
    {
        mBufferSize = sysconf(_SC_GETPW_R_SIZE_MAX);
        if (mBufferSize == -1UL) {
            // We're probably on bionic, where 1KiB should be enough for anyone.
            // TODO: fix bionic to return 1024 like glibc.
            mBufferSize = 1024;
        }
        mBuffer.reset(new char[mBufferSize]);
    }

    AutoPtr<IStructPasswd> getpwnam(
        /* [in] */ const char* name)
    {
        return process("getpwnam_r",
                getpwnam_r(name, &mPwd, mBuffer.get(), mBufferSize, &mResult));
    }

    AutoPtr<IStructPasswd> getpwuid(
        /* [in] */ uid_t uid)
    {
        return process("getpwuid_r",
                getpwuid_r(uid, &mPwd, mBuffer.get(), mBufferSize, &mResult));
    }

    struct passwd* get() {
        return mResult;
    }

private:
    AutoPtr<IStructPasswd> process(
        /* [in] */ const char* syscall,
        /* [in] */ int error)
    {
        if (mResult == NULL) {
            errno = error;
            return NULL;
        }
        return MakeStructPasswd(*mResult);
    }

    UniquePtr<char[]> mBuffer;
    size_t mBufferSize;
    struct passwd mPwd;
    struct passwd* mResult;
};

ECode CPosix::Accept(
    /* [in] */ Int32 fd,
    /* [in] */ IInetSocketAddress* peerAddress,
    /* [out] */ Int32* retFd)
{
    VALIDATE_NOT_NULL(retFd);

    sockaddr_storage ss;
    socklen_t sl = sizeof(ss);
    memset(&ss, 0, sizeof(ss));
    sockaddr* peer =
            (peerAddress != NULL) ? reinterpret_cast<sockaddr*>(&ss) : NULL;
    socklen_t* peerLength = (peerAddress != NULL) ? &sl : 0;
    Int32 clientFd = NET_FAILURE_RETRY(Int32, accept, fd, peer, peerLength);
    if (clientFd == -1
            || !FillInetSocketAddress(clientFd, peerAddress, &ss)) {
        close(clientFd);
        *retFd = 0;
        return E_LIBCORE_SOCKET_EXCEPTION;
    }
    *retFd = clientFd;
    return NOERROR;
}

ECode CPosix::Access(
    /* [in] */ const String& path,
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    if (path.IsNull()) {
        *succeeded = FALSE;
        return NOERROR;
    }
    int rc = TEMP_FAILURE_RETRY(access(path.string(), mode));
    if (rc == -1) {
        *succeeded = FALSE;
        return E_LIBCORE_IO_EXCEPTION;
    }
    *succeeded = (rc == 0);
    return NOERROR;
}

ECode CPosix::Bind(
    /* [in] */ Int32 fd,
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port)
{
    sockaddr_storage ss;
    if (!InetAddressToSockaddr(address, port, &ss)) {
        return NOERROR;
    }
    const sockaddr* sa = reinterpret_cast<const sockaddr*>(&ss);
    // We don't need the return value because we'll already have thrown.
    Int32 rc = NET_FAILURE_RETRY(Int32, bind, fd, sa, sizeof(sockaddr_storage));
    if (rc == -1) {
        return E_LIBCORE_SOCKET_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Chmod(
    /* [in] */ const String& path,
    /* [in] */ Int32 mode)
{
    if (path.IsNull()) {
        return NOERROR;
    }
    Int32 rc = TEMP_FAILURE_RETRY(chmod(path.string(), mode));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Chown(
    /* [in] */ const String& path,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    if (path.IsNull()) {
        return NOERROR;
    }
    Int32 rc = TEMP_FAILURE_RETRY(chown(path.string(), uid, gid));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Close(
    /* [in] */ Int32 fd)
{
    // Even if close(2) fails with EINTR, the fd will have been closed.
    // Using TEMP_FAILURE_RETRY will either lead to EBADF or closing someone else's fd.
    // http://lkml.indiana.edu/hypermail/linux/kernel/0509.1/0877.html
    Int32 rc = close(fd);
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Connect(
    /* [in] */ Int32 fd,
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port)
{
    sockaddr_storage ss;
    if (!InetAddressToSockaddr(address, port, &ss)) {
        return NOERROR;
    }
    const sockaddr* sa = reinterpret_cast<const sockaddr*>(&ss);
    // We don't need the return value because we'll already have thrown.
    Int32 rc =
            NET_FAILURE_RETRY(Int32, connect, fd, sa, sizeof(sockaddr_storage));
    if (rc == -1) {
        return E_LIBCORE_SOCKET_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Dup(
    /* [in] */ Int32 oldFd,
    /* [out] */ Int32* retFd)
{
    VALIDATE_NOT_NULL(retFd);

    *retFd = TEMP_FAILURE_RETRY(dup(oldFd));
    if (*retFd == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Dup2(
    /* [in] */ Int32 oldFd,
    /* [in] */ Int32 newFd,
    /* [out] */ Int32* retFd)
{
    VALIDATE_NOT_NULL(retFd);

    *retFd = TEMP_FAILURE_RETRY(dup2(oldFd, newFd));
    if (*retFd == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Environ(
/* [out, callee] */ArrayOf<String>** env)
{
    VALIDATE_NOT_NULL(env);

    AutoPtr<ArrayOf<String> > obj = ToStringArray(environ);
    *env = obj;
    INTERFACE_ADDREF(*env)
    return NOERROR;
}

ECode CPosix::Fchmod(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 mode)
{
    Int32 rc = TEMP_FAILURE_RETRY(fchmod(fd, mode));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Fchown(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    Int32 rc = TEMP_FAILURE_RETRY(fchown(fd, uid, gid));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::FcntlVoid(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 cmd,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = TEMP_FAILURE_RETRY(fcntl(fd, cmd));
    if (*result == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::FcntlInt64(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 cmd,
    /* [in] */ Int64 arg,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = TEMP_FAILURE_RETRY(fcntl(fd, cmd, arg));
    if (*result == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::FcntlFlock(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 cmd,
    /* [in] */ IStructFlock* arg,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    struct flock64 lock;
    memset(&lock, 0, sizeof(lock));
    arg->GetType(&lock.l_type);
    arg->GetWhence(&lock.l_whence);
    arg->GetStart(&lock.l_start);
    arg->GetLen(&lock.l_len);
    arg->GetPid(&lock.l_pid);

    *result = TEMP_FAILURE_RETRY(fcntl(fd, cmd, &lock));
    if (*result == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    arg->SetType(lock.l_type);
    arg->SetWhence(lock.l_whence);
    arg->SetStart(lock.l_start);
    arg->SetLen(lock.l_len);
    arg->SetPid(lock.l_pid);
    return NOERROR;
}

ECode CPosix::Fdatasync(
    /* [in] */ Int32 fd)
{
    Int32 rc = TEMP_FAILURE_RETRY(fdatasync(fd));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Fstat(
    /* [in] */ Int32 fd,
    /* [out] */ IStructStat** stat)
{
    VALIDATE_NOT_NULL(stat);

    struct stat sb;
    Int32 rc = TEMP_FAILURE_RETRY(fstat(fd, &sb));
    if (rc == -1) {
        *stat = NULL;
        return E_LIBCORE_IO_EXCEPTION;
    }
    AutoPtr<IStructStat> obj = MakeStructStat(sb);
    *stat = obj;
    INTERFACE_ADDREF(*stat);
    return NOERROR;
}

ECode CPosix::Fstatfs(
    /* [in] */ Int32 fd,
    /* [out] */ IStructStatFs** statFs)
{
    VALIDATE_NOT_NULL(statFs);

    struct statfs sb;
    Int32 rc = TEMP_FAILURE_RETRY(fstatfs(fd, &sb));
    if (rc == -1) {
        *statFs = NULL;
        return E_LIBCORE_IO_EXCEPTION;
    }
    AutoPtr<IStructStatFs> obj = MakeStructStatFs(sb);
    *statFs = obj;
    INTERFACE_ADDREF(*statFs);
    return NOERROR;
}

ECode CPosix::Fsync(
    /* [in] */ Int32 fd)
{
    Int32 rc = TEMP_FAILURE_RETRY(fsync(fd));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Ftruncate(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 length)
{
    Int32 rc = TEMP_FAILURE_RETRY(ftruncate64(fd, length));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Gai_strerror(
    /* [in] */ Int32 error,
    /* [out] */ String* strerror)
{
    VALIDATE_NOT_NULL(strerror);

    *strerror = gai_strerror(error);
    return NOERROR;
}

ECode CPosix::Getaddrinfo(
    /* [in] */ const String& node,
    /* [in] */ IStructAddrinfo* _hints,
    /* [out, callee] */ArrayOf<IInetAddress*>** info)
{
    VALIDATE_NOT_NULL(info);

    if (node.IsNull()) {
        *info = NULL;
        return NOERROR;
    }

    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    _hints->GetFlags(&hints.ai_flags);
    _hints->GetFamily(&hints.ai_family);
    _hints->GetSocktype(&hints.ai_socktype);
    _hints->GetProtocol(&hints.ai_protocol);

    addrinfo* addressList = NULL;
    errno = 0;
    int rc = getaddrinfo(node.string(), NULL, &hints, &addressList);
    UniquePtr<addrinfo, addrinfo_deleter> addressListDeleter(addressList);
    if (rc != 0) {
        *info = NULL;
        return E_LIBCORE_IO_EXCEPTION;
    }

    // Count results so we know how to size the output array.
    Int32 addressCount = 0;
    for (addrinfo* ai = addressList; ai != NULL; ai = ai->ai_next) {
        if (ai->ai_family == AF_INET || ai->ai_family == AF_INET6) {
            ++addressCount;
        } else {
//            ALOGE("getaddrinfo unexpected ai_family %i", ai->ai_family);
        }
    }
    if (addressCount == 0) {
        *info = NULL;
        return NOERROR;
    }

    // Prepare output array.
    AutoPtr<ArrayOf<IInetAddress*> > result =
            ArrayOf<IInetAddress*>::Alloc(addressCount);
    if (result == NULL) {
        *info = NULL;
        return NOERROR;
    }

    // Examine returned addresses one by one, save them in the output array.
    Int32 index = 0;
    for (addrinfo* ai = addressList; ai != NULL; ai = ai->ai_next) {
        if (ai->ai_family != AF_INET && ai->ai_family != AF_INET6) {
            // Unknown address family. Skip this address.
//            ALOGE("getaddrinfo unexpected ai_family %i", ai->ai_family);
            continue;
        }

        // Convert each IP address into a Java byte array.
        sockaddr_storage* address =
                reinterpret_cast<sockaddr_storage*>(ai->ai_addr);
        AutoPtr<IInetAddress> inetAddress = SockaddrToInetAddress(
                address, NULL);
        if (inetAddress == NULL) {
            *info = NULL;
            return NOERROR;
        }
        result->Set(index, inetAddress.Get());
        ++index;
    }
    *info = result;
    INTERFACE_ADDREF(*info)
    return NOERROR;
}

ECode CPosix::Getegid(
    /* [out] */ Int32* egid)
{
    VALIDATE_NOT_NULL(egid);

    *egid = getegid();
    return NOERROR;
}

ECode CPosix::Geteuid(
    /* [out] */ Int32* euid)
{
    VALIDATE_NOT_NULL(euid);

    *euid = geteuid();
    return NOERROR;
}

ECode CPosix::Getgid(
    /* [out] */ Int32* gid)
{
    VALIDATE_NOT_NULL(gid);

    *gid = getgid();
    return NOERROR;
}

ECode CPosix::Getenv(
    /* [in] */ const String& name,
    /* [out] */ String* env)
{
    VALIDATE_NOT_NULL(env);

    if (name.IsNull()) {
        *env = NULL;
        return NOERROR;
    }
    *env = getenv(name.string());
    return NOERROR;
}

ECode CPosix::Getnameinfo(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 flags,
    /* [out] */ String* nameinfo)
{
    VALIDATE_NOT_NULL(nameinfo);

    sockaddr_storage ss;
    if (!InetAddressToSockaddrVerbatim(address, 0, &ss)) {
        *nameinfo = NULL;
        return NOERROR;
    }
    // TODO: bionic's getnameinfo(3) seems to want its length parameter to be exactly
    // sizeof(sockaddr_in) for an IPv4 address and sizeof (sockaddr_in6) for an
    // IPv6 address. Fix getnameinfo so it accepts sizeof(sockaddr_storage), and
    // then remove this hack.
    socklen_t size =
            (ss.ss_family == AF_INET) ?
                    sizeof(sockaddr_in) : sizeof(sockaddr_in6);
    char buf[NI_MAXHOST]; // NI_MAXHOST is longer than INET6_ADDRSTRLEN.
    errno = 0;
    Int32 rc = getnameinfo(reinterpret_cast<sockaddr*>(&ss), size, buf,
            sizeof(buf), NULL, 0, flags);
    if (rc != 0) {
        *nameinfo = NULL;
        return E_LIBCORE_IO_EXCEPTION;
    }
    *nameinfo = buf;
    return NOERROR;
}

ECode CPosix::Getpid(
    /* [out] */ Int32* pid)
{
    VALIDATE_NOT_NULL(pid);

    *pid = getpid();
    return NOERROR;
}

ECode CPosix::Getppid(
    /* [out] */ Int32* ppid)
{
    VALIDATE_NOT_NULL(ppid);

    *ppid = getppid();
    return NOERROR;
}

ECode CPosix::Getpwnam(
    /* [in] */ const String& name,
    /* [out] */ IStructPasswd** pwnam)
{
    VALIDATE_NOT_NULL(pwnam);

    if (name.IsNull()) {
        *pwnam = NULL;
        return NOERROR;
    }
    AutoPtr<IStructPasswd> obj = Passwd().getpwnam(name.string());
    *pwnam = obj;
    INTERFACE_ADDREF(*pwnam);
    return NOERROR;
}

ECode CPosix::Getpwuid(
    /* [in] */ Int32 uid,
    /* [out] */ IStructPasswd** pwuid)
{
    VALIDATE_NOT_NULL(pwuid);

    AutoPtr<IStructPasswd> obj = Passwd().getpwuid(uid);
    *pwuid = obj;
    INTERFACE_ADDREF(*pwuid);
    return NOERROR;
}

ECode CPosix::Getsockname(
    /* [in] */ Int32 fd,
    /* [out] */ ISocketAddress** sockname)
{
    VALIDATE_NOT_NULL(sockname);
    *sockname = NULL;

    sockaddr_storage ss;
    sockaddr* sa = reinterpret_cast<sockaddr*>(&ss);
    socklen_t byteCount = sizeof(ss);
    memset(&ss, 0, byteCount);
    Int32 rc = TEMP_FAILURE_RETRY(getsockname(fd, sa, &byteCount));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }

    AutoPtr<ISocketAddress> obj = MakeSocketAddress(&ss);
    *sockname = obj;
    INTERFACE_ADDREF(*sockname);
    return NOERROR;
}

ECode CPosix::GetsockoptByte(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ Int32* sockopt)
{
    VALIDATE_NOT_NULL(sockopt);

    u_char result = 0;
    socklen_t size = sizeof(result);
    Int32 rc = TEMP_FAILURE_RETRY(getsockopt(fd, level, option, &result, &size));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    *sockopt = result;
    return NOERROR;
}

ECode CPosix::GetsockoptInAddr(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IInetAddress** addr)
{
    VALIDATE_NOT_NULL(addr);

    sockaddr_storage ss;
    memset(&ss, 0, sizeof(ss));
    ss.ss_family = AF_INET; // This is only for the IPv4-only IP_MULTICAST_IF.
    sockaddr_in* sa = reinterpret_cast<sockaddr_in*>(&ss);
    socklen_t size = sizeof(sa->sin_addr);
    Int32 rc =
            TEMP_FAILURE_RETRY(getsockopt(fd, level, option, &sa->sin_addr, &size));
    if (rc == -1) {
        *addr = NULL;
        return E_LIBCORE_IO_EXCEPTION;
    }
    AutoPtr<IInetAddress> obj = SockaddrToInetAddress(&ss, NULL);
    *addr = obj;
    INTERFACE_ADDREF(*addr);
    return NOERROR;
}

ECode CPosix::GetsockoptInt(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ Int32* sockopt)
{
    VALIDATE_NOT_NULL(sockopt);

    Int32 result = 0;
    socklen_t size = sizeof(result);
    Int32 rc = TEMP_FAILURE_RETRY(getsockopt(fd, level, option, &result, &size));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    *sockopt = result;
    return NOERROR;
}

ECode CPosix::GetsockoptLinger(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IStructLinger** linger)
{
    VALIDATE_NOT_NULL(linger);

    struct linger l;
    socklen_t size = sizeof(l);
    memset(&l, 0, size);
    Int32 rc = TEMP_FAILURE_RETRY(getsockopt(fd, level, option, &l, &size));
    if (rc == -1) {
        *linger = NULL;
        return E_LIBCORE_IO_EXCEPTION;
    }
    AutoPtr<IStructLinger> obj = MakeStructLinger(l);
    *linger = obj;
    INTERFACE_ADDREF(*linger);
    return NOERROR;
}

ECode CPosix::GetsockoptTimeval(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [out] */ IStructTimeval** timeval)
{
    VALIDATE_NOT_NULL(timeval);

    struct timeval tv;
    socklen_t size = sizeof(tv);
    memset(&tv, 0, size);
    Int32 rc = TEMP_FAILURE_RETRY(getsockopt(fd, level, option, &tv, &size));
    if (rc == -1) {
        *timeval = NULL;
        return E_LIBCORE_IO_EXCEPTION;
    }
    AutoPtr<IStructTimeval> obj = MakeStructTimeval(tv);
    *timeval = obj;
    INTERFACE_ADDREF(*timeval);
    return NOERROR;
}

ECode CPosix::Getuid(
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid);

    *uid = getuid();
    return NOERROR;
}

ECode CPosix::If_indextoname(
    /* [in] */ Int32 index,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    char buf[IF_NAMESIZE];
    char* name = if_indextoname(index, buf);
    // if_indextoname(3) returns NULL on failure, which will come out of NewStringUTF unscathed.
    // There's no useful information in errno, so we don't bother throwing. Callers can null-check.
    *str = String(name);
    return NOERROR;
}

ECode CPosix::Inet_pton(
    /* [in] */ Int32 family,
    /* [in] */ const String& address,
    /* [out] */ IInetAddress** addr)
{
    VALIDATE_NOT_NULL(addr);

    if (address.IsNull()) {
        *addr = NULL;
        return NOERROR;
    }
    sockaddr_storage ss;
    memset(&ss, 0, sizeof(ss));
    // sockaddr_in and sockaddr_in6 are at the same address, so we can use either here.
    void* dst = &reinterpret_cast<sockaddr_in*>(&ss)->sin_addr;
    if (inet_pton(family, address.string(), dst) != 1) {
        *addr = NULL;
        return NOERROR;
    }
    ss.ss_family = family;
    AutoPtr<IInetAddress> obj = SockaddrToInetAddress(&ss, NULL);
    *addr = obj;
    INTERFACE_ADDREF(*addr);
    return NOERROR;
}

ECode CPosix::IoctlInetAddress(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 cmd,
    /* [in] */ const String& interfaceName,
    /* [out] */ IInetAddress** addr)
{
    VALIDATE_NOT_NULL(addr);

    struct ifreq req;
    if (!FillIfreq(interfaceName, req)) {
        *addr = NULL;
        return NOERROR;
    }
    Int32 rc = TEMP_FAILURE_RETRY(ioctl(fd, cmd, &req));
    if (rc == -1) {
        *addr = NULL;
        return E_LIBCORE_IO_EXCEPTION;
    }
    AutoPtr<IInetAddress> obj = SockaddrToInetAddress(
            reinterpret_cast<sockaddr_storage*>(&req.ifr_addr), NULL);
    *addr = obj;
    INTERFACE_ADDREF(*addr);
    return NOERROR;
}

ECode CPosix::IoctlInt(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 cmd,
    /* [in] */ Int32* arg,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    // This is complicated because ioctls may return their result by updating their argument
    // or via their return value, so we need to support both.
    Int32 rc = TEMP_FAILURE_RETRY(ioctl(fd, cmd, arg));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Isatty(
    /* [in] */ Int32 fd,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = TEMP_FAILURE_RETRY(isatty(fd)) == 1;
    return NOERROR;
}

ECode CPosix::Kill(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 signal)
{
    Int32 rc = TEMP_FAILURE_RETRY(kill(pid, signal));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Lchown(
    /* [in] */ const String& path,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    if (path.IsNull()) {
        return NOERROR;
    }
    Int32 rc = TEMP_FAILURE_RETRY(lchown(path.string(), uid, gid));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Listen(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 backlog)
{
    Int32 rc = TEMP_FAILURE_RETRY(listen(fd, backlog));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Lseek(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int32 whence,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = TEMP_FAILURE_RETRY(lseek64(fd, offset, whence));
    if (*result == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Lstat(
    /* [in] */ const String& path,
    /* [out] */ IStructStat** stat)
{
    VALIDATE_NOT_NULL(stat);

    AutoPtr<IStructStat> obj = DoStat(path, TRUE);
    *stat = obj;
    INTERFACE_ADDREF(*stat);
    return obj != NULL ? NOERROR : E_LIBCORE_IO_EXCEPTION;
}

ECode CPosix::Mincore(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount,
    /* [in] */ const ArrayOf<Byte>& vector)
{
    void* ptr = reinterpret_cast<void*>(static_cast<uintptr_t>(address));
    unsigned char* vec = reinterpret_cast<unsigned char*>(vector.GetPayload());
    Int32 rc = TEMP_FAILURE_RETRY(mincore(ptr, byteCount, vec));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Mkdir(
    /* [in] */ const String& path,
    /* [in] */ Int32 mode)
{
    if (path.IsNull()) {
        return NOERROR;
    }
    Int32 rc = TEMP_FAILURE_RETRY(mkdir(path.string(), mode));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Mlock(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount)
{
    void* ptr = reinterpret_cast<void*>(static_cast<uintptr_t>(address));
    Int32 rc = TEMP_FAILURE_RETRY(mlock(ptr, byteCount));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Mmap(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount,
    /* [in] */ Int32 prot,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 fd,
    /* [in] */ Int64 offset,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    void* suggestedPtr =
            reinterpret_cast<void*>(static_cast<uintptr_t>(address));
    void* ptr = mmap(suggestedPtr, byteCount, prot, flags, fd, offset);
    if (ptr == MAP_FAILED) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    *result = static_cast<Int64>(reinterpret_cast<uintptr_t>(ptr));
    return NOERROR;
}

ECode CPosix::Msync(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount,
    /* [in] */ Int32 flags)
{
    void* ptr = reinterpret_cast<void*>(static_cast<uintptr_t>(address));
    Int32 rc = TEMP_FAILURE_RETRY(msync(ptr, byteCount, flags));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Munlock(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount)
{
    void* ptr = reinterpret_cast<void*>(static_cast<uintptr_t>(address));
    Int32 rc = TEMP_FAILURE_RETRY(munlock(ptr, byteCount));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Munmap(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount)
{
    void* ptr = reinterpret_cast<void*>(static_cast<uintptr_t>(address));
    Int32 rc = TEMP_FAILURE_RETRY(munmap(ptr, byteCount));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Open(
    /* [in] */ const String& path,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mode,
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);

    if (path.IsNull()) {
        *fd = -1;
        return NOERROR;
    }
    *fd = TEMP_FAILURE_RETRY(open(path.string(), flags, mode));
    if (*fd == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Pipe(
    /* [out, callee] */ ArrayOf<Int32>** fdArray)
{
    VALIDATE_NOT_NULL(fdArray);
    *fdArray = NULL;

    int fds[2];
    Int32 rc = TEMP_FAILURE_RETRY(pipe(&fds[0]));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }

    *fdArray = ArrayOf<Int32>::Alloc(2);
    if (*fdArray == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }

    INTERFACE_ADDREF(*fdArray);
    for (int i = 0; i < 2; ++i) {
        (**fdArray)[i] = fds[i];
    }
    return NOERROR;
}

ECode CPosix::Poll(
    /* [in] */ const ArrayOf<IStructPollfd*>& fdStructs,
    /* [in] */ Int32 timeoutMs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    // Turn the Java libcore.io.StructPollfd[] into a C++ struct pollfd[].
    size_t arrayLength = fdStructs.GetLength();
    UniquePtr<struct pollfd[]> fds(new struct pollfd[arrayLength]);
    memset(fds.get(), 0, sizeof(struct pollfd) * arrayLength);
    size_t count = 0; // Some trailing array elements may be irrelevant. (See below.)
    for (size_t i = 0; i < arrayLength; ++i) {
        fdStructs[i]->GetFd(&fds[count].fd);
        fdStructs[i]->GetEvents(&fds[count].events);
        ++count;
    }

    // Since we don't know which fds -- if any -- are sockets, be conservative and register
    // all fds for asynchronous socket close monitoring.
    Vector<AsynchronousSocketCloseMonitor*> monitors;
    for (size_t i = 0; i < count; ++i) {
        monitors.PushBack(new AsynchronousSocketCloseMonitor(fds[i].fd));
    }
    Int32 rc = poll(fds.get(), count, timeoutMs);
    for (size_t i = 0; i < monitors.GetSize(); ++i) {
        delete monitors[i];
    }
    if (rc == -1) {
        *result = -1;
        return E_LIBCORE_IO_EXCEPTION;
    }

    // Update the revents fields in the Java libcore.io.StructPollfd[].
    for (size_t i = 0; i < count; ++i) {
        fdStructs[i]->GetRevents(&fds[i].revents);
    }
    *result = rc;
    return NOERROR;
}

ECode CPosix::Pread(
    /* [in] */ Int32 fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);

    if (bytes == NULL) {
        *num = -1;
        return NOERROR;
    }
    *num =
            TEMP_FAILURE_RETRY(pread64(fd, bytes->GetPayload() + byteOffset, byteCount, offset));
    if (*num == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Pwrite(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);

    *num =
            TEMP_FAILURE_RETRY(pwrite64(fd, bytes.GetPayload() + byteOffset, byteCount, offset));
    if (*num == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Read(
    /* [in] */ Int32 fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);

    if (bytes == NULL) {
        *num = -1;
        return NOERROR;
    }
    *num = TEMP_FAILURE_RETRY(read(fd, bytes->GetPayload() + byteOffset, byteCount));
    if (*num == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Recvfrom(
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

    sockaddr_storage ss;
    socklen_t sl = sizeof(ss);
    memset(&ss, 0, sizeof(ss));
    sockaddr* from =
            (srcAddress != NULL) ? reinterpret_cast<sockaddr*>(&ss) : NULL;
    socklen_t* fromLength = (srcAddress != NULL) ? &sl : 0;
    Int32 recvCount =
            NET_FAILURE_RETRY(ssize_t, recvfrom, fd,
                    bytes->GetPayload() + byteOffset, byteCount, flags, from, fromLength);
    FillInetSocketAddress(recvCount, srcAddress, &ss);
    *num = recvCount;
    return NOERROR;
}

ECode CPosix::Remove(
    /* [in] */ const String& path)
{
    if (path.IsNull()) {
        return NOERROR;
    }
    Int32 rc = TEMP_FAILURE_RETRY(remove(path.string()));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Rename(
    /* [in] */ const String& oldPath,
    /* [in] */ const String& newPath)
{
    if (oldPath.IsNull()) {
        return NOERROR;
    }
    if (newPath.IsNull()) {
        return NOERROR;
    }
    Int32 rc = TEMP_FAILURE_RETRY(rename(oldPath.string(), newPath.string()));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Sendto(
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

    sockaddr_storage ss;
    if (inetAddress != NULL
            && !InetAddressToSockaddr(inetAddress, port, &ss)) {
        *result = -1;
        return NOERROR;
    }
    const sockaddr* to =
            (inetAddress != NULL) ?
                    reinterpret_cast<const sockaddr*>(&ss) : NULL;
    socklen_t toLength = (inetAddress != NULL) ? sizeof(ss) : 0;
    *result = NET_FAILURE_RETRY(ssize_t, sendto, fd, bytes.GetPayload() + byteOffset, byteCount, flags, to, toLength);
    return NOERROR;
}

ECode CPosix::Sendfile(
    /* [in] */ Int32 outFd,
    /* [in] */ Int32 inFd,
    /* [in, out] */ Int64* inOffset,
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    off_t offset = 0;
    off_t* offsetPtr = NULL;
    if (inOffset != NULL) {
        // TODO: fix bionic so we can have a 64-bit off_t!
        offset = *inOffset;
        offsetPtr = &offset;
    }
    *result = TEMP_FAILURE_RETRY(sendfile(outFd, inFd, offsetPtr, byteCount));
    if (inOffset != NULL) {
        *inOffset = offset;
    }
    if (*result == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Setegid(
    /* [in] */ Int32 egid)
{
    Int32 rc = TEMP_FAILURE_RETRY(setegid(egid));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Seteuid(
    /* [in] */ Int32 euid)
{
    Int32 rc = TEMP_FAILURE_RETRY(seteuid(euid));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Setgid(
    /* [in] */ Int32 gid)
{
    Int32 rc = TEMP_FAILURE_RETRY(setgid(gid));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Setsid(
    /* [out] */ Int32* sid)
{
    VALIDATE_NOT_NULL(sid);

    *sid = TEMP_FAILURE_RETRY(setsid());
    if (*sid == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::SetsockoptByte(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ Int32 value)
{
    u_char byte = value;
    Int32 rc =
            TEMP_FAILURE_RETRY(setsockopt(fd, level, option, &byte, sizeof(byte)));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::SetsockoptIfreq(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ const String& interfaceName)
{
    struct ifreq req;
    if (!FillIfreq(interfaceName, req)) {
        return NOERROR;
    }
    Int32 rc =
            TEMP_FAILURE_RETRY(setsockopt(fd, level, option, &req, sizeof(req)));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::SetsockoptInt(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ Int32 value)
{
    Int32 rc =
            TEMP_FAILURE_RETRY(setsockopt(fd, level, option, &value, sizeof(value)));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::SetsockoptIpMreqn(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ Int32 value)
{
    ip_mreqn req;
    memset(&req, 0, sizeof(req));
    req.imr_ifindex = value;
    Int32 rc =
            TEMP_FAILURE_RETRY(setsockopt(fd, level, option, &req, sizeof(req)));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::SetsockoptGroupReq(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ IStructGroupReq* value)
{
    struct group_req req;
    memset(&req, 0, sizeof(req));

    Int32 itf;
    value->GetGrInterface(&itf);
    req.gr_interface = itf;
    // Get the IPv4 or IPv6 multicast address to join or leave.
    AutoPtr<IInetAddress> group;
    value->GetGrGroup((IInetAddress**)&group);
    if (!InetAddressToSockaddrVerbatim(group, 0, &req.gr_group)) {
        return NOERROR;
    }

    Int32 rc =
            TEMP_FAILURE_RETRY(setsockopt(fd, level, option, &req, sizeof(req)));
    if (rc == -1 && errno == EINVAL) {
        // Maybe we're a 32-bit binary talking to a 64-bit kernel?
        // glibc doesn't automatically handle this.
        struct group_req64 {
            uint32_t gr_interface;
            uint32_t my_padding;sockaddr_storage gr_group;
        };
        group_req64 req64;
        req64.gr_interface = req.gr_interface;
        memcpy(&req64.gr_group, &req.gr_group, sizeof(req.gr_group));
        rc =
                TEMP_FAILURE_RETRY(setsockopt(fd, level, option, &req64, sizeof(req64)));
    }
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::SetsockoptLinger(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ IStructLinger* linger)
{
    Int32 onoff, l;
    linger->GetOnoff(&onoff);
    linger->GetLinger(&l);
    struct linger value;
    value.l_onoff = onoff;
    value.l_linger = l;

    Int32 rc =
            TEMP_FAILURE_RETRY(setsockopt(fd, level, option, &value, sizeof(value)));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::SetsockoptTimeval(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 level,
    /* [in] */ Int32 option,
    /* [in] */ IStructTimeval* timeval)
{
    Int64 sec, usec;
    timeval->GetSec(&sec);
    timeval->GetUsec(&usec);
    struct timeval value;
    value.tv_sec = sec;
    value.tv_usec = usec;
    Int32 rc =
            TEMP_FAILURE_RETRY(setsockopt(fd, level, option, &value, sizeof(value)));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Setuid(
    /* [in] */ Int32 uid)
{
    Int32 rc = TEMP_FAILURE_RETRY(setuid(uid));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Shutdown(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 how)
{
    Int32 rc = TEMP_FAILURE_RETRY(shutdown(fd, how));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Socket(
    /* [in] */ Int32 socketDomain,
    /* [in] */ Int32 type,
    /* [in] */ Int32 protocol,
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);

    *fd = TEMP_FAILURE_RETRY(socket(socketDomain, type, protocol));
    if (*fd == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Socketpair(
    /* [in] */ Int32 socketDomain,
    /* [in] */ Int32 type,
    /* [in] */ Int32 protocol,
    /* [out] */ Int32* fd1,
    /* [out] */ Int32* fd2)
{
    VALIDATE_NOT_NULL(fd1);
    VALIDATE_NOT_NULL(fd2);

    Int32 fds[2];
    Int32 rc = TEMP_FAILURE_RETRY(socketpair(socketDomain, type, protocol, fds));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    } else {
        *fd1 = fds[0];
        *fd2 = fds[1];
        return NOERROR;
    }
}

ECode CPosix::Stat(
    /* [in] */ const String& path,
    /* [out] */ IStructStat** stat)
{
    VALIDATE_NOT_NULL(stat);

    AutoPtr<IStructStat> obj = DoStat(path, FALSE);
    *stat = obj;
    INTERFACE_ADDREF(*stat);
    return obj != NULL ? NOERROR : E_LIBCORE_IO_EXCEPTION;
}

ECode CPosix::Statfs(
    /* [in] */ const String& path,
    /* [out] */ IStructStatFs** statFs)
{
    VALIDATE_NOT_NULL(statFs);

    if (path.IsNull()) {
        *statFs = NULL;
        return NOERROR;
    }
    struct statfs sb;
    Int32 rc = TEMP_FAILURE_RETRY(statfs(path.string(), &sb));
    if (rc == -1) {
        *statFs = NULL;
        return E_LIBCORE_IO_EXCEPTION;
    }
    AutoPtr<IStructStatFs> obj = MakeStructStatFs(sb);
    *statFs = obj;
    INTERFACE_ADDREF(*statFs);
    return NOERROR;
}

ECode CPosix::Strerror(
    /* [in] */ Int32 errnum,
    /* [out] */ String* strerr)
{
    VALIDATE_NOT_NULL(strerr);

    char buffer[BUFSIZ];
    const char* message = StrError(errnum, buffer, sizeof(buffer));
    *strerr = String(message);
    return NOERROR;
}

ECode CPosix::Symlink(
    /* [in] */ const String& oldPath,
    /* [in] */ const String& newPath)
{
    if (oldPath.IsNull()) {
        return NOERROR;
    }
    if (newPath.IsNull()) {
        return NOERROR;
    }
    Int32 rc = TEMP_FAILURE_RETRY(symlink(oldPath.string(), newPath.string()));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Sysconf(
    /* [in] */ Int32 name,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    // Since -1 is a valid result from sysconf(3), detecting failure is a little more awkward.
    errno = 0;
    *result = sysconf(name);
    if (*result == -1ll && errno == EINVAL) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Tcdrain(
    /* [in] */ Int32 fd)
{
    Int32 rc = TEMP_FAILURE_RETRY(tcdrain(fd));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CPosix::Umask(
    /* [in] */ Int32 mask,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = umask(mask);
    return NOERROR;
}

ECode CPosix::Uname(
    /* [out] */ IStructUtsname** utsname)
{
    VALIDATE_NOT_NULL(utsname);

    struct utsname buf;
    if (TEMP_FAILURE_RETRY(uname(&buf)) == -1) {
        *utsname = NULL; // Can't happen.
        return NOERROR;
    }
    AutoPtr<IStructUtsname> obj = MakeStructUtsname(buf);
    *utsname = obj;
    INTERFACE_ADDREF(*utsname);
    return NOERROR;
}

ECode CPosix::Waitpid(
    /* [in] */ Int32 pid,
    /* [in, out] */ Int32* _status,
    /* [in] */ Int32 options,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 status;
    Int32 rc = TEMP_FAILURE_RETRY(waitpid(pid, &status, options));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    } else {
        *_status = status;
    }
    *result = rc;
    return NOERROR;
}

ECode CPosix::Write(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);

    Int32 rc = TEMP_FAILURE_RETRY(write(fd, bytes.GetPayload() + byteOffset, byteCount));
    if (rc == -1) {
        return E_LIBCORE_IO_EXCEPTION;
    }
    *num = rc;
    return NOERROR;
}

ECode CPosix::SendtoEx(
    /* [in] */ Int32 fd,
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int32 flags,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    Boolean isflag = FALSE;
    buffer->IsDirect(&isflag);
    if (isflag) {
        AutoPtr< ArrayOf<Byte> > outbyte;
        buffer->GetArray((ArrayOf<Byte>**)&outbyte);
        Int32 position = 0;
        Int32 remaining = 0;
        buffer->GetPosition(&position);
        buffer->GetRemaining(&remaining);
        return Sendto(fd, *outbyte, position, remaining, flags, inetAddress, port, result);
    }
    else {
        AutoPtr< ArrayOf<Byte> > outbyte = NioUtils::GetUnsafeArray((HeapByteBuffer*) buffer->Probe(EIID_HeapByteBuffer));
        Int32 offset = NioUtils::GetUnsafeArrayOffset((HeapByteBuffer*) buffer->Probe(EIID_HeapByteBuffer));
        Int32 position = 0;
        buffer->GetPosition(&position);
        Int32 remaining = 0;
        buffer->GetRemaining(&remaining);
        return Sendto(fd, *outbyte, offset + position, remaining, flags, inetAddress, port, result);
    }
}

ECode CPosix::RecvfromEx(
    /* [in] */ Int32 fd,
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int32 flags,
    /* [in] */ IInetSocketAddress* srcAddress,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num)

    Boolean isflag = FALSE;
    buffer->IsDirect(&isflag);
    if (isflag) {
        AutoPtr< ArrayOf<Byte> > outbyte;
        buffer->GetArray((ArrayOf<Byte>**)&outbyte);
        Int32 position = 0;
        Int32 remaining = 0;
        buffer->GetPosition(&position);
        buffer->GetRemaining(&remaining);
        return Recvfrom(fd, outbyte, position, remaining, flags, srcAddress, num);
    }
    else {
        AutoPtr< ArrayOf<Byte> > outbyte = NioUtils::GetUnsafeArray((HeapByteBuffer*) buffer->Probe(EIID_HeapByteBuffer));
        Int32 offset = NioUtils::GetUnsafeArrayOffset((HeapByteBuffer*) buffer->Probe(EIID_HeapByteBuffer));
        Int32 position = 0;
        buffer->GetPosition(&position);
        Int32 remaining = 0;
        buffer->GetRemaining(&remaining);
        return Recvfrom(fd, outbyte, offset + position, remaining, flags, srcAddress, num);
    }
}

ECode CPosix::ReadEx(
    /* [in] */ Int32 fd,
    /* [in] */ IByteBuffer* buffer,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num)

    Boolean isflag = FALSE;
    buffer->IsDirect(&isflag);
    if (isflag) {
        AutoPtr< ArrayOf<Byte> > outbyte;
        buffer->GetArray((ArrayOf<Byte>**)&outbyte);
        Int32 position = 0;
        Int32 remaining = 0;
        buffer->GetPosition(&position);
        buffer->GetRemaining(&remaining);
        return Read(fd, outbyte, position, remaining, num);
    }
    else {
        AutoPtr< ArrayOf<Byte> > outbyte = NioUtils::GetUnsafeArray((HeapByteBuffer*) buffer->Probe(EIID_HeapByteBuffer));
        Int32 offset = NioUtils::GetUnsafeArrayOffset((HeapByteBuffer*) buffer->Probe(EIID_HeapByteBuffer));
        Int32 position = 0;
        buffer->GetPosition(&position);
        Int32 remaining = 0;
        buffer->GetRemaining(&remaining);
        return Read(fd, outbyte, offset + position, remaining, num);
    }
}

ECode CPosix::Readv(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<IInterface*>& buffers,
    /* [in] */ const ArrayOf<Int32>& offsets,
    /* [in] */ const ArrayOf<Int32>& byteCounts,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num)

    Int32 buflen = buffers.GetLength();
    AutoPtr< ArrayOf<ByteBuffer*> > outbyte = ArrayOf<ByteBuffer*>::Alloc(buflen);
    for (Int32 i = 0; i < buflen; ++i) {
        outbyte->Set(i, (ByteBuffer*) buffers[i]->Probe(EIID_ByteBuffer));
    }
    assert(0 && "TODO");
    // IoVec ioVec(outbyte, env->GetArrayLength(buffers));
    // if (!ioVec.init(buffers, offsets, byteCounts)) {
    //     return -1;
    // }
    // int fd = jniGetFDFromFileDescriptor(env, javaFd);
    // return throwIfMinusOne(env, "readv", TEMP_FAILURE_RETRY(readv(fd, ioVec.get(), ioVec.size())));
    return NOERROR;
}

ECode CPosix::PwriteEx(
    /* [in] */ Int32 fd,
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num)

    Boolean isflag = FALSE;
    buffer->IsDirect(&isflag);
    if (isflag) {
        AutoPtr< ArrayOf<Byte> > outbyte;
        buffer->GetArray((ArrayOf<Byte>**)&outbyte);
        Int32 position = 0;
        Int32 remaining = 0;
        buffer->GetPosition(&position);
        buffer->GetRemaining(&remaining);
        return Pwrite(fd, *outbyte, position, remaining, offset, num);
    }
    else {
        AutoPtr< ArrayOf<Byte> > outbyte = NioUtils::GetUnsafeArray((HeapByteBuffer*) buffer->Probe(EIID_HeapByteBuffer));
        Int32 offset2 = NioUtils::GetUnsafeArrayOffset((HeapByteBuffer*) buffer->Probe(EIID_HeapByteBuffer));
        Int32 position = 0;
        buffer->GetPosition(&position);
        Int32 remaining = 0;
        buffer->GetRemaining(&remaining);
        return Pwrite(fd, *outbyte, offset2 + position, remaining, offset, num);
    }
}

ECode CPosix::PreadEx(
    /* [in] */ Int32 fd,
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int64 offset,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num)

    Boolean isflag = FALSE;
    buffer->IsDirect(&isflag);
    if (isflag) {
        AutoPtr< ArrayOf<Byte> > outbyte;
        buffer->GetArray((ArrayOf<Byte>**)&outbyte);
        Int32 position = 0;
        Int32 remaining = 0;
        buffer->GetPosition(&position);
        buffer->GetRemaining(&remaining);
        return Pread(fd, outbyte, position, remaining, offset, num);
    }
    else {
        AutoPtr< ArrayOf<Byte> > outbyte = NioUtils::GetUnsafeArray((HeapByteBuffer*) buffer->Probe(EIID_HeapByteBuffer));
        Int32 offset2 = NioUtils::GetUnsafeArrayOffset((HeapByteBuffer*) buffer->Probe(EIID_HeapByteBuffer));
        Int32 position = 0;
        buffer->GetPosition(&position);
        Int32 remaining = 0;
        buffer->GetRemaining(&remaining);
        return Pread(fd, outbyte, offset2 + position, remaining, offset, num);
    }
}

ECode CPosix::WriteEx(
    /* [in] */ Int32 fd,
    /* [in] */ IByteBuffer* buffer,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num)

    Boolean isflag = FALSE;
    buffer->IsDirect(&isflag);
    if (isflag) {
        AutoPtr< ArrayOf<Byte> > outbyte;
        buffer->GetArray((ArrayOf<Byte>**)&outbyte);
        Int32 position = 0;
        Int32 remaining = 0;
        buffer->GetPosition(&position);
        buffer->GetRemaining(&remaining);
        return Write(fd, *outbyte, position, remaining, num);
    }
    else {
        AutoPtr< ArrayOf<Byte> > outbyte = NioUtils::GetUnsafeArray((HeapByteBuffer*) buffer->Probe(EIID_HeapByteBuffer));
        Int32 offset2 = NioUtils::GetUnsafeArrayOffset((HeapByteBuffer*) buffer->Probe(EIID_HeapByteBuffer));
        Int32 position = 0;
        buffer->GetPosition(&position);
        Int32 remaining = 0;
        buffer->GetRemaining(&remaining);
        return Write(fd, *outbyte, offset2 + position, remaining, num);
    }
}

ECode CPosix::Writev(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<IInterface*>& buffers,
    /* [in] */ const ArrayOf<Int32>& offsets,
    /* [in] */ const ArrayOf<Int32>& byteCounts,
    /* [out] */ Int32* result)
{
    assert(0 && "TODO");
    // IoVec<ScopedBytesRO> ioVec(env, env->GetArrayLength(buffers));
    // if (!ioVec.init(buffers, offsets, byteCounts)) {
    //     return -1;
    // }
    // int fd = jniGetFDFromFileDescriptor(env, javaFd);
    // return throwIfMinusOne(env, "writev", TEMP_FAILURE_RETRY(writev(fd, ioVec.get(), ioVec.size())));
    return NOERROR;
}

} // namespace IO
} // namespace Libcore
