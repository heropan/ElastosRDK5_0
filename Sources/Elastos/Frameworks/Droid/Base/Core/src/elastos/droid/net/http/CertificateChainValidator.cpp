
#include "CertificateChainValidator.h"

using Elastos::Net::Ssl::ISSLSession;
using Elastos::Net::Ssl::IX509TrustManager;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

AutoPtr<IDefaultHostnameVerifier> CertificateChainValidator::mVerifier;


CAR_INTERFACE_IMPL(CertificateChainValidator, ICertificateChainValidator);

CertificateChainValidator::CertificateChainValidator()
{}

ECode CertificateChainValidator::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode CertificateChainValidator::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode CertificateChainValidator::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CertificateChainValidator::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    ICertificateChainValidator * o = ICertificateChainValidator::Probe(other);
    if (o != NULL) {
        *result = (o == THIS_PROBE(ICertificateChainValidator));
    }
    return NOERROR;
}

ECode CertificateChainValidator::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = (Int32)THIS_PROBE(ICertificateChainValidator);
    return NOERROR;
}

ECode CertificateChainValidator::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    StringBuilder sb("CertificateChainValidator:(");
    sb += (Int32)THIS_PROBE(ICertificateChainValidator);
    sb += ")";
    sb.ToString(info);
    return NOERROR;
}


ECode CertificateChainValidator::DoHandshakeAndValidateServerCertificates(
    /* [in] */ Elastos::Droid::Net::Http::IHttpsConnection* connection,
    /* [in] */ ISSLSocket* sslSocket,
    /* [in] */ const String& sDomain,
    /* [out] */ Elastos::Droid::Net::Http::ISslError** err)
{
    // // get a valid SSLSession, close the socket if we fail
    // AutoPtr<ISSLSession> sslSession;
    // sslSocket->GetSession((ISSLSession**)&sslSession);
    // Boolean isValid;
    // sslSession.isValid(&isValid);
    // if (!isValid) {
    //     return CloseSocketThrowException(sslSocket, String("failed to perform SSL handshake");
    // }

    // // retrieve the chain of the server peer certificates
    // AutoPtr<ArrayOf<ICertificate*> > peerCertificates;
    // sslSession = NULL;
    // sslSocket->GetSession((ISSLSession**)&sslSession);
    // sslSession->GetPeerCertificates((ArrayOf<ICertificate*>*)&peerCertificates);

    // if (peerCertificates == NULL || peerCertificates->GetLength() == 0) {
    //     return CloseSocketThrowException(sslSocket, String("failed to retrieve peer certificates"));
    // } else {
    //     // update the SSL certificate associated with the connection
    //     if (connection != NULL) {
    //         if ((*peerCertificates)[0] != NULL) {
    //             AutoPtr<ISslCertificate> sslc;
    //             CSslCertificate::New((X509Certificate)((*peerCertificates)[0]), (ISslCertificate**)&sslc);
    //             connection->SetCertificate(sslc);
    //         }
    //     }
    // }

    // return VerifyServerDomainAndCertificates((X509Certificate[]) peerCertificates, sDomain, String("RSA"), err;
    return E_NOT_IMPLEMENTED;
}

ECode CertificateChainValidator::VerifyServerDomainAndCertificates(
    /* [in] */ ArrayOf<IX509Certificate*>* chain,
    /* [in] */ const String& sDomain,
    /* [in] */ const String& authType,
    /* [out] */ ISslError** err)
{
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
}

ECode CertificateChainValidator::CloseSocketThrowException(
        /* [in] */ ISSLSocket* socket,
        /* [in] */ const String& errorMessage,
        /* [in] */ const String& defaultErrorMessage)
{
    return CloseSocketThrowException(
            socket, !errorMessage.IsNullOrEmpty() ? errorMessage : defaultErrorMessage);
}

ECode CertificateChainValidator::CloseSocketThrowException(
        /* [in] */ ISSLSocket* socket,
        /* [in] */ const String& errorMessage)
{
    // if (HttpLog.LOGV) {
    //     HttpLog.v("validation error: " + errorMessage);
    // }

    if (socket != NULL) {
        AutoPtr<ISSLSession> session;
        // socket->GetSession(&session);
        // if (session != NULL) {
        //     session->Invalidate();
        // }

        // socket->Close();
    }

    // throw new SSLHandshakeException(errorMessage);
    // return E_SSLHANDS_SHAKE_EXCEPTION;
    return E_NOT_IMPLEMENTED;
}

}
}
}
}

