
#include "net/CLinkSocket.h"
#include "net/CLinkProperties.h"
#include "net/CLinkCapabilities.h"

using Elastos::Droid::Net::CLinkProperties;
using Elastos::Droid::Net::CLinkCapabilities;
using Elastos::Core::CObjectContainer;

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * Default constructor
 */
ECode CLinkSocket::constructor()
{
//    if (DBG) log("LinkSocket() EX");
    return NOERROR;
}

/**
 * Creates a new unconnected socket.
 * @param notifier a reference to a class that implements {@code LinkSocketNotifier}
 */
ECode CLinkSocket::constructor(
    /* [in] */ ILinkSocketNotifier* notifier)
{
//    if (DBG) log("LinkSocket(notifier) EX");
    return NOERROR;
}

/**
 * Creates a new unconnected socket usign the given proxy type.
 * @param notifier a reference to a class that implements {@code LinkSocketNotifier}
 * @param proxy the specified proxy for this socket
 * @throws IllegalArgumentException if the argument proxy is NULL or of an invalid type.
 * @throws SecurityException if a security manager exists and it denies the permission
 *                           to connect to the given proxy.
 */
ECode CLinkSocket::constructor(
    /* [in] */ ILinkSocketNotifier* notifier,
    /* [in] */ IProxy* proxy)
{
//    if (DBG) log("LinkSocket(notifier, proxy) EX");
    return NOERROR;
}

/**
 * @return the {@code LinkProperties} for the socket
 */
