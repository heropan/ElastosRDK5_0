
#ifndef __CSSLCERTIFICATESOCKETFACTORYHELPER_H__
#define __CSSLCERTIFICATESOCKETFACTORYHELPER_H__

#include "_CSSLCertificateSocketFactoryHelper.h"
#include "CSSLCertificateSocketFactory"

using namespace Org::Apache::Http::Conn::Ssl;

using Elastos::Net::ISocket;
using Elastos::Net::ISocketFactory;
//using Elastos::Net::ISSLSocketFactory;
using Elastos::Net::ISSLSessionCache;


namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CSSLCertificateSocketFactoryHelper)
{
public:
    /**
     * Returns a new socket factory instance with an optional handshake timeout.
     *
     * @param handshakeTimeoutMillis to use for SSL connection handshake, or 0
     *         for none.  The socket timeout is reset to 0 after the handshake.
     * @return a new SSLSocketFactory with the specified parameters
     */
    CARAPI GetDefault(
        /* [in] */ Int32 handshakeTimeoutMillis,
        /* [out] */ ISocketFactory** socketFactory);

    /**
     * Returns a new socket factory instance with an optional handshake timeout
     * and SSL session cache.
     *
     * @param handshakeTimeoutMillis to use for SSL connection handshake, or 0
     *         for none.  The socket timeout is reset to 0 after the handshake.
     * @param cache The {@link SSLSessionCache} to use, or null for no cache.
     * @return a new SSLSocketFactory with the specified parameters
     */
    CARAPI GetDefaultEx(
        /* [in] */ Int32 handshakeTimeoutMillis,
        /* [in] */ ISSLSessionCache* cache,
        /* [out] */ Elastos::Net::Ssl::ISSLSocketFactory** sslSocketFactory);

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
    CARAPI GetInsecure(
        /* [in] */ Int32 handshakeTimeoutMillis,
        /* [in] */ ISSLSessionCache* cache,
        /* [out] */ Elastos::Net::Ssl::ISSLSocketFactory** sslSocketFactory);

    /**
     * Returns a socket factory (also named SSLSocketFactory, but in a different
     * namespace) for use with the Apache HTTP stack.
     *
     * @param handshakeTimeoutMillis to use for SSL connection handshake, or 0
     *         for none.  The socket timeout is reset to 0 after the handshake.
     * @param cache The {@link SSLSessionCache} to use, or null for no cache.
     * @return a new SocketFactory with the specified parameters
     */
    CARAPI GetHttpSocketFactory(
        /* [in] */ Int32 handshakeTimeoutMillis,
        /* [in] */ ISSLSessionCache* cache,
        /* [out] */ Org::Apache::Http::Conn::Ssl::ISSLSocketFactory** sslSocketFactory);

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
     CARAPI VerifyHostname(
        /* [in] */ ISocket* socket,
        /* [in] */ const String& hostname);

private:

};

} //namespace Net
} //namespace Droid
} //namespace Elastos

#endif // __CSSLCERTIFICATESOCKETFACTORYHELPER_H__
