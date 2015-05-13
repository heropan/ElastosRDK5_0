
#include "location/CProviderProperties.h"

namespace Elastos {
namespace Droid {
namespace Location {

CProviderProperties::CProviderProperties()
     : mRequiresNetwork(FALSE)
     , mRequiresSatellite(FALSE)
     , mRequiresCell(FALSE)
     , mHasMonetaryCost(FALSE)
     , mSupportsAltitude(FALSE)
     , mSupportsSpeed(FALSE)
     , mSupportsBearing(FALSE)
     , mPowerRequirement(0)
     , mAccuracy(0)
{}

ECode CProviderProperties::constructor()
{
    return NOERROR;
}

ECode CProviderProperties::constructor(
    /* [in] */ Boolean mRequiresNetwork,
    /* [in] */ Boolean mRequiresSatellite,
    /* [in] */ Boolean mRequiresCell,
    /* [in] */ Boolean mHasMonetaryCost,
    /* [in] */ Boolean mSupportsAltitude,
    /* [in] */ Boolean mSupportsSpeed,
    /* [in] */ Boolean mSupportsBearing,
    /* [in] */ Int32 mPowerRequirement,
    /* [in] */ Int32 mAccuracy)
{
    this->mRequiresNetwork = mRequiresNetwork;
    this->mRequiresSatellite = mRequiresSatellite;
    this->mRequiresCell = mRequiresCell;
    this->mHasMonetaryCost = mHasMonetaryCost;
    this->mSupportsAltitude = mSupportsAltitude;
    this->mSupportsSpeed = mSupportsSpeed;
    this->mSupportsBearing = mSupportsBearing;
    this->mPowerRequirement = mPowerRequirement;
    this->mAccuracy = mAccuracy;

    return NOERROR;
}

//@Override
ECode CProviderProperties::DescribeContents(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;
    return NOERROR;
}

//@Override
ECode CProviderProperties::WriteToParcel(
    /* [in] */ IParcel* parcel)
{
    VALIDATE_NOT_NULL(parcel);
    parcel->WriteBoolean(mRequiresNetwork);
    parcel->WriteBoolean(mRequiresSatellite);
    parcel->WriteBoolean(mRequiresCell);
    parcel->WriteBoolean(mHasMonetaryCost);
    parcel->WriteBoolean(mSupportsAltitude);
    parcel->WriteBoolean(mSupportsSpeed);
    parcel->WriteBoolean(mSupportsBearing);
    parcel->WriteInt32(mPowerRequirement);
    parcel->WriteInt32(mAccuracy);

    return NOERROR;
}

ECode CProviderProperties::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    VALIDATE_NOT_NULL(parcel);

     parcel->ReadBoolean(&mRequiresNetwork);
     parcel->ReadBoolean(&mRequiresSatellite);
     parcel->ReadBoolean(&mRequiresCell);
     parcel->ReadBoolean(&mHasMonetaryCost);
     parcel->ReadBoolean(&mSupportsAltitude);
     parcel->ReadBoolean(&mSupportsSpeed);
     parcel->ReadBoolean(&mSupportsBearing);
     parcel->ReadInt32(&mPowerRequirement);
     parcel->ReadInt32(&mAccuracy);

    return NOERROR;
}

    /**
     * True if provider requires access to a
     * data network (e.g., the Internet), false otherwise.
     */
ECode CProviderProperties::SetRequiresNetwork(
        /* [in] */ Boolean network)
{
    mRequiresNetwork = network;
    return NOERROR;
}

ECode CProviderProperties::GetRequiresNetwork(
        /* [out] */ Boolean* network)
{
    *network = mRequiresNetwork;
    return NOERROR;
}

    /**
     * True if the provider requires access to a
     * satellite-based positioning system (e.g., GPS), false
     * otherwise.
     */
ECode CProviderProperties::SetRequiresSatellite(
        /* [in] */ Boolean satellite)
{
    mRequiresSatellite = satellite;
    return NOERROR;
}

ECode CProviderProperties::GetRequiresSatellite(
        /* [out] */ Boolean* satellite)
{
    *satellite = mRequiresSatellite;
    return NOERROR;
}
    /**
     * True if the provider requires access to an appropriate
     * cellular network (e.g., to make use of cell tower IDs), false
     * otherwise.
     */
ECode CProviderProperties::SetRequiresCell(
        /* [in] */ Boolean cell)
{
    mRequiresCell = cell;
    return NOERROR;
}

ECode CProviderProperties::GetRequiresCell(
        /* [out] */ Boolean* cell)
{
    *cell = mRequiresCell;
    return NOERROR;
}
    /**
     * True if the use of this provider may result in a
     * monetary charge to the user, false if use is free.  It is up to
     * each provider to give accurate information. Cell (network) usage
     * is not considered monetary cost.
     */
ECode CProviderProperties::SetHasMonetaryCost(
        /* [in] */ Boolean hasMonetaryCost)
{
    mHasMonetaryCost = hasMonetaryCost;
    return NOERROR;
}

ECode CProviderProperties::GetHasMonetaryCost(
        /* [out] */ Boolean* hasMonetaryCost)
{
    *hasMonetaryCost = mHasMonetaryCost;
    return NOERROR;
}

    /**
     * True if the provider is able to provide altitude
     * information, false otherwise.  A provider that reports altitude
     * under most circumstances but may occasionally not report it
     * should return true.
     */
ECode CProviderProperties::SetSupportsAltitude(
        /* [in] */ Boolean supportsAltitude)
{
    mSupportsAltitude = supportsAltitude;
    return NOERROR;
}

ECode CProviderProperties::GetSupportsAltitude(
        /* [out] */ Boolean* supportsAltitude)
{
    *supportsAltitude = mSupportsAltitude;
    return NOERROR;
}
    /**
     * True if the provider is able to provide speed
     * information, false otherwise.  A provider that reports speed
     * under most circumstances but may occasionally not report it
     * should return true.
     */
ECode CProviderProperties::SetSupportsSpeed(
        /* [in] */ Boolean supportsSpeed)
{
    mSupportsSpeed = supportsSpeed;
    return NOERROR;
}

ECode CProviderProperties::GetSupportsSpeed(
        /* [out] */ Boolean* supportsSpeed)
{
    *supportsSpeed = mSupportsSpeed;
    return NOERROR;
}

    /**
     * True if the provider is able to provide bearing
     * information, false otherwise.  A provider that reports bearing
     * under most circumstances but may occasionally not report it
     * should return true.
     */
ECode CProviderProperties::SetSupportsBearing(
        /* [in] */ Boolean supportsBearing)
{
    mSupportsBearing = supportsBearing;
    return NOERROR;
}

ECode CProviderProperties::GetSupportsBearing(
        /* [out] */ Boolean* supportsBearing)
{
    *supportsBearing = mSupportsBearing;
    return NOERROR;
}

    /**
     * Power requirement for this provider.
     *
     * @return the power requirement for this provider, as one of the
     * constants Criteria.POWER_*.
     */

ECode CProviderProperties::SetPowerRequirement(
        /* [in] */ Int32 powerRequirement)
{
    mPowerRequirement = powerRequirement;
    return NOERROR;
}

ECode CProviderProperties::GetPowerRequirement(
        /* [out] */ Int32* powerRequirement)
{
    *powerRequirement = mPowerRequirement;
    return NOERROR;
}

    /**
     * Constant describing the horizontal accuracy returned
     * by this provider.
     *
     * @return the horizontal accuracy for this provider, as one of the
     * constants Criteria.ACCURACY_COARSE or Criteria.ACCURACY_FINE
     */
ECode CProviderProperties::SetAccuracy(
        /* [in] */ Int32 accuracy)
{
    return NOERROR;
}

ECode CProviderProperties::GetAccuracy(
        /* [out] */ Int32* accuracy)
{
    return NOERROR;
}

}//namespace Location
}//namespace Droid
}//namespace Elastos
