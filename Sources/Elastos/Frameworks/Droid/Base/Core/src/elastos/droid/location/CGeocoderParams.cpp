
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/location/CGeocoderParams.h"

using Elastos::Utility::CLocale;

namespace Elastos {
namespace Droid {
namespace Location {

ECode CGeocoderParams::constructor()
{
    return NOERROR;
}

/**
 * This object is only constructed by the Geocoder class
 *
 * @hide
 */
ECode CGeocoderParams::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ILocale* locale)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(locale);
    mLocale = locale;
    context->GetPackageName(&mPackageName);

    return NOERROR;
}

/**
 * returns the Geocoder's locale
 */
ECode CGeocoderParams::GetLocale(
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);
    *locale = mLocale;
    REFCOUNT_ADD(*locale);
    return NOERROR;
}

/**
 * returns the package name of the Geocoder's client
 */
ECode CGeocoderParams::GetClientPackage(
    /* [out] */ String* packageName)
{
    VALIDATE_NOT_NULL(packageName);
    *packageName = mPackageName;

    return NOERROR;
}

ECode CGeocoderParams::DescribeContents(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    return NOERROR;
}

ECode CGeocoderParams::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    String language;
    String country;
    String variant;

    parcel->ReadString(&language);
    parcel->ReadString(&country);
    parcel->ReadString(&variant);

    CLocale::New(language, country, variant, (ILocale**)&mLocale);
    parcel->ReadString(&mPackageName);

    return NOERROR;
}

ECode CGeocoderParams::WriteToParcel(
    /* [in] */ IParcel* parcel)
{
    VALIDATE_NOT_NULL(parcel);

    String language, country, variant;
    mLocale->GetLanguage(&language);
    mLocale->GetCountry(&country);
    mLocale->GetVariant(&variant);

    parcel->WriteString(language);
    parcel->WriteString(country);
    parcel->WriteString(variant);
    parcel->WriteString(mPackageName);

    return NOERROR;
}

}//namespace Location
}//namespace Droid
}//namespace Elastos
