
#include "net/CSSLCertificateSocketFactory.h"
#include "os/SystemProperties.h"

using namespace Org::Apache::Http::Conn::Ssl;
using namespace Org::Apache::Harmony::Xnet::Provider::Jsse;
using Elastos::Droid::Content::IContext;
using Elastos::IO::IFile;
using Elastos::Net::ISocket;
using Elastos::Net::ISocketFactory;
using Elastos::Net::Ssl::ISSLSocketFactory;
using Elastos::Net::Ssl::ISSLSessionCache;
using Elastos::Net::Ssl::ITrustManager;
using Elastos::Net::Ssl::IKeyManager
using Elastos::Net::Ssl::IX509TrustManager;

namespace Elastos {
namespace Droid {
namespace Net {

const CSSLCertificateSocketFactory::String TAG = "SSLCertificateSocketFactory";
ArrayOf<TrustManagerNative>* CSSLCertificateSocketFactory::INSECURE_TRUST_MANAGER = new TrustManagerNative();
AutoPtr<IHostnameVerifier> CSSLCertificateSocketFactory::HOSTNAME_VERIFIER =
                                HttpsURLConnection::GetDefaultHostnameVerifier();


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
CSSLCertificateSocketFactory::TrustManagerNative::TrustManagerNative()
    : CTrustManager()
    , CX509TrustManager()
{
}

PInterface CSSLCertificateSocketFactory::TrustManagerNative::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    } else if (riid == EIID_ITrustManager) {
        return (ITrustManager*)this;
    } else if (riid == EIID_IX509TrustManager) {
        return (IX509TrustManager*)this;
    }
    return NULL;
}

UInt32 CSSLCertificateSocketFactory::TrustManagerNative::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CSSLCertificateSocketFactory::TrustManagerNative::Release()
{
    return ElRefBase::Release();
}

CSSLCertificateSocketFactory::TrustManagerNative::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CSSLCertificateSocketFactory::TrustManagerNative::GetAcceptedIssuers(
    /* [out] */ ArrayOf<IX509Certificate*>* issuers)
{
    *issuers = NULL;
    return NOERROR;
}

ECode  CSSLCertificateSocketFactory::TrustManagerNative::CheckClientTrusted(
    /* [in] */ ArrayOf<IX509Certificate*>* certs,
    /* [in] */ const String& authType)
{
    return NOERROR;
}

ECode  CSSLCertificateSocketFactory::TrustManagerNative::CheckServerTrusted(
    /* [in] */ ArrayOf<IX509Certificate*>* certs,
    /* [in] */ const String& authType)
{
    return NOERROR;
}

/** @deprecated Use {@link #getDefault(int)} instead. */
//@Deprecated
CSSLCertificateSocketFactory::constructor(
    /* [in] */ Int32 handshakeTimeoutMillis)
{
    Init(handshakeTimeoutMillis, NULL, TRUE);
}

void CSSLCertificateSocketFactory::Init(
    /* [in] */ Int32 handshakeTimeoutMillis,
    /* [in] */ ISSLSessionCache* cache,
    /* [in] */ Boolean secure)
{
    mHandshakeTimeoutMillis = handshakeTimeoutMillis;
    mSessionCache = cache == NULL ? NULL : cache->mSessionCache;
    mSecure = secure;
}

AutoPtr<ISSLSocketFactory> CSSLCertificateSocketFactory::MakeSocketFactory(
    /* [in] */ const ArrayOf<IKeyManager>& keyManagers,
    /* [in] */ const ArrayOf<ITrustManager>& trustManagers)
{
    //try {
        AutoPtr<IOpenSSLContextImpl> sslContext;
        COpenSSLContextImpl::New((IOpenSSLContextImpl**)&sslContext);
        sslContext->EngineInit(keyManagers, trustManagers, NULL);

        AutoPtr<IClientSessionContext> sessionContext;
        sslContext->EngineGetClientSessionContext((IClientSessionContext**)&sessionContext);
        sessionContext->SetPersistentCache(mSessionCache);

        AutoPtr<ISSLSocketFactory> factory;
        sslContext->EngineGetSocketFactory((ISSLSocketFactory**)&factory);

        return factory;
    //} catch (KeyManagementException e) {
    //    Log.wtf(TAG, e);
    //    return (SSLSocketFactory) SSLSocketFactory.getDefault();  // Fallback
    //}
}

Boolean CSSLCertificateSocketFactory::IsSslCheckRelaxed()
{
    return String("1").Equals(SystemProperties::Get("ro.debuggable")) &&
            String("yes").Equals(SystemProperties::Get("socket.relaxsslcheck"));
}

/*synchronized*/
AutoPtr<ISSLSocketFactory> CSSLCertificateSocketFactory::GetDelegate()
{
    Mutex::Autolock lock(mLock);
    // Relax the SSL check if instructed (for this factory, or systemwide)
    if (!mSecure || IsSslCheckRelaxed()) {
        if (mInsecureFactory == NULL) {
            if (mSecure) {
                //Log.w(TAG, "*** BYPASSING SSL SECURITY CHECKS (socket.relaxsslcheck=yes) ***");
            } else {
                //Log.w(TAG, "Bypassing SSL security checks at caller's request");
            }
            mInsecureFactory = MakeSocketFactory(mKeyManagers, CSSLCertificateSocketFactory::INSECURE_TRUST_MANAGER);
        }
        return mInsecureFactory;
    } else {
        if (mSecureFactory == NULL) {
            mSecureFactory = MakeSocketFactory(mKeyManagers, mTrustManagers);
        }
        return mSecureFactory;
    }
}

/**
 * Sets the {@link TrustManager}s to be used for connections made by this factory.
 */
ECode CSSLCertificateSocketFactory::SetTrustManagers(
    /* [in] */ ArrayOf<ITrustManager*>* trustManager)
{
    mTrustManagers = trustManager;

    // Clear out all cached secure factories since configurations have changed.
    mSecureFactory = NULL;
    // Note - insecure factories only ever use the INSECURE_TRUST_MANAGER so they need not
    // be cleared out here.
    return NOERROR;
}

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
ECode CSSLCertificateSocketFactory::SetNpnProtocols(
    /* [in] */ ArrayOf<Byte>* npnProtocols)
{
    mNpnProtocols = ToNpnProtocolsList(npnProtocols);
    if(mNpnProtocols == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Returns an array containing the concatenation of length-prefixed byte
 * strings.
 */
AutoPtr<ArrayOf<Byte> > CSSLCertificateSocketFactory::ToNpnProtocolsList(
    /* [in] */ ArrayOf<Byte>* npnProtocols)
{
    Int32 length;
    *npnProtocols.GetLength(&length);
    if (length == 0) {
        //throw new IllegalArgumentException("npnProtocols.length == 0");
        return NULL;
    }
    Int32 totalLength = 0;

    for (ArrayOf<Byte>* s : npnProtocols) {
        Int32 length2;
        s->GetLength(&length2);
        if (length2 == 0 || length2 > 255) {
            //throw new IllegalArgumentException("s.length == 0 || s.length > 255: " + s.length);
            return NULL;
        }
        totalLength += 1 + length2;
    }

    AutoPtr<ArrayOf<Byte> > result = ArrayOf<Byte>::Alloc(totalLength);
    Int32 pos = 0;
    for (ArrayOf<Byte>* s : npnProtocols) {
        Int32 length3;
        s->GetLength(&length3);
        result[pos++] = (Byte)length3;
        for (Byte b : *s) {
            result[pos++] = b;
        }
    }
    return result;
}

/**
 * Returns the <a href="http://technotes.googlecode.com/git/nextprotoneg.html">Next
 * Protocol Negotiation (NPN)</a> protocol selected by client and server, or
 * null if no protocol was negotiated.
 *
 * @param socket a socket created by this factory.
 * @throws IllegalArgumentException if the socket was not created by this factory.
 */
ECode CSSLCertificateSocketFactory::GetNpnSelectedProtocol(
    /* [in] */ ISocket* socket,
    /* [out, callee] */ ArrayOf<Byte>* protocol)
{
    VALLIDATE_NOT_NULL(protocol);

    AutpPtr<IOpenSSLSocketImpl> openSslSocketImpl;
    openSslSocketImpl = CastToOpenSSLSocket(socket);

    if(openSslSocketImpl == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return openSslSocketImpl->GetNpnSelectedProtocol(protocol);
}

/**
 * Sets the {@link KeyManager}s to be used for connections made by this factory.
 */
ECode CSSLCertificateSocketFactory::SetKeyManagers(
    /* [in] */ ArrayOf<IKeyManager*>* keyManagers)
{
    mKeyManagers = keyManagers;

    // Clear out any existing cached factories since configurations have changed.
    mSecureFactory = NULL;
    mInsecureFactory = NULL;
    return NOERROR;
}

/**
 * Enables <a href="http://tools.ietf.org/html/rfc5077#section-3.2">session ticket</a>
 * support on the given socket.
 *
 * @param socket a socket created by this factory
 * @param useSessionTickets {@code true} to enable session ticket support on this socket.
 * @throws IllegalArgumentException if the socket was not created by this factory.
 */
ECode CSSLCertificateSocketFactory::SetUseSessionTickets(
    /* [in] */ ISocket* socket,
    /* [in] */ Boolean useSessionTickets)
{
    AutpPtr<IOpenSSLSocketImpl> openSslSocketImpl;
    openSslSocketImpl = CastToOpenSSLSocket(socket);

    if(openSslSocketImpl == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return openSslSocketImpl->SetUseSessionTickets(useSessionTickets);
}

/**
 * Turns on <a href="http://tools.ietf.org/html/rfc6066#section-3">Server
 * Name Indication (SNI)</a> on a given socket.
 *
 * @param socket a socket created by this factory.
 * @param hostName the desired SNI hostname, null to disable.
 * @throws IllegalArgumentException if the socket was not created by this factory.
 */
ECode CSSLCertificateSocketFactory::SetHostname(
    /* [in] */ ISocket* socket,
    /* [in] */ const String& hostName)
{
    AutpPtr<IOpenSSLSocketImpl> openSslSocketImpl;
    openSslSocketImpl = CastToOpenSSLSocket(socket);

    if(openSslSocketImpl == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return openSslSocketImpl->SetHostname(hostName);
}

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
ECode CSSLCertificateSocketFactory::SetSoWriteTimeout(
    /* [in] */ ISocket* socket,
    /* [in] */ Int32 writeTimeoutMilliseconds)
{
    AutpPtr<IOpenSSLSocketImpl> openSslSocketImpl;
    openSslSocketImpl = CastToOpenSSLSocket(socket);

    if(openSslSocketImpl == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return openSslSocketImpl->SetSoWriteTimeout(writeTimeoutMilliseconds);
}

AutoPtr<IOpenSSLSocketImpl> CSSLCertificateSocketFactory::CastToOpenSSLSocket(
    /* [in] */ ISocket* socket)
{
    if((IOpenSSLSocketImpl*)socket->Probe(EIID_OpenSSLSocketImpl) == NULL) {
       // throw new IllegalArgumentException("Socket not created by this factory: "
       //     + socket);
       return NULL;
    }

    return (IOpenSSLSocketImpl*)socket->Probe(EIID_OpenSSLSocketImpl);
}


/**
 * {@inheritDoc}
 *
 * <p>This method verifies the peer's certificate hostname after connecting
 * (unless created with {@link #getInsecure(int, SSLSessionCache)}).
 */
//@Override
ECode CSSLCertificateSocketFactory::CreateSocket(
    /* [in] */ ISocket* k,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ Boolean close,
    /* [out] */ ISocket** socket)
{
    VALLIDATE_NOT_NULL(socket);

    GetDelegate()->CreateSocket(k, host, port, close, socket);
    ((IOpenSSLSocketImpl*)*socket)->SetNpnProtocols(mNpnProtocols);
    ((IOpenSSLSocketImpl*)*socket)->SetHandshakeTimeout(mHandshakeTimeoutMillis);
    if (mSecure) {
        AutoPtr<ISSLCertificateSocketFactoryHelper> helper;
        CSSLCertificateSocketFactoryHelper::AcquireSingleton((ISSLCertificateSocketFactoryHelper**)&helper);
        if(helper == NULL) {
            return E_FAIL;
        }
        return helper->VerifyHostname(*socket, host);
    }
    return NOERROR;
}

/**
 * Creates a new socket which is not connected to any remote host.
 * You must use {@link Socket#connect} to connect the socket.
 *
 * <p class="caution"><b>Warning:</b> Hostname verification is not performed
 * with this method.  You MUST verify the server's identity after connecting
 * the socket to avoid man-in-the-middle attacks.</p>
 */
//@Override
ECode CSSLCertificateSocketFactory::CreateSocketEx(
    /* [out] */ ISocket** socket)
{
    VALLIDATE_NOT_NULL(socket);

    GetDelegate()->CreateSocket(socket);
    ((IOpenSSLSocketImpl*)*socket)->SetNpnProtocols(mNpnProtocols);
    ((IOpenSSLSocketImpl*)*socket)->SetHandshakeTimeout(mHandshakeTimeoutMillis);
    return NOERROR;
}

/**
 * {@inheritDoc}
 *
 * <p class="caution"><b>Warning:</b> Hostname verification is not performed
 * with this method.  You MUST verify the server's identity after connecting
 * the socket to avoid man-in-the-middle attacks.</p>
 */
//@Override
ECode CreateSocketEx2(
    /* [in] */ IInetAddress* addr,
    /* [in] */ Int32 port,
    /* [in] */ IInetAddress* localAddr,
    /* [in] */ Int32 localPort,
    /* [out] */ ISocket** socket)
{
    VALLIDATE_NOT_NULL(socket);

    GetDelegate()->CreateSocket(addr, port, localAddr, localPort, socket);
    ((IOpenSSLSocketImpl*)*socket)->SetNpnProtocols(mNpnProtocols);
    ((IOpenSSLSocketImpl*)*socket)->SetHandshakeTimeout(mHandshakeTimeoutMillis);
    return NOERROR;
}

/**
 * {@inheritDoc}
 *
 * <p class="caution"><b>Warning:</b> Hostname verification is not performed
 * with this method.  You MUST verify the server's identity after connecting
 * the socket to avoid man-in-the-middle attacks.</p>
 */
//@Override
ECode CSSLCertificateSocketFactory::CreateSocketEx3(
    /* [in] */ IInetAddress* addr,
    /* [in] */ Int32 port,
    /* [out] */ ISocket** socket)
{
    VALLIDATE_NOT_NULL(socket);

    GetDelegate()->CreateSocket(addr, port, socket);
    ((IOpenSSLSocketImpl*)*socket)->SetNpnProtocols(mNpnProtocols);
    ((IOpenSSLSocketImpl*)*socket)->SetHandshakeTimeout(mHandshakeTimeoutMillis);
    return NOERROR;
}

/**
 * {@inheritDoc}
 *
 * <p>This method verifies the peer's certificate hostname after connecting
 * (unless created with {@link #getInsecure(int, SSLSessionCache)}).
 */
//@Override
ECode CSSLCertificateSocketFactory::CreateSocketEx4(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ IInetAddress* localAddr,
    /* [in] */ Int32 localPort,
    /* [out] */ ISocket** socket)
{
    VALLIDATE_NOT_NULL(socket);

    GetDelegate()->CreateSocket(host, port, localAddr, localPort, socket);
    ((IOpenSSLSocketImpl*)*socket)->SetNpnProtocols(mNpnProtocols);
    ((IOpenSSLSocketImpl*)*socket)->SetHandshakeTimeout(mHandshakeTimeoutMillis);
    if (mSecure) {
        AutoPtr<ISSLCertificateSocketFactoryHelper> helper;
        CSSLCertificateSocketFactoryHelper::AcquireSingleton((ISSLCertificateSocketFactoryHelper**)&helper);
        if(helper == NULL) {
            return E_FAIL;
        }
        return helper->VerifyHostname(*socket, host);
    }
    return NOERROR;
}

/**
 * {@inheritDoc}
 *
 * <p>This method verifies the peer's certificate hostname after connecting
 * (unless created with {@link #getInsecure(int, SSLSessionCache)}).
 */
//@Override
ECode CSSLCertificateSocketFactory::CreateSocketEx5(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [out] */ ISocket** socket)
{
    VALLIDATE_NOT_NULL(socket);

    GetDelegate()->CreateSocket(host, port, socket);
    ((IOpenSSLSocketImpl*)*socket)->SetNpnProtocols(mNpnProtocols);
    ((IOpenSSLSocketImpl*)*socket)->SetHandshakeTimeout(mHandshakeTimeoutMillis);
    if (mSecure) {
        AutoPtr<ISSLCertificateSocketFactoryHelper> helper;
        CSSLCertificateSocketFactoryHelper::AcquireSingleton((ISSLCertificateSocketFactoryHelper**)&helper);
        if(helper == NULL) {
            return E_FAIL;
        }
        return helper->VerifyHostname(*socket, host);
    }
    return NOERROR;
}

//@Override
ECode CSSLCertificateSocketFactory::GetDefaultCipherSuites(
    /* [out, callee] */ ArrayOf<String>* suites)
{
    return GetDelegate()->GetSupportedCipherSuites(suites);
}

//@Override
ECode CSSLCertificateSocketFactory::GetSupportedCipherSuites(
    /* [out, callee] */ ArrayOf<String>* suites)
{
    return GetDelegate()->GetSupportedCipherSuites(suites);
}

} //namespace Net
} //namespace Droid
} //namespace Elastos
