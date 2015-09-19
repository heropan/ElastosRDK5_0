
#ifndef __ELASTOS_DROID_NET_HTTP_CSSLERROR_H__
#define __ELASTOS_DROID_NET_HTTP_CSSLERROR_H__

#include "_Elastos_Droid_Net_Http_CSslError.h"
#include "elautoptr.h"

using namespace Elastos::Core;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * This class represents a set of one or more SSL errors and the associated SSL
 * certificate.
 */
CarClass(CSslError)
{
    friend class CSslErrorHelper;

public:
    CSslError();

    ~CSslError();

     /**
     * Creates a new SslError object using the supplied error and certificate.
     * The URL will be set to the empty string.
     * @param error The SSL error
     * @param certificate The associated SSL certificate
     * @deprecated Use {@link #SslError(int, SslCertificate, String)}
     *
     *@Deprecated
     */
    CARAPI constructor(
        /* [in] */ Int32 error,
        /* [in] */ ISslCertificate* certificate);

    /**
     * Creates a new SslError object using the supplied error and certificate.
     * The URL will be set to the empty string.
     * @param error The SSL error
     * @param certificate The associated SSL certificate
     * @deprecated Use {@link #SslError(int, X509Certificate, String)}
     *
     * @Deprecated
     */
    CARAPI constructor(
        /* [in] */ Int32 error,
        /* [in] */ Elastos::Security::Cert::IX509Certificate* certificate);

    /**
     * Creates a new SslError object using the supplied error, certificate and
     * URL.
     * @param error The SSL error
     * @param certificate The associated SSL certificate
     * @param url The associated URL
     */
    CARAPI constructor(
        /* [in] */ Int32 error,
        /* [in] */ ISslCertificate* certificate,
        /* [in] */ const String& url);

    /**
     * Creates a new SslError object using the supplied error, certificate and
     * URL.
     * @param error The SSL error
     * @param certificate The associated SSL certificate
     * @param url The associated URL
     */
    CARAPI constructor(
        /* [in] */ Int32 error,
        /* [in] */ Elastos::Security::Cert::IX509Certificate* certificate,
        /* [in] */ const String& url);

    /**
     * Gets the SSL certificate associated with this object.
     * @return The SSL certificate, non-null.
     */
    CARAPI GetCertificate(
        /* [out] */ ISslCertificate** certificate);

    /**
     * Gets the URL associated with this object.
     * @return The URL, non-null.
     */
    CARAPI GetUrl(
        /* [out] */ String* url);

    /**
     * Adds the supplied SSL error to the set.
     * @param error The SSL error to add
     * @return True if the error being added is a known SSL error, otherwise
     *         false.
     */
    CARAPI AddError(
        /* [in] */ Int32 error,
        /* [out] */ Boolean* result);

    /**
     * Determines whether this object includes the supplied error.
     * @param error The SSL error to check for
     * @return True if this object includes the error, otherwise false.
     */
    CARAPI HasError(
        /* [in] */ Int32 error,
        /* [out] */ Boolean* hasError);

    /**
     * Gets the most severe SSL error in this object's set of errors.
     * Returns -1 if the set is empty.
     * @return The most severe SSL error, or -1 if the set is empty.
     */
    CARAPI GetPrimaryError(
        /* [out] */ Int32* error);

    /**
     * Returns a string representation of this object.
     * @return A String representation of this object.
     */
    CARAPI ToString(
        /* [out] */ String* str);

private:
    CARAPI Init(
        /* [in] */ Int32 error,
        /* [in] */ ISslCertificate* certificate,
        /* [in] */ const String& url);

private:
    /**
     * Individual SSL errors (in the order from the least to the most severe):
     */

    /**
     * The certificate is not yet valid
     */
    static const Int32 SSL_NOTYETVALID;
    /**
     * The certificate has expired
     */
    static const Int32 SSL_EXPIRED;
    /**
     * Hostname mismatch
     */
    static const Int32 SSL_IDMISMATCH;
    /**
     * The certificate authority is not trusted
     */
    static const Int32 SSL_UNTRUSTED;
    /**
     * The date of the certificate is invalid
     */
    static const Int32 SSL_DATE_INVALID;
    /**
     * A generic error occurred
     */
    static const Int32 SSL_INVALID;


    /**
     * The number of different SSL errors.
     * @deprecated This constant is not necessary for using the SslError API and
     *             can change from release to release.
     */
    // Update if you add a new SSL error!!!
    /*@Deprecated*/
    static const Int32 SSL_MAX_ERROR;

    /**
     * The SSL error set bitfield (each individual error is a bit index;
     * multiple individual errors can be OR-ed)
     */
    Int32 mErrors;

    /**
     * The SSL certificate associated with the error set
     */
    AutoPtr<ISslCertificate> mCertificate;

    /**
     * The URL associated with the error set.
     */
    String mUrl;

};

}
}
}
}

#endif // __ELASTOS_DROID_NET_HTTP_CSSLERROR_H__
