
#ifndef __CSSLCERTIFICATESOCKETFACTORY_H__
#define __CSSLCERTIFICATESOCKETFACTORY_H__

#include "_CSSLCertificateSocketFactory.h"

using namespace Org::Apache::Harmony::Xnet::Provider::Jsse;

using Elastos::Net::ISocket;
using Elastos::Net::ISSLSocketFactory;
using Elastos::Net::ISSLSessionCache;

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * SSLSocketFactory implementation with several extra features:
 *
 * <ul>
 * <li>Timeout specification for SSL handshake operations
 * <li>Hostname verification in most cases (see WARNINGs below)
 * <li>Optional SSL session caching with {@link SSLSessionCache}
 * <li>Optionally bypass all SSL certificate checks
 * </ul>
 *
 * The handshake timeout does not apply to actual TCP socket connection.
 * If you want a connection timeout as well, use {@link #createSocket()}
 * and {@link Socket#connect(SocketAddress, int)}, after which you
 * must verify the identity of the server you are connected to.
 *
 * <p class="caution"><b>Most {@link SSLSocketFactory} implementations do not
 * verify the server's identity, allowing man-in-the-middle attacks.</b>
 * This implementation does check the server's certificate hostname, but only
 * for createSocket variants that specify a hostname.  When using methods that
 * use {@link InetAddress} or which return an unconnected socket, you MUST
 * verify the server's identity yourself to ensure a secure connection.</p>
 *
 * <p>One way to verify the server's identity is to use
 * {@link HttpsURLConnection#getDefaultHostnameVerifier()} to get a
 * {@link HostnameVerifier} to verify the certificate hostname.
 *
 * <p>On development devices, "setprop socket.relaxsslcheck yes" bypasses all
 * SSL certificate and hostname checks for testing purposes.  This setting
 * requires root access.
 */

CarClass(CSSLCertificateSocketFactory), public CSSLSocketFactory
{
public:
    friend class CSSLCertificateSocketFactoryHelper;

    /** @deprecated Use {@link #getDefault(int)} instead. */
    //@Deprecated
    constructor(
        /* [in] */ Int32 handshakeTimeoutMillis);

    CSSLCertificateSocketFactory

    /**
     * Sets the {@link TrustManager}s to be used for connections made by this factory.
     */
    CARAPI SetTrustManagers(
        /* [in] */ ArrayOf<ITrustManager>* trustManager);

    /**
     * Sets the <a href="http://technotes.googlecode.com/git/nextprotoneg.html">Next
     * Protocol Negotiation (NPN)</a> protocols that this peer is interested in.
     *
     * <p>For servers this is the sequence of protocols to advertise as
     * supported, in order of preference. This list is sent unencrypted to
     * all clients that support NPN.
     *
     * <p>For clients this is a list of supported protocols to match against the
     * server's list. If there is no protocol supported by both client and
     * server then the first protocol in the client's list will be selected.
     * The order of the client's protocols is otherwise insignificant.
     *
     * @param npnProtocols a non-empty list of protocol byte arrays. All arrays
     *     must be non-empty and of length less than 256.
     */
    CARAPI SetNpnProtocols(
        /* [in] */ ArrayOf<Byte>* npnProtocols);

    /**
     * Returns the <a href="http://technotes.googlecode.com/git/nextprotoneg.html">Next
     * Protocol Negotiation (NPN)</a> protocol selected by client and server, or
     * null if no protocol was negotiated.
     *
     * @param socket a socket created by this factory.
     * @throws IllegalArgumentException if the socket was not created by this factory.
     */
    CARAPI GetNpnSelectedProtocol(
        /* [in] */ ISocket* socket,
        /* [out, callee] */ ArrayOf<Byte>* protocol);

    /**
     * Sets the {@link KeyManager}s to be used for connections made by this factory.
     */
    CARAPI SetKeyManagers(
        /* [in] */ ArrayOf<IKeyManager>* keyManagers);

    /**
     * Enables <a href="http://tools.ietf.org/html/rfc5077#section-3.2">session ticket</a>
     * support on the given socket.
     *
     * @param socket a socket created by this factory
     * @param useSessionTickets {@code true} to enable session ticket support on this socket.
     * @throws IllegalArgumentException if the socket was not created by this factory.
     */
    CARAPI SetUseSessionTickets(
        /* [in] */ ISocket* socket,
        /* [in] */ Boolean useSessionTickets);

    /**
     * Turns on <a href="http://tools.ietf.org/html/rfc6066#section-3">Server
     * Name Indication (SNI)</a> on a given socket.
     *
     * @param socket a socket created by this factory.
     * @param hostName the desired SNI hostname, null to disable.
     * @throws IllegalArgumentException if the socket was not created by this factory.
     */
    CARAPI SetHostname(
        /* [in] */ ISocket* socket,
        /* [in] */ const String& hostName);

     /**
     * Sets this socket's SO_SNDTIMEO write timeout in milliseconds.
     * Use 0 for no timeout.
     * To take effect, this option must be set before the blocking method was called.
     *
     * @param socket a socket created by this factory.
     * @param timeout the desired write timeout in milliseconds.
     * @throws IllegalArgumentException if the socket was not created by this factory.
     *
     * @hide
     */
    CARAPI SetSoWriteTimeout(
        /* [in] */ ISocket* socket,
        /* [in] */ Int32 writeTimeoutMilliseconds);

    /**
     * {@inheritDoc}
     *
     * <p>This method verifies the peer's certificate hostname after connecting
     * (unless created with {@link #getInsecure(int, SSLSessionCache)}).
     */
    //@Override
    CARAPI CreateSocket(
        /* [in] */ ISocket* k,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ Boolean close,
        /* [out] */ ISocket** socket);

    /**
     * Creates a new socket which is not connected to any remote host.
     * You must use {@link Socket#connect} to connect the socket.
     *
     * <p class="caution"><b>Warning:</b> Hostname verification is not performed
     * with this method.  You MUST verify the server's identity after connecting
     * the socket to avoid man-in-the-middle attacks.</p>
     */
    //@Override
    CARAPI CreateSocket(
        /* [out] */ ISocket** socket);

    /**
     * {@inheritDoc}
     *
     * <p class="caution"><b>Warning:</b> Hostname verification is not performed
     * with this method.  You MUST verify the server's identity after connecting
     * the socket to avoid man-in-the-middle attacks.</p>
     */
    //@Override
    CARAPI CreateSocket(
        /* [in] */ IInetAddress* addr,
        /* [in] */ Int32 port,
        /* [in] */ IInetAddress* localAddr,
        /* [in] */ Int32 localPort,
        /* [out] */ ISocket** socket);

    /**
     * {@inheritDoc}
     *
     * <p class="caution"><b>Warning:</b> Hostname verification is not performed
     * with this method.  You MUST verify the server's identity after connecting
     * the socket to avoid man-in-the-middle attacks.</p>
     */
    //@Override
    CARAPI CreateSocket(
        /* [in] */ IInetAddress* addr,
        /* [in] */ Int32 port,
        /* [out] */ ISocket** socket);

    /**
     * {@inheritDoc}
     *
     * <p>This method verifies the peer's certificate hostname after connecting
     * (unless created with {@link #getInsecure(int, SSLSessionCache)}).
     */
    //@Override
    CARAPI CreateSocket(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ IInetAddress* localAddr,
        /* [in] */ Int32 localPort,
        /* [out] */ ISocket** socket);

    /**
     * {@inheritDoc}
     *
     * <p>This method verifies the peer's certificate hostname after connecting
     * (unless created with {@link #getInsecure(int, SSLSessionCache)}).
     */
    //@Override
    CARAPI CreateSocket(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [out] */ ISocket** socket);

    //@Override
    CARAPI GetDefaultCipherSuites(
        /* [out, callee] */ ArrayOf<String>* suites);

    //@Override
    CARAPI GetSupportedCipherSuites(
        /* [out, callee] */ ArrayOf<String>* suites);

private:

    class TrustManagerNative
    : public ElRefBase
    , public ITrustManager
    , public IX509TrustManager
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* object,
            /* [out] */ InterfaceID* iid);

        CARAPI GetAcceptedIssuers(
            /* [out] */ ArrayOf<IX509Certificate*>* issuers);

        CARAPI CheckClientTrusted(
            /* [in] */ ArrayOf<IX509Certificate*>* certs,
            /* [in] */ const String& authType);

        CARAPI CheckServerTrusted(
            /* [in] */ ArrayOf<IX509Certificate*>* certs,
            /* [in] */ const String& authType);
    };

    static const String TAG;// = "SSLCertificateSocketFactory";
    static ArrayOf<TrustManagerNative>* INSECURE_TRUST_MANAGER;
    static AutoPtr<IHostnameVerifier> HOSTNAME_VERIFIER;

    AutoPtr<ISSLSocketFactory> mInsecureFactory;
    AutoPtr<SSLSocketFactory> mSecureFactory;
    AutoPtr<ISSLClientSessionCache> mSessionCache;
    AutoPtr<ArrayOf<ITrustManager*> > mTrustManagers;
    AutoPtr<ArrayOf<IKeyManager*> > mKeyManagers;
    AutoPtr<ArrayOf<Byte> > mNpnProtocols;
    Int32 mHandshakeTimeoutMillis;
    Object mLock;

    CARAPI_(void) Init(
        /* [in] */ Int32 handshakeTimeoutMillis,
        /* [in] */ ISSLSessionCache* cache,
        /* [in] */ Boolean secure);

    CARAPI_(AutoPtr<ISSLSocketFactory>) MakeSocketFactory(
        /* [in] */ const ArrayOf<IKeyManager>& keyManagers,
        /* [in] */ const ArrayOf<ITrustManager>& trustManagers);

    static CARAPI_(Boolean) IsSslCheckRelaxed();

    /*synchronized*/ CARAPI_(AutoPtr<ISSLSocketFactory>) GetDelegate();

    /**
     * Returns an array containing the concatenation of length-prefixed byte
     * strings.
     */
    static CARAPI_(AutoPtr<ArrayOf<Byte> >) ToNpnProtocolsList(
        /* [in] */ ArrayOf<Byte*>* npnProtocols);

    static CARAPI_(AutoPtr<IOpenSSLSocketImpl>) CastToOpenSSLSocket(
        /* [in] */ ISocket* socket);
};

} //namespace Net
} //namespace Droid
} //namespace Elastos

#endif // __CSSLCERTIFICATESOCKETFACTORY_H__
