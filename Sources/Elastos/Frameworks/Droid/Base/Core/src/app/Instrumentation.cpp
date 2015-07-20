
#include "app/Instrumentation.h"
#include "app/Activity.h"
#include "app/ActivityManagerNative.h"
#include "app/Fragment.h"
#include "os/SystemClock.h"
#include "os/Process.h"
#include "os/ServiceManager.h"
#ifdef DROID_CORE
#include "app/CActivityThreadHelper.h"
#include "app/CInstrumentationActivityMonitor.h"
#include "app/CInstrumentationActivityResult.h"
#include "content/CComponentName.h"
#include "content/CIntent.h"
#include "content/res/CConfiguration.h"
#include "hardware/input/CInputManagerHelper.h"
#include "os/CBundle.h"
#include "os/CUserHandleHelper.h"
#include "privacy/CPrivacySettingsManager.h"
#include "view/CKeyEvent.h"
#include "view/CKeyEventHelper.h"
#include "view/CViewConfigurationHelper.h"
#include "view/CKeyCharacterMapHelper.h"
#else
#include "Elastos.Droid.Core.h"
#endif
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::CStringWrapper;
using Elastos::Core::StringBuilder;
using Elastos::Core::EIID_IRunnable;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::CObjectContainer;
using Elastos::Core::CThread;
using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::Find;
using Elastos::Droid::App::Activity;
using Elastos::Droid::App::Fragment;
using Elastos::Droid::App::IActivityThreadHelper;
using Elastos::Droid::App::CActivityThreadHelper;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::Res::CConfiguration;
using Elastos::Droid::Hardware::Input::IInputManager;
using Elastos::Droid::Hardware::Input::IInputManagerHelper;
using Elastos::Droid::Hardware::Input::CInputManagerHelper;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::EIID_IIdleHandler;
using Elastos::Droid::Privacy::IIPrivacySettingsManager;
using Elastos::Droid::Privacy::CPrivacySettingsManager;
using Elastos::Droid::Privacy::IPrivacySettings;
using Elastos::Droid::Telephony::ITelephonyManager;
using Elastos::Droid::View::CKeyEvent;
using Elastos::Droid::View::IKeyEventHelper;
using Elastos::Droid::View::CKeyEventHelper;
using Elastos::Droid::View::IKeyCharacterMap;
using Elastos::Droid::View::IKeyCharacterMapHelper;
using Elastos::Droid::View::CKeyCharacterMapHelper;
using Elastos::Droid::View::IInputDevice;
using Elastos::Droid::View::IIWindowManager;
using Elastos::Droid::View::IViewConfigurationHelper;
using Elastos::Droid::View::CViewConfigurationHelper;

namespace Elastos {
namespace Droid {
namespace App {

//====================================================
// Instrumentation::InstrumentationThread
//====================================================

Instrumentation::InstrumentationThread::InstrumentationThread(
    /* [in] */ const String& name,
    /* [in] */ Instrumentation* host)
    : mHost(host)
{
    Thread::constructor(name);
}

ECode Instrumentation::InstrumentationThread::Run()
{
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    // try {
    if (FAILED(Process::SetThreadPriority(IProcess::THREAD_PRIORITY_URGENT_DISPLAY))) {
        Slogger::W("Instrumentation", "Exception setting priority of instrumentation thread %d", Process::MyTid());
    }
    // } catch (RuntimeException e) {
    //     Log.w(TAG, "Exception setting priority of instrumentation thread "
    //             + Process.myTid(), e);
    // }
    if (mHost->mAutomaticPerformanceSnapshots) {
        mHost->StartPerformanceSnapshot();
    }
    mHost->OnStart();
    return NOERROR;
}


//====================================================
// Instrumentation::EmptyRunnable
//====================================================

CAR_INTERFACE_IMPL(Instrumentation::EmptyRunnable, IRunnable)

Instrumentation::EmptyRunnable::Run()
{
    return NOERROR;
}


//====================================================
// Instrumentation::SyncRunnable
//====================================================

Instrumentation::SyncRunnable::SyncRunnable(
    /* [in] */ IRunnable* target)
    : mTarget(target)
    , mComplete(FALSE)
{}

CAR_INTERFACE_IMPL(Instrumentation::SyncRunnable, IRunnable)

Instrumentation::SyncRunnable::Run()
{
    mTarget->Run();
    Object::Autolock lock(this);
    mComplete = TRUE;
    NotifyAll();
    return NOERROR;
}

void Instrumentation::SyncRunnable::WaitForComplete()
{
    Object::Autolock lock(this);
    while (!mComplete) {
        // try {
        Wait();
        // } catch (InterruptedException e) {
        // }
    }
}


//====================================================
// Instrumentation::ActivityGoing
//====================================================

Instrumentation::ActivityGoing::ActivityGoing(
    /* [in] */ ActivityWaiter* waiter,
    /* [in] */ Instrumentation* host)
    : mWaiter(waiter)
    , mHost(host)
{}

CAR_INTERFACE_IMPL(Instrumentation::ActivityGoing, IIdleHandler)

Instrumentation::ActivityGoing::QueueIdle(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    Object::Autolock lock(mHost->mSync);
    mHost->mWaitingActivities->Remove(mWaiter);
    mHost->mSync.NotifyAll();
    *res = FALSE;
    return NOERROR;
}


//====================================================
// Instrumentation::IIdleHandler
//====================================================

CAR_INTERFACE_IMPL(Instrumentation::Idler, IIdleHandler)

Instrumentation::Idler::Idler(
    /* [in] */ IRunnable* callback)
    : mCallback(callback)
    , mIdle(FALSE)
{}

ECode Instrumentation::Idler::QueueIdle(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if (mCallback != NULL) {
        mCallback->Run();
    }
    Object::Autolock lock(this);
    mIdle = TRUE;
    NotifyAll();
    *res = FALSE;
    return NOERROR;
}

void Instrumentation::Idler::WaitForIdle()
{
    Object::Autolock lock(this);
    while (!mIdle) {
        // try {
        Wait();
        // } catch (InterruptedException e) {
        // }
    }
}


//====================================================
// Instrumentation::MenuRunnable
//====================================================

Instrumentation::MenuRunnable::MenuRunnable(
    /* [in] */ IActivity* activity,
    /* [in] */ Int32 identifier,
    /* [in] */ Int32 flags)
    : mActivity(activity)
    , mIdentifier(identifier)
    , mFlags(flags)
    , mReturnValue(FALSE)
{}

CAR_INTERFACE_IMPL(Instrumentation::MenuRunnable, IRunnable)

ECode Instrumentation::MenuRunnable::Run()
{
    AutoPtr<IWindow> win;
    mActivity->GetWindow((IWindow**)&win);

    return win->PerformPanelIdentifierAction(IWindow::FEATURE_OPTIONS_PANEL, mIdentifier, mFlags, &mReturnValue);
}


//====================================================
// Instrumentation::ContextMenuRunnable
//====================================================

Instrumentation::ContextMenuRunnable::ContextMenuRunnable(
    /* [in] */ IActivity* activity,
    /* [in] */ Int32 identifier,
    /* [in] */ Int32 flags)
    : mActivity(activity)
    , mIdentifier(identifier)
    , mFlags(flags)
    , mReturnValue(FALSE)
{}

CAR_INTERFACE_IMPL(Instrumentation::ContextMenuRunnable, IRunnable)

ECode Instrumentation::ContextMenuRunnable::Run()
{
    AutoPtr<IWindow> win;
    mActivity->GetWindow((IWindow**)&win);

    return win->PerformPanelIdentifierAction(IWindow::FEATURE_OPTIONS_PANEL, mIdentifier, mFlags, &mReturnValue);
}


//====================================================
// Instrumentation::BlockPhoneCallRunnable
//====================================================

CAR_INTERFACE_IMPL(Instrumentation::BlockPhoneCallRunnable, IRunnable)

Instrumentation::BlockPhoneCallRunnable::Run()
{
    // SM: Not clear why there is this delay
    // try{
    Thread::Sleep(1000); //wait 1 Second
    // } catch(Exception e){
    //     //nothing here
    // }
    AutoPtr<IIntent> privacy;
    CIntent::New(String("android.privacy.BLOCKED_PHONE_CALL"), (IIntent**)&privacy);
    AutoPtr<IBundle> extras;
    CBundle::New((IBundle**)&extras);
    String packageName;
    mContext->GetPackageName(&packageName);
    extras->PutString(String("packageName"), packageName);
    extras->PutInt32(String("phoneState"), ITelephonyManager::CALL_STATE_IDLE);
    privacy->PutExtras(extras);
    mContext->SendBroadcast(privacy);
    Slogger::I("PrivacyContext", "sent privacy intent");
    return NOERROR;
}


//====================================================
// Instrumentation
//====================================================

const String Instrumentation::TAG("Instrumentation");

Instrumentation::Instrumentation()
    : mAutomaticPerformanceSnapshots(FALSE)
{}

Instrumentation::~Instrumentation()
{}

ECode Instrumentation::Initialize()
{
    return NOERROR;
}

ECode Instrumentation::OnCreate(
    /* [in] */ IBundle* arguments)
{
    return NOERROR;
}

ECode Instrumentation::Start()
{
    if (mRunner != NULL) {
        Slogger::E(TAG, "Instrumentation already started");
        return E_RUNTIME_EXCEPTION;
    }
    StringBuilder sb("Instr: ");
    AutoPtr<IClassInfo> clsInfo;
    _CObject_ReflectClassInfo(Probe(EIID_IObject), (IClassInfo**)&clsInfo);
    StringBuf_<512> nameBuf;
    clsInfo->GetName(&nameBuf);
    sb += (const char*)nameBuf;
    mRunner = (IThread*)new InstrumentationThread(sb.ToString(), this);
    mRunner->Start();
    return NOERROR;
}

ECode Instrumentation::OnStart()
{
    return NOERROR;
}

ECode Instrumentation::OnException(
    /* [in] */ IInterface* obj,
    /* [in] */ ECode e,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode Instrumentation::SendStatus(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* results)
{
    VALIDATE_NOT_NULL(results)

    if (mWatcher != NULL) {
//    try {
        if (FAILED(mWatcher->InstrumentationStatus(mComponent, resultCode, results))) {
            mWatcher = NULL;
        }
//    }
//    catch (RemoteException e) {
//        mWatcher = null;
//    }
    }
    return NOERROR;
}

ECode Instrumentation::Finish(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* results)
{
    VALIDATE_NOT_NULL(results)

    if (mAutomaticPerformanceSnapshots) {
        EndPerformanceSnapshot();
    }
    if (mPerfMetrics != NULL) {
        results->PutAll(mPerfMetrics);
    }
    return mThread->FinishInstrumentation(resultCode, results);
}

ECode Instrumentation::SetAutomaticPerformanceSnapshots()
{
    mAutomaticPerformanceSnapshots = TRUE;
//    mPerformanceCollector = new PerformanceCollector();
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode Instrumentation::StartPerformanceSnapshot()
{
    Boolean isProfiling;
    if (IsProfiling(&isProfiling), !isProfiling) {
        assert(0);
        // mPerformanceCollector->BeginSnapshot(null);
    }
    return E_NOT_IMPLEMENTED;
}

ECode Instrumentation::EndPerformanceSnapshot()
{
    Boolean isProfiling;
    if (IsProfiling(&isProfiling), !isProfiling) {
        assert(0);
        // mPerfMetrics = mPerformanceCollector.endSnapshot();
    }
    return E_NOT_IMPLEMENTED;
}

ECode Instrumentation::OnDestroy()
{
    return NOERROR;
}

ECode Instrumentation::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context)
    *context = mInstrContext;
    REFCOUNT_ADD(*context)
    return NOERROR;
}

ECode Instrumentation::GetComponentName(
    /* [out] */ IComponentName** component)
{
    VALIDATE_NOT_NULL(component)
    *component = mComponent;
    REFCOUNT_ADD(*component)
    return NOERROR;
}

ECode Instrumentation::GetTargetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context)
    *context = mAppContext;
    REFCOUNT_ADD(*context)
    return NOERROR;
}

ECode Instrumentation::IsProfiling(
    /* [out] */ Boolean* isProfiling)
{
    *isProfiling = FALSE;
    assert(0);
//    return mThread.isProfiling();
    return E_NOT_IMPLEMENTED;
}

ECode Instrumentation::StartProfiling()
{
//     if (mThread.isProfiling()) {
//         File file = new File(mThread.getProfileFilePath());
//         file.getParentFile().mkdirs();
//         Debug.startMethodTracing(file.toString(), 8 * 1024 * 1024);
//     }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode Instrumentation::StopProfiling()
{
//     if (mThread.isProfiling()) {
//         Debug.stopMethodTracing();
//     }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode Instrumentation::SetInTouchMode(
    /* [in] */ Boolean inTouch)
{
    // try {
    AutoPtr<IIWindowManager> wm = (IIWindowManager*)ServiceManager::GetService(IContext::WINDOW_SERVICE).Get();
    return wm->SetInTouchMode(inTouch);
    // } catch (RemoteException e) {
    //     // Shouldn't happen!
    // }
}

ECode Instrumentation::WaitForIdle(
    /* [in] */ IRunnable* recipient)
{
    AutoPtr<IIdleHandler> idleH = (IIdleHandler*)new Idler(recipient);
    mMessageQueue->AddIdleHandler(idleH);
    AutoPtr<IHandler> h;
    mThread->GetHandler((IHandler**)&h);
    AutoPtr<EmptyRunnable> runnable = new EmptyRunnable();
    Boolean result;
    h->Post(runnable, &result);
    return NOERROR;
}

ECode Instrumentation::WaitForIdleSync()
{
    FAIL_RETURN(ValidateNotAppThread())
    AutoPtr<Idler> idler = new Idler(NULL);
    mMessageQueue->AddIdleHandler(idler);
    AutoPtr<IHandler> h;
    mThread->GetHandler((IHandler**)&h);
    AutoPtr<EmptyRunnable> runnable = new EmptyRunnable();
    Boolean result;
    h->Post(runnable, &result);
    idler->WaitForIdle();
    return NOERROR;
}

ECode Instrumentation::RunOnMainSync(
    /* [in] */ IRunnable* runner)
{
    FAIL_RETURN(ValidateNotAppThread())
    AutoPtr<IHandler> h;
    mThread->GetHandler((IHandler**)&h);
    AutoPtr<SyncRunnable> sr = new SyncRunnable(runner);
    Boolean result;
    h->Post(sr, &result);
    sr->WaitForComplete();
    return NOERROR;
}

ECode Instrumentation::StartActivitySync(
    /* [in] */ IIntent* aIntent,
    /* [out] */ IActivity** activity)
{
    VALIDATE_NOT_NULL(activity)
    *activity = NULL;

    FAIL_RETURN(ValidateNotAppThread())

    Object::Autolock lock(mSync);

    AutoPtr<IIntent> intent;
    CIntent::New(aIntent, (IIntent**)&intent);

    AutoPtr<IContext> targetContext;
    GetTargetContext((IContext**)&targetContext);
    AutoPtr<IPackageManager> pm;
    targetContext->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IActivityInfo> ai;
    intent->ResolveActivityInfo(pm, 0, (IActivityInfo**)&ai);
    if (ai == NULL) {
        //throw new RuntimeException("Unable to resolve activity for: " + intent);
        Slogger::E(TAG, "Unable to resolve activity for: %p", intent.Get());
        return E_RUNTIME_EXCEPTION;
    }

    String aiProcName;
    ai->GetProcessName(&aiProcName);
    String myProc;
    mThread->GetProcessName(&myProc);
    if (!aiProcName.Equals(myProc)) {
//             // todo: if this intent is ambiguous, look here to see if
//             // there is a single match that is in our package.
//             throw new RuntimeException("Intent in process "
//                     + myProc + " resolved to different process "
//                     + ai.processName + ": " + intent);
        Slogger::E(TAG, "Intent in process %s resolved to different process %s: %p",
                myProc.string(), aiProcName.string(), intent.Get());
        return E_RUNTIME_EXCEPTION;
    }

    String packageName, aiName;
    AutoPtr<IApplicationInfo> appInfo;
    ai->GetApplicationInfo((IApplicationInfo**)&appInfo);
    appInfo->GetPackageName(&packageName);
    ai->GetName(&aiName);
    AutoPtr<IComponentName> cn;
    CComponentName::New(packageName, aiName, (IComponentName**)&cn);
    intent->SetComponent(cn);

     if (mWaitingActivities == NULL) {
        mWaitingActivities = new List<AutoPtr<ActivityWaiter> >();
    }
    AutoPtr<ActivityWaiter> aw = new ActivityWaiter(intent);
    mWaitingActivities->PushBack(aw);

    targetContext->StartActivity(intent);

    do {
        // try {
        mSync.Wait();
        // } catch (InterruptedException e) {
        // }
    } while (Find(mWaitingActivities->Begin(), mWaitingActivities->End(), aw) != mWaitingActivities->End());

    *activity = aw->mActivity;
    REFCOUNT_ADD(*activity)
    return NOERROR;
}

ECode Instrumentation::AddMonitor(
    /* [in] */ IInstrumentationActivityMonitor* monitor)
{
    Object::Autolock lock(mSync);
    if (mActivityMonitors == NULL) {
        mActivityMonitors = new List<AutoPtr<IInstrumentationActivityMonitor> >();
    }
    mActivityMonitors->PushBack(monitor);
    return NOERROR;
}

ECode Instrumentation::AddMonitor(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ IInstrumentationActivityResult* result,
    /* [in] */ Boolean block,
    /* [out] */ IInstrumentationActivityMonitor** monitor)
{
    VALIDATE_NOT_NULL(monitor)
    AutoPtr<IInstrumentationActivityMonitor> am;
    CInstrumentationActivityMonitor::New(filter, result, block,
            (IInstrumentationActivityMonitor**)&am);
    AddMonitor(am);
    *monitor = am;
    REFCOUNT_ADD(*monitor)
    return NOERROR;
}

ECode Instrumentation::AddMonitor(
    /* [in] */ const String& cls,
    /* [in] */ IInstrumentationActivityResult* result,
    /* [in] */ Boolean block,
    /* [out] */ IInstrumentationActivityMonitor** monitor)
{
    VALIDATE_NOT_NULL(monitor)
    AutoPtr<IInstrumentationActivityMonitor> am;
    CInstrumentationActivityMonitor::New(cls, result, block,
            (IInstrumentationActivityMonitor**)&am);
    AddMonitor(am);
    *monitor = am;
    REFCOUNT_ADD(*monitor)
    return NOERROR;
}

ECode Instrumentation::CheckMonitorHit(
    /* [in] */ IInstrumentationActivityMonitor* monitor,
    /* [in] */ Int32 minHits,
    /* [out] */ Boolean* reached)
{
    VALIDATE_NOT_NULL(reached)
    WaitForIdleSync();

    Object::Autolock lock(mSync);
    Int32 hits;
    if (monitor->GetHits(&hits), hits < minHits) {
        *reached = FALSE;
        return NOERROR;
    }
    mActivityMonitors->Remove(monitor);
    *reached = TRUE;
    return NOERROR;
}

ECode Instrumentation::WaitForMonitor(
    /* [in] */ IInstrumentationActivityMonitor* monitor,
    /* [out] */ IActivity** activity)
{
    VALIDATE_NOT_NULL(activity)

    AutoPtr<IActivity> act;
    monitor->WaitForActivity((IActivity**)&act);
    {
        Object::Autolock lock(mSync);
        mActivityMonitors->Remove(monitor);
    }
    *activity  = act;
    REFCOUNT_ADD(*activity)
    return NOERROR;
}

ECode Instrumentation::WaitForMonitorWithTimeout(
    /* [in] */ IInstrumentationActivityMonitor* monitor,
    /* [in] */ Int64 timeOut,
    /* [out] */ IActivity** activity)
{
    VALIDATE_NOT_NULL(activity);

    AutoPtr<IActivity> act;
    monitor->WaitForActivityWithTimeout(timeOut, (IActivity**)&act);
    {
        Object::Autolock lock(mSync);
        mActivityMonitors->Remove(monitor);
    }
    *activity  = act;
    REFCOUNT_ADD(*activity)
    return NOERROR;
}

ECode Instrumentation::RemoveMonitor(
    /* [in] */ IInstrumentationActivityMonitor* monitor)
{
    Object::Autolock lock(mSync);
    mActivityMonitors->Remove(monitor);
    return NOERROR;
}

ECode Instrumentation::InvokeMenuActionSync(
    /* [in] */ IActivity* targetActivity,
    /* [in] */ Int32 id,
    /* [in] */ Int32 flag,
    /* [out] */ Boolean* success)
{
    VALIDATE_NOT_NULL(success)
    AutoPtr<MenuRunnable> mr = new MenuRunnable(targetActivity, id, flag);
    RunOnMainSync(mr);
    *success = mr->mReturnValue;
    return NOERROR;
}

ECode Instrumentation::InvokeContextMenuAction(
    /* [in] */ IActivity* targetActivity,
    /* [in] */ Int32 id,
    /* [in] */ Int32 flag,
    /* [out] */ Boolean* success)
{
    VALIDATE_NOT_NULL(success)
    FAIL_RETURN(ValidateNotAppThread())

    // Bring up context menu for current focus.
    // It'd be nice to do this through code, but currently ListView depends on
    //   long press to set metadata for its selected child

    AutoPtr<IKeyEvent> downEvent;
    CKeyEvent::New(IKeyEvent::ACTION_DOWN, IKeyEvent::KEYCODE_DPAD_CENTER, (IKeyEvent**)&downEvent);
    SendKeySync(downEvent);

    // Need to wait for long press
    WaitForIdleSync();
    // try {
    AutoPtr<IViewConfigurationHelper> helper;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&helper);
    Int32 timeout;
    helper->GetLongPressTimeout(&timeout);
    ECode ec = Thread::Sleep(timeout);
    if (FAILED(ec)) {
        Slogger::E(TAG, "Could not sleep for long press timeout, 0x%08x", ec);
        *success = FALSE;
        return ec;
    }
    // } catch (InterruptedException e) {
    //     Log.e(TAG, "Could not sleep for long press timeout", e);
    //     return false;
    // }

    AutoPtr<IKeyEvent> upEvent;
    CKeyEvent::New(IKeyEvent::ACTION_UP, IKeyEvent::KEYCODE_DPAD_CENTER, (IKeyEvent**)&upEvent);
    SendKeySync(upEvent);

    // Wait for context menu to appear
    WaitForIdleSync();

    AutoPtr<ContextMenuRunnable> cmr = new ContextMenuRunnable(targetActivity, id, flag);
    RunOnMainSync(cmr);
    *success = cmr->mReturnValue;
    return NOERROR;
}

ECode Instrumentation::SendStringSync(
    /* [in] */ const String& text)
{
    if (text.IsNull()) {
        return NOERROR;
    }

    AutoPtr<IKeyCharacterMapHelper> helper;
    CKeyCharacterMapHelper::AcquireSingleton((IKeyCharacterMapHelper**)&helper);
    AutoPtr<IKeyCharacterMap> keyCharacterMap;
    helper->Load(IKeyCharacterMap::VIRTUAL_KEYBOARD, (IKeyCharacterMap**)&keyCharacterMap);

    AutoPtr< ArrayOf<IKeyEvent*> > events;
    AutoPtr< ArrayOf<Char32> > charArray = text.GetChars();
    keyCharacterMap->GetEvents(charArray, (ArrayOf<IKeyEvent*>**)&events);

    if (events != NULL) {
        for (Int32 i = 0; i < events->GetLength(); i++) {
            // We have to change the time of an event before injecting it because
            // all KeyEvents returned by KeyCharacterMap.getEvents() have the same
            // time stamp and the system rejects too old events. Hence, it is
            // possible for an event to become stale before it is injected if it
            // takes too long to inject the preceding ones.
            AutoPtr<IKeyEventHelper> helper;
            CKeyEventHelper::AcquireSingleton((IKeyEventHelper**)&helper);
            AutoPtr<IKeyEvent> keyEvent;
            helper->ChangeTimeRepeat((*events)[i], SystemClock::GetUptimeMillis(), 0, (IKeyEvent**)&keyEvent);
            SendKeySync(keyEvent);
        }
    }
    return NOERROR;
}

ECode Instrumentation::SendKeySync(
    /* [in] */ IKeyEvent* event)
{
    FAIL_RETURN(ValidateNotAppThread())

    Int64 downTime, eventTime;
    event->GetDownTime(&downTime);
    event->GetEventTime(&eventTime);
    Int32 action, code, repeatCount, metaState, deviceId, scancode, source, flags;
    event->GetAction(&action);
    event->GetKeyCode(&code);
    event->GetRepeatCount(&repeatCount);
    event->GetMetaState(&metaState);
    event->GetDeviceId(&deviceId);
    event->GetScanCode(&scancode);
    event->GetSource(&source);
    event->GetFlags(&flags);
    if (source == IInputDevice::SOURCE_UNKNOWN) {
        source = IInputDevice::SOURCE_KEYBOARD;
    }
    if (eventTime == 0) {
        eventTime = SystemClock::GetUptimeMillis();
    }
    if (downTime == 0) {
        downTime = eventTime;
    }

    AutoPtr<IKeyEvent> newEvent;
    CKeyEvent::New(downTime, eventTime, action, code, repeatCount, metaState,
            deviceId, scancode, flags | IKeyEvent::FLAG_FROM_SYSTEM, source,
            (IKeyEvent**)&newEvent);
    Boolean result;
    AutoPtr<IInputManagerHelper> helper;
    CInputManagerHelper::AcquireSingleton((IInputManagerHelper**)&helper);
    AutoPtr<IInputManager> manager;
    helper->GetInstance((IInputManager**)&manager);
    return manager->InjectInputEvent(
            newEvent, IInputManager::INJECT_INPUT_EVENT_MODE_WAIT_FOR_FINISH, &result);
}

ECode Instrumentation::SendKeyDownUpSync(
    /* [in] */ Int32 key)
{
    AutoPtr<IKeyEvent> downEvent;
    CKeyEvent::New(IKeyEvent::ACTION_DOWN, key, (IKeyEvent**)&downEvent);
    FAIL_RETURN(SendKeySync(downEvent))

    AutoPtr<IKeyEvent> upEvent;
    CKeyEvent::New(IKeyEvent::ACTION_UP, key, (IKeyEvent**)&upEvent);
    return SendKeySync(upEvent);
}

ECode Instrumentation::SendCharacterSync(
    /* [in] */ Int32 keyCode)
{
    AutoPtr<IKeyEvent> downEvent;
    CKeyEvent::New(IKeyEvent::ACTION_DOWN, keyCode, (IKeyEvent**)&downEvent);
    FAIL_RETURN(SendKeySync(downEvent))

    AutoPtr<IKeyEvent> upEvent;
    CKeyEvent::New(IKeyEvent::ACTION_UP, keyCode, (IKeyEvent**)&upEvent);
    return SendKeySync(upEvent);
}

ECode Instrumentation::SendPointerSync(
    /* [in] */ IMotionEvent* event)
{
    VALIDATE_NOT_NULL(event)
    FAIL_RETURN(ValidateNotAppThread())

    Int32 source;
    if (event->GetSource(&source), (source & IInputDevice::SOURCE_CLASS_POINTER) == 0) {
        event->SetSource(IInputDevice::SOURCE_TOUCHSCREEN);
    }

    Boolean result;
    AutoPtr<IInputManagerHelper> helper;
    CInputManagerHelper::AcquireSingleton((IInputManagerHelper**)&helper);
    AutoPtr<IInputManager> manager;
    helper->GetInstance((IInputManager**)&manager);
    return manager->InjectInputEvent(event,
            IInputManager::INJECT_INPUT_EVENT_MODE_WAIT_FOR_FINISH, &result);
}

ECode Instrumentation::SendTrackballEventSync(
    /* [in] */ IMotionEvent* event)
{
    VALIDATE_NOT_NULL(event)
    FAIL_RETURN(ValidateNotAppThread())

    Int32 source;
    if (event->GetSource(&source), (source & IInputDevice::SOURCE_CLASS_TRACKBALL) == 0) {
        event->SetSource(IInputDevice::SOURCE_TRACKBALL);
    }

    Boolean result;
    AutoPtr<IInputManagerHelper> helper;
    CInputManagerHelper::AcquireSingleton((IInputManagerHelper**)&helper);
    AutoPtr<IInputManager> manager;
    helper->GetInstance((IInputManager**)&manager);
    return manager->InjectInputEvent(event,
            IInputManager::INJECT_INPUT_EVENT_MODE_WAIT_FOR_FINISH, &result);
}

ECode Instrumentation::NewApplication(
    /* [in] */ IClassLoader* cl,
    /* [in] */ const String& className,
    /* [in] */ IContext* context,
    /* [out] */ IApplication** app)
{
    VALIDATE_NOT_NULL(app)
    *app = NULL;
    VALIDATE_NOT_NULL(cl)
    if (className.IsNullOrEmpty()) {
        return E_INVALID_ARGUMENT;
    }
    AutoPtr<IClassInfo> clsInfo;
    FAIL_RETURN(cl->LoadClass(className, (IClassInfo**)&clsInfo));
    return NewApplication(clsInfo, context, app);
}

ECode Instrumentation::NewApplication(
    /* [in] */ IClassInfo* clazz,
    /* [in] */ IContext* context,
    /* [out] */ IApplication** app)
{
    VALIDATE_NOT_NULL(app)
    *app = NULL;
    VALIDATE_NOT_NULL(clazz)

    AutoPtr<IInterface> obj;
    FAIL_RETURN(clazz->CreateObject((IInterface**)&obj))
    IApplication* _app = IApplication::Probe(obj);
    if (_app) FAIL_RETURN(_app->Attach(context))
    *app = _app;
    REFCOUNT_ADD(*app)
    return NOERROR;
}

ECode Instrumentation::NewApplication(
    /* [in] */ const ClassID& clsid,
    /* [in] */ IContext* context,
    /* [out] */ IApplication** app)
{
    VALIDATE_NOT_NULL(app)
    *app = NULL;
    AutoPtr<IInterface> obj;
    FAIL_RETURN(_CObject_CreateInstance(clsid, RGM_SAME_DOMAIN, EIID_IApplication, (IInterface**)&obj))
    IApplication* _app = IApplication::Probe(obj);
    if (_app) FAIL_RETURN(_app->Attach(context))
    *app = _app;
    REFCOUNT_ADD(*app)
    return NOERROR;
}

ECode Instrumentation::CallApplicationOnCreate(
    /* [in] */ IApplication* app)
{
    return app->OnCreate();
}

ECode Instrumentation::NewActivity(
    /* [in] */ IClassInfo* clazz,
    /* [in] */ IContext* context,
    /* [in] */ IBinder* token,
    /* [in] */ IApplication* application,
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* info,
    /* [in] */ ICharSequence* title,
    /* [in] */ IActivity* parent,
    /* [in] */ const String& id,
    /* [in] */ IInterface* lastNonConfigurationInstance,
    /* [out] */ IActivity** activity)
{
    VALIDATE_NOT_NULL(activity)
    *activity = NULL;
    AutoPtr<IInterface> obj;
    FAIL_RETURN(clazz->CreateObject((IInterface**)&obj))
    IActivity* actObj = IActivity::Probe(obj);
    AutoPtr<IConfiguration> config;
    CConfiguration::New((IConfiguration**)&config);
    AutoPtr<Activity> act = reinterpret_cast<Activity*>(actObj->Probe(EIID_Activity));
    FAIL_RETURN(act->Attach(
            context, NULL, (IInstrumentation*)this->Probe(EIID_IInstrumentation), token, application, intent,
            info, title, parent, id, lastNonConfigurationInstance, config))
    *activity = actObj;
    REFCOUNT_ADD(*activity)
    return NOERROR;
}

ECode Instrumentation::NewActivity(
    /* [in] */ IClassLoader* cl,
    /* [in] */ const String& className,
    /* [in] */ IIntent* intent,
    /* [out] */ IActivity** activity)
{
    VALIDATE_NOT_NULL(activity)
    *activity = NULL;
    AutoPtr<IClassInfo> clsInfo;
    FAIL_RETURN(cl->LoadClass(className, (IClassInfo**)&clsInfo))
    AutoPtr<IInterface> obj;
    FAIL_RETURN(clsInfo->CreateObject((IInterface**)&obj))
    *activity = IActivity::Probe(obj);
    REFCOUNT_ADD(*activity)
    return NOERROR;
}

ECode Instrumentation::CallActivityOnCreate(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* icicle)
{
    if (mWaitingActivities != NULL) {
        Object::Autolock lock(mSync);
        List<AutoPtr<ActivityWaiter> >::Iterator it = mWaitingActivities->Begin();
        for (; it != mWaitingActivities->End(); ++it) {
            AutoPtr<ActivityWaiter> aw = *it;
            AutoPtr<IIntent> intent = aw->mIntent;
            AutoPtr<IIntent> aIntent;
            activity->GetIntent((IIntent**)&aIntent);
            Boolean eq;
            if (intent->FilterEquals(aIntent, &eq), eq) {
                aw->mActivity = activity;
                AutoPtr<ActivityGoing> activityGoing = new ActivityGoing(aw, this);
                mMessageQueue->AddIdleHandler(activityGoing);
            }
        }
    }

    activity->PerformCreate(icicle);

    if (mActivityMonitors != NULL) {
        Object::Autolock lock(mSync);
        List<AutoPtr<IInstrumentationActivityMonitor> >::Iterator it;
        for (it = mActivityMonitors->Begin(); it != mActivityMonitors->End(); ++it) {
            AutoPtr<IIntent> intent;
            activity->GetIntent((IIntent**)&intent);
            Boolean result;
            (*it)->Match(activity, activity, intent, &result);
        }
    }
    return NOERROR;
}

ECode Instrumentation::CallActivityOnDestroy(
    /* [in] */ IActivity* activity)
{
      // TODO: the following block causes intermittent hangs when using startActivity
      // temporarily comment out until root cause is fixed (bug 2630683)
//      if (mWaitingActivities != null) {
//          synchronized (mSync) {
//              final int N = mWaitingActivities->size();
//              for (int i=0; i<N; i++) {
//                  final ActivityWaiter aw = mWaitingActivities->get(i);
//                  final Intent intent = aw.intent;
//                  if (intent.filterEquals(activity.getIntent())) {
//                      aw.activity = activity;
//                      mMessageQueue.addIdleHandler(new ActivityGoing(aw));
//                  }
//              }
//          }
//      }

    activity->PerformDestroy();

    if (mActivityMonitors != NULL) {
        Object::Autolock lock(mSync);
        List<AutoPtr<IInstrumentationActivityMonitor> >::Iterator it;
        for (it = mActivityMonitors->Begin(); it != mActivityMonitors->End(); ++it) {
            AutoPtr<IIntent> aIntent;
            activity->GetIntent((IIntent**)&aIntent);
            Boolean result;
            (*it)->Match(activity, activity, aIntent, &result);
        }
    }
    return NOERROR;
}

ECode Instrumentation::CallActivityOnRestoreInstanceState(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* savedInstanceState)
{
    return activity->PerformRestoreInstanceState(savedInstanceState);
}

ECode Instrumentation::CallActivityOnPostCreate(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* icicle)
{
    AutoPtr<Activity> act = reinterpret_cast<Activity*>(activity->Probe(EIID_Activity));
    return act->OnPostCreate(icicle);
}

ECode Instrumentation::CallActivityOnNewIntent(
    /* [in] */ IActivity *activity,
    /* [in] */ IIntent *intent)
{
    AutoPtr<Activity> act = reinterpret_cast<Activity*>(activity->Probe(EIID_Activity));
    return act->OnNewIntent(intent);
}

ECode Instrumentation::CallActivityOnStart(
    /* [in] */ IActivity* activity)
{
    AutoPtr<Activity> act = reinterpret_cast<Activity*>(activity->Probe(EIID_Activity));
    return act->OnStart();
}

ECode Instrumentation::CallActivityOnRestart(
    /* [in] */ IActivity* activity)
{
    AutoPtr<Activity> act = reinterpret_cast<Activity*>(activity->Probe(EIID_Activity));
    return act->OnRestart();
}

ECode Instrumentation::CallActivityOnResume(
    /* [in] */ IActivity* activity)
{
    AutoPtr<Activity> act = reinterpret_cast<Activity*>(activity->Probe(EIID_Activity));
    act->SetResumed(TRUE);
    act->OnResume();

    if (mActivityMonitors != NULL) {
        Object::Autolock lock(mSync);
        Boolean result;
        List<AutoPtr<IInstrumentationActivityMonitor> >::Iterator it;
        for(it = mActivityMonitors->Begin(); it != mActivityMonitors->End(); ++it) {
            AutoPtr<IIntent> intent;
            activity->GetIntent((IIntent**)&intent);
            (*it)->Match(activity, activity, intent, &result);
        }
    }
    return NOERROR;
}

ECode Instrumentation::CallActivityOnStop(
    /* [in] */ IActivity* activity)
{
    AutoPtr<Activity> act = reinterpret_cast<Activity*>(activity->Probe(EIID_Activity));
    return act->OnStop();
}

ECode Instrumentation::CallActivityOnSaveInstanceState(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* outState)
{
    return activity->PerformSaveInstanceState(outState);
}

ECode Instrumentation::CallActivityOnPause(
    /* [in] */ IActivity* activity)
{
    return activity->PerformPause();
}

ECode Instrumentation::CallActivityOnUserLeaving(
    /* [in] */ IActivity* activity)
{
    return activity->PerformUserLeaving();
}

ECode Instrumentation::StartAllocCounting()
{
//     // Before we start trigger a GC and reset the debug counts. Run the
//     // finalizers and another GC before starting and stopping the alloc
//     // counts. This will free up any objects that were just sitting around
//     // waiting for their finalizers to be run.
//     Runtime.getRuntime().gc();
//     Runtime.getRuntime().runFinalization();
//     Runtime.getRuntime().gc();

//     Debug.resetAllCounts();

//     // start the counts
//     Debug.startAllocCounting();
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode Instrumentation::StopAllocCounting()
{
//     Runtime.getRuntime().gc();
//     Runtime.getRuntime().runFinalization();
//     Runtime.getRuntime().gc();
//     Debug.stopAllocCounting();
    assert(0);
    return E_NOT_IMPLEMENTED;
}

void Instrumentation::AddValue(
    /* [in] */ const String& key,
    /* [in] */ Int32 value,
    /* [in] */ IBundle* results)
{
    Boolean contains;
    if (results->ContainsKey(key, &contains), contains) {
        AutoPtr<IObjectContainer> list;
        results->GetIntegerArrayList(key, (IObjectContainer**)&list);
        if (list != NULL) {
            AutoPtr<IInteger32> integer;
            CInteger32::New(value, (IInteger32**)&integer);
            list->Add(integer);
        }
    }
    else {
        AutoPtr<IObjectContainer> list;
        CObjectContainer::New((IObjectContainer**)&list);
        AutoPtr<IInteger32> integer;
        CInteger32::New(value, (IInteger32**)&integer);
        results->PutIntegerArrayList(key, list);
    }
}

ECode Instrumentation::GetAllocCounts(
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle)
    AutoPtr<IBundle> results;
    CBundle::New((IBundle**)&results);
    // results->PutInt64(String("global_alloc_count"), Debug.getGlobalAllocCount());
    // results->PutInt64(String("global_alloc_size"), Debug.getGlobalAllocSize());
    // results->PutInt64(String("global_freed_count"), Debug.getGlobalFreedCount());
    // results->PutInt64(String("global_freed_size"), Debug.getGlobalFreedSize());
    // results->PutInt64(String("gc_invocation_count"), Debug.getGlobalGcInvocationCount());
    assert(0);
    *bundle = results;
    REFCOUNT_ADD(*bundle)
    return NOERROR;
}

ECode Instrumentation::GetBinderCounts(
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle)
    AutoPtr<IBundle> results;
    CBundle::New((IBundle**)&results);
    // results->PutInt64(String("sent_transactions"), Debug.getBinderSentTransactions());
    // results->PutInt64(String("received_transactions"), Debug.getBinderReceivedTransactions());
    assert(0);
    *bundle = results;
    REFCOUNT_ADD(*bundle)
    return NOERROR;
}

ECode Instrumentation::ExecStartActivity(
    /* [in] */ IContext* who,
    /* [in] */ IBinder* contextThread,
    /* [in] */ IBinder* token,
    /* [in] */ IActivity* target,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IBundle* options,
    /* [out] */ IInstrumentationActivityResult** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    // BEGIN privacy-added
    Boolean allowIntent = TRUE;
    // try{
    String packageName;
    who->GetPackageName(&packageName);
    Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity: execStartActivity for %s", packageName.string());
    String action;
    intent->GetAction(&action);
    if (!action.IsNull() && (action.Equals(IIntent::ACTION_CALL) || action.Equals(IIntent::ACTION_DIAL))) {
        allowIntent = FALSE;
        Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity: Intent action = Intent.ACTION_CALL or Intent.ACTION_DIAL for "
                , packageName.string());
        Boolean isAvailable;
        if (mPrvSvc == NULL || mPrvSvc->IsServiceAvailable(&isAvailable), !isAvailable) {
            // mPrvSvc = new PrivacySettingsManager(who, IPrivacySettingsManager.Stub.asInterface(ServiceManager.getService("privacy")));
            AutoPtr<IIPrivacySettingsManager> manager = (IIPrivacySettingsManager*)ServiceManager::GetService(String("privacy")).Get();
            mPrvSvc = NULL;
            CPrivacySettingsManager::New(who, manager, (IPrivacySettingsManager**)&mPrvSvc);
            if (mPrvSvc != NULL) {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity: Obtained privacy service");
            }
            else {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity: Privacy service not obtained");
            }
        }
        else {
            Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity: Already had privacy service");
        }

        if (mPrvSvc->IsServiceAvailable(&isAvailable), !isAvailable) {
            Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity: Privacy service not available: rejecting call attempt");
            allowIntent = FALSE;
            mPrvSvc->Notification(packageName, IPrivacySettings::EMPTY, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
        }
        else {
            AutoPtr<IPrivacySettings> privacySettings;
            mPrvSvc->GetSettings(packageName, (IPrivacySettings**)&privacySettings);
            Byte phoneCallSetting;
            if (privacySettings == NULL) {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity: Call allowed: No settings for package: %s", packageName.string());
                allowIntent = TRUE;
                mPrvSvc->Notification(packageName, IPrivacySettings::REAL, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
            }
            else if (phoneCallSetting == IPrivacySettings::REAL) {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity: Call allowed: Settings permit ", packageName.string());
                allowIntent = TRUE;
                mPrvSvc->Notification(packageName, phoneCallSetting, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
            }
            else {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity: Call denied: Settings deny ", packageName.string());
                // No settings = allowed; any phone call setting but real == disallowed

                allowIntent = FALSE;

                // test if broadcasting works! SM: I don't know what 'test if broadcasting works' means.
                // Send the notification intent
                AutoPtr<IContext> tmp = who;
                // SM: the BLOCKED_PHONE_CALL intent is handled by the privacy service to fake a change in call state
                AutoPtr<IRunnable> runnable = new BlockPhoneCallRunnable(tmp);
                AutoPtr<IThread> t;
                CThread::New(runnable, (IThread**)&t);
                t->Start();
                privacySettings->GetPhoneCallSetting(&phoneCallSetting);
                mPrvSvc->Notification(packageName, phoneCallSetting, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
            }
        }
    }
    // } catch(Exception e){
    //      if(who != null) {
    //          Log.e(TAG,"PDroid:Instrumentation:execStartActivity: Exception occurred handling intent for ", packageName.string(), e);
    //      } else {
    //          Log.e(TAG,"PDroid:Instrumentation:execStartActivity: Exception occurred handling intent for unknown package", e);
    //      }
    // }
    // END privacy-added

    AutoPtr<IApplicationThread> whoThread = IApplicationThread::Probe(contextThread);
    if (mActivityMonitors != NULL) {
        Object::Autolock lock(mSync);
        List<AutoPtr<IInstrumentationActivityMonitor> >::Iterator it;
        for(it = mActivityMonitors->Begin(); it != mActivityMonitors->End(); ++it) {
            AutoPtr<IInstrumentationActivityMonitor> am = *it;
            Boolean match;
            if (am->Match(who, NULL, intent, &match), match) {
                Int32 time;
                am->GetHits(&time);
                am->SetHits(time++);
                Boolean isBlock;
                if (am->IsBlocking(&isBlock), isBlock) {
                    if(requestCode >= 0) {
                        return am->GetResult(result);
                    }
                    else {
                        return NOERROR;
                    }
                }
                break;
            }
        }
    }

    // BEGIN privacy-added
    // try{
    if (!allowIntent) {
        return CInstrumentationActivityResult::New(requestCode, intent, result);
    }
    // } catch(Exception e) {
    //     Log.e(TAG,"PDroid:Instrumentation:execStartActivity: Exception occurred while trying to create ActivityResult", e);
    //     return null;
    // }
    // END privacy-added

    intent->SetAllowFds(FALSE);
    Boolean bval;
    intent->MigrateExtraStreamToClipData(&bval);
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    AutoPtr<IContentResolver> resolver;
    who->GetContentResolver((IContentResolver**)&resolver);
    String type;
    intent->ResolveTypeIfNeeded(resolver, &type);
    String resultWho;
    if (target != NULL) target->GetID(&resultWho);
    Int32 res;
    am->StartActivity(whoThread, intent, type,
             token, resultWho, requestCode, 0, String(NULL), NULL, options, &res);
    CheckStartActivityResult(res, intent);
    *result = NULL;
    return NOERROR;
}

ECode Instrumentation::ExecStartActivities(
    /* [in] */ IContext* who,
    /* [in] */ IBinder* contextThread,
    /* [in] */ IBinder* token,
    /* [in] */ IActivity* target,
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options)
{
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    Int32 id;
    helper->GetMyUserId(&id);
    return ExecStartActivitiesAsUser(who, contextThread, token, target, intents, options, id);
}

ECode Instrumentation::ExecStartActivitiesAsUser(
    /* [in] */ IContext* who,
    /* [in] */ IBinder* contextThread,
    /* [in] */ IBinder* token,
    /* [in] */ IActivity* target,
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options,
    /* [in] */ Int32 userId)
{
    AutoPtr<IApplicationThread> whoThread = IApplicationThread::Probe(contextThread);

    // BEGIN privacy-added
    String packageName;
    who->GetPackageName(&packageName);
    Slogger::D(TAG, "PDroid:Instrumentation:execStartActivitiesAsUser: execStartActivitiesAsUser for %s", packageName.string());
    if (intents != NULL) {
        Boolean checkPrivacySettings = FALSE;

        // If any intents are Intent.ACTION_CALL or Intent.ACTION_DIAL, need to check permissions
        for (Int32 i = 0; i < intents->GetLength(); ++i) {
            AutoPtr<IIntent> intent = (*intents)[i];
            // try {
            String action;
            intent->GetAction(&action);
            if (!action.IsNull() && (action.Equals(IIntent::ACTION_CALL) || action.Equals(IIntent::ACTION_DIAL))) {
                checkPrivacySettings = TRUE;
                break;
            }
            // } catch (Exception e) {
            //     Log.e(TAG,"PDroid:Instrumentation:execStartActivitiesAsUser: Exception occurred when checking intents for ", packageName.string(), e);
            //     // If an exception occurred, then check the privacy settings as the default action
            //     checkPrivacySettings = true;
            // }
        }

        if (!checkPrivacySettings) {
            Slogger::D(TAG, "PDroid:Instrumentation:execStartActivitiesAsUser: No provided intents triggered checking for %s"
                    , packageName.string());
        }
        else {
            Slogger::D(TAG, "PDroid:Instrumentation:execStartActivitiesAsUser: One or more intents triggered checking for %s"
                    , packageName.string());

            Boolean isAvailable;
            if (mPrvSvc == NULL || (mPrvSvc->IsServiceAvailable(&isAvailable), !isAvailable)) {
                // mPrvSvc = new PrivacySettingsManager(who, IPrivacySettingsManager.Stub.asInterface(ServiceManager.getService("privacy")));
                AutoPtr<IIPrivacySettingsManager> psm = (IIPrivacySettingsManager*)ServiceManager::GetService(String("privacy")).Get();
                mPrvSvc = NULL;
                CPrivacySettingsManager::New(who, psm, (IPrivacySettingsManager**)&mPrvSvc);
                if (mPrvSvc != NULL) {
                    Slogger::D(TAG, "PDroid:Instrumentation:execStartActivitiesAsUser: Obtained privacy service");
                }
                else {
                    Slogger::D(TAG, "PDroid:Instrumentation:execStartActivitiesAsUser: Privacy service not obtained");
                }
            }
            else {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivitiesAsUser: Already had privacy service");
            }

            Boolean allowCallIntents = FALSE;
            if (mPrvSvc->IsServiceAvailable(&isAvailable), &isAvailable) {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivitiesAsUser: Privacy service not available - assuming permission denied");
                allowCallIntents = FALSE;
                mPrvSvc->Notification(packageName, IPrivacySettings::EMPTY, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
            }
            else {
                AutoPtr<IPrivacySettings> privacySettings;
                mPrvSvc->GetSettings(packageName, (IPrivacySettings**)&privacySettings);
                Byte phoneCallSetting;
                if (privacySettings == NULL) {
                    Slogger::D(TAG, "PDroid:Instrumentation:execStartActivitiesAsUser: Call intents allowed: No settings for package: %s"
                            , packageName.string());
                    allowCallIntents = TRUE;
                    mPrvSvc->Notification(packageName, IPrivacySettings::EMPTY, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
                }
                else if (privacySettings->GetPhoneCallSetting(&phoneCallSetting), phoneCallSetting == IPrivacySettings::REAL) {
                    Slogger::D(TAG, "PDroid:Instrumentation:execStartActivitiesAsUser: Call intents allowed: Settings permit %s"
                            , packageName.string());
                    allowCallIntents = TRUE;
                    mPrvSvc->Notification(packageName, phoneCallSetting, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
                }
                else {
                    Slogger::D(TAG, "PDroid:Instrumentation:execStartActivitiesAsUser: Call intents denied: Settings deny %s"
                            , packageName.string());
                    allowCallIntents = FALSE;
                    privacySettings->GetPhoneCallSetting(&phoneCallSetting);
                    mPrvSvc->Notification(packageName, phoneCallSetting, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
                }
            }

            // If call intents are not allowed, need to regenerate the
            // intents list to remove call-related intents
            if (!allowCallIntents) {
                AutoPtr< ArrayOf<IIntent*> > filteredIntents = ArrayOf<IIntent*>::Alloc(intents->GetLength());
                for (Int32 i = 0, j = 0; i < intents->GetLength(); i++) {
                    // try {
                    AutoPtr<IIntent> intent = (*intents)[i];
                    String action;
                    intent->GetAction(&action);
                    if (action.IsNull() || !(action.Equals(IIntent::ACTION_CALL) || action.Equals(IIntent::ACTION_DIAL))) {
                        (*filteredIntents)[j] = intent;
                        j++;
                    }
                    // } catch (Exception e) {
                    //     Log.e(TAG,"PDroid:Instrumentation:execStartActivitiesAsUser: Exception occurred when checking intent for ", packageName.string(), e);
                    // }
                }
                intents->Copy(filteredIntents);

                // Send the notification intent
                AutoPtr<IContext> tmp = who;
                // SM: the BLOCKED_PHONE_CALL intent is handled by the privacy service to fake a change in call state
                AutoPtr<IRunnable> runnable = (IRunnable*)new BlockPhoneCallRunnable(tmp);
                AutoPtr<IThread> t;
                CThread::New(runnable, (IThread**)&t);
                t->Start();
            }
        }
    }
    // END privacy-added

    if (mActivityMonitors != NULL) {
        Object::Autolock lock(mSync);
        List<AutoPtr<IInstrumentationActivityMonitor> >::Iterator it;
        for(it = mActivityMonitors->Begin(); it != mActivityMonitors->End(); ++it) {
            AutoPtr<IInstrumentationActivityMonitor> am = *it;
            Boolean match;
            am->Match(who, NULL, (*intents)[0], &match);
            if (match) {
                Int32 time;
                am->GetHits(&time);
                am->SetHits(++time);
                Boolean isBlock;
                am->IsBlocking(&isBlock);
                if (isBlock) {
                    return NOERROR;
                }
                break;
            }
        }
    }

    AutoPtr<IContentResolver> resolver;
    who->GetContentResolver((IContentResolver**)&resolver);
    Int32 length = intents->GetLength();
    AutoPtr <ArrayOf<String> > resolvedTypes = ArrayOf<String>::Alloc(length);
    for (Int32 i = 0; i < length; ++i) {
        (*intents)[i]->SetAllowFds(FALSE);
        String type;
        (*intents)[i]->ResolveTypeIfNeeded(resolver, &type);
        (*resolvedTypes)[i] = type;
    }

    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    Int32 res;
    FAIL_RETURN(am->StartActivities(whoThread, intents, resolvedTypes,
            token, options, userId, &res))
    return CheckStartActivityResult(res, (*intents)[0]);
}

ECode Instrumentation::ExecStartActivity(
    /* [in] */ IContext* who,
    /* [in] */ IBinder* contextThread,
    /* [in] */ IBinder* token,
    /* [in] */ IFragment* target,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IBundle* options,
    /* [out] */ IInstrumentationActivityResult** activityResult)
{
    VALIDATE_NOT_NULL(activityResult)
    *activityResult = NULL;

    AutoPtr<IApplicationThread> whoThread = IApplicationThread::Probe(contextThread);

    // BEGIN privacy-added
    Boolean allowIntent = TRUE;
    // try{
    String packageName;
    who->GetPackageName(&packageName);
    Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with Fragments): execStartActivity for %S", packageName.string());
    String action;
    intent->GetAction(&action);
    if (!action.IsNull() && (action.Equals(IIntent::ACTION_CALL) || action.Equals(IIntent::ACTION_DIAL))) {
        allowIntent = FALSE;
        Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with Fragments): Intent action = Intent.ACTION_CALL or Intent.ACTION_DIAL for "
                , packageName.string());
        Boolean isAvailable;
        if (mPrvSvc == NULL || (mPrvSvc->IsServiceAvailable(&isAvailable), !isAvailable)) {
            // mPrvSvc = new PrivacySettingsManager(who, IPrivacySettingsManager.Stub.asInterface(ServiceManager.getService("privacy")));
            AutoPtr<IIPrivacySettingsManager> psm = (IIPrivacySettingsManager*)ServiceManager::GetService(String("privacy")).Get();
            mPrvSvc = NULL;
            CPrivacySettingsManager::New(who, psm, (IPrivacySettingsManager**)&mPrvSvc);
            if (mPrvSvc != NULL) {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with Fragments): Obtained privacy service");
            }
            else {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with Fragments): Privacy service not obtained");
            }
        }
        else {
            Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with Fragments): Already had privacy service");
        }

        if (mPrvSvc->IsServiceAvailable(&isAvailable), !isAvailable) {
            Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with Fragments): Privacy service not available: rejecting call attempt");
            allowIntent = FALSE;
            mPrvSvc->Notification(packageName, IPrivacySettings::EMPTY, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
        }
        else {
            AutoPtr<IPrivacySettings> privacySettings;
            mPrvSvc->GetSettings(packageName, (IPrivacySettings**)&privacySettings);
            Byte phoneCallSetting;
            if (privacySettings == NULL) {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with Fragments): Call allowed: No settings for package: "
                        , packageName.string());
                allowIntent = TRUE;
                mPrvSvc->Notification(packageName, IPrivacySettings::REAL, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
            }
            else if (privacySettings->GetPhoneCallSetting(&phoneCallSetting), phoneCallSetting == IPrivacySettings::REAL) {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with Fragments): Call allowed: Settings permit "
                        , packageName.string());
                allowIntent = TRUE;
                mPrvSvc->Notification(packageName, phoneCallSetting, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
            }
            else {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with Fragments): Call denied: Settings deny ", packageName.string());
                // No settings = allowed; any phone call setting but real == disallowed

                // test if broadcasting works! SM: I don't know what 'test if broadcasting works' means.
                // Send the notification intent
                AutoPtr<IContext> tmp = who;
                allowIntent = FALSE;
                // SM: Why is all of this done? It seems like a weirdly unnecessary bit of code...
                AutoPtr<IRunnable> runnable = new BlockPhoneCallRunnable(tmp);
                AutoPtr<IThread> t;
                CThread::New(runnable, (IThread**)&t);
                t->Start();
                privacySettings->GetPhoneCallSetting(&phoneCallSetting);
                mPrvSvc->Notification(packageName, phoneCallSetting, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
            }
        }
    }
    // } catch(Exception e){
    //      if(who != null) {
    //          Log.e(TAG,"PDroid:Instrumentation:execStartActivity (with Fragments): Exception occurred handling intent for ", packageName.string(), e);
    //      } else {
    //          Log.e(TAG,"PDroid:Instrumentation:execStartActivity (with Fragments): Exception occurred handling intent for unknown package", e);
    //      }
    // }
    // END privacy-added


    if (mActivityMonitors != NULL) {
        Object::Autolock lock(mSync);
        if (!mActivityMonitors->IsEmpty()) {
            List<AutoPtr<IInstrumentationActivityMonitor> >::Iterator it;
            for(it = mActivityMonitors->Begin(); it != mActivityMonitors->End(); ++it) {
                AutoPtr<IInstrumentationActivityMonitor> am = *it;
                Boolean match;
                am->Match(who, NULL, intent, &match);
                if (match) {
                    Int32 time;
                    am->GetHits(&time);
                    am->SetHits(++time);
                    Boolean isBlock;
                    am->IsBlocking(&isBlock);
                    if (isBlock) {
                        if(requestCode >= 0 ) {
                            return am->GetResult(activityResult);
                        }
                        else {
                            return NOERROR;
                        }
                    }
                    break;
                }
            }
        }
    }

    // BEGIN privacy-added
    // try{
    if (!allowIntent) return CInstrumentationActivityResult::New(requestCode, intent, activityResult);
    // } catch(Exception e) {
    //     Log.e(TAG,"PDroid:Instrumentation:execStartActivity (with Fragments): Exception occurred while trying to create ActivityResult", e);
    //     return null;
    // }
    // END privacy-added

    intent->SetAllowFds(FALSE);
    Boolean bval;
    intent->MigrateExtraStreamToClipData(&bval);
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    AutoPtr<IContentResolver> resolver;
    who->GetContentResolver((IContentResolver**)&resolver);
    String type;
    intent->ResolveTypeIfNeeded(resolver, &type);
    String whoStr;
    if (target) target->GetWho(&whoStr);
    Int32 res;
    FAIL_RETURN(am->StartActivity(whoThread, intent, type,
             token, whoStr, requestCode, 0, String(NULL), NULL, options, &res))
    return CheckStartActivityResult(res, intent);
}

ECode Instrumentation::ExecStartActivity(
    /* [in] */ IContext* who,
    /* [in] */ IBinder* contextThread,
    /* [in] */ IBinder* token,
    /* [in] */ IActivity* target,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IBundle* options,
    /* [in] */ IUserHandle* user,
    /* [out] */ IInstrumentationActivityResult** activityResult)
{
    VALIDATE_NOT_NULL(activityResult)
    *activityResult = NULL;

    AutoPtr<IApplicationThread> whoThread = IApplicationThread::Probe(contextThread);

    // BEGIN privacy-added
    Boolean allowIntent = TRUE;
    // try{
    String packageName;
    who->GetPackageName(&packageName);
    Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with UserHandle): execStartActivity for "
            , packageName.string());
    String action;
    intent->GetAction(&action);
    if (!action.IsNull() && (action.Equals(IIntent::ACTION_CALL) || action.Equals(IIntent::ACTION_DIAL))) {
        allowIntent = FALSE;
        Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with UserHandle): Intent action = Intent.ACTION_CALL or Intent.ACTION_DIAL for "
                , packageName.string());
        Boolean isAvailable;
        if (mPrvSvc == NULL || (mPrvSvc->IsServiceAvailable(&isAvailable), !isAvailable)) {
            // mPrvSvc = new PrivacySettingsManager(who, IPrivacySettingsManager.Stub.asInterface(ServiceManager.getService("privacy")));
            AutoPtr<IIPrivacySettingsManager> psm = (IIPrivacySettingsManager*)ServiceManager::GetService(String("privacy")).Get();
            mPrvSvc = NULL;
            CPrivacySettingsManager::New(who, psm, (IPrivacySettingsManager**)&mPrvSvc);
            if (mPrvSvc != NULL) {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with UserHandle): Obtained privacy service");
            }
            else {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with UserHandle): Privacy service not obtained");
            }
        }
        else {
            Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with UserHandle): Already had privacy service");
        }

        if (mPrvSvc->IsServiceAvailable(&isAvailable), !isAvailable) {
            Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with UserHandle): Privacy service not available: rejecting call attempt");
            allowIntent = FALSE;
            mPrvSvc->Notification(packageName, IPrivacySettings::EMPTY, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
        }
        else {
            AutoPtr<IPrivacySettings> privacySettings;
            mPrvSvc->GetSettings(packageName, (IPrivacySettings**)&privacySettings);
            Byte phoneCallSetting;
            if (privacySettings == NULL) {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with UserHandle): Call allowed: No settings for package: "
                        , packageName.string());
                allowIntent = TRUE;
                mPrvSvc->Notification(packageName, IPrivacySettings::REAL, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
            }
            else if (privacySettings->GetPhoneCallSetting(&phoneCallSetting), phoneCallSetting == IPrivacySettings::REAL) {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with UserHandle): Call allowed: Settings permit "
                        , packageName.string());
                allowIntent = TRUE;
                mPrvSvc->Notification(packageName, phoneCallSetting, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
            }
            else {
                Slogger::D(TAG, "PDroid:Instrumentation:execStartActivity (with UserHandle): Call denied: Settings deny ", packageName.string());
                // No settings = allowed; any phone call setting but real == disallowed

                // test if broadcasting works! SM: I don't know what 'test if broadcasting works' means.
                // Send the notification intent
                AutoPtr<IContext> tmp = who;
                allowIntent = FALSE;
                // SM: Why is all of this done? It seems like a weirdly unnecessary bit of code...
                AutoPtr<IRunnable> runnable = new BlockPhoneCallRunnable(tmp);
                AutoPtr<IThread> t;
                CThread::New(runnable, (IThread**)&t);
                t->Start();
                privacySettings->GetPhoneCallSetting(&phoneCallSetting);
                mPrvSvc->Notification(packageName, phoneCallSetting, IPrivacySettings::DATA_PHONE_CALL, String(NULL));
            }
        }
    }
    // } catch(Exception e){
    //      if(who != null) {
    //          Log.e(TAG,"PDroid:Instrumentation:execStartActivity (with UserHandle): Exception occurred handling intent for ", packageName.string(), e);
    //      } else {
    //          Log.e(TAG,"PDroid:Instrumentation:execStartActivity (with UserHandle): Exception occurred handling intent for unknown package", e);
    //      }
    // }
    // END privacy-added

    if (mActivityMonitors != NULL) {
        Object::Autolock lock(mSync);
        if (!mActivityMonitors->IsEmpty()) {
            List<AutoPtr<IInstrumentationActivityMonitor> >::Iterator it;
            for(it = mActivityMonitors->Begin(); it != mActivityMonitors->End(); ++it) {
                AutoPtr<IInstrumentationActivityMonitor> am = *it;
                Boolean match;
                am->Match(who, NULL, intent, &match);
                if (match) {
                    Int32 time;
                    am->GetHits(&time);
                    am->SetHits(++time);
                    Boolean isBlock;
                    am->IsBlocking(&isBlock);
                    if (isBlock) {
                        if(requestCode >= 0 ) {
                            return am->GetResult(activityResult);
                        }
                        else {
                            return NOERROR;
                        }
                    }
                    break;
                }
            }
        }
    }

     // BEGIN privacy-added
    // try{
    if (!allowIntent) return CInstrumentationActivityResult::New(requestCode, intent, activityResult);
    // } catch(Exception e) {
    //     Log.e(TAG,"PDroid:Instrumentation:execStartActivity (with UserHandle): Exception occurred while trying to create ActivityResult", e);
    //     return null;
    // }
    // END privacy-added

    intent->SetAllowFds(FALSE);
    Boolean bval;
    intent->MigrateExtraStreamToClipData(&bval);
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    AutoPtr<IContentResolver> resolver;
    who->GetContentResolver((IContentResolver**)&resolver);
    String type;
    intent->ResolveTypeIfNeeded(resolver, &type);
    String resultWho;
    if (target != NULL) target->GetID(&resultWho);
    Int32 userId;
    user->GetIdentifier(&userId);
    Int32 res;
    FAIL_RETURN(am->StartActivityAsUser(whoThread, intent, type,
             token, resultWho, requestCode, 0, String(NULL), NULL, options, userId, &res))
    return CheckStartActivityResult(res, intent);
}

ECode Instrumentation::Init(
    /* [in] */ IActivityThread* thread,
    /* [in] */ IContext* instrContext,
    /* [in] */ IContext* appContext,
    /* [in] */ IComponentName* component,
    /* [in] */ IInstrumentationWatcher* watcher)
{
    mThread = thread;
    AutoPtr<ILooper> looper;
    mThread->GetLooper((ILooper**)&looper);
    looper->GetQueue((IMessageQueue**)&mMessageQueue);
    mInstrContext = instrContext;
    mAppContext = appContext;
    mComponent = component;
    mWatcher = watcher;
    return NOERROR;
}

ECode Instrumentation::CheckStartActivityResult(
    /* [in] */ Int32 res,
    /* [in] */ IIntent* intent)
{
    if (res >= IActivityManager::START_SUCCESS) {
         return NOERROR;
    }

    AutoPtr<IComponentName> component;
    intent->GetComponent((IComponentName**)&component);
    switch (res) {
        case IActivityManager::START_INTENT_NOT_RESOLVED:
        case IActivityManager::START_CLASS_NOT_FOUND:
            if (component != NULL) {
                //throw new ActivityNotFoundException(
                //        "Unable to find explicit activity class "
                //        + ((Intent)intent).getComponent().toShortString()
                //        + "; have you declared this activity in your AndroidManifest.xml?");
                String shortStr;
                component->ToShortString(&shortStr);
                Slogger::E(TAG, "Unable to find explicit activity class %s; have you declared this activity in your AndroidManifest.xml?"
                        , shortStr.string());
                return E_ACTIVITY_NOT_FOUND_EXCEPTION;
            }
            //throw new ActivityNotFoundException(
            //        "No Activity found to handle " + intent);
            Slogger::E(TAG, "No Activity found to handle %p", intent);
            return E_ACTIVITY_NOT_FOUND_EXCEPTION;
        case IActivityManager::START_PERMISSION_DENIED:
            //throw new SecurityException("Not allowed to start activity "
            //         + intent);
            Slogger::E(TAG, "Not allowed to start activity %p", intent);
            return E_SECURITY_EXCEPTION;
        case IActivityManager::START_FORWARD_AND_REQUEST_CONFLICT:
            // throw new AndroidRuntimeException(
            //         "FORWARD_RESULT_FLAG used while also requesting a result");
            Slogger::E(TAG, "FORWARD_RESULT_FLAG used while also requesting a result");
            return E_RUNTIME_EXCEPTION;
        case IActivityManager::START_NOT_ACTIVITY:
            //throw new IllegalArgumentException(
            //        "PendingIntent is not an activity");
            Slogger::E(TAG, "PendingIntent is not an activity");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        default:
            //throw new AndroidRuntimeException("Unknown error code "
            //        + res + " when starting " + intent);
            Slogger::E(TAG, "Unknown error code %d when starting %p", res, intent);
            return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode Instrumentation::ValidateNotAppThread()
{
    AutoPtr<IActivityThreadHelper> helper;
    CActivityThreadHelper::AcquireSingleton((IActivityThreadHelper**)&helper);
    AutoPtr<IActivityThread> at;
    if (helper->GetCurrentActivityThread((IActivityThread**)&at), at != NULL) {
//         throw new RuntimeException(
//             "This method can not be called from the main application thread");
        Slogger::E(TAG, "This method can not be called from the main application thread");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos

