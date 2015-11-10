
#include "elastos/droid/net/http/SslError.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL(SslError, Object, ISslError)

SslError::SslError()
    : mErrors(0)
{}

ECode SslError::constructor(
    /* [in] */ Int32 error,
    /* [in] */ ISslCertificate* certificate)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    return constructor(error, certificate, String(""));
#endif
}

ECode SslError::constructor(
    /* [in] */ Int32 error,
    /* [in] */ IX509Certificate* certificate)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<ISslCertificate> cert;
    // TODO:
    // cert = (ISslCertificate*)certificate->Probe(EIID_ISslCertificate);
    return constructor(error, cert, String(""));
#endif
}

ECode SslError::constructor(
    /* [in] */ Int32 error,
    /* [in] */ ISslCertificate* certificate,
    /* [in] */ const String& url)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(certificate);
    if (url.IsNullOrEmpty()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Boolean result;
    AddError(error, &result);
    mCertificate = certificate;
    mUrl = url;
    return NOERROR;
#endif
}

ECode SslError::constructor(
    /* [in] */ Int32 error,
    /* [in] */ IX509Certificate* certificate,
    /* [in] */ const String& url)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<ISslCertificate> cert;
    // TODO:
    // CSslCertificate::New(certificate, (ISslCertificate**)&cert);
    return constructor(error, cert, url);
#endif
}

ECode SslError::SslErrorFromChromiumErrorCode(
    /* [in] */ Int32 error,
    /* [in] */ ISslCertificate* cert,
    /* [in] */ const String& url,
    /* [out] */ ISslError** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(sslError);

    // The chromium error codes are in:
    // external/chromium/net/base/net_error_list.h
    assert (error >= -299 && error <= -200);

    if (error == -200)
        return CSslError::New(CSslError::SSL_IDMISMATCH, cert, url, sslError);
    if (error == -201)
        return CSslError::New(CSslError::SSL_DATE_INVALID, cert, url, sslError);
    if (error == -202)
        return CSslError::New(CSslError::SSL_UNTRUSTED, cert, url, sslError);
    // Map all other codes to SSL_INVALID.
    return CSslError::New(CSslError::SSL_INVALID, cert, url, sslError);
#endif
}

ECode SslError::GetCertificate(
    /* [out] */ ISslCertificate** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(certificate);
    *certificate = mCertificate;
    REFCOUNT_ADD(*certificate);
    return NOERROR;
#endif
}

ECode SslError::GetUrl(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(url);
    *url = mUrl;
    return NOERROR;
#endif
}

ECode SslError::AddError(
    /* [in] */ Int32 error,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    Boolean rval = (0 <= error && error < SSL_MAX_ERROR);
    if (rval) {
        mErrors |= (0x1 << error);
    }

    *result = rval;
    return NOERROR;
#endif
}

ECode SslError::HasError(
    /* [in] */ Int32 error,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    Boolean rval = (0 <= error && error < SSL_MAX_ERROR);
    if (rval) {
        rval = ((mErrors & (0x1 << error)) != 0);
    }

    *hasError = rval;
    return NOERROR;
#endif
}

ECode SslError::GetPrimaryError(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (mErrors != 0) {
        // go from the most to the least severe errors
        for (Int32 iError = SSL_MAX_ERROR - 1; iError >= 0; --iError) {
            if ((mErrors & (0x1 << iError)) != 0) {
                *error = iError;
                return NOERROR;
            }
        }
        // mErrors should never be set to an invalid value.
        assert(FALSE);
    }

    *error = -1;
    return NOERROR;
#endif
}

ECode SslError::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    Int32 err;
    GetPrimaryError(&err);
    String sCert;
    mCertificate->ToString(&sCert);

    *str = String("primary error: ") + StringUtils::Int32ToString(err)
        + String(" certificate: ") + sCert
        + String(" on URL: ") + mUrl;

    return NOERROR;
#endif
}


} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
