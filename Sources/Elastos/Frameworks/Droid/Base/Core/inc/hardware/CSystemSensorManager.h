
#ifndef __ELASTOS_DROID_HARDWARE_CSYSTEMSENSORMANAGER_H__
#define  __ELASTOS_DROID_HARDWARE_CSYSTEMSENSORMANAGER_H__

#include "_Elastos_Droid_Hardware_CSystemSensorManager.h"
#include "hardware/SensorManager.h"
#include <elastos/utility/etl/List.h>
#include "os/Runnable.h"
#include "os/HandlerBase.h"

using Elastos::Utility::Etl::List;
using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Net::ILocalSocket;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::Runnable;

namespace Elastos {
namespace Droid {
namespace Hardware {

/**
 * Sensor manager implementation that communicates with the built-in
 * system sensors.
 *
 * @hide
 */
CarClass(CSystemSensorManager), public SensorManager
{
private:
    class SensorThread: public ElRefBase
    {
    private:
        class SensorThreadRunnable : public Runnable
        {
        public:
            SensorThreadRunnable(
                /* [in] */ SensorThread* host);

            virtual CARAPI Run();

        private:
            CARAPI_(Boolean) Open();

        public:
            Object  mLock;

        private:
            SensorThread* mHost;
        };

    public:
        SensorThread(
            /* [in] */ CSystemSensorManager* host);

    protected:
        // @Override
        // protected void finalize() {
        // }

        // must be called with sListeners lock
        CARAPI_(Boolean) StartLocked();

    protected:
        AutoPtr<IThread> mThread;
        CSystemSensorManager* mHost;
        Boolean mSensorsReady;
        friend class CSystemSensorManager;
    };

    /*-----------------------------------------------------------------------*/

    class ListenerDelegate: public ElRefBase
    {
    private:
    class MyHandler : public HandlerBase
    {
        public:
            MyHandler(
                /* [in] */ ILooper* looper,
                /* [in] */ ListenerDelegate* host)
                : HandlerBase(looper)
                , mHost(host)
            {}

            CARAPI HandleMessage(
                /* [in] */ IMessage* msg);
        private:
            ListenerDelegate* mHost;
        };

    public:
        ListenerDelegate(
            /* [in] */ ISensorEventListener* listener,
            /* [in] */ ISensor* sensor,
            /* [in] */ IHandler* handler,
            /* [in] */ CSystemSensorManager* host);

        CARAPI_(AutoPtr<IInterface>) GetListener();

        CARAPI_(void) AddSensor(
            /* [in] */ ISensor* sensor);

        CARAPI_(Int32) RemoveSensor(
            /* [in] */ ISensor* sensor);

        CARAPI_(Boolean) HasSensor(
            /* [in] */ ISensor* sensor);

        CARAPI_(AutoPtr<List< AutoPtr<ISensor> > >) GetSensors();

        CARAPI_(void) OnSensorChangedLocked(
            /* [in] */ ISensor* sensor,
            /* [in] */ ArrayOf<Float>* values,
            /* [in] */ ArrayOf<Int64>* timestamp,
            /* [in] */ Int32 accuracy);

    public:
        HashMap<Int32, Boolean> mSensors/* = new SparseBooleanArray()*/;
        HashMap<Int32, Boolean> mFirstEvent/* = new SparseBooleanArray()*/;
        HashMap<Int32, Int32> mSensorAccuracies/* = new SparseIntArray()*/;

    private:
        AutoPtr<ISensorEventListener> mSensorEventListener;
        AutoPtr< List< AutoPtr<ISensor> > > mSensorList/* = new ArrayList<Sensor>()*/;
        AutoPtr<IHandler> mHandler;
        CSystemSensorManager* mHost;
    };

public:

    CSystemSensorManager();

    /**
     * {@hide}
     */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ILooper* mainLooper);

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
        /* [out] */ Boolean* state);

    CARAPI RegisterListener(
        /* [in] */ ISensorListener* listener,
        /* [in] */ Int32 sensors,
        /* [in] */ Int32 rate,
        /* [out] */ Boolean* state);

    CARAPI UnregisterListener(
        /* [in] */ ISensorListener* listener);

    CARAPI UnregisterListener(
        /* [in] */ ISensorListener* listener,
        /* [in] */ Int32 sensors);

    CARAPI UnregisterListener(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor);

    CARAPI UnregisterListener(
        /* [in] */ ISensorEventListener* listener);

    CARAPI RegisterListener(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 rate,
        /* [out] */ Boolean* state);

    CARAPI RegisterListener(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 rate,
        /* [in] */ IHandler* handler,
        /* [out] */ Boolean* state);

protected:
    /** @hide */
    //@Override
    CARAPI GetFullSensorList(
        /* [out, callee] */ ArrayOf<ISensor*>** sensors);

    /** @hide */
    //@Override
    CARAPI_(Boolean) RegisterListenerImpl(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 delay,
        /* [in] */ IHandler* handler);

    /** @hide */
    //@Override
    CARAPI_(void) UnregisterListenerImpl(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor);

    // Used within this module from outside SensorManager, don't make private
    static CARAPI_(Int32) sensors_create_queue();

    static CARAPI_(void) sensors_destroy_queue(
        /* [in] */ Int32 queue);

    static CARAPI_(Boolean) sensors_enable_sensor(
        /* [in] */ Int32 queue,
        /* [in] */ const String& name,
        /* [in] */ Int32 sensor,
        /* [in] */ Int32 enable);

    static CARAPI_(Int32) sensors_data_poll(
        /* [in] */ Int32 queue,
        /* [in] */ ArrayOf<Float>* values,
        /* [in] */ ArrayOf<Int32>* status,
        /* [in] */ ArrayOf<Int64>* timestamp);

private:

    CARAPI_(Boolean) EnableSensorLocked(
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 delay);

    CARAPI_(Boolean) DisableSensorLocked(
        /* [in] */ ISensor* sensor);

    static CARAPI_(void) nativeClassInit();

    static CARAPI_(Int32) sensors_module_init();

    static CARAPI_(Int32) sensors_module_get_next_sensor(
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 next);

private:
    static const Int32 SENSOR_DISABLE;
    static Boolean sSensorModuleInitialized;
    // static ArrayList<Sensor> sFullSensorsList = new ArrayList<Sensor>();
    static List< AutoPtr<ISensor> > sFullSensorsList/* = new ArrayList<Sensor>()*/;
    /* The thread and the sensor list are global to the process
     * but the actual thread is spawned on demand */
    static AutoPtr<SensorThread> sSensorThread;
    static Int32 sQueue;

    // Used within this module from outside SensorManager, don't make private
    static /*SparseArray<Sensor>*/ HashMap<Int32, ISensor*> sHandleToSensor/* = new SparseArray<Sensor>()*/;
    static List<ListenerDelegate*> sListeners/* = new ArrayList<ListenerDelegate>()*/;
    static Object sListenersLock;

    // Common pool of sensor events.
    static AutoPtr<SensorEventPool> sPool;

    // Looper associated with the context in which this instance was created.
    AutoPtr<ILooper> mMainLooper;
    static AutoPtr<IContext> mContext;

    friend class ListenerDelegate;
};

} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_HARDWARE_CSYSTEMSENSORMANAGER_H__