ECode CLinkSocket::GetLinkProperties(
    /* [out] */ ILinkProperties** result)
{
    VALIDATE_NOT_NULL(result);

//    if (DBG) log("LinkProperties() EX");
    AutoPtr<ILinkProperties> linkPro;
    CLinkProperties::New((ILinkProperties**)&linkPro);
    *result = linkPro;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Set the {@code LinkCapabilies} needed for this socket.  If the socket is already connected
 * or is a duplicate socket the request is ignored and {@code FALSE} will
 * be returned. A needs map can be created via the {@code createNeedsMap} static
 * method.
 * @param needs the needs of the socket
 * @return {@code true} if needs are successfully set, {@code FALSE} otherwise
 */
ECode CLinkSocket::SetNeededCapabilities(
    /* [in] */ ILinkCapabilities* needs,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
//    if (DBG) log("setNeeds() EX");
    *result = FALSE;
    return NOERROR;
}

/**
 * @return the LinkCapabilites set by setNeededCapabilities, empty if none has been set
 */
ECode CLinkSocket::GetNeededCapabilities(
    /* [out] */ ILinkCapabilities** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ILinkCapabilities> linkCap;
//    if (DBG) log("getNeeds() EX");
    *result = linkCap;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * @return all of the {@code LinkCapabilities} of the link used by this socket
 */
ECode CLinkSocket::GetCapabilities(
    /* [out] */ ILinkCapabilities** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ILinkCapabilities> linkCap;
    //    if (DBG) log("getCapabilities() EX");
    *result = linkCap;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Returns this LinkSockets set of capabilities, filtered according to
 * the given {@code Set}.  Capabilities in the Set but not available from
 * the link will not be reported in the results.  Capabilities of the link
 * but not listed in the Set will also not be reported in the results.
 * @param capabilities {@code Set} of capabilities requested
 * @return the filtered {@code LinkCapabilities} of this LinkSocket, may be empty
 */
ECode CLinkSocket::GetCapabilities(
    /* [in] */ IObjectContainer* capabilities,
    /* [out] */ ILinkCapabilities** result)
{
    VALIDATE_NOT_NULL(result);
//    if (DBG) log("getCapabilities(capabilities) EX");
    AutoPtr<ILinkCapabilities> linkCap;
    CLinkCapabilities::New((ILinkCapabilities**)&linkCap);
    *result = linkCap;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Provide the set of capabilities the application is interested in tracking
 * for this LinkSocket.
 * @param capabilities a {@code Set} of capabilities to track
 */
ECode CLinkSocket::SetTrackedCapabilities(
    /* [in] */ IObjectContainer* capabilities)
{
//    if (DBG) log("setTrackedCapabilities(capabilities) EX");
    return NOERROR;
}

/**
 * @return the {@code LinkCapabilities} that are tracked, empty if none has been set.
 */
ECode CLinkSocket::GetTrackedCapabilities(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IObjectContainer> out;
    CObjectContainer::New((IObjectContainer**)&out);
//    if (DBG) log("getTrackedCapabilities(capabilities) EX");
    *result = out;
    REFCOUNT_ADD(*result);
    return NOERROR;
}
/**
 * Connects this socket to the given remote host address and port specified
 * by the SocketAddress.
 * TODO add comment on all these that the network selection happens during connect
 * and may take 30 seconds
 * @deprecated Use {@code connect(String dstName, int dstPort)}
 *             Using this method may result in reduced functionality.
 * @param remoteAddr the address and port of the remote host to connect to.
 * @throws IllegalArgumentException if the SocketAddress is invalid or not supported.
 * @throws IOException if the socket is already connected or an error occurs
 *                     while connecting
 */
//@Override
ECode CLinkSocket::Connect(
    /* [in] */ ISocketAddress* remoteAddr)
{
//    if (DBG) log("connect(dstName, dstPort, timeout) EX");
    return NOERROR;
}

/**
 * Connects this socket to the given remote host address and port specified
 * by the SocketAddress with the specified timeout.
 * @deprecated Use {@code connect(String dstName, int dstPort, int timeout)}
 *             instead.  Using this method may result in reduced functionality.
 * @param remoteAddr the address and port of the remote host to connect to
 * @throws IllegalArgumentException if the given SocketAddress is invalid
 * @throws IOException if the socket is already connected or an error occurs
 *                     while connecting
 * @throws SocketTimeoutException if the timeout expires
 */
//@Override
ECode CLinkSocket::Connect(
    /* [in] */ ISocketAddress* remoteAddr,
    /* [in] */ Int32 timeout)
{
//    if (DBG) log("connect(dstName, dstPort, timeout) EX");
    return NOERROR;
}
/**
 * Connects this socket to the given remote host address and port specified
 * by dstName and dstPort.
 * @param dstName the address of the remote host to connect to
 * @param dstPort the port to connect to on the remote host
 * @param timeout the timeout value in milliseconds or 0 for infinite timeout
 * @throws UnknownHostException if the given dstName is invalid
 * @throws IOException if the socket is already connected or an error occurs
 *                     while connecting
 * @throws SocketTimeoutException if the timeout fires
 */
ECode CLinkSocket::Connect(
    /* [in] */ const String&  dstName,
    /* [in] */ Int32 dstPort,
    /* [in] */ Int32 timeout)
{
//    if (DBG) log("connect(remoteAddr, timeout) EX DEPRECATED");
    return NOERROR;
}
 /**
 * Connects this socket to the given remote host address and port specified
 * by dstName and dstPort.
 * @param dstName the address of the remote host to connect to
 * @param dstPort the port to connect to on the remote host
 * @throws UnknownHostException if the given dstName is invalid
 * @throws IOException if the socket is already connected or an error occurs
 *                     while connecting
 */
ECode CLinkSocket::Connect(
    /* [in] */ const String&  dstName,
    /* [in] */ Int32 dstPort)
{
//    if (DBG) log("connect(remoteAddr) EX DEPRECATED");
    return NOERROR;
}

/**
 * Connect a duplicate socket socket to the same remote host address and port
 * as the original with a timeout parameter.
 * @param timeout the timeout value in milliseconds or 0 for infinite timeout
 * @throws IOException if the socket is already connected or an error occurs
 *                     while connecting
 */
ECode CLinkSocket::Connect(
    /* [in] */ Int32 timeout)
{
//    if (DBG) log("connect(timeout) EX");
    return NOERROR;
}

/**
 * Connect a duplicate socket socket to the same remote host address and port
 * as the original.
 * @throws IOException if the socket is already connected or an error occurs
 *                     while connecting
 */
ECode CLinkSocket::Connect()
{
//    if (DBG) log("connect() EX");
    return NOERROR;
}

/**
 * Closes the socket.  It is not possible to reconnect or rebind to this
 * socket thereafter which means a new socket instance has to be created.
 * @throws IOException if an error occurs while closing the socket
 */
//@Override
ECode CLinkSocket::Close()
{
    Mutex::Autolock lock(mLock);
//    if (DBG) log("close() EX");
    return NOERROR;
}

/**
 * Request that a new LinkSocket be created using a different radio
 * (such as WiFi or 3G) than the current LinkSocket.  If a different
 * radio is available a call back will be made via {@code onBetterLinkAvail}.
 * If unable to find a better radio, application will be notified via
 * {@code onNewLinkUnavailable}
 * @see LinkSocketNotifier#onBetterLinkAvailable(LinkSocket, LinkSocket)
 * @param linkRequestReason reason for requesting a new link.
 */
ECode CLinkSocket::RequestNewLink(
    /* [in] */ ILinkRequestReason* linkRequestReason)
{
//    if (DBG) log("requestNewLink(linkRequestReason) EX");
    return NOERROR;
}

/**
 * @deprecated LinkSocket will automatically pick the optimum interface
 *             to bind to
 * @param localAddr the specific address and port on the local machine
 *                  to bind to
 * @throws IOException always as this method is deprecated for LinkSocket
 */
//@Override
//@Deprecated
ECode CLinkSocket::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
//    if (DBG) log("bind(localAddr) EX throws IOException");
//    throw new UnsupportedOperationException("bind is deprecated for LinkSocket");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CLinkSocket::IsBound(
    /* [out] */ Boolean* isBound)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::IsConnected(
    /* [out] */ Boolean* isConnected)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::ShutdownInput()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::ShutdownOutput()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::SetTrafficClass(
    /* [in] */ Int32 value)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetTrafficClass(
    /* [out] */ Int32* value)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::SetOOBInline(
    /* [in] */ Boolean oobinline)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetOOBInline(
    /* [out] */ Boolean* oobinline)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetInetAddress(
    /* [out] */ IInetAddress** address)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::SetKeepAlive(
    /* [in] */ Boolean keepAlive)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::IsInputShutdown(
    /* [out] */ Boolean* isInputShutdown)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::IsOutputShutdown(
    /* [out] */ Boolean* isOutputShutdown)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::SetReuseAddress(
    /* [in] */ Boolean reuse)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetReuseAddress(
    /* [out] */ Boolean* reuse)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetPort(
    /* [out] */ Int32* port)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetSoLinger(
    /* [out] */ Int32* value)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::SetSoLinger(
    /* [in] */ Boolean on,
    /* [in] */ Int32 timeout)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::SetSoTimeout(
    /* [in] */ Int32 timeout)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::SetTcpNoDelay(
    /* [in] */ Boolean on)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::SetSendBufferSize(
    /* [in] */ Int32 size)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetSendBufferSize(
    /* [out] */ Int32* size)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetSoTimeout(
    /* [out] */ Int32* timeout)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetTcpNoDelay(
    /* [out] */ Boolean* noDelay)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetLocalAddress(
    /* [out] */ IInetAddress** address)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetLocalPort(
    /* [out] */ Int32* localPort)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetKeepAlive(
    /* [out] */ Boolean* keepAlive)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::SendUrgentData(
    /* [in] */ Int32 value)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetInputStream(
    /* [out] */ IInputStream** is)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetOutputStream(
    /* [out] */ IOutputStream** os)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetLocalSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetRemoteSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetChannel(
    /* [out] */ ISocketChannel** channel)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fd)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CLinkSocket::SetPerformancePreferences(
    /* [in] */ Int32 connectionTime,
    /* [in] */ Int32 latency,
    /* [in] */ Int32 bandwidth)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}


} // namespace Net
} // namepsace Droid
} // namespace Elastos
