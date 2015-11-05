
#include "elastos/droid/net/http/HttpAuthHeader.h"

using Elastos::Utility::ILocale;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL(HttpAuthHeader, Object, IHttpAuthHeader)

const String HttpAuthHeader::REALM_TOKEN("realm");
const String HttpAuthHeader::NONCE_TOKEN("nonce");
const String HttpAuthHeader::STALE_TOKEN("stale");
const String HttpAuthHeader::OPAQUE_TOKEN("opaque");
const String HttpAuthHeader::QOP_TOKEN("qop");
const String HttpAuthHeader::ALGORITHM_TOKEN("algorithm");

HttpAuthHeader::HttpAuthHeader()
    : mScheme(0)
    , mStale(FALSE)
    , mIsProxy(FALSE)
{}

ECode HttpAuthHeader::constructor(
    /* [in] */ const String& header)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (header != NULL) {
        ParseHeader(header);
    }
    return NOERROR;
#endif
}

ECode HttpAuthHeader::IsProxy(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(isProxy);
    *isProxy = mIsProxy;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::SetProxy()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    mIsProxy = TRUE;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::GetUsername(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(username);
    *username = mUsername;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::SetUsername(
    /* [in] */ const String& username)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    mUsername = username;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::GetPassword(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(password);
    *password = mPassword;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::SetPassword(
    /* [in] */ const String& password)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    mPassword = password;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::IsBasic(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(isBasic);
    *isBasic = mScheme == BASIC;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::IsDigest(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(isDigest);
    *isDigest = mScheme == DIGEST;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::GetScheme(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(scheme);
    *scheme = mScheme;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::GetStale(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(stale);
    *stale = mStale;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::GetRealm(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(realm);
    *realm = mRealm;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::GetNonce(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(nonce);
    *nonce = mNonce;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::GetOpaque(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(opaque);
    *opaque = mOpaque;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::GetQop(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(qop);
    *qop = mQop;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::GetAlgorithm(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(algorithm);
    *algorithm = mAlgorithm;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::IsSupportedScheme(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(isSupport);

    // it is a good idea to enforce non-null realms!
    if (mRealm != NULL) {
        if (mScheme == BASIC) {
            *isSupport = TRUE;
        } else {
            if (mScheme == DIGEST) {
                *isSupport = mAlgorithm.Equals("md5") &&
                    (mQop == NULL || mQop.Equals("auth"));
            }
        }
    }

    *isSupport = FALSE;
    return NOERROR;
#endif
}

ECode HttpAuthHeader::ParseHeader(
    /* [in] */ const String& header)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (HttpLog::LOGV) {
        HttpLog::V(String("HttpAuthHeader.parseHeader(): header: ") + header);
    }

    if (header != NULL) {
        String parameters;
        ParseScheme(header, &parameters);
        if (parameters != NULL) {
            // if we have a supported scheme
            if (mScheme != UNKNOWN) {
                ParseParameters(parameters);
            }
        }
    }
    return NOERROR;
#endif
}

ECode HttpAuthHeader::ParseScheme(
    /* [in] */ const String& header,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (header != NULL) {
        Int32 i = header.IndexOf(' ');
        if (i >= 0) {
            String localScheme = header.Substring(0, i).Trim();
            if (localScheme.EqualsIgnoreCase(DIGEST_TOKEN)) {
                mScheme = DIGEST;

                // md5 is the default algorithm!!!
                mAlgorithm = String("md5");
            } else {
                if (localScheme.EqualsIgnoreCase(BASIC_TOKEN)) {
                    mScheme = BASIC;
                }
            }

            *scheme = header.Substring(i + 1);
        }
    }

    *scheme = String(NULL);
    return NOERROR;
#endif
}

ECode HttpAuthHeader::ParseParameters(
    /* [in] */ const String& parameters)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (HttpLog::LOGV) {
        HttpLog::V(String("HttpAuthHeader.parseParameters(): parameters: ") + parameters);
    }

    if (parameters != NULL) {
        Int32 i;
        String loParameters = parameters;
        do {
            i = loParameters.IndexOf(',');
            if (i < 0) {
                // have only one parameter
                ParseParameter(loParameters);
            } else {
                ParseParameter(loParameters.Substring(0, i));
                loParameters = loParameters.Substring(i + 1);
            }
        } while (i >= 0);
    }
    return NOERROR;
#endif
}

ECode HttpAuthHeader::ParseParameter(
    /* [in] */ const String& parameter)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (parameter != NULL) {
        // here, we are looking for the 1st occurence of '=' only!!!
        Int32 i = parameter.IndexOf('=');
        if (i >= 0) {
            String token = parameter.Substring(0, i).Trim();
            String value =
                TrimDoubleQuotesIfAny(parameter.Substring(i + 1).Trim());

            if (HttpLog::LOGV) {
                HttpLog::V(String("HttpAuthHeader.parseParameter(): token: ") + token
                    + String(" value: ") + value);
            }

            if (token.EqualsIgnoreCase(REALM_TOKEN)) {
                mRealm = value;
            } else {
                if (mScheme == DIGEST) {
                    ParseParameter(token, value);
                }
            }
        }
    }
    return NOERROR;
#endif
}

ECode HttpAuthHeader::ParseParameter(
    /* [in] */ const String& token,
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (token != NULL && value != NULL) {
        if (token.EqualsIgnoreCase(NONCE_TOKEN)) {
            mNonce = value;
            return NOERROR;
        }

        if (token.EqualsIgnoreCase(STALE_TOKEN)) {
            ParseStale(value);
            return NOERROR;
        }

        if (token.EqualsIgnoreCase(OPAQUE_TOKEN)) {
            mOpaque = value;
            return NOERROR;
        }

        if (token.EqualsIgnoreCase(QOP_TOKEN)) {
            mQop = value.ToLowerCase(ILocale::ROOT);
            return NOERROR;
        }

        if (token.EqualsIgnoreCase(ALGORITHM_TOKEN)) {
            mAlgorithm = value.ToLowerCase(ILocale::ROOT);
            return NOERROR;
        }
    }
    return NOERROR;
#endif
}

ECode HttpAuthHeader::ParseStale(
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (value != NULL) {
        if (value.EqualsIgnoreCase("true")) {
            mStale = true;
        }
    }
    return NOERROR;
#endif
}

ECode HttpAuthHeader::TrimDoubleQuotesIfAny(
    /* [in] */ const String& value,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (value != NULL) {
        Int32 len = value.GetLength();
        if (len > 2
                && value.GetChar(0) == '\"'
                && value.GetChar(len -1) == '\"') {
            return value.Substring(1, len - 1);
        }
    }

    return value;
#endif
}

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
