
#ifndef __ELASTOS_DROID_WEBKIT_DEVICEMOTIONSERVICE_H__
#define __ELASTOS_DROID_WEBKIT_DEVICEMOTIONSERVICE_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Core::IRunnable;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Hardware::ISensor;
using Elastos::Droid::Hardware::ISensorEvent;
using Elastos::Droid::Hardware::ISensorManager;
using Elastos::Droid::Hardware::ISensorEventListener;
using Elastos::Droid::Os::IHandler;

struct ElaDeviceMotionClientImplCallback
{
    void (*start)(IInterface*);
    void (*stop)(IInterface*);
    void (*suspend)(IInterface*);
    void (*resume)(IInterface*);
};

namespace Elastos {
namespace Droid {
namespace Webkit {

class DeviceMotionService;
class DeviceMotionAndOrientationManager;

class DeviceMotionService
    : public Object
    , public ISensorEventListener
{
private:
    class ErrorEventRunnable
        : public Object
        , public IRunnable
    {
    public:
        ErrorEventRunnable(
            /* [in] */ DeviceMotionService* owner);

        CAR_INTERFACE_DECL();

        CARAPI Run();

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        DeviceMotionService* mOwner;
    };

    class InnerRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerRunnable(
            /* [in] */ DeviceMotionService* owner);

        CAR_INTERFACE_DECL();

        CARAPI Run();

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        DeviceMotionService* mOwner;
    };

public:
    DeviceMotionService(
        /* [in] */ DeviceMotionAndOrientationManager* manager,
        /* [in] */ IContext* context);

    CAR_INTERFACE_DECL();

    virtual CARAPI_(void) Start();

    virtual CARAPI_(void) Stop();

    virtual CARAPI_(void) Suspend();

    virtual CARAPI_(void) Resume();

    /**
     * SensorEventListener implementation.
     * Callbacks happen on the thread on which we registered - the WebCore thread.
     */
    CARAPI OnSensorChanged(
        /* [in] */ ISensorEvent* event);

    CARAPI OnAccuracyChanged(
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 accuracy);

    static void  Start(
        /* [in] */ IInterface* obj);

    static void Stop(
        /* [in] */ IInterface* obj);

    static void Suspend(
        /* [in] */ IInterface* obj);

    static void Resume(
        /* [in] */ IInterface* obj);

    CARAPI ToString(
        /* [out] */ String* info);

private:
    CARAPI_(void) SendErrorEvent();

    CARAPI_(void) CreateHandler();

    CARAPI_(void) StartSendingUpdates();

    CARAPI_(void) StopSendingUpdates();

    CARAPI_(void) RegisterForSensor();

    CARAPI_(AutoPtr<ISensorManager>) GetSensorManager();

    CARAPI_(Boolean) RegisterForAccelerometerSensor();

    CARAPI_(void) UnregisterFromSensor();

private:
    AutoPtr<DeviceMotionAndOrientationManager> mManager;
    Boolean mIsRunning;
    AutoPtr<IHandler> mHandler;
    AutoPtr<ISensorManager> mSensorManager;
    AutoPtr<IContext> mContext;
    Boolean mHaveSentErrorEvent;
    AutoPtr<IRunnable> mUpdateRunnable;
    AutoPtr<ArrayOf<Float> > mLastAcceleration;

    static const Int32 INTERVAL_MILLIS = 100;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_DEVICEMOTIONSERVICE_H__
