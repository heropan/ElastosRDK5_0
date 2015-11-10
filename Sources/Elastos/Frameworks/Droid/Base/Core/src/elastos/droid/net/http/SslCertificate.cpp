
#include "elastos/droid/net/http/SslCertificate.h"

using Elastos::Droid::Text::Format::IDateFormat;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::Widget::ITextView;

using Elastos::IO::IByteArrayInputStream;
using Elastos::Math::IBigInteger;
using Elastos::Security::Cert::ICertificate;
using Elastos::Security::Cert::ICertificateFactory;
using Elastos::Security::IMessageDigest;
using Elastos::Text::ISimpleDateFormat;
using Elastos::Utility::IVector;

using Org::Bouncycastle::Asn1::X509::IX509Name;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {


CAR_INTERFACE_IMPL(SslCertificate, Object, ISslCertificate)

String SslCertificate::sISO_8601_DATE_FORMAT("yyyy-MM-dd HH:mm:ssZ");
const String SslCertificate::ISSUED_TO("issued-to");
const String SslCertificate::ISSUED_BY("issued-by");
const String SslCertificate::VALID_NOT_BEFORE("valid-not-before");
const String SslCertificate::VALID_NOT_AFTER("valid-not-after");
const String SslCertificate::X509_CERTIFICATE("x509-certificate");

SslCertificate::SslCertificate()
{}

SslCertificate::SslCertificate(
        /* [in] */ const String& issuedTo,
        /* [in] */ const String& issuedBy,
        /* [in] */ Elastos::Utility::IDate* validNotBefore,
        /* [in] */ Elastos::Utility::IDate* validNotAfter,
        /* [in] */ IX509Certificate* x509Certificate)
{
    Init(issuedTo, issuedBy, validNotBefore, validNotAfter, x509Certificate);
}

ECode SslCertificate::SaveState(
    /* [in] */ ISslCertificate* certificate,
    /* [out] */ IBundle** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(bundle);

    if (certificate == NULL) {
        *bundle = NULL;
        return NOERROR;
    }

    AutoPtr<IBundle> localbundle;
    // TODO:
    // CBundle::New((IBundle**)&localbundle);
    AutoPtr<ISslCertificateDName> dNameTo;
    certificate->GetIssuedTo((ISslCertificateDName**)&dNameTo);
    String strTo;
    dNameTo->GetDName(&strTo);
    localbundle->PutString(CSslCertificate::ISSUED_TO, strTo);
    AutoPtr<ISslCertificateDName> dNameBy;
    certificate->GetIssuedTo((ISslCertificateDName**)&dNameBy);
    String strBy;
    dNameBy->GetDName(&strBy);
    localbundle->PutString(CSslCertificate::ISSUED_BY, strBy);
    String strNotBefore;
    certificate->GetValidNotBefore(&strNotBefore);
    localbundle->PutString(CSslCertificate::VALID_NOT_BEFORE, strNotBefore);
    String strNotAfter;
    certificate->GetValidNotAfter(&strNotAfter);
    localbundle->PutString(CSslCertificate::VALID_NOT_AFTER, strNotAfter);
    AutoPtr<IX509Certificate> x509Certificate = ((CSslCertificate*)certificate)->mX509Certificate;
    if (x509Certificate != NULL) {
        AutoPtr<ArrayOf<Byte> > bytesEncoded;
        // TODO:
        // x509Certificate->GetEncoded(&bytesEncoded);
        // localbundle->PutByteArray(CSslCertificate::X509_CERTIFICATE, bytesEncoded);

    }
    *bundle = localbundle;
    REFCOUNT_ADD(*bundle);
    return NOERROR;
#endif
}

ECode SslCertificate::RestoreState(
    /* [in] */ IBundle* bundle,
    /* [out] */ ISslCertificate** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(certificate);

    if (bundle == NULL) {
        *certificate = NULL;
        return NOERROR;
    }

    AutoPtr<IX509Certificate> x509Certificate;
    AutoPtr<ArrayOf<Byte> > bytes;
    // TODO:
    // bundle->GetByteArray(CSslCertificate::X509_CERTIFICATE, &bytes);
    if (bytes != NULL) {
        AutoPtr<ICertificateFactoryHelper> helper;
        // CCertificateFactoryHelper::AcquireSingleton((ICertificateFactoryHelper**)&helper);
        AutoPtr<ICertificateFactory> certFactory;
        // helper->GetInstance(String("X.509"), (ICertificateFactory**)&certFactory);
        AutoPtr<ICertificate> cert;
        AutoPtr<IByteArrayInputStream> stream;
        // CByteArrayInputStream::New(bytes, (IByteArrayInputStream**)&stream);
        // if (!FAILED(certFactory->GenerateCertificate(stream, (ICertificate**)&cert))) {
        //     x509Certificate = (IX509Certificate*)cert->Probe(EIID_IX509Certificate);
        // }
    }

    String issuedTo;
    bundle->GetString(CSslCertificate::ISSUED_TO, &issuedTo);
    String issuedBy;
    bundle->GetString(CSslCertificate::ISSUED_BY, &issuedBy);
    String strNotBefore;
    bundle->GetString(CSslCertificate::VALID_NOT_BEFORE, &issuedBy);
    AutoPtr<IDate> dateNOtBefore = CSslCertificate::ParseDate(strNotBefore);
    String strNotAfter;
    bundle->GetString(CSslCertificate::VALID_NOT_AFTER, &strNotAfter);
    AutoPtr<IDate> dateNOtAfter = CSslCertificate::ParseDate(strNotBefore);

    CSslCertificate* cert = new CSslCertificate(issuedTo, issuedBy, dateNOtBefore, dateNOtAfter, x509Certificate);
    *certificate = (ISslCertificate*)cert->Probe(EIID_ISslCertificate);
    REFCOUNT_ADD(*certificate);
    return NOERROR;
#endif
}

ECode SslCertificate::constructor(
    /* [in] */ const String& issuedTo,
    /* [in] */ const String& issuedBy,
    /* [in] */ const String& validNotBefore,
    /* [in] */ const String& validNotAfter)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<IDate> notBefore = ParseDate(validNotBefore);
    AutoPtr<IDate> notAfter = ParseDate(validNotAfter);

    return Init(issuedTo, issuedBy, notBefore, notAfter, NULL);
#endif
}

