
#include <HttpConnection.h>
#include <IoUtils.h>
#include <InetAddress.h>
#include <CSocket.h>
#include <CInetSocketAddress.h>

using Elastos::IO::IoUtils;
using Elastos::Net::InetAddress;
using Elastos::Net::CSocket;
using Elastos::Net::CInetSocketAddress;

namespace Elastos {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL(HttpConnection::Address, IHttpConnectionAddress)

// HttpConnection::Address::Address(
//     /* [in] */ IURI* uri,
//     /* [in] */ ISSLSocketFactory* sslSocketFactory)
// {
//     this.proxy = null;
//     this.requiresTunnel = false;
//     this.uriHost = uri.getHost();
//     this.uriPort = uri.getEffectivePort();
//     this.sslSocketFactory = sslSocketFactory;
//     this.socketHost = uriHost;
//     this.socketPort = uriPort;
//     if (uriHost == null) {
//         throw new UnknownHostException(uri.toString());
//     }
// }

// HttpConnection::Address::Address(
//     /* [in] */ IURI* uri,
//     /* [in] */ ISSLSocketFactory* sslSocketFactory,
//     /* [in] */ IProxy* proxy,
//     /* [in] */ Boolean requiresTunnel)
// {
//     this.proxy = proxy;
//     this.requiresTunnel = requiresTunnel;
//     this.uriHost = uri.getHost();
//     this.uriPort = uri.getEffectivePort();
//     this.sslSocketFactory = sslSocketFactory;

//     SocketAddress proxyAddress = proxy.address();
//     if (!(proxyAddress instanceof InetSocketAddress)) {
//         throw new IllegalArgumentException("Proxy.address() is not an InetSocketAddress: "
//                 + proxyAddress.getClass());
//     }
//     InetSocketAddress proxySocketAddress = (InetSocketAddress) proxyAddress;
//     this.socketHost = proxySocketAddress.getHostName();
//     this.socketPort = proxySocketAddress.getPort();
//     if (uriHost == null) {
//         throw new UnknownHostException(uri.toString());
//     }
// }

ECode HttpConnection::Address::GetProxy(
    /* [out] */ IProxy** outproxy)
{
    VALIDATE_NOT_NULL(outproxy)

    *outproxy = mProxy;
    REFCOUNT_ADD(*outproxy)

    return NOERROR;
}

ECode HttpConnection::Address::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IHttpConnectionAddress> res = (IHttpConnectionAddress*)other->Probe(EIID_IHttpConnectionAddress);
    if (res) {
        AutoPtr<Address> that = (Address *)(res.Get());
        assert(0 && "TODO");
        *value = mProxy == that->mProxy
                && mUriHost.Equals(that->mUriHost)
                && mUriPort == that->mUriPort
                // && Objects.equal(this.sslSocketFactory, that.sslSocketFactory)
                && mRequiresTunnel == that->mRequiresTunnel;
    }
    else {
        *value = FALSE;
    }
    return NOERROR;
}

ECode HttpConnection::Address::GetHashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    Int32 result = 17;
    result = 31 * result + mUriHost.GetHashCode();
    result = 31 * result + mUriPort;
    assert(0 && "TODO");
    // result = 31 * result + (sslSocketFactory != null ? sslSocketFactory.hashCode() : 0);
    // result = 31 * result + (proxy != null ? proxy.hashCode() : 0);
    result = 31 * result + (mRequiresTunnel ? 1 : 0);
    *value = result;
    return NOERROR;
}

ECode HttpConnection::Address::Connect(
    /* [in] */ Int32 connectTimeout,
    /* [out] */ IHttpConnection** hc)
{
    VALIDATE_NOT_NULL(hc)

    AutoPtr<IHttpConnection> httc = (IHttpConnection*) new HttpConnection((IHttpConnectionAddress*)this->Probe(EIID_IHttpConnectionAddress), connectTimeout);
    *hc = httc;
    REFCOUNT_ADD(*hc)

    return NOERROR;
}

CAR_INTERFACE_IMPL(HttpConnection, IHttpConnection)

ECode HttpConnection::CloseSocketAndStreams()
{
    assert(0 && "TODO");
    // IoUtils.closeQuietly(sslOutputStream);
    // IoUtils.closeQuietly(sslInputStream);
    // IoUtils.closeQuietly(sslSocket);
    FAIL_RETURN(IoUtils::CloseQuietly(mOutputStream));
    FAIL_RETURN(IoUtils::CloseQuietly(mInputStream));
    // FAIL_RETURN(IoUtils::CloseQuietly(mSocket));
    return NOERROR;
}

ECode HttpConnection::SetSoTimeout(
    /* [in] */ Int32 readTimeout)
{
    return mSocket->SetSoTimeout(readTimeout);
}

ECode HttpConnection::GetOutputStream(
    /* [out] */ IOutputStream** os)
{
    VALIDATE_NOT_NULL(os)

    assert(0 && "TODO");
    // if (sslSocket != null) {
    //     if (sslOutputStream == null) {
    //         sslOutputStream = sslSocket.getOutputStream();
    //     }
    //     return sslOutputStream;
    // } else if(outputStream == null) {
    //     outputStream = socket.getOutputStream();
    // }
    // return outputStream;
    return NOERROR;
}

ECode HttpConnection::GetInputStream(
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is)

    assert(0 && "TODO");
    // if (sslSocket != null) {
    //     if (sslInputStream == null) {
    //         sslInputStream = sslSocket.getInputStream();
    //     }
    //     return sslInputStream;
    // } else if (inputStream == null) {

    //      * Buffer the socket stream to permit efficient parsing of HTTP
    //      * headers and chunk sizes. Benchmarks suggest 128 is sufficient.
    //      * We cannot buffer when setting up a tunnel because we may consume
    //      * bytes intended for the SSL socket.

    //     int bufferSize = 128;
    //     inputStream = address.requiresTunnel
    //             ? socket.getInputStream()
    //             : new BufferedInputStream(socket.getInputStream(), bufferSize);
    // }
    // return inputStream;
    return NOERROR;
}

ECode HttpConnection::GetAddress(
    /* [out] */ IHttpConnectionAddress** hca)
{
    VALIDATE_NOT_NULL(hca)

    *hca = mAddress;
    REFCOUNT_ADD(*hca)

    return NOERROR;
}

// public void setupSecureSocket(SSLSocketFactory sslSocketFactory, boolean tlsTolerant)
//         throws IOException {
//     // create the wrapper over connected socket
//     unverifiedSocket = (SSLSocket) sslSocketFactory.createSocket(socket,
//             address.uriHost, address.uriPort, true /* autoClose */);
//     // tlsTolerant mimics Chrome's behavior
//     if (tlsTolerant && unverifiedSocket instanceof OpenSSLSocketImpl) {
//         OpenSSLSocketImpl openSslSocket = (OpenSSLSocketImpl) unverifiedSocket;
//         openSslSocket.setUseSessionTickets(true);
//         openSslSocket.setHostname(address.uriHost);
//         // use SSLSocketFactory default enabled protocols
//     } else {
//         unverifiedSocket.setEnabledProtocols(new String [] { "SSLv3" });
//     }
//     // force handshake, which can throw
//     unverifiedSocket.startHandshake();
// }

// public SSLSocket verifySecureSocketHostname(HostnameVerifier hostnameVerifier)
//         throws IOException {
//     if (!hostnameVerifier.verify(address.uriHost, unverifiedSocket.getSession())) {
//         throw new IOException("Hostname '" + address.uriHost + "' was not verified");
//     }
//     sslSocket = unverifiedSocket;
//     return sslSocket;
// }

// public SSLSocket getSecureSocketIfConnected() {
//     return sslSocket;
// }

ECode HttpConnection::IsRecycled(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mRecycled;
    return NOERROR;
}

ECode HttpConnection::SetRecycled()
{
    mRecycled = TRUE;

    return NOERROR;
}

