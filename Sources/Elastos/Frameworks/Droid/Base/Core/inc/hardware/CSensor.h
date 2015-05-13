
#ifndef  __CSENSOR_H__
#define  __CSENSOR_H__

#include "_CSensor.h"

namespace Elastos {
namespace Droid {
namespace Hardware {

CarClass(CSensor)
{
public:
    CSensor();

    CARAPI constructor();

    /**
     * @return name string of the sensor.
     */
    CARAPI GetName(
        /* [out] */ String* name);

    /**
     * @return vendor string of this sensor.
     */
    CARAPI GetVendor(
        /* [out] */ String* vendor);

    /**
     * @return generic type of this sensor.
     */
    CARAPI GetType(
        /* [out] */ Int32* type);

    /**
     * @return version of the sensor's module.
     */
    CARAPI GetVersion(
        /* [out] */ Int32* version);

    /**
     * @return maximum range of the sensor in the sensor's unit.
     */
    CARAPI GetMaximumRange(
        /* [out] */ Float* range);

    /**
     * @return resolution of the sensor in the sensor's unit.
     */
    CARAPI GetResolution(
        /* [out] */ Float* resolution);

    /**
     * @return the power in mA used by this sensor while in use
     */
    CARAPI GetPower(
        /* [out] */ Float* power);

    /**
     * @return the minimum delay allowed between two events in microsecond
     * or zero if this sensor only returns a value when the data it's measuring
     * changes.
     */
    CARAPI GetMinDelay(
        /* [out] */ Int32* delay);

    CARAPI_(Int32) GetHandle();

private:
    CARAPI_(void) SetRange(
        /* [in] */ Float max,
        /* [in] */ Float res);

    // @Override
    // public String toString() {
    //     return "{Sensor name=\"" + mName + "\", vendor=\"" + mVendor + "\", version=" + mVersion
    //             + ", type=" + mType + ", maxRange=" + mMaxRange + ", resolution=" + mResolution
    //             + ", power=" + mPower + ", minDelay=" + mMinDelay + "}";
    // }
private:
    /* Some of these fields are set only by the native bindings in
     * SensorManager.
     */
    String  mName;
    String  mVendor;
    Int32   mVersion;
    Int32   mHandle;
    Int32   mType;
    Float   mMaxRange;
    Float   mResolution;
    Float   mPower;
    Int32   mMinDelay;

friend class CSensorManagerHelper;
friend class CSystemSensorManager;
};


} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif  //__CSENSOR_H__
