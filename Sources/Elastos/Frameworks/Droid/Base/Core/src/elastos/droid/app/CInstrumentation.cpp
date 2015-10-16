
#include "elastos/droid/app/CInstrumentation.h"

namespace Elastos {
namespace Droid {
namespace App {

PInterface CInstrumentation::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IActivity*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    else if (riid == EIID_IInstrumentation) {
        return (IInstrumentation*)this;
    }
    return NULL;
}

UInt32 CInstrumentation::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CInstrumentation::Release()
{
    return ElRefBase::Release();
}

ECode CInstrumentation::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IInstrumentation *)this) {
        *pIID = EIID_IInstrumentation;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }
    return E_INVALID_ARGUMENT;
}

ECode CInstrumentation::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInstrumentation::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInstrumentation::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CInstrumentation::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CInstrumentation::GetHashCode(
    /* [out] */ Int32* hash)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CInstrumentation::ToString(
    /* [out] */ String* str)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CInstrumentation::OnCreate(
    /* [in] */ IBundle* arguments)
{
    return Instrumentation::OnCreate(arguments);
}

ECode CInstrumentation::Start()
{
    return Instrumentation::Start();
}

ECode CInstrumentation::OnStart()
{
    return Instrumentation::OnStart();
}

ECode CInstrumentation::OnException(
    /* [in] */ IInterface* obj,
    /* [in] */ ECode e,
    /* [out] */ Boolean* result)
{
    return Instrumentation::OnException(obj, e, result);
}

ECode CInstrumentation::SendStatus(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* results)
{
    return Instrumentation::SendStatus(resultCode, results);
}

ECode CInstrumentation::Finish(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* results)
{
    return Instrumentation::Finish(resultCode, results);
}

ECode CInstrumentation::SetAutomaticPerformanceSnapshots()
{
    return Instrumentation::SetAutomaticPerformanceSnapshots();
}

ECode CInstrumentation::StartPerformanceSnapshot()
{
    return Instrumentation::StartPerformanceSnapshot();
}

ECode CInstrumentation::EndPerformanceSnapshot()
{
    return Instrumentation::EndPerformanceSnapshot();
}

ECode CInstrumentation::OnDestroy()
{
    return Instrumentation::OnDestroy();
}

ECode CInstrumentation::GetContext(
    /* [out] */ IContext** context)
{
    return Instrumentation::GetContext(context);
}

ECode CInstrumentation::GetComponentName(
    /* [out] */ IComponentName** component)
{
    return Instrumentation::GetComponentName(component);
}

ECode CInstrumentation::GetTargetContext(
    /* [out] */ IContext** context)
{
    return Instrumentation::GetTargetContext(context);
}

ECode CInstrumentation::IsProfiling(
    /* [out] */ Boolean* isProfiling)
{
    return Instrumentation::IsProfiling(isProfiling);
}

ECode CInstrumentation::StartProfiling()
{
    return Instrumentation::StartProfiling();
}

ECode CInstrumentation::StopProfiling()
{
    return Instrumentation::StopProfiling();
}

ECode CInstrumentation::SetInTouchMode(
    /* [in] */ Boolean inTouch)
{
    return Instrumentation::SetInTouchMode(inTouch);
}

ECode CInstrumentation::WaitForIdle(
    /* [in] */ IRunnable* recipient)
{
    return Instrumentation::WaitForIdle(recipient);
}

ECode CInstrumentation::WaitForIdleSync()
{
    return Instrumentation::WaitForIdleSync();
}

ECode CInstrumentation::RunOnMainSync(
    /* [in] */ IRunnable* runner)
{
    return Instrumentation::RunOnMainSync(runner);
}

ECode CInstrumentation::StartActivitySync(
    /* [in] */ IIntent* intent,
    /* [out] */ IActivity** activity)
{
    return Instrumentation::StartActivitySync(intent, activity);
}

ECode CInstrumentation::AddMonitor(
    /* [in] */ IInstrumentationActivityMonitor* monitor)
{
    return Instrumentation::AddMonitor(monitor);
}

ECode CInstrumentation::AddMonitor(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ IInstrumentationActivityResult* result,
    /* [in] */ Boolean block,
    /* [out] */ IInstrumentationActivityMonitor** monitor)
{
    return Instrumentation::AddMonitor(filter, result, block, monitor);
}

ECode CInstrumentation::AddMonitor(
    /* [in] */ const String& cls,
    /* [in] */ IInstrumentationActivityResult* result,
    /* [in] */ Boolean block,
    /* [out] */ IInstrumentationActivityMonitor** monitor)
{
    return Instrumentation::AddMonitor(cls, result, block, monitor);
}

ECode CInstrumentation::CheckMonitorHit(
    /* [in] */ IInstrumentationActivityMonitor* monitor,
    /* [in] */ Int32 minHits,
    /* [out] */ Boolean* reached)
{
    return Instrumentation::CheckMonitorHit(monitor, minHits, reached);
}

ECode CInstrumentation::WaitForMonitor(
    /* [in] */ IInstrumentationActivityMonitor* monitor,
    /* [out] */ IActivity** activity)
{
    return Instrumentation::WaitForMonitor(monitor, activity);
}

ECode CInstrumentation::WaitForMonitorWithTimeout(
    /* [in] */ IInstrumentationActivityMonitor* monitor,
    /* [in] */ Int64 timeOut,
    /* [out] */ IActivity** activity)
{
    return Instrumentation::WaitForMonitorWithTimeout(monitor, timeOut, activity);
}

ECode CInstrumentation::RemoveMonitor(
    /* [in] */ IInstrumentationActivityMonitor* monitor)
{
    return Instrumentation::RemoveMonitor(monitor);
}

ECode CInstrumentation::InvokeMenuActionSync(
    /* [in] */ IActivity* targetActivity,
    /* [in] */ Int32 id,
    /* [in] */ Int32 flag,
    /* [out] */ Boolean* success)
{
    return Instrumentation::InvokeMenuActionSync(targetActivity, id, flag, success);
}

ECode CInstrumentation::InvokeContextMenuAction(
    /* [in] */ IActivity* targetActivity,
    /* [in] */ Int32 id,
    /* [in] */ Int32 flag,
    /* [out] */ Boolean* success)
{
    return Instrumentation::InvokeContextMenuAction(targetActivity, id, flag, success);
}

ECode CInstrumentation::SendStringSync(
    /* [in] */ const String& text)
{
    return Instrumentation::SendStringSync(text);
}

ECode CInstrumentation::SendKeySync(
    /* [in] */ IKeyEvent* event)
{
    return Instrumentation::SendKeySync(event);
}

ECode CInstrumentation::SendKeyDownUpSync(
    /* [in] */ Int32 key)
{
    return Instrumentation::SendKeyDownUpSync(key);
}

ECode CInstrumentation::SendCharacterSync(
    /* [in] */ Int32 keyCode)
{
    return Instrumentation::SendCharacterSync(keyCode);
}

ECode CInstrumentation::SendPointerSync(
    /* [in] */ IMotionEvent* event)
{
    return Instrumentation::SendPointerSync(event);
}

ECode CInstrumentation::SendTrackballEventSync(
    /* [in] */ IMotionEvent* event)
{
    return Instrumentation::SendTrackballEventSync(event);
}

ECode CInstrumentation::NewApplication(
    /* [in] */ IClassLoader* cl,
    /* [in] */ const String& className,
    /* [in] */ IContext* ctx,
    /* [out] */ IApplication** app)
{
    return Instrumentation::NewApplication(cl, className, ctx, app);
}

ECode CInstrumentation::CallApplicationOnCreate(
    /* [in] */ IApplication* app)
{
    return Instrumentation::CallApplicationOnCreate(app);
}

ECode CInstrumentation::NewActivity(
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
    return Instrumentation::NewActivity(clazz, context, token, application, intent, info,
            title, parent, id, lastNonConfigurationInstance, activity);
}

ECode CInstrumentation::NewActivity(
    /* [in] */ IClassLoader* cl,
    /* [in] */ const String& className,
    /* [in] */ IIntent* intent,
    /* [out] */ IActivity** activity)
{
    return Instrumentation::NewActivity(cl, className, intent, activity);
}

ECode CInstrumentation::CallActivityOnCreate(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* icicle)
{
    return Instrumentation::CallActivityOnCreate(activity, icicle);
}

ECode CInstrumentation::CallActivityOnDestroy(
    /* [in] */ IActivity* activity)
{
    return Instrumentation::CallActivityOnDestroy(activity);
}

ECode CInstrumentation::CallActivityOnRestoreInstanceState(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* savedInstanceState)
{
    return Instrumentation::CallActivityOnRestoreInstanceState(activity, savedInstanceState);
}

ECode CInstrumentation::CallActivityOnPostCreate(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* icicle)
{
    return Instrumentation::CallActivityOnPostCreate(activity, icicle);
}

ECode CInstrumentation::CallActivityOnNewIntent(
    /* [in] */ IActivity* activity,
    /* [in] */ IIntent* intent)
{
    return Instrumentation::CallActivityOnNewIntent(activity, intent);
}

ECode CInstrumentation::CallActivityOnStart(
    /* [in] */ IActivity* activity)
{
    return Instrumentation::CallActivityOnStart(activity);
}

ECode CInstrumentation::CallActivityOnRestart(
    /* [in] */ IActivity* activity)
{
    return Instrumentation::CallActivityOnRestart(activity);
}

ECode CInstrumentation::CallActivityOnResume(
    /* [in] */ IActivity* activity)
{
    return Instrumentation::CallActivityOnResume(activity);
}

ECode CInstrumentation::CallActivityOnStop(
    /* [in] */ IActivity* activity)
{
    return Instrumentation::CallActivityOnStop(activity);
}

ECode CInstrumentation::CallActivityOnSaveInstanceState(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* outState)
{
    return Instrumentation::CallActivityOnSaveInstanceState(activity, outState);
}

ECode CInstrumentation::CallActivityOnPause(
    /* [in] */ IActivity* activity)
{
    return Instrumentation::CallActivityOnPause(activity);
}

ECode CInstrumentation::CallActivityOnUserLeaving(
    /* [in] */ IActivity* activity)
{
    return Instrumentation::CallActivityOnUserLeaving(activity);
}

ECode CInstrumentation::StartAllocCounting()
{
    return Instrumentation::StartAllocCounting();
}

ECode CInstrumentation::StopAllocCounting()
{
    return Instrumentation::StopAllocCounting();
}

ECode CInstrumentation::GetAllocCounts(
    /* [out] */ IBundle** bundle)
{
    return Instrumentation::GetAllocCounts(bundle);
}

ECode CInstrumentation::GetBinderCounts(
    /* [out] */ IBundle** bundle)
{
    return Instrumentation::GetBinderCounts(bundle);
}

ECode CInstrumentation::ExecStartActivity(
    /* [in] */ IContext* who,
    /* [in] */ IBinder* contextThread,
    /* [in] */ IBinder* token,
    /* [in] */ IActivity* target,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IBundle* options,
    /* [out] */ IInstrumentationActivityResult** result)
{
    return Instrumentation::ExecStartActivity(who, contextThread, token, target, intent, requestCode, options, result);
}

ECode CInstrumentation::ExecStartActivities(
    /* [in] */ IContext* who,
    /* [in] */ IBinder* contextThread,
    /* [in] */ IBinder* token,
    /* [in] */ IActivity* target,
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options)
{
    return Instrumentation::ExecStartActivities(who, contextThread, token, target, intents, options);
}

ECode CInstrumentation::ExecStartActivitiesAsUser(
    /* [in] */ IContext* who,
    /* [in] */ IBinder* contextThread,
    /* [in] */ IBinder* token,
    /* [in] */ IActivity* target,
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options,
    /* [in] */ Int32 userId)
{
    return Instrumentation::ExecStartActivitiesAsUser(who, contextThread, token, target, intents, options, userId);
}

ECode CInstrumentation::ExecStartActivity(
    /* [in] */ IContext* who,
    /* [in] */ IBinder* contextThread,
    /* [in] */ IBinder* token,
    /* [in] */ IFragment* target,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IBundle* options,
    /* [out] */ IInstrumentationActivityResult** activityResult)
{
    return Instrumentation::ExecStartActivity(who, contextThread, token, target, intent, requestCode, options, activityResult);
}

ECode CInstrumentation::ExecStartActivity(
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
    return Instrumentation::ExecStartActivity(who, contextThread, token, target, intent, requestCode, options, user, activityResult);
}

ECode CInstrumentation::Init(
    /* [in] */ IActivityThread* thread,
    /* [in] */ IContext* instrContext,
    /* [in] */ IContext* appContext,
    /* [in] */ IComponentName* component,
    /* [in] */ IInstrumentationWatcher* watcher)
{
    return Instrumentation::Init(thread, instrContext, appContext, component, watcher);
}

} // namespace App
} // namespace Droid
} // namespace Elastos

