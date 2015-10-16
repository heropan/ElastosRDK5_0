
#include "elastos/droid/app/CApplicationThread.h"
#include "elastos/droid/os/Binder.h"
#include "elastos/droid/os/Process.h"
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/os/CServiceManager.h"
#include "elastos/droid/os/CDebug.h"
#include <elastos/utility/logging/Slogger.h>


using Elastos::Core::CString;
using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::ITimeZoneHelper;
using Elastos::Utility::CTimeZoneHelper;
using Elastos::Net::IInetAddressHelper;
using Elastos::Net::CInetAddressHelper;
using Elastos::Droid::Content::IIntentReceiver;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::IDebug;
using Elastos::Droid::Os::CDebug;

namespace Elastos {
namespace Droid {
namespace App {

const String CApplicationThread::HEAP_COLUMN("%13s %8s %8s %8s %8s %8s %8s");
const String CApplicationThread::ONE_COUNT_COLUMN("%21s %8d");
const String CApplicationThread::TWO_COUNT_COLUMNS("%21s %8d %21s %8d");
const String CApplicationThread::DB_INFO_FORMAT("  %8s %8s %14s %14s  %s");

void CApplicationThread::UpdatePendingConfiguration(
    /* [in] */ IConfiguration* config)
{
    AutoLock lock(mPackagesLock);

    Boolean isNewer;
    if (mAThread->mPendingConfiguration == NULL ||
        (mAThread->mPendingConfiguration->IsOtherSeqNewer(config, &isNewer), isNewer)) {
        mAThread->mPendingConfiguration = config;
    }
}

ECode CApplicationThread::SchedulePauseActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean finished,
    /* [in] */ Boolean userLeaving,
    /* [in] */ Int32 configChanges)
{
    return mAThread->QueueOrSendMessage(
        finished ? CActivityThread::H::PAUSE_ACTIVITY_FINISHING : CActivityThread::H::PAUSE_ACTIVITY,
        token, (userLeaving ? 1 : 0), configChanges);
}

ECode CApplicationThread::ScheduleStopActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean showWindow,
    /* [in] */ Int32 configChanges)
{
    return mAThread->QueueOrSendMessage(
        showWindow ? CActivityThread::H::STOP_ACTIVITY_SHOW : CActivityThread::H::STOP_ACTIVITY_HIDE,
        token, 0, configChanges);
}

ECode CApplicationThread::ScheduleWindowVisibility(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean showWindow)
{
    return mAThread->QueueOrSendMessage(
        showWindow ? CActivityThread::H::SHOW_WINDOW : CActivityThread::H::HIDE_WINDOW, token);
}

ECode CApplicationThread::ScheduleSleeping(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean sleeping)
{
    return mAThread->QueueOrSendMessage(CActivityThread::H::SLEEPING,
        token, sleeping ? 1 : 0);
}

ECode CApplicationThread::ScheduleResumeActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean isForward)
{
    return mAThread->QueueOrSendMessage(CActivityThread::H::RESUME_ACTIVITY,
        token, isForward ? 1 : 0);
}

ECode CApplicationThread::ScheduleSendResult(
    /* [in] */ IBinder* token,
    /* [in] */ IObjectContainer* results)
{
    AutoPtr<CActivityThread::ResultData> res = new CActivityThread::ResultData();
    res->mToken = token;
    AutoPtr<IObjectEnumerator> enumerator;
    results->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> obj;
        enumerator->Current((IInterface**)&obj);
        res->mResults.PushBack(IResultInfo::Probe(obj));
    }

    return mAThread->QueueOrSendMessage(CActivityThread::H::SEND_RESULT, res);
}

ECode CApplicationThread::ScheduleLaunchActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 ident,
    /* [in] */ IActivityInfo* info,
    /* [in] */ IConfiguration* curConfig,
    /* [in] */ ICompatibilityInfo* compatInfo,
    /* [in] */ IBundle* state,
    /* [in] */ IObjectContainer* pendingResults,
    /* [in] */ IObjectContainer* pendingNewIntents,
    /* [in] */ Boolean notResumed,
    /* [in] */ Boolean isForward,
    /* [in] */ const String& profileName,
    /* [in] */ IParcelFileDescriptor* profileFd,
    /* [in] */ Boolean autoStopProfiler)
{
    AutoPtr<CActivityThread::ActivityClientRecord> r = new CActivityThread::ActivityClientRecord();
    r->mToken = token;
    r->mIdent = ident;
    r->mIntent = intent;
    r->mActivityInfo = info;
    r->mCompatInfo = compatInfo;
    r->mState = (CBundle*)state;

    if (pendingResults != NULL) {
        Int32 count;
        pendingResults->GetObjectCount(&count);
        if (count > 0) {
            r->mPendingResults = new List< AutoPtr<IResultInfo> >();
            AutoPtr<IObjectEnumerator> enumerator;
            pendingResults->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
            Boolean hasNext;
            while(enumerator->MoveNext(&hasNext), hasNext) {
                AutoPtr<IInterface> obj;
                enumerator->Current((IInterface**)&obj);
                r->mPendingResults->PushBack(IResultInfo::Probe(obj));
            }
        }
    }

    if (pendingNewIntents != NULL) {
        Int32 count;
        pendingNewIntents->GetObjectCount(&count);
        if (count > 0) {
            r->mPendingIntents = new List< AutoPtr<IIntent> >();
            AutoPtr<IObjectEnumerator> enumerator;
            pendingNewIntents->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
            Boolean hasNext;
            while(enumerator->MoveNext(&hasNext), hasNext) {
                AutoPtr<IIntent> obj;
                enumerator->Current((IInterface**)&obj);
                r->mPendingIntents->PushBack(obj);
            }
        }
    }

    r->mStartsNotResumed = notResumed;
    r->mIsForward = isForward;

    r->mProfileFile = profileName;
    r->mProfileFd = profileFd;
    r->mAutoStopProfiler = autoStopProfiler;

    UpdatePendingConfiguration(curConfig);

    return mAThread->QueueOrSendMessage(CActivityThread::H::LAUNCH_ACTIVITY, r);
}

ECode CApplicationThread::ScheduleRelaunchActivity(
    /* [in] */ IBinder* token,
    /* [in] */ ArrayOf<IResultInfo*>* pendingResults,
    /* [in] */ ArrayOf<IIntent*>* pendingNewIntents,
    /* [in] */ Int32 configChanges,
    /* [in] */ Boolean notResumed,
    /* [in] */ IConfiguration* config)
{
    return mAThread->RequestRelaunchActivity(token, pendingResults, pendingNewIntents,
            configChanges, notResumed, config, TRUE);
}

ECode CApplicationThread::ScheduleNewIntent(
    /* [in] */ IObjectContainer* intents,
    /* [in] */ IBinder *token)
{
    AutoPtr<CActivityThread::NewIntentData> data = new CActivityThread::NewIntentData();
    AutoPtr<IObjectEnumerator> enumerator;
    intents->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> obj;
        enumerator->Current((IInterface**)&obj);
        AutoPtr<IIntent> intent = IIntent::Probe(obj);
        data->mIntents.PushBack(intent);
    }
    data->mToken = token;

    return mAThread->QueueOrSendMessage(CActivityThread::H::NEW_INTENT, data);
}

ECode CApplicationThread::ScheduleDestroyActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean finishing,
    /* [in] */ Int32 configChanges)
{
    return mAThread->QueueOrSendMessage(CActivityThread::H::DESTROY_ACTIVITY,
        token, finishing ? 1 : 0, configChanges);
}

ECode CApplicationThread::ScheduleReceiver(
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* info,
    /* [in] */ ICompatibilityInfo* compatInfo,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean sync,
    /* [in] */ Int32 sendingUser)
{
    AutoPtr<CActivityThread::ReceiverData> r = new CActivityThread::ReceiverData(intent, resultCode, data, extras,
        sync, FALSE, IBinder::Probe((IApplicationThread*)mAThread->mAppThread.Get()), sendingUser);
    r->mInfo = info;
    r->mCompatInfo = (CCompatibilityInfo*)compatInfo;

    return mAThread->QueueOrSendMessage(CActivityThread::H::RECEIVER, r);
}

ECode CApplicationThread::ScheduleCreateBackupAgent(
    /* [in] */ IApplicationInfo* app,
    /* [in] */ ICompatibilityInfo* compatInfo,
    /* [in] */ Int32 backupMode)
{
    AutoPtr<CActivityThread::CreateBackupAgentData> d = new CActivityThread::CreateBackupAgentData();
    d->mAppInfo = app;
    d->mCompatInfo = (CCompatibilityInfo*)compatInfo;
    d->mBackupMode = backupMode;

    return mAThread->QueueOrSendMessage(CActivityThread::H::CREATE_BACKUP_AGENT, d);
}

