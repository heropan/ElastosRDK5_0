
#ifndef __ELASTOS_DROID_LOCATION_CGEOCODERPARAMS_H__
#define __ELASTOS_DROID_LOCATION_CGEOCODERPARAMS_H__

#include "_CGeocoderParams.h"

using Elastos::Droid::Content::IContext;
using Libcore::ICU::ILocale;

namespace Elastos {
namespace Droid {
namespace Location {

 /**
 * This class contains extra parameters to pass to an IGeocodeProvider
 * implementation from the Geocoder class.  Currently this contains the
 * language, country and variant information from the Geocoder's locale
 * as well as the Geocoder client's package name for geocoder server
 * logging.  This information is kept in a separate class to allow for
 * future expansion of the IGeocodeProvider interface.
 *
 * @hide
 */
CarClass(CGeocoderParams)
{
public:
    CARAPI constructor();

    /**
     * This object is only constructed by the Geocoder class
     *
     * @hide
     */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ILocale* locale);

    /**
     * returns the Geocoder's locale
     */
    CARAPI GetLocale(
        /* [out] */ ILocale** locale);

    /**
     * returns the package name of the Geocoder's client
     */
    CARAPI GetClientPackage(
        /* [out] */ String* packageName);

    CARAPI DescribeContents(
        /* [out] */ Int32* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* parcel);

private:
    AutoPtr<ILocale> mLocale;
    String mPackageName;
};

}//namespace Location
}//namespace Droid
}//namespace Elastos

#endif //__ELASTOS_DROID_LOCATION_CGEOCODERPARAMS_H__
