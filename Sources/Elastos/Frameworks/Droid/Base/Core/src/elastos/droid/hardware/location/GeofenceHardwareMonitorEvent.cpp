
#include "elastos/droid/hardware/location/GeofenceHardwareMonitorEvent.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Location {

CAR_INTERFACE_IMPL_2(GeofenceHardwareMonitorEvent, Object, IGeofenceHardwareMonitorEvent, IParcelable)

GeofenceHardwareMonitorEvent::GeofenceHardwareMonitorEvent()
    : mMonitoringType(0)
    , mMonitoringStatus(0)
    , mSourceTechnologies(0)
    , mLocation(NULL)
{
}

GeofenceHardwareMonitorEvent::~GeofenceHardwareMonitorEvent()
{
}

ECode GeofenceHardwareMonitorEvent::constructor()
{
    return NOERROR;
}

ECode GeofenceHardwareMonitorEvent::constructor(
    /* [in] */ Int32 monitoringType,
    /* [in] */ Int32 monitoringStatus,
    /* [in] */ Int32 sourceTechnologies,
    /* [in] */ ILocation* location)
{
    mMonitoringType = monitoringType;
    mMonitoringStatus = monitoringStatus;
    mSourceTechnologies = sourceTechnologies;
    mLocation = location;
    return NOERROR;
}

ECode GeofenceHardwareMonitorEvent::GetMonitoringType(
        /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);

    *type = mMonitoringType;
    return NOERROR;
}

ECode GeofenceHardwareMonitorEvent::GetMonitoringStatus(
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);

    *status = mMonitoringStatus;
    return NOERROR;
}

ECode GeofenceHardwareMonitorEvent::GetSourceTechnologies(
    /* [out] */ Int32* technologies)
{
    VALIDATE_NOT_NULL(technologies);

    *technologies = mSourceTechnologies;
    return NOERROR;
}

ECode GeofenceHardwareMonitorEvent::GetLocation(
    /* [out] */ ILocation** location)
{
    VALIDATE_NOT_NULL(location);

    *location = mLocation;
    REFCOUNT_ADD(*location);
    return NOERROR;
}

ECode GeofenceHardwareMonitorEvent::DescribeContents(
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);

    *val = 0;
    return NOERROR;
}

ECode GeofenceHardwareMonitorEvent::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteInt32(mMonitoringType))
    FAIL_RETURN(dest->WriteInt32(mMonitoringStatus))
    FAIL_RETURN(dest->WriteInt32(mSourceTechnologies))
    FAIL_RETURN(dest->WriteInterfacePtr(mLocation))

    return NOERROR;
}

ECode GeofenceHardwareMonitorEvent::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(source->ReadInt32(&mMonitoringType))
    FAIL_RETURN(source->ReadInt32(&mMonitoringStatus))
    FAIL_RETURN(source->ReadInt32(&mSourceTechnologies))
    FAIL_RETURN(source->ReadInterfacePtr((Handle32*)(ILocation**)&mLocation))

    return NOERROR;
}

ECode GeofenceHardwareMonitorEvent::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuilder builder;
    builder += "GeofenceHardwareMonitorEvent: type=";
    builder += mMonitoringType;
    builder += ", status=";
    builder += mMonitoringStatus;
    builder += ", sources=";
    builder += mSourceTechnologies;
    builder += ", location=";
    builder += Object::ToString(mLocation);

    return builder.ToString(str);
}

} //Location
} //Hardware
} //Droid
} //Elastos