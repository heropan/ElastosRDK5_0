
#include "location/CGeofence.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Location {

AutoPtr<IGeofence> CGeofence::CreateCircle(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Float radius)
{
    AutoPtr<IGeofence> tmp;
    CGeofence::New(latitude, longitude, radius, (IGeofence**)&tmp);
    return tmp;
}

ECode CGeofence::constructor()
{
    return NOERROR;
}

ECode CGeofence::constructor(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Float radius)
{
    CheckRadius(radius);
    CheckLatLong(latitude, longitude);
    mType = IGeofence::TYPE_HORIZONTAL_CIRCLE;
    mLatitude = latitude;
    mLongitude = longitude;
    mRadius = radius;

    return NOERROR;
}

/** @hide */
ECode CGeofence::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;

    return NOERROR;
}

/** @hide */
ECode CGeofence::GetLatitude(
    /* [out] */ Double* latitude)
{
    VALIDATE_NOT_NULL(latitude);
    *latitude = mLatitude;

    return NOERROR;
}

/** @hide */
ECode CGeofence::GetLongitude(
    /* [out] */ Double* longitude)
{
    VALIDATE_NOT_NULL(longitude);
    *longitude = mLongitude;

    return NOERROR;
}

/** @hide */
ECode CGeofence::GetRadius(
    /* [out] */ Float* radius)
{
    VALIDATE_NOT_NULL(radius);
    *radius = mRadius;

    return NOERROR;
}

//@Override
ECode CGeofence::DescribeContents(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;
    return NOERROR;
}

ECode CGeofence::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    Int32 type;
    Double latitude;
    Double longitude;
    Float radius;

    in->ReadInt32(&type);
    in->ReadDouble(&latitude);
    in->ReadDouble(&longitude);
    in->ReadFloat(&radius);

    CheckType(type);

    CheckRadius(radius);
    CheckLatLong(latitude, longitude);
    mType = IGeofence::TYPE_HORIZONTAL_CIRCLE;
    mLatitude = latitude;
    mLongitude = longitude;
    mRadius = radius;

    return NOERROR;
}

//@Override
ECode CGeofence::WriteToParcel(
    /* [in] */ IParcel* parcel)
{
    VALIDATE_NOT_NULL(parcel);
    parcel->WriteInt32(mType);
    parcel->WriteDouble(mLatitude);
    parcel->WriteDouble(mLongitude);
    parcel->WriteFloat(mRadius);

    return NOERROR;
}

ECode CGeofence::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst);

    if(o == NULL) {
        *rst = FALSE;
        return NOERROR;
    }
    if(o->Probe(EIID_IGeofence) == NULL) {
        *rst = FALSE;
        return NOERROR;
    }
    IGeofence* obj = (IGeofence*)(o->Probe(EIID_IGeofence));
    CGeofence* other = (CGeofence*)obj;
    if (this == other) {
        *rst = TRUE;
        return NOERROR;
    }

    if (mRadius != other->mRadius) {
        *rst = FALSE;
        return NOERROR;
    }
    if (mLatitude != other->mLatitude) {
        *rst = FALSE;
        return NOERROR;
    }
    if (mLongitude != other->mLongitude) {
        *rst = FALSE;
        return NOERROR;
    }
    if (mType != other->mType) {
        *rst = FALSE;
        return NOERROR;
    }
    *rst = TRUE;
    return NOERROR;
}

ECode CGeofence::GetHashCode(
    /* [out] */ Int32* hash)
{
    using Elastos::Core::Math;

    VALIDATE_NOT_NULL(hash);
    Int32 prime = 31;
    Int32 result = 1;
    Int64 temp;
    temp = Math::DoubleToInt64Bits(mLatitude);
    result = prime * result + (Int32) (temp ^ (temp >> 32));
    temp = Math::DoubleToInt64Bits(mLongitude);
    result = prime * result + (Int32) (temp ^ (temp >> 32));
    result = prime * result + Math::FloatToInt32Bits(mRadius);
    result = prime * result + mType;
    *hash = result;
    return NOERROR;
}

//@Override
ECode CGeofence::ToString(
    /* [in] */ String* strOut)
{
    VALIDATE_NOT_NULL(strOut);
    StringBuilder sbc("Geofence[");
    sbc.AppendString(TypeToString(mType));
    sbc.AppendCStr(" ");
    sbc.AppendFloat(mLatitude);
    sbc.AppendCStr(", ");
    sbc.AppendFloat(mLongitude);
    sbc.AppendCStr(" ");
    sbc.AppendFloat(mRadius);
    sbc.AppendCStr("m]");
    sbc.ToString(strOut);

    //return String.format("Geofence[%s %.6f, %.6f %.0fm]",
    //            TypeToString(mType), mLatitude, mLongitude, mRadius);

    return NOERROR;
}

void CGeofence::CheckRadius(
    /* [in] */ Float radius)
{
    if (radius <= 0) {
    //    throw new IllegalArgumentException("invalid radius: " + radius);
        assert(0);
    }
}

void CGeofence::CheckLatLong(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude)
{
    if (latitude > 90.0 || latitude < -90.0) {
    //    throw new IllegalArgumentException("invalid latitude: " + latitude);
        assert(0);
    }

    if (longitude > 180.0 || longitude < -180.0) {
    //    throw new IllegalArgumentException("invalid longitude: " + longitude);
        assert(0);
    }
}

void CGeofence::CheckType(
    /* [in] */ Int32 type)
{
    if (type != TYPE_HORIZONTAL_CIRCLE) {
    //    throw new IllegalArgumentException("invalid type: " + type);
        assert(0);
    }
}

String CGeofence::TypeToString(
    /* [in] */ Int32 type)
{
    String str;
    switch (type) {
        case IGeofence::TYPE_HORIZONTAL_CIRCLE:
            {
                str = "CIRCLE";
                return str;
            }

        default:
            CheckType(type);
            str = "";
            return str;
    }
}

}//namespace Location
}//namespace Droid
}//namespace Elastos
