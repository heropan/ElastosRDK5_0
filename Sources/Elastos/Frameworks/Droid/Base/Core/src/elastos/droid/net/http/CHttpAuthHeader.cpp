
#include "CHttpAuthHeader.h"
#include "HttpLog.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

const String CHttpAuthHeader::REALM_TOKEN = String("realm");
const String CHttpAuthHeader::NONCE_TOKEN = String("nonce");
const String CHttpAuthHeader::STALE_TOKEN = String("stale");
const String CHttpAuthHeader::OPAQUE_TOKEN = String("opaque");
const String CHttpAuthHeader::QOP_TOKEN = String("qop");
const String CHttpAuthHeader::ALGORITHM_TOKEN = String("algorithm");

CHttpAuthHeader::CHttpAuthHeader()
    : mScheme(0)
    , mStale(FALSE)
    , mIsProxy(FALSE)
{}

CHttpAuthHeader::~CHttpAuthHeader()
{}

ECode CHttpAuthHeader::constructor(
    /* [in] */ const String& header)
{
    if (header != NULL) {
        ParseHeader(header);
    }
    return NOERROR;
}

ECode CHttpAuthHeader::IsProxy(
    /* [out] */ Boolean* isProxy)
{
    VALIDATE_NOT_NULL(isProxy);
    *isProxy = mIsProxy;
    return NOERROR;
}

ECode CHttpAuthHeader::SetProxy()
{
    mIsProxy = TRUE;
    return NOERROR;
}

ECode CHttpAuthHeader::GetUsername(
    /* [out] */ String* username)
{
    VALIDATE_NOT_NULL(username);
    *username = mUsername;
    return NOERROR;
}

ECode CHttpAuthHeader::SetUsername(
    /* [in] */ const String& username)
{
    mUsername = username;
    return NOERROR;
}

ECode CHttpAuthHeader::GetPassword(
    /* [out] */ String* password)
{
    VALIDATE_NOT_NULL(password);
    *password = mPassword;
    return NOERROR;
}

ECode CHttpAuthHeader::SetPassword(
    /* [in] */ const String& password)
{
    mPassword = password;
    return NOERROR;
}

ECode CHttpAuthHeader::IsBasic(
    /* [out] */ Boolean* isBasic)
{
    VALIDATE_NOT_NULL(isBasic);
    *isBasic = mScheme == BASIC;
    return NOERROR;
}

ECode CHttpAuthHeader::IsDigest(
    /* [out] */ Boolean* isDigest)
{
    VALIDATE_NOT_NULL(isDigest);
    *isDigest = mScheme == DIGEST;
    return NOERROR;
}

ECode CHttpAuthHeader::GetScheme(
    /* [out] */ Int32* scheme)
{
    VALIDATE_NOT_NULL(scheme);
    *scheme = mScheme;
    return NOERROR;
}

ECode CHttpAuthHeader::GetStale(
    /* [out] */ Boolean* stale)
{
    VALIDATE_NOT_NULL(stale);
    *stale = mStale;
    return NOERROR;
}

ECode CHttpAuthHeader::GetRealm(
    /* [out] */ String* realm)
{
    VALIDATE_NOT_NULL(realm);
    *realm = mRealm;
    return NOERROR;
}

ECode CHttpAuthHeader::GetNonce(
    /* [out] */ String* nonce)
{
    VALIDATE_NOT_NULL(nonce);
    *nonce = mNonce;
    return NOERROR;
}

ECode CHttpAuthHeader::GetOpaque(
    /* [out] */ String* opaque)
{
    VALIDATE_NOT_NULL(opaque);
    *opaque = mOpaque;
    return NOERROR;
}

ECode CHttpAuthHeader::GetQop(
    /* [out] */ String* qop)
{
    VALIDATE_NOT_NULL(qop);
    *qop = mQop;
    return NOERROR;
}

ECode CHttpAuthHeader::GetAlgorithm(
    /* [out] */ String* algorithm)
{
    VALIDATE_NOT_NULL(algorithm);
    *algorithm = mAlgorithm;
    return NOERROR;
}

ECode CHttpAuthHeader::IsSupportedScheme(
    /* [out] */ Boolean* isSupport)
{
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
}

ECode CHttpAuthHeader::ParseHeader(
    /* [in] */ const String& header)
{
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
}

ECode CHttpAuthHeader::ParseScheme(
    /* [in] */ const String& header,
    /* [out] */ String* scheme)
{
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
}

ECode CHttpAuthHeader::ParseParameters(
    /* [in] */ const String& parameters)
{
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
}

ECode CHttpAuthHeader::ParseParameter(
    /* [in] */ const String& parameter)
{
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
}

ECode CHttpAuthHeader::ParseParameter(
    /* [in] */ const String& token,
    /* [in] */ const String& value)
{
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
            mQop = value.ToLowerCase();
            return NOERROR;
        }

        if (token.EqualsIgnoreCase(ALGORITHM_TOKEN)) {
            mAlgorithm = value.ToLowerCase();
            return NOERROR;
        }
    }
    return NOERROR;
}

ECode CHttpAuthHeader::ParseStale(
    /* [in] */ const String& value)
{
    if (value != NULL) {
        if (value.EqualsIgnoreCase("true")) {
            mStale = true;
        }
    }
    return NOERROR;
}

String CHttpAuthHeader::TrimDoubleQuotesIfAny(
    /* [in] */ const String& value)
{
    if (value != NULL) {
        Int32 len = value.GetLength();
        if (len > 2
                && value.GetChar(0) == '\"'
                && value.GetChar(len -1) == '\"') {
            return value.Substring(1, len - 1);
        }
    }

    return value;
}

}
}
}
}