ECode CApplicationThread::ScheduleDestroyBackupAgent(
    /* [in] */ IApplicationInfo* app,
    /* [in] */ ICompatibilityInfo* compatInfo)
{
    AutoPtr<CActivityThread::CreateBackupAgentData> d = new CActivityThread::CreateBackupAgentData();
    d->mAppInfo = app;
    d->mCompatInfo = (CCompatibilityInfo*)compatInfo;

    return mAThread->QueueOrSendMessage(CActivityThread::H::DESTROY_BACKUP_AGENT, d);
}

ECode CApplicationThread::ScheduleCreateService(
    /* [in] */ IBinder* token,
    /* [in] */ IServiceInfo* info,
    /* [in] */ ICompatibilityInfo* compatInfo)
{
    AutoPtr<CActivityThread::CreateServiceData> s = new CActivityThread::CreateServiceData();
    s->mToken = token;
    s->mInfo = info;
    s->mCompatInfo = compatInfo;

    return mAThread->QueueOrSendMessage(CActivityThread::H::CREATE_SERVICE, s);
}

ECode CApplicationThread::ScheduleBindService(
    /* [in] */ IBinder* token,
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean rebind)
{
    AutoPtr<CActivityThread::BindServiceData> s = new CActivityThread::BindServiceData();
    s->mToken = token;
    s->mIntent = intent;
    s->mRebind = rebind;

    if (CActivityThread::DEBUG_SERVICE) {
        Slogger::V(CActivityThread::TAG, "scheduleBindService token=%p intent=%p uid=%d pid=%d"
            , token, intent, Binder::GetCallingUid(), Binder::GetCallingPid());
    }

    return mAThread->QueueOrSendMessage(CActivityThread::H::BIND_SERVICE, s);
}

ECode CApplicationThread::ScheduleUnbindService(
    /* [in] */ IBinder* token,
    /* [in] */ IIntent* intent)
{
    AutoPtr<CActivityThread::BindServiceData> s = new CActivityThread::BindServiceData();
    s->mToken = token;
    s->mIntent = intent;

    return mAThread->QueueOrSendMessage(CActivityThread::H::UNBIND_SERVICE, s);
}

ECode CApplicationThread::ScheduleServiceArgs(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean taskRemoved,
    /* [in] */ Int32 startId,
    /* [in] */ Int32 flags,
    /* [in] */ IIntent* args)
{
    AutoPtr<CActivityThread::ServiceArgsData> s = new CActivityThread::ServiceArgsData();
    s->mToken = token;
    s->mTaskRemoved = taskRemoved;
    s->mStartId = startId;
    s->mFlags = flags;
    s->mArgs = args;

    return mAThread->QueueOrSendMessage(CActivityThread::H::SERVICE_ARGS, s);
}

ECode CApplicationThread::ScheduleStopService(
    /* [in] */ IBinder* token)
{
    return mAThread->QueueOrSendMessage(CActivityThread::H::STOP_SERVICE, token);
}

ECode CApplicationThread::BindApplication(
    /* [in] */ const String& processName,
    /* [in] */ IApplicationInfo* appInfo,
    /* [in] */ IObjectContainer* providers,
    /* [in] */ IComponentName* instrumentationName,
    /* [in] */ const String& profileFile,
    /* [in] */ IParcelFileDescriptor* profileFd,
    /* [in] */ Boolean autoStopProfiler,
    /* [in] */ IBundle* instrumentationArgs,
    /* [in] */ IInstrumentationWatcher* instrumentationWatcher,
    /* [in] */ Int32 debugMode,
    /* [in] */ Boolean enableOpenGlTrace,
    /* [in] */ Boolean isRestrictedBackupMode,
    /* [in] */ Boolean persistent,
    /* [in] */ IConfiguration* config,
    /* [in] */ ICompatibilityInfo* compatInfo,
    /* [in] */ IObjectStringMap* services,
    /* [in] */ IBundle* coreSettings)
{
    if (services != NULL) {
        // Setup the service cache in the ServiceManager
        AutoPtr<CServiceManager> serviceManager;
        CServiceManager::AcquireSingletonByFriend((CServiceManager**)&serviceManager);
        serviceManager->InitServiceCache(services);
    }
    SetCoreSettings(coreSettings);

    AutoPtr<CActivityThread::AppBindData> data = new CActivityThread::AppBindData();
    data->mProcessName = processName;
    data->mAppInfo = appInfo;
    data->mProviders = providers;
    data->mInstrumentationName = instrumentationName;
    data->mInstrumentationArgs = instrumentationArgs;
    data->mInstrumentationWatcher = instrumentationWatcher;
    data->mDebugMode = debugMode;
    data->mEnableOpenGlTrace = enableOpenGlTrace;
    data->mRestrictedBackupMode = isRestrictedBackupMode;
    data->mPersistent = persistent;
    data->mConfig = config;
    data->mCompatInfo = compatInfo;
    data->mInitProfileFile = profileFile;
    data->mInitProfileFd = profileFd;
    data->mInitAutoStopProfiler = FALSE;

    return mAThread->QueueOrSendMessage(CActivityThread::H::BIND_APPLICATION, data);
}