// public static HttpConnection connect(URI uri, SSLSocketFactory sslSocketFactory,
//         Proxy proxy, boolean requiresTunnel, int connectTimeout) throws IOException {
//     /*
//      * Try an explicitly-specified proxy.
//      */
//     if (proxy != null) {
//         Address address = (proxy.type() == Proxy.Type.DIRECT)
//                 ? new Address(uri, sslSocketFactory)
//                 : new Address(uri, sslSocketFactory, proxy, requiresTunnel);
//         return HttpConnectionPool.INSTANCE.get(address, connectTimeout);
//     }

//     /*
//      * Try connecting to each of the proxies provided by the ProxySelector
//      * until a connection succeeds.
//      */
//     ProxySelector selector = ProxySelector.getDefault();
//     List<Proxy> proxyList = selector.select(uri);
//     if (proxyList != null) {
//         for (Proxy selectedProxy : proxyList) {
//             if (selectedProxy.type() == Proxy.Type.DIRECT) {
//                 // the same as NO_PROXY
//                 // TODO: if the selector recommends a direct connection, attempt that?
//                 continue;
//             }
//             try {
//                 Address address = new Address(uri, sslSocketFactory,
//                         selectedProxy, requiresTunnel);
//                 return HttpConnectionPool.INSTANCE.get(address, connectTimeout);
//             } catch (IOException e) {
//                 // failed to connect, tell it to the selector
//                 selector.connectFailed(uri, selectedProxy.address(), e);
//             }
//         }
//     }

//     /*
//      * Try a direct connection. If this fails, this method will throw.
//      */
//     return HttpConnectionPool.INSTANCE.get(new Address(uri, sslSocketFactory), connectTimeout);
// }

AutoPtr<ISocket> HttpConnection::GetSocket()
{
    assert(0 && "TODO");
    // return sslSocket != null ? sslSocket : socket;
    return NULL;
}

Boolean HttpConnection::IsEligibleForRecycling()
{
    Boolean isflag = FALSE;
    Boolean isinput = FALSE;
    Boolean isoutput = FALSE;
    return (mSocket->IsClosed(&isflag), isflag = !isflag)
            && (mSocket->IsInputShutdown(&isinput), isinput = !isinput)
            && (mSocket->IsOutputShutdown(&isoutput), isoutput = !isoutput);
}

HttpConnection::HttpConnection(
    /* [in] */ IHttpConnectionAddress* config,
    /* [in] */ Int32 connectTimeout)
{
    mAddress = config;

    /*
     * Try each of the host's addresses for best behavior in mixed IPv4/IPv6
     * environments. See http://b/2876927
     * TODO: add a hidden method so that Socket.tryAllAddresses can does this for us
     */
     AutoPtr<ISocket> socketCandidate;
     ProxyType outtype;
    AutoPtr< ArrayOf<IInetAddress*> > addresses;
    InetAddress::GetAllByName(((Address*)config)->mSocketHost, (ArrayOf<IInetAddress*>**)&addresses);
    for (Int32 i = 0; i < addresses->GetLength(); i++) {
        socketCandidate = NULL;
        if (((Address*)config)->mProxy != NULL
            && (((Address*)config)->mProxy->GetType(&outtype), outtype) != ProxyType_HTTP)
        {
            CSocket::New(((Address*)config)->mProxy, (ISocket**)&socketCandidate);
        }
        else {
            CSocket::New((ISocket**)&socketCandidate);
        }

        // try {
        AutoPtr<IInetSocketAddress> inetscok;
        assert(0 && "TODO");
        // CInetSocketAddress::New((IInetAddress*)(*addresses)[i], ((Address*)config)->mSocketPort);
        socketCandidate->Connect(inetscok, connectTimeout);
        break;
        // } catch (IOException e) {
        //     if (i == addresses.length - 1) {
        //         throw e;
        //     }
        // }
    }

    mSocket = socketCandidate;
}

} // namespace Http
} // namespace Net
} // namespace Elastos
