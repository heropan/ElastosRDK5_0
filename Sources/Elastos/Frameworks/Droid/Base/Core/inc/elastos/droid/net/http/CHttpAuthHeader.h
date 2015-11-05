
#ifndef __ELASTOS_DROID_NET_HTTP_CHTTPAUTHHEADER_H__
#define __ELASTOS_DROID_NET_HTTP_CHTTPAUTHHEADER_H__

#include "_Elastos_Droid_Net_Http_CHttpAuthHeader.h"
#include "elastos/droid/net/http/HttpAuthHeader.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * HttpAuthHeader: a class to store HTTP authentication-header parameters.
 * For more information, see: RFC 2617: HTTP Authentication.
 *
 * {@hide}
 */
CarClass(CHttpAuthHeader)
    , public HttpAuthHeader
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_HTTP_CHTTPAUTHHEADER_H__

#if 0 // old CHttpAuthHeader.h
#ifndef __ELASTOS_DROID_NET_HTTP_CHTTPAUTHHEADER_H__
#define __ELASTOS_DROID_NET_HTTP_CHTTPAUTHHEADER_H__

#include "_Elastos_Droid_Net_Http_CHttpAuthHeader.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * HttpAuthHeader: a class to store HTTP authentication-header parameters.
 * For more information, see: RFC 2617: HTTP Authentication.
 *
 * {@hide}
 */
CarClass(CHttpAuthHeader)
{
public:
    CHttpAuthHeader();

    ~CHttpAuthHeader();

    /**
     * Creates a new HTTP-authentication header object from the
     * input header string.
     * The header string is assumed to contain parameters of at
     * most one authentication-scheme (ensured by the caller).
     */
    CARAPI constructor(
        /* [in] */ const String& header);

    /**
     * @return True iff this is a proxy authentication header.
     */
    CARAPI IsProxy(
        /* [out] */ Boolean* isProxy);

    /**
     * Marks this header as a proxy authentication header.
     */
    CARAPI SetProxy();

    /**
     * @return The username string.
     */
    CARAPI GetUsername(
        /* [out] */ String* username);

    /**
     * Sets the username string.
     */
    CARAPI SetUsername(
        /* [in] */ const String& username);

    /**
     * @return The password string.
     */
    CARAPI GetPassword(
        /* [out] */ String* password);

    /**
     * Sets the password string.
     */
    CARAPI SetPassword(
        /* [in] */ const String& password);

    /**
     * @return True iff this is the  BASIC-authentication request.
     */
    CARAPI IsBasic(
        /* [out] */ Boolean* isBasic);

    /**
     * @return True iff this is the DIGEST-authentication request.
     */
    CARAPI IsDigest(
        /* [out] */ Boolean* isDigest);

    /**
     * @return The authentication scheme requested. We currently
     * support two schemes:
     * HttpAuthHeader.BASIC  - basic, and
     * HttpAuthHeader.DIGEST - digest (algorithm=MD5, QOP="auth").
     */
    CARAPI GetScheme(
        /* [out] */ Int32* scheme);

    /**
     * @return True if indicating that the previous request from
     * the client was rejected because the nonce value was stale.
     */
    CARAPI GetStale(
        /* [out] */ Boolean* stale);

    /**
     * @return The realm value or null if there is none.
     */
    CARAPI GetRealm(
        /* [out] */ String* realm);

    /**
     * @return The nonce value or null if there is none.
     */
    CARAPI GetNonce(
        /* [out] */ String* nonce);

    /**
     * @return The opaque value or null if there is none.
     */
    CARAPI GetOpaque(
        /* [out] */ String* opaque);

    /**
     * @return The QOP ("quality-of_protection") value or null if
     * there is none. The QOP value is always lower-case.
     */
    CARAPI GetQop(
        /* [out] */ String* qop);

    /**
     * @return The name of the algorithm used or null if there is
     * none. By default, MD5 is used.
     */
    CARAPI GetAlgorithm(
        /* [out] */ String* algorithm);

    /**
     * @return True iff the authentication scheme requested by the
     * server is supported; currently supported schemes:
     * BASIC,
     * DIGEST (only algorithm="md5", no qop or qop="auth).
     */
    CARAPI IsSupportedScheme(
        /* [out] */ Boolean* isSupport);

private:
    /**
     * Parses the header scheme name and then scheme parameters if
     * the scheme is supported.
     */
    CARAPI ParseHeader(
        /* [in] */ const String& header);

    /**
     * Parses the authentication scheme name. If we have a Digest
     * scheme, sets the algorithm value to the default of MD5.
     * @return The authentication scheme parameters string to be
     * parsed later (if the scheme is supported) or null if failed
     * to parse the scheme (the header value is null?).
     */
    CARAPI ParseScheme(
        /* [in] */ const String& header,
        /* [out] */ String* scheme);

    /**
     * Parses a comma-separated list of authentification scheme
     * parameters.
     */
    CARAPI ParseParameters(
        /* [in] */ const String& parameters);

    /**
     * Parses a single authentication scheme parameter. The parameter
     * string is expected to follow the format: PARAMETER=VALUE.
     */
    CARAPI ParseParameter(
        /* [in] */ const String& parameter);

    /**
     * If the token is a known parameter name, parses and initializes
     * the token value.
     */
    CARAPI ParseParameter(
        /* [in] */ const String& token,
        /* [in] */ const String& value);

    /**
     * Parses and initializes the 'stale' paramer value. Any value
     * different from case-insensitive "true" is considered "false".
     */
    CARAPI ParseStale(
        /* [in] */ const String& value);

    /**
     * Trims double-quotes around a parameter value if there are any.
     * @return The string value without the outermost pair of double-
     * quotes or null if the original value is null.
     */
    static CARAPI_(String) TrimDoubleQuotesIfAny(
        /* [in] */ const String& value);

private:
    static const String REALM_TOKEN;
    static const String NONCE_TOKEN;
    static const String STALE_TOKEN;
    static const String OPAQUE_TOKEN;
    static const String QOP_TOKEN ;
    static const String ALGORITHM_TOKEN;

    /**
     * An authentication scheme. We currently support two different schemes:
     * HttpAuthHeader.BASIC  - basic, and
     * HttpAuthHeader.DIGEST - digest (algorithm=MD5, QOP="auth").
     */
    Int32 mScheme;

    /**
     * A flag, indicating that the previous request from the client was
     * rejected because the nonce value was stale. If stale is TRUE
     * (case-insensitive), the client may wish to simply retry the request
     * with a new encrypted response, without reprompting the user for a
     * new username and password.
     */
    Boolean mStale;

    /**
     * A string to be displayed to users so they know which username and
     * password to use.
     */
    String mRealm;

    /**
     * A server-specified data string which should be uniquely generated
     * each time a 401 response is made.
     */
    String mNonce;

    /**
     * A string of data, specified by the server, which should be returned
     *  by the client unchanged in the Authorization header of subsequent
     * requests with URIs in the same protection space.
     */
    String mOpaque;

    /**
     * This directive is optional, but is made so only for backward
     * compatibility with RFC 2069 [6]; it SHOULD be used by all
     * implementations compliant with this version of the Digest scheme.
     * If present, it is a quoted string of one or more tokens indicating
     * the "quality of protection" values supported by the server.  The
     * value "auth" indicates authentication; the value "auth-int"
     * indicates authentication with integrity protection.
     */
    String mQop;

    /**
     * A string indicating a pair of algorithms used to produce the digest
     * and a checksum. If this is not present it is assumed to be "MD5".
     */
    String mAlgorithm;

    /**
     * Is this authentication request a proxy authentication request?
     */
    Boolean mIsProxy;

    /**
     * Username string we get from the user.
     */
    String mUsername;

    /**
     * Password string we get from the user.
     */
    String mPassword;
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_HTTP_CHTTPAUTHHEADER_H__
#endif