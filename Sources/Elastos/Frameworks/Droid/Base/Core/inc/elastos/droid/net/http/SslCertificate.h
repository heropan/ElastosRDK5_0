
#ifndef __ELASTOS_DROID_NET_HTTP_SSLCERTIFICATE_H__
#define __ELASTOS_DROID_NET_HTTP_SSLCERTIFICATE_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::View::IView;

using Elastos::Security::Cert::IX509Certificate;
using Elastos::Utility::IDate;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * SSL certificate info (certificate details) class
 */
class SslCertificate
    : public Object
    , public ISslCertificate
{
public:
    CAR_INTERFACE_DECL()

    SslCertificate();

    /**
     * Saves the certificate state to a bundle
     * @param certificate The SSL certificate to store
     * @return A bundle with the certificate stored in it or null if fails
     */
    static CARAPI SaveState(
        /* [in] */ ISslCertificate* certificate,
        /* [out] */ IBundle** result);

    /**
     * Restores the certificate stored in the bundle
     * @param bundle The bundle with the certificate state stored in it
     * @return The SSL certificate stored in the bundle or null if fails
     */
    static CARAPI RestoreState(
        /* [in] */ IBundle* bundle,
        /* [out] */ ISslCertificate** result);

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
    // @Deprecated
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
    // @Deprecated
    CARAPI constructor(
        /* [in] */ const String& issuedTo,
        /* [in] */ const String& issuedBy,
        /* [in] */ IDate* validNotBefore,
        /* [in] */ IDate* validNotAfter);

    /**
     * Creates a new SSL certificate object from an X509 certificate
     * @param certificate X509 certificate
     */
    CARAPI constructor(
        /* [in] */ IX509Certificate* certificate);

    /**
     * @return Not-before date from the certificate validity period or
     * "" if none has been set
     */
    CARAPI GetValidNotBeforeDate(
        /* [out] */ IDate** result);

    /**
     * @return Not-before date from the certificate validity period in
     * ISO 8601 format or "" if none has been set
     *
     * @deprecated Use {@link #getValidNotBeforeDate()}
     */
    // @Deprecated
    CARAPI GetValidNotBefore(
        /* [out] */ String* result);

    /**
     * @return Not-after date from the certificate validity period or
     * "" if none has been set
     */
    CARAPI GetValidNotAfterDate(
        /* [out] */ IDate** result);

    /**
     * @return Not-after date from the certificate validity period in
     * ISO 8601 format or "" if none has been set
     *
     * @deprecated Use {@link #getValidNotAfterDate()}
     */
    // @Deprecated
    CARAPI GetValidNotAfter(
        /* [out] */ String* result);

    /**
     * @return Issued-to distinguished name or null if none has been set
     */
    CARAPI GetIssuedTo(
        /* [out] */ ISslCertificateDName** result);

    /**
     * @return Issued-by distinguished name or null if none has been set
     */
    CARAPI GetIssuedBy(
        /* [out] */ ISslCertificateDName** result);

    /**
     * @return A string representation of this certificate for debugging
     */
    CARAPI ToString(
        /* [out] */ String* result);

    /**
     * Inflates the SSL certificate view (helper method).
     * @return The resultant certificate view with issued-to, issued-by,
     * issued-on, expires-on, and possibly other fields set.
     *
     * @hide Used by Browser and Settings
     */
    CARAPI InflateCertificateView(
        /* [in] */ IContext* context,
        /* [out] */ IView** result);

private:
    SslCertificate(
        /* [in] */ const String& issuedTo,
        /* [in] */ const String& issuedBy,
        /* [in] */ Elastos::Utility::IDate* validNotBefore,
        /* [in] */ Elastos::Utility::IDate* validNotAfter,
        /* [in] */ IX509Certificate* x509Certificate);

    CARAPI Init(
        /* [in] */ const String& issuedTo,
        /* [in] */ const String& issuedBy,
        /* [in] */ IDate* validNotBefore,
        /* [in] */ IDate* validNotAfter,
        /* [in] */ IX509Certificate* x509Certificate);

    /**
     * Convenience for UI presentation, not intended as public API.
     */
    static CARAPI GetSerialNumber(
        /* [in] */ IX509Certificate* x509Certificate,
        /* [out] */ String* result);

    /**
     * Convenience for UI presentation, not intended as public API.
     */
    static CARAPI GetDigest(
        /* [in] */ IX509Certificate* x509Certificate,
        /* [in] */ const String& algorithm,
        /* [out] */ String* result);

    static CARAPI Fingerprint(
        /* [in] */ ArrayOf<Byte>* bytes,
        /* [out] */ String* result);

    /**
     * Parse an ISO 8601 date converting ParseExceptions to a null result;
     */
    static CARAPI ParseDate(
        /* [in] */ const String& string,
        /* [out] */ IDate** result);

    /**
     * Format a date as an ISO 8601 string, return "" for a null date
     */
    static CARAPI FormatDate(
        /* [in] */ IDate* date,
        /* [out] */ String* result);

    /**
     * Clone a possibly null Date
     */
    static CARAPI CloneDate(
        /* [in] */ IDate* date,
        /* [out] */ IDate** result);

    /**
     * Formats the certificate date to a properly localized date string.
     * @return Properly localized version of the certificate date string and
     * the "" if it fails to localize.
     */
    CARAPI FormatCertificateDate(
        /* [in] */ IContext* context,
        /* [in] */ IDate* certificateDate,
        /* [out] */ String* result);

private:
    /**
     * SimpleDateFormat pattern for an ISO 8601 date
     */
    static String sISO_8601_DATE_FORMAT;

    /**
     * Name of the entity this certificate is issued to
     */
    /* const */ AutoPtr<ISslCertificateDName> mIssuedTo;

    /**
     * Name of the entity this certificate is issued by
     */
    /* const */ AutoPtr<ISslCertificateDName> mIssuedBy;

    /**
     * Not-before date from the validity period
     */
    /* const */ AutoPtr<IDate> mValidNotBefore;

    /**
     * Not-after date from the validity period
     */
    /* const */ AutoPtr<IDate> mValidNotAfter;

    /**
     * The original source certificate, if available.
     *
     * TODO If deprecated constructors are removed, this should always
     * be available, and saveState and restoreState can be simplified
     * to be unconditional.
     */
    /* const */ AutoPtr<IX509Certificate> mX509Certificate;

    /**
     * Bundle key names
     */
    static const String ISSUED_TO;

    static const String ISSUED_BY;

    static const String VALID_NOT_BEFORE;

    static const String VALID_NOT_AFTER;

    static const String X509_CERTIFICATE;
};


/**
 * A distinguished name helper class: a 3-tuple of:
 * <ul>
 *   <li>the most specific common name (CN)</li>
 *   <li>the most specific organization (O)</li>
 *   <li>the most specific organizational unit (OU)</li>
 * <ul>
 */
class SslCertificateDName
    : public Object
    , public ISslCertificateDName
{
public:
    CAR_INTERFACE_DECL()

    /**
     * Creates a new {@code DName} from a string. The attributes
     * are assumed to come in most significant to least
     * significant order which is true of human readable values
     * returned by methods such as {@code X500Principal.getName()}.
     * Be aware that the underlying sources of distinguished names
     * such as instances of {@code X509Certificate} are encoded in
     * least significant to most significant order, so make sure
     * the value passed here has the expected ordering of
     * attributes.
     */
    CARAPI constructor(
        /* [in] */ const String& dName);

    /**
     * @return The distinguished name (normally includes CN, O, and OU names)
     */
    CARAPI GetDName(
        /* [out] */ String* result);

    /**
     * @return The most specific Common-name (CN) component of this name
     */
    CARAPI GetCName(
        /* [out] */ String* result);

    /**
     * @return The most specific Organization (O) component of this name
     */
    CARAPI GetOName(
        /* [out] */ String* result);

    /**
     * @return The most specific Organizational Unit (OU) component of this name
     */
    CARAPI GetUName(
        /* [out] */ String* result);

private:
    /**
     * Distinguished name (normally includes CN, O, and OU names)
     */
    String mDName;

    /**
     * Common-name (CN) component of the name
     */
    String mCName;

    /**
     * Organization (O) component of the name
     */
    String mOName;

    /**
     * Organizational Unit (OU) component of the name
     */
    String mUName;
};

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_HTTP_SSLCERTIFICATE_H__
