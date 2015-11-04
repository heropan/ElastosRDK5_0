
#include "elastos/droid/hardware/location/GeofenceHardwareRequestParcelable.h"
#include "elastos/droid/hardware/location/GeofenceHardwareRequest.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Location {

CAR_INTERFACE_IMPL_2(GeofenceHardwareRequestParcelable, Object, IGeofenceHardwareRequestParcelable, IParcelable)

GeofenceHardwareRequestParcelable::GeofenceHardwareRequestParcelable()
    : mId(0)
    , mRequest(NULL)
{
}

GeofenceHardwareRequestParcelable::~GeofenceHardwareRequestParcelable()
{
}

ECode GeofenceHardwareRequestParcelable::constructor()
{
    return NOERROR;
}

ECode GeofenceHardwareRequestParcelable::constructor(
    /* [in] */ Int32 id,
    /* [in] */ IGeofenceHardwareRequest* request)
{
    mId = id;
    mRequest = request;
    return NOERROR;
}

ECode GeofenceHardwareRequestParcelable::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);

    *id = mId;
    return NOERROR;
}

ECode GeofenceHardwareRequestParcelable::GetLatitude(
    /* [out] */ Double* latitude)
{
    VALIDATE_NOT_NULL(latitude);

    return mRequest->GetLatitude(latitude);
}

ECode GeofenceHardwareRequestParcelable::GetLongitude(
    /* [out] */ Double* longitude)
{
    VALIDATE_NOT_NULL(longitude);

    return mRequest->GetLongitude(longitude);
}

ECode GeofenceHardwareRequestParcelable::GetRadius(
    /* [out] */ Double* radius)
{
    VALIDATE_NOT_NULL(radius);

    return mRequest->GetRadius(radius);
}

ECode GeofenceHardwareRequestParcelable::GetMonitorTransitions(
    /* [out] */ Int32* transitions)
{
    VALIDATE_NOT_NULL(transitions);

    return mRequest->GetMonitorTransitions(transitions);
}

ECode GeofenceHardwareRequestParcelable::GetUnknownTimer(
    /* [out] */ Int32* timer)
{
    VALIDATE_NOT_NULL(timer);

    return mRequest->GetUnknownTimer(timer);
}

ECode GeofenceHardwareRequestParcelable::GetNotificationResponsiveness(
    /* [out] */ Int32* responsiveness)
{
    VALIDATE_NOT_NULL(responsiveness);

    return mRequest->GetNotificationResponsiveness(responsiveness);
}

ECode GeofenceHardwareRequestParcelable::GetLastTransition(
    /* [out] */ Int32* transition)
{
    VALIDATE_NOT_NULL(transition);

    return mRequest->GetLastTransition(transition);
}

ECode GeofenceHardwareRequestParcelable::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);

    return mRequest->GetType(type);
}

ECode GeofenceHardwareRequestParcelable::GetSourceTechnologies(
    /* [out] */ Int32* technologies)
{
    VALIDATE_NOT_NULL(technologies);

    return mRequest->GetSourceTechnologies(technologies);
}

ECode GeofenceHardwareRequestParcelable::DescribeContents(
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);

    *val = 0;
    return NOERROR;
}

ECode GeofenceHardwareRequestParcelable::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    Int32 type;
    FAIL_RETURN(GetType(&type))
    FAIL_RETURN(dest->WriteInt32(type))

    Double latitude;
    FAIL_RETURN(GetLatitude(&latitude))
    FAIL_RETURN(dest->WriteDouble(latitude))

    Double longitude;
    FAIL_RETURN(GetLongitude(&longitude))
    FAIL_RETURN(dest->WriteDouble(longitude))

    Double radius;
    FAIL_RETURN(GetRadius(&radius))
    FAIL_RETURN(dest->WriteDouble(radius))

    Int32 transition;
    FAIL_RETURN(GetLastTransition(&transition))
    FAIL_RETURN(dest->WriteInt32(transition))

    Int32 transitions;
    FAIL_RETURN(GetMonitorTransitions(&transitions))
    FAIL_RETURN(dest->WriteInt32(transitions))

    Int32 timer;
    FAIL_RETURN(GetUnknownTimer(&timer))
    FAIL_RETURN(dest->WriteInt32(timer))

    Int32 responsiveness;
    FAIL_RETURN(GetNotificationResponsiveness(&responsiveness))
    FAIL_RETURN(dest->WriteInt32(responsiveness))

    Int32 technologies;
    FAIL_RETURN(GetSourceTechnologies(&technologies))
    FAIL_RETURN(dest->WriteInt32(technologies))

    FAIL_RETURN(dest->WriteInt32(mId))

    return NOERROR;
}

ECode GeofenceHardwareRequestParcelable::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 type;
    FAIL_RETURN(source->ReadInt32(&type))
    ((GeofenceHardwareRequest*)mRequest.Get())->mType = type;

    Double latitude;
    FAIL_RETURN(source->ReadDouble(&latitude))
    ((GeofenceHardwareRequest*)mRequest.Get())->mLatitude = latitude;

    Double longitude;
    FAIL_RETURN(source->ReadDouble(&longitude))
    ((GeofenceHardwareRequest*)mRequest.Get())->mLongitude = longitude;

    Double radius;
    FAIL_RETURN(source->ReadDouble(&radius))
    ((GeofenceHardwareRequest*)mRequest.Get())->mRadius = radius;

    Int32 transition;
    FAIL_RETURN(source->ReadInt32(&transition))
    ((GeofenceHardwareRequest*)mRequest.Get())->mLastTransition = transition;

    Int32 transitions;
    FAIL_RETURN(source->ReadInt32(&transitions))
    ((GeofenceHardwareRequest*)mRequest.Get())->mMonitorTransitions = transitions;

    Int32 timer;
    FAIL_RETURN(source->ReadInt32(&timer))
    ((GeofenceHardwareRequest*)mRequest.Get())->mUnknownTimer = timer;

    Int32 responsiveness;
    FAIL_RETURN(source->ReadInt32(&responsiveness))
    ((GeofenceHardwareRequest*)mRequest.Get())->mNotificationResponsiveness = responsiveness;

    Int32 technologies;
    FAIL_RETURN(source->ReadInt32(&technologies))
    ((GeofenceHardwareRequest*)mRequest.Get())->mSourceTechnologies = technologies;

    FAIL_RETURN(source->ReadInt32(&mId))

    return NOERROR;
}

ECode GeofenceHardwareRequestParcelable::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuilder builder;
    builder += "id=";
    builder += mId;
    builder += ", type=";
    Int32 type;
    FAIL_RETURN(mRequest->GetType(&type))
    builder += type;
    builder += ", latitude=";
    Double latitude;
    FAIL_RETURN(mRequest->GetLatitude(&latitude))
    builder += latitude;
    builder += ", longitude=";
    Double longitude;
    FAIL_RETURN(mRequest->GetLongitude(&longitude))
    builder += longitude;
    builder += ", radius=";
    Double radius;
    FAIL_RETURN(mRequest->GetRadius(&radius))
    builder += radius;
    builder += ", lastTransition=";
    Int32 transition;
    FAIL_RETURN(mRequest->GetLastTransition(&transition))
    builder += transition;
    builder += ", unknownTimer=";
    Int32 timer;
    FAIL_RETURN(mRequest->GetUnknownTimer(&timer))
    builder += timer;
    builder += ", monitorTransitions=";
    Int32 transitions;
    FAIL_RETURN(mRequest->GetMonitorTransitions(&transitions))
    builder += transitions;
    builder += ", notificationResponsiveness=";
    Int32 responsiveness;
    FAIL_RETURN(mRequest->GetNotificationResponsiveness(&responsiveness))
    builder += responsiveness;
    builder += ", sourceTechnologies=";
    Int32 technologies;
    FAIL_RETURN(mRequest->GetSourceTechnologies(&technologies))
    builder += technologies;

    return builder.ToString(str);
}

} //Location
} //Hardware
} //Droid
} //Elastos