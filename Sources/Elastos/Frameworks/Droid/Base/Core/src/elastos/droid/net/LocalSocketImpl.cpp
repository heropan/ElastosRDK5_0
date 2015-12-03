
#include "elastos/droid/net/LocalSocketImpl.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(LocalSocketImpl, Object, ILocalSocketImpl)

#if 0 // TODO: Translated before. Need check.
static Boolean droid_opt_to_real(Int32 optID, int* opt, int* level)
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
                return -1;
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
#endif

ECode LocalSocketImpl::Pending_native(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    int fd;

    fd = jniGetFDFromFileDescriptor(env, fileDescriptor);

    if (env->ExceptionOccurred() != NULL) {
        return (jint)-1;
    }

    int pending;
    int ret = ioctl(fd, TIOCOUTQ, &pending);

    // If this were a non-socket fd, there would be other cases to worry
    // about...

    //ALOGD("socket_pending, ioctl ret:%d, pending:%d", ret, pending);
    if (ret < 0) {
        jniThrowIOException(env, errno);
        return (jint) 0;
    }

    return (jint)pending;
#endif
}

ECode LocalSocketImpl::Available_native(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode LocalSocketImpl::Read_native(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode LocalSocketImpl::Readba_native(
    /* [in] */ ArrayOf<Byte>* b,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode LocalSocketImpl::Writeba_native(
    /* [in] */ ArrayOf<Byte>* b,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ IFileDescriptor* fd)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode LocalSocketImpl::Write_native(
    /* [in] */ Int32 b,
    /* [in] */ IFileDescriptor* fd)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode LocalSocketImpl::ConnectLocal(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ const String& name,
    /* [in] */ Int32 ns)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
#endif
}

ECode LocalSocketImpl::BindLocal(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ const String& name,
    /* [in] */ Int32 ns)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
#endif
}

ECode LocalSocketImpl::Listen_native(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 backlog)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    int ret;
    Int32 fd;

    fileDescriptor->GetDescriptor(&fd);

    ret = listen(fd, backlog);

    if (ret < 0) {
        return E_IO_EXCEPTION;
    }
    return NOERROR;
#endif
}

ECode LocalSocketImpl::Shutdown(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Boolean shutdownInput)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    int ret;
    Int32 fd;

    fileDescriptor->GetDescriptor(&fd);

    ret = shutdown(fd, shutdownInput ? SHUT_RD : SHUT_WR);

    if (ret < 0) {
        return E_IO_EXCEPTION;
    }
    return NOERROR;
#endif
}

ECode LocalSocketImpl::GetPeerCredentials_native(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ ICredentials** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
    REFCOUNT_ADD(*credentials);
    return NOERROR;
#endif
}

ECode LocalSocketImpl::GetOption_native(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 optID,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode LocalSocketImpl::SetOption_native(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 optID,
    /* [in] */ Int32 b,
    /* [in] */ Int32 value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode LocalSocketImpl::Accept(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ILocalSocketImpl* s,
    /* [out] */ IFileDescriptor** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode LocalSocketImpl::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
#endif
}

ECode LocalSocketImpl::constructor(
    /* [in] */ IFileDescriptor* fd)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.fd = fd;
#endif
}

ECode LocalSocketImpl::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    assert(0);
    return String(NULL);
#endif
}

ECode LocalSocketImpl::Create(
    /* [in] */ Int32 sockType)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        // no error if socket already created
        // need this for LocalServerSocket.accept()
        if (fd == NULL) {
            int osType;
            switch (sockType) {
                case LocalSocket.SOCKET_DGRAM:
                    osType = OsConstants.SOCK_DGRAM;
                    break;
                case LocalSocket.SOCKET_STREAM:
                    osType = OsConstants.SOCK_STREAM;
                    break;
                case LocalSocket.SOCKET_SEQPACKET:
                    osType = OsConstants.SOCK_SEQPACKET;
                    break;
                default:
                    throw new IllegalStateException("unknown sockType");
            }
            try {
                fd = Os.socket(OsConstants.AF_UNIX, osType, 0);
                mFdCreatedInternally = TRUE;
            } catch (ErrnoException e) {
                e.rethrowAsIOException();
            }
        }
#endif
}

ECode LocalSocketImpl::Close()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(LocalSocketImpl.this) {
            if ((fd == NULL) || (mFdCreatedInternally == FALSE)) {
                fd = NULL;
                return NOERROR;
            }
            try {
                Os.close(fd);
            } catch (ErrnoException e) {
                e.rethrowAsIOException();
            }
            fd = NULL;
        }
#endif
}

ECode LocalSocketImpl::Connect(
    /* [in] */ ILocalSocketAddress* address,
    /* [in] */ Int32 timeout)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    String name;
    address->GetName(&name);
    LocalSocketAddressNamespace ns;
    address->GetNamespace(&ns);
    return ConnectLocal(mFd, name, (Int32)ns);
#endif
}

ECode LocalSocketImpl::Bind(
    /* [in] */ ILocalSocketAddress* endpoint)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    String name;
    endpoint->GetName(&name);
    LocalSocketAddressNamespace ns;
    endpoint->GetNamespace(&ns);
    return BindLocal(mFd, name, (Int32)ns);
#endif
}

ECode LocalSocketImpl::Listen(
    /* [in] */ Int32 backlog)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    return NativeListen(mFd, backlog);
#endif
}

ECode LocalSocketImpl::Accept(
    /* [in] */ ILocalSocketImpl* s)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    s->mFd = NULL;
    s->mFdCreatedInternally = TRUE;
    return NativeAccept(mFd, s, (IFileDescriptor**)&s->mFd);
#endif
}

ECode LocalSocketImpl::GetInputStream(
    /* [out] */ IInputStream** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(is);

    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    {
        synchronized(mLock) {

            if (mFis == NULL) {
                mFis = new SocketInputStream(this);
            }

            *is = mFis;
            REFCOUNT_ADD(*is);
        }
        return NOERROR;
    }
#endif
}

ECode LocalSocketImpl::GetOutputStream(
    /* [out] */ IOutputStream** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(os);

    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    {
        synchronized(mLock) {
            if (mFos == NULL) {
                mFos = new SocketOutputStream(this);
            }

            *os = mFos;
            REFCOUNT_ADD(*os);
        }
        return NOERROR;
    }
#endif
}

ECode LocalSocketImpl::Available(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(avail);

    AutoPtr<IInputStream> is;
    FAIL_RETURN(GetInputStream((IInputStream**)&is));
    return is->Available(avail);
#endif
}

ECode LocalSocketImpl::ShutdownInput()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    return Shutdown(mFd, TRUE);
#endif
}

ECode LocalSocketImpl::ShutdownOutput()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (mFd == NULL) {
//        throw new IOException("socket not created");
        return E_IO_EXCEPTION;
    }

    return Shutdown(mFd, FALSE);
#endif
}

ECode LocalSocketImpl::GetFileDescriptor(
    /* [out] */ IFileDescriptor** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return fd;
#endif
}

ECode LocalSocketImpl::SupportsUrgentData(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return FALSE;
#endif
}

ECode LocalSocketImpl::SendUrgentData(
    /* [in] */ Int32 data)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
//    throw new RuntimeException ("not impled");
    return E_RUNTIME_EXCEPTION;
#endif
}

ECode LocalSocketImpl::GetOption(
    /* [in] */ Int32 optID,
    /* [out] */ IInterface** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode LocalSocketImpl::SetOption(
    /* [in] */ Int32 optID,
    /* [in] */ IInterface* value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode LocalSocketImpl::SetFileDescriptorsForSend(
    /* [in] */ ArrayOf<IFileDescriptor*>* fds)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    synchronized(mWriteMonitor) {

        mOutboundFileDescriptors = fds;
    }
    return NOERROR;
#endif
}

ECode LocalSocketImpl::GetAncillaryFileDescriptors(
    /* [out, callee] */ ArrayOf<IFileDescriptor*>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    synchronized(mReadMonitor) {

        AutoPtr< ArrayOf<IFileDescriptor*> > result = mInboundFileDescriptors;

        mInboundFileDescriptors = NULL;
    }
    return result;
#endif
}

ECode LocalSocketImpl::GetPeerCredentials(
    /* [out] */ ICredentials** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    return NativeGetPeerCredentials(mFd, result);
#endif
}

ECode LocalSocketImpl::GetSockAddress(
    /* [out] */ ILocalSocketAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = NULL;
    return NOERROR;
    //TODO implement this
    //return getSockName_native(fd);
#endif
}

ECode LocalSocketImpl::Finalize()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        close();
#endif
}

//====================================================================================
//              LocalSocketImpl::SocketInputStream
//====================================================================================
ECode LocalSocketImpl::SocketInputStream::Available(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                FileDescriptor myFd = fd;
                if (myFd == NULL) throw new IOException("socket closed");
                return available_native(myFd);
#endif
}

ECode LocalSocketImpl::SocketInputStream::Close()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    return mOwner->Close();
#endif
}

ECode LocalSocketImpl::SocketInputStream::Read(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    synchronized(mOwner->mReadMonitor) {
        AutoPtr<IFileDescriptor> myFd = mOwner->mFd;
        if (myFd == NULL) {
            // throw new IOException("socket closed");
            return E_IO_EXCEPTION;
        }
    }
    return mOwner->NativeRead(myFd, result);
#endif
}

ECode LocalSocketImpl::SocketInputStream::Read(
    /* [in] */ ArrayOf<Byte>* b,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    assert(b != NULL);
    return ReadBytes(b, 0, b->GetLength(), result);
#endif
}

ECode LocalSocketImpl::SocketInputStream::Read(
    /* [in] */ ArrayOf<Byte>* b,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    synchronized(mOwner->mReadMonitor) {
        AutoPtr<IFileDescriptor> myFd = mOwner->mFd;
        if (myFd == NULL) {
            // throw new IOException("socket closed");
            return E_IO_EXCEPTION;
        }

        if (off < 0 || len < 0 || (off + len) > b->GetLength()) {
    //        throw new ArrayIndexOutOfBoundsException();
            return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
        }

    }
    return mOwner->NativeReadba(b, off, len, myFd, result);
#endif
}

//====================================================================================
//              LocalSocketImpl::SocketOutputStream
//====================================================================================

ECode LocalSocketImpl::SocketOutputStream::Close()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    return mOwner->Close();
#endif
}

ECode LocalSocketImpl::SocketOutputStream::Write(
    /* [in] */ ArrayOf<Byte>* b)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    return WriteBytes(b, 0, b.GetLength());
#endif
}

ECode LocalSocketImpl::SocketOutputStream::Write(
    /* [in] */ ArrayOf<Byte>* b,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    synchronized(mOwner->mWriteMonitor) {

        AutoPtr<IFileDescriptor> myFd = mOwner->mFd;
        if (myFd == NULL) {
            // throw new IOException("socket closed");
            return E_IO_EXCEPTION;
        }

        if (off < 0 || len < 0 || (off + len) > b.GetLength()) {
    //        throw new ArrayIndexOutOfBoundsException();
            return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
        }
    }
    return mOwner->NativeWriteba(b, off, len, myFd);
#endif
}

ECode LocalSocketImpl::SocketOutputStream::Write(
    /* [in] */ Int32 b)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    synchronized(mOwner->mWriteMonitor) {

        AutoPtr<IFileDescriptor> myFd = mOwner->mFd;
        if (myFd == NULL) {
            // throw new IOException("socket closed");
            return E_IO_EXCEPTION;
        }
    }
    return mOwner->NativeWrite(b, myFd);
#endif
}

ECode LocalSocketImpl::SocketOutputStream::Flush()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                FileDescriptor myFd = fd;
                if (myFd == NULL) throw new IOException("socket closed");
                while(pending_native(myFd) > 0) {
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException ie) {
                        return NOERROR;
                    }
                }
#endif
}

} // namespace Net
} // namespace Droid
} // namespace Elastos

#if 0 // Reserve old LocalSocketImpl.cpp temporarily for probably use.
LocalSocketImpl::SocketInputStream::SocketInputStream(
    /* [in] */ LocalSocketImpl* owner)
    : mOwner(owner)
{}

/**
 * An input stream for local sockets. Needed because we may
 * need to read ancillary data.
 */
ECode LocalSocketImpl::SocketInputStream::Available(
    /* [out] */ Int32* result)
{
    return mOwner->NativeAvailable(mOwner->mFd, result);
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
        FAIL_RETURN(ReadBytes(buffer.Get(), 0, toRead, &read));
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
    synchronized(mLock) {

        if (mFd == NULL) return NOERROR;
        FAIL_RETURN(NativeClose(mFd));
        mFd = NULL;
    }
    return NOERROR;
}
#endif