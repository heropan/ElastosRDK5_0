
#include "elastos/droid/net/SSLCertificateSocketFactory.h"

using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Utility::ILog;

using Elastos::Net::IInetAddress;
using Elastos::Net::ISocket;
using Elastos::Security::IPrivateKey;
using Elastosx::Net::ISocketFactory;
using Elastosx::Net::Ssl::EIID_ITrustManager;
using Elastosx::Net::Ssl::IHostnameVerifier;
using Elastosx::Net::Ssl::IHttpsURLConnection;
using Elastosx::Net::Ssl::IKeyManager;
using Elastosx::Net::Ssl::ISSLSession;
using Elastosx::Net::Ssl::ISSLSocket;
using Elastosx::Net::Ssl::ISSLSocketFactory;
using Elastosx::Net::Ssl::ITrustManager;
using Elastosx::Net::Ssl::IX509TrustManager;

using Org::Conscrypt::IOpenSSLContextImpl;
using Org::Conscrypt::IOpenSSLSocketImpl;
using Org::Conscrypt::ISSLClientSessionCache;

namespace Elastos {
namespace Droid {
namespace Net {

//==========================================================
// SSLCertificateSocketFactory::InnerSub_TrustManager
//==========================================================
CAR_INTERFACE_IMPL(SSLCertificateSocketFactory::InnerSub_TrustManager, Object, ITrustManager)

ECode SSLCertificateSocketFactory::InnerSub_TrustManager::GetAcceptedIssuers(
    /* [out, callee] */ ArrayOf<IX509Certificate*>** result)
{
    VALIDATE_NOT_NULL(result)

    *result = NULL;
    return NOERROR;
}

ECode SSLCertificateSocketFactory::InnerSub_TrustManager::CheckClientTrusted(
    /* [in] */ ArrayOf<IX509Certificate*>* certs,
    /* [in] */ const String& authType)
{
    return NOERROR;
}

ECode SSLCertificateSocketFactory::InnerSub_TrustManager::CheckServerTrusted(
    /* [in] */ ArrayOf<IX509Certificate*>* certs,
    /* [in] */ const String& authType)
{
    return NOERROR;
}

//==========================================================
// SSLCertificateSocketFactory
//==========================================================
CAR_INTERFACE_IMPL(SSLCertificateSocketFactory, SSLSocketFactory, ISSLCertificateSocketFactory)

const String SSLCertificateSocketFactory::TAG("SSLCertificateSocketFactory");

SSLCertificateSocketFactory::SSLCertificateSocketFactory()
    : mInsecureFactory(NULL)
    , mSecureFactory(NULL)
    , mTrustManagers(NULL)
    , mKeyManagers(NULL)
    , mNpnProtocols(NULL)
    , mAlpnProtocols(NULL)
    , mChannelIdPrivateKey(NULL)
{}

ECode SSLCertificateSocketFactory::constructor(
    /* [in] */ Int32 handshakeTimeoutMillis)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(handshakeTimeoutMillis, NULL, TRUE);
#endif
}

SSLCertificateSocketFactory::SSLCertificateSocketFactory(
    /* [in] */ Int32 handshakeTimeoutMillis,
    /* [in] */ ISSLSessionCache* cache,
    /* [in] */ Boolean secure)
    : mInsecureFactory(NULL)
    , mSecureFactory(NULL)
    , mTrustManagers(NULL)
    , mKeyManagers(NULL)
    , mNpnProtocols(NULL)
    , mAlpnProtocols(NULL)
    , mChannelIdPrivateKey(NULL)
{
#if 0 // TODO: Translate codes below
        mHandshakeTimeoutMillis = handshakeTimeoutMillis;
        mSessionCache = cache == NULL ? NULL : cache.mSessionCache;
        mSecure = secure;
#endif
}

ECode SSLCertificateSocketFactory::GetDefault(
    /* [in] */ Int32 handshakeTimeoutMillis,
    /* [out] */ ISocketFactory** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return new SSLCertificateSocketFactory(handshakeTimeoutMillis, NULL, TRUE);
#endif
}

ECode SSLCertificateSocketFactory::GetDefault(
    /* [in] */ Int32 handshakeTimeoutMillis,
    /* [in] */ ISSLSessionCache* cache,
    /* [out] */ ISSLSocketFactory** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return new SSLCertificateSocketFactory(handshakeTimeoutMillis, cache, TRUE);
#endif
}

ECode SSLCertificateSocketFactory::GetInsecure(
    /* [in] */ Int32 handshakeTimeoutMillis,
    /* [in] */ ISSLSessionCache* cache,
    /* [out] */ ISSLSocketFactory** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return new SSLCertificateSocketFactory(handshakeTimeoutMillis, cache, FALSE);
#endif
}

ECode SSLCertificateSocketFactory::GetHttpSocketFactory(
    /* [in] */ Int32 handshakeTimeoutMillis,
    /* [in] */ ISSLSessionCache* cache,
    /* [out] */ Org::Apache::Http::Conn::SSL::ISSLSocketFactory** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return new org.apache.http.conn.ssl.SSLSocketFactory(
                new SSLCertificateSocketFactory(handshakeTimeoutMillis, cache, TRUE));
#endif
}

ECode SSLCertificateSocketFactory::VerifyHostname(
    /* [in] */ ISocket* socket,
    /* [in] */ const String& hostname)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (!(ISSLSocket::Probe(socket) != NULL)) {
            throw new IllegalArgumentException("Attempt to verify non-SSL socket");
        }
        if (!isSslCheckRelaxed()) {
            // The code at the start of OpenSSLSocketImpl.startHandshake()
            // ensures that the call is idempotent, so we can safely call it.
            SSLSocket ssl = (SSLSocket) socket;
            ssl.startHandshake();
            SSLSession session = ssl.getSession();
            if (session == NULL) {
                throw new SSLException("Cannot verify SSL socket without session");
            }
            if (!HttpsURLConnection.getDefaultHostnameVerifier().verify(hostname, session)) {
                throw new SSLPeerUnverifiedException("Cannot verify hostname: " + hostname);
            }
        }
#endif
}

ECode SSLCertificateSocketFactory::MakeSocketFactory(
    /* [in] */ ArrayOf<IKeyManager*>* keyManagers,
    /* [in] */ ArrayOf<ITrustManager*>* trustManagers,
    /* [out] */ ISSLSocketFactory** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            OpenSSLContextImpl sslContext = new OpenSSLContextImpl();
            sslContext.engineInit(keyManagers, trustManagers, NULL);
            sslContext.engineGetClientSessionContext().setPersistentCache(mSessionCache);
            return sslContext.engineGetSocketFactory();
        } catch (KeyManagementException e) {
            Log.wtf(TAG, e);
            return (SSLSocketFactory) SSLSocketFactory.getDefault();  // Fallback
        }
#endif
}

ECode SSLCertificateSocketFactory::IsSslCheckRelaxed(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return "1".equals(SystemProperties.get("ro.debuggable")) &&
            "yes".equals(SystemProperties.get("socket.relaxsslcheck"));
#endif
}

ECode SSLCertificateSocketFactory::GetDelegate(
    /* [out] */ ISSLSocketFactory** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        // Relax the SSL check if instructed (for this factory, or systemwide)
        if (!mSecure || isSslCheckRelaxed()) {
            if (mInsecureFactory == NULL) {
                if (mSecure) {
                    Log.w(TAG, "*** BYPASSING SSL SECURITY CHECKS (socket.relaxsslcheck=yes) ***");
                } else {
                    Log.w(TAG, "Bypassing SSL security checks at caller's request");
                }
                mInsecureFactory = makeSocketFactory(mKeyManagers, INSECURE_TRUST_MANAGER);
            }
            return mInsecureFactory;
        } else {
            if (mSecureFactory == NULL) {
                mSecureFactory = makeSocketFactory(mKeyManagers, mTrustManagers);
            }
            return mSecureFactory;
        }
#endif
}

ECode SSLCertificateSocketFactory::SetTrustManagers(
    /* [in] */ ArrayOf<ITrustManager*>* trustManager)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mTrustManagers = trustManager;
        // Clear out all cached secure factories since configurations have changed.
        mSecureFactory = NULL;
        // Note - insecure factories only ever use the INSECURE_TRUST_MANAGER so they need not
        // be cleared out here.
#endif
}

ECode SSLCertificateSocketFactory::SetNpnProtocols(
    /* [in] */ ArrayOf<IArrayOf*>* npnProtocols)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.mNpnProtocols = toLengthPrefixedList(npnProtocols);
