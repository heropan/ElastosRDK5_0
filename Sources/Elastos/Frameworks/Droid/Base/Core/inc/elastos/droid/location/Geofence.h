
#ifndef __ELASTOS_DROID_LOCATION_GEOFENCE_H__
#define __ELASTOS_DROID_LOCATION_GEOFENCE_H__

#include "elastos/droid/ext/frameworkext.h"

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
class Geofence
    : public Object
    , public IGeofence
    , public IParcelable
{
private:
    Geofence(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Float radius);

public:
    CAR_INTERFACE_DECL()

    Geofence();

    static CARAPI CreateCircle(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Float radius,
        /* [out] */ IGeofence** geofence);

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
    const Int32 mType;
    const Double mLatitude;
    const Double mLongitude;
    const Float mRadius;
};

}//namespace Location
}//namespace Droid
}//namespace Elastos

#endif // __ELASTOS_DROID_LOCATION_GEOFENCE_H__
