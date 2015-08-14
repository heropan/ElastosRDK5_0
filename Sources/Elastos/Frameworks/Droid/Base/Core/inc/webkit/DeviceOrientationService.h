
#ifndef __ELASTOS_DROID_WEBKIT_DEVICEORIENTATIONSERVICE_H__
#define __ELASTOS_DROID_WEBKIT_DEVICEORIENTATIONSERVICE_H__

#include "ext/frameworkext.h"
#include "webkit/DeviceMotionAndOrientationManager.h"

using Elastos::Core::IRunnable;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Hardware::ISensor;
using Elastos::Droid::Hardware::ISensorEvent;
using Elastos::Droid::Hardware::ISensorEventListener;
using Elastos::Droid::Hardware::ISensorManager;

struct ElaDeviceOrientationClientImplCallback
{
    void (*start)(IInterface*);
    void (*stop)(IInterface*);
    void (*suspend)(IInterface*);
    void (*resume)(IInterface*);
};

namespace Elastos {
namespace Droid {
namespace Webkit {

class DeviceMotionAndOrientationManager;

class DeviceOrientationService
    : public ElRefBase
    , public ISensorEventListener
{
private:
    class InnerRunnable
        : public ElLightRefBase
        , public IRunnable
    {
    public:
        InnerRunnable(
            /* [in] */ DeviceOrientationService* owner);

        CAR_INTERFACE_DECL();

        CARAPI Run();

    private:
        DeviceOrientationService* mOwner;
    };

public:
    DeviceOrientationService(
        /* [in] */ DeviceMotionAndOrientationManager* manager,
        /* [in] */ IContext* context);

    CAR_INTERFACE_DECL();

    CARAPI_(void) Start();

    CARAPI_(void) Stop();

    CARAPI_(void) Suspend();

    CARAPI_(void) Resume();

    /**
     * SensorEventListener implementation.
     * Callbacks happen on the thread on which we registered - the WebCore thread.
     */
    CARAPI OnSensorChanged(
        /* [in] */ ISensorEvent* event);

    CARAPI OnAccuracyChanged(
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 accuracy);

    static void Start(
        /* [in] */ IInterface* obj);

    static void Stop(
        /* [in] */ IInterface* obj);

    static void Suspend(
        /* [in] */ IInterface* obj);

    static void Resume(
        /* [in] */ IInterface* obj);

private:
    CARAPI_(void) SendErrorEvent();

    CARAPI_(void) RegisterForSensors();

    CARAPI_(void) GetOrientationUsingGetRotationMatrix();

    CARAPI_(AutoPtr<ISensorManager>) GetSensorManager();

    CARAPI_(Boolean) RegisterForAccelerometerSensor();

    CARAPI_(Boolean) RegisterForMagneticFieldSensor();

    CARAPI_(void) UnregisterFromSensors();

    CARAPI_(void) MaybeSendChange(
        /* [in] */ Double alpha,
        /* [in] */ Double beta,
        /* [in] */ Double gamma);

private:
    // The gravity vector expressed in the body frame.
    AutoPtr< ArrayOf<Float> > mGravityVector;
    // The geomagnetic vector expressed in the body frame.
    AutoPtr< ArrayOf<Float> > mMagneticFieldVector;

    AutoPtr<DeviceMotionAndOrientationManager> mManager;
    Boolean mIsRunning;
    AutoPtr<IHandler> mHandler;
    AutoPtr<ISensorManager> mSensorManager;
    AutoPtr<IContext> mContext;
    Double mAlpha;
    Double mBeta;
    Double mGamma;
    Boolean mHaveSentErrorEvent;

    static const Double DELTA_DEGRESS = 1.0;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_DEVICEORIENTATIONSERVICE_H__
