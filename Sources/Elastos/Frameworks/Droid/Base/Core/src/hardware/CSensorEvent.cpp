
#include "hardware/CSensorEvent.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Hardware {

CSensorEvent::CSensorEvent()
    : mAccuracy(0)
    , mTimestamp(0)
{
}

ECode CSensorEvent::SetValues(
    /* [in] */ ArrayOf<Float>* values)
{
    mValues = values;
    return NOERROR;
}

ECode CSensorEvent::GetValues(
    /* [out, callee] */ ArrayOf<Float>** values)
{
    VALIDATE_NOT_NULL(values);
    *values = mValues;
    REFCOUNT_ADD(*values);
    return NOERROR;
}

ECode CSensorEvent::SetOriginalValue(
    /* [in] */ ArrayOf<Float>* values)
{
    mOriginalValues = values;
    return NOERROR;
}

ECode CSensorEvent::GetOriginalValue(
    /* [out, callee] */ ArrayOf<Float>** values)
{
    VALIDATE_NOT_NULL(values);
    *values = mOriginalValues;
    REFCOUNT_ADD(*values);
    return NOERROR;
}

ECode CSensorEvent::GetSensor(
    /* [out] */ ISensor** sensor)
{
    VALIDATE_NOT_NULL(sensor);
    *sensor = mSensor;
    REFCOUNT_ADD(*sensor);
    return NOERROR;
}

ECode CSensorEvent::GetAccuracy(
    /* [out] */ Int32* accuracy)
{
    VALIDATE_NOT_NULL(accuracy);
    *accuracy = mAccuracy;
    return NOERROR;
}

ECode CSensorEvent::GetTimestamp(
    /* [out] */ Int64* timestamp)
{
    VALIDATE_NOT_NULL(timestamp);
    *timestamp = mTimestamp;
    return NOERROR;
}

ECode CSensorEvent::constructor(
    /* [in] */ Int32 size)
{
    mValues = ArrayOf<Float>::Alloc(size);
    mOriginalValues = ArrayOf<Float>::Alloc(size);
    return NOERROR;
}


} // namespace Hardware
} // namespace Droid
} // namespace Elastos
