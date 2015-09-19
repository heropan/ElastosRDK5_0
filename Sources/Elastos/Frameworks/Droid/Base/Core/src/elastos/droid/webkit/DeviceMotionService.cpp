
#include "os/CHandler.h"
#include "webkit/DeviceMotionService.h"
#include "webkit/DeviceMotionAndOrientationManager.h"

#include <elastos/core/Thread.h>

using Elastos::Core::CDouble;
using Elastos::Core::IDouble;
using Elastos::Core::EIID_IRunnable;
using Elastos::Core::IThread;
using Elastos::Core::Thread;
using Elastos::Droid::Hardware::EIID_ISensorEventListener;
using Elastos::Droid::Os::CHandler;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//           DeviceMotionService::ErrorEventRunnable
//===============================================================

DeviceMotionService::ErrorEventRunnable::ErrorEventRunnable(
    /* [in] */ DeviceMotionService* owner)
    : mOwner(owner)
{
}

CAR_INTERFACE_IMPL_LIGHT(DeviceMotionService::ErrorEventRunnable, IRunnable);

ECode DeviceMotionService::ErrorEventRunnable::Run()
{
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String name;
    thread->GetName(&name);
    assert(WebViewCore::THREAD_NAME.Equals(name));
    if (mOwner->mIsRunning) {
        // The special case of all nulls is used to signify a failure to get data.
        mOwner->mManager->OnMotionChange(NULL, NULL, NULL, 0.0);
    }
    return NOERROR;
}

//===============================================================
//            DeviceMotionService::InnerRunnable
//===============================================================

DeviceMotionService::InnerRunnable::InnerRunnable(
    /* [in] */ DeviceMotionService* owner)
    : mOwner(owner)
{
}

CAR_INTERFACE_IMPL_LIGHT(DeviceMotionService::InnerRunnable, IRunnable);

ECode DeviceMotionService::InnerRunnable::Run()
{
    assert(mOwner->mIsRunning);
    AutoPtr<IDouble> value1, value2, value3;
    CDouble::New((*(mOwner->mLastAcceleration))[0], (IDouble**)&value1);
    CDouble::New((*(mOwner->mLastAcceleration))[1], (IDouble**)&value2);
    CDouble::New((*(mOwner->mLastAcceleration))[2], (IDouble**)&value3);
    mOwner->mManager->OnMotionChange(value1, value2, value3, INTERVAL_MILLIS);
    Boolean result = FALSE;
    mOwner->mHandler->PostDelayed(mOwner->mUpdateRunnable, INTERVAL_MILLIS, &result);
    // Now that we have successfully sent some data, reset whether we've sent an error.
    mOwner->mHaveSentErrorEvent = FALSE;
    return NOERROR;
}

//===============================================================
//                   DeviceMotionService
//===============================================================

const Int32 DeviceMotionService::INTERVAL_MILLIS;

DeviceMotionService::DeviceMotionService(
    /* [in] */ DeviceMotionAndOrientationManager* manager,
    /* [in] */ IContext* context)
{
    mManager = manager;
    assert(mManager != NULL);
    mContext = context;
    assert(mContext != NULL);
}

CAR_INTERFACE_IMPL(DeviceMotionService, ISensorEventListener);

void DeviceMotionService::Start()
{
    mIsRunning = TRUE;
    RegisterForSensor();
}

void DeviceMotionService::Stop()
{
    mIsRunning = FALSE;
    StopSendingUpdates();
    UnregisterFromSensor();
}

void DeviceMotionService::Suspend()
{
    if (mIsRunning) {
        StopSendingUpdates();
        UnregisterFromSensor();
    }
}

void DeviceMotionService::Resume()
{
    if (mIsRunning) {
        RegisterForSensor();
    }
}

void DeviceMotionService::Start(
    /* [in] */ IInterface* obj)
{
    ((DeviceMotionService*)obj)->Start();
}

void DeviceMotionService::Stop(
    /* [in] */ IInterface* obj)
{
    ((DeviceMotionService*)obj)->Stop();
}

void DeviceMotionService::Suspend(
    /* [in] */ IInterface* obj)
{
    ((DeviceMotionService*)obj)->Suspend();
}

void DeviceMotionService::Resume(
    /* [in] */ IInterface* obj)
{
    ((DeviceMotionService*)obj)->Resume();
}

/**
 * SensorEventListener implementation.
 * Callbacks happen on the thread on which we registered - the WebCore thread.
 */
ECode DeviceMotionService::OnSensorChanged(
    /* [in] */ ISensorEvent* event)
{
    VALIDATE_NOT_NULL(event);

    AutoPtr< ArrayOf<Float> > values;
    event->GetValues((ArrayOf<Float>**)&values);
    assert(values.GetLength() == 3);
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String name;
    thread->GetName(&name);
    assert(WebViewCore::THREAD_NAME.Equals(name));
    AutoPtr<ISensor> sensor;
    event->GetSensor((ISensor**)&sensor);
    Int32 type;
    sensor->GetType(&type);
    assert(type == ISensor::TYPE_ACCELEROMETER);

    // We may get callbacks after the call to getSensorManager().unregisterListener() returns.
    if (!mIsRunning) {
        return NOERROR;
    }

    Boolean firstData = mLastAcceleration == NULL;
    mLastAcceleration = values;
    if (firstData) {
            StartSendingUpdates();
    }

    return NOERROR;
}

ECode DeviceMotionService::OnAccuracyChanged(
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 accuracy)
{
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String name;
    thread->GetName(&name);
    assert(WebViewCore::THREAD_NAME.Equals(name));
    return NOERROR;
}

void DeviceMotionService::SendErrorEvent()
{
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String name;
    thread->GetName(&name);
    assert(WebViewCore::THREAD_NAME.Equals(name));
    // The spec requires that each listener receives the error event only once.
    if (mHaveSentErrorEvent)
        return;
    mHaveSentErrorEvent = TRUE;
    CreateHandler();
    AutoPtr<IRunnable> runnable = new ErrorEventRunnable(this);
    Boolean result = FALSE;
    mHandler->Post(runnable, &result);
}

void DeviceMotionService::CreateHandler()
{
    if (mHandler != NULL) {
        return;
    }

    CHandler::New((IHandler**)&mHandler);
    mUpdateRunnable = new InnerRunnable(this);
}

void DeviceMotionService::StartSendingUpdates()
{
    CreateHandler();
    mUpdateRunnable->Run();
}

void DeviceMotionService::StopSendingUpdates()
{
    assert(0);
//    mHandler->RemoveCallbacks(mUpdateRunnable);
    mLastAcceleration = NULL;
}

void DeviceMotionService::RegisterForSensor()
{
    if (!RegisterForAccelerometerSensor()) {
        SendErrorEvent();
    }
}

AutoPtr<ISensorManager> DeviceMotionService::GetSensorManager()
{
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String name;
    thread->GetName(&name);
    assert(WebViewCore::THREAD_NAME.Equals(name));
    if (mSensorManager == NULL) {
        mContext->GetSystemService(IContext::SENSOR_SERVICE,
            (IInterface**)&mSensorManager);
    }
    return mSensorManager;
}

Boolean DeviceMotionService::RegisterForAccelerometerSensor()
{
    AutoPtr< ArrayOf<ISensor*> > sensors;
    GetSensorManager()->GetSensorList(ISensor::TYPE_ACCELEROMETER, (ArrayOf<ISensor*>**)&sensors);
    if (sensors->GetLength() == 0) {
        return FALSE;
    }
    CreateHandler();
    // TODO: Consider handling multiple sensors.
    Boolean result = FALSE;
    GetSensorManager()->RegisterListener(
            this, (*sensors)[0], ISensorManager::SENSOR_DELAY_UI, mHandler, &result);
    return result;
}

void DeviceMotionService::UnregisterFromSensor()
{
    GetSensorManager()->UnregisterListener(this);
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
