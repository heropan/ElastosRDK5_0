
#include "elastos/droid/server/display/DisplayPowerController.h"


using Elastos::Droid::Server::LocalServices;
using Elastos::Droid::Server::Am::BatteryStatsService;
using Elastos::Droid::Server::Lights::LightsManager;

using Elastos::Droid::Internal::App::IBatteryStats;
using Elastos::Droid::Animation::IAnimator;
using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Hardware::ISensor;
using Elastos::Droid::Hardware::ISensorEvent;
using Elastos::Droid::Hardware::ISensorEventListener;
using Elastos::Droid::Hardware::ISensorManager;
using Elastos::Droid::Hardware::Display::IDisplayPowerCallbacks;
using Elastos::Droid::Hardware::Display::IDisplayPowerRequest;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::IPowerManager;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Utility::MathUtils;
using Elastos::Droid::Utility::ISpline;
using Elastos::Droid::Utility::TimeUtils;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::IWindowManagerPolicy;

using Elastos::IO::IPrintWriter;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

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
 * blocker as Int64 as the display is not ready.  So most of the work done here
 * does not need to worry about holding a suspend blocker unless it happens
 * independently of the display ready signal.
   *
 * For debugging, you can make the color fade and brightness animations run
 * slower by changing the "animator duration scale" option in Development Settings.
 */
class DisplayPowerController
    : public Object
    , public IAutomaticBrightnessControllerCallbacks
{
private:
    class DisplayControllerHandler
        : public Handler
    {
        DisplayControllerHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ DisplayPowerController* host)
            : Handler(looper, NULL, TRUE /*async*/);
        {
        }

        //@Override
        CARAPI HandleMessage(
            /* [in] */ IMessage* msg)
        {
            switch (msg.what) {
                case MSG_UPDATE_POWER_STATE:
                    UpdatePowerState();
                    break;

                case MSG_PROXIMITY_SENSOR_DEBOUNCED:
                    DebounceProximitySensor();
                    break;

                case MSG_SCREEN_ON_UNBLOCKED:
                    if (mPendingScreenOnUnblocker == msg.obj) {
                        UnBlockScreenOn();
                        UpdatePowerState();
                    }
                    break;
            }
        }
    private:
        DisplayPowerController* mHost;
    };

    class SensorEventListener
        : public Object
        , public ISensorEventListener
    {
    public:
        CAR_INTERFACE_DECL()

        SensorEventListener(
            /* [in] */ DisplayPowerController* host);

        //@Override
        CARAPI OnSensorChanged(
            /* [in] */ ISensorEvent* event)
        {
            if (mProximitySensorEnabled) {
                final Int64 time = SystemClock.uptimeMillis();
                final Float distance = event.values[0];
                Boolean positive = distance >= 0.0f && distance < mProximityThreshold;
                HandleProximitySensorEvent(time, positive);
            }
        }

        //@Override
        CARAPI OnAccuracyChanged(
            /* [in] */ ISensor* sensor,
            /* [in] */ Int32 accuracy)
        {
            // Not used.
        }
    private:
        DisplayPowerController* mHost;
    };

    class ScreenOnUnblocker
        : public Object
        , public IScreenOnListener
    {
    public:
        CAR_INTERFACE_DECL()

        ScreenOnUnblocker(
            /* [in] */ DisplayPowerController* host);

        //@Override
        CARAPI OnScreenOn()
        {
            Message msg = mHandler.obtainMessage(MSG_SCREEN_ON_UNBLOCKED, this);
            msg.setAsynchronous(TRUE);
            mHandler.sendMessage(msg);
        }
    };

    class AnimatorListener
        : public Object
        , public IAnimatorListener
    {
    public:
        CAR_INTERFACE_DECL()

        AnimatorListener(
            /* [in] */ DisplayPowerController* host);

        //@Override
        CARAPI OnAnimationStart(
            /* [in] */ IAnimator* animation)
        {
        }

        //@Override
        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation)
        {
            SendUpdatePowerState();
        }
        //@Override
        CARAPI OnAnimationRepeat(
            /* [in] */ IAnimator* animation)
        {
        }
        //@Override
        CARAPI OnAnimationCancel(
            /* [in] */ IAnimator* animation)
        {
        }
    private:
        DisplayPowerController* mHost;
    };

    class RampAnimatorListener
        : public Object
        , public IRampAnimatorListener
    {
    public:
        CAR_INTERFACE_DECL()

        RampAnimatorListener(
            /* [in] */ DisplayPowerController* host);

        //@Override
        CARAPI OnAnimationEnd()
        {
            SendUpdatePowerState();
        }
    private:
        DisplayPowerController* mHost;
    };

    class CleanListener
        : public Runnable
    {
    public:
        CleanListener(
            /* [in] */ DisplayPowerController* host);

        //@Override
        CARAPI Run()
        {
            SendUpdatePowerState();
        }
    private:
        DisplayPowerController* mHost;
    };

    class OnStateChangedRunnable
        : public Runnable
    {
    public:
        OnStateChangedRunnable(
            /* [in] */ DisplayPowerController* host);

        //@Override
        CARAPI Run()
        {
            mCallbacks.onStateChanged();
            mCallbacks.releaseSuspendBlocker();
        }
    private:
        DisplayPowerController* mHost;
    };

    class OnProximityPositiveRunnable
        : public Runnable
    {
    public:
        OnProximityPositiveRunnable(
            /* [in] */ DisplayPowerController* host);

        //@Override
        CARAPI Run()
        {
            mCallbacks.onProximityPositive();
            mCallbacks.releaseSuspendBlocker();
        }
    private:
        DisplayPowerController* mHost;
    };

    class OnProximityNegativeRunnable
        : public Runnable
    {
    public:
        OnProximityNegativeRunnable(
            /* [in] */ DisplayPowerController* host);

        //@Override
        CARAPI Run()
        {
            mCallbacks.onProximityNegative();
            mCallbacks.releaseSuspendBlocker();
        }
    private:
        DisplayPowerController* mHost;
    };

public:

    /**
     * Creates the display power controller.
     */
    DisplayPowerController(
        /* [in] */ IContext* context,
        /* [in] */ DisplayPowerCallbacks* callbacks,
        /* [in] */ IHandler* handler,
        /* [in] */ ISensorManager* sensorManager,
        /* [in] */ IDisplayBlanker* blanker)
    {
        mHandler = new DisplayControllerHandler(handler.getLooper());
        mCallbacks = callbacks;

        mBatteryStats = BatteryStatsService.getService();
        mLights = LocalServices.getService(LightsManager.class);
        mSensorManager = sensorManager;
        mWindowManagerPolicy = LocalServices.getService(WindowManagerPolicy.class);
        mBlanker = blanker;
        mContext = context;

        final Resources resources = context.getResources();
        final Int32 screenBrightnessSettingMinimum = ClampAbsoluteBrightness(resources.getInteger(
                com.android.internal.R.integer.config_screenBrightnessSettingMinimum));

        mScreenBrightnessDozeConfig = ClampAbsoluteBrightness(resources.getInteger(
                com.android.internal.R.integer.config_screenBrightnessDoze));

        mScreenBrightnessDimConfig = ClampAbsoluteBrightness(resources.getInteger(
                com.android.internal.R.integer.config_screenBrightnessDim));

        mScreenBrightnessDarkConfig = ClampAbsoluteBrightness(resources.getInteger(
                com.android.internal.R.integer.config_screenBrightnessDark));
        if (mScreenBrightnessDarkConfig > mScreenBrightnessDimConfig) {
            Slog.w(TAG, "Expected config_screenBrightnessDark ("
                    + mScreenBrightnessDarkConfig + ") to be less than or equal to "
                    + "config_screenBrightnessDim (" + mScreenBrightnessDimConfig + ").");
        }
        if (mScreenBrightnessDarkConfig > mScreenBrightnessDimConfig) {
            Slog.w(TAG, "Expected config_screenBrightnessDark ("
                    + mScreenBrightnessDarkConfig + ") to be less than or equal to "
                    + "config_screenBrightnessSettingMinimum ("
                    + screenBrightnessSettingMinimum + ").");
        }

        Int32 screenBrightnessRangeMinimum = Math.min(Math.min(
                screenBrightnessSettingMinimum, mScreenBrightnessDimConfig),
                mScreenBrightnessDarkConfig);

        mScreenBrightnessRangeMaximum = PowerManager.BRIGHTNESS_ON;

        mUseSoftwareAutoBrightnessConfig = resources.getBoolean(
                com.android.internal.R.bool.config_automatic_brightness_available);
        if (mUseSoftwareAutoBrightnessConfig) {
            Int32[] lux = resources.getIntArray(
                    com.android.internal.R.array.config_autoBrightnessLevels);
            Int32[] screenBrightness = resources.getIntArray(
                    com.android.internal.R.array.config_autoBrightnessLcdBacklightValues);
            Int32 lightSensorWarmUpTimeConfig = resources.getInteger(
                    com.android.internal.R.integer.config_lightSensorWarmupTime);

            Spline screenAutoBrightnessSpline = CreateAutoBrightnessSpline(lux, screenBrightness);
            if (screenAutoBrightnessSpline == NULL) {
                Slog.e(TAG, "Error in config.xml.  config_autoBrightnessLcdBacklightValues "
                        + "(size " + screenBrightness.length + ") "
                        + "must be monotic and have exactly one more entry than "
                        + "config_autoBrightnessLevels (size " + lux.length + ") "
                        + "which must be strictly increasing.  "
                        + "Auto-brightness will be disabled.");
                mUseSoftwareAutoBrightnessConfig = FALSE;
            } else {
                Int32 bottom = ClampAbsoluteBrightness(screenBrightness[0]);
                if (mScreenBrightnessDarkConfig > bottom) {
                    Slog.w(TAG, "config_screenBrightnessDark (" + mScreenBrightnessDarkConfig
                            + ") should be less than or equal to the first value of "
                            + "config_autoBrightnessLcdBacklightValues ("
                            + bottom + ").");
                }
                if (bottom < screenBrightnessRangeMinimum) {
                    screenBrightnessRangeMinimum = bottom;
                }
                mAutomaticBrightnessController = new AutomaticBrightnessController(this,
                        handler.getLooper(), sensorManager, screenAutoBrightnessSpline,
                        lightSensorWarmUpTimeConfig, screenBrightnessRangeMinimum,
                        mScreenBrightnessRangeMaximum);
            }
        }

        mScreenBrightnessRangeMinimum = screenBrightnessRangeMinimum;

        mColorFadeFadesConfig = resources.getBoolean(
                com.android.internal.R.bool.config_animateScreenLights);

        if (!DEBUG_PRETEND_PROXIMITY_SENSOR_ABSENT) {
            mProximitySensor = mSensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY);
            if (mProximitySensor != NULL) {
                mProximityThreshold = Math.min(mProximitySensor.getMaximumRange(),
                        TYPICAL_PROXIMITY_THRESHOLD);
            }
        }

    }

    /**
     * Returns TRUE if the proximity sensor screen-off function is available.
     */
    Boolean IsProximitySensorAvailable()
    {
        return mProximitySensor != NULL;
    }

    /**
     * Requests a new power state.
     * The controller makes a copy of the provided object and then
     * begins adjusting the power state to match what was requested.
     *
     * @param request The requested power state.
     * @param waitForNegativeProximity If TRUE, issues a request to wait for
     * negative proximity before turning the screen back on, assuming the screen
     * was turned off by the proximity sensor.
     * @return True if display is ready, FALSE if there are important changes that must
     * be made asynchronously (such as turning the screen on), in which case the caller
     * should grab a wake lock, watch for {@link DisplayPowerCallbacks#onStateChanged()}
     * then try the request again later until the state converges.
     */
    Boolean RequestPowerState(
        /* [in] */ IDisplayPowerRequest* request,
        /* [in] */ Boolean waitForNegativeProximity)
    {
        if (DEBUG) {
            Slog.d(TAG, "RequestPowerState: "
                    + request + ", waitForNegativeProximity=" + waitForNegativeProximity);
        }

        synchronized (mLock) {
            Boolean changed = FALSE;

            if (waitForNegativeProximity
                    && !mPendingWaitForNegativeProximityLocked) {
                mPendingWaitForNegativeProximityLocked = TRUE;
                changed = TRUE;
            }

            if (mPendingRequestLocked == NULL) {
                mPendingRequestLocked = new DisplayPowerRequest(request);
                changed = TRUE;
            } else if (!mPendingRequestLocked.equals(request)) {
                mPendingRequestLocked.copyFrom(request);
                changed = TRUE;
            }

            if (changed) {
                mDisplayReadyLocked = FALSE;
            }

            if (changed && !mPendingRequestChangedLocked) {
                mPendingRequestChangedLocked = TRUE;
                SendUpdatePowerStateLocked();
            }

            return mDisplayReadyLocked;
        }
    }

    //@Override
    CARAPI UpdateBrightness()
    {
        SendUpdatePowerState();
    }

    void Dump(
        /* [in] */ IPrintWriter* pw)
    {
        synchronized (mLock) {
            pw.println();
            pw.println("Display Power Controller Locked State:");
            pw.println("  mDisplayReadyLocked=" + mDisplayReadyLocked);
            pw.println("  mPendingRequestLocked=" + mPendingRequestLocked);
            pw.println("  mPendingRequestChangedLocked=" + mPendingRequestChangedLocked);
            pw.println("  mPendingWaitForNegativeProximityLocked="
                    + mPendingWaitForNegativeProximityLocked);
            pw.println("  mPendingUpdatePowerStateLocked=" + mPendingUpdatePowerStateLocked);
        }

        pw.println();
        pw.println("Display Power Controller Configuration:");
        pw.println("  mScreenBrightnessDozeConfig=" + mScreenBrightnessDozeConfig);
        pw.println("  mScreenBrightnessDimConfig=" + mScreenBrightnessDimConfig);
        pw.println("  mScreenBrightnessDarkConfig=" + mScreenBrightnessDarkConfig);
        pw.println("  mScreenBrightnessRangeMinimum=" + mScreenBrightnessRangeMinimum);
        pw.println("  mScreenBrightnessRangeMaximum=" + mScreenBrightnessRangeMaximum);
        pw.println("  mUseSoftwareAutoBrightnessConfig=" + mUseSoftwareAutoBrightnessConfig);
        pw.println("  mColorFadeFadesConfig=" + mColorFadeFadesConfig);

        mHandler.runWithScissors(new Runnable() {
            //@Override
            public void run() {
                DumpLocal(pw);
            }
        }, 1000);
    }

