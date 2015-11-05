
#include "elastos/droid/location/LocationProvider.h"

namespace Elastos {
namespace Droid {
namespace Location {

CAR_INTERFACE_IMPL(LocationProvider, Object, ILocationProvider)

LocationProvider::LocationProvider()
{
}

ECode LocationProvider::constructor(
    /* [in] */ const String& name,
    /* [in] */ IProviderProperties* properties)
{
    Int32 length = name.GetLength();
    if (name.RegionMatches(0, ILocationProvider::BAD_CHARS_REGEX, 0, length)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    assert(properties != NULL);
    mName = name;
    mProperties = properties;
    return NOERROR;
}

ECode LocationProvider::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode LocationProvider::MeetsCriteria(
    /* [in] */ ICriteria* criteria,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(criteria);
    VALIDATE_NOT_NULL(result);
    *result = PropertiesMeetCriteria(mName, mProperties, criteria);
    return NOERROR;
}

Boolean LocationProvider::PropertiesMeetCriteria(
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

ECode LocationProvider::RequiresNetwork(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    mProperties->GetRequiresNetwork(result);
    return NOERROR;
}

ECode LocationProvider::RequiresSatellite(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    mProperties->GetRequiresSatellite(result);
    return NOERROR;
}

ECode LocationProvider::RequiresCell(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    mProperties->GetRequiresCell(result);
    return NOERROR;
}

ECode LocationProvider::HasMonetaryCost(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    mProperties->GetHasMonetaryCost(result);
    return NOERROR;
}

ECode LocationProvider::SupportsAltitude(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    mProperties->GetSupportsAltitude(result);
    return NOERROR;
}

ECode LocationProvider::SupportsSpeed(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    mProperties->GetSupportsSpeed(result);
    return NOERROR;
}

ECode LocationProvider::SupportsBearing(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    mProperties->GetSupportsBearing(result);
    return NOERROR;
}

ECode LocationProvider::GetPowerRequirement(
    /* [out] */ Int32* pr)
{
    VALIDATE_NOT_NULL(pr);
    mProperties->GetPowerRequirement(pr);
    return NOERROR;
}

ECode LocationProvider::GetAccuracy(
    /* [out] */ Int32* accuracy)
{
    VALIDATE_NOT_NULL(accuracy);
    mProperties->GetAccuracy(accuracy);
    return NOERROR;
}

} // namespace Location
} // namepsace Droid
} // namespace Elastos