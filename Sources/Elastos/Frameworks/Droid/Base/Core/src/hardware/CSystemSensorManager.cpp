
#include "hardware/CSystemSensorManager.h"
#include "hardware/CSensorEvent.h"
#include "hardware/CSensor.h"
#include "net/CLocalSocket.h"
#include "net/CLocalSocketAddress.h"
#include "os/SystemProperties.h"
#include "os/Process.h"
#include "content/CIntentFilter.h"
#include "content/CIntent.h"
#include <unistd.h>
#include <gui/SensorManager.h>
#include <android/sensor.h>
#include <gui/Sensor.h>
#include "provider/Settings.h"

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::EIID_IRunnable;
using Elastos::IO::IFlushable;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Net::CLocalSocket;
using Elastos::Droid::Net::CLocalSocketAddress;
using Elastos::Droid::Net::ILocalSocketAddress;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Droid::Provider::Settings;
using Elastos::Droid::Content::IContentResolver;

namespace Elastos {
namespace Droid {
namespace Hardware {

const Int32 CSystemSensorManager::SENSOR_DISABLE = -1;
Boolean CSystemSensorManager::sSensorModuleInitialized = FALSE;
List< AutoPtr<ISensor> > CSystemSensorManager::sFullSensorsList;
AutoPtr<CSystemSensorManager::SensorThread> CSystemSensorManager::sSensorThread;
Int32 CSystemSensorManager::sQueue = 0;
HashMap<Int32, ISensor*> CSystemSensorManager::sHandleToSensor;
List<CSystemSensorManager::ListenerDelegate*> CSystemSensorManager::sListeners;
Mutex CSystemSensorManager::sListenersLock;
AutoPtr<SensorManager::SensorEventPool> CSystemSensorManager::sPool;
AutoPtr<IContext> CSystemSensorManager::mContext;

CSystemSensorManager::SensorThread::SensorThreadRunnable::SensorThreadRunnable(
    /* [in] */ SensorThread* host)
    : mHost(host)
{
}

Boolean CSystemSensorManager::SensorThread::SensorThreadRunnable::Open()
{
    // NOTE: this cannot synchronize on sListeners, since
    // it's held in the main thread at least until we
    // return from here.
    mHost->mHost->sQueue = sensors_create_queue();
    return TRUE;
}

ECode CSystemSensorManager::SensorThread::SensorThreadRunnable::Run()
{
    //Log.d(TAG, "entering main sensor thread");
    AutoPtr<ArrayOf<Float> > values = ArrayOf<Float>::Alloc(3);
    AutoPtr<ArrayOf<Int32> > status = ArrayOf<Int32>::Alloc(1);
    AutoPtr<ArrayOf<Int64> > timestamp = ArrayOf<Int64>::Alloc(1);

    Process::SetThreadPriority(IProcess::THREAD_PRIORITY_URGENT_DISPLAY);

    if (!Open()) {
        return NOERROR;
    }

    /*synchronized (this) */
    {
        AutoLock lock(mLock);
        // we've open the driver, we're ready to open the sensors
        mHost->mSensorsReady = TRUE;

        //TODO
        // this.notify();
    }

    while (TRUE) {
        // wait for an event
        Int32 sensor = sensors_data_poll(sQueue, values, status, timestamp);

        Int32 accuracy = (*status)[0];
        {
            AutoLock lock(sListenersLock);
            if (sensor == -1 || sListeners.IsEmpty()) {
                // we lost the connection to the event stream. this happens
                // when the last listener is removed or if there is an error
                if (sensor == -1 && !sListeners.IsEmpty()) {
                    // log a warning in case of abnormal termination
                    // Log.e(TAG, "_sensors_data_poll() failed, we bail out: sensors=" + sensor);
                }
                // we have no more listeners or polling failed, terminate the thread
                sensors_destroy_queue(sQueue);
                sQueue = 0;
                mHost->mThread = NULL;
                break;
            }

            AutoPtr<ISensor> sensorObject = sHandleToSensor[sensor];
            if (sensorObject != NULL) {
                // report the sensor event to all listeners that
                // care about it.
                List<ListenerDelegate*>::Iterator ator = sListeners.Begin();
                for (; ator != sListeners.End(); ++ator) {
                    AutoPtr<ListenerDelegate> listener = *ator;
                    if (listener->HasSensor(sensorObject)) {
                        // this is asynchronous (okay to call
                        // with sListeners lock held).
                        listener->OnSensorChangedLocked(sensorObject,
                                values, timestamp, accuracy);
                    }
                }
            }
        }
    }
    //Log.d(TAG, "exiting main sensor thread");

    return NOERROR;
}

CSystemSensorManager::SensorThread::SensorThread(
    /* [in] */ CSystemSensorManager* host)
    : mHost(host)
    , mSensorsReady(FALSE)
{
}

Boolean CSystemSensorManager::SensorThread::StartLocked()
{
    // try {
    if (mThread == NULL) {
        mSensorsReady = FALSE;
        AutoPtr<SensorThreadRunnable> runnable = new SensorThreadRunnable(this);
        AutoPtr<IThread> thread;
        CThread::New(runnable, String("SensorThread")/*SensorThread.class.getName()*/, (IThread**)&thread);
        thread->Start();
        {
            AutoLock lock(runnable->mLock);
            while (mSensorsReady == FALSE) {
                //TODO
                // runnable.wait();
                usleep(1 * 1000 * 1000);
            }
        }

        mThread = thread;
    }
    // } catch (InterruptedException e) {
    // }
    return mThread == NULL ? FALSE : TRUE;
}


ECode CSystemSensorManager::ListenerDelegate::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);
    CSensorEvent* t = (CSensorEvent*)(ISensorEvent::Probe(obj));

