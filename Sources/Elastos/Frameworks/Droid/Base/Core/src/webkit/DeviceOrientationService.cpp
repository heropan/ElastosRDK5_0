
#include "hardware/CSensorManagerHelper.h"
#include "os/CHandler.h"
#include "webkit/DeviceOrientationService.h"
#include "webkit/DeviceMotionAndOrientationManager.h"

#include <elastos/Math.h>
#include <elastos/Thread.h>

using Elastos::Core::CDouble;
using Elastos::Core::IDouble;
using Elastos::Core::EIID_IRunnable;
using Elastos::Core::Threading::Thread;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Hardware::EIID_ISensorEventListener;
using Elastos::Droid::Hardware::CSensorManagerHelper;
using Elastos::Droid::Hardware::ISensorManagerHelper;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//            DeviceOrientationService::InnerRunnable
//===============================================================

DeviceOrientationService::InnerRunnable::InnerRunnable(
    /* [in] */ DeviceOrientationService* owner)
    : mOwner(owner)
{
}

CAR_INTERFACE_IMPL_LIGHT(DeviceOrientationService::InnerRunnable, IRunnable);

ECode DeviceOrientationService::InnerRunnable::Run()
{
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String name;
    thread->GetName(&name);
    assert(CWebViewCore::THREAD_NAME.Equals(name));
    if (mOwner->mIsRunning) {
        // The special case of all nulls is used to signify a failure to get data.
        mOwner->mManager->OnOrientationChange(NULL, NULL, NULL);
    }

    return NOERROR;
}

//===============================================================
//                   DeviceOrientationService
//===============================================================

const Double DeviceOrientationService::DELTA_DEGRESS;

DeviceOrientationService::DeviceOrientationService(
    /* [in] */ DeviceMotionAndOrientationManager* manager,
    /* [in] */ IContext* context)
    : mManager(manager)
    , mContext(context)
    , mIsRunning(FALSE)
    , mAlpha(0.0)
    , mBeta(0.0)
    , mGamma(0.0)
    , mHaveSentErrorEvent(FALSE)
{
    assert(mManager != NULL);
    assert(mContext != NULL);
}

CAR_INTERFACE_IMPL(DeviceOrientationService, ISensorEventListener);

void DeviceOrientationService::Start()
{
    mIsRunning = TRUE;
    RegisterForSensors();
}

void DeviceOrientationService::Stop()
{
    mIsRunning = FALSE;
    UnregisterFromSensors();
}

void DeviceOrientationService::Suspend()
{
    if (mIsRunning) {
        UnregisterFromSensors();
    }
}

void DeviceOrientationService::Resume()
{
    if (mIsRunning) {
        RegisterForSensors();
    }
}
void DeviceOrientationService::Start(
    /* [in] */ IInterface* obj)
{
    ((DeviceOrientationService*)obj)->Start();
}

void DeviceOrientationService::Stop(
    /* [in] */ IInterface* obj)
{
    ((DeviceOrientationService*)obj)->Stop();
}

void DeviceOrientationService::Suspend(
    /* [in] */ IInterface* obj)
{
    ((DeviceOrientationService*)obj)->Suspend();
}

void DeviceOrientationService::Resume(
    /* [in] */ IInterface* obj)
{
    ((DeviceOrientationService*)obj)->Resume();
}


/**
 * SensorEventListener implementation.
 * Callbacks happen on the thread on which we registered - the WebCore thread.
 */
ECode DeviceOrientationService::OnSensorChanged(
    /* [in] */ ISensorEvent* event)
{
    VALIDATE_NOT_NULL(event);

    AutoPtr< ArrayOf<Float> > values;
    event->GetValues((ArrayOf<Float>**)&values);
    assert(values.GetLength() == 3);
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String name;
    thread->GetName(&name);
    assert (CWebViewCore::THREAD_NAME.Equals(name));

    // We may get callbacks after the call to getSensorManager().unregisterListener() returns.
    if (!mIsRunning) {
        return NOERROR;
    }

    AutoPtr<ISensor> sensor;
    event->GetSensor((ISensor**)&sensor);
    Int32 type;
    sensor->GetType(&type);
    switch (type) {
      case ISensor::TYPE_ACCELEROMETER:
        if (mGravityVector == NULL) {
            mGravityVector = ArrayOf<Float>::Alloc(3);
        }
        (*mGravityVector)[0] = (*values)[0];
        (*mGravityVector)[1] = (*values)[1];
        (*mGravityVector)[2] = (*values)[2];
        GetOrientationUsingGetRotationMatrix();
        break;
      case ISensor::TYPE_MAGNETIC_FIELD:
        if (mMagneticFieldVector == NULL) {
            mMagneticFieldVector = ArrayOf<Float>::Alloc(3);
        }
        (*mMagneticFieldVector)[0] = (*values)[0];
        (*mMagneticFieldVector)[1] = (*values)[1];
        (*mMagneticFieldVector)[2] = (*values)[2];
        GetOrientationUsingGetRotationMatrix();
        break;
      default:
        assert(FALSE);
    }
}

ECode DeviceOrientationService::OnAccuracyChanged(
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 accuracy)
{
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String name;
    thread->GetName(&name);
    assert (CWebViewCore::THREAD_NAME.Equals(name));
    return NOERROR;
}

void DeviceOrientationService::SendErrorEvent()
{
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String name;
    thread->GetName(&name);
    assert (CWebViewCore::THREAD_NAME.Equals(name));
    // The spec requires that each listener receives the error event only once.
    if (mHaveSentErrorEvent)
        return;
    mHaveSentErrorEvent = TRUE;
    AutoPtr<IRunnable> runnable = new InnerRunnable(this);
    Boolean result = FALSE;
    mHandler->Post(runnable, &result);
}

void DeviceOrientationService::RegisterForSensors()
{
    if (mHandler == NULL) {
        CHandler::New((IHandler**)&mHandler);
    }

    if (!RegisterForAccelerometerSensor() || !RegisterForMagneticFieldSensor()) {
        UnregisterFromSensors();
        SendErrorEvent();
    }
}

void DeviceOrientationService::GetOrientationUsingGetRotationMatrix()
{
    if (mGravityVector == NULL || mMagneticFieldVector == NULL) {
        return;
    }

    // Get the rotation matrix.
    // The rotation matrix that transforms from the body frame to the earth frame.
    AutoPtr< ArrayOf<Float> > deviceRotationMatrix = ArrayOf<Float>::Alloc(9);
    AutoPtr<ISensorManagerHelper> smh;
    CSensorManagerHelper::AcquireSingleton((ISensorManagerHelper**)&smh);
    Boolean success = FALSE;
    smh->GetRotationMatrix(
            deviceRotationMatrix, NULL, mGravityVector, mMagneticFieldVector, &success);
    if (!success) {
        return;
    }

    // Convert rotation matrix to rotation angles.
    // Assuming that the rotations are appied in the order listed at
    // http://developer.android.com/reference/android/hardware/SensorEvent.html#values
    // the rotations are applied about the same axes and in the same order as required by the
    // API. The only conversions are sign changes as follows.
    // The angles are in radians
    AutoPtr< ArrayOf<Float> > rotationAngles = ArrayOf<Float>::Alloc(3);
    AutoPtr< ArrayOf<Float> > fields;
    smh->GetOrientation(deviceRotationMatrix, rotationAngles, (ArrayOf<Float>**)&fields);
    Double alpha = Elastos::Core::Math::ToDegrees(-(*rotationAngles)[0]);
    while (alpha < 0.0) { alpha += 360.0; } // [0, 360)
    Double beta = Elastos::Core::Math::ToDegrees(-(*rotationAngles)[1]);
    while (beta < -180.0) { beta += 360.0; } // [-180, 180)
    Double gamma = Elastos::Core::Math::ToDegrees((*rotationAngles)[2]);
    while (gamma < -90.0) { gamma += 360.0; } // [-90, 90)

    MaybeSendChange(alpha, beta, gamma);
}

AutoPtr<ISensorManager> DeviceOrientationService::GetSensorManager()
{
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String name;
    thread->GetName(&name);
    assert(CWebViewCore::THREAD_NAME.Equals(name));
    if (mSensorManager == NULL) {
        mContext->GetSystemService(IContext::SENSOR_SERVICE, (IInterface**)&mSensorManager);
    }
    return mSensorManager;
}

Boolean DeviceOrientationService::RegisterForAccelerometerSensor()
{
    AutoPtr< ArrayOf<ISensor*> > sensors;
    GetSensorManager()->GetSensorList(ISensor::TYPE_ACCELEROMETER, (ArrayOf<ISensor*>**)&sensors);
    if (!sensors || sensors->GetLength() == 0) {
        return FALSE;
    }
    // TODO: Consider handling multiple sensors.
    Boolean result = FALSE;
    GetSensorManager()->RegisterListenerEx3(
            this, (*sensors)[0], ISensorManager::SENSOR_DELAY_FASTEST, mHandler, &result);
    return result;
}

Boolean DeviceOrientationService::RegisterForMagneticFieldSensor()
{
    AutoPtr< ArrayOf<ISensor*> > sensors;
    GetSensorManager()->GetSensorList(ISensor::TYPE_MAGNETIC_FIELD, (ArrayOf<ISensor*>**)&sensors);
    if (!sensors || sensors->GetLength() == 0) {
        return FALSE;
    }
    // TODO: Consider handling multiple sensors.
    Boolean result = FALSE;
    GetSensorManager()->RegisterListenerEx3(
            this, (*sensors)[0], ISensorManager::SENSOR_DELAY_FASTEST, mHandler, &result);
    return result;
}

void DeviceOrientationService::UnregisterFromSensors()
{
    GetSensorManager()->UnregisterListenerEx3(this);
}

void DeviceOrientationService::MaybeSendChange(
    /* [in] */ Double alpha,
    /* [in] */ Double beta,
    /* [in] */ Double gamma)
{
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String name;
    thread->GetName(&name);
    assert(CWebViewCore::THREAD_NAME.Equals(name));
    if (mAlpha == NULL || mBeta == NULL || mGamma == NULL
            || Elastos::Core::Math::Abs(alpha - mAlpha) > DELTA_DEGRESS
            || Elastos::Core::Math::Abs(beta - mBeta) > DELTA_DEGRESS
            || Elastos::Core::Math::Abs(gamma - mGamma) > DELTA_DEGRESS) {
        mAlpha = alpha;
        mBeta = beta;
        mGamma = gamma;
        AutoPtr<IDouble> iAlpha, iBeta, iGamma;
        CDouble::New(mAlpha, (IDouble**)&iAlpha);
        CDouble::New(mBeta, (IDouble**)&iBeta);
        CDouble::New(mGamma, (IDouble**)&iGamma);
        mManager->OnOrientationChange(iAlpha, iBeta, iGamma);
        // Now that we have successfully sent some data, reset whether we've sent an error.
        mHaveSentErrorEvent = FALSE;
    }
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