ECode CApplicationThread::ScheduleExit()
{
    return mAThread->QueueOrSendMessage(CActivityThread::H::EXIT_APPLICATION,
        NULL);
}

ECode CApplicationThread::ScheduleSuicide()
{
    return mAThread->QueueOrSendMessage(CActivityThread::H::SUICIDE,
        NULL);
}

ECode CApplicationThread::RequestThumbnail(
    /* [in] */ IBinder* token)
{
    return mAThread->QueueOrSendMessage(CActivityThread::H::REQUEST_THUMBNAIL, token);
}

ECode CApplicationThread::ScheduleConfigurationChanged(
    /* [in] */ IConfiguration* config)
{
    UpdatePendingConfiguration(config);

    return mAThread->QueueOrSendMessage(CActivityThread::H::CONFIGURATION_CHANGED, config);
}

ECode CApplicationThread::UpdateTimeZone()
{
    AutoPtr<ITimeZoneHelper> helper;
    CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&helper);
    return helper->SetDefault(NULL);
}

ECode CApplicationThread::ClearDnsCache()
{
    // a non-standard API to get this to libcore
    AutoPtr<IInetAddressHelper> helper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&helper);
    return helper->ClearDnsCache();
}

ECode CApplicationThread::SetHttpProxy(
    /* [in] */ const String& host,
    /* [in] */ const String& port,
    /* [in] */ const String& exclList)
{
    assert(0 && "TODO");
//     Proxy.setHttpProxySystemProperty(host, port, exclList);
    return E_NOT_IMPLEMENTED;
}

ECode CApplicationThread::ProcessInBackground()
{
    mAThread->mH->RemoveMessages(CActivityThread::H::GC_WHEN_IDLE);
    AutoPtr<IMessage> msg;
    mAThread->mH->ObtainMessage(CActivityThread::H::GC_WHEN_IDLE, (IMessage**)&msg);
    Boolean bval;
    return mAThread->mH->SendMessage(msg, &bval);
}

ECode CApplicationThread::DumpService(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IBinder* servicetoken,
    /* [in] */ ArrayOf<String>* args)
{
    AutoPtr<CActivityThread::DumpComponentInfo> data = new CActivityThread::DumpComponentInfo();
//     try {
//         data.fd = ParcelFileDescriptor.dup(fd);
    data->mToken = servicetoken;
    data->mArgs->Copy(args);

    return mAThread->QueueOrSendMessage(CActivityThread::H::DUMP_SERVICE, data);
//     } catch (IOException e) {
//         Slog.w(TAG, "dumpService failed", e);
//     }
}

ECode CApplicationThread::ScheduleRegisteredReceiver(
    /* [in] */ IIntentReceiver* receiver,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& dataStr,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky,
    /* [in] */ Int32 sendingUser)
{
    return receiver->PerformReceive(intent, resultCode, dataStr,
            extras, ordered, sticky, sendingUser);
}

ECode CApplicationThread::ScheduleLowMemory()
{
    return mAThread->QueueOrSendMessage(CActivityThread::H::LOW_MEMORY, NULL);
}

ECode CApplicationThread::ScheduleActivityConfigurationChanged(
    /* [in] */ IBinder* token)
{
    return mAThread->QueueOrSendMessage(CActivityThread::H::ACTIVITY_CONFIGURATION_CHANGED, token);
}

ECode CApplicationThread::ProfilerControl(
    /* [in] */ Boolean start,
    /* [in] */ const String& path,
    /* [in] */ IParcelFileDescriptor* fd,
    /* [in] */ Int32 profileType)
{
    AutoPtr<CActivityThread::ProfilerControlData> pcd = new CActivityThread::ProfilerControlData();
    pcd->mPath = path;
    pcd->mFd = fd;

    return mAThread->QueueOrSendMessage(CActivityThread::H::PROFILER_CONTROL,
        pcd, start ? 1 : 0, profileType);
}

ECode CApplicationThread::DumpHeap(
    /* [in] */ Boolean managed,
    /* [in] */ const String& path,
    /* [in] */ IParcelFileDescriptor* fd)
{
    AutoPtr<CActivityThread::DumpHeapData> dhd = new CActivityThread::DumpHeapData();
    dhd->mPath = path;
    dhd->mFd = fd;

    return mAThread->QueueOrSendMessage(CActivityThread::H::DUMP_HEAP, dhd, managed ? 1 : 0);
}

ECode CApplicationThread::SetSchedulingGroup(
    /* [in] */ Int32 group)
{
    // Note: do this immediately, since going into the foreground
    // should happen regardless of what pending work we have to do
    // and the activity manager will wait for us to report back that
    // we are done before sending us to the background.
//     try {
    Process::SetProcessGroup(Process::MyPid(), group);
//     } catch (Exception e) {
//         Slog.w(TAG, "Failed setting process group to " + group, e);
//     }
    return NOERROR;
}

ECode CApplicationThread::GetMemoryInfo(
    /* [in] */ IDebugMemoryInfo* outInfo)
{
    AutoPtr<IDebug> dbg;
    CDebug::AcquireSingleton((IDebug**)&dbg);
    return dbg->GetMemoryInfo(outInfo);
}

ECode CApplicationThread::DispatchPackageBroadcast(
    /* [in] */ Int32 cmd,
    /* [in] */ ArrayOf<String>* packages)
{
    AutoPtr<IObjectContainer> container;
    if (packages != NULL) {
        CObjectContainer::New((IObjectContainer**)&container);
        for (Int32 i = 0; i < packages->GetLength(); ++i) {
            AutoPtr<ICharSequence> seq;
            CString::New((*packages)[i], (ICharSequence**)&seq);
            container->Add(seq);
        }
    }

    return mAThread->QueueOrSendMessage(CActivityThread::H::DISPATCH_PACKAGE_BROADCAST,
        container, cmd);
}

ECode CApplicationThread::ScheduleCrash(
    /* [in] */ const String& msg)
{
    AutoPtr<ICharSequence> seq;
    CString::New(msg, (ICharSequence**)&seq);

    return mAThread->QueueOrSendMessage(CActivityThread::H::SCHEDULE_CRASH, seq);
}

ECode CApplicationThread::DumpActivity(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IBinder* activitytoken,
    /* [in] */ const String& prefix,
    /* [in] */ ArrayOf<String>* args)
{
    AutoPtr<CActivityThread::DumpComponentInfo> data = new CActivityThread::DumpComponentInfo();
//     try {
//         data.fd = ParcelFileDescriptor.dup(fd);
    data->mToken = activitytoken;
    data->mPrefix = prefix;
    data->mArgs->Copy(args);

    return mAThread->QueueOrSendMessage(CActivityThread::H::DUMP_ACTIVITY, data);
//     } catch (IOException e) {
//         Slog.w(TAG, "dumpActivity failed", e);
//     }
}

ECode CApplicationThread::DumpProvider(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IBinder* providertoken,
    /* [in] */ ArrayOf<String>* args)
{
    AutoPtr<CActivityThread::DumpComponentInfo> data = new CActivityThread::DumpComponentInfo();
//     try {
//         data.fd = ParcelFileDescriptor.dup(fd);
    data->mToken = providertoken;
    data->mArgs->Copy(args);

    return mAThread->QueueOrSendMessage(CActivityThread::H::DUMP_PROVIDER, data);
//     } catch (IOException e) {
//         Slog.w(TAG, "dumpProvider failed", e);
//     }
}

