
#include "location/CAddress.h"
#include "os/CBundle.h"
#include <elastos/core/Math.h>

using Libcore::ICU::CLocale;
using Elastos::Droid::Os::CBundle;

namespace Elastos {
namespace Droid {
namespace Location {

CAddress::CAddress()
    : mMaxAddressLineIndex(-1)
    , mHasLatitude(FALSE)
    , mHasLongitude(FALSE)
{
}

CAddress::~CAddress()
{
    mAddressLines = NULL;
}

ECode CAddress::GetLocale(
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);
    *locale = mLocale;
    REFCOUNT_ADD(mLocale);

    return NOERROR;
}

ECode CAddress::GetMaxAddressLineIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mMaxAddressLineIndex;

    return NOERROR;
}

ECode CAddress::GetAddressLine(
    /* [in] */ Int32 index,
    /* [out] */ String* line)
{
    VALIDATE_NOT_NULL(line);
    if (index < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("index = " + index + " < 0");
    }

    if (mAddressLines != NULL) {
        HashMap<Int32, String>::Iterator it = mAddressLines->Find(index);
        if (it != mAddressLines->End()) {
            *line = it->mSecond;
        }
    }
    else {
        *line = String(NULL);
    }

    return NOERROR;
}

ECode CAddress::SetAddressLine(
    /* [in] */ Int32 index,
    /* [in] */ const String& line)
{
    if (index < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("index = " + index + " < 0");
    }
    if (mAddressLines == NULL) {
        mAddressLines = new HashMap<Int32, String>();
    }
    (*mAddressLines)[index] = line;

    if (line.IsNull()) {
        // We've eliminated a line, recompute the max index
        mMaxAddressLineIndex = -1;
        HashMap<Int32, String>::Iterator it;
        for (it = mAddressLines->Begin(); it != mAddressLines->End(); ++it) {
            mMaxAddressLineIndex = Elastos::Core::Math::Max(mMaxAddressLineIndex, it->mFirst);
        }
    }
    else {
        mMaxAddressLineIndex = Elastos::Core::Math::Max(mMaxAddressLineIndex, index);
    }
    return NOERROR;
}

ECode CAddress::GetFeatureName(
    /* [out] */ String* featureName)
{
    VALIDATE_NOT_NULL(featureName);
    *featureName = mFeatureName;

    return NOERROR;
}

ECode CAddress::SetFeatureName(
    /* [in] */ const String& featureName)
{
    mFeatureName = featureName;

    return NOERROR;
}

ECode CAddress::GetAdminArea(
    /* [out] */ String* adminArea)
{
    VALIDATE_NOT_NULL(adminArea);
    *adminArea = mAdminArea;

    return NOERROR;
}

ECode CAddress::SetAdminArea(
    /* [in] */ const String& adminArea)
{
    mAdminArea = adminArea;

    return NOERROR;
}

ECode CAddress::GetSubAdminArea(
    /* [out] */ String* subAdminArea)
{
    VALIDATE_NOT_NULL(subAdminArea);
    *subAdminArea = mSubAdminArea;

    return NOERROR;
}

ECode CAddress::SetSubAdminArea(
    /* [in] */ const String& subAdminArea)
{
    mSubAdminArea = subAdminArea;

    return NOERROR;
}

ECode CAddress::GetLocality(
    /* [out] */ String* locality)
{
    VALIDATE_NOT_NULL(locality);
    *locality = mLocality;

    return NOERROR;
}

ECode CAddress::SetLocality(
    /* [in] */ const String& locality)
{
    mLocality = locality;

    return NOERROR;
}

ECode CAddress::GetSubLocality(
    /* [out] */ String* sublocality)
{
    VALIDATE_NOT_NULL(sublocality);
    *sublocality = mSubLocality;

    return NOERROR;
}

ECode CAddress::SetSubLocality(
    /* [in] */ const String& sublocality)
{
    mSubLocality = sublocality;

    return NOERROR;
}

ECode CAddress::GetThoroughfare(
    /* [out] */ String* thoroughfare)
{
    VALIDATE_NOT_NULL(thoroughfare);
    *thoroughfare = mThoroughfare;

    return NOERROR;
}

ECode CAddress::SetThoroughfare(
    /* [in] */ const String& thoroughfare)
{
    mThoroughfare = thoroughfare;

    return NOERROR;
}

ECode CAddress::GetSubThoroughfare(
    /* [out] */ String* subthoroughfare)
{
    VALIDATE_NOT_NULL(subthoroughfare);
    *subthoroughfare = mSubThoroughfare;

    return NOERROR;
}

ECode CAddress::SetSubThoroughfare(
    /* [in] */ const String& subthoroughfare)
{
    mSubThoroughfare = subthoroughfare;

    return NOERROR;
}

ECode CAddress::GetPremises(
    /* [out] */ String* premises)
{
    VALIDATE_NOT_NULL(premises);
    *premises = mPremises;

    return NOERROR;
}

ECode CAddress::SetPremises(
    /* [in] */ const String& premises)
{
    mPremises = premises;

    return NOERROR;
}

ECode CAddress::GetPostalCode(
    /* [out] */ String* postalCode)
{
    VALIDATE_NOT_NULL(postalCode);
    *postalCode = mPostalCode;

    return NOERROR;
}

ECode CAddress::SetPostalCode(
    /* [in] */ const String& postalCode)
{
    mPostalCode = postalCode;

    return NOERROR;
}

ECode CAddress::GetCountryCode(
    /* [out] */ String* countryCode)
{
    VALIDATE_NOT_NULL(countryCode);
    *countryCode = mCountryCode;

    return NOERROR;
}

ECode CAddress::SetCountryCode(
    /* [in] */ const String& countryCode)
{
    mCountryCode = countryCode;

    return NOERROR;
}

ECode CAddress::GetCountryName(
    /* [out] */ String* countryName)
{
    VALIDATE_NOT_NULL(countryName);
    *countryName = mCountryName;

    return NOERROR;
}

ECode CAddress::SetCountryName(
    /* [in] */ const String& countryName)
{
    mCountryName = countryName;

    return NOERROR;
}

ECode CAddress::HasLatitude(
    /* [out] */ Boolean* hasLatitude)
{
    VALIDATE_NOT_NULL(hasLatitude);
    *hasLatitude = mHasLatitude;

    return NOERROR;
}

ECode CAddress::GetLatitude(
    /* [out] */ Double* latitude)
{
    VALIDATE_NOT_NULL(latitude);

    if (mHasLatitude) {
        *latitude = mLatitude;
        return NOERROR;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalStateException();
    }
}

ECode CAddress::SetLatitude(
    /* [in] */ Double latitude)
{
    mLatitude = latitude;
    mHasLatitude = TRUE;

    return NOERROR;
}

ECode CAddress::ClearLatitude()
{
    mHasLatitude = FALSE;

    return NOERROR;
}

ECode CAddress::HasLongitude(
    /* [out] */ Boolean* hasLongitude)
{
    VALIDATE_NOT_NULL(hasLongitude);
    *hasLongitude = mHasLongitude;

    return NOERROR;
}

ECode CAddress::GetLongitude(
    /* [out] */ Double* longitude)
{
    VALIDATE_NOT_NULL(longitude);

    if (mHasLongitude) {
        *longitude =mLongitude;
        return NOERROR;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalStateException();
    }
}

ECode CAddress::SetLongitude(
    /* [in] */ Double longitude)
{
    mLongitude = longitude;
    mHasLongitude = TRUE;

    return NOERROR;
}

ECode CAddress::ClearLongitude()
{
    mHasLongitude = FALSE;

    return NOERROR;
}

ECode CAddress::GetPhone(
    /* [out] */ String* phone)
{
    VALIDATE_NOT_NULL(phone);
    *phone = mPhone;

    return NOERROR;
}

ECode CAddress::SetPhone(
    /* [in] */ const String& phone)
{
    mPhone = phone;

    return NOERROR;
}

ECode CAddress::GetUrl(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    *url = mUrl;

    return NOERROR;
}

ECode CAddress::SetUrl(
    /* [in] */ const String& Url)
{
    mUrl = Url;

    return NOERROR;
}

ECode CAddress::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras);
    *extras = mExtras.Get();
    REFCOUNT_ADD(*extras);

    return NOERROR;
}

ECode CAddress::SetExtras(
    /* [in] */ IBundle* extras)
{
    if (extras != NULL) {
        ASSERT_SUCCEEDED(CBundle::New(extras, (IBundle**)&mExtras));
    }
    else {
        mExtras = NULL;
    }

    return NOERROR;
}

ECode CAddress::GetDescription(
    /* [out] */ String* str)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAddress::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    String language, country;
    source->ReadString(&language);
    source->ReadString(&country);
    AutoPtr<ILocale> locale;
    !country.IsNullOrEmpty() ?
        CLocale::New(language, country, (ILocale**)&locale) :
        CLocale::New(language, (ILocale**)&locale);
    AutoPtr<CAddress> a;
    ASSERT_SUCCEEDED(CAddress::NewByFriend(locale, (CAddress**)&a));

    Int32 N;
    source->ReadInt32(&N);
    if (N > 0) {
        a->mAddressLines = new HashMap<Int32, String>(N);
        for (Int32 i = 0; i < N; i++) {
            Int32 index;
            source->ReadInt32(&index);
            String line;
            source->ReadString(&line);
            (*(a->mAddressLines))[index] = line;
            a->mMaxAddressLineIndex =
                Elastos::Core::Math::Max(a->mMaxAddressLineIndex, index);
        }
    }
    else {
        a->mAddressLines = NULL;
        a->mMaxAddressLineIndex = -1;
    }
    source->ReadString(&(a->mFeatureName));
    source->ReadString(&(a->mAdminArea));
    source->ReadString(&(a->mSubAdminArea));
    source->ReadString(&(a->mLocality));
    source->ReadString(&(a->mSubLocality));
    source->ReadString(&(a->mThoroughfare));
    source->ReadString(&(a->mSubThoroughfare));
    source->ReadString(&(a->mPremises));
    source->ReadString(&(a->mPostalCode));
    source->ReadString(&(a->mCountryCode));
    source->ReadString(&(a->mCountryName));
    Int32 value;
    source->ReadInt32(&value);
    a->mHasLatitude = value == 0 ? FALSE : TRUE;
    if (a->mHasLatitude) {
        source->ReadDouble(&(a->mLatitude));
    }
    source->ReadInt32(&value);
    a->mHasLongitude = value == 0 ? FALSE : TRUE;
    if (a->mHasLongitude) {
        source->ReadDouble(&(a->mLongitude));
    }
    source->ReadString(&(a->mPhone));
    source->ReadString(&(a->mUrl));

    AutoPtr<IInterface> info;
    source->ReadInterfacePtrPtr((Handle32*)&info);
    a->mExtras = (info != NULL)? (IBundle*)
            info->Probe(Elastos::Droid::Os::EIID_IBundle) : NULL;

    return NOERROR;
}

ECode CAddress::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAddress::constructor()
{
    return NOERROR;
}

ECode CAddress::constructor(
    /* [in] */ ILocale* locale)
{
    mLocale = locale;

    return NOERROR;
}

}//namespace Location
}//namespace Droid
}//namespace Elastos