ECode SslCertificate::constructor(
    /* [in] */ const String& issuedTo,
    /* [in] */ const String& issuedBy,
    /* [in] */ IDate* validNotBefore,
    /* [in] */ IDate* validNotAfter)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    return Init(issuedTo, issuedBy, validNotBefore, validNotAfter, NULL);
#endif
}

ECode SslCertificate::constructor(
    /* [in] */ IX509Certificate* certificate)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

SslCertificate::Init(
    /* [in] */ const String& issuedTo,
    /* [in] */ const String& issuedBy,
    /* [in] */ IDate* validNotBefore,
    /* [in] */ IDate* validNotAfter,
    /* [in] */ IX509Certificate* x509Certificate)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // TODO:
    // CSslCertificateDName::New(issuedTo, (ISslCertificateDName**)&mIssuedTo);
    // CSslCertificateDName::New(issuedBy, (ISslCertificateDName**)&mIssuedBy);
    mValidNotBefore = CloneDate(validNotBefore);
    mValidNotAfter  = CloneDate(validNotAfter);
    mX509Certificate = x509Certificate;

    return NOERROR;
#endif
}

ECode SslCertificate::GetValidNotBeforeDate(
    /* [out] */ IDate** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(date);
    *date = CloneDate(mValidNotBefore);
    return NOERROR;
#endif
}

ECode SslCertificate::GetValidNotBefore(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(valid);
    *valid = FormatDate(mValidNotBefore);
    return NOERROR;
#endif
}

ECode SslCertificate::GetValidNotAfterDate(
    /* [out] */ IDate** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(date);
    *date = CloneDate(mValidNotAfter);
    return NOERROR;
#endif
}

ECode SslCertificate::GetValidNotAfter(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(valid);
    *valid = FormatDate(mValidNotAfter);
    return NOERROR;
#endif
}

ECode SslCertificate::GetIssuedTo(
    /* [out] */ ISslCertificateDName** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(issuedTo);
    *issuedTo = mIssuedTo;
    REFCOUNT_ADD(*issuedTo);
    return NOERROR;
#endif
}

ECode SslCertificate::GetIssuedBy(
    /* [out] */ ISslCertificateDName** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(issuedBy);
    *issuedBy = mIssuedBy;
    REFCOUNT_ADD(*issuedBy);
    return NOERROR;
#endif
}