// @Override
ECode CApplicationThread::DumpMemInfo(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Boolean checkin,
    /* [in] */ Boolean all,
    /* [in] */ ArrayOf<String>* args,
    /* [out] */ IDebugMemoryInfo** mInfo)
{
//     FileOutputStream fout = new FileOutputStream(fd);
//     PrintWriter pw = new PrintWriter(fout);
//     try {
//         return dumpMemInfo(pw, checkin, all);
//     } finally {
//         pw.flush();
//     }
    return E_NOT_IMPLEMENTED;
}

AutoPtr<IDebugMemoryInfo> CApplicationThread::DumpMemInfo(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ Boolean checkin,
    /* [in] */ Boolean all)
{
//     long nativeMax = Debug.getNativeHeapSize() / 1024;
//     long nativeAllocated = Debug.getNativeHeapAllocatedSize() / 1024;
//     long nativeFree = Debug.getNativeHeapFreeSize() / 1024;

//     Debug.MemoryInfo memInfo = new Debug.MemoryInfo();
//     Debug.getMemoryInfo(memInfo);

//     if (!all) {
//         return memInfo;
//     }

//     Runtime runtime = Runtime.getRuntime();

//     long dalvikMax = runtime.totalMemory() / 1024;
//     long dalvikFree = runtime.freeMemory() / 1024;
//     long dalvikAllocated = dalvikMax - dalvikFree;
//     long viewInstanceCount = ViewDebug.getViewInstanceCount();
//     long viewRootInstanceCount = ViewDebug.getViewRootImplCount();
//     long appContextInstanceCount = Debug.countInstancesOfClass(ContextImpl.class);
//     long activityInstanceCount = Debug.countInstancesOfClass(Activity.class);
//     int globalAssetCount = AssetManager.getGlobalAssetCount();
//     int globalAssetManagerCount = AssetManager.getGlobalAssetManagerCount();
//     int binderLocalObjectCount = Debug.getBinderLocalObjectCount();
//     int binderProxyObjectCount = Debug.getBinderProxyObjectCount();
//     int binderDeathObjectCount = Debug.getBinderDeathObjectCount();
//     long openSslSocketCount = Debug.countInstancesOfClass(OpenSSLSocketImpl.class);
//     SQLiteDebug.PagerStats stats = SQLiteDebug.getDatabaseInfo();

//     // For checkin, we print one long comma-separated list of values
//     if (checkin) {
//         // NOTE: if you change anything significant below, also consider changing
//         // ACTIVITY_THREAD_CHECKIN_VERSION.
//         String processName = (mBoundApplication != null)
//                 ? mBoundApplication.processName : "unknown";

//         // Header
//         pw.print(ACTIVITY_THREAD_CHECKIN_VERSION); pw.print(',');
//         pw.print(Process.myPid()); pw.print(',');
//         pw.print(processName); pw.print(',');

//         // Heap info - max
//         pw.print(nativeMax); pw.print(',');
//         pw.print(dalvikMax); pw.print(',');
//         pw.print("N/A,");
//         pw.print(nativeMax + dalvikMax); pw.print(',');

//         // Heap info - allocated
//         pw.print(nativeAllocated); pw.print(',');
//         pw.print(dalvikAllocated); pw.print(',');
//         pw.print("N/A,");
//         pw.print(nativeAllocated + dalvikAllocated); pw.print(',');

//         // Heap info - free
//         pw.print(nativeFree); pw.print(',');
//         pw.print(dalvikFree); pw.print(',');
//         pw.print("N/A,");
//         pw.print(nativeFree + dalvikFree); pw.print(',');

//         // Heap info - proportional set size
//         pw.print(memInfo.nativePss); pw.print(',');
//         pw.print(memInfo.dalvikPss); pw.print(',');
//         pw.print(memInfo.otherPss); pw.print(',');
//         pw.print(memInfo.nativePss + memInfo.dalvikPss + memInfo.otherPss); pw.print(',');

//         // Heap info - shared
//         pw.print(memInfo.nativeSharedDirty); pw.print(',');
//         pw.print(memInfo.dalvikSharedDirty); pw.print(',');
//         pw.print(memInfo.otherSharedDirty); pw.print(',');
//         pw.print(memInfo.nativeSharedDirty + memInfo.dalvikSharedDirty
//                 + memInfo.otherSharedDirty); pw.print(',');

//         // Heap info - private
//         pw.print(memInfo.nativePrivateDirty); pw.print(',');
//         pw.print(memInfo.dalvikPrivateDirty); pw.print(',');
//         pw.print(memInfo.otherPrivateDirty); pw.print(',');
//         pw.print(memInfo.nativePrivateDirty + memInfo.dalvikPrivateDirty
//                 + memInfo.otherPrivateDirty); pw.print(',');

//         // Object counts
//         pw.print(viewInstanceCount); pw.print(',');
//         pw.print(viewRootInstanceCount); pw.print(',');
//         pw.print(appContextInstanceCount); pw.print(',');
//         pw.print(activityInstanceCount); pw.print(',');

//         pw.print(globalAssetCount); pw.print(',');
//         pw.print(globalAssetManagerCount); pw.print(',');
//         pw.print(binderLocalObjectCount); pw.print(',');
//         pw.print(binderProxyObjectCount); pw.print(',');

//         pw.print(binderDeathObjectCount); pw.print(',');
//         pw.print(openSslSocketCount); pw.print(',');

//         // SQL
//         pw.print(stats.memoryUsed / 1024); pw.print(',');
//         pw.print(stats.memoryUsed / 1024); pw.print(',');
//         pw.print(stats.pageCacheOverflow / 1024); pw.print(',');
//         pw.print(stats.largestMemAlloc / 1024);
//         for (int i = 0; i < stats.dbStats.size(); i++) {
//             DbStats dbStats = stats.dbStats.get(i);
//             pw.print(','); pw.print(dbStats.dbName);
//             pw.print(','); pw.print(dbStats.pageSize);
//             pw.print(','); pw.print(dbStats.dbSize);
//             pw.print(','); pw.print(dbStats.lookaside);
//             pw.print(','); pw.print(dbStats.cache);
//             pw.print(','); pw.print(dbStats.cache);
//         }
//         pw.println();

//         return memInfo;
//     }

//     // otherwise, show human-readable format
//     printRow(pw, HEAP_COLUMN, "", "", "Shared", "Private", "Heap", "Heap", "Heap");
//     printRow(pw, HEAP_COLUMN, "", "Pss", "Dirty", "Dirty", "Size", "Alloc", "Free");
//     printRow(pw, HEAP_COLUMN, "", "------", "------", "------", "------", "------",
//             "------");
//     printRow(pw, HEAP_COLUMN, "Native", memInfo.nativePss, memInfo.nativeSharedDirty,
//             memInfo.nativePrivateDirty, nativeMax, nativeAllocated, nativeFree);
//     printRow(pw, HEAP_COLUMN, "Dalvik", memInfo.dalvikPss, memInfo.dalvikSharedDirty,
//             memInfo.dalvikPrivateDirty, dalvikMax, dalvikAllocated, dalvikFree);

//     int otherPss = memInfo.otherPss;
//     int otherSharedDirty = memInfo.otherSharedDirty;
//     int otherPrivateDirty = memInfo.otherPrivateDirty;

//     for (int i=0; i<Debug.MemoryInfo.NUM_OTHER_STATS; i++) {
//         printRow(pw, HEAP_COLUMN, Debug.MemoryInfo.getOtherLabel(i),
//                 memInfo.getOtherPss(i), memInfo.getOtherSharedDirty(i),
//                 memInfo.getOtherPrivateDirty(i), "", "", "");
//         otherPss -= memInfo.getOtherPss(i);
//         otherSharedDirty -= memInfo.getOtherSharedDirty(i);
//         otherPrivateDirty -= memInfo.getOtherPrivateDirty(i);
//     }

//     printRow(pw, HEAP_COLUMN, "Unknown", otherPss, otherSharedDirty,
//             otherPrivateDirty, "", "", "");
//     printRow(pw, HEAP_COLUMN, "TOTAL", memInfo.getTotalPss(),
//             memInfo.getTotalSharedDirty(), memInfo.getTotalPrivateDirty(),
//             nativeMax+dalvikMax, nativeAllocated+dalvikAllocated,
//             nativeFree+dalvikFree);

//     pw.println(" ");
//     pw.println(" Objects");
//     printRow(pw, TWO_COUNT_COLUMNS, "Views:", viewInstanceCount, "ViewRootImpl:",
//             viewRootInstanceCount);

//     printRow(pw, TWO_COUNT_COLUMNS, "AppContexts:", appContextInstanceCount,
//             "Activities:", activityInstanceCount);

//     printRow(pw, TWO_COUNT_COLUMNS, "Assets:", globalAssetCount,
//             "AssetManagers:", globalAssetManagerCount);

//     printRow(pw, TWO_COUNT_COLUMNS, "Local Binders:", binderLocalObjectCount,
//             "Proxy Binders:", binderProxyObjectCount);
//     printRow(pw, ONE_COUNT_COLUMN, "Death Recipients:", binderDeathObjectCount);

//     printRow(pw, ONE_COUNT_COLUMN, "OpenSSL Sockets:", openSslSocketCount);

//     // SQLite mem info
//     pw.println(" ");
//     pw.println(" SQL");
//     printRow(pw, ONE_COUNT_COLUMN, "MEMORY_USED:", stats.memoryUsed / 1024);
//     printRow(pw, TWO_COUNT_COLUMNS, "PAGECACHE_OVERFLOW:",
//             stats.pageCacheOverflow / 1024, "MALLOC_SIZE:", stats.largestMemAlloc / 1024);
//     pw.println(" ");
//     int N = stats.dbStats.size();
//     if (N > 0) {
//         pw.println(" DATABASES");
//         printRow(pw, "  %8s %8s %14s %14s  %s", "pgsz", "dbsz", "Lookaside(b)", "cache",
//                 "Dbname");
//         for (int i = 0; i < N; i++) {
//             DbStats dbStats = stats.dbStats.get(i);
//             printRow(pw, DB_INFO_FORMAT,
//                     (dbStats.pageSize > 0) ? String.valueOf(dbStats.pageSize) : " ",
//                     (dbStats.dbSize > 0) ? String.valueOf(dbStats.dbSize) : " ",
//                     (dbStats.lookaside > 0) ? String.valueOf(dbStats.lookaside) : " ",
//                     dbStats.cache, dbStats.dbName);
//         }
//     }

//     // Asset details.
//     String assetAlloc = AssetManager.getAssetAllocations();
//     if (assetAlloc != null) {
//         pw.println(" ");
//         pw.println(" Asset Allocations");
//         pw.print(assetAlloc);
//     }

//     return memInfo;
    return NULL;
}

// @Override
ECode CApplicationThread::DumpGfxInfo(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<String>* args)
{
//     dumpGraphicsInfo(fd);
//     WindowManagerGlobal.getInstance().dumpGfxInfo(fd);
    return E_NOT_IMPLEMENTED;
}

// @Override
ECode CApplicationThread::DumpDbInfo(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<String>* args)
{
//     PrintWriter pw = new PrintWriter(new FileOutputStream(fd));
//     PrintWriterPrinter printer = new PrintWriterPrinter(pw);
//     SQLiteDebug.dump(printer, args);
//     pw.flush();
    return E_NOT_IMPLEMENTED;
}

// @Override
ECode CApplicationThread::UnstableProviderDied(
    /* [in] */ IBinder* provider)
{
    return mAThread->QueueOrSendMessage(CActivityThread::H::UNSTABLE_PROVIDER_DIED, provider);
}

void CApplicationThread::PrintRow(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface>* objs)
{
//     pw.println(String.format(format, objs));
}

ECode CApplicationThread::SetCoreSettings(
    /* [in] */ IBundle* coreSettings)
{
    return mAThread->QueueOrSendMessage(CActivityThread::H::SET_CORE_SETTINGS, coreSettings);
}

ECode CApplicationThread::UpdatePackageCompatibilityInfo(
    /* [in] */ const String& pkg,
    /* [in] */ ICompatibilityInfo* info)
{
    AutoPtr<CActivityThread::UpdateCompatibilityData> ucd = new CActivityThread::UpdateCompatibilityData();
    ucd->mPkg = pkg;
    ucd->mInfo = info;

    return mAThread->QueueOrSendMessage(CActivityThread::H::UPDATE_PACKAGE_COMPATIBILITY_INFO, ucd);
}

ECode CApplicationThread::ScheduleTrimMemory(
    /* [in] */ Int32 level)
{
    return mAThread->QueueOrSendMessage(CActivityThread::H::TRIM_MEMORY,
        NULL, level);
}

ECode CApplicationThread::ToString(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string);
    String info("CApplicationThread:");
    info.AppendFormat(" %p, activity thread: %p", this, mAThread);
    *string = info;
    return NOERROR;
}


} // namespace App
} // namespace Droid
} // namespace Elastos

