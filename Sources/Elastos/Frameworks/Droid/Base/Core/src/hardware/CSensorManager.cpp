
#include "hardware/CSensorManager.h"


namespace Elastos {
namespace Droid {
namespace Hardware {

PInterface CSensorManager::Probe(
    /* [in] */ REIID riid)
{
    return _CSensorManager::Probe(riid);
}

ECode CSensorManager::GetSensors(
    /* [out] */ Int32* sensors)
{
    return SensorManager::GetSensors(sensors);
}

ECode CSensorManager::GetSensorList(
    /* [in] */ Int32 type,
    /* [out, callee] */ ArrayOf<ISensor*>** sensors)
{
    return SensorManager::GetSensorList(type, sensors);
}

ECode CSensorManager::GetDefaultSensor(
    /* [in] */ Int32 type,
    /* [out] */ ISensor** sensor)
{
    return SensorManager::GetDefaultSensor(type, sensor);
}

ECode CSensorManager::RegisterListener(
    /* [in] */ ISensorListener* listener,
    /* [in] */ Int32 sensors,
    /* [out] */ Boolean* supported)
{
    return SensorManager::RegisterListener(listener, sensors, supported);
}

ECode CSensorManager::RegisterListenerEx(
    /* [in] */ ISensorListener* listener,
    /* [in] */ Int32 sensors,
    /* [in] */ Int32 rate,
    /* [out] */ Boolean* supported)
{
    return SensorManager::RegisterListenerEx(listener, sensors, rate, supported);
}

ECode CSensorManager::UnregisterListener(
    /* [in] */ ISensorListener* listener)
{
    return SensorManager::UnregisterListener(listener);
}

ECode CSensorManager::UnregisterListenerEx(
    /* [in] */ ISensorListener* listener,
    /* [in] */ Int32 sensors)
{
    return SensorManager::UnregisterListenerEx(listener, sensors);
}

ECode CSensorManager::UnregisterListenerEx2(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor)
{
    return SensorManager::UnregisterListenerEx2(listener, sensor);
}

ECode CSensorManager::UnregisterListenerEx3(
    /* [in] */ ISensorEventListener* listener)
{
    return SensorManager::UnregisterListenerEx3(listener);
}

ECode CSensorManager::RegisterListenerEx2(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 rate,
    /* [out] */ Boolean* supported)
{
    return SensorManager::RegisterListenerEx2(listener, sensor, rate, supported);
}

ECode CSensorManager::RegisterListenerEx3(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 rate,
    /* [in] */ IHandler* handler,
    /* [out] */ Boolean* supported)
{
    return SensorManager::RegisterListenerEx3(listener, sensor, rate, handler, supported);
}

ECode CSensorManager::GetFullSensorList(
    /* [out, callee] */ ArrayOf<ISensor*>** sensors)
{
    assert(0);
    return NOERROR;
}

void CSensorManager::UnregisterListenerImpl(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor)
{
    //Pure virtual method.
    assert(0);
}

Boolean CSensorManager::RegisterListenerImpl(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 delay,
    /* [in] */ IHandler* handler)
{
    //Pure virtual method.
    assert(0);
    return FALSE;
}

} // namespace Hardware
} // namespace Droid
} // namespace Elastos
