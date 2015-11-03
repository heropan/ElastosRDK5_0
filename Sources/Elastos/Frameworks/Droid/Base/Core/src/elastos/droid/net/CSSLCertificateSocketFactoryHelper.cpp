
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/net/CSSLCertificateSocketFactory.h"
#include "elastos/droid/net/CSSLCertificateSocketFactoryHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(CSSLCertificateSocketFactoryHelper, Singleton, ISSLCertificateSocketFactoryHelper)

CAR_SINGLETON_IMPL(CSSLCertificateSocketFactoryHelper)

ECode CSSLCertificateSocketFactoryHelper::GetDefault(
    /* [in] */ Int32 handshakeTimeoutMillis,
    /* [out] */ ISocketFactory** result)
{
    return CSSLCertificateSocketFactory::GetDefault(handshakeTimeoutMillis, result);
}

ECode CSSLCertificateSocketFactoryHelper::GetDefault(
    /* [in] */ Int32 handshakeTimeoutMillis,
    /* [in] */ ISSLSessionCache* cache,
    /* [out] */ ISSLSocketFactory** result)
{
    return CSSLCertificateSocketFactory::GetDefault(handshakeTimeoutMillis, cache, result);
}

ECode CSSLCertificateSocketFactoryHelper::GetInsecure(
    /* [in] */ Int32 handshakeTimeoutMillis,
    /* [in] */ ISSLSessionCache* cache,
    /* [out] */ ISSLSocketFactory** result)
{
    return CSSLCertificateSocketFactory::GetInsecure(handshakeTimeoutMillis, cache, result);
}

ECode CSSLCertificateSocketFactoryHelper::GetHttpSocketFactory(
    /* [in] */ Int32 handshakeTimeoutMillis,
    /* [in] */ ISSLSessionCache* cache,
    /* [out] */ Org::Apache::Http::Conn::SSL::ISSLSocketFactory** result)
{
    return CSSLCertificateSocketFactory::GetHttpSocketFactory(handshakeTimeoutMillis, cache, result);
}

ECode CSSLCertificateSocketFactoryHelper::VerifyHostname(
    /* [in] */ ISocket* socket,
    /* [in] */ const String& hostname)
{
    return CSSLCertificateSocketFactory::VerifyHostname(socket, hostname);
}

ECode CSSLCertificateSocketFactoryHelper::ToLengthPrefixedList(
    /* [in] */ ArrayOf<Byte>* items,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return CSSLCertificateSocketFactory::ToLengthPrefixedList(items, result);
}

} //namespace Net
} //namespace Droid
} //namespace Elastos

#if 0 // old CSSLCertificateSocketFactoryHelper.cpp
/**
 * Returns a new socket factory instance with an optional handshake timeout.
 *
 * @param handshakeTimeoutMillis to use for SSL connection handshake, or 0
 *         for none.  The socket timeout is reset to 0 after the handshake.
 * @return a new SSLSocketFactory with the specified parameters
 */
ECode CSSLCertificateSocketFactoryHelper::GetDefault(
    /* [in] */ Int32 handshakeTimeoutMillis,
    /* [out] */ ISocketFactory** socketFactory)
{
    VALLIDATE_NOT_NULL(socketFactory);
    CSSLCertificateSocketFactory::New(handshakeTimeoutMillis, (ISSLCertificateSocketFactory**)socketFactory);
    *socketFactory->Init(handshakeTimeoutMillis, NULL, TRUE);
    return NOERROR;
}

/**
 * Returns a new socket factory instance with an optional handshake timeout
 * and SSL session cache.
 *
 * @param handshakeTimeoutMillis to use for SSL connection handshake, or 0
 *         for none.  The socket timeout is reset to 0 after the handshake.
 * @param cache The {@link SSLSessionCache} to use, or null for no cache.
 * @return a new SSLSocketFactory with the specified parameters
 */
ECode CSSLCertificateSocketFactoryHelper::GetDefault(
    /* [in] */ Int32 handshakeTimeoutMillis,
    /* [in] */ ISSLSessionCache* cache,
    /* [out] */ Elastos::Net::Ssl::ISSLSocketFactory** sslSocketFactory)
{
    VALLIDATE_NOT_NULL(sslSocketFactory);
    CSSLCertificateSocketFactory::New(handshakeTimeoutMillis, (ISSLCertificateSocketFactory**)sslSocketFactory);
    *sslSocketFactory->Init(handshakeTimeoutMillis, cache, TRUE);
    return NOERROR;
}

