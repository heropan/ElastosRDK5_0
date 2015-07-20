
#include "hardware/SensorManager.h"
#include "hardware/CSensorEvent.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Utility::CObjectMap;
using Elastos::Core::CObjectContainer;


namespace Elastos {
namespace Droid {
namespace Hardware {

String SensorManager::TAG("SensorManager");
Mutex SensorManager::mLock;


AutoPtr<ISensorEvent> SensorManager::SensorEventPool::CreateSensorEvent()
{
    // maximal size for all legacy events is 3
    AutoPtr<ISensorEvent> value;
    CSensorEvent::New(3, (ISensorEvent**)&value);
    return value;
}

SensorManager::SensorEventPool::SensorEventPool(
    /* [in] */ Int32 poolSize)
    : mPoolSize(poolSize)
    , mNumItemsInPool(poolSize)
{
    mPool = ArrayOf<ISensorEvent*>::Alloc(poolSize);
}

UInt32 SensorManager::SensorEventPool::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SensorManager::SensorEventPool::Release()
{
    return ElRefBase::Release();
}

AutoPtr<ISensorEvent> SensorManager::SensorEventPool::GetFromPool()
{
    AutoPtr<ISensorEvent> t;
    {
        AutoLock lock(mLock);
        if (mNumItemsInPool > 0) {
            // remove the "top" item from the pool
            const Int32 index = mPoolSize - mNumItemsInPool;
            t = (*mPool)[index];
            mPool->Set(index, NULL);
            mNumItemsInPool--;
        }
    }
    if (t == NULL) {
        // the pool was empty or this item was removed from the pool for
        // the first time. In any case, we need to create a new item.
        t = CreateSensorEvent();
    }
    return t;
}

void SensorManager::SensorEventPool::ReturnToPool(
    /* [in] */ ISensorEvent* t)
{
    AutoLock lock(mLock);
    // is there space left in the pool?
    if (mNumItemsInPool < mPoolSize) {
        // if so, return the item to the pool
        mNumItemsInPool++;
        const Int32 index = mPoolSize - mNumItemsInPool;
        (*mPool)[index] = t;
    }
}

SensorManager::SensorManager()
{
    CObjectMap::New((IObjectMap**)&mSensorListByType);
}

ECode SensorManager::GetSensors(
    /* [out] */ Int32* sensors)
{
    assert(sensors != NULL);
    *sensors = GetLegacySensorManager()->GetSensors();
    return NOERROR;
}

ECode SensorManager::GetSensorList(
    /* [in] */ Int32 type,
    /* [out, callee] */ ArrayOf<ISensor*>** sensors)
{
    assert(sensors != NULL);

    // cache the returned lists the first time
    AutoPtr<IObjectContainer> list;
    AutoPtr<ArrayOf<ISensor*> > fullList;
    GetFullSensorList((ArrayOf<ISensor*>**)&fullList);
    {
        AutoLock lock(mSensorListByTypeLock);

        AutoPtr<IInteger32> key;
        CInteger32::New(type, (IInteger32**)&key);
        mSensorListByType->Get(key.Get(), (IInterface**)&list);
        if (list == NULL) {
            CObjectContainer::New((IObjectContainer**)&list);
            Int32 length = fullList != NULL ? fullList->GetLength() : 0;
            if (type == ISensor::TYPE_ALL) {
                for (Int32 i = 0; i < length; i++) {
                    list->Add((*fullList)[i]);
                }
            } else {
                for (Int32 i = 0; i < length; i++) {
                    AutoPtr<ISensor> sensor = (*fullList)[i];
                    Int32 value = 0;
                    sensor->GetType(&value);
                    if (value == type) {
                        list->Add(sensor.Get());
                    }
                }
            }

            //TODO : make the list's item read only;
            // list = Collections.unmodifiableList(list);
            mSensorListByType->Put(key.Get(), list);
        }
    }

    assert(list != NULL);
    Int32 count = 0;
    list->GetObjectCount(&count);
    if (count > 0) {
        *sensors = ArrayOf<ISensor*>::Alloc(count);
        REFCOUNT_ADD(*sensors);

        AutoPtr<IObjectEnumerator> enumObj;
        list->GetObjectEnumerator((IObjectEnumerator**)&enumObj);
        Boolean has = FALSE;
        Int32 index = 0;
        while (enumObj->MoveNext(&has), has) {
            AutoPtr<ISensor> sensor;
            enumObj->Current((IInterface**)&sensor);
            (*sensors)->Set(index++, sensor.Get());
        }
    }

    return NOERROR;
}

ECode SensorManager::GetDefaultSensor(
    /* [in] */ Int32 type,
    /* [out] */ ISensor** sensor)
{
    assert(sensor != NULL);
    *sensor = NULL;
    // TODO: need to be smarter, for now, just return the 1st sensor
    AutoPtr<ArrayOf<ISensor*> > l;
    GetSensorList(type, (ArrayOf<ISensor*>**)&l);

    if (l != NULL && l->GetLength() > 0) {
        *sensor = (*l)[0];
        REFCOUNT_ADD(*sensor);
    }

    return NOERROR;
}

ECode SensorManager::RegisterListener(
    /* [in] */ ISensorListener* listener,
    /* [in] */ Int32 sensors,
    /* [out] */ Boolean* state)
{
    return RegisterListener(listener, sensors, ISensorManager::SENSOR_DELAY_NORMAL, state);
}

ECode SensorManager::RegisterListener(
    /* [in] */ ISensorListener* listener,
    /* [in] */ Int32 sensors,
    /* [in] */ Int32 rate,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = GetLegacySensorManager()->RegisterListener(listener, sensors, rate);
    return NOERROR;
}

ECode SensorManager::UnregisterListener(
    /* [in] */ ISensorListener* listener)
{
    return UnregisterListener(listener, ISensorManager::SENSOR_ALL | ISensorManager::SENSOR_ORIENTATION_RAW);
}

ECode SensorManager::UnregisterListener(
    /* [in] */ ISensorListener* listener,
    /* [in] */ Int32 sensors)
{
    GetLegacySensorManager()->UnregisterListener(listener, sensors);
    return NOERROR;
}

ECode SensorManager::UnregisterListener(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor)
{
    if (listener == NULL || sensor == NULL) {
        return NOERROR;
    }

    UnregisterListenerImpl(listener, sensor);
    return NOERROR;
}

ECode SensorManager::UnregisterListener(
    /* [in] */ ISensorEventListener* listener)
{
    if (listener == NULL) {
        return NOERROR;
    }

    UnregisterListenerImpl(listener, NULL);
    return NOERROR;
}

ECode SensorManager::RegisterListener(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 rate,
    /* [out] */ Boolean* state)
{
    return RegisterListener(listener, sensor, rate, NULL, state);
}

ECode SensorManager::RegisterListener(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 rate,
    /* [in] */ IHandler* handler,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    if (listener == NULL || sensor == NULL) {
        return FALSE;
    }

    Int32 delay = -1;
    switch (rate) {
        case ISensorManager::SENSOR_DELAY_FASTEST:
            delay = 0;
            break;
        case ISensorManager::SENSOR_DELAY_GAME:
            delay = 20000;
            break;
        case ISensorManager::SENSOR_DELAY_UI:
            delay = 66667;
            break;
        case ISensorManager::SENSOR_DELAY_NORMAL:
            delay = 200000;
            break;
        default:
            delay = rate;
            break;
    }

    *state = RegisterListenerImpl(listener, sensor, delay, handler);
    return NOERROR;
}

AutoPtr<LegacySensorManager> SensorManager::GetLegacySensorManager()
{
    AutoLock lock(mSensorListByTypeLock);
    if (mLegacySensorManager == NULL) {
        // Log.i(TAG, "This application is using deprecated SensorManager API which will "
        //         + "be removed someday.  Please consider switching to the new API.");
        mLegacySensorManager = new LegacySensorManager((ISensorManager*)this->Probe(EIID_ISensorManager));
    }

    return mLegacySensorManager;
}


} // namespace Hardware
} // namespace Droid
} // namespace Elastos
