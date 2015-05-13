
#include "CSslError.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

const Int32 CSslError::SSL_NOTYETVALID = 0;
const Int32 CSslError::SSL_EXPIRED = 1;
const Int32 CSslError::SSL_IDMISMATCH = 2;
const Int32 CSslError::SSL_UNTRUSTED = 3;
const Int32 CSslError::SSL_DATE_INVALID = 4;
const Int32 CSslError::SSL_INVALID = 5;
const Int32 CSslError::SSL_MAX_ERROR = 6;

CSslError::CSslError()
    : mErrors(0)
{}

CSslError::~CSslError()
{}

ECode CSslError::constructor(
    /* [in] */ Int32 error,
    /* [in] */ ISslCertificate* certificate)
{
    return Init(error, certificate, String(""));
}

ECode CSslError::constructor(
    /* [in] */ Int32 error,
    /* [in] */ Elastos::Security::Cert::IX509Certificate* certificate)
{
    AutoPtr<ISslCertificate> cert;
    // TODO:
    // cert = (ISslCertificate*)certificate->Probe(EIID_ISslCertificate);
    return Init(error, cert, String(""));
}

ECode CSslError::constructor(
    /* [in] */ Int32 error,
    /* [in] */ Elastos::Droid::Net::Http::ISslCertificate* certificate,
    /* [in] */ const String& url)
{
    return Init(error, certificate, url);
}

ECode CSslError::constructor(
    /* [in] */ Int32 error,
    /* [in] */ Elastos::Security::Cert::IX509Certificate* certificate,
    /* [in] */ const String& url)
{
    AutoPtr<ISslCertificate> cert;
    // TODO:
    // CSslCertificate::New(certificate, (ISslCertificate**)&cert);
    return Init(error, cert, url);
}

ECode CSslError::Init(
    /* [in] */ Int32 error,
    /* [in] */ ISslCertificate* certificate,
    /* [in] */ const String& url)
{
    VALIDATE_NOT_NULL(certificate);
    if (url.IsNullOrEmpty()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Boolean result;
    AddError(error, &result);
    mCertificate = certificate;
    mUrl = url;
    return NOERROR;
}

ECode CSslError::GetCertificate(
    /* [out] */ ISslCertificate** certificate)
{
    VALIDATE_NOT_NULL(certificate);
    *certificate = mCertificate;
    INTERFACE_ADDREF(*certificate);
    return NOERROR;
}

ECode CSslError::GetUrl(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    *url = mUrl;
    return NOERROR;
}

ECode CSslError::AddError(
    /* [in] */ Int32 error,
    /* [out] */ Boolean* result)
{
    Boolean rval = (0 <= error && error < SSL_MAX_ERROR);
    if (rval) {
        mErrors |= (0x1 << error);
    }

    *result = rval;
    return NOERROR;
}

ECode CSslError::HasError(
    /* [in] */ Int32 error,
    /* [out] */ Boolean* hasError)
{
    Boolean rval = (0 <= error && error < SSL_MAX_ERROR);
    if (rval) {
        rval = ((mErrors & (0x1 << error)) != 0);
    }

    *hasError = rval;
    return NOERROR;
}

ECode CSslError::GetPrimaryError(
    /* [out] */ Int32* error)
{
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
}

ECode CSslError::ToString(
    /* [out] */ String* str)
{
    Int32 err;
    GetPrimaryError(&err);
    String sCert;
    mCertificate->ToString(&sCert);

    *str = String("primary error: ") + StringUtils::Int32ToString(err)
        + String(" certificate: ") + sCert
        + String(" on URL: ") + mUrl;

    return NOERROR;
}

}
}
}
}