ECode SslCertificate::GetSerialNumber(
    /* [in] */ IX509Certificate* x509Certificate,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

ECode SslCertificate::GetDigest(
    /* [in] */ IX509Certificate* x509Certificate,
    /* [in] */ const String& algorithm,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

ECode SslCertificate::Fingerprint(
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

ECode SslCertificate::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(str);

    String issuedTo;
    mIssuedTo->GetDName(&issuedTo);
    String issuedBy;
    mIssuedBy->GetDName(&issuedBy);

    *str = String("Issued to: ") + issuedTo + String(";\n")
        + String("Issued by: ") + issuedBy + String(";\n");
    return NOERROR;
#endif
}

ECode SslCertificate::ParseDate(
    /* [in] */ const String& string,
    /* [out] */ IDate** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<ISimpleDateFormat> format;
    // TODO:
    // CSimpleDateFormat::New(ISO_8601_DATE_FORMAT, (ISimpleDateFormat**)&format);
    AutoPtr<IDate> date;
    // if(!FAILED(format->Parse(string, &date))) {
    //     return date;
    // }

    return NULL;
#endif
}

ECode SslCertificate::FormatDate(
    /* [in] */ IDate* date,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<ISimpleDateFormat> format;
    // TODO:
    // CSimpleDateFormat::New(ISO_8601_DATE_FORMAT, (ISimpleDateFormat**)&format);
    String strDate;
    // format->Format(date, &strDate);

    return strDate;
#endif
}

ECode SslCertificate::CloneDate(
    /* [in] */ IDate* date,
    /* [out] */ IDate** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (date == NULL) {
        return NULL;
    }
    AutoPtr<IInterface> newDate;
    Elastos::Core::ICloneable::Probe(date)->Clone((IInterface**)&newDate);

    return IDate::Probe(newDate);
#endif
}

ECode SslCertificate::InflateCertificateView(
    /* [in] */ IContext* context,
    /* [out] */ IView** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

ECode SslCertificate::FormatCertificateDate(
    /* [in] */ IContext* context,
    /* [in] */ IDate* certificateDate,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

//===============================================================
// SslCertificateDName
//===============================================================
CAR_INTERFACE_IMPL(SslCertificateDName, Object, ISslCertificateDName)

ECode SslCertificateDName::constructor(
    /* [in] */ const String& dName)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (dName != NULL) {
        mDName = dName;

        AutoPtr<IX509Name> x509Name;
        // TODO:
        // CX509Name::New(dName, (IX509Name**)&x509Name);

        Vector<String>* val;
        // x509Name->GetValues(&val);
        Vector<String>* oid;
        // x509Name->GetOIDs(&oid);

        // for (Int32 i = 0; i < oid->GetSize(); i++) {
        //     if (oid->At(i).Equals(IX509Name::CN)) {
        //         if (mCName.IsNull()) {
        //             mCName = (String) val->At(i);
        //         }
        //         continue;
        //     }

        //     if (oid->At(i).Equals(IX509Name::O)) {
        //         if (mOName.IsNull()) {
        //             mOName = (String) val->At(i);
        //         }
        //         continue;
        //     }

        //     if (oid->At(i).Equals(IX509Name::OU)) {
        //         if (mUName.IsNull()) {
        //             mUName = (String) val->At(i);
        //         }
        //         continue;
        //     }
        // }
    }
    return NOERROR;
#endif
}

ECode SslCertificateDName::GetDName(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(dName);
    if (mDName.IsNullOrEmpty()) {
        *dName = String("");
    } else {
        *dName = mDName;
    }
    return NOERROR;
#endif
}

ECode SslCertificateDName::GetCName(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(cName);
    if (mCName.IsNullOrEmpty()) {
        *cName = String("");
    } else {
        *cName = mCName;
    }
    return NOERROR;
#endif
}

ECode SslCertificateDName::GetOName(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(oName);
    if (mOName.IsNullOrEmpty()) {
        *oName = String("");
    } else {
        *oName = mOName;
    }
    return NOERROR;
#endif
}

ECode SslCertificateDName::GetUName(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(uName);
    if (mUName.IsNullOrEmpty()) {
        *uName = String("");
    } else {
        *uName = mUName;
    }
    return NOERROR;
#endif
}


} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
