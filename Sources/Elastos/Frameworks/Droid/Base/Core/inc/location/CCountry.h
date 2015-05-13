
#ifndef __CCOUNTRY_H__
#define __CCOUNTRY_H__

#include "ext/frameworkext.h"
#include "_CCountry.h"
#include "location/Country.h"

namespace Elastos {
namespace Droid {
namespace Location {

/**
 * This class wraps the country information.
 *
 * @hide
 */
CarClass(CCountry), public Country
{
public:
    CARAPI constructor();

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
    CARAPI constructor(
        /* [in] */ const String& countryIso,
        /* [in] */ const Int32 source);

    CARAPI constructor(
        /* [in] */ ICountry* country);

    /**
     * @return the ISO 3166-1 two letters country code
     */
    CARAPI GetCountryIso(
        /* [out] */ String* strOut);

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
    CARAPI GetSource(
        /* [out] */ Int32* source);

    /**
     * Returns the time that this object was created (which we assume to be the time that the source
     * was consulted).
     */
    CARAPI GetTimestamp(
        /* [out] */ Int64* timestamp);

    CARAPI DescribeContents(
        /* [out] */ Int32* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* parcel);

    /**
     * Compare the specified country to this country object ignoring the source
     * and timestamp fields, return true if the countryIso fields are equal
     *
     * @param country the country to compare
     * @return true if the specified country's countryIso field is equal to this
     *         country's, false otherwise.
     */
    CARAPI EqualsIgnoreSource(
        /* [in] */ ICountry* country,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI ToString(
        /* [out] */ String* strOut);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* rst);
};

}//namespace Location
}//namespace Droid
}//namespace Elastos

#endif //__CCOUNTRY_H__
