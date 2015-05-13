#ifndef __DISPLAYPOWERCONTROLLER_H__
#define __DISPLAYPOWERCONTROLLER_H__

#include "ext/frameworkdef.h"
#include "Elastos.Droid.Server_server.h"
#include "LightsService.h"
#include "TwilightService.h"
#include "power/Notifier.h"
#include "power/DisplayPowerRequest.h"
#include "power/DisplayPowerState.h"
#include "power/RampAnimator.h"
#include "display/CDisplayManagerService.h"
#include <elastos/Mutex.h>

using Elastos::Core::IRunnable;
using Elastos::Core::Threading::Mutex;
using Elastos::IO::IPrintWriter;
using Elastos::Droid::Animation::IAnimator;
using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::Animation::IAnimatorListener;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Hardware::ISensor;
using Elastos::Droid::Hardware::ISensorEventListener;
using Elastos::Droid::Hardware::ISensorEvent;
using Elastos::Droid::Hardware::ISensorManager;
using Elastos::Droid::Hardware::Display::IDisplayManager;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::View::IDisplay;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

class Notifier;
class DisplayPowerRequest;
class DisplayPowerState;

/**
 * Controls the power state of the display.
 *
 * Handles the proximity sensor, light sensor, and animations between states
 * including the screen off animation.
 *
 * This component acts independently of the rest of the power manager service.
 * In particular, it does not share any state and it only communicates
 * via asynchronous callbacks to inform the power manager that something has
 * changed.
 *
 * Everything this class does internally is serialized on its handler although
 * it may be accessed by other threads from the outside.
 *
 * Note that the power manager service guarantees that it will hold a suspend
 * blocker as long as the display is not ready.  So most of the work done here
 * does not need to worry about holding a suspend blocker unless it happens
 * independently of the display ready signal.
 *
 * For debugging, you can make the electron beam and brightness animations run
 * slower by changing the "animator duration scale" option in Development Settings.
 */
class DisplayPowerController : public ElRefBase
{
public:
    /**
     * Asynchronous callbacks from the power controller to the power manager service.
     */
    interface IDisplayPowerControllerCallbacks : public IInterface
    {
    public:
        virtual CARAPI_(void) OnStateChanged() = 0;

        virtual CARAPI_(void) OnProximityPositive() = 0;

        virtual CARAPI_(void) OnProximityNegative() = 0;
    };

private:
    class DisplayControllerHandler : public HandlerBase
    {
    public:
        DisplayControllerHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ DisplayPowerController* service)
            : HandlerBase(looper, NULL, TRUE)
            , mHost(service)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        DisplayPowerController* mHost;
    };

    class MyAnimatorListener
        : public ElRefBase
        , public IAnimatorListener
    {
    public:
        MyAnimatorListener(
            /* [in] */ DisplayPowerController* host);

        CAR_INTERFACE_DECL();

        CARAPI OnAnimationStart(
            /* [in] */ IAnimator* animation);

        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);

        CARAPI OnAnimationRepeat(
            /* [in] */ IAnimator* animation);

        CARAPI OnAnimationCancel(
            /* [in] */ IAnimator* animation);

    private:
        DisplayPowerController* mHost;
    };

    class CleanListener
        : public ElRefBase
        , public IRunnable
    {
    public:
        CleanListener(
            /* [in] */ DisplayPowerController* host);

        CAR_INTERFACE_DECL();

        CARAPI Run();

    private:
        DisplayPowerController* mHost;
    };

    class OnStateChangedRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        OnStateChangedRunnable(
            /* [in] */ DisplayPowerController* host);

        CAR_INTERFACE_DECL();

        CARAPI Run();

    private:
        DisplayPowerController* mHost;
    };

    class OnProximityPositiveRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        OnProximityPositiveRunnable(
            /* [in] */ DisplayPowerController* host);

        CAR_INTERFACE_DECL();

        CARAPI Run();

    private:
        DisplayPowerController* mHost;
    };

    class OnProximityNegativeRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        OnProximityNegativeRunnable(
            /* [in] */ DisplayPowerController* host);

        CAR_INTERFACE_DECL();

        CARAPI Run();

    private:
        DisplayPowerController* mHost;
    };

    class ProximitySensorListener
        : public ElRefBase
        , public ISensorEventListener
    {
    public:
        ProximitySensorListener(
            /* [in] */ DisplayPowerController* host);

        CAR_INTERFACE_DECL();

        CARAPI OnSensorChanged(
            /* [in] */ ISensorEvent* event);

        CARAPI OnAccuracyChanged(
            /* [in] */ ISensor* sensor,
            /* [in] */ Int32 accuracy);

    private:
        DisplayPowerController* mHost;
    };

    class LightSensorListener
        : public ElRefBase
        , public ISensorEventListener
    {
    public:
        LightSensorListener(
            /* [in] */ DisplayPowerController* host);

        CAR_INTERFACE_DECL();

        CARAPI OnSensorChanged(
            /* [in] */ ISensorEvent* event);

        CARAPI OnAccuracyChanged(
            /* [in] */ ISensor* sensor,
            /* [in] */ Int32 accuracy);

    private:
        DisplayPowerController* mHost;
    };

    class MyTwilightListener
        : public ElRefBase
        , public TwilightService::ITwilightListener
    {
    public:
        CAR_INTERFACE_DECL()

        MyTwilightListener(
            /* [in] */ DisplayPowerController* host);

        CARAPI OnTwilightStateChanged();

    private:
        DisplayPowerController* mHost;
    };

public:
    /**
     * Creates the display power controller.
     */
    DisplayPowerController(
        /* [in] */ ILooper* looper,
        /* [in] */ IContext* context,
        /* [in] */ Notifier* notifier,
        /* [in] */ LightsService* lights,
        /* [in] */ TwilightService* twilight,
        /* [in] */ ISensorManager* sensorManager,
        /* [in] */ CDisplayManagerService* displayManager,
        /* [in] */ IDisplayBlanker* displayBlanker,
        /* [in] */ IDisplayPowerControllerCallbacks* callbacks,
        /* [in] */ IHandler* callbackHandler);

    /**
     * Returns true if the proximity sensor screen-off function is available.
     */
    CARAPI_(Boolean) IsProximitySensorAvailable();

    /**
     * Requests a new power state.
     * The controller makes a copy of the provided object and then
     * begins adjusting the power state to match what was requested.
     *
     * @param request The requested power state.
     * @param waitForNegativeProximity If true, issues a request to wait for
     * negative proximity before turning the screen back on, assuming the screen
     * was turned off by the proximity sensor.
     * @return True if display is ready, false if there are important changes that must
     * be made asynchronously (such as turning the screen on), in which case the caller
     * should grab a wake lock, watch for {@link Callbacks#onStateChanged()} then try
     * the request again later until the state converges.
     */
    CARAPI_(Boolean) RequestPowerState(
        /* [in] */ DisplayPowerRequest* request,
        /* [in] */ Boolean waitForNegativeProximity);

private:
    // static static Spline createAutoBrightnessSpline(int[] lux, int[] brightness);

    CARAPI_(void) SendUpdatePowerState();

    CARAPI_(void) SendUpdatePowerStateLocked();

    CARAPI_(void) Initialize();

    CARAPI_(void) UpdatePowerState();

    CARAPI_(void) BlockScreenOn();

    CARAPI_(void) UnblockScreenOn();

    CARAPI_(void) SetScreenOn(
        /* [in] */ Boolean on);

    CARAPI_(Int32) ClampScreenBrightness(
        /* [in] */ Int32 value);

    static CARAPI_(Int32) ClampAbsoluteBrightness(
        /* [in] */ Int32 value);

    static CARAPI_(Int32) Clamp(
        /* [in] */ Int32 value,
        /* [in] */ Int32 min,
        /* [in] */ Int32 max);

    static CARAPI_(Float) NormalizeAbsoluteBrightness(
        /* [in] */ Int32 value);

    CARAPI_(void) AnimateScreenBrightness(
        /* [in] */ Int32 target,
        /* [in] */ Int32 rate);

    CARAPI_(void) SetProximitySensorEnabled(
        /* [in] */ Boolean enable);

    CARAPI_(void) HandleProximitySensorEvent(
        /* [in] */ Int64 time,
        /* [in] */ Boolean positive);

    CARAPI_(void) DebounceProximitySensor();

    CARAPI_(void) SetLightSensorEnabled(
        /* [in] */ Boolean enable,
        /* [in] */ Boolean updateAutoBrightness);

    CARAPI_(void) HandleLightSensorEvent(
        /* [in] */ Int64 time,
        /* [in] */ Float lux);

    CARAPI_(void) ApplyLightSensorMeasurement(
        /* [in] */ Int64 time,
        /* [in] */ Float lux);

    CARAPI_(void) UpdateAmbientLux(
        /* [in] */ Int64 time);

    CARAPI_(void) DebounceLightSensor();

    CARAPI_(void) UpdateAutoBrightness(
        /* [in] */ Boolean sendUpdate);

    static CARAPI_(Float) GetTwilightGamma(
        /* [in] */ Int64 now,
        /* [in] */ Int64 lastSunset,
        /* [in] */ Int64 nextSunrise);

    static CARAPI_(Float) Lerp(
        /* [in] */ Int64 x,
        /* [in] */ Int64 y,
        /* [in] */ Int64 alpha);

    CARAPI_(void) SendOnStateChanged();

    CARAPI_(void) SendOnProximityPositive();

    CARAPI_(void) SendOnProximityNegative();

    // CARAPI_(void) Dump(
    //     /* [in] */ const IPrintWriter* pw);

    // CARAPI_(void) DumpLocal(
    //     /* [in] */ IPrintWriter* pw);

    static CARAPI_(String) ProximityToString(
        /* [in] */ Int32 state);

    static CARAPI_(Boolean) WantScreenOn(
        /* [in] */ Int32 state);

private:
    static const String TAG;

    static Boolean DEBUG;
    static const Boolean DEBUG_PRETEND_PROXIMITY_SENSOR_ABSENT;
    static const Boolean DEBUG_PRETEND_LIGHT_SENSOR_ABSENT;

    // If true, uses the electron beam on animation.
    // We might want to turn this off if we cannot get a guarantee that the screen
    // actually turns on and starts showing new content after the call to set the
    // screen state returns.  Playing the animation can also be somewhat slow.
    static const Boolean USE_ELECTRON_BEAM_ON_ANIMATION;

    // If true, enables the use of the screen auto-brightness adjustment setting.
    static const Boolean USE_SCREEN_AUTO_BRIGHTNESS_ADJUSTMENT;

    // The maximum range of gamma adjustment possible using the screen
    // auto-brightness adjustment setting.
    static const Float SCREEN_AUTO_BRIGHTNESS_ADJUSTMENT_MAX_GAMMA;

    // The minimum reduction in brightness when dimmed.
    static const Int32 SCREEN_DIM_MINIMUM_REDUCTION;

    // If true, enables the use of the current time as an auto-brightness adjustment.
    // The basic idea here is to expand the dynamic range of auto-brightness
    // when it is especially dark outside.  The light sensor tends to perform
    // poorly at low light levels so we compensate for it by making an
    // assumption about the environment.
    static const Boolean USE_TWILIGHT_ADJUSTMENT;

    // Specifies the maximum magnitude of the time of day adjustment.
    static const Float TWILIGHT_ADJUSTMENT_MAX_GAMMA;

    // The amount of time after or before sunrise over which to start adjusting
    // the gamma.  We want the change to happen gradually so that it is below the
    // threshold of perceptibility and so that the adjustment has maximum effect
    // well after dusk.
    static const Int64 TWILIGHT_ADJUSTMENT_TIME;

    static const Int32 ELECTRON_BEAM_ON_ANIMATION_DURATION_MILLIS;
    static const Int32 ELECTRON_BEAM_OFF_ANIMATION_DURATION_MILLIS;

    static const Int32 MSG_UPDATE_POWER_STATE;// = 1;
    static const Int32 MSG_PROXIMITY_SENSOR_DEBOUNCED;// = 2;
    static const Int32 MSG_LIGHT_SENSOR_DEBOUNCED;// = 3;

    static const Int32 PROXIMITY_UNKNOWN;
    static const Int32 PROXIMITY_NEGATIVE;
    static const Int32 PROXIMITY_POSITIVE;

    // Proximity sensor debounce delay in milliseconds for positive or negative transitions.
    static const Int32 PROXIMITY_SENSOR_POSITIVE_DEBOUNCE_DELAY;
    static const Int32 PROXIMITY_SENSOR_NEGATIVE_DEBOUNCE_DELAY;

    // Trigger proximity if distance is less than 5 cm.
    static const Float TYPICAL_PROXIMITY_THRESHOLD;

    // Light sensor event rate in milliseconds.
    static const Int32 LIGHT_SENSOR_RATE_MILLIS;

    // A rate for generating synthetic light sensor events in the case where the light
    // sensor hasn't reported any new data in a while and we need it to update the
    // debounce filter.  We only synthesize light sensor measurements when needed.
    static const Int32 SYNTHETIC_LIGHT_SENSOR_RATE_MILLIS;

    // Brightness animation ramp rate in brightness units per second.
    static const Int32 BRIGHTNESS_RAMP_RATE_FAST;
    static const Int32 BRIGHTNESS_RAMP_RATE_SLOW;

    // IIR filter time constants in milliseconds for computing two moving averages of
    // the light samples.  One is a long-term average and the other is a short-term average.
    // We can use these filters to assess trends in ambient brightness.
    // The short term average gives us a filtered but relatively low latency measurement.
    // The long term average informs us about the overall trend.
    static const Int64 SHORT_TERM_AVERAGE_LIGHT_TIME_CONSTANT;
    static const Int64 LONG_TERM_AVERAGE_LIGHT_TIME_CONSTANT;

    // Stability requirements in milliseconds for accepting a new brightness
    // level.  This is used for debouncing the light sensor.  Different constants
    // are used to debounce the light sensor when adapting to brighter or darker environments.
    // This parameter controls how quickly brightness changes occur in response to
    // an observed change in light level that exceeds the hysteresis threshold.
    static const Int64 BRIGHTENING_LIGHT_DEBOUNCE;
    static const Int64 DARKENING_LIGHT_DEBOUNCE;

    // Hysteresis constraints for brightening or darkening.
    // The recent lux must have changed by at least this fraction relative to the
    // current ambient lux before a change will be considered.
    static const Float BRIGHTENING_LIGHT_HYSTERESIS;
    static const Float DARKENING_LIGHT_HYSTERESIS;

    Mutex mLock;

    // Notifier for sending asynchronous notifications.
    AutoPtr<Notifier> mNotifier;

    // The display blanker.
    AutoPtr<IDisplayBlanker> mDisplayBlanker;

    // Our handler.
    AutoPtr<IHandler> mHandler;

    // Asynchronous callbacks into the power manager service.
    // Only invoked from the handler thread while no locks are held.
    AutoPtr<IDisplayPowerControllerCallbacks> mCallbacks;
    AutoPtr<IHandler> mCallbackHandler;

    // The lights service.
    AutoPtr<LightsService> mLights;

    // The twilight service.
    AutoPtr<TwilightService> mTwilight;

    // The display manager.
    AutoPtr<CDisplayManagerService> mDisplayManager;

    // The sensor manager.
    AutoPtr<ISensorManager> mSensorManager;

    // The proximity sensor, or null if not available or needed.
    AutoPtr<ISensor> mProximitySensor;

    // The light sensor, or null if not available or needed.
    AutoPtr<ISensor> mLightSensor;

    // The dim screen brightness.
    Int32 mScreenBrightnessDimConfig;

    // The minimum allowed brightness.
    Int32 mScreenBrightnessRangeMinimum;

    // The maximum allowed brightness.
    Int32 mScreenBrightnessRangeMaximum;

    // True if auto-brightness should be used.
    Boolean mUseSoftwareAutoBrightnessConfig;

    // The auto-brightness spline adjustment.
    // The brightness values have been scaled to a range of 0..1.
    // Spline mScreenAutoBrightnessSpline;

    // Amount of time to delay auto-brightness after screen on while waiting for
    // the light sensor to warm-up in milliseconds.
    // May be 0 if no warm-up is required.
    Int32 mLightSensorWarmUpTimeConfig;

    // True if we should fade the screen while turning it off, false if we should play
    // a stylish electron beam animation instead.
    Boolean mElectronBeamFadesConfig;

    // The pending power request.
    // Initially null until the first call to requestPowerState.
    // Guarded by mLock.
    AutoPtr<DisplayPowerRequest> mPendingRequestLocked;

    // True if a request has been made to wait for the proximity sensor to go negative.
    // Guarded by mLock.
    Boolean mPendingWaitForNegativeProximityLocked;

    // True if the pending power request or wait for negative proximity flag
    // has been changed since the last update occurred.
    // Guarded by mLock.
    Boolean mPendingRequestChangedLocked;

    // Set to true when the important parts of the pending power request have been applied.
    // The important parts are mainly the screen state.  Brightness changes may occur
    // concurrently.
    // Guarded by mLock.
    Boolean mDisplayReadyLocked;

    // Set to true if a power state update is required.
    // Guarded by mLock.
    Boolean mPendingUpdatePowerStateLocked;

    /* The following state must only be accessed by the handler thread. */

    // The currently requested power state.
    // The power controller will progressively update its internal state to match
    // the requested power state.  Initially null until the first update.
    AutoPtr<DisplayPowerRequest> mPowerRequest;

    // The current power state.
    // Must only be accessed on the handler thread.
    AutoPtr<DisplayPowerState> mPowerState;

    // True if the device should wait for negative proximity sensor before
    // waking up the screen.  This is set to false as soon as a negative
    // proximity sensor measurement is observed or when the device is forced to
    // go to sleep by the user.  While true, the screen remains off.
    Boolean mWaitingForNegativeProximity;

    // The actual proximity sensor threshold value.
    Float mProximityThreshold;

    // Set to true if the proximity sensor listener has been registered
    // with the sensor manager.
    Boolean mProximitySensorEnabled;

    // The debounced proximity sensor state.
    Int32 mProximity;

    // The raw non-debounced proximity sensor state.
    Int32 mPendingProximity;
    Int64 mPendingProximityDebounceTime;

    // True if the screen was turned off because of the proximity sensor.
    // When the screen turns on again, we report user activity to the power manager.
    Boolean mScreenOffBecauseOfProximity;

    // True if the screen on is being blocked.
    Boolean mScreenOnWasBlocked;

    // The elapsed real time when the screen on was blocked.
    Int64 mScreenOnBlockStartRealTime;

    // Set to true if the light sensor is enabled.
    Boolean mLightSensorEnabled;

    // The time when the light sensor was enabled.
    Int64 mLightSensorEnableTime;

    // The currently accepted nominal ambient light level.
    Float mAmbientLux;

    // True if mAmbientLux holds a valid value.
    Boolean mAmbientLuxValid;

    // The most recent light sample.
    Float mLastObservedLux;

    // The time of the most light recent sample.
    Int64 mLastObservedLuxTime;

    // The number of light samples collected since the light sensor was enabled.
    Int32 mRecentLightSamples;

    // The long-term and short-term filtered light measurements.
    Float mRecentShortTermAverageLux;
    Float mRecentLongTermAverageLux;

    // The direction in which the average lux is moving relative to the current ambient lux.
    //    0 if not changing or within hysteresis threshold.
    //    1 if brightening beyond hysteresis threshold.
    //   -1 if darkening beyond hysteresis threshold.
    Int32 mDebounceLuxDirection;

    // The time when the average lux last changed direction.
    Int64 mDebounceLuxTime;

    // The screen brightness level that has been chosen by the auto-brightness
    // algorithm.  The actual brightness should ramp towards this value.
    // We preserve this value even when we stop using the light sensor so
    // that we can quickly revert to the previous auto-brightness level
    // while the light sensor warms up.
    // Use -1 if there is no current auto-brightness value available.
    Int32 mScreenAutoBrightness;

    // The last screen auto-brightness gamma.  (For printing in dump() only.)
    Float mLastScreenAutoBrightnessGamma;

    // True if the screen auto-brightness value is actually being used to
    // set the display brightness.
    Boolean mUsingScreenAutoBrightness;

    // Animators.
    AutoPtr<IObjectAnimator> mElectronBeamOnAnimator;
    AutoPtr<IObjectAnimator> mElectronBeamOffAnimator;
    RampAnimator<AutoPtr<DisplayPowerState> >* mScreenBrightnessRampAnimator;

    // Twilight changed.  We might recalculate auto-brightness values.
    Boolean mTwilightChanged;

    AutoPtr<IAnimatorListener> mAnimatorListener;
    AutoPtr<IRunnable> mCleanListener;
    AutoPtr<IRunnable> mOnStateChangedRunnable;
    AutoPtr<IRunnable> mOnProximityPositiveRunnable;
    AutoPtr<IRunnable> mOnProximityNegativeRunnable;

    AutoPtr<ISensorEventListener> mProximitySensorListener;
    AutoPtr<ISensorEventListener> mLightSensorListener;
    AutoPtr<TwilightService::ITwilightListener> mTwilightListener;

    friend class MyAnimatorListener;
    friend class OnStateChangedRunnable;
    friend class CleanListener;
    friend class OnProximityPositiveRunnable;
    friend class OnProximityNegativeRunnable;
    friend class ProximitySensorListener;
    friend class LightSensorListener;
    friend class MyTwilightListener;
};

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __DISPLAYPOWERCONTROLLER_H__
