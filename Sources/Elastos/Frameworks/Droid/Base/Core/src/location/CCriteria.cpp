
#include "location/CCriteria.h"

namespace Elastos {
namespace Droid {
namespace Location {

CCriteria::CCriteria()
    : mHorizontalAccuracy(Criteria_NO_REQUIREMENT)
    , mVerticalAccuracy(Criteria_NO_REQUIREMENT)
    , mSpeedAccuracy(Criteria_NO_REQUIREMENT)
    , mBearingAccuracy(Criteria_NO_REQUIREMENT)
    , mPowerRequirement(Criteria_NO_REQUIREMENT)
    , mAltitudeRequired(FALSE)
    , mBearingRequired(FALSE)
    , mSpeedRequired(FALSE)
    , mCostAllowed(FALSE)
{
}

ECode CCriteria::SetHorizontalAccuracy(
    /* [in] */ Int32 accuracy)
{
    if (accuracy < Criteria_NO_REQUIREMENT || accuracy > Criteria_ACCURACY_HIGH) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("accuracy=" + accuracy);
    }
    mHorizontalAccuracy = accuracy;

    return NOERROR;
}

ECode CCriteria::GetHorizontalAccuracy(
    /* [out] */ Int32* accuracy)
{
    VALIDATE_NOT_NULL(accuracy);
    *accuracy = mHorizontalAccuracy;

    return NOERROR;
}

ECode CCriteria::SetVerticalAccuracy(
    /* [in] */ Int32 accuracy)
{
    if (accuracy < Criteria_NO_REQUIREMENT || accuracy > Criteria_ACCURACY_HIGH) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("accuracy=" + accuracy);
    }
    mVerticalAccuracy = accuracy;

    return NOERROR;
}

ECode CCriteria::GetVerticalAccuracy(
    /* [out] */ Int32* accuracy)
{
    VALIDATE_NOT_NULL(accuracy);
    *accuracy = mVerticalAccuracy;

    return NOERROR;
}

ECode CCriteria::SetSpeedAccuracy(
    /* [in] */ Int32 accuracy)
{
    if (accuracy < Criteria_NO_REQUIREMENT || accuracy > Criteria_ACCURACY_HIGH) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("accuracy=" + accuracy);
    }
    mSpeedAccuracy = accuracy;

    return NOERROR;
}

ECode CCriteria::GetSpeedAccuracy(
    /* [out] */ Int32* accuracy)
{
    VALIDATE_NOT_NULL(accuracy);
    *accuracy = mSpeedAccuracy;

    return NOERROR;
}

ECode CCriteria::SetBearingAccuracy(
    /* [in] */ Int32 accuracy)
{
    if (accuracy < Criteria_NO_REQUIREMENT || accuracy > Criteria_ACCURACY_HIGH) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("accuracy=" + accuracy);
    }
    mBearingAccuracy = accuracy;

    return NOERROR;
}

ECode CCriteria::GetBearingAccuracy(
    /* [out] */ Int32* accuracy)
{
    VALIDATE_NOT_NULL(accuracy);
    *accuracy = mBearingAccuracy;

    return NOERROR;
}

ECode CCriteria::SetAccuracy(
    /* [in] */ Int32 accuracy)
{
    if (accuracy < Criteria_NO_REQUIREMENT || accuracy > Criteria_ACCURACY_COARSE) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("accuracy=" + accuracy);
    }
    if (accuracy == Criteria_ACCURACY_FINE) {
        mHorizontalAccuracy = Criteria_ACCURACY_HIGH;
    }
    else {
        mHorizontalAccuracy = Criteria_ACCURACY_LOW;
    }

    return NOERROR;
}

ECode CCriteria::GetAccuracy(
    /* [out] */ Int32* accuracy)
{
    VALIDATE_NOT_NULL(accuracy);

    if (mHorizontalAccuracy >= Criteria_ACCURACY_HIGH) {
        *accuracy = Criteria_ACCURACY_FINE;
    }
    else {
        *accuracy = Criteria_ACCURACY_COARSE;
    }

    return NOERROR;
}

ECode CCriteria::SetPowerRequirement(
    /* [in] */ Int32 level)
{
    if (level < Criteria_NO_REQUIREMENT || level > Criteria_POWER_HIGH) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("level=" + level);
    }
    mPowerRequirement = level;

    return NOERROR;
}

ECode CCriteria::GetPowerRequirement(
    /* [out] */ Int32* requirement)
{
    VALIDATE_NOT_NULL(requirement);
    *requirement = mPowerRequirement;

    return NOERROR;
}

ECode CCriteria::SetCostAllowed(
    /* [in] */ Boolean costAllowed)
{
    mCostAllowed = costAllowed;

    return NOERROR;
}

ECode CCriteria::IsCostAllowed(
    /* [out] */ Boolean* costAllowed)
{
    VALIDATE_NOT_NULL(costAllowed);
    *costAllowed = mCostAllowed;

    return NOERROR;
}

ECode CCriteria::SetAltitudeRequired(
    /* [in] */ Boolean altitudeRequired)
{
    mAltitudeRequired = altitudeRequired;

    return NOERROR;
}

ECode CCriteria::IsAltitudeRequired(
    /* [out] */ Boolean* altitudeRequired)
{
    VALIDATE_NOT_NULL(altitudeRequired);
    *altitudeRequired = mAltitudeRequired;

    return NOERROR;
}

ECode CCriteria::SetSpeedRequired(
    /* [in] */ Boolean speedRequired)
{
    mSpeedRequired = speedRequired;

    return NOERROR;
}

ECode CCriteria::IsSpeedRequired(
    /* [out] */ Boolean* speedRequired)
{
    VALIDATE_NOT_NULL(speedRequired);
    *speedRequired = mSpeedRequired;

    return NOERROR;
}

ECode CCriteria::SetBearingRequired(
    /* [in] */ Boolean bearingRequired)
{
    mBearingRequired = bearingRequired;

    return NOERROR;
}

ECode CCriteria::IsBearingRequired(
    /* [out] */ Boolean* bearingRequired)
{
    VALIDATE_NOT_NULL(bearingRequired);
    *bearingRequired = mBearingRequired;

    return NOERROR;
}

ECode CCriteria::GetDescription(
    /* [out] */ String* str)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCriteria::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    AutoPtr<CCriteria> c;
    ASSERT_SUCCEEDED(CCriteria::NewByFriend((CCriteria**)&c));
    source->ReadInt32(&(c->mHorizontalAccuracy));
    source->ReadInt32(&(c->mVerticalAccuracy));
    source->ReadInt32(&(c->mSpeedAccuracy));
    source->ReadInt32(&(c->mBearingAccuracy));
    source->ReadInt32(&(c->mPowerRequirement));

    Int32 value;
    source->ReadInt32(&value);
    c->mAltitudeRequired = value != 0;
    source->ReadInt32(&value);
    c->mBearingRequired = value != 0;
    source->ReadInt32(&value);
    c->mSpeedRequired = value != 0;
    source->ReadInt32(&value);
    c->mCostAllowed = value != 0;

    return NOERROR;
}

ECode CCriteria::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mHorizontalAccuracy);
    dest->WriteInt32(mVerticalAccuracy);
    dest->WriteInt32(mSpeedAccuracy);
    dest->WriteInt32(mBearingAccuracy);
    dest->WriteInt32(mPowerRequirement);
    dest->WriteInt32(mAltitudeRequired ? 1 : 0);
    dest->WriteInt32(mBearingRequired ? 1 : 0);
    dest->WriteInt32(mSpeedRequired ? 1 : 0);
    dest->WriteInt32(mCostAllowed ? 1 : 0);

    return NOERROR;
}

ECode CCriteria::constructor()
{
    return NOERROR;
}

ECode CCriteria::constructor(
    /* [in] */ ICriteria* criteria)
{
    AutoPtr<CCriteria> criteriaCls = (CCriteria*)criteria;

    mHorizontalAccuracy = criteriaCls->mHorizontalAccuracy;
    mVerticalAccuracy = criteriaCls->mVerticalAccuracy;
    mSpeedAccuracy = criteriaCls->mSpeedAccuracy;
    mBearingAccuracy = criteriaCls->mBearingAccuracy;
    mPowerRequirement = criteriaCls->mPowerRequirement;
    mAltitudeRequired = criteriaCls->mAltitudeRequired;
    mBearingRequired = criteriaCls->mBearingRequired;
    mSpeedRequired = criteriaCls->mSpeedRequired;
    mCostAllowed = criteriaCls->mCostAllowed;

    return NOERROR;
}

ECode CCriteria::PowerToString(
    /* [in] */ Int32 power,
    /* [out] */ String* strOut)
{
    VALIDATE_NOT_NULL(strOut);

    switch (power) {
        case Criteria_NO_REQUIREMENT:
            {
                *strOut = "NO_REQ";
                return NOERROR;
            }

        case Criteria_POWER_LOW:
            {
                *strOut = "LOW";
                return NOERROR;
            }

        case Criteria_POWER_MEDIUM:
            {
                *strOut = "MEDIUM";
                return NOERROR;
            }

        case Criteria_POWER_HIGH:
            {
                *strOut = "HIGH";
                return NOERROR;
            }

        default:
            {
                *strOut = "???";
                return NOERROR;
            }
    }
}

ECode CCriteria::AccuracyToString(
    /* [in] */ Int32 accuracy,
    /* [out] */ String* strOut)
{
    VALIDATE_NOT_NULL(strOut);

    switch (accuracy) {
        case Criteria_NO_REQUIREMENT:
            {
                *strOut = "---";
                return NOERROR;
            }

        case Criteria_ACCURACY_HIGH:
            {
                *strOut = "HIGH";
                return NOERROR;
            }

        case Criteria_ACCURACY_MEDIUM:
            {
                *strOut = "MEDIUM";
                return NOERROR;
            }

        case Criteria_ACCURACY_LOW:
            {
                *strOut = "LOW";
                return NOERROR;
            }

        default:
            {
                *strOut = "???";
                return NOERROR;
            }
    }
}

}//namespace Location
}//namespace Droid
}//namespace Elastos
