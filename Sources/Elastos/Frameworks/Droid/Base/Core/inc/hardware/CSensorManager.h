
#ifndef  __CSENSORMANAGER_H__
#define  __CSENSORMANAGER_H__

#include "_CSensorManager.h"
#include "hardware/LegacySensorManager.h"
#include "hardware/SensorManager.h"


namespace Elastos {
namespace Droid {
namespace Hardware {


CarClass(CSensorManager), public SensorManager
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetSensors(
        /* [out] */ Int32* sensors);

    CARAPI GetSensorList(
        /* [in] */ Int32 type,
        /* [out, callee] */ ArrayOf<ISensor*>** sensors);

    CARAPI GetDefaultSensor(
        /* [in] */ Int32 type,
        /* [out] */ ISensor** sensor);

    CARAPI RegisterListener(
        /* [in] */ ISensorListener* listener,
        /* [in] */ Int32 sensors,
        /* [out] */ Boolean* supported);

    CARAPI RegisterListenerEx(
        /* [in] */ ISensorListener* listener,
        /* [in] */ Int32 sensors,
        /* [in] */ Int32 rate,
        /* [out] */ Boolean* supported);

    CARAPI UnregisterListener(
        /* [in] */ ISensorListener* listener);

    CARAPI UnregisterListenerEx(
        /* [in] */ ISensorListener* listener,
        /* [in] */ Int32 sensors);

    CARAPI UnregisterListenerEx2(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor);

    CARAPI UnregisterListenerEx3(
        /* [in] */ ISensorEventListener* listener);

    CARAPI RegisterListenerEx2(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 rate,
        /* [out] */ Boolean* supported);

    CARAPI RegisterListenerEx3(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 rate,
        /* [in] */ IHandler* handler,
        /* [out] */ Boolean* supported);

protected:
    /**
     * Gets the full list of sensors that are available.
     * @hide
     */
    virtual CARAPI GetFullSensorList(
        /* [out, callee] */ ArrayOf<ISensor*>** sensors);

    virtual CARAPI_(void) UnregisterListenerImpl(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor);

    virtual CARAPI_(Boolean) RegisterListenerImpl(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 delay,
        /* [in] */ IHandler* handler);

friend class LegacySensorManager;
};

} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif //__CSENSORMANAGER_H__
