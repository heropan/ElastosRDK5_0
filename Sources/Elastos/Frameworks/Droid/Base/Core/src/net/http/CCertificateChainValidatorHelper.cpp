
#include "CCertificateChainValidatorHelper.h"

using Elastos::Net::Ssl::IX509TrustManager;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

AutoPtr<ICertificateChainValidator> CCertificateChainValidatorHelper::mInstance;

ECode CCertificateChainValidatorHelper::GetInstance(
    /* [out] */ Elastos::Droid::Net::Http::ICertificateChainValidator** instance)
{
    VALIDATE_NOT_NULL(instance);

    if (mInstance == NULL) {
        CertificateChainValidator* c = new CertificateChainValidator();
        mInstance = (ICertificateChainValidator*)c->Probe(EIID_ICertificateChainValidator);
    }

    // TODO:
    // if (mVerifier == NULL) {
    //     CDefaultHostnameVerifier* c = new CDefaultHostnameVerifier();
    //     mVerifier = (IDefaultHostnameVerifier*)c->Probe(EIID_IDefaultHostnameVerifier);
    // }

    *instance = mInstance;
    return NOERROR;
}

ECode CCertificateChainValidatorHelper::VerifyServerCertificates(
    /* [in] */ IObjectContainer* certChain,
    /* [in] */ const String& sDomain,
    /* [in] */ const String& authType,
    /* [out] */ Elastos::Droid::Net::Http::ISslError** err)
{
    Int32 count;
    if (certChain == NULL || (certChain->GetObjectCount(&count), count) == 0) {
        // throw new IllegalArgumentException("bad certificate chain");
        return E_ILLEGALARGUMENT_EXCEPTION;
    }

    Int32 length;
    certChain->GetObjectCount(&length);
    AutoPtr<ArrayOf<IX509Certificate> > serverCertificates = ArrayOf<IX509Certificate>::Alloc(length);

    for (Int32 i = 0; i < length; ++i) {
    //     serverCertificates[i] = new X509CertImpl(certChain[i]);
    }

    // return verifyServerDomainAndCertificates(serverCertificates, domain, authType);
    return E_NOT_IMPLEMENTED;
}

ECode CCertificateChainValidatorHelper::HandleTrustStorageUpdate()
{

    // AutoPtr<IX509TrustManager> x509TrustManager;
    // AutoPtr<ISSLParametersImpl> impl;
    // CSSLParametersImpl::AcquireSingleton((ISSLParametersImpl**)&impl);
    // impl->GetDefaultTrustManager((IX509TrustManager**)&x509TrustManager);
    // AutoPtr<ITrustManagerImpl> tmImpl = x509TrustManager->Probe(EIID_ITrustManagerImpl);
    // if(tmImpl != NULL) {
        // tmImpl->HandleTrustStorageUpdate();
    // }

    // return NOERROR;
    return E_NOT_IMPLEMENTED;
}

}
}
}
}

