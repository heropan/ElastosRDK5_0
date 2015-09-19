
#ifndef __ELASTOS_DROID_LOCATION_CGEOFENCE_H__
#define __ELASTOS_DROID_LOCATION_CGEOFENCE_H__

#include "ext/frameworkext.h"
#include "_Elastos_Droid_Location_CGeofence.h"

namespace Elastos {
namespace Droid {
namespace Location {

/**
 * Represents a geographical boundary, also known as a geofence.
 *
 * <p>Currently only circular geofences are supported and they do not support altitude changes.
 *
 * @hide
 */
CarClass(CGeofence)
{
public:
    /**
     * Create a circular geofence (on a flat, horizontal plane).
     *
     * @param latitude latitude in degrees
     * @param longitude longitude in degrees
     * @param radius radius in meters
     * @return a new geofence
     * @throws IllegalArgumentException if any parameters are out of range
     */
    static AutoPtr<IGeofence> CreateCircle(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Float radius);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Float radius);

    /** @hide */
    CARAPI GetType(
        /* [out] */ Int32* type);

    /** @hide */
    CARAPI GetLatitude(
        /* [out] */ Double* latitude);

    /** @hide */
    CARAPI GetLongitude(
        /* [out] */ Double* longitude);

    /** @hide */
    CARAPI GetRadius(
        /* [out] */ Float* radius);

    //@Override
    CARAPI DescribeContents(
        /* [out] */ Int32* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    //@Override
    CARAPI WriteToParcel(
        /* [in] */ IParcel* parcel);

    //@Override
    CARAPI ToString(
        /* [out] */ String* strOut);

    CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* rst);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);
private:
    static CARAPI_(void) CheckRadius(
        /* [in] */ Float radius);

    static CARAPI_(void) CheckLatLong(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude);

    static CARAPI_(void) CheckType(
        /* [in] */ Int32 type);

    static CARAPI_(String) TypeToString(
        /* [in] */ Int32 type);

private:
    Int32 mType;
    Double mLatitude;
    Double mLongitude;
    Float mRadius;
};

}//namespace Location
}//namespace Droid
}//namespace Elastos

#endif // __ELASTOS_DROID_LOCATION_CGEOFENCE_H__