    Int32 handle = ((CSensor*)(t->mSensor.Get()))->GetHandle();
    Int32 type = 0;
    switch (t->mSensor->GetType(&type), type) {
        // Only report accuracy for sensors that support it.
        case ISensor::TYPE_MAGNETIC_FIELD:
        case ISensor::TYPE_ORIENTATION: {
            // call onAccuracyChanged() only if the value changes
            Int32 accuracy = mHost->mSensorAccuracies[handle];
            if ((t->mAccuracy >= 0) && (accuracy != t->mAccuracy)) {
                mHost->mSensorAccuracies[handle] = t->mAccuracy;
                mHost->mSensorEventListener->OnAccuracyChanged(t->mSensor, t->mAccuracy);
            }
            break;
        }
        default: {
            // For other sensors, just report the accuracy once
            if (mHost->mFirstEvent[handle] == FALSE) {
                mHost->mFirstEvent[handle] = TRUE;
                mHost->mSensorEventListener->OnAccuracyChanged(
                    t->mSensor, SENSOR_STATUS_ACCURACY_HIGH);
            }
            break;
        }
    }

    mHost->mSensorEventListener->OnSensorChanged(ISensorEvent::Probe(obj));
    sPool->ReturnToPool(t);

    return NOERROR;
}

CSystemSensorManager::ListenerDelegate::ListenerDelegate(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor,
    /* [in] */ IHandler* handler,
    /* [in] */ CSystemSensorManager* host)
    : mHost(host)
{
    mSensorList = new List< AutoPtr<ISensor> >();
    mSensorEventListener = listener;
    AutoPtr<ILooper> looper;
    if (handler != NULL) {
        handler->GetLooper((ILooper**)&looper);
    }
    else {
        looper = mHost->mMainLooper;
    }

    // currently we create one Handler instance per listener, but we could
    // have one per looper (we'd need to pass the ListenerDelegate
    // instance to handleMessage and keep track of them separately).
    mHandler = new MyHandler(looper, this);
    AddSensor(sensor);
}

AutoPtr<IInterface> CSystemSensorManager::ListenerDelegate::GetListener()
{
    return mSensorEventListener;
}

void CSystemSensorManager::ListenerDelegate::AddSensor(
    /* [in] */ ISensor* sensor)
{
    assert(sensor != NULL);
    mSensors[((CSensor*)sensor)->GetHandle()] = TRUE;
    mSensorList->PushBack(sensor);
}

Int32 CSystemSensorManager::ListenerDelegate::RemoveSensor(
    /* [in] */ ISensor* sensor)
{
    mSensors.Erase(((CSensor*)sensor)->GetHandle());
    mSensorList->Remove(sensor);
    return mSensors.GetSize();
}

Boolean CSystemSensorManager::ListenerDelegate::HasSensor(
    /* [in] */ ISensor* sensor)
{
    return mSensors[((CSensor*)sensor)->GetHandle()];
}

AutoPtr< List< AutoPtr<ISensor> > > CSystemSensorManager::ListenerDelegate::GetSensors()
{
    return mSensorList;
}

void CSystemSensorManager::ListenerDelegate::OnSensorChangedLocked(
    /* [in] */ ISensor* sensor,
    /* [in] */ ArrayOf<Float>* values,
    /* [in] */ ArrayOf<Int64>* timestamp,
    /* [in] */ Int32 accuracy)
{
    AutoPtr<ISensorEvent> event = sPool->GetFromPool();
    CSensorEvent* t = (CSensorEvent*)event.Get();
    AutoPtr<ArrayOf<Float> > v = t->mValues;
    (*v)[0] = (*values)[0];
    (*v)[1] = (*values)[1];
    (*v)[2] = (*values)[2];

    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    String str;
    assert(0 && "TODO");
    //TODO String str = Settings::System::GetString(cr, ISettingsSystem::ACCELEROMETER_COORDINATE);
    Int32 stype;
    sensor->GetType(&stype);
    if(!str.IsNull() && str.Equals("special")
        && ((stype == ISensor::TYPE_ACCELEROMETER) || (stype == ISensor::TYPE_GRAVITY))) {
        (*v)[0] = (*values)[1];
        (*v)[1] = -(*values)[0];
        (*v)[2] = (*values)[2];
    }
    else{
        (*v)[0] = (*values)[0];
        (*v)[1] = (*values)[1];
        (*v)[2] = (*values)[2];
    }

    Int32 hwr = SystemProperties::GetInt32(String("ro.sf.hwrotation"), 0);
    if (hwr == 270) {
        (*t->mOriginalValues)[0]  = (*values)[1];
        (*t->mOriginalValues)[1]  = -(*values)[0];
        (*t->mOriginalValues)[2]  = (*values)[2];
        (*v)[0] = (*values)[1];
        (*v)[1] = -(*values)[0];
        (*v)[2] = (*values)[2];
    }
    else {
        (*t->mOriginalValues)[0]  = (*values)[0];
        (*t->mOriginalValues)[1]  = (*values)[1];
        (*t->mOriginalValues)[2]  = (*values)[2];
    }

    t->mTimestamp = (*timestamp)[0];
    t->mAccuracy = accuracy;
    t->mSensor = sensor;

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(0, event, (IMessage**)&msg);
    msg->SetAsynchronous(TRUE);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

CSystemSensorManager::CSystemSensorManager()
{
}

ECode CSystemSensorManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ILooper* mainLooper)
{
    mMainLooper = mainLooper;
    mContext = context;

    {
        AutoLock lock(sListenersLock);
        if (!sSensorModuleInitialized) {
            sSensorModuleInitialized = TRUE;

            nativeClassInit();

            // initialize the sensor list
            sensors_module_init();
            Int32 i = 0;
            do {
                AutoPtr<ISensor> sensor;
                CSensor::New((ISensor**)&sensor);
                i = sensors_module_get_next_sensor(sensor, i);

                if (i>=0) {
                    //Log.d(TAG, "found sensor: " + sensor.getName() +
                    //        ", handle=" + sensor.getHandle());
                    sFullSensorsList.PushBack(sensor);
                    Int32 h = ((CSensor*)sensor.Get())->GetHandle();

                    // sHandleToSensor.append(sensor->GetHandle(), sensor);
                    sHandleToSensor[h] = sensor.Get();
                }
            } while (i>0);

            sPool = new SensorEventPool(sFullSensorsList.GetSize() * 2);
            sSensorThread = new SensorThread(this);
        }
    }

    return NOERROR;
}

PInterface CSystemSensorManager::Probe(
    /* [in] */ REIID riid)
{
    return _CSystemSensorManager::Probe(riid);
}

CSystemSensorManager::GetSensors(
    /* [out] */ Int32* sensors)
{
    return SensorManager::GetSensors(sensors);
}

CSystemSensorManager::GetSensorList(
    /* [in] */ Int32 type,
    /* [out, callee] */ ArrayOf<ISensor*>** sensors)
{
    return SensorManager::GetSensorList(type, sensors);
}

CSystemSensorManager::GetDefaultSensor(
    /* [in] */ Int32 type,
    /* [out] */ ISensor** sensor)
{
    return SensorManager::GetDefaultSensor(type, sensor);
}

CSystemSensorManager::RegisterListener(
    /* [in] */ ISensorListener* listener,
    /* [in] */ Int32 sensors,
    /* [out] */ Boolean* state)
{
    return SensorManager::RegisterListener(listener, sensors, state);
}

CSystemSensorManager::RegisterListener(
    /* [in] */ ISensorListener* listener,
    /* [in] */ Int32 sensors,
    /* [in] */ Int32 rate,
    /* [out] */ Boolean* state)
{
    return SensorManager::RegisterListener(listener, sensors, rate, state);
}

CSystemSensorManager::UnregisterListener(
    /* [in] */ ISensorListener* listener)
{
    return SensorManager::UnregisterListener(listener);
}

CSystemSensorManager::UnregisterListener(
    /* [in] */ ISensorListener* listener,
    /* [in] */ Int32 sensors)
{
    return SensorManager::UnregisterListener(listener, sensors);
}

CSystemSensorManager::UnregisterListener(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor)
{
    return SensorManager::UnregisterListener(listener, sensor);
}

CSystemSensorManager::UnregisterListener(
    /* [in] */ ISensorEventListener* listener)
{
    return SensorManager::UnregisterListener(listener);
}

CSystemSensorManager::RegisterListener(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 rate,
    /* [out] */ Boolean* state)
{
    return SensorManager::RegisterListener(listener, sensor, rate, state);
}

CSystemSensorManager::RegisterListener(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 rate,
    /* [in] */ IHandler* handler,
    /* [out] */ Boolean* state)
{
    return SensorManager::RegisterListener(listener, sensor, rate, handler, state);
}

ECode CSystemSensorManager::GetFullSensorList(
    /* [out, callee] */ ArrayOf<ISensor*>** sensors)
{
    VALIDATE_NOT_NULL(sensors);
    *sensors = NULL;
    if (sFullSensorsList.IsEmpty() == FALSE) {
        *sensors = ArrayOf<ISensor*>::Alloc(sFullSensorsList.GetSize());

        List< AutoPtr<ISensor> >::Iterator ator = sFullSensorsList.Begin();
        Int32 index = 0;
        for (; ator != sFullSensorsList.End(); ++ator) {
            (*sensors)->Set(index++, *ator);
        }
    }
    REFCOUNT_ADDREF(*sensors);

    return NOERROR;
}

Boolean CSystemSensorManager::EnableSensorLocked(
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 delay)
{
    Boolean result = FALSE;
    List<ListenerDelegate*>::Iterator ator = sListeners.Begin();
    for (; ator != sListeners.End(); ++ator) {
        AutoPtr<ListenerDelegate> i = *ator;
        if (i->HasSensor(sensor)) {
            assert(sensor != NULL);

            String name;
            sensor->GetName(&name);

            Int32 handle = ((CSensor*)sensor)->GetHandle();
            result = sensors_enable_sensor(sQueue, name, handle, delay);
            break;
        }
    }

    return result;
}

Boolean CSystemSensorManager::DisableSensorLocked(
    /* [in] */ ISensor* sensor)
{
    List<ListenerDelegate*>::Iterator ator = sListeners.Begin();
    for (; ator != sListeners.End(); ++ator) {
        AutoPtr<ListenerDelegate> i = *ator;
        if (i->HasSensor(sensor)) {
            // not an error, it's just that this sensor is still in use
            return TRUE;
        }
    }

    assert(sensor != NULL);
    String name;
    sensor->GetName(&name);

    Int32 handle = ((CSensor*)sensor)->GetHandle();
    return sensors_enable_sensor(sQueue, name, handle, SENSOR_DISABLE);
}

Boolean CSystemSensorManager::RegisterListenerImpl(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 delay,
    /* [in] */ IHandler* handler)
{
    Boolean result = TRUE;
    {
        AutoLock lock(sListenersLock);
        // look for this listener in our list
        AutoPtr<ListenerDelegate> l;

        List<ListenerDelegate*>::Iterator ator = sListeners.Begin();
        for (; ator != sListeners.End(); ++ator) {
            AutoPtr<ListenerDelegate> i = *ator;

            if (i->GetListener() == listener) {
                l = i;
                break;
            }
        }

        // if we don't find it, add it to the list
        if (l == NULL) {
            l = new ListenerDelegate(listener, sensor, handler, this);
            sListeners.PushBack(l);
            // if the list is not empty, start our main thread
            if (!sListeners.IsEmpty()) {
                if (sSensorThread->StartLocked()) {
                    if (!EnableSensorLocked(sensor, delay)) {
                        // oops. there was an error
                        sListeners.Remove(l);
                        result = FALSE;
                    }
                } else {
                    // there was an error, remove the listener
                    sListeners.Remove(l);
                    result = FALSE;
                }
            } else {
                // weird, we couldn't add the listener
                result = FALSE;
            }
        } else if (!l->HasSensor(sensor)) {
            l->AddSensor(sensor);
            if (!EnableSensorLocked(sensor, delay)) {
                // oops. there was an error
                l->RemoveSensor(sensor);
                result = FALSE;
            }
        }
    }

    return result;
}

void CSystemSensorManager::UnregisterListenerImpl(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor)
{
    {
        AutoLock lock(sListenersLock);
        List<ListenerDelegate*>::Iterator ator = sListeners.Begin();
        //Int32 type = 0;
        //assert(sensor != NULL);
        //sensor->GetType(&type);
        for (; ator != sListeners.End(); ++ator) {
            AutoPtr<ListenerDelegate> l = *ator;
            if (l->GetListener() == listener) {
                if (sensor == NULL) {
                    sListeners.Remove(l.Get());

                    // disable all sensors for this listener
                    AutoPtr< List< AutoPtr<ISensor> > > tmp = l->GetSensors();
                    assert(tmp != NULL);

                    List< AutoPtr<ISensor> >::Iterator tmpAtor = tmp->Begin();
                    for (; tmpAtor != tmp->End(); ++tmpAtor) {
                        AutoPtr<ISensor> s = *tmpAtor;
                        DisableSensorLocked(s);
                    }
                }
                else if (l->RemoveSensor(sensor) == 0) {
                    // if we have no more sensors enabled on this listener,
                    // take it off the list.
                    sListeners.Remove(l.Get());
                    DisableSensorLocked(sensor);
                }
                break;
            }
        }
    }
}

Int32 CSystemSensorManager::sensors_create_queue()
{
    android::SensorManager& mgr(android::SensorManager::getInstance());
    android::sp<android::SensorEventQueue> queue(mgr.createEventQueue());
    queue->incStrong((const char*) String("CSystemSensorManager")/*clazz*/);
    return reinterpret_cast<int>(queue.get());
}

void CSystemSensorManager::sensors_destroy_queue(
    /* [in] */ Int32 value)
{
    android::sp<android::SensorEventQueue> queue(reinterpret_cast<android::SensorEventQueue *>(value));
    if (queue != 0) {
        queue->decStrong((const char*) String("CSystemSensorManager")/*clazz*/);
    }
}

Boolean CSystemSensorManager::sensors_enable_sensor(
    /* [in] */ Int32 nativeQueue,
    /* [in] */ const String& name,
    /* [in] */ Int32 sensor,
    /* [in] */ Int32 delay)
{
    android::sp<android::SensorEventQueue> queue(reinterpret_cast<android::SensorEventQueue *>(nativeQueue));
    if (queue == 0) return FALSE;
    ECode res = NOERROR;
    if (delay >= 0) {
        res = queue->enableSensor(sensor, delay);
    } else {
        res = queue->disableSensor(sensor);
    }
    return res == NOERROR ? TRUE : FALSE;
}

Int32 CSystemSensorManager::sensors_data_poll(
    /* [in] */ Int32 nativeQueue,
    /* [in] */ ArrayOf<Float>* values,
    /* [in] */ ArrayOf<Int32>* status,
    /* [in] */ ArrayOf<Int64>* timestamp)
{
    android::sp<android::SensorEventQueue> queue(reinterpret_cast<android::SensorEventQueue *>(nativeQueue));
    if (queue == 0) return -1;

    ECode res = NOERROR;
    ASensorEvent event;

    res = queue->read(&event, 1);
    if (res == 0) {
        res = queue->waitForEvent();
        if (res != NOERROR)
            return -1;
        // here we're guaranteed to have an event
        res = queue->read(&event, 1);
        // ALOGE_IF(res==0, "sensors_data_poll: nothing to read after waitForEvent()");
    }
    if (res <= 0) {
        return -1;
    }

    Int32 accuracy = event.vector.status;
    assert(values != NULL);
    // env->SetFloatArrayRegion(values, 0, 3, event.vector.v);
    (*values)[0] = event.vector.v[0];
    (*values)[1] = event.vector.v[1];
    (*values)[2] = event.vector.v[2];

    assert(status != NULL);
    // env->SetIntArrayRegion(status, 0, 1, &accuracy);
    (*status)[0] = accuracy;

    assert(timestamp != NULL);
    // env->SetLongArrayRegion(timestamp, 0, 1, &event.timestamp);
    (*timestamp)[0] = event.timestamp;

    return event.sensor;
}

void CSystemSensorManager::nativeClassInit()
{
    // jclass sensorClass = _env->FindClass("android/hardware/Sensor");
    // android::SensorOffsets& sensorOffsets = gSensorOffsets;
    // sensorOffsets.name        = _env->GetFieldID(sensorClass, "mName",      "Ljava/lang/String;");
    // sensorOffsets.vendor      = _env->GetFieldID(sensorClass, "mVendor",    "Ljava/lang/String;");
    // sensorOffsets.version     = _env->GetFieldID(sensorClass, "mVersion",   "I");
    // sensorOffsets.handle      = _env->GetFieldID(sensorClass, "mHandle",    "I");
    // sensorOffsets.type        = _env->GetFieldID(sensorClass, "mType",      "I");
    // sensorOffsets.range       = _env->GetFieldID(sensorClass, "mMaxRange",  "F");
    // sensorOffsets.resolution  = _env->GetFieldID(sensorClass, "mResolution","F");
    // sensorOffsets.power       = _env->GetFieldID(sensorClass, "mPower",     "F");
    // sensorOffsets.minDelay    = _env->GetFieldID(sensorClass, "mMinDelay",  "I");
}

Int32 CSystemSensorManager::sensors_module_init()
{
    android::SensorManager::getInstance();
    return 0;
}

Int32 CSystemSensorManager::sensors_module_get_next_sensor(
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 next)
{
    android::SensorManager& mgr(android::SensorManager::getInstance());

    android::Sensor const* const* sensorList;
    size_t count = mgr.getSensorList(&sensorList);
    if (size_t(next) >= count)
        return -1;

    android::Sensor const* const list = sensorList[next];
    // const android::SensorOffsets& sensorOffsets(gSensorOffsets);
    // jstring name = env->NewStringUTF(list->getName().string());
    // jstring vendor = env->NewStringUTF(list->getVendor().string());
    // env->SetObjectField(sensor, sensorOffsets.name,      name);
    // env->SetObjectField(sensor, sensorOffsets.vendor,    vendor);
    // env->SetIntField(sensor, sensorOffsets.version,      list->getVersion());
    // env->SetIntField(sensor, sensorOffsets.handle,       list->getHandle());
    // env->SetIntField(sensor, sensorOffsets.type,         list->getType());
    // env->SetFloatField(sensor, sensorOffsets.range,      list->getMaxValue());
    // env->SetFloatField(sensor, sensorOffsets.resolution, list->getResolution());
    // env->SetFloatField(sensor, sensorOffsets.power,      list->getPowerUsage());
    // env->SetIntField(sensor, sensorOffsets.minDelay,     list->getMinDelay());
    ((CSensor*)sensor)->mName = String(list->getName().string());
    ((CSensor*)sensor)->mVendor = String(list->getVendor().string());
    ((CSensor*)sensor)->mVersion = list->getVersion();
    ((CSensor*)sensor)->mHandle = list->getHandle();
    ((CSensor*)sensor)->mType = list->getType();
    ((CSensor*)sensor)->mMaxRange = list->getMaxValue();
    ((CSensor*)sensor)->mResolution = list->getResolution();
    ((CSensor*)sensor)->mPower = list->getPowerUsage();
    ((CSensor*)sensor)->mMinDelay = list->getMinDelay();

    next++;
    return size_t(next) < count ? next : 0;
}


} // namespace Hardware
} // namespace Droid
} // namespace Elastos
