
#ifndef __ELASTOS_DROID_HARDWARE_CSENSOREVENT_H__
#define  __ELASTOS_DROID_HARDWARE_CSENSOREVENT_H__

#include "_Elastos_Droid_Hardware_CSensorEvent.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Hardware {

CarClass(CSensorEvent)
    , public Object
    , public ISensorEvent
{
public:
    CSensorEvent();

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI SetValues(
        /* [in] */ ArrayOf<Float>* values);

    CARAPI GetValues(
        /* [out, callee] */ ArrayOf<Float>** values);

    CARAPI GetSensor(
        /* [out] */ ISensor** sensor);

    CARAPI GetAccuracy(
        /* [out] */ Int32* accuracy);

    CARAPI GetTimestamp(
        /* [out] */ Int64* timestamp);

    CARAPI SetOriginalValue(
        /* [in] */ ArrayOf<Float>* values);

    CARAPI GetOriginalValue(
        /* [out, callee] */ ArrayOf<Float>** values);

    CARAPI constructor(
        /* [in] */ Int32 size);

public:
    AutoPtr<ArrayOf<Float> > mValues;

    AutoPtr<ArrayOf<Float> > mOriginalValues;

    /**
     * The sensor that generated this event. See
     * {@link android.hardware.SensorManager SensorManager} for details.
     */
    AutoPtr<ISensor> mSensor;

    /**
     * The accuracy of this event. See {@link android.hardware.SensorManager
     * SensorManager} for details.
     */
    Int32 mAccuracy;


    /**
     * The time in nanosecond at which the event happened
     */
    Int64 mTimestamp;
};

} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_HARDWARE_CSENSOREVENT_H__
