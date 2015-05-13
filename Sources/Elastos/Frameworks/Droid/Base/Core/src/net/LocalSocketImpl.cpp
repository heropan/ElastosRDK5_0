
#include "ext/frameworkdef.h"
#include "net/LocalSocketImpl.h"
#include "net/CCredentials.h"
#include <elastos/Math.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <unistd.h>
#include <cutils/sockets.h>


using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::IBoolean;
using Elastos::IO::CFileDescriptor;
using Elastos::Net::ISocketOptions;
using Elastos::Droid::Net::CCredentials;

namespace Elastos {
namespace Droid {
namespace Net {

LocalSocketImpl::SocketInputStream::SocketInputStream(
    /* [in] */ LocalSocketImpl* owner)
    : mOwner(owner)
{}

PInterface LocalSocketImpl::SocketInputStream::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IInputStream*)this;
    }
    else if (riid == Elastos::IO::EIID_IInputStream) {
        return (IInputStream*)this;
    }

    return NULL;
}

UInt32 LocalSocketImpl::SocketInputStream::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LocalSocketImpl::SocketInputStream::Release()
{
    return ElRefBase::Release();
}

ECode LocalSocketImpl::SocketInputStream::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [in] */ InterfaceID* pIID)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode LocalSocketImpl::SocketInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = InputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

/**
 * An input stream for local sockets. Needed because we may
 * need to read ancillary data.
 */
ECode LocalSocketImpl::SocketInputStream::Available(
    /* [out] */ Int32* result)
{
    return mOwner->NativeAvailable(mOwner->mFd, result);
}

ECode LocalSocketImpl::SocketInputStream::Close()
{
    return mOwner->Close();
}

ECode LocalSocketImpl::SocketInputStream::Read(
    /* [out] */ Int32* result)
{
    Mutex::Autolock lock(mOwner->mReadMonitor);

    AutoPtr<IFileDescriptor> myFd = mOwner->mFd;
    if (myFd == NULL) {
        // throw new IOException("socket closed");
        return E_IO_EXCEPTION;
    }

    return mOwner->NativeRead(myFd, result);
}

ECode LocalSocketImpl::SocketInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* b,
    /* [out] */ Int32* result)
{
    assert(b != NULL);
    return ReadBytesEx(b, 0, b->GetLength(), result);
}

ECode LocalSocketImpl::SocketInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* b,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ Int32* result)
{
    Mutex::Autolock lock(mOwner->mReadMonitor);

    AutoPtr<IFileDescriptor> myFd = mOwner->mFd;
    if (myFd == NULL) {
        // throw new IOException("socket closed");
        return E_IO_EXCEPTION;
    }

    if (off < 0 || len < 0 || (off + len) > b->GetLength()) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    return mOwner->NativeReadba(b, off, len, myFd, result);
}

ECode LocalSocketImpl::SocketInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return InputStream::Mark(readLimit);
}

ECode LocalSocketImpl::SocketInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return InputStream::IsMarkSupported(supported);
}

ECode LocalSocketImpl::SocketInputStream::Reset()
{
    return InputStream::Reset();
}

ECode LocalSocketImpl::SocketInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    AutoPtr<ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(4096);

    Int64 skipped = 0;
    while (skipped < byteCount) {
        Int32 toRead = (Int32)Elastos::Core::Math::Min(byteCount - skipped, (Int64)buffer->GetLength());
        Int32 read = 0;
        FAIL_RETURN(ReadBytesEx(buffer.Get(), 0, toRead, &read));
        if (read == -1) {
            break;
        }
        skipped += read;
        if (read < toRead) {
            break;
        }
    }

    *number = skipped;

    return NOERROR;
}


LocalSocketImpl::SocketOutputStream::SocketOutputStream(
    /* [in] */ LocalSocketImpl* owner)
    : mOwner(owner)
{}

PInterface LocalSocketImpl::SocketOutputStream::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IOutputStream*)this;
    }
    else if (riid == Elastos::IO::EIID_IOutputStream) {
        return (IOutputStream*)this;
    }
    else if (riid == Elastos::IO::EIID_IFlushable) {
        return (IFlushable*)this;
    }

    return NULL;
}

UInt32 LocalSocketImpl::SocketOutputStream::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LocalSocketImpl::SocketOutputStream::Release()
{
    return ElRefBase::Release();
}

ECode LocalSocketImpl::SocketOutputStream::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [in] */ InterfaceID* pIID)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

/**
 * An output stream for local sockets. Needed because we may
 * need to read ancillary data.
 */
ECode LocalSocketImpl::SocketOutputStream::Close()
{
    return mOwner->Close();
}

ECode LocalSocketImpl::SocketOutputStream::Write (
    /* [in] */ Int32 b)
{
    Mutex::Autolock lock(mOwner->mWriteMonitor);

    AutoPtr<IFileDescriptor> myFd = mOwner->mFd;
    if (myFd == NULL) {
        // throw new IOException("socket closed");
        return E_IO_EXCEPTION;
    }

    return mOwner->NativeWrite(b, myFd);
}

ECode LocalSocketImpl::SocketOutputStream::WriteBytes (
    /* [in] */ const ArrayOf<Byte>& b)
{
    return WriteBytesEx(b, 0, b.GetLength());
}

ECode LocalSocketImpl::SocketOutputStream::WriteBytesEx (
    /* [in] */ const ArrayOf<Byte>& b,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len)
{
    Mutex::Autolock lock(mOwner->mWriteMonitor);

    AutoPtr<IFileDescriptor> myFd = mOwner->mFd;
    if (myFd == NULL) {
        // throw new IOException("socket closed");
        return E_IO_EXCEPTION;
    }

    if (off < 0 || len < 0 || (off + len) > b.GetLength()) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return mOwner->NativeWriteba(b, off, len, myFd);
}

ECode LocalSocketImpl::SocketOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);
    *hasError = FALSE;
    return NOERROR;
}

ECode LocalSocketImpl::SocketOutputStream::Flush()
{
    return NOERROR;
}

ECode LocalSocketImpl::SocketOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = OutputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

/**
 * Create a new instance.
 */
LocalSocketImpl::LocalSocketImpl()
{}

/**
 * Create a new instance from a file descriptor representing
 * a bound socket. The state of the file descriptor is not checked here
 *  but the caller can verify socket state by calling listen().
 *
 * @param fd non-NULL; bound file descriptor
 */
LocalSocketImpl::LocalSocketImpl(
    /* [in] */ IFileDescriptor* fd)
    : mFd(fd)
{}

LocalSocketImpl::~LocalSocketImpl()
{
    Close();
}

ECode LocalSocketImpl::NativeAvailable(
    /* [in] */ IFileDescriptor* fileDescriptor,
    /* [out] */ Int32* size)
{
    Int32 fd;

    fileDescriptor->GetDescriptor(&fd);

    int avail;
    int ret = ioctl(fd, FIONREAD, &avail);

    // If this were a non-socket fd, there would be other cases to worry
    // about...

    if (ret < 0) {
        //jniThrowIOException(env, errno);
        *size = 0;
        return E_IO_EXCEPTION;
    }

    *size = avail;
    return NOERROR;
}

ECode LocalSocketImpl::NativeClose(
    /* [in] */ IFileDescriptor* fileDescriptor)
{
    Int32 fd;
    Int32 err;

    if (fileDescriptor == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    fileDescriptor->GetDescriptor(&fd);

    do {
        err = close(fd);
    } while (err < 0 && errno == EINTR);

    if (err < 0) {
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Processes ancillary data, handling only
 * SCM_RIGHTS. Creates appropriate objects and sets appropriate
 * fields in the LocalSocketImpl object. Returns 0 on success
 * or -1 if an exception was thrown.
 */
static int socket_process_cmsg(
    /* [in] */ LocalSocketImpl* thisObj,
    /* [in] */ struct msghdr * pMsg)
{
    struct cmsghdr *cmsgptr;

    for (cmsgptr = CMSG_FIRSTHDR(pMsg);
            cmsgptr != NULL; cmsgptr = CMSG_NXTHDR(pMsg, cmsgptr)) {

        if (cmsgptr->cmsg_level != SOL_SOCKET) {
            continue;
        }

        if (cmsgptr->cmsg_type == SCM_RIGHTS) {
            Int32 *pDescriptors = (Int32 *)CMSG_DATA(cmsgptr);
            Int32 count
                = ((cmsgptr->cmsg_len - CMSG_LEN(0)) / sizeof(int));

            if (count < 0) {
//                jniThrowException(env, "java/io/IOException", "invalid cmsg length");
            }

            AutoPtr< ArrayOf<IFileDescriptor*> > fdArray = ArrayOf<IFileDescriptor*>::Alloc(count);

            if (fdArray == NULL) {
                return -1;
            }

            for (Int32 i = 0; i < count; i++) {
                AutoPtr<IFileDescriptor> fdObject;
                CFileDescriptor::New((IFileDescriptor**)&fdObject);
                fdObject->SetDescriptor(pDescriptors[i]);

                fdArray->Set(i, fdObject);
            }

            thisObj->mInboundFileDescriptors = fdArray;
        }
    }

    return 0;
}

/**
 * Reads data from a socket into buf, processing any ancillary data
 * and adding it to thisJ.
 *
 * Returns the length of normal data read, or -1 if an exception has
 * been thrown in this function.
 */
static ssize_t socket_read_all(
    /* [in] */ LocalSocketImpl* thisObj,
    /* [in] */ Int32 fd,
    /* [in] */ void *buffer,
    /* [in] */ size_t len)
{
    ssize_t ret;
    ssize_t bytesread = 0;
    struct msghdr msg;
    struct iovec iv;
    unsigned char *buf = (unsigned char *)buffer;
    // Enough buffer for a pile of fd's. We throw an exception if
    // this buffer is too small.
    struct cmsghdr cmsgbuf[2*sizeof(cmsghdr) + 0x100];

    memset(&msg, 0, sizeof(msg));
    memset(&iv, 0, sizeof(iv));

    iv.iov_base = buf;
    iv.iov_len = len;

    msg.msg_iov = &iv;
    msg.msg_iovlen = 1;
    msg.msg_control = cmsgbuf;
    msg.msg_controllen = sizeof(cmsgbuf);

    do {
        ret = recvmsg(fd, &msg, MSG_NOSIGNAL);
    } while (ret < 0 && errno == EINTR);

    if (ret < 0 && errno == EPIPE) {
        // Treat this as an end of stream
        return 0;
    }

    if (ret < 0) {
//        jniThrowIOException(env, errno);
        return -1;
    }

    if ((msg.msg_flags & (MSG_CTRUNC | MSG_OOB | MSG_ERRQUEUE)) != 0) {
        // To us, any of the above flags are a fatal error

//        jniThrowException(env, "java/io/IOException",
//                "Unexpected error or truncation during recvmsg()");
        return -1;
    }

    if (ret >= 0) {
        socket_process_cmsg(thisObj, &msg);
    }

    return ret;
}

/**
 * Writes all the data in the specified buffer to the specified socket.
 *
 * Returns 0 on success or -1 if an exception was thrown.
 */
static Int32 socket_write_all(
    /* [in] */ LocalSocketImpl* thisObj,
    /* [in] */ Int32 fd,
    /* [in] */ void* buf,
    /* [in] */ size_t len)
{
    ssize_t ret;
    struct msghdr msg;
    unsigned char *buffer = (unsigned char *)buf;
    memset(&msg, 0, sizeof(msg));

    AutoPtr< ArrayOf<IFileDescriptor*> > outboundFds =
            thisObj->mOutboundFileDescriptors;

    struct cmsghdr *cmsg;
    Int32 countFds = outboundFds == NULL ? 0 : outboundFds->GetLength();
    Int32 fds[countFds];
    char msgbuf[CMSG_SPACE(countFds)];

    // Add any pending outbound file descriptors to the message
    if (outboundFds != NULL) {
        for (Int32 i = 0; i < countFds; i++) {
            AutoPtr<IFileDescriptor> fdObject = (*outboundFds)[i];

            fdObject->GetDescriptor(&fds[i]);
        }

        // See "man cmsg" really
        msg.msg_control = msgbuf;
        msg.msg_controllen = sizeof msgbuf;
        cmsg = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;
        cmsg->cmsg_len = CMSG_LEN(sizeof fds);
        memcpy(CMSG_DATA(cmsg), fds, sizeof fds);
    }

    // We only write our msg_control during the first write
    while (len > 0) {
        struct iovec iv;
        memset(&iv, 0, sizeof(iv));

        iv.iov_base = buffer;
        iv.iov_len = len;

        msg.msg_iov = &iv;
        msg.msg_iovlen = 1;

        do {
            ret = sendmsg(fd, &msg, MSG_NOSIGNAL);
        } while (ret < 0 && errno == EINTR);

        if (ret < 0) {
            // jniThrowIOException(env, errno);
            return -1;
        }

        buffer += ret;
        len -= ret;

        // Wipes out any msg_control too
        memset(&msg, 0, sizeof(msg));
    }

    return 0;
}

ECode LocalSocketImpl::NativeRead(
    /* [in] */ IFileDescriptor* fileDescriptor,
    /* [out] */ Int32* result)
{
    Int32 fd;
    Int32 err;

    if (fileDescriptor == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    fileDescriptor->GetDescriptor(&fd);

    unsigned char buf;

    err = socket_read_all(this, fd, &buf, 1);

    if (err < 0) {
        *result = 0;
        return E_IO_EXCEPTION;
    }

    if (err == 0) {
        // end of file
        *result = -1;
        return NOERROR;
    }

    *result = (Int32)buf;
    return NOERROR;
}

ECode LocalSocketImpl::NativeReadba(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ IFileDescriptor* fileDescriptor,
    /* [out] */ Int32* result)
{
    Int32 fd;
    Byte* byteBuffer;
    int ret;

    if (fileDescriptor == NULL || buffer == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    if (off < 0 || len < 0 || (off + len) > buffer->GetLength()) {
//        jniThrowException(env, "java/lang/ArrayIndexOutOfBoundsException", NULL);
//        return (jint)-1;
        *result = -1;
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (len == 0) {
        // because socket_read_all returns 0 on EOF
        *result = 0;
        return NOERROR;
    }

    fileDescriptor->GetDescriptor(&fd);

    byteBuffer = buffer->GetPayload();

    ret = socket_read_all(this, fd, byteBuffer + off, len);

    *result = ((ret == 0) ? -1 : ret);
    return NOERROR;
}

ECode LocalSocketImpl::NativeWriteba(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ IFileDescriptor* fileDescriptor)
{
    Int32 fd;
    int err;
    Byte* byteBuffer;

    if (fileDescriptor == NULL) {
//        jniThrowNULLPointerException(env, NULL);
//        return;
        return E_NULL_POINTER_EXCEPTION;
    }

    if (off < 0 || len < 0 || (off + len) > buffer.GetLength()) {
//        jniThrowException(env, "java/lang/ArrayIndexOutOfBoundsException", NULL);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    fileDescriptor->GetDescriptor(&fd);

    byteBuffer = buffer.GetPayload();

    err = socket_write_all(this, fd,
            byteBuffer + off, len);

    // A return of -1 above means an exception is pending
    if (err < 0) return E_IO_EXCEPTION;
    return NOERROR;
}

ECode LocalSocketImpl::NativeWrite(
    /* [in] */ Int32 b,
    /* [in] */ IFileDescriptor* fileDescriptor)
{
    Int32 fd;
    int err;

    if (fileDescriptor == NULL) {
//        jniThrowNullPointerException(env, NULL);
        return E_NULL_POINTER_EXCEPTION;
    }

    fileDescriptor->GetDescriptor(&fd);

    err = socket_write_all(this, fd, &b, 1);

    // A return of -1 above means an exception is pending
    if (err < 0) return E_IO_EXCEPTION;
    return NOERROR;
}

ECode LocalSocketImpl::ConnectLocal(
    /* [in] */ IFileDescriptor* fileDescriptor,
    /* [in] */ const String& name,
    /* [in] */ Int32 namespaceId)
{
    int ret;
    const char *nameUtf8;
    Int32 fd;

    nameUtf8 = (const char*)name;

    fileDescriptor->GetDescriptor(&fd);

    ret = socket_local_client_connect(
                fd,
                nameUtf8,
                namespaceId,
                SOCK_STREAM);

    if (ret < 0) {
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode LocalSocketImpl::BindLocal(
    /* [in] */ IFileDescriptor* fileDescriptor,
    /* [in] */ const String& name,
    /* [in] */ Int32 namespaceId)
{
    int ret;
    Int32 fd;
    const char *nameUtf8;

    if (name.IsNull()) {
//        jniThrowNullPointerException(env, NULL);
        return E_NULL_POINTER_EXCEPTION;
    }

    fileDescriptor->GetDescriptor(&fd);

    nameUtf8 = (const char*)name;

    ret = socket_local_server_bind(fd, nameUtf8, namespaceId);

    if (ret < 0) {
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode LocalSocketImpl::NativeCreate(
    /* [in] */ Boolean stream,
    /* [out] */ IFileDescriptor** fd)
{
    int ret;

    ret = socket(PF_LOCAL, stream ? SOCK_STREAM : SOCK_DGRAM, 0);

    if (ret < 0) {
        *fd = NULL;
        return E_IO_EXCEPTION;
    }

    FAIL_RETURN(CFileDescriptor::New(fd));
    (*fd)->SetDescriptor(ret);
    return NOERROR;
}

ECode LocalSocketImpl::NativeListen(
    /* [in] */ IFileDescriptor* fileDescriptor,
    /* [in] */ Int32 backlog)
{
    int ret;
    Int32 fd;

    fileDescriptor->GetDescriptor(&fd);

    ret = listen(fd, backlog);

    if (ret < 0) {
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode LocalSocketImpl::Shutdown(
    /* [in] */ IFileDescriptor* fileDescriptor,
    /* [in] */ Boolean shutdownInput)
{
    int ret;
    Int32 fd;

    fileDescriptor->GetDescriptor(&fd);

    ret = shutdown(fd, shutdownInput ? SHUT_RD : SHUT_WR);

    if (ret < 0) {
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode LocalSocketImpl::NativeGetPeerCredentials(
    /* [in] */ IFileDescriptor* fileDescriptor,
    /* [out] */ ICredentials** credentials)
{
    int err;
    Int32 fd;

    if (fileDescriptor == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    fileDescriptor->GetDescriptor(&fd);

    struct ucred creds;

    memset(&creds, 0, sizeof(creds));
    socklen_t szCreds = sizeof(creds);

    err = getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &creds, &szCreds);

    if (err < 0) {
        *credentials = NULL;
        return E_IO_EXCEPTION;
    }

    if (szCreds == 0) {
        *credentials = NULL;
        return NOERROR;
    }
    AutoPtr<ICredentials>  crden;
    CCredentials::New(creds.pid, creds.uid, creds.gid, (ICredentials**)&crden);
    *credentials = crden;
    INTERFACE_ADDREF(*credentials);
    return NOERROR;
}

static Boolean
droid_opt_to_real(Int32 optID, int* opt, int* level)
{
    switch (optID)
    {
        case 4098:
            *opt = SO_RCVBUF;
            *level = SOL_SOCKET;
            return TRUE;
        case 4097:
            *opt = SO_SNDBUF;
            *level = SOL_SOCKET;
            return TRUE;
        case 4102:
            *opt = SO_SNDTIMEO;
            *level = SOL_SOCKET;
            return TRUE;
        case 128:
            *opt = SO_LINGER;
            *level = SOL_SOCKET;
            return TRUE;
        case 1:
            *opt = TCP_NODELAY;
            *level = IPPROTO_TCP;
            return TRUE;
        case 4:
            *opt = SO_REUSEADDR;
            *level = SOL_SOCKET;
            return TRUE;

    }
    return FALSE;
}

ECode LocalSocketImpl::NativeGetOption(
    /* [in] */ IFileDescriptor* fileDescriptor,
    /* [in] */ Int32 optID,
    /* [out] */ Int32* option)
{
    Int32 ret, value;
    int opt, level;
    Int32 fd;

    socklen_t size = sizeof(int);

    if (!droid_opt_to_real(optID, &opt, &level)) {
        *option = 0;
        return E_IO_EXCEPTION;
    }

    fileDescriptor->GetDescriptor(&fd);

    switch (opt)
    {
        case SO_LINGER:
        {
            struct linger lingr;
            size = sizeof(lingr);
            ret = getsockopt(fd, level, opt, &lingr, &size);
            if (!lingr.l_onoff) {
                value = -1;
            } else {
                value = lingr.l_linger;
            }
            break;
        }
        default:
            ret = getsockopt(fd, level, opt, &value, &size);
            break;
    }


    if (ret != 0) {
        *option = 0;
        return E_IO_EXCEPTION;
    }

    *option = value;
    return NOERROR;
}

ECode LocalSocketImpl::NativeSetOption(
    /* [in] */ IFileDescriptor* fileDescriptor,
    /* [in] */ Int32 optID,
    /* [in] */ Int32 boolValue,
    /* [in] */ Int32 intValue)
{
    Int32 ret;
    Int32 optname;
    Int32 level;
    Int32 fd;

    if (!droid_opt_to_real(optID, &optname, &level)) {
        return E_IO_EXCEPTION;
    }

    fileDescriptor->GetDescriptor(&fd);

    switch (optname) {
        case SO_LINGER: {
            /*
             * SO_LINGER is special because it needs to use a special
             * "linger" struct as well as use the incoming boolean
             * argument specially.
             */
            struct linger lingr;
            lingr.l_onoff = boolValue ? 1 : 0; // Force it to be 0 or 1.
            lingr.l_linger = intValue;
            ret = setsockopt(fd, level, optname, &lingr, sizeof(lingr));
            break;
        }
        case SO_SNDTIMEO: {
            /*
             * SO_TIMEOUT from the core library gets converted to
             * SO_SNDTIMEO, but the option is supposed to set both
             * send and receive timeouts. Note: The incoming timeout
             * value is in milliseconds.
             */
            struct timeval timeout;
            timeout.tv_sec = intValue / 1000;
            timeout.tv_usec = (intValue % 1000) * 1000;

            ret = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO,
                    (void *)&timeout, sizeof(timeout));

            if (ret == 0) {
                ret = setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO,
                        (void *)&timeout, sizeof(timeout));
            }

            break;
        }
        default: {
            /*
             * In all other cases, the translated option level and
             * optname may be used directly for a call to setsockopt().
             */
            ret = setsockopt(fd, level, optname, &intValue, sizeof(intValue));
            break;
        }
    }

    if (ret != 0) {
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Accepts a connection on a server socket.
 *
 * @param fd file descriptor of server socket
 * @param s socket implementation that will become the new socket
 * @return file descriptor of new socket
 */
ECode LocalSocketImpl::NativeAccept(
    /* [in] */ IFileDescriptor* fileDescriptor,
    /* [in] */ LocalSocketImpl* s,
    /* [out] */ IFileDescriptor** fdObj)
{
    union {
        struct sockaddr address;
        struct sockaddr_un un_address;
    } sa;

    int ret;
    Int32 retFD;
    Int32 fd;
    socklen_t addrlen;

    if (s == NULL) {
//        jniThrowNullPointerException(env, NULL);
        *fdObj = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    fileDescriptor->GetDescriptor(&fd);

    do {
        addrlen = sizeof(sa);
        ret = accept(fd, &(sa.address), &addrlen);
    } while (ret < 0 && errno == EINTR);

    if (ret < 0) {
        *fdObj = NULL;
        return E_IO_EXCEPTION;
    }

    retFD = ret;

    CFileDescriptor::New(fdObj);
    (*fdObj)->SetDescriptor(retFD);
    return NOERROR;
}

String LocalSocketImpl::ToString()
{
    assert(0);
    return String(NULL);
}

/**
 * Creates a socket in the underlying OS.
 *
 * @param stream TRUE if this should be a stream socket, FALSE for
 * datagram.
 * @throws IOException
 */
ECode LocalSocketImpl::Create(
    /* [in] */ Boolean stream)
{
    // no error if socket already created
    // need this for LocalServerSocket.accept()
    if (mFd == NULL) {
        return NativeCreate(stream, (IFileDescriptor**)&mFd);
    }
    return NOERROR;
}

/**
 * Closes the socket.
 *
 * @throws IOException
 */
ECode LocalSocketImpl::Close()
{
    Mutex::Autolock lock(mLock);

    if (mFd == NULL) return NOERROR;
    FAIL_RETURN(NativeClose(mFd));
    mFd = NULL;
    return NOERROR;
}

/** note timeout presently ignored */
ECode LocalSocketImpl::Connect(
    /* [in] */ ILocalSocketAddress* address,
    /* [in] */ Int32 timeout)
{
    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    String name;
    address->GetName(&name);
    LocalSocketAddressNamespace ns;
    address->GetNamespace(&ns);
    return ConnectLocal(mFd, name, (Int32)ns);
}

/**
 * Binds this socket to an endpoint name. May only be called on an instance
 * that has not yet been bound.
 *
 * @param endpoint endpoint address
 * @throws IOException
 */
ECode LocalSocketImpl::Bind(
    /* [in] */ ILocalSocketAddress* endpoint)
{
    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    String name;
    endpoint->GetName(&name);
    LocalSocketAddressNamespace ns;
    endpoint->GetNamespace(&ns);
    return BindLocal(mFd, name, (Int32)ns);
}

ECode LocalSocketImpl::Listen(
    /* [in] */ Int32 backlog)
{
    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    return NativeListen(mFd, backlog);
}

/**
 * Accepts a new connection to the socket. Blocks until a new
 * connection arrives.
 *
 * @param s a socket that will be used to represent the new connection.
 * @throws IOException
 */
ECode LocalSocketImpl::Accept(
    /* [in] */ LocalSocketImpl* s)
{
    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    s->mFd = NULL;
    return NativeAccept(mFd, s, (IFileDescriptor**)&s->mFd);
}

/**
 * Retrieves the input stream for this instance.
 *
 * @return input stream
 * @throws IOException if socket has been Closed or cannot be created.
 */
ECode LocalSocketImpl::GetInputStream(
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is);

    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    {
        Mutex::Autolock lock(mLock);

        if (mFis == NULL) {
            mFis = new SocketInputStream(this);
        }

        *is = mFis;
        INTERFACE_ADDREF(*is);
        return NOERROR;
    }
}

/**
 * Retrieves the output stream for this instance.
 *
 * @return output stream
 * @throws IOException if socket has been Closed or cannot be created.
 */
ECode LocalSocketImpl::GetOutputStream(
    /* [out] */ IOutputStream** os)
{
    VALIDATE_NOT_NULL(os);

    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    {
        Mutex::Autolock lock(mLock);

        if (mFos == NULL) {
            mFos = new SocketOutputStream(this);
        }

        *os = mFos;
        INTERFACE_ADDREF(*os);
        return NOERROR;
    }
}

/**
 * Returns the number of bytes available for reading without blocking.
 *
 * @return >= 0 count bytes available
 * @throws IOException
 */
ECode LocalSocketImpl::Available(
    /* [out] */ Int32* avail)
{
    VALIDATE_NOT_NULL(avail);

    AutoPtr<IInputStream> is;
    FAIL_RETURN(GetInputStream((IInputStream**)&is));
    return is->Available(avail);
}

/**
 * Shuts down the input side of the socket.
 *
 * @throws IOException
 */
ECode LocalSocketImpl::ShutdownInput()
{
    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    return Shutdown(mFd, TRUE);
}

/**
 * Shuts down the output side of the socket.
 *
 * @throws IOException
 */
ECode LocalSocketImpl::ShutdownOutput()
{
    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    return Shutdown(mFd, FALSE);
}

AutoPtr<IFileDescriptor> LocalSocketImpl::GetFileDescriptor()
{
    return mFd;
}

Boolean LocalSocketImpl::SupportsUrgentData()
{
    return FALSE;
}

ECode LocalSocketImpl::SendUrgentData(
    /* [in] */ Int32 data)
{
//    throw new RuntimeException ("not impled");
    return E_RUNTIME_EXCEPTION;
}

ECode LocalSocketImpl::GetOption(
    /* [in] */ Int32 optID,
    /* [out] */ IInterface** result)
{
    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    if (optID == ISocketOptions::SO_TIMEOUT) {
        *result = 0;
        return NOERROR;
    }

    Int32 value;
    FAIL_RETURN(NativeGetOption(mFd, optID, &value));
    switch (optID) {
        case 4098 /*ISocketOptions::SO_RCVBUF*/:
        case 4097/*ISocketOptions::SO_SNDBUF*/:
            return CInteger32::New(value, (IInteger32**)result);
        case 4/*ISocketOptions::SO_REUSEADDR*/:
        default:
            return CInteger32::New(value, (IInteger32**)result);
    }
}

ECode LocalSocketImpl::SetOption(
    /* [in] */ Int32 optID,
    /* [in] */ IInterface* value)
{
    /*
     * Boolean.FALSE is used to disable some options, so it
     * is important to distinguish between FALSE and unset.
     * We define it here that -1 is unset, 0 is FALSE, and 1
     * is TRUE.
     */
    Int32 boolValue = -1;
    Int32 intValue = 0;

    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    if (IInteger32::Probe(value) != NULL) {
        IInteger32::Probe(value)->GetValue(&intValue);
    }
    else if (IBoolean::Probe(value) != NULL) {
        Boolean bv;
        IBoolean::Probe(value)->GetValue(&bv);
        boolValue = bv? 1 : 0;
    }
    else {
//        throw new IOException("bad value: " + value);
        return E_IO_EXCEPTION;
    }

    return NativeSetOption(mFd, optID, boolValue, intValue);
}

/**
 * Enqueues a set of file descriptors to send to the peer. The queue
 * is one deep. The file descriptors will be sent with the next write
 * of normal data, and will be delivered in a single ancillary message.
 * See "man 7 unix" SCM_RIGHTS on a desktop Linux machine.
 *
 * @param fds non-NULL; file descriptors to send.
 * @throws IOException
 */
void LocalSocketImpl::SetFileDescriptorsForSend(
    /* [in] */ ArrayOf<IFileDescriptor*>* fds)
{
    Mutex::Autolock lock(mWriteMonitor);

    mOutboundFileDescriptors = fds;
}

/**
 * Retrieves a set of file descriptors that a peer has sent through
 * an ancillary message. This method retrieves the most recent set sent,
 * and then returns NULL until a new set arrives.
 * File descriptors may only be passed along with regular data, so this
 * method can only return a non-NULL after a read operation.
 *
 * @return NULL or file descriptor array
 * @throws IOException
 */
AutoPtr< ArrayOf<IFileDescriptor*> > LocalSocketImpl::GetAncillaryFileDescriptors()
{
    Mutex::Autolock lock(mReadMonitor);

    AutoPtr< ArrayOf<IFileDescriptor*> > result = mInboundFileDescriptors;

    mInboundFileDescriptors = NULL;
    return result;
}

/**
 * Retrieves the credentials of this socket's peer. Only valid on
 * connected sockets.
 *
 * @return non-NULL; peer credentials
 * @throws IOException
 */
ECode LocalSocketImpl::GetPeerCredentials(
    /* [out] */ ICredentials** result)
{
    VALIDATE_NOT_NULL(result);
    return NativeGetPeerCredentials(mFd, result);
}

/**
 * Retrieves the socket name from the OS.
 *
 * @return non-NULL; socket name
 * @throws IOException on failure
 */
ECode LocalSocketImpl::GetSockAddress(
    /* [out] */ ILocalSocketAddress** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;
    return NOERROR;
    //TODO implement this
    //return getSockName_native(fd);
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos

