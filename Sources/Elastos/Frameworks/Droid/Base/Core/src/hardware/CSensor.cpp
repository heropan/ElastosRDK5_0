
#include "hardware/CSensor.h"

namespace Elastos {
namespace Droid {
namespace Hardware {


CSensor::CSensor()
    : mVersion(0)
    , mHandle(0)
    , mType(0)
    , mMaxRange(0)
    , mResolution(0)
    , mPower(0)
    , mMinDelay(0)
{
}

ECode CSensor::constructor()
{
    return NOERROR;
}

ECode CSensor::GetName(
    /* [out] */ String* name)
{
    assert(name != NULL);
    *name = mName;
    return NOERROR;
}

ECode CSensor::GetVendor(
    /* [out] */ String* vendor)
{
    assert(vendor != NULL);
    *vendor = mVendor;
    return NOERROR;
}

ECode CSensor::GetType(
    /* [out] */ Int32* type)
{
    assert(type != NULL);
    *type = mType;
    return NOERROR;
}

ECode CSensor::GetVersion(
    /* [out] */ Int32* version)
{
    assert(version != NULL);
    *version = mVersion;
    return NOERROR;
}

ECode CSensor::GetMaximumRange(
    /* [out] */ Float* range)
{
    assert(range != NULL);
    *range = mMaxRange;
    return NOERROR;
}

ECode CSensor::GetResolution(
    /* [out] */ Float* resolution)
{
    assert(resolution != NULL);
    *resolution = mResolution;
    return NOERROR;
}

ECode CSensor::GetPower(
    /* [out] */ Float* power)
{
    assert(power != NULL);
    *power = mPower;
    return NOERROR;
}

ECode CSensor::GetMinDelay(
    /* [out] */ Int32* delay)
{
    assert(delay != NULL);
    *delay = mMinDelay;
    return NOERROR;
}

Int32 CSensor::GetHandle()
{
    return mHandle;
}

void CSensor::SetRange(
    /* [in] */ Float max,
    /* [in] */ Float res)
{
    mMaxRange = max;
    mResolution = res;
}


} // namespace Hardware
} // namespace Droid
} // namespace Elastos