/**
 * Returns a new instance of a socket factory with all SSL security checks
 * disabled, using an optional handshake timeout and SSL session cache.
 *
 * <p class="caution"><b>Warning:</b> Sockets created using this factory
 * are vulnerable to man-in-the-middle attacks!</p>
 *
 * @param handshakeTimeoutMillis to use for SSL connection handshake, or 0
 *         for none.  The socket timeout is reset to 0 after the handshake.
 * @param cache The {@link SSLSessionCache} to use, or null for no cache.
 * @return an insecure SSLSocketFactory with the specified parameters
 */
ECode CSSLCertificateSocketFactoryHelper::GetInsecure(
    /* [in] */ Int32 handshakeTimeoutMillis,
    /* [in] */ ISSLSessionCache* cache,
    /* [out] */ Elastos::Net::Ssl::ISSLSocketFactory** sslSocketFactory)
{
    VALLIDATE_NOT_NULL(sslSocketFactory);
    CSSLCertificateSocketFactory::New(handshakeTimeoutMillis, (ISSLCertificateSocketFactory**)sslSocketFactory);
    *sslSocketFactory->Init(handshakeTimeoutMillis, cache, FALSE);
    return NOERROR;
}

/**
 * Returns a socket factory (also named SSLSocketFactory, but in a different
 * namespace) for use with the Apache HTTP stack.
 *
 * @param handshakeTimeoutMillis to use for SSL connection handshake, or 0
 *         for none.  The socket timeout is reset to 0 after the handshake.
 * @param cache The {@link SSLSessionCache} to use, or null for no cache.
 * @return a new SocketFactory with the specified parameters
 */
ECode CSSLCertificateSocketFactoryHelper::GetHttpSocketFactory(
    /* [in] */ Int32 handshakeTimeoutMillis,
    /* [in] */ ISSLSessionCache* cache,
    /* [out] */ Org::Apache::Http::Conn::Ssl::ISSLSocketFactory** sslSocketFactory)
{
    VALLIDATE_NOT_NULL(sslSocketFactory);
    AutoPtr<ISSLCertificateSocketFactory> certSocketFactory;
    CSSLCertificateSocketFactory::New(handshakeTimeoutMillis, (ISSLCertificateSocketFactory**)&certSocketFactory);
    Org::Apache::Http::Conn::Ssl::CSSLSocketFactory::New(certSocketFactory, sslSocketFactory);
    return NOERROR;
}

/**
* Verify the hostname of the certificate used by the other end of a
* connected socket.  You MUST call this if you did not supply a hostname
* to {@link #createSocket()}.  It is harmless to call this method
* redundantly if the hostname has already been verified.
*
* <p>Wildcard certificates are allowed to verify any matching hostname,
* so "foo.bar.example.com" is verified if the peer has a certificate
* for "*.example.com".
*
* @param socket An SSL socket which has been connected to a server
* @param hostname The expected hostname of the remote server
* @throws IOException if something goes wrong handshaking with the server
* @throws SSLPeerUnverifiedException if the server cannot prove its identity
*
* @hide
*/
ECode CSSLCertificateSocketFactoryHelper::VerifyHostname(
/* [in] */ ISocket* socket,
/* [in] */ const String& hostname)
{
    if (ISSLSocket::Probe(socket) == NULL) {
        //throw new IllegalArgumentException("Attempt to verify non-SSL socket");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (!CSSLCertificateSocketFactory::IsSslCheckRelaxed()) {
        // The code at the start of OpenSSLSocketImpl.startHandshake()
        // ensures that the call is idempotent, so we can safely call it.
        AutoPtr<ISSLSocket> ssl = ISSLSocket::Probe(socket);
        ssl->StartHandshake();

        AutoPtr<SSLSession> session = ssl->GetSession();
        if (session == NULL) {
            //throw new SSLException("Cannot verify SSL socket without session");
            return E_FAIL;
        }
        Boolean result;
        CSSLCertificateSocketFactory::HOSTNAME_VERIFIER->Verify(hostname, session, &result);
        if (!result) {
            //throw new SSLPeerUnverifiedException("Cannot verify hostname: " + hostname);
            return E_FAIL;
        }
    }
}
#endif