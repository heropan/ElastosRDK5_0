
#include "elastos/droid/net/http/CertificateChainValidator.h"
#include "elastos/droid/net/http/CCertificateChainValidator.h"

using Elastos::Droid::Utility::ISlog;

using Elastos::IO::IByteArrayInputStream;
using Elastos::Security::Cert::ICertificate;
using Elastos::Security::Cert::ICertificateFactory;
using Elastos::Security::IKeyStore;
using Elastosx::Net::Ssl::IHttpsURLConnection;
using Elastosx::Net::Ssl::IHttpsURLConnectionHelper;
using Elastosx::Net::Ssl::ISSLSession;
using Elastosx::Net::Ssl::ITrustManager;
using Elastosx::Net::Ssl::ITrustManagerFactory;

using Org::Conscrypt::ISSLParametersImpl;
using Org::Conscrypt::ITrustManagerImpl;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

//==============================================================================
// CertificateChainValidator::NoPreloadHolder
//==============================================================================
const AutoPtr<ICertificateChainValidator> CertificateChainValidator::NoPreloadHolder::sInstance = InitInstance();
const AutoPtr<IHostnameVerifier> CertificateChainValidator::NoPreloadHolder::sVerifier = InitVerifier();

AutoPtr<ICertificateChainValidator> CertificateChainValidator::NoPreloadHolder::InitInstance()
{
    AutoPtr<CCertificateChainValidator> ret;
    CCertificateChainValidator::NewByFriend((CCertificateChainValidator**)&ret);
    return (ICertificateChainValidator*) ret;
}

AutoPtr<IHostnameVerifier> CertificateChainValidator::NoPreloadHolder::InitVerifier()
{
    AutoPtr<IHostnameVerifier> ret;
    AutoPtr<IHttpsURLConnectionHelper> huc;

    // TODO: Waiting for CHttpsURLConnectionHelper
    assert(0);
    // CHttpsURLConnectionHelper::AcquireSingleton((IHttpsURLConnectionHelper**)&huc);
    // huc->GetDefaultHostnameVerifier(&ret);

    return ret;
}

//==============================================================================
// CertificateChainValidator
//==============================================================================
CAR_INTERFACE_IMPL(CertificateChainValidator, Object, ICertificateChainValidator)

const String CertificateChainValidator::TAG("CertificateChainValidator");

CertificateChainValidator::CertificateChainValidator()
{}

ECode CertificateChainValidator::GetInstance(
    /* [out] */ ICertificateChainValidator** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return NoPreloadHolder.sInstance;

#endif
}

ECode CertificateChainValidator::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            TrustManagerFactory tmf = TrustManagerFactory.getInstance("X.509");
            tmf.init((KeyStore) null);
            for (TrustManager tm : tmf.getTrustManagers()) {
                if (tm instanceof X509TrustManager) {
                    mTrustManager = (X509TrustManager) tm;
                }
            }
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException("X.509 TrustManagerFactory must be available", e);
        } catch (KeyStoreException e) {
            throw new RuntimeException("X.509 TrustManagerFactory cannot be initialized", e);
        }
        if (mTrustManager == null) {
            throw new RuntimeException(
                    "None of the X.509 TrustManagers are X509TrustManager");
        }

#endif
}

ECode CertificateChainValidator::DoHandshakeAndValidateServerCertificates(
    /* [in] */ IHttpsConnection* connection,
    /* [in] */ ISSLSocket* sslSocket,
    /* [in] */ const String& domain,
    /* [out] */ ISslError** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        // get a valid SSLSession, close the socket if we fail
        SSLSession sslSession = sslSocket.getSession();
        if (!sslSession.isValid()) {
            closeSocketThrowException(sslSocket, "failed to perform SSL handshake");
        }
        // retrieve the chain of the server peer certificates
        Certificate[] peerCertificates =
            sslSocket.getSession().getPeerCertificates();
        if (peerCertificates == null || peerCertificates.length == 0) {
            closeSocketThrowException(
                sslSocket, "failed to retrieve peer certificates");
        } else {
            // update the SSL certificate associated with the connection
            if (connection != null) {
                if (peerCertificates[0] != null) {
                    connection.setCertificate(
                        new SslCertificate((X509Certificate)peerCertificates[0]));
                }
            }
        }
        return verifyServerDomainAndCertificates((X509Certificate[]) peerCertificates, domain, "RSA");

#endif
}

ECode CertificateChainValidator::VerifyServerCertificates(
    /* [in] */ ArrayOf<IArrayOf>* certChain,
    /* [in] */ const String& domain,
    /* [in] */ const String& authType,
    /* [out] */ ISslError** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (certChain == null || certChain.length == 0) {
            throw new IllegalArgumentException("bad certificate chain");
        }
        X509Certificate[] serverCertificates = new X509Certificate[certChain.length];
        try {
            CertificateFactory cf = CertificateFactory.getInstance("X.509");
            for (int i = 0; i < certChain.length; ++i) {
                serverCertificates[i] = (X509Certificate) cf.generateCertificate(
                        new ByteArrayInputStream(certChain[i]));
            }
        } catch (CertificateException e) {
            throw new IOException("can't read certificate", e);
        }
        return verifyServerDomainAndCertificates(serverCertificates, domain, authType);

#endif
}

ECode CertificateChainValidator::HandleTrustStorageUpdate()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        TrustManagerFactory tmf;
        try {
            tmf = TrustManagerFactory.getInstance("X.509");
            tmf.init((KeyStore) null);
        } catch (NoSuchAlgorithmException e) {
            Slog.w(TAG, "Couldn't find default X.509 TrustManagerFactory");
            return;
        } catch (KeyStoreException e) {
            Slog.w(TAG, "Couldn't initialize default X.509 TrustManagerFactory", e);
            return;
        }
        TrustManager[] tms = tmf.getTrustManagers();
        boolean sentUpdate = false;
        for (TrustManager tm : tms) {
            try {
                Method updateMethod = tm.getClass().getDeclaredMethod("handleTrustStorageUpdate");
                updateMethod.setAccessible(true);
                updateMethod.invoke(tm);
                sentUpdate = true;
            } catch (Exception e) {
            }
        }
        if (!sentUpdate) {
            Slog.w(TAG, "Didn't find a TrustManager to handle CA list update");
        }

#endif
}

ECode CertificateChainValidator::VerifyServerDomainAndCertificates(
    /* [in] */ ArrayOf<IX509Certificate*>* chain,
    /* [in] */ const String& domain,
    /* [in] */ const String& authType,
    /* [out] */ ISslError** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check. Source code changed.
    VALIDATE_NOT_NULL(err);
    // check if the first certificate in the chain is for this site
    AutoPtr<IX509Certificate> currCertificate = (*chain)[0];
    if (currCertificate == NULL) {
        // throw new IllegalArgumentException("certificate for this site is null");
        // return E_ILLEGALARGUMENT_EXCEPTION;
    }

    Boolean verifier;
    // mVerifier->Verify(sDomain, currCertificate, &verifier);
    Boolean valid = !sDomain.IsNullOrEmpty() && verifier;
    if (!valid) {
        // if (HttpLog.LOGV) {
        //     HttpLog.v("certificate not for this host: " + domain);
        // }
        CSslError::New(ISslError::SSL_IDMISMATCH, currCertificate, err);
        return E_RUNTIME_EXCEPTION;
    }


    AutoPtr<IX509TrustManager> x509TrustManager;
    // CSSLParametersImpl::AcquireSingleton((ISSLParametersImpl**)&impl);
    // impl->GetDefaultTrustManager((IX509TrustManager**)&x509TrustManager);
    // AutoPtr<ITrustManagerImpl> tmImpl = x509TrustManager->Probe(EIID_ITrustManagerImpl);
    ECode res;
    // if (tmImpl != NULL) {
    //     res = tmImpl->CheckServerTrusted(chain, authType, domain);
    // } else {
    //     res = x509TrustManager->CheckServerTrusted(chain, authType);
    // }

    if(FAILED(res)) {
        // if (HttpLog.LOGV) {
        //     HttpLog.v("failed to validate the certificate chain, error: " +
        //         e.getMessage());
        // }
        CSslError::New(ISslError::SSL_UNTRUSTED, currCertificate, err);
        return res;
    }

    *err = NULL;
    return NOERROR;
#endif
}

ECode CertificateChainValidator::GetTrustManager(
    /* [out] */ IX509TrustManager** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mTrustManager;

#endif
}

ECode CertificateChainValidator::CloseSocketThrowException(
    /* [in] */ ISSLSocket* socket,
    /* [in] */ const String& errorMessage,
    /* [in] */ const String& defaultErrorMessage)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    return CloseSocketThrowException(
            socket, !errorMessage.IsNullOrEmpty() ? errorMessage : defaultErrorMessage);
#endif
}

ECode CertificateChainValidator::CloseSocketThrowException(
    /* [in] */ ISSLSocket* socket,
    /* [in] */ const String& errorMessage)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (HttpLog.LOGV) {
            HttpLog.v("validation error: " + errorMessage);
        }
        if (socket != null) {
            SSLSession session = socket.getSession();
            if (session != null) {
                session.invalidate();
            }
            socket.close();
        }
        throw new SSLHandshakeException(errorMessage);

#endif
}


} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
