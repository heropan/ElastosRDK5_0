
#ifndef __ELASTOS_DROID_LOCATION_CGEOCODER_H__
#define __ELASTOS_DROID_LOCATION_CGEOCODER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "_Elastos_Droid_Location_CGeocoder.h"

using Elastos::Droid::Content::IContext;
using Libcore::ICU::ILocale;

namespace Elastos {
namespace Droid {
namespace Location {

CarClass(CGeocoder)
{
private:
    static const String TAG;

public:
    /**
     * Returns true if the Geocoder methods getFromLocation and
     * getFromLocationName are implemented.  Lack of network
     * connectivity may still cause these methods to return null or
     * empty lists.
     */
    static CARAPI IsPresent(
        /* [out] */ Boolean* isPresent);

    /**
     * Returns an array of Addresses that are known to describe the
     * area immediately surrounding the given latitude and longitude.
     * The returned addresses will be localized for the locale
     * provided to this class's constructor.
     *
     * <p> The returned values may be obtained by means of a network lookup.
     * The results are a best guess and are not guaranteed to be meaningful or
     * correct. It may be useful to call this method from a thread separate from your
     * primary UI thread.
     *
     * @param latitude the latitude a point for the search
     * @param longitude the longitude a point for the search
     * @param maxResults max number of addresses to return. Smaller numbers (1 to 5) are recommended
     *
     * @return a list of Address objects. Returns null or empty list if no matches were
     * found or there is no backend service available.
     *
     * @throws IllegalArgumentException if latitude is
     * less than -90 or greater than 90
     * @throws IllegalArgumentException if longitude is
     * less than -180 or greater than 180
     * @throws IOException if the network is unavailable or any other
     * I/O problem occurs
     */
    CARAPI GetFromLocation(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Int32 maxResults,
        /* [out] */ IObjectContainer** addressContainer);

    /**
     * Returns an array of Addresses that are known to describe the
     * named location, which may be a place name such as "Dalvik,
     * Iceland", an address such as "1600 Amphitheatre Parkway,
     * Mountain View, CA", an airport code such as "SFO", etc..  The
     * returned addresses will be localized for the locale provided to
     * this class's constructor.
     *
     * <p> The query will block and returned values will be obtained by means of a network lookup.
     * The results are a best guess and are not guaranteed to be meaningful or
     * correct. It may be useful to call this method from a thread separate from your
     * primary UI thread.
     *
     * @param locationName a user-supplied description of a location
     * @param maxResults max number of results to return. Smaller numbers (1 to 5) are recommended
     *
     * @return a list of Address objects. Returns null or empty list if no matches were
     * found or there is no backend service available.
     *
     * @throws IllegalArgumentException if locationName is null
     * @throws IOException if the network is unavailable or any other
     * I/O problem occurs
     */
    CARAPI GetFromLocationName(
        /* [in] */ const String& locationName,
        /* [in] */ Int32 maxResults,
        /* [out] */ IObjectContainer** addressContainer);

    /**
     * Returns an array of Addresses that are known to describe the
     * named location, which may be a place name such as "Dalvik,
     * Iceland", an address such as "1600 Amphitheatre Parkway,
     * Mountain View, CA", an airport code such as "SFO", etc..  The
     * returned addresses will be localized for the locale provided to
     * this class's constructor.
     *
     * <p> You may specify a bounding box for the search results by including
     * the Latitude and Longitude of the Lower Left point and Upper Right
     * point of the box.
     *
     * <p> The query will block and returned values will be obtained by means of a network lookup.
     * The results are a best guess and are not guaranteed to be meaningful or
     * correct. It may be useful to call this method from a thread separate from your
     * primary UI thread.
     *
     * @param locationName a user-supplied description of a location
     * @param maxResults max number of addresses to return. Smaller numbers (1 to 5) are recommended
     * @param lowerLeftLatitude the latitude of the lower left corner of the bounding box
     * @param lowerLeftLongitude the longitude of the lower left corner of the bounding box
     * @param upperRightLatitude the latitude of the upper right corner of the bounding box
     * @param upperRightLongitude the longitude of the upper right corner of the bounding box
     *
     * @return a list of Address objects. Returns null or empty list if no matches were
     * found or there is no backend service available.
     *
     * @throws IllegalArgumentException if locationName is null
     * @throws IllegalArgumentException if any latitude is
     * less than -90 or greater than 90
     * @throws IllegalArgumentException if any longitude is
     * less than -180 or greater than 180
     * @throws IOException if the network is unavailable or any other
     * I/O problem occurs
     */
    CARAPI GetFromLocationName(
        /* [in] */ const String& locationName,
        /* [in] */ Int32 maxResults,
        /* [in] */ Double lowerLeftLatitude,
        /* [in] */ Double lowerLeftLongitude,
        /* [in] */ Double upperRightLatitude,
        /* [in] */ Double upperRightLongitude,
        /* [out] */ IObjectContainer** addressContainer);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ILocale* locale);

    CARAPI constructor(
        /* [in] */ IContext* context);

private:
    AutoPtr<IGeocoderParams> mParams;
    AutoPtr<IILocationManager> mService;
};

}//namespace Location
}//namespace Droid
}//namespace Elastos

#endif // __ELASTOS_DROID_LOCATION_CGEOCODER_H__
