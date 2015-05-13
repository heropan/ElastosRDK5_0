
#include "CSslCertificate.h"
#include "elastos/StringUtils.h"
#include "elastos/StringBuilder.h"
#include "view/LayoutInflater.h"
#include "R.h"

using namespace Elastos::Core;
using namespace Elastos::Droid::View;

using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Text::Format::IDateFormat;
using Elastos::Droid::Text::Format::IDateFormatHelper;
// using Elastos::Math::IBigInteger;
using Elastos::Security::IMessageDigest;
using Elastos::Security::IMessageDigestHelper;
using Elastos::Text::ISimpleDateFormat;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

String CSslCertificate::ISO_8601_DATE_FORMAT = String("yyyy-MM-dd HH:mm:ssZ");
const String CSslCertificate::ISSUED_TO = String("issued-to");
const String CSslCertificate::ISSUED_BY = String("issued-by");
const String CSslCertificate::VALID_NOT_BEFORE = String("valid-not-before");
const String CSslCertificate::VALID_NOT_AFTER = String("valid-not-after");
const String CSslCertificate::X509_CERTIFICATE = String("x509-certificate");

CSslCertificate::CSslCertificate()
{}

ECode CSslCertificate::constructor(
    /* [in] */ const String& issuedTo,
    /* [in] */ const String& issuedBy,
    /* [in] */ const String& validNotBefore,
    /* [in] */ const String& validNotAfter)
{
    AutoPtr<IDate> notBefore = ParseDate(validNotBefore);
    AutoPtr<IDate> notAfter = ParseDate(validNotAfter);

    return Init(issuedTo, issuedBy, notBefore, notAfter, NULL);
}

ECode CSslCertificate::constructor(
    /* [in] */ const String& issuedTo,
    /* [in] */ const String& issuedBy,
    /* [in] */ Elastos::Utility::IDate* validNotBefore,
    /* [in] */ Elastos::Utility::IDate* validNotAfter)
{
    return Init(issuedTo, issuedBy, validNotBefore, validNotAfter, NULL);
}

ECode CSslCertificate::constructor(
    /* [in] */ IX509Certificate* x509Certificate)
{
    AutoPtr<IPrincipal> subDN;
    // TODO:
    // x509Certificate->GetSubjectDN((IPrincipal**)&subDN);
    String issuedTo;
    // subDN->GetName(&issuedTo);
    AutoPtr<IPrincipal> issbDN;
    // x509Certificate->GetSubjectDN((IPrincipal**)&issbDN);
    String issuedBy;
    // issbDN->GetName(&issuedBy);
    AutoPtr<IDate> notBefore;
    // x509Certificate->GetNotBefore((IDate**)&notBefore);
    AutoPtr<IDate> notAfter;
    // x509Certificate->GetNotAfter((IDate**)&notAfter);

    return Init(issuedTo, issuedBy, notBefore, notAfter, x509Certificate);
}

CSslCertificate::CSslCertificate(
        /* [in] */ const String& issuedTo,
        /* [in] */ const String& issuedBy,
        /* [in] */ Elastos::Utility::IDate* validNotBefore,
        /* [in] */ Elastos::Utility::IDate* validNotAfter,
        /* [in] */ IX509Certificate* x509Certificate)
{
    Init(issuedTo, issuedBy, validNotBefore, validNotAfter, x509Certificate);
}

ECode CSslCertificate::Init(
    /* [in] */ const String& issuedTo,
    /* [in] */ const String& issuedBy,
    /* [in] */ Elastos::Utility::IDate* validNotBefore,
    /* [in] */ Elastos::Utility::IDate* validNotAfter,
    /* [in] */ IX509Certificate* x509Certificate)
{
    // TODO:
    // CSslCertificateDName::New(issuedTo, (ISslCertificateDName**)&mIssuedTo);
    // CSslCertificateDName::New(issuedBy, (ISslCertificateDName**)&mIssuedBy);
    mValidNotBefore = CloneDate(validNotBefore);
    mValidNotAfter  = CloneDate(validNotAfter);
    mX509Certificate = x509Certificate;

    return NOERROR;
}

ECode CSslCertificate::GetValidNotBeforeDate(
    /* [out] */ Elastos::Utility::IDate** date)
{
    VALIDATE_NOT_NULL(date);
    *date = CloneDate(mValidNotBefore);
    return NOERROR;
}

ECode CSslCertificate::GetValidNotBefore(
    /* [out] */ String* valid)
{
    VALIDATE_NOT_NULL(valid);
    *valid = FormatDate(mValidNotBefore);
    return NOERROR;
}

ECode CSslCertificate::GetValidNotAfterDate(
    /* [out] */ Elastos::Utility::IDate** date)
{
    VALIDATE_NOT_NULL(date);
    *date = CloneDate(mValidNotAfter);
    return NOERROR;
}

ECode CSslCertificate::GetValidNotAfter(
    /* [out] */ String* valid)
{
    VALIDATE_NOT_NULL(valid);
    *valid = FormatDate(mValidNotAfter);
    return NOERROR;
}

ECode CSslCertificate::GetIssuedTo(
    /* [out] */ ISslCertificateDName** issuedTo)
{
    VALIDATE_NOT_NULL(issuedTo);
    *issuedTo = mIssuedTo;
    INTERFACE_ADDREF(*issuedTo);
    return NOERROR;
}

ECode CSslCertificate::GetIssuedBy(
    /* [out] */ ISslCertificateDName** issuedBy)
{
    VALIDATE_NOT_NULL(issuedBy);
    *issuedBy = mIssuedBy;
    INTERFACE_ADDREF(*issuedBy);
    return NOERROR;
}

String CSslCertificate::GetSerialNumber(
    /* [in] */ IX509Certificate* x509Certificate)
{
    if (x509Certificate == NULL) {
        return String("");
    }
    // TODO:
    // AutoPtr<IBigInteger> serialNumber;
    // x509Certificate->GetSerialNumber((IBigInteger**)&serialNumber);
    // if (serialNumber == NULL) {
        // return String("");
    // }
    AutoPtr<ArrayOf<Byte> > serArray;
    // serialNumber->toByteArray((ArrayOf<Byte>**)&serArray);
    return Fingerprint(serArray);
}

String CSslCertificate::GetDigest(
    /* [in] */ IX509Certificate* x509Certificate,
    /* [in] */ const String& algorithm)
{
    if (x509Certificate == NULL) {
        return String("");
    }

    AutoPtr<ArrayOf<Byte> >bytes;
    // TODO:
    // x509Certificate->GetEncoded((ArrayOf<Byte> **)&bytes);
    AutoPtr<IMessageDigestHelper> helper;
    AutoPtr<IMessageDigest> md;
    // if(FAILED(helper->GetInstance(algorithm, (IMessageDigest**)&md))) {
    //     return String("");
    // }
    AutoPtr<ArrayOf<Byte> >digest;
    // if(FAILED(md->Digest(bytes, (ArrayOf<Byte>**)&digest))) {
    //     return String("");
    // }

    return Fingerprint(digest);
}

const String CSslCertificate::Fingerprint(
    /* [in] */ ArrayOf<Byte>* bytes)
{
    if (bytes == NULL) {
        return String("");
    }
    AutoPtr<StringBuilder> sb = new StringBuilder();
    for (Int32 i = 0; i < bytes->GetLength(); i++) {
        Byte b = (*bytes)[i];
        StringUtils::AppendByteAsHex(*sb, b, TRUE);
        if (i + 1 != bytes->GetLength()) {
            sb->AppendChar(':');
        }
    }
    return sb->ToString();
}

ECode CSslCertificate::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    String issuedTo;
    mIssuedTo->GetDName(&issuedTo);
    String issuedBy;
    mIssuedBy->GetDName(&issuedBy);

    *str = String("Issued to: ") + issuedTo + String(";\n")
        + String("Issued by: ") + issuedBy + String(";\n");
    return NOERROR;
}

AutoPtr<IDate> CSslCertificate::ParseDate(
    /* [in] */ const String& string)
{
    AutoPtr<ISimpleDateFormat> format;
    // TODO:
    // CSimpleDateFormat::New(ISO_8601_DATE_FORMAT, (ISimpleDateFormat**)&format);
    AutoPtr<IDate> date;
    // if(!FAILED(format->ParseEx(string, &date))) {
    //     return date;
    // }

    return NULL;
}

String CSslCertificate::FormatDate(
    /* [in] */ IDate* date)
{
    AutoPtr<ISimpleDateFormat> format;
    // TODO:
    // CSimpleDateFormat::New(ISO_8601_DATE_FORMAT, (ISimpleDateFormat**)&format);
    String strDate;
    // format->Format(date, &strDate);

    return strDate;
}

AutoPtr<IDate> CSslCertificate::CloneDate(
    /* [in] */ IDate* date)
{
    if (date == NULL) {
        return NULL;
    }
    AutoPtr<IInterface> newDate;
    Elastos::Core::ICloneable::Probe(date)->Clone((IInterface**)&newDate);

    return IDate::Probe(newDate);
}

ECode CSslCertificate::InflateCertificateView(
    /* [in] */ Elastos::Droid::Content::IContext* context,
    /* [out] */ Elastos::Droid::View::IView** view)
{
    VALIDATE_NOT_NULL(view);

    AutoPtr<ILayoutInflater> factory;
    LayoutInflater::From(context, (ILayoutInflater**)&factory);

    AutoPtr<IView> certificateView;
    // factory->Inflate(R::layout::ssl_certificate, NULL, (IView**)&certificateView);

    // issued to:
    AutoPtr<ISslCertificateDName> issuedTo;
    GetIssuedTo((ISslCertificateDName**)&issuedTo);
    if (issuedTo != NULL) {
        AutoPtr<ITextView> commonView;
        // certificateView->FindViewById(R::id::to_common, (ITextView**)&commonView);
        String sCName;
        issuedTo->GetCName(&sCName);
        // commonView->SetText(sCName);

        AutoPtr<ITextView> orgView;
        // certificateView->FindViewById(R::id::to_org, (ITextView**)&orgView);
        String sOName;
        issuedTo->GetCName(&sOName);
        // commonView->SetText(sOName);

        AutoPtr<ITextView> unitView;
        // certificateView->FindViewById(R::id::to_org_unit, (ITextView**)&unitView);
        String sUName;
        issuedTo->GetUName(&sUName);
        // commonView->SetText(sUName);
    }
    // serial number:
    AutoPtr<ITextView> serView;
    // certificateView->FindViewById(R::id::serial_number, (ITextView**)&serView);
    String sSerNum = GetSerialNumber(mX509Certificate);
    // serView->SetText(sSerNum);

    // issued by:
    AutoPtr<ISslCertificateDName> issuedBy;
    GetIssuedBy((ISslCertificateDName**)&issuedBy);
    if (issuedBy != NULL) {
        AutoPtr<ITextView> commonView;
        // certificateView->FindViewById(R::id::by_common, (ITextView**)&commonView);
        String sCName;
        issuedBy->GetCName(&sCName);
        // commonView->SetText(sCName);

        AutoPtr<ITextView> orgView;
        // certificateView->FindViewById(R::id::by_org, (ITextView**)&orgView);
        String sOName;
        issuedBy->GetCName(&sOName);
        // commonView->SetText(sOName);

        AutoPtr<ITextView> unitView;
        // certificateView->FindViewById(R::id::by_org_unit, (ITextView**)&unitView);
        String sUName;
        issuedBy->GetUName(&sUName);
        // commonView->SetText(sUName);
    }

    // issued on:
    AutoPtr<IDate> beforeDate;
    GetValidNotBeforeDate((IDate**)&beforeDate);
    String issuedOn = FormatCertificateDate(context, beforeDate);
    AutoPtr<ITextView> isOnView;
    // certificateView->FindViewById(R::id::issued_on, (ItextView**)&isOnView);
    // isOnView->SetText(issuedOn);

    // expires on:
    AutoPtr<IDate> afterDate;
    GetValidNotAfterDate((IDate**)&afterDate);
    String expiresOn = FormatCertificateDate(context, afterDate);
    AutoPtr<ITextView> expOnView;
    // certificateView->FindViewById(R::id::issued_on, (ItextView**)&expOnView);
    // expOnView->SetText(expiresOn);

    // fingerprints:
    AutoPtr<ITextView> sha256View;
    // certificateView->FindViewById(R::id::sha256_fingerprint, (ItextView**)&sha256View);
    String sSha256Digest = GetDigest(mX509Certificate, String("SHA256"));
    // sha256View->SetText(sSha256Digest);

    AutoPtr<ITextView> sha1View;
    // certificateView->FindViewById(R::id::sha1_fingerprint, (ItextView**)&sha1View);
    String sSha1Digest = GetDigest(mX509Certificate, String("SHA1"));
    // sha1View->SetText(sSha1Digest);

    *view = (IView*)certificateView->Probe(EIID_IView);
    return NOERROR;
}

String CSslCertificate::FormatCertificateDate(
    /* [in] */ IContext* context,
    /* [in] */ IDate* certificateDate)
{
    if (certificateDate == NULL) {
        return String("");
    }
    AutoPtr<IDateFormatHelper> helper;
    // TODO:
    // CDateFormatHelper::AcquireSingleton((IDateFormatHelper**)&helper);
    AutoPtr<IDateFormat> format;
    // helper->GetDateFormat(context, (IDateFormat**)&format)
    String sdate;
    // format->Format(certificateDate, &sdate);

    return sdate;
}

}
}
}
}

