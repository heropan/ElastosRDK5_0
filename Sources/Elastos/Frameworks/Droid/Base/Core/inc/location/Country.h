
#ifndef __COUNTRY_H__
#define __COUNTRY_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Location {

/**
 * This class wraps the country information.
 *
 * @hide
 */
class Country
{
public:
    Country() {}

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
    Country(
        /* [in] */ const String& countryIso,
        /* [in] */ const Int32 source);

    Country(
        /* [in] */ ICountry* country);

    /**
     * @return the ISO 3166-1 two letters country code
     */
    virtual CARAPI GetCountryIso(
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
    virtual CARAPI GetSource(
        /* [out] */ Int32* source);

    /**
     * Returns the time that this object was created (which we assume to be the time that the source
     * was consulted).
     */
    virtual CARAPI GetTimestamp(
        /* [out] */ Int64* timestamp);

    virtual CARAPI DescribeContents(
        /* [out] */ Int32* result);

    virtual CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* parcel);

    /**
     * Compare the specified country to this country object ignoring the source
     * and timestamp fields, return true if the countryIso fields are equal
     *
     * @param country the country to compare
     * @return true if the specified country's countryIso field is equal to this
     *         country's, false otherwise.
     */
    virtual CARAPI EqualsIgnoreSource(
        /* [in] */ ICountry* country,
        /* [out] */ Boolean* result);

    //@Override
    virtual CARAPI ToString(
        /* [out] */ String* strOut);

protected:

    virtual CARAPI Init(
        /* [in] */ const String& countryIso,
        /* [in] */ const Int32 source);

    virtual CARAPI Init(
        /* [in] */ const String& countryIso,
        /* [in] */ const Int32 source,
        /* [in] */ Int64 timestamp);

    virtual CARAPI Init(
        /* [in] */ ICountry* country);

private:
    Country(
        /* [in] */ const String& countryIso,
        /* [in] */ const Int32 source,
        /* [in] */ Int64 timestamp);

private:
    /**
     * The ISO 3166-1 two letters country code.
     */
    String mCountryIso;

    /**
     * Where the country code came from.
     */
    Int32 mSource;

    Int32 mHashCode;

    /**
     * Time that this object was created (which we assume to be the time that the source was
     * consulted). This time is in milliseconds since boot up.
     */
    Int64 mTimestamp;
};

}//namespace Location
}//namespace Droid
}//namespace Elastos

#endif //__COUNTRY_H__