#endif
}

ECode SSLCertificateSocketFactory::SetAlpnProtocols(
    /* [in] */ ArrayOf<IArrayOf*>* protocols)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.mAlpnProtocols = toLengthPrefixedList(protocols);
#endif
}

ECode SSLCertificateSocketFactory::ToLengthPrefixedList(
    /* [in] */ ArrayOf<Byte>* items,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (items.length == 0) {
            throw new IllegalArgumentException("items.length == 0");
        }
        Int32 totalLength = 0;
        for (byte[] s : items) {
            if (s.length == 0 || s.length > 255) {
                throw new IllegalArgumentException("s.length == 0 || s.length > 255: " + s.length);
            }
            totalLength += 1 + s.length;
        }
        byte[] result = new byte[totalLength];
        Int32 pos = 0;
        for (byte[] s : items) {
            result[pos++] = (byte) s.length;
            for (byte b : s) {
                result[pos++] = b;
            }
        }
        return result;
#endif
}

ECode SSLCertificateSocketFactory::GetNpnSelectedProtocol(
    /* [in] */ ISocket* socket,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return castToOpenSSLSocket(socket).getNpnSelectedProtocol();
#endif
}

ECode SSLCertificateSocketFactory::GetAlpnSelectedProtocol(
    /* [in] */ ISocket* socket,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return castToOpenSSLSocket(socket).getAlpnSelectedProtocol();
#endif
}

ECode SSLCertificateSocketFactory::SetKeyManagers(
    /* [in] */ ArrayOf<IKeyManager*>* keyManagers)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mKeyManagers = keyManagers;
        // Clear out any existing cached factories since configurations have changed.
        mSecureFactory = NULL;
        mInsecureFactory = NULL;
#endif
}

ECode SSLCertificateSocketFactory::SetChannelIdPrivateKey(
    /* [in] */ IPrivateKey* privateKey)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mChannelIdPrivateKey = privateKey;
#endif
}

ECode SSLCertificateSocketFactory::SetUseSessionTickets(
    /* [in] */ ISocket* socket,
    /* [in] */ Boolean useSessionTickets)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        castToOpenSSLSocket(socket).setUseSessionTickets(useSessionTickets);
#endif
}

ECode SSLCertificateSocketFactory::SetHostname(
    /* [in] */ ISocket* socket,
    /* [in] */ const String& hostName)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        castToOpenSSLSocket(socket).setHostname(hostName);
#endif
}

ECode SSLCertificateSocketFactory::SetSoWriteTimeout(
    /* [in] */ ISocket* socket,
    /* [in] */ Int32 writeTimeoutMilliseconds)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        castToOpenSSLSocket(socket).setSoWriteTimeout(writeTimeoutMilliseconds);
#endif
}

ECode SSLCertificateSocketFactory::CastToOpenSSLSocket(
    /* [in] */ ISocket* socket,
    /* [out] */ IOpenSSLSocketImpl** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (!(IOpenSSLSocketImpl::Probe(socket) != NULL)) {
            throw new IllegalArgumentException("Socket not created by this factory: "
                    + socket);
        }
        return (OpenSSLSocketImpl) socket;
#endif
}

ECode SSLCertificateSocketFactory::CreateSocket(
    /* [in] */ ISocket* k,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ Boolean close,
    /* [out] */ ISocket** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        OpenSSLSocketImpl s = (OpenSSLSocketImpl) getDelegate().createSocket(k, host, port, close);
        s.setNpnProtocols(mNpnProtocols);
        s.setAlpnProtocols(mAlpnProtocols);
        s.setHandshakeTimeout(mHandshakeTimeoutMillis);
        s.setChannelIdPrivateKey(mChannelIdPrivateKey);
        if (mSecure) {
            verifyHostname(s, host);
        }
        return s;
#endif
}

ECode SSLCertificateSocketFactory::CreateSocket(
    /* [out] */ ISocket** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        OpenSSLSocketImpl s = (OpenSSLSocketImpl) getDelegate().createSocket();
        s.setNpnProtocols(mNpnProtocols);
        s.setAlpnProtocols(mAlpnProtocols);
        s.setHandshakeTimeout(mHandshakeTimeoutMillis);
        s.setChannelIdPrivateKey(mChannelIdPrivateKey);
        return s;
#endif
}

