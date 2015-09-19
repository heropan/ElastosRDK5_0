
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

const String SslUtil::TAG("SslUtil");

/**
 * Creates an SslError object from a chromium net error code.
 */
AutoPtr<ISslError> SslUtil::SslErrorFromNetErrorCode(
    /* [in] */ Int32 error,
    /* [in] */ ISslCertificate* cert,
    /* [in] */ String url)
{
    assert (error >= NetError::ERR_CERT_END && error <= NetError::ERR_CERT_COMMON_NAME_INVALID);
    switch(error) {
        case NetError::ERR_CERT_COMMON_NAME_INVALID: {
                AutoPtr<ISslError> sslError;
                CSslError::New(ISslError::SSL_IDMISMATCH, cert, url, (ISslError**)&sslError);
                return sslError;
            }
        case NetError::ERR_CERT_DATE_INVALID: {
                AutoPtr<ISslError> sslError;
                CSslError::New(ISslError::SSL_DATE_INVALID, cert, url, (ISslError**)&sslError);
                return sslError;
            }
        case NetError::ERR_CERT_AUTHORITY_INVALID: {
                AutoPtr<ISslError> sslError;
                CSslError::New(ISslError::SSL_UNTRUSTED, cert, url, (ISslError**)&sslError);
                return sslError;
            }
        default:
            break;
    }

    // Map all other codes to SSL_INVALID.
    AutoPtr<ISslError> sslError;
    CSslError::New(ISslError::SSL_INVALID, cert, url, (ISslError**)&sslError);
    return sslError;
}

AutoPtr<ISslCertificate> SslUtil::GetCertificateFromDerBytes(
    /* [in] */ ArrayOf<Byte>* derBytes)
{
    if (derBytes == NULL) {
        return NULL;
    }

    // try {
        X509Certificate x509Certificate =
                X509Util.createCertificateFromBytes(derBytes);
        return new SslCertificate(x509Certificate);
    // } catch (CertificateException e) {
    //     // A SSL related exception must have occured.  This shouldn't happen.
    //     Log.w(TAG, "Could not read certificate: " + e);
    // } catch (KeyStoreException e) {
    //     // A SSL related exception must have occured.  This shouldn't happen.
    //     Log.w(TAG, "Could not read certificate: " + e);
    // } catch (NoSuchAlgorithmException e) {
    //     // A SSL related exception must have occured.  This shouldn't happen.
    //     Log.w(TAG, "Could not read certificate: " + e);
    // }

    return NULL;
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos