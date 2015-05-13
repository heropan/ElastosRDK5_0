
#include "location/CCountry.h"

namespace Elastos {
namespace Droid {
namespace Location {

ECode CCountry::constructor()
{
    return NOERROR;
}

/**
 * @param countryIso the ISO 3166-1 two letters country code.
 * @param source where the countryIso came from, could be one of below
 *        values
 *        <p>
 *        <ul>
 *        <li>{@link #COUNTRY_SOURCE_NETWORK}</li>
 *        <li>{@link #COUNTRY_SOURCE_LOCATION}</li>
 *        <li>{@link #COUNTRY_SOURCE_SIM}</li>
 *        <li>{@link #COUNTRY_SOURCE_LOCALE}</li>
 *        </ul>
 */
ECode CCountry::constructor(
    /* [in] */ const String& countryIso,
    /* [in] */ const Int32 source)
{
    return Country::Init(countryIso, source);
}

ECode CCountry::constructor(
    /* [in] */ ICountry* country)
{
    return Country::Init(country);
}

/**
 * @return the ISO 3166-1 two letters country code
 */
ECode CCountry::GetCountryIso(
    /* [out] */ String* strOut)
{
    return Country::GetCountryIso(strOut);
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
ECode CCountry::GetSource(
    /* [out] */ Int32* source)
{
    return Country::GetSource(source);
}

/**
 * Returns the time that this object was created (which we assume to be the time that the source
 * was consulted).
 */
ECode CCountry::GetTimestamp(
    /* [out] */ Int64* timestamp)
{
    return Country::GetTimestamp(timestamp);
}

ECode CCountry::DescribeContents(
    /* [out] */ Int32* result)
{
    return Country::DescribeContents(result);
}

ECode CCountry::ReadFromParcel(
        /* [in] */ IParcel* parcel)
{
    return Country::ReadFromParcel(parcel);
}

ECode CCountry::WriteToParcel(
    /* [in] */ IParcel* parcel)
{
    return Country::WriteToParcel(parcel);
}

/**
 * Compare the specified country to this country object ignoring the source
 * and timestamp fields, return true if the countryIso fields are equal
 *
 * @param country the country to compare
 * @return true if the specified country's countryIso field is equal to this
 *         country's, false otherwise.
 */
ECode CCountry::EqualsIgnoreSource(
    /* [in] */ ICountry* country,
    /* [out] */ Boolean* result)
{
    return Country::EqualsIgnoreSource(country, result);
}

//@Override
ECode CCountry::ToString(
    /* [out] */ String* strOut)
{
    return Country::ToString(strOut);
}

ECode CCountry::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* rst)
{
    AutoPtr<ICountry> obj = ICountry::Probe(object);
    if (obj == NULL) {
        *rst = FALSE;
        return NOERROR;
    }
    if (this == obj.Get()) {
        *rst = TRUE;
        return NOERROR;
    }
    Int32 source1, source2;
    GetSource(&source1);
    obj->GetSource(&source2);
    String str1, str2;
    GetCountryIso(&str1);
    obj->GetCountryIso(&str2);
    if (str1.Equals(str2) && source1 == source2) {
        *rst = TRUE;
        return NOERROR;
    }
    *rst = FALSE;
    return NOERROR;
}

} // namespace Location
} // namepsace Droid
} // namespace Elastos
