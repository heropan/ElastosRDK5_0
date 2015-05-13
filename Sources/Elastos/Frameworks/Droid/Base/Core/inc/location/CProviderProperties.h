
#ifndef __PROVIDERPROPERTIES_H__
#define __PROVIDERPROPERTIES_H__

#include "_CProviderProperties.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Location {

/**
 * A Parcelable containing (legacy) location provider properties.
 * This object is just used inside the framework and system services.
 * @hide
 */
CarClass(CProviderProperties)
{
public:
    CProviderProperties();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Boolean mRequiresNetwork,
        /* [in] */ Boolean mRequiresSatellite,
        /* [in] */ Boolean mRequiresCell,
        /* [in] */ Boolean mHasMonetaryCost,
        /* [in] */ Boolean mSupportsAltitude,
        /* [in] */ Boolean mSupportsSpeed,
        /* [in] */ Boolean mSupportsBearing,
        /* [in] */ Int32 mPowerRequirement,
        /* [in] */ Int32 mAccuracy);

    //@Override
    CARAPI DescribeContents(
        /* [out] */ Int32* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    //@Override
    CARAPI WriteToParcel(
        /* [in] */ IParcel* parcel);

    /**
     * True if provider requires access to a
     * data network (e.g., the Internet), false otherwise.
     */
    CARAPI SetRequiresNetwork(
            /* [in] */ Boolean network);

    CARAPI GetRequiresNetwork(
            /* [out] */ Boolean* network);

    /**
     * True if the provider requires access to a
     * satellite-based positioning system (e.g., GPS), false
     * otherwise.
     */
    CARAPI SetRequiresSatellite(
            /* [in] */ Boolean satellite);

    CARAPI GetRequiresSatellite(
            /* [out] */ Boolean* satellite);
    /**
     * True if the provider requires access to an appropriate
     * cellular network (e.g., to make use of cell tower IDs), false
     * otherwise.
     */
    CARAPI SetRequiresCell(
            /* [in] */ Boolean cell);

    CARAPI GetRequiresCell(
            /* [out] */ Boolean* cell);
    /**
     * True if the use of this provider may result in a
     * monetary charge to the user, false if use is free.  It is up to
     * each provider to give accurate information. Cell (network) usage
     * is not considered monetary cost.
     */
    CARAPI SetHasMonetaryCost(
            /* [in] */ Boolean hasMonetaryCost);

    CARAPI GetHasMonetaryCost(
            /* [out] */ Boolean* hasMonetaryCost);

    /**
     * True if the provider is able to provide altitude
     * information, false otherwise.  A provider that reports altitude
     * under most circumstances but may occasionally not report it
     * should return true.
     */
    CARAPI SetSupportsAltitude(
            /* [in] */ Boolean supportsAltitude);

    CARAPI GetSupportsAltitude(
            /* [out] */ Boolean* supportsAltitude);
    /**
     * True if the provider is able to provide speed
     * information, false otherwise.  A provider that reports speed
     * under most circumstances but may occasionally not report it
     * should return true.
     */
    CARAPI SetSupportsSpeed(
            /* [in] */ Boolean supportsSpeed);

    CARAPI GetSupportsSpeed(
            /* [out] */ Boolean* supportsSpeed);

    /**
     * True if the provider is able to provide bearing
     * information, false otherwise.  A provider that reports bearing
     * under most circumstances but may occasionally not report it
     * should return true.
     */
    CARAPI SetSupportsBearing(
            /* [in] */ Boolean supportsBearing);

    CARAPI GetSupportsBearing(
            /* [out] */ Boolean* supportsBearing);

    /**
     * Power requirement for this provider.
     *
     * @return the power requirement for this provider, as one of the
     * constants Criteria.POWER_*.
     */

    CARAPI SetPowerRequirement(
            /* [in] */ Int32 powerRequirement);

    CARAPI GetPowerRequirement(
            /* [out] */ Int32* powerRequirement);

    /**
     * Constant describing the horizontal accuracy returned
     * by this provider.
     *
     * @return the horizontal accuracy for this provider, as one of the
     * constants Criteria.ACCURACY_COARSE or Criteria.ACCURACY_FINE
     */
    CARAPI SetAccuracy(
            /* [in] */ Int32 accuracy);

    CARAPI GetAccuracy(
            /* [out] */ Int32* accuracy);

public:
    /**
     * True if provider requires access to a
     * data network (e.g., the Internet), false otherwise.
     */
    Boolean mRequiresNetwork;

    /**
     * True if the provider requires access to a
     * satellite-based positioning system (e.g., GPS), false
     * otherwise.
     */
    Boolean mRequiresSatellite;

    /**
     * True if the provider requires access to an appropriate
     * cellular network (e.g., to make use of cell tower IDs), false
     * otherwise.
     */
    Boolean mRequiresCell;

    /**
     * True if the use of this provider may result in a
     * monetary charge to the user, false if use is free.  It is up to
     * each provider to give accurate information. Cell (network) usage
     * is not considered monetary cost.
     */
    Boolean mHasMonetaryCost;

    /**
     * True if the provider is able to provide altitude
     * information, false otherwise.  A provider that reports altitude
     * under most circumstances but may occasionally not report it
     * should return true.
     */
    Boolean mSupportsAltitude;

    /**
     * True if the provider is able to provide speed
     * information, false otherwise.  A provider that reports speed
     * under most circumstances but may occasionally not report it
     * should return true.
     */
    Boolean mSupportsSpeed;

    /**
     * True if the provider is able to provide bearing
     * information, false otherwise.  A provider that reports bearing
     * under most circumstances but may occasionally not report it
     * should return true.
     */
    Boolean mSupportsBearing;

    /**
     * Power requirement for this provider.
     *
     * @return the power requirement for this provider, as one of the
     * constants Criteria.POWER_*.
     */
    Int32 mPowerRequirement;

    /**
     * Constant describing the horizontal accuracy returned
     * by this provider.
     *
     * @return the horizontal accuracy for this provider, as one of the
     * constants Criteria.ACCURACY_COARSE or Criteria.ACCURACY_FINE
     */
    Int32 mAccuracy;
};

}//namespace Location
}//namespace Droid
}//namespace Elastos

#endif //__PROVIDERPROPERTIES_H__
