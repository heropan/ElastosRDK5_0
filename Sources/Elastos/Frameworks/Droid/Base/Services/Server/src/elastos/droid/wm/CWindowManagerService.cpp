
#include "wm/CWindowManagerService.h"
#include "wm/StartingData.h"
#include "wm/ScreenRotationAnimation.h"
#include "wm/KeyguardDisableHandler.h"
#include "wm/ViewServer.h"
#include "wm/DragState.h"
#include "wm/InputMonitor.h"
#include "wm/FakeWindowImpl.h"
#include "elastos/droid/os/Process.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/os/Binder.h"
#include "elastos/droid/os/Looper.h"
#include "elastos/droid/os/SomeArgs.h"
#include "elastos/droid/app/ActivityManagerNative.h"
#include "am/CActivityManagerService.h"
#include "am/CBatteryStatsService.h"
#include "power/ShutdownThread.h"
#include "R.h"
#include "Manifest.h"
#include <elastos/core/Thread.h>
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Slogger.h>
#include <binder/Parcel.h>
#include <binder/IServiceManager.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::CStringWrapper;
using Elastos::Core::StringUtils;
using Elastos::Utility::IDate;
using Elastos::Utility::CDate;
using Elastos::Utility::Logging::Slogger;
using Elastos::Core::CObjectContainer;
using Elastos::IO::IWriter;
using Elastos::IO::CPrintWriter;
using Elastos::IO::IStringWriter;
using Elastos::IO::CStringWriter;
using Elastos::IO::IBufferedWriter;
using Elastos::IO::IOutputStream;
using Elastos::IO::IOutputStreamWriter;
using Elastos::IO::COutputStreamWriter;
using Elastos::IO::CBufferedWriter;
using Elastos::IO::IInputStream;
using Elastos::IO::CDataInputStream;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::CFile;
using Elastos::IO::IDataInputStream;
using Elastos::IO::IDataInput;
using Elastos::IO::IFlushable;
using Elastos::IO::ICloseable;
using Elastos::Text::IDateFormat;
using Elastos::Text::IDateFormatHelper;
using Elastos::Text::CDateFormatHelper;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::Process;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IWindowManagerPolicyWindowManagerFuncs;
using Elastos::Droid::View::IChoreographerHelper;
using Elastos::Droid::View::CChoreographerHelper;
using Elastos::Droid::View::IInputChannelHelper;
using Elastos::Droid::View::CInputChannelHelper;
using Elastos::Droid::View::IWindowManagerGlobal;
using Elastos::Droid::View::IInternalInsetsInfo;
using Elastos::Droid::View::CInputChannel;
using Elastos::Droid::View::ISurfaceHelper;
using Elastos::Droid::View::IWindowInfoHelper;
using Elastos::Droid::View::IInputDevice;
// using Elastos::Droid::View::CWindowInfoHelper;
using Elastos::Droid::View::CSurfaceHelper;
using Elastos::Droid::View::CSurfaceSession;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::CSurface;
using Elastos::Droid::View::EIID_IOnKeyguardExitResult;
using Elastos::Droid::View::EIID_IIWindowManager;
using Elastos::Droid::View::EIID_IWindowManagerPolicyWindowManagerFuncs;
using Elastos::Droid::View::Animation::IAnimationUtils;
using Elastos::Droid::View::Animation::CAnimationUtils;
using Elastos::Droid::View::Animation::IAnimationSet;
using Elastos::Droid::View::Animation::CAnimationSet;
using Elastos::Droid::View::Animation::EIID_IInterpolator;
using Elastos::Droid::View::Animation::IAlphaAnimation;
using Elastos::Droid::View::Animation::CAlphaAnimation;
using Elastos::Droid::View::Animation::IScaleAnimation;
using Elastos::Droid::View::Animation::CScaleAnimation;
using Elastos::Droid::View::Animation::IDecelerateInterpolator;
using Elastos::Droid::View::Animation::CDecelerateInterpolator;
using Elastos::Droid::View::Animation::EIID_IAnimation;
using Elastos::Droid::Animation::IValueAnimatorHelper;
using Elastos::Droid::Animation::CValueAnimatorHelper;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::Res::CConfiguration;
using Elastos::Droid::Content::Res::IConfigurationHelper;
using Elastos::Droid::Content::Res::CConfigurationHelper;
using Elastos::Droid::Content::Res::ICompatibilityInfoHelper;
using Elastos::Droid::Content::Res::CCompatibilityInfoHelper;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::App::IActivityOptions;
using Elastos::Droid::App::ActivityManagerNative;
using Elastos::Droid::App::IStatusBarManager;
using Elastos::Droid::App::Admin::IDevicePolicyManager;
using Elastos::Droid::Internal::Policy::IPolicyManager;
using Elastos::Droid::Internal::Policy::CPolicyManager;
using Elastos::Droid::Graphics::CPoint;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::CRegion;
using Elastos::Droid::Graphics::CRectF;
using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::Graphics::BitmapConfig;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Graphics::CMatrix;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::CCanvas;
using Elastos::Droid::Hardware::Display::EIID_IDisplayListener;
using Elastos::Droid::Utility::CDisplayMetrics;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::Os::CBinder;
using Elastos::Droid::Os::ITrace;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::SomeArgs;
// using Elastos::Droid::Os::CTrace;
using Elastos::Droid::Utility::ITypedValueHelper;
using Elastos::Droid::Utility::CTypedValueHelper;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Server::Am::CActivityManagerService;
using Elastos::Droid::Server::Am::CBatteryStatsService;
using Elastos::Droid::Server::Power::ShutdownThread;
using Elastos::Droid::Provider::ISettingsGlobal;
using Elastos::Droid::Provider::CSettingsGlobal;
using Elastos::Droid::View::IWindowInfo;
using Elastos::Droid::View::CWindowInfo;
using Elastos::Droid::View::IWindowInfoHelper;
using Elastos::Droid::View::CWindowInfoHelper;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

const Int32 CWindowManagerService::LayoutFields::SET_UPDATE_ROTATION                = 1 << 0;
const Int32 CWindowManagerService::LayoutFields::SET_WALLPAPER_MAY_CHANGE           = 1 << 1;
const Int32 CWindowManagerService::LayoutFields::SET_FORCE_HIDING_CHANGED           = 1 << 2;
const Int32 CWindowManagerService::LayoutFields::SET_ORIENTATION_CHANGE_COMPLETE    = 1 << 3;
const Int32 CWindowManagerService::LayoutFields::SET_TURN_ON_SCREEN                 = 1 << 4;
const Int32 CWindowManagerService::LayoutFields::DISPLAY_CONTENT_UNKNOWN = 0;
const Int32 CWindowManagerService::LayoutFields::DISPLAY_CONTENT_MIRROR = 1;
const Int32 CWindowManagerService::LayoutFields::DISPLAY_CONTENT_UNIQUE = 2;

CWindowManagerService::LayoutFields::LayoutFields()
    : mWallpaperForceHidingChanged(FALSE)
    , mWallpaperMayChange(FALSE)
    , mOrientationChangeComplete(TRUE)
    , mAdjResult(0)
    , mObscured(FALSE)
    , mDimming(FALSE)
    , mSyswin(FALSE)
    , mScreenBrightness(-1)
    , mButtonBrightness(-1)
    , mUserActivityTimeout(-1)
    , mUpdateRotation(FALSE)
    , mDisplayHasContent(DISPLAY_CONTENT_UNKNOWN)
{}


CWindowManagerService::AppWindowAnimParams::AppWindowAnimParams(
    /* [in] */ AppWindowAnimator* appAnimator)
    : mAppAnimator(appAnimator)
{
    AutoPtr<AppWindowToken> atoken = appAnimator->GetAppToken();
    if (atoken) {
        List< AutoPtr<WindowState> >::Iterator it = atoken->mAllAppWindows.Begin();
        for (; it != atoken->mAllAppWindows.End(); ++it) {
            mWinAnimators.PushBack((*it)->mWinAnimator);
        }
    }
}

CWindowManagerService::AppWindowAnimParams::~AppWindowAnimParams()
{
    mWinAnimators.Clear();
}


const Int64 CWindowManagerService::LayoutToAnimatorParams::WALLPAPER_TOKENS_CHANGED = 1 << 0;

CWindowManagerService::LayoutToAnimatorParams::LayoutToAnimatorParams()
    : mParamsModified(FALSE)
    , mChanges(0)
    , mAnimationScheduled(FALSE)
{ }

CWindowManagerService::LayoutToAnimatorParams::~LayoutToAnimatorParams()
{
    mWinAnimatorLists.Clear();
    mDimParams.Clear();
    mWallpaperTokens.Clear();
    mAppWindowAnimParams.Clear();
}

CWindowManagerService::DragInputEventReceiver::DragInputEventReceiver(
    /* [in] */ IInputChannel* inputChannel,
    /* [in] */ ILooper* looper,
    /* [in] */ CWindowManagerService* host)
    : InputEventReceiver(inputChannel, looper)
    , mHost(host)
{}

ECode CWindowManagerService::DragInputEventReceiver::OnInputEvent(
    /* [in] */ IInputEvent* event)
{
    VALIDATE_NOT_NULL(event);

    Boolean handled = FALSE;
    // try {
    Int32 source;
    event->GetSource(&source);
    if (IMotionEvent::Probe(event) != NULL
            && (source & IInputDevice::SOURCE_CLASS_POINTER) != 0
            && mHost->mDragState != NULL) {
        AutoPtr<IMotionEvent> motionEvent = IMotionEvent::Probe(event);
        Boolean endDrag = FALSE;
        Float newX;
        motionEvent->GetRawX(&newX);
        Float newY;
        motionEvent->GetRawY(&newY);

        Int32 action;
        motionEvent->GetAction(&action);
        switch (action) {
        case IMotionEvent::ACTION_DOWN: {
            if (CWindowManagerService::DEBUG_DRAG) {
                Slogger::W(CWindowManagerService::TAG, "Unexpected ACTION_DOWN in drag layer");
            }
        } break;

        case IMotionEvent::ACTION_MOVE: {
            {
                AutoLock lock(mHost->mWindowMapLock);
                // move the surface and tell the involved window(s) where we are
                mHost->mDragState->NotifyMoveLw(newX, newY);
            }
        } break;

        case IMotionEvent::ACTION_UP: {
            if (CWindowManagerService::DEBUG_DRAG) Slogger::D(CWindowManagerService::TAG, "Got UP on move channel; dropping at %f,%f",
                    newX, newY);
            {
                AutoLock lock(mHost->mWindowMapLock);
                endDrag = mHost->mDragState->NotifyDropLw(newX, newY);
            }
        } break;

        case IMotionEvent::ACTION_CANCEL: {
            if (CWindowManagerService::DEBUG_DRAG) Slogger::D(CWindowManagerService::TAG, "Drag cancelled!");
            endDrag = TRUE;
        } break;
        }

        if (endDrag) {
            if (CWindowManagerService::DEBUG_DRAG) Slogger::D(CWindowManagerService::TAG, "Drag ended; tearing down state");
            // tell all the windows that the drag has ended
            {
                AutoLock lock(mHost->mWindowMapLock);
                mHost->mDragState->EndDragLw();
            }
        }

        handled = TRUE;
    }
    // } catch (Exception e) {
    //     Slog.e(TAG, "Exception caught by drag handleMotion", e);
    // } finally {
    FinishInputEvent(event, handled);
    // }
    return NOERROR;
}


CWindowManagerService::WindowManagerServiceCreator::WindowManagerServiceCreator(
    /* [in] */ IContext* context,
    /* [in] */ CPowerManagerService* pm,
    /* [in] */ CDisplayManagerService* dm,
    /* [in] */ CInputManagerService* im,
    /* [in] */ IHandler* uiHandler,
    /* [in] */ Boolean haveInputMethods,
    /* [in] */ Boolean showBootMsgs,
    /* [in] */ Boolean onlyCore)
    : mContext(context)
    , mPm(pm)
    , mDm(dm)
    , mIm(im)
    , mUiHandler(uiHandler)
    , mHaveInputMethods(haveInputMethods)
    , mShowBootMsgs(showBootMsgs)
    , mOnlyCore(onlyCore)
{}

UInt32 CWindowManagerService::WindowManagerServiceCreator::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CWindowManagerService::WindowManagerServiceCreator::Release()
{
    return ElRefBase::Release();
}

PInterface CWindowManagerService::WindowManagerServiceCreator::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IRunnable*)this;
    }
    else if (riid == Elastos::Core::EIID_IRunnable) {
        return (IRunnable*)this;
    }

    return NULL;
}

ECode CWindowManagerService::WindowManagerServiceCreator::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [in] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowManagerService::WindowManagerServiceCreator::Run()
{
    return CWindowManagerService::NewByFriend(mContext, (Handle32)mPm.Get(), (Handle32)mDm.Get(), (Handle32)mIm.Get(), mUiHandler,
            mHaveInputMethods, mShowBootMsgs, mOnlyCore, (CWindowManagerService**)&mInstance);
}


CWindowManagerService::PolicyInitializer::PolicyInitializer(
    /* [in] */ CWindowManagerService* wmService)
    : mHost(wmService)
{}

UInt32 CWindowManagerService::PolicyInitializer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CWindowManagerService::PolicyInitializer::Release()
{
    return ElRefBase::Release();
}

PInterface CWindowManagerService::PolicyInitializer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IRunnable*)this;
    }
    else if (riid == Elastos::Core::EIID_IRunnable) {
        return (IRunnable*)this;
    }

    return NULL;
}

ECode CWindowManagerService::PolicyInitializer::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [in] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowManagerService::PolicyInitializer::Run()
{
    // TODO:
    // WindowManagerPolicyThread.set(Thread::GetCurrentThread(), Looper::GetMyLooper());

    FAIL_RETURN(mHost->mPolicy->Init(mHost->mContext,
            (IIWindowManager*)mHost->Probe(EIID_IIWindowManager),
            (IWindowManagerPolicyWindowManagerFuncs*)mHost->Probe(EIID_IWindowManagerPolicyWindowManagerFuncs)));
    Int32 layer;
    mHost->mPolicy->GetAboveUniverseLayer(&layer);
    mHost->mAnimator->mAboveUniverseLayer = layer * TYPE_LAYER_MULTIPLIER
            + TYPE_LAYER_OFFSET;
    return NOERROR;
}


CAR_INTERFACE_IMPL(CWindowManagerService::RotationWatcherDeathRecipint, IProxyDeathRecipient);

CWindowManagerService::RotationWatcherDeathRecipint::RotationWatcherDeathRecipint(
    /* [in] */ CWindowManagerService* owner,
    /* [in] */ IBinder* watcherBinder)
    : mOwner(owner)
    , mWatcherBinder(watcherBinder)
{
}

ECode CWindowManagerService::RotationWatcherDeathRecipint::ProxyDied()
{
    AutoLock lock(mOwner->mWindowMapLock);

    List<AutoPtr<IRotationWatcher> >::Iterator iter = mOwner->mRotationWatchers.Begin();
    while (iter != mOwner->mRotationWatchers.End()) {
        AutoPtr<IRotationWatcher> watcher = *iter;
        if (mWatcherBinder.Get() == IBinder::Probe(watcher)) {
            if (watcher != NULL) {
                AutoPtr<IProxy> proxy = (IProxy*)watcher->Probe(EIID_IProxy);
                assert(proxy != NULL);
                Boolean res;
                proxy->UnlinkToDeath(this, 0, &res);
            }
            iter = mOwner->mRotationWatchers.Erase(iter);
        }
        else
            ++iter;
    }
    return NOERROR;
}

ECode CWindowManagerService::LocalBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (IDevicePolicyManager::ACTION_DEVICE_POLICY_MANAGER_STATE_CHANGED.Equals(action)) {
        Boolean result;
        mHost->mKeyguardDisableHandler->SendEmptyMessage(
            KeyguardDisableHandler::KEYGUARD_POLICY_CHANGED, &result);
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL(CWindowManagerService::LocalInterpolator, IInterpolator);

ECode CWindowManagerService::LocalInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);

    if (input < RECENTS_THUMBNAIL_FADEOUT_FRACTION) {
        // linear response
        *output = input / RECENTS_THUMBNAIL_FADEOUT_FRACTION;
        return NOERROR;
    }
    // complete
    *output = 1;
    return NOERROR;
}

CAR_INTERFACE_IMPL(CWindowManagerService::SecurelyOnKeyguardExitResult, IOnKeyguardExitResult);

ECode CWindowManagerService::SecurelyOnKeyguardExitResult::OnKeyguardExitResult(
    /* [in] */ Boolean success)
{
    // try {
    return mCallback->OnKeyguardExitResult(success);
    // } catch (RemoteException e) {
    //     // Client has died, we don't care.
    // }
}


const String CWindowManagerService::TAG("CWindowManager");
const Boolean CWindowManagerService::DEBUG = FALSE;
const Boolean CWindowManagerService::DEBUG_ADD_REMOVE = FALSE;
const Boolean CWindowManagerService::DEBUG_FOCUS = FALSE;
const Boolean CWindowManagerService::DEBUG_ANIM = FALSE;
const Boolean CWindowManagerService::DEBUG_LAYOUT = FALSE;
const Boolean CWindowManagerService::DEBUG_RESIZE = FALSE;
const Boolean CWindowManagerService::DEBUG_LAYERS = FALSE;
const Boolean CWindowManagerService::DEBUG_INPUT = FALSE;
const Boolean CWindowManagerService::DEBUG_INPUT_METHOD = FALSE;
const Boolean CWindowManagerService::DEBUG_VISIBILITY = FALSE;
const Boolean CWindowManagerService::DEBUG_WINDOW_MOVEMENT = FALSE;
const Boolean CWindowManagerService::DEBUG_TOKEN_MOVEMENT = FALSE;
const Boolean CWindowManagerService::DEBUG_ORIENTATION = FALSE;
const Boolean CWindowManagerService::DEBUG_APP_ORIENTATION = FALSE;
const Boolean CWindowManagerService::DEBUG_CONFIGURATION = FALSE;
const Boolean CWindowManagerService::DEBUG_APP_TRANSITIONS = FALSE;
const Boolean CWindowManagerService::DEBUG_STARTING_WINDOW = TRUE;
const Boolean CWindowManagerService::DEBUG_REORDER = FALSE;
const Boolean CWindowManagerService::DEBUG_WALLPAPER = FALSE;
const Boolean CWindowManagerService::DEBUG_WALLPAPER_LIGHT = FALSE || DEBUG_WALLPAPER;
const Boolean CWindowManagerService::DEBUG_DRAG = FALSE;
const Boolean CWindowManagerService::DEBUG_SCREEN_ON = FALSE;
const Boolean CWindowManagerService::DEBUG_SCREENSHOT = FALSE;
const Boolean CWindowManagerService::DEBUG_BOOT = FALSE;
const Boolean CWindowManagerService::DEBUG_LAYOUT_REPEATS = TRUE;
const Boolean CWindowManagerService::DEBUG_SURFACE_TRACE = FALSE;
const Boolean CWindowManagerService::DEBUG_WINDOW_TRACE = FALSE;

const Boolean CWindowManagerService::SHOW_SURFACE_ALLOC = FALSE;
const Boolean CWindowManagerService::SHOW_TRANSACTIONS = FALSE;
const Boolean CWindowManagerService::SHOW_LIGHT_TRANSACTIONS = FALSE || SHOW_TRANSACTIONS;
const Boolean CWindowManagerService::HIDE_STACK_CRAWLS = TRUE;
const Int32 CWindowManagerService::LAYOUT_REPEAT_THRESHOLD = 4;
const Boolean CWindowManagerService::PROFILE_ORIENTATION = FALSE;
const Boolean CWindowManagerService::mLocalLOGV = DEBUG;
const Int32 CWindowManagerService::TYPE_LAYER_MULTIPLIER = 10000;
const Int32 CWindowManagerService::TYPE_LAYER_OFFSET = 1000;
const Int32 CWindowManagerService::WINDOW_LAYER_MULTIPLIER = 5;
const Int32 CWindowManagerService::LAYER_OFFSET_DIM = 1;
const Int32 CWindowManagerService::LAYER_OFFSET_BLUR = 2;
const Int32 CWindowManagerService::LAYER_OFFSET_THUMBNAIL = WINDOW_LAYER_MULTIPLIER - 1;
const Int32 CWindowManagerService::FREEZE_LAYER = (TYPE_LAYER_MULTIPLIER * 200) + 1;
const Int32 CWindowManagerService::MASK_LAYER = TYPE_LAYER_MULTIPLIER * 200;
const Int32 CWindowManagerService::MAX_ANIMATION_DURATION = 10*1000;
const Int32 CWindowManagerService::DEFAULT_DIM_DURATION = 200;
const Int32 CWindowManagerService::DEFAULT_FADE_IN_OUT_DURATION = 400;
const Int32 CWindowManagerService::WINDOW_FREEZE_TIMEOUT_DURATION = 2000;
const Float CWindowManagerService::RECENTS_THUMBNAIL_FADEOUT_FRACTION = 0.25f;
const Boolean CWindowManagerService::CUSTOM_SCREEN_ROTATION = TRUE;
const Int64 CWindowManagerService::DEFAULT_INPUT_DISPATCHING_TIMEOUT_NANOS = 5000 * 1000000LL;
const Int32 CWindowManagerService::UPDATE_FOCUS_NORMAL = 0;
const Int32 CWindowManagerService::UPDATE_FOCUS_WILL_ASSIGN_LAYERS = 1;
const Int32 CWindowManagerService::UPDATE_FOCUS_PLACING_SURFACES = 2;
const Int32 CWindowManagerService::UPDATE_FOCUS_WILL_PLACE_SURFACES = 3;
const Int64 CWindowManagerService::WALLPAPER_TIMEOUT = 150LL;
const Int64 CWindowManagerService::WALLPAPER_TIMEOUT_RECOVERY = 10000LL;
const Int32 CWindowManagerService::ADJUST_WALLPAPER_LAYERS_CHANGED = 1<<1;
const Int32 CWindowManagerService::ADJUST_WALLPAPER_VISIBILITY_CHANGED = 1<<2;
const Int32 CWindowManagerService::INPUT_DEVICES_READY_FOR_SAFE_MODE_DETECTION_TIMEOUT_MILLIS = 1000;
const String CWindowManagerService::SYSTEM_SECURE("ro.secure");
const String CWindowManagerService::SYSTEM_DEBUGGABLE("ro.debuggable");
const Float CWindowManagerService::THUMBNAIL_ANIMATION_DECELERATE_FACTOR = 1.5f;

static List< AutoPtr<WindowState> >::Iterator Find(
    /* [in] */ List< AutoPtr<WindowState> >& windows,
    /* [in] */ WindowState* win)
{
    List< AutoPtr<WindowState> >::Iterator it = windows.Begin();
    for (; it != windows.End(); ++it) {
        if ((*it).Get() == win) {
            break;
        }
    }
    return it;
}

CWindowManagerService::CWindowManagerService()
    : mCurrentUserId(0)
    , mHaveInputMethods(FALSE)
    , mAllowBootMessages(FALSE)
    , mLimitedAlphaCompositing(FALSE)
    , mDisplayReady(FALSE)
    , mSafeMode(FALSE)
    , mDisplayEnabled(FALSE)
    , mSystemBooted(FALSE)
    , mForceDisplayEnabled(FALSE)
    , mShowingBootMessages(FALSE)
    , mForcedAppOrientation(IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED)
    , mAltOrientation(FALSE)
    , mDeferredRotationPauseCount(0)
    , mSystemDecorLayer(0)
    , mTraversalScheduled(FALSE)
    , mDisplayFrozen(FALSE)
    , mWaitingForConfig(FALSE)
    , mWindowsFreezingScreen(FALSE)
    , mClientFreezingScreen(FALSE)
    , mAppsFreezingScreen(0)
    , mLastWindowForcedOrientation(IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED)
    , mLayoutSeq(0)
    , mLastStatusBarVisibility(0)
    , mFocusMayChange(FALSE)
    , mNextAppTransition(IWindowManagerPolicy::TRANSIT_UNSET)
    , mNextAppTransitionType(IActivityOptions::ANIM_NONE)
    , mNextAppTransitionScaleUp(FALSE)
    , mNextAppTransitionEnter(0)
    , mNextAppTransitionExit(0)
    , mNextAppTransitionStartX(0)
    , mNextAppTransitionStartY(0)
    , mNextAppTransitionStartWidth(0)
    , mNextAppTransitionStartHeight(0)
    , mAppTransitionReady(FALSE)
    , mAppTransitionRunning(FALSE)
    , mAppTransitionTimeout(FALSE)
    , mStartingIconInTransition(FALSE)
    , mSkipAppTransitionAnimation(FALSE)
    , mIsTouchDevice(FALSE)
    , mInputMethodTargetWaitingAnim(FALSE)
    , mInputMethodAnimLayerAdjustment(0)
    , mHardKeyboardAvailable(FALSE)
    , mHardKeyboardEnabled(FALSE)
    , mWallpaperAnimLayerAdjustment(0)
    , mLastWallpaperX(-1)
    , mLastWallpaperY(-1)
    , mLastWallpaperXStep(-1)
    , mLastWallpaperYStep(-1)
    , mLastWallpaperTimeoutTime(0)
    , mWindowAnimationScale(0.5f)
    , mTransitionAnimationScale(0.5f)
    , mAnimatorDurationScale(0.5f)
    , mTurnOnScreen(FALSE)
    , mInTouchMode(TRUE)
    , mCompatibleScreenScale(0)
    , mOnlyCore(FALSE)
    , mHeadless(FALSE)
    , mTransactionSequence(0)
    , mLayoutRepeatCount(0)
    , mWindowsChanged(FALSE)
    , mEventDispatchingEnabled(FALSE)
    , mInLayout(FALSE)
{
    mBroadcastReceiver = new LocalBroadcastReceiver(this);

    AutoPtr<IPolicyManager> pm;
    ASSERT_SUCCEEDED(CPolicyManager::AcquireSingleton((IPolicyManager**)&pm));
    ASSERT_SUCCEEDED(pm->MakeNewWindowManager((IWindowManagerPolicy**)&mPolicy));

    mPendingRemoveTmp = ArrayOf<WindowState*>::Alloc(20);
    mRebuildTmp = ArrayOf<WindowState*>::Alloc(20);
    mTmpFloats = ArrayOf<Float>::Alloc(9);

    ASSERT_SUCCEEDED(CConfiguration::New((IConfiguration**)&mCurConfiguration));

    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    sysProp->GetInt32(String("ro.display.defaultorientation"), 0, &mRotation);

    ASSERT_SUCCEEDED(CRect::New((IRect**)&mSystemDecorRect));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mScreenRect));

    ASSERT_SUCCEEDED(CDisplayMetrics::New((IDisplayMetrics**)&mDisplayMetrics));
    ASSERT_SUCCEEDED(CDisplayMetrics::New((IDisplayMetrics**)&mRealDisplayMetrics));
    ASSERT_SUCCEEDED(CDisplayMetrics::New((IDisplayMetrics**)&mTmpDisplayMetrics));
    ASSERT_SUCCEEDED(CDisplayMetrics::New((IDisplayMetrics**)&mCompatDisplayMetrics));

    mH = new H(this);

    AutoPtr<IChoreographerHelper> helper;
    ASSERT_SUCCEEDED(CChoreographerHelper::AcquireSingleton(
            (IChoreographerHelper**)&helper));
    helper->GetInstance((IChoreographer**)&mChoreographer);

    mInnerFields = new LayoutFields();
    mLayoutToAnim = new LayoutToAnimatorParams();

    ASSERT_SUCCEEDED(CRegion::New((IRegion**)&mTempRegion));
    ASSERT_SUCCEEDED(CConfiguration::New((IConfiguration**)&mTempConfiguration));
    mInputMonitor = new InputMonitor(this);
}

CWindowManagerService::~CWindowManagerService()
{
    mForceRemoves = NULL;
}

AutoPtr<CWindowManagerService> CWindowManagerService::Main(
    /* [in] */ IContext* context,
    /* [in] */ CPowerManagerService* pm,
    /* [in] */ CDisplayManagerService* dm,
    /* [in] */ CInputManagerService* im,
    /* [in] */ IHandler* uiHandler,
    /* [in] */ IHandler* wmHandler,
    /* [in] */ Boolean haveInputMethods,
    /* [in] */ Boolean showBootMsgs,
    /* [in] */ Boolean onlyCore)
{
    AutoPtr<WindowManagerServiceCreator> createServiceRunnable =
            new WindowManagerServiceCreator(context, pm,
            dm, im, uiHandler, haveInputMethods, showBootMsgs, onlyCore);
    Boolean result;
    wmHandler->RunWithScissors((IRunnable*)createServiceRunnable.Get(), 0, &result);
    assert(createServiceRunnable->mInstance != NULL);
    return createServiceRunnable->mInstance;
}

void CWindowManagerService::InitPolicy(
    /* [in] */ IHandler* uiHandler)
{
    AutoPtr<IRunnable> runnable = new PolicyInitializer(this);
    Boolean result;
    uiHandler->RunWithScissors(runnable, 0, &result);
}

ECode CWindowManagerService::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Handle32 pm,
    /* [in] */ Handle32 displayManager,
    /* [in] */ Handle32 inputManager,
    /* [in] */ IHandler* uiHandler,
    /* [in] */ Boolean haveInputMethods,
    /* [in] */ Boolean showBootMsgs,
    /* [in] */ Boolean onlyCore)
{
    mContext = context;
    mHaveInputMethods = haveInputMethods;
    mAllowBootMessages = showBootMsgs;
    mOnlyCore = onlyCore;
    AutoPtr<IResources> resources;
    ASSERT_SUCCEEDED(mContext->GetResources((IResources**)&resources));
    resources->GetBoolean(R::bool_::config_sf_limitedAlpha, &mLimitedAlphaCompositing);
    mDisplayManagerService = (CDisplayManagerService*)displayManager;
    mDisplayManagerService->IsHeadless(&mHeadless);

    ASSERT_SUCCEEDED(context->GetSystemService(
       IContext::DISPLAY_SERVICE, (IInterface**)&mDisplayManager));
    mDisplayManager->RegisterDisplayListener(
            (IDisplayListener*)this->Probe(EIID_IDisplayListener), NULL);
    AutoPtr< ArrayOf<IDisplay*> > displays;
    mDisplayManager->GetDisplays((ArrayOf<IDisplay*>**)&displays);
    for (Int32 i = 0; i < displays->GetLength(); ++i) {
        AutoPtr<IDisplay> display = (*displays)[i];
        CreateDisplayContentLocked(display);
    }

    mKeyguardDisableHandler = new KeyguardDisableHandler(mContext, mPolicy);

    mPowerManager = (CPowerManagerService*)pm;
    mPowerManager->SetPolicy(mPolicy);
    AutoPtr<IPowerManager> pmc;
    ASSERT_SUCCEEDED(context->GetSystemService(IContext::POWER_SERVICE,
            (IInterface**)&pmc));
    pmc->NewWakeLock(IPowerManager::PARTIAL_WAKE_LOCK,
            String("SCREEN_FROZEN"), (IPowerManagerWakeLock**)&mScreenFrozenLock);
    mScreenFrozenLock->SetReferenceCounted(FALSE);

    mActivityManager = ActivityManagerNative::GetDefault();
    mBatteryStats = CBatteryStatsService::GetService();

    // Get persisted window scale setting
    AutoPtr<ISettingsGlobal> sg;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sg);
    AutoPtr<IContentResolver> cr;
    context->GetContentResolver((IContentResolver**)&cr);
    sg->GetFloat(cr, ISettingsGlobal::WINDOW_ANIMATION_SCALE,
            mWindowAnimationScale, &mWindowAnimationScale);
    sg->GetFloat(cr, ISettingsGlobal::TRANSITION_ANIMATION_SCALE,
            mAnimatorDurationScale, &mTransitionAnimationScale);
    Float value;
    sg->GetFloat(cr, ISettingsGlobal::ANIMATOR_DURATION_SCALE,
            mAnimatorDurationScale, &value);
    SetAnimatorDurationScale(value);

    // Track changes to DevicePolicyManager state so we can enable/disable keyguard.
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(IDevicePolicyManager::ACTION_DEVICE_POLICY_MANAGER_STATE_CHANGED);
    AutoPtr<IIntent> intent;
    mContext->RegisterReceiver(mBroadcastReceiver, filter, (IIntent**)&intent);

    pmc->NewWakeLock(IPowerManager::SCREEN_BRIGHT_WAKE_LOCK
            | IPowerManager::ON_AFTER_RELEASE, TAG,
            (IPowerManagerWakeLock**)&mHoldingScreenWakeLock);
    mHoldingScreenWakeLock->SetReferenceCounted(FALSE);

    mInputManager = (CInputManagerService*)inputManager;
    ASSERT_SUCCEEDED(CSurfaceSession::New((ISurfaceSession**)&mFxSession));
    mAnimator = new WindowAnimator(this);

    InitPolicy(uiHandler);

    // Add ourself to the Watchdog monitors.
    // TODO:
    // Watchdog.getInstance().addMonitor(this);

    AutoPtr<ISurfaceHelper> surfaceHelper;
    CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&surfaceHelper);
    surfaceHelper->OpenTransaction();
    //try {
    CreateWatermarkInTransaction();
    //} finally {
    surfaceHelper->CloseTransaction();
    //}

    // for surfaceflinger to get service window
    mWindowBinder = new WindowBinder();
    mWindowBinder->Register();
    return NOERROR;
}

AutoPtr<InputMonitor> CWindowManagerService::GetInputMonitor()
{
    return mInputMonitor;
}

void CWindowManagerService::PlaceWindowAfter(
    /* [in] */ WindowState* pos,
    /* [in] */ WindowState* window)
{
    List< AutoPtr<WindowState> >& windows = pos->GetWindowList();
    List< AutoPtr<WindowState> >::Iterator it = Find(windows, pos);
    if (DEBUG_FOCUS || DEBUG_WINDOW_MOVEMENT || DEBUG_ADD_REMOVE) {
        Int32 i = 0;
        for (; i < windows.GetSize(); ++i) {
            if (windows[i].Get() == pos) {
                break;
            }
        }
        Slogger::V(TAG, "Adding window %p at %d of %d (after %p)",
                window, i, windows.GetSize(), pos);
    }
    if (it == windows.End()) {
        windows.PushBack(window);
    }
    else {
        windows.Insert(++it, window);
    }
    mWindowsChanged = TRUE;
}

void CWindowManagerService::PlaceWindowBefore(
    /* [in] */ WindowState* pos,
    /* [in] */ WindowState* window)
{
    List< AutoPtr<WindowState> >& windows = pos->GetWindowList();
    List< AutoPtr<WindowState> >::Iterator it = Find(windows, pos);
    if (DEBUG_FOCUS || DEBUG_WINDOW_MOVEMENT || DEBUG_ADD_REMOVE) {
        Int32 i = 0;
        for (; i < windows.GetSize(); ++i) {
            if (windows[i].Get() == pos) {
                break;
            }
        }
        Slogger::V(TAG, "Adding window %p at %d of %d (before %p)",
                window, i, windows.GetSize(), pos);
    }
    windows.Insert(it, window);
    mWindowsChanged = TRUE;
}

List< AutoPtr<WindowState> >::Iterator
CWindowManagerService::FindIteratorBasedOnAppTokens(
    /* [in] */ WindowState* win)
{
    List< AutoPtr<WindowState> >& windows = win->GetWindowList();
    List< AutoPtr<WindowState> >::ReverseIterator rit = windows.RBegin();
    for(; rit != windows.REnd(); ++rit) {
        AutoPtr<WindowState> wentry = *rit;
        if(wentry->mAppToken == win->mAppToken) {
            return (++rit).GetBase();
        }
    }
    return windows.End();
}

AutoPtr<List<AutoPtr<WindowState> > > CWindowManagerService::GetTokenWindowsOnDisplay(
    /* [in] */ WindowToken* token,
    /* [in] */ DisplayContent* displayContent)
{
    AutoPtr<List<AutoPtr<WindowState> > > windowList = new List< AutoPtr<WindowState> >();
    List< AutoPtr<WindowState> >::Iterator it = token->mWindows.Begin();
    for (; it != token->mWindows.End(); ++it) {
        AutoPtr<WindowState> win = *it;
        if (win->mDisplayContent.Get() == displayContent) {
            windowList->PushBack(win);
        }
    }
    return windowList;
}

List<AutoPtr<WindowState> >::Iterator CWindowManagerService::IndexOfWinInWindowList(
    /* [in] */ WindowState* targetWin,
    /* [in] */ List<AutoPtr<WindowState> >& windows)
{
    List<AutoPtr<WindowState> >::Iterator it = windows.Begin();
    for(; it != windows.End(); it++)
    {
        AutoPtr<WindowState> w = *it;
        if (w.Get() == targetWin)
        {
            return it;
        }
        if (!w->mChildWindows.IsEmpty())
        {
            if(IndexOfWinInWindowList(targetWin, w->mChildWindows) != w->mChildWindows.End())
            {
                return it;
            }
        }
    }
    return windows.End();
}

void CWindowManagerService::AddWindowToListInOrderLocked(
    /* [in] */ WindowState* win,
    /* [in] */ Boolean addToToken)
{
    assert(win != NULL);
    AutoPtr<IIWindow> client = win->mClient;
    AutoPtr<WindowToken> token = win->mToken;
    AutoPtr<DisplayContent> displayContent = win->mDisplayContent;

    List< AutoPtr<WindowState> >& windows = win->GetWindowList();
    AutoPtr<WindowState> attached = win->mAttachedWindow;
    AutoPtr<List<AutoPtr<WindowState> > > tokenWindowList = GetTokenWindowsOnDisplay(token, displayContent);
    if (attached == NULL) {
        List<AutoPtr<WindowState> >::Iterator tokenWindowsPosIt = token->mWindows.Begin();
        if (token->mAppWindowToken != NULL) {
            if (tokenWindowList->Begin() != tokenWindowList->End()) {
                // If this application has existing windows, we
                // simply place the new window on top of them... but
                // keep the starting window on top.
                Int32 winType;
                win->mAttrs->GetType(&winType);
                if (winType == IWindowManagerLayoutParams::TYPE_BASE_APPLICATION) {
                    // Base windows go behind everything else.
                    AutoPtr<WindowState> lowestWindow = *tokenWindowList->Begin();
                    PlaceWindowBefore(lowestWindow, win);
                    tokenWindowsPosIt = IndexOfWinInWindowList(lowestWindow, token->mWindows);
                }
                else {
                    AutoPtr<AppWindowToken> atoken = win->mAppToken;
                    AutoPtr<WindowState> lastWindow = *tokenWindowList->RBegin();
                    if (atoken != NULL && lastWindow == atoken->mStartingWindow) {
                        PlaceWindowBefore(lastWindow, win);
                        tokenWindowsPosIt = IndexOfWinInWindowList(lastWindow, token->mWindows);
                    }
                    else {
                        List< AutoPtr<WindowState> >::Iterator newIdxIt
                                = FindIteratorBasedOnAppTokens(win);
                        //there is a window above this one associated with the same
                        //apptoken note that the window could be a floating window
                        //that was created later or a window at the top of the list of
                        //windows associated with this token.
                        if (DEBUG_FOCUS || DEBUG_WINDOW_MOVEMENT || DEBUG_ADD_REMOVE) {
                            Slogger::V(TAG, "Adding window %p of %d", win, windows.GetSize());
                        }
                        windows.Insert(++newIdxIt, win);
                        if (newIdxIt == win->GetWindowList().End()) {
                            // No window from token found on win's display.
                            tokenWindowsPosIt = token->mWindows.Begin();
                        }
                        else {
                            tokenWindowsPosIt = ++IndexOfWinInWindowList(*newIdxIt, token->mWindows);
                        }
                        mWindowsChanged = true;
                    }
                }
            }
            else {
                // No windows from this token on this display
                if (mLocalLOGV) Slogger::V(
                    TAG, "Figuring out where to add app window %p (token=%p)",
                        client.Get(), token.Get());
                // Figure out where the window should go, based on the
                // order of applications.
                AutoPtr<WindowState> pos;
                List< AutoPtr<AppWindowToken> >::ReverseIterator awtRIt = mAnimatingAppTokens.RBegin();
                for (; awtRIt != mAnimatingAppTokens.REnd(); ++awtRIt) {
                    AutoPtr<AppWindowToken> t = *awtRIt;
                    if (t == token) {
                        ++awtRIt;
                        break;
                    }

                    // We haven't reached the token yet; if this token
                    // is not going to the bottom and has windows on this display, we can
                    // use it as an anchor for when we do reach the token.
                    tokenWindowList = GetTokenWindowsOnDisplay(t, win->mDisplayContent);
                    if (!t->mSendingToBottom && tokenWindowList->Begin() != tokenWindowList->End()) {
                        pos = *(tokenWindowList->Begin());
                    }
                }
                // We now know the index into the apps.  If we found
                // an app window above, that gives us the position; else
                // we need to look some more.
                if (pos != NULL) {
                    // Move behind any windows attached to this one.
                    AutoPtr<WindowToken> atoken;
                    HashMap<AutoPtr<IBinder>, AutoPtr<WindowToken> >::Iterator tmIt
                            = mTokenMap.Find(IBinder::Probe(pos->mClient.Get()));
                    if (tmIt != mTokenMap.End()) {
                        atoken = tmIt->mSecond;
                    }
                    if (atoken != NULL) {
                        tokenWindowList = GetTokenWindowsOnDisplay(atoken, win->mDisplayContent);
                        if (tokenWindowList->Begin() != tokenWindowList->End()) {
                            AutoPtr<WindowState> bottom = *(tokenWindowList->Begin());
                            if (bottom->mSubLayer < 0) {
                                pos = bottom;
                            }
                        }
                    }
                    PlaceWindowBefore(pos, win);
                }
                else {
                    // Continue looking down until we find the first
                    // token that has windows on this display.
                    for (; awtRIt != mAnimatingAppTokens.REnd(); ++awtRIt) {
                        AutoPtr<AppWindowToken> t = *awtRIt;
                        tokenWindowList = GetTokenWindowsOnDisplay(t.Get(),
                                win->mDisplayContent);
                        if (tokenWindowList->Begin() != tokenWindowList->End()) {
                            pos = *(tokenWindowList->RBegin());
                            break;
                        }
                    }
                    if (pos != NULL) {
                        // Move in front of any windows attached to this
                        // one.
                        AutoPtr<WindowToken> atoken;
                        HashMap<AutoPtr<IBinder>, AutoPtr<WindowToken> >::Iterator tmIt
                                = mTokenMap.Find(IBinder::Probe(pos->mClient.Get()));
                        if (tmIt != mTokenMap.End()) {
                            atoken = tmIt->mSecond;
                        }
                        if (atoken != NULL) {
                            if (atoken->mWindows.Begin() != atoken->mWindows.End()) {
                                AutoPtr<WindowState> top = *(atoken->mWindows.RBegin());
                                if (top->mSubLayer >= 0) {
                                    pos = top;
                                }
                            }
                        }
                        PlaceWindowAfter(pos, win);
                    }
                    else {
                        // Just search for the start of this layer.
                        Int32 myLayer = win->mBaseLayer;
                        List< AutoPtr<WindowState> >::Iterator winIt = windows.Begin();
                        for (; winIt != windows.End(); winIt++) {
                            AutoPtr<WindowState> w = *winIt;
                            if (w->mBaseLayer > myLayer) {
                                break;
                            }
                        }
                        if (DEBUG_FOCUS || DEBUG_WINDOW_MOVEMENT || DEBUG_ADD_REMOVE) {
                            Slogger::V(TAG, "Adding window %p  of %d", win, windows.GetSize());
                        }
                        windows.Insert(winIt, win);
                        mWindowsChanged = TRUE;
                    }
                }
            }
        }
        else {
            // Figure out where window should go, based on layer.
            Int32 myLayer = win->mBaseLayer;
            List< AutoPtr<WindowState> >::ReverseIterator winRIt = windows.RBegin();
            for (; winRIt != windows.REnd(); ++winRIt) {
                if ((*winRIt)->mBaseLayer <= myLayer) {
                    break;
                }
            }
            if (DEBUG_FOCUS || DEBUG_WINDOW_MOVEMENT || DEBUG_ADD_REMOVE) {
                Slogger::V(TAG, "Adding window %p of %d", win, windows.GetSize());
            }
            windows.Insert(winRIt.GetBase(), win);
            mWindowsChanged = TRUE;
        }

        if (addToToken) {
            if (DEBUG_ADD_REMOVE) {
                Slogger::V(TAG, "Adding %p to %p", win, token.Get());
            }
            token->mWindows.Insert(tokenWindowsPosIt, win);
        }
    }
    else {
        // Figure out this window's ordering relative to the window
        // it is attached to.
        Int32 sublayer = win->mSubLayer;
        Int32 largestSublayer = Elastos::Core::Math::INT32_MIN_VALUE;
        AutoPtr<WindowState> windowWithLargestSublayer;
        List< AutoPtr<WindowState> >::Iterator twIt = tokenWindowList->Begin();
        for (; twIt != tokenWindowList->End(); ++twIt) {
            AutoPtr<WindowState> w = *twIt;
            Int32 wSublayer = w->mSubLayer;
            if (wSublayer >= largestSublayer) {
                largestSublayer = wSublayer;
                windowWithLargestSublayer = w;
            }
            if (sublayer < 0) {
                // For negative sublayers, we go below all windows
                // in the same sublayer.
                if (wSublayer >= sublayer) {
                    if (addToToken) {
                        if (DEBUG_ADD_REMOVE) Slogger::V(TAG, "Adding %p to %p", win, token.Get());
                        token->mWindows.Insert(twIt, win);
                    }
                    PlaceWindowBefore(wSublayer >= 0 ? attached : w, win);
                    break;
                }
            }
            else {
                // For positive sublayers, we go above all windows
                // in the same sublayer.
                if (wSublayer > sublayer) {
                    if (addToToken) {
                        if (DEBUG_ADD_REMOVE) Slogger::V(TAG, "Adding %p to %p", win, token.Get());
                        token->mWindows.Insert(twIt, win);
                    }
                    PlaceWindowBefore(w, win);
                    break;
                }
            }
        }
        if (twIt == tokenWindowList->End()) {
            if (addToToken) {
                if (DEBUG_ADD_REMOVE) Slogger::V(TAG, "Adding %p to %p", win, token.Get());
                token->mWindows.PushBack(win);
            }
            if (sublayer < 0) {
                PlaceWindowBefore(attached, win);
            }
            else {
                PlaceWindowAfter(largestSublayer >= 0
                        ? windowWithLargestSublayer
                        : attached, win);
            }
        }
    }

    if (win->mAppToken != NULL && addToToken) {
        win->mAppToken->mAllAppWindows.PushBack(win);
    }
}

Boolean CWindowManagerService::CanBeImeTarget(
    /* [in] */ WindowState* w)
{
    Int32 flags, type;
    w->mAttrs->GetFlags(&flags);
    Int32 fl = flags
            & (IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE | IWindowManagerLayoutParams::FLAG_ALT_FOCUSABLE_IM);
    if (fl == 0
            || fl == (IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE | IWindowManagerLayoutParams::FLAG_ALT_FOCUSABLE_IM)
            || (w->mAttrs->GetType(&type), type == IWindowManagerLayoutParams::TYPE_APPLICATION_STARTING)) {
        if (DEBUG_INPUT_METHOD) {
            Slogger::I(TAG, "isVisibleOrAdding %p: %d", w, w->IsVisibleOrAdding());
            if (!w->IsVisibleOrAdding()) {
                Slogger::I(TAG, "  mSurface=%p relayoutCalled=%d viewVis=%d policyVis=%d policyVisAfterAnim=%d attachHid=%d exiting=%d destroying=%d",
                        w->mWinAnimator->mSurface.Get(),
                        w->mRelayoutCalled, w->mViewVisibility,
                        w->mPolicyVisibility,
                        w->mPolicyVisibilityAfterAnim,
                        w->mAttachedHidden,
                        w->mExiting, w->mDestroying);
                if (w->mAppToken != NULL) {
                    Slogger::I(TAG, "  mAppToken.hiddenRequested=%d", w->mAppToken->mHiddenRequested);
                }
            }
        }
        return w->IsVisibleOrAdding();
    }
    return FALSE;
}

List< AutoPtr<WindowState> >::Iterator CWindowManagerService::FindDesiredInputMethodWindowItLocked(
    /* [in] */ Boolean willMove)
{
    // TODO(multidisplay): Needs some serious rethought when the target and IME are not on the
    // same display. Or even when the current IME/target are not on the same screen as the next
    // IME/target. For now only look for input windows on the main screen.
    List< AutoPtr<WindowState> >& windows = GetDefaultWindowListLocked();
    List< AutoPtr<WindowState> >::ReverseIterator rit = windows.RBegin();
    AutoPtr<WindowState> w;
    for (; rit != windows.REnd(); ++rit) {
        w = *rit;

        // if (DEBUG_INPUT_METHOD && willMove) Slog.i(TAG, "Checking window @" + i
        //         + " " + w + " fl=0x" + Integer.toHexString(w.mAttrs.flags));
        if (CanBeImeTarget(w)) {
            // Slogger::I(TAG, "Putting input method here!");

            // Yet more tricksyness!  If this window is a "starting"
            // window, we do actually want to be on top of it, but
            // it is not -really- where input will go.  So if the caller
            // is not actually looking to move the IME, look down below
            // for a real window to target...
            Int32 type;
            if (!willMove
                    && (w->mAttrs->GetType(&type), type == IWindowManagerLayoutParams::TYPE_APPLICATION_STARTING)
                    && rit != --windows.REnd()) {
                List< AutoPtr<WindowState> >::ReverseIterator trit = rit;
                AutoPtr<WindowState> wb = *(++trit);
                if (wb->mAppToken == w->mAppToken && CanBeImeTarget(wb)) {
                    ++rit;
                    w = wb;
                }
            }
            break;
        }
    }

    // Now w is either mWindows[0] or an IME (or null if mWindows is empty).

    if (DEBUG_INPUT_METHOD && willMove) Slogger::V(TAG, "Proposed new IME target: %p", w.Get());

    // Now, a special case -- if the last target's window is in the
    // process of exiting, and is above the new target, keep on the
    // last target to avoid flicker.  Consider for example a Dialog with
    // the IME shown: when the Dialog is dismissed, we want to keep
    // the IME above it until it is completely gone so it doesn't drop
    // behind the dialog or its full-screen scrim.
    AutoPtr<WindowState> curTarget = mInputMethodTarget;
    if (curTarget != NULL && w != NULL) {
        Boolean isDisplayed;
        curTarget->IsDisplayedLw(&isDisplayed);
        if (isDisplayed && curTarget->IsClosing()
                && (curTarget->mWinAnimator->mAnimLayer > w->mWinAnimator->mAnimLayer)) {
            if (DEBUG_INPUT_METHOD) Slogger::V(TAG, "Current target higher, not changing");
            List< AutoPtr<WindowState> >::Iterator result = Find(windows, curTarget);
            return ++result;
        }
    }

    if (DEBUG_INPUT_METHOD) Slogger::V(TAG, "Desired input method target=%p willMove=%d",
            w.Get(), willMove);

    if (willMove && w != NULL) {
        AutoPtr<AppWindowToken> token = curTarget == NULL ? NULL : curTarget->mAppToken;
        if (token != NULL) {

            // Now some fun for dealing with window animations that
            // modify the Z order.  We need to look at all windows below
            // the current target that are in this app, finding the highest
            // visible one in layering.
            AutoPtr<WindowState> highestTarget;
            List< AutoPtr<WindowState> >::Iterator highestPosIt;
            if (token->mAppAnimator->mAnimating || token->mAppAnimator->mAnimation != NULL) {
                List< AutoPtr<WindowState> >& curWindows = curTarget->GetWindowList();
                List< AutoPtr<WindowState> >::ReverseIterator posRIt(Find(curWindows, curTarget));
                while (posRIt != curWindows.REnd()) {
                    AutoPtr<WindowState> win = *posRIt;
                    if (win->mAppToken != token) {
                        break;
                    }
                    if (!win->mRemoved) {
                        if (highestTarget == NULL || win->mWinAnimator->mAnimLayer >
                                highestTarget->mWinAnimator->mAnimLayer) {
                            highestTarget = win;
                            highestPosIt = --(posRIt.GetBase());
                        }
                    }
                    ++posRIt;
                }
            }

            if (highestTarget != NULL) {
                if (DEBUG_INPUT_METHOD) Slogger::V(TAG, "mNextAppTransition=%d %p animating=%d layer=%d new layer=%d",
                        mNextAppTransition, highestTarget.Get(),
                        highestTarget->mWinAnimator->IsAnimating(),
                        highestTarget->mWinAnimator->mAnimLayer,
                        w->mWinAnimator->mAnimLayer);

                if (mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET) {
                    // If we are currently setting up for an animation,
                    // hold everything until we can find out what will happen.
                    mInputMethodTargetWaitingAnim = TRUE;
                    mInputMethodTarget = highestTarget;
                    return ++highestPosIt;
                }
                else if (highestTarget->mWinAnimator->IsAnimating() &&
                        highestTarget->mWinAnimator->mAnimLayer > w->mWinAnimator->mAnimLayer) {
                    // If the window we are currently targeting is involved
                    // with an animation, and it is on top of the next target
                    // we will be over, then hold off on moving until
                    // that is done.
                    mInputMethodTargetWaitingAnim = TRUE;
                    mInputMethodTarget = highestTarget;
                    return ++highestPosIt;
                }
            }
        }
    }

    // Slog.i(TAG, "Placing input method @" + (i+1));
    if (w != NULL) {
        if (willMove) {
            // if (DEBUG_INPUT_METHOD) Slog.w(TAG, "Moving IM target from " + curTarget + " to "
            //         + w + (HIDE_STACK_CRAWLS ? "" : " Callers=" + Debug.getCallers(4)));
            mInputMethodTarget = w;
            mInputMethodTargetWaitingAnim = FALSE;
            if (w->mAppToken != NULL) {
                SetInputMethodAnimLayerAdjustment(w->mAppToken->mAppAnimator->mAnimLayerAdjustment);
            }
            else {
                SetInputMethodAnimLayerAdjustment(0);
            }
        }
        return rit.GetBase();
    }
    if (willMove) {
        // if (DEBUG_INPUT_METHOD) Slog.w(TAG, "Moving IM target from " + curTarget + " to null."
        //             + (HIDE_STACK_CRAWLS ? "" : " Callers=" + Debug.getCallers(4)));
        mInputMethodTarget = NULL;
        SetInputMethodAnimLayerAdjustment(0);
    }

    return windows.End();
}

void CWindowManagerService::AddInputMethodWindowToListLocked(
    /* [in] */ WindowState* win)
{
    List< AutoPtr<WindowState> >& windows = GetDefaultWindowListLocked();
    List< AutoPtr<WindowState> >::Iterator posIt = FindDesiredInputMethodWindowItLocked(TRUE);
    if (posIt != windows.End()) {
        win->mTargetAppToken = mInputMethodTarget->mAppToken;
        // if (DEBUG_WINDOW_MOVEMENT) Slog.v(
        //         TAG, "Adding input method window " + win + " at " + pos);
        windows.Insert(posIt, win);
        mWindowsChanged = TRUE;
        MoveInputMethodDialogsLocked(++posIt);
        return;
    }
    win->mTargetAppToken = NULL;
    AddWindowToListInOrderLocked(win, TRUE);
    MoveInputMethodDialogsLocked(posIt);
}

void CWindowManagerService::SetInputMethodAnimLayerAdjustment(
    /* [in] */ Int32 adj)
{
    if (DEBUG_LAYERS) Slogger::V(TAG, "Setting im layer adj to %d", adj);
    mInputMethodAnimLayerAdjustment = adj;
    AutoPtr<WindowState> imw = mInputMethodWindow;
    List< AutoPtr<WindowState> >::ReverseIterator rit;
    if (imw != NULL) {
        imw->mWinAnimator->mAnimLayer = imw->mLayer + adj;
        if (DEBUG_LAYERS) Slogger::V(TAG, "IM win %p anim layer: %d", imw.Get(),
                imw->mWinAnimator->mAnimLayer);
        for (rit = imw->mChildWindows.RBegin(); rit != imw->mChildWindows.REnd(); ++rit) {
            AutoPtr<WindowState> cw = *rit;
            cw->mWinAnimator->mAnimLayer = cw->mLayer + adj;
            if (DEBUG_LAYERS) Slogger::V(TAG, "IM win %p anim layer: %d", cw.Get(),
                    cw->mWinAnimator->mAnimLayer);
        }
    }
    for (rit = mInputMethodDialogs.RBegin(); rit != mInputMethodDialogs.REnd(); ++rit) {
        imw = *rit;
        imw->mWinAnimator->mAnimLayer = imw->mLayer + adj;
        if (DEBUG_LAYERS) Slogger::V(TAG, "IM win %p anim layer: %d", imw.Get(),
                imw->mWinAnimator->mAnimLayer);
    }
}

List< AutoPtr<WindowState> >::Iterator CWindowManagerService::TmpRemoveWindowLocked(
    /* [in] */ List< AutoPtr<WindowState> >::Iterator interestingPosIt,
    /* [in] */ WindowState* win)
{
    AutoPtr<WindowState> aWin = win;
    List< AutoPtr<WindowState> >& windows = win->GetWindowList();
    List< AutoPtr<WindowState> >::Iterator wposIt =
            Find(windows.Begin(), windows.End(), aWin);
    if (wposIt != windows.End()) {
        if (DEBUG_WINDOW_MOVEMENT) Slogger::V(TAG, "Temp removing : %p", win);
        windows.Erase(wposIt);
        mWindowsChanged = true;
        List< AutoPtr<WindowState> >::ReverseIterator rit = win->mChildWindows.RBegin();
        for (; rit != win->mChildWindows.REnd(); ++rit) {
            AutoPtr<WindowState> cw = *rit;
            List< AutoPtr<WindowState> >::Iterator cposIt =
                    Find(windows.Begin(), windows.End(), cw);
            if (cposIt != windows.End()) {
                if (DEBUG_WINDOW_MOVEMENT) Slogger::V(TAG, "Temp removing child : %p", cw.Get());
                windows.Erase(cposIt);
            }
        }
    }
    return interestingPosIt;
}

void CWindowManagerService::ReAddWindowToListInOrderLocked(
    /* [in] */ WindowState* win)
{
    AddWindowToListInOrderLocked(win, FALSE);
    // This is a hack to get all of the child windows added as well
    // at the right position.  Child windows should be rare and
    // this case should be rare, so it shouldn't be that big a deal.
    List< AutoPtr<WindowState> >& windows = win->GetWindowList();
    List< AutoPtr<WindowState> >::Iterator it = Find(windows, win);
    if (it != windows.End()) {
        if (DEBUG_WINDOW_MOVEMENT) Slogger::V(TAG, "ReAdd removing from : %p", win);
        windows.Erase(it);
        mWindowsChanged = TRUE;
        ReAddWindowLocked(it, win);
    }
}

void CWindowManagerService::LogWindowList(
    /* [in] */ List< AutoPtr<WindowState> >& windows,
    /* [in] */ const String& prefix)
{
    List< AutoPtr<WindowState> >::ReverseIterator rit;
    Int32 i = 0;
    for (rit = windows.RBegin(); rit != windows.REnd(); ++rit, ++i) {
       Slogger::V(TAG, "%s#: %d, %p", prefix.string(), i, (*rit).Get());
       PFL_EX("%s#: %d, %p", prefix.string(), i, (*rit).Get());
    }
}

void CWindowManagerService::MoveInputMethodDialogsLocked(
    /* [in] */ List< AutoPtr<WindowState> >::Iterator posIt)
{
    List< AutoPtr<WindowState> >& dialogs = mInputMethodDialogs;

    // TODO(multidisplay): IMEs are only supported on the default display.
    List< AutoPtr<WindowState> >& windows = GetDefaultWindowListLocked();
    List< AutoPtr<WindowState> >::Iterator it = dialogs.Begin();
    // if (DEBUG_INPUT_METHOD) Slog.v(TAG, "Removing " + N + " dialogs w/pos=" + pos);
    for (; it != dialogs.End(); ++it) {
        posIt = TmpRemoveWindowLocked(posIt, *it);
    }
    // if (DEBUG_INPUT_METHOD) {
    //     Slog.v(TAG, "Window list w/pos=" + pos);
    //     logWindowList("  ");
    // }

    if (posIt != windows.End()) {
        AutoPtr<AppWindowToken> targetAppToken = mInputMethodTarget->mAppToken;
        if (posIt != windows.End()) {
            AutoPtr<WindowState> wp = *posIt;
            if (wp == mInputMethodWindow) {
                ++posIt;
            }
        }
        // if (DEBUG_INPUT_METHOD) Slog.v(TAG, "Adding " + N + " dialogs at pos=" + pos);
        for (it = dialogs.Begin(); it != dialogs.End(); ++it) {
            AutoPtr<WindowState> win = *it;
            win->mTargetAppToken = targetAppToken;
            posIt = ReAddWindowLocked(posIt, win);
        }
        // if (DEBUG_INPUT_METHOD) {
        //     Slog.v(TAG, "Final window list:");
        //     logWindowList(windows, "  ");
        // }
        return;
    }
    for (it = dialogs.Begin(); it != dialogs.End(); ++it) {
        AutoPtr<WindowState> win = *it;
        win->mTargetAppToken = NULL;
        ReAddWindowToListInOrderLocked(win);
//        if (DEBUG_INPUT_METHOD) {
//            Slog.v(TAG, "No IM target, final list:");
//            logWindowList("  ");
//        }
    }
}

Boolean CWindowManagerService::MoveInputMethodWindowsIfNeededLocked(
    /* [in] */ Boolean needAssignLayers)
{
    AutoPtr<WindowState> imWin = mInputMethodWindow;
    if (imWin == NULL && mInputMethodDialogs.Begin() == mInputMethodDialogs.End()) {
        return FALSE;
    }

    // TODO(multidisplay): IMEs are only supported on the default display.
    List< AutoPtr<WindowState> >& windows = GetDefaultWindowListLocked();
    List< AutoPtr<WindowState> >::Iterator imPosIt = FindDesiredInputMethodWindowItLocked(TRUE);
    if (imPosIt != windows.End()) {
        // In this case, the input method windows are to be placed
        // immediately above the window they are targeting.

        // First check to see if the input method windows are already
        // located here, and contiguous.
        AutoPtr<WindowState> firstImWin = (imPosIt != windows.End()) ? *imPosIt : NULL;

        // Figure out the actual input method window that should be
        // at the bottom of their stack.
        AutoPtr<WindowState> baseImWin = imWin != NULL
                ? imWin : *(mInputMethodDialogs.Begin());
        if (baseImWin->mChildWindows.Begin() != baseImWin->mChildWindows.End()) {
            AutoPtr<WindowState> cw = *baseImWin->mChildWindows.Begin();
            if (cw->mSubLayer < 0) baseImWin = cw;
        }

        if (firstImWin == baseImWin) {
            // The windows haven't moved...  but are they still contiguous?
            // First find the top IM window.
            List< AutoPtr<WindowState> >::Iterator posIt = imPosIt;
            if (++posIt != windows.End()) {
                for (; posIt != windows.End(); ++posIt) {
                    if (!(*posIt)->mIsImWindow) {
                        break;
                    }
                }

                if (posIt != windows.End()) ++posIt;

                // Now there should be no more input method windows above.
                for (; posIt != windows.End(); ++posIt) {
                    if ((*posIt)->mIsImWindow) {
                        break;
                    }
                }
            }

            if (posIt == windows.End()) {
                // All is good!
                return FALSE;
            }
        }

        if (imWin != NULL) {
            // if (DEBUG_INPUT_METHOD) {
            //     Slog.v(TAG, "Moving IM from " + imPos);
            //     logWindowList(windows, "  ");
            // }
            imPosIt = TmpRemoveWindowLocked(imPosIt, imWin);
            // if (DEBUG_INPUT_METHOD) {
            //     Slog.v(TAG, "List after removing with new pos " + imPos + ":");
            //     logWindowList(windows, "  ");
            // }
            imWin->mTargetAppToken = mInputMethodTarget->mAppToken;
            ReAddWindowLocked(imPosIt, imWin);
            // if (DEBUG_INPUT_METHOD) {
            //     Slog.v(TAG, "List after moving IM to " + imPos + ":");
            //     logWindowList(windows, "  ");
            // }
            if (mInputMethodDialogs.Begin() != mInputMethodDialogs.End()) {
                List< AutoPtr<WindowState> >::Iterator temp = imPosIt;
                MoveInputMethodDialogsLocked(++temp);
            }
        }
        else {
            MoveInputMethodDialogsLocked(imPosIt);
        }
    }
    else {
        // In this case, the input method windows go in a fixed layer,
        // because they aren't currently associated with a focus window.
        if (imWin != NULL) {
            // if (DEBUG_INPUT_METHOD) Slog.v(TAG, "Moving IM from " + imPos);
            TmpRemoveWindowLocked(windows.Begin(), imWin);
            imWin->mTargetAppToken = NULL;
            ReAddWindowToListInOrderLocked(imWin);
            // if (DEBUG_INPUT_METHOD) {
            //     Slog.v(TAG, "List with no IM target:");
            //     logWindowList("  ");
            // }
            if (mInputMethodDialogs.Begin() != mInputMethodDialogs.End()) {
                MoveInputMethodDialogsLocked(windows.End());
            }
        }
        else {
            MoveInputMethodDialogsLocked(windows.End());
        }

    }

    if (needAssignLayers) {
        AssignLayersLocked(windows);
    }

    return TRUE;
}

void CWindowManagerService::AdjustInputMethodDialogsLocked()
{
    MoveInputMethodDialogsLocked(FindDesiredInputMethodWindowItLocked(TRUE));
}

Boolean CWindowManagerService::IsWallpaperVisible(
    /* [in] */ WindowState* wallpaperTarget)
{
    // if (DEBUG_WALLPAPER) Slog.v(TAG, "Wallpaper vis: target " + wallpaperTarget + ", obscured="
    //         + (wallpaperTarget != null ? Boolean.toString(wallpaperTarget.mObscured) : "??")
    //         + " anim=" + ((wallpaperTarget != null && wallpaperTarget.mAppToken != null)
    //                 ? wallpaperTarget.mAppToken.mAppAnimator.animation : null)
    //         + " upper=" + mUpperWallpaperTarget
    //         + " lower=" + mLowerWallpaperTarget);
    return (wallpaperTarget != NULL
                    && (!wallpaperTarget->mObscured || (wallpaperTarget->mAppToken != NULL
                            && wallpaperTarget->mAppToken->mAppAnimator->mAnimation != NULL)))
            || mUpperWallpaperTarget != NULL
            || mLowerWallpaperTarget != NULL;
}

Int32 CWindowManagerService::AdjustWallpaperWindowsLocked()
{
    mInnerFields->mWallpaperMayChange = FALSE;
    Int32 changed = 0;
    Boolean targetChanged = FALSE;

    // TODO(multidisplay): Wallpapers on main screen only.
    AutoPtr<IDisplayInfo> displayInfo
            = GetDefaultDisplayContentLocked()->GetDisplayInfo();
    Int32 dw, dh;
    displayInfo->GetAppWidth(&dw);
    displayInfo->GetAppHeight(&dh);

    // First find top-most window that has asked to be on top of the
    // wallpaper; all wallpapers go behind it.
    List< AutoPtr<WindowState> >& windows = GetDefaultWindowListLocked();
    AutoPtr<WindowState> w;
    AutoPtr<WindowState> foundW;
    List< AutoPtr<WindowState> >::Iterator foundIt = windows.Begin();
    AutoPtr<WindowState> topCurW;
    List< AutoPtr<WindowState> >::Iterator topCurIt = windows.Begin();
    List< AutoPtr<WindowState> >::Iterator windowDetachedIt = windows.End();
    List< AutoPtr<WindowState> >::ReverseIterator rit;
    for (rit = windows.RBegin(); rit != windows.REnd(); ++rit) {
        w = *rit;
        Int32 wType;
        w->mAttrs->GetType(&wType);
        if ((wType == IWindowManagerLayoutParams::TYPE_WALLPAPER)) {
            if (topCurW == NULL) {
                topCurW = w;
                topCurIt = --rit.GetBase();
            }
            continue;
        }
        topCurW = NULL;
        if (w != mWindowDetachedWallpaper && w->mAppToken != NULL) {
            // If this window's app token is hidden and not animating,
            // it is of no interest to us.
            if (w->mAppToken->mHidden && w->mAppToken->mAppAnimator->mAnimation == NULL) {
                if (DEBUG_WALLPAPER) Slogger::V(TAG,
                        "Skipping hidden and not animating token: %p", w.Get());
                continue;
            }
        }
        // if (DEBUG_WALLPAPER) Slog.v(TAG, "Win #" + i + " " + w + ": readyfordisplay="
        //         + w.isReadyForDisplay() + " mDrawState=" + w.mWinAnimator.mDrawState);
        Int32 wFlags;
        w->mAttrs->GetFlags(&wFlags);
        if ((wFlags & IWindowManagerLayoutParams::FLAG_SHOW_WALLPAPER) != 0
                && w->IsReadyForDisplay() && (mWallpaperTarget == w || w->IsDrawnLw())) {
            // if (DEBUG_WALLPAPER) Slog.v(TAG,
            //         "Found wallpaper target: #" + i + "=" + w);
            foundW = w;
            foundIt = --rit.GetBase();
            if (w == mWallpaperTarget && w->mWinAnimator->IsAnimating()) {
                // The current wallpaper target is animating, so we'll
                // look behind it for another possible target and figure
                // out what is going on below.
                if (DEBUG_WALLPAPER) Slogger::V(TAG, "Win %p: token animating, looking behind.", w.Get());
                continue;
            }
            break;
        }
        else if (w == mWindowDetachedWallpaper) {
            windowDetachedIt = --rit.GetBase();
        }
    }

    if (foundW == NULL && windowDetachedIt != windows.End()) {
        // if (DEBUG_WALLPAPER_LIGHT) Slog.v(TAG,
        //         "Found animating detached wallpaper activity: #" + i + "=" + w);
        foundW = w;
        foundIt = windowDetachedIt;
    }

    if (mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET) {
        // If we are currently waiting for an app transition, and either
        // the current target or the next target are involved with it,
        // then hold off on doing anything with the wallpaper.
        // Note that we are checking here for just whether the target
        // is part of an app token...  which is potentially overly aggressive
        // (the app token may not be involved in the transition), but good
        // enough (we'll just wait until whatever transition is pending
        // executes).
        if (mWallpaperTarget != NULL && mWallpaperTarget->mAppToken != NULL) {
            if (DEBUG_WALLPAPER_LIGHT) Slogger::V(TAG,
                    "Wallpaper not changing: waiting for app anim in current target");
            return 0;
        }
        if (foundW != NULL && foundW->mAppToken != NULL) {
            if (DEBUG_WALLPAPER) Slogger::V(TAG,
                    "Wallpaper not changing: waiting for app anim in found target");
            return 0;
        }
    }

    if (mWallpaperTarget != foundW
            && (mLowerWallpaperTarget == NULL || mLowerWallpaperTarget != foundW)) {
        if (DEBUG_WALLPAPER_LIGHT) {
            Slogger::V(TAG, "New wallpaper target: %p oldTarget: %p", foundW.Get(),
                    mWallpaperTarget.Get());
        }

        mLowerWallpaperTarget = NULL;
        mUpperWallpaperTarget = NULL;

        AutoPtr<WindowState> oldW = mWallpaperTarget;
        mWallpaperTarget = foundW;
        targetChanged = TRUE;

        // Now what is happening...  if the current and new targets are
        // animating, then we are in our super special mode!
        if (foundW != NULL && oldW != NULL) {
            Boolean oldAnim = oldW->mWinAnimator->mAnimation != NULL
                    || (oldW->mAppToken != NULL
                        && oldW->mAppToken->mAppAnimator->mAnimation != NULL);
            Boolean foundAnim = foundW->mWinAnimator->mAnimation != NULL
                    || (foundW->mAppToken != NULL &&
                        foundW->mAppToken->mAppAnimator->mAnimation != NULL);
            if (DEBUG_WALLPAPER_LIGHT) {
                Slogger::V(TAG, "New animation: %d old animation: %d", foundAnim,
                        oldAnim);
            }
            if (foundAnim && oldAnim) {
                List< AutoPtr<WindowState> >::Iterator oldIt = Find(windows, oldW);
                // if (DEBUG_WALLPAPER_LIGHT) {
                //     Slog.v(TAG, "New i: " + foundI + " old i: " + oldI);
                // }
                if (oldIt != windows.End()) {
                    // if (DEBUG_WALLPAPER_LIGHT) {
                    //     Slog.v(TAG, "Animating wallpapers: old#" + oldI
                    //             + "=" + oldW + "; new#" + foundI
                    //             + "=" + foundW);
                    // }

                    // Set the new target correctly.
                    if (foundW->mAppToken != NULL && foundW->mAppToken->mHiddenRequested) {
                        if (DEBUG_WALLPAPER) {
                            Slogger::V(TAG, "Old wallpaper still the target.");
                        }
                        mWallpaperTarget = oldW;
                        foundW = oldW;
                        foundIt = oldIt;
                    }
                    // Now set the upper and lower wallpaper targets
                    // correctly, and make sure that we are positioning
                    // the wallpaper below the lower.
                    else {
                        Boolean isUpper = TRUE;
                        List< AutoPtr<WindowState> >::Iterator it = windows.Begin();
                        for (; it != windows.End(); ++it) {
                            if (it == foundIt) {
                                isUpper = FALSE;
                                break;
                            }
                            if (it == oldIt) {
                                isUpper = TRUE;
                                break;
                            }
                        }
                        if (isUpper) {
                            // The new target is on top of the old one.
                            if (DEBUG_WALLPAPER_LIGHT) {
                                Slogger::V(TAG, "Found target above old target.");
                            }
                            mUpperWallpaperTarget = foundW;
                            mLowerWallpaperTarget = oldW;
                            foundW = oldW;
                            foundIt = oldIt;
                        }
                        else {
                            // The new target is below the old one.
                            if (DEBUG_WALLPAPER_LIGHT) {
                                Slogger::V(TAG, "Found target below old target.");
                            }
                            mUpperWallpaperTarget = oldW;
                            mLowerWallpaperTarget = foundW;
                        }
                    }
                }
            }
        }
    }
    else if (mLowerWallpaperTarget != NULL) {
        // Is it time to stop animating?
        Boolean lowerAnimating = mLowerWallpaperTarget->mWinAnimator->mAnimation != NULL
                || (mLowerWallpaperTarget->mAppToken != NULL
                        && mLowerWallpaperTarget->mAppToken->mAppAnimator->mAnimation != NULL);
        Boolean upperAnimating = mUpperWallpaperTarget->mWinAnimator->mAnimation != NULL
                || (mUpperWallpaperTarget->mAppToken != NULL
                        && mUpperWallpaperTarget->mAppToken->mAppAnimator->mAnimation != NULL);
        if (!lowerAnimating || !upperAnimating) {
            if (DEBUG_WALLPAPER_LIGHT) {
                Slogger::V(TAG, "No longer animating wallpaper targets!");
            }
            mLowerWallpaperTarget = NULL;
            mUpperWallpaperTarget = NULL;
            mWallpaperTarget = foundW;
            targetChanged = TRUE;
        }
    }

    Boolean visible = foundW != NULL;
    if (visible) {
        // The window is visible to the compositor...  but is it visible
        // to the user?  That is what the wallpaper cares about.
        visible = IsWallpaperVisible(foundW);
        if (DEBUG_WALLPAPER) Slogger::V(TAG, "Wallpaper visibility: %d", visible);

        // If the wallpaper target is animating, we may need to copy
        // its layer adjustment.  Only do this if we are not transfering
        // between two wallpaper targets.
        mWallpaperAnimLayerAdjustment =
                (mLowerWallpaperTarget == NULL && foundW->mAppToken != NULL)
                ? foundW->mAppToken->mAppAnimator->mAnimLayerAdjustment : 0;

        Int32 wallpaperLayer;
        mPolicy->GetMaxWallpaperLayer(&wallpaperLayer);
        Int32 maxLayer = wallpaperLayer * TYPE_LAYER_MULTIPLIER + TYPE_LAYER_OFFSET;

        // Now w is the window we are supposed to be behind...  but we
        // need to be sure to also be behind any of its attached windows,
        // AND any starting window associated with it, AND below the
        // maximum layer the policy allows for wallpapers.
        while (foundIt != windows.Begin()) {
            AutoPtr<WindowState> wb = *(--List< AutoPtr<WindowState> >::Iterator(foundIt));
            Int32 wbType;
            wb->mAttrs->GetType(&wbType);
            if (wb->mBaseLayer < maxLayer &&
                    wb->mAttachedWindow != foundW &&
                    (foundW->mAttachedWindow == NULL ||
                            wb->mAttachedWindow != foundW->mAttachedWindow) &&
                    (wbType != IWindowManagerLayoutParams::TYPE_APPLICATION_STARTING ||
                            foundW->mToken == NULL || wb->mToken != foundW->mToken)) {
                // This window is not related to the previous one in any
                // interesting way, so stop here.
                break;
            }
            foundW = wb;
            foundIt--;
        }
    }
    else {
        if (DEBUG_WALLPAPER) Slogger::V(TAG, "No wallpaper target");
    }

    if (foundW == NULL && topCurW != NULL) {
        // There is no wallpaper target, so it goes at the bottom.
        // We will assume it is the same place as last time, if known.
        foundW = topCurW;
        foundIt = ++List< AutoPtr<WindowState> >::Iterator(topCurIt);
    }
    else {
        // Okay i is the position immediately above the wallpaper.  Look at
        // what is below it for later.
        foundW = foundIt != windows.Begin()
                ? *(--List< AutoPtr<WindowState> >::Iterator(foundIt)) : NULL;
    }

    if (visible) {
        if (mWallpaperTarget->mWallpaperX >= 0) {
            mLastWallpaperX = mWallpaperTarget->mWallpaperX;
            mLastWallpaperXStep = mWallpaperTarget->mWallpaperXStep;
        }
        if (mWallpaperTarget->mWallpaperY >= 0) {
            mLastWallpaperY = mWallpaperTarget->mWallpaperY;
            mLastWallpaperYStep = mWallpaperTarget->mWallpaperYStep;
        }
    }

    // Start stepping backwards from here, ensuring that our wallpaper windows
    // are correctly placed.
    List< AutoPtr<WindowToken> >::ReverseIterator wtRit;
    for (wtRit = mWallpaperTokens.RBegin(); wtRit != mWallpaperTokens.REnd(); ++wtRit) {
        AutoPtr<WindowToken> token = *wtRit;
        if (token->mHidden == visible) {
            if (DEBUG_WALLPAPER_LIGHT) Slogger::D(TAG,
                    "Wallpaper token %p hidden=%d", token.Get(), !visible);
            changed |= ADJUST_WALLPAPER_VISIBILITY_CHANGED;
            token->mHidden = !visible;
            // Need to do a layout to ensure the wallpaper now has the
            // correct size.
            GetDefaultDisplayContentLocked()->mLayoutNeeded = TRUE;
        }

        for (rit = token->mWindows.RBegin(); rit != token->mWindows.REnd(); ++rit) {
            AutoPtr<WindowState> wallpaper = *rit;

            if (visible) {
                UpdateWallpaperOffsetLocked(wallpaper, dw, dh, FALSE);
            }

            // First, make sure the client has the current visibility
            // state.
            DispatchWallpaperVisibility(wallpaper, visible);

            wallpaper->mWinAnimator->mAnimLayer = wallpaper->mLayer
                    + mWallpaperAnimLayerAdjustment;
            if (DEBUG_LAYERS || DEBUG_WALLPAPER_LIGHT) Slogger::V(TAG, "adjustWallpaper win %p anim layer: %d",
                    wallpaper.Get(), wallpaper->mWinAnimator->mAnimLayer);

            // First, if this window is at the current index, then all
            // is well.
            if (wallpaper == foundW) {
                foundIt--;
                foundW = foundIt != windows.Begin()
                        ? *(--List< AutoPtr<WindowState> >::Iterator(foundIt)) : NULL;
                continue;
            }

            // The window didn't match...  the current wallpaper window,
            // wherever it is, is in the wrong place, so make sure it is
            // not in the list.
            List< AutoPtr<WindowState> >::Iterator oldIt = Find(windows, wallpaper);
            if (oldIt != windows.End()) {
                // if (DEBUG_WINDOW_MOVEMENT) Slogger::V(TAG, "Wallpaper removing at %d: %p",
                //         oldIndex, wallpaper.Get());
                windows.Erase(oldIt);
                mWindowsChanged = TRUE;
            }

            // Now stick it in.
             if (DEBUG_WALLPAPER_LIGHT || DEBUG_WINDOW_MOVEMENT || DEBUG_ADD_REMOVE) {
                 Slogger::V(TAG, "Moving wallpaper %p", wallpaper.Get());
             }

            windows.Insert(foundIt, wallpaper);
            mWindowsChanged = TRUE;
            changed |= ADJUST_WALLPAPER_LAYERS_CHANGED;
        }
    }

    if (targetChanged && DEBUG_WALLPAPER_LIGHT) {
        Slogger::D(TAG, "New wallpaper: target=%p lower=%p upper=%p", mWallpaperTarget.Get(),
                mLowerWallpaperTarget.Get(), mUpperWallpaperTarget.Get());
    }

    return changed;
}

void CWindowManagerService::SetWallpaperAnimLayerAdjustmentLocked(
    /* [in] */ Int32 adj)
{
    if (DEBUG_LAYERS || DEBUG_WALLPAPER) Slogger::V(TAG,
            "Setting wallpaper layer adj to %d", adj);
    mWallpaperAnimLayerAdjustment = adj;
    List< AutoPtr<WindowToken> >::ReverseIterator rit;
    for (rit = mWallpaperTokens.RBegin(); rit != mWallpaperTokens.REnd(); ++rit) {
        AutoPtr<WindowToken> token = *rit;
        List< AutoPtr<WindowState> >::ReverseIterator writ = token->mWindows.RBegin();
        for (; writ != token->mWindows.REnd(); ++writ) {
            AutoPtr<WindowState> wallpaper = *writ;
            wallpaper->mWinAnimator->mAnimLayer = wallpaper->mLayer + adj;
            if (DEBUG_LAYERS || DEBUG_WALLPAPER) Slogger::V(TAG, "setWallpaper win %p anim layer: %d",
                    wallpaper.Get(), wallpaper->mWinAnimator->mAnimLayer);
        }
    }
}

Boolean CWindowManagerService::UpdateWallpaperOffsetLocked(
    /* [in] */ WindowState* wallpaperWin,
    /* [in] */ Int32 dw,
    /* [in] */ Int32 dh,
    /* [in] */ Boolean sync)
{
    Boolean changed = FALSE;
    Boolean rawChanged = FALSE;
    Float wpx = mLastWallpaperX >= 0 ? mLastWallpaperX : 0.5;
    Float wpxs = mLastWallpaperXStep >= 0 ? mLastWallpaperXStep : -1.0;
    Int32 wallpaperWinRight, wallpaperWinLeft;
    wallpaperWin->mFrame->GetRight(&wallpaperWinRight);
    wallpaperWin->mFrame->GetLeft(&wallpaperWinLeft);
    Int32 availw = wallpaperWinRight - wallpaperWinLeft - dw;
    Int32 offset = availw > 0 ? - (Int32)(availw * wpx + 0.5) : 0;
    changed = wallpaperWin->mXOffset != offset;
    if (changed) {
        if (DEBUG_WALLPAPER) Slogger::V(TAG, "Update wallpaper %p x: %d",
                wallpaperWin, offset);
        wallpaperWin->mXOffset = offset;
    }
    if (wallpaperWin->mWallpaperX != wpx || wallpaperWin->mWallpaperXStep != wpxs) {
        wallpaperWin->mWallpaperX = wpx;
        wallpaperWin->mWallpaperXStep = wpxs;
        rawChanged = TRUE;
    }

    Float wpy = mLastWallpaperY >= 0 ? mLastWallpaperY : 0.5;
    Float wpys = mLastWallpaperYStep >= 0 ? mLastWallpaperYStep : -1.0;
    Int32 wallpaperWinBottom, wallpaperWinTop;
    wallpaperWin->mFrame->GetBottom(&wallpaperWinBottom);
    wallpaperWin->mFrame->GetTop(&wallpaperWinTop);
    Int32 availh = wallpaperWinBottom - wallpaperWinTop - dh;
    offset = availh > 0 ? - (Int32)(availh * wpy + 0.5) : 0;
    if (wallpaperWin->mYOffset != offset) {
        if (DEBUG_WALLPAPER) Slogger::V(TAG, "Update wallpaper %p y: %d",
                wallpaperWin, offset);
        changed = TRUE;
        wallpaperWin->mYOffset = offset;
    }
    if (wallpaperWin->mWallpaperY != wpy || wallpaperWin->mWallpaperYStep != wpys) {
        wallpaperWin->mWallpaperY = wpy;
        wallpaperWin->mWallpaperYStep = wpys;
        rawChanged = TRUE;
    }

    if (rawChanged) {
        // try {
        if (DEBUG_WALLPAPER) Slogger::V(TAG, "Report new wp offset %p x=%f y=%f",
                wallpaperWin, wallpaperWin->mWallpaperX,
                wallpaperWin->mWallpaperY);
        if (sync) {
            mWaitingOnWallpaper = wallpaperWin;
        }
        ECode ec = wallpaperWin->mClient->DispatchWallpaperOffsets(
                wallpaperWin->mWallpaperX, wallpaperWin->mWallpaperY,
                wallpaperWin->mWallpaperXStep, wallpaperWin->mWallpaperYStep, sync);
        if (FAILED(ec)) {
            return changed;
        }
        if (sync) {
            if (mWaitingOnWallpaper != NULL) {
                Int64 start = SystemClock::GetUptimeMillis();
                if ((mLastWallpaperTimeoutTime + WALLPAPER_TIMEOUT_RECOVERY)
                        < start) {
                    // try {
                    if (DEBUG_WALLPAPER) Slogger::V(TAG,
                            "Waiting for offset complete...");
                    mWindowMapLock.Wait(WALLPAPER_TIMEOUT);
                    // } catch (InterruptedException e) {
                    // }
                    if (DEBUG_WALLPAPER) Slogger::V(TAG, "Offset complete!");
                    if ((start + WALLPAPER_TIMEOUT)
                            < SystemClock::GetUptimeMillis()) {
                        Slogger::I(TAG, "Timeout waiting for wallpaper to offset: %p",
                                wallpaperWin);
                        mLastWallpaperTimeoutTime = start;
                    }
                }
                mWaitingOnWallpaper = NULL;
            }
        }
        // } catch (RemoteException e) {
        // }
    }

    return changed;
}

void CWindowManagerService::WallpaperOffsetsComplete(
    /* [in] */ IBinder* window)
{
    AutoLock lock(mWindowMapLock);

    if (mWaitingOnWallpaper != NULL &&
            IBinder::Probe(mWaitingOnWallpaper->mClient.Get()) == window) {
        mWaitingOnWallpaper = NULL;
        mWindowMapLock.NotifyAll();
    }
}

void CWindowManagerService::UpdateWallpaperOffsetLocked(
    /* [in] */ WindowState* changingTarget,
    /* [in] */ Boolean sync)
{
    AutoPtr<DisplayContent> displayContent = changingTarget->mDisplayContent;
    AutoPtr<IDisplayInfo> displayInfo = displayContent->GetDisplayInfo();
    Int32 dw, dh;
    displayInfo->GetAppWidth(&dw);
    displayInfo->GetAppHeight(&dh);

    AutoPtr<WindowState> target = mWallpaperTarget;
    if (target != NULL) {
        if (target->mWallpaperX >= 0) {
            mLastWallpaperX = target->mWallpaperX;
        }
        else if (changingTarget->mWallpaperX >= 0) {
            mLastWallpaperX = changingTarget->mWallpaperX;
        }
        if (target->mWallpaperY >= 0) {
            mLastWallpaperY = target->mWallpaperY;
        }
        else if (changingTarget->mWallpaperY >= 0) {
            mLastWallpaperY = changingTarget->mWallpaperY;
        }
    }

    List< AutoPtr<WindowToken> >::ReverseIterator rit;
    for (rit = mWallpaperTokens.RBegin(); rit != mWallpaperTokens.REnd(); ++rit) {
        AutoPtr<WindowToken> token = *rit;
        List< AutoPtr<WindowState> >::ReverseIterator wRIt;
        for (wRIt = token->mWindows.RBegin(); wRIt != token->mWindows.REnd(); ++wRIt) {
            AutoPtr<WindowState> wallpaper = *wRIt;
            if (UpdateWallpaperOffsetLocked(wallpaper, dw, dh, sync)) {
                AutoPtr<WindowStateAnimator> winAnimator = wallpaper->mWinAnimator;
                winAnimator->ComputeShownFrameLocked();
                // No need to lay out the windows - we can just set the wallpaper position
                // directly.
                // TODO(cmautner): Don't move this from here, just lock the WindowAnimator.
                Float left = 0, top = 0;
                wallpaper->mShownFrame->GetLeft(&left);
                wallpaper->mShownFrame->GetTop(&top);
                if (winAnimator->mSurfaceX != left || winAnimator->mSurfaceY != top) {
                    winAnimator->SetWallpaperOffset((Int32)left, (Int32)top);
                }
                // We only want to be synchronous with one wallpaper.
                sync = false;
            }
        }
    }
}

void CWindowManagerService::DispatchWallpaperVisibility(
    /* [in] */ WindowState* wallpaper,
    /* [in] */ Boolean visible)
{
    if (wallpaper->mWallpaperVisible != visible) {
        wallpaper->mWallpaperVisible = visible;
        //try {
        // if (DEBUG_VISIBILITY || DEBUG_WALLPAPER_LIGHT) Slog.v(TAG,
        //         "Updating vis of wallpaper " + wallpaper
        //         + ": " + visible + " from:\n" + Debug.getCallers(4, "  "));
        wallpaper->mClient->DispatchAppVisibility(visible);
        // } catch (RemoteException e) {
        // }
    }
}

void CWindowManagerService::UpdateWallpaperVisibilityLocked()
{
    Boolean visible = IsWallpaperVisible(mWallpaperTarget);
    AutoPtr<DisplayContent> displayContent = mWallpaperTarget->mDisplayContent;
    AutoPtr<IDisplayInfo> displayInfo = displayContent->GetDisplayInfo();
    Int32 dw, dh;
    displayInfo->GetAppWidth(&dw);
    displayInfo->GetAppHeight(&dh);

    List< AutoPtr<WindowToken> >::ReverseIterator rit = mWallpaperTokens.RBegin();
    for (; rit != mWallpaperTokens.REnd(); ++rit) {
        AutoPtr<WindowToken> token = *rit;
        if (token->mHidden == visible) {
            token->mHidden = !visible;
            // Need to do a layout to ensure the wallpaper now has the
            // correct size.
            GetDefaultDisplayContentLocked()->mLayoutNeeded = TRUE;
        }

        List< AutoPtr<WindowState> >::ReverseIterator wRIt = token->mWindows.RBegin();
        for (; wRIt != token->mWindows.REnd(); ++wRIt) {
            AutoPtr<WindowState> wallpaper = *wRIt;
            if (visible) {
                UpdateWallpaperOffsetLocked(wallpaper, dw, dh, FALSE);
            }

            DispatchWallpaperVisibility(wallpaper, visible);
        }
    }
}

Int32 CWindowManagerService::AddWindow(
    /* [in] */ CSession* session,
    /* [in] */ IIWindow* client,
    /* [in] */ Int32 seq,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [in] */ Int32 viewVisibility,
    /* [in] */ Int32 displayId,
    /* [in] */ IRect* inContentInsets,
    /* [in] */ IInputChannel* inInputChannel,
    /* [out] */ IRect** outContentInsets,
    /* [out] */ IInputChannel** outInputChannel)
{
    Int32 res;
    mPolicy->CheckAddPermission(attrs, &res);
    if (res != IWindowManagerGlobal::ADD_OKAY) {
        return res;
    }

    Boolean reportNewConfig = FALSE;
    AutoPtr<WindowState> attachedWindow;
    AutoPtr<WindowState> win;
    Int64 origId;
    Int32 type;
    attrs->GetType(&type);

    {
        AutoLock lock(mWindowMapLock);

        assert(mDisplayReady);
        // if (!mDisplayReady) {
        //     throw new IllegalStateException("Display has not been initialialized");
        // }

        AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
        if (displayContent == NULL) {
            return IWindowManagerGlobal::ADD_INVALID_DISPLAY;
        }

        HashMap<AutoPtr<IBinder>, AutoPtr<WindowState> >::Iterator it
            = mWindowMap.Find(IBinder::Probe(client));
        if (it != mWindowMap.End() && it->mSecond != NULL) {
            Slogger::W(TAG, "Window %p is already added", client);
            return IWindowManagerGlobal::ADD_DUPLICATE_ADD;
        }

        AutoPtr<IBinder> attrsToken;
        attrs->GetToken((IBinder**)&attrsToken);
        if (type >= IWindowManagerLayoutParams::FIRST_SUB_WINDOW
                && type <= IWindowManagerLayoutParams::LAST_SUB_WINDOW) {
            WindowForClientLocked(NULL, attrsToken, FALSE, (WindowState**)&attachedWindow);
            if (attachedWindow == NULL) {
                Slogger::W(TAG,
                        "Attempted to add window with token that is not a window: %p.  Aborting."
                        , attrsToken.Get());
                return IWindowManagerGlobal::ADD_BAD_SUBWINDOW_TOKEN;
            }
            Int32 attachedWinType;
            attachedWindow->mAttrs->GetType(&attachedWinType);
            if (attachedWinType >= IWindowManagerLayoutParams::FIRST_SUB_WINDOW
                    && attachedWinType <= IWindowManagerLayoutParams::LAST_SUB_WINDOW) {
                Slogger::W(TAG,
                        "Attempted to add window with token that is a sub-window: %p.  Aborting."
                        , attrsToken.Get());
                return IWindowManagerGlobal::ADD_BAD_SUBWINDOW_TOKEN;
            }
        }

        Boolean addToken = FALSE;
        AutoPtr<WindowToken> token;
        if (attrsToken != NULL) {

            HashMap<AutoPtr<IBinder>, AutoPtr<WindowToken> >::Iterator tokenIt
                    = mTokenMap.Find(attrsToken);
            if (tokenIt != mTokenMap.End()) {
                token = tokenIt->mSecond;
            }
        }
        if (token == NULL) {
            if (type >= IWindowManagerLayoutParams::FIRST_APPLICATION_WINDOW
                    && type <= IWindowManagerLayoutParams::LAST_APPLICATION_WINDOW) {
                Slogger::W(TAG,
                        "Attempted to add application window with unknown token %p.  Aborting."
                        , attrsToken.Get());
                return IWindowManagerGlobal::ADD_BAD_APP_TOKEN;
            }
            if (type == IWindowManagerLayoutParams::TYPE_INPUT_METHOD) {
                Slogger::W(TAG,
                        "Attempted to add input method window with unknown token %p.  Aborting."
                        , attrsToken.Get());
                return IWindowManagerGlobal::ADD_BAD_APP_TOKEN;
            }
            if (type == IWindowManagerLayoutParams::TYPE_WALLPAPER) {
                Slogger::W(TAG,
                        "Attempted to add wallpaper window with unknown token %p.  Aborting."
                        , attrsToken.Get());
                return IWindowManagerGlobal::ADD_BAD_APP_TOKEN;
            }
            if (type == IWindowManagerLayoutParams::TYPE_DREAM) {
                Slogger::W(TAG,
                        "Attempted to add Dream window with unknown token %p.  Aborting."
                        , attrsToken.Get());
                return IWindowManagerGlobal::ADD_BAD_APP_TOKEN;
            }
            token = new WindowToken(this, attrsToken, -1, FALSE);
            addToken = TRUE;
        }
        else if (type >= IWindowManagerLayoutParams::FIRST_APPLICATION_WINDOW
                && type <= IWindowManagerLayoutParams::LAST_APPLICATION_WINDOW) {
            AutoPtr<AppWindowToken> atoken = token->mAppWindowToken;
            if (atoken == NULL) {
                Slogger::W(TAG,
                    "Attempted to add window with non-application token %p.  Aborting."
                    , token.Get());
                return IWindowManagerGlobal::ADD_NOT_APP_TOKEN;
            }
            else if (atoken->mRemoved) {
                Slogger::W(TAG,
                        "Attempted to add window with exiting application token %p.  Aborting."
                        , token.Get());
                return IWindowManagerGlobal::ADD_APP_EXITING;
            }
            if (type == IWindowManagerLayoutParams::TYPE_APPLICATION_STARTING
                    && atoken->mFirstWindowDrawn) {
                // No need for this guy!
                AutoPtr<ICharSequence> title;
                attrs->GetTitle((ICharSequence**)&title);
                String str;
                title->ToString(&str);
                if (mLocalLOGV) Slogger::V(TAG,
                        TAG, "**** NO NEED TO START: %s", str.string());
                return IWindowManagerGlobal::ADD_STARTING_NOT_NEEDED;
            }
        }
        else if (type == IWindowManagerLayoutParams::TYPE_INPUT_METHOD) {
            if (token->mWindowType != IWindowManagerLayoutParams::TYPE_INPUT_METHOD) {
                Slogger::W(TAG,
                        "Attempted to add input method window with bad token %p.  Aborting."
                        , attrsToken.Get());
                return IWindowManagerGlobal::ADD_BAD_APP_TOKEN;
            }
        }
        else if (type == IWindowManagerLayoutParams::TYPE_WALLPAPER) {
            if (token->mWindowType != IWindowManagerLayoutParams::TYPE_WALLPAPER) {
                Slogger::W(TAG,
                        "Attempted to add wallpaper window with bad token %p.  Aborting."
                        , attrsToken.Get());
                return IWindowManagerGlobal::ADD_BAD_APP_TOKEN;
            }
        }
        else if (type == IWindowManagerLayoutParams::TYPE_DREAM) {
            if (token->mWindowType != IWindowManagerLayoutParams::TYPE_DREAM) {
                Slogger::W(TAG, "Attempted to add Dream window with bad token %p.  Aborting."
                        , attrsToken.Get());
                return IWindowManagerGlobal::ADD_BAD_APP_TOKEN;
            }
        }

        win = new WindowState(this, session, client, token,
                attachedWindow, seq, attrs, viewVisibility, displayContent);
        if (win->mDeathRecipient == NULL) {
            // Client has apparently died, so there is no reason to
            // continue.
            Slogger::W(TAG, "Adding window client %p that is dead, aborting.", client);
            return IWindowManagerGlobal::ADD_APP_EXITING;
        }

        mPolicy->AdjustWindowParamsLw(win->mAttrs);
        Boolean result;
        mPolicy->CheckShowToOwnerOnly(attrs, &result);
        win->SetShowToOwnerOnlyLocked(result);

        mPolicy->PrepareAddWindowLw(win, attrs, &res);
        if (res != IWindowManagerGlobal::ADD_OKAY) {
            return res;
        }

        Int32 inputFeatures;
        attrs->GetInputFeatures(&inputFeatures);
        if (outInputChannel != NULL && (inputFeatures
                & IWindowManagerLayoutParams::INPUT_FEATURE_NO_INPUT_CHANNEL) == 0) {
            String name = win->MakeInputChannelName();
            AutoPtr<IInputChannelHelper> helper;
            ASSERT_SUCCEEDED(CInputChannelHelper::AcquireSingleton(
                    (IInputChannelHelper**)&helper));
            AutoPtr<IInputChannel> serverChannel;
            AutoPtr<IInputChannel> clientChannel;
            ASSERT_SUCCEEDED(helper->OpenInputChannelPair(name,
                    (IInputChannel**)&serverChannel, (IInputChannel**)&clientChannel));
            win->SetInputChannel(serverChannel);
            ASSERT_SUCCEEDED(CInputChannel::New(outInputChannel));
            clientChannel->TransferTo(*outInputChannel);

            ASSERT_SUCCEEDED(mInputManager->RegisterInputChannel(win->mInputChannel,
                    win->mInputWindowHandle));
        }

        // From now on, no exceptions or errors allowed!

        res = IWindowManagerGlobal::ADD_OKAY;

        origId = Binder::ClearCallingIdentity();

        if (addToken) {
            if (attrsToken != NULL)
                mTokenMap[attrsToken] = token;
        }
        win->Attach();
        mWindowMap[IBinder::Probe(client)] = win;

        if (type == IWindowManagerLayoutParams::TYPE_APPLICATION_STARTING &&
                token->mAppWindowToken != NULL) {
            token->mAppWindowToken->mStartingWindow = win;
            if (DEBUG_STARTING_WINDOW) {
                Slogger::V (TAG, "addWindow: %p startingWindow=%p",
                    token->mAppWindowToken, win.Get());
            }
        }

        Boolean imMayMove = TRUE;

        if (type == IWindowManagerLayoutParams::TYPE_INPUT_METHOD) {
            win->mGivenInsetsPending = TRUE;
            mInputMethodWindow = win;
            AddInputMethodWindowToListLocked(win);
            imMayMove = FALSE;
        }
        else if (type == IWindowManagerLayoutParams::TYPE_INPUT_METHOD_DIALOG) {
            mInputMethodDialogs.PushBack(win);
            AddWindowToListInOrderLocked(win, TRUE);
            AdjustInputMethodDialogsLocked();
            imMayMove = FALSE;
        }
        else {
            AddWindowToListInOrderLocked(win, TRUE);
            if (type == IWindowManagerLayoutParams::TYPE_WALLPAPER) {
                mLastWallpaperTimeoutTime = 0;
                AdjustWallpaperWindowsLocked();
            }
            else {
                Int32 attrsFlags;
                attrs->GetFlags(&attrsFlags);
                if ((attrsFlags & IWindowManagerLayoutParams::FLAG_SHOW_WALLPAPER) != 0) {
                    AdjustWallpaperWindowsLocked();
                }
                else if (mWallpaperTarget != NULL
                        && mWallpaperTarget->mLayer >= win->mBaseLayer) {
                    // If there is currently a wallpaper being shown, and
                    // the base layer of the new window is below the current
                    // layer of the target window, then adjust the wallpaper.
                    // This is to avoid a new window being placed between the
                    // wallpaper and its target.
                    AdjustWallpaperWindowsLocked();
                }
            }
        }

        win->mWinAnimator->mEnterAnimationPending = TRUE;

        if (displayContent->mIsDefaultDisplay) {
            mPolicy->GetContentInsetHintLw(attrs, inContentInsets);
        }
        else {
            inContentInsets->SetEmpty();
        }

        if (mInTouchMode) {
            res |= IWindowManagerGlobal::ADD_FLAG_IN_TOUCH_MODE;
        }
        if (win->mAppToken == NULL || !win->mAppToken->mClientHidden) {
            res |= IWindowManagerGlobal::ADD_FLAG_APP_VISIBLE;
        }

        Boolean focusChanged = FALSE;
        if (win->CanReceiveKeys()) {
            focusChanged = UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_ASSIGN_LAYERS,
                    FALSE /*updateInputWindows*/);
            if (focusChanged) {
                imMayMove = FALSE;
            }
        }

        if (imMayMove) {
            MoveInputMethodWindowsIfNeededLocked(FALSE);
        }

        AssignLayersLocked(displayContent->GetWindowList());
        // Don't do layout here, the window must call
        // relayout to be displayed, so we'll do it there.

        //dump();

        if (focusChanged) {
            FinishUpdateFocusedWindowAfterAssignLayersLocked(FALSE /*updateInputWindows*/);
        }
        mInputMonitor->UpdateInputWindowsLw(FALSE /*force*/);

        // if (localLOGV) Slog.v(
        //         TAG, "New client " + client.asBinder()
        //         + ": window=" + win);

        if (win->IsVisibleOrAdding() && UpdateOrientationFromAppTokensLocked(FALSE)) {
            reportNewConfig = TRUE;
        }
    }

    if (reportNewConfig) {
       SendNewConfiguration();
    }
    Binder::RestoreCallingIdentity(origId);

    *outContentInsets = inContentInsets;
    REFCOUNT_ADD(*outContentInsets);
    return res;
}

void CWindowManagerService::RemoveWindow(
    /* [in] */ CSession* session,
    /* [in] */ IIWindow* client)
{
    AutoLock lock(mWindowMapLock);

    AutoPtr<WindowState> win;
    WindowForClientLocked(session, client, FALSE, (WindowState**)&win);
    if (win == NULL) {
        return;
    }
    RemoveWindowLocked(session, win);
}

void CWindowManagerService::RemoveWindowLocked(
    /* [in] */ CSession* session,
    /* [in] */ WindowState* win)
{
    // if (localLOGV || DEBUG_FOCUS) Slog.v(
    //         TAG, "Remove " + win + " client="
    //         + Integer.toHexString(System.identityHashCode(
    //             win.mClient.asBinder()))
    //         + ", surface=" + win.mWinAnimator.mSurface);

    Int64 origId = Binder::ClearCallingIdentity();

    win->DisposeInputChannel();

    // if (DEBUG_APP_TRANSITIONS) Slog.v(
    //         TAG, "Remove " + win + ": mSurface=" + win.mWinAnimator.mSurface
    //         + " mExiting=" + win.mExiting
    //         + " isAnimating=" + win.mWinAnimator.isAnimating()
    //         + " app-animation="
    //         + (win.mAppToken != null ? win.mAppToken.mAppAnimator.animation : null)
    //         + " inPendingTransaction="
    //         + (win.mAppToken != null ? win.mAppToken.inPendingTransaction : false)
    //         + " mDisplayFrozen=" + mDisplayFrozen);
    // Visibility of the removed window. Will be used later to update orientation later on.
    Boolean wasVisible = FALSE;
    // First, see if we need to run an animation.  If we do, we have
    // to hold off on removing the window until the animation is done.
    // If the display is frozen, just remove immediately, since the
    // animation wouldn't be seen.
    if (win->mHasSurface && OkToDisplay()) {
        // If we are not currently running the exit animation, we
        // need to see about starting one.
        wasVisible = win->IsWinVisibleLw();
        if (wasVisible) {
            Int32 transit = IWindowManagerPolicy::TRANSIT_EXIT;
            Int32 type;
            win->mAttrs->GetType(&type);
            if (type == IWindowManagerLayoutParams::TYPE_APPLICATION_STARTING) {
                transit = IWindowManagerPolicy::TRANSIT_PREVIEW_DONE;
            }
            // Try starting an animation.
            if (win->mWinAnimator->ApplyAnimationLocked(transit, FALSE)) {
                win->mExiting = TRUE;
            }
            ScheduleNotifyWindowTranstionIfNeededLocked(win, transit);
        }
        if (win->mExiting || win->mWinAnimator->IsAnimating()) {
            // The exit animation is running... wait for it!
            //Slog.i(TAG, "*** Running exit animation...");
            win->mExiting = TRUE;
            win->mRemoveOnExit = TRUE;
            win->mDisplayContent->mLayoutNeeded = TRUE;
            UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES,
                    FALSE /*updateInputWindows*/);
            PerformLayoutAndPlaceSurfacesLocked();
            mInputMonitor->UpdateInputWindowsLw(FALSE /*force*/);
            if (win->mAppToken != NULL) {
                win->mAppToken->UpdateReportedVisibilityLocked();
            }
            //dump();
            Binder::RestoreCallingIdentity(origId);
            return;
        }
    }

    RemoveWindowInnerLocked(session, win);
    // Removing a visible window will effect the computed orientation
    // So just update orientation if needed.
    if (wasVisible && ComputeForcedAppOrientationLocked()
        != mForcedAppOrientation && UpdateOrientationFromAppTokensLocked(FALSE)) {
        Boolean result;
        mH->SendEmptyMessage(H::SEND_NEW_CONFIGURATION, &result);
    }
    UpdateFocusedWindowLocked(UPDATE_FOCUS_NORMAL, TRUE /*updateInputWindows*/);
    Binder::RestoreCallingIdentity(origId);
}

void CWindowManagerService::RemoveWindowInnerLocked(
    /* [in] */ CSession* session,
    /* [in] */ WindowState* win)
{
    if (win->mRemoved) {
        // Nothing to do.
        return;
    }

    List< AutoPtr<WindowState> >::ReverseIterator rit = win->mChildWindows.RBegin();
    for (; rit != win->mChildWindows.REnd(); ++rit) {
        AutoPtr<WindowState> cwin = *rit;
        // Slog.w(TAG, "Force-removing child win " + cwin + " from container "
        //         + win);
        RemoveWindowInnerLocked(cwin->mSession, cwin);
    }

    win->mRemoved = TRUE;

    if (mInputMethodTarget.Get() == win) {
        MoveInputMethodWindowsIfNeededLocked(FALSE);
    }

    // if (false) {
    //     RuntimeException e = new RuntimeException("here");
    //     e.fillInStackTrace();
    //     Slog.w(TAG, "Removing window " + win, e);
    // }

    mPolicy->RemoveWindowLw(win);
    win->RemoveLocked();

    if (DEBUG_ADD_REMOVE) Slogger::V(TAG, "removeWindowInnerLocked: %p", win);
    mWindowMap.Erase(IBinder::Probe(win->mClient.Get()));

    List< AutoPtr<WindowState> >& windows = win->GetWindowList();
    windows.Remove(win);
    mPendingRemove.Remove(win);
    mResizingWindows.Remove(win);
    mWindowsChanged = TRUE;
    if (DEBUG_WINDOW_MOVEMENT) Slogger::V(TAG, "Final remove of window: %p", win);

    Int32 winType, winFlags;
    if (mInputMethodWindow.Get() == win) {
        mInputMethodWindow = NULL;
    }
    else {
        win->mAttrs->GetType(&winType);
        if (winType == IWindowManagerLayoutParams::TYPE_INPUT_METHOD_DIALOG) {
            mInputMethodDialogs.Remove(win);
        }
    }

    AutoPtr<WindowToken> token = win->mToken;
    AutoPtr<AppWindowToken> atoken = win->mAppToken;
    if (DEBUG_ADD_REMOVE) Slogger::V(TAG, "Removing %p from %p", win, token.Get());
    token->mWindows.Remove(win);
    if (atoken != NULL) {
        atoken->mAllAppWindows.Remove(win);
    }
    // if (localLOGV) Slog.v(
    //         TAG, "**** Removing window " + win + ": count="
    //         + token.windows.size());
    if (token->mWindows.Begin() == token->mWindows.End()) {
        if (!token->mExplicit && token->mToken != NULL) {
            mTokenMap.Erase(token->mToken);
        }
        else if (atoken != NULL) {
            atoken->mFirstWindowDrawn = FALSE;
        }
    }

    if (atoken != NULL) {
        if (atoken->mStartingWindow.Get() == win) {
            atoken->mStartingWindow = NULL;
        }
        else if (atoken->mAllAppWindows.IsEmpty() && atoken->mStartingData != NULL) {
            // If this is the last window and we had requested a starting
            // transition window, well there is no point now.
            if (DEBUG_STARTING_WINDOW) {
                Slogger::V(TAG, "Nulling last startingWindow");
            }
            atoken->mStartingData = NULL;
        }
        else if (atoken->mAllAppWindows.GetSize() == 1 && atoken->mStartingView != NULL) {
            // If this is the last window except for a starting transition
            // window, we need to get rid of the starting transition.
            if (DEBUG_STARTING_WINDOW) {
                Slogger::V(TAG, "Schedule remove starting %p : no more real windows", token.Get());
            }

            AutoPtr<IMessage> msg;
            mH->ObtainMessage(H::REMOVE_STARTING, atoken, (IMessage**)&msg);
            Boolean result;
            mH->SendMessage(msg, &result);
        }
    }

    win->mAttrs->GetType(&winType);
    if (winType == IWindowManagerLayoutParams::TYPE_WALLPAPER) {
        mLastWallpaperTimeoutTime = 0;
        AdjustWallpaperWindowsLocked();
    }
    else {
        win->mAttrs->GetFlags(&winFlags);
        if ((winFlags & IWindowManagerLayoutParams::FLAG_SHOW_WALLPAPER) != 0) {
            AdjustWallpaperWindowsLocked();
        }
    }

    if (!mInLayout) {
        AssignLayersLocked(windows);
        win->mDisplayContent->mLayoutNeeded = TRUE;
        PerformLayoutAndPlaceSurfacesLocked();
        if (win->mAppToken != NULL) {
            win->mAppToken->UpdateReportedVisibilityLocked();
        }
    }

    mInputMonitor->UpdateInputWindowsLw(TRUE /*force*/);
}

// void CWindowManagerService::LogSurface(
//     /* [in] */ WindowState* w,
//     /* [in] */ const String& msg,
//     /* [in] */ RuntimeException where)
// {
//     String str = "  SURFACE " + msg + ": " + w;
//     if (where != null) {
//         Slog.i(TAG, str, where);
//     } else {
//         Slog.i(TAG, str);
//     }
// }

// static void logSurface(Surface s, String title, String msg, RuntimeException where) {
//     String str = "  SURFACE " + s + ": " + msg + " / " + title;
//     if (where != null) {
//         Slog.i(TAG, str, where);
//     } else {
//         Slog.i(TAG, str);
//     }
// }

void CWindowManagerService::SetTransparentRegionHint(
    /* [in] */ WindowStateAnimator* winAnimator,
    /* [in] */ IRegion* region)
{
    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArg1 = winAnimator;
    args->mArg2 = region;

    AutoPtr<IMessage> msg;
    mH->ObtainMessage(H::SET_TRANSPARENT_REGION, args.Get(), (IMessage**)&msg);
    Boolean result;
    mH->SendMessage(msg, &result);
}

void CWindowManagerService::SetTransparentRegionWindow(
    /* [in] */ CSession* session,
    /* [in] */ IIWindow* client,
    /* [in] */ IRegion* region)
{
    Int64 origId = Binder::ClearCallingIdentity();
    // try {
    AutoLock lock(mWindowMapLock);

    AutoPtr<WindowState> w;
    WindowForClientLocked(session, client, FALSE, (WindowState**)&w);
    if (w != NULL && w->mHasSurface) {
        SetTransparentRegionHint(w->mWinAnimator, region);
    }
    // } finally {
    Binder::RestoreCallingIdentity(origId);
    // }
}

void CWindowManagerService::SetInsetsWindow(
    /* [in] */ CSession* session,
    /* [in] */ IIWindow* client,
    /* [in] */ Int32 touchableInsets,
    /* [in] */ IRect* contentInsets,
    /* [in] */ IRect* visibleInsets,
    /* [in] */ IRegion* touchableRegion)
{
    Int64 origId = Binder::ClearCallingIdentity();
    // try {
    AutoLock lock(mWindowMapLock);

    AutoPtr<WindowState> w;
    WindowForClientLocked(session, client, FALSE, (WindowState**)&w);
    if (w != NULL) {
        w->mGivenInsetsPending = FALSE;
        w->mGivenContentInsets->Set(contentInsets);
        w->mGivenVisibleInsets->Set(visibleInsets);
        Boolean result;
        w->mGivenTouchableRegion->Set(touchableRegion, &result);
        w->mTouchableInsets = touchableInsets;
        if (w->mGlobalScale != 1) {
            w->mGivenContentInsets->Scale(w->mGlobalScale);
            w->mGivenVisibleInsets->Scale(w->mGlobalScale);
            w->mGivenTouchableRegion->Scale(w->mGlobalScale);
        }
        w->mDisplayContent->mLayoutNeeded = TRUE;
        PerformLayoutAndPlaceSurfacesLocked();
    }
    // } finally {
    Binder::RestoreCallingIdentity(origId);
    // }
}

void CWindowManagerService::GetWindowDisplayFrame(
    /* [in] */ CSession* session,
    /* [in] */ IIWindow* client,
    /* [in] */ IRect* outDisplayFrame)
{
    AutoLock lock(mWindowMapLock);

    AutoPtr<WindowState> win;
    WindowForClientLocked(session, client, FALSE, (WindowState**)&win);
    if (win == NULL) {
        outDisplayFrame->SetEmpty();
        return;
    }
    outDisplayFrame->Set(win->mDisplayFrame);
}

void CWindowManagerService::SetWindowWallpaperPositionLocked(
    /* [in] */ WindowState* window,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float xStep,
    /* [in] */ Float yStep)
{
    if (window->mWallpaperX != x || window->mWallpaperY != y)  {
        window->mWallpaperX = x;
        window->mWallpaperY = y;
        window->mWallpaperXStep = xStep;
        window->mWallpaperYStep = yStep;
        UpdateWallpaperOffsetLocked(window, TRUE);
    }
}

void CWindowManagerService::WallpaperCommandComplete(
    /* [in] */ IBinder* window,
    /* [in] */ IBundle* result)
{
    AutoLock lock(mWindowMapLock);

    if (mWaitingOnWallpaper != NULL &&
            IBinder::Probe(mWaitingOnWallpaper->mClient.Get()) == window) {
        mWaitingOnWallpaper = NULL;
        mWindowMapLock.NotifyAll();
    }
}

AutoPtr<IBundle> CWindowManagerService::SendWindowWallpaperCommandLocked(
    /* [in] */ WindowState* window,
    /* [in] */ const String& action,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean sync)
{
    if (window == mWallpaperTarget || window == mLowerWallpaperTarget
            || window == mUpperWallpaperTarget) {
        Boolean doWait = sync;
        List< AutoPtr<WindowToken> >::ReverseIterator rit = mWallpaperTokens.RBegin();
        for (; rit != mWallpaperTokens.REnd(); ++rit) {
            AutoPtr<WindowToken> token = *rit;
            List< AutoPtr<WindowState> >::ReverseIterator wRIt;
            for (wRIt = token->mWindows.RBegin(); wRIt != token->mWindows.REnd(); ++wRIt) {
                AutoPtr<WindowState> wallpaper = *wRIt;
                // try {
                wallpaper->mClient->DispatchWallpaperCommand(action,
                        x, y, z, extras, sync);
                // We only want to be synchronous with one wallpaper.
                sync = FALSE;
                // } catch (RemoteException e) {
                // }
            }
        }

        if (doWait) {
            // XXX Need to wait for result.
        }
    }

    return NULL;
}

void CWindowManagerService::SetUniverseTransformLocked(
    /* [in] */ WindowState* window,
    /* [in] */ Float alpha,
    /* [in] */ Float offx,
    /* [in] */ Float offy,
    /* [in] */ Float dsdx,
    /* [in] */ Float dtdx,
    /* [in] */ Float dsdy,
    /* [in] */ Float dtdy)
{
    AutoPtr<ITransformation> transform = window->mWinAnimator->mUniverseTransform;
    transform->SetAlpha(alpha);
    AutoPtr<IMatrix> matrix;
    transform->GetMatrix((IMatrix**)&matrix);
    matrix->GetValues(mTmpFloats);
    (*mTmpFloats)[IMatrix::MTRANS_X] = offx;
    (*mTmpFloats)[IMatrix::MTRANS_Y] = offy;
    (*mTmpFloats)[IMatrix::MSCALE_X] = dsdx;
    (*mTmpFloats)[IMatrix::MSKEW_Y] = dtdx;
    (*mTmpFloats)[IMatrix::MSKEW_X] = dsdy;
    (*mTmpFloats)[IMatrix::MSCALE_Y] = dtdy;
    matrix->SetValues(*mTmpFloats);
    AutoPtr<IDisplayInfo> displayInfo = window->mDisplayContent->GetDisplayInfo();
    AutoPtr<IRectF> dispRect;
    Int32 logicalW, logicalH;
    displayInfo->GetLogicalWidth(&logicalW);
    displayInfo->GetLogicalHeight(&logicalH);
    ASSERT_SUCCEEDED(CRectF::New(0, 0, logicalW, logicalH, (IRectF**)&dispRect));
    Boolean res;
    matrix->MapRect(dispRect, &res);
    window->mGivenTouchableRegion->Set(0, 0, logicalW, logicalH, &res);
    Float left = 0, top = 0, right = 0, bottom = 0;
    dispRect->GetLeft(&left);
    dispRect->GetTop(&top);
    dispRect->GetRight(&right);
    dispRect->GetBottom(&bottom);
    window->mGivenTouchableRegion->Op((Int32)left, (Int32)top, (Int32)right,
            (Int32)bottom, Elastos::Droid::Graphics::RegionOp_DIFFERENCE, &res);
    window->mTouchableInsets = IInternalInsetsInfo::TOUCHABLE_INSETS_REGION;
    window->mDisplayContent->mLayoutNeeded = TRUE;
    PerformLayoutAndPlaceSurfacesLocked();
}

void CWindowManagerService::OnRectangleOnScreenRequested(
    /* [in] */ IBinder* token,
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate)
{
    AutoLock lock(mWindowMapLock);

    AutoPtr<WindowState> window;
    HashMap<AutoPtr<IBinder>, AutoPtr<WindowState> >::Iterator it
        = mWindowMap.Find(AutoPtr<IBinder>(token));
    if (it != mWindowMap.End()) {
        window = it->mSecond;
    }
    if (window != NULL) {
        ScheduleNotifyRectangleOnScreenRequestedIfNeededLocked(window, rectangle,
                immediate);
    }
}

void CWindowManagerService::ScheduleNotifyRectangleOnScreenRequestedIfNeededLocked(
    /* [in] */ WindowState* window,
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate)
{
    AutoPtr<DisplayContent> displayContent = window->mDisplayContent;
    if (displayContent->mDisplayContentChangeListeners != NULL) {
        Int32 count;
        displayContent->mDisplayContentChangeListeners->GetRegisteredCallbackCount(&count);
        if (count > 0) {
            AutoPtr<IMessage> msg;
            AutoPtr<IRect> rect;
            CRect::New(rectangle, (IRect**)&rect);
            mH->ObtainMessage(H::NOTIFY_RECTANGLE_ON_SCREEN_REQUESTED,
                immediate ? 1:0, 0, rect, (IMessage**)&msg);
            msg->SendToTarget();
        }
    }
}

void CWindowManagerService::HandleNotifyRectangleOnScreenRequested(
    /* [in] */ Int32 displayId,
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate)
{
    AutoPtr<IRemoteCallbackList> callbacks;
    {
        AutoLock lock(mWindowMapLock);
        AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
        if (displayContent == NULL) {
            return;
        }
        callbacks = displayContent->mDisplayContentChangeListeners;
        if (callbacks == NULL) {
            return;
        }

    }

    Int32 callbackCount;
    callbacks->BeginBroadcast(&callbackCount);
    // try {
    for (Int32 i = 0; i < callbackCount; i++) {
        // try {
        AutoPtr<IDisplayContentChangeListener> l;
        callbacks->GetBroadcastItem(i, (IInterface**)&l);
        l->OnRectangleOnScreenRequested(displayId, rectangle, immediate);
        // } catch (RemoteException re) {
        //     /* ignore */
        // }
    }
    // } finally {
    callbacks->FinishBroadcast();
    // }
}

Int32 CWindowManagerService::RelayoutWindow(
    /* [in] */ CSession* session,
    /* [in] */ IIWindow* client,
    /* [in] */ Int32 seq,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [in] */ Int32 requestedWidth,
    /* [in] */ Int32 requestedHeight,
    /* [in] */ Int32 viewVisibility,
    /* [in] */ Int32 flags,
    /* [in] */ IRect* inFrame,
    /* [in] */ IRect* inContentInsets,
    /* [in] */ IRect* inVisibleInsets,
    /* [in] */ IConfiguration* inConfig,
    /* [in] */ ISurface* inSurface,
    /* [out] */ IRect** outFrame,
    /* [out] */ IRect** outContentInsets,
    /* [out] */ IRect** outVisibleInsets,
    /* [out] */ IConfiguration** outConfig,
    /* [out] */ ISurface** outSurface)
{
    Boolean toBeDisplayed = FALSE;
    Boolean inTouchMode;
    Boolean configChanged;
    Boolean surfaceChanged = FALSE;
    Boolean animating;

    // if they don't have this permission, mask out the status bar bits
    Int32 systemUiVisibility = 0;
    if (attrs != NULL) {
        Int32 uiVisibility, subUiVisibility;
        attrs->GetSystemUiVisibility(&uiVisibility);
        attrs->GetSubtreeSystemUiVisibility(&subUiVisibility);
        systemUiVisibility = (uiVisibility | subUiVisibility);
        if ((systemUiVisibility & IStatusBarManager::DISABLE_MASK) != 0) {
            Int32 perm;
            FAIL_RETURN(mContext->CheckCallingOrSelfPermission(
                    Elastos::Droid::Manifest::Permission::STATUS_BAR,
                    &perm));
            if (perm != IPackageManager::PERMISSION_GRANTED) {
                systemUiVisibility &= ~IStatusBarManager::DISABLE_MASK;
            }
        }
    }

    Int64 origId = Binder::ClearCallingIdentity();

    {
        AutoLock lock(mWindowMapLock);

        // TODO(cmautner): synchronize on mAnimator or win.mWinAnimator.
        AutoPtr<WindowState> win;
        WindowForClientLocked(session, client, FALSE, (WindowState**)&win);
        if (win == NULL) {
            Binder::RestoreCallingIdentity(origId);
            return 0;
        }

        AutoPtr<WindowStateAnimator> winAnimator = win->mWinAnimator;
        if (win->mRequestedWidth != requestedWidth
                || win->mRequestedHeight != requestedHeight) {
            win->mLayoutNeeded = TRUE;
            win->mRequestedWidth = requestedWidth;
            win->mRequestedHeight = requestedHeight;
        }
        if (attrs != NULL && seq == win->mSeq) {
            win->mSystemUiVisibility = systemUiVisibility;
        }

        if (attrs != NULL) {
            mPolicy->AdjustWindowParamsLw(attrs);
        }

        winAnimator->mSurfaceDestroyDeferred =
                (flags & IWindowManagerGlobal::RELAYOUT_DEFER_SURFACE_DESTROY) != 0;

        Int32 winFlags = 0;
        win->mAttrs->GetFlags(&winFlags);
        Int32 attrChanges = 0;
        Int32 flagChanges = 0;
        if (attrs != NULL) {
            Int32 winType, type;
            win->mAttrs->GetType(&winType);
            attrs->GetType(&type);
            if (winType != type) {
                Binder::RestoreCallingIdentity(origId);
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
                // throw new IllegalArgumentException(
                //         "Window type can not be changed after the window is added.");
            }
            Int32 attrsFlags;
            attrs->GetFlags(&attrsFlags);
            flagChanges = winFlags ^= attrsFlags;
            win->mAttrs->SetFlags(winFlags);
            win->mAttrs->CopyFrom(attrs, &attrChanges);
            if ((attrChanges & (IWindowManagerLayoutParams::LAYOUT_CHANGED
                    | IWindowManagerLayoutParams::SYSTEM_UI_VISIBILITY_CHANGED)) != 0) {
                win->mLayoutNeeded = TRUE;
            }
        }

        // if (DEBUG_LAYOUT) Slog.v(TAG, "Relayout " + win + ": viewVisibility=" + viewVisibility
        //         + " req=" + requestedWidth + "x" + requestedHeight + " " + win.mAttrs);

        win->mAttrs->GetFlags(&winFlags);
        win->mEnforceSizeCompat = (winFlags &
                IWindowManagerLayoutParams::FLAG_COMPATIBLE_WINDOW) != 0;

        if ((attrChanges & IWindowManagerLayoutParams::ALPHA_CHANGED) != 0) {
            attrs->GetAlpha(&winAnimator->mAlpha);
        }

        win->mAttrs->GetFlags(&winFlags);
        Boolean scaledWindow =
            ((winFlags & IWindowManagerLayoutParams::FLAG_SCALED) != 0);

        if (scaledWindow) {
            // requested{Width|Height} Surface's physical size
            // attrs.{width|height} Size on screen
            Int32 width, height;
            attrs->GetWidth(&width);
            attrs->GetHeight(&height);
            win->mHScale = (width  != requestedWidth)  ?
                    (width  / (Float)requestedWidth) : 1.0f;
            win->mVScale = (height != requestedHeight) ?
                    (height / (Float)requestedHeight) : 1.0f;
        }
        else {
            win->mHScale = win->mVScale = 1;
        }

        Boolean imMayMove = (flagChanges & (
                IWindowManagerLayoutParams::FLAG_ALT_FOCUSABLE_IM |
                IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE)) != 0;

        Boolean isDefaultDisplay;
        win->IsDefaultDisplay(&isDefaultDisplay);
        Boolean focusMayChange = isDefaultDisplay && (win->mViewVisibility != viewVisibility
                || ((flagChanges & IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE) != 0)
                || (!win->mRelayoutCalled));

        Boolean wallpaperMayMove = win->mViewVisibility != viewVisibility
                && (winFlags & IWindowManagerLayoutParams::FLAG_SHOW_WALLPAPER) != 0;

        win->mRelayoutCalled = TRUE;
        Int32 oldVisibility = win->mViewVisibility;
        win->mViewVisibility = viewVisibility;
        // if (DEBUG_SCREEN_ON) {
        //     RuntimeException stack = new RuntimeException();
        //     stack.fillInStackTrace();
        //     Slog.i(TAG, "Relayout " + win + ": oldVis=" + oldVisibility
        //             + " newVis=" + viewVisibility, stack);
        // }
        if (viewVisibility == IView::VISIBLE &&
                (win->mAppToken == NULL || !win->mAppToken->mClientHidden)) {
            Boolean isVisible;
            win->IsVisibleLw(&isVisible);
            toBeDisplayed = !isVisible;
            if (win->mExiting) {
                winAnimator->CancelExitAnimationForNextAnimationLocked();
                win->mExiting = FALSE;
            }
            if (win->mDestroying) {
                win->mDestroying = FALSE;
                mDestroySurface.Remove(win);
            }
            if (oldVisibility == IView::GONE) {
                winAnimator->mEnterAnimationPending = TRUE;
            }
            if (toBeDisplayed) {
                if (win->IsDrawnLw() && OkToDisplay()) {
                    winAnimator->ApplyEnterAnimationLocked();
                }
                if ((winFlags & IWindowManagerLayoutParams::FLAG_TURN_SCREEN_ON) != 0) {
                    // if (DEBUG_VISIBILITY) Slog.v(TAG,
                    //         "Relayout window turning screen on: " + win);
                    win->mTurnOnScreen = TRUE;
                }
                if (win->IsConfigChanged()) {
                    // if (DEBUG_CONFIGURATION) Slog.i(TAG, "Window " + win
                    //         + " visible with new config: " + mCurConfiguration);
                    inConfig->SetTo(mCurConfiguration);
                }
            }
            if ((attrChanges & IWindowManagerLayoutParams::FORMAT_CHANGED) != 0) {
                // To change the format, we need to re-build the surface.
                winAnimator->DestroySurfaceLocked(FALSE);
                toBeDisplayed = TRUE;
                surfaceChanged = TRUE;
            }
            // try {
            if (!win->mHasSurface) {
                surfaceChanged = TRUE;
            }
            AutoPtr<ISurface> surface = winAnimator->CreateSurfaceLocked();
            if (surface != NULL) {
                ECode ec = inSurface->CopyFrom(surface);
                if (FAILED(ec)) {
                    mInputMonitor->UpdateInputWindowsLw(TRUE /*force*/);
                    Binder::RestoreCallingIdentity(origId);
                    return 0;
                }
                // if (SHOW_TRANSACTIONS) Slog.i(TAG,
                //         "  OUT SURFACE " + outSurface + ": copied");
            }
            else {
                // For some reason there isn't a surface.  Clear the
                // caller's object so they see the same state.
                inSurface->ReleaseSurface();
            }
            // catch (Exception e) {
            //     mInputMonitor.updateInputWindowsLw(true /*force*/);

            //     Slog.w(TAG, "Exception thrown when creating surface for client "
            //              + client + " (" + win.mAttrs.getTitle() + ")",
            //              e);
            //     Binder::RestoreCallingIdentity(origId);
            //     return 0;
            // }
            if (toBeDisplayed) {
                focusMayChange = isDefaultDisplay;
            }
            Int32 winType;
            win->mAttrs->GetType(&winType);
            if (winType == IWindowManagerLayoutParams::TYPE_INPUT_METHOD
                    && mInputMethodWindow == NULL) {
                mInputMethodWindow = win;
                imMayMove = TRUE;
            }
            if (winType == IWindowManagerLayoutParams::TYPE_INPUT_METHOD
                    && win->mAppToken != NULL
                    && win->mAppToken->mStartingWindow != NULL) {
                // Special handling of starting window over the base
                // window of the app: propagate lock screen flags to it,
                // to provide the correct semantics while starting.
                Int32 mask =
                    IWindowManagerLayoutParams::FLAG_SHOW_WHEN_LOCKED
                    | IWindowManagerLayoutParams::FLAG_DISMISS_KEYGUARD
                    | IWindowManagerLayoutParams::FLAG_ALLOW_LOCK_WHILE_SCREEN_ON;
                AutoPtr<IWindowManagerLayoutParams> sa
                        = win->mAppToken->mStartingWindow->mAttrs;
                Int32 swFlags;
                sa->GetFlags(&swFlags);
                swFlags = (swFlags &~ mask) | (winFlags & mask);
                sa->SetFlags(swFlags);
            }
        }
        else {
            winAnimator->mEnterAnimationPending = FALSE;
            if (winAnimator->mSurface != NULL) {
                // if (DEBUG_VISIBILITY) Slog.i(TAG, "Relayout invis " + win
                //         + ": mExiting=" + win.mExiting);
                // If we are not currently running the exit animation, we
                // need to see about starting one.
                if (!win->mExiting) {
                    surfaceChanged = TRUE;
                    // Try starting an animation; if there isn't one, we
                    // can destroy the surface right away.
                    Int32 transit = IWindowManagerPolicy::TRANSIT_EXIT;
                    Int32 type1;
                    AutoPtr<IWindowManagerLayoutParams> winAttrs;
                    win->GetAttrs((IWindowManagerLayoutParams**)&winAttrs);
                    winAttrs->GetType(&type1);
                    if (type1 == IWindowManagerLayoutParams::TYPE_APPLICATION_STARTING) {
                        transit = IWindowManagerPolicy::TRANSIT_PREVIEW_DONE;
                    }
                    if (win->IsWinVisibleLw() &&
                            winAnimator->ApplyAnimationLocked(transit, FALSE)) {
                        focusMayChange = isDefaultDisplay;
                        win->mExiting = TRUE;
                    }
                    else if (win->mWinAnimator->IsAnimating()) {
                        // Currently in a hide animation... turn this into
                        // an exit.
                        win->mExiting = TRUE;
                    }
                    else if (win == mWallpaperTarget) {
                        // If the wallpaper is currently behind this
                        // window, we need to change both of them inside
                        // of a transaction to avoid artifacts.
                        win->mExiting = TRUE;
                        win->mWinAnimator->mAnimating = TRUE;
                    }
                    else {
                        if (mInputMethodWindow == win) {
                            mInputMethodWindow = NULL;
                        }
                        winAnimator->DestroySurfaceLocked(FALSE);
                    }
                    ScheduleNotifyWindowTranstionIfNeededLocked(win, transit);
                }
            }

            inSurface->ReleaseSurface();
            // if (DEBUG_VISIBILITY) Slog.i(TAG, "Releasing surface in: " + win);
        }

        if (focusMayChange) {
            // System.out.println("Focus may change: " + win.mAttrs.getTitle());
            if (UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES,
                    FALSE /*updateInputWindows*/)) {
                imMayMove = FALSE;
            }
            // System.out.println("Relayout " + win + ": focus=" + mCurrentFocus);
        }

        // updateFocusedWindowLocked() already assigned layers so we only need to
        // reassign them at this point if the IM window state gets shuffled
        Boolean assignLayers = FALSE;

        if (imMayMove) {
            if (MoveInputMethodWindowsIfNeededLocked(FALSE) || toBeDisplayed) {
                // Little hack here -- we -should- be able to rely on the
                // function to return TRUE if the IME has moved and needs
                // its layer recomputed.  However, if the IME was hidden
                // and isn't actually moved in the list, its layer may be
                // out of data so we make sure to recompute it.
                assignLayers = TRUE;
            }
        }
        if (wallpaperMayMove) {
            if ((AdjustWallpaperWindowsLocked() & ADJUST_WALLPAPER_LAYERS_CHANGED) != 0) {
                assignLayers = TRUE;
            }
        }

        win->mDisplayContent->mLayoutNeeded = TRUE;
        win->mGivenInsetsPending = (flags &
                IWindowManagerGlobal::RELAYOUT_INSETS_PENDING) != 0;
        if (assignLayers) {
            AssignLayersLocked(win->GetWindowList());
        }
        configChanged = UpdateOrientationFromAppTokensLocked(FALSE);
        PerformLayoutAndPlaceSurfacesLocked();
        if (toBeDisplayed && win->mIsWallpaper) {
            AutoPtr<IDisplayInfo> displayInfo = GetDefaultDisplayInfoLocked();
            Int32 width, height;
            displayInfo->GetAppWidth(&width);
            displayInfo->GetAppHeight(&height);
            UpdateWallpaperOffsetLocked(win, width, height, FALSE);
        }
        if (win->mAppToken != NULL) {
            win->mAppToken->UpdateReportedVisibilityLocked();
        }
        inFrame->Set(win->mCompatFrame);
        inContentInsets->Set(win->mContentInsets);
        inVisibleInsets->Set(win->mVisibleInsets);
        // if (localLOGV) Slog.v(
        //     TAG, "Relayout given client " + client.asBinder()
        //     + ", requestedWidth=" + requestedWidth
        //     + ", requestedHeight=" + requestedHeight
        //     + ", viewVisibility=" + viewVisibility
        //     + "\nRelayout returning frame=" + outFrame
        //     + ", surface=" + outSurface);

        // if (localLOGV || DEBUG_FOCUS) Slog.v(
        //     TAG, "Relayout of " + win + ": focusMayChange=" + focusMayChange);

        inTouchMode = mInTouchMode;
        animating = mAnimator->mAnimating;
        List< AutoPtr<WindowState> >::Iterator it = Find(mRelayoutWhileAnimating, win);
        if (animating && it == mRelayoutWhileAnimating.End()) {
            mRelayoutWhileAnimating.PushBack(win);
        }

        mInputMonitor->UpdateInputWindowsLw(TRUE /*force*/);

        // if (DEBUG_LAYOUT) {
        //     Slog.v(TAG, "Relayout complete " + win + ": outFrame=" + outFrame.toShortString());
        // }
    }

    if (configChanged) {
        SendNewConfiguration();
    }

    Binder::RestoreCallingIdentity(origId);

    *outFrame = inFrame;
    REFCOUNT_ADD(*outFrame);
    *outContentInsets = inContentInsets;
    REFCOUNT_ADD(*outContentInsets);
    *outVisibleInsets = inVisibleInsets;
    REFCOUNT_ADD(*outVisibleInsets);
    *outConfig = inConfig;
    REFCOUNT_ADD(*outConfig);
    *outSurface = inSurface;
    REFCOUNT_ADD(*outSurface);

    return (inTouchMode ? IWindowManagerGlobal::RELAYOUT_RES_IN_TOUCH_MODE : 0)
            | (toBeDisplayed ? IWindowManagerGlobal::RELAYOUT_RES_FIRST_TIME : 0)
            | (surfaceChanged ? IWindowManagerGlobal::RELAYOUT_RES_SURFACE_CHANGED : 0)
            | (animating ? IWindowManagerGlobal::RELAYOUT_RES_ANIMATING : 0);
}

void CWindowManagerService::PerformDeferredDestroyWindow(
    /* [in] */ CSession* session,
    /* [in] */ IIWindow* client)
{
    Int64 origId = Binder::ClearCallingIdentity();

    // try {
    AutoLock lock(mWindowMapLock);

    AutoPtr<WindowState> win;
    WindowForClientLocked(session, client, FALSE, (WindowState**)&win);
    if (win == NULL) {
        return;
    }
    win->mWinAnimator->DestroyDeferredSurfaceLocked(FALSE);
    // } finally {
    Binder::RestoreCallingIdentity(origId);
    // }
}

Boolean CWindowManagerService::OutOfMemoryWindow(
    /* [in] */ CSession* session,
    /* [in] */ IIWindow* client)
{
    Int64 origId = Binder::ClearCallingIdentity();

    // try {
    AutoLock lock(mWindowMapLock);

    AutoPtr<WindowState> win;
    WindowForClientLocked(session, client, FALSE, (WindowState**)&win);
    Binder::RestoreCallingIdentity(origId);
    if (win == NULL) {
        return FALSE;
    }
    return ReclaimSomeSurfaceMemoryLocked(win->mWinAnimator, String("from-client"), FALSE);
}

void CWindowManagerService::FinishDrawingWindow(
    /* [in] */ CSession* session,
    /* [in] */ IIWindow* client)
{
    Int64 origId = Binder::ClearCallingIdentity();
    AutoLock lock(mWindowMapLock);

    AutoPtr<WindowState> win;
    WindowForClientLocked(session, client, FALSE, (WindowState**)&win);
    if (win != NULL && win->mWinAnimator->FinishDrawingLocked()) {
        Int32 flags;
        win->mAttrs->GetFlags(&flags);
        if ((flags & IWindowManagerLayoutParams::FLAG_SHOW_WALLPAPER) != 0) {
            AdjustWallpaperWindowsLocked();
        }
        win->mDisplayContent->mLayoutNeeded = TRUE;
        PerformLayoutAndPlaceSurfacesLocked();
    }

    Binder::RestoreCallingIdentity(origId);
}

ECode CWindowManagerService::GetWindowCompatibilityScale(
    /* [in] */ IBinder* windowToken,
    /* [out] */ Float* scale)
{
    VALIDATE_NOT_NULL(scale);

    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::RETRIEVE_WINDOW_INFO,
            String("getWindowCompatibilityScale()"))) {
        Slogger::E(TAG, "Requires RETRIEVE_WINDOW_INFO permission.");
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Requires RETRIEVE_WINDOW_INFO permission.");
    }
    AutoLock lock(mWindowMapLock);
    AutoPtr<WindowState> windowState;
    HashMap<AutoPtr<IBinder>, AutoPtr<WindowState> >::Iterator it
        = mWindowMap.Find(AutoPtr<IBinder>(windowToken));
    if (it != mWindowMap.End()) {
        windowState = it->mSecond;
    }
    *scale = (windowState != NULL) ? windowState->mGlobalScale : 1.0;
    return NOERROR;
}

ECode CWindowManagerService::GetWindowInfo(
    /* [in] */ IBinder* token,
    /* [out] */ IWindowInfo** info)
{
    VALIDATE_NOT_NULL(info);
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::RETRIEVE_WINDOW_INFO,
            String("getWindowInfo()"))) {
        Slogger::E(TAG, "Requires RETRIEVE_WINDOW_INFO permission.");
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Requires RETRIEVE_WINDOW_INFO permission.");
    }
    AutoLock lock(mWindowMapLock);
    AutoPtr<WindowState> window;
    HashMap<AutoPtr<IBinder>, AutoPtr<WindowState> >::Iterator it
            = mWindowMap.Find(AutoPtr<IBinder>(token));
    if (it != mWindowMap.End()) {
        window = it->mSecond;
    }
    AutoPtr<IWindowInfo> winInfo;
    if (window != NULL) {
        winInfo = GetWindowInfoForWindowStateLocked(window);
    }
    *info = winInfo;
    REFCOUNT_ADD(*info);
    return NOERROR;
}

ECode CWindowManagerService::GetVisibleWindowsForDisplay(
    /* [in] */ Int32 displayId,
    /* [out] */ IObjectContainer** outInfos)
{
    VALIDATE_NOT_NULL(outInfos);

    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::RETRIEVE_WINDOW_INFO,
            String("getWindowInfo()"))) {
        Slogger::E(TAG, "Requires RETRIEVE_WINDOW_INFO permission.");
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Requires RETRIEVE_WINDOW_INFO permission.");
    }
    AutoLock lock(mWindowMapLock);
    AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
    if (displayContent == NULL) {
        *outInfos = NULL;
        return NOERROR;
    }

    AutoPtr<IObjectContainer> infos;
    CObjectContainer::New((IObjectContainer**)&infos);
    List< AutoPtr<WindowState> >& windows = displayContent->GetWindowList();
    List< AutoPtr<WindowState> >::Iterator it = windows.Begin();
    for (; it != windows.End(); it++) {
        AutoPtr<WindowState> window = *it;
        Int32 type;
        window->mAttrs->GetType(&type);
        Boolean isVisible;
        if ((window->IsVisibleLw(&isVisible), isVisible)
                || type == IWindowManagerLayoutParams::TYPE_UNIVERSE_BACKGROUND) {
            AutoPtr<IWindowInfo> info = GetWindowInfoForWindowStateLocked(window);
            infos->Add((IInterface*)info);
        }
    }
    *outInfos = infos;
    REFCOUNT_ADD(*outInfos);

    return NOERROR;
}

ECode CWindowManagerService::MagnifyDisplay(
    /* [in] */ Int32 displayId,
    /* [in] */ Float scale,
    /* [in] */ Float offsetX,
    /* [in] */ Float offsetY)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MAGNIFY_DISPLAY,
            String("magnifyDisplay()"))) {
        Slogger::E(TAG, "Requires MAGNIFY_DISPLAY permission.");
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Requires MAGNIFY_DISPLAY permission.");
    }

    AutoLock lock(mWindowMapLock);
    AutoPtr<MagnificationSpec> spec = GetDisplayMagnificationSpecLocked(displayId);
    if (spec != NULL) {
        Boolean scaleChanged = spec->mScale != scale;
        Boolean offsetChanged = spec->mOffsetX != offsetX || spec->mOffsetY != offsetY;
        if (!scaleChanged && !offsetChanged) {
            return NOERROR;
        }
        spec->Initialize(scale, offsetX, offsetY);
        // If the offset has changed we need to re-add the input windows
        // since the offsets have to be propagated to the input system.
        if (offsetChanged) {
            // TODO(multidisplay): Input only occurs on the default display.
            if (displayId == IDisplay::DEFAULT_DISPLAY) {
                mInputMonitor->UpdateInputWindowsLw(TRUE);
            }
        }
        ScheduleAnimationLocked();
    }

    return NOERROR;
}

AutoPtr<MagnificationSpec> CWindowManagerService::GetDisplayMagnificationSpecLocked(
    /* [in] */ Int32 displayId)
{
    AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
    if (displayContent != NULL) {
        if (displayContent->mMagnificationSpec == NULL) {
            displayContent->mMagnificationSpec = new MagnificationSpec();
        }
        return displayContent->mMagnificationSpec;
    }
    return NULL;
}

AutoPtr<IWindowInfo> CWindowManagerService::GetWindowInfoForWindowStateLocked(
    /* [in] */ WindowState* window)
{
    AutoPtr<IWindowInfoHelper> helper;
    CWindowInfoHelper::AcquireSingleton((IWindowInfoHelper**)&helper);
    AutoPtr<IWindowInfo> info;
    ASSERT_SUCCEEDED(helper->Obtain((IWindowInfo**)&info));
    info->SetToken(window->mToken->mToken);
    AutoPtr<IRect> rect;
    info->GetFrame((IRect**)&rect);
    rect->Set(window->mFrame);
    Int32 attrsType;
    window->mAttrs->GetType(&attrsType);
    info->SetType(attrsType);
    info->SetDisplayId(window->GetDisplayId());
    info->SetCompatibilityScale(window->mGlobalScale);
    Boolean isVisible;
    window->IsVisibleLw(&isVisible);
    info->SetVisible(isVisible ||
            attrsType == IWindowManagerLayoutParams::TYPE_UNIVERSE_BACKGROUND);
    info->SetLayer(window->mLayer);
    window->GetTouchableRegion(mTempRegion);
    AutoPtr<IRect> region;
    info->GetTouchableRegion((IRect**)&region);
    Boolean result;
    mTempRegion->GetBounds(region, &result);
    return info;
}

AutoPtr<AttributeCache::Entry> CWindowManagerService::GetCachedAnimations(
    /* [in] */ Int32 userId,
    /* [in] */ IWindowManagerLayoutParams* lp)
{
    // if (DEBUG_ANIM) Slog.v(TAG, "Loading animations: layout params pkg="
    //         + (lp != null ? lp.packageName : null)
    //         + " resId=0x" + (lp != null ? Integer.toHexString(lp.windowAnimations) : null));
    if (lp != NULL) {
        // If this is a system resource, don't try to load it from the
        // application resources.  It is nice to avoid loading application
        // resources if we can.
        Int32 windowAnimations;
        lp->GetWindowAnimations(&windowAnimations);
        if (windowAnimations != 0) {
            String lpPackageName;
            lp->GetPackageName(&lpPackageName);
            String packageName = !lpPackageName.IsNull() ? lpPackageName : String("android");
            Int32 resId;
            lp->GetWindowAnimations(&resId);
            if ((resId & 0xFF000000) == 0x01000000) {
                packageName = "android";
            }
            // if (DEBUG_ANIM) Slog.v(TAG, "Loading animations: picked package="
            //         + packageName);
            AutoPtr<ArrayOf<Int32> > styleable = ArrayOf<Int32>::Alloc(
                    const_cast<Int32 *>(R::styleable::WindowAnimation),
                    ARRAY_SIZE(R::styleable::WindowAnimation));
            return AttributeCache::GetInstance()->Get(userId, packageName, resId,
                    styleable);
        }
    }
    return NULL;
}

AutoPtr<AttributeCache::Entry> CWindowManagerService::GetCachedAnimations(
    /* [in] */ Int32 userId,
    /* [in] */ const String& _packageName,
    /* [in] */ Int32 resId)
{
    String packageName = _packageName;
    // if (DEBUG_ANIM) Slog.v(TAG, "Loading animations: package="
    //         + packageName + " resId=0x" + Integer.toHexString(resId));
    if (!packageName.IsNull()) {
        if ((resId & 0xFF000000) == 0x01000000) {
            packageName = "android";
        }
        // if (DEBUG_ANIM) Slog.v(TAG, "Loading animations: picked package="
        //         + packageName);
        AutoPtr<ArrayOf<Int32> > styleable = ArrayOf<Int32>::Alloc(
                const_cast<Int32 *>(R::styleable::WindowAnimation),
                ARRAY_SIZE(R::styleable::WindowAnimation));
        return AttributeCache::GetInstance()->Get(userId, packageName, resId,
                styleable);
    }
    return NULL;
}

AutoPtr<IAnimation> CWindowManagerService::LoadAnimation(
    /* [in] */ Int32 userId,
    /* [in] */ IWindowManagerLayoutParams* lp,
    /* [in] */ Int32 animAttr)
{
    Int32 anim = 0;
    AutoPtr<IContext> context = mContext;
    if (animAttr >= 0) {
        AutoPtr<AttributeCache::Entry> ent = GetCachedAnimations(userId, lp);
        if (ent != NULL) {
            context = ent->mContext;
            ent->mArray->GetResourceId(animAttr, 0, &anim);
        }
    }
    if (anim != 0) {
        AutoPtr<IAnimationUtils> animationUtils;
        CAnimationUtils::AcquireSingleton((IAnimationUtils**)&animationUtils);
        AutoPtr<IAnimation> animation;
        animationUtils->LoadAnimation(context, anim, (IAnimation**)&animation);
        return animation;
    }
    return NULL;
}

AutoPtr<IAnimation> CWindowManagerService::LoadAnimation(
    /* [in] */ Int32 userId,
    /* [in] */ const String& packageName,
    /* [in] */ Int32 resId)
{
    Int32 anim = 0;
    AutoPtr<IContext> context = mContext;
    if (resId >= 0) {
        AutoPtr<AttributeCache::Entry> ent = GetCachedAnimations(userId, packageName, resId);
        if (ent != NULL) {
            context = ent->mContext;
            anim = resId;
        }
    }
    if (anim != 0) {
        AutoPtr<IAnimationUtils> animationUtils;
        CAnimationUtils::AcquireSingleton((IAnimationUtils**)&animationUtils);
        AutoPtr<IAnimation> animation;
        animationUtils->LoadAnimation(context, anim, (IAnimation**)&animation);
        return animation;
    }
    return NULL;
}

AutoPtr<IAnimation> CWindowManagerService::CreateExitAnimationLocked(
    /* [in] */ Int32 transit,
    /* [in] */ Int32 duration)
{
    AutoPtr<IAnimation> a;
    if (transit == IWindowManagerPolicy::TRANSIT_WALLPAPER_INTRA_OPEN ||
            transit == IWindowManagerPolicy::TRANSIT_WALLPAPER_INTRA_CLOSE) {
        // If we are on top of the wallpaper, we need an animation that
        // correctly handles the wallpaper staying static behind all of
        // the animated elements.  To do this, will just have the existing
        // element fade out.
        ASSERT_SUCCEEDED(CAlphaAnimation::New(1, 0, (IAlphaAnimation**)(IAnimation**)&a));
        a->SetDetachWallpaper(TRUE);
        a->SetDuration(duration);
        return a;
    }
    // For normal animations, the exiting element just holds in place.
    ASSERT_SUCCEEDED(CAlphaAnimation::New(1, 1, (IAlphaAnimation**)(IAnimation**)&a));
    a->SetDuration(duration);
    return a;
}

Float CWindowManagerService::ComputePivot(
    /* [in] */ Int32 startPos,
    /* [in] */ Float finalScale)
{
    Float denom = finalScale-1;
    if (Elastos::Core::Math::Abs(denom) < 0.0001) {
        return startPos;
    }
    return -startPos / denom;
}

AutoPtr<IAnimation> CWindowManagerService::CreateScaleUpAnimationLocked(
    /* [in] */ Int32 transit,
    /* [in] */ Boolean enter)
{
    AutoPtr<IAnimation> a;
    // Pick the desired duration.  If this is an inter-activity transition,
    // it  is the standard duration for that.  Otherwise we use the longer
    // task transition duration.
    Int32 duration;
    AutoPtr<IResources> resource;
    switch (transit) {
        case IWindowManagerPolicy::TRANSIT_ACTIVITY_OPEN:
        case IWindowManagerPolicy::TRANSIT_ACTIVITY_CLOSE:
            mContext->GetResources((IResources**)&resource);
            resource->GetInteger(R::integer::config_shortAnimTime, &duration);
            break;
        default:
            duration = 300;
            break;
    }
    // TODO(multidisplay): For now assume all app animation is on main display.
    AutoPtr<IDisplayInfo> displayInfo = GetDefaultDisplayInfoLocked();
    if (enter) {
        // Entering app zooms out from the center of the initial rect.
        Int32 appW, appH;
        displayInfo->GetAppWidth(&appW);
        displayInfo->GetAppHeight(&appH);
        Float scaleW = mNextAppTransitionStartWidth / (Float)appW;
        Float scaleH = mNextAppTransitionStartHeight / (Float)appH;
        AutoPtr<IAnimation> scale;
        ASSERT_SUCCEEDED(CScaleAnimation::New(scaleW, 1, scaleH, 1,
                ComputePivot(mNextAppTransitionStartX, scaleW),
                ComputePivot(mNextAppTransitionStartY, scaleH),
                (IScaleAnimation**)(IAnimation**)&scale));
        scale->SetDuration(duration);
        AutoPtr<IAnimationSet> set;
        ASSERT_SUCCEEDED(CAnimationSet::New(TRUE, (IAnimationSet**)&set));
        AutoPtr<IAnimation> alpha;
        ASSERT_SUCCEEDED(CAlphaAnimation::New(0, 1,
                (IAlphaAnimation**)(IAnimation**)&alpha));
        scale->SetDuration(duration);
        set->AddAnimation(scale);
        alpha->SetDuration(duration);
        set->AddAnimation(alpha);
        set->SetDetachWallpaper(TRUE);
        a = (IAnimation*)set->Probe(Elastos::Droid::View::Animation::EIID_IAnimation);
    }
    else {
        a = CreateExitAnimationLocked(transit, duration);
    }
    a->SetFillAfter(TRUE);
    AutoPtr<IAnimationUtils> animationUtils;
    CAnimationUtils::AcquireSingleton((IAnimationUtils**)&animationUtils);
    AutoPtr<IInterpolator> interpolator;
    animationUtils->LoadInterpolator(mContext, R::interpolator::decelerate_cubic,
            (IInterpolator**)&interpolator);
    a->SetInterpolator(interpolator);
    Int32 w, h;
    displayInfo->GetAppWidth(&w);
    displayInfo->GetAppHeight(&h);
    a->Initialize(w, h, w, h);
    return a;
}

AutoPtr<IAnimation> CWindowManagerService::CreateThumbnailAnimationLocked(
    /* [in] */ Int32 transit,
    /* [in] */ Boolean enter,
    /* [in] */ Boolean thumb,
    /* [in] */ Boolean scaleUp)
{
    AutoPtr<IAnimation> a;
    Int32 thumbWidthI;
    mNextAppTransitionThumbnail->GetWidth(&thumbWidthI);
    Float thumbWidth = thumbWidthI > 0 ? thumbWidthI : 1;
    Int32 thumbHeightI;
    mNextAppTransitionThumbnail->GetHeight(&thumbHeightI);
    Float thumbHeight = thumbHeightI > 0 ? thumbHeightI : 1;

    AutoPtr<IAnimationUtils> animationUtils;
    CAnimationUtils::AcquireSingleton((IAnimationUtils**)&animationUtils);

    // Pick the desired duration.  If this is an inter-activity transition,
    // it  is the standard duration for that.  Otherwise we use the longer
    // task transition duration.
    Int32 duration;
    AutoPtr<IResources> resource;
    switch (transit) {
        case IWindowManagerPolicy::TRANSIT_ACTIVITY_OPEN:
        case IWindowManagerPolicy::TRANSIT_ACTIVITY_CLOSE:
            mContext->GetResources((IResources**)&resource);
            resource->GetInteger(R::integer::config_shortAnimTime, &duration);
            break;
        default:
            duration = 250;
            break;
    }

    // TOOD(multidisplay): For now assume all app animation is on the main screen.
    AutoPtr<IDisplayInfo> displayInfo = GetDefaultDisplayInfoLocked();
    Int32 w, h;
    displayInfo->GetAppWidth(&w);
    displayInfo->GetAppHeight(&h);
    if (thumb) {
        // Animation for zooming thumbnail from its initial size to
        // filling the screen.
        if (scaleUp) {
            Float scaleW = w / thumbWidth;
            Float scaleH = h / thumbHeight;

            AutoPtr<IAnimation> scale;
            ASSERT_SUCCEEDED(CScaleAnimation::New(1, scaleW, 1, scaleH,
                    ComputePivot(mNextAppTransitionStartX, 1 / scaleW),
                    ComputePivot(mNextAppTransitionStartY, 1 / scaleH),
                    (IScaleAnimation**)(IAnimation**)&scale));
            AutoPtr<IAnimationSet> set;
            ASSERT_SUCCEEDED(CAnimationSet::New(FALSE, (IAnimationSet**)&set));
            AutoPtr<IAnimation> alpha;
            ASSERT_SUCCEEDED(CAlphaAnimation::New(1, 0, (IAlphaAnimation**)(IAnimation**)&alpha));
            scale->SetDuration(duration);
            AutoPtr<IInterpolator> interpolator;
            animationUtils->LoadInterpolator(mContext, R::interpolator::decelerate_quad, (IInterpolator**)&interpolator);
            scale->SetInterpolator(interpolator);
            set->AddAnimation(scale);
            alpha->SetDuration(duration);
            AutoPtr<IInterpolator> ipolator = new LocalInterpolator();
            alpha->SetInterpolator(ipolator);
            set->AddAnimation(alpha);
            set->SetFillBefore(TRUE);
            a = IAnimation::Probe(set);
        }
        else {
            Float scaleW = w / thumbWidth;
            Float scaleH = h / thumbHeight;

            AutoPtr<IAnimation> scale;
            ASSERT_SUCCEEDED(CScaleAnimation::New(scaleW, 1, scaleH, 1,
                    ComputePivot(mNextAppTransitionStartX, 1 / scaleW),
                    ComputePivot(mNextAppTransitionStartY, 1 / scaleH),
                    (IScaleAnimation**)(IAnimation**)&scale));
            AutoPtr<IAnimationSet> set;
            ASSERT_SUCCEEDED(CAnimationSet::New(TRUE, (IAnimationSet**)&set));
            AutoPtr<IAnimation> alpha;
            ASSERT_SUCCEEDED(CAlphaAnimation::New(1, 1, (IAlphaAnimation**)(IAnimation**)&alpha));
            scale->SetDuration(duration);
            AutoPtr<IInterpolator> interpolator;
            CDecelerateInterpolator::New(THUMBNAIL_ANIMATION_DECELERATE_FACTOR,
                    (IDecelerateInterpolator**)&interpolator);
            scale->SetInterpolator(interpolator);
            set->AddAnimation(scale);
            alpha->SetDuration(duration);
            set->AddAnimation(alpha);
            set->SetFillBefore(TRUE);

            a = IAnimation::Probe(set);
        }
    }
    else if (enter) {
        // Entering app zooms out from the center of the thumbnail.
        if (scaleUp) {
            Float scaleW = thumbWidth / w;
            Float scaleH = thumbHeight / h;
            AutoPtr<IAnimation> scale;
            ASSERT_SUCCEEDED(CScaleAnimation::New(scaleW, 1, scaleH, 1,
                    ComputePivot(mNextAppTransitionStartX, scaleW),
                    ComputePivot(mNextAppTransitionStartY, scaleH),
                    (IScaleAnimation**)(IAnimation**)&scale));
            scale->SetDuration(duration);
            AutoPtr<IInterpolator> interpolator;
            CDecelerateInterpolator::New(THUMBNAIL_ANIMATION_DECELERATE_FACTOR,
                    (IDecelerateInterpolator**)&interpolator);
            scale->SetInterpolator(interpolator);
            scale->SetFillBefore(TRUE);
            a = scale;
        }
        else {
            // noop animation
            ASSERT_SUCCEEDED(CAlphaAnimation::New(1, 1,
                    (IAlphaAnimation**)(IAnimation**)&a));
            a->SetDuration(duration);
        }
    }
    else {
        // Exiting app
        if (scaleUp) {
            if (transit == IWindowManagerPolicy::TRANSIT_WALLPAPER_INTRA_OPEN) {
                // Fade out while bringing up selected activity. This keeps the
                // current activity from showing through a launching wallpaper
                // activity.
                ASSERT_SUCCEEDED(CAlphaAnimation::New(1, 0, (IAlphaAnimation**)(IAnimation**)&a));
            }
            else {
                // noop animation
                ASSERT_SUCCEEDED(CAlphaAnimation::New(1, 1, (IAlphaAnimation**)(IAnimation**)&a));
            }
            a->SetDuration(duration);
        }
        else {
            Float scaleW = thumbWidth / w;
            Float scaleH = thumbHeight / h;
            AutoPtr<IAnimation> scale;
            ASSERT_SUCCEEDED(CScaleAnimation::New(1, scaleW, 1, scaleH,
                    ComputePivot(mNextAppTransitionStartX, scaleW),
                    ComputePivot(mNextAppTransitionStartY, scaleH),
                    (IScaleAnimation**)(IAnimation**)&scale));
            scale->SetDuration(duration);
            AutoPtr<IInterpolator> scaleInterpolator;
            CDecelerateInterpolator::New(THUMBNAIL_ANIMATION_DECELERATE_FACTOR,
                    (IDecelerateInterpolator**)&scaleInterpolator);
            scale->SetInterpolator(scaleInterpolator);
            scale->SetFillBefore(TRUE);

            AutoPtr<IAnimation> alpha;
            ASSERT_SUCCEEDED(CAlphaAnimation::New(1, 0, (IAlphaAnimation**)(IAnimation**)&alpha));
            alpha->SetDuration(duration);
            AutoPtr<IInterpolator> alphaInterpolator;
            CDecelerateInterpolator::New(THUMBNAIL_ANIMATION_DECELERATE_FACTOR,
                    (IDecelerateInterpolator**)&alphaInterpolator);
            alpha->SetInterpolator(alphaInterpolator);

            AutoPtr<IAnimationSet> set;
            ASSERT_SUCCEEDED(CAnimationSet::New(TRUE, (IAnimationSet**)&set));
            set->AddAnimation(scale);
            set->AddAnimation(alpha);
            set->SetFillBefore(TRUE);
            set->SetZAdjustment(IAnimation::ZORDER_TOP);
            a = IAnimation::Probe(set);
        }
    }
    a->SetFillAfter(TRUE);
    AutoPtr<IInterpolator> interpolator;
    animationUtils->LoadInterpolator(mContext, R::interpolator::decelerate_quad,
            (IInterpolator**)&interpolator);
    a->SetInterpolator(interpolator);
    a->Initialize(w, h, w, h);
    return a;
}

Boolean CWindowManagerService::ApplyAnimationLocked(
    /* [in] */ AppWindowToken* atoken,
    /* [in] */ IWindowManagerLayoutParams* lp,
    /* [in] */ Int32 transit,
    /* [in] */ Boolean enter)
{
    // Only apply an animation if the display isn't frozen.  If it is
    // frozen, there is no reason to animate and it can cause strange
    // artifacts when we unfreeze the display if some different animation
    // is running.
    if (OkToDisplay()) {
        AutoPtr<IAnimation> a;
        Boolean initialized = FALSE;
        if (mNextAppTransitionType == IActivityOptions::ANIM_CUSTOM) {
            a = LoadAnimation(atoken->mUserId, mNextAppTransitionPackage, enter ?
                    mNextAppTransitionEnter : mNextAppTransitionExit);
            // if (DEBUG_APP_TRANSITIONS || DEBUG_ANIM) Slog.v(TAG,
            //         "applyAnimation: atoken=" + atoken
            //         + " anim=" + a + " nextAppTransition=ANIM_CUSTOM"
            //         + " transit=" + transit + " isEntrance=" + enter
            //         + " Callers=" + Debug.getCallers(3));
        }
        else if (mNextAppTransitionType == IActivityOptions::ANIM_SCALE_UP) {
            a = CreateScaleUpAnimationLocked(transit, enter);
            initialized = TRUE;
            // if (DEBUG_APP_TRANSITIONS || DEBUG_ANIM) Slog.v(TAG,
            //         "applyAnimation: atoken=" + atoken
            //         + " anim=" + a + " nextAppTransition=ANIM_SCALE_UP"
            //         + " transit=" + transit + " isEntrance=" + enter
            //         + " Callers=" + Debug.getCallers(3));
        }
        else if (mNextAppTransitionType == IActivityOptions::ANIM_THUMBNAIL_SCALE_UP ||
                mNextAppTransitionType == IActivityOptions::ANIM_THUMBNAIL_SCALE_DOWN) {
            Boolean scaleUp = (mNextAppTransitionType == IActivityOptions::ANIM_THUMBNAIL_SCALE_UP);
            a = CreateThumbnailAnimationLocked(transit, enter, FALSE, scaleUp);
            initialized = TRUE;
            // if (DEBUG_APP_TRANSITIONS || DEBUG_ANIM) {
            //     String animName = scaleUp ? "ANIM_THUMBNAIL_SCALE_UP" : "ANIM_THUMBNAIL_SCALE_DOWN";
            //     Slog.v(TAG, "applyAnimation: atoken=" + atoken
            //             + " anim=" + a + " nextAppTransition=" + animName
            //             + " transit=" + transit + " isEntrance=" + enter
            //             + " Callers=" + Debug.getCallers(3));
            // }
        }
        else {
            Int32 animAttr = 0;
            switch (transit) {
                case IWindowManagerPolicy::TRANSIT_ACTIVITY_OPEN:
                    animAttr = enter
                            ? R::styleable::WindowAnimation_activityOpenEnterAnimation
                            : R::styleable::WindowAnimation_activityOpenExitAnimation;
                    break;
                case IWindowManagerPolicy::TRANSIT_ACTIVITY_CLOSE:
                    animAttr = enter
                            ? R::styleable::WindowAnimation_activityCloseEnterAnimation
                            : R::styleable::WindowAnimation_activityCloseExitAnimation;
                    break;
                case IWindowManagerPolicy::TRANSIT_TASK_OPEN:
                    animAttr = enter
                            ? R::styleable::WindowAnimation_taskOpenEnterAnimation
                            : R::styleable::WindowAnimation_taskOpenExitAnimation;
                    break;
                case IWindowManagerPolicy::TRANSIT_TASK_CLOSE:
                    animAttr = enter
                            ? R::styleable::WindowAnimation_taskCloseEnterAnimation
                            : R::styleable::WindowAnimation_taskCloseExitAnimation;
                    break;
                case IWindowManagerPolicy::TRANSIT_TASK_TO_FRONT:
                    animAttr = enter
                            ? R::styleable::WindowAnimation_taskToFrontEnterAnimation
                            : R::styleable::WindowAnimation_taskToFrontExitAnimation;
                    break;
                case IWindowManagerPolicy::TRANSIT_TASK_TO_BACK:
                    animAttr = enter
                            ? R::styleable::WindowAnimation_taskToBackEnterAnimation
                            : R::styleable::WindowAnimation_taskToBackExitAnimation;
                    break;
                case IWindowManagerPolicy::TRANSIT_WALLPAPER_OPEN:
                    animAttr = enter
                            ? R::styleable::WindowAnimation_wallpaperOpenEnterAnimation
                            : R::styleable::WindowAnimation_wallpaperOpenExitAnimation;
                    break;
                case IWindowManagerPolicy::TRANSIT_WALLPAPER_CLOSE:
                    animAttr = enter
                            ? R::styleable::WindowAnimation_wallpaperCloseEnterAnimation
                            : R::styleable::WindowAnimation_wallpaperCloseExitAnimation;
                    break;
                case IWindowManagerPolicy::TRANSIT_WALLPAPER_INTRA_OPEN:
                    animAttr = enter
                            ? R::styleable::WindowAnimation_wallpaperIntraOpenEnterAnimation
                            : R::styleable::WindowAnimation_wallpaperIntraOpenExitAnimation;
                    break;
                case IWindowManagerPolicy::TRANSIT_WALLPAPER_INTRA_CLOSE:
                    animAttr = enter
                            ? R::styleable::WindowAnimation_wallpaperIntraCloseEnterAnimation
                            : R::styleable::WindowAnimation_wallpaperIntraCloseExitAnimation;
                    break;
            }
            a = animAttr != 0 ? LoadAnimation(atoken->mUserId, lp, animAttr) : NULL;
            // if (DEBUG_APP_TRANSITIONS || DEBUG_ANIM) Slog.v(TAG,
            //         "applyAnimation: atoken=" + atoken
            //         + " anim=" + a
            //         + " animAttr=0x" + Integer.toHexString(animAttr)
            //         + " transit=" + transit + " isEntrance=" + enter
            //         + " Callers=" + Debug.getCallers(3));
        }

        if (a != NULL) {
            // if (DEBUG_ANIM) {
            //     RuntimeException e = null;
            //     if (!HIDE_STACK_CRAWLS) {
            //         e = new RuntimeException();
            //         e.fillInStackTrace();
            //     }
            //     Slog.v(TAG, "Loaded animation " + a + " for " + atoken, e);
            // }
            atoken->mAppAnimator->SetAnimation(a, initialized);
        }
    }
    else {
        atoken->mAppAnimator->ClearAnimation();
    }

    return atoken->mAppAnimator->mAnimation != NULL;
}

// -------------------------------------------------------------
// Application Window Tokens
// -------------------------------------------------------------

void CWindowManagerService::ValidateAppTokens(
    /* [in] */ List< AutoPtr<IBinder> > & tokens)
{
    List< AutoPtr<IBinder> >::ReverseIterator rit = tokens.RBegin();
    List< AutoPtr<AppWindowToken> >::ReverseIterator appRIt = mAppTokens.RBegin();
    while (rit != tokens.REnd() && appRIt != mAppTokens.REnd()) {
        AutoPtr<AppWindowToken> atoken = *appRIt;
        if (atoken->mRemoved) {
            ++appRIt;
            continue;
        }
        if (*rit != atoken->mToken) {
            Slogger::W(TAG, "Tokens out of sync: external is %p, internal is %p",
                    (*rit).Get(), atoken->mToken.Get());
        }
        ++rit;
        ++appRIt;
    }
    while (rit != tokens.REnd()) {
        Slogger::W(TAG, "External token not found: %p", (*rit).Get());
        ++rit;
    }
    while (appRIt != mAppTokens.REnd()) {
        AutoPtr<AppWindowToken> atoken = *appRIt;
        if (!atoken->mRemoved) {
            Slogger::W(TAG, "Invalid internal token: %p", atoken->mToken.Get());
        }
        ++appRIt;
    }
}

Boolean CWindowManagerService::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& func)
{
    // Quick check: if the calling permission is me, it's all okay.
    if (Binder::GetCallingPid() == Process::MyPid()) {
      return TRUE;
    }

    Int32 perm;
    FAIL_RETURN(mContext->CheckCallingPermission(permission, &perm));
    if (perm == IPackageManager::PERMISSION_GRANTED) {
      return TRUE;
    }

    Slogger::W(TAG, "Permission Denial: %s from pid=%d, uid=%d requires %s",
            (const char*)func, Binder::GetCallingPid(), Binder::GetCallingUid(),
            (const char*)permission);
    return FALSE;
}

Boolean CWindowManagerService::OkToDisplay()
{
    Boolean isON;
    mPolicy->IsScreenOnFully(&isON);
    return !mDisplayFrozen && mDisplayEnabled && isON;
}

AutoPtr<AppWindowToken> CWindowManagerService::FindAppWindowToken(
    /* [in] */ IBinder* token)
{
    HashMap<AutoPtr<IBinder>, AutoPtr<WindowToken>  >::Iterator
        wtit = mTokenMap.Find(AutoPtr<IBinder>(token));
    if (wtit != mTokenMap.End() && wtit->mSecond != NULL) {
        return wtit->mSecond->mAppWindowToken;
    }
    else {
        return NULL;
    }
}

ECode CWindowManagerService::AddWindowToken(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 type)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("AddWindowToken()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoLock lock(mWindowMapLock);

    AutoPtr<WindowToken> wtoken;
    HashMap<AutoPtr<IBinder>, AutoPtr<WindowToken> >::Iterator tokenIt = mTokenMap.Find(token);
    if (tokenIt != mTokenMap.End()) {
        wtoken = tokenIt->mSecond;
    }

    if (wtoken != NULL) {
        Slogger::W(TAG, "Attempted to add existing input method token: %p", token);
        return NOERROR;
    }
    wtoken = new WindowToken(this, token, type, TRUE);
    mTokenMap[token] = wtoken;
    if (type == IWindowManagerLayoutParams::TYPE_WALLPAPER) {
        mWallpaperTokens.PushBack(wtoken);
    }

    return NOERROR;
}

ECode CWindowManagerService::RemoveWindowToken(
    /* [in] */ IBinder* token)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("RemoveWindowToken()"))) {
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Int64 origId = Binder::ClearCallingIdentity();
    AutoLock lock(mWindowMapLock);

    AutoPtr<WindowToken> wtoken;
    HashMap<AutoPtr<IBinder>, AutoPtr<WindowToken> >::Iterator tokenIt
            = mTokenMap.Find(token);
    if (tokenIt != mTokenMap.End()) {
        wtoken = tokenIt->mSecond;
        mTokenMap.Erase(tokenIt);
    }
    if (wtoken != NULL) {
        Boolean delayed = FALSE;
        if (!wtoken->mHidden) {
            Boolean changed = FALSE;
            List< AutoPtr<WindowState> >::Iterator it = wtoken->mWindows.Begin();
            for (; it != wtoken->mWindows.End(); ++it) {
                AutoPtr<WindowState> win = *it;

                if (win->mWinAnimator->IsAnimating()) {
                    delayed = TRUE;
                }

                if (win->IsVisibleNow()) {
                    win->mWinAnimator->ApplyAnimationLocked(
                            IWindowManagerPolicy::TRANSIT_EXIT, FALSE);
                    ScheduleNotifyWindowTranstionIfNeededLocked(win,
                            IWindowManagerPolicy::TRANSIT_EXIT);
                    changed = TRUE;
                    win->mDisplayContent->mLayoutNeeded = TRUE;
                }
            }

            wtoken->mHidden = TRUE;

            if (changed) {
                PerformLayoutAndPlaceSurfacesLocked();
                UpdateFocusedWindowLocked(UPDATE_FOCUS_NORMAL,
                        FALSE /*updateInputWindows*/);
            }

            if (delayed) {
                mExitingTokens.PushBack(wtoken);
            }
            else if (wtoken->mWindowType == IWindowManagerLayoutParams::TYPE_WALLPAPER) {
                mWallpaperTokens.Remove(wtoken);
                UpdateLayoutToAnimWallpaperTokens();
            }
        }

        mInputMonitor->UpdateInputWindowsLw(TRUE /*force*/);
    }
    else {
        Slogger::W(TAG, "Attempted to remove non-existing token: %p", token);
    }

    Binder::RestoreCallingIdentity(origId);
    return NOERROR;
}

void CWindowManagerService::AddAppTokenToAnimating(
    /* [in] */ Int32 addPos,
    /* [in] */ Int32 userId,
    /* [in] */ AppWindowToken* atoken)
{
    if (addPos == 0 || (UInt32)addPos == mAnimatingAppTokens.GetSize()) {
        // It was inserted into the beginning or end of mAppTokens. Honor that.
        mAnimatingAppTokens.Insert(addPos, AutoPtr<AppWindowToken>(atoken));
        return;
    }
    // Find the item immediately above the mAppTokens insertion point and put the token
    // immediately below that one in mAnimatingAppTokens.
    AutoPtr<AppWindowToken> aboveAnchor = mAppTokens[addPos + 1];
    List< AutoPtr<AppWindowToken> >::Iterator it = mAnimatingAppTokens.Begin();
    for (; it != mAnimatingAppTokens.End(); ++it) {
        if (*it == aboveAnchor) break;
    }
    mAnimatingAppTokens.Insert(it, atoken);
}

ECode CWindowManagerService::AddAppToken(
    /* [in] */ Int32 addPos,
    /* [in] */ Int32 userId,
    /* [in] */ IApplicationToken* token,
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 requestedOrientation,
    /* [in] */ Boolean fullscreen,
    /* [in] */ Boolean showWhenLocked)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("AddAppToken()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    // Get the dispatching timeout here while we are not holding any locks so that it
    // can be cached by the AppWindowToken.  The timeout value is used later by the
    // input dispatcher in code that does hold locks.  If we did not cache the value
    // here we would run the chance of introducing a deadlock between the window manager
    // (which holds locks while updating the input dispatcher state) and the activity manager
    // (which holds locks while querying the application token).
    Int64 inputDispatchingTimeoutNanos;
    // try {
    Int64 timeout;
    ECode ec = token->GetKeyDispatchingTimeout(&timeout);
    if (FAILED(ec)) {
        Slogger::W(TAG, "Could not get dispatching timeout.0x%08x", ec);
        inputDispatchingTimeoutNanos = DEFAULT_INPUT_DISPATCHING_TIMEOUT_NANOS;
    }
    else {
        inputDispatchingTimeoutNanos = timeout * 1000000ll;
    }
    // } catch (RemoteException ex) {
    //     Slog.w(TAG, "Could not get dispatching timeout.", ex);
    //     inputDispatchingTimeoutNanos = DEFAULT_INPUT_DISPATCHING_TIMEOUT_NANOS;
    // }

    AutoLock lock(mWindowMapLock);

    AutoPtr<AppWindowToken> atoken = FindAppWindowToken(IBinder::Probe(token));
    if (atoken != NULL) {
        Slogger::W(TAG, "Attempted to add existing app token: %p", token);
        return NOERROR;
    }

    atoken = new AppWindowToken(this, userId, token);
    atoken->Init();

    atoken->mInputDispatchingTimeoutNanos = inputDispatchingTimeoutNanos;
    atoken->mGroupId = groupId;
    atoken->mAppFullscreen = fullscreen;
    atoken->mShowWhenLocked = showWhenLocked;
    atoken->mRequestedOrientation = requestedOrientation;
    if (DEBUG_TOKEN_MOVEMENT || DEBUG_ADD_REMOVE) {
        Slogger::V(TAG, "addAppToken: %p at %d", atoken.Get(), addPos);
    }
    mAppTokens.Insert(addPos, atoken);
    AddAppTokenToAnimating(addPos, userId, atoken);
    mTokenMap[IBinder::Probe(token)] = atoken;

    // Application tokens start out hidden.
    atoken->mHidden = TRUE;
    atoken->mHiddenRequested = TRUE;

    return NOERROR;
}

ECode CWindowManagerService::SetAppGroupId(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 groupId)
{
    if (!CheckCallingPermission(
        Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
        String("SetAppStartingIcon()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoLock lock(mWindowMapLock);

    AutoPtr<AppWindowToken> atoken = FindAppWindowToken(token);
    if (atoken == NULL) {
        Slogger::W(TAG, "Attempted to set group id of non-existing app token: %p", token);
        return NOERROR;
    }
    atoken->mGroupId = groupId;
    return NOERROR;
}

Int32 CWindowManagerService::GetOrientationFromWindowsLocked()
{
    if (mDisplayFrozen
            || mOpeningApps.Begin() != mOpeningApps.End()
            || mClosingApps.Begin() != mClosingApps.End()) {
        // If the display is frozen, some activities may be in the middle
        // of restarting, and thus have removed their old window.  If the
        // window has the flag to hide the lock screen, then the lock screen
        // can re-appear and inflict its own orientation on us.  Keep the
        // orientation stable until this all settles down.
        return mLastWindowForcedOrientation;
    }

    // TODO(multidisplay): Change to the correct display.
    List< AutoPtr<WindowState> >& windows = GetDefaultWindowListLocked();
    List< AutoPtr<WindowState> >::ReverseIterator rit = windows.RBegin();
    while (rit != windows.REnd()) {
        AutoPtr<WindowState> wtoken = *rit;
        ++rit;
        if (wtoken->mAppToken != NULL) {
            // We hit an application window. so the orientation will be determined by the
            // app window. No point in continuing further.
            return (mLastWindowForcedOrientation = IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED);
        }
        Boolean isVisible;
        if ((wtoken->IsVisibleLw(&isVisible), !isVisible) || !wtoken->mPolicyVisibilityAfterAnim) {
            continue;
        }

        Int32 req;
        wtoken->mAttrs->GetScreenOrientation(&req);
        if((req == IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED) ||
                (req == IActivityInfo::SCREEN_ORIENTATION_BEHIND)){
            continue;
        }

        // if (DEBUG_ORIENTATION) Slog.v(TAG, win + " forcing orientation to " + req);
        return (mLastWindowForcedOrientation = req);
    }
    return (mLastWindowForcedOrientation = IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED);
}

Int32 CWindowManagerService::GetOrientationFromAppTokensLocked()
{
    Int32 curGroup = 0;
    Int32 lastOrientation = IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED;
    Boolean findingBehind = FALSE;
    Boolean haveGroup = FALSE;
    Boolean lastFullscreen = FALSE;
    List< AutoPtr<AppWindowToken> >::ReverseIterator rit;
    for (rit = mAppTokens.RBegin(); rit != mAppTokens.REnd(); ++rit) {
        AutoPtr<AppWindowToken> atoken = *rit;

        // if (DEBUG_APP_ORIENTATION) Slog.v(TAG, "Checking app orientation: " + atoken);

        // if we're about to tear down this window and not seek for
        // the behind activity, don't use it for orientation
        if (!findingBehind
                && (!atoken->mHidden && atoken->mHiddenRequested)) {
            continue;
        }

        if (haveGroup == true && curGroup != atoken->mGroupId) {
            // If we have hit a new application group, and the bottom
            // of the previous group didn't explicitly say to use
            // the orientation behind it, and the last app was
            // full screen, then we'll stick with the
            // user's orientation.
            if (lastOrientation != IActivityInfo::SCREEN_ORIENTATION_BEHIND
                    && lastFullscreen) {
                if (DEBUG_ORIENTATION) Slogger::V(TAG, "Done at %p -- end of group, return %d", atoken.Get(),
                        lastOrientation);
                return lastOrientation;
            }
        }

        // We ignore any hidden applications on the top.
        if (atoken->mHiddenRequested || atoken->mWillBeHidden) {
            if (DEBUG_ORIENTATION) Slogger::V(TAG, "Skipping %p -- hidden on top", atoken.Get());
            continue;
        }

        if (!haveGroup) {
            haveGroup = TRUE;
            curGroup = atoken->mGroupId;
            lastOrientation = atoken->mRequestedOrientation;
        }

        Int32 orientation = atoken->mRequestedOrientation;
        // If this application is fullscreen, and didn't explicitly say
        // to use the orientation behind it, then just take whatever
        // orientation it has and ignores whatever is under it.
        lastFullscreen = atoken->mAppFullscreen;
        if (lastFullscreen
                && orientation != IActivityInfo::SCREEN_ORIENTATION_BEHIND) {
            if (DEBUG_ORIENTATION) Slogger::V(TAG, "Done at %p -- full screen, return %d", atoken.Get(),
                    orientation);
            return orientation;
        }
        // If this application has requested an explicit orientation,
        // then use it.
        if (orientation != IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED
                && orientation != IActivityInfo::SCREEN_ORIENTATION_BEHIND) {
            return orientation;
        }
        findingBehind |= (orientation == IActivityInfo::SCREEN_ORIENTATION_BEHIND);
    }
    if (DEBUG_ORIENTATION) Slogger::V(TAG, "No app is requesting an orientation");
    return IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED;
}

ECode CWindowManagerService::UpdateOrientationFromAppTokens(
    /* [in] */ IConfiguration* currentConfig,
    /* [in] */ IBinder* freezeThisOneIfNeeded,
    /* [out] */ IConfiguration** config)
{
    VALIDATE_NOT_NULL(config);

    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("UpdateOrientationFromAppTokens()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Int64 ident = Binder::ClearCallingIdentity();

    AutoLock lock(mWindowMapLock);

    AutoPtr<IConfiguration> temp = UpdateOrientationFromAppTokensLocked(
            currentConfig, freezeThisOneIfNeeded);

    Binder::RestoreCallingIdentity(ident);
    *config = temp;
    REFCOUNT_ADD(*config);
    return NOERROR;
}

AutoPtr<IConfiguration> CWindowManagerService::UpdateOrientationFromAppTokensLocked(
    /* [in] */ IConfiguration* currentConfig,
    /* [in] */ IBinder* freezeThisOneIfNeeded)
{
    AutoPtr<IConfiguration> config;

    if (UpdateOrientationFromAppTokensLocked(FALSE)) {
        if (freezeThisOneIfNeeded != NULL) {
            AutoPtr<AppWindowToken> atoken = FindAppWindowToken(freezeThisOneIfNeeded);
            if (atoken != NULL) {
                StartAppFreezingScreenLocked(atoken,
                        IActivityInfo::CONFIG_ORIENTATION);
            }
        }
        config = ComputeNewConfigurationLocked();

    }
    else if (currentConfig != NULL) {
        // No obvious action we need to take, but if our current
        // state mismatches the activity manager's, update it,
        // disregarding font scale, which should remain set to
        // the value of the previous configuration.
        mTempConfiguration->SetToDefaults();
        Float fontScale;
        currentConfig->GetFontScale(&fontScale);
        mTempConfiguration->SetFontScale(fontScale);
        if (ComputeScreenConfigurationLocked(mTempConfiguration)) {
            Int32 res;
            currentConfig->Diff(mTempConfiguration, &res);
            if (res != 0) {
                mWaitingForConfig = TRUE;
                GetDefaultDisplayContentLocked()->mLayoutNeeded = TRUE;
                StartFreezingDisplayLocked(FALSE, 0, 0);
                ASSERT_SUCCEEDED(CConfiguration::New(mTempConfiguration,
                        (IConfiguration**)&config));
            }
        }
    }

    return config;
}

Boolean CWindowManagerService::UpdateOrientationFromAppTokensLocked(
    /* [in] */ Boolean inTransaction)
{
    Int64 ident = Binder::ClearCallingIdentity();

    //try {
    Int32 req = ComputeForcedAppOrientationLocked();

    if (req != mForcedAppOrientation) {
        mForcedAppOrientation = req;
        //send a message to Policy indicating orientation change to take
        //action like disabling/enabling sensors etc.,
        mPolicy->SetCurrentOrientationLw(req);
        if (UpdateRotationUncheckedLocked(inTransaction)) {
            // changed
            Binder::RestoreCallingIdentity(ident);
            return TRUE;
        }
    }

    Binder::RestoreCallingIdentity(ident);
    return FALSE;
}

Int32 CWindowManagerService::ComputeForcedAppOrientationLocked()
{
    Int32 req = GetOrientationFromWindowsLocked();
    if (req == IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED) {
        req = GetOrientationFromAppTokensLocked();
    }
    req = IActivityInfo::SCREEN_ORIENTATION_LANDSCAPE;
    return req;
}

ECode CWindowManagerService::SetNewConfiguration(
    /* [in] */ IConfiguration* config)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("SetNewConfiguration()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoLock lock(mWindowMapLock);
    mCurConfiguration = NULL;
    FAIL_RETURN(CConfiguration::New(config, (IConfiguration**)&mCurConfiguration));
    mWaitingForConfig = FALSE;
    PerformLayoutAndPlaceSurfacesLocked();
    return NOERROR;
}

ECode CWindowManagerService::SetAppOrientation(
    /* [in] */ IApplicationToken* token,
    /* [in] */ Int32 requestedOrientation)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("SetAppOrientation()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoLock lock(mWindowMapLock);
    AutoPtr<AppWindowToken> atoken = FindAppWindowToken(IBinder::Probe(token));
    if (atoken == NULL) {
        Slogger::W(TAG, "Attempted to set orientation of non-existing app token: %p", token);
        return NOERROR;
    }

    atoken->mRequestedOrientation = requestedOrientation;

    return NOERROR;
}

ECode CWindowManagerService::GetAppOrientation(
    /* [in] */ IApplicationToken* token,
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);

    AutoLock lock(mWindowMapLock);
    AutoPtr<AppWindowToken> wtoken = FindAppWindowToken(IBinder::Probe(token));
    if (wtoken == NULL) {
        *orientation = IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED;
        return NOERROR;
    }

    *orientation = wtoken->mRequestedOrientation;
    return NOERROR;
}

ECode CWindowManagerService::SetFocusedApp(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean moveFocusNow)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("SetFocusedApp()"))) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoLock lock(mWindowMapLock);

    Boolean changed = FALSE;
    if (token == NULL) {
        // if (DEBUG_FOCUS) Slog.v(TAG, "Clearing focused app, was " + mFocusedApp);
        changed = mFocusedApp != NULL;
        mFocusedApp = NULL;
        if (changed) {
            mInputMonitor->SetFocusedAppLw(NULL);
        }
    }
    else {
        AutoPtr<AppWindowToken> newFocus = FindAppWindowToken(token);
        if (newFocus == NULL) {
            Slogger::W(TAG, "Attempted to set focus to non-existing app token: %p");
            return NOERROR;
        }
        changed = mFocusedApp != newFocus;
        mFocusedApp = newFocus;
        // if (DEBUG_FOCUS) Slog.v(TAG, "Set focused app to: " + mFocusedApp);
        if (changed) {
            mInputMonitor->SetFocusedAppLw(newFocus);
        }
    }

    if (moveFocusNow && changed) {
        Int64 origId = Binder::ClearCallingIdentity();
        UpdateFocusedWindowLocked(UPDATE_FOCUS_NORMAL, TRUE /*updateInputWindows*/);
        Binder::RestoreCallingIdentity(origId);
    }

    return NOERROR;
}

ECode CWindowManagerService::PrepareAppTransition(
    /* [in] */ Int32 transit,
    /* [in] */ Boolean alwaysKeepCurrent)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("PrepareAppTransition()"))) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoLock lock(mWindowMapLock);

    // if (DEBUG_APP_TRANSITIONS) Slog.v(
    //         TAG, "Prepare app transition: transit=" + transit
    //         + " mNextAppTransition=" + mNextAppTransition
    //         + " alwaysKeepCurrent=" + alwaysKeepCurrent
    //         + " Callers=" + Debug.getCallers(3));

    if (OkToDisplay()) {
        if (mNextAppTransition == IWindowManagerPolicy::TRANSIT_UNSET
                || mNextAppTransition == IWindowManagerPolicy::TRANSIT_NONE) {
            mNextAppTransition = transit;
        }
        else if (!alwaysKeepCurrent) {
            if (transit == IWindowManagerPolicy::TRANSIT_TASK_OPEN
                    && mNextAppTransition == IWindowManagerPolicy::TRANSIT_TASK_CLOSE) {
                // Opening a new task always supersedes a close for the anim.
                mNextAppTransition = transit;
            }
            else if (transit == IWindowManagerPolicy::TRANSIT_ACTIVITY_OPEN
                    && mNextAppTransition == IWindowManagerPolicy::TRANSIT_ACTIVITY_CLOSE) {
                // Opening a new activity always supersedes a close for the anim.
                mNextAppTransition = transit;
            }
        }

        mAppTransitionReady = FALSE;
        mAppTransitionTimeout = FALSE;
        mStartingIconInTransition = FALSE;
        mSkipAppTransitionAnimation = FALSE;

        mH->RemoveMessages(H::APP_TRANSITION_TIMEOUT);
        Boolean result;
        mH->SendEmptyMessageDelayed(H::APP_TRANSITION_TIMEOUT, 5000, &result);
    }

    return NOERROR;
}

ECode CWindowManagerService::GetPendingAppTransition(
    /* [out] */ Int32* transit)
{
    VALIDATE_NOT_NULL(transit);

    *transit = mNextAppTransition;
    return NOERROR;
}

void CWindowManagerService::ScheduleAnimationCallback(
    /* [in] */ IRemoteCallback* cb)
{
    if (cb != NULL) {
        AutoPtr<IMessage> msg;
        mH->ObtainMessage(H::DO_ANIMATION_CALLBACK, cb, (IMessage**)&msg);
        Boolean result;
        mH->SendMessage(msg, &result);
    }
}

ECode CWindowManagerService::OverridePendingAppTransition(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 enterAnim,
    /* [in] */ Int32 exitAnim,
    /* [in] */ IRemoteCallback* startedCallback)
{
    AutoLock lock(mWindowMapLock);

    if (mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET) {
        mNextAppTransitionType = IActivityOptions::ANIM_CUSTOM;
        mNextAppTransitionPackage = capsuleName;
        mNextAppTransitionThumbnail = NULL;
        mNextAppTransitionEnter = enterAnim;
        mNextAppTransitionExit = exitAnim;
        ScheduleAnimationCallback(mNextAppTransitionCallback);
        mNextAppTransitionCallback = startedCallback;
    }
    else {
        ScheduleAnimationCallback(startedCallback);
    }

    return NOERROR;
}

ECode CWindowManagerService::OverridePendingAppTransitionScaleUp(
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 startWidth,
    /* [in] */ Int32 startHeight)
{
    AutoLock lock(mWindowMapLock);

    if (mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET) {
        mNextAppTransitionType = IActivityOptions::ANIM_SCALE_UP;
        mNextAppTransitionPackage = NULL;
        mNextAppTransitionThumbnail = NULL;
        mNextAppTransitionStartX = startX;
        mNextAppTransitionStartY = startY;
        mNextAppTransitionStartWidth = startWidth;
        mNextAppTransitionStartHeight = startHeight;
        ScheduleAnimationCallback(mNextAppTransitionCallback);
        mNextAppTransitionCallback = NULL;
    }
    return NOERROR;
}

ECode CWindowManagerService::OverridePendingAppTransitionThumb(
    /* [in] */ IBitmap* srcThumb,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ IRemoteCallback* startedCallback,
    /* [in] */ Boolean scaleUp)
{
    AutoLock lock(mWindowMapLock);

    if (mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET) {
        mNextAppTransitionType = scaleUp
                ? IActivityOptions::ANIM_THUMBNAIL_SCALE_UP : IActivityOptions::ANIM_THUMBNAIL_SCALE_DOWN;
        mNextAppTransitionPackage = NULL;
        mNextAppTransitionThumbnail = srcThumb;
        mNextAppTransitionScaleUp = scaleUp;
        mNextAppTransitionStartX = startX;
        mNextAppTransitionStartY = startY;
        ScheduleAnimationCallback(mNextAppTransitionCallback);
        mNextAppTransitionCallback = startedCallback;
    }
    else {
        ScheduleAnimationCallback(startedCallback);
    }
    return NOERROR;
}

ECode CWindowManagerService::ExecuteAppTransition()
{
// Slogger::D(TAG, "==== File: %s, Line: %d ====", __FILE__, __LINE__);
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("ExecuteAppTransition()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoLock lock(mWindowMapLock);

    // if (DEBUG_APP_TRANSITIONS) {
    //     RuntimeException e = new RuntimeException("here");
    //     e.fillInStackTrace();
    //     Slog.w(TAG, "Execute app transition: mNextAppTransition="
    //             + mNextAppTransition, e);
    // }
    if (mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET) {
        mAppTransitionReady = TRUE;
        Int64 origId = Binder::ClearCallingIdentity();
        PerformLayoutAndPlaceSurfacesLocked();
        Binder::RestoreCallingIdentity(origId);
    }

    return NOERROR;
}

ECode CWindowManagerService::SetAppStartingWindow(
    /* [in] */ IBinder* token,
    /* [in] */ const String& pkg,
    /* [in] */ Int32 theme,
    /* [in] */ ICompatibilityInfo* compatInfo,
    /* [in] */ ICharSequence* nonLocalizedLabel,
    /* [in] */ Int32 labelRes,
    /* [in] */ Int32 icon,
    /* [in] */ Int32 windowFlags,
    /* [in] */ IBinder* transferFrom,
    /* [in] */ Boolean createIfNeeded)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("SetAppStartingIcon()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoLock lock(mWindowMapLock);

    if (DEBUG_STARTING_WINDOW) {
        Slogger::V(TAG, "setAppStartingWindow: token=%p pkg=%s transferFrom=%p",
            token, pkg.string(), transferFrom);
    }

    AutoPtr<AppWindowToken> wtoken = FindAppWindowToken(token);
    if (wtoken == NULL) {
        // Slog.w(TAG, "Attempted to set icon of non-existing app token: " + token);
        return NOERROR;
    }

    // If the display is frozen, we won't do anything until the
    // actual window is displayed so there is no reason to put in
    // the starting window.
    if (!OkToDisplay()) {
        return NOERROR;
    }

    if (wtoken->mStartingData != NULL) {
        return NOERROR;
    }

    if (transferFrom != NULL) {
        AutoPtr<AppWindowToken> ttoken = FindAppWindowToken(transferFrom);
        if (ttoken != NULL) {
            AutoPtr<WindowState> startingWindow = ttoken->mStartingWindow;
            if (startingWindow != NULL) {
                if (mStartingIconInTransition) {
                    // In this case, the starting icon has already
                    // been displayed, so start letting windows get
                    // shown immediately without any more transitions.
                    mSkipAppTransitionAnimation = TRUE;
                }
                if (DEBUG_STARTING_WINDOW) {
                    Slogger::V(TAG, "Moving existing starting %p from %p to %p",
                        startingWindow.Get(), ttoken.Get(), wtoken.Get());
                }
                Int64 origId = Binder::ClearCallingIdentity();

                // Transfer the starting window over to the new
                // token.
                wtoken->mStartingData = ttoken->mStartingData;
                wtoken->mStartingView = ttoken->mStartingView;
                wtoken->mStartingDisplayed = ttoken->mStartingDisplayed;
                wtoken->mStartingWindow = startingWindow;
                ttoken->mStartingData = NULL;
                ttoken->mStartingView = NULL;
                ttoken->mStartingWindow = NULL;
                ttoken->mStartingMoved = TRUE;
                startingWindow->mToken = wtoken;
                startingWindow->mRootToken = wtoken;
                startingWindow->mAppToken = wtoken;
                startingWindow->mWinAnimator->mAppAnimator = wtoken->mAppAnimator;

                if (DEBUG_WINDOW_MOVEMENT || DEBUG_ADD_REMOVE || DEBUG_STARTING_WINDOW) {
                    Slogger::V(TAG, "Removing starting window: %p", startingWindow.Get());
                }
                startingWindow->GetWindowList().Remove(startingWindow);
                mWindowsChanged = TRUE;
                if (DEBUG_ADD_REMOVE) {
                    Slogger::V(TAG, "Removing starting %p from %p", startingWindow.Get(), ttoken.Get());
                }
                ttoken->mWindows.Remove(startingWindow);
                ttoken->mAllAppWindows.Remove(startingWindow);
                AddWindowToListInOrderLocked(startingWindow, TRUE);

                // Propagate other interesting state between the
                // tokens.  If the old token is displayed, we should
                // immediately force the new one to be displayed.  If
                // it is animating, we need to move that animation to
                // the new one.
                if (ttoken->mAllDrawn) {
                    wtoken->mAllDrawn = TRUE;
                }
                if (ttoken->mFirstWindowDrawn) {
                    wtoken->mFirstWindowDrawn = TRUE;
                }
                if (!ttoken->mHidden) {
                    wtoken->mHidden = FALSE;
                    wtoken->mHiddenRequested = FALSE;
                    wtoken->mWillBeHidden = FALSE;
                }
                if (wtoken->mClientHidden != ttoken->mClientHidden) {
                    wtoken->mClientHidden = ttoken->mClientHidden;
                    wtoken->SendAppVisibilityToClients();
                }
                AutoPtr<AppWindowAnimator> tAppAnimator = ttoken->mAppAnimator;
                AutoPtr<AppWindowAnimator> wAppAnimator = wtoken->mAppAnimator;
                if (tAppAnimator->mAnimation != NULL) {
                    wAppAnimator->mAnimation = tAppAnimator->mAnimation;
                    wAppAnimator->mAnimating = tAppAnimator->mAnimating;
                    wAppAnimator->mAnimLayerAdjustment = tAppAnimator->mAnimLayerAdjustment;
                    tAppAnimator->mAnimation = NULL;
                    tAppAnimator->mAnimLayerAdjustment = 0;
                    wAppAnimator->UpdateLayers();
                    tAppAnimator->UpdateLayers();
                }

                UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES,
                        TRUE /*updateInputWindows*/);
                GetDefaultDisplayContentLocked()->mLayoutNeeded = TRUE;
                PerformLayoutAndPlaceSurfacesLocked();
                Binder::RestoreCallingIdentity(origId);
                return NOERROR;
            }
            else if (ttoken->mStartingData != NULL) {
                // The previous app was getting ready to show a
                // starting window, but hasn't yet done so.  Steal it!
                if (DEBUG_STARTING_WINDOW) {
                    Slogger::V(TAG, "Moving pending starting from %p to %p",
                        ttoken.Get(), wtoken.Get());
                }
                wtoken->mStartingData = ttoken->mStartingData;
                ttoken->mStartingData = NULL;
                ttoken->mStartingMoved = TRUE;

                AutoPtr<IMessage> msg;
                mH->ObtainMessage(H::ADD_STARTING, wtoken.Get(), (IMessage**)&msg);

                // Note: we really want to do sendMessageAtFrontOfQueue() because we
                // want to process the message ASAP, before any other queued
                // messages.
                Boolean result;
                return mH->SendMessageAtFrontOfQueue(msg, &result);
            }

            AutoPtr<AppWindowAnimator> tAppAnimator = ttoken->mAppAnimator;
            AutoPtr<AppWindowAnimator> wAppAnimator = wtoken->mAppAnimator;
            if (tAppAnimator->mThumbnail != NULL) {
                // The old token is animating with a thumbnail, transfer
                // that to the new token.
                if (wAppAnimator->mThumbnail != NULL) {
                    wAppAnimator->mThumbnail->Destroy();
                }
                wAppAnimator->mThumbnail = tAppAnimator->mThumbnail;
                wAppAnimator->mThumbnailX = tAppAnimator->mThumbnailX;
                wAppAnimator->mThumbnailY = tAppAnimator->mThumbnailY;
                wAppAnimator->mThumbnailLayer = tAppAnimator->mThumbnailLayer;
                wAppAnimator->mThumbnailAnimation = tAppAnimator->mThumbnailAnimation;
                tAppAnimator->mThumbnail = NULL;
            }
        }
    }

    // There is no existing starting window, and the caller doesn't
    // want us to create one, so that's it!
    if (!createIfNeeded) {
        return NOERROR;
    }

    // If this is a translucent or wallpaper window, then don't
    // show a starting window -- the current effect (a full-screen
    // opaque starting window that fades away to the real contents
    // when it is ready) does not work for this.
    if (DEBUG_STARTING_WINDOW) {
        Slogger::V(TAG, "Checking theme of starting window: 0x%d", theme);
    }
    if (theme != 0) {
        AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::Window),
            ARRAY_SIZE(R::styleable::Window));

        AutoPtr<AttributeCache::Entry> ent = AttributeCache::GetInstance()->Get(
            wtoken->mUserId, pkg, theme, attrIds);
        if (ent == NULL) {
            // Whoops!  App doesn't exist.  Um.  Okay.  We'll just
            // pretend like we didn't see that.
            return NOERROR;
        }
        // if (DEBUG_STARTING_WINDOW) Slog.v(TAG, "Translucent="
        //         + ent.array.getBoolean(
        //                 com.android.internal.R.styleable.Window_windowIsTranslucent, false)
        //         + " Floating="
        //         + ent.array.getBoolean(
        //                 com.android.internal.R.styleable.Window_windowIsFloating, false)
        //         + " ShowWallpaper="
        //         + ent.array.getBoolean(
        //                 com.android.internal.R.styleable.Window_windowShowWallpaper, false));
        Boolean res;
        ent->mArray->GetBoolean(R::styleable::Window_windowIsTranslucent,
                FALSE, &res);
        if (res) {
            return NOERROR;
        }
        ent->mArray->GetBoolean(R::styleable::Window_windowIsFloating,
                FALSE, &res);
        if (res) {
            return NOERROR;
        }
        ent->mArray->GetBoolean(R::styleable::Window_windowShowWallpaper,
                FALSE, &res);
        if (res) {
            if (mWallpaperTarget == NULL) {
                // If this theme is requesting a wallpaper, and the wallpaper
                // is not curently visible, then this effectively serves as
                // an opaque window and our starting window transition animation
                // can still work.  We just need to make sure the starting window
                // is also showing the wallpaper.
                windowFlags |= IWindowManagerLayoutParams::FLAG_SHOW_WALLPAPER;
            }
            else {
                return NOERROR;
            }
        }
    }

    if (DEBUG_STARTING_WINDOW) {
        Slogger::V(TAG, "Creating StartingData");
    }
    mStartingIconInTransition = TRUE;
    wtoken->mStartingData = new StartingData(
            pkg, theme, compatInfo, nonLocalizedLabel, labelRes, icon, windowFlags);

    AutoPtr<IMessage> msg;
    mH->ObtainMessage(H::ADD_STARTING, wtoken.Get(), (IMessage**)&msg);

    // Note: we really want to do sendMessageAtFrontOfQueue() because we
    // want to process the message ASAP, before any other queued
    // messages.
    Boolean result;
    return mH->SendMessageAtFrontOfQueue(msg, &result);
}

ECode CWindowManagerService::SetAppWillBeHidden(
    /* [in] */ IBinder* token)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("SetAppWillBeHidden()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoPtr<AppWindowToken> wtoken;

    AutoLock lock(mWindowMapLock);

    wtoken = FindAppWindowToken(token);
    if (wtoken != NULL) {
        wtoken->mWillBeHidden = true;
    }

    return NOERROR;
}

Boolean CWindowManagerService::SetTokenVisibilityLocked(
    /* [in] */ AppWindowToken* wtoken,
    /* [in] */ IWindowManagerLayoutParams* lp,
    /* [in] */ Boolean visible,
    /* [in] */ Int32 transit,
    /* [in] */ Boolean performLayout)
{
    Boolean delayed = FALSE;

    if (wtoken->mClientHidden == visible) {
        wtoken->mClientHidden = !visible;
        wtoken->SendAppVisibilityToClients();
    }

    wtoken->mWillBeHidden = FALSE;
    if (wtoken->mHidden == visible) {
        Boolean changed = FALSE;
        // if (DEBUG_APP_TRANSITIONS) Slog.v(
        //         TAG, "Changing app " + wtoken + " hidden=" + wtoken.hidden
        //         + " performLayout=" + performLayout);

        Boolean runningAppAnimation = FALSE;

        if (transit != IWindowManagerPolicy::TRANSIT_UNSET) {
            if (wtoken->mAppAnimator->mAnimation == AppWindowAnimator::sDummyAnimation) {
                wtoken->mAppAnimator->mAnimation = NULL;
            }
            if (ApplyAnimationLocked(wtoken, lp, transit, visible)) {
                delayed = runningAppAnimation = TRUE;
            }
            AutoPtr<WindowState> window = wtoken->FindMainWindow();
            if (window != NULL) {
                ScheduleNotifyWindowTranstionIfNeededLocked(window, transit);
            }
            changed = TRUE;
        }

        List< AutoPtr<WindowState> >::Iterator it = wtoken->mAllAppWindows.Begin();
        for (; it != wtoken->mAllAppWindows.End(); ++it) {
            AutoPtr<WindowState> win = *it;
            if (win == wtoken->mStartingWindow) {
                continue;
            }

            // Slog.i(TAG, "Window " + win + ": vis=" + win.isVisible());
            // win.dump("  ");
            if (visible) {
                if (!win->IsVisibleNow()) {
                    if (!runningAppAnimation) {
                        win->mWinAnimator->ApplyAnimationLocked(
                                IWindowManagerPolicy::TRANSIT_ENTER, TRUE);
                        ScheduleNotifyWindowTranstionIfNeededLocked(win,
                                IWindowManagerPolicy::TRANSIT_ENTER);
                    }
                    changed = TRUE;
                    win->mDisplayContent->mLayoutNeeded = TRUE;
                }
            }
            else if (win->IsVisibleNow()) {
                if (!runningAppAnimation) {
                    win->mWinAnimator->ApplyAnimationLocked(
                            IWindowManagerPolicy::TRANSIT_EXIT, FALSE);
                    ScheduleNotifyWindowTranstionIfNeededLocked(win,
                            IWindowManagerPolicy::TRANSIT_EXIT);
                }
                changed = TRUE;
                win->mDisplayContent->mLayoutNeeded = TRUE;
            }
        }

        wtoken->mHidden = wtoken->mHiddenRequested = !visible;
        if (!visible) {
            UnsetAppFreezingScreenLocked(wtoken, TRUE, TRUE);
        }
        else {
            // If we are being set visible, and the starting window is
            // not yet displayed, then make sure it doesn't get displayed.
            AutoPtr<WindowState> swin = wtoken->mStartingWindow;
            if (swin != NULL && !swin->IsDrawnLw()) {
                swin->mPolicyVisibility = FALSE;
                swin->mPolicyVisibilityAfterAnim = FALSE;
             }
        }

        // if (DEBUG_APP_TRANSITIONS) Slog.v(TAG, "setTokenVisibilityLocked: " + wtoken
        //         + ": hidden=" + wtoken.hidden + " hiddenRequested="
        //         + wtoken.hiddenRequested);

        if (changed) {
            mInputMonitor->SetUpdateInputWindowsNeededLw();
            if (performLayout) {
                UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES,
                        FALSE /*updateInputWindows*/);
                PerformLayoutAndPlaceSurfacesLocked();
            }
            mInputMonitor->UpdateInputWindowsLw(FALSE /*force*/);
        }
    }

    if (wtoken->mAppAnimator->mAnimation != NULL) {
        delayed = TRUE;
    }

    List< AutoPtr<WindowState> >::ReverseIterator rit = wtoken->mAllAppWindows.RBegin();
    for (; rit != wtoken->mAllAppWindows.REnd(); ++rit) {
        if ((*rit)->mWinAnimator->IsWindowAnimating()) {
            delayed = TRUE;
        }
    }

    return delayed;
}

ECode CWindowManagerService::SetAppVisibility(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean visible)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("SetAppVisibility()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoPtr<AppWindowToken> wtoken;

    AutoLock lock(mWindowMapLock);

    wtoken = FindAppWindowToken(token);
    if (wtoken == NULL) {
        Slogger::W(TAG, "Attempted to set visibility of non-existing app token: %p", token);
        return NOERROR;
    }

    // if (DEBUG_APP_TRANSITIONS || DEBUG_ORIENTATION) {
    //     RuntimeException e = null;
    //     if (!HIDE_STACK_CRAWLS) {
    //         e = new RuntimeException();
    //         e.fillInStackTrace();
    //     }
    //     Slog.v(TAG, "setAppVisibility(" + token + ", visible=" + visible
    //             + "): mNextAppTransition=" + mNextAppTransition
    //             + " hidden=" + wtoken.hidden
    //             + " hiddenRequested=" + wtoken.hiddenRequested, e);
    // }

    // If we are preparing an app transition, then delay changing
    // the visibility of this token until we execute that transition.
    if (OkToDisplay()
            && mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET) {
        // Already in requested state, don't do anything more.
        if (wtoken->mHiddenRequested != visible) {
            return NOERROR;
        }
        wtoken->mHiddenRequested = !visible;

        if (!wtoken->mStartingDisplayed) {
            // if (DEBUG_APP_TRANSITIONS) Slog.v(
            //         TAG, "Setting dummy animation on: " + wtoken);
            wtoken->mAppAnimator->SetDummyAnimation();
        }
        mOpeningApps.Remove(wtoken);
        mClosingApps.Remove(wtoken);
        wtoken->mWaitingToShow = wtoken->mWaitingToHide = FALSE;
        wtoken->mInPendingTransaction = TRUE;
        if (visible) {
            mOpeningApps.PushBack(wtoken);
            wtoken->mStartingDisplayed = FALSE;

            // If the token is currently hidden (should be the
            // common case), then we need to set up to wait for
            // its windows to be ready.
            if (wtoken->mHidden) {
                wtoken->mAllDrawn = FALSE;
                wtoken->mWaitingToShow = TRUE;

                if (wtoken->mClientHidden) {
                    // In the case where we are making an app visible
                    // but holding off for a transition, we still need
                    // to tell the client to make its windows visible so
                    // they get drawn.  Otherwise, we will wait on
                    // performing the transition until all windows have
                    // been drawn, they never will be, and we are sad.
                    wtoken->mClientHidden = FALSE;
                    wtoken->SendAppVisibilityToClients();
                }
            }
        }
        else {
            mClosingApps.PushBack(wtoken);

            // If the token is currently visible (should be the
            // common case), then set up to wait for it to be hidden.
            if (!wtoken->mHidden) {
                wtoken->mWaitingToHide = TRUE;
            }
        }
        return NOERROR;
    }

    Int64 origId = Binder::ClearCallingIdentity();
    SetTokenVisibilityLocked(wtoken, NULL, visible,
            IWindowManagerPolicy::TRANSIT_UNSET, TRUE);
    wtoken->UpdateReportedVisibilityLocked();
    Binder::RestoreCallingIdentity(origId);

    return NOERROR;
}

void CWindowManagerService::UnsetAppFreezingScreenLocked(
    /* [in] */ AppWindowToken* wtoken,
    /* [in] */ Boolean unfreezeSurfaceNow,
    /* [in] */ Boolean force)
{
    if (wtoken->mAppAnimator->mFreezingScreen) {
        // if (DEBUG_ORIENTATION) Slog.v(TAG, "Clear freezing of " + wtoken
        //         + " force=" + force);
        Boolean unfrozeWindows = FALSE;
        List< AutoPtr<WindowState> >::Iterator it = wtoken->mAllAppWindows.Begin();
        for (; it != wtoken->mAllAppWindows.End(); ++it) {
            AutoPtr<WindowState> w = *it;
            if (w->mAppFreezing) {
                w->mAppFreezing = FALSE;
                if (w->mHasSurface && !w->mOrientationChanging) {
                    // if (DEBUG_ORIENTATION) Slog.v(TAG, "set mOrientationChanging of " + w);
                    w->mOrientationChanging = TRUE;
                    mInnerFields->mOrientationChangeComplete = FALSE;
                }
                unfrozeWindows = TRUE;
                w->mDisplayContent->mLayoutNeeded = TRUE;
            }
        }
        if (force || unfrozeWindows) {
            // if (DEBUG_ORIENTATION) Slog.v(TAG, "No longer freezing: " + wtoken);
            wtoken->mAppAnimator->mFreezingScreen = FALSE;
            mAppsFreezingScreen--;
        }
        if (unfreezeSurfaceNow) {
            if (unfrozeWindows) {
                PerformLayoutAndPlaceSurfacesLocked();
            }
            StopFreezingDisplayLocked();
        }
    }
}

void CWindowManagerService::StartAppFreezingScreenLocked(
    /* [in] */ AppWindowToken* wtoken,
    /* [in] */ Int32 configChanges)
{
    // if (DEBUG_ORIENTATION) {
    //     RuntimeException e = null;
    //     if (!HIDE_STACK_CRAWLS) {
    //         e = new RuntimeException();
    //         e.fillInStackTrace();
    //     }
    //     Slog.i(TAG, "Set freezing of " + wtoken.appToken
    //             + ": hidden=" + wtoken.hidden + " freezing="
    //             + wtoken.mAppAnimator.freezingScreen, e);
    // }
    if (!wtoken->mHiddenRequested) {
        if (!wtoken->mAppAnimator->mFreezingScreen) {
            wtoken->mAppAnimator->mFreezingScreen = TRUE;
            mAppsFreezingScreen++;
            if (mAppsFreezingScreen == 1) {
                StartFreezingDisplayLocked(FALSE, 0, 0);

                mH->RemoveMessages(H::APP_FREEZE_TIMEOUT);
                Boolean result;
                mH->SendEmptyMessageDelayed(H::APP_FREEZE_TIMEOUT, 5000, &result);
            }
        }

        List< AutoPtr<WindowState> >::Iterator it = wtoken->mAllAppWindows.Begin();
        for (; it != wtoken->mAllAppWindows.End(); ++it) {
            AutoPtr<WindowState> w = *it;
            w->mAppFreezing = TRUE;
        }
    }
}

ECode CWindowManagerService::StartAppFreezingScreen(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 configChanges)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("StartAppFreezingScreen()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoLock lock(mWindowMapLock);

    if (configChanges == 0 && OkToDisplay()) {
        // if (DEBUG_ORIENTATION) Slog.v(TAG, "Skipping set freeze of " + token);
        return NOERROR;
    }

    AutoPtr<AppWindowToken> wtoken = FindAppWindowToken(token);
    if (wtoken == NULL || wtoken->mAppToken == NULL) {
        Slogger::W(TAG, "Attempted to freeze screen with non-existing app token: %p"
                , wtoken.Get());
        return NOERROR;
    }
    Int64 origId = Binder::ClearCallingIdentity();
    StartAppFreezingScreenLocked(wtoken, configChanges);
    Binder::RestoreCallingIdentity(origId);

    return NOERROR;
}

ECode CWindowManagerService::StopAppFreezingScreen(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean force)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("StopAppFreezingScreen()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoLock lock(mWindowMapLock);

    AutoPtr<AppWindowToken> wtoken = FindAppWindowToken(token);
    if (wtoken == NULL || wtoken->mAppToken == NULL) {
        return NOERROR;
    }
    Int64 origId = Binder::ClearCallingIdentity();
    // if (DEBUG_ORIENTATION) Slog.v(TAG, "Clear freezing of " + token
    //         + ": hidden=" + wtoken.hidden + " freezing=" + wtoken.mAppAnimator.freezingScreen);
    UnsetAppFreezingScreenLocked(wtoken, TRUE, force);
    Binder::RestoreCallingIdentity(origId);

    return NOERROR;
}

ECode CWindowManagerService::RemoveAppToken(
    /* [in] */ IBinder* token)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("RemoveAppToken()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoPtr<AppWindowToken> wtoken;
    AutoPtr<AppWindowToken> startingToken;
    Boolean delayed = FALSE;

    Int64 origId = Binder::ClearCallingIdentity();
    {
        AutoLock lock(mWindowMapLock);

        AutoPtr<WindowToken> basewtoken;
        HashMap<AutoPtr<IBinder>, AutoPtr<WindowToken> >::Iterator tokenIt
                = mTokenMap.Find(token);
        if (tokenIt != mTokenMap.End()) {
            basewtoken = tokenIt->mSecond;
            mTokenMap.Erase(tokenIt);
        }
        if (basewtoken != NULL && (wtoken = basewtoken->mAppWindowToken) != NULL) {
            if (DEBUG_APP_TRANSITIONS) Slogger::V(TAG, "Removing app token: %p", wtoken.Get());
            delayed = SetTokenVisibilityLocked(wtoken, NULL, FALSE,
                    IWindowManagerPolicy::TRANSIT_UNSET, TRUE);
            wtoken->mInPendingTransaction = FALSE;
            mOpeningApps.Remove(wtoken);
            wtoken->mWaitingToShow = FALSE;
            List< AutoPtr<AppWindowToken> >::Iterator it = mClosingApps.Begin();
            for (; it != mClosingApps.End(); ++it) {
                if (*it == wtoken) {
                    break;
                }
            }
            if (it != mClosingApps.End()) {
                delayed = TRUE;
            }
            else if (mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET) {
                mClosingApps.PushBack(wtoken);
                wtoken->mWaitingToHide = TRUE;
                delayed = TRUE;
            }
            // if (DEBUG_APP_TRANSITIONS) Slog.v(
            //         TAG, "Removing app " + wtoken + " delayed=" + delayed
            //         + " animation=" + wtoken.mAppAnimator.animation
            //         + " animating=" + wtoken.mAppAnimator.animating);
            if (delayed) {
                // set the token aside because it has an active animation to be finished
                if (DEBUG_ADD_REMOVE || DEBUG_TOKEN_MOVEMENT) {
                    Slogger::V(TAG, "removeAppToken make exiting: %p", wtoken.Get());
                }
                mExitingAppTokens.PushBack(wtoken);
            }
            else {
                // Make sure there is no animation running on this token,
                // so any windows associated with it will be removed as
                // soon as their animations are complete
                wtoken->mAppAnimator->ClearAnimation();
                wtoken->mAppAnimator->mAnimating = FALSE;
            }
            if (DEBUG_ADD_REMOVE || DEBUG_TOKEN_MOVEMENT) {
                Slogger::V(TAG, "removeAppToken: %p", wtoken.Get());
            }
            mAppTokens.Remove(wtoken);
            mAnimatingAppTokens.Remove(wtoken);
            wtoken->mRemoved = TRUE;
            if (wtoken->mStartingData != NULL) {
                startingToken = wtoken;
            }
            UnsetAppFreezingScreenLocked(wtoken, TRUE, TRUE);
            if (mFocusedApp == wtoken) {
                // if (DEBUG_FOCUS) Slog.v(TAG, "Removing focused app token:" + wtoken);
                mFocusedApp = NULL;
                UpdateFocusedWindowLocked(UPDATE_FOCUS_NORMAL, TRUE /*updateInputWindows*/);
                mInputMonitor->SetFocusedAppLw(NULL);
            }
        }
        else {
            Slogger::W(TAG, "Attempted to remove non-existing app token: %p", token);
        }

        if (!delayed && wtoken != NULL) {
            wtoken->UpdateReportedVisibilityLocked();
        }
    }
    Binder::RestoreCallingIdentity(origId);

    if (startingToken != NULL) {
        if (DEBUG_STARTING_WINDOW) {
            Slogger::V(TAG, "Schedule remove starting %p: app token removed", startingToken.Get());
        }

        AutoPtr<IMessage> msg;
        mH->ObtainMessage(H::REMOVE_STARTING, startingToken.Get(), (IMessage**)&msg);
        Boolean result;
        return mH->SendMessageDelayed(msg, 2000, &result);
    }

    return NOERROR;
}

Boolean CWindowManagerService::TmpRemoveAppWindowsLocked(
    /* [in] */ WindowToken* token)
{
    if (token->mWindows.Begin() == token->mWindows.End()) {
        return FALSE;
    }

    mWindowsChanged = TRUE;
    List< AutoPtr<WindowState> >::Iterator it = token->mWindows.Begin();
    for (; it != token->mWindows.End(); ++it) {
        AutoPtr<WindowState> win = *it;
        if (DEBUG_WINDOW_MOVEMENT) Slogger::V(TAG, "Tmp removing app window %p", win.Get());
        win->GetWindowList().Remove(win);
        List< AutoPtr<WindowState> >::ReverseIterator crit = win->mChildWindows.RBegin();
        for (; crit != win->mChildWindows.REnd(); ++crit) {
            AutoPtr<WindowState> cwin = *crit;
            if (DEBUG_WINDOW_MOVEMENT) Slogger::V(TAG, "Tmp removing child window %p", cwin.Get());
            cwin->GetWindowList().Remove(cwin);
        }
    }
    return TRUE;
}

void CWindowManagerService::DumpAppTokensLocked()
{
    List< AutoPtr<AppWindowToken> >::ReverseIterator rit;
    Int32 i = 0;
    for (rit = mAppTokens.RBegin(); rit != mAppTokens.REnd(); ++rit, ++i) {
        Slogger::V(TAG, "  AppWindowToken #%d : %p", i, (*rit)->mToken.Get());
        PFL_EX("  AppWindowToken #%d : %p", i, (*rit)->mToken.Get())
    }
}

void CWindowManagerService::DumpAnimatingAppTokensLocked()
{
    List< AutoPtr<AppWindowToken> >::ReverseIterator rit;
    Int32 i = 0;
    for (rit = mAnimatingAppTokens.RBegin(); rit != mAnimatingAppTokens.REnd(); ++rit, ++i) {
        Slogger::V(TAG, "  AnimatingAppWindowToken #%d: %p", i, (*rit)->mToken.Get());
        PFL_EX("  AnimatingAppWindowToken #%d: %p", i, (*rit)->mToken.Get())
    }
}

void CWindowManagerService::DumpWindowsLocked()
{
    HashMap<Int32, AutoPtr<DisplayContent> >::Iterator it;
    for (it = mDisplayContents.Begin(); it != mDisplayContents.End(); ++it) {
        AutoPtr<DisplayContent> content = it->mSecond;
        if (content != NULL) {
            List< AutoPtr<WindowState> >& windows = content->GetWindowList();
            if (windows.Begin() != windows.End()) {
                List< AutoPtr<WindowState> >::ReverseIterator rit;
                Int32 i = 0;
                for (rit = windows.RBegin(); rit != windows.REnd(); ++rit, ++i) {
                    AutoPtr<WindowState> w = *rit;
                    Slogger::V(TAG, "  WindowState #%d : %p, AnimLayer= %d", i, w.Get(), w->mWinAnimator->mAnimLayer);
                    PFL_EX("  WindowState #%d : %p, AnimLayer= %d", i, w.Get(), w->mWinAnimator->mAnimLayer)
                }
            }
        }
    }
}

List< AutoPtr<WindowState> >::Iterator CWindowManagerService::FindWindowOffsetLocked(
    /* [in] */ List< AutoPtr<WindowState> >& windows,
    /* [in] */ List< AutoPtr<AppWindowToken> >::Iterator tokenPosIt)
{
    if (tokenPosIt == mAppTokens.End()) {
        List< AutoPtr<WindowState> >::ReverseIterator wsrit = windows.RBegin();
        while (wsrit != windows.REnd()) {
            AutoPtr<WindowState> win = *wsrit;
            AutoPtr<IApplicationToken> t;
            win->GetAppToken((IApplicationToken**)&t);
            if (t != NULL) {
                return wsrit.GetBase();
            }
            ++wsrit;
        }
    }

    List< AutoPtr<AppWindowToken> >::ReverseIterator tokenPosRit(tokenPosIt);
    while (tokenPosRit != mAppTokens.REnd()) {
        // Find the first app token below the new position that has
        // a window displayed.
        AutoPtr<AppWindowToken> wtoken = *tokenPosRit;
        // if (DEBUG_REORDER) Slog.v(TAG, "Looking for lower windows @ "
        //         + tokenPos + " -- " + wtoken.token);
        if (wtoken->mSendingToBottom) {
            // if (DEBUG_REORDER) Slog.v(TAG,
            //         "Skipping token -- currently sending to bottom");
            tokenPosRit++;
            continue;
        }
        List< AutoPtr<WindowState> >::ReverseIterator wsRit = wtoken->mWindows.RBegin();
        while (wsRit != wtoken->mWindows.REnd()) {
            AutoPtr<WindowState> win = *wsRit;
            List< AutoPtr<WindowState> >::ReverseIterator cwsRit = win->mChildWindows.RBegin();
            while (cwsRit != win->mChildWindows.REnd()) {
                AutoPtr<WindowState> cwin = *cwsRit;
                if (cwin->mSubLayer >= 0) {
                    List< AutoPtr<WindowState> >::ReverseIterator wsRit2 = windows.RBegin();
                    for (; wsRit2 != windows.REnd(); wsRit2++) {
                        if (*wsRit2 == cwin) {
                            // if (DEBUG_REORDER) Slog.v(TAG,
                            //         "Found child win @" + (pos+1));
                            return wsRit2.GetBase();
                        }
                    }
                }
                cwsRit++;
            }
            List< AutoPtr<WindowState> >::ReverseIterator wsRit2 = windows.RBegin();
            for (;wsRit2 != windows.REnd(); wsRit2++) {
                if (*wsRit2 == win) {
                    // if (DEBUG_REORDER) Slog.v(TAG, "Found win @" + (pos+1));
                    return wsRit2.GetBase();
                }
            }
            wsRit++;
        }
        tokenPosRit++;
    }

    return windows.Begin();
}

List< AutoPtr<WindowState> >::Iterator CWindowManagerService::ReAddWindowLocked(
    /* [in] */ List< AutoPtr<WindowState> >::Iterator indexIt,
    /* [in] */ WindowState* win)
{
    List< AutoPtr<WindowState> >& windows = win->GetWindowList();
    Boolean added = FALSE;
    List< AutoPtr<WindowState> >::Iterator cwIt = win->mChildWindows.Begin();
    for (; cwIt != win->mChildWindows.End(); cwIt++) {
        AutoPtr<WindowState> cwin = *cwIt;
        if (!added && cwin->mSubLayer >= 0) {
            // if (DEBUG_WINDOW_MOVEMENT) Slog.v(TAG, "Re-adding child window at "
            //         + index + ": " + cwin);
            win->mRebuilding = FALSE;
            windows.Insert(indexIt, win);
            added = TRUE;
        }
        // if (DEBUG_WINDOW_MOVEMENT) Slog.v(TAG, "Re-adding window at "
        //         + index + ": " + cwin);
        cwin->mRebuilding = FALSE;
        windows.Insert(indexIt, cwin);
    }
    if (!added) {
        // if (DEBUG_WINDOW_MOVEMENT) Slog.v(TAG, "Re-adding window at "
        //         + index + ": " + win);
        win->mRebuilding = FALSE;
        windows.Insert(indexIt, win);
    }
    mWindowsChanged = TRUE;
    return indexIt;
}

List< AutoPtr<WindowState> >::Iterator CWindowManagerService::ReAddAppWindowsLocked(
    /* [in] */ DisplayContent* displayContent,
    /* [in] */ List< AutoPtr<WindowState> >::Iterator indexIt,
    /* [in] */ WindowToken* token)
{
    List< AutoPtr<WindowState> >::Iterator it = token->mWindows.Begin();
    for (; it != token->mWindows.End(); ++it) {
        AutoPtr<WindowState> win = *it;
        if (win->mDisplayContent.Get() == displayContent) {
            indexIt = ReAddWindowLocked(indexIt, *it);
        }
    }
    return indexIt;
}

ECode CWindowManagerService::MoveAppToken(
    /* [in] */ Int32 index,
    /* [in] */ IBinder* token)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("MoveAppToken()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoLock lock(mWindowMapLock);

    // if (DEBUG_REORDER) Slog.v(TAG, "Initial app tokens:");
    // if (DEBUG_REORDER) dumpAppTokensLocked();
    AutoPtr<AppWindowToken> wtoken = FindAppWindowToken(token);
    Int32 oldIndex = 0;
    List< AutoPtr<AppWindowToken> >::Iterator it;
    for (it = mAppTokens.Begin(); it != mAppTokens.End(); ++it) {
        if (*it == wtoken) break;
        oldIndex++;
    }
    // if (DEBUG_TOKEN_MOVEMENT || DEBUG_REORDER) Slog.v(TAG,
    //         "Start moving token " + wtoken + " initially at "
    //         + oldIndex);
    if (oldIndex > index && mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET
                && !mAppTransitionRunning) {
        // animation towards back has not started, copy old list for duration of animation.
        mAnimatingAppTokens.Clear();
        mAnimatingAppTokens.Assign(mAppTokens.Begin(), mAppTokens.End());
    }
    if (wtoken == NULL || it == mAppTokens.End()) {
        Slogger::W(TAG, "Attempting to reorder token that doesn't exist: %p (%p)",
                token, wtoken.Get());
        return NOERROR;
    }

    mAppTokens.Remove(wtoken);
    List< AutoPtr<AppWindowToken> >::Iterator insertIt = mAppTokens.Insert(index, wtoken);
    // if (DEBUG_REORDER) Slog.v(TAG, "Moved " + token + " to " + index + ":");
    // else if (DEBUG_TOKEN_MOVEMENT) Slog.v(TAG, "Moved " + token + " to " + index);
    if (DEBUG_REORDER) DumpAppTokensLocked();
    if (mNextAppTransition == IWindowManagerPolicy::TRANSIT_UNSET && !mAppTransitionRunning) {
        // Not animating, bring animating app list in line with mAppTokens.
        mAnimatingAppTokens.Clear();
        mAnimatingAppTokens.Assign(mAppTokens.Begin(), mAppTokens.End());

        // Bring window ordering, window focus and input window in line with new app token
        Int64 origId = Binder::ClearCallingIdentity();
        if (DEBUG_REORDER) Slogger::V(TAG, "Removing windows in %p:", token);
        if (DEBUG_REORDER) DumpWindowsLocked();
        if (TmpRemoveAppWindowsLocked(wtoken)) {
            if (DEBUG_REORDER) Slogger::V(TAG, "Adding windows back in:");
            if (DEBUG_REORDER) DumpWindowsLocked();
            HashMap<Int32, AutoPtr<DisplayContent> >::Iterator dcit;
            for (dcit = mDisplayContents.Begin(); dcit != mDisplayContents.End(); ++dcit) {
                AutoPtr<DisplayContent> displayContent = dcit->mSecond;
                List< AutoPtr<WindowState> >& windows = displayContent->GetWindowList();
                List< AutoPtr<WindowState> >::Iterator posIt
                        = FindWindowOffsetLocked(windows, insertIt);
                List< AutoPtr<WindowState> >::Iterator newPosIt
                        = ReAddAppWindowsLocked(displayContent, posIt, wtoken);
                if (posIt != newPosIt) {
                    displayContent->mLayoutNeeded = TRUE;
                }
            }
            if (DEBUG_REORDER) Slogger::V(TAG, "Final window list:");
            if (DEBUG_REORDER) DumpWindowsLocked();
            UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES,
                    FALSE /*updateInputWindows*/);
            mInputMonitor->SetUpdateInputWindowsNeededLw();
            PerformLayoutAndPlaceSurfacesLocked();
            mInputMonitor->UpdateInputWindowsLw(FALSE /*force*/);
        }
        Binder::RestoreCallingIdentity(origId);
    }

    return NOERROR;
}

void CWindowManagerService::RemoveAppTokensLocked(
    /* [in] */ IObjectContainer* tokens)
{
    // XXX This should be done more efficiently!
    // (take advantage of the fact that both lists should be
    // ordered in the same way.)
    AutoPtr<IObjectEnumerator> objEmu;
    tokens->GetObjectEnumerator((IObjectEnumerator**)&objEmu);
    if (objEmu.Get() == NULL) {
        return;
    }

    Boolean isSucceeded;
    while (objEmu->MoveNext(&isSucceeded), isSucceeded) {
        AutoPtr<IInterface> tokenObj;
        objEmu->Current((IInterface**)&tokenObj);
        AutoPtr<IBinder> token = IBinder::Probe(tokenObj);

        if (token) {
            AutoPtr<AppWindowToken> wtoken = FindAppWindowToken(token);
            if (DEBUG_REORDER || DEBUG_TOKEN_MOVEMENT)
                Slogger::V(TAG, "Temporarily removing %p ", wtoken.Get());

            mAppTokens.Remove(wtoken);
            // if (!mAppTokens.remove(wtoken)) {
            //     Slog.w(TAG, "Attempting to reorder token that doesn't exist: "
            //           + token + " (" + wtoken + ")");
            //     i--;
            //     N--;
            // }
        }
    }
}

void CWindowManagerService::MoveAppWindowsLocked(
    /* [in] */ AppWindowToken* wtoken,
    /* [in] */ List< AutoPtr<AppWindowToken> >::Iterator tokenPosIt,
    /* [in] */ Boolean updateFocusAndLayout)
{
    // First remove all of the windows from the list.
    TmpRemoveAppWindowsLocked(wtoken);

    // Where to start adding?
    HashMap<Int32, AutoPtr<DisplayContent> >::Iterator it;
    for (it = mDisplayContents.Begin(); it != mDisplayContents.End(); ++it) {
        AutoPtr<DisplayContent> displayContent = it->mSecond;
        List< AutoPtr<WindowState> >& windows = displayContent->GetWindowList();
        List< AutoPtr<WindowState> >::Iterator posIt
                = FindWindowOffsetLocked(windows, tokenPosIt);
        List< AutoPtr<WindowState> >::Iterator newPosIt
                = ReAddAppWindowsLocked(displayContent, posIt, wtoken);
        if (posIt != newPosIt) {
            displayContent->mLayoutNeeded = TRUE;
        }

        if (updateFocusAndLayout &&
                !UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES,
                FALSE /*updateInputWindows*/)) {
            AssignLayersLocked(windows);
        }
    }

    if (updateFocusAndLayout) {
        mInputMonitor->SetUpdateInputWindowsNeededLw();

        // Note that the above updateFocusedWindowLocked conditional used to sit here.

        if (!mInLayout) {
            PerformLayoutAndPlaceSurfacesLocked();
        }
        mInputMonitor->UpdateInputWindowsLw(FALSE /*force*/);
    }
}

void CWindowManagerService::MoveAppWindowsLocked(
    /* [in] */ IObjectContainer* tokens,
    /* [in] */ List< AutoPtr<AppWindowToken> >::Iterator tokenPosIt)
{
    // First remove all of the windows from the list.
    AutoPtr<IObjectEnumerator> objEmu;
    ASSERT_SUCCEEDED(tokens->GetObjectEnumerator((IObjectEnumerator**)&objEmu));

    Boolean isSucceeded;
    while (objEmu->MoveNext(&isSucceeded), isSucceeded) {
        AutoPtr<IInterface> tokenObj;
        objEmu->Current((IInterface**)&tokenObj);
        AutoPtr<IBinder> tokenBinder = IBinder::Probe(tokenObj);

        AutoPtr<WindowToken> token;
        HashMap<AutoPtr<IBinder>, AutoPtr<WindowToken> >::Iterator it
                = mTokenMap.Find(tokenBinder);
        if (it != mTokenMap.End()) {
            token = it->mSecond;
        }
        if (token != NULL) {
            TmpRemoveAppWindowsLocked(token);
        }
    }

    // And now add them back at the correct place.
    //Int32 dcIdx = 0, wsIdx = 0;
    HashMap<Int32, AutoPtr<DisplayContent> >::Iterator dcit;
    for (dcit = mDisplayContents.Begin(); dcit != mDisplayContents.End(); ++dcit) {
        AutoPtr<DisplayContent> displayContent = dcit->mSecond;
        List< AutoPtr<WindowState> >& windows = displayContent->GetWindowList();
        // LogWindowList(windows, String("BEFORE MoveAppWindowsLocked"));
        // Where to start adding?
        List< AutoPtr<WindowState> >::Iterator posIt
                = FindWindowOffsetLocked(windows, tokenPosIt);

        objEmu->Reset();
        while (objEmu->MoveNext(&isSucceeded), isSucceeded) {
            AutoPtr<IInterface> tokenObj;
            objEmu->Current((IInterface**)&tokenObj);
            AutoPtr<IBinder> tokenBinder = IBinder::Probe(tokenObj);

            AutoPtr<WindowToken> token;
            HashMap<AutoPtr<IBinder>, AutoPtr<WindowToken> >::Iterator it
                    = mTokenMap.Find(tokenBinder);
            if (it != mTokenMap.End()) {
                token = it->mSecond;
            }
            if (token != NULL) {
                List< AutoPtr<WindowState> >::Iterator newPosIt
                        = ReAddAppWindowsLocked(displayContent, posIt, token);
                if (newPosIt != posIt) {
                    displayContent->mLayoutNeeded = TRUE;
                }
                posIt = newPosIt;
            }
        }
        if (!UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES,
                FALSE /*updateInputWindows*/)) {
            AssignLayersLocked(windows);
        }

        windows = displayContent->GetWindowList();
        // LogWindowList(windows, String("AFTER MoveAppWindowsLocked"));
    }

    mInputMonitor->SetUpdateInputWindowsNeededLw();

    // Note that the above updateFocusedWindowLocked used to sit here.
    PerformLayoutAndPlaceSurfacesLocked();
    mInputMonitor->UpdateInputWindowsLw(FALSE /*force*/);

    //dump();
}

ECode CWindowManagerService::MoveAppTokensToTop(
    /* [in] */ IObjectContainer* tokens)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("MoveAppTokensToTop()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Int64 origId = Binder::ClearCallingIdentity();

    AutoLock lock(mWindowMapLock);

    RemoveAppTokensLocked(tokens);
    AutoPtr<IObjectEnumerator> objEmu;
    ASSERT_SUCCEEDED(tokens->GetObjectEnumerator((IObjectEnumerator**)&objEmu));

    Boolean isSucceeded;
    while (objEmu->MoveNext(&isSucceeded), isSucceeded) {
        AutoPtr<IInterface> tokenObj;
        objEmu->Current((IInterface**)&tokenObj);
        AutoPtr<IBinder> token = IBinder::Probe(tokenObj);

        AutoPtr<AppWindowToken> wt = FindAppWindowToken(token);
        if (wt != NULL) {
            if (DEBUG_TOKEN_MOVEMENT || DEBUG_REORDER)
                Slogger::V(TAG, "Adding next to top: %p", wt.Get());
            mAppTokens.PushBack(wt);
            if (mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET) {
                wt->mSendingToBottom = FALSE;
            }
        }
    }

    if (!mAppTransitionRunning) {
        mAnimatingAppTokens.Clear();
        mAnimatingAppTokens.Assign(mAppTokens.Begin(), mAppTokens.End());
        MoveAppWindowsLocked(tokens, mAppTokens.End());
    }
    Binder::RestoreCallingIdentity(origId);

    return NOERROR;
}

ECode CWindowManagerService::MoveAppTokensToBottom(
    /* [in] */ IObjectContainer* tokens)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("MoveAppTokensToBottom()"))) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    Int64 origId = Binder::ClearCallingIdentity();
    {
        AutoLock lock(mWindowMapLock);

        AutoPtr<IObjectEnumerator> objEmu;
        ASSERT_SUCCEEDED(tokens->GetObjectEnumerator((IObjectEnumerator**)&objEmu));
        Boolean isSucceeded;
        objEmu->MoveNext(&isSucceeded);
        if (isSucceeded && !mAppTransitionRunning) {
            // animating towards back, hang onto old list for duration of animation.
            mAnimatingAppTokens.Clear();
            mAnimatingAppTokens.Assign(mAppTokens.Begin(), mAppTokens.End());
        }
        RemoveAppTokensLocked(tokens);
        objEmu->Reset();
        List< AutoPtr<AppWindowToken> >::Iterator posIt = mAppTokens.Begin();
        while (objEmu->MoveNext(&isSucceeded), isSucceeded) {
            AutoPtr<IInterface> tokenObj;
            objEmu->Current((IInterface**)&tokenObj);
            AutoPtr<IBinder> token = IBinder::Probe(tokenObj);

            AutoPtr<AppWindowToken> wt = FindAppWindowToken(token);
            if (wt != NULL) {
                // if (DEBUG_TOKEN_MOVEMENT) Slog.v(TAG,
                //         "Adding next to bottom: " + wt + " at " + pos);
                posIt = mAppTokens.Insert(posIt, wt);
                if (mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET) {
                    wt->mSendingToBottom = TRUE;
                }
                posIt++;
            }
        }

        if (!mAppTransitionRunning) {
            mAnimatingAppTokens.Clear();
            mAnimatingAppTokens.Assign(mAppTokens.Begin(), mAppTokens.End());
            MoveAppWindowsLocked(tokens, mAppTokens.Begin());
        }
    }
    Binder::RestoreCallingIdentity(origId);
    return NOERROR;
}

// -------------------------------------------------------------
// Misc IWindowSession methods
// -------------------------------------------------------------

ECode CWindowManagerService::StartFreezingScreen(
    /* [in] */ Int32 exitAnim,
    /* [in] */ Int32 enterAnim)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::FREEZE_SCREEN,
            String("startFreezingScreen()"))) {
        Slogger::E(TAG, "Requires FREEZE_SCREEN permission");
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Requires FREEZE_SCREEN permission");
    }

    AutoLock lock(mWindowMapLock);

    if (!mClientFreezingScreen) {
        mClientFreezingScreen = TRUE;
        Int64 origId = Binder::ClearCallingIdentity();
        // try {
        mH->RemoveMessages(H::CLIENT_FREEZE_TIMEOUT);
        Boolean result;
        mH->SendEmptyMessageDelayed(H::CLIENT_FREEZE_TIMEOUT, 5000, &result);
        // } finally {
        //     Binder::RestoreCallingIdentity(origId);
        // }
        Binder::RestoreCallingIdentity(origId);
    }
    return NOERROR;
}

ECode CWindowManagerService::StopFreezingScreen()
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::FREEZE_SCREEN,
            String("stopFreezingScreen()"))) {
        Slogger::E(TAG, "Requires FREEZE_SCREEN permission");
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Requires FREEZE_SCREEN permission");
    }

    AutoLock lock(mWindowMapLock);

    if (mClientFreezingScreen) {
        mClientFreezingScreen = FALSE;
        Int64 origId = Binder::ClearCallingIdentity();
        // try {
        StopFreezingDisplayLocked();
        // } finally {
        //     Binder::RestoreCallingIdentity(origId);
        // }
        Binder::RestoreCallingIdentity(origId);
    }
    return NOERROR;
}

ECode CWindowManagerService::DisableKeyguard(
    /* [in] */ IBinder* token,
    /* [in] */ const String& tag)
{
    Int32 perm;
    FAIL_RETURN(mContext->CheckCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::DISABLE_KEYGUARD,
            &perm));
    if (perm != IPackageManager::PERMISSION_GRANTED) {
        //throw new SecurityException("Requires DISABLE_KEYGUARD permission");
        Slogger::E(TAG, "Requires DISABLE_KEYGUARD permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(tag, (ICharSequence**)&seq);
    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArg1 = token;
    args->mArg2 = seq;

    AutoPtr<IMessage> msg;
    mKeyguardDisableHandler->ObtainMessage(KeyguardDisableHandler::KEYGUARD_DISABLE,
        args, (IMessage**)&msg);
    Boolean result;
    return mKeyguardDisableHandler->SendMessage(msg, &result);
}

ECode CWindowManagerService::ReenableKeyguard(
    /* [in] */ IBinder* token)
{
    Int32 perm;
    FAIL_RETURN(mContext->CheckCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::DISABLE_KEYGUARD,
            &perm));
    if (perm != IPackageManager::PERMISSION_GRANTED) {
        //throw new SecurityException("Requires DISABLE_KEYGUARD permission");
        Slogger::E(TAG, "Requires DISABLE_KEYGUARD permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoPtr<IMessage> msg;
    mKeyguardDisableHandler->ObtainMessage(KeyguardDisableHandler::KEYGUARD_REENABLE,
        token, (IMessage**)&msg);
    Boolean result;
    return mKeyguardDisableHandler->SendMessage(msg, &result);
}

ECode CWindowManagerService::ExitKeyguardSecurely(
    /* [in] */ IOnKeyguardExitResult* keyguardCallback)
{
    Int32 perm;
    FAIL_RETURN(mContext->CheckCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::DISABLE_KEYGUARD,
            &perm));
    if (perm != IPackageManager::PERMISSION_GRANTED) {
        //throw new SecurityException("Requires DISABLE_KEYGUARD permission");
        return E_SECURITY_EXCEPTION;
    }
    AutoPtr<IOnKeyguardExitResult> result = new SecurelyOnKeyguardExitResult(keyguardCallback);
    mPolicy->ExitKeyguardSecurely(result);
    return NOERROR;
}

ECode CWindowManagerService::InKeyguardRestrictedInputMode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mPolicy->InKeyguardRestrictedKeyInputMode(result);
}

ECode CWindowManagerService::IsKeyguardLocked(
    /* [out] */ Boolean* isSecure)
{
    VALIDATE_NOT_NULL(isSecure);
    return mPolicy->IsKeyguardLocked(isSecure);
}

ECode CWindowManagerService::IsKeyguardSecure(
    /* [out] */ Boolean* isSecure)
{
    VALIDATE_NOT_NULL(isSecure);
    return mPolicy->IsKeyguardSecure(isSecure);
}

ECode CWindowManagerService::DismissKeyguard()
{
    Int32 perm;
    FAIL_RETURN(mContext->CheckCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::DISABLE_KEYGUARD,
            &perm));
    if (perm != IPackageManager::PERMISSION_GRANTED) {
        //throw new SecurityException("Requires DISABLE_KEYGUARD permission");
        Slogger::E(TAG, "Requires DISABLE_KEYGUARD permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoLock lock(mWindowMapLock);
    return mPolicy->DismissKeyguardLw();
}

ECode CWindowManagerService::CloseSystemDialogs(
    /* [in] */ const String& reason)
{
    AutoLock lock(mWindowMapLock);
    HashMap<Int32, AutoPtr<DisplayContent> >::Iterator it;
    for (it = mDisplayContents.Begin(); it != mDisplayContents.End(); ++it) {
        AutoPtr<DisplayContent> content = it->mSecond;
        if (content != NULL) {
            List< AutoPtr<WindowState> >& windows = content->GetWindowList();
            if (windows.Begin() != windows.End()) {
                List< AutoPtr<WindowState> >::Iterator wit;
                for (wit = windows.Begin(); wit != windows.End(); ++wit) {
                    AutoPtr<WindowState> w = *wit;
                    if (w->mHasSurface) {
                        // try {
                        w->mClient->CloseSystemDialogs(reason);
                        // } catch (RemoteException e) {
                        // }
                    }
                }
            }
        }
    }

    return NOERROR;
}

Float CWindowManagerService::FixScale(
    /* [in] */ Float scale)
{
    if (scale < 0) scale = 0;
    else if (scale > 20) scale = 20;
    return Elastos::Core::Math::Abs(scale);
}

ECode CWindowManagerService::SetAnimationScale(
    /* [in] */ Int32 which,
    /* [in] */ Float scale)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::SET_ANIMATION_SCALE,
            String("SetAnimationScale()"))) {
        //throw new SecurityException("Requires SET_ANIMATION_SCALE permission");
        Slogger::E(TAG, "Requires SET_ANIMATION_SCALE permission");
        return E_SECURITY_EXCEPTION;
    }

    if (scale < 0) scale = 0;
    else if (scale > 20) scale = 20;
    scale = Elastos::Core::Math::Abs(scale);
    switch (which) {
        case 0: mWindowAnimationScale = FixScale(scale); break;
        case 1: mTransitionAnimationScale = FixScale(scale); break;
        case 2: mAnimatorDurationScale = FixScale(scale); break;
    }

    // Persist setting
    Boolean result;
    return mH->SendEmptyMessage(H::PERSIST_ANIMATION_SCALE, &result);
}

ECode CWindowManagerService::SetAnimationScales(
    /* [in] */ ArrayOf<Float>* scales)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::SET_ANIMATION_SCALE,
            String("SetAnimationScale()"))) {
        //throw new SecurityException("Requires SET_ANIMATION_SCALE permission");
        Slogger::E(TAG, "Requires SET_ANIMATION_SCALE permission");
        return E_SECURITY_EXCEPTION;
    }

    if (scales != NULL) {
        Int32 len = scales->GetLength();
        if (len >= 1) {
            mWindowAnimationScale = FixScale((*scales)[0]);
        }
        if (len >= 2) {
            mTransitionAnimationScale = FixScale((*scales)[1]);
        }
        if (len >= 3) {
            SetAnimatorDurationScale(FixScale((*scales)[2]));
        }
    }

    // Persist setting
    Boolean result;
    return mH->SendEmptyMessage(H::PERSIST_ANIMATION_SCALE, &result);
}

void CWindowManagerService::SetAnimatorDurationScale(
    /* [in] */ Float scale)
{
    mAnimatorDurationScale = scale;
    AutoPtr<IValueAnimatorHelper> helper;
    CValueAnimatorHelper::AcquireSingleton((IValueAnimatorHelper**)&helper);
    helper->SetDurationScale(scale);
}

ECode CWindowManagerService::GetAnimationScale(
    /* [in] */ Int32 which,
    /* [out] */ Float* scale)
{
    VALIDATE_NOT_NULL(scale);

    switch (which) {
        case 0:
            *scale = mWindowAnimationScale;
            return NOERROR;
        case 1:
            *scale = mTransitionAnimationScale;
            return NOERROR;
        case 2:
            *scale = mAnimatorDurationScale;
            return NOERROR;
    }
    *scale = 0;
    return NOERROR;
}

ECode CWindowManagerService::GetAnimationScales(
    /* [out, callee] */ ArrayOf<Float>** scales)
{
    VALIDATE_NOT_NULL(scales);
    AutoPtr< ArrayOf<Float> > temp = ArrayOf<Float>::Alloc(3);
    (*temp)[0] = mWindowAnimationScale;
    (*temp)[1] = mTransitionAnimationScale;
    (*temp)[2] = mAnimatorDurationScale;
    *scales = temp;
    REFCOUNT_ADD(*scales);
    return NOERROR;
}

ECode CWindowManagerService::GetLidState(
    /* [out] */ Int32* lidState)
{
    VALIDATE_NOT_NULL(lidState);

    Int32 sw =mInputManager->GetSwitchState(-1, IInputDevice::SOURCE_ANY,
            CInputManagerService::_SW_LID);
    if (sw > 0) {
        // Switch state: AKEY_STATE_DOWN or AKEY_STATE_VIRTUAL.
        *lidState = LID_CLOSED;
    }
    else if (sw == 0) {
        // Switch state: AKEY_STATE_UP.
        *lidState = LID_OPEN;
    }
    else {
        // Switch state: AKEY_STATE_UNKNOWN.
        *lidState = LID_ABSENT;
    }
    return NOERROR;
}

ECode CWindowManagerService::MonitorInput(
    /* [in] */ const String& name,
    /* [out] */ IInputChannel** inputChannel)
{
    VALIDATE_NOT_NULL(inputChannel);
    return mInputManager->MonitorInput(name, inputChannel);
}

ECode CWindowManagerService::SwitchKeyboardLayout(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 direction)
{
    mInputManager->SwitchKeyboardLayout(deviceId, direction);
    return NOERROR;
}

ECode CWindowManagerService::Shutdown(
    /* [in] */ Boolean confirm)
{
    ShutdownThread::Shutdown(mContext, confirm, mPolicy);
    return NOERROR;
}

ECode CWindowManagerService::RebootSafeMode(
    /* [in] */ Boolean confirm)
{
    ShutdownThread::RebootSafeMode(mContext, confirm);
    return NOERROR;
}

ECode CWindowManagerService::SetInputFilter(
    /* [in] */ IIInputFilter* filter)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::FILTER_EVENTS,
            String("setInputFilter()"))) {
        Slogger::E(TAG, "Requires FILTER_EVENTS permission");
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Requires FILTER_EVENTS permission");
    }
    mInputManager->SetInputFilter(filter);
    return NOERROR;
}

void CWindowManagerService::SetCurrentUser(
    /* [in] */ Int32 newUserId)
{
    AutoLock lock(mWindowMapLock);
    mCurrentUserId = newUserId;
    mPolicy->SetCurrentUserLw(newUserId);

    // Hide windows that should not be seen by the new user.
    HashMap<Int32, AutoPtr<DisplayContent> >::Iterator it;
    for (it = mDisplayContents.Begin(); it != mDisplayContents.End(); ++it) {
        List< AutoPtr<WindowState> >& windows = it->mSecond->GetWindowList();
        List< AutoPtr<WindowState> >::Iterator wit;
        for (wit = windows.Begin(); wit != windows.End(); ++wit) {
            AutoPtr<WindowState> win = *wit;
            Int32 type;
            win->mAttrs->GetType(&type);
            Slogger::W(TAG, "current user violation %d hiding %p, attrs=%d, belonging to ",
                    newUserId, win.Get(), type, win->mOwnerUid);
            if (win->IsHiddenFromUserLocked()) {
                Boolean result;
                win->HideLw(FALSE, &result);
            }
        }
    }
    PerformLayoutAndPlaceSurfacesLocked();
}

void CWindowManagerService::EnableScreenAfterBoot()
{
    {
        AutoLock lock(mWindowMapLock);

        // if (DEBUG_BOOT) {
        //     RuntimeException here = new RuntimeException("here");
        //     here.fillInStackTrace();
        //     Slog.i(TAG, "enableScreenAfterBoot: mDisplayEnabled=" + mDisplayEnabled
        //             + " mForceDisplayEnabled=" + mForceDisplayEnabled
        //             + " mShowingBootMessages=" + mShowingBootMessages
        //             + " mSystemBooted=" + mSystemBooted, here);
        // }
        if (mSystemBooted) {
            return;
        }
        mSystemBooted = TRUE;
        HideBootMessagesLocked();
        // If the screen still doesn't come up after 30 seconds, give
        // up and turn it on.
        Boolean result;
        mH->SendEmptyMessageDelayed(H::BOOT_TIMEOUT, 30*1000, &result);
    }

    mPolicy->SystemBooted();

    PerformEnableScreen();
}

void CWindowManagerService::EnableScreenIfNeededLocked()
{
    // if (DEBUG_BOOT) {
    //     RuntimeException here = new RuntimeException("here");
    //     here.fillInStackTrace();
    //     Slog.i(TAG, "enableScreenIfNeededLocked: mDisplayEnabled=" + mDisplayEnabled
    //             + " mForceDisplayEnabled=" + mForceDisplayEnabled
    //             + " mShowingBootMessages=" + mShowingBootMessages
    //             + " mSystemBooted=" + mSystemBooted, here);
    // }
    if (mDisplayEnabled) {
        return;
    }
    if (!mSystemBooted && !mShowingBootMessages) {
        return;
    }

    Boolean result;
    mH->SendEmptyMessage(H::ENABLE_SCREEN, &result);
}

void CWindowManagerService::PerformBootTimeout()
{
    {
        AutoLock lock(mWindowMapLock);

        if (mDisplayEnabled || mHeadless) {
            return;
        }
        Slogger::W(TAG, "***** BOOT TIMEOUT: forcing display enabled");
        mForceDisplayEnabled = TRUE;
    }
    PerformEnableScreen();
}

void CWindowManagerService::PerformEnableScreen()
{
    {
        AutoLock lock(mWindowMapLock);

        // if (DEBUG_BOOT) {
        //     RuntimeException here = new RuntimeException("here");
        //     here.fillInStackTrace();
        //     Slog.i(TAG, "performEnableScreen: mDisplayEnabled=" + mDisplayEnabled
        //             + " mForceDisplayEnabled=" + mForceDisplayEnabled
        //             + " mShowingBootMessages=" + mShowingBootMessages
        //             + " mSystemBooted=" + mSystemBooted
        //             + " mOnlyCore=" + mOnlyCore, here);
        // }
        if (mDisplayEnabled) {
            return;
        }
        if (!mSystemBooted && !mShowingBootMessages) {
            return;
        }

        if (!mForceDisplayEnabled) {
            // Don't enable the screen until all existing windows
            // have been drawn.
            Boolean haveBootMsg = FALSE;
            Boolean haveApp = FALSE;
            // if the wallpaper service is disabled on the device, we're never going to have
            // wallpaper, don't bother waiting for it
            // todo: Eric
            Boolean haveWallpaper = TRUE;
            AutoPtr<IResources> r;
            mContext->GetResources((IResources**)&r);
            Boolean res;
            r->GetBoolean(R::bool_::config_enableWallpaperService, &res);
            Boolean wallpaperEnabled = res && !mOnlyCore;
            Boolean haveKeyguard = TRUE;
            // TODO(multidisplay): Expand to all displays?
            List< AutoPtr<WindowState> >& windows = GetDefaultWindowListLocked();
            List< AutoPtr<WindowState> >::Iterator it;
            for (it = windows.Begin(); it != windows.End(); ++it) {
                AutoPtr<WindowState> w = *it;
                Int32 type;
                w->mAttrs->GetType(&type);
                if (type == IWindowManagerLayoutParams::TYPE_KEYGUARD) {
                    // Only if there is a keyguard attached to the window manager
                    // will we consider ourselves as having a keyguard.  If it
                    // isn't attached, we don't know if it wants to be shown or
                    // hidden.  If it is attached, we will say we have a keyguard
                    // if the window doesn't want to be visible, because in that
                    // case it explicitly doesn't want to be shown so we should
                    // not delay turning the screen on for it.
                    Boolean vis = w->mViewVisibility == IView::VISIBLE
                            && w->mPolicyVisibility;
                    haveKeyguard = !vis;
                }
                Boolean isVisible;
                w->IsVisibleLw(&isVisible);
                if (isVisible && !w->mObscured && !w->IsDrawnLw()) {
                    return;
                }
                if (w->IsDrawnLw()) {
                    if (type == IWindowManagerLayoutParams::TYPE_BOOT_PROGRESS) {
                        haveBootMsg = TRUE;
                    }
                    else if (type == IWindowManagerLayoutParams::TYPE_APPLICATION) {
                        haveApp = TRUE;
                    }
                    else if (type == IWindowManagerLayoutParams::TYPE_WALLPAPER) {
                        haveWallpaper = TRUE;
                    }
                    else if (type == IWindowManagerLayoutParams::TYPE_KEYGUARD) {
                        haveKeyguard = TRUE;
                    }
                }
            }

            // if (DEBUG_SCREEN_ON || DEBUG_BOOT) {
            //     Slog.i(TAG, "******** booted=" + mSystemBooted + " msg=" + mShowingBootMessages
            //             + " haveBoot=" + haveBootMsg + " haveApp=" + haveApp
            //             + " haveWall=" + haveWallpaper + " wallEnabled=" + wallpaperEnabled
            //             + " haveKeyguard=" + haveKeyguard);
            // }

            // If we are turning on the screen to show the boot message,
            // don't do it until the boot message is actually displayed.
            if (!mSystemBooted && !haveBootMsg) {
                return;
            }

            // If we are turning on the screen after the boot is completed
            // normally, don't do so until we have the application and
            // wallpaper.
            if (mSystemBooted && ((!haveApp && !haveKeyguard) ||
                    (wallpaperEnabled && !haveWallpaper))) {
                return;
            }
        }

        mDisplayEnabled = TRUE;
        // if (DEBUG_SCREEN_ON || DEBUG_BOOT) Slog.i(TAG, "******************** ENABLING SCREEN!");
        // if (FALSE) {
        //     StringWriter sw = new StringWriter();
        //     PrintWriter pw = new PrintWriter(sw);
        //     this.dump(null, pw, null);
        //     Slog.i(TAG, sw.toString());
        // }
        // try {
        android::sp<android::IServiceManager> sm = android::defaultServiceManager();
        android::sp<android::IBinder> surfaceFlinger = sm->getService(android::String16("SurfaceFlinger"));

        if (surfaceFlinger != NULL) {
            Slogger::I(TAG, "******* TELLING SURFACE FLINGER WE ARE BOOTED!");
            android::Parcel data;
            data.writeInterfaceToken(android::String16("android.ui.ISurfaceComposer"));
            surfaceFlinger->transact(android::IBinder::FIRST_CALL_TRANSACTION, // BOOT_FINISHED
                                    data, NULL, 0);
        }
        // } catch (RemoteException ex) {
        //     Slog.e(TAG, "Boot completed: SurfaceFlinger is dead!");
        // }

        // Enable input dispatch.
        mInputMonitor->SetEventDispatchingLw(mEventDispatchingEnabled);
    }

    mPolicy->EnableScreenAfterBoot();

    // Make sure the last requested orientation has been applied.
    UpdateRotationUnchecked(FALSE, FALSE);
}

void CWindowManagerService::ShowBootMessage(
    /* [in] */ ICharSequence* msg,
    /* [in] */ Boolean always)
{
    Boolean first = FALSE;
    {
        AutoLock lock(mWindowMapLock);
        // if (DEBUG_BOOT) {
        //     RuntimeException here = new RuntimeException("here");
        //     here.fillInStackTrace();
        //     Slog.i(TAG, "showBootMessage: msg=" + msg + " always=" + always
        //             + " mAllowBootMessages=" + mAllowBootMessages
        //             + " mShowingBootMessages=" + mShowingBootMessages
        //             + " mSystemBooted=" + mSystemBooted, here);
        // }
        if (!mAllowBootMessages) {
            return;
        }
        if (!mShowingBootMessages) {
            if (!always) {
                return;
            }
            first = TRUE;
        }
        if (mSystemBooted) {
            return;
        }
        mShowingBootMessages = TRUE;
        mPolicy->ShowBootMessage(msg, always);
    }
    if (first) {
        PerformEnableScreen();
    }
}

void CWindowManagerService::HideBootMessagesLocked()
{
    // if (DEBUG_BOOT) {
    //     RuntimeException here = new RuntimeException("here");
    //     here.fillInStackTrace();
    //     Slog.i(TAG, "hideBootMessagesLocked: mDisplayEnabled=" + mDisplayEnabled
    //             + " mForceDisplayEnabled=" + mForceDisplayEnabled
    //             + " mShowingBootMessages=" + mShowingBootMessages
    //             + " mSystemBooted=" + mSystemBooted, here);
    // }
    if (mShowingBootMessages) {
        mShowingBootMessages = FALSE;
        mPolicy->HideBootMessages();
    }
}

ECode CWindowManagerService::SetInTouchMode(
    /* [in] */ Boolean mode)
{
    AutoLock lock(mWindowMapLock);
    mInTouchMode = mode;
    return NOERROR;
}

ECode CWindowManagerService::ShowStrictModeViolation(
    /* [in] */ Boolean on)
{
    if (mHeadless) return NOERROR;
    Int32 pid = Binder::GetCallingPid();
    AutoPtr<IMessage> msg;
    mH->ObtainMessage(H::SHOW_STRICT_MODE_VIOLATION, on ? 1 : 0, pid, (IMessage**)&msg);

    Boolean result;
    return mH->SendMessage(msg, &result);
}

void CWindowManagerService::ShowStrictModeViolation(
    /* [in] */ Int32 arg,
    /* [in] */ Int32 pid)
{
    Boolean on = arg != 0;
    AutoLock lock(mWindowMapLock);
    // Ignoring requests to enable the red border from clients
    // which aren't on screen.  (e.g. Broadcast Receivers in
    // the background..)
    if (on) {
        Boolean isVisible = FALSE;
        HashMap<Int32, AutoPtr<DisplayContent> >::Iterator it;
        for (it = mDisplayContents.Begin(); it != mDisplayContents.End(); ++it) {
            AutoPtr<DisplayContent> content = it->mSecond;
            if (content != NULL) {
                List< AutoPtr<WindowState> >& windows = content->GetWindowList();
                if (windows.Begin() != windows.End()) {
                    List< AutoPtr<WindowState> >::Iterator wit;
                    for (wit = windows.Begin(); wit != windows.End(); ++wit) {
                        AutoPtr<WindowState> ws = *wit;
                        Boolean res;
                        if (ws->mSession->mPid == pid && (ws->IsVisibleLw(&res), res)) {
                            isVisible = TRUE;
                            break;
                        }
                    }
                }
            }
        }
        if (!isVisible) {
            return;
        }
    }

    // if (SHOW_LIGHT_TRANSACTIONS) Slog.i(TAG,
    //         ">>> OPEN TRANSACTION showStrictModeViolation");
    AutoPtr<ISurfaceHelper> surfaceHelper;
    CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&surfaceHelper);
    surfaceHelper->OpenTransaction();
    // try {
    // TODO(multi-display): support multiple displays
    if (mStrictModeFlash == NULL) {
        mStrictModeFlash = new StrictModeFlash(
                GetDefaultDisplayContentLocked()->GetDisplay(), mFxSession);
    }
    mStrictModeFlash->SetVisibility(on);
    surfaceHelper->CloseTransaction();
    // } finally {
    surfaceHelper->CloseTransaction();
    if (SHOW_LIGHT_TRANSACTIONS) Slogger::I(TAG,
            "<<< CLOSE TRANSACTION showStrictModeViolation");
    // }
}

ECode CWindowManagerService::SetStrictModeVisualIndicatorPreference(
    /* [in] */ const String& value)
{
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    sysProp->Set(String("persist.sys.strictmode.visual")/*TODO: StrictMode::VISUAL_PROPERTY*/, value);
    return NOERROR;
}

ECode CWindowManagerService::ScreenshotApplications(
    /* [in] */ IBinder* appToken,
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 maxWidth,
    /* [in] */ Int32 maxHeight,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    *bitmap = NULL;

    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::READ_FRAME_BUFFER,
            String("ScreenshotApplications()"))) {
        Slogger::E(TAG, "Requires READ_FRAME_BUFFER permission");
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Requires READ_FRAME_BUFFER permission");
    }

    AutoPtr<IBitmap> rawss;

    Int32 maxLayer = 0;
    AutoPtr<IRect> frame;
    ASSERT_SUCCEEDED(CRect::New((IRect**)&frame));

    Float scale;
    Int32 dw, dh;
    Int32 rot;

    {
        AutoLock lock(mWindowMapLock);
        Int64 ident = Binder::ClearCallingIdentity();

        AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
        if (displayContent == NULL) {
            Binder::RestoreCallingIdentity(ident);
            return NOERROR;
        }
        AutoPtr<IDisplayInfo> displayInfo = displayContent->GetDisplayInfo();
        displayInfo->GetLogicalWidth(&dw);
        displayInfo->GetLogicalHeight(&dh);

        Int32 layer;
        mPolicy->WindowTypeToLayerLw(IWindowManagerLayoutParams::TYPE_APPLICATION, &layer);
        Int32 aboveAppLayer = layer * TYPE_LAYER_MULTIPLIER + TYPE_LAYER_OFFSET;
        aboveAppLayer += TYPE_LAYER_MULTIPLIER;

        Boolean isImeTarget = mInputMethodTarget != NULL
                && mInputMethodTarget->mAppToken != NULL
                && mInputMethodTarget->mAppToken->mAppToken != NULL
                && IBinder::Probe(mInputMethodTarget->mAppToken->mAppToken) == appToken;

        // Figure out the part of the screen that is actually the app.
        AutoPtr<ISurfaceHelper> helper;
        CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&helper);
        helper->OpenTransaction();
        Int32 surfaceW, surfaceH;

        Boolean including = FALSE;
        List< AutoPtr<WindowState> >& windows = displayContent->GetWindowList();

        List< AutoPtr<WindowState> >::ReverseIterator rit;
        for (rit = windows.RBegin(); rit != windows.REnd(); ++rit) {
            AutoPtr<WindowState> ws = *rit;
            if (!ws->mHasSurface) {
                continue;
            }
            if (ws->mLayer >= aboveAppLayer) {
                continue;
            }
            // When we will skip windows: when we are not including
            // ones behind a window we didn't skip, and we are actually
            // taking a screenshot of a specific app.
            if (!including && appToken != NULL) {
                // Also, we can possibly skip this window if it is not
                // an IME target or the application for the screenshot
                // is not the current IME target.
                if (!ws->mIsImWindow || !isImeTarget) {
                    // And finally, this window is of no interest if it
                    // is not associated with the screenshot app.
                    if (ws->mAppToken == NULL || ws->mAppToken->mToken.Get() != appToken) {
                        continue;
                    }
                }
            }

            // We keep on including windows until we go past a full-screen
            // window.
            including = !ws->mIsImWindow && !ws->IsFullscreen(dw, dh);

            AutoPtr<WindowStateAnimator> winAnimator = ws->mWinAnimator;

            // The setSize() method causes all previous Surface transactions to sync to
            // the SurfaceFlinger. This will force any outstanding setLayer calls to be
            // synced as well for screen capture. Without this we can get black bitmaps.
            AutoPtr<ISurface> surface = winAnimator->mSurface;
            surface->GetWidth(&surfaceW);
            surface->GetHeight(&surfaceH);
            surface->SetSize(surfaceW, surfaceH);

            if (maxLayer < winAnimator->mSurfaceLayer) {
                maxLayer = winAnimator->mSurfaceLayer;
            }

            // Don't include wallpaper in bounds calculation
            if (!ws->mIsWallpaper) {
                AutoPtr<IRect> wf = ws->mFrame;
                AutoPtr<IRect> cr = ws->mContentInsets;
                Int32 v1, v2;
                wf->GetLeft(&v1);
                cr->GetLeft(&v2);
                Int32 left = v1 + v2;
                wf->GetTop(&v1);
                cr->GetTop(&v2);
                Int32 top = v1 + v2;
                wf->GetRight(&v1);
                cr->GetRight(&v2);
                Int32 right = v1 - v2;
                wf->GetBottom(&v1);
                cr->GetBottom(&v2);
                Int32 bottom = v1 - v2;
                frame->Union(left, top, right, bottom);
            }
        }

        helper->CloseTransaction();
        Binder::RestoreCallingIdentity(ident);

        // Constrain frame to the screen size.
        Boolean result;
        frame->Intersect(0, 0, dw, dh, &result);

        Boolean isEmpty;
        frame->IsEmpty(&isEmpty);
        if (isEmpty || maxLayer == 0) {
            return NOERROR;
        }

        // The screenshot API does not apply the current screen rotation.
        AutoPtr<IDisplay> d = GetDefaultDisplayContentLocked()->GetDisplay();
        d->GetRotation(&rot);
        AutoPtr<ISystemProperties> sysProp;
        CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
        Int32 value;
        sysProp->GetInt32(String("ro.sf.hwrotation"), 0, &value);
        if (value == 270) {
            rot = (rot + 3) % 4;
        }

        Int32 fw, fh;
        frame->GetWidth(&fw);
        frame->GetHeight(&fh);

        // Constrain thumbnail to smaller of screen width or height. Assumes aspect
        // of thumbnail is the same as the screen (in landscape) or square.
        Float targetWidthScale = maxWidth / (Float) fw;
        Float targetHeightScale = maxHeight / (Float) fh;
        if (dw <= dh) {
            scale = targetWidthScale;
            // If aspect of thumbnail is the same as the screen (in landscape),
            // select the slightly larger value so we fill the entire bitmap
            if (targetHeightScale > scale && (Int32)(targetHeightScale * fw) == maxWidth) {
                scale = targetHeightScale;
            }
        }
        else {
            scale = targetHeightScale;
            // If aspect of thumbnail is the same as the screen (in landscape),
            // select the slightly larger value so we fill the entire bitmap
            if (targetWidthScale > scale && (Int32)(targetWidthScale * fh) == maxHeight) {
                scale = targetWidthScale;
            }
        }

        // The screen shot will contain the entire screen.
        dw = (Int32)(dw * scale);
        dh = (Int32)(dh * scale);
        if (rot == ISurface::ROTATION_90 || rot == ISurface::ROTATION_270) {
            Int32 tmp = dw;
            dw = dh;
            dh = tmp;
            rot = (rot == ISurface::ROTATION_90) ?
                    ISurface::ROTATION_270 : ISurface::ROTATION_90;
        }
        // if (DEBUG_SCREENSHOT) {
        //     Slog.i(TAG, "Screenshot: " + dw + "x" + dh + " from 0 to " + maxLayer);
        //     for (int i = 0; i < windows.size(); i++) {
        //         WindowState win = windows.get(i);
        //         Slog.i(TAG, win + ": " + win.mLayer
        //                 + " animLayer=" + win.mWinAnimator.mAnimLayer
        //                 + " surfaceLayer=" + win.mWinAnimator.mSurfaceLayer);
        //     }
        // }
        helper->Screenshot(dw, dh, 0, maxLayer, (IBitmap**)&rawss);
    }

    if (rawss == NULL) {
        Slogger::W(TAG, "Failure taking screenshot for (%dx%d) to layer %d",
            dw, dh, maxLayer);
        return NOERROR;
    }

    BitmapConfig config;
    rawss->GetConfig(&config);
    AutoPtr<IBitmap> bm;
    AutoPtr<IBitmapFactory> factory;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory);
    ASSERT_SUCCEEDED(factory->CreateBitmap(maxWidth, maxHeight, config, (IBitmap**)&bm));

    AutoPtr<IMatrix> matrix;
    ASSERT_SUCCEEDED(CMatrix::New((IMatrix**)&matrix));
    ScreenRotationAnimation::CreateRotationMatrix(rot, dw, dh, matrix);
    Int32 left, top;
    frame->GetLeft(&left);
    frame->GetTop(&top);
    Boolean result;
    matrix->PostTranslate(-(Float)Elastos::Core::Math::Ceil(left * scale),
            -(Float)Elastos::Core::Math::Ceil(top * scale), &result);
    AutoPtr<ICanvas> canvas;
    ASSERT_SUCCEEDED(CCanvas::New(bm, (ICanvas**)&canvas));
    canvas->DrawBitmap(rawss, matrix, NULL);
    canvas->SetBitmap(NULL);

    rawss->Recycle();
    *bitmap = bm;
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

ECode CWindowManagerService::FreezeRotation(
    /* [in] */ Int32 rotation)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::SET_ORIENTATION,
            String("freezeRotation()"))) {
        Slogger::E(TAG, "Requires SET_ORIENTATION permission");
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Requires SET_ORIENTATION permission");
    }
    if (rotation < -1 || rotation > ISurface::ROTATION_270) {
        Slogger::E(TAG, "Rotation argument must be -1 or a valid rotation constant.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("Rotation argument must be -1 or a valid "
        //         + "rotation constant.");
    }

    if (DEBUG_ORIENTATION) Slogger::V(TAG, "freezeRotation: mRotation=%d", mRotation);

    mPolicy->SetUserRotationMode(IWindowManagerPolicy::USER_ROTATION_LOCKED,
            rotation == -1 ? mRotation : rotation);
    UpdateRotationUnchecked(FALSE, FALSE);

    return NOERROR;
}

ECode CWindowManagerService::ThawRotation()
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::SET_ORIENTATION,
            String("thawRotation()"))) {
        Slogger::E(TAG, "Requires SET_ORIENTATION permission");
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Requires SET_ORIENTATION permission");
    }

    if (DEBUG_ORIENTATION) Slogger::V(TAG, "thawRotation: mRotation=%d", mRotation);

    mPolicy->SetUserRotationMode(IWindowManagerPolicy::USER_ROTATION_FREE, 777); // rot not used
    UpdateRotationUnchecked(FALSE, FALSE);
    return NOERROR;
}

ECode CWindowManagerService::UpdateRotation(
    /* [in] */ Boolean alwaysSendConfiguration,
    /* [in] */ Boolean forceRelayout)
{
    UpdateRotationUnchecked(alwaysSendConfiguration, forceRelayout);
    return NOERROR;
}

void CWindowManagerService::PauseRotationLocked()
{
    mDeferredRotationPauseCount += 1;
}

void CWindowManagerService::ResumeRotationLocked()
{
    if (mDeferredRotationPauseCount > 0) {
        mDeferredRotationPauseCount -= 1;
        if (mDeferredRotationPauseCount == 0) {
            Boolean changed = UpdateRotationUncheckedLocked(FALSE);
            if (changed) {
                Boolean result;
                mH->SendEmptyMessage(H::SEND_NEW_CONFIGURATION, &result);
            }
        }
    }
}

void CWindowManagerService::UpdateRotationUnchecked(
    /* [in] */ Boolean alwaysSendConfiguration,
    /* [in] */ Boolean forceRelayout)
{
    // if(DEBUG_ORIENTATION) Slog.v(TAG, "updateRotationUnchecked("
    //            + "alwaysSendConfiguration=" + alwaysSendConfiguration + ")");

    Int64 origId = Binder::ClearCallingIdentity();
    Boolean changed;
    {
        AutoLock lock(mWindowMapLock);
        changed = UpdateRotationUncheckedLocked(FALSE);
        if (!changed || forceRelayout) {
            GetDefaultDisplayContentLocked()->mLayoutNeeded = TRUE;
            PerformLayoutAndPlaceSurfacesLocked();
        }
    }

    if (changed || alwaysSendConfiguration) {
        //add by kinier
        // try{
        if (alwaysSendConfiguration == FALSE){
            AutoPtr<IIntent> intent;
            CIntent::New(String("android.window.action.ROTATION"), (IIntent**)&intent);
            intent->AddFlags(IIntent::FLAG_RECEIVER_REPLACE_PENDING);
            mContext->SendStickyBroadcast(intent);
        }
        // }catch(Exception e){
        // }
        SendNewConfiguration();
    }

    Binder::RestoreCallingIdentity(origId);
}

Boolean CWindowManagerService::UpdateRotationUncheckedLocked(
    /* [in] */ Boolean inTransaction)
{
    if (mDeferredRotationPauseCount > 0) {
        // Rotation updates have been paused temporarily.  Defer the update until
        // updates have been resumed.
        // if (DEBUG_ORIENTATION) Slog.v(TAG, "Deferring rotation, rotation is paused.");
        return FALSE;
    }

    AutoPtr<ScreenRotationAnimation> screenRotationAnimation =
            mAnimator->GetScreenRotationAnimationLocked(IDisplay::DEFAULT_DISPLAY);
    if (screenRotationAnimation != NULL && screenRotationAnimation->IsAnimating()) {
        // Rotation updates cannot be performed while the previous rotation change
        // animation is still in progress.  Skip this update.  We will try updating
        // again after the animation is finished and the display is unfrozen.
        // if (DEBUG_ORIENTATION) Slog.v(TAG, "Deferring rotation, animation in progress.");
        return FALSE;
    }

    if (!mDisplayEnabled) {
        // No point choosing a rotation if the display is not enabled.
        // if (DEBUG_ORIENTATION) Slog.v(TAG, "Deferring rotation, display is not enabled.");
        return FALSE;
    }

    // TODO: Implement forced rotation changes.
    //       Set mAltOrientation to indicate that the application is receiving
    //       an orientation that has different metrics than it expected.
    //       eg. Portrait instead of Landscape.

    // todo: Eric
    Int32 rotation = ISurface::ROTATION_0;
    mPolicy->RotationForOrientationLw(mForcedAppOrientation, mRotation, &rotation);
    Boolean isCompatible;
    mPolicy->RotationHasCompatibleMetricsLw(
            mForcedAppOrientation, rotation, &isCompatible);
    Boolean altOrientation = !isCompatible;

    // if (DEBUG_ORIENTATION) {
    //     Slog.v(TAG, "Application requested orientation "
    //             + mForcedAppOrientation + ", got rotation " + rotation
    //             + " which has " + (altOrientation ? "incompatible" : "compatible")
    //             + " metrics");
    // }

    if (mRotation == rotation && mAltOrientation == altOrientation) {
        // No change.
        return FALSE;
    }

    // if (DEBUG_ORIENTATION) {
    //     Slog.v(TAG,
    //         "Rotation changed to " + rotation + (altOrientation ? " (alt)" : "")
    //         + " from " + mRotation + (mAltOrientation ? " (alt)" : "")
    //         + ", forceApp=" + mForcedAppOrientation);
    // }

    mRotation = rotation;
    mAltOrientation = altOrientation;
    mPolicy->SetRotationLw(mRotation);

    mWindowsFreezingScreen = TRUE;
    mH->RemoveMessages(H::WINDOW_FREEZE_TIMEOUT);
    Boolean result;
    mH->SendEmptyMessageDelayed(H::WINDOW_FREEZE_TIMEOUT,
        WINDOW_FREEZE_TIMEOUT_DURATION, &result);

    mWaitingForConfig = TRUE;
    GetDefaultDisplayContentLocked()->mLayoutNeeded = TRUE;
    StartFreezingDisplayLocked(inTransaction, 0, 0);
    // startFreezingDisplayLocked can reset the ScreenRotationAnimation.
    screenRotationAnimation =
            mAnimator->GetScreenRotationAnimationLocked(IDisplay::DEFAULT_DISPLAY);

    // We need to update our screen size information to match the new
    // rotation.  Note that this is redundant with the later call to
    // sendNewConfiguration() that must be called after this function
    // returns...  however we need to do the screen size part of that
    // before then so we have the correct size to use when initializing
    // the rotation animation for the new rotation.
    ComputeScreenConfigurationLocked(NULL);

    AutoPtr<DisplayContent> displayContent = GetDefaultDisplayContentLocked();
    AutoPtr<IDisplayInfo> displayInfo = displayContent->GetDisplayInfo();

    //add by Bevis. rotate the video.
    //Log.e(TAG, "#########call MediaPlayer.setRotation(), rotation = " + rotation);
    //TODO MediaPlayer::SetRotation(rotation);

    if (!inTransaction) {
        if (SHOW_TRANSACTIONS) {
            Slogger::I(TAG, ">>> OPEN TRANSACTION setRotationUnchecked");
        }
        AutoPtr<ISurfaceHelper> helper;
        CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&helper);
        helper->OpenTransaction();
    }
    // try {
    // NOTE: We disable the rotation in the emulator because
    //       it doesn't support hardware OpenGL emulation yet.
    if (CUSTOM_SCREEN_ROTATION && screenRotationAnimation != NULL
            && screenRotationAnimation->HasScreenshot()) {
        Int32 w, h;
        displayInfo->GetLogicalWidth(&w);
        displayInfo->GetLogicalHeight(&h);
        if (screenRotationAnimation->SetRotationInTransaction(
                rotation, mFxSession,
                MAX_ANIMATION_DURATION, mTransitionAnimationScale, w, h)) {
            UpdateLayoutToAnimationLocked();
        }
    }

    mDisplayManagerService->PerformTraversalInTransactionFromWindowManager();
    // } finally {
    if (!inTransaction) {
        AutoPtr<ISurfaceHelper> helper;
        CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&helper);
        helper->CloseTransaction();
        if (SHOW_LIGHT_TRANSACTIONS) {
            Slogger::I(TAG, "<<< CLOSE TRANSACTION setRotationUnchecked");
        }
    }
    // }

    List< AutoPtr<WindowState> >& windows = displayContent->GetWindowList();
    List< AutoPtr<WindowState> >::ReverseIterator rit;
    for (rit = windows.RBegin(); rit != windows.REnd(); ++rit) {
        AutoPtr<WindowState> w = *rit;
        if (w->mHasSurface) {
            // if (DEBUG_ORIENTATION) Slog.v(TAG, "Set mOrientationChanging of " + w);
            w->mOrientationChanging = TRUE;
            mInnerFields->mOrientationChangeComplete = FALSE;
        }
    }

    List< AutoPtr<IRotationWatcher> >::ReverseIterator watcherRIt
            = mRotationWatchers.RBegin();
    for (;  watcherRIt != mRotationWatchers.REnd(); ++watcherRIt) {
        // try {
        (*watcherRIt)->OnRotationChanged(rotation);
        // } catch (RemoteException e) {
        // }
    }

    ScheduleNotifyRotationChangedIfNeededLocked(displayContent, rotation);

    return TRUE;
}

ECode CWindowManagerService::GetRotation(
    /* [out] */ Int32* rotation)
{
    VALIDATE_NOT_NULL(rotation);
    *rotation = mRotation;
    return NOERROR;
}

ECode CWindowManagerService::WatchRotation(
    /* [in] */ IRotationWatcher* watcher,
    /* [out] */ Int32* rotation)
{
    VALIDATE_NOT_NULL(rotation);

    AutoPtr<IBinder> watcherBinder = IBinder::Probe(watcher);
    AutoPtr<RotationWatcherDeathRecipint> dr =
        new RotationWatcherDeathRecipint(this, watcherBinder);

    AutoLock lock(mWindowMapLock);

    AutoPtr<IProxy> proxy = (IProxy*)watcher->Probe(EIID_IProxy);
    assert(proxy != NULL);
    proxy->LinkToDeath(dr.Get(), 0);
    mRotationWatchers.PushBack(watcher);

    *rotation = mRotation;
    return NOERROR;
}

ECode CWindowManagerService::GetPreferredOptionsPanelGravity(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    AutoLock lock(mWindowMapLock);
    Int32 rotation;
    GetRotation(&rotation);

    // TODO(multidisplay): Assume that such devices physical keys are on the main screen.
    AutoPtr<DisplayContent> displayContent = GetDefaultDisplayContentLocked();
    if (displayContent->mInitialDisplayWidth < displayContent->mInitialDisplayHeight) {
        // On devices with a natural orientation of portrait
        switch (rotation) {
            default:
            case ISurface::ROTATION_0:
                *value = IGravity::CENTER_HORIZONTAL | IGravity::BOTTOM;
            case ISurface::ROTATION_90:
                *value = IGravity::RIGHT | IGravity::BOTTOM;
            case ISurface::ROTATION_180:
                *value = IGravity::CENTER_HORIZONTAL | IGravity::BOTTOM;
            case ISurface::ROTATION_270:
                *value = IGravity::START | IGravity::BOTTOM;
        }
    }
    else {
        // On devices with a natural orientation of landscape
        switch (rotation) {
            default:
            case ISurface::ROTATION_0:
                *value = IGravity::RIGHT | IGravity::BOTTOM;
            case ISurface::ROTATION_90:
                *value = IGravity::CENTER_HORIZONTAL | IGravity::BOTTOM;
            case ISurface::ROTATION_180:
                *value = IGravity::START | IGravity::BOTTOM;
            case ISurface::ROTATION_270:
                *value = IGravity::CENTER_HORIZONTAL | IGravity::BOTTOM;
        }
    }

    return NOERROR;
}

ECode CWindowManagerService::StartViewServer(
    /* [in] */ Int32 port,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (IsSystemSecure()) {
        *result = FALSE;
        return NOERROR;
    }

    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::DUMP, String("startViewServer"))) {
        *result = FALSE;
        return NOERROR;
    }

    if (port < 1024) {
        *result = FALSE;
        return NOERROR;
    }

    if (mViewServer != NULL) {
        if (!mViewServer->IsRunning()) {
            // try {
            ECode ec = mViewServer->Start(result);
            if (FAILED(ec)) {
                Slogger::W(TAG, "View server did not start");
                *result = FALSE;
                return ec;
            }
            return NOERROR;
            // } catch (IOException e) {
            //     Slog.w(TAG, "View server did not start");
            // }
        }
    }

    // try {
    mViewServer = new ViewServer(this, port);
    ECode ec = mViewServer->Start(result);
    if (FAILED(ec)) {
        Slogger::W(TAG, "View server did not start");
        *result = FALSE;
        return ec;
    }
    // } catch (IOException e) {
    //     Slog.w(TAG, "View server did not start");
    // }
    return NOERROR;
}

Boolean CWindowManagerService::IsSystemSecure()
{
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    String value1, value2;
    return (sysProp->Get(SYSTEM_SECURE, String("1"), &value1), String("1").Equals(value1)) &&
            (sysProp->Get(SYSTEM_DEBUGGABLE, String("0"), &value2), String("0").Equals(value2));
}

ECode CWindowManagerService::StopViewServer(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if (IsSystemSecure()) {
        return NOERROR;
    }

    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::DUMP, String("stopViewServer"))) {
        return NOERROR;
    }

    if (mViewServer != NULL) {
        *result = mViewServer->Stop();
    }
    return NOERROR;
}

ECode CWindowManagerService::IsViewServerRunning(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (IsSystemSecure()) {
        *result = FALSE;
        return NOERROR;
    }

    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::DUMP, String("isViewServerRunning"))) {
        *result = FALSE;
        return NOERROR;
    }

    *result = mViewServer != NULL && mViewServer->IsRunning();
    return NOERROR;
}

Boolean CWindowManagerService::ViewServerListWindows(
    /* [in] */ ISocket* client)
{
    if (IsSystemSecure()) {
        return FALSE;
    }

    Boolean result = TRUE;

    List< AutoPtr<WindowState> > windows;

    {
        AutoLock lock(mWindowMapLock);
        //noinspection unchecked
        HashMap<Int32, AutoPtr<DisplayContent> >::Iterator it;
        for (it = mDisplayContents.Begin(); it != mDisplayContents.End(); ++it) {
            List< AutoPtr<WindowState> > wins = it->mSecond->GetWindowList();
            List< AutoPtr<WindowState> >::Iterator  wit;
            for (wit = wins.Begin(); wit != wins.End(); ++wit) {
                windows.PushBack(*wit);
            }
        }
    }

    AutoPtr<IBufferedWriter> out;

    // Any uncaught exception will crash the system process
    // try {
    AutoPtr<IOutputStream> clientStream;
    if (FAILED(client->GetOutputStream((IOutputStream**)&clientStream))) {
        return FALSE;
    }
    AutoPtr<IOutputStreamWriter> osWriter;
    if (FAILED(COutputStreamWriter::New(clientStream, (IOutputStreamWriter**)&osWriter))) {
        return FALSE;
    }
    if (FAILED(CBufferedWriter::New(osWriter, 8 * 1024, (IBufferedWriter**)&out))) {
        return FALSE;
    }

    Int32 focusHashcode = (Int32)mCurrentFocus.Get();
    Int32 hashcode;

    List< AutoPtr<WindowState> >::Iterator it;
    for (it = windows.Begin(); it != windows.End(); ++it) {
        AutoPtr<WindowState> w = *it;
        hashcode = (Int32)w.Get();
        out->WriteString(StringUtils::Int32ToHexString(hashcode));
        if (FAILED(out->Write(' '))) {
            result = FALSE;
            goto fail;
        }

        if (hashcode == focusHashcode) {
            if (FAILED(out->Write('$'))) {
                result = FALSE;
                goto fail;
            }
        }

        AutoPtr<ICharSequence> cs;
        w->mAttrs->GetTitle((ICharSequence**)&cs);
        if (FAILED(out->AppendCharSequence(cs))) {
            result = FALSE;
            goto fail;
        }
        if (FAILED(out->Write('\n'))) {
            result = FALSE;
            goto fail;
        }
    }

    if (FAILED(out->WriteString(String("DONE.\n")))) {
        result = FALSE;
        goto fail;
    }
    if (FAILED(IFlushable::Probe(out.Get())->Flush())) {
        result = FALSE;
        goto fail;
    }
    // } catch (Exception e) {
    //     result = false;
    // } finally {
fail:
    // try {
    if (FAILED(ICloseable::Probe(out.Get())->Close())) {
        result = FALSE;
    }
    // } catch (IOException e) {
    //     result = false;
    // }

    return result;
}

Boolean CWindowManagerService::ViewServerGetFocusedWindow(
    /* [in] */ ISocket* client)
{
    if (IsSystemSecure()) {
        return FALSE;
    }

    Boolean result = TRUE;

    AutoPtr<WindowState> focusedWindow = GetFocusedWindow();

    AutoPtr<IBufferedWriter> out;

    // Any uncaught exception will crash the system process
    // try {
    AutoPtr<IOutputStream> clientStream;
    if (FAILED(client->GetOutputStream((IOutputStream**)&clientStream))) {
        return FALSE;
    }
    AutoPtr<IOutputStreamWriter> osWriter;
    if (FAILED(COutputStreamWriter::New(clientStream, (IOutputStreamWriter**)&osWriter))) {
        return FALSE;
    }
    if (FAILED(CBufferedWriter::New(osWriter, 8 * 1024, (IBufferedWriter**)&out))) {
        return FALSE;
    }

    if(focusedWindow != NULL) {
        out->WriteString(StringUtils::Int32ToHexString((Int32)focusedWindow.Get()));
        if (FAILED(out->Write(' '))) {
            result = FALSE;
            goto fail;
        }
        AutoPtr<ICharSequence> cs;
        focusedWindow->mAttrs->GetTitle((ICharSequence**)&cs);
        if (FAILED(out->AppendCharSequence(cs))) {
            result = FALSE;
            goto fail;
        }
    }
    if (FAILED(out->Write('\n'))) {
        result = FALSE;
        goto fail;
    }
    if (FAILED(IFlushable::Probe(out.Get())->Flush())) {
        result = FALSE;
        goto fail;
    }
    // } catch (Exception e) {
    //     result = false;
    // } finally {
    //     if (out != null) {
    //         try {
    //             out.close();
    //         } catch (IOException e) {
    //             result = false;
    //         }
    //     }
    // }

fail:
    // try {
    if (FAILED(ICloseable::Probe(out.Get())->Close())) {
        result = FALSE;
    }

    return result;
}

Boolean CWindowManagerService::ViewServerWindowCommand(
    /* [in] */ ISocket* client,
    /* [in] */ const String& command,
    /* [in] */ const String& param)
{
    if (IsSystemSecure()) {
        return FALSE;
    }

    String parameters(param);
    Boolean success = TRUE;
    AutoPtr<IParcel> data;
    AutoPtr<IParcel> reply;

    AutoPtr<IBufferedWriter> out;

    // Any uncaught exception will crash the system process
    // try {
    // Find the hashcode of the window
    Int32 index = parameters.IndexOf(' ');
    if (index == -1) {
        index = parameters.GetLength();
    }
    String code = parameters.Substring(0, index);
    Int32 hashCode = 0;
    // int hashCode = (int) Long.parseLong(code, 16);

    // Extract the command's parameter after the window description
    if ((UInt32)index < parameters.GetLength()) {
        parameters = parameters.Substring(index + 1);
    }
    else {
        parameters = String("");
    }

    AutoPtr<WindowState> window = FindWindow(hashCode);
    if (window == NULL) {
        return FALSE;
    }

    assert(0);
    // data = Parcel.obtain();
    // data.writeInterfaceToken("android.view.IWindow");
    // data->WriteString(command);
    // data->WriteString(parameters);
    // data->WriteInt32(1);
    // ParcelFileDescriptor.fromSocket(client).writeToParcel(data, 0);

    // reply = Parcel.obtain();

    AutoPtr<IBinder> binder = IBinder::Probe(window->mClient.Get());
    // TODO: GET THE TRANSACTION CODE IN A SAFER MANNER
    // binder.transact(IBinder.FIRST_CALL_TRANSACTION, data, reply, 0);

    // reply.readException();

    Boolean result;
    client->IsOutputShutdown(&result);
    if (!result) {
        AutoPtr<IOutputStream> clientStream;
        if (FAILED(client->GetOutputStream((IOutputStream**)&clientStream))) {
            success = FALSE;
        }
        AutoPtr<IOutputStreamWriter> osWriter;
        if (FAILED(COutputStreamWriter::New(clientStream, (IOutputStreamWriter**)&osWriter))) {
            success = FALSE;
        }
        if (FAILED(CBufferedWriter::New(osWriter, (IBufferedWriter**)&out))) {
            success = FALSE;
        }
        out->WriteString(String("DONE\n"));
        IFlushable::Probe(out.Get())->Flush();
    }

    // } catch (Exception e) {
    //     Slog.w(TAG, "Could not send command " + command + " with parameters " + parameters, e);
    //     success = false;
    // } finally {
    // if (data != NULL) {
    //     data->Recycle();
    // }
    // if (reply != NULL) {
    //     reply->Recycle();
    // }
    if (out != NULL) {
        // try {
        ICloseable::Probe(out.Get())->Close();
        // } catch (IOException e) {

        // }
    //     }
    }

    return success;
}

ECode CWindowManagerService::AddDisplayContentChangeListener(
    /* [in] */ Int32 displayId,
    /* [in] */ IDisplayContentChangeListener* listener)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::RETRIEVE_WINDOW_INFO,
            String("addDisplayContentChangeListener()"))) {
        Slogger::E(TAG, "Requires RETRIEVE_WINDOW_INFO permission");
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Requires RETRIEVE_WINDOW_INFO permission");
    }
    AutoLock lock(mWindowMapLock);
    AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
    if (displayContent != NULL) {
        if (displayContent->mDisplayContentChangeListeners == NULL) {
            CRemoteCallbackList::New((IRemoteCallbackList**)&displayContent->mDisplayContentChangeListeners);
            Boolean result;
            displayContent->mDisplayContentChangeListeners->Register(
                    (IInterface*)listener, &result);
        }
    }
    return NOERROR;
}

ECode CWindowManagerService::RemoveDisplayContentChangeListener(
    /* [in] */ Int32 displayId,
    /* [in] */ IDisplayContentChangeListener* listener)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::RETRIEVE_WINDOW_INFO,
            String("removeDisplayContentChangeListener()"))) {
        Slogger::E(TAG, "Requires RETRIEVE_WINDOW_INFO permission");
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Requires RETRIEVE_WINDOW_INFO permission");
    }
    AutoLock lock(mWindowMapLock);
    AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
    if (displayContent != NULL) {
        if (displayContent->mDisplayContentChangeListeners != NULL) {
            Boolean result;
            displayContent->mDisplayContentChangeListeners->Unregister(
                    (IInterface*)listener, &result);
            Int32 count;
            displayContent->mDisplayContentChangeListeners->GetRegisteredCallbackCount(&count);
            if (count == 0) {
                displayContent->mDisplayContentChangeListeners = NULL;
            }
        }
    }
    return NOERROR;
}

void CWindowManagerService::ScheduleNotifyWindowTranstionIfNeededLocked(
    /* [in] */ WindowState* window,
    /* [in] */ Int32 transition)
{
    AutoPtr<DisplayContent> displayContent = window->mDisplayContent;
    if (displayContent->mDisplayContentChangeListeners != NULL) {
        AutoPtr<IWindowInfo> info = GetWindowInfoForWindowStateLocked(window);

        AutoPtr<IMessage> msg;
        mH->ObtainMessage(H::NOTIFY_WINDOW_TRANSITION, transition, 0,
            info, (IMessage**)&msg);
        msg->SendToTarget();
    }
}

void CWindowManagerService::HandleNotifyWindowTransition(
    /* [in] */ Int32 transition,
    /* [in] */ IWindowInfo* info)
{
    AutoPtr<IRemoteCallbackList> callbacks;
    Int32 displayId = -1;
    {
        AutoLock lock(mWindowMapLock);
        info->GetDisplayId(&displayId);
        AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
        if (displayContent == NULL) {
            return;
        }
        callbacks = displayContent->mDisplayContentChangeListeners;
        if (callbacks == NULL) {
            return;
        }
    }
    Int32 callbackCount;
    callbacks->BeginBroadcast(&callbackCount);
    // try {
    for (Int32 i = 0; i < callbackCount; i++) {
        // try {
        AutoPtr<IDisplayContentChangeListener> listener;
        ASSERT_SUCCEEDED(callbacks->GetBroadcastItem(i, (IInterface**)&listener));
        listener->OnWindowTransition(displayId, transition, info);
        // } catch (RemoteException re) {
        //     /* ignore */
        // }
    }
    callbacks->FinishBroadcast();
}

void CWindowManagerService::ScheduleNotifyRotationChangedIfNeededLocked(
    /* [in] */ DisplayContent* displayContent,
    /* [in] */ Int32 rotation)
{
    Int32 cbCount;
    if (displayContent->mDisplayContentChangeListeners != NULL
            && (displayContent->mDisplayContentChangeListeners->GetRegisteredCallbackCount(&cbCount), cbCount > 0)) {
        AutoPtr<IMessage> msg;
        mH->ObtainMessage(H::NOTIFY_ROTATION_CHANGED,
            displayContent->GetDisplayId(), rotation, (IMessage**)&msg);
        Boolean result;
        mH->SendMessage(msg, &result);
    }
}

void CWindowManagerService::HandleNotifyRotationChanged(
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 rotation)
{
    AutoPtr<IRemoteCallbackList> callbacks;
    {
        AutoLock lock(mWindowMapLock);
        AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
        if (displayContent == NULL) {
            return;
        }
        callbacks = displayContent->mDisplayContentChangeListeners;
        if (callbacks == NULL) {
            return;
        }
    }
    // try {
    Int32 watcherCount;
    callbacks->BeginBroadcast(&watcherCount);
    for (Int32 i = 0; i < watcherCount; i++) {
        // try {
        AutoPtr<IDisplayContentChangeListener> listener;
        ASSERT_SUCCEEDED(callbacks->GetBroadcastItem(i, (IInterface**)&listener));
        listener->OnRotationChanged(rotation);
        // } catch (RemoteException re) {
        //     /* ignore */
        // }
    }
    callbacks->FinishBroadcast();
}

void CWindowManagerService::ScheduleNotifyWindowLayersChangedIfNeededLocked(
    /* [in] */ DisplayContent* displayContent)
{
    Int32 cbCount;
    if (displayContent->mDisplayContentChangeListeners != NULL
            && (displayContent->mDisplayContentChangeListeners->GetRegisteredCallbackCount(&cbCount), cbCount > 0)) {
        AutoPtr<IMessage> msg;
        mH->ObtainMessage(H::NOTIFY_WINDOW_LAYERS_CHANGED,
            (IInterface*)displayContent, (IMessage**)&msg);
        msg->SendToTarget();
    }
}

void CWindowManagerService::HandleNotifyWindowLayersChanged(
    /* [in] */ DisplayContent* displayContent)
{
    AutoPtr<IRemoteCallbackList> callbacks;
    {
        AutoLock lock(mWindowMapLock);
        callbacks = displayContent->mDisplayContentChangeListeners;
        if (callbacks == NULL) {
            return;
        }
    }
    // try {
    Int32 watcherCount;
    callbacks->BeginBroadcast(&watcherCount);
    for (Int32 i = 0; i < watcherCount; i++) {
        // try {
        AutoPtr<IDisplayContentChangeListener> listener;
        ASSERT_SUCCEEDED(callbacks->GetBroadcastItem(i, (IInterface**)&listener));
        listener->OnWindowLayersChanged(displayContent->GetDisplayId());
        // } catch (RemoteException re) {
        //     /* ignore */
        // }
    }
    // } finally {
    //     callbacks.finishBroadcast();
    // }
    callbacks->FinishBroadcast();
}

void CWindowManagerService::AddWindowChangeListener(
    /* [in] */ IWindowChangeListener* listener)
{
    AutoLock lock(mWindowMapLock);
    mWindowChangeListeners.PushBack(listener);
}

void CWindowManagerService::RemoveWindowChangeListener(
    /* [in] */ IWindowChangeListener* listener)
{
    AutoLock lock(mWindowMapLock);
    mWindowChangeListeners.Remove(listener);
}

void CWindowManagerService::NotifyWindowsChanged()
{
    AutoPtr<ArrayOf<IWindowChangeListener*> > windowChangeListeners;

    {
        AutoLock lock(mWindowMapLock);
        if (mWindowChangeListeners.IsEmpty())
            return;

        windowChangeListeners = ArrayOf<IWindowChangeListener*>::Alloc(mWindowChangeListeners.GetSize());
        List< AutoPtr<IWindowChangeListener> >::Iterator it = mWindowChangeListeners.Begin();
        for (Int32 i = 0; it != mWindowChangeListeners.End(); ++it, ++i) {
            windowChangeListeners->Set(i, *it);
        }
    }

    if (windowChangeListeners != NULL) {
        for (Int32 i = 0; i < windowChangeListeners->GetLength(); ++i) {
            (*windowChangeListeners)[i]->WindowsChanged();
        }
    }
}

void CWindowManagerService::NotifyFocusChanged()
{
    AutoPtr<ArrayOf<IWindowChangeListener*> > windowChangeListeners;

    {
        AutoLock lock(mWindowMapLock);
        if (mWindowChangeListeners.IsEmpty())
            return;

        windowChangeListeners = ArrayOf<IWindowChangeListener*>::Alloc(mWindowChangeListeners.GetSize());
        List< AutoPtr<IWindowChangeListener> >::Iterator it = mWindowChangeListeners.Begin();
        for (Int32 i = 0; it != mWindowChangeListeners.End(); ++it, ++i) {
            windowChangeListeners->Set(i, *it);
        }
    }

    if (windowChangeListeners != NULL) {
        for (Int32 i = 0; i < windowChangeListeners->GetLength(); ++i) {
            (*windowChangeListeners)[i]->FocusChanged();
        }
    }
}

AutoPtr<WindowState> CWindowManagerService::FindWindow(
    /* [in] */ Int32 hashCode)
{
    if (hashCode == -1) {
        return GetFocusedWindow();
    }

    AutoLock lock(mWindowMapLock);
    HashMap<Int32, AutoPtr<DisplayContent> >::Iterator it;
    for (it = mDisplayContents.Begin(); it != mDisplayContents.End(); ++it) {
        AutoPtr<DisplayContent> content = it->mSecond;
        if (content != NULL) {
            List< AutoPtr<WindowState> >& windows = content->GetWindowList();
            if (windows.Begin() != windows.End()) {
                List< AutoPtr<WindowState> >::Iterator wit;
                for (wit = windows.Begin(); wit != windows.End(); ++wit) {
                    assert(0);
                    return NULL;
                    // if (System.identityHashCode(w) == hashCode) {
                    //     return w;
                    // }
                }
            }
        }
    }

    return NULL;
}

ECode CWindowManagerService::SendNewConfiguration()
{
    // try {
    return ((CActivityManagerService*)(mActivityManager.Get()))->UpdateConfiguration(NULL);
    // } catch (RemoteException e) {
    // }
}

AutoPtr<IConfiguration> CWindowManagerService::ComputeNewConfiguration()
{
    AutoLock lock(mWindowMapLock);
    AutoPtr<IConfiguration> config = ComputeNewConfigurationLocked();
    if (config == NULL && mWaitingForConfig) {
        // Nothing changed but we are waiting for something... stop that!
        mWaitingForConfig = FALSE;
        PerformLayoutAndPlaceSurfacesLocked();
    }
    return config;
}

AutoPtr<IConfiguration> CWindowManagerService::ComputeNewConfigurationLocked()
{
    AutoPtr<IConfiguration> config;
    ASSERT_SUCCEEDED(CConfiguration::New((IConfiguration**)&config));
    config->SetFontScale(0);
    if (!ComputeScreenConfigurationLocked(config)) {
        return NULL;
    }
    return config;
}

void CWindowManagerService::AdjustDisplaySizeRanges(
    /* [in] */ IDisplayInfo* displayInfo,
    /* [in] */ Int32 rotation,
    /* [in] */ Int32 dw,
    /* [in] */ Int32 dh)
{
    // TODO: Multidisplay: for now only use with default display.
    Int32 width;
    mPolicy->GetConfigDisplayWidth(dw, dh, rotation, &width);
    Int32 smallAppW;
    displayInfo->GetSmallestNominalAppWidth(&smallAppW);
    if (width < smallAppW) {
        displayInfo->SetSmallestNominalAppWidth(width);
    }
    Int32 largeAppW;
    displayInfo->GetLargestNominalAppWidth(&largeAppW);
    if (width > largeAppW) {
        displayInfo->SetLargestNominalAppWidth(width);
    }
    Int32 height;
    mPolicy->GetConfigDisplayHeight(dw, dh, rotation, &height);
    Int32 smallAppH;
    displayInfo->GetSmallestNominalAppHeight(&smallAppH);
    if (height < smallAppH) {
        displayInfo->SetSmallestNominalAppHeight(height);
    }
    Int32 largeAppH;
    displayInfo->GetLargestNominalAppHeight(&largeAppH);
    if (height > largeAppH) {
        displayInfo->SetLargestNominalAppHeight(height);
    }
}

Int32 CWindowManagerService::ReduceConfigLayout(
    /* [in] */ Int32 curLayout,
    /* [in] */ Int32 rotation,
    /* [in] */ Float density,
    /* [in] */ Int32 dw,
    /* [in] */ Int32 dh)
{
    // TODO: Multidisplay: for now only use with default display.
    // Get the app screen size at this rotation.
    Int32 w;
    mPolicy->GetNonDecorDisplayWidth(dw, dh, rotation, &w);
    Int32 h;
    mPolicy->GetNonDecorDisplayHeight(dw, dh, rotation, &h);

    // Compute the screen layout size class for this rotation.
    Int32 longSize = w;
    Int32 shortSize = h;
    if (longSize < shortSize) {
        Int32 tmp = longSize;
        longSize = shortSize;
        shortSize = tmp;
    }
    longSize = (Int32)(longSize / density);
    shortSize = (Int32)(shortSize / density);
    AutoPtr<IConfigurationHelper> helper;
    CConfigurationHelper::AcquireSingleton((IConfigurationHelper**)&helper);
    Int32 result;
    helper->ReduceScreenLayout(curLayout, longSize, shortSize, &result);
    return result;
}

void CWindowManagerService::ComputeSizeRangesAndScreenLayout(
    /* [in] */ IDisplayInfo* displayInfo,
    /* [in] */ Boolean rotated,
    /* [in] */ Int32 dw,
    /* [in] */ Int32 dh,
    /* [in] */ Float density,
    /* [in] */ IConfiguration* outConfig)
{
    // TODO: Multidisplay: for now only use with default display.

    // We need to determine the smallest width that will occur under normal
    // operation.  To this, start with the base screen size and compute the
    // width under the different possible rotations.  We need to un-rotate
    // the current screen dimensions before doing this.
    Int32 unrotDw, unrotDh;
    if (rotated) {
        unrotDw = dh;
        unrotDh = dw;
    }
    else {
        unrotDw = dw;
        unrotDh = dh;
    }
    displayInfo->SetSmallestNominalAppWidth(1<<30);
    displayInfo->SetSmallestNominalAppHeight(1<<30);
    displayInfo->SetLargestNominalAppWidth(0);
    displayInfo->SetLargestNominalAppHeight(0);
    AdjustDisplaySizeRanges(displayInfo, ISurface::ROTATION_0, unrotDw, unrotDh);
    AdjustDisplaySizeRanges(displayInfo, ISurface::ROTATION_90, unrotDh, unrotDw);
    AdjustDisplaySizeRanges(displayInfo, ISurface::ROTATION_180, unrotDw, unrotDh);
    AdjustDisplaySizeRanges(displayInfo, ISurface::ROTATION_270, unrotDh, unrotDw);
    AutoPtr<IConfigurationHelper> helper;
    CConfigurationHelper::AcquireSingleton((IConfigurationHelper**)&helper);
    Int32 layout;
    outConfig->GetScreenLayout(&layout);
    Int32 sl;
    helper->ResetScreenLayout(layout, &sl);
    sl = ReduceConfigLayout(sl, ISurface::ROTATION_0, density, unrotDw, unrotDh);
    sl = ReduceConfigLayout(sl, ISurface::ROTATION_90, density, unrotDh, unrotDw);
    sl = ReduceConfigLayout(sl, ISurface::ROTATION_180, density, unrotDw, unrotDh);
    sl = ReduceConfigLayout(sl, ISurface::ROTATION_270, density, unrotDh, unrotDw);
    Int32 w;
    displayInfo->GetSmallestNominalAppWidth(&w);
    outConfig->SetSmallestScreenWidthDp((Int32)(w / density));
    outConfig->SetScreenLayout(sl);
}

Int32 CWindowManagerService::ReduceCompatConfigWidthSize(
    /* [in] */ Int32 curSize,
    /* [in] */ Int32 rotation,
    /* [in] */ IDisplayMetrics* dm,
    /* [in] */ Int32 dw,
    /* [in] */ Int32 dh)
{
    // TODO: Multidisplay: for now only use with default display.
    assert(dm != NULL);
    Int32 width;
    mPolicy->GetNonDecorDisplayWidth(dw, dh, rotation, &width);
    dm->SetNoncompatWidthPixels(width);
    Int32 height;
    mPolicy->GetNonDecorDisplayHeight(dw, dh, rotation, &height);
    dm->SetNoncompatHeightPixels(height);
    AutoPtr<ICompatibilityInfoHelper> helper;
    CCompatibilityInfoHelper::AcquireSingleton((ICompatibilityInfoHelper**)&helper);
    Float scale;
    helper->ComputeCompatibleScaling(dm, NULL, &scale);
    Float density;
    dm->GetDensity(&density);
    Int32 size = (Int32)(((width / scale) / density) + 0.5);
    if (curSize == 0 || size < curSize) {
        curSize = size;
    }
    return curSize;
}

Int32 CWindowManagerService::ComputeCompatSmallestWidth(
    /* [in] */ Boolean rotated,
    /* [in] */ IDisplayMetrics* dm,
    /* [in] */ Int32 dw,
    /* [in] */ Int32 dh)
{
    // TODO: Multidisplay: for now only use with default display.
    mTmpDisplayMetrics->SetTo(dm);
    AutoPtr<IDisplayMetrics> tmpDm = mTmpDisplayMetrics;
    Int32 unrotDw, unrotDh;
    if (rotated) {
        unrotDw = dh;
        unrotDh = dw;
    }
    else {
        unrotDw = dw;
        unrotDh = dh;
    }
    Int32 sw = ReduceCompatConfigWidthSize(0, ISurface::ROTATION_0, tmpDm, unrotDw, unrotDh);
    sw = ReduceCompatConfigWidthSize(sw, ISurface::ROTATION_90, tmpDm, unrotDh, unrotDw);
    sw = ReduceCompatConfigWidthSize(sw, ISurface::ROTATION_180, tmpDm, unrotDw, unrotDh);
    sw = ReduceCompatConfigWidthSize(sw, ISurface::ROTATION_270, tmpDm, unrotDh, unrotDw);
    return sw;
}

Boolean CWindowManagerService::ComputeScreenConfigurationLocked(
    /* [in] */ IConfiguration* config)
{
    if (!mDisplayReady) {
        return FALSE;
    }

    // TODO(multidisplay): For now, apply Configuration to main screen only.
    AutoPtr<DisplayContent> displayContent = GetDefaultDisplayContentLocked();

    // Use the effective "visual" dimensions based on current rotation
    Boolean rotated = (mRotation == ISurface::ROTATION_90
            || mRotation == ISurface::ROTATION_270);
    Int32 realdw = rotated ?
            displayContent->mBaseDisplayHeight : displayContent->mBaseDisplayWidth;
    Int32 realdh = rotated ?
            displayContent->mBaseDisplayWidth : displayContent->mBaseDisplayHeight;
    Int32 dw = realdw;
    Int32 dh = realdh;

    if (mAltOrientation) {
        if (realdw > realdh) {
            // Turn landscape into portrait.
            Int32 maxw = (Int32)(realdh / 1.3);
            if (maxw < realdw) {
                dw = maxw;
            }
        }
        else {
            // Turn portrait into landscape.
            Int32 maxh = (Int32)(realdw / 1.3);
            if (maxh < realdh) {
                dh = maxh;
            }
        }
    }

    if (config != NULL) {
        config->SetOrientation((dw <= dh) ? IConfiguration::ORIENTATION_PORTRAIT :
                IConfiguration::ORIENTATION_LANDSCAPE);
    }

    // Update application display metrics.
    Int32 appWidth;
    mPolicy->GetNonDecorDisplayWidth(dw, dh, mRotation, &appWidth);
    Int32 appHeight;
    mPolicy->GetNonDecorDisplayHeight(dw, dh, mRotation, &appHeight);
    AutoPtr<IDisplayInfo> displayInfo = displayContent->GetDisplayInfo();
    {
        AutoLock lock(displayContent->mDisplaySizeLock);
        displayInfo->SetRotation(mRotation);
        displayInfo->SetLogicalWidth(dw);
        displayInfo->SetLogicalHeight(dh);
        displayInfo->SetLogicalDensityDpi(displayContent->mBaseDisplayDensity);
        displayInfo->SetAppWidth(appWidth);
        displayInfo->SetAppHeight(appHeight);
        displayInfo->GetLogicalMetrics(mRealDisplayMetrics, NULL);
        displayInfo->GetAppMetrics(mDisplayMetrics, NULL);
        mDisplayManagerService->SetDisplayInfoOverrideFromWindowManager(
            displayContent->GetDisplayId(), displayInfo);

        mAnimator->SetDisplayDimensions(dw, dh, appWidth, appHeight);

    }
    // if (false) {
    //     Slog.i(TAG, "Set app display size: " + appWidth + " x " + appHeight);
    // }

    AutoPtr<IDisplayMetrics> dm = mDisplayMetrics;
    AutoPtr<ICompatibilityInfoHelper> helper;
    CCompatibilityInfoHelper::AcquireSingleton((ICompatibilityInfoHelper**)&helper);
    helper->ComputeCompatibleScaling(dm,
            mCompatDisplayMetrics, &mCompatibleScreenScale);

    if (config != NULL) {
        Int32 disW, disH;
        mPolicy->GetConfigDisplayWidth(dw, dh, mRotation, &disW);
        mPolicy->GetConfigDisplayHeight(dw, dh, mRotation, &disH);
        Float density;
        dm->GetDensity(&density);
        config->SetScreenWidthDp((Int32)(disW / density));
        config->SetScreenHeightDp((Int32)(disH / density));
        ComputeSizeRangesAndScreenLayout(displayInfo, rotated, dw, dh, density, config);

        config->SetCompatScreenWidthDp((Int32)((disW / density) / mCompatibleScreenScale));
        config->SetCompatScreenHeightDp((Int32)((disH / density) / mCompatibleScreenScale));
        config->SetCompatSmallestScreenWidthDp(
                ComputeCompatSmallestWidth(rotated, dm, dw, dh));
        config->SetDensityDpi(displayContent->mBaseDisplayDensity);

        // Update the configuration based on available input devices, lid switch,
        // and platform configuration.
        config->SetTouchscreen(IConfiguration::TOUCHSCREEN_NOTOUCH);
        config->SetKeyboard(IConfiguration::KEYBOARD_NOKEYS);
        config->SetNavigation(IConfiguration::NAVIGATION_NONAV);

        Int32 keyboardPresence = 0;
        Int32 navigationPresence = 0;
        AutoPtr< ArrayOf<IInputDevice*> > devices = mInputManager->GetInputDevices();
        Int32 len = devices->GetLength();
        for (Int32 i = 0; i < len; i++) {
            AutoPtr<IInputDevice> device = (*devices)[i];
            Boolean isVirtual;
            device->IsVirtual(&isVirtual);
            if (!isVirtual) {
                Int32 sources;
                device->GetSources(&sources);
                Boolean isExternal;
                device->IsExternal(&isExternal);
                Int32 presenceFlag =  isExternal?
                        IWindowManagerPolicy::PRESENCE_EXTERNAL :
                                IWindowManagerPolicy::PRESENCE_INTERNAL;

                if (mIsTouchDevice) {
                    if ((sources & IInputDevice::SOURCE_TOUCHSCREEN) ==
                            IInputDevice::SOURCE_TOUCHSCREEN) {
                        config->SetTouchscreen(IConfiguration::TOUCHSCREEN_FINGER);
                    }
                }
                else {
                    config->SetTouchscreen(IConfiguration::TOUCHSCREEN_NOTOUCH);
                }

                Int32 navigation;
                config->GetNavigation(&navigation);
                if ((sources & IInputDevice::SOURCE_TRACKBALL)
                            == IInputDevice::SOURCE_TRACKBALL) {
                    config->SetNavigation(IConfiguration::NAVIGATION_TRACKBALL);
                    navigationPresence |= presenceFlag;
                }
                else if ((sources & IInputDevice::SOURCE_DPAD) == IInputDevice::SOURCE_DPAD
                        && navigation == IConfiguration::NAVIGATION_NONAV) {
                    config->SetNavigation(IConfiguration::NAVIGATION_DPAD);
                    navigationPresence |= presenceFlag;
                }

                Int32 type;
                device->GetKeyboardType(&type);
                if (type == IInputDevice::KEYBOARD_TYPE_ALPHABETIC) {
                    config->SetKeyboard(IConfiguration::KEYBOARD_QWERTY);
                    keyboardPresence |= presenceFlag;
                }
            }
        }

        // Determine whether a hard keyboard is available and enabled.
        Int32 keyboard;
        config->GetKeyboard(&keyboard);
        Boolean hardKeyboardAvailable = keyboard != IConfiguration::KEYBOARD_NOKEYS;
        if (hardKeyboardAvailable != mHardKeyboardAvailable) {
            mHardKeyboardAvailable = hardKeyboardAvailable;

            // Log.d(TAG,"--------------ro.sw.hidesoftkbwhenhardkbin");
            AutoPtr<ISystemProperties> sysProp;
            CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
            String hidesoftkbwhenhardkbin;
            sysProp->Get(String("ro.sw.hidesoftkbwhenhardkbin"), &hidesoftkbwhenhardkbin);
            if (hidesoftkbwhenhardkbin.Equals("1")) {
                mHardKeyboardEnabled = hardKeyboardAvailable;
            }
            else {
                mHardKeyboardEnabled = FALSE;
            }

            mH->RemoveMessages(H::REPORT_HARD_KEYBOARD_STATUS_CHANGE);
            Boolean result;
            mH->SendEmptyMessage(H::REPORT_HARD_KEYBOARD_STATUS_CHANGE, &result);
        }
        if (!mHardKeyboardEnabled) {
            keyboard = IConfiguration::KEYBOARD_NOKEYS;
        }

        // Let the policy update hidden states.
        config->SetKeyboardHidden(IConfiguration::KEYBOARDHIDDEN_NO);
        config->SetHardKeyboardHidden(IConfiguration::HARDKEYBOARDHIDDEN_NO);
        config->SetNavigationHidden(IConfiguration::NAVIGATIONHIDDEN_NO);
        mPolicy->AdjustConfigurationLw(config, keyboardPresence, navigationPresence);
    }

    return TRUE;
}

Boolean CWindowManagerService::IsHardKeyboardAvailable()
{
    AutoLock lock(mWindowMapLock);
    return mHardKeyboardAvailable;
}

Boolean CWindowManagerService::IsHardKeyboardEnabled()
{
    AutoLock lock(mWindowMapLock);
    return mHardKeyboardEnabled;
}

void CWindowManagerService::SetHardKeyboardEnabled(
    /* [in] */ Boolean enabled)
{
    AutoLock lock(mWindowMapLock);
    if (mHardKeyboardEnabled != enabled) {
        mHardKeyboardEnabled = enabled;
        Boolean result;
        mH->SendEmptyMessage(H::SEND_NEW_CONFIGURATION, &result);
    }
}

void CWindowManagerService::SetOnHardKeyboardStatusChangeListener(
    /* [in] */ IOnHardKeyboardStatusChangeListener* listener)
{
    AutoLock lock(mWindowMapLock);
    mHardKeyboardStatusChangeListener = listener;
}

void CWindowManagerService::NotifyHardKeyboardStatusChange()
{
    Boolean available, enabled;
    AutoPtr<IOnHardKeyboardStatusChangeListener> listener;
    {
        AutoLock lock(mWindowMapLock);
        listener = mHardKeyboardStatusChangeListener;
        available = mHardKeyboardAvailable;
        enabled = mHardKeyboardEnabled;
    }

    if (listener != NULL) {
        listener->OnHardKeyboardStatusChange(available, enabled);
    }
}

// -------------------------------------------------------------
// Drag and drop
// -------------------------------------------------------------

AutoPtr<IBinder> CWindowManagerService::PrepareDragSurface(
    /* [in] */ IIWindow* window,
    /* [in] */ ISurfaceSession* session,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ ISurface* outSurface)
{
    // if (DEBUG_DRAG) {
    //     Slog.d(TAG, "prepare drag surface: w=" + width + " h=" + height
    //             + " flags=" + Integer.toHexString(flags) + " win=" + window
    //             + " asbinder=" + window.asBinder());
    // }

    // Int32 callerPid = Binder::GetCallingPid();
    Int64 origId = Binder::ClearCallingIdentity();
    AutoPtr<IBinder> token;

    // try {
    AutoLock lock(mWindowMapLock);
    // try {
    if (mDragState == NULL) {
        // TODO(multi-display): support other displays
        AutoPtr<DisplayContent> displayContent = GetDefaultDisplayContentLocked();
        AutoPtr<IDisplay> display = displayContent->GetDisplay();
        AutoPtr<ISurface> surface;
        ECode ec = CSurface::New(session, String("drag surface"), width, height,
                IPixelFormat::TRANSLUCENT, ISurface::HIDDEN, (ISurface**)&surface);
        if (FAILED(ec)) {
            Slogger::E(TAG, "Can't allocate drag surface w=%d h=%d 0x%08x",
                    width, height, ec);
            mDragState->Reset();
            mDragState = NULL;
            Binder::RestoreCallingIdentity(origId);
            return token;
        }
        Int32 stack;
        display->GetLayerStack(&stack);
        surface->SetLayerStack(stack);
        // if (SHOW_TRANSACTIONS) Slog.i(TAG, "  DRAG "
        //         + surface + ": CREATE");
        outSurface->CopyFrom(surface);
        AutoPtr<IBinder> winBinder = IBinder::Probe(window);
        CBinder::New((IBinder**)&token);
        mDragState = new DragState(this, token, surface, /*flags*/ 0, winBinder);
        token = NULL;
        CBinder::New((IBinder**)&token);
        mDragState->mToken = token;

        // 5 second timeout for this window to actually begin the drag
        AutoPtr<IInterface> obj = winBinder ? winBinder->Probe(EIID_IInterface) : NULL;
        mH->RemoveMessages(H::DRAG_START_TIMEOUT, obj);
        AutoPtr<IMessage> msg;
        mH->ObtainMessage(H::DRAG_START_TIMEOUT, obj, (IMessage**)&msg);
        Boolean result;
        mH->SendMessageDelayed(msg, 5000, &result);
    }
    else {
        Slogger::W(TAG, "Drag already in progress");
    }
    // } catch (Surface.OutOfResourcesException e) {
    //     Slog.e(TAG, "Can't allocate drag surface w=" + width + " h=" + height, e);
    //     if (mDragState != null) {
    //         mDragState.reset();
    //         mDragState = null;
    //     }
    // }
    // } finally {
    //     Binder::RestoreCallingIdentity(origId);
    // }
    Binder::RestoreCallingIdentity(origId);

    return token;
}

// -------------------------------------------------------------
// Input Events and Focus Management
// -------------------------------------------------------------

ECode CWindowManagerService::PauseKeyDispatching(
    /* [in] */ IBinder* token)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("PauseKeyDispatching()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoLock lock(mWindowMapLock);
    AutoPtr<WindowToken> wtoken;
    HashMap<AutoPtr<IBinder>, AutoPtr<WindowToken> >::Iterator it
            = mTokenMap.Find(token);
    if (it != mTokenMap.End()) {
        wtoken = it->mSecond;
    }

    if (wtoken != NULL) {
        mInputMonitor->PauseDispatchingLw(wtoken);
    }

    return NOERROR;
}

ECode CWindowManagerService::ResumeKeyDispatching(
    /* [in] */ IBinder* token)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("ResumeKeyDispatching()"))) {
        // throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoLock lock(mWindowMapLock);
    AutoPtr<WindowToken> wtoken;
    HashMap<AutoPtr<IBinder>, AutoPtr<WindowToken> >::Iterator it
            = mTokenMap.Find(token);
    if (it != mTokenMap.End()) {
        wtoken = it->mSecond;
    }

    if (wtoken != NULL) {
        mInputMonitor->ResumeDispatchingLw(wtoken);
    }

    return NOERROR;
}

ECode CWindowManagerService::SetEventDispatching(
    /* [in] */ Boolean enabled)
{
    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::MANAGE_APP_TOKENS,
            String("ResumeKeyDispatching()"))) {
        //throw new SecurityException("Requires MANAGE_APP_TOKENS permission");
        Slogger::E(TAG, "Requires MANAGE_APP_TOKENS permission");
        return E_SECURITY_EXCEPTION;
    }

    AutoLock lock(mWindowMapLock);
    mEventDispatchingEnabled = enabled;
    if (mDisplayEnabled) {
        mInputMonitor->SetEventDispatchingLw(enabled);
    }
    SendScreenStatusToClientsLocked();

    return NOERROR;
}

ECode CWindowManagerService::GetFocusedWindowToken(
    /* [out] */ IBinder** token)
{
    VALIDATE_NOT_NULL(token);

    if (!CheckCallingPermission(
            Elastos::Droid::Manifest::Permission::RETRIEVE_WINDOW_INFO,
            String("getFocusedWindowToken()"))) {
        Slogger::E(TAG, "Requires RETRIEVE_WINDOW_INFO permission.");
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Requires RETRIEVE_WINDOW_INFO permission.");
    }
    AutoLock lock(mWindowMapLock);
    AutoPtr<WindowState> windowState = GetFocusedWindowLocked();
    if (windowState != NULL) {
        AutoPtr<IBinder> b = IBinder::Probe(windowState->mClient.Get());
        *token = b;
        REFCOUNT_ADD(*token);
    }
    *token = NULL;
   return NOERROR;
}

AutoPtr<WindowState> CWindowManagerService::GetFocusedWindow()
{
    AutoLock lock(mWindowMapLock);
    return GetFocusedWindowLocked();
}

AutoPtr<WindowState> CWindowManagerService::GetFocusedWindowLocked()
{
    return mCurrentFocus;
}

Boolean CWindowManagerService::DetectSafeMode()
{
    if (!mInputMonitor->WaitForInputDevicesReady(
            INPUT_DEVICES_READY_FOR_SAFE_MODE_DETECTION_TIMEOUT_MILLIS)) {
        Slogger::W(TAG, "Devices still not ready after waiting %d milliseconds before attempting to detect safe mode."
               , INPUT_DEVICES_READY_FOR_SAFE_MODE_DETECTION_TIMEOUT_MILLIS);
    }

    Int32 menuState = mInputManager->GetKeyCodeState(
        -1, IInputDevice::SOURCE_ANY, IKeyEvent::KEYCODE_MENU);
    Int32 sState = mInputManager->GetKeyCodeState(
        -1, IInputDevice::SOURCE_ANY, IKeyEvent::KEYCODE_S);
    Int32 dpadState = mInputManager->GetKeyCodeState(
        -1, IInputDevice::SOURCE_DPAD, IKeyEvent::KEYCODE_DPAD_CENTER);
    Int32 trackballState = mInputManager->GetScanCodeState(
        -1, IInputDevice::SOURCE_TRACKBALL, CInputManagerService::_BTN_MOUSE);
    Int32 volumeDownState = mInputManager->GetKeyCodeState(
        -1, IInputDevice::SOURCE_ANY, IKeyEvent::KEYCODE_VOLUME_DOWN);
    mSafeMode = menuState > 0 || sState > 0 || dpadState > 0 || trackballState > 0
            || volumeDownState > 0;

    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    Int32 value;
    if (sysProp->GetInt32(ShutdownThread::REBOOT_SAFEMODE_PROPERTY, 0, &value), value != 0) {
        mSafeMode = TRUE;
        sysProp->Set(ShutdownThread::REBOOT_SAFEMODE_PROPERTY, String(""));
    }

    if (mSafeMode) {
        Slogger::I(TAG, "SAFE MODE ENABLED (menu=%d s=%d dpad=%d trackball=%d)",
                menuState, sState, dpadState, trackballState);
    }
    else {
        Slogger::I(TAG, "SAFE MODE not enabled");
    }
    mPolicy->SetSafeMode(mSafeMode);
    return mSafeMode;
}

void CWindowManagerService::DisplayReady()
{
    DisplayReady(IDisplay::DEFAULT_DISPLAY);

    {
        AutoLock lock(mWindowMapLock);
        AutoPtr<DisplayContent> displayContent = GetDefaultDisplayContentLocked();
        ReadForcedDisplaySizeAndDensityLocked(displayContent);

        mDisplayReady = TRUE;
        AutoPtr<IPackageManager> capsuleManager;
        mContext->GetPackageManager((IPackageManager**)&capsuleManager);
        capsuleManager->HasSystemFeature(IPackageManager::FEATURE_TOUCHSCREEN,
                &mIsTouchDevice);

        AutoPtr<IDisplayInfo> displayInfo = GetDefaultDisplayInfoLocked();
        Int32 logicalW, logicalH, appW, appH;
        displayInfo->GetLogicalWidth(&logicalW);
        displayInfo->GetLogicalHeight(&logicalH);
        displayInfo->GetAppWidth(&appW);
        displayInfo->GetAppHeight(&appH);
        mAnimator->SetDisplayDimensions(logicalW, logicalH, appW, appH);

        mPolicy->SetInitialDisplaySize(displayContent->GetDisplay(),
                displayContent->mInitialDisplayWidth,
                displayContent->mInitialDisplayHeight,
                displayContent->mInitialDisplayDensity);
    }

    // try {
    mActivityManager->UpdateConfiguration(NULL);
    // } catch (RemoteException e) {
    // }
}

void CWindowManagerService::DisplayReady(
    /* [in] */ Int32 displayId)
{
    AutoLock lock(mWindowMapLock);
    AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
    if (displayContent != NULL) {
        mAnimator->AddDisplayLocked(displayId);
        AutoLock lock(displayContent->mDisplaySizeLock);
        // Bootstrap the default logical display from the display manager.
        AutoPtr<IDisplayInfo> displayInfo = displayContent->GetDisplayInfo();
        AutoPtr<IDisplayInfo> newDisplayInfo;
        mDisplayManagerService->GetDisplayInfo(displayId, (IDisplayInfo**)&newDisplayInfo);
        if (newDisplayInfo != NULL) {
            displayInfo->CopyFrom(newDisplayInfo);
        }
        displayInfo->GetLogicalWidth(&displayContent->mInitialDisplayWidth);
        displayInfo->GetLogicalHeight(&displayContent->mInitialDisplayHeight);
        displayInfo->GetLogicalDensityDpi(&displayContent->mInitialDisplayDensity);
        displayContent->mBaseDisplayWidth = displayContent->mInitialDisplayWidth;
        displayContent->mBaseDisplayHeight = displayContent->mInitialDisplayHeight;
        displayContent->mBaseDisplayDensity = displayContent->mInitialDisplayDensity;
    }
}

ECode CWindowManagerService::SystemReady()
{
    return mPolicy->SystemReady();
}

void CWindowManagerService::SendScreenStatusToClientsLocked()
{
    Boolean on;
    mPowerManager->IsScreenOn(&on);
    HashMap<Int32, AutoPtr<DisplayContent> >::Iterator it;
    for (it = mDisplayContents.Begin(); it != mDisplayContents.End(); ++it) {
        AutoPtr<DisplayContent> content = it->mSecond;
        if (content != NULL) {
            List< AutoPtr<WindowState> >& windows = content->GetWindowList();
            if (windows.Begin() != windows.End()) {
                List< AutoPtr<WindowState> >::Iterator wit;
                for (wit = windows.Begin(); wit != windows.End(); ++wit) {
                    // try {
                    (*wit)->mClient->DispatchScreenState(on);
                    // } catch (RemoteException e) {
                    //     // Ignored
                    // }
                }
            }
        }
    }
}

// -------------------------------------------------------------
// Async Handler
// -------------------------------------------------------------
const Int32 CWindowManagerService::H::REPORT_FOCUS_CHANGE = 2;
const Int32 CWindowManagerService::H::REPORT_LOSING_FOCUS = 3;
const Int32 CWindowManagerService::H::DO_TRAVERSAL = 4;
const Int32 CWindowManagerService::H::ADD_STARTING = 5;
const Int32 CWindowManagerService::H::REMOVE_STARTING = 6;
const Int32 CWindowManagerService::H::FINISHED_STARTING = 7;
const Int32 CWindowManagerService::H::REPORT_APPLICATION_TOKEN_WINDOWS = 8;
const Int32 CWindowManagerService::H::REPORT_APPLICATION_TOKEN_DRAWN = 9;
const Int32 CWindowManagerService::H::WINDOW_FREEZE_TIMEOUT = 11;

const Int32 CWindowManagerService::H::APP_TRANSITION_TIMEOUT = 13;
const Int32 CWindowManagerService::H::PERSIST_ANIMATION_SCALE = 14;
const Int32 CWindowManagerService::H::FORCE_GC = 15;
const Int32 CWindowManagerService::H::ENABLE_SCREEN = 16;
const Int32 CWindowManagerService::H::APP_FREEZE_TIMEOUT = 17;
const Int32 CWindowManagerService::H::SEND_NEW_CONFIGURATION = 18;
const Int32 CWindowManagerService::H::REPORT_WINDOWS_CHANGE = 19;
const Int32 CWindowManagerService::H::DRAG_START_TIMEOUT = 20;
const Int32 CWindowManagerService::H::DRAG_END_TIMEOUT = 21;
const Int32 CWindowManagerService::H::REPORT_HARD_KEYBOARD_STATUS_CHANGE = 22;
const Int32 CWindowManagerService::H::BOOT_TIMEOUT = 23;
const Int32 CWindowManagerService::H::WAITING_FOR_DRAWN_TIMEOUT = 24;
const Int32 CWindowManagerService::H::UPDATE_ANIM_PARAMETERS = 25;
const Int32 CWindowManagerService::H::SHOW_STRICT_MODE_VIOLATION = 26;
const Int32 CWindowManagerService::H::DO_ANIMATION_CALLBACK = 27;
const Int32 CWindowManagerService::H::NOTIFY_ROTATION_CHANGED = 28;
const Int32 CWindowManagerService::H::NOTIFY_WINDOW_TRANSITION = 29;
const Int32 CWindowManagerService::H::NOTIFY_RECTANGLE_ON_SCREEN_REQUESTED = 30;
const Int32 CWindowManagerService::H::NOTIFY_WINDOW_LAYERS_CHANGED = 31;

const Int32 CWindowManagerService::H::DO_DISPLAY_ADDED = 32;
const Int32 CWindowManagerService::H::DO_DISPLAY_REMOVED = 33;
const Int32 CWindowManagerService::H::DO_DISPLAY_CHANGED = 34;

const Int32 CWindowManagerService::H::CLIENT_FREEZE_TIMEOUT = 35;

const Int32 CWindowManagerService::H::ANIMATOR_WHAT_OFFSET = 100000;
const Int32 CWindowManagerService::H::SET_TRANSPARENT_REGION = ANIMATOR_WHAT_OFFSET + 1;
const Int32 CWindowManagerService::H::CLEAR_PENDING_ACTIONS = ANIMATOR_WHAT_OFFSET + 2;

ECode CWindowManagerService::H::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);

    if (CWindowManagerService::DEBUG_WINDOW_TRACE) {
        Slogger::V(CWindowManagerService::TAG, "handleMessage: entry what=%d", what);
    }

    switch (what) {
        case REPORT_FOCUS_CHANGE: {
            mHost->HandleReportFocusChange();
            break;
        }
        case REPORT_LOSING_FOCUS: {
            mHost->HandleReportLosingFocus();
            break;
        }
        case DO_TRAVERSAL: {
            mHost->HandleDoTraversal();
            break;
        }
        case ADD_STARTING: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AppWindowToken* wtoken = (AppWindowToken*)obj.Get();
            mHost->HandleAddStarting(wtoken);
            break;
        }
        case REMOVE_STARTING: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AppWindowToken* wtoken = (AppWindowToken*)obj.Get();
            mHost->HandleRemoveStarting(wtoken);
            break;
        }
        case FINISHED_STARTING: {
            mHost->HandleFinishedStarting();
            break;
        }
        case REPORT_APPLICATION_TOKEN_DRAWN: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AppWindowToken* wtoken = (AppWindowToken*)obj.Get();
            mHost->HandleReportApplicationTokenDrawn(wtoken);
            break;
        }
        case REPORT_APPLICATION_TOKEN_WINDOWS: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AppWindowToken* wtoken = (AppWindowToken*)obj.Get();
            Boolean nowVisible = arg1 != 0;
            Boolean nowGone = arg2 != 0;
            mHost->HandleReportApplicationTokenWindows(nowVisible, nowGone, wtoken);
            break;
        }
        case WINDOW_FREEZE_TIMEOUT: {
            mHost->HandleWindowFreezeTimeout();
            break;
        }
        case APP_TRANSITION_TIMEOUT: {
            mHost->HandleAppTransitionTimeout();
            break;
        }
        case PERSIST_ANIMATION_SCALE: {
            mHost->HandlePersistAnimationScale();
            break;
        }
        case FORCE_GC: {
            mHost->HandleForceGc();
            break;
        }
        case ENABLE_SCREEN: {
            mHost->PerformEnableScreen();
            break;
        }
        case APP_FREEZE_TIMEOUT: {
            mHost->HandleAppFreezeTimeout();
            break;
        }
        case CLIENT_FREEZE_TIMEOUT: {
            mHost->HandleClientFreezeTimeout();
            break;
        }
        case SEND_NEW_CONFIGURATION: {
            RemoveMessages(SEND_NEW_CONFIGURATION);
            mHost->SendNewConfiguration();
            break;
        }
        case REPORT_WINDOWS_CHANGE: {
            mHost->HandleReportWindowsChange();
            break;
        }
        case DRAG_START_TIMEOUT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<IBinder> binder = IBinder::Probe(obj);
            mHost->HandleDragStartTimeout(binder);
            break;
            break;
        }
        case DRAG_END_TIMEOUT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<IBinder> binder = IBinder::Probe(obj);
            mHost->HandleDragEndTimeout(binder);
            break;
        }
        case REPORT_HARD_KEYBOARD_STATUS_CHANGE: {
            mHost->NotifyHardKeyboardStatusChange();
            break;
        }
        case BOOT_TIMEOUT: {
            mHost->PerformBootTimeout();
            break;
        }
        case WAITING_FOR_DRAWN_TIMEOUT: {
            AutoPtr<IInterface> argsObj;
            msg->GetObj((IInterface**)&argsObj);
            SomeArgs* args = (SomeArgs*)argsObj.Get();
            WindowState* windowState = (WindowState*)args->mArg1.Get();
            assert(windowState != NULL);
            IRemoteCallback* remoteCallback = IRemoteCallback::Probe(args->mArg2);
            assert(remoteCallback != NULL);
            mHost->HandleWaitingForDrawnTimeout(windowState, remoteCallback);
            args->Recycle();
            break;
        }
        case UPDATE_ANIM_PARAMETERS: {
            mHost->HandleUpdateAnimParameters();
            break;
        }
        case SHOW_STRICT_MODE_VIOLATION: {
            mHost->ShowStrictModeViolation(arg1, arg2);
            break;
        }
        case SET_TRANSPARENT_REGION: {
            AutoPtr<IInterface> argsObj;
            msg->GetObj((IInterface**)&argsObj);
            SomeArgs* args = (SomeArgs*)argsObj.Get();
            WindowStateAnimator* winAnimator = (WindowStateAnimator*)args->mArg1.Get();
            IRegion* region = IRegion::Probe(args->mArg2);
            winAnimator->SetTransparentRegionHint(region);
            args->Recycle();
            break;
        }
        case CLEAR_PENDING_ACTIONS: {
            mHost->mAnimator->ClearPendingActions();
            break;
        }
        case DO_ANIMATION_CALLBACK: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<IRemoteCallback> callback = IRemoteCallback::Probe(obj);
            callback->SendResult(NULL);
            break;
        }
        case NOTIFY_ROTATION_CHANGED: {
            mHost->HandleNotifyRotationChanged(arg1, arg2);
            break;
        }
        case NOTIFY_WINDOW_TRANSITION: {
            Int32 transition = arg1;
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<IWindowInfo> info = IWindowInfo::Probe(obj);
            mHost->HandleNotifyWindowTransition(transition, info);
            break;
        }
        case NOTIFY_RECTANGLE_ON_SCREEN_REQUESTED: {
            Int32 displayId = arg1;
            Boolean immediate = (arg2 == 1);
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<IRect> rect = IRect::Probe(obj);
            mHost->HandleNotifyRectangleOnScreenRequested(displayId, rect, immediate);
            break;
        }
        case NOTIFY_WINDOW_LAYERS_CHANGED: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            DisplayContent* displayContent = (DisplayContent*)obj.Get();
            mHost->HandleNotifyWindowLayersChanged(displayContent);
            break;
        }
        case DO_DISPLAY_ADDED: {
            mHost->HandleDoDisplayAdded(arg1);
            break;
        }
        case DO_DISPLAY_REMOVED: {
            mHost->HandleDoDisplayRemoved(arg1);
            break;
        }
        case DO_DISPLAY_CHANGED: {
            mHost->HandleDoDisplayChanged(arg1);
            break;
        }
    }

    if (CWindowManagerService::DEBUG_WINDOW_TRACE) {
        Slogger::V(CWindowManagerService::TAG, "handleMessage: exit");
    }
    return NOERROR;
}

ECode CWindowManagerService::HandleReportFocusChange()
{
    AutoPtr<WindowState> lastFocus;
    AutoPtr<WindowState> newFocus;

    {
        AutoLock lock(mWindowMapLock);

        lastFocus = mLastFocus;
        newFocus = mCurrentFocus;
        if (lastFocus == newFocus) {
            // Focus is not changing, so nothing to do.
            return NOERROR;
        }

        mLastFocus = newFocus;
        // Slogger::I(TAG, "Focus moving from %p to %p", lastFocus.Get(), newFocus.Get());

        Boolean isDisplayed;
        if (newFocus != NULL && lastFocus != NULL
                && (newFocus->IsDisplayedLw(&isDisplayed), !isDisplayed)) {
            Slogger::I(TAG, "Delaying loss of focus...");
            mLosingFocus.PushBack(lastFocus);
            lastFocus = NULL;
        }
    }

    if (lastFocus != newFocus) {
        // System.out.println("Changing focus from " + lastFocus
        //                    + " to " + newFocus);
        if (newFocus != NULL) {
            Slogger::I(TAG, "Gaining focus: %p", newFocus.Get());
            FAIL_RETURN(newFocus->mClient->WindowFocusChanged(TRUE, mInTouchMode));
            NotifyFocusChanged();
        }

        if (lastFocus != NULL) {
            Slogger::I(TAG, "Losing focus: %p", lastFocus.Get());
            return lastFocus->mClient->WindowFocusChanged(FALSE, mInTouchMode);
        }
    }

    return NOERROR;
}

ECode CWindowManagerService::HandleReportLosingFocus()
{
    List< AutoPtr<WindowState> > losers;

    {
        AutoLock lock(mWindowMapLock);
        losers.Assign(mLosingFocus.Begin(), mLosingFocus.End());
        mLosingFocus.Clear();
    }

    List< AutoPtr<WindowState> >::Iterator it;
    for (it = losers.Begin(); it != losers.End(); ++it) {
        // try {
            //Slog.i(TAG, "Losing delayed focus: " + losers.get(i));
        return (*it)->mClient->WindowFocusChanged(FALSE, mInTouchMode);
        // } catch (RemoteException e) {
        //      // Ignore if process has died.
        // }
    }

    return NOERROR;
}

ECode CWindowManagerService::HandleDoTraversal()
{
    AutoLock lock(mWindowMapLock);
    mTraversalScheduled = FALSE;
    PerformLayoutAndPlaceSurfacesLocked();
    return NOERROR;
}

ECode CWindowManagerService::HandleAddStarting(
    /* [in] */ AppWindowToken* wtoken)
{
    AutoPtr<StartingData> sd = wtoken->mStartingData;

    if (sd == NULL) {
        // Animation has been canceled... do nothing.
        return NOERROR;
    }

    if (DEBUG_STARTING_WINDOW) {
        Slogger::V(TAG, "Add starting %p: pkg=%s", wtoken, sd->mPkg.string());
    }

    AutoPtr<IView> view;
    if (FAILED(mPolicy->AddStartingWindow(
            wtoken->mToken, sd->mPkg,
            sd->mTheme, sd->mCompatInfo, sd->mNonLocalizedLabel, sd->mLabelRes,
            sd->mIcon, sd->mWindowFlags, (IView**)&view))) {
        Slogger::W(TAG, "Exception when adding starting window");
    }

    if (view != NULL) {
        Boolean abort = FALSE;

        {
            AutoLock lock(mWindowMapLock);

            if (wtoken->mRemoved || wtoken->mStartingData == NULL) {
                // If the window was successfully added, then
                // we need to remove it.
                if (wtoken->mStartingWindow != NULL) {
                    if (DEBUG_STARTING_WINDOW) {
                        Slogger::V(TAG, "Aborted starting %p: removed=%d  startingData=%p",
                             wtoken, wtoken->mRemoved, wtoken->mStartingData.Get());
                    }
                    wtoken->mStartingWindow = NULL;
                    wtoken->mStartingData = NULL;
                    abort = TRUE;
                }
            }
            else {
                wtoken->mStartingView = view;
            }

            if (DEBUG_STARTING_WINDOW) {
                Slogger::V(TAG, "Added starting %p: startingWindow=%p, startingView=%p",
                    wtoken, wtoken->mStartingWindow.Get(), wtoken->mStartingView.Get());
            }
        }

        if (abort) {
            if (FAILED(mPolicy->RemoveStartingWindow(wtoken->mToken, view))) {
                Slogger::W(TAG, "Exception when removing starting window");
            }
        }
    }

    return NOERROR;
}

ECode CWindowManagerService::HandleRemoveStarting(
    /* [in] */ AppWindowToken* wtoken)
{
    AutoPtr<IBinder> token;
    AutoPtr<IView> view;

    {
        AutoLock lock(mWindowMapLock);

        if (DEBUG_STARTING_WINDOW) {
            Slogger::V(TAG, "Remove starting %p: startingWindow=%p, startingView=%p",
                wtoken, wtoken->mStartingWindow.Get(), wtoken->mStartingView.Get());
        }

        if (wtoken->mStartingWindow != NULL) {
            view = wtoken->mStartingView;
            token = wtoken->mToken;
            wtoken->mStartingData = NULL;
            wtoken->mStartingView = NULL;
            wtoken->mStartingWindow = NULL;
            wtoken->mStartingDisplayed = FALSE;
        }
    }

    if (view != NULL) {
        if (FAILED(mPolicy->RemoveStartingWindow(token, view))) {
            Slogger::W(TAG, "Exception when removing starting window");
        }
    }

    return NOERROR;
}

ECode CWindowManagerService::HandleFinishedStarting()
{
    AutoPtr<IBinder> token;
    AutoPtr<IView> view;
    while (TRUE) {
        {
            AutoLock lock(mWindowMapLock);

            if (mFinishedStarting.Begin() == mFinishedStarting.End()) {
                break;
            }

            AutoPtr<AppWindowToken> wtoken = mFinishedStarting.GetBack();
            mFinishedStarting.PopBack();

            if (DEBUG_STARTING_WINDOW) {
                Slogger::V(TAG, "Finished starting %p: startingWindow=%p startingView=%p",
                    wtoken.Get(), wtoken->mStartingWindow.Get(), wtoken->mStartingView.Get());
            }

            if (wtoken->mStartingWindow == NULL) {
                continue;
            }

            view = wtoken->mStartingView;
            token = wtoken->mToken;
            wtoken->mStartingData = NULL;
            wtoken->mStartingView = NULL;
            wtoken->mStartingWindow = NULL;
            wtoken->mStartingDisplayed = FALSE;
        }

        if (FAILED(mPolicy->RemoveStartingWindow(token, view))) {
            Slogger::W(TAG, "Exception when removing starting window");
        }
    }

    return NOERROR;
}

ECode CWindowManagerService::HandleReportApplicationTokenDrawn(
    /* [in] */ AppWindowToken* wtoken)
{
    // try {
    // if (DEBUG_VISIBILITY) Slog.v(
    //         TAG, "Reporting drawn in " + wtoken);
    ECode ec = wtoken->mAppToken->WindowsDrawn();
    // } catch (RemoteException ex) {
    // }
    return ec;
}

ECode CWindowManagerService::HandleReportApplicationTokenWindows(
    /* [in] */ Boolean nowVisible,
    /* [in] */ Boolean nowGone,
    /* [in] */ AppWindowToken* wtoken)
{
    // try {
    // if (DEBUG_VISIBILITY) Slog.v(
    //         TAG, "Reporting visible in " + wtoken
    //         + " visible=" + nowVisible
    //         + " gone=" + nowGone);
    ECode ec = NOERROR;
    if (nowVisible) {
        ec = wtoken->mAppToken->WindowsVisible();
    }
    else {
        ec = wtoken->mAppToken->WindowsGone();
    }
    // } catch (RemoteException ex) {
    // }
    return ec;
}

ECode CWindowManagerService::HandleWindowFreezeTimeout()
{
    // TODO(multidisplay): Can non-default displays rotate?
    AutoLock lock(mWindowMapLock);

    Slogger::W(TAG, "Window freeze timeout expired.");

    List< AutoPtr<WindowState> >& windows = GetDefaultWindowListLocked();
    List< AutoPtr<WindowState> >::ReverseIterator rit = windows.RBegin();
    for (; rit != windows.REnd(); ++rit) {
        if ((*rit)->mOrientationChanging) {
            (*rit)->mOrientationChanging = FALSE;
            Slogger::W(TAG, "Force clearing orientation change: %p", (*rit).Get());
        }
    }

    PerformLayoutAndPlaceSurfacesLocked();
    return NOERROR;
}

ECode CWindowManagerService::HandleAppTransitionTimeout()
{
    AutoLock lock(mWindowMapLock);

    if (mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET) {
        // if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
        //     "*** APP TRANSITION TIMEOUT");
        mAppTransitionReady = TRUE;
        mAppTransitionTimeout = TRUE;
        mAnimatingAppTokens.Clear();
        mAnimatingAppTokens.Assign(mAppTokens.Begin(), mAppTokens.End());
        PerformLayoutAndPlaceSurfacesLocked();
    }

    return NOERROR;
}

ECode CWindowManagerService::HandlePersistAnimationScale()
{
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    AutoPtr<ISettingsGlobal> sg;
    FAIL_RETURN(CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sg));
    Boolean result;
    sg->PutFloat(cr, ISettingsGlobal::WINDOW_ANIMATION_SCALE,
            mWindowAnimationScale, &result);
    sg->PutFloat(cr, ISettingsGlobal::TRANSITION_ANIMATION_SCALE,
            mTransitionAnimationScale, &result);
    sg->PutFloat(cr, ISettingsGlobal::ANIMATOR_DURATION_SCALE,
            mAnimatorDurationScale, &result);

    return NOERROR;
}

ECode CWindowManagerService::HandleForceGc()
{
    AutoLock lock(mWindowMapLock);
    {
        AutoLock lock(mAnimatorLock);

        // Since we're holding both mWindowMap and mAnimator we don't need to
        // hold mAnimator.mLayoutToAnim.
        if (mAnimator->mAnimating || mLayoutToAnim->mAnimationScheduled) {
            // If we are animating, don't do the gc now but
            // delay a bit so we don't interrupt the animation.
            AutoPtr<IMessage> msg;
            mH->ObtainMessage(H::FORCE_GC, (IMessage**)&msg);
            Boolean result;
            return mH->SendMessageDelayed(msg, 2000, &result);
        }
        // If we are currently rotating the display, it will
        // schedule a new message when done.
        if (mDisplayFrozen) {
            return NOERROR;
        }
    }

    //Runtime.getRuntime().gc();

    return NOERROR;
}

ECode CWindowManagerService::HandleAppFreezeTimeout()
{
    AutoLock lock(mWindowMapLock);
    {
        AutoLock lock(mAnimatorLock);

        Slogger::W(TAG, "App freeze timeout expired.");
        List< AutoPtr<AppWindowToken> >::ReverseIterator rit = mAppTokens.RBegin();
        for (; rit != mAppTokens.REnd(); ++rit) {
            if ((*rit)->mAppAnimator->mFreezingScreen) {
                Slogger::W(TAG, "Force clearing freeze: %p", (*rit).Get());
                UnsetAppFreezingScreenLocked(*rit, TRUE, TRUE);
            }
        }
    }
    return NOERROR;
}

ECode CWindowManagerService::HandleClientFreezeTimeout()
{
    AutoLock lock(mWindowMapLock);
    if (mClientFreezingScreen) {
        mClientFreezingScreen = FALSE;
        StopFreezingDisplayLocked();
    }
    return NOERROR;
}

ECode CWindowManagerService::HandleReportWindowsChange()
{
    if (mWindowsChanged) {
        {
            AutoLock lock(mWindowMapLock);
            mWindowsChanged = FALSE;
        }
        NotifyWindowsChanged();
    }

    return NOERROR;
}

ECode CWindowManagerService::HandleDragStartTimeout(
    /* [in] */ IBinder* winBinder)
{
    // if (DEBUG_DRAG) {
    //     Slog.w(TAG, "Timeout starting drag by win " + win);
    // }
    AutoLock lock(mWindowMapLock);
    // !!! TODO: ANR the app that has failed to start the drag in time
    if (mDragState != NULL) {
        mDragState->Unregister();
        mInputMonitor->UpdateInputWindowsLw(TRUE /*force*/);
        mDragState->Reset();
        mDragState = NULL;
    }
    return NOERROR;
}

ECode CWindowManagerService::HandleDragEndTimeout(
    /* [in] */ IBinder* winBinder)
{
    // if (DEBUG_DRAG) {
    //     Slog.w(TAG, "Timeout ending drag to win " + win);
    // }
    AutoLock lock(mWindowMapLock);
    // !!! TODO: ANR the drag-receiving app
    if (mDragState != NULL) {
        mDragState->mDragResult = FALSE;
        mDragState->EndDragLw();
    }

    return NOERROR;
}

ECode CWindowManagerService::HandleWaitingForDrawnTimeout(
    /* [in] */ WindowState* windowState,
    /* [in] */ IRemoteCallback* remoteCallback)
{
    {
        AutoLock lock(mWindowMapLock);
        Pair< AutoPtr<WindowState>, AutoPtr<IRemoteCallback> > pair(windowState, remoteCallback);
        Slogger::W(TAG, "Timeout waiting for drawn: %s", windowState->ToString().string());
        mWaitingForDrawn.Remove(pair);
    }

    // try {
    remoteCallback->SendResult(NULL);
    // } catch (RemoteException e) {
    // }
    return NOERROR;
}

ECode CWindowManagerService::HandleUpdateAnimParameters()
{
    // Used to send multiple changes from the animation side to the layout side.
    AutoLock lock(mWindowMapLock);
    if (CopyAnimToLayoutParamsLocked()) {
        Boolean result;
        mH->SendEmptyMessage(H::CLEAR_PENDING_ACTIONS, &result);

        PerformLayoutAndPlaceSurfacesLocked();
    }
    return NOERROR;
}

ECode CWindowManagerService::HandleDoDisplayAdded(
    /* [in] */ Int32 displayId)
{
    AutoLock lock(mWindowMapLock);
    HandleDisplayAddedLocked(displayId);
    return NOERROR;
}

ECode CWindowManagerService::HandleDoDisplayRemoved(
    /* [in] */ Int32 displayId)
{
    AutoLock lock(mWindowMapLock);
    HandleDisplayRemovedLocked(displayId);
    return NOERROR;
}

// -------------------------------------------------------------
// IWindowManager API
// -------------------------------------------------------------
ECode CWindowManagerService::OpenSession(
    /* [in] */ IInputMethodClient* client,
    /* [in] */ IInputContext* inputContext,
    /* [out] */ IWindowSession** session)
{
    VALIDATE_NOT_NULL(session);
    *session = NULL;

    if (client == NULL || inputContext == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("NULL client");
        // throw new IllegalArgumentException("NULL inputContext");
    }

    AutoPtr<CSession> csession;
    CSession::NewByFriend(
        (Handle32)this, client, inputContext, (CSession**)&csession);
    *session = csession;
    REFCOUNT_ADD(*session);
    return NOERROR;
}

ECode CWindowManagerService::InputMethodClientHasFocus(
    /* [in] */ IInputMethodClient* client,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoLock lock(mWindowMapLock);
    // The focus for the client is the window immediately below
    // where we would place the input method window.
    List< AutoPtr<WindowState> >& windows = GetDefaultWindowListLocked();
    List< AutoPtr<WindowState> >::Iterator it = FindDesiredInputMethodWindowItLocked(FALSE);
    if (it != windows.End() && it != windows.Begin()) {
        // TODO(multidisplay): IMEs are only supported on the default display.
        AutoPtr<WindowState> imFocus = *(--it);
        // if (DEBUG_INPUT_METHOD) {
        //     Slog.i(TAG, "Desired input method target: " + imFocus);
        //     Slog.i(TAG, "Current focus: " + mCurrentFocus);
        //     Slog.i(TAG, "Last focus: " + mLastFocus);
        // }
        if (imFocus != NULL) {
            // This may be a starting window, in which case we still want
            // to count it as okay.
            Int32 type;
            imFocus->mAttrs->GetType(&type);
            if (type == IWindowManagerLayoutParams::TYPE_APPLICATION_STARTING
                    && imFocus->mAppToken != NULL) {
                // The client has definitely started, so it really should
                // have a window in this app token.  Let's look for it.
                List< AutoPtr<WindowState> >::Iterator wit
                        = imFocus->mAppToken->mWindows.Begin();
                for (; wit != imFocus->mAppToken->mWindows.End(); ++wit) {
                    AutoPtr<WindowState> w = *wit;
                    if (w != imFocus) {
                        Slogger::I(TAG, "Switching to real app window: %d", w.Get());
                        imFocus = w;
                        break;
                    }
                }
            }
            // if (DEBUG_INPUT_METHOD) {
            //     Slog.i(TAG, "IM target client: " + imFocus.mSession.mClient);
            //     if (imFocus.mSession.mClient != null) {
            //         Slog.i(TAG, "IM target client binder: "
            //                 + imFocus.mSession.mClient.asBinder());
            //         Slog.i(TAG, "Requesting client binder: " + client.asBinder());
            //     }
            // }
            if (imFocus->mSession->mClient != NULL &&
                    imFocus->mSession->mClient.Get() == client) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }

    // Okay, how about this...  what is the current focus?
    // It seems in some cases we may not have moved the IM
    // target window, such as when it was in a pop-up window,
    // so let's also look at the current focus.  (An example:
    // go to Gmail, start searching so the keyboard goes up,
    // press home.  Sometimes the IME won't go down.)
    // Would be nice to fix this more correctly, but it's
    // way at the end of a release, and this should be good enough.
    if (mCurrentFocus != NULL && mCurrentFocus->mSession->mClient != NULL
            && mCurrentFocus->mSession->mClient.Get() == client) {
        *result = TRUE;
        return NOERROR;
    }

    *result = FALSE;
    return NOERROR;
}

void CWindowManagerService::GetInitialDisplaySize(
    /* [in] */ Int32 displayId,
    /* [in] */ IPoint** size)
{
    // TODO(cmautner): Access to DisplayContent should be locked on mWindowMap. Doing that
    //  could lead to deadlock since this is called from ActivityManager.
    AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
    if (displayContent != NULL) {
        AutoLock lock(displayContent->mDisplaySizeLock);
        CPoint::New(displayContent->mInitialDisplayWidth, displayContent->mInitialDisplayHeight, size);
        // size->SetX(displayContent->mInitialDisplayWidth);
        // size->SetY(displayContent->mInitialDisplayHeight);
    }
}

ECode CWindowManagerService::SetForcedDisplaySize(
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    AutoLock lock(mWindowMapLock);
    // Set some sort of reasonable bounds on the size of the display that we
    // will try to emulate.
    Int32 MIN_WIDTH = 200;
    Int32 MIN_HEIGHT = 200;
    Int32 MAX_SCALE = 2;
    AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
    if (displayContent != NULL) {
        width = Elastos::Core::Math::Min(Elastos::Core::Math::Max(width, MIN_WIDTH),
                displayContent->mInitialDisplayWidth * MAX_SCALE);
        height = Elastos::Core::Math::Min(Elastos::Core::Math::Max(height, MIN_HEIGHT),
                displayContent->mInitialDisplayHeight * MAX_SCALE);
        SetForcedDisplaySizeLocked(displayContent, width, height);
        AutoPtr<IContentResolver> cr;
        mContext->GetContentResolver((IContentResolver**)&cr);
        AutoPtr<ISettingsGlobal> sg;
        CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sg);
        Boolean result;
        sg->PutString(cr, ISettingsGlobal::DISPLAY_SIZE_FORCED,
                StringUtils::Int32ToString(width) + String(",") + StringUtils::Int32ToString(height), &result);
    }
    return NOERROR;
}

void CWindowManagerService::ReadForcedDisplaySizeAndDensityLocked(
    /* [in] */ DisplayContent* displayContent)
{
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    AutoPtr<ISettingsGlobal> sg;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sg);
    String sizeStr;
    sg->GetString(cr, ISettingsGlobal::DISPLAY_SIZE_FORCED, &sizeStr);
    if (!sizeStr.IsNullOrEmpty()) {
        Int32 pos = sizeStr.IndexOf(',');
        if (pos > 0 && sizeStr.LastIndexOf(',') == pos) {
            Int32 width, height;
            // try {
            width = StringUtils::ParseInt32(sizeStr.Substring(0, pos));
            height = StringUtils::ParseInt32(sizeStr.Substring(pos+1));
            AutoLock lock(displayContent->mDisplaySizeLock);
            if (displayContent->mBaseDisplayWidth != width
                    || displayContent->mBaseDisplayHeight != height) {
                // Slog.i(TAG, "FORCED DISPLAY SIZE: " + width + "x" + height);
                displayContent->mBaseDisplayWidth = width;
                displayContent->mBaseDisplayHeight = height;
            }
            // } catch (NumberFormatException ex) {
            // }
        }
    }


    String densityStr;
    sg->GetString(cr,
            ISettingsGlobal::DISPLAY_DENSITY_FORCED, &densityStr);
    if (!densityStr.IsNullOrEmpty()) {
        Int32 density;
        // try {
        density = StringUtils::ParseInt32(densityStr);
        AutoLock lock(displayContent->mDisplaySizeLock);
        if (displayContent->mBaseDisplayDensity != density) {
            Slogger::I(TAG, "FORCED DISPLAY DENSITY: %d", density);
            displayContent->mBaseDisplayDensity = density;
        }
        // } catch (NumberFormatException ex) {
        // }
    }
}

void CWindowManagerService::SetForcedDisplaySizeLocked(
    /* [in] */ DisplayContent* displayContent,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    Slogger::I(TAG, "Using new display size: %dx%d", width, height);
    {
        AutoLock lock(displayContent->mDisplaySizeLock);
        displayContent->mBaseDisplayWidth = width;
        displayContent->mBaseDisplayHeight = height;
    }

    ReconfigureDisplayLocked(displayContent);
}

ECode CWindowManagerService::ClearForcedDisplaySize(
    /* [in] */ Int32 displayId)
{
    AutoLock lock(mWindowMapLock);
    AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
    if (displayContent != NULL) {
        SetForcedDisplaySizeLocked(displayContent, displayContent->mInitialDisplayWidth,
                displayContent->mInitialDisplayHeight);
        AutoPtr<IContentResolver> cr;
        mContext->GetContentResolver((IContentResolver**)&cr);
        AutoPtr<ISettingsGlobal> sg;
        CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sg);
        Boolean result;
        sg->PutString(cr, ISettingsGlobal::DISPLAY_SIZE_FORCED, String(""), &result);
    }

    return NOERROR;
}

ECode CWindowManagerService::SetForcedDisplayDensity(
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 density)
{
    AutoLock lock(mWindowMapLock);
    AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
    if (displayContent != NULL) {
        SetForcedDisplayDensityLocked(displayContent, density);
        AutoPtr<IContentResolver> cr;
        mContext->GetContentResolver((IContentResolver**)&cr);
        AutoPtr<ISettingsGlobal> sg;
        CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sg);
        Boolean result;
        sg->PutString(cr, ISettingsGlobal::DISPLAY_SIZE_FORCED,
                StringUtils::Int32ToString(density), &result);
    }

    return NOERROR;
}

void CWindowManagerService::SetForcedDisplayDensityLocked(
    /* [in] */ DisplayContent* displayContent,
    /* [in] */ Int32 density)
{
    Slogger::I(TAG, "Using new display density: %d", density);

    {
        AutoLock lock(displayContent->mDisplaySizeLock);
        displayContent->mBaseDisplayDensity = density;
    }
    ReconfigureDisplayLocked(displayContent);
}

ECode CWindowManagerService::ClearForcedDisplayDensity(
    /* [in] */ Int32 displayId)
{
    AutoLock lock(mWindowMapLock);
    AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
    if (displayContent != NULL) {
        SetForcedDisplayDensityLocked(displayContent,
                displayContent->mInitialDisplayDensity);
        AutoPtr<IContentResolver> cr;
        mContext->GetContentResolver((IContentResolver**)&cr);
        AutoPtr<ISettingsGlobal> sg;
        CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sg);
        Boolean result;
        sg->PutString(cr, ISettingsGlobal::DISPLAY_SIZE_FORCED, String(""), &result);
    }

    return NOERROR;
}

void CWindowManagerService::ReconfigureDisplayLocked(
    /* [in] */ DisplayContent* displayContent)
{
    // TODO: Multidisplay: for now only use with default display.
    mPolicy->SetInitialDisplaySize(displayContent->GetDisplay(),
            displayContent->mBaseDisplayWidth,
            displayContent->mBaseDisplayHeight,
            displayContent->mBaseDisplayDensity);

    displayContent->mLayoutNeeded = TRUE;

    Boolean configChanged = UpdateOrientationFromAppTokensLocked(FALSE);
    mTempConfiguration->SetToDefaults();
    Float scale;
    mCurConfiguration->GetFontScale(&scale);
    mTempConfiguration->SetFontScale(scale);
    if (ComputeScreenConfigurationLocked(mTempConfiguration)) {
        Int32 result;
        mCurConfiguration->Diff(mTempConfiguration, &result);
        if (result != 0) {
            configChanged = TRUE;
        }
    }

    if (configChanged) {
        mWaitingForConfig = TRUE;
        StartFreezingDisplayLocked(FALSE, 0, 0);
        Boolean result;
        mH->SendEmptyMessage(H::SEND_NEW_CONFIGURATION, &result);
    }

    PerformLayoutAndPlaceSurfacesLocked();
}

ECode CWindowManagerService::HasSystemNavBar(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mPolicy->HasSystemNavBar(result);
}

// -------------------------------------------------------------
// Internals
// -------------------------------------------------------------
ECode CWindowManagerService::WindowForClientLocked(
    /* [in] */ CSession* session,
    /* [in] */ IIWindow* client,
    /* [in] */ Boolean throwOnError,
    /* [out] */ WindowState** win)
{
    return WindowForClientLocked(session, IBinder::Probe(client), throwOnError, win);
}

ECode CWindowManagerService::WindowForClientLocked(
    /* [in] */ CSession* session,
    /* [in] */ IBinder* client,
    /* [in] */ Boolean throwOnError,
    /* [out] */ WindowState** w)
{
    AutoPtr<WindowState> win;
    HashMap<AutoPtr<IBinder>, AutoPtr<WindowState> >::Iterator it
            = mWindowMap.Find(client);
    if (it != mWindowMap.End()) {
        win = it->mSecond;
    }

    // if (localLOGV) Slog.v(
    //         TAG, "Looking up client " + client + ": " + win);
    if (win == NULL) {
        // RuntimeException ex = new IllegalArgumentException(
        //         "Requested window " + client + " does not exist");
        *w = NULL;
        if (throwOnError) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        return NOERROR;
    }
    if (session != NULL && win->mSession.Get() != session) {
        // RuntimeException ex = new IllegalArgumentException(
        //         "Requested window " + client + " is in session " +
        //         win.mSession + ", not " + session);
        *w = NULL;
        if (throwOnError) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        Slogger::W(TAG, "Failed looking up window");
        return NOERROR;
    }

    *w = win;
    REFCOUNT_ADD(*w);
    return NOERROR;
}

void CWindowManagerService::RebuildAppWindowListLocked()
{
    HashMap<Int32, AutoPtr<DisplayContent> >::Iterator it;
    for (it = mDisplayContents.Begin(); it != mDisplayContents.End(); ++it) {
        RebuildAppWindowListLocked(it->mSecond);
    }
}

void CWindowManagerService::RebuildAppWindowListLocked(
    /* [in] */ DisplayContent* displayContent)
{
    List< AutoPtr<WindowState> >& windows = displayContent->GetWindowList();
    Int32 NW = windows.GetSize();
    List< AutoPtr<WindowState> >::Iterator lastBelow = windows.End();
    Int32 numRemoved = 0;

    if (mRebuildTmp->GetLength() < NW) {
        mRebuildTmp = ArrayOf<WindowState*>::Alloc(NW + 10);
    }

    // First remove all existing app windows.
    List< AutoPtr<WindowState> >::Iterator it = windows.Begin();
    while (it != windows.End()) {
        AutoPtr<WindowState> w = *it;
        if (w->mAppToken != NULL) {
            w->mRebuilding = TRUE;
            mRebuildTmp->Set(numRemoved, w);
            it = windows.Erase(it);
            mWindowsChanged = TRUE;
            numRemoved++;
            continue;
        }
        else if (lastBelow == --List< AutoPtr<WindowState> >::Iterator(it)) {
            Int32 type;
            w->mAttrs->GetType(&type);
            if (type == IWindowManagerLayoutParams::TYPE_WALLPAPER
                    || type == IWindowManagerLayoutParams::TYPE_UNIVERSE_BACKGROUND) {
                lastBelow = it;
            }
        }
        ++it;
    }

    // The wallpaper window(s) typically live at the bottom of the stack,
    // so skip them before adding app tokens.
    lastBelow++;
    it = lastBelow;

    // First add all of the exiting app tokens...  these are no longer
    // in the main app list, but still have windows shown.  We put them
    // in the back because now that the animation is over we no longer
    // will care about them.
    List< AutoPtr<AppWindowToken> >::Iterator awtIt = mExitingAppTokens.Begin();
    for (; awtIt != mExitingAppTokens.End(); ++awtIt) {
        it = ReAddAppWindowsLocked(displayContent, it, *awtIt);
    }

    // And add in the still active app tokens in Z order.
    for (awtIt = mAnimatingAppTokens.Begin(); awtIt != mAnimatingAppTokens.End(); ++awtIt) {
        it = ReAddAppWindowsLocked(displayContent, it, *awtIt);
    }

    Int32 i = 0;
    List< AutoPtr<WindowState> >::Iterator tempIt;
    Int32 indexOfIt, indexofLastBelow;
    for (tempIt = windows.Begin(); tempIt != windows.End(); ++tempIt, ++i) {
        if (*tempIt == *lastBelow) indexofLastBelow = i;
        if (*tempIt == *it) {
            indexOfIt = i;
            break;
        }
    }
    indexOfIt -= indexofLastBelow;
    if (i != numRemoved) {
        Slogger::W(TAG, "Rebuild removed %d windows but added %d", numRemoved, i);
        for (i = 0; i < numRemoved; i++) {
            AutoPtr<WindowState> ws = (*mRebuildTmp)[i];
            if (ws->mRebuilding) {
                AutoPtr<IStringWriter> sw;
                CStringWriter::New((IStringWriter**)&sw);
                AutoPtr<IPrintWriter> pw;
                CPrintWriter::New(IWriter::Probe(sw), (IPrintWriter**)&pw);
                // ws.dump(pw, "", true);
                IFlushable::Probe(pw)->Flush();
                Slogger::W(TAG, "This window was lost: %p", ws.Get());
                // TODO:
                // String swStr;
                // sw->ToString(&swStr);
                // Slogger::W(TAG, swStr);
                ws->mWinAnimator->DestroySurfaceLocked(FALSE);
            }
        }

        // Slogger::W(TAG, "Current app token list:");
        // DumpAnimatingAppTokensLocked();
        // Slogger::W(TAG, "Final window list:");
        // DumpWindowsLocked();
    }
}

void CWindowManagerService::AssignLayersLocked(
    /* [in] */ List< AutoPtr<WindowState> >& windows)
{
    Int32 curBaseLayer = 0;
    Int32 curLayer = 0;
    List< AutoPtr<WindowState> >::Iterator it;

    // if (DEBUG_LAYERS) {
    //     RuntimeException here = new RuntimeException("here");
    //     here.fillInStackTrace();
    //     Slog.v(TAG, "Assigning layers", here);
    // }

    Boolean anyLayerChanged = FALSE;

    for (it = windows.Begin(); it != windows.End(); it++) {
        AutoPtr<WindowState> w = *it;
        AutoPtr<WindowStateAnimator> winAnimator = w->mWinAnimator;
        Boolean layerChanged = FALSE;
        Int32 oldLayer = w->mLayer;
        if (w->mBaseLayer == curBaseLayer || w->mIsImWindow
                || (it != windows.Begin() && w->mIsWallpaper)) {
            curLayer += WINDOW_LAYER_MULTIPLIER;
            w->mLayer = curLayer;
        }
        else {
            curBaseLayer = curLayer = w->mBaseLayer;
            w->mLayer = curLayer;
        }
        if (w->mLayer != oldLayer) {
            layerChanged = TRUE;
            anyLayerChanged = TRUE;
        }
        oldLayer = winAnimator->mAnimLayer;
        if (w->mTargetAppToken != NULL) {
            winAnimator->mAnimLayer =
                    w->mLayer + w->mTargetAppToken->mAppAnimator->mAnimLayerAdjustment;
        }
        else if (w->mAppToken != NULL) {
            winAnimator->mAnimLayer =
                    w->mLayer + w->mAppToken->mAppAnimator->mAnimLayerAdjustment;
        }
        else {
            winAnimator->mAnimLayer = w->mLayer;
        }
        if (w->mIsImWindow) {
            winAnimator->mAnimLayer += mInputMethodAnimLayerAdjustment;
        }
        else if (w->mIsWallpaper) {
            winAnimator->mAnimLayer += mWallpaperAnimLayerAdjustment;
        }
        if (winAnimator->mAnimLayer != oldLayer) {
            layerChanged = TRUE;
            anyLayerChanged = TRUE;
        }
        if (layerChanged && mAnimator->IsDimmingLocked(winAnimator)) {
            // Force an animation pass just to update the mDimAnimator layer.
            UpdateLayoutToAnimationLocked();
        }

        if (DEBUG_LAYERS)
            Slogger::V(TAG, "Assign layer %p : mBase=%d, mLayer=%d, animLayer=%d",
                w.Get(), w->mBaseLayer, w->mLayer, winAnimator->mAnimLayer);
        // System.out.println(
        //     "Assigned layer " + curLayer + " to " + w.mClient.asBinder());
    }

    if (anyLayerChanged) {
        ScheduleNotifyWindowLayersChangedIfNeededLocked(GetDefaultDisplayContentLocked());
    }
}

void CWindowManagerService::PerformLayoutAndPlaceSurfacesLocked()
{
    Int32 loopCount = 6;
    do {
        mTraversalScheduled = FALSE;
        PerformLayoutAndPlaceSurfacesLockedLoop();
        mH->RemoveMessages(H::DO_TRAVERSAL);
        loopCount--;
    }
    while (mTraversalScheduled && loopCount > 0);
}

void CWindowManagerService::PerformLayoutAndPlaceSurfacesLockedLoop()
{
    if (mInLayout) {
        // if (DEBUG) {
        //     throw new RuntimeException("Recursive call!");
        // }
        // Slog.w(TAG, "performLayoutAndPlaceSurfacesLocked called while in layout. Callers="
        //         + Debug.getCallers(3));
        return;
    }
    // TODO: CActivityManagerService::UpdateConfigurationLocked ScheduleConfigurationChanged
    //
    if (mWaitingForConfig) {
        // Our configuration has changed (most likely rotation), but we
        // don't yet have the complete configuration to report to
        // applications.  Don't do any window layout until we have it.
        return;
    }

    if (!mDisplayReady) {
        // Not yet initialized, nothing to do.
        return;
    }

    // AutoPtr<ITrace> trace;
    // ASSERT_SUCCEEDED(CTrace::AcquireSingleton((ITrace**)&trace));
    // trace->TraceBegin(ITrace::TRACE_TAG_WINDOW_MANAGER, String("wmLayout"));
    mInLayout = TRUE;
    Boolean recoveringMemory = FALSE;

    // try {
    if (mForceRemoves != NULL) {
        recoveringMemory = TRUE;
        // Wait a little it for things to settle down, and off we go.
        List< AutoPtr<WindowState> >::Iterator it;
        for (it = mForceRemoves->Begin(); it != mForceRemoves->End(); ++it) {
            AutoPtr<WindowState> ws = *it;
            Slogger::I(TAG, "Force removing: %p", ws.Get());
            RemoveWindowInnerLocked(ws->mSession, ws);
        }
        mForceRemoves = NULL;
        Slogger::W(TAG, "Due to memory failure, waiting a bit for next layout");
        Object tmp;
        {
            AutoLock lock(tmp);
            // try {
            tmp.Wait(250);
            // } catch (InterruptedException e) {
            // }
        }
    }
    // catch (RuntimeException e) {
    //     Log.wtf(TAG, "Unhandled exception while force removing for memory", e);
    // }

    // try {
    PerformLayoutAndPlaceSurfacesLockedInner(recoveringMemory);

    mInLayout = FALSE;

    if (NeedsLayout()) {
        if (++mLayoutRepeatCount < 6) {
            RequestTraversalLocked();
        }
        else {
            Slogger::E(TAG, "Performed 6 layouts in a row. Skipping");
            mLayoutRepeatCount = 0;
        }
    }
    else {
        mLayoutRepeatCount = 0;
    }

    if (mWindowsChanged && mWindowChangeListeners.IsEmpty()) {
        mH->RemoveMessages(H::REPORT_WINDOWS_CHANGE);
        Boolean result;
        mH->SendEmptyMessage(H::REPORT_WINDOWS_CHANGE, &result);
    }
    // } catch (RuntimeException e) {
    //     mInLayout = false;
    //     Log.wtf(TAG, "Unhandled exception while laying out windows", e);
    // }

    // trace->TraceEnd(ITrace::TRACE_TAG_WINDOW_MANAGER);
}

void CWindowManagerService::PerformLayoutLockedInner(
    /* [in] */ DisplayContent* displayContent,
    /* [in] */ Boolean initial,
    /* [in] */ Boolean updateInputWindows)
{
    if (!displayContent->mLayoutNeeded) {
        return;
    }
    displayContent->mLayoutNeeded = FALSE;
    List< AutoPtr<WindowState> > windows = displayContent->GetWindowList();
    Boolean isDefaultDisplay = displayContent->mIsDefaultDisplay;

    AutoPtr<IDisplayInfo> displayInfo = displayContent->GetDisplayInfo();
    Int32 dw, dh;
    displayInfo->GetLogicalWidth(&dw);
    displayInfo->GetLogicalHeight(&dh);

    List< AutoPtr<FakeWindowImpl> >::Iterator fwIt;
    for (fwIt = mFakeWindows.Begin(); fwIt != mFakeWindows.End(); ++fwIt) {
        (*fwIt)->Layout(dw, dh);
    }

    // if (DEBUG_LAYOUT) {
    //     Slog.v(TAG, "-------------------------------------");
    //     Slog.v(TAG, "performLayout: needed="
    //             + displayContent.layoutNeeded + " dw=" + dw + " dh=" + dh);
    // }

    AutoPtr<WindowStateAnimator> universeBackground;

    mPolicy->BeginLayoutLw(isDefaultDisplay, dw, dh, mRotation);
    if (isDefaultDisplay) {
        // Not needed on non-default displays.
        mPolicy->GetSystemDecorRectLw(mSystemDecorRect, &mSystemDecorLayer);
        mScreenRect->Set(0, 0, dw, dh);
    }

    Int32 seq = mLayoutSeq + 1;
    if (seq < 0) seq = 0;
    mLayoutSeq = seq;

    Boolean behindDream = FALSE;

    // First perform layout of any root windows (not attached
    // to another window).
    List< AutoPtr<WindowState> >::ReverseIterator topAttached = windows.REnd();
    List< AutoPtr<WindowState> >::ReverseIterator wRIt;
    for (wRIt = windows.RBegin(); wRIt != windows.REnd(); ++wRIt) {
        AutoPtr<WindowState> win = *wRIt;

        // Don't do layout of a window if it is not visible, or
        // soon won't be visible, to avoid wasting time and funky
        // changes while a window is animating away.
        Boolean result, isGone;
        mPolicy->CanBeForceHidden(win, win->mAttrs, &result);
        win->IsGoneForLayoutLw(&isGone);
        Boolean gone = (behindDream && result) || isGone;

        // if (DEBUG_LAYOUT && !win->mLayoutAttached) {
        //     Slog.v(TAG, "1ST PASS " + win
        //             + ": gone=" + gone + " mHaveFrame=" + win.mHaveFrame
        //             + " mLayoutAttached=" + win.mLayoutAttached
        //             + " screen changed=" + win.isConfigChanged());
        //     final AppWindowToken atoken = win.mAppToken;
        //     if (gone) Slog.v(TAG, "  GONE: mViewVisibility="
        //             + win.mViewVisibility + " mRelayoutCalled="
        //             + win.mRelayoutCalled + " hidden="
        //             + win.mRootToken.hidden + " hiddenRequested="
        //             + (atoken != null && atoken.hiddenRequested)
        //             + " mAttachedHidden=" + win.mAttachedHidden);
        //     else Slog.v(TAG, "  VIS: mViewVisibility="
        //             + win.mViewVisibility + " mRelayoutCalled="
        //             + win.mRelayoutCalled + " hidden="
        //             + win.mRootToken.hidden + " hiddenRequested="
        //             + (atoken != null && atoken.hiddenRequested)
        //             + " mAttachedHidden=" + win.mAttachedHidden);
        // }

        // If this view is GONE, then skip it -- keep the current
        // frame, and let the caller know so they can ignore it
        // if they want.  (We do the normal layout for INVISIBLE
        // windows, since that means "perform layout as normal,
        // just don't display").
        Int32 type;
        win->mAttrs->GetType(&type);
        if (!gone || !win->mHaveFrame || win->mLayoutNeeded
                || ((type == IWindowManagerLayoutParams::TYPE_KEYGUARD || type == IWindowManagerLayoutParams::TYPE_WALLPAPER) &&
                        win->IsConfigChanged())
                || type == IWindowManagerLayoutParams::TYPE_UNIVERSE_BACKGROUND) {
            if (!win->mLayoutAttached) {
                if (initial) {
                    //Slog.i(TAG, "Window " + this + " clearing mContentChanged - initial");
                    win->mContentChanged = FALSE;
                }
                if (type == IWindowManagerLayoutParams::TYPE_DREAM) {
                    // Don't layout windows behind a dream, so that if it
                    // does stuff like hide the status bar we won't get a
                    // bad transition when it goes away.
                    behindDream = TRUE;
                }
                win->mLayoutNeeded = FALSE;
                win->Prelayout();
                mPolicy->LayoutWindowLw(win, win->mAttrs, NULL);
                win->mLayoutSeq = seq;
                // if (DEBUG_LAYOUT) Slog.v(TAG, "  LAYOUT: mFrame="
                //         + win.mFrame + " mContainingFrame="
                //         + win.mContainingFrame + " mDisplayFrame="
                //         + win.mDisplayFrame);
            }
            else {
                if (topAttached == windows.REnd()) topAttached = wRIt;
            }
        }
        if (win->mViewVisibility == IView::VISIBLE
                && type == IWindowManagerLayoutParams::TYPE_UNIVERSE_BACKGROUND
                && universeBackground == NULL) {
            universeBackground = win->mWinAnimator;
        }
    }

    if (mAnimator->mUniverseBackground  != universeBackground) {
        mFocusMayChange = TRUE;
        mAnimator->mUniverseBackground = universeBackground;
    }

    Boolean attachedBehindDream = FALSE;

    // Now perform layout of attached windows, which usually
    // depend on the position of the window they are attached to.
    // XXX does not deal with windows that are attached to windows
    // that are themselves attached.
    for (wRIt = topAttached; wRIt != windows.REnd(); ++wRIt) {
        AutoPtr<WindowState> win = *wRIt;
        if (win->mLayoutAttached) {
            if (DEBUG_LAYOUT) {
                Slogger::V(TAG, "2ND PASS %p mHaveFrame=%d mViewVisibility=%d mRelayoutCalled=%d",
                        win.Get(), win->mHaveFrame, win->mViewVisibility, win->mRelayoutCalled);
            }
            // If this view is GONE, then skip it -- keep the current
            // frame, and let the caller know so they can ignore it
            // if they want.  (We do the normal layout for INVISIBLE
            // windows, since that means "perform layout as normal,
            // just don't display").
            Boolean result;
            if (attachedBehindDream &&
                    (mPolicy->CanBeForceHidden(win, win->mAttrs, &result), result)) {
                continue;
            }
            if ((win->mViewVisibility != IView::GONE && win->mRelayoutCalled)
                    || !win->mHaveFrame || win->mLayoutNeeded) {
                if (initial) {
                    Slogger::I(TAG, "Window %p  clearing mContentChanged - initial", this);
                    win->mContentChanged = FALSE;
                }
                win->mLayoutNeeded = FALSE;
                win->Prelayout();
                mPolicy->LayoutWindowLw(win, win->mAttrs, win->mAttachedWindow);
                win->mLayoutSeq = seq;
                // if (DEBUG_LAYOUT) Slog.v(TAG, "  LAYOUT: mFrame="
                //         + win.mFrame + " mContainingFrame="
                //         + win.mContainingFrame + " mDisplayFrame="
                //         + win.mDisplayFrame);
            }
        }
        else {
            // Don't layout windows behind a dream, so that if it
            // does stuff like hide the status bar we won't get a
            // bad transition when it goes away.
            Int32 type;
            win->mAttrs->GetType(&type);
            if (type == IWindowManagerLayoutParams::TYPE_DREAM)
                attachedBehindDream = behindDream;
        }
    }

    // Window frames may have changed.  Tell the input dispatcher about it.
    mInputMonitor->SetUpdateInputWindowsNeededLw();
    if (updateInputWindows) {
        mInputMonitor->UpdateInputWindowsLw(FALSE /*force*/);
    }

    mPolicy->FinishLayoutLw();
}

void CWindowManagerService::MakeWindowFreezingScreenIfNeededLocked(
    /* [in] */ WindowState* w)
{
    // If the screen is currently frozen or off, then keep
    // it frozen/off until this window draws at its new
    // orientation.
    if (!OkToDisplay()) {
        // if (DEBUG_ORIENTATION) Slog.v(TAG,
        //         "Changing surface while display frozen: " + w);
        w->mOrientationChanging = TRUE;
        mInnerFields->mOrientationChangeComplete = FALSE;
        if (!mWindowsFreezingScreen) {
            mWindowsFreezingScreen = TRUE;
            // XXX should probably keep timeout from
            // when we first froze the display.
            mH->RemoveMessages(H::WINDOW_FREEZE_TIMEOUT);
            Boolean result;
            mH->SendEmptyMessageDelayed(H::WINDOW_FREEZE_TIMEOUT,
                WINDOW_FREEZE_TIMEOUT_DURATION, &result);
        }
    }
}

Int32 CWindowManagerService::HandleAppTransitionReadyLocked(
    /* [in] */ List< AutoPtr<WindowState> >& windows)
{
    Int32 changes = 0;
    Boolean goodToGo = true;
    // if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
    //         "Checking " + NN + " opening apps (frozen="
    //         + mDisplayFrozen + " timeout="
    //         + mAppTransitionTimeout + ")...");
    if (!mDisplayFrozen && !mAppTransitionTimeout) {
        // If the display isn't frozen, wait to do anything until
        // all of the apps are ready.  Otherwise just go because
        // we'll unfreeze the display when everyone is ready.
        List< AutoPtr<AppWindowToken> >::Iterator it;
        for (it = mOpeningApps.Begin(); it != mOpeningApps.End() && goodToGo; ++it) {
            AutoPtr<AppWindowToken> wtoken = *it;
            // if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
            //         "Check opening app=" + wtoken + ": allDrawn="
            //         + wtoken.allDrawn + " startingDisplayed="
            //         + wtoken.startingDisplayed + " startingMoved="
            //         + wtoken.startingMoved);
            if (!wtoken->mAllDrawn && !wtoken->mStartingDisplayed
                    && !wtoken->mStartingMoved) {
                goodToGo = FALSE;
            }
        }
    }
    if (goodToGo) {
        if (DEBUG_APP_TRANSITIONS) Slogger::V(TAG, "**** GOOD TO GO");
        Int32 transit = mNextAppTransition;
        if (mSkipAppTransitionAnimation) {
            transit = IWindowManagerPolicy::TRANSIT_UNSET;
        }
        mNextAppTransition = IWindowManagerPolicy::TRANSIT_UNSET;
        mAppTransitionReady = FALSE;
        mAppTransitionRunning = TRUE;
        mAppTransitionTimeout = FALSE;
        mStartingIconInTransition = FALSE;
        mSkipAppTransitionAnimation = FALSE;

        mH->RemoveMessages(H::APP_TRANSITION_TIMEOUT);

        RebuildAppWindowListLocked();

        // if wallpaper is animating in or out set oldWallpaper to null else to wallpaper
        AutoPtr<WindowState> oldWallpaper =
                mWallpaperTarget != NULL && mWallpaperTarget->mWinAnimator->IsAnimating()
                        && !mWallpaperTarget->mWinAnimator->IsDummyAnimation()
                ? NULL : mWallpaperTarget;

        AdjustWallpaperWindowsLocked();
        mInnerFields->mWallpaperMayChange = FALSE;

        // The top-most window will supply the layout params,
        // and we will determine it below.
        AutoPtr<IWindowManagerLayoutParams> animLp;
        Int32 bestAnimLayer = -1;
        Boolean fullscreenAnim = FALSE;

        // if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
        //         "New wallpaper target=" + mWallpaperTarget
        //         + ", oldWallpaper=" + oldWallpaper
        //         + ", lower target=" + mLowerWallpaperTarget
        //         + ", upper target=" + mUpperWallpaperTarget);
        Int32 foundWallpapers = 0;
        // Do a first pass through the tokens for two
        // things:
        // (1) Determine if both the closing and opening
        // app token sets are wallpaper targets, in which
        // case special animations are needed
        // (since the wallpaper needs to stay static
        // behind them).
        // (2) Find the layout params of the top-most
        // application window in the tokens, which is
        // what will control the animation theme.
        List< AutoPtr<AppWindowToken> >::Iterator it;
        for (it = mClosingApps.Begin(); it != mClosingApps.End(); ++it) {
            AutoPtr<AppWindowToken> wtoken = *it;
            Int32 mode = 1;
            if (mLowerWallpaperTarget != NULL) {
                if (mLowerWallpaperTarget->mAppToken == wtoken
                        || mUpperWallpaperTarget->mAppToken == wtoken) {
                    foundWallpapers |= mode;
                }
            }
            if (wtoken->mAppFullscreen) {
                AutoPtr<WindowState> ws = wtoken->FindMainWindow();
                if (ws != NULL) {
                    animLp = ws->mAttrs;
                    bestAnimLayer = ws->mLayer;
                    fullscreenAnim = TRUE;
                }
            }
            else if (!fullscreenAnim) {
                AutoPtr<WindowState> ws = wtoken->FindMainWindow();
                if (ws != NULL) {
                    if (ws->mLayer > bestAnimLayer) {
                        animLp = ws->mAttrs;
                        bestAnimLayer = ws->mLayer;
                    }
                }
            }
        }
        for (it = mOpeningApps.Begin(); it != mOpeningApps.End(); ++it) {
            AutoPtr<AppWindowToken> wtoken = *it;
            Int32 mode = 2;
            if (mLowerWallpaperTarget != NULL) {
                if (mLowerWallpaperTarget->mAppToken == wtoken
                        || mUpperWallpaperTarget->mAppToken == wtoken) {
                    foundWallpapers |= mode;
                }
            }
            if (wtoken->mAppFullscreen) {
                AutoPtr<WindowState> ws = wtoken->FindMainWindow();
                if (ws != NULL) {
                    animLp = ws->mAttrs;
                    bestAnimLayer = ws->mLayer;
                    fullscreenAnim = TRUE;
                }
            }
            else if (!fullscreenAnim) {
                AutoPtr<WindowState> ws = wtoken->FindMainWindow();
                if (ws != NULL) {
                    if (ws->mLayer > bestAnimLayer) {
                        animLp = ws->mAttrs;
                        bestAnimLayer = ws->mLayer;
                    }
                }
            }
        }

        AutoPtr<AppWindowToken> temp;
        if (foundWallpapers == 3) {
            if (DEBUG_APP_TRANSITIONS) Slogger::V(TAG,
                    "Wallpaper animation!");
            switch (transit) {
                case IWindowManagerPolicy::TRANSIT_ACTIVITY_OPEN:
                case IWindowManagerPolicy::TRANSIT_TASK_OPEN:
                case IWindowManagerPolicy::TRANSIT_TASK_TO_FRONT:
                    transit = IWindowManagerPolicy::TRANSIT_WALLPAPER_INTRA_OPEN;
                    break;
                case IWindowManagerPolicy::TRANSIT_ACTIVITY_CLOSE:
                case IWindowManagerPolicy::TRANSIT_TASK_CLOSE:
                case IWindowManagerPolicy::TRANSIT_TASK_TO_BACK:
                    transit = IWindowManagerPolicy::TRANSIT_WALLPAPER_INTRA_CLOSE;
                    break;
            }
            // if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
            //         "New transit: " + transit);
        }
        else if ((oldWallpaper != NULL) && (
            Find(mOpeningApps.Begin(), mOpeningApps.End(), temp = oldWallpaper->mAppToken) == mOpeningApps.End())) {
            // We are transitioning from an activity with
            // a wallpaper to one without.
            transit = IWindowManagerPolicy::TRANSIT_WALLPAPER_CLOSE;
            // if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
            //         "New transit away from wallpaper: " + transit);
        }
        else if (mWallpaperTarget != NULL) {
            Boolean isVisible;
            mWallpaperTarget->IsVisibleLw(&isVisible);
            if (isVisible) {
                // We are transitioning from an activity without
                // a wallpaper to now showing the wallpaper
                transit = IWindowManagerPolicy::TRANSIT_WALLPAPER_OPEN;
                // if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
                //         "New transit into wallpaper: " + transit);
            }
        }

        // If all closing windows are obscured, then there is
        // no need to do an animation.  This is the case, for
        // example, when this transition is being done behind
        // the lock screen.
        Boolean allowed;
        mPolicy->AllowAppAnimationsLw(&allowed);
        if (!allowed) {
            animLp = NULL;
        }

        AutoPtr<AppWindowToken> topOpeningApp;
        Int32 topOpeningLayer = 0;

        for (it = mOpeningApps.Begin(); it != mOpeningApps.End(); ++it) {
            AutoPtr<AppWindowToken> wtoken = *it;
            AutoPtr<AppWindowAnimator> appAnimator = wtoken->mAppAnimator;
            // if (DEBUG_APP_TRANSITIONS) Slog.v(TAG, "Now opening app" + wtoken);
            appAnimator->ClearThumbnail();
            wtoken->mReportedVisible = FALSE;
            wtoken->mInPendingTransaction = FALSE;
            appAnimator->mAnimation = NULL;
            SetTokenVisibilityLocked(wtoken, animLp, TRUE, transit, FALSE);
            wtoken->UpdateReportedVisibilityLocked();
            wtoken->mWaitingToShow = FALSE;

            appAnimator->mAllAppWinAnimators.Clear();
            List< AutoPtr<WindowState> >::Iterator appWIt = wtoken->mAllAppWindows.Begin();
            for (; appWIt != wtoken->mAllAppWindows.End(); ++appWIt) {
                appAnimator->mAllAppWinAnimators.PushBack((*appWIt)->mWinAnimator);
            }
            mAnimator->mAnimating |= appAnimator->ShowAllWindowsLocked();

            if (animLp != NULL) {
                Int32 layer = -1;
                List< AutoPtr<WindowState> >::Iterator tokenWIt = wtoken->mWindows.Begin();
                for (; tokenWIt != wtoken->mWindows.End(); ++tokenWIt) {
                    AutoPtr<WindowState> win = *tokenWIt;
                    if (win->mWinAnimator->mAnimLayer > layer) {
                        layer = win->mWinAnimator->mAnimLayer;
                    }
                }
                if (topOpeningApp == NULL || layer > topOpeningLayer) {
                    topOpeningApp = wtoken;
                    topOpeningLayer = layer;
                }
            }
        }
        for (it = mClosingApps.Begin(); it != mClosingApps.End(); ++it) {
            AutoPtr<AppWindowToken> wtoken = *it;
            // if (DEBUG_APP_TRANSITIONS) Slog.v(TAG,
            //         "Now closing app " + wtoken);
            wtoken->mAppAnimator->ClearThumbnail();
            wtoken->mInPendingTransaction = FALSE;
            wtoken->mAppAnimator->mAnimation = NULL;
            SetTokenVisibilityLocked(wtoken, animLp, FALSE,
                    transit, FALSE);
            wtoken->UpdateReportedVisibilityLocked();
            wtoken->mWaitingToHide = FALSE;
            // Force the allDrawn flag, because we want to start
            // this guy's animations regardless of whether it's
            // gotten drawn.
            wtoken->mAllDrawn = TRUE;
        }

        if (mNextAppTransitionThumbnail != NULL && topOpeningApp != NULL
                && topOpeningApp->mAppAnimator->mAnimation != NULL) {
            // This thumbnail animation is very special, we need to have
            // an extra surface with the thumbnail included with the animation.
            AutoPtr<IRect> dirty;
            Int32 width, height;
            mNextAppTransitionThumbnail->GetWidth(&width);
            mNextAppTransitionThumbnail->GetHeight(&height);
            CRect::New(0, 0, width, height, (IRect**)&dirty);
            // try {
            // TODO(multi-display): support other displays
            AutoPtr<DisplayContent> displayContent = GetDefaultDisplayContentLocked();
            AutoPtr<IDisplay> display = displayContent->GetDisplay();
            Int32 rectW, rectH;
            dirty->GetWidth(&rectW);
            dirty->GetHeight(&rectH);
            AutoPtr<ISurface> surface;
            if (FAILED(CSurface::New(mFxSession, String("thumbnail anim"),
                    rectW, rectH, IPixelFormat::TRANSLUCENT, ISurface::HIDDEN,
                    (ISurface**)&surface))) {
                Slogger::E(TAG, "Can't allocate thumbnail surface w=%d h=%d",
                        rectW, rectH);
                topOpeningApp->mAppAnimator->ClearThumbnail();
                goto next;
            }

            Int32 strack;
            display->GetLayerStack(&strack);
            surface->SetLayerStack(strack);
            topOpeningApp->mAppAnimator->mThumbnail = surface;
            // if (SHOW_TRANSACTIONS) Slog.i(TAG, "  THUMBNAIL "
            //         + surface + ": CREATE");
            AutoPtr<ISurface> drawSurface;
            ASSERT_SUCCEEDED(CSurface::New((ISurface**)&drawSurface));
            drawSurface->CopyFrom(surface);
            AutoPtr<ICanvas> c;
            ASSERT_SUCCEEDED(drawSurface->LockCanvas(dirty, (ICanvas**)&c));
            c->DrawBitmap(mNextAppTransitionThumbnail, 0, 0, NULL);
            drawSurface->UnlockCanvasAndPost(c);
            drawSurface->ReleaseSurface();
            topOpeningApp->mAppAnimator->mThumbnailLayer = topOpeningLayer;
            AutoPtr<IAnimation> anim = CreateThumbnailAnimationLocked(
                    transit, TRUE, TRUE, mNextAppTransitionScaleUp);
            topOpeningApp->mAppAnimator->mThumbnailAnimation = anim;
            anim->RestrictDuration(MAX_ANIMATION_DURATION);
            anim->ScaleCurrentDuration(mTransitionAnimationScale);
            topOpeningApp->mAppAnimator->mThumbnailX = mNextAppTransitionStartX;
            topOpeningApp->mAppAnimator->mThumbnailY = mNextAppTransitionStartY;
            // } catch (Surface.OutOfResourcesException e) {
            //     Slog.e(TAG, "Can't allocate thumbnail surface w=" + dirty.width()
            //             + " h=" + dirty.height(), e);
            //     topOpeningApp.mAppAnimator.clearThumbnail();
            // }
        }

next:
        mNextAppTransitionType = IActivityOptions::ANIM_NONE;
        mNextAppTransitionPackage = NULL;
        mNextAppTransitionThumbnail = NULL;
        ScheduleAnimationCallback(mNextAppTransitionCallback);
        mNextAppTransitionCallback = NULL;

        mOpeningApps.Clear();
        mClosingApps.Clear();

        // This has changed the visibility of windows, so perform
        // a new layout to get them all up-to-date.
        changes |= IWindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT
                | IWindowManagerPolicy::FINISH_LAYOUT_REDO_CONFIG;
        GetDefaultDisplayContentLocked()->mLayoutNeeded = TRUE;

        // TODO(multidisplay): IMEs are only supported on the default display.
        if (windows == GetDefaultWindowListLocked()
                && !MoveInputMethodWindowsIfNeededLocked(TRUE)) {
            AssignLayersLocked(windows);
        }
        UpdateFocusedWindowLocked(UPDATE_FOCUS_PLACING_SURFACES, FALSE /*updateInputWindows*/);
        mFocusMayChange = FALSE;
    }

    return changes;
}

Int32 CWindowManagerService::HandleAnimatingStoppedAndTransitionLocked()
{
    Int32 changes = 0;

    mAppTransitionRunning = FALSE;
    // Restore window app tokens to the ActivityManager views
    List< AutoPtr<AppWindowToken> >::ReverseIterator rit;
    for (rit = mAnimatingAppTokens.RBegin(); rit != mAnimatingAppTokens.REnd(); ++rit) {
        (*rit)->mSendingToBottom = FALSE;
    }
    mAnimatingAppTokens.Clear();
    mAnimatingAppTokens.Assign(mAppTokens.Begin(), mAppTokens.End());
    RebuildAppWindowListLocked();

    changes |= IWindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT;
    mInnerFields->mAdjResult |= ADJUST_WALLPAPER_LAYERS_CHANGED;
    MoveInputMethodWindowsIfNeededLocked(TRUE);
    mInnerFields->mWallpaperMayChange = TRUE;
    // Since the window list has been rebuilt, focus might
    // have to be recomputed since the actual order of windows
    // might have changed again.
    mFocusMayChange = TRUE;

    return changes;
}

Int32 CWindowManagerService::AnimateAwayWallpaperLocked()
{
    Int32 changes = 0;
    AutoPtr<WindowState> oldWallpaper = mWallpaperTarget;
    if (mLowerWallpaperTarget != NULL
            && mLowerWallpaperTarget->mAppToken != NULL) {
        // if (DEBUG_WALLPAPER_LIGHT) Slog.v(TAG,
        //         "wallpaperForceHiding changed with lower="
        //         + mLowerWallpaperTarget);
        // if (DEBUG_WALLPAPER_LIGHT) Slog.v(TAG,
        //         "hidden=" + mLowerWallpaperTarget.mAppToken.hidden +
        //         " hiddenRequested=" + mLowerWallpaperTarget.mAppToken.hiddenRequested);
        if (mLowerWallpaperTarget->mAppToken->mHidden) {
            // The lower target has become hidden before we
            // actually started the animation...  let's completely
            // re-evaluate everything.
            mLowerWallpaperTarget = mUpperWallpaperTarget = NULL;
            changes |= IWindowManagerPolicy::FINISH_LAYOUT_REDO_ANIM;
        }
    }
    mInnerFields->mAdjResult |= AdjustWallpaperWindowsLocked();
    // if (DEBUG_WALLPAPER_LIGHT) Slog.v(TAG, "****** OLD: " + oldWallpaper
    //         + " NEW: " + mWallpaperTarget
    //         + " LOWER: " + mLowerWallpaperTarget);
    return changes;
}

void CWindowManagerService::UpdateResizingWindows(
    /* [in] */ WindowState* w)
{
    AutoPtr<WindowStateAnimator> winAnimator = w->mWinAnimator;
    if (w->mHasSurface && w->mLayoutSeq == mLayoutSeq) {
        Boolean equals;
        w->mLastContentInsets->Equals(w->mContentInsets, &equals);
        w->mContentInsetsChanged |= !equals;
        w->mLastVisibleInsets->Equals(w->mVisibleInsets, &equals);
        w->mVisibleInsetsChanged |= !equals;
        Boolean configChanged = w->IsConfigChanged();
        // if (DEBUG_CONFIGURATION && configChanged) {
        //     Slog.v(TAG, "Win " + w + " config changed: "
        //             + mCurConfiguration);
        // }
        // if (localLOGV) Slog.v(TAG, "Resizing " + w
        //         + ": configChanged=" + configChanged
        //         + " last=" + w.mLastFrame + " frame=" + w.mFrame);
        w->mLastFrame->Set(w->mFrame);
        if (w->mContentInsetsChanged
                || w->mVisibleInsetsChanged
                || winAnimator->mSurfaceResized
                || configChanged) {
            // if (DEBUG_RESIZE || DEBUG_ORIENTATION) {
            //     Slog.v(TAG, "Resize reasons: "
            //             + " contentInsetsChanged=" + w.mContentInsetsChanged
            //             + " visibleInsetsChanged=" + w.mVisibleInsetsChanged
            //             + " surfaceResized=" + winAnimator.mSurfaceResized
            //             + " configChanged=" + configChanged);
            // }

            w->mLastContentInsets->Set(w->mContentInsets);
            w->mLastVisibleInsets->Set(w->mVisibleInsets);
            MakeWindowFreezingScreenIfNeededLocked(w);
            // If the orientation is changing, then we need to
            // hold off on unfreezing the display until this
            // window has been redrawn; to do that, we need
            // to go through the process of getting informed
            // by the application when it has finished drawing.
            if (w->mOrientationChanging) {
                // if (DEBUG_SURFACE_TRACE || DEBUG_ANIM || DEBUG_ORIENTATION) Slog.v(TAG,
                //         "Orientation start waiting for draw mDrawState=DRAW_PENDING in "
                //         + w + ", surface " + winAnimator.mSurface);
                winAnimator->mDrawState = WindowStateAnimator::DRAW_PENDING;
                if (w->mAppToken != NULL) {
                    w->mAppToken->mAllDrawn = FALSE;
                }
            }
            List< AutoPtr<WindowState> >::Iterator it;
            for (it = mResizingWindows.Begin(); it != mResizingWindows.End(); ++it) {
                if ((*it).Get() == w) break;
            }
            if (it == mResizingWindows.End()) {
                // if (DEBUG_RESIZE || DEBUG_ORIENTATION) Slog.v(TAG,
                //         "Resizing window " + w + " to " + winAnimator.mSurfaceW
                //         + "x" + winAnimator.mSurfaceH);
                mResizingWindows.PushBack(w);
            }
        }
        else if (w->mOrientationChanging) {
            if (w->IsDrawnLw()) {
                // if (DEBUG_ORIENTATION) Slog.v(TAG,
                //         "Orientation not waiting for draw in "
                //         + w + ", surface " + winAnimator.mSurface);
                w->mOrientationChanging = FALSE;
            }
        }
    }
}

void CWindowManagerService::HandleNotObscuredLocked(
    /* [in] */ WindowState* w,
    /* [in] */ Int64 currentTime,
    /* [in] */ Int32 innerDw,
    /* [in] */ Int32 innerDh)
{
    AutoPtr<IWindowManagerLayoutParams> attrs = w->mAttrs;
    Int32 attrFlags;
    attrs->GetFlags(&attrFlags);
    Boolean canBeSeen;
    w->IsDisplayedLw(&canBeSeen);

    if (w->mHasSurface) {
        if ((attrFlags & IWindowManagerLayoutParams::FLAG_KEEP_SCREEN_ON) != 0) {
            mInnerFields->mHoldScreen = w->mSession;
        }
        Float screenbrightness = 0;
        if (!mInnerFields->mSyswin
                && (attrs->GetScreenBrightness(&screenbrightness), screenbrightness >= 0)
                && mInnerFields->mScreenBrightness < 0) {
            mInnerFields->mScreenBrightness = screenbrightness;
        }
        Float buttonBrightness = 0;
        if (!mInnerFields->mSyswin
                && (attrs->GetButtonBrightness(&buttonBrightness), buttonBrightness >= 0)
                && mInnerFields->mButtonBrightness < 0) {
            mInnerFields->mButtonBrightness = buttonBrightness;
        }
        Int32 timeout = 0;
        if (!mInnerFields->mSyswin
                && (attrs->GetUserActivityTimeout(&timeout), timeout >= 0)
                && mInnerFields->mUserActivityTimeout < 0) {
            mInnerFields->mUserActivityTimeout = timeout;
        }

        Int32 type;
        attrs->GetType(&type);
        if (canBeSeen
                && (type == IWindowManagerLayoutParams::TYPE_SYSTEM_DIALOG
                 || type == IWindowManagerLayoutParams::TYPE_RECENTS_OVERLAY
                 || type == IWindowManagerLayoutParams::TYPE_KEYGUARD
                 || type == IWindowManagerLayoutParams::TYPE_SYSTEM_ERROR)) {
            mInnerFields->mSyswin = TRUE;
        }

        if (canBeSeen) {
            if (type == IWindowManagerLayoutParams::TYPE_DREAM
                    || type == IWindowManagerLayoutParams::TYPE_KEYGUARD) {
                mInnerFields->mDisplayHasContent = LayoutFields::DISPLAY_CONTENT_MIRROR;
            }
            else if (mInnerFields->mDisplayHasContent
                    == LayoutFields::DISPLAY_CONTENT_UNKNOWN) {
                mInnerFields->mDisplayHasContent = LayoutFields::DISPLAY_CONTENT_UNIQUE;
            }
        }
    }

    Boolean opaqueDrawn = canBeSeen && w->IsOpaqueDrawn();
    if (opaqueDrawn && w->IsFullscreen(innerDw, innerDh)) {
        // This window completely covers everything behind it,
        // so we want to leave all of them as undimmed (for
        // performance reasons).
        mInnerFields->mObscured = TRUE;
    }
    else if (canBeSeen && (attrFlags & IWindowManagerLayoutParams::FLAG_DIM_BEHIND) != 0
            && !(w->mAppToken != NULL && w->mAppToken->mHiddenRequested)
            && !w->mExiting) {
        // if (localLOGV) Slog.v(TAG, "Win " + w + " obscured=" + mInnerFields.mObscured);
        if (!mInnerFields->mDimming) {
            // Slog.i(TAG, "DIM BEHIND: " + w);
            mInnerFields->mDimming = TRUE;
            AutoPtr<WindowStateAnimator> winAnimator = w->mWinAnimator;
            if (!mAnimator->IsDimmingLocked(winAnimator)) {
                Int32 type;
                attrs->GetType(&type);
                Int32 width, height;
                if (type == IWindowManagerLayoutParams::TYPE_BOOT_PROGRESS) {
                    AutoPtr<IDisplayInfo> displayInfo = w->mDisplayContent->GetDisplayInfo();
                    displayInfo->GetLogicalWidth(&width);
                    displayInfo->GetLogicalHeight(&height);
                }
                else {
                    width = innerDw;
                    height = innerDh;
                }
                Float dimAmount;
                attrs->GetDimAmount(&dimAmount);
                StartDimmingLocked(
                        winAnimator, w->mExiting ? 0 : dimAmount, width, height);
            }
        }
    }
}

void CWindowManagerService::UpdateAllDrawnLocked()
{
    // See if any windows have been drawn, so they (and others
    // associated with them) can now be shown.
    List< AutoPtr<AppWindowToken> >& appTokens = mAnimatingAppTokens;
    List< AutoPtr<AppWindowToken> >::Iterator it = appTokens.Begin();
    for (; it != appTokens.End(); ++it) {
        AutoPtr<AppWindowToken> wtoken = *it;
        if (!wtoken->mAllDrawn) {
            Int32 numInteresting = wtoken->mNumInterestingWindows;
            if (numInteresting > 0 && wtoken->mNumDrawnWindows >= numInteresting) {
                // if (WindowManagerService.DEBUG_VISIBILITY) Slog.v(TAG,
                //         "allDrawn: " + wtoken
                //         + " interesting=" + numInteresting
                //         + " drawn=" + wtoken.numDrawnWindows);
                wtoken->mAllDrawn = TRUE;
            }
        }
    }
}

void CWindowManagerService::PerformLayoutAndPlaceSurfacesLockedInner(
    /* [in] */ Boolean recoveringMemory)
{
    // if (DEBUG_WINDOW_TRACE) {
    //     Slog.v(TAG, "performLayoutAndPlaceSurfacesLockedInner: entry. Called by "
    //             + Debug.getCallers(3));
    // }

    Int64 currentTime = SystemClock::GetUptimeMillis();

    if (mFocusMayChange) {
        mFocusMayChange = FALSE;
        UpdateFocusedWindowLocked(UPDATE_FOCUS_WILL_PLACE_SURFACES,
                FALSE /*updateInputWindows*/);
    }

    // Initialize state of exiting tokens.
    List< AutoPtr<WindowToken> >::ReverseIterator wtRIt = mExitingTokens.RBegin();
    for (; wtRIt != mExitingTokens.REnd(); ++wtRIt) {
        (*wtRIt)->mHasVisible = FALSE;
    }

    // Initialize state of exiting applications.
    List< AutoPtr<AppWindowToken> >::ReverseIterator appWinRIt = mExitingAppTokens.RBegin();
    for (; appWinRIt != mExitingAppTokens.REnd(); ++appWinRIt) {
        (*appWinRIt)->mHasVisible = FALSE;
    }

    mInnerFields->mHoldScreen = NULL;
    mInnerFields->mScreenBrightness = -1;
    mInnerFields->mButtonBrightness = -1;
    mInnerFields->mUserActivityTimeout = -1;
    mInnerFields->mDisplayHasContent = LayoutFields::DISPLAY_CONTENT_UNKNOWN;

    mTransactionSequence++;

    AutoPtr<DisplayContent> defaultDisplay = GetDefaultDisplayContentLocked();
    AutoPtr<IDisplayInfo> defaultInfo = defaultDisplay->GetDisplayInfo();
    Int32 defaultDw, defaultDh;
    defaultInfo->GetLogicalWidth(&defaultDw);
    defaultInfo->GetLogicalHeight(&defaultDh);

    // if (SHOW_LIGHT_TRANSACTIONS) Slog.i(TAG,
    //         ">>> OPEN TRANSACTION performLayoutAndPlaceSurfaces");
    AutoPtr<ISurfaceHelper> helper;
    CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&helper);
    helper->OpenTransaction();

    // try {
    if (mWatermark != NULL) {
        mWatermark->PositionSurface(defaultDw, defaultDh);
    }
    if (mStrictModeFlash != NULL) {
        mStrictModeFlash->PositionSurface(defaultDw, defaultDh);
    }

    Boolean focusDisplayed = FALSE;
    Boolean updateAllDrawn = FALSE;

    HashMap<Int32, AutoPtr<DisplayContent> >::Iterator it;
    for (it = mDisplayContents.Begin(); it != mDisplayContents.End(); ++it) {
        AutoPtr<DisplayContent> displayContent = it->mSecond;
        List< AutoPtr<WindowState> >& windows = displayContent->GetWindowList();
        AutoPtr<IDisplayInfo> displayInfo = displayContent->GetDisplayInfo();
        Int32 displayId = displayContent->GetDisplayId();
        Int32 dw, dh, innerDw, innerDh;
        displayInfo->GetLogicalWidth(&dw);
        displayInfo->GetLogicalHeight(&dh);
        displayInfo->GetAppWidth(&innerDw);
        displayInfo->GetAppHeight(&innerDh);
        Boolean isDefaultDisplay = (displayId == IDisplay::DEFAULT_DISPLAY);

        // Reset for each display unless we are forcing mirroring.
        if (mInnerFields->mDisplayHasContent != LayoutFields::DISPLAY_CONTENT_MIRROR) {
            mInnerFields->mDisplayHasContent = LayoutFields::DISPLAY_CONTENT_UNKNOWN;
        }

        Int32 repeats = 0;
        do {
            repeats++;
            if (repeats > 6) {
                // Slogger::W(TAG, "Animation repeat aborted after too many iterations");
                displayContent->mLayoutNeeded = FALSE;
                break;
            }

            if (DEBUG_LAYOUT_REPEATS) DebugLayoutRepeats(String("On entry to LockedInner"),
                displayContent->mPendingLayoutChanges);

            if (isDefaultDisplay && ((displayContent->mPendingLayoutChanges
                    & IWindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER) != 0)
                    && ((AdjustWallpaperWindowsLocked()
                            & ADJUST_WALLPAPER_LAYERS_CHANGED) != 0)) {
                AssignLayersLocked(windows);
                displayContent->mLayoutNeeded = TRUE;
            }

            if (isDefaultDisplay && (displayContent->mPendingLayoutChanges
                    & IWindowManagerPolicy::FINISH_LAYOUT_REDO_CONFIG) != 0) {
                if (DEBUG_LAYOUT) Slogger::V(TAG, "Computing new config from layout");
                if (UpdateOrientationFromAppTokensLocked(TRUE)) {
                    displayContent->mLayoutNeeded = TRUE;
                    Boolean result;
                    mH->SendEmptyMessage(H::SEND_NEW_CONFIGURATION, &result);
                }
            }

            if ((displayContent->mPendingLayoutChanges
                    & IWindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT) != 0) {
                displayContent->mLayoutNeeded = TRUE;
            }

            // FIRST LOOP: Perform a layout, if needed.
            if (repeats < 4) {
                PerformLayoutLockedInner(displayContent, repeats == 1,
                        FALSE /*updateInputWindows*/);
            }
            else {
                Slogger::W(TAG, "Layout repeat skipped after too many iterations");
            }

            // FIRST AND ONE HALF LOOP: Make WindowManagerPolicy think
            // it is animating.
            displayContent->mPendingLayoutChanges = 0;

            StringBuilder sb("loop number ");
            sb += mLayoutRepeatCount;
            if (DEBUG_LAYOUT_REPEATS) DebugLayoutRepeats(sb.ToString(), displayContent->mPendingLayoutChanges);

            if (isDefaultDisplay) {
                mPolicy->BeginPostLayoutPolicyLw(dw, dh);
                List< AutoPtr<WindowState> >::ReverseIterator winRIt = windows.RBegin();
                for (; winRIt != windows.REnd(); ++winRIt) {
                    AutoPtr<WindowState> w = *winRIt;
                    if (w->mHasSurface) {
                        mPolicy->ApplyPostLayoutPolicyLw(w, w->mAttrs);
                    }
                }
                Int32 result;
                mPolicy->FinishPostLayoutPolicyLw(&result);
                displayContent->mPendingLayoutChanges |= result;
                if (DEBUG_LAYOUT_REPEATS) DebugLayoutRepeats(
                    String("after finishPostLayoutPolicyLw"), displayContent->mPendingLayoutChanges);
            }
        }
        while (displayContent->mPendingLayoutChanges != 0);

        mInnerFields->mObscured = FALSE;
        mInnerFields->mDimming = FALSE;
        mInnerFields->mSyswin = FALSE;

        // Only used if default window
        Boolean someoneLosingFocus = !mLosingFocus.IsEmpty();

        List< AutoPtr<WindowState> >::ReverseIterator winRIt = windows.RBegin();
        for (; winRIt != windows.REnd(); ++winRIt) {
            AutoPtr<WindowState> w = *winRIt;

            Boolean obscuredChanged = w->mObscured != mInnerFields->mObscured;

            // Update effect.
            w->mObscured = mInnerFields->mObscured;
            if (!mInnerFields->mObscured) {
                HandleNotObscuredLocked(w, currentTime, innerDw, innerDh);
            }

            Boolean isVisible;
            if (isDefaultDisplay && obscuredChanged && (mWallpaperTarget == w)
                    && (w->IsVisibleLw(&isVisible), isVisible)) {
                // This is the wallpaper target and its obscured state
                // changed... make sure the current wallaper's visibility
                // has been updated accordingly.
                UpdateWallpaperVisibilityLocked();
            }

            AutoPtr<WindowStateAnimator> winAnimator = w->mWinAnimator;

            // If the window has moved due to its containing
            // content frame changing, then we'd like to animate
            // it.
            if (w->mHasSurface && w->ShouldAnimateMove()) {
                // Frame has moved, containing content frame
                // has also moved, and we're not currently animating...
                // let's do something.
                AutoPtr<IAnimation> a;
                AutoPtr<IAnimationUtils> animationUtils;
                CAnimationUtils::AcquireSingleton((IAnimationUtils**)&animationUtils);
                animationUtils->LoadAnimation(mContext,
                        R::anim::window_move_from_decor, (IAnimation**)&a);
                winAnimator->SetAnimation(a);
                Int32 left1, left2;
                w->mLastFrame->GetLeft(&left1);
                w->mFrame->GetLeft(&left2);
                winAnimator->mAnimDw = left1 - left2;
                Int32 top1, top2;
                w->mLastFrame->GetTop(&top1);
                w->mFrame->GetTop(&top2);
                winAnimator->mAnimDh = top1 - top2;
                // try {
                w->mClient->Moved(left2, top2);
                // } catch (RemoteException e) {
                // }
            }

            //Slog.i(TAG, "Window " + this + " clearing mContentChanged - done placing");
            w->mContentChanged = FALSE;

            // Moved from updateWindowsAndWallpaperLocked().
            if (w->mHasSurface) {
                // Take care of the window being ready to display.
                Boolean committed =
                        winAnimator->CommitFinishDrawingLocked(currentTime);
                if (isDefaultDisplay && committed) {
                    Int32 type;
                    w->mAttrs->GetType(&type);
                    if (type == IWindowManagerLayoutParams::TYPE_DREAM) {
                        // HACK: When a dream is shown, it may at that
                        // point hide the lock screen.  So we need to
                        // redo the layout to let the phone window manager
                        // make this happen.
                        displayContent->mPendingLayoutChanges |=
                                IWindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT;
                        if (CWindowManagerService::DEBUG_LAYOUT_REPEATS) {
                            DebugLayoutRepeats(
                                String("dream and commitFinishDrawingLocked true"),
                                displayContent->mPendingLayoutChanges);
                        }
                    }
                    Int32 flags;
                    w->mAttrs->GetFlags(&flags);
                    if ((flags & IWindowManagerLayoutParams::FLAG_SHOW_WALLPAPER) != 0) {
                        // if (DEBUG_WALLPAPER_LIGHT) Slog.v(TAG,
                        //         "First draw done in potential wallpaper target " + w);
                        mInnerFields->mWallpaperMayChange = TRUE;
                        displayContent->mPendingLayoutChanges |=
                                IWindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER;
                        if (CWindowManagerService::DEBUG_LAYOUT_REPEATS) {
                            DebugLayoutRepeats(
                                String("wallpaper and commitFinishDrawingLocked true"),
                                displayContent->mPendingLayoutChanges);
                        }
                    }
                }

                winAnimator->SetSurfaceBoundariesLocked(recoveringMemory);

                AutoPtr<AppWindowToken> atoken = w->mAppToken;
                if (DEBUG_STARTING_WINDOW && atoken != NULL
                        && w == atoken->mStartingWindow) {
                    Slogger::D(TAG, "updateWindows: starting %p isOnScreen=%d allDrawn=%d freezingScreen=%p",
                        w.Get(), w->IsOnScreen(), atoken->mAllDrawn, atoken->mAppAnimator->mFreezingScreen);
                }
                if (atoken != NULL
                        && (!atoken->mAllDrawn || atoken->mAppAnimator->mFreezingScreen)) {
                    if (atoken->mLastTransactionSequence != mTransactionSequence) {
                        atoken->mLastTransactionSequence = mTransactionSequence;
                        atoken->mNumInterestingWindows = atoken->mNumDrawnWindows = 0;
                        atoken->mStartingDisplayed = FALSE;
                    }
                    if ((w->IsOnScreen() || winAnimator->mAttrType == IWindowManagerLayoutParams::TYPE_BASE_APPLICATION)
                            && !w->mExiting && !w->mDestroying) {
                        // if (WindowManagerService.DEBUG_VISIBILITY ||
                        //         WindowManagerService.DEBUG_ORIENTATION) {
                        //     Slog.v(TAG, "Eval win " + w + ": isDrawn=" + w.isDrawnLw()
                        //             + ", isAnimating=" + winAnimator.isAnimating());
                        //     if (!w.isDrawnLw()) {
                        //         Slog.v(TAG, "Not displayed: s=" + winAnimator.mSurface
                        //                 + " pv=" + w.mPolicyVisibility
                        //                 + " mDrawState=" + winAnimator.mDrawState
                        //                 + " ah=" + w.mAttachedHidden
                        //                 + " th=" + atoken.hiddenRequested
                        //                 + " a=" + winAnimator.mAnimating);
                        //     }
                        // }
                        if (w != atoken->mStartingWindow) {
                            if (!atoken->mAppAnimator->mFreezingScreen || !w->mAppFreezing) {
                                atoken->mNumInterestingWindows++;
                                if (w->IsDrawnLw()) {
                                    atoken->mNumDrawnWindows++;
                                    // if (WindowManagerService.DEBUG_VISIBILITY ||
                                    //         WindowManagerService.DEBUG_ORIENTATION) Slog.v(TAG,
                                    //         "tokenMayBeDrawn: " + atoken
                                    //         + " freezingScreen=" + atoken.mAppAnimator.freezingScreen
                                    //         + " mAppFreezing=" + w.mAppFreezing);
                                    updateAllDrawn = TRUE;
                                }
                            }
                        }
                        else if (w->IsDrawnLw()) {
                            atoken->mStartingDisplayed = TRUE;
                        }
                    }
                }
            }

            Boolean isDisplayed;
            if (isDefaultDisplay && someoneLosingFocus && (w == mCurrentFocus)
                    && (w->IsDisplayedLw(&isDisplayed), isDisplayed)) {
                focusDisplayed = TRUE;
            }

            UpdateResizingWindows(w);
        }

        Boolean hasUniqueContent;
        switch (mInnerFields->mDisplayHasContent) {
            case LayoutFields::DISPLAY_CONTENT_MIRROR:
                hasUniqueContent = isDefaultDisplay;
                break;
            case LayoutFields::DISPLAY_CONTENT_UNIQUE:
                hasUniqueContent = TRUE;
                break;
            case LayoutFields::DISPLAY_CONTENT_UNKNOWN:
            default:
                hasUniqueContent = FALSE;
                break;
        }

        mDisplayManagerService->SetDisplayHasContent(displayId, hasUniqueContent,
            TRUE /* inTraversal, must call performTraversalInTrans... below */);

        if (!mInnerFields->mDimming && mAnimator->IsDimmingLocked(displayId)) {
            StopDimmingLocked(displayId);
        }
    }

    if (updateAllDrawn) {
        UpdateAllDrawnLocked();
    }

    if (focusDisplayed) {
        Boolean result;
        mH->SendEmptyMessage(H::REPORT_LOSING_FOCUS, &result);
    }

    // Give the display manager a chance to adjust properties
    // like display rotation if it needs to.

    mDisplayManagerService->PerformTraversalInTransactionFromWindowManager();
    // } catch (RuntimeException e) {
    //     Log.wtf(TAG, "Unhandled exception in Window Manager", e);
    // } finally {
    helper->CloseTransaction();
    if (SHOW_LIGHT_TRANSACTIONS) Slogger::I(TAG,
            "<<< CLOSE TRANSACTION performLayoutAndPlaceSurfaces");
    // }

    List< AutoPtr<WindowState> >& defaultWindows = defaultDisplay->GetWindowList();

    // If we are ready to perform an app transition, check through
    // all of the app tokens to be shown and see if they are ready
    // to go.
    if (mAppTransitionReady) {
        defaultDisplay->mPendingLayoutChanges |= HandleAppTransitionReadyLocked(defaultWindows);
        if (DEBUG_LAYOUT_REPEATS) DebugLayoutRepeats(String("after handleAppTransitionReadyLocked"),
            defaultDisplay->mPendingLayoutChanges);
    }

    mInnerFields->mAdjResult = 0;

    if (!mAnimator->mAnimating && mAppTransitionRunning) {
        // We have finished the animation of an app transition.  To do
        // this, we have delayed a lot of operations like showing and
        // hiding apps, moving apps in Z-order, etc.  The app token list
        // reflects the correct Z-order, but the window list may now
        // be out of sync with it.  So here we will just rebuild the
        // entire app window list.  Fun!
        defaultDisplay->mPendingLayoutChanges |= HandleAnimatingStoppedAndTransitionLocked();
        if (DEBUG_LAYOUT_REPEATS) DebugLayoutRepeats(String("after handleAnimStopAndXitionLock"),
            defaultDisplay->mPendingLayoutChanges);
    }

    if (mInnerFields->mWallpaperForceHidingChanged && defaultDisplay->mPendingLayoutChanges == 0
            && !mAppTransitionReady) {
        // At this point, there was a window with a wallpaper that
        // was force hiding other windows behind it, but now it
        // is going away.  This may be simple -- just animate
        // away the wallpaper and its window -- or it may be
        // hard -- the wallpaper now needs to be shown behind
        // something that was hidden.
        defaultDisplay->mPendingLayoutChanges |= AnimateAwayWallpaperLocked();
        if (DEBUG_LAYOUT_REPEATS) DebugLayoutRepeats(String("after animateAwayWallpaperLocked"),
            defaultDisplay->mPendingLayoutChanges);
    }
    mInnerFields->mWallpaperForceHidingChanged = FALSE;

    if (mInnerFields->mWallpaperMayChange) {
        if (CWindowManagerService::DEBUG_WALLPAPER_LIGHT) Slogger::V(TAG,
                "Wallpaper may change!  Adjusting");
        mInnerFields->mAdjResult |= AdjustWallpaperWindowsLocked();
    }

    if ((mInnerFields->mAdjResult & ADJUST_WALLPAPER_LAYERS_CHANGED) != 0) {
        if (DEBUG_WALLPAPER_LIGHT) Slogger::V(TAG,
                "Wallpaper layer changed: assigning layers + relayout");
        defaultDisplay->mPendingLayoutChanges |= IWindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT;
        AssignLayersLocked(defaultWindows);
    }
    else if ((mInnerFields->mAdjResult & ADJUST_WALLPAPER_VISIBILITY_CHANGED) != 0) {
        if (DEBUG_WALLPAPER_LIGHT) Slogger::V(TAG,
                "Wallpaper visibility changed: relayout");
        defaultDisplay->mPendingLayoutChanges |= IWindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT;
    }

    if (mFocusMayChange) {
        mFocusMayChange = FALSE;
        if (UpdateFocusedWindowLocked(UPDATE_FOCUS_PLACING_SURFACES,
                FALSE /*updateInputWindows*/)) {
            defaultDisplay->mPendingLayoutChanges |= IWindowManagerPolicy::FINISH_LAYOUT_REDO_ANIM;
            mInnerFields->mAdjResult = 0;
        }
    }

    if (NeedsLayout()) {
        defaultDisplay->mPendingLayoutChanges |= IWindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT;
        if (DEBUG_LAYOUT_REPEATS) DebugLayoutRepeats(String("mLayoutNeeded"),
                defaultDisplay->mPendingLayoutChanges);
    }

    List< AutoPtr<WindowState> >::ReverseIterator resizingWinRIt = mResizingWindows.RBegin();
    while (resizingWinRIt != mResizingWindows.REnd()) {
        AutoPtr<WindowState> win = *resizingWinRIt;
        if (win->mAppFreezing) {
            // Don't remove this window until rotation has completed.
            ++resizingWinRIt;
            continue;
        }
        AutoPtr<WindowStateAnimator> winAnimator = win->mWinAnimator;
        // try {
        // if (DEBUG_RESIZE || DEBUG_ORIENTATION) Slog.v(TAG,
        //         "Reporting new frame to " + win + ": " + win.mCompatFrame);
        //Int32 diff = 0;
        Boolean configChanged = win->IsConfigChanged();
        // if ((DEBUG_RESIZE || DEBUG_ORIENTATION || DEBUG_CONFIGURATION)
        //         && configChanged) {
        //     Slog.i(TAG, "Sending new config to window " + win + ": "
        //             + winAnimator.mSurfaceW + "x" + winAnimator.mSurfaceH
        //             + " / " + mCurConfiguration + " / 0x"
        //             + Integer.toHexString(diff));
        // }
        win->SetConfiguration(mCurConfiguration);
        // if (DEBUG_ORIENTATION &&
        //         winAnimator.mDrawState == WindowStateAnimator.DRAW_PENDING) Slog.i(
        //         TAG, "Resizing " + win + " WITH DRAW PENDING");
        if (FAILED(win->mClient->Resized(win->mFrame, win->mLastContentInsets,
                win->mLastVisibleInsets,
                winAnimator->mDrawState == WindowStateAnimator::DRAW_PENDING,
                configChanged ? win->mConfiguration : NULL))) {
            win->mOrientationChanging = FALSE;
        }
        else {
            win->mContentInsetsChanged = FALSE;
            win->mVisibleInsetsChanged = FALSE;
            winAnimator->mSurfaceResized = FALSE;
        }

        resizingWinRIt = List< AutoPtr<WindowState> >::ReverseIterator(
            mResizingWindows.Erase(--(resizingWinRIt.GetBase())));
        // } catch (RemoteException e) {
        //     win.mOrientationChanging = false;
        // }
    }

    // if (DEBUG_ORIENTATION && mDisplayFrozen) Slog.v(TAG,
    //         "With display frozen, orientationChangeComplete="
    //         + mInnerFields.mOrientationChangeComplete);
    if (mInnerFields->mOrientationChangeComplete) {
        if (mWindowsFreezingScreen) {
            mWindowsFreezingScreen = FALSE;
            mH->RemoveMessages(H::WINDOW_FREEZE_TIMEOUT);
        }
        StopFreezingDisplayLocked();
    }

    // Destroy the surface of any windows that are no longer visible.
    Boolean wallpaperDestroyed = FALSE;
    if (!mDestroySurface.IsEmpty()) {
        List< AutoPtr<WindowState> >::ReverseIterator destroyWinRit;
        for (destroyWinRit = mDestroySurface.RBegin(); destroyWinRit != mDestroySurface.REnd(); ++destroyWinRit) {
            AutoPtr<WindowState> win = *destroyWinRit;
            win->mDestroying = FALSE;
            if (mInputMethodWindow == win) {
                mInputMethodWindow = NULL;
            }
            if (win == mWallpaperTarget) {
                wallpaperDestroyed = TRUE;
            }
            win->mWinAnimator->DestroySurfaceLocked(FALSE);
        }
        mDestroySurface.Clear();
    }

    // Time to remove any exiting tokens?
    List< AutoPtr<WindowToken> >::ReverseIterator exitingTRIt = mExitingTokens.RBegin();
    for (; exitingTRIt != mExitingTokens.REnd();) {
        AutoPtr<WindowToken> token = *exitingTRIt;
        if (!token->mHasVisible) {
            if (token->mWindowType == IWindowManagerLayoutParams::TYPE_WALLPAPER) {
                mWallpaperTokens.Remove(token);
                UpdateLayoutToAnimWallpaperTokens();
            }

            exitingTRIt = List< AutoPtr<WindowToken> >::ReverseIterator(
                mExitingTokens.Erase(--(exitingTRIt.GetBase())));
        }
        else {
            ++exitingTRIt;
        }
    }

    // Time to remove any exiting applications?
    for (appWinRIt = mExitingAppTokens.RBegin(); appWinRIt != mExitingAppTokens.REnd();) {
        AutoPtr<AppWindowToken> token = *appWinRIt;
        List< AutoPtr<AppWindowToken> >::Iterator closingIt = mClosingApps.Begin();
        for (; closingIt != mClosingApps.End(); ++closingIt) {
            if (*closingIt == token) break;
        }
        if (!token->mHasVisible && closingIt == mClosingApps.End()) {
            // Make sure there is no animation running on this token,
            // so any windows associated with it will be removed as
            // soon as their animations are complete
            token->mAppAnimator->ClearAnimation();
            token->mAppAnimator->mAnimating = FALSE;
            if (DEBUG_ADD_REMOVE || DEBUG_TOKEN_MOVEMENT) {
                Slogger::V(TAG, "performLayout: App token exiting now removed %p", token.Get());
            }
            mAppTokens.Remove(token);
            mAnimatingAppTokens.Remove(token);
            appWinRIt = List< AutoPtr<AppWindowToken> >::ReverseIterator(
                mExitingAppTokens.Erase(--(appWinRIt.GetBase())));
        }
        else {
            ++appWinRIt;
        }
    }

    if (!mAnimator->mAnimating &&
            mRelayoutWhileAnimating.Begin() != mRelayoutWhileAnimating.End()) {
        List< AutoPtr<WindowState> >::ReverseIterator rit = mRelayoutWhileAnimating.RBegin();
        for (; rit != mRelayoutWhileAnimating.REnd(); ++rit) {
            // try {
            (*rit)->mClient->DoneAnimating();
            // } catch (RemoteException e) {
            // }
        }
        mRelayoutWhileAnimating.Clear();
    }

    if (wallpaperDestroyed && (AdjustWallpaperWindowsLocked() != 0)) {
        GetDefaultDisplayContentLocked()->mLayoutNeeded = TRUE;
    }

    for (it = mDisplayContents.Begin(); it != mDisplayContents.End(); ++it) {
        AutoPtr<DisplayContent> displayContent = it->mSecond;
        if (displayContent->mPendingLayoutChanges != 0) {
            displayContent->mLayoutNeeded = TRUE;
        }
    }

    // Finally update all input windows now that the window changes have stabilized.
    mInputMonitor->UpdateInputWindowsLw(TRUE /*force*/);

    SetHoldScreenLocked((CSession*)mInnerFields->mHoldScreen.Get());
    if (!mDisplayFrozen) {
        if (mInnerFields->mScreenBrightness < 0 || mInnerFields->mScreenBrightness > 1.0f) {
            mPowerManager->SetScreenBrightnessOverrideFromWindowManager(-1);
        }
        else {
            mPowerManager->SetScreenBrightnessOverrideFromWindowManager(
                    ToBrightnessOverride(mInnerFields->mScreenBrightness));
        }
        if (mInnerFields->mButtonBrightness < 0 || mInnerFields->mButtonBrightness > 1.0f) {
            mPowerManager->SetButtonBrightnessOverrideFromWindowManager(-1);
        }
        else {
            mPowerManager->SetButtonBrightnessOverrideFromWindowManager(
                    ToBrightnessOverride(mInnerFields->mButtonBrightness));
        }
        mPowerManager->SetUserActivityTimeoutOverrideFromWindowManager(
                mInnerFields->mUserActivityTimeout);
    }

    if (mTurnOnScreen) {
        if (DEBUG_VISIBILITY) Slogger::V(TAG, "Turning screen on after layout!");
        mPowerManager->WakeUp(SystemClock::GetUptimeMillis());
        mTurnOnScreen = FALSE;
    }

    if (mInnerFields->mUpdateRotation) {
        if (DEBUG_ORIENTATION) Slogger::D(TAG, "Performing post-rotate rotation");
        if (UpdateRotationUncheckedLocked(FALSE)) {
            Boolean result;
            mH->SendEmptyMessage(H::SEND_NEW_CONFIGURATION, &result);
        }
        else {
            mInnerFields->mUpdateRotation = FALSE;
        }
    }

    if (mInnerFields->mOrientationChangeComplete && !defaultDisplay->mLayoutNeeded
            && !mInnerFields->mUpdateRotation) {
        CheckDrawnWindowsLocked();
    }

    Int32 N = mPendingRemove.GetSize();
    if (N > 0) {
        if (mPendingRemoveTmp->GetLength() < N) {
            mPendingRemoveTmp = ArrayOf<WindowState*>::Alloc(N + 10);
        }
        List< AutoPtr<WindowState> >::Iterator winIt = mPendingRemove.Begin();
        Int32 i = 0;
        for (; winIt != mPendingRemove.End(); ++winIt, ++i) {
            mPendingRemoveTmp->Set(i, *winIt);
        }
        mPendingRemove.Clear();
        List< AutoPtr<DisplayContent> > displayList;
        List< AutoPtr<DisplayContent> >::Iterator dcit;
        for (i = 0; i < N; i++) {
            AutoPtr<WindowState> w = (*mPendingRemoveTmp)[i];
            RemoveWindowInnerLocked(w->mSession, w);
            for (dcit = displayList.Begin(); dcit != displayList.End(); ++dcit) {
                if (*dcit == w->mDisplayContent) break;
            }
            if (dcit == displayList.End()) {
                displayList.PushBack(w->mDisplayContent);
            }
        }

        for (dcit = displayList.Begin(); dcit != displayList.End(); ++dcit) {
            AssignLayersLocked((*dcit)->GetWindowList());
            (*dcit)->mLayoutNeeded = TRUE;
        }
    }

    // Check to see if we are now in a state where the screen should
    // be enabled, because the window obscured flags have changed.
    EnableScreenIfNeededLocked();

    UpdateLayoutToAnimationLocked();

    // if (DEBUG_WINDOW_TRACE) {
    //     Slog.e(TAG, "performLayoutAndPlaceSurfacesLockedInner exit: animating="
    //             + mAnimator.mAnimating);
    // }
}

Int32 CWindowManagerService::ToBrightnessOverride(
    /* [in] */ Float value)
{
    return (Int32)(value * IPowerManager::BRIGHTNESS_ON);
}

void CWindowManagerService::CheckDrawnWindowsLocked()
{
    if (mWaitingForDrawn.Begin() != mWaitingForDrawn.End()) {
        typedef Pair<AutoPtr<WindowState>, AutoPtr<IRemoteCallback> > WindowCallbackPair;
        typedef List< WindowCallbackPair > WindowCallBackList;
        WindowCallBackList::ReverseIterator rit = mWaitingForDrawn.RBegin();
        for (; rit != mWaitingForDrawn.REnd();) {
            WindowCallbackPair pair = *rit;
            AutoPtr<WindowState> win = pair.mFirst;
            //Slog.i(TAG, "Waiting for drawn " + win + ": removed="
            //        + win.mRemoved + " visible=" + win.isVisibleLw()
            //        + " shown=" + win.mSurfaceShown);
            Boolean isVisible;
            win->IsVisibleLw(&isVisible);
            if (win->mRemoved || !isVisible) {
                // Window has been removed or made invisible; no draw
                // will now happen, so stop waiting.
                Slogger::W(TAG, "Aborted waiting for drawn: %p", win.Get());
                // try {
                pair.mSecond->SendResult(NULL);
                // } catch (RemoteException e) {
                // }

                rit = WindowCallBackList::ReverseIterator(mWaitingForDrawn.Erase(--(rit.GetBase())));
                mH->RemoveMessages(H::WAITING_FOR_DRAWN_TIMEOUT);
                continue;
            }
            else if (win->mWinAnimator->mSurfaceShown) {
                // Window is now drawn (and shown).
                // try {
                pair.mSecond->SendResult(NULL);
                // } catch (RemoteException e) {
                // }
                rit = WindowCallBackList::ReverseIterator(mWaitingForDrawn.Erase(--(rit.GetBase())));
                mH->RemoveMessages(H::WAITING_FOR_DRAWN_TIMEOUT);
                continue;
            }
            else {
                ++rit;
            }
        }
    }
}

ECode CWindowManagerService::WaitForWindowDrawn(
    /* [in] */ IBinder* token,
    /* [in] */ IRemoteCallback* callback,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (token != NULL && callback != NULL) {
        AutoLock lock(mWindowMapLock);
        AutoPtr<WindowState> win;
        WindowForClientLocked(NULL, token, TRUE, (WindowState**)&win);
        if (win != NULL) {
            AutoPtr<SomeArgs> args = SomeArgs::Obtain();
            args->mArg1 = win.Get();
            args->mArg2 = callback;

            AutoPtr<IMessage> msg;
            mH->ObtainMessage(H::WAITING_FOR_DRAWN_TIMEOUT, args, (IMessage**)&msg);
            Boolean result;
            mH->SendMessageDelayed(msg, 2000, &result);

            Pair<AutoPtr<WindowState>, AutoPtr<IRemoteCallback> > pair(win, callback);
            mWaitingForDrawn.PushBack(pair);
            CheckDrawnWindowsLocked();
            return TRUE;
        }
    }
   return FALSE;
}

void CWindowManagerService::SetHoldScreenLocked(
    /* [in] */ CSession* newHoldScreen)
{
    Boolean hold = newHoldScreen != NULL;

    if (hold && mHoldingScreenOn.Get() != newHoldScreen) {
        AutoPtr<IWorkSource> workSource;
        CWorkSource::New(newHoldScreen->mUid, (IWorkSource**)&workSource);
        mHoldingScreenWakeLock->SetWorkSource(workSource);
    }
    mHoldingScreenOn = newHoldScreen;

    Boolean state;
    mHoldingScreenWakeLock->IsHeld(&state);
    if (hold != state) {
        if (hold) {
            mHoldingScreenWakeLock->AcquireLock();
            mPolicy->KeepScreenOnStartedLw();
        }
        else {
            mPolicy->KeepScreenOnStoppedLw();
            mHoldingScreenWakeLock->ReleaseLock();
        }
    }
}

ECode CWindowManagerService::RequestTraversal()
{
    AutoLock lock(mWindowMapLock);
    RequestTraversalLocked();
    return NOERROR;
}

void CWindowManagerService::RequestTraversalLocked()
{
    if (!mTraversalScheduled) {
        mTraversalScheduled = TRUE;
        Boolean result;
        mH->SendEmptyMessage(H::DO_TRAVERSAL, &result);
    }
}

void CWindowManagerService::ScheduleAnimationLocked()
{
    AutoPtr<LayoutToAnimatorParams> layoutToAnim = mLayoutToAnim;
    if (!layoutToAnim->mAnimationScheduled) {
        layoutToAnim->mAnimationScheduled = TRUE;
        mChoreographer->PostCallback(
                IChoreographer::CALLBACK_ANIMATION, mAnimator->mAnimationRunnable, NULL);
    }
}

void CWindowManagerService::UpdateLayoutToAnimationLocked()
{
    AutoPtr<LayoutToAnimatorParams> layoutToAnim = mLayoutToAnim;
    AutoLock lock(mLayoutToAnimLock);
    // Copy local params to transfer params.
    HashMap<Int32, AutoPtr<List<AutoPtr<WindowStateAnimator> > > >& allWinAnimatorLists = layoutToAnim->mWinAnimatorLists;
    allWinAnimatorLists.Clear();

    HashMap<Int32, AutoPtr<DisplayContent> >::Iterator it;
    for (it = mDisplayContents.Begin(); it != mDisplayContents.End(); ++it) {
        AutoPtr<DisplayContent> displayContent = it->mSecond;
        AutoPtr<List<AutoPtr<WindowStateAnimator> > > winAnimatorList = new List< AutoPtr<WindowStateAnimator> >;
        List< AutoPtr<WindowState> >& windows = displayContent->GetWindowList();
        List< AutoPtr<WindowState> >::Iterator winIt;
        for (winIt = windows.Begin(); winIt != windows.End(); ++winIt) {
            AutoPtr<WindowStateAnimator> winAnimator = (*winIt)->mWinAnimator;
            if (winAnimator->mSurface != NULL) {
                winAnimatorList->PushBack(winAnimator);
            }
        }

#if defined(_DEBUG)
        HashMap<Int32, AutoPtr<List<AutoPtr<WindowStateAnimator> > > >::Iterator animIter;
        animIter = allWinAnimatorLists.Find(displayContent->GetDisplayId());
        assert(animIter == allWinAnimatorLists.End());
#endif
        allWinAnimatorLists[displayContent->GetDisplayId()] = winAnimatorList;
    }

    // if (WindowManagerService.DEBUG_WALLPAPER_LIGHT) {
    //     if (mWallpaperTarget != layoutToAnim.mWallpaperTarget
    //             || mLowerWallpaperTarget != layoutToAnim.mLowerWallpaperTarget
    //             || mUpperWallpaperTarget != layoutToAnim.mUpperWallpaperTarget) {
    //         Slog.d(TAG, "Pushing anim wallpaper: target=" + mWallpaperTarget
    //                 + " lower=" + mLowerWallpaperTarget + " upper="
    //                 + mUpperWallpaperTarget + "\n" + Debug.getCallers(5, "  "));
    //     }
    // }
    layoutToAnim->mWallpaperTarget = mWallpaperTarget;
    layoutToAnim->mLowerWallpaperTarget = mLowerWallpaperTarget;
    layoutToAnim->mUpperWallpaperTarget = mUpperWallpaperTarget;

    List< AutoPtr<AppWindowAnimParams> >& paramList = layoutToAnim->mAppWindowAnimParams;
    paramList.Clear();
    List< AutoPtr<AppWindowToken> >::Iterator appWinIt = mAnimatingAppTokens.Begin();
    for (; appWinIt != mAnimatingAppTokens.End(); ++appWinIt) {
        AutoPtr<AppWindowAnimParams> params = new AppWindowAnimParams((*appWinIt)->mAppAnimator);
        paramList.PushBack(params);
    }

    layoutToAnim->mParamsModified = TRUE;
    ScheduleAnimationLocked();
}

void CWindowManagerService::UpdateLayoutToAnimWallpaperTokens()
{
    AutoLock lock(mLayoutToAnimLock);
    mLayoutToAnim->mWallpaperTokens.Clear();
    mLayoutToAnim->mWallpaperTokens.Insert(mLayoutToAnim->mWallpaperTokens.Begin(), mWallpaperTokens.Begin(), mWallpaperTokens.End());
    mLayoutToAnim->mChanges |= LayoutToAnimatorParams::WALLPAPER_TOKENS_CHANGED;
}

void CWindowManagerService::SetAnimDimParams(
    /* [in] */ Int32 displayId,
    /* [in] */ DimAnimator::Parameters* params)
{
    AutoLock lock(mLayoutToAnimLock);
    mLayoutToAnim->mDimParams[displayId] = params;
    ScheduleAnimationLocked();
}

void CWindowManagerService::StartDimmingLocked(
    /* [in] */ WindowStateAnimator* winAnimator,
    /* [in] */ Float target,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    SetAnimDimParams(winAnimator->mWin->GetDisplayId(),
            new DimAnimator::Parameters(winAnimator, width, height, target));
}

void CWindowManagerService::StopDimmingLocked(
    /* [in] */ Int32 displayId)
{
    SetAnimDimParams(displayId, NULL);
}

Boolean CWindowManagerService::NeedsLayout()
{
    HashMap<Int32, AutoPtr<DisplayContent> >::Iterator it;
    for (it = mDisplayContents.Begin(); it != mDisplayContents.End(); ++it) {
        if (it->mSecond->mLayoutNeeded) {
            return TRUE;
        }
    }
    return FALSE;
}

Boolean CWindowManagerService::CopyAnimToLayoutParamsLocked()
{
    Boolean doRequest = FALSE;
    AutoPtr<WindowAnimator::AnimatorToLayoutParams> animToLayout = mAnimator->mAnimToLayout;
    AutoLock lock(mAnimator->mAnimToLayoutLock);
    animToLayout->mUpdateQueued = FALSE;
    Int32 bulkUpdateParams = animToLayout->mBulkUpdateParams;
    // TODO(cmautner): As the number of bits grows, use masks of bit groups to
    //  eliminate unnecessary tests.
    if ((bulkUpdateParams & LayoutFields::SET_UPDATE_ROTATION) != 0) {
        mInnerFields->mUpdateRotation = TRUE;
        doRequest = TRUE;
    }
    if ((bulkUpdateParams & LayoutFields::SET_WALLPAPER_MAY_CHANGE) != 0) {
        mInnerFields->mWallpaperMayChange = TRUE;
        doRequest = TRUE;
    }
    if ((bulkUpdateParams & LayoutFields::SET_FORCE_HIDING_CHANGED) != 0) {
        mInnerFields->mWallpaperForceHidingChanged = TRUE;
        doRequest = TRUE;
    }
    if ((bulkUpdateParams & LayoutFields::SET_ORIENTATION_CHANGE_COMPLETE) == 0) {
        mInnerFields->mOrientationChangeComplete = FALSE;
    }
    else {
        mInnerFields->mOrientationChangeComplete = TRUE;
        if (mWindowsFreezingScreen) {
            doRequest = TRUE;
        }
    }
    if ((bulkUpdateParams & LayoutFields::SET_TURN_ON_SCREEN) != 0) {
        mTurnOnScreen = TRUE;
    }

    AutoPtr<HashMap<Int32, Int32> > pendingLayouts = animToLayout->mPendingLayoutChanges;
    if (pendingLayouts->Begin() != pendingLayouts->End()) {
        doRequest = TRUE;
    }
    HashMap<Int32, Int32>::Iterator it = pendingLayouts->Begin();
    for (; it != pendingLayouts->End(); ++it) {
        AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(it->mFirst);
        if (displayContent != NULL) {
            displayContent->mPendingLayoutChanges |= it->mSecond;
        }
    }

    mWindowDetachedWallpaper = animToLayout->mWindowDetachedWallpaper;

    return doRequest;
}

Boolean CWindowManagerService::ReclaimSomeSurfaceMemoryLocked(
    /* [in] */ WindowStateAnimator* winAnimator,
    /* [in] */ const String& operation,
    /* [in] */ Boolean secure)
{
    AutoPtr<ISurface> surface = winAnimator->mSurface;
    Boolean leakedSurface = FALSE;
    Boolean killedApps = FALSE;

    // EventLog.writeEvent(EventLogTags.WM_NO_SURFACE_MEMORY, winAnimator.mWin.toString(),
    //         winAnimator.mSession.mPid, operation);

    if (mForceRemoves == NULL) {
        mForceRemoves = new List< AutoPtr<WindowState> >();
    }

    Int64 callingIdentity = Binder::ClearCallingIdentity();
    // try {
        // There was some problem...   first, do a sanity check of the
        // window list to make sure we haven't left any dangling surfaces
        // around.

    Slogger::I(TAG, "Out of memory for surface!  Looking for leaks...");
    HashMap<Int32, AutoPtr<DisplayContent> >::Iterator it;
    for (it = mDisplayContents.Begin(); it != mDisplayContents.End(); ++it) {
        AutoPtr<DisplayContent> content = it->mSecond;
        if (content != NULL) {
            List< AutoPtr<WindowState> >& windows = content->GetWindowList();
            if (windows.Begin() != windows.End()) {
                List< AutoPtr<WindowState> >::Iterator it;
                for (it = windows.Begin(); it != windows.End(); ++it) {
                    AutoPtr<WindowState> ws = *it;
                    AutoPtr<WindowStateAnimator> wsa = ws->mWinAnimator;
                    if (wsa->mSurface != NULL) {
                        HashSet<AutoPtr<IWindowSession> >::Iterator sessionIt
                                = mSessions.Find(wsa->mSession);
                        if (sessionIt == mSessions.End()) {
                            Slogger::W(TAG, "LEAKED SURFACE (session doesn't exist): %p surface=%p token=%p pid=%d uid=%d"
                                    , ws.Get(), wsa->mSurface.Get(), ws->mToken.Get()
                                    , ws->mSession->mPid, ws->mSession->mUid);
                            // if (SHOW_TRANSACTIONS) logSurface(ws, "LEAK DESTROY", null);
                            wsa->mSurface->Destroy();
                            wsa->mSurfaceShown = FALSE;
                            wsa->mSurface = NULL;
                            ws->mHasSurface = FALSE;
                            mForceRemoves->PushBack(ws);
                            leakedSurface = TRUE;
                        }
                        else if (ws->mAppToken != NULL && ws->mAppToken->mClientHidden) {
                            Slogger::W(TAG, "LEAKED SURFACE (app token hidden): %p surface=%p token=%p"
                                    , ws.Get(), wsa->mSurface.Get(), ws->mAppToken);
                            // if (SHOW_TRANSACTIONS) logSurface(ws, "LEAK DESTROY", null);
                            wsa->mSurface->Destroy();
                            wsa->mSurfaceShown = FALSE;
                            wsa->mSurface = NULL;
                            ws->mHasSurface = FALSE;
                            leakedSurface = TRUE;
                        }
                    }
                }
            }
        }
    }

    if (!leakedSurface) {
        Slogger::W(TAG, "No leaked surfaces; killing applicatons!");
        HashMap<Int32, Int32> pidCandidates;
        for (it = mDisplayContents.Begin(); it != mDisplayContents.End(); ++it) {
            AutoPtr<DisplayContent> content = it->mSecond;
            if (content != NULL) {
                List< AutoPtr<WindowState> >& windows = content->GetWindowList();
                if (windows.Begin() != windows.End()) {
                    List< AutoPtr<WindowState> >::Iterator it;
                    for (it = windows.Begin(); it != windows.End(); ++it) {
                        AutoPtr<WindowState> ws = *it;
                        List< AutoPtr<WindowState> >::Iterator winIt;
                        for (winIt = mForceRemoves->Begin(); winIt != mForceRemoves->End(); ++winIt) {
                            if (*winIt == ws) break;
                        }
                        if (winIt != mForceRemoves->End()) {
                            continue;
                        }
                        AutoPtr<WindowStateAnimator> wsa = ws->mWinAnimator;
                        if (wsa->mSurface != NULL) {
                            pidCandidates[wsa->mSession->mPid] = wsa->mSession->mPid;
                        }
                    }
                }
            }
        }
        if (pidCandidates.Begin() != pidCandidates.End()) {
            AutoPtr< ArrayOf<Int32> > pids = ArrayOf<Int32>::Alloc(pidCandidates.GetSize());
            HashMap<Int32, Int32>::Iterator pidIt = pidCandidates.Begin();
            for (Int32 i = 0; i < pids->GetLength(); i++) {
                (*pids)[i] = pidIt->mFirst;
                ++pidIt;
            }
            // try {
            Boolean result;
            mActivityManager->KillPids(pids, String("Free memory"), secure, &result);
            if (result) {
                killedApps = TRUE;
            }
            // } catch (RemoteException e) {
            // }
        }
    }

    if (leakedSurface || killedApps) {
        // We managed to reclaim some memory, so get rid of the trouble
        // surface and ask the app to request another one.
        // Slog.w(TAG, "Looks like we have reclaimed some memory, clearing surface for retry.");
        if (surface != NULL) {
            // if (SHOW_TRANSACTIONS || SHOW_SURFACE_ALLOC) logSurface(winAnimator.mWin,
            //         "RECOVER DESTROY", null);
            surface->Destroy();
            winAnimator->mSurfaceShown = FALSE;
            winAnimator->mSurface = NULL;
            winAnimator->mWin->mHasSurface = FALSE;
        }

        // try {
        winAnimator->mWin->mClient->DispatchGetNewSurface();
        // } catch (RemoteException e) {
        // }
    }
    // } finally {
    //     Binder::RestoreCallingIdentity(callingIdentity);
    // }
    Binder::RestoreCallingIdentity(callingIdentity);

    return leakedSurface || killedApps;
}

Boolean CWindowManagerService::UpdateFocusedWindowLocked(
    /* [in] */ Int32 mode,
    /* [in] */ Boolean updateInputWindows)
{
    AutoPtr<WindowState> newFocus = ComputeFocusedWindowLocked();
    if (mCurrentFocus != newFocus) {
        // AutoPtr<ITrace> trace;
        // CTrace::AcquireSingleton((ITrace**)&trace);
        // trace->TraceBegin(ITrace::TRACE_TAG_WINDOW_MANAGER, String("wmUpdateFocus"));
        // This check makes sure that we don't already have the focus
        // change message pending.
        Boolean result;
        mH->RemoveMessages(H::REPORT_FOCUS_CHANGE);
        mH->SendEmptyMessage(H::REPORT_FOCUS_CHANGE, &result);
        // if (localLOGV) Slog.v(
        //     TAG, "Changing focus from " + mCurrentFocus + " to " + newFocus);
        AutoPtr<WindowState> oldFocus = mCurrentFocus;
        mCurrentFocus = newFocus;
        mAnimator->SetCurrentFocus(newFocus);
        mLosingFocus.Remove(newFocus);
        Int32 focusChanged;
        mPolicy->FocusChangedLw(oldFocus, newFocus, &focusChanged);

        // TODO(multidisplay): Focused windows on default display only.
        AutoPtr<DisplayContent> displayContent = GetDefaultDisplayContentLocked();

        AutoPtr<WindowState> imWindow = mInputMethodWindow;
        if (newFocus != imWindow && oldFocus != imWindow) {
            if (MoveInputMethodWindowsIfNeededLocked(
                    mode != UPDATE_FOCUS_WILL_ASSIGN_LAYERS &&
                    mode != UPDATE_FOCUS_WILL_PLACE_SURFACES)) {
                displayContent->mLayoutNeeded = TRUE;
            }
            if (mode == UPDATE_FOCUS_PLACING_SURFACES) {
                PerformLayoutLockedInner(displayContent, TRUE /*initial*/, updateInputWindows);
                focusChanged &= ~IWindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT;
            }
            else if (mode == UPDATE_FOCUS_WILL_PLACE_SURFACES) {
                // Client will do the layout, but we need to assign layers
                // for handleNewWindowLocked() below.
                AssignLayersLocked(displayContent->GetWindowList());
            }
        }

        if ((focusChanged & IWindowManagerPolicy::FINISH_LAYOUT_REDO_LAYOUT) != 0) {
            // The change in focus caused us to need to do a layout.  Okay.
            displayContent->mLayoutNeeded = TRUE;
            if (mode == UPDATE_FOCUS_PLACING_SURFACES) {
                PerformLayoutLockedInner(displayContent, TRUE /*initial*/, updateInputWindows);
            }
        }

        if (mode != UPDATE_FOCUS_WILL_ASSIGN_LAYERS) {
            // If we defer assigning layers, then the caller is responsible for
            // doing this part.
            FinishUpdateFocusedWindowAfterAssignLayersLocked(updateInputWindows);
        }

        // tracse->TraceEnd(ITrace::TRACE_TAG_WINDOW_MANAGER);
        return TRUE;
    }
    return FALSE;
}

void CWindowManagerService::FinishUpdateFocusedWindowAfterAssignLayersLocked(
    /* [in] */ Boolean updateInputWindows)
{
    mInputMonitor->SetInputFocusLw(mCurrentFocus, updateInputWindows);
}

AutoPtr<WindowState> CWindowManagerService::ComputeFocusedWindowLocked()
{
    if (mAnimator->mUniverseBackground != NULL
            && mAnimator->mUniverseBackground->mWin->CanReceiveKeys()) {
        return mAnimator->mUniverseBackground->mWin;
    }

    HashMap<Int32, AutoPtr<DisplayContent> >::Iterator it = mDisplayContents.Begin();
    for (; it != mDisplayContents.End(); ++it) {
        AutoPtr<DisplayContent> displayContent = it->mSecond;
        AutoPtr<WindowState> win = FindFocusedWindowLocked(displayContent);
        if (win != NULL) {
            return win;
        }
    }
    return NULL;
}

AutoPtr<WindowState> CWindowManagerService::FindFocusedWindowLocked(
    /* [in] */ DisplayContent* displayContent)
{
    List< AutoPtr<AppWindowToken> >::Iterator nextAppIt = --mAppTokens.End();
    AutoPtr<WindowToken> nextApp = mAppTokens.Begin() != mAppTokens.End() ?
            *nextAppIt : NULL;

    List< AutoPtr<WindowState> > windows = displayContent->GetWindowList();
    List< AutoPtr<WindowState> >::ReverseIterator rit;
    for (rit = windows.RBegin(); rit != windows.REnd(); ++rit) {
        AutoPtr<WindowState> win = *rit;

        // if (localLOGV || DEBUG_FOCUS) Slog.v(
        //     TAG, "Looking for focus: " + i
        //     + " = " + win
        //     + ", flags=" + win.mAttrs.flags
        //     + ", canReceive=" + win.canReceiveKeys());

        AutoPtr<AppWindowToken> thisApp = win->mAppToken;

        // If this window's application has been removed, just skip it.
        if (thisApp != NULL && (thisApp->mRemoved || thisApp->mSendingToBottom)) {
            // if (DEBUG_FOCUS) Slog.v(TAG, "Skipping app because " + (thisApp.removed
            //         ? "removed" : "sendingToBottom"));
            continue;
        }

        // If there is a focused app, don't allow focus to go to any
        // windows below it.  If this is an application window, step
        // through the app tokens until we find its app.
        Int32 type;
        if (thisApp != NULL && nextApp != NULL && thisApp != nextApp
                && (win->mAttrs->GetType(&type), type != IWindowManagerLayoutParams::TYPE_APPLICATION_STARTING)) {
            List< AutoPtr<AppWindowToken> >::Iterator origAppIt = nextAppIt;
            while (nextAppIt != mAppTokens.Begin()) {
                if (nextApp == mFocusedApp) {
                    // Whoops, we are below the focused app...  no focus
                    // for you!
                    // if (localLOGV || DEBUG_FOCUS) Slog.v(
                    //     TAG, "Reached focused app: " + mFocusedApp);
                    return NULL;
                }
                nextAppIt--;
                nextApp = *nextAppIt;
                if (nextApp == thisApp) {
                    break;
                }
            }
            if (thisApp != nextApp) {
                // Uh oh, the app token doesn't exist!  This shouldn't
                // happen, but if it does we can get totally hosed...
                // so restart at the original app.
                nextAppIt = origAppIt;
                nextApp = *nextAppIt;
            }
        }

        // Dispatch to this window if it is wants key events.
        if (win->CanReceiveKeys()) {
            // if (DEBUG_FOCUS) Slog.v(
            //         TAG, "Found focus @ " + i + " = " + win);
            return win;
        }
    }
    return NULL;
}

void CWindowManagerService::StartFreezingDisplayLocked(
    /* [in] */ Boolean inTransaction,
    /* [in] */ Int32 exitAnim,
    /* [in] */ Int32 enterAnim)
{
    if (mDisplayFrozen) {
        return;
    }

    Boolean isON;
    mPolicy->IsScreenOnFully(&isON);
    if (!mDisplayReady || !isON) {
        // No need to freeze the screen before the system is ready or if
        // the screen is off.
        return;
    }

    mScreenFrozenLock->AcquireLock();

    mDisplayFrozen = TRUE;

    mInputMonitor->FreezeInputDispatchingLw();

    // Clear the last input window -- that is just used for
    // clean transitions between IMEs, and if we are freezing
    // the screen then the whole world is changing behind the scenes.
    mPolicy->SetLastInputMethodWindowLw(NULL, NULL);

    if (mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET) {
        mNextAppTransition = IWindowManagerPolicy::TRANSIT_UNSET;
        mNextAppTransitionType = IActivityOptions::ANIM_NONE;
        mNextAppTransitionPackage = NULL;
        mNextAppTransitionThumbnail = NULL;
        mAppTransitionReady = TRUE;
    }

    // if (PROFILE_ORIENTATION) {
    //     File file = new File("/data/system/frozen");
    //     Debug.startMethodTracing(file.toString(), 8 * 1024 * 1024);
    // }

    if (CUSTOM_SCREEN_ROTATION) {
        AutoPtr<DisplayContent> displayContent = GetDefaultDisplayContentLocked();
        Int32 displayId = displayContent->GetDisplayId();
        AutoPtr<ScreenRotationAnimation> screenRotationAnimation =
                mAnimator->GetScreenRotationAnimationLocked(displayId);
        if (screenRotationAnimation != NULL) {
            screenRotationAnimation->Kill();
        }

        // TODO(multidisplay): rotation on main screen only.
        AutoPtr<IDisplay> display = displayContent->GetDisplay();
        AutoPtr<IDisplayInfo> displayInfo = displayContent->GetDisplayInfo();
        Int32 width, height, rotation;
        displayInfo->GetLogicalWidth(&width);
        displayInfo->GetLogicalHeight(&height);
        displayInfo->GetRotation(&rotation);
        screenRotationAnimation = new ScreenRotationAnimation(mContext,
                display, mFxSession, inTransaction, width,
                height, rotation, exitAnim, enterAnim);
        mAnimator->SetScreenRotationAnimationLocked(displayId, screenRotationAnimation);
    }
}

void CWindowManagerService::StopFreezingDisplayLocked()
{
    if (!mDisplayFrozen) {
        return;
    }

    if (mWaitingForConfig || mAppsFreezingScreen > 0 || mWindowsFreezingScreen
            || mClientFreezingScreen) {
        // if (DEBUG_ORIENTATION) Slog.d(TAG,
        //     "stopFreezingDisplayLocked: Returning mWaitingForConfig=" + mWaitingForConfig
        //     + ", mAppsFreezingScreen=" + mAppsFreezingScreen
        //     + ", mWindowsFreezingScreen=" + mWindowsFreezingScreen
        //     + ", mClientFreezingScreen=" + mClientFreezingScreen);
        return;
    }

    mDisplayFrozen = FALSE;
    mH->RemoveMessages(H::APP_FREEZE_TIMEOUT);
    mH->RemoveMessages(H::CLIENT_FREEZE_TIMEOUT);

    // if (PROFILE_ORIENTATION) {
    //     Debug.stopMethodTracing();
    // }

    Boolean updateRotation = FALSE;

    AutoPtr<DisplayContent> displayContent = GetDefaultDisplayContentLocked();
    Int32 displayId = displayContent->GetDisplayId();
    AutoPtr<ScreenRotationAnimation> screenRotationAnimation =
            mAnimator->GetScreenRotationAnimationLocked(displayId);
    if (CUSTOM_SCREEN_ROTATION && screenRotationAnimation != NULL
            && screenRotationAnimation->HasScreenshot()) {
        // if (DEBUG_ORIENTATION) Slog.i(TAG, "**** Dismissing screen rotation animation");
        // TODO(multidisplay): rotation on main screen only.
        AutoPtr<IDisplayInfo> displayInfo = displayContent->GetDisplayInfo();
        Int32 width, height;
        displayInfo->GetLogicalWidth(&width);
        displayInfo->GetLogicalHeight(&height);
        if (screenRotationAnimation->Dismiss(mFxSession, MAX_ANIMATION_DURATION,
                mTransitionAnimationScale, width, height)) {
            UpdateLayoutToAnimationLocked();
        }
        else {
            screenRotationAnimation->Kill();
            screenRotationAnimation = NULL;
            mAnimator->SetScreenRotationAnimationLocked(displayId, screenRotationAnimation);
            updateRotation = TRUE;
        }
    }
    else {
        if (screenRotationAnimation != NULL) {
            screenRotationAnimation->Kill();
            screenRotationAnimation = NULL;
            mAnimator->SetScreenRotationAnimationLocked(displayId, screenRotationAnimation);
        }
        updateRotation = TRUE;
    }

    mInputMonitor->ThawInputDispatchingLw();

    // While the display is frozen we don't re-compute the orientation
    // to avoid inconsistent states.  However, something interesting
    // could have actually changed during that time so re-evaluate it
    // now to catch that.
    Boolean configChanged = UpdateOrientationFromAppTokensLocked(FALSE);

    // A little kludge: a lot could have happened while the
    // display was frozen, so now that we are coming back we
    // do a gc so that any remote references the system
    // processes holds on others can be released if they are
    // no longer needed.
    Boolean result;
    mH->RemoveMessages(H::FORCE_GC);
    mH->SendEmptyMessageDelayed(H::FORCE_GC, 2000, &result);

    mScreenFrozenLock->ReleaseLock();

    if (updateRotation) {
        // if (DEBUG_ORIENTATION) Slog.d(TAG, "Performing post-rotate rotation");
        configChanged |= UpdateRotationUncheckedLocked(FALSE);
    }

    if (configChanged) {
        Boolean result;
        mH->SendEmptyMessage(H::SEND_NEW_CONFIGURATION, &result);
    }
}

Int32 CWindowManagerService::GetPropertyInt(
    /* [in] */ ArrayOf<String>* tokens,
    /* [in] */ Int32 index,
    /* [in] */ Int32 defUnits,
    /* [in] */ Int32 defDps,
    /* [in] */ IDisplayMetrics* dm)
{
    if (index < tokens->GetLength()) {
        String str = (*tokens)[index];
        if (!str.IsNullOrEmpty()) {
           // try {
            Int32 val = StringUtils::ParseInt32(str);
            return val;
           // } catch (Exception e) {
           // }
        }
    }
    if (defUnits == ITypedValue::COMPLEX_UNIT_PX) {
        return defDps;
    }
    AutoPtr<ITypedValueHelper> helper;
    CTypedValueHelper::AcquireSingleton((ITypedValueHelper**)&helper);
    Float val;
    helper->ApplyDimension(defUnits, defDps, dm, &val);
    return (Int32)val;
}

void CWindowManagerService::CreateWatermarkInTransaction()
{
    if (mWatermark != NULL) {
        return;
    }

    AutoPtr<IFile> file;
    CFile::New(String("/system/etc/setup.conf"), (IFile**)&file);
    AutoPtr<IFileInputStream> in;
    AutoPtr<IDataInputStream> ind;
    String line;
    // try {
    if (FAILED(CFileInputStream::New(file, (IFileInputStream**)&in))) {
        goto fail;
    }
    if (FAILED(CDataInputStream::New((IInputStream*)in.Get(), (IDataInputStream**)&ind))) {
        goto fail;
    }
    if (FAILED(IDataInput::Probe(ind.Get())->ReadLine(&line))) {
        goto fail;
    }
    if (!line.IsNull()) {
        AutoPtr< ArrayOf<String> > toks;
        StringUtils::Split(line, String("%"), (ArrayOf<String>**)&toks);
        if (toks != NULL && toks->GetLength() > 0) {
            mWatermark = new Watermark(GetDefaultDisplayContentLocked()->GetDisplay(),
                    mRealDisplayMetrics, mFxSession, toks);
        }
    }
    // } catch (FileNotFoundException e) {
    // } catch (IOException e) {
    // } finally {
    //     if (in != null) {
    //         try {
    //             in.close();
    //         } catch (IOException e) {
    //         }
    //     }
    // }
fail:
    if (in != NULL) {
        // try {
        ICloseable::Probe(in.Get())->Close();
        // } catch (IOException e) {
        // }
    }
}

ECode CWindowManagerService::StatusBarVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    Int32 perm;
    FAIL_RETURN(mContext->CheckCallingOrSelfPermission(
            Elastos::Droid::Manifest::Permission::STATUS_BAR,
            &perm));
    if (perm != IPackageManager::PERMISSION_GRANTED) {
        Slogger::E(TAG, "Caller does not hold permission android.permission.STATUS_BAR");
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Caller does not hold permission "
        //         + Elastos::Droid::Manifest::Permission::STATUS_BAR);
    }

    AutoLock lock(mWindowMapLock);
    mLastStatusBarVisibility = visibility;
    mPolicy->AdjustSystemUiVisibilityLw(visibility, &visibility);
    UpdateStatusBarVisibilityLocked(visibility);

    return NOERROR;
}

void CWindowManagerService::UpdateStatusBarVisibilityLocked(
    /* [in] */ Int32 visibility)
{
    mInputManager->SetSystemUiVisibility(visibility);
    List< AutoPtr<WindowState> >& windows = GetDefaultWindowListLocked();
    List< AutoPtr<WindowState> >::Iterator it = windows.Begin();
    for (; it != windows. End(); ++it) {
        AutoPtr<WindowState> ws = *it;
        // try {
        Int32 curValue = ws->mSystemUiVisibility;
        Int32 diff = curValue ^ visibility;
        // We are only interested in differences of one of the
        // clearable flags...
        diff &= IView::SYSTEM_UI_CLEARABLE_FLAGS;
        // ...if it has actually been cleared.
        diff &= ~visibility;
        Int32 newValue = (curValue & ~diff) | (visibility & diff);
        if (newValue != curValue) {
            ws->mSeq++;
            ws->mSystemUiVisibility = newValue;
        }
        Boolean hasSystemUiListeners;
        ws->mAttrs->GetHasSystemUiListeners(&hasSystemUiListeners);
        if (newValue != curValue || hasSystemUiListeners) {
            ws->mClient->DispatchSystemUiVisibilityChanged(ws->mSeq,
                    visibility, newValue, diff);
        }
        // } catch (RemoteException e) {
        //     // so sorry
        // }
    }
}

ECode CWindowManagerService::ReevaluateStatusBarVisibility()
{
    AutoLock lock(mWindowMapLock);
    Int32 visibility;
    mPolicy->AdjustSystemUiVisibilityLw(mLastStatusBarVisibility, &visibility);
    UpdateStatusBarVisibilityLocked(visibility);
    PerformLayoutAndPlaceSurfacesLocked();

    return NOERROR;
}

ECode CWindowManagerService::AddFakeWindow(
    /* [in] */ ILooper* looper,
    /* [in] */ IInputEventReceiverFactory* inputEventReceiverFactory,
    /* [in] */ const String& name,
    /* [in] */ Int32 windowType,
    /* [in] */ Int32 layoutParamsFlags,
    /* [in] */ Boolean canReceiveKeys,
    /* [in] */ Boolean hasFocus,
    /* [in] */ Boolean touchFullscreen,
    /* [out] */ IFakeWindow** fakeWindow)
{
    VALIDATE_NOT_NULL(fakeWindow);
    AutoLock lock(mWindowMapLock);
    AutoPtr<FakeWindowImpl> fw = new FakeWindowImpl(this, looper, inputEventReceiverFactory,
            name, windowType,
            layoutParamsFlags, canReceiveKeys, hasFocus, touchFullscreen);
    List< AutoPtr<FakeWindowImpl> >::Iterator it = mFakeWindows.Begin();
    while (it != mFakeWindows.End()) {
        if ((*it)->mWindowLayer <= fw->mWindowLayer) {
            break;
        }
        it++;
    }
    mFakeWindows.Insert(it, fw);
    mInputMonitor->UpdateInputWindowsLw(TRUE);
    *fakeWindow = (IFakeWindow*)fw;
    REFCOUNT_ADD(*fakeWindow);

    return NOERROR;
}

Boolean CWindowManagerService::RemoveFakeWindowLocked(
    /* [in] */ IFakeWindow* window)
{
    AutoLock lock(mWindowMapLock);
    List< AutoPtr<FakeWindowImpl> >::Iterator it = mFakeWindows.Begin();
    for (; it != mFakeWindows.End(); ++it) {
        if ((*it).Get() == window) {
            mFakeWindows.Erase(it);
            mInputMonitor->UpdateInputWindowsLw(TRUE);
            return TRUE;
        }
    }
    return FALSE;
}

void CWindowManagerService::SaveLastInputMethodWindowForTransition()
{
    AutoLock lock(mWindowMapLock);
    // TODO(multidisplay): Pass in the displayID.
    AutoPtr<DisplayContent> displayContent = GetDefaultDisplayContentLocked();
    if (mInputMethodWindow != NULL) {
        mPolicy->SetLastInputMethodWindowLw(mInputMethodWindow, mInputMethodTarget);
    }
}

ECode CWindowManagerService::HasNavigationBar(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mPolicy->HasNavigationBar(result);
}

ECode CWindowManagerService::LockNow(
    /* [in] */ IBundle* options)
{
    return mPolicy->LockNow(options);
}

ECode CWindowManagerService::IsSafeModeEnabled(
    /* [out] */ Boolean* isSafe)
{
    VALIDATE_NOT_NULL(isSafe);
    *isSafe = mSafeMode;
    return NOERROR;
}

ECode CWindowManagerService::ShowAssistant()
{
    // TODO: What permission?
    Int32 perm;
    FAIL_RETURN(mContext->CheckCallingOrSelfPermission(Elastos::Droid::Manifest::Permission::DEVICE_POWER,
            &perm));
    if (perm != IPackageManager::PERMISSION_GRANTED) {
        return NOERROR;
    }
    return mPolicy->ShowAssistant();
}

ECode CWindowManagerService::KeyEnterMouseMode()
{
    return mInputManager->KeyEnterMouseMode();
}

ECode CWindowManagerService::KeyExitMouseMode()
{
    return mInputManager->KeyExitMouseMode();
}

ECode CWindowManagerService::KeySetMouseMoveCode(int left,int right,int top,int bottom)
{
    return mInputManager->KeySetMouseMoveCode(left, right, top, bottom);
}

ECode CWindowManagerService::KeySetMouseBtnCode(int leftbtn,int midbtn,int rightbtn)
{
    return mInputManager->KeySetMouseBtnCode(leftbtn, midbtn, rightbtn);
}

ECode CWindowManagerService::KeySetMouseDistance(int distance)
{
    return mInputManager->KeySetMouseDistance(distance);
}

ECode CWindowManagerService::ResetInputCalibration()
{
    return mInputManager->ResetTouchCalibration();
}

void CWindowManagerService::SaveANRStateLocked(
    /* [in] */ AppWindowToken* appWindowToken,
    /* [in] */ WindowState* windowState)
{
    AutoPtr<IStringWriter> sw;
    CStringWriter::New((IStringWriter**)&sw);
    AutoPtr<IPrintWriter> pw;
    CPrintWriter::New(sw, (IPrintWriter**)&pw);
    AutoPtr<IDate> date;
    CDate::New((IDate**)&date);
    AutoPtr<IDateFormatHelper> dfh;
    CDateFormatHelper::AcquireSingleton((IDateFormatHelper **)&dfh);
    AutoPtr<IDateFormat> format;
    dfh->GetInstance((IDateFormat**)&format);
    String dStr;
    format->FormatDate(date, &dStr);
    pw->PrintStringln(String("  ANR time: ") + dStr);
    if (appWindowToken != NULL) {
        pw->PrintStringln(String("  Application at fault: ") + appWindowToken->mStringName);
    }
    if (windowState != NULL) {
        AutoPtr<ICharSequence> title;
        windowState->mAttrs->GetTitle((ICharSequence**)&title);
        String tcs;
        title->ToString(&tcs);
        pw->PrintStringln(String("  Window at fault: ") + tcs);
    }
    pw->Println();
    // DumpWindowsNoHeaderLocked(pw, TRUE, NULL);
    ICloseable::Probe(pw)->Close();
    // mLastANRState = sw->ToString();
}

void CWindowManagerService::Monitor()
{
    AutoLock lock(mWindowMapLock);
}

void CWindowManagerService::DebugLayoutRepeats(
    /* [in] */ const String& msg,
    /* [in] */ Int32 pendingLayoutChanges)
{
    if (mLayoutRepeatCount >= LAYOUT_REPEAT_THRESHOLD) {
        Slogger::V(TAG, "Layouts looping: %s, mPendingLayoutChanges = 0x%x", msg.string(),
                pendingLayoutChanges);
    }
}

void CWindowManagerService::CreateDisplayContentLocked(
    /* [in] */ IDisplay* display)
{
    // if (display == NULL) {
    //     throw new IllegalArgumentException("getDisplayContent: display must not be null");
    // }
    assert(display != NULL);
    AutoPtr<DisplayContent> displayContent = new DisplayContent(display);
    Int32 key;
    display->GetDisplayId(&key);
    mDisplayContents[key] = displayContent;
}

AutoPtr<DisplayContent> CWindowManagerService::GetDisplayContentLocked(
    /* [in] */ Int32 displayId)
{
    AutoPtr<DisplayContent> displayContent;
    HashMap<Int32, AutoPtr<DisplayContent> >::Iterator it = mDisplayContents.Find(displayId);
    if (it != mDisplayContents.End()) {
        displayContent = it->mSecond;
    }
    if (displayContent == NULL) {
        AutoPtr<IDisplay> display;
        mDisplayManager->GetDisplay(displayId, (IDisplay**)&display);
        if (display != NULL) {
            displayContent = new DisplayContent(display);
            mDisplayContents[displayId] = displayContent;
        }
    }
    return displayContent;
}

AutoPtr<DisplayContent> CWindowManagerService::GetDefaultDisplayContentLocked()
{
    return GetDisplayContentLocked(IDisplay::DEFAULT_DISPLAY);
}

List< AutoPtr<WindowState> >& CWindowManagerService::GetDefaultWindowListLocked()
{
    return GetDefaultDisplayContentLocked()->GetWindowList();
}

AutoPtr<IDisplayInfo> CWindowManagerService::GetDefaultDisplayInfoLocked()
{
    return GetDefaultDisplayContentLocked()->GetDisplayInfo();
}

ECode CWindowManagerService::OnDisplayAdded(
    /* [in] */ Int32 displayId)
{
    AutoPtr<IMessage> msg;
    mH->ObtainMessage(H::DO_DISPLAY_ADDED, displayId, 0, (IMessage**)&msg);
    Boolean result;
    return mH->SendMessage(msg, &result);
}

void CWindowManagerService::HandleDisplayAddedLocked(
    /* [in] */ Int32 displayId)
{
    AutoPtr<IDisplay> display;
    mDisplayManager->GetDisplay(displayId, (IDisplay**)&display);
    if (display != NULL) {
        CreateDisplayContentLocked(display);
        DisplayReady(displayId);
    }
}

void CWindowManagerService::HandleDisplayRemovedLocked(
    /* [in] */ Int32 displayId)
{}

ECode CWindowManagerService::OnDisplayRemoved(
    /* [in] */ Int32 displayId)
{
    AutoPtr<IMessage> msg;
    mH->ObtainMessage(H::DO_DISPLAY_REMOVED, displayId, 0, (IMessage**)&msg);
    Boolean result;
    return mH->SendMessage(msg, &result);
}

ECode CWindowManagerService::OnDisplayChanged(
    /* [in] */ Int32 displayId)
{
    AutoPtr<IMessage> msg;
    mH->ObtainMessage(H::DO_DISPLAY_CHANGED, displayId, 0, (IMessage**)&msg);
    Boolean result;
    return mH->SendMessage(msg, &result);
}

ECode CWindowManagerService::HandleDoDisplayChanged(
    /* [in] */ Int32 displayId)
{
    Slogger::D(TAG, " >> HandleDoDisplayChanged: %d", displayId);
    AutoLock lock(mWindowMapLock);
    HandleDisplayChangedLocked(displayId);
    return NOERROR;
}

void CWindowManagerService::HandleDisplayChangedLocked(
    /* [in] */ Int32 displayId)
{
    AutoPtr<DisplayContent> displayContent = GetDisplayContentLocked(displayId);
    if (displayContent != NULL) {
        displayContent->UpdateDisplayInfo();
    }
}


} // Wm
} // Server
} // Droid
} // Elastos
