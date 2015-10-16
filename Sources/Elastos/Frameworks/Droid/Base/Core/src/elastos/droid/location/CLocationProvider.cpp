
#include "elastos/droid/location/CLocationProvider.h"
#include "elastos/droid/location/CProviderProperties.h"

namespace Elastos {
namespace Droid {
namespace Location {

/**
 * Constructs a LocationProvider with the given name.   Provider names must
 * consist only of the characters [a-zA-Z0-9].
 *
 * @throws IllegalArgumentException if name contains an illegal character
 *
 * @hide
 */
ECode CLocationProvider::constructor(
    /* [in] */ const String& name,
    /* [in] */ IProviderProperties* properties)
{
//    if (name.matches(BAD_CHARS_REGEX)) {
//        throw new IllegalArgumentException("provider name contains illegal character: " + name);
//    }

    assert(properties != NULL);
    mName = name;
    mProperties = properties;

    return NOERROR;
}

/**
 * Returns the name of this provider.
 */
ECode CLocationProvider::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

/**
 * Returns true if this provider meets the given criteria,
 * false otherwise.
 */
ECode CLocationProvider::MeetsCriteria(
    /* [in] */ ICriteria* criteria,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(criteria);
    VALIDATE_NOT_NULL(result);

    *result = PropertiesMeetCriteria(mName, mProperties, criteria);

    return NOERROR;
}

/**
 * @hide
 */
Boolean CLocationProvider::PropertiesMeetCriteria(
    /* [in] */ const String& name,
    /* [in] */ IProviderProperties* properties,
    /* [in] */ ICriteria* criteria)
{
    Int32 value, value2;
    Boolean bValue, bValue2;
    if (ILocationManager::PASSIVE_PROVIDER.Equals(name)) {
        // passive provider never matches
        return FALSE;
    }
    if (properties == NULL) {
        // unfortunately this can happen for provider in remote services
        // that have not finished binding yet
        return FALSE;
    }
    criteria->GetAccuracy(&value);
    properties->GetAccuracy(&value2);
    if (value != ICriteria::Criteria_NO_REQUIREMENT && value < value2) {
        return FALSE;
    }
    criteria->GetPowerRequirement(&value);
    properties->GetPowerRequirement(&value2);
    if (value != ICriteria::Criteria_NO_REQUIREMENT && value < value2) {
        return FALSE;
    }

    criteria->IsAltitudeRequired(&bValue);
    properties->GetSupportsAltitude(&bValue2);
    if (bValue && !bValue2) {
        return FALSE;
    }

    criteria->IsSpeedRequired(&bValue);
    properties->GetSupportsSpeed(&bValue2);
    if (bValue && !bValue2) {
        return FALSE;
    }
    criteria->IsBearingRequired(&bValue);
    properties->GetSupportsBearing(&bValue2);
    if (bValue && !bValue2) {
        return FALSE;
    }
    criteria->IsCostAllowed(&bValue);
    properties->GetHasMonetaryCost(&bValue2);
    if (bValue && !bValue2) {
        return FALSE;
    }
    return TRUE;
}

/**
 * Returns true if the provider requires access to a
 * data network (e.g., the Internet), false otherwise.
 */
ECode CLocationProvider::RequiresNetwork(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ((CProviderProperties*)(mProperties.Get()))->mRequiresNetwork;
    return NOERROR;
}

/**
 * Returns true if the provider requires access to a
 * satellite-based positioning system (e.g., GPS), false
 * otherwise.
 */
ECode CLocationProvider::RequiresSatellite(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ((CProviderProperties*)(mProperties.Get()))->mRequiresSatellite;
    return NOERROR;
}

/**
 * Returns true if the provider requires access to an appropriate
 * cellular network (e.g., to make use of cell tower IDs), false
 * otherwise.
 */
ECode CLocationProvider::RequiresCell(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ((CProviderProperties*)(mProperties.Get()))->mRequiresCell;
    return NOERROR;
}

/**
 * Returns true if the use of this provider may result in a
 * monetary charge to the user, false if use is free.  It is up to
 * each provider to give accurate information.
 */
ECode CLocationProvider::HasMonetaryCost(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ((CProviderProperties*)(mProperties.Get()))->mHasMonetaryCost;
    return NOERROR;
}

/**
 * Returns true if the provider is able to provide altitude
 * information, false otherwise.  A provider that reports altitude
 * under most circumstances but may occassionally not report it
 * should return true.
 */
ECode CLocationProvider::SupportsAltitude(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ((CProviderProperties*)(mProperties.Get()))->mSupportsAltitude;
    return NOERROR;
}

/**
 * Returns true if the provider is able to provide speed
 * information, false otherwise.  A provider that reports speed
 * under most circumstances but may occassionally not report it
 * should return true.
 */
ECode CLocationProvider::SupportsSpeed(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ((CProviderProperties*)(mProperties.Get()))->mSupportsSpeed;
    return NOERROR;
}

/**
 * Returns true if the provider is able to provide bearing
 * information, false otherwise.  A provider that reports bearing
 * under most circumstances but may occassionally not report it
 * should return true.
 */
ECode CLocationProvider::SupportsBearing(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ((CProviderProperties*)(mProperties.Get()))->mSupportsBearing;
    return NOERROR;
}

/**
 * Returns the power requirement for this provider.
 *
 * @return the power requirement for this provider, as one of the
 * constants Criteria.POWER_REQUIREMENT_*.
 */
ECode CLocationProvider::GetPowerRequirement(
    /* [out] */ Int32* pr)
{
    VALIDATE_NOT_NULL(pr);
    *pr = ((CProviderProperties*)(mProperties.Get()))->mPowerRequirement;
    return NOERROR;
}

/**
 * Returns a constant describing horizontal accuracy of this provider.
 * If the provider returns finer grain or exact location,
 * {@link Criteria#ACCURACY_FINE} is returned, otherwise if the
 * location is only approximate then {@link Criteria#ACCURACY_COARSE}
 * is returned.
 */
ECode CLocationProvider::GetAccuracy(
    /* [out] */ Int32* accuracy)
{
    VALIDATE_NOT_NULL(accuracy);
    *accuracy = ((CProviderProperties*)(mProperties.Get()))->mAccuracy;
    return NOERROR;
}

} // namespace Location
} // namepsace Droid
} // namespace Elastos