private:
    void SendUpdatePowerState()
    {
        synchronized (mLock) {
            SendUpdatePowerStateLocked();
        }
    }

    void SendUpdatePowerStateLocked()
    {
        if (!mPendingUpdatePowerStateLocked) {
            mPendingUpdatePowerStateLocked = TRUE;
            Message msg = mHandler.obtainMessage(MSG_UPDATE_POWER_STATE);
            msg.setAsynchronous(TRUE);
            mHandler.sendMessage(msg);
        }
    }

    void Initialize()
    {
        // Initialize the power state object for the default display.
        // In the future, we might manage multiple displays independently.
        mPowerState = new DisplayPowerState(mBlanker,
                mLights.getLight(LightsManager.LIGHT_ID_BACKLIGHT),
                new ColorFade(Display.DEFAULT_DISPLAY));

        mColorFadeOnAnimator = ObjectAnimator.ofFloat(
                mPowerState, DisplayPowerState.COLOR_FADE_LEVEL, 0.0f, 1.0f);
        mColorFadeOnAnimator.setDuration(COLOR_FADE_ON_ANIMATION_DURATION_MILLIS);
        mColorFadeOnAnimator.addListener(mAnimatorListener);

        mColorFadeOffAnimator = ObjectAnimator.ofFloat(
                mPowerState, DisplayPowerState.COLOR_FADE_LEVEL, 1.0f, 0.0f);
        mColorFadeOffAnimator.setDuration(COLOR_FADE_OFF_ANIMATION_DURATION_MILLIS);
        mColorFadeOffAnimator.addListener(mAnimatorListener);

        mScreenBrightnessRampAnimator = new RampAnimator<DisplayPowerState>(
                mPowerState, DisplayPowerState.SCREEN_BRIGHTNESS);
        mScreenBrightnessRampAnimator.setListener(mRampAnimatorListener);

        // Initialize screen state for battery stats.
        try {
            mBatteryStats.noteScreenState(mPowerState.getScreenState());
            mBatteryStats.noteScreenBrightness(mPowerState.getScreenBrightness());
        } catch (RemoteException ex) {
            // same process
        }
    }

    void UpdatePowerState()
    {
        // Update the power state request.
        final Boolean mustNotify;
        Boolean mustInitialize = FALSE;
        Boolean autoBrightnessAdjustmentChanged = FALSE;

        synchronized (mLock) {
            mPendingUpdatePowerStateLocked = FALSE;
            if (mPendingRequestLocked == NULL) {
                return; // wait until first actual power request
            }

            if (mPowerRequest == NULL) {
                mPowerRequest = new DisplayPowerRequest(mPendingRequestLocked);
                mWaitingForNegativeProximity = mPendingWaitForNegativeProximityLocked;
                mPendingWaitForNegativeProximityLocked = FALSE;
                mPendingRequestChangedLocked = FALSE;
                mustInitialize = TRUE;
            } else if (mPendingRequestChangedLocked) {
                autoBrightnessAdjustmentChanged = (mPowerRequest.screenAutoBrightnessAdjustment
                        != mPendingRequestLocked.screenAutoBrightnessAdjustment);
                mPowerRequest.copyFrom(mPendingRequestLocked);
                mWaitingForNegativeProximity |= mPendingWaitForNegativeProximityLocked;
                mPendingWaitForNegativeProximityLocked = FALSE;
                mPendingRequestChangedLocked = FALSE;
                mDisplayReadyLocked = FALSE;
            }

            mustNotify = !mDisplayReadyLocked;
        }

        // Initialize things the first time the power state is changed.
        if (mustInitialize) {
            Initialize();
        }

        // Compute the basic display state using the policy.
        // We might override this below based on other factors.
        Int32 state;
        Int32 brightness = PowerManager.BRIGHTNESS_DEFAULT;
        Boolean performScreenOffTransition = FALSE;
        switch (mPowerRequest.policy) {
            case DisplayPowerRequest.POLICY_OFF:
                state = Display.STATE_OFF;
                performScreenOffTransition = TRUE;
                break;
            case DisplayPowerRequest.POLICY_DOZE:
                if (mPowerRequest.dozeScreenState != Display.STATE_UNKNOWN) {
                    state = mPowerRequest.dozeScreenState;
                } else {
                    state = Display.STATE_DOZE;
                }
                brightness = mPowerRequest.dozeScreenBrightness;
                break;
            case DisplayPowerRequest.POLICY_DIM:
            case DisplayPowerRequest.POLICY_BRIGHT:
            default:
                state = Display.STATE_ON;
                break;
        }
        assert(state != Display.STATE_UNKNOWN);

        // Apply the proximity sensor.
        if (mProximitySensor != NULL) {
            if (mPowerRequest.useProximitySensor && state != Display.STATE_OFF) {
                SetProximitySensorEnabled(TRUE);
                if (!mScreenOffBecauseOfProximity
                        && mProximity == PROXIMITY_POSITIVE) {
                    mScreenOffBecauseOfProximity = TRUE;
                    SendOnProximityPositiveWithWakelock();
                }
            } else if (mWaitingForNegativeProximity
                    && mScreenOffBecauseOfProximity
                    && mProximity == PROXIMITY_POSITIVE
                    && state != Display.STATE_OFF) {
                SetProximitySensorEnabled(TRUE);
            } else {
                SetProximitySensorEnabled(FALSE);
                mWaitingForNegativeProximity = FALSE;
            }
            if (mScreenOffBecauseOfProximity
                    && mProximity != PROXIMITY_POSITIVE) {
                mScreenOffBecauseOfProximity = FALSE;
                SendOnProximityNegativeWithWakelock();
            }
        } else {
            mWaitingForNegativeProximity = FALSE;
        }
        if (mScreenOffBecauseOfProximity) {
            state = Display.STATE_OFF;
        }

        // Animate the screen state change unless already animating.
        // The transition may be deferred, so after this point we will use the
        // actual state instead of the desired one.
        AnimateScreenStateChange(state, performScreenOffTransition);
        state = mPowerState.getScreenState();

        // Use zero brightness when screen is off.
        if (state == Display.STATE_OFF) {
            brightness = PowerManager.BRIGHTNESS_OFF;
        }

        // Use default brightness when dozing unless overridden.
        if (brightness < 0 && (state == Display.STATE_DOZE
                || state == Display.STATE_DOZE_SUSPEND)) {
            brightness = mScreenBrightnessDozeConfig;
        }

        // Configure auto-brightness.
        Boolean autoBrightnessEnabled = FALSE;
        if (mAutomaticBrightnessController != NULL) {
            autoBrightnessEnabled = mPowerRequest.useAutoBrightness
                    && state == Display.STATE_ON && brightness < 0;
            mAutomaticBrightnessController.configure(autoBrightnessEnabled,
                    mPowerRequest.screenAutoBrightnessAdjustment);
        }

        // Apply auto-brightness.
        Boolean slowChange = FALSE;
        if (brightness < 0) {
            if (autoBrightnessEnabled) {
                brightness = mAutomaticBrightnessController.getAutomaticScreenBrightness();
            }
            if (brightness >= 0) {
                // Use current auto-brightness value and slowly adjust to changes.
                brightness = ClampScreenBrightness(brightness);
                if (mAppliedAutoBrightness && !autoBrightnessAdjustmentChanged) {
                    slowChange = TRUE; // slowly adapt to auto-brightness
                }
                mAppliedAutoBrightness = TRUE;
            } else {
                mAppliedAutoBrightness = FALSE;
            }
        } else {
            mAppliedAutoBrightness = FALSE;
        }

        // Apply manual brightness.
        // Use the current brightness setting from the request, which is expected
        // provide a nominal default value for the case where auto-brightness
        // is not ready yet.
        if (brightness < 0) {
            brightness = ClampScreenBrightness(mPowerRequest.screenBrightness);
        }

        // Apply dimming by at least some minimum amount when user activity
        // timeout is about to expire.
        if (mPowerRequest.policy == DisplayPowerRequest.POLICY_DIM) {
            if (brightness > mScreenBrightnessRangeMinimum) {
                brightness = Math.max(Math.min(brightness - SCREEN_DIM_MINIMUM_REDUCTION,
                        mScreenBrightnessDimConfig), mScreenBrightnessRangeMinimum);
            }
            if (!mAppliedDimming) {
                slowChange = FALSE;
            }
            mAppliedDimming = TRUE;
        }

        // If low power mode is enabled, cut the brightness level by half
        // as Int64 as it is above the minimum threshold.
        if (mPowerRequest.lowPowerMode) {
            if (brightness > mScreenBrightnessRangeMinimum) {
                brightness = Math.max(brightness / 2, mScreenBrightnessRangeMinimum);
            }
            if (!mAppliedLowPower) {
                slowChange = FALSE;
            }
            mAppliedLowPower = TRUE;
        }

        // Animate the screen brightness when the screen is on or dozing.
        // Skip the animation when the screen is off or suspended.
        if (state == Display.STATE_ON || state == Display.STATE_DOZE) {
            AnimateScreenBrightness(brightness,
                    slowChange ? BRIGHTNESS_RAMP_RATE_SLOW : BRIGHTNESS_RAMP_RATE_FAST);
        } else {
            AnimateScreenBrightness(brightness, 0);
        }

        // Determine whether the display is ready for use in the newly requested state.
        // Note that we do not wait for the brightness ramp animation to complete before
        // reporting the display is ready because we only need to ensure the screen is in the
        // right power state even as it continues to converge on the desired brightness.
        final Boolean ready = mPendingScreenOnUnblocker == NULL
                && !mColorFadeOnAnimator.isStarted()
                && !mColorFadeOffAnimator.isStarted()
                && mPowerState.waitUntilClean(mCleanListener);
        final Boolean finished = ready
                && !mScreenBrightnessRampAnimator.isAnimating();

        // Grab a wake lock if we have unfinished business.
        if (!finished && !mUnfinishedBusiness) {
            if (DEBUG) {
                Slog.d(TAG, "Unfinished business...");
            }
            mCallbacks.acquireSuspendBlocker();
            mUnfinishedBusiness = TRUE;
        }

        // Notify the power manager when ready.
        if (ready && mustNotify) {
            // Send state change.
            synchronized (mLock) {
                if (!mPendingRequestChangedLocked) {
                    mDisplayReadyLocked = TRUE;

                    if (DEBUG) {
                        Slog.d(TAG, "Display ready!");
                    }
                }
            }
            SendOnStateChangedWithWakelock();
        }

        // Release the wake lock when we have no unfinished business.
        if (finished && mUnfinishedBusiness) {
            if (DEBUG) {
                Slog.d(TAG, "Finished business...");
            }
            mUnfinishedBusiness = FALSE;
            mCallbacks.releaseSuspendBlocker();
        }
    }


    void BlockScreenOn()
    {
        if (mPendingScreenOnUnblocker == NULL) {
            Trace.asyncTraceBegin(Trace.TRACE_TAG_POWER, SCREEN_ON_BLOCKED_TRACE_NAME, 0);
            mPendingScreenOnUnblocker = new ScreenOnUnblocker();
            mScreenOnBlockStartRealTime = SystemClock.elapsedRealtime();
            Slog.i(TAG, "Blocking screen on until initial contents have been drawn.");
        }
    }

    void UnBlockScreenOn()
    {
        if (mPendingScreenOnUnblocker != NULL) {
            mPendingScreenOnUnblocker = NULL;
            Int64 delay = SystemClock.elapsedRealtime() - mScreenOnBlockStartRealTime;
            Slog.i(TAG, "Unblocked screen on after " + delay + " ms");
            Trace.asyncTraceEnd(Trace.TRACE_TAG_POWER, SCREEN_ON_BLOCKED_TRACE_NAME, 0);
        }
    }

    Boolean SetScreenState(
        /* [in] */ Int32 state)
    {
        if (mPowerState.getScreenState() != state) {
            final Boolean wasOn = (mPowerState.getScreenState() != Display.STATE_OFF);
            mPowerState.SetScreenState(state);

            // Tell battery stats about the transition.
            try {
                mBatteryStats.noteScreenState(state);
            } catch (RemoteException ex) {
                // same process
            }

            // Tell the window manager what's happening.
            // Temporarily block turning the screen on until the window manager is ready
            // by leaving a black surface covering the screen.  This surface is essentially
            // the final state of the color fade animation.
            Boolean isOn = (state != Display.STATE_OFF);
            if (wasOn && !isOn) {
                UnBlockScreenOn();
                mWindowManagerPolicy.screenTurnedOff();
            } else if (!wasOn && isOn) {
                if (mPowerState.getColorFadeLevel() == 0.0f) {
                    BlockScreenOn();
                } else {
                    UnBlockScreenOn();
                }
                mWindowManagerPolicy.screenTurningOn(mPendingScreenOnUnblocker);
            }
        }
        return mPendingScreenOnUnblocker == NULL;
    }

    Int32 ClampScreenBrightness(
        /* [in] */ Int32 value)
    {
        return MathUtils.constrain(
                value, mScreenBrightnessRangeMinimum, mScreenBrightnessRangeMaximum);
    }

    void AnimateScreenBrightness(
        /* [in] */ Int32 target,
        /* [in] */ Int32 rate)
    {
        if (DEBUG) {
            Slog.d(TAG, "Animating brightness: target=" + target +", rate=" + rate);
        }
        if (mScreenBrightnessRampAnimator.animateTo(target, rate)) {
            try {
                mBatteryStats.noteScreenBrightness(target);
            } catch (RemoteException ex) {
                // same process
            }
        }
    }

    void AnimateScreenStateChange(
        /* [in] */ Int32 target,
        /* [in] */ Boolean performScreenOffTransition)
    {
        // If there is already an animation in progress, don't interfere with it.
        if (mColorFadeOnAnimator.isStarted()
                || mColorFadeOffAnimator.isStarted()) {
            return;
        }

        // If we were in the process of turning off the screen but didn't quite
        // finish.  Then finish up now to prevent a jarring transition back
        // to screen on if we skipped blocking screen on as usual.
        if (mPendingScreenOff && target != Display.STATE_OFF) {
            SetScreenState(Display.STATE_OFF);
            mPendingScreenOff = FALSE;
        }

        if (target == Display.STATE_ON) {
            // Want screen on.  The contents of the screen may not yet
            // be visible if the color fade has not been dismissed because
            // its last frame of animation is solid black.
            if (!SetScreenState(Display.STATE_ON)) {
                return; // screen on blocked
            }
            if (USE_COLOR_FADE_ON_ANIMATION && mPowerRequest.isBrightOrDim()) {
                // Perform screen on animation.
                if (mPowerState.getColorFadeLevel() == 1.0f) {
                    mPowerState.dismissColorFade();
                } else if (mPowerState.prepareColorFade(mContext,
                        mColorFadeFadesConfig ?
                                ColorFade.MODE_FADE :
                                        ColorFade.MODE_WARM_UP)) {
                    mColorFadeOnAnimator.start();
                } else {
                    mColorFadeOnAnimator.end();
                }
            } else {
                // Skip screen on animation.
                mPowerState.setColorFadeLevel(1.0f);
                mPowerState.dismissColorFade();
            }
        } else if (target == Display.STATE_DOZE) {
            // Want screen dozing.
            // Wait for brightness animation to complete beforehand when entering doze
            // from screen on to prevent a perceptible jump because brightness may operate
            // differently when the display is configured for dozing.
            if (mScreenBrightnessRampAnimator.isAnimating()
                    && mPowerState.getScreenState() == Display.STATE_ON) {
                return;
            }

            // Set screen state.
            if (!SetScreenState(Display.STATE_DOZE)) {
                return; // screen on blocked
            }

            // Dismiss the black surface without fanfare.
            mPowerState.setColorFadeLevel(1.0f);
            mPowerState.dismissColorFade();
        } else if (target == Display.STATE_DOZE_SUSPEND) {
            // Want screen dozing and suspended.
            // Wait for brightness animation to complete beforehand unless already
            // suspended because we may not be able to change it after suspension.
            if (mScreenBrightnessRampAnimator.isAnimating()
                    && mPowerState.getScreenState() != Display.STATE_DOZE_SUSPEND) {
                return;
            }

            // If not already suspending, temporarily set the state to doze until the
            // screen on is unblocked, then suspend.
            if (mPowerState.getScreenState() != Display.STATE_DOZE_SUSPEND) {
                if (!SetScreenState(Display.STATE_DOZE)) {
                    return; // screen on blocked
                }
                SetScreenState(Display.STATE_DOZE_SUSPEND); // already on so can't block
            }

            // Dismiss the black surface without fanfare.
            mPowerState.setColorFadeLevel(1.0f);
            mPowerState.dismissColorFade();
        } else {
            // Want screen off.
            mPendingScreenOff = TRUE;
            if (mPowerState.getColorFadeLevel() == 0.0f) {
                // Turn the screen off.
                // A black surface is already hiding the contents of the screen.
                SetScreenState(Display.STATE_OFF);
                mPendingScreenOff = FALSE;
            } else if (performScreenOffTransition
                    && mPowerState.prepareColorFade(mContext,
                            mColorFadeFadesConfig ?
                                    ColorFade.MODE_FADE : ColorFade.MODE_COOL_DOWN)
                    && mPowerState.getScreenState() != Display.STATE_OFF) {
                // Perform the screen off animation.
                mColorFadeOffAnimator.start();
            } else {
                // Skip the screen off animation and add a black surface to hide the
                // contents of the screen.
                mColorFadeOffAnimator.end();
            }
        }
    }

    void SetProximitySensorEnabled(
        /* [in] */ Boolean enable)
    {
        if (enable) {
            if (!mProximitySensorEnabled) {
                // Register the listener.
                // Proximity sensor state already cleared initially.
                mProximitySensorEnabled = TRUE;
                mSensorManager.registerListener(mProximitySensorListener, mProximitySensor,
                        SensorManager.SENSOR_DELAY_NORMAL, mHandler);
            }
        } else {
            if (mProximitySensorEnabled) {
                // Unregister the listener.
                // Clear the proximity sensor state for next time.
                mProximitySensorEnabled = FALSE;
                mProximity = PROXIMITY_UNKNOWN;
                mPendingProximity = PROXIMITY_UNKNOWN;
                mHandler.removeMessages(MSG_PROXIMITY_SENSOR_DEBOUNCED);
                mSensorManager.unregisterListener(mProximitySensorListener);
                ClearPendingProximityDebounceTime(); // release wake lock (must be last)
            }
        }
    }

    void HandleProximitySensorEvent(
        /* [in] */ Int64 time,
        /* [in] */ Boolean positive)
    {
        if (mProximitySensorEnabled) {
            if (mPendingProximity == PROXIMITY_NEGATIVE && !positive) {
                return; // no change
            }
            if (mPendingProximity == PROXIMITY_POSITIVE && positive) {
                return; // no change
            }

            // Only accept a proximity sensor reading if it remains
            // stable for the entire debounce delay.  We hold a wake lock while
            // debouncing the sensor.
            mHandler.removeMessages(MSG_PROXIMITY_SENSOR_DEBOUNCED);
            if (positive) {
                mPendingProximity = PROXIMITY_POSITIVE;
                SetPendingProximityDebounceTime(
                        time + PROXIMITY_SENSOR_POSITIVE_DEBOUNCE_DELAY); // acquire wake lock
            } else {
                mPendingProximity = PROXIMITY_NEGATIVE;
                SetPendingProximityDebounceTime(
                        time + PROXIMITY_SENSOR_NEGATIVE_DEBOUNCE_DELAY); // acquire wake lock
            }

            // Debounce the new sensor reading.
            DebounceProximitySensor();
        }
    }

    void DebounceProximitySensor()
    {
        if (mProximitySensorEnabled
                && mPendingProximity != PROXIMITY_UNKNOWN
                && mPendingProximityDebounceTime >= 0) {
            final Int64 now = SystemClock.uptimeMillis();
            if (mPendingProximityDebounceTime <= now) {
                // Sensor reading accepted.  Apply the change then release the wake lock.
                mProximity = mPendingProximity;
                UpdatePowerState();
                ClearPendingProximityDebounceTime(); // release wake lock (must be last)
            } else {
                // Need to wait a little longer.
                // Debounce again later.  We continue holding a wake lock while waiting.
                Message msg = mHandler.obtainMessage(MSG_PROXIMITY_SENSOR_DEBOUNCED);
                msg.setAsynchronous(TRUE);
                mHandler.sendMessageAtTime(msg, mPendingProximityDebounceTime);
            }
        }
    }

    void ClearPendingProximityDebounceTime()
    {
        if (mPendingProximityDebounceTime >= 0) {
            mPendingProximityDebounceTime = -1;
            mCallbacks.releaseSuspendBlocker(); // release wake lock
        }
    }

    void SetPendingProximityDebounceTime(
        /* [in] */ Int64 debounceTime)
    {
        if (mPendingProximityDebounceTime < 0) {
            mCallbacks.acquireSuspendBlocker(); // acquire wake lock
        }
        mPendingProximityDebounceTime = debounceTime;
    }

    void SendOnStateChangedWithWakelock()
    {
        mCallbacks.acquireSuspendBlocker();
        mHandler.post(mOnStateChangedRunnable);
    }

    void SendOnProximityPositiveWithWakelock()
    {
        mCallbacks.acquireSuspendBlocker();
        mHandler.post(mOnProximityPositiveRunnable);
    }

    void SendOnProximityNegativeWithWakelock()
    {
        mCallbacks.acquireSuspendBlocker();
        mHandler.post(mOnProximityNegativeRunnable);
    }

    void DumpLocal(
        /* [in] */ IPrintWriter* pw)
    {
        pw.println();
        pw.println("Display Power Controller Thread State:");
        pw.println("  mPowerRequest=" + mPowerRequest);
        pw.println("  mWaitingForNegativeProximity=" + mWaitingForNegativeProximity);

        pw.println("  mProximitySensor=" + mProximitySensor);
        pw.println("  mProximitySensorEnabled=" + mProximitySensorEnabled);
        pw.println("  mProximityThreshold=" + mProximityThreshold);
        pw.println("  mProximity=" + ProximityToString(mProximity));
        pw.println("  mPendingProximity=" + ProximityToString(mPendingProximity));
        pw.println("  mPendingProximityDebounceTime="
                + TimeUtils.formatUptime(mPendingProximityDebounceTime));
        pw.println("  mScreenOffBecauseOfProximity=" + mScreenOffBecauseOfProximity);
        pw.println("  mAppliedAutoBrightness=" + mAppliedAutoBrightness);
        pw.println("  mAppliedDimming=" + mAppliedDimming);
        pw.println("  mAppliedLowPower=" + mAppliedLowPower);
        pw.println("  mPendingScreenOnUnblocker=" + mPendingScreenOnUnblocker);
        pw.println("  mPendingScreenOff=" + mPendingScreenOff);

        pw.println("  mScreenBrightnessRampAnimator.isAnimating()=" +
                mScreenBrightnessRampAnimator.isAnimating());

        if (mColorFadeOnAnimator != NULL) {
            pw.println("  mColorFadeOnAnimator.isStarted()=" +
                    mColorFadeOnAnimator.isStarted());
        }
        if (mColorFadeOffAnimator != NULL) {
            pw.println("  mColorFadeOffAnimator.isStarted()=" +
                    mColorFadeOffAnimator.isStarted());
        }

        if (mPowerState != NULL) {
            mPowerState.dump(pw);
        }

        if (mAutomaticBrightnessController != NULL) {
            mAutomaticBrightnessController.dump(pw);
        }

    }

    static String ProximityToString(
        /* [in] */ Int32 state)
    {
        switch (state) {
            case PROXIMITY_UNKNOWN:
                return "Unknown";
            case PROXIMITY_NEGATIVE:
                return "Negative";
            case PROXIMITY_POSITIVE:
                return "Positive";
            default:
                return Integer.toString(state);
        }
    }

    static AutoPtr<ISpline> CreateAutoBrightnessSpline(
        /* [in] */ ArrayOf<Int32>* lux,
        /* [in] */ ArrayOf<Int32>* brightness)
    {
        try {
            final Int32 n = brightness.length;
            Float[] x = new Float[n];
            Float[] y = new Float[n];
            y[0] = NormalizeAbsoluteBrightness(brightness[0]);
            for (Int32 i = 1; i < n; i++) {
                x[i] = lux[i - 1];
                y[i] = NormalizeAbsoluteBrightness(brightness[i]);
            }

            Spline spline = Spline.createSpline(x, y);
            if (DEBUG) {
                Slog.d(TAG, "Auto-brightness spline: " + spline);
                for (Float v = 1f; v < lux[lux.length - 1] * 1.25f; v *= 1.25f) {
                    Slog.d(TAG, String.format("  %7.1f: %7.1f", v, spline.interpolate(v)));
                }
            }
            return spline;
        } catch (IllegalArgumentException ex) {
            Slog.e(TAG, "Could not create auto-brightness spline.", ex);
            return NULL;
        }
    }

    static Float NormalizeAbsoluteBrightness(
        /* [in] */ Int32 value)
    {
        return (Float)ClampAbsoluteBrightness(value) / PowerManager.BRIGHTNESS_ON;
    }

    static Int32 ClampAbsoluteBrightness(
        /* [in] */ Int32 value)
    {
        return MathUtils.constrain(value, PowerManager.BRIGHTNESS_OFF, PowerManager.BRIGHTNESS_ON);
    }

private:
    static const String TAG = "DisplayPowerController";

    static Boolean DEBUG = FALSE;
    static const Boolean DEBUG_PRETEND_PROXIMITY_SENSOR_ABSENT = FALSE;

    static const String SCREEN_ON_BLOCKED_TRACE_NAME = "Screen on blocked";

    // If TRUE, uses the color fade on animation.
    // We might want to turn this off if we cannot get a guarantee that the screen
    // actually turns on and starts showing new content after the call to set the
    // screen state returns.  Playing the animation can also be somewhat slow.
    static const Boolean USE_COLOR_FADE_ON_ANIMATION = FALSE;

    // The minimum reduction in brightness when dimmed.
    static const Int32 SCREEN_DIM_MINIMUM_REDUCTION = 10;

    static const Int32 COLOR_FADE_ON_ANIMATION_DURATION_MILLIS = 250;
    static const Int32 COLOR_FADE_OFF_ANIMATION_DURATION_MILLIS = 400;

    static const Int32 MSG_UPDATE_POWER_STATE = 1;
    static const Int32 MSG_PROXIMITY_SENSOR_DEBOUNCED = 2;
    static const Int32 MSG_SCREEN_ON_UNBLOCKED = 3;

    static const Int32 PROXIMITY_UNKNOWN = -1;
    static const Int32 PROXIMITY_NEGATIVE = 0;
    static const Int32 PROXIMITY_POSITIVE = 1;

    // Proximity sensor debounce delay in milliseconds for positive or negative transitions.
    static const Int32 PROXIMITY_SENSOR_POSITIVE_DEBOUNCE_DELAY = 0;
    static const Int32 PROXIMITY_SENSOR_NEGATIVE_DEBOUNCE_DELAY = 250;

    // Trigger proximity if distance is less than 5 cm.
    static const Float TYPICAL_PROXIMITY_THRESHOLD = 5.0f;

    // Brightness animation ramp rate in brightness units per second.
    static const Int32 BRIGHTNESS_RAMP_RATE_FAST = 200;
    static const Int32 BRIGHTNESS_RAMP_RATE_SLOW = 40;

    Object mLock;

    AutoPtr<IContext> mContext;

    // Our handler.
    AutoPtr<DisplayControllerHandler> mHandler;

    // Asynchronous callbacks into the power manager service.
    // Only invoked from the handler thread while no locks are held.
    AutoPtr<IDisplayPowerCallbacks> mCallbacks;

    // Battery stats.
    AutoPtr<IIBatteryStats> mBatteryStats;

    // The lights service.
    AutoPtr<ILightsManager> mLights;

    // The sensor manager.
    AutoPtr<ISensorManager> mSensorManager;

    // The window manager policy.
    AutoPtr<IWindowManagerPolicy> mWindowManagerPolicy;

    // The display blanker.
    AutoPtr<IDisplayBlanker> mBlanker;

    // The proximity sensor, or NULL if not available or needed.
    Sensor mProximitySensor;

    // The doze screen brightness.
    Int32 mScreenBrightnessDozeConfig;

    // The dim screen brightness.
    Int32 mScreenBrightnessDimConfig;

    // The minimum screen brightness to use in a very dark room.
    Int32 mScreenBrightnessDarkConfig;

    // The minimum allowed brightness.
    Int32 mScreenBrightnessRangeMinimum;

    // The maximum allowed brightness.
    Int32 mScreenBrightnessRangeMaximum;

    // True if auto-brightness should be used.
    Boolean mUseSoftwareAutoBrightnessConfig;

    // True if we should fade the screen while turning it off, FALSE if we should play
    // a stylish color fade animation instead.
    Boolean mColorFadeFadesConfig;

    // The pending power request.
    // Initially NULL until the first call to RequestPowerState.
    // Guarded by mLock.
    AutoPtr<IDisplayPowerRequest> mPendingRequestLocked;

    // True if a request has been made to wait for the proximity sensor to go negative.
    // Guarded by mLock.
    Boolean mPendingWaitForNegativeProximityLocked;

    // True if the pending power request or wait for negative proximity flag
    // has been changed since the last update occurred.
    // Guarded by mLock.
    Boolean mPendingRequestChangedLocked;

    // Set to TRUE when the important parts of the pending power request have been applied.
    // The important parts are mainly the screen state.  Brightness changes may occur
    // concurrently.
    // Guarded by mLock.
    Boolean mDisplayReadyLocked;

    // Set to TRUE if a power state update is required.
    // Guarded by mLock.
    Boolean mPendingUpdatePowerStateLocked;

    /* The following state must only be accessed by the handler thread. */

    // The currently requested power state.
    // The power controller will progressively update its internal state to match
    // the requested power state.  Initially NULL until the first update.
    AutoPtr<DisplayPowerRequest> mPowerRequest;

    // The current power state.
    // Must only be accessed on the handler thread.
    AutoPtr<IDisplayPowerState> mPowerState;

    // True if the device should wait for negative proximity sensor before
    // waking up the screen.  This is set to FALSE as soon as a negative
    // proximity sensor measurement is observed or when the device is forced to
    // go to sleep by the user.  While TRUE, the screen remains off.
    Boolean mWaitingForNegativeProximity;

    // The actual proximity sensor threshold value.
    Float mProximityThreshold;

    // Set to TRUE if the proximity sensor listener has been registered
    // with the sensor manager.
    Boolean mProximitySensorEnabled;

    // The debounced proximity sensor state.
    Int32 mProximity = PROXIMITY_UNKNOWN;

    // The raw non-debounced proximity sensor state.
    Int32 mPendingProximity = PROXIMITY_UNKNOWN;
    Int64 mPendingProximityDebounceTime = -1; // -1 if fully debounced

    // True if the screen was turned off because of the proximity sensor.
    // When the screen turns on again, we report user activity to the power manager.
    Boolean mScreenOffBecauseOfProximity;

    // The currently active screen on unblocker.  This field is non-NULL whenever
    // we are waiting for a callback to release it and unblock the screen.
    AutoPtr<ScreenOnUnblocker> mPendingScreenOnUnblocker;

    // True if we were in the process of turning off the screen.
    // This allows us to recover more gracefully from situations where we abort
    // turning off the screen.
    Boolean mPendingScreenOff;

    // True if we have unfinished business and are holding a suspend blocker.
    Boolean mUnfinishedBusiness;

    // The elapsed real time when the screen on was blocked.
    Int64 mScreenOnBlockStartRealTime;

    // Remembers whether certain kinds of brightness adjustments
    // were recently applied so that we can decide how to transition.
    Boolean mAppliedAutoBrightness;
    Boolean mAppliedDimming;
    Boolean mAppliedLowPower;

    // The controller for the automatic brightness level.
    AutomaticBrightnessController mAutomaticBrightnessController;

    // Animators.
    AutoPtr<IObjectAnimator> mColorFadeOnAnimator;
    AutoPtr<IObjectAnimator> mColorFadeOffAnimator;
    AutoPtr<RampAnimator> mScreenBrightnessRampAnimator;

    AutoPtr<ISensorEventListener> mProximitySensorListener
    AutoPtr<IAnimatorAnimatorListener> mAnimatorListener
    AutoPtr<IRampAnimatorListener> mRampAnimatorListener
    AutoPtr<IRunnable> mCleanListener
    AutoPtr<IRunnable> mOnStateChangedRunnable
    AutoPtr<IRunnable> mOnProximityPositiveRunnable
    AutoPtr<IRunnable> mOnProximityNegativeRunnable
};

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos