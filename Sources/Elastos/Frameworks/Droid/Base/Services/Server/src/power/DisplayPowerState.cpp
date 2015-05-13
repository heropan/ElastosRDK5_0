
#include "power/DisplayPowerState.h"
#include "os/Handler.h"
#include <elastos/Slogger.h>

using Elastos::Core::EIID_IRunnable;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Os::IPowerManager;
using Elastos::Droid::Os::CHandler;
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
namespace Power {

CAR_INTERFACE_IMPL_2(DisplayPowerState::DisplayPowerStateFloatProperty, IFloatProperty, IProperty)
CAR_INTERFACE_IMPL(DisplayPowerState, IInterface)

DisplayPowerState::DisplayPowerStateFloatProperty::DisplayPowerStateFloatProperty(
    /* [in] */ const String& name)
{
    mName = name;
    mClsId = ECLSID_CFloat;
}

ECode DisplayPowerState::DisplayPowerStateFloatProperty::IsReadOnly(
    /* [out] */ Boolean* readOnly)
{
    *readOnly = FALSE;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateFloatProperty::Set(
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* value)
{
    if(IFloat::Probe(value) == NULL)
    {
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
    AutoPtr<DisplayPowerState> dps = (DisplayPowerState*)(obj);
    Float level = dps->GetElectronBeamLevel();
    AutoPtr<IFloat> rstTmp;
    CFloat::New(level, (IFloat**)&rstTmp);
    *rst = rstTmp;
    INTERFACE_ADDREF(*rst)
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateFloatProperty::GetName(
    /* [out] */ String* name)
{
    *name = mName;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateFloatProperty::GetType(
    /* [out] */ ClassID* id)
{
    *id = mClsId;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateFloatProperty::IsWriteOnly(
    /* [out] */ Boolean* writeOnly)
{
    *writeOnly = FALSE;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateFloatProperty::ForbiddenOperate(
    /* [out] */ Boolean* forbidden)
{
    *forbidden = FALSE;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateFloatProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Float value)
{
    AutoPtr<DisplayPowerState> dps = (DisplayPowerState*)(obj);
    dps->SetElectronBeamLevel(value);
    return NOERROR;
}

CAR_INTERFACE_IMPL_2(DisplayPowerState::DisplayPowerStateInt32Property, IInt32Property, IProperty)

DisplayPowerState::DisplayPowerStateInt32Property::DisplayPowerStateInt32Property(
    /* [in] */ const String& name)
{
    mName = name;
    mClsId = ECLSID_CInteger32;
}

ECode DisplayPowerState::DisplayPowerStateInt32Property::IsReadOnly(
    /* [out] */ Boolean* readOnly)
{
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
    INTERFACE_ADDREF(*rst)
    return NOERROR;
}


ECode DisplayPowerState::DisplayPowerStateInt32Property::GetName(
    /* [out] */ String* name)
{
    *name = mName;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateInt32Property::GetType(
    /* [out] */ ClassID* id)
{
    *id = mClsId;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateInt32Property::IsWriteOnly(
    /* [out] */ Boolean* writeOnly)
{
    *writeOnly = FALSE;
    return NOERROR;
}

ECode DisplayPowerState::DisplayPowerStateInt32Property::ForbiddenOperate(
    /* [out] */ Boolean* forbidden)
{
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

DisplayPowerState::PhotonicModulator::TaskRunnable::TaskRunnable(
    /* [in] */ PhotonicModulator* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(DisplayPowerState::PhotonicModulator::TaskRunnable, IRunnable);

ECode DisplayPowerState::PhotonicModulator::TaskRunnable::Run()
{
    // Apply pending changes until done.
    for (;;) {
        Boolean on;
        Boolean onChanged;
        Int32 backlight;
        Boolean backlightChanged;
        {
            Mutex::Autolock lock(mHost->mLock);
            on = mHost->mPendingOn;
            onChanged = (on != mHost->mActualOn);
            backlight = mHost->mPendingBacklight;
            backlightChanged = (backlight != mHost->mActualBacklight);
            if (!onChanged && !backlightChanged) {
                mHost->mChangeInProgress = FALSE;
                break;
            }
            mHost->mActualOn = on;
            mHost->mActualBacklight = backlight;
        }

        if (DEBUG) {
            Slogger::D(TAG, "Updating screen state: on=%d, backlight=%d", on, backlight);
        }
        if (onChanged && on) {
            mHost->mHost->mDisplayBlanker->UnblankAllDisplays();
        }
        if (backlightChanged) {
            mHost->mHost->mBacklight->SetBrightness(backlight);
        }
        if (onChanged && !on) {
            mHost->mHost->mDisplayBlanker->BlankAllDisplays();
        }
    }

    // Let the outer class know that all changes have been applied.
    mHost->mHost->PostScreenUpdateThreadSafe();

    return NOERROR;
}


const Boolean DisplayPowerState::PhotonicModulator::INITIAL_SCREEN_ON = FALSE;
const Int32 DisplayPowerState::PhotonicModulator::INITIAL_BACKLIGHT = -1;

DisplayPowerState::PhotonicModulator::PhotonicModulator(
    /* [in] */ DisplayPowerState* host)
    : mPendingOn(FALSE)
    , mPendingBacklight(0)
    , mActualOn(FALSE)
    , mActualBacklight(0)
    , mChangeInProgress(FALSE)
    , mHost(host)
{
    mTask = (IRunnable*)new TaskRunnable(this);
}

Boolean DisplayPowerState::PhotonicModulator::SetState(
    /* [in] */ Boolean on,
    /* [in] */ Int32 backlight)
{
    Mutex::Autolock lock(mLock);
    if (on != mPendingOn || backlight != mPendingBacklight) {
        if (DEBUG) {
            Slogger::D(TAG, "Requesting new screen state: on=%d, backlight=%d", on, backlight);
        }

        mPendingOn = on;
        mPendingBacklight = backlight;

        if (!mChangeInProgress) {
            mChangeInProgress = TRUE;
            // AsyncTask.THREAD_POOL_EXECUTOR.execute(mTask);
        }
    }
    return mChangeInProgress;
}

// void DisplayPowerState::PhotonicModulator::Dump(
//     /* [in] */ IPrintWriter* pw);


DisplayPowerState::ScreenUpdateRunnable::ScreenUpdateRunnable(
    /* [in] */ DisplayPowerState* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(DisplayPowerState::ScreenUpdateRunnable, IRunnable);

ECode DisplayPowerState::ScreenUpdateRunnable::Run()
{
    mHost->mScreenUpdatePending = FALSE;

    Int32 brightness = mHost->mScreenOn && mHost->mElectronBeamLevel > 0.0f ?
            mHost->mScreenBrightness : 0;
    if (mHost->mPhotonicModulator->SetState(mHost->mScreenOn, brightness)) {
        mHost->mScreenReady = TRUE;
        mHost->InvokeCleanListenerIfNeeded();
    }
    return NOERROR;
}


DisplayPowerState::ElectronBeamDrawRunnable::ElectronBeamDrawRunnable(
    /* [in] */ DisplayPowerState* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(DisplayPowerState::ElectronBeamDrawRunnable, IRunnable);

ECode DisplayPowerState::ElectronBeamDrawRunnable::Run()
{
    mHost->mElectronBeamDrawPending = FALSE;

    if (mHost->mElectronBeamPrepared) {
        mHost->mElectronBeam->Draw(mHost->mElectronBeamLevel);
    }

    mHost->mElectronBeamReady = TRUE;
    mHost->InvokeCleanListenerIfNeeded();
    return NOERROR;
}

const String DisplayPowerState::fName("electronBeamLevel");
const String DisplayPowerState::iName("screenBrightness");
const String DisplayPowerState::TAG("DisplayPowerState");
Boolean DisplayPowerState::DEBUG = FALSE;
AutoPtr<IFloatProperty> DisplayPowerState::ELECTRON_BEAM_LEVEL = InitFloatProperty(fName);
AutoPtr<IInt32Property> DisplayPowerState::SCREEN_BRIGHTNESS = InitInt32Property(iName);

AutoPtr<IFloatProperty> DisplayPowerState::InitFloatProperty(
    /* [in] */ const String& name)
{
    AutoPtr<IFloatProperty> fp = new DisplayPowerStateFloatProperty(name);
    return fp;
}

AutoPtr<IInt32Property> DisplayPowerState::InitInt32Property(
    /* [in] */ const String& name)
{
    AutoPtr<IInt32Property> ip = new DisplayPowerStateInt32Property(name);
    return ip;
}

DisplayPowerState::DisplayPowerState(
    /* [in] */ ElectronBeam* electronBean,
    /* [in] */ IDisplayBlanker* displayBlanker,
    /* [in] */ LightsService::Light* backlight)
    : mElectronBeam(electronBean)
    , mDisplayBlanker(displayBlanker)
    , mBacklight(backlight)
    , mScreenOn(TRUE)
    , mScreenBrightness(IPowerManager::BRIGHTNESS_ON)
    , mScreenReady(FALSE)
    , mScreenUpdatePending(FALSE)
    , mElectronBeamPrepared(FALSE)
    , mElectronBeamLevel(1.0f)
    , mElectronBeamReady(TRUE)
    , mElectronBeamDrawPending(FALSE)
{
    mScreenUpdateRunnable = (IRunnable*)new ScreenUpdateRunnable(this);
    mElectronBeamDrawRunnable = (IRunnable*)new ElectronBeamDrawRunnable(this);

    ASSERT_SUCCEEDED(CHandler::New(TRUE, (IHandler**)&mHandler));
    AutoPtr<IChoreographerHelper> helper;
    ASSERT_SUCCEEDED(CChoreographerHelper::AcquireSingleton((IChoreographerHelper**)&helper));
    helper->GetInstance((IChoreographer**)&mChoreographer);
    mPhotonicModulator = new PhotonicModulator(this);

    // At boot time, we know that the screen is on and the electron beam
    // animation is not playing.  We don't know the screen's brightness though,
    // so prepare to set it to a known state when the state is next applied.
    // Although we set the brightness to full on here, the display power controller
    // will reset the brightness to a new level immediately before the changes
    // actually have a chance to be applied.
    ScheduleScreenUpdate();
}

void DisplayPowerState::SetScreenOn(
    /* [in] */ Boolean on)
{
    if (mScreenOn != on) {
        if (DEBUG) {
            Slogger::D(TAG, "setScreenOn: on=%d", on);
        }

        mScreenOn = on;
        mScreenReady = FALSE;
        ScheduleScreenUpdate();
    }
}

Boolean DisplayPowerState::IsScreenOn()
{
    return mScreenOn;
}

void DisplayPowerState::SetScreenBrightness(
    /* [in] */ Int32 brightness)
{
    if (mScreenBrightness != brightness) {
        if (DEBUG) {
            Slogger::D(TAG, "setScreenBrightness: brightness=%d", brightness);
        }

        mScreenBrightness = brightness;
        if (mScreenOn) {
            mScreenReady = FALSE;
            ScheduleScreenUpdate();
        }
    }
}

Int32 DisplayPowerState::GetScreenBrightness()
{
    return mScreenBrightness;
}

Boolean DisplayPowerState::PrepareElectronBeam(
    /* [in] */ Int32 mode)
{
    if (!mElectronBeam->Prepare(mode)) {
        mElectronBeamPrepared = FALSE;
        mElectronBeamReady = TRUE;
        return FALSE;
    }

    mElectronBeamPrepared = TRUE;
    mElectronBeamReady = FALSE;
    ScheduleElectronBeamDraw();
    return TRUE;
}

void DisplayPowerState::DismissElectronBeam()
{
    mElectronBeam->Dismiss();
    mElectronBeamPrepared = FALSE;
    mElectronBeamReady = TRUE;
}

void DisplayPowerState::SetElectronBeamLevel(
    /* [in] */ Float level)
{
    if (mElectronBeamLevel != level) {
        if (DEBUG) {
            Slogger::D(TAG, "setElectronBeamLevel: level=%f", level);
        }

        mElectronBeamLevel = level;
        if (mScreenOn) {
            mScreenReady = FALSE;
            ScheduleScreenUpdate(); // update backlight brightness
        }
        if (mElectronBeamPrepared) {
            mElectronBeamReady = FALSE;
            ScheduleElectronBeamDraw();
        }
    }
}

Float DisplayPowerState::GetElectronBeamLevel()
{
    return mElectronBeamLevel;
}

Boolean DisplayPowerState::WaitUntilClean(
    /* [in] */ IRunnable* listener)
{
    if (!mScreenReady || !mElectronBeamReady) {
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
    SLOGGERD("DisplayPowerState", "DisplayPowerState::PostScreenUpdateThreadSafe: mHandler = %p", mHandler.Get())
    mHandler->RemoveCallbacks(mScreenUpdateRunnable);
    Boolean result;
    mHandler->Post(mScreenUpdateRunnable, &result);
}

void DisplayPowerState::ScheduleElectronBeamDraw()
{
    if (!mElectronBeamDrawPending) {
        mElectronBeamDrawPending = TRUE;
        mChoreographer->PostCallback(IChoreographer::CALLBACK_TRAVERSAL,
                mElectronBeamDrawRunnable, NULL);
    }
}

void DisplayPowerState::InvokeCleanListenerIfNeeded()
{
    AutoPtr<IRunnable> listener = mCleanListener;
    if (listener != NULL && mScreenReady && mElectronBeamReady) {
        mCleanListener = NULL;
        listener->Run();
    }
}


} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos
