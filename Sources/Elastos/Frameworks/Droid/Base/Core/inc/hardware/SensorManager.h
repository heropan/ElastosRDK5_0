
#ifndef  __SENSORMANAGER_H__
#define  __SENSORMANAGER_H__

#include "_CSensorManager.h"
#include "hardware/LegacySensorManager.h"

using Elastos::Core::Mutex;
using Elastos::Droid::Os::IHandler;
using Elastos::Utility::IObjectMap;

namespace Elastos {
namespace Droid {
namespace Hardware {


class SensorManager
{
protected:
    /**
     * Sensor event pool implementation.
     * @hide
     */
    class SensorEventPool: public ElRefBase
    {
    public:
        SensorEventPool(
            /* [in] */ Int32 poolSize);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI_(AutoPtr<ISensorEvent>) GetFromPool();

        CARAPI_(void) ReturnToPool(
            /* [in] */ ISensorEvent* t);

    private:
        CARAPI_(AutoPtr<ISensorEvent>) CreateSensorEvent();

    private:
        Int32 mPoolSize;
        AutoPtr<ArrayOf<ISensorEvent*> > mPool;
        Int32 mNumItemsInPool;
        Mutex mLock;
    };

public:
    /**
     * {@hide}
     */
    SensorManager();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    /**
     * @return available sensors.
     * @deprecated This method is deprecated, use
     *             {@link SensorManager#getSensorList(Int32)} instead
     */
    //@Deprecated
    virtual CARAPI GetSensors(
        /* [out] */ Int32* sensors);

    /**
     * Use this method to get the list of available sensors of a certain type.
     * Make multiple calls to get sensors of different types or use
     * {@link android.hardware.Sensor#TYPE_ALL Sensor.TYPE_ALL} to get all the
     * sensors.
     *
     * @param type
     *        of sensors requested
     *
     * @return a list of sensors matching the asked type.
     *
     * @see #getDefaultSensor(Int32)
     * @see Sensor
     */
    virtual CARAPI GetSensorList(
        /* [in] */ Int32 type,
        /* [out, callee] */ ArrayOf<ISensor*>** sensors);

    /**
     * Use this method to get the default sensor for a given type. Note that the
     * returned sensor could be a composite sensor, and its data could be
     * averaged or filtered. If you need to access the raw sensors use
     * {@link SensorManager#getSensorList(Int32) getSensorList}.
     *
     * @param type
     *        of sensors requested
     *
     * @return the default sensors matching the asked type.
     *
     * @see #getSensorList(Int32)
     * @see Sensor
     */
    virtual CARAPI GetDefaultSensor(
        /* [in] */ Int32 type,
        /* [out] */ ISensor** sensor);

    /**
     * Registers a listener for given sensors.
     *
     * @deprecated This method is deprecated, use
     *             {@link SensorManager#registerListener(SensorEventListener, Sensor, Int32)}
     *             instead.
     *
     * @param listener
     *        sensor listener object
     *
     * @param sensors
     *        a bit masks of the sensors to register to
     *
     * @return <code>true</code> if the sensor is supported and successfully
     *         enabled
     */
    //@Deprecated
    virtual CARAPI RegisterListener(
        /* [in] */ ISensorListener* listener,
        /* [in] */ Int32 sensors,
        /* [out] */ Boolean* state);

    /**
     * Registers a SensorListener for given sensors.
     *
     * @deprecated This method is deprecated, use
     *             {@link SensorManager#registerListener(SensorEventListener, Sensor, Int32)}
     *             instead.
     *
     * @param listener
     *        sensor listener object
     *
     * @param sensors
     *        a bit masks of the sensors to register to
     *
     * @param rate
     *        rate of events. This is only a hint to the system. events may be
     *        received faster or slower than the specified rate. Usually events
     *        are received faster. The value must be one of
     *        {@link #SENSOR_DELAY_NORMAL}, {@link #SENSOR_DELAY_UI},
     *        {@link #SENSOR_DELAY_GAME}, or {@link #SENSOR_DELAY_FASTEST}.
     *
     * @return <code>true</code> if the sensor is supported and successfully
     *         enabled
     */
    //@Deprecated
    virtual CARAPI RegisterListener(
        /* [in] */ ISensorListener* listener,
        /* [in] */ Int32 sensors,
        /* [in] */ Int32 rate,
        /* [out] */ Boolean* state);

    /**
     * Unregisters a listener for all sensors.
     *
     * @deprecated This method is deprecated, use
     *             {@link SensorManager#unregisterListener(SensorEventListener)}
     *             instead.
     *
     * @param listener
     *        a SensorListener object
     */
    //@Deprecated
    virtual CARAPI UnregisterListener(
        /* [in] */ ISensorListener* listener);

    /**
     * Unregisters a listener for the sensors with which it is registered.
     *
     * @deprecated This method is deprecated, use
     *             {@link SensorManager#unregisterListener(SensorEventListener, Sensor)}
     *             instead.
     *
     * @param listener
     *        a SensorListener object
     *
     * @param sensors
     *        a bit masks of the sensors to unregister from
     */
    // /@Deprecated
    virtual CARAPI UnregisterListener(
        /* [in] */ ISensorListener* listener,
        /* [in] */ Int32 sensors);

    /**
     * Unregisters a listener for the sensors with which it is registered.
     *
     * @param listener
     *        a SensorEventListener object
     *
     * @param sensor
     *        the sensor to unregister from
     *
     * @see #unregisterListener(SensorEventListener)
     * @see #registerListener(SensorEventListener, Sensor, Int32)
     *
     */
    virtual CARAPI UnregisterListener(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor);

    /**
     * Unregisters a listener for all sensors.
     *
     * @param listener
     *        a SensorListener object
     *
     * @see #unregisterListener(SensorEventListener, Sensor)
     * @see #registerListener(SensorEventListener, Sensor, Int32)
     *
     */
    virtual CARAPI UnregisterListener(
        /* [in] */ ISensorEventListener* listener);

    /**
     * Registers a {@link android.hardware.SensorEventListener
     * SensorEventListener} for the given sensor.
     *
     * @param listener
     *        A {@link android.hardware.SensorEventListener SensorEventListener}
     *        object.
     *
     * @param sensor
     *        The {@link android.hardware.Sensor Sensor} to register to.
     *
     * @param rate
     *        The rate {@link android.hardware.SensorEvent sensor events} are
     *        delivered at. This is only a hint to the system. Events may be
     *        received faster or slower than the specified rate. Usually events
     *        are received faster. The value must be one of
     *        {@link #SENSOR_DELAY_NORMAL}, {@link #SENSOR_DELAY_UI},
     *        {@link #SENSOR_DELAY_GAME}, or {@link #SENSOR_DELAY_FASTEST}
     *        or, the desired delay between events in microsecond.
     *
     * @return <code>true</code> if the sensor is supported and successfully
     *         enabled.
     *
     * @see #registerListener(SensorEventListener, Sensor, Int32, Handler)
     * @see #unregisterListener(SensorEventListener)
     * @see #unregisterListener(SensorEventListener, Sensor)
     *
     */
    virtual CARAPI RegisterListener(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 rate,
        /* [out] */ Boolean* state);

    /**
     * Registers a {@link android.hardware.SensorEventListener
     * SensorEventListener} for the given sensor.
     *
     * @param listener
     *        A {@link android.hardware.SensorEventListener SensorEventListener}
     *        object.
     *
     * @param sensor
     *        The {@link android.hardware.Sensor Sensor} to register to.
     *
     * @param rate
     *        The rate {@link android.hardware.SensorEvent sensor events} are
     *        delivered at. This is only a hint to the system. Events may be
     *        received faster or slower than the specified rate. Usually events
     *        are received faster. The value must be one of
     *        {@link #SENSOR_DELAY_NORMAL}, {@link #SENSOR_DELAY_UI},
     *        {@link #SENSOR_DELAY_GAME}, or {@link #SENSOR_DELAY_FASTEST}.
     *        or, the desired delay between events in microsecond.
     *
     * @param handler
     *        The {@link android.os.Handler Handler} the
     *        {@link android.hardware.SensorEvent sensor events} will be
     *        delivered to.
     *
     * @return true if the sensor is supported and successfully enabled.
     *
     * @see #registerListener(SensorEventListener, Sensor, Int32)
     * @see #unregisterListener(SensorEventListener)
     * @see #unregisterListener(SensorEventListener, Sensor)
     *
     */
    virtual CARAPI RegisterListener(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 rate,
        /* [in] */ IHandler* handler,
        /* [out] */ Boolean* state);

protected:
    /**
     * Gets the full list of sensors that are available.
     * @hide
     */
    virtual CARAPI GetFullSensorList(
        /* [out, callee] */ ArrayOf<ISensor*>** sensors) = 0;

    /** @hide */
    virtual CARAPI_(void) UnregisterListenerImpl(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor) = 0;

    /** @hide */
    virtual CARAPI_(Boolean) RegisterListenerImpl(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 delay,
        /* [in] */ IHandler* handler) = 0;

private:
    CARAPI_(AutoPtr<LegacySensorManager>) GetLegacySensorManager();

public:
    static Mutex mLock;

protected:
    static String TAG;

private:
    // Cached lists of sensors by type.  Guarded by mSensorListByType.
    // final SparseArray<List<Sensor>> mSensorListByType =
    //         new SparseArray<List<Sensor>>();
    AutoPtr<IObjectMap> mSensorListByType;
    Mutex mSensorListByTypeLock;

    // Legacy sensor manager implementation.  Guarded by mSensorListByType during initialization.
    AutoPtr<LegacySensorManager> mLegacySensorManager;
};

} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif //__SENSORMANAGER_H__