ECode SSLCertificateSocketFactory::CreateSocket(
    /* [in] */ IInetAddress* addr,
    /* [in] */ Int32 port,
    /* [in] */ IInetAddress* localAddr,
    /* [in] */ Int32 localPort,
    /* [out] */ ISocket** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        OpenSSLSocketImpl s = (OpenSSLSocketImpl) getDelegate().createSocket(
                addr, port, localAddr, localPort);
        s.setNpnProtocols(mNpnProtocols);
        s.setAlpnProtocols(mAlpnProtocols);
        s.setHandshakeTimeout(mHandshakeTimeoutMillis);
        s.setChannelIdPrivateKey(mChannelIdPrivateKey);
        return s;
#endif
}

ECode SSLCertificateSocketFactory::CreateSocket(
    /* [in] */ IInetAddress* addr,
    /* [in] */ Int32 port,
    /* [out] */ ISocket** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        OpenSSLSocketImpl s = (OpenSSLSocketImpl) getDelegate().createSocket(addr, port);
        s.setNpnProtocols(mNpnProtocols);
        s.setAlpnProtocols(mAlpnProtocols);
        s.setHandshakeTimeout(mHandshakeTimeoutMillis);
        s.setChannelIdPrivateKey(mChannelIdPrivateKey);
        return s;
#endif
}

ECode SSLCertificateSocketFactory::CreateSocket(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ IInetAddress* localAddr,
    /* [in] */ Int32 localPort,
    /* [out] */ ISocket** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        OpenSSLSocketImpl s = (OpenSSLSocketImpl) getDelegate().createSocket(
                host, port, localAddr, localPort);
        s.setNpnProtocols(mNpnProtocols);
        s.setAlpnProtocols(mAlpnProtocols);
        s.setHandshakeTimeout(mHandshakeTimeoutMillis);
        s.setChannelIdPrivateKey(mChannelIdPrivateKey);
        if (mSecure) {
            verifyHostname(s, host);
        }
        return s;
#endif
}

ECode SSLCertificateSocketFactory::CreateSocket(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [out] */ ISocket** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        OpenSSLSocketImpl s = (OpenSSLSocketImpl) getDelegate().createSocket(host, port);
        s.setNpnProtocols(mNpnProtocols);
        s.setAlpnProtocols(mAlpnProtocols);
        s.setHandshakeTimeout(mHandshakeTimeoutMillis);
        s.setChannelIdPrivateKey(mChannelIdPrivateKey);
        if (mSecure) {
            verifyHostname(s, host);
        }
        return s;
#endif
}

ECode SSLCertificateSocketFactory::GetDefaultCipherSuites(
    /* [out, callee] */ ArrayOf<String>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return getDelegate().getDefaultCipherSuites();
#endif
}

ECode SSLCertificateSocketFactory::GetSupportedCipherSuites(
    /* [out, callee] */ ArrayOf<String>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return getDelegate().getSupportedCipherSuites();
#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos

#if 0 // old CCSSLCertificateSocketFactory.cpp
#include "elastos/droid/net/CSSLCertificateSocketFactory.h"
#include "elastos/droid/os/SystemProperties.h"

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
    synchronized(mLock) {
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
        return CSSLCertificateSocketFactoryHelper::VerifyHostname(*socket, host);
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
ECode CSSLCertificateSocketFactory::CreateSocket(
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
ECode CreateSocket(
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
ECode CSSLCertificateSocketFactory::CreateSocket(
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
ECode CSSLCertificateSocketFactory::CreateSocket(
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
        if(helper == NULL) {
            return E_FAIL;
        }
        return CSSLCertificateSocketFactoryHelper::VerifyHostname(*socket, host);
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
ECode CSSLCertificateSocketFactory::CreateSocket(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [out] */ ISocket** socket)
{
    VALLIDATE_NOT_NULL(socket);

    GetDelegate()->CreateSocket(host, port, socket);
    ((IOpenSSLSocketImpl*)*socket)->SetNpnProtocols(mNpnProtocols);
    ((IOpenSSLSocketImpl*)*socket)->SetHandshakeTimeout(mHandshakeTimeoutMillis);
    if (mSecure) {
        if(helper == NULL) {
            return E_FAIL;
        }
        return CSSLCertificateSocketFactoryHelper::VerifyHostname(*socket, host);
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
#endif
