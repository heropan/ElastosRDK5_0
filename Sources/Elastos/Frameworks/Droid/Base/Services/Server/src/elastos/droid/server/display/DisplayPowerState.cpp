
#include "power/DisplayPowerState.h"
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/os/AsyncTask.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::EIID_IRunnable;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Os::IPowerManager;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::AsyncTask;
using Elastos::Droid::View::IChoreographerHelper;
using Elastos::Droid::View::CChoreographerHelper;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::IFloat;
using Elastos::Core::CFloat;
using Elastos::Core::ECLSID_CInteger32;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

//=====================================================================
//  DisplayPowerState::DisplayPowerStateFloatProperty
//=====================================================================
CAR_INTERFACE_IMPL_2(DisplayPowerState::DisplayPowerStateFloatProperty, Object, IFloatProperty, IProperty)

DisplayPowerState::DisplayPowerStateFloatProperty::DisplayPowerStateFloatProperty(
    /* [in] */ const String& name)
{
    mName = name;
    mClsId = ECLSID_CFloat;
}

ECode DisplayPowerState::DisplayPowerStateFloatProperty::IsReadOnly(
    /* [out] */ Boolean* readOnly)
{
    VALIDATE_NOT_NULL(readOnly)
    *readOnly = FALSE;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateFloatProperty::Set(
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* value)
{
    if (IFloat::Probe(value) == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Float v;
    IFloat::Probe(value)->GetValue(&v);
    return SetValue(obj, v);
}

ECode DisplayPowerState::DisplayPowerStateFloatProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    VALIDATE_NOT_NULL(rst)
    AutoPtr<DisplayPowerState> dps = (DisplayPowerState*)(obj);
    Float level = dps->GetColorFadeLevel();
    AutoPtr<IFloat> rstTmp;
    CFloat::New(level, (IFloat**)&rstTmp);
    *rst = rstTmp;
    REFCOUNT_ADD(*rst)
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateFloatProperty::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    *name = mName;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateFloatProperty::GetType(
    /* [out] */ ClassID* id)
{
    VALIDATE_NOT_NULL(id)
    *id = mClsId;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateFloatProperty::IsWriteOnly(
    /* [out] */ Boolean* writeOnly)
{
    VALIDATE_NOT_NULL(writeOnly)
    *writeOnly = FALSE;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateFloatProperty::ForbiddenOperate(
    /* [out] */ Boolean* forbidden)
{
    VALIDATE_NOT_NULL(forbidden)
    *forbidden = FALSE;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateFloatProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Float value)
{
    AutoPtr<DisplayPowerState> dps = (DisplayPowerState*)(obj);
    dps->SetColorFadeLevel(value);
    return NOERROR;
}

//=====================================================================
//  DisplayPowerState::DisplayPowerStateInt32Property
//=====================================================================
CAR_INTERFACE_IMPL_2(DisplayPowerState::DisplayPowerStateInt32Property, Object, IInt32Property, IProperty)

DisplayPowerState::DisplayPowerStateInt32Property::DisplayPowerStateInt32Property(
    /* [in] */ const String& name)
{
    mName = name;
    mClsId = ECLSID_CInteger32;
}

ECode DisplayPowerState::DisplayPowerStateInt32Property::IsReadOnly(
    /* [out] */ Boolean* readOnly)
{
    VALIDATE_NOT_NULL(readOnly)
    *readOnly = FALSE;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateInt32Property::Set(
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* value)
{
    IInteger32* iobj = IInteger32::Probe(value);
    if (iobj == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 v;
    iobj->GetValue(&v);
    return SetValue(obj, v);
}

ECode DisplayPowerState::DisplayPowerStateInt32Property::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    AutoPtr<DisplayPowerState> dps = (DisplayPowerState*)(obj);
    Float level = dps->GetScreenBrightness();
    AutoPtr<IInteger32> rstTmp;
    CInteger32::New(level, (IInteger32**)&rstTmp);
    *rst = rstTmp;
    REFCOUNT_ADD(*rst)
    return NOERROR;
}


ECode DisplayPowerState::DisplayPowerStateInt32Property::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    *name = mName;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateInt32Property::GetType(
    /* [out] */ ClassID* id)
{
    VALIDATE_NOT_NULL(id)
    *id = mClsId;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateInt32Property::IsWriteOnly(
    /* [out] */ Boolean* writeOnly)
{
    VALIDATE_NOT_NULL(writeOnly)
    *writeOnly = FALSE;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateInt32Property::ForbiddenOperate(
    /* [out] */ Boolean* forbidden)
{
    VALIDATE_NOT_NULL(forbidden)
    *forbidden = FALSE;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateInt32Property::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 value)
{
    AutoPtr<DisplayPowerState> dps = (DisplayPowerState*)(obj);
    dps->SetScreenBrightness(value);
    return NOERROR;
}
//=====================================================================
//  DisplayPowerState::PhotonicModulator::TaskRunnable
//=====================================================================
DisplayPowerState::PhotonicModulator::TaskRunnable::TaskRunnable(
    /* [in] */ PhotonicModulator* host)
    : mHost(host)
{}

ECode DisplayPowerState::PhotonicModulator::TaskRunnable::Run()
{
    for (;;) {
        // Get pending change.
        Int32 state;
        Boolean stateChanged;
        Int32 backlight;
        Boolean backlightChanged;
        {
            AutoLock lock(mHost->mLock);
            state = mHost->mPendingState;
            stateChanged = (state != mHost->mActualState);
            backlight = mHost->mPendingBacklight;
            backlightChanged = (backlight != mHost->mActualBacklight);
            if (!stateChanged && !backlightChanged) {
                // All changed applied, notify outer class and wait for more.
                mHost->mChangeInProgress = false;
                mHost->PostScreenUpdateThreadSafe();
                // try {
                    mLock.Wait();
                // } catch (InterruptedException ex) { }
                continue;
            }
            mActualState = state;
            mActualBacklight = backlight;
        }

        // Apply pending change.
        if (DEBUG) {
            Slogger::D(TAG, "Updating screen state: state=%d, backlight=%d", state, backlight);
        }

        Boolean suspending = Display.isSuspendedState(state);
        if (stateChanged && !suspending) {
            RequestDisplayState(state);
        }
        if (backlightChanged) {
            SetBrightness(backlight);
        }
        if (stateChanged && suspending) {
            RequestDisplayState(state);
        }
    }

    return NOERROR;
}

//=====================================================================
//  DisplayPowerState::PhotonicModulator
//=====================================================================
const Boolean DisplayPowerState::PhotonicModulator::INITIAL_SCREEN_ON = IDisplay::STATE_OFF;
const Int32 DisplayPowerState::PhotonicModulator::INITIAL_BACKLIGHT = -1;

DisplayPowerState::PhotonicModulator::PhotonicModulator(
    /* [in] */ DisplayPowerState* host)
    : mPendingState(FALSE)
    , mPendingBacklight(0)
    , mActualState(FALSE)
    , mActualBacklight(0)
    , mChangeInProgress(FALSE)
    , mHost(host)
{
    mTask = (IRunnable*)new TaskRunnable(this);
}

Boolean DisplayPowerState::PhotonicModulator::SetState(
    /* [in] */ Int32 state,
    /* [in] */ Int32 backlight)
{
    AutoLock lock(mLock);
    if (state != mPendingState || backlight != mPendingBacklight) {
        if (DEBUG) {
            Slogger::D(TAG, "Requesting new screen state: state=%d, backlight=%d", state, backlight);
        }

        mPendingState = state;
        mPendingBacklight = backlight;

        if (!mChangeInProgress) {
            mChangeInProgress = TRUE;
            mLock.NotifyAll();
        }
    }
    return !mChangeInProgress;
}

// void DisplayPowerState::PhotonicModulator::Dump(
//     /* [in] */ IPrintWriter* pw);

void DisplayPowerState::PhotonicModulator::RequestDisplayState(
    /* [in] */ Int32 state)
{
    Trace.traceBegin(Trace.TRACE_TAG_POWER, "requestDisplayState("
            + Display.stateToString(state) + ")");
    try {
        mBlanker.requestDisplayState(state);
    } finally {
        Trace.traceEnd(Trace.TRACE_TAG_POWER);
    }
}

void DisplayPowerState::PhotonicModulator::SetBrightness(
    /* [in] */ Int32 backlight)
{
    Trace.traceBegin(Trace.TRACE_TAG_POWER, "setBrightness(" + backlight + ")");
    try {
        mBacklight.setBrightness(backlight);
    } finally {
        Trace.traceEnd(Trace.TRACE_TAG_POWER);
    }
}

//=====================================================================
//  DisplayPowerState::ScreenUpdateRunnable
//=====================================================================
DisplayPowerState::ScreenUpdateRunnable::ScreenUpdateRunnable(
    /* [in] */ DisplayPowerState* host)
    : mHost(host)
{}

ECode DisplayPowerState::ScreenUpdateRunnable::Run()
{
    mHost->mScreenUpdatePending = FALSE;

    Int32 brightness = mHost->mScreenState != IDisplay::STATE_OFF
        && mHost->mColorFadeLevel > 0.0f ? mHost->mScreenBrightness : 0;
    if (mHost->mPhotonicModulator->SetState(mHost->mScreenState, brightness)) {
        mHost->mScreenReady = TRUE;
        mHost->InvokeCleanListenerIfNeeded();
    }
    return NOERROR;
}

//=====================================================================
//  DisplayPowerState::ColorFadeDrawRunnable
//=====================================================================
DisplayPowerState::ColorFadeDrawRunnable::ColorFadeDrawRunnable(
    /* [in] */ DisplayPowerState* host)
    : mHost(host)
{}

ECode DisplayPowerState::ColorFadeDrawRunnable::Run()
{
    mHost->mColorFadeDrawPending = FALSE;

    if (mHost->mColorFadePrepared) {
        mHost->mColorFade->Draw(mHost->mColorFadeLevel);
    }

    mHost->mColorFadeReady = TRUE;
    mHost->InvokeCleanListenerIfNeeded();
    return NOERROR;
}

//=====================================================================
//  DisplayPowerState
//=====================================================================
const String DisplayPowerState::fName("electronBeamLevel");
const String DisplayPowerState::iName("screenBrightness");
const String DisplayPowerState::TAG("DisplayPowerState");

static AutoPtr<IFloatProperty> InitFloatProperty(
    /* [in] */ const String& name)
{
    AutoPtr<IFloatProperty> fp = new DisplayPowerStateFloatProperty(name);
    return fp;
}

static AutoPtr<IInt32Property> InitInt32Property(
    /* [in] */ const String& name)
{
    AutoPtr<IInt32Property> ip = new DisplayPowerStateInt32Property(name);
    return ip;
}

Boolean DisplayPowerState::DEBUG = FALSE;
AutoPtr<IFloatProperty> DisplayPowerState::COLOR_FADE_LEVEL = InitFloatProperty(fName);
AutoPtr<IInt32Property> DisplayPowerState::SCREEN_BRIGHTNESS = InitInt32Property(iName);

DisplayPowerState::DisplayPowerState(
    /* [in] */ IDisplayBlanker* blanker,
    /* [in] */ LightsService::Light* backlight,
    /* [in] */ ColorFade* electronBeam)
    : mColorFade(electronBean)
    , mBlanker(blanker)
    , mBacklight(backlight)
    , mColorFade(electronBeam)
    , mScreenState(IDisplay::STATE_ON)
    , mScreenBrightness(IPowerManager::BRIGHTNESS_ON)
    , mScreenReady(FALSE)
    , mScreenUpdatePending(FALSE)
    , mColorFadePrepared(FALSE)
    , mColorFadeLevel(1.0f)
    , mColorFadeReady(TRUE)
    , mColorFadeDrawPending(FALSE)
{
    mScreenUpdateRunnable = (IRunnable*)new ScreenUpdateRunnable(this);
    mColorFadeDrawRunnable = (IRunnable*)new ColorFadeDrawRunnable(this);

    ASSERT_SUCCEEDED(CHandler::New(TRUE, (IHandler**)&mHandler));
    AutoPtr<IChoreographerHelper> helper;
    ASSERT_SUCCEEDED(CChoreographerHelper::AcquireSingleton((IChoreographerHelper**)&helper));
    helper->GetInstance((IChoreographer**)&mChoreographer);

    mPhotonicModulator = new PhotonicModulator(this);
    mPhotonicModulator->Start();

    // At boot time, we know that the screen is on and the electron beam
    // animation is not playing.  We don't know the screen's brightness though,
    // so prepare to set it to a known state when the state is next applied.
    // Although we set the brightness to full on here, the display power controller
    // will reset the brightness to a new level immediately before the changes
    // actually have a chance to be applied.
    ScheduleScreenUpdate();
}

void DisplayPowerState::SetScreenState(
    /* [in] */ Int32 state)
{
    if (mScreenState != state) {
        if (DEBUG) {
            Slogger::D(TAG, "setScreenOn: on=%d", state);
        }

        mScreenState = state;
        mScreenReady = FALSE;
        ScheduleScreenUpdate();
    }
}

Int32 DisplayPowerState::GetScreenState()
{
    return mScreenState;
}

void DisplayPowerState::SetScreenBrightness(
    /* [in] */ Int32 brightness)
{
    if (mScreenBrightness != brightness) {
        if (DEBUG) {
            Slogger::D(TAG, "setScreenBrightness: brightness=%d", brightness);
        }

        mScreenBrightness = brightness;
        if (mScreenState != IDisplay::STATE_OFF) {
            mScreenReady = FALSE;
            ScheduleScreenUpdate();
        }
    }
}

Int32 DisplayPowerState::GetScreenBrightness()
{
    return mScreenBrightness;
}

Boolean DisplayPowerState::PrepareColorFade(
    /* [in] */ IContext* context,
    /* [in] */ Int32 mode)
{
    if (!mColorFade->Prepare(mode)) {
        mColorFadePrepared = FALSE;
        mColorFadeReady = TRUE;
        return FALSE;
    }

    mColorFadePrepared = TRUE;
    mColorFadeReady = FALSE;
    ScheduleColorFadeDraw();
    return TRUE;
}

void DisplayPowerState::DismissColorFade()
{
    mColorFade->Dismiss();
    mColorFadePrepared = FALSE;
    mColorFadeReady = TRUE;
}

void DisplayPowerState::SetColorFadeLevel(
    /* [in] */ Float level)
{
    if (mColorFadeLevel != level) {
        if (DEBUG) {
            Slogger::D(TAG, "setColorFadeLevel: level=%f", level);
        }

        mColorFadeLevel = level;
        if (mScreenState != IDisplay::STATE_OFF) {
            mScreenReady = FALSE;
            ScheduleScreenUpdate(); // update backlight brightness
        }
        if (mColorFadePrepared) {
            mColorFadeReady = FALSE;
            ScheduleColorFadeDraw();
        }
    }
}

Float DisplayPowerState::GetColorFadeLevel()
{
    return mColorFadeLevel;
}

Boolean DisplayPowerState::WaitUntilClean(
    /* [in] */ IRunnable* listener)
{
    if (!mScreenReady || !mColorFadeReady) {
        mCleanListener = listener;
        return FALSE;
    }
    else {
        mCleanListener = NULL;
        return TRUE;
    }
}

// void DisplayPowerState::Dump(
//     /* [in] */ IPrintWriter* pw);

void DisplayPowerState::ScheduleScreenUpdate()
{
    if (!mScreenUpdatePending) {
        mScreenUpdatePending = TRUE;
        PostScreenUpdateThreadSafe();
    }
}

void DisplayPowerState::PostScreenUpdateThreadSafe()
{
    mHandler->RemoveCallbacks(mScreenUpdateRunnable);
    Boolean result;
    mHandler->Post(mScreenUpdateRunnable, &result);
}

void DisplayPowerState::ScheduleColorFadeDraw()
{
    if (!mColorFadeDrawPending) {
        mColorFadeDrawPending = TRUE;
        mChoreographer->PostCallback(IChoreographer::CALLBACK_TRAVERSAL,
                mColorFadeDrawRunnable, NULL);
    }
}

void DisplayPowerState::InvokeCleanListenerIfNeeded()
{
    AutoPtr<IRunnable> listener = mCleanListener;
    if (listener != NULL && mScreenReady && mColorFadeReady) {
        mCleanListener = NULL;
        listener->Run();
    }
}


} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos
