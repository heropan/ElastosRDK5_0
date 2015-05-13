
#ifndef __CSSLCERTIFICATE_H__
#define __CSSLCERTIFICATE_H__

#include "_CSslCertificate.h"

using Elastos::Droid::Content::IContext;
using Elastos::Utility::IDate;
using Elastos::Security::IPrincipal;
using Elastos::Security::Cert::IX509Certificate;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * SSL certificate info (certificate details) class
 */
CarClass(CSslCertificate)
{
    friend class CSslCertificateDName;
    friend class CSslCertificateHelper;

public:
    CSslCertificate();

    /**
     * Creates a new SSL certificate object
     * @param issuedTo The entity this certificate is issued to
     * @param issuedBy The entity that issued this certificate
     * @param validNotBefore The not-before date from the certificate
     *     validity period in ISO 8601 format
     * @param validNotAfter The not-after date from the certificate
     *     validity period in ISO 8601 format
     * @deprecated Use {@link #SslCertificate(X509Certificate)}
     */
    /*@Deprecated*/
    CARAPI constructor(
        /* [in] */ const String& issuedTo,
        /* [in] */ const String& issuedBy,
        /* [in] */ const String& validNotBefore,
        /* [in] */ const String& validNotAfter);

    /**
     * Creates a new SSL certificate object
     * @param issuedTo The entity this certificate is issued to
     * @param issuedBy The entity that issued this certificate
     * @param validNotBefore The not-before date from the certificate validity period
     * @param validNotAfter The not-after date from the certificate validity period
     * @deprecated Use {@link #SslCertificate(X509Certificate)}
     */
    /*@Deprecated*/
    CARAPI constructor(
        /* [in] */ const String& issuedTo,
        /* [in] */ const String& issuedBy,
        /* [in] */ Elastos::Utility::IDate* validNotBefore,
        /* [in] */ Elastos::Utility::IDate* validNotAfter);

    /**
     * Creates a new SSL certificate object from an X509 certificate
     * @param certificate X509 certificate
     */
    CARAPI constructor(
        /* [in] */ IX509Certificate* x509Certificate);

    /**
     * @return Not-before date from the certificate validity period or
     * "" if none has been set
     */
    CARAPI GetValidNotBeforeDate(
        /* [out] */ Elastos::Utility::IDate** date);

    /**
     * @return Not-before date from the certificate validity period in
     * ISO 8601 format or "" if none has been set
     *
     * @deprecated Use {@link #getValidNotBeforeDate()}
     */
    /*@Deprecated*/
    CARAPI GetValidNotBefore(
        /* [out] */ String* valid);

    /**
     * @return Not-after date from the certificate validity period or
     * "" if none has been set
     */
    CARAPI GetValidNotAfterDate(
        /* [out] */ Elastos::Utility::IDate** date);

    /**
     * @return Not-after date from the certificate validity period in
     * ISO 8601 format or "" if none has been set
     *
     * @deprecated Use {@link #getValidNotAfterDate()}
     */
    /*@Deprecated*/
    CARAPI GetValidNotAfter(
        /* [out] */ String* valid);

    /**
     * @return Issued-to distinguished name or null if none has been set
     */
    CARAPI GetIssuedTo(
        /* [out] */ ISslCertificateDName** issuedTo);

    /**
     * @return Issued-by distinguished name or null if none has been set
     */
    CARAPI GetIssuedBy(
        /* [out] */ ISslCertificateDName** issuedBy);

    /**
     * @return A string representation of this certificate for debugging
     */
    CARAPI ToString(
        /* [out] */ String* str);

    /**
     * Inflates the SSL certificate view (helper method).
     * @return The resultant certificate view with issued-to, issued-by,
     * issued-on, expires-on, and possibly other fields set.
     *
     * @hide Used by Browser and Settings
     */
    CARAPI InflateCertificateView(
        /* [in] */ Elastos::Droid::Content::IContext* context,
        /* [out] */ Elastos::Droid::View::IView** view);

private:
    CSslCertificate(
        /* [in] */ const String& issuedTo,
        /* [in] */ const String& issuedBy,
        /* [in] */ Elastos::Utility::IDate* validNotBefore,
        /* [in] */ Elastos::Utility::IDate* validNotAfter,
        /* [in] */ IX509Certificate* x509Certificate);

    CARAPI Init(
        /* [in] */ const String& issuedTo,
        /* [in] */ const String& issuedBy,
        /* [in] */ Elastos::Utility::IDate* validNotBefore,
        /* [in] */ Elastos::Utility::IDate* validNotAfter,
        /* [in] */ IX509Certificate* x509Certificate);

    /**
     * Convenience for UI presentation, not intended as public API.
     */
    static CARAPI_(String) GetSerialNumber(
        /* [in] */ IX509Certificate* x509Certificate);

    /**
     * Convenience for UI presentation, not intended as public API.
     */
    static CARAPI_(String) GetDigest(
        /* [in] */ IX509Certificate* x509Certificate,
        /* [in] */ const String& algorithm);

    static const String Fingerprint(
        /* [in] */ ArrayOf<Byte>* bytes);

    /**
     * Parse an ISO 8601 date converting ParseExceptions to a null result;
     */
     static CARAPI_(AutoPtr<IDate>) ParseDate(
        /* [in] */ const String& string);

    /**
     * Format a date as an ISO 8601 string, return "" for a null date
     */
    static CARAPI_(String) FormatDate(
        /* [in] */ IDate* date);

    /**
     * Clone a possibly null Date
     */
    static CARAPI_(AutoPtr<IDate>) CloneDate(
        /* [in] */ IDate* date);

    /**
     * Formats the certificate date to a properly localized date string.
     * @return Properly localized version of the certificate date string and
     * the "" if it fails to localize.
     */
    CARAPI_(String) FormatCertificateDate(
        /* [in] */ IContext* context,
        /* [in] */ IDate* certificateDate);

private:
    /**
     * SimpleDateFormat pattern for an ISO 8601 date
     */
    static String ISO_8601_DATE_FORMAT;

    /**
     * Name of the entity this certificate is issued to
     */
    AutoPtr<ISslCertificateDName> mIssuedTo;

    /**
     * Name of the entity this certificate is issued by
     */
    AutoPtr<ISslCertificateDName> mIssuedBy;

    /**
     * Not-before date from the validity period
     */
    AutoPtr<IDate> mValidNotBefore;

    /**
     * Not-after date from the validity period
     */
    AutoPtr<IDate> mValidNotAfter;

    /**
     * The original source certificate, if available.
     *
     * TODO If deprecated constructors are removed, this should always
     * be available, and saveState and restoreState can be simplified
     * to be unconditional.
     */
    AutoPtr<IX509Certificate> mX509Certificate;

    /**
     * Bundle key names
     */
    static const String ISSUED_TO;
    static const String ISSUED_BY;
    static const String VALID_NOT_BEFORE;
    static const String VALID_NOT_AFTER;
    static const String X509_CERTIFICATE;
};

}
}
}
}

#endif // __CSSLCERTIFICATE_H__
