
#include "elastos/droid/location/Country.h"
#include "elastos/droid/os/SystemClock.h"

#include <elastos/core/StringBuilder.h>

using Elastos::Utility::ILocale;
using Elastos::Core::StringBuilder;
using Elastos::Droid::Os::SystemClock;

namespace Elastos {
namespace Droid {
namespace Location {

Country::Country(
    /* [in] */ const String& countryIso,
    /* [in] */ const Int32 source)
{
    Init(countryIso, source);
}

Country::Country(
    /* [in] */ const String& countryIso,
    /* [in] */ const Int32 source,
    /* [in] */ Int64 timestamp)
{
    Init(countryIso, source, timestamp);
}

Country::Country(
    /* [in] */ ICountry* country)
{
    Init(country);
}

/**
 * @return the ISO 3166-1 two letters country code
 */
ECode Country::GetCountryIso(
    /* [out] */ String* strOut)
{
    VALIDATE_NOT_NULL(strOut);
    *strOut = mCountryIso;

    return NOERROR;
}

/**
 * @return where the country code came from, could be one of below values
 *         <p>
 *         <ul>
 *         <li>{@link #COUNTRY_SOURCE_NETWORK}</li>
 *         <li>{@link #COUNTRY_SOURCE_LOCATION}</li>
 *         <li>{@link #COUNTRY_SOURCE_SIM}</li>
 *         <li>{@link #COUNTRY_SOURCE_LOCALE}</li>
 *         </ul>
 */
ECode Country::GetSource(
    /* [out] */ Int32* source)
{
    VALIDATE_NOT_NULL(source);
    *source = mSource;

    return NOERROR;
}

/**
 * Returns the time that this object was created (which we assume to be the time that the source
 * was consulted).
 */
ECode Country::GetTimestamp(
    /* [out] */ Int64* timestamp)
{
    VALIDATE_NOT_NULL(timestamp);
    *timestamp = mTimestamp;

    return NOERROR;
}

ECode Country::DescribeContents(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    return NOERROR;
}

ECode Country::ReadFromParcel(
        /* [in] */ IParcel* parcel)
{
    parcel->ReadString(&mCountryIso);
    parcel->ReadInt32(&mSource);
    parcel->ReadInt64(&mTimestamp);

    return NOERROR;
}

ECode Country::WriteToParcel(
    /* [in] */ IParcel* parcel)
{
    VALIDATE_NOT_NULL(parcel);
    parcel->WriteString(mCountryIso);
    parcel->WriteInt32(mSource);
    parcel->WriteInt64(mTimestamp);

    return NOERROR;
}

/**
 * Compare the specified country to this country object ignoring the source
 * and timestamp fields, return true if the countryIso fields are equal
 *
 * @param country the country to compare
 * @return true if the specified country's countryIso field is equal to this
 *         country's, false otherwise.
 */
ECode Country::EqualsIgnoreSource(
    /* [in] */ ICountry* country,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(country);
    VALIDATE_NOT_NULL(result);

//    *result = country != NULL && mCountryIso.equals(country.getCountryIso());

    return NOERROR;
}

//@Override
ECode Country::ToString(
    /* [out] */ String* strOut)
{
    VALIDATE_NOT_NULL(strOut);
    //return "Country {ISO=" + mCountryIso + ", source=" + mSource + ", time=" + mTimestamp + "}";
    StringBuilder sbc("Country {ISO=");
    sbc.AppendString(mCountryIso);
    sbc.AppendCStr(", source=");
    sbc.AppendInt32(mSource);
    sbc.AppendCStr(", time=");
    sbc.AppendInt64(mTimestamp);
    sbc.AppendCStr("}");
    sbc.ToString(strOut);

    return NOERROR;
}

ECode Country::Init(
    /* [in] */ const String& countryIso,
    /* [in] */ const Int32 source)
{
    //if (countryIso == null || source < COUNTRY_SOURCE_NETWORK
    //        || source > COUNTRY_SOURCE_LOCALE) {
    //    throw new IllegalArgumentException();
    //}
    if (countryIso.IsNullOrEmpty() || source < ICountry::COUNTRY_SOURCE_NETWORK
            || source > ICountry::COUNTRY_SOURCE_LOCALE) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    //mCountryIso = countryIso.ToUpperCase(ILocale::US);
    mCountryIso = countryIso.ToUpperCase();

    mSource = source;
    mTimestamp = SystemClock::GetElapsedRealtime();

    return NOERROR;
}

ECode Country::Init(
    /* [in] */ const String& countryIso,
    /* [in] */ const Int32 source,
    /* [in] */ Int64 timestamp)
{
    //if (countryIso == null || source < COUNTRY_SOURCE_NETWORK
    //        || source > COUNTRY_SOURCE_LOCALE) {
    //    throw new IllegalArgumentException();
    //}
    if (countryIso.IsNullOrEmpty() || source < ICountry::COUNTRY_SOURCE_NETWORK
            || source > ICountry::COUNTRY_SOURCE_LOCALE) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    //mCountryIso = countryIso.ToUpperCase(Locale.US);
    mCountryIso = countryIso.ToUpperCase();

    mSource = source;
    mTimestamp = timestamp;

    return NOERROR;
}

ECode Country::Init(
    /* [in] */ ICountry* country)
{
    VALIDATE_NOT_NULL(country);
    Country* _country = (Country*)country;

    mCountryIso = _country->mCountryIso;
    mSource = _country->mSource;
    mTimestamp = _country->mTimestamp;

    return NOERROR;
}

} // namespace Location
} // namepsace Droid
} // namespace Elastos
