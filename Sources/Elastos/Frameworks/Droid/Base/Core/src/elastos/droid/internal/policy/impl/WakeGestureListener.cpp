
#include "elastos/droid/internal/policy/impl/WakeGestureListener.h"

using Elastos::Droid::Hardware::EIID_ITriggerEventListener;
using Elastos::Core::EIID_IRunnable;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//            WakeGestureListener::InnerTriggerEventListener
//=====================================================================
CAR_INTERFACE_IMPL(WakeGestureListener::InnerTriggerEventListener, Object, ITriggerEventListener)

WakeGestureListener::InnerTriggerEventListener::InnerTriggerEventListener(
    /* [in] */ WakeGestureListener* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WakeGestureListener::InnerTriggerEventListener::OnTrigger(
    /* [in] */ ITriggerEvent* event)
{
    VALIDATE_NOT_NULL(event);
    // ==================before translated======================
    // synchronized (mLock) {
    //     mTriggerRequested = false;
    //     mHandler.post(mWakeUpRunnable);
    // }
    assert(0);
    return NOERROR;
}

//=====================================================================
//                  WakeGestureListener::InnerRunnable
//=====================================================================
CAR_INTERFACE_IMPL(WakeGestureListener::InnerRunnable, Object, IRunnable)

WakeGestureListener::InnerRunnable::InnerRunnable(
    /* [in] */ WakeGestureListener* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WakeGestureListener::InnerRunnable::Run()
{
    // ==================before translated======================
    // onWakeUp();
    assert(0);
    return NOERROR;
}

//=====================================================================
//                         WakeGestureListener
//=====================================================================
CAR_INTERFACE_IMPL(WakeGestureListener, Object, IWakeGestureListener)

const String WakeGestureListener::TAG("WakeGestureListener");

WakeGestureListener::WakeGestureListener()
{
}

ECode WakeGestureListener::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler)
{
    // ==================before translated======================
    // mSensorManager = (SensorManager)context.getSystemService(Context.SENSOR_SERVICE);
    // mHandler = handler;
    //
    // mSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_WAKE_GESTURE);
    return NOERROR;
}

ECode WakeGestureListener::IsSupported(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // synchronized (mLock) {
    //     return mSensor != null;
    // }
    assert(0);
    return NOERROR;
}

ECode WakeGestureListener::RequestWakeUpTrigger()
{
    // ==================before translated======================
    // synchronized (mLock) {
    //     if (mSensor != null && !mTriggerRequested) {
    //         mTriggerRequested = true;
    //         mSensorManager.requestTriggerSensor(mListener, mSensor);
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode WakeGestureListener::CancelWakeUpTrigger()
{
    // ==================before translated======================
    // synchronized (mLock) {
    //     if (mSensor != null && mTriggerRequested) {
    //         mTriggerRequested = false;
    //         mSensorManager.cancelTriggerSensor(mListener, mSensor);
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode WakeGestureListener::Dump(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& prefix)
{
    VALIDATE_NOT_NULL(pw);
    // ==================before translated======================
    // synchronized (mLock) {
    //     pw.println(prefix + TAG);
    //     prefix += "  ";
    //     pw.println(prefix + "mTriggerRequested=" + mTriggerRequested);
    //     pw.println(prefix + "mSensor=" + mSensor);
    // }
    assert(0);
    return NOERROR;
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos


