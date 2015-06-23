
#include "power/DisplayPowerController.h"
#include "os/SystemClock.h"
#include "os/Handler.h"
#include "R.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::EIID_IRunnable;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Animation::EIID_IAnimatorListener;
using Elastos::Droid::Animation::IObjectAnimatorHelper;
using Elastos::Droid::Animation::CObjectAnimatorHelper;
using Elastos::Droid::Hardware::EIID_ISensorManager;
using Elastos::Droid::Hardware::EIID_ISensorEventListener;
using Elastos::Droid::Hardware::ISystemSensorManager;
using Elastos::Droid::Hardware::CSystemSensorManager;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::CPowerManagerHelper;
using Elastos::Droid::Text::Format::IDateUtils;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

const Int32 DisplayPowerController::MSG_UPDATE_POWER_STATE = 1;
const Int32 DisplayPowerController::MSG_PROXIMITY_SENSOR_DEBOUNCED = 2;
const Int32 DisplayPowerController::MSG_LIGHT_SENSOR_DEBOUNCED = 3;

ECode DisplayPowerController::DisplayControllerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    switch(what) {
        case DisplayPowerController::MSG_UPDATE_POWER_STATE:
            mHost->UpdatePowerState();
            break;

        case DisplayPowerController::MSG_PROXIMITY_SENSOR_DEBOUNCED:
            mHost->DebounceProximitySensor();
            break;

        case DisplayPowerController::MSG_LIGHT_SENSOR_DEBOUNCED:
            mHost->DebounceLightSensor();
            break;
    }

    return NOERROR;
}

DisplayPowerController::MyAnimatorListener::MyAnimatorListener(
    /* [in] */ DisplayPowerController* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(DisplayPowerController::MyAnimatorListener, IAnimatorListener);

ECode DisplayPowerController::MyAnimatorListener::OnAnimationStart(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

ECode DisplayPowerController::MyAnimatorListener::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    mHost->SendUpdatePowerState();
    return NOERROR;
}

ECode DisplayPowerController::MyAnimatorListener::OnAnimationRepeat(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

ECode DisplayPowerController::MyAnimatorListener::OnAnimationCancel(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}


DisplayPowerController::CleanListener::CleanListener(
    /* [in] */ DisplayPowerController* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(DisplayPowerController::CleanListener, IRunnable);

ECode DisplayPowerController::CleanListener::Run()
{
    mHost->SendUpdatePowerState();
    return NOERROR;
}


DisplayPowerController::OnStateChangedRunnable::OnStateChangedRunnable(
    /* [in] */ DisplayPowerController* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(DisplayPowerController::OnStateChangedRunnable, IRunnable);

ECode DisplayPowerController::OnStateChangedRunnable::Run()
{
    mHost->mCallbacks->OnStateChanged();
    return NOERROR;
}


DisplayPowerController::OnProximityPositiveRunnable::OnProximityPositiveRunnable(
    /* [in] */ DisplayPowerController* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(DisplayPowerController::OnProximityPositiveRunnable, IRunnable);

ECode DisplayPowerController::OnProximityPositiveRunnable::Run()
{
    mHost->mCallbacks->OnProximityPositive();
    return NOERROR;
}

DisplayPowerController::OnProximityNegativeRunnable::OnProximityNegativeRunnable(
    /* [in] */ DisplayPowerController* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(DisplayPowerController::OnProximityNegativeRunnable, IRunnable);

ECode DisplayPowerController::OnProximityNegativeRunnable::Run()
{
    mHost->mCallbacks->OnProximityNegative();
    return NOERROR;
}


DisplayPowerController::ProximitySensorListener::ProximitySensorListener(
    /* [in] */ DisplayPowerController* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(DisplayPowerController::ProximitySensorListener, ISensorEventListener);

ECode DisplayPowerController::ProximitySensorListener::OnSensorChanged(
    /* [in] */ ISensorEvent* event)
{
    if (mHost->mProximitySensorEnabled) {
        Int64 time = SystemClock::GetUptimeMillis();
        AutoPtr< ArrayOf<Float> > values;
        event->GetValues((ArrayOf<Float>**)&values);
        Float distance = (*values)[0];
        Boolean positive = distance >= 0.0f && distance < mHost->mProximityThreshold;
        mHost->HandleProximitySensorEvent(time, positive);
    }
    return NOERROR;
}

ECode DisplayPowerController::ProximitySensorListener::OnAccuracyChanged(
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 accuracy)
{
    // Not used.
    return NOERROR;
}


DisplayPowerController::LightSensorListener::LightSensorListener(
    /* [in] */ DisplayPowerController* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(DisplayPowerController::LightSensorListener, ISensorEventListener);

ECode DisplayPowerController::LightSensorListener::OnSensorChanged(
    /* [in] */ ISensorEvent* event)
{
    if (mHost->mLightSensorEnabled) {
        Int64 time = SystemClock::GetUptimeMillis();
        AutoPtr< ArrayOf<Float> > values;
        event->GetValues((ArrayOf<Float>**)&values);
        Float lux = (*values)[0];
        mHost->HandleLightSensorEvent(time, lux);
    }
    return NOERROR;
}

ECode DisplayPowerController::LightSensorListener::OnAccuracyChanged(
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 accuracy)
{
    // Not used.
    return NOERROR;
}


CAR_INTERFACE_IMPL(DisplayPowerController::MyTwilightListener, IInterface)

DisplayPowerController::MyTwilightListener::MyTwilightListener(
    /* [in] */ DisplayPowerController* host)
    : mHost(host)
{}

ECode DisplayPowerController::MyTwilightListener::OnTwilightStateChanged()
{
    Slogger::D(TAG, "DisplayPowerController::MyTwilightListener::OnTwilightStateChanged()");
    mHost->mTwilightChanged = TRUE;
    mHost->UpdatePowerState();
    return NOERROR;
}


static Boolean InitUseScreenAutoBrightnessAdjustment()
{
    AutoPtr<IPowerManagerHelper> helper;
    CPowerManagerHelper::AcquireSingleton((IPowerManagerHelper**)&helper);
    Boolean result;
    helper->UseScreenAutoBrightnessAdjustmentFeature(&result);
    return result;
}

static Boolean InitUseTwilightAdjustmentFeature()
{
    AutoPtr<IPowerManagerHelper> helper;
    CPowerManagerHelper::AcquireSingleton((IPowerManagerHelper**)&helper);
    Boolean result;
    helper->UseTwilightAdjustmentFeature(&result);
    return result;
}

const String DisplayPowerController::TAG("DisplayPowerController");
Boolean DisplayPowerController::DEBUG = FALSE;
const Boolean DisplayPowerController::DEBUG_PRETEND_PROXIMITY_SENSOR_ABSENT = FALSE;
const Boolean DisplayPowerController::DEBUG_PRETEND_LIGHT_SENSOR_ABSENT = FALSE;
const Boolean DisplayPowerController::USE_ELECTRON_BEAM_ON_ANIMATION = FALSE;
const Boolean DisplayPowerController::USE_SCREEN_AUTO_BRIGHTNESS_ADJUSTMENT
        = InitUseScreenAutoBrightnessAdjustment();
const Float DisplayPowerController::SCREEN_AUTO_BRIGHTNESS_ADJUSTMENT_MAX_GAMMA = 3.0f;
const Int32 DisplayPowerController::SCREEN_DIM_MINIMUM_REDUCTION = 10;
const Boolean DisplayPowerController::USE_TWILIGHT_ADJUSTMENT
        = InitUseTwilightAdjustmentFeature();
const Float DisplayPowerController::TWILIGHT_ADJUSTMENT_MAX_GAMMA = 1.5f;
const Int64 DisplayPowerController::TWILIGHT_ADJUSTMENT_TIME = IDateUtils::HOUR_IN_MILLIS * 2;
const Int32 DisplayPowerController::ELECTRON_BEAM_ON_ANIMATION_DURATION_MILLIS = 250;
const Int32 DisplayPowerController::ELECTRON_BEAM_OFF_ANIMATION_DURATION_MILLIS = 400;
const Int32 DisplayPowerController::PROXIMITY_UNKNOWN = -1;
const Int32 DisplayPowerController::PROXIMITY_NEGATIVE = 0;
const Int32 DisplayPowerController::PROXIMITY_POSITIVE = 1;
const Int32 DisplayPowerController::PROXIMITY_SENSOR_POSITIVE_DEBOUNCE_DELAY = 0;
const Int32 DisplayPowerController::PROXIMITY_SENSOR_NEGATIVE_DEBOUNCE_DELAY = 500;
const Float DisplayPowerController::TYPICAL_PROXIMITY_THRESHOLD = 5.0f;
const Int32 DisplayPowerController::LIGHT_SENSOR_RATE_MILLIS = 1000;
const Int32 DisplayPowerController::SYNTHETIC_LIGHT_SENSOR_RATE_MILLIS = LIGHT_SENSOR_RATE_MILLIS * 2;
const Int32 DisplayPowerController::BRIGHTNESS_RAMP_RATE_FAST = 200;
const Int32 DisplayPowerController::BRIGHTNESS_RAMP_RATE_SLOW = 40;
const Int64 DisplayPowerController::SHORT_TERM_AVERAGE_LIGHT_TIME_CONSTANT = 1000;
const Int64 DisplayPowerController::LONG_TERM_AVERAGE_LIGHT_TIME_CONSTANT = 5000;
const Int64 DisplayPowerController::BRIGHTENING_LIGHT_DEBOUNCE = 4000;
const Int64 DisplayPowerController::DARKENING_LIGHT_DEBOUNCE = 8000;
const Float DisplayPowerController::BRIGHTENING_LIGHT_HYSTERESIS = 0.10f;
const Float DisplayPowerController::DARKENING_LIGHT_HYSTERESIS = 0.20f;

DisplayPowerController::DisplayPowerController(
    /* [in] */ ILooper* looper,
    /* [in] */ IContext* context,
    /* [in] */ Notifier* notifier,
    /* [in] */ LightsService* lights,
    /* [in] */ TwilightService* twilight,
    /* [in] */ ISensorManager* sensorManager,
    /* [in] */ CDisplayManagerService* displayManager,
    /* [in] */ IDisplayBlanker* displayBlanker,
    /* [in] */ IDisplayPowerControllerCallbacks* callbacks,
    /* [in] */ IHandler* callbackHandler)
    : mNotifier(notifier)
    , mDisplayBlanker(displayBlanker)
    , mCallbacks(callbacks)
    , mCallbackHandler(callbackHandler)
    , mLights(lights)
    , mTwilight(twilight)
    , mDisplayManager(displayManager)
    , mSensorManager(sensorManager)
    , mPendingWaitForNegativeProximityLocked(FALSE)
    , mPendingRequestChangedLocked(FALSE)
    , mDisplayReadyLocked(FALSE)
    , mPendingUpdatePowerStateLocked(FALSE)
    , mWaitingForNegativeProximity(FALSE)
    , mProximityThreshold(0)
    , mProximitySensorEnabled(FALSE)
    , mProximity(PROXIMITY_UNKNOWN)
    , mPendingProximity(PROXIMITY_UNKNOWN)
    , mPendingProximityDebounceTime(0)
    , mScreenOffBecauseOfProximity(FALSE)
    , mScreenOnWasBlocked(FALSE)
    , mScreenOnBlockStartRealTime(0)
    , mLightSensorEnabled(FALSE)
    , mLightSensorEnableTime(0)
    , mAmbientLux(0)
    , mAmbientLuxValid(FALSE)
    , mLastObservedLux(0)
    , mLastObservedLuxTime(0)
    , mRecentLightSamples(0)
    , mRecentShortTermAverageLux(0)
    , mRecentLongTermAverageLux(0)
    , mDebounceLuxDirection(0)
    , mDebounceLuxTime(0)
    , mScreenAutoBrightness(0)
    , mLastScreenAutoBrightnessGamma(0)
    , mUsingScreenAutoBrightness(FALSE)
    , mTwilightChanged(FALSE)
{
    mAnimatorListener = (IAnimatorListener*)new MyAnimatorListener(this);
    mCleanListener = (IRunnable*)new CleanListener(this);
    mOnStateChangedRunnable = (IRunnable*)new OnStateChangedRunnable(this);
    mOnProximityPositiveRunnable = (IRunnable*)new OnProximityPositiveRunnable(this);
    mOnProximityNegativeRunnable = (IRunnable*)new OnProximityNegativeRunnable(this);
    mProximitySensorListener = (ISensorEventListener*)new ProximitySensorListener(this);
    mLightSensorListener = (ISensorEventListener*)new LightSensorListener(this);
    mTwilightListener = (TwilightService::ITwilightListener*)new MyTwilightListener(this);

    mHandler = new DisplayControllerHandler(looper, this);

    AutoPtr<IResources> resources;
    ASSERT_SUCCEEDED(context->GetResources((IResources**)&resources));

    Int32 i;
    resources->GetInteger(R::integer::config_screenBrightnessDim, &i);
    mScreenBrightnessDimConfig = ClampAbsoluteBrightness(i);

    resources->GetInteger(R::integer::config_screenBrightnessSettingMinimum, &i);
    Int32 screenBrightnessMinimum = Elastos::Core::Math::Min(i, mScreenBrightnessDimConfig);

    resources->GetBoolean(R::bool_::config_automatic_brightness_available,
            &mUseSoftwareAutoBrightnessConfig);
    if (mUseSoftwareAutoBrightnessConfig) {
        AutoPtr< ArrayOf<Int32> > lux;
        resources->GetIntArray(R::array::config_autoBrightnessLevels, (ArrayOf<Int32>**)&lux);
        AutoPtr< ArrayOf<Int32> > screenBrightness;
        resources->GetIntArray(R::array::config_autoBrightnessLcdBacklightValues,
                (ArrayOf<Int32>**)&screenBrightness);

        // mScreenAutoBrightnessSpline = CreateAutoBrightnessSpline(lux, screenBrightness);
        // if (mScreenAutoBrightnessSpline == null) {
        //     Slog.e(TAG, "Error in config.xml.  config_autoBrightnessLcdBacklightValues "
        //             + "(size " + screenBrightness.length + ") "
        //             + "must be monotic and have exactly one more entry than "
        //             + "config_autoBrightnessLevels (size " + lux.length + ") "
        //             + "which must be strictly increasing.  "
        //             + "Auto-brightness will be disabled.");
        //     mUseSoftwareAutoBrightnessConfig = false;
        // } else {
        //     if (screenBrightness[0] < screenBrightnessMinimum) {
        //         screenBrightnessMinimum = screenBrightness[0];
        //     }
        // }
        mUseSoftwareAutoBrightnessConfig = FALSE;

        resources->GetInteger(
                R::integer::config_lightSensorWarmupTime, &mLightSensorWarmUpTimeConfig);
    }

    mScreenBrightnessRangeMinimum = ClampAbsoluteBrightness(screenBrightnessMinimum);
    mScreenBrightnessRangeMaximum = IPowerManager::BRIGHTNESS_ON;

    resources->GetBoolean(R::bool_::config_animateScreenLights, &mElectronBeamFadesConfig);

    if (!DEBUG_PRETEND_PROXIMITY_SENSOR_ABSENT) {
        mSensorManager->GetDefaultSensor(ISensor::TYPE_PROXIMITY, (ISensor**)&mProximitySensor);
        if (mProximitySensor != NULL) {
            Float range;
            mProximitySensor->GetMaximumRange(&range);
            mProximityThreshold = Elastos::Core::Math::Min(range, TYPICAL_PROXIMITY_THRESHOLD);
        }
    }

    if (mUseSoftwareAutoBrightnessConfig
            && !DEBUG_PRETEND_LIGHT_SENSOR_ABSENT) {
        mSensorManager->GetDefaultSensor(ISensor::TYPE_LIGHT, (ISensor**)&mLightSensor);
    }

    if (mUseSoftwareAutoBrightnessConfig && USE_TWILIGHT_ADJUSTMENT) {
        mTwilight->RegisterListener(mTwilightListener, mHandler);
    }
}

// static Spline createAutoBrightnessSpline(int[] lux, int[] brightness) {
//     try {
//         final int n = brightness.length;
//         float[] x = new float[n];
//         float[] y = new float[n];
//         y[0] = normalizeAbsoluteBrightness(brightness[0]);
//         for (int i = 1; i < n; i++) {
//             x[i] = lux[i - 1];
//             y[i] = normalizeAbsoluteBrightness(brightness[i]);
//         }

//         Spline spline = Spline.createMonotoneCubicSpline(x, y);
//         if (DEBUG) {
//             Slog.d(TAG, "Auto-brightness spline: " + spline);
//             for (float v = 1f; v < lux[lux.length - 1] * 1.25f; v *= 1.25f) {
//                 Slog.d(TAG, String.format("  %7.1f: %7.1f", v, spline.interpolate(v)));
//             }
//         }
//         return spline;
//     } catch (IllegalArgumentException ex) {
//         Slog.e(TAG, "Could not create auto-brightness spline.", ex);
//         return null;
//     }
// }

Boolean DisplayPowerController::IsProximitySensorAvailable()
{
    return mProximitySensor != NULL;
}

Boolean DisplayPowerController::RequestPowerState(
    /* [in] */ DisplayPowerRequest* request,
    /* [in] */ Boolean waitForNegativeProximity)
{
    if (DEBUG) {
        Slogger::D(TAG, "requestPowerState: %p, waitForNegativeProximity=%d"
            , request, waitForNegativeProximity);
    }

    Mutex::Autolock lock(mLock);
    Boolean changed = FALSE;

    if (waitForNegativeProximity && !mPendingWaitForNegativeProximityLocked) {
        mPendingWaitForNegativeProximityLocked = TRUE;
        changed = TRUE;
    }

    if (mPendingRequestLocked == NULL) {
        mPendingRequestLocked = new DisplayPowerRequest(request);
        changed = TRUE;
    }
    else if (!mPendingRequestLocked->Equals(request)) {
        mPendingRequestLocked->CopyFrom(request);
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

void DisplayPowerController::SendUpdatePowerState()
{
    Mutex::Autolock lock(mLock);
    SendUpdatePowerStateLocked();
}

void DisplayPowerController::SendUpdatePowerStateLocked()
{
    if (!mPendingUpdatePowerStateLocked) {
        mPendingUpdatePowerStateLocked = TRUE;

        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(MSG_UPDATE_POWER_STATE, (IMessage**)&msg);
        msg->SetAsynchronous(TRUE);
        Boolean result;
        mHandler->SendMessage(msg, &result);
    }
}

void DisplayPowerController::Initialize()
{
    AutoPtr<ElectronBeam> eb = new ElectronBeam(mDisplayManager);
    mPowerState = new DisplayPowerState(eb, mDisplayBlanker,
            mLights->GetLight(LightsService::_LIGHT_ID_BACKLIGHT));
    AutoPtr<IObjectAnimatorHelper> animatorHelper;
    ASSERT_SUCCEEDED(CObjectAnimatorHelper::AcquireSingleton(
            (IObjectAnimatorHelper**)&animatorHelper));
    AutoPtr<ArrayOf<Float> > pfArray1 = ArrayOf<Float>::Alloc(2);
    (*pfArray1)[0] = 0.0f;
    (*pfArray1)[1] = 1.0f;

    AutoPtr<ArrayOf<Float> > pfArray2 = ArrayOf<Float>::Alloc(2);
    (*pfArray2)[0] = 1.0f;
    (*pfArray2)[1] = 0.0f;

    ASSERT_SUCCEEDED(animatorHelper->OfFloat(mPowerState,
            DisplayPowerState::ELECTRON_BEAM_LEVEL, pfArray1,
            (IObjectAnimator**)&mElectronBeamOnAnimator));
    mElectronBeamOnAnimator->SetDuration(ELECTRON_BEAM_ON_ANIMATION_DURATION_MILLIS);
    mElectronBeamOnAnimator->AddListener(mAnimatorListener);

    ASSERT_SUCCEEDED(animatorHelper->OfFloat(mPowerState,
            DisplayPowerState::ELECTRON_BEAM_LEVEL, pfArray2,
            (IObjectAnimator**)&mElectronBeamOffAnimator));
    mElectronBeamOffAnimator->SetDuration(ELECTRON_BEAM_OFF_ANIMATION_DURATION_MILLIS);
    mElectronBeamOffAnimator->AddListener(mAnimatorListener);

    mScreenBrightnessRampAnimator = new RampAnimator<AutoPtr<DisplayPowerState> >(
            mPowerState, DisplayPowerState::SCREEN_BRIGHTNESS);
}

void DisplayPowerController::UpdatePowerState()
{
    // Update the power state request.
    Boolean mustNotify = FALSE;
    Boolean mustInitialize = FALSE;
    Boolean updateAutoBrightness = mTwilightChanged;
    Boolean wasDim = FALSE;
    mTwilightChanged = FALSE;

    {
        Mutex::Autolock lock(mLock);
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
        }
        else if (mPendingRequestChangedLocked) {
            if (mPowerRequest->mScreenAutoBrightnessAdjustment
                    != mPendingRequestLocked->mScreenAutoBrightnessAdjustment) {
                updateAutoBrightness = TRUE;
            }
            wasDim = (mPowerRequest->mScreenState == DisplayPowerRequest::SCREEN_STATE_DIM);
            mPowerRequest->CopyFrom(mPendingRequestLocked);
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

    // Apply the proximity sensor.
    if (mProximitySensor != NULL) {
        if (mPowerRequest->mUseProximitySensor
                && mPowerRequest->mScreenState != DisplayPowerRequest::SCREEN_STATE_OFF) {
            SetProximitySensorEnabled(TRUE);
            if (!mScreenOffBecauseOfProximity
                    && mProximity == PROXIMITY_POSITIVE) {
                mScreenOffBecauseOfProximity = TRUE;
                SendOnProximityPositive();
                SetScreenOn(FALSE);
            }
        }
        else if (mWaitingForNegativeProximity
                && mScreenOffBecauseOfProximity
                && mProximity == PROXIMITY_POSITIVE
                && mPowerRequest->mScreenState != DisplayPowerRequest::SCREEN_STATE_OFF) {
            SetProximitySensorEnabled(TRUE);
        }
        else {
            SetProximitySensorEnabled(FALSE);
            mWaitingForNegativeProximity = FALSE;
        }
        if (mScreenOffBecauseOfProximity
                && mProximity != PROXIMITY_POSITIVE) {
            mScreenOffBecauseOfProximity = FALSE;
            SendOnProximityNegative();
        }
    }
    else {
        mWaitingForNegativeProximity = FALSE;
    }

    // Turn on the light sensor if needed.
    if (mLightSensor != NULL) {
        SetLightSensorEnabled(mPowerRequest->mUseAutoBrightness
                && WantScreenOn(mPowerRequest->mScreenState), updateAutoBrightness);
    }

    // Set the screen brightness.
    if (WantScreenOn(mPowerRequest->mScreenState)) {
        Int32 target;
        Boolean slow;
        if (mScreenAutoBrightness >= 0 && mLightSensorEnabled) {
            // Use current auto-brightness value.
            target = mScreenAutoBrightness;
            slow = mUsingScreenAutoBrightness;
            mUsingScreenAutoBrightness = TRUE;
        }
        else {
            // Light sensor is disabled or not ready yet.
            // Use the current brightness setting from the request, which is expected
            // provide a nominal default value for the case where auto-brightness
            // is not ready yet.
            target = mPowerRequest->mScreenBrightness;
            slow = FALSE;
            mUsingScreenAutoBrightness = FALSE;
        }
        if (mPowerRequest->mScreenState == DisplayPowerRequest::SCREEN_STATE_DIM) {
            // Dim quickly by at least some minimum amount.
            target = Elastos::Core::Math::Min(target - SCREEN_DIM_MINIMUM_REDUCTION,
                    mScreenBrightnessDimConfig);
            slow = FALSE;
        }
        else if (wasDim) {
            // Brighten quickly.
            slow = FALSE;
        }
        AnimateScreenBrightness(ClampScreenBrightness(target),
                slow ? BRIGHTNESS_RAMP_RATE_SLOW : BRIGHTNESS_RAMP_RATE_FAST);
    }
    else {
        // Screen is off.  Don't bother changing the brightness.
        mUsingScreenAutoBrightness = FALSE;
    }

    // Animate the screen on or off.
    if (!mScreenOffBecauseOfProximity) {
        if (WantScreenOn(mPowerRequest->mScreenState)) {
            // Want screen on.
            // Wait for previous off animation to complete beforehand.
            // It is relatively short but if we cancel it and switch to the
            // on animation immediately then the results are pretty ugly.
            Boolean isStarted;
            if (mElectronBeamOffAnimator->IsStarted(&isStarted), !isStarted) {
                // Turn the screen on.  The contents of the screen may not yet
                // be visible if the electron beam has not been dismissed because
                // its last frame of animation is solid black.
                SetScreenOn(TRUE);

                if (mPowerRequest->mBlockScreenOn
                        && mPowerState->GetElectronBeamLevel() == 0.0f) {
                    BlockScreenOn();
                }
                else {
                    UnblockScreenOn();
                    if (USE_ELECTRON_BEAM_ON_ANIMATION) {
                        if (mElectronBeamOnAnimator->IsStarted(&isStarted), !isStarted) {
                            if (mPowerState->GetElectronBeamLevel() == 1.0f) {
                                mPowerState->DismissElectronBeam();
                            }
                            else if (mPowerState->PrepareElectronBeam(
                                    mElectronBeamFadesConfig ?
                                            ElectronBeam::MODE_FADE :
                                                    ElectronBeam::MODE_WARM_UP)) {
                                mElectronBeamOnAnimator->Start();
                            }
                            else {
                                mElectronBeamOnAnimator->End();
                            }
                        }
                    }
                    else {
                        mPowerState->SetElectronBeamLevel(1.0f);
                        mPowerState->DismissElectronBeam();
                    }
                }
            }
        }
        else {
            // Want screen off.
            // Wait for previous on animation to complete beforehand.
            Boolean isStarted;
            if (mElectronBeamOnAnimator->IsStarted(&isStarted), !isStarted) {
                Boolean isOffStarted;
                if (mElectronBeamOffAnimator->IsStarted(&isOffStarted), !isOffStarted) {
                    if (mPowerState->GetElectronBeamLevel() == 0.0f) {
                        SetScreenOn(FALSE);
                    }
                    else if (mPowerState->PrepareElectronBeam(
                            mElectronBeamFadesConfig ?
                                    ElectronBeam::MODE_FADE :
                                            ElectronBeam::MODE_COOL_DOWN)
                            && mPowerState->IsScreenOn()) {
                        mElectronBeamOffAnimator->Start();
                    }
                    else {
                        mElectronBeamOffAnimator->End();
                    }
                }
            }
        }
    }

    // Report whether the display is ready for use.
    // We mostly care about the screen state here, ignoring brightness changes
    // which will be handled asynchronously.
    Boolean isOnStarted, isOffStarted;
    if (mustNotify
            && !mScreenOnWasBlocked
            && (mElectronBeamOnAnimator->IsStarted(&isOnStarted), !isOnStarted)
            && (mElectronBeamOffAnimator->IsStarted(&isOffStarted), !isOffStarted)
            && mPowerState->WaitUntilClean(mCleanListener)) {
        {
            Mutex::Autolock lock(mLock);
            if (!mPendingRequestChangedLocked) {
                mDisplayReadyLocked = TRUE;

                if (DEBUG) {
                    Slogger::D(TAG, "Display ready!");
                }
            }
        }
        SendOnStateChanged();
    }
}

void DisplayPowerController::BlockScreenOn()
{
    if (!mScreenOnWasBlocked) {
        mScreenOnWasBlocked = TRUE;
        if (DEBUG) {
            Slogger::D(TAG, "Blocked screen on.");
            mScreenOnBlockStartRealTime = SystemClock::GetElapsedRealtime();
        }
    }
}

void DisplayPowerController::UnblockScreenOn()
{
    if (mScreenOnWasBlocked)
    {
        mScreenOnWasBlocked = FALSE;
        if (DEBUG) {
            Slogger::D(TAG, "Unblocked screen on after %d ms",
                    SystemClock::GetElapsedRealtime() - mScreenOnBlockStartRealTime);
        }
    }
}

void DisplayPowerController::SetScreenOn(
    /* [in] */ Boolean on)
{
    if (!mPowerState->IsScreenOn() == on) {
        mPowerState->SetScreenOn(on);
        if (on) {
            mNotifier->OnScreenOn();
        }
        else {
            mNotifier->OnScreenOff();
        }
    }
}

Int32 DisplayPowerController::ClampScreenBrightness(
    /* [in] */Int32 value)
{
    return Clamp(value, mScreenBrightnessRangeMinimum, mScreenBrightnessRangeMaximum);
}

Int32 DisplayPowerController::ClampAbsoluteBrightness(
    /* [in] */ Int32 value)
{
    return Clamp(value, IPowerManager::BRIGHTNESS_OFF, IPowerManager::BRIGHTNESS_ON);
}

Int32 DisplayPowerController::Clamp(
    /* [in] */Int32 value,
    /* [in] */Int32 min,
    /* [in] */Int32 max)
{
    if (value <= min) {
        return min;
    }
    if (value >= max) {
        return max;
    }
    return value;
}

Float DisplayPowerController::NormalizeAbsoluteBrightness(
    /* [in] */ Int32 value)
{
    return (Float)ClampAbsoluteBrightness(value) / IPowerManager::BRIGHTNESS_ON;
}

void DisplayPowerController::AnimateScreenBrightness(
    /* [in] */ Int32 target,
    /* [in] */ Int32 rate)
{
    if (mScreenBrightnessRampAnimator->AnimateTo(target, rate)) {
        mNotifier->OnScreenBrightness(target);
    }
}

void DisplayPowerController::SetProximitySensorEnabled(
    /* [in] */ Boolean enable)
{
    if (enable) {
        if (!mProximitySensorEnabled) {
            mProximitySensorEnabled = TRUE;
            mPendingProximity = PROXIMITY_UNKNOWN;
            Boolean result;
            mSensorManager->RegisterListener(mProximitySensorListener, mProximitySensor,
                ISensorManager::SENSOR_DELAY_NORMAL, mHandler, &result);
        }
    }
    else {
        if (mProximitySensorEnabled) {
            mProximitySensorEnabled = FALSE;
            mProximity = PROXIMITY_UNKNOWN;
            mHandler->RemoveMessages(MSG_PROXIMITY_SENSOR_DEBOUNCED);
            mSensorManager->UnregisterListener(mProximitySensorListener);
        }
    }
}

void DisplayPowerController::HandleProximitySensorEvent(
    /* [in] */ Int64 time,
    /* [in] */ Boolean positive)
{
    if (mPendingProximity == PROXIMITY_NEGATIVE && !positive) {
        return; // no change
    }
    if (mPendingProximity == PROXIMITY_POSITIVE && positive) {
        return; // no change
    }

    // Only accept a proximity sensor reading if it remains
    // stable for the entire debounce delay.
    mHandler->RemoveMessages(MSG_PROXIMITY_SENSOR_DEBOUNCED);
    if (positive) {
        mPendingProximity = PROXIMITY_POSITIVE;
        mPendingProximityDebounceTime = time + PROXIMITY_SENSOR_POSITIVE_DEBOUNCE_DELAY;
    }
    else {
        mPendingProximity = PROXIMITY_NEGATIVE;
        mPendingProximityDebounceTime = time + PROXIMITY_SENSOR_NEGATIVE_DEBOUNCE_DELAY;
    }
    DebounceProximitySensor();
}

void DisplayPowerController::DebounceProximitySensor()
{
    if (mPendingProximity != PROXIMITY_UNKNOWN) {
        Int64 now = SystemClock::GetUptimeMillis();
        if (mPendingProximityDebounceTime <= now) {
            mProximity = mPendingProximity;
            SendUpdatePowerState();
        }
        else {
            AutoPtr<IMessage> msg;
            mHandler->ObtainMessage(MSG_PROXIMITY_SENSOR_DEBOUNCED, (IMessage**)&msg);
            msg->SetAsynchronous(TRUE);
            Boolean result;
            mHandler->SendMessageAtTime(msg, mPendingProximityDebounceTime, &result);
        }
    }
}

void DisplayPowerController::SetLightSensorEnabled(
    /* [in] */ Boolean enable,
    /* [in] */ Boolean updateAutoBrightness)
{
    if (enable) {
        if (!mLightSensorEnabled) {
            updateAutoBrightness = TRUE;
            mLightSensorEnabled = TRUE;
            mLightSensorEnableTime = SystemClock::GetUptimeMillis();
            Boolean result;
            mSensorManager->RegisterListener(mLightSensorListener, mLightSensor,
                    LIGHT_SENSOR_RATE_MILLIS * 1000, mHandler, &result);
        }
    }
    else {
        if (mLightSensorEnabled) {
            mLightSensorEnabled = FALSE;
            mAmbientLuxValid = FALSE;
            mRecentLightSamples = 0;
            mHandler->RemoveMessages(MSG_LIGHT_SENSOR_DEBOUNCED);
        }
    }
    if (updateAutoBrightness) {
        UpdateAutoBrightness(FALSE);
    }
}

void DisplayPowerController::HandleLightSensorEvent(
    /* [in] */ Int64 time,
    /* [in] */ Float lux)
{
    mHandler->RemoveMessages(MSG_LIGHT_SENSOR_DEBOUNCED);

    ApplyLightSensorMeasurement(time, lux);
    UpdateAmbientLux(time);
}

void DisplayPowerController::ApplyLightSensorMeasurement(
    /* [in] */ Int64 time,
    /* [in] */ Float lux)
{
    // Update our filters.
    mRecentLightSamples += 1;
    if (mRecentLightSamples == 1) {
        mRecentShortTermAverageLux = lux;
        mRecentLongTermAverageLux = lux;
    }
    else {
        Int64 timeDelta = time - mLastObservedLuxTime;
        mRecentShortTermAverageLux += (lux - mRecentShortTermAverageLux)
                * timeDelta / (SHORT_TERM_AVERAGE_LIGHT_TIME_CONSTANT + timeDelta);
        mRecentLongTermAverageLux += (lux - mRecentLongTermAverageLux)
                * timeDelta / (LONG_TERM_AVERAGE_LIGHT_TIME_CONSTANT + timeDelta);
    }

    // Remember this sample value.
    mLastObservedLux = lux;
    mLastObservedLuxTime = time;
}

void DisplayPowerController::UpdateAmbientLux(
    /* [in] */ Int64 time)
{
    // If the light sensor was just turned on then immediately update our initial
    // estimate of the current ambient light level.
    if (!mAmbientLuxValid
            || (time - mLightSensorEnableTime) < mLightSensorWarmUpTimeConfig) {
        mAmbientLux = mRecentShortTermAverageLux;
        mAmbientLuxValid = TRUE;
        mDebounceLuxDirection = 0;
        mDebounceLuxTime = time;
        if (DEBUG) {
            Slogger::D(TAG, "updateAmbientLux: Initializing: , mRecentShortTermAverageLux=%f, mRecentLongTermAverageLux=%f, mAmbientLux=%f"
                    , mRecentShortTermAverageLux, mRecentLongTermAverageLux,  mAmbientLux);
        }
        UpdateAutoBrightness(TRUE);
        return;
    }

    // Determine whether the ambient environment appears to be brightening.
    Float brighteningLuxThreshold = mAmbientLux * (1.0f + BRIGHTENING_LIGHT_HYSTERESIS);
    if (mRecentShortTermAverageLux > brighteningLuxThreshold
            && mRecentLongTermAverageLux > brighteningLuxThreshold) {
        if (mDebounceLuxDirection <= 0) {
            mDebounceLuxDirection = 1;
            mDebounceLuxTime = time;
            if (DEBUG) {
                Slogger::D(TAG, "updateAmbientLux: Possibly brightened, waiting for %dms: brighteningLuxThreshold=%f, mRecentShortTermAverageLux=%f, mRecentLongTermAverageLux=%f, mAmbientLux=%f"
                        , BRIGHTENING_LIGHT_DEBOUNCE, brighteningLuxThreshold
                        , mRecentShortTermAverageLux, mRecentLongTermAverageLux, mAmbientLux);
            }
        }
        Int64 debounceTime = mDebounceLuxTime + BRIGHTENING_LIGHT_DEBOUNCE;
        if (time >= debounceTime) {
            mAmbientLux = mRecentShortTermAverageLux;
            if (DEBUG) {
                Slogger::D(TAG, "updateAmbientLux: Brightened: brighteningLuxThreshold=%f, mRecentShortTermAverageLux=%f, mRecentLongTermAverageLux=%f, mAmbientLux=%f"
                        , brighteningLuxThreshold, mRecentShortTermAverageLux
                        , mRecentLongTermAverageLux, mAmbientLux);
            }
            UpdateAutoBrightness(TRUE);
        }
        else {
            Boolean result;
            mHandler->SendEmptyMessageAtTime(
                MSG_LIGHT_SENSOR_DEBOUNCED, debounceTime, &result);
        }
        return;
    }

    // Determine whether the ambient environment appears to be darkening.
    Float darkeningLuxThreshold = mAmbientLux * (1.0f - DARKENING_LIGHT_HYSTERESIS);
    if (mRecentShortTermAverageLux < darkeningLuxThreshold
            && mRecentLongTermAverageLux < darkeningLuxThreshold) {
        if (mDebounceLuxDirection >= 0) {
            mDebounceLuxDirection = -1;
            mDebounceLuxTime = time;
            if (DEBUG) {
                Slogger::D(TAG, "updateAmbientLux: Possibly darkened, waiting for %f ms: darkeningLuxThreshold=%f, mRecentShortTermAverageLux=%f, mRecentLongTermAverageLux=%f, mAmbientLux=%f"
                        , DARKENING_LIGHT_DEBOUNCE , darkeningLuxThreshold
                        , mRecentShortTermAverageLux , mRecentLongTermAverageLux
                        , mAmbientLux);
            }
        }
        Int64 debounceTime = mDebounceLuxTime + DARKENING_LIGHT_DEBOUNCE;
        if (time >= debounceTime) {
            // Be conservative about reducing the brightness, only reduce it a little bit
            // at a time to avoid having to bump it up again soon.
            mAmbientLux = Elastos::Core::Math::Max(mRecentShortTermAverageLux, mRecentLongTermAverageLux);
            if (DEBUG) {
                Slogger::D(TAG, "updateAmbientLux: Darkened: darkeningLuxThreshold=%f, mRecentShortTermAverageLux=%f, mRecentLongTermAverageLux=%f, mAmbientLux=%f"
                        , darkeningLuxThreshold, mRecentShortTermAverageLux
                        , mRecentLongTermAverageLux, mAmbientLux);
            }
            UpdateAutoBrightness(TRUE);
        }
        else {
            Boolean result;
            mHandler->SendEmptyMessageAtTime(
                MSG_LIGHT_SENSOR_DEBOUNCED, debounceTime, &result);
        }
        return;
    }

    // No change or change is within the hysteresis thresholds.
    if (mDebounceLuxDirection != 0) {
        mDebounceLuxDirection = 0;
        mDebounceLuxTime = time;
        if (DEBUG) {
            Slogger::D(TAG, "updateAmbientLux: Canceled debounce: brighteningLuxThreshold=%f, darkeningLuxThreshold=%f, mRecentShortTermAverageLux=%f, mRecentLongTermAverageLux=%f, mAmbientLux=%f"
                    , brighteningLuxThreshold, darkeningLuxThreshold
                    , mRecentShortTermAverageLux, mRecentLongTermAverageLux, mAmbientLux);
        }
    }

    // If the light level does not change, then the sensor may not report
    // a new value.  This can cause problems for the auto-brightness algorithm
    // because the filters might not be updated.  To work around it, we want to
    // make sure to update the filters whenever the observed light level could
    // possibly exceed one of the hysteresis thresholds.
    if (mLastObservedLux > brighteningLuxThreshold
            || mLastObservedLux < darkeningLuxThreshold) {
        Boolean result;
        mHandler->SendEmptyMessageAtTime(
            MSG_LIGHT_SENSOR_DEBOUNCED, time + SYNTHETIC_LIGHT_SENSOR_RATE_MILLIS, &result);
    }
}

void DisplayPowerController::DebounceLightSensor()
{
    if (mLightSensorEnabled) {
        Int64 time = SystemClock::GetUptimeMillis();
        if (time >= mLastObservedLuxTime + SYNTHETIC_LIGHT_SENSOR_RATE_MILLIS) {
            if (DEBUG) {
                Slogger::D(TAG, "debounceLightSensor: Synthesizing light sensor measurement after %d ms."
                        , time - mLastObservedLuxTime);
            }
            ApplyLightSensorMeasurement(time, mLastObservedLux);
        }
        UpdateAmbientLux(time);
    }
}

void DisplayPowerController::UpdateAutoBrightness(
    /* [in] */ Boolean sendUpdate)
{
    if (!mAmbientLuxValid) {
        return;
    }

    Float value = 0.0f;//mScreenAutoBrightnessSpline.interpolate(mAmbientLux);
    Float gamma = 1.0f;

    if (USE_SCREEN_AUTO_BRIGHTNESS_ADJUSTMENT
            && mPowerRequest->mScreenAutoBrightnessAdjustment != 0.0f) {
        Float adjGamma = (Float)Elastos::Core::Math::Pow((Double)SCREEN_AUTO_BRIGHTNESS_ADJUSTMENT_MAX_GAMMA,
                (Double)Elastos::Core::Math::Min(1.0f, Elastos::Core::Math::Max(-1.0f,
                        -mPowerRequest->mScreenAutoBrightnessAdjustment)));
        gamma *= adjGamma;
        if (DEBUG) {
            Slogger::D(TAG, "updateAutoBrightness: adjGamma=%f", adjGamma);
        }
    }

    if (USE_TWILIGHT_ADJUSTMENT) {
        AutoPtr<TwilightService::TwilightState> state;
        mTwilight->GetCurrentState((TwilightService::TwilightState**)&state);
        if (state != NULL && state->IsNight()) {
            AutoPtr<ISystem> system;
            Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
            Int64 now;
            system->GetCurrentTimeMillis(&now);
            Float earlyGamma =
                    GetTwilightGamma(now, state->GetYesterdaySunset(), state->GetTodaySunrise());
            Float lateGamma =
                    GetTwilightGamma(now, state->GetTodaySunset(), state->GetTomorrowSunrise());
            gamma *= earlyGamma * lateGamma;
            if (DEBUG) {
                Slogger::D(TAG, "updateAutoBrightness: earlyGamma=%f, lateGamma=%f"
                        , earlyGamma, lateGamma);
            }
        }
    }

    if (gamma != 1.0f) {
        Float in = value;
        value = (Float)Elastos::Core::Math::Pow((Double)value, (Double)gamma);
        if (DEBUG) {
            Slogger::D(TAG, "updateAutoBrightness: gamma=%f, in=%f, out=%f"
                    , gamma, in, value);
        }
    }

    Int32 newScreenAutoBrightness = ClampScreenBrightness(
            Elastos::Core::Math::Round(value * IPowerManager::BRIGHTNESS_ON));
    if (mScreenAutoBrightness != newScreenAutoBrightness) {
        if (DEBUG) {
            Slogger::D(TAG, "updateAutoBrightness: mScreenAutoBrightness=%d, newScreenAutoBrightness=%d"
                    , mScreenAutoBrightness, newScreenAutoBrightness);
        }

        mScreenAutoBrightness = newScreenAutoBrightness;
        mLastScreenAutoBrightnessGamma = gamma;
        if (sendUpdate) {
            SendUpdatePowerState();
        }
    }
}

Float DisplayPowerController::GetTwilightGamma(
    /* [in] */ Int64 now,
    /* [in] */ Int64 lastSunset,
    /* [in] */ Int64 nextSunrise)
{
    if (lastSunset < 0 || nextSunrise < 0
            || now < lastSunset || now > nextSunrise) {
        return 1.0f;
    }

    if (now < lastSunset + TWILIGHT_ADJUSTMENT_TIME) {
        return Lerp(1.0f, TWILIGHT_ADJUSTMENT_MAX_GAMMA,
                (Float)(now - lastSunset) / TWILIGHT_ADJUSTMENT_TIME);
    }

    if (now > nextSunrise - TWILIGHT_ADJUSTMENT_TIME) {
        return Lerp(1.0f, TWILIGHT_ADJUSTMENT_MAX_GAMMA,
                (Float)(nextSunrise - now) / TWILIGHT_ADJUSTMENT_TIME);
    }

    return TWILIGHT_ADJUSTMENT_MAX_GAMMA;
}

Float DisplayPowerController::Lerp(
    /* [in] */ Int64 x,
    /* [in] */ Int64 y,
    /* [in] */ Int64 alpha)
{
    return x + (y - x) * alpha;
}

void DisplayPowerController::SendOnStateChanged()
{
    Boolean result;
    mCallbackHandler->Post(mOnStateChangedRunnable, &result);
}

void DisplayPowerController::SendOnProximityPositive()
{
    Boolean result;
    mCallbackHandler->Post(mOnProximityPositiveRunnable, &result);
}

void DisplayPowerController::SendOnProximityNegative()
{
    Boolean result;
    mCallbackHandler->Post(mOnProximityNegativeRunnable, &result);
}

// void DisplayPowerController::Dump(
//     /* [in] */ const IPrintWriter* pw)
// {
//     synchronized (mLock) {
//         pw.println();
//         pw.println("Display Controller Locked State:");
//         pw.println("  mDisplayReadyLocked=" + mDisplayReadyLocked);
//         pw.println("  mPendingRequestLocked=" + mPendingRequestLocked);
//         pw.println("  mPendingRequestChangedLocked=" + mPendingRequestChangedLocked);
//         pw.println("  mPendingWaitForNegativeProximityLocked="
//                 + mPendingWaitForNegativeProximityLocked);
//         pw.println("  mPendingUpdatePowerStateLocked=" + mPendingUpdatePowerStateLocked);
//     }

//     pw.println();
//     pw.println("Display Controller Configuration:");
//     pw.println("  mScreenBrightnessDimConfig=" + mScreenBrightnessDimConfig);
//     pw.println("  mScreenBrightnessRangeMinimum=" + mScreenBrightnessRangeMinimum);
//     pw.println("  mScreenBrightnessRangeMaximum=" + mScreenBrightnessRangeMaximum);
//     pw.println("  mUseSoftwareAutoBrightnessConfig="
//             + mUseSoftwareAutoBrightnessConfig);
//     pw.println("  mScreenAutoBrightnessSpline=" + mScreenAutoBrightnessSpline);
//     pw.println("  mLightSensorWarmUpTimeConfig=" + mLightSensorWarmUpTimeConfig);

//     mHandler.runWithScissors(new Runnable() {
//         @Override
//         public void run() {
//             dumpLocal(pw);
//         }
//     }, 1000);
// }

// void DisplayPowerController::DumpLocal(
//     /* [in] */ IPrintWriter* pw)
// {
//     pw.println();
//     pw.println("Display Controller Thread State:");
//     pw.println("  mPowerRequest=" + mPowerRequest);
//     pw.println("  mWaitingForNegativeProximity=" + mWaitingForNegativeProximity);

//     pw.println("  mProximitySensor=" + mProximitySensor);
//     pw.println("  mProximitySensorEnabled=" + mProximitySensorEnabled);
//     pw.println("  mProximityThreshold=" + mProximityThreshold);
//     pw.println("  mProximity=" + proximityToString(mProximity));
//     pw.println("  mPendingProximity=" + proximityToString(mPendingProximity));
//     pw.println("  mPendingProximityDebounceTime="
//             + TimeUtils.formatUptime(mPendingProximityDebounceTime));
//     pw.println("  mScreenOffBecauseOfProximity=" + mScreenOffBecauseOfProximity);

//     pw.println("  mLightSensor=" + mLightSensor);
//     pw.println("  mLightSensorEnabled=" + mLightSensorEnabled);
//     pw.println("  mLightSensorEnableTime="
//             + TimeUtils.formatUptime(mLightSensorEnableTime));
//     pw.println("  mAmbientLux=" + mAmbientLux);
//     pw.println("  mAmbientLuxValid=" + mAmbientLuxValid);
//     pw.println("  mLastObservedLux=" + mLastObservedLux);
//     pw.println("  mLastObservedLuxTime="
//             + TimeUtils.formatUptime(mLastObservedLuxTime));
//     pw.println("  mRecentLightSamples=" + mRecentLightSamples);
//     pw.println("  mRecentShortTermAverageLux=" + mRecentShortTermAverageLux);
//     pw.println("  mRecentLongTermAverageLux=" + mRecentLongTermAverageLux);
//     pw.println("  mDebounceLuxDirection=" + mDebounceLuxDirection);
//     pw.println("  mDebounceLuxTime=" + TimeUtils.formatUptime(mDebounceLuxTime));
//     pw.println("  mScreenAutoBrightness=" + mScreenAutoBrightness);
//     pw.println("  mUsingScreenAutoBrightness=" + mUsingScreenAutoBrightness);
//     pw.println("  mLastScreenAutoBrightnessGamma=" + mLastScreenAutoBrightnessGamma);
//     pw.println("  mTwilight.getCurrentState()=" + mTwilight.getCurrentState());

//     if (mElectronBeamOnAnimator != null) {
//         pw.println("  mElectronBeamOnAnimator.isStarted()=" +
//                 mElectronBeamOnAnimator.isStarted());
//     }
//     if (mElectronBeamOffAnimator != null) {
//         pw.println("  mElectronBeamOffAnimator.isStarted()=" +
//                 mElectronBeamOffAnimator.isStarted());
//     }

//     if (mPowerState != null) {
//         mPowerState.dump(pw);
//     }
// }

String DisplayPowerController::ProximityToString(
    /* [in] */ Int32 state)
{
    switch (state) {
        case PROXIMITY_UNKNOWN:
            return String("Unknown");
        case PROXIMITY_NEGATIVE:
            return String("Negative");
        case PROXIMITY_POSITIVE:
            return String("Positive");
        default:
            return StringUtils::Int32ToString(state);
    }
}

Boolean DisplayPowerController::WantScreenOn(
    /* [in] */ Int32 state)
{
    switch (state) {
        case DisplayPowerRequest::SCREEN_STATE_BRIGHT:
        case DisplayPowerRequest::SCREEN_STATE_DIM:
            return TRUE;
    }
    return FALSE;
}

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos
