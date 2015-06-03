
#include "CIoBridge.h"
#include "CLibcore.h"
#include "COsConstants.h"
#include "AsynchronousSocketCloseMonitor.h"
#include "CInetSocketAddress.h"
#include "CStructLinger.h"
#include "CStructPollfd.h"
#include "CStructTimeval.h"
#include <elastos/Math.h>
#include "CInteger32.h"
#include "CBoolean.h"
#include "IoUtils.h"
#include "CFileDescriptor.h"
#include "CInet6Address.h"
#include "NetworkInterface.h"
#include "CSystem.h"

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::IO::IoUtils;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::CFileDescriptor;
using Elastos::Net::CInetSocketAddress;
using Elastos::Net::IInet4Address;
using Elastos::Net::IInet6Address;
using Elastos::Net::CInet6Address;
using Elastos::Net::ISocketAddress;
using Elastos::Net::ISocketOptions;
using Elastos::Net::INetworkInterface;
using Elastos::Net::NetworkInterface;
using Elastos::Net::ISocketOptions;

namespace Libcore {
namespace IO {

const Int32 CIoBridge::MCAST_JOIN_GROUP;
const Int32 CIoBridge::MCAST_LEAVE_GROUP;
const Int32 CIoBridge::IP_MULTICAST_TTL;

ECode CIoBridge::_Available(
    /* [in] */ Int32 fd,
    /* [out] */ Int32* avail)
{
    VALIDATE_NOT_NULL(avail);

//    try {
    Int32 available, result;
    FAIL_RETURN(CLibcore::sOs->IoctlInt(fd, COsConstants::sFIONREAD, &available, &result));
    if (available < 0) {
        // If the fd refers to a regular file, the result is the difference between
        // the file size and the file position. This may be negative if the position
        // is past the end of the file. If the fd refers to a special file masquerading
        // as a regular file, the result may be negative because the special file
        // may appear to have zero size and yet a previous read call may have
        // read some amount of data and caused the file position to be advanced.
        available = 0;
    }
    *avail = available;
    return NOERROR;
//    } catch (ErrnoException errnoException) {
//        if (errnoException.errno == ENOTTY) {
//            // The fd is unwilling to opine about its read buffer.
//            return 0;
//        }
//        throw errnoException.rethrowAsIOException();
//    }
}

ECode CIoBridge::_Bind(
    /* [in] */ Int32 fd,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port)
{
    AutoPtr<IInet6Address> inet6 = IInet6Address::Probe(inetAddress);
    Int32 scopeid = 0;
    if (inet6 != NULL && (inet6->GetScopeId(&scopeid), scopeid) == 0) {
       // Linux won't let you bind a link-local address without a scope id. Find one.
        AutoPtr<INetworkInterface> nif;
        NetworkInterface::GetByInetAddress(inetAddress, (INetworkInterface**)&nif);
        if (nif == NULL) {
            // throw new SocketException("Can't bind to a link-local address without a scope id: " + inetAddress);
            return E_LIBCORE_SOCKET_EXCEPTION;
        }
        // try {
        String hostname;
        inetAddress->GetHostName(&hostname);
        AutoPtr< ArrayOf<Byte> > outbyte;
        inetAddress->GetAddress((ArrayOf<Byte>**)&outbyte);
        Int32 index = 0;
        nif->GetIndex(&index);
        CInet6Address::GetByAddress(hostname, outbyte, index, (IInetAddress**)&inetAddress);
        // } catch (UnknownHostException ex) {
        //     // throw new AssertionError(ex); // Can't happen.
        // }
    }
    // try {
    return CLibcore::sOs->Bind(fd, inetAddress, port);
    // } catch (ErrnoException errnoException) {
    //    throw new BindException(errnoException.getMessage(), errnoException);
    // }
}

ECode CIoBridge::_Connect(
    /* [in] */ Int32 fd,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [out] */ Boolean* result)
{
//    try {
    return _Connect(fd, inetAddress, port, 0, result);
//    } catch (SocketTimeoutException ex) {
//        throw new AssertionError(ex); // Can't happen for a connect without a timeout.
//    }
}

ECode CIoBridge::_Connect(
    /* [in] */ Int32 fd,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [in] */ Int32 timeoutMs,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // try {
    return ConnectErrno(fd, inetAddress, port, timeoutMs, result);
    // } catch (ErrnoException errnoException) {
    //     throw new ConnectException(connectDetail(inetAddress, port, timeoutMs, errnoException), errnoException);
    // } catch (SocketException ex) {
    //     throw ex; // We don't want to doubly wrap these.
    // } catch (SocketTimeoutException ex) {
    //     throw ex; // We don't want to doubly wrap these.
    // } catch (IOException ex) {
    //     throw new SocketException(ex);
    // }
}

ECode CIoBridge::ConnectErrno(
    /* [in] */ Int32 fd,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [in] */ Int32 timeoutMs,
    /* [out] */ Boolean* result)
{
    // With no timeout, just call connect(2) directly.
    if (timeoutMs == 0) {
        FAIL_RETURN(CLibcore::sOs->Connect(fd, inetAddress, port));
        *result = TRUE;
        return NOERROR;
    }

    // For connect with a timeout, we:
    //   1. set the socket to non-blocking,
    //   2. connect(2),
    //   3. loop using poll(2) to decide whether we're connected, whether we should keep
    //      waiting, or whether we've seen a permanent failure and should give up,
    //   4. set the socket back to blocking.

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    AutoPtr<IFileDescriptor> outfd;
    CFileDescriptor::New((IFileDescriptor**)&outfd);
    outfd->SetDescriptor(fd);
    // 1. set the socket to non-blocking.
    IoUtils::SetBlocking(outfd, FALSE);

    // 2. call connect(2) non-blocking.
    Int64 finishTimeMs;
    system->GetCurrentTimeMillis(&finishTimeMs);
    finishTimeMs += timeoutMs;
    // try {
    ECode ec = CLibcore::sOs->Connect(fd, inetAddress, port);
    if (ec == NOERROR) {
        IoUtils::SetBlocking(outfd, TRUE); // 4. set the socket back to blocking.
        *result = TRUE; // We connected immediately.
        return NOERROR;
    }

    // } catch (ErrnoException errnoException) {
    //    if (errnoException.errno != EINPROGRESS) {
    //        throw errnoException;
    //    }
    //    // EINPROGRESS means we should keep trying...
    // }

    // 3. loop using poll(2).
    Int32 remainingTimeoutMs = 0;
    Int64 millis;

    Boolean isflag = FALSE;
    do {
        system->GetCurrentTimeMillis(&millis);
        remainingTimeoutMs = (Int32) (finishTimeMs - millis);
        if (remainingTimeoutMs <= 0) {
            // throw new SocketTimeoutException(connectDetail(inetAddress, port, timeoutMs, null));
            return E_SOCKET_TIMEOUT_EXCEPTION;
        }
    } while (!(CIoBridge::_IsConnected(fd, inetAddress, port, timeoutMs, remainingTimeoutMs, &isflag), isflag));
    IoUtils::SetBlocking(outfd, TRUE); // 4. set the socket back to blocking.
    *result = TRUE; // Or we'd have thrown.
    return NOERROR;
}

ECode CIoBridge::_CloseSocket(
    /* [in] */ Int32 fd)
{
    if (fd == -1) {
        // Socket.close doesn't throw if you try to close an already-closed socket.
        return NOERROR;
    }
    AsynchronousSocketCloseMonitor::signalBlockedThreads(fd);
    // try {
    return CLibcore::sOs->Close(fd);
    // } catch (ErrnoException errnoException) {
    //     // TODO: are there any cases in which we should throw?
    // }
}

ECode CIoBridge::_IsConnected(
    /* [in] */ Int32 fd,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [in] */ Int32 timeoutMs,
    /* [in] */ Int32 remainingTimeoutMs,
    /* [out] */ Boolean* isConnected)
{
    VALIDATE_NOT_NULL(isConnected);

    // ErrnoException cause;
    // try {
    ArrayOf_<IStructPollfd*, 1> pollFds;
    CStructPollfd::New(&(pollFds[0]));
    pollFds[0]->SetFd(fd);
    pollFds[0]->SetEvents((Int16)COsConstants::sPOLLOUT);
    Int32 rc;
    FAIL_RETURN(CLibcore::sOs->Poll(pollFds, remainingTimeoutMs, &rc));
    if (rc == 0) {
        *isConnected = FALSE; // Timeout.
        return NOERROR;
    }
    Int32 connectError;
    FAIL_RETURN(CLibcore::sOs->GetsockoptInt(fd,
            COsConstants::sSOL_SOCKET, COsConstants::sSO_ERROR, &connectError));
    if (connectError == 0) {
        *isConnected = TRUE; // Success!
        return NOERROR;
    }
    // throw new ErrnoException("isConnected", connectError); // The connect(2) failed.
    return E_LIBCORE_CONNECT_EXCEPTION;
    // } catch (ErrnoException errnoException) {
    //     if (!fd.valid()) {
    //         throw new SocketException("Socket closed");
    //     }
    //     if (errnoException.errno == EINTR) {
    //         return false; // Punt and ask the caller to try again.
    //     } else {
    //         cause = errnoException;
    //     }
    // }
    // TODO: is it really helpful/necessary to throw so many different exceptions?
    // String detail = connectDetail(inetAddress, port, timeoutMs, cause);
    // if (cause.errno == ECONNRESET || cause.errno == ECONNREFUSED ||
    //         cause.errno == EADDRNOTAVAIL || cause.errno == EADDRINUSE ||
    //         cause.errno == ENETUNREACH) {
    //     throw new ConnectException(detail, cause);
    // } else if (cause.errno == EACCES) {
    //     throw new SecurityException(detail, cause);
    // } else if (cause.errno == ETIMEDOUT) {
    //     throw new SocketTimeoutException(detail, cause);
    // }
    // throw new SocketException(detail, cause);
}

ECode CIoBridge::_GetSocketOption(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 option,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);

    // try {
    return GetSocketOptionErrno(fd, option, value);
    // } catch (ErrnoException errnoException) {
    //     throw errnoException.rethrowAsSocketException();
    // }
}

ECode CIoBridge::GetSocketOptionErrno(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 option,
    /* [out] */ IInterface** value)
{
    switch (option) {
    case ISocketOptions::IP_MULTICAST_IF:
    {
        // This is IPv4-only.
        AutoPtr<IInetAddress> iObj;
        CLibcore::sOs->GetsockoptInAddr(fd,
                COsConstants::sIPPROTO_IP, COsConstants::sIP_MULTICAST_IF, (IInetAddress**)&iObj);
        *value = iObj;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    case ISocketOptions::IP_MULTICAST_IF2:
    {
        // This is IPv6-only.
        Int32 socketopt;
        CLibcore::sOs->GetsockoptInt(fd,
                COsConstants::sIPPROTO_IPV6, COsConstants::sIPV6_MULTICAST_IF, &socketopt);
        AutoPtr<IInteger32> iObj;
        CInteger32::New(socketopt, (IInteger32**)&iObj);
        *value = iObj;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    case ISocketOptions::IP_MULTICAST_LOOP:
    {
        // Since setting this from java.net always sets IPv4 and IPv6 to the same value,
        // it doesn't matter which we return.
        Int32 socketopt;
        CLibcore::sOs->GetsockoptInt(fd,
                COsConstants::sIPPROTO_IPV6, COsConstants::sIPV6_MULTICAST_LOOP, &socketopt);
        Boolean b = BooleanFromInt(socketopt);
        AutoPtr<IBoolean> bObj;
        CBoolean::New(b, (IBoolean**)&bObj);
        *value = bObj;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    case IP_MULTICAST_TTL:
    {
        // Since setting this from java.net always sets IPv4 and IPv6 to the same value,
        // it doesn't matter which we return.
        Int32 socketopt;
        CLibcore::sOs->GetsockoptInt(fd,
                COsConstants::sIPPROTO_IPV6, COsConstants::sIPV6_MULTICAST_HOPS, &socketopt);
        AutoPtr<IInteger32> iObj;
        CInteger32::New(socketopt, (IInteger32**)&iObj);
        *value = iObj;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    case ISocketOptions::IP_TOS:
    {
        // Since setting this from java.net always sets IPv4 and IPv6 to the same value,
        // it doesn't matter which we return.
        Int32 socketopt;
        CLibcore::sOs->GetsockoptInt(fd,
                COsConstants::sIPPROTO_IPV6, COsConstants::sIPV6_TCLASS, &socketopt);
        AutoPtr<IInteger32> iObj;
        CInteger32::New(socketopt, (IInteger32**)&iObj);
        *value = iObj;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    case ISocketOptions::_SO_BROADCAST:
    {
        Int32 socketopt;
        CLibcore::sOs->GetsockoptInt(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_BROADCAST, &socketopt);
        Boolean b = BooleanFromInt(socketopt);
        AutoPtr<IBoolean> bObj;
        CBoolean::New(b, (IBoolean**)&bObj);
        *value = bObj;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    case ISocketOptions::SO_KEEPALIVE:
    {
        Int32 socketopt;
        CLibcore::sOs->GetsockoptInt(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_KEEPALIVE, &socketopt);
        Boolean b = BooleanFromInt(socketopt);
        AutoPtr<IBoolean> bObj;
        CBoolean::New(b, (IBoolean**)&bObj);
        *value = bObj;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    case ISocketOptions::SO_LINGER:
    {
        AutoPtr<IStructLinger> linger;
        CLibcore::sOs->GetsockoptLinger(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_LINGER, (IStructLinger**)&linger);
        Boolean isOn;
        if (linger->IsOn(&isOn), !isOn) {
            AutoPtr<IBoolean> bObj;
            CBoolean::New(FALSE, (IBoolean**)&bObj);
            *value = bObj;
            REFCOUNT_ADD(*value);
            return NOERROR;
        }
        Int32 l;
        linger->GetLinger(&l);
        AutoPtr<IInteger32> iObj;
        CInteger32::New(l, (IInteger32**)&iObj);
        *value = iObj;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    case ISocketOptions::SO_OOBINLINE:
    {
        Int32 socketopt;
        CLibcore::sOs->GetsockoptInt(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_OOBINLINE, &socketopt);
        Boolean b = BooleanFromInt(socketopt);
        AutoPtr<IBoolean> bObj;
        CBoolean::New(b, (IBoolean**)&bObj);
        *value = bObj;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    case ISocketOptions::SO_RCVBUF:
    {
        Int32 socketopt;
        CLibcore::sOs->GetsockoptInt(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_SNDBUF, &socketopt);
        AutoPtr<IInteger32> iObj;
        CInteger32::New(socketopt, (IInteger32**)&iObj);
        *value = iObj;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    case ISocketOptions::SO_REUSEADDR:
    {
        Int32 socketopt;
        CLibcore::sOs->GetsockoptInt(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_REUSEADDR, &socketopt);
        Boolean b = BooleanFromInt(socketopt);
        AutoPtr<IBoolean> bObj;
        CBoolean::New(b, (IBoolean**)&bObj);
        *value = bObj;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    case ISocketOptions::SO_SNDBUF:
    {
        Int32 socketopt;
        CLibcore::sOs->GetsockoptInt(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_SNDBUF, &socketopt);
        AutoPtr<IInteger32> iObj;
        CInteger32::New(socketopt, (IInteger32**)&iObj);
        *value = iObj;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    case ISocketOptions::SO_TIMEOUT:
    {
        AutoPtr<IStructTimeval> tv;
        CLibcore::sOs->GetsockoptTimeval(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_RCVTIMEO, (IStructTimeval**)&tv);
        Int64 millis;
        tv->ToMillis(&millis);
        AutoPtr<IInteger32> iObj;
        CInteger32::New((Int32)millis, (IInteger32**)&iObj);
        *value = iObj;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    case ISocketOptions::TCP_NODELAY:
    {
        Int32 socketopt;
        CLibcore::sOs->GetsockoptInt(fd,
                COsConstants::sIPPROTO_TCP, COsConstants::sTCP_NODELAY, &socketopt);
        Boolean b = BooleanFromInt(socketopt);
        AutoPtr<IBoolean> bObj;
        CBoolean::New(b, (IBoolean**)&bObj);
        *value = bObj;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    default:
        // throw new SocketException("Unknown socket option: " + option);
        return E_LIBCORE_SOCKET_EXCEPTION;
    }
}

Boolean CIoBridge::BooleanFromInt(
    /* [in] */ Int32 i)
{
    return (i != 0);
}

Int32 CIoBridge::BooleanToInt(
    /* [in] */ Boolean b)
{
    return b ? 1 : 0;
}

ECode CIoBridge::_SetSocketOption(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 option,
    /* [in] */ IInterface* value)
{
    // try {
    return SetSocketOptionErrno(fd, option, value);
    // } catch (ErrnoException errnoException) {
    //     throw errnoException.rethrowAsSocketException();
    // }
}

ECode CIoBridge::SetSocketOptionErrno(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 option,
    /* [in] */ IInterface* value)
{
    switch (option) {
    case ISocketOptions::IP_MULTICAST_IF:
        // throw new UnsupportedOperationException("Use IP_MULTICAST_IF2 on Android");
        return E_LIBCORE_UNSUPPORTED_OPERATION_EXCEPTION;
    case ISocketOptions::IP_MULTICAST_IF2:
    {
        // Although IPv6 was cleaned up to use int, IPv4 uses an ip_mreqn containing an int.
        assert(IInteger32::Probe(value) != NULL);
        Int32 i;
        IInteger32::Probe(value)->GetValue(&i);
        FAIL_RETURN(CLibcore::sOs->SetsockoptIpMreqn(fd,
                COsConstants::sIPPROTO_IP, COsConstants::sIP_MULTICAST_IF, i));
        FAIL_RETURN(CLibcore::sOs->SetsockoptInt(fd,
                COsConstants::sIPPROTO_IPV6, COsConstants::sIPV6_MULTICAST_IF, i));
        return NOERROR;
    }
    case ISocketOptions::IP_MULTICAST_LOOP:
    {
        // Although IPv6 was cleaned up to use int, IPv4 multicast loopback uses a byte.
        assert(IBoolean::Probe(value) != NULL);
        Boolean b = FALSE;
        IBoolean::Probe(value)->GetValue(&b);
        FAIL_RETURN(CLibcore::sOs->SetsockoptByte(fd,
                COsConstants::sIPPROTO_IP, COsConstants::sIP_MULTICAST_LOOP, BooleanToInt(b)));
        FAIL_RETURN(CLibcore::sOs->SetsockoptInt(fd,
                COsConstants::sIPPROTO_IPV6, COsConstants::sIPV6_MULTICAST_LOOP, BooleanToInt(b)));
        return NOERROR;
    }
    case CIoBridge::IP_MULTICAST_TTL:
    {
        // Although IPv6 was cleaned up to use int, and IPv4 non-multicast TTL uses int,
        // IPv4 multicast TTL uses a byte.
        assert(IInteger32::Probe(value) != NULL);
        Int32 i;
        IInteger32::Probe(value)->GetValue(&i);
        FAIL_RETURN(CLibcore::sOs->SetsockoptByte(fd,
                COsConstants::sIPPROTO_IP, COsConstants::sIP_MULTICAST_TTL, i));
        FAIL_RETURN(CLibcore::sOs->SetsockoptInt(fd,
                COsConstants::sIPPROTO_IPV6, COsConstants::sIPV6_MULTICAST_HOPS, i));
        return NOERROR;
    }
    case ISocketOptions::IP_TOS:
    {
        assert(IInteger32::Probe(value) != NULL);
        Int32 i;
        IInteger32::Probe(value)->GetValue(&i);
        FAIL_RETURN(CLibcore::sOs->SetsockoptInt(fd,
                COsConstants::sIPPROTO_IP, COsConstants::sIP_TOS, i));
        FAIL_RETURN(CLibcore::sOs->SetsockoptInt(fd,
                COsConstants::sIPPROTO_IPV6, COsConstants::sIPV6_TCLASS, i));
        return NOERROR;
    }
    case ISocketOptions::_SO_BROADCAST:
    {
        assert(IBoolean::Probe(value) != NULL);
        Boolean b = FALSE;
        IBoolean::Probe(value)->GetValue(&b);
        FAIL_RETURN(CLibcore::sOs->SetsockoptInt(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_BROADCAST, BooleanToInt(b)));
        return NOERROR;
    }
    case ISocketOptions::SO_KEEPALIVE:
    {
        assert(IBoolean::Probe(value) != NULL);
        Boolean b = FALSE;
        IBoolean::Probe(value)->GetValue(&b);
        FAIL_RETURN(CLibcore::sOs->SetsockoptInt(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_KEEPALIVE, BooleanToInt(b)));
        return NOERROR;
    }
    case ISocketOptions::SO_LINGER:
    {
        Boolean on = FALSE;
        Int32 seconds = 0;
        if (IInteger32::Probe(value) != NULL) {
            on = TRUE;
            Int32 i;
            IInteger32::Probe(value)->GetValue(&i);
            seconds = Elastos::Utility::Min(i, 65535);
        }
        AutoPtr<IStructLinger> linger;
        CStructLinger::New(BooleanToInt(on), seconds, (IStructLinger**)&linger);
        FAIL_RETURN(CLibcore::sOs->SetsockoptLinger(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_LINGER, linger));
        return NOERROR;
    }
    case ISocketOptions::SO_OOBINLINE:
    {
        assert(IBoolean::Probe(value) != NULL);
        Boolean b = FALSE;
        IBoolean::Probe(value)->GetValue(&b);
        FAIL_RETURN(CLibcore::sOs->SetsockoptInt(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_OOBINLINE, BooleanToInt(b)));
        return NOERROR;
    }
    case ISocketOptions::SO_RCVBUF:
    {
        assert(IInteger32::Probe(value) != NULL);
        Int32 i;
        IInteger32::Probe(value)->GetValue(&i);
        FAIL_RETURN(CLibcore::sOs->SetsockoptInt(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_RCVBUF, i));
        return NOERROR;
    }
    case ISocketOptions::SO_REUSEADDR:
    {
        assert(IBoolean::Probe(value) != NULL);
        Boolean b = FALSE;
        IBoolean::Probe(value)->GetValue(&b);
        FAIL_RETURN(CLibcore::sOs->SetsockoptInt(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_REUSEADDR, BooleanToInt(b)));
        return NOERROR;
    }
    case ISocketOptions::SO_SNDBUF:
    {
        assert(IInteger32::Probe(value) != NULL);
        Int32 i;
        IInteger32::Probe(value)->GetValue(&i);
        FAIL_RETURN(CLibcore::sOs->SetsockoptInt(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_SNDBUF, i));
        return NOERROR;
    }
    case ISocketOptions::SO_TIMEOUT:
    {
        assert(IInteger32::Probe(value) != NULL);
        Int32 millis;
        IInteger32::Probe(value)->GetValue(&millis);
        AutoPtr<IStructTimeval> tv;
        CStructTimeval::FromMillis(millis, (IStructTimeval**)&tv);
        FAIL_RETURN(CLibcore::sOs->SetsockoptTimeval(fd,
                COsConstants::sSOL_SOCKET, COsConstants::sSO_RCVTIMEO, tv));
        return NOERROR;
    }
    case ISocketOptions::TCP_NODELAY:
    {
        assert(IBoolean::Probe(value) != NULL);
        Boolean b = FALSE;
        IBoolean::Probe(value)->GetValue(&b);
        FAIL_RETURN(CLibcore::sOs->SetsockoptInt(fd,
                COsConstants::sIPPROTO_TCP, COsConstants::sTCP_NODELAY, BooleanToInt(b)));
        return NOERROR;
    }
    case CIoBridge::MCAST_JOIN_GROUP:
    case CIoBridge::MCAST_LEAVE_GROUP:
    {
        assert(IStructGroupReq::Probe(value) != NULL);
        AutoPtr<IStructGroupReq> groupReq = IStructGroupReq::Probe(value);
        AutoPtr<IInetAddress> group;
        groupReq->GetGrGroup((IInetAddress**)&group);
        Int32 level = (IInet4Address::Probe(group) != NULL) ? COsConstants::sIPPROTO_IP : COsConstants::sIPPROTO_IPV6;
        Int32 op = (option == MCAST_JOIN_GROUP) ? MCAST_JOIN_GROUP : MCAST_LEAVE_GROUP;
        FAIL_RETURN(CLibcore::sOs->SetsockoptGroupReq(fd, level, op, groupReq));
        return NOERROR;
    }
    default:
        // throw new SocketException("Unknown socket option: " + option);
        return E_LIBCORE_SOCKET_EXCEPTION;
    }
}

ECode CIoBridge::_Open(
    /* [in] */ const String& path,
    /* [in] */ Int32 flags,
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);

    *fd = -1;
    AutoPtr<IStructStat> stat;
    // try {
    // On Android, we don't want default permissions to allow global access.
    Int32 mode = ((flags & COsConstants::sO_ACCMODE) == COsConstants::sO_RDONLY) ? 0 : 0600;
    FAIL_GOTO(CLibcore::sOs->Open(path, flags, mode, fd), FILE_NOT_FOUND);
    if (*fd != -1) {
        // Posix open(2) fails with EISDIR only if you ask for write permission.
        // Java disallows reading directories too.
        CLibcore::sOs->Fstat(*fd, (IStructStat**)&stat);
        stat->GetMode(&mode);
        if (COsConstants::_S_ISDIR(mode)) {
            // throw new ErrnoException("open", EISDIR);
            goto FILE_NOT_FOUND;
        }
    }
    return NOERROR;

FILE_NOT_FOUND:
    if (*fd != -1) {
        AutoPtr<CFileDescriptor> fDes;
        CFileDescriptor::NewByFriend((CFileDescriptor**)&fDes);
        fDes->SetDescriptor(*fd);
        IoUtils::Close(fDes);
    }
    return E_LIBCORE_FILE_NOT_FOUND_EXCEPTION;
}

ECode CIoBridge::_Read(
    /* [in] */ Int32 fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(bytes);
    VALIDATE_NOT_NULL(count);

    if ((byteOffset | byteCount) < 0 || byteOffset > bytes->GetLength() ||
        bytes->GetLength() - byteOffset < byteCount) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset, count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (byteCount == 0) {
        *count = 0;
        return NOERROR;
    }
    // try {
    FAIL_RETURN(CLibcore::sOs->Read(fd, bytes, byteOffset, byteCount, count));
    if (*count == 0) {
        *count = -1;
        return NOERROR;
    }
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     if (errnoException.errno == EAGAIN) {
    //         // We return 0 rather than throw if we try to read from an empty non-blocking pipe.
    //         return 0;
    //     }
    //     throw errnoException.rethrowAsIOException();
    // }
}

ECode CIoBridge::_Write(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte> & bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount)
{
    if ((byteOffset | byteCount) < 0 || byteOffset > bytes.GetLength() ||
        bytes.GetLength() - byteOffset < byteCount) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset, count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (byteCount == 0) {
        return NOERROR;
    }
    // try {
    while (byteCount > 0) {
        Int32 bytesWritten;
        FAIL_RETURN(CLibcore::sOs->Write(fd, bytes, byteOffset, byteCount, &bytesWritten));
        byteCount -= bytesWritten;
        byteOffset += bytesWritten;
    }
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     throw errnoException.rethrowAsIOException();
    // }
}

ECode CIoBridge::_Sendto(
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

    Boolean isDatagram = (inetAddress != NULL);
    if (!isDatagram && byteCount <= 0) {
        *result = 0;
        return NOERROR;
    }
    // try {
    return CLibcore::sOs->Sendto(fd, bytes, byteOffset, byteCount, flags, inetAddress, port, result);
    // } catch (ErrnoException errnoException) {
    //     result = maybeThrowAfterSendto(isDatagram, errnoException);
    // }
}

ECode CIoBridge::_Sendto(
    /* [in] */ Int32 fd,
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int32 flags,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    Boolean isDatagram = (inetAddress != NULL);
    Int32 remainvalue = 0;
    buffer->GetRemaining(&remainvalue);
    if (!isDatagram && remainvalue == 0) {
        *result = 0;
        return NOERROR;
    }
    // try {
    return CLibcore::sOs->SendtoEx(fd, buffer, flags, inetAddress, port, result);
    // } catch (ErrnoException errnoException) {
    //     result = maybeThrowAfterSendto(isDatagram, errnoException);
    // }
}

ECode CIoBridge::_Recvfrom(
    /* [in] */ Boolean isRead,
    /* [in] */ Int32 fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int32 flags,
    /* [in] */ IDatagramPacket* packet,
    /* [in] */ Boolean isConnected,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    // try {
    AutoPtr<IInetSocketAddress> srcAddress;
    if (packet != NULL && !isConnected) {
        CInetSocketAddress::New((IInetSocketAddress**)&srcAddress);
    }
    FAIL_RETURN(CLibcore::sOs->Recvfrom(fd, bytes, byteOffset, byteCount, flags, srcAddress, result));
    *result = PostRecvfrom(isRead, packet, isConnected, srcAddress, *result);
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     result = maybeThrowAfterRecvfrom(isRead, isConnected, errnoException);
    // }
    // return result;
}

ECode CIoBridge::_Recvfrom(
    /* [in] */ Boolean isRead,
    /* [in] */ Int32 fd,
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int32 flags,
    /* [in] */ IDatagramPacket* packet,
    /* [in] */ Boolean isConnected,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    // try {
    AutoPtr<IInetSocketAddress> srcAddress;
    if (packet != NULL && !isConnected) {
        CInetSocketAddress::New((IInetSocketAddress**)&srcAddress);
    }

    FAIL_RETURN(CLibcore::sOs->RecvfromEx(fd, buffer, flags, srcAddress, result));
    *result = PostRecvfrom(isRead, packet, isConnected, srcAddress, *result);
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     result = maybeThrowAfterRecvfrom(isRead, isConnected, errnoException);
    // }
}


Int32 CIoBridge::PostRecvfrom(
    /* [in] */ Boolean isRead,
    /* [in] */ IDatagramPacket* packet,
    /* [in] */ Boolean isConnected,
    /* [in] */ IInetSocketAddress* srcAddress,
    /* [in] */ Int32 byteCount)
{
    if (isRead && byteCount == 0) {
        return -1;
    }
    if (packet != NULL) {
        packet->SetLength(byteCount);
        if (!isConnected) {
            AutoPtr<IInetAddress> addr;
            srcAddress->GetAddress((IInetAddress**)&addr);
            Int32 port;
            srcAddress->GetPort(&port);
            packet->SetAddress(addr);
            packet->SetPort(port);
        }
    }
    return byteCount;
}

ECode CIoBridge::_Socket(
    /* [in] */ Boolean stream,
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);

    // try {
    FAIL_RETURN(CLibcore::sOs->Socket(COsConstants::sAF_INET6, stream ?
            COsConstants::sSOCK_STREAM : COsConstants::sSOCK_DGRAM, 0, fd));

    // The RFC (http://www.ietf.org/rfc/rfc3493.txt) says that IPV6_MULTICAST_HOPS defaults
    // to 1. The Linux kernel (at least up to 2.6.38) accidentally defaults to 64 (which
    // would be correct for the *unicast* hop limit).
    // See http://www.spinics.net/lists/netdev/msg129022.html, though no patch appears to
    // have been applied as a result of that discussion. If that bug is ever fixed, we can
    // remove this code. Until then, we manually set the hop limit on IPv6 datagram sockets.
    // (IPv4 is already correct.)
    if (!stream) {
        CLibcore::sOs->SetsockoptInt(*fd,
                COsConstants::sIPPROTO_IPV6, COsConstants::sIPV6_MULTICAST_HOPS, 1);
    }

    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     throw errnoException.rethrowAsSocketException();
    // }
}

ECode CIoBridge::_GetSocketLocalAddress(
    /* [in] */ Int32 fd,
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    // try {
    AutoPtr<ISocketAddress> sa;
    FAIL_RETURN(CLibcore::sOs->Getsockname(fd, (ISocketAddress**)&sa));
    assert(IInetSocketAddress::Probe(sa) != NULL);
    return IInetSocketAddress::Probe(sa)->GetAddress(address);
    // } catch (ErrnoException errnoException) {
    //     throw new AssertionError(errnoException);
    // }
}

ECode CIoBridge::_GetSocketLocalPort(
    /* [in] */ Int32 fd,
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);

    // try {
    AutoPtr<ISocketAddress> sa;
    FAIL_RETURN(CLibcore::sOs->Getsockname(fd, (ISocketAddress**)&sa));
    assert(IInetSocketAddress::Probe(sa) != NULL);
    return IInetSocketAddress::Probe(sa)->GetPort(port);
    // } catch (ErrnoException errnoException) {
    //     throw new AssertionError(errnoException);
    // }
}

ECode CIoBridge::Available(
    /* [in] */ Int32 fd,
    /* [out] */ Int32* avail)
{
    return _Available(fd, avail);
}

ECode CIoBridge::Bind(
    /* [in] */ Int32 fd,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port)
{
    return _Bind(fd, inetAddress, port);
}

ECode CIoBridge::Connect(
    /* [in] */ Int32 fd,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [out] */ Boolean* result)
{
//    try {
    return ConnectEx(fd, inetAddress, port, 0, result);
//    } catch (SocketTimeoutException ex) {
//        throw new AssertionError(ex); // Can't happen for a connect without a timeout.
//    }
}

ECode CIoBridge::ConnectEx(
    /* [in] */ Int32 fd,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [in] */ Int32 timeoutMs,
    /* [out] */ Boolean* result)
{
    return _Connect(fd, inetAddress, port, timeoutMs, result);
}

ECode CIoBridge::CloseSocket(
    /* [in] */ Int32 fd)
{
    return _CloseSocket(fd);
}

ECode CIoBridge::IsConnected(
    /* [in] */ Int32 fd,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [in] */ Int32 timeoutMs,
    /* [in] */ Int32 remainingTimeoutMs,
    /* [out] */ Boolean* isConnected)
{
    return _IsConnected(fd, inetAddress, port, timeoutMs, remainingTimeoutMs, isConnected);
}

ECode CIoBridge::GetSocketOption(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 option,
    /* [out] */ IInterface** value)
{
    return _GetSocketOption(fd, option, value);
}

ECode CIoBridge::SetSocketOption(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 option,
    /* [in] */ IInterface* value)
{
    return _SetSocketOption(fd, option, value);
}

ECode CIoBridge::Open(
    /* [in] */ const String& path,
    /* [in] */ Int32 flags,
    /* [out] */ Int32* fd)
{
    return _Open(path, flags, fd);
}

ECode CIoBridge::Read(
    /* [in] */ Int32 fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* count)
{
    return _Read(fd, bytes, byteOffset, byteCount, count);
}

ECode CIoBridge::Write(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount)
{
    return _Write(fd, bytes, byteOffset, byteCount);
}

ECode CIoBridge::Sendto(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int32 flags,
    /* [in] */ IInetAddress* inetAddress,
    /* [in] */ Int32 port,
    /* [out] */ Int32* result)
{
    return _Sendto(fd, bytes, byteOffset, byteCount, flags, inetAddress, port, result);
}

ECode CIoBridge::SendtoEx(
        /* [in] */ Int32 fd,
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int32 flags,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [out] */ Int32* result)
{
    return _Sendto(fd, buffer, flags, inetAddress, port, result);
}

ECode CIoBridge::Recvfrom(
    /* [in] */ Boolean isRead,
    /* [in] */ Int32 fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int32 flags,
    /* [in] */ IDatagramPacket* packet,
    /* [in] */ Boolean isConnected,
    /* [out] */ Int32* result)
{
    return _Recvfrom(isRead, fd, bytes, byteOffset, byteCount, flags, packet, isConnected, result);
}

ECode CIoBridge::RecvfromEx(
    /* [in] */ Boolean isRead,
    /* [in] */ Int32 fd,
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int32 flags,
    /* [in] */ IDatagramPacket* packet,
    /* [in] */ Boolean isConnected,
    /* [out] */ Int32* result)
{
    return _Recvfrom(isRead, fd, buffer, flags, packet, isConnected, result);
}

ECode CIoBridge::Socket(
    /* [in] */ Boolean stream,
    /* [out] */ Int32* fd)
{
    return _Socket(stream, fd);
}

ECode CIoBridge::GetSocketLocalAddress(
    /* [in] */ Int32 fd,
    /* [out] */ IInetAddress** address)
{
    return _GetSocketLocalAddress(fd, address);
}

ECode CIoBridge::GetSocketLocalPort(
    /* [in] */ Int32 fd,
    /* [out] */ Int32* port)
{
    return _GetSocketLocalPort(fd, port);
}

} // namespace IO
} // namespace Libcore
