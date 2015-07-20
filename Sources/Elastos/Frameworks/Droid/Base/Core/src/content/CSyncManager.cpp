
#include "content/CIntent.h"
#include "content/CIntentFilter.h"
#include "content/CSyncManager.h"
#include "content/CSyncOperation.h"
#include "content/CSyncResult.h"
#include "content/CSyncStorageEngine.h"
#include "content/CSyncStorageEngineHelper.h"
#include "content/CSyncStorageEngineDayStats.h"
#include "content/pm/RegisteredServicesCache.h"
//***#include "os/CBundle.h"
//***#include "os/CApartment.h"
#include "util/CPair.h"
#include <elastos/core/StringUtils.h>
#include "app/CActivityManager.h"

using Elastos::Droid::App::CActivityManager;
using Elastos::Droid::Content::Pm::RegisteredServicesCache;
using Elastos::Droid::Utility::CPair;
using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Content {

Boolean CSyncManager::sIsLargeRAM = FALSE;
Boolean CSyncManager::sIsStaticMemberInitialized = InitStaticMembers();
const Int64 CSyncManager::LOCAL_SYNC_DELAY = 0;
//***const Int64 CSyncManager::LOCAL_SYNC_DELAY = SystemProperties::GetInt64("sync.local_sync_delay", 30 * 1000 /* 30 seconds */);
const Int64 CSyncManager::MAX_TIME_PER_SYNC = 0;
//***const Int64 CSyncManager::MAX_TIME_PER_SYNC = SystemProperties::GetInt64("sync.max_time_per_sync", 5 * 60 * 1000 /* 5 minutes */);
const Int64 CSyncManager::SYNC_NOTIFICATION_DELAY = 0;
//***const Int64 CSyncManager::SYNC_NOTIFICATION_DELAY = SystemProperties::GetInt64("sync.notification_delay", 30 * 1000 /* 30 seconds */);
const Int32 CSyncManager::MAX_SIMULTANEOUS_REGULAR_SYNCS = 0;
//***const Int32 CSyncManager::MAX_SIMULTANEOUS_REGULAR_SYNCS = SystemProperties::GetInt32("sync.max_regular_syncs", sIsLargeRAM ? 2 : 1);
const Int32 CSyncManager::MAX_SIMULTANEOUS_INITIALIZATION_SYNCS = 0;
//***const Int32 CSyncManager::MAX_SIMULTANEOUS_INITIALIZATION_SYNCS = SystemProperties::GetInt32("sync.max_init_syncs", sIsLargeRAM ? 5 : 2);
const String CSyncManager::TAG("SyncManager");
const String CSyncManager::SYNC_WAKE_LOCK_PREFIX = String("*sync*");
const String CSyncManager::HANDLE_SYNC_ALARM_WAKE_LOCK = String("SyncManagerHandleSyncAlarm");
const String CSyncManager::SYNC_LOOP_WAKE_LOCK = String("SyncLoopWakeLock");
const String CSyncManager::ACTION_SYNC_ALARM = String("android.content.syncmanager.SYNC_ALARM");
/* const */ AutoPtr<ArrayOf<IAccountAndUser *> > CSyncManager::INITIAL_ACCOUNTS_ARRAY = ArrayOf<IAccountAndUser *>::Alloc(0);
const Int64 CSyncManager::INITIAL_SYNC_RETRY_TIME_IN_MS;
const Int64 CSyncManager::DEFAULT_MAX_SYNC_RETRY_TIME_IN_SECONDS;
const Int32 CSyncManager::DELAY_RETRY_SYNC_IN_PROGRESS_IN_SECONDS;
const Int32 CSyncManager::INITIALIZATION_UNBIND_DELAY_MS;
const Int64 CSyncManager::SYNC_ALARM_TIMEOUT_MIN;
const Int64 CSyncManager::SYNC_ALARM_TIMEOUT_MAX;
const Int32 CSyncManager::SyncHandler::MESSAGE_SYNC_FINISHED;
const Int32 CSyncManager::SyncHandler::MESSAGE_SYNC_ALARM;
const Int32 CSyncManager::SyncHandler::MESSAGE_CHECK_ALARMS;
const Int32 CSyncManager::SyncHandler::MESSAGE_SERVICE_CONNECTED;
const Int32 CSyncManager::SyncHandler::MESSAGE_SERVICE_DISCONNECTED;
const Int32 CSyncManager::SyncHandler::MESSAGE_CANCEL;

CSyncManager::SyncHandlerMessagePayload::SyncHandlerMessagePayload(
    /* [in] */ ActiveSyncContext* syncContext,
    /* [in] */ ISyncResult* syncRet)
    : activeSyncContext(syncContext)
    , syncResult(syncRet)
{}

CSyncManager::SyncHandlerMessagePayload::~SyncHandlerMessagePayload()
{}

CAR_INTERFACE_IMPL(CSyncManager::SyncHandlerMessagePayload, IInterface)

CSyncManager::SyncAlarmIntentReceiver::SyncAlarmIntentReceiver(
    /* [in] */ CSyncManager* syncMgr)
    : mSyncMgr(syncMgr)
{}

CSyncManager::SyncAlarmIntentReceiver::~SyncAlarmIntentReceiver()
{}

ECode CSyncManager::SyncAlarmIntentReceiver::Initialize()
{
    return NOERROR;
}

ECode CSyncManager::SyncAlarmIntentReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    FAIL_RETURN(mSyncMgr->mHandleAlarmWakeLock->AcquireLock());
    FAIL_RETURN(mSyncMgr->SendSyncAlarmMessage());
    return NOERROR;
}

CSyncManager::ActiveSyncContext::ActiveSyncContext(
    /* [in] */ ISyncOperation* syncOperation,
    /* [in] */ Int64 historyRowId,
    /* [in] */ Int32 syncAdapterUid,
    /* [in] */ CSyncManager* syncMgr)
    : mSyncOperation(syncOperation)
    , mHistoryRowId(historyRowId)
    , mSyncAdapter(NULL)
    , mStartTime(0)
    , mTimeoutStartTime(0)
    , mBound(FALSE)
    , mSyncWakeLock(NULL)
    , mSyncAdapterUid(syncAdapterUid)
    , mSyncInfo(NULL)
    , mIsLinkedToDeath(FALSE)
    , mSyncMgr(syncMgr)
{
//    super();
//    mStartTime = SystemClock.elapsedRealtime();
    mTimeoutStartTime = mStartTime;
    AutoPtr<IAccount> account;
    ASSERT_SUCCEEDED(mSyncOperation->GetAccount((IAccount**)&account))
    String authority;
    ASSERT_SUCCEEDED(mSyncOperation->GetAuthority(&authority))
    ASSERT_SUCCEEDED(mSyncMgr->mSyncHandler->GetSyncWakeLock(account, authority, (IPowerManagerWakeLock**)&mSyncWakeLock))
    AutoPtr<IWorkSource> workSource;
//***    ASSERT_SUCCEEDED(CWorkSource::New(syncAdapterUid, (IWorkSource**)&workSource))
    ASSERT_SUCCEEDED(mSyncWakeLock->SetWorkSource(workSource))
    ASSERT_SUCCEEDED(mSyncWakeLock->AcquireLock())
}

CSyncManager::ActiveSyncContext::~ActiveSyncContext()
{}

CAR_INTERFACE_IMPL_2(CSyncManager::ActiveSyncContext, IISyncContext, IServiceConnection)

ECode CSyncManager::ActiveSyncContext::SendHeartbeat()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::ActiveSyncContext::OnFinished(
    /* [in] */ ISyncResult* result)
{
//***    if (Logger::IsLoggable(CSyncManager::TAG, Logger::VERBOSE)) {
//***        String str;
//***        ToString(&str);
//***        Logger::V(CSyncManager::TAG, String("onFinished: ") + str);
//***    }

    // include "this" in the message so that the handler can ignore it if this
    // ActiveSyncContext is no longer the mActiveSyncContext at message handling
    // time
    FAIL_RETURN(mSyncMgr->SendSyncFinishedOrCanceledMessage(this, result))
    return NOERROR;
}

ECode CSyncManager::ActiveSyncContext::ToString(
    /* [in] */ IStringBuilder* sb)
{
    sb->AppendString(String("startTime "));
    sb->AppendInt64(mStartTime);
    sb->AppendString(String(", mTimeoutStartTime "));
    sb->AppendInt64(mTimeoutStartTime);
    sb->AppendString(String(", mHistoryRowId "));
    sb->AppendInt64(mHistoryRowId);
    sb->AppendString(String(", syncOperation "));
    sb->AppendObject(mSyncOperation);
    return NOERROR;
}

ECode CSyncManager::ActiveSyncContext::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    AutoPtr<ServiceConnectionData> connData = new ServiceConnectionData(this, mSyncAdapter.Get());
    AutoPtr<IMessage> msg;
    mSyncMgr->mSyncHandler->ObtainMessage(
        CSyncManager::SyncHandler::MESSAGE_SERVICE_CONNECTED,
        connData, (IMessage**)&msg);
    Boolean result;
    return mSyncMgr->mSyncHandler->SendMessage(msg, &result);
}

ECode CSyncManager::ActiveSyncContext::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    AutoPtr<ServiceConnectionData> connData = new ServiceConnectionData(this, NULL);
    AutoPtr<IMessage> msg;
    mSyncMgr->mSyncHandler->ObtainMessage(
        CSyncManager::SyncHandler::MESSAGE_SERVICE_DISCONNECTED,
        connData.Get(), (IMessage**)&msg);
    Boolean result;
    return mSyncMgr->mSyncHandler->SendMessage(msg, &result);
}

ECode CSyncManager::ActiveSyncContext::BindToSyncAdapter(
    /* [in] */ RegisteredServicesCache::ServiceInfo* info,
    /* [in] */ Int32 userId,
    /* [out] */ Boolean* result)
{
//***    if (Logger::IsLoggable(CSyncManager::TAG, Logger::VERBOSE)) {
//        Logger::D(CSyncManager::TAG, "bindToSyncAdapter: " + info.componentName + ", connection " + this);
//***    }

    AutoPtr<IIntent> intent;
    FAIL_RETURN(CIntent::New((IIntent**)&intent))
    FAIL_RETURN(intent->SetAction(String("android.content.SyncAdapter")))
    AutoPtr<IComponentName> component;
    component = info->mComponentName;
    FAIL_RETURN(intent->SetComponent(component))
//    intent->PutInt32Extra(IIntent::EXTRA_CLIENT_LABEL, R::string::sync_binding_label);

    AutoPtr<IPendingIntentHelper> pendingHelper;
//***    FAIL_RETURN(CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&pendingHelper))
    AutoPtr<IIntent> tmpIntent;
    FAIL_RETURN(CIntent::New(String("android.settings.SYNC_SETTINGS") /* Settings.ACTION_SYNC_SETTINGS */, (IIntent**)&tmpIntent))
    AutoPtr<IUserHandle> userHandle;
//***    FAIL_RETURN(CUserHandle::New(userId, (IUserHandle**)&userHandle))
    AutoPtr<IPendingIntent> pengdingIntent;
    FAIL_RETURN(pendingHelper->GetActivityAsUser(mSyncMgr->mContext, 0, tmpIntent, 0, (IBundle*) NULL,
            userHandle, (IPendingIntent**)&pengdingIntent))
    AutoPtr<IParcelable> parcelable = (IParcelable*) pengdingIntent->Probe(EIID_IParcelable);
    FAIL_RETURN(intent->PutParcelableExtra(IIntent::EXTRA_CLIENT_INTENT, parcelable))
    mBound = TRUE;
    Boolean bindResult = FALSE;
    AutoPtr<IServiceConnection> serviceConn = (IServiceConnection*) this->Probe(EIID_IServiceConnection);
    Int32 id = 0;
    FAIL_RETURN(mSyncOperation->GetUserId(&id))
    FAIL_RETURN(mSyncMgr->mContext->BindService(intent, serviceConn, IContext::BIND_AUTO_CREATE | IContext::BIND_NOT_FOREGROUND
            | IContext::BIND_ALLOW_OOM_MANAGEMENT, id, &bindResult))
    if (!bindResult) {
        mBound = FALSE;
    }
    *result = bindResult;
    return NOERROR;
}

ECode CSyncManager::ActiveSyncContext::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<IStringBuilder> sb = new StringBuilder();
    FAIL_RETURN(ToString(sb))
    return sb->ToString(str);
}

ECode CSyncManager::ActiveSyncContext::ProxyDied()
{
    return mSyncMgr->SendSyncFinishedOrCanceledMessage(this, (ISyncResult*) NULL);
}

ECode CSyncManager::ActiveSyncContext::Close()
{
//***    if (Logger::IsLoggable(CSyncManager::TAG, Logger::VERBOSE)) {
//***        String str;
//***        ToString(&str);
//***        Logger::D(CSyncManager::TAG, String("unBindFromSyncAdapter: connection ") + str);
//***    }
    if (mBound) {
        mBound = FALSE;
        AutoPtr<IServiceConnection> serviceConn = (IServiceConnection*) this->Probe(EIID_IServiceConnection);
        FAIL_RETURN(mSyncMgr->mContext->UnbindService(serviceConn))
    }
    FAIL_RETURN(mSyncWakeLock->ReleaseLock())
    FAIL_RETURN(mSyncWakeLock->SetWorkSource((IWorkSource*) NULL))
    return NOERROR;
}

CSyncManager::ServiceConnectionData::ServiceConnectionData(
    /* [in] */ ActiveSyncContext* syncContext,
    /* [in] */ IISyncAdapter* adapter)
    : activeSyncContext(syncContext)
    , syncAdapter(adapter)
{}

CSyncManager::ServiceConnectionData::~ServiceConnectionData()
{}

CAR_INTERFACE_IMPL(CSyncManager::ServiceConnectionData, IInterface)

CSyncManager::SyncHandler::SyncNotificationInfo::SyncNotificationInfo()
    : isActive(FALSE)
    , startTime(0)
{}

CSyncManager::SyncHandler::SyncNotificationInfo::~SyncNotificationInfo()
{}

CAR_INTERFACE_IMPL(CSyncManager::SyncHandler::SyncNotificationInfo, IInterface)

ECode CSyncManager::SyncHandler::SyncNotificationInfo::ToString(
    /* [in] */ IStringBuilder* sb)
{
    sb->AppendString(String("isActive "));
    sb->AppendBoolean(isActive);
    sb->AppendString(String(", startTime "));
    sb->AppendInt64(startTime);
    return NOERROR;
}

ECode CSyncManager::SyncHandler::SyncNotificationInfo::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<IStringBuilder> sb = new StringBuilder();
    FAIL_RETURN(ToString(sb))
    return sb->ToString(str);
}

CSyncManager::SyncHandler::SyncHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ CSyncManager* syncMgr)
    : HandlerBase(looper)
    , mSyncNotificationInfo(NULL)
    , mSyncTimeTracker(NULL)
    , mAlarmScheduleTime(0)
    , mReadyToRunLatch(NULL)
    , mSyncMgr(syncMgr)
{
    mSyncNotificationInfo = new CSyncManager::SyncHandler::SyncNotificationInfo();
    mSyncTimeTracker = new CSyncManager::SyncTimeTracker(syncMgr);
//***    CCountDownLatch::New(1, (ICountDownLatch**)&mReadyToRunLatch);
}

CSyncManager::SyncHandler::~SyncHandler()
{
}

ECode CSyncManager::SyncHandler::OnBootCompleted()
{
    mSyncMgr->mBootCompleted = TRUE;
    FAIL_RETURN(mSyncMgr->DoDatabaseCleanup())

    if (NULL != mReadyToRunLatch) {
        FAIL_RETURN(mReadyToRunLatch->CountDown())
    }
    return NOERROR;
}

ECode CSyncManager::SyncHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    Int64 earliestFuturePollTime = 0x7FFFFFFFFFFFFFFFL; // Long.MAX_VALUE;
    Int64 nextPendingSyncTime = 0x7FFFFFFFFFFFFFFFL; // Long.MAX_VALUE;

    // Setting the value here instead of a method because we want the dumpsys logs
    // to have the most recent value used.
    // try {
        FAIL_RETURN(WaitUntilReadyToRun())
        FAIL_RETURN(mSyncMgr->ReadDataConnectionState(&(mSyncMgr->mDataConnectionIsConnected)))
        FAIL_RETURN(mSyncMgr->mSyncManagerWakeLock->AcquireLock())
        // Always do this first so that we be sure that any periodic syncs that
        // are ready to run have been converted into pending syncs. This allows the
        // logic that considers the next steps to take based on the set of pending syncs
        // to also take into account the periodic syncs.
        FAIL_RETURN(ScheduleReadyPeriodicSyncs(&earliestFuturePollTime))
        switch (what) {
            case CSyncManager::SyncHandler::MESSAGE_CANCEL:
            {
                AutoPtr<IPair> payload = IPair::Probe(obj);
//                if (Log.isLoggable(TAG, Log.VERBOSE)) {
//                    Log.d(TAG, "handleSyncHandlerMessage: MESSAGE_SERVICE_CANCEL: "
//                            + payload.first + ", " + payload.second);
//                }
                AutoPtr<IAccount> account;
                AutoPtr<ICharSequence> cs;
                FAIL_RETURN(payload->GetFirst((IInterface**)&account))
                FAIL_RETURN(payload->GetSecond((IInterface**)&cs))
                String str;
                FAIL_RETURN(cs->ToString(&str))
                FAIL_RETURN(CancelActiveSyncLocked(account, arg1, str))
                FAIL_RETURN(MaybeStartNextSyncLocked(&nextPendingSyncTime))
                break;
            }

            case CSyncManager::SyncHandler::MESSAGE_SYNC_FINISHED:
            {
//                if (Log.isLoggable(TAG, Log.VERBOSE)) {
//                    Log.v(TAG, "handleSyncHandlerMessage: MESSAGE_SYNC_FINISHED");
//                }
                AutoPtr<SyncHandlerMessagePayload> payload = (SyncHandlerMessagePayload*) obj.Get();
                Boolean ret = FALSE;
                FAIL_RETURN(mSyncMgr->IsSyncStillActive(payload->activeSyncContext, &ret))
                if (!ret) {
//                    Log.d(TAG, "handleSyncHandlerMessage: dropping since the "
//                            + "sync is no longer active: "
//                            + payload.activeSyncContext);
                    break;
                }
                FAIL_RETURN(RunSyncFinishedOrCanceledLocked(payload->syncResult, payload->activeSyncContext))

                // since a sync just finished check if it is time to start a new sync
                FAIL_RETURN(MaybeStartNextSyncLocked(&nextPendingSyncTime))
                break;
            }

            case CSyncManager::SyncHandler::MESSAGE_SERVICE_CONNECTED:
            {
                AutoPtr<ServiceConnectionData> msgData = (ServiceConnectionData*) obj.Get();
//                if (Log.isLoggable(TAG, Log.VERBOSE)) {
//                    Log.d(TAG, "handleSyncHandlerMessage: MESSAGE_SERVICE_CONNECTED: "
//                            + msgData.activeSyncContext);
//                }
                // check that this isn't an old message
                Boolean ret = FALSE;
                FAIL_RETURN(mSyncMgr->IsSyncStillActive(msgData->activeSyncContext, &ret))
                if (ret) {
                    FAIL_RETURN(RunBoundToSyncAdapter(msgData->activeSyncContext, msgData->syncAdapter))
                }
                break;
            }

            case CSyncManager::SyncHandler::MESSAGE_SERVICE_DISCONNECTED:
            {
                AutoPtr<ActiveSyncContext> currentSyncContext = ((ServiceConnectionData*) obj.Get())->activeSyncContext;
//                if (Log.isLoggable(TAG, Log.VERBOSE)) {
//                    Log.d(TAG, "handleSyncHandlerMessage: MESSAGE_SERVICE_DISCONNECTED: "
//                            + currentSyncContext);
//                }
                // check that this isn't an old message
                Boolean ret = FALSE;
                FAIL_RETURN(mSyncMgr->IsSyncStillActive(currentSyncContext, &ret))
                if (ret) {
                    // cancel the sync if we have a syncadapter, which means one is
                    // outstanding
                    if (NULL != currentSyncContext->mSyncAdapter) {
                        //try {
                        FAIL_RETURN(currentSyncContext->mSyncAdapter->CancelSync((IISyncContext*) currentSyncContext))
                        //} catch (RemoteException e) {
                            // we don't need to retry this in this case
                        //}
                    }

                    // pretend that the sync failed with an IOException,
                    // which is a soft error
                    AutoPtr<ISyncResult> syncResult;
                    FAIL_RETURN(CSyncResult::New((ISyncResult**)&syncResult))
                    AutoPtr<ISyncStats> stats;
                    FAIL_RETURN(syncResult->GetStats((ISyncStats**)&stats))
                    Int64 numIoExceptions = 0;
                    FAIL_RETURN(stats->GetNumIoExceptions(&numIoExceptions))
                    numIoExceptions++;
                    FAIL_RETURN(stats->SetNumIoExceptions(numIoExceptions))
                    FAIL_RETURN(RunSyncFinishedOrCanceledLocked(syncResult, currentSyncContext))

                    // since a sync just finished check if it is time to start a new sync
                    FAIL_RETURN(MaybeStartNextSyncLocked(&nextPendingSyncTime))
                }

                break;
            }

            case CSyncManager::SyncHandler::MESSAGE_SYNC_ALARM:
            {
//                boolean isLoggable = Log.isLoggable(TAG, Log.VERBOSE);
//                if (isLoggable) {
//                    Log.v(TAG, "handleSyncHandlerMessage: MESSAGE_SYNC_ALARM");
//                }
                mAlarmScheduleTime = 0;
                //try {
                    FAIL_RETURN(MaybeStartNextSyncLocked(&nextPendingSyncTime))
                //} finally {
                    FAIL_RETURN(mSyncMgr->mHandleAlarmWakeLock->ReleaseLock())
                //}
                break;
            }

            case CSyncManager::SyncHandler::MESSAGE_CHECK_ALARMS:
            {
//                if (Log.isLoggable(TAG, Log.VERBOSE)) {
//                    Log.v(TAG, "handleSyncHandlerMessage: MESSAGE_CHECK_ALARMS");
//                }
                FAIL_RETURN(MaybeStartNextSyncLocked(&nextPendingSyncTime))
                break;
            }
        }
    // } finally {
    //     manageSyncNotificationLocked();
    //     manageSyncAlarmLocked(earliestFuturePollTime, nextPendingSyncTime);
    //     mSyncTimeTracker.update();
    //     mSyncManagerWakeLock.release();
    // }

    FAIL_RETURN(ManageSyncNotificationLocked())
    FAIL_RETURN(ManageSyncAlarmLocked(earliestFuturePollTime, nextPendingSyncTime))
    FAIL_RETURN(mSyncTimeTracker->Update())
    FAIL_RETURN(mSyncMgr->mSyncManagerWakeLock->ReleaseLock())
    return NOERROR;
}

ECode CSyncManager::SyncHandler::InsertStartSyncEvent(
    /* [in] */ ISyncOperation* syncOperation,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)
    Int32 source = 0;
    FAIL_RETURN(syncOperation->GetSyncSource(&source))
    Int64 now = 0;
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    system->GetCurrentTimeMillis(&now);

    AutoPtr<IAccount> account;
    Int32 userId = 0;
    String authority;
    Boolean ret = FALSE;
    FAIL_RETURN(syncOperation->GetAccount((IAccount**)&account))
    FAIL_RETURN(syncOperation->GetUserId(&userId))
    FAIL_RETURN(syncOperation->GetAuthority(&authority))
    FAIL_RETURN(syncOperation->IsInitialization(&ret))

//    EventLog.writeEvent(2720, syncOperation.authority,
//                        SyncStorageEngine.EVENT_START, source,
//                        syncOperation.account.name.hashCode());

    return mSyncMgr->mSyncStorageEngine->InsertStartSyncEvent(account, userId, authority,
            now, source, ret, result);
}

ECode CSyncManager::SyncHandler::StopSyncEvent(
    /* [in] */ Int64 rowId,
    /* [in] */ ISyncOperation* syncOperation,
    /* [in] */ const String& resultMessage,
    /* [in] */ Int32 upstreamActivity,
    /* [in] */ Int32 downstreamActivity,
    /* [in] */ Int64 elapsedTime)
{
//    EventLog.writeEvent(2720, syncOperation.authority,
//                        SyncStorageEngine.EVENT_STOP, syncOperation.syncSource,
//                        syncOperation.account.name.hashCode());

    return mSyncMgr->mSyncStorageEngine->StopSyncEvent(rowId, elapsedTime,
            resultMessage, downstreamActivity, upstreamActivity);
}

ECode CSyncManager::SyncHandler::GetSyncWakeLock(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ IPowerManagerWakeLock** wakeLock)
{
    VALIDATE_NOT_NULL(wakeLock)
    AutoPtr<IPair> wakeLockKey;
    AutoPtr<ICharSequence> authObj;
    FAIL_RETURN(CStringWrapper::New(authority, (ICharSequence**)&authObj))
    FAIL_RETURN(CPair::New(account, authObj, (IPair**)&wakeLockKey))
    HashMap<AutoPtr<IPair>, AutoPtr<IPowerManagerWakeLock> >::Iterator it = mWakeLocks.Find(wakeLockKey);
    *wakeLock = it->mSecond;
    REFCOUNT_ADD(*wakeLock);

    if (NULL == *wakeLock) {
        String name(CSyncManager::SYNC_WAKE_LOCK_PREFIX);
        name += "_";
        name += authority;
        name += "_";
        String str;
        FAIL_RETURN(account->ToString(&str))
        name += str;
        FAIL_RETURN(mSyncMgr->mPowerManager->NewWakeLock(IPowerManager::PARTIAL_WAKE_LOCK, name, wakeLock))
        FAIL_RETURN((*wakeLock)->SetReferenceCounted(FALSE))
        ///mWakeLocks.Insert(HashMap<AutoPtr<IPair>, AutoPtr<IPowerManagerWakeLock> >::ValueType(wakeLockKey, *wakeLock));
        mWakeLocks[wakeLockKey] = *wakeLock;
    }

    return NOERROR;
}

ECode CSyncManager::SyncHandler::WaitUntilReadyToRun()
{
    AutoPtr<ICountDownLatch> latch = mReadyToRunLatch;
    if (NULL != latch) {
        while (TRUE) {
                ECode ec = latch->Await();
                mReadyToRunLatch = NULL;
                if (ec == E_INTERRUPTED_EXCEPTION) {
//***                    AutoPtr<IThread> thread = Thread::GetCurrentThread();
//***                    FAIL_RETURN(thread->Interrupt())
                }
                return ec;
        }
    }
    return NOERROR;
}

ECode CSyncManager::SyncHandler::ScheduleReadyPeriodicSyncs(
    /* [out] */ Int64* startTime)
{
    VALIDATE_NOT_NULL(startTime)
    Boolean backgroundDataUsageAllowed = FALSE;
    AutoPtr<IConnectivityManager> connManager;
    FAIL_RETURN(mSyncMgr->GetConnectivityManager((IConnectivityManager**)&connManager))
    FAIL_RETURN(connManager->GetBackgroundDataSetting(&backgroundDataUsageAllowed))
    Int64 earliestFuturePollTime = 0x7FFFFFFFFFFFFFFFL; // Long.MAX_VALUE;
    if (!backgroundDataUsageAllowed) {
        *startTime = earliestFuturePollTime;
        return NOERROR;
    }

    AutoPtr<ArrayOf<IAccountAndUser *> > accounts = mSyncMgr->mRunningAccounts;
    Int64 nowAbsolute;
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    system->GetCurrentTimeMillis(&nowAbsolute);
    Int64 shiftedNowAbsolute = (0 < nowAbsolute - mSyncMgr->mSyncRandomOffsetMillis)
                                ? (nowAbsolute  - mSyncMgr->mSyncRandomOffsetMillis) : 0;

    AutoPtr<IObjectContainer> objContainer;
    AutoPtr<IObjectEnumerator> objEnumerator;
    FAIL_RETURN(mSyncMgr->mSyncStorageEngine->GetAuthorities((IObjectContainer**)&objContainer))

    if (NULL != objContainer) {
        FAIL_RETURN(objContainer->GetObjectEnumerator((IObjectEnumerator**)&objEnumerator))
        Boolean hasNext = FALSE;
        Boolean ret = FALSE;
        AutoPtr<ISyncStorageEngineAuthorityInfo> info;
        AutoPtr<IAccount> account;
        Int32 userId = 0;
        String authority;

        while ((objEnumerator->MoveNext(&hasNext), hasNext)) {
            info = NULL;
            account = NULL;
            FAIL_RETURN(objEnumerator->Current((IInterface**)&info))
            FAIL_RETURN(info->GetAccount((IAccount**)&account))
            FAIL_RETURN(info->GetUserId(&userId))
            // skip the sync if the account of this operation no longer exists
            FAIL_RETURN(mSyncMgr->ContainsAccountAndUser(accounts, account, userId, &ret))
            if (!ret) continue;

            FAIL_RETURN(info->GetAuthority(&authority))
            Boolean ret2 = FALSE;
            Boolean ret3 = FALSE;
            FAIL_RETURN(mSyncMgr->mSyncStorageEngine->GetMasterSyncAutomatically(userId, &ret2))
            FAIL_RETURN(mSyncMgr->mSyncStorageEngine->GetSyncAutomatically(account, userId, authority, &ret3))
            if (!ret2 || !ret3) continue;

            Int32 result = 0;
            FAIL_RETURN(mSyncMgr->mSyncStorageEngine->GetIsSyncable(account, userId, authority, &result))
            if (result == 0) continue;

            AutoPtr<ISyncStatusInfo> status;
            FAIL_RETURN(mSyncMgr->mSyncStorageEngine->GetOrCreateSyncStatus(info, (ISyncStatusInfo**)&status))

            AutoPtr<IObjectContainer> container;
            AutoPtr<IObjectEnumerator> enumerator;
            FAIL_RETURN(info->GetPeriodicSyncs((IObjectContainer**)&container))

            if (NULL != container) {
                FAIL_RETURN(container->GetObjectEnumerator((IObjectEnumerator**)&enumerator))
                Boolean hasNext = FALSE;
                AutoPtr<IPair> pair;
                AutoPtr<IBundle> extras;
                AutoPtr<IInteger64> secondObj;
                Int64 second = 0;
                Int32 index = 0;

                while ((enumerator->MoveNext(&hasNext), hasNext)) {
                    pair = NULL;
                    extras = NULL;
                    secondObj = NULL;
                    FAIL_RETURN(enumerator->Current((IInterface**)&pair))
                    FAIL_RETURN(pair->GetFirst((IInterface**)&extras))
                    FAIL_RETURN(pair->GetSecond((IInterface**)&secondObj))
                    FAIL_RETURN(secondObj->GetValue(&second))

                    Int64 periodInMillis = second * 1000;
                    // find when this periodic sync was last scheduled to run
                    Int64 lastPollTimeAbsolute = 0;
                    FAIL_RETURN(status->GetPeriodicSyncTime(index, &lastPollTimeAbsolute))
                    Int64 remainingMillis = periodInMillis - (shiftedNowAbsolute % periodInMillis);

                    /*
                     * Sync scheduling strategy:
                     *    Set the next periodic sync based on a random offset (in seconds).
                     *
                     *    Also sync right now if any of the following cases hold
                     *    and mark it as having been scheduled
                     *
                     * Case 1:  This sync is ready to run now.
                     * Case 2:  If the lastPollTimeAbsolute is in the future,
                     *          sync now and reinitialize. This can happen for
                     *          example if the user changed the time, synced and
                     *          changed back.
                     * Case 3:  If we failed to sync at the last scheduled time
                     */
                    if (remainingMillis == periodInMillis  // Case 1
                            || lastPollTimeAbsolute > nowAbsolute // Case 2
                            || (nowAbsolute - lastPollTimeAbsolute
                                    >= periodInMillis)) { // Case 3
                        // Sync now
                        AutoPtr<IPair> backoff;
                        FAIL_RETURN(mSyncMgr->mSyncStorageEngine->GetBackoff(account, userId, authority, (IPair**)&backoff))
                        AutoPtr<ISyncAdapterType> adapterType;
                        AutoPtr<RegisteredServicesCache::ServiceInfo> syncAdapterInfo;
    //                    final RegisteredServicesCache.ServiceInfo<SyncAdapterType> syncAdapterInfo;
    //                    syncAdapterInfo = mSyncMgr->mSyncAdapters.getServiceInfo(
    //                            SyncAdapterType.newKey(authority, account.type),
    //                            userId);
                        if (NULL == syncAdapterInfo) {
                            index++;
                            continue;
                        }

                        Int64 first = 0;
                        if (NULL != backoff) {
                            AutoPtr<IInteger64> backoffFirst;
                            FAIL_RETURN(backoff->GetFirst((IInterface**)&backoffFirst))
                            FAIL_RETURN(backoffFirst->GetValue(&first))
                        }
                        Int64 delayUntilTime = 0;
                        FAIL_RETURN(mSyncMgr->mSyncStorageEngine->GetDelayUntilTime(account, userId, authority,
                                &delayUntilTime))
                        adapterType = ISyncAdapterType::Probe(syncAdapterInfo->mType);
                        Boolean isAllow = FALSE;
                        FAIL_RETURN(adapterType->AllowParallelSyncs(&isAllow))
                        AutoPtr<ISyncOperation> syncOperation;
                        FAIL_RETURN(CSyncOperation::New(account, userId, ISyncStorageEngine::SOURCE_PERIODIC,
                                authority, extras, 0 /* delay */,first, delayUntilTime, isAllow,
                                (ISyncOperation**)&syncOperation))
                        FAIL_RETURN(mSyncMgr->ScheduleSyncOperation(syncOperation))
                        FAIL_RETURN(status->SetPeriodicSyncTime(index, nowAbsolute))
                    }
                    // Compute when this periodic sync should next run
                    Int64 nextPollTimeAbsolute = nowAbsolute + remainingMillis;

                    // remember this time if it is earlier than earliestFuturePollTime
                    if (nextPollTimeAbsolute < earliestFuturePollTime) {
                        earliestFuturePollTime = nextPollTimeAbsolute;
                    }
                    index++;
                }
            }
        }
    }

    if (earliestFuturePollTime == 0x7FFFFFFFFFFFFFFFL /* Long.MAX_VALUE */) {
        *startTime = 0x7FFFFFFFFFFFFFFFL; // Long.MAX_VALUE;
        return NOERROR;
    }

    // convert absolute time to elapsed time
    Int64 realtime = 0;
//    realtime = SystemClock.elapsedRealtime();
    *startTime = realtime +
            ((earliestFuturePollTime < nowAbsolute) ? 0 : (earliestFuturePollTime - nowAbsolute));
    return NOERROR;
}

ECode CSyncManager::SyncHandler::MaybeStartNextSyncLocked(
    /* [out] */ Int64* nextTime)
{
    VALIDATE_NOT_NULL(nextTime)
//***    Boolean isLoggable = Logger::IsLoggable(CSyncManager::TAG, Logger::VERBOSE);
//    if (isLoggable) Log.v(TAG, "maybeStartNextSync");

    // If we aren't ready to run (e.g. the data connection is down), get out.
    if (!mSyncMgr->mDataConnectionIsConnected) {
//***        if (isLoggable) {
//***            Logger::V(CSyncManager::TAG, String("maybeStartNextSync: no data connection, skipping"));
//***        }
        *nextTime = 0x7FFFFFFFFFFFFFFFL; // Long.MAX_VALUE;
        return NOERROR;
    }

    if (mSyncMgr->mStorageIsLow) {
//***        if (isLoggable) {
//***            Logger::V(CSyncManager::TAG, String("maybeStartNextSync: memory low, skipping"));
//***        }
        *nextTime = 0x7FFFFFFFFFFFFFFFL; // Long.MAX_VALUE;
        return NOERROR;
    }

    // If the accounts aren't known yet then we aren't ready to run. We will be kicked
    // when the account lookup request does complete.
    AutoPtr<ArrayOf<IAccountAndUser *> > accounts = mSyncMgr->mRunningAccounts;
    if (accounts == INITIAL_ACCOUNTS_ARRAY) {
//***        if (isLoggable) {
//***            Logger::V(CSyncManager::TAG, String("maybeStartNextSync: accounts not known, skipping"));
//***        }
        *nextTime = 0x7FFFFFFFFFFFFFFFL; // Long.MAX_VALUE;
        return NOERROR;
    }

    // Otherwise consume SyncOperations from the head of the SyncQueue until one is
    // found that is runnable (not disabled, etc). If that one is ready to run then
    // start it, otherwise just get out.
    AutoPtr<IConnectivityManager> connManager;
    FAIL_RETURN(mSyncMgr->GetConnectivityManager((IConnectivityManager**)&connManager))
    Boolean backgroundDataUsageAllowed = FALSE;
    FAIL_RETURN(connManager->GetBackgroundDataSetting(&backgroundDataUsageAllowed))
    Int64 now = 0;
//    now = SystemClock.elapsedRealtime();

    // will be set to the next time that a sync should be considered for running
    Int64 nextReadyToRunTime = 0x7FFFFFFFFFFFFFFFL; // Long.MAX_VALUE;

    // order the sync queue, dropping syncs that are not allowed
    List<AutoPtr<ISyncOperation> > operations;
    {
        AutoLock lock(mSyncMgr->mSyncQueueLock);
//        if (isLoggable) {
//            Log.v(TAG, "build the operation array, syncQueue size is "
//                + mSyncQueue.getOperations().size());
//        }

        //final Iterator<SyncOperation> operationIterator = mSyncMgr->mSyncQueue->GetOperations().iterator();
        AutoPtr<ArrayOf<ISyncOperation *> > operationsArray;
        FAIL_RETURN(mSyncMgr->mSyncQueue->GetOperations((ArrayOf<ISyncOperation *>**)&operationsArray))
        AutoPtr<IActivityManager> activityManager;
        FAIL_RETURN(mSyncMgr->mContext->GetSystemService(IContext::ACTIVITY_SERVICE, (IInterface**)&activityManager))
        assert(activityManager != NULL);
        //final Set<Integer> removedUsers = Sets.newHashSet();
        AutoPtr<HashSet<Int32> > removedUsers = new HashSet<Int32>();
        const Int32 N = operationsArray->GetLength();
        AutoPtr<ISyncOperation> op;
        AutoPtr<IAccount> account;
        Int32 userId = 0;
        String authority;
        Boolean ret = FALSE;

        for (Int32 i = 0; i < N; i++) {
            op = (*operationsArray)[i];
            account = NULL;
            FAIL_RETURN(op->GetAccount((IAccount**)&account))
            FAIL_RETURN(op->GetUserId(&userId))

            // drop the sync if the account of this operation no longer exists
            FAIL_RETURN(mSyncMgr->ContainsAccountAndUser(accounts, account, userId, &ret))
            if (!ret) {
                AutoPtr<ISyncStorageEnginePendingOperation> pengdingOp;
                FAIL_RETURN(op->GetPendingOperation((ISyncStorageEnginePendingOperation**)&pengdingOp))
                FAIL_RETURN(mSyncMgr->mSyncStorageEngine->DeleteFromPending(pengdingOp, &ret))
                FAIL_RETURN(mSyncMgr->mSyncQueue->Remove(op))
                continue;
            }

            // drop this sync request if it isn't syncable
            FAIL_RETURN(op->GetAuthority(&authority))
            Int32 syncableState = 0;
            FAIL_RETURN(mSyncMgr->mSyncStorageEngine->GetIsSyncable(account, userId, authority, &syncableState))
            if (syncableState == 0) {
                AutoPtr<ISyncStorageEnginePendingOperation> pengdingOp;
                FAIL_RETURN(op->GetPendingOperation((ISyncStorageEnginePendingOperation**)&pengdingOp))
                FAIL_RETURN(mSyncMgr->mSyncStorageEngine->DeleteFromPending(pengdingOp, &ret))
                FAIL_RETURN(mSyncMgr->mSyncQueue->Remove(op))
                continue;
            }

            // if the user in not running, drop the request
            FAIL_RETURN(activityManager->IsUserRunning(userId, &ret))
            if (!ret) {
                AutoPtr<IUserInfo> userInfo;
                FAIL_RETURN(mSyncMgr->mUserManager->GetUserInfo(userId, (IUserInfo**)&userInfo))
                if (NULL == userInfo) {
                    removedUsers->Insert(userId);
                }
                continue;
            }

            // if the next run time is in the future, meaning there are no syncs ready
            // to run, return the time
            Int64 effectiveRunTime = 0;
            FAIL_RETURN(op->GetEffectiveRunTime(&effectiveRunTime))
            if (effectiveRunTime > now) {
                if (nextReadyToRunTime > effectiveRunTime) {
                    nextReadyToRunTime = effectiveRunTime;
                }
                continue;
            }

//            final RegisteredServicesCache.ServiceInfo<SyncAdapterType> syncAdapterInfo;
//            syncAdapterInfo = mSyncAdapters.getServiceInfo(
//                    SyncAdapterType.newKey(op.authority, op.account.type), op.userId);
            AutoPtr<RegisteredServicesCache::ServiceInfo> syncAdapterInfo;
            // only proceed if network is connected for requesting UID
            Boolean uidNetworkConnected = FALSE;
//            if (NULL != syncAdapterInfo) {
//                final NetworkInfo networkInfo = getConnectivityManager()
//                        .getActiveNetworkInfoForUid(syncAdapterInfo.uid);
//                uidNetworkConnected = networkInfo != null && networkInfo.isConnected();
//            }

            // skip the sync if it isn't manual, and auto sync or
            // background data usage is disabled or network is
            // disconnected for the target UID.
            AutoPtr<IBundle> extras;
            FAIL_RETURN(op->GetExtras((IBundle**)&extras))
            FAIL_RETURN(extras->GetBoolean(IContentResolver::SYNC_EXTRAS_IGNORE_SETTINGS, FALSE, &ret))
            Boolean ret2 = FALSE;
            Boolean ret3 = FALSE;

            if (!ret && (syncableState > 0)
                    && (!(mSyncMgr->mSyncStorageEngine->GetMasterSyncAutomatically(userId, &ret2), ret2)
                        || !backgroundDataUsageAllowed
                        || !uidNetworkConnected
                        || !(mSyncMgr->mSyncStorageEngine->GetSyncAutomatically(account, userId, authority, &ret3), ret3))) {
                AutoPtr<ISyncStorageEnginePendingOperation> pengdingOp;
                FAIL_RETURN(op->GetPendingOperation((ISyncStorageEnginePendingOperation**)&pengdingOp))
                FAIL_RETURN(mSyncMgr->mSyncStorageEngine->DeleteFromPending(pengdingOp, &ret))
                FAIL_RETURN(mSyncMgr->mSyncQueue->Remove(op))
                continue;
            }

            operations.PushBack(op);
        }

        HashSet<Int32>::Iterator it = removedUsers->Begin();
        for (; it != removedUsers->End(); it++) {
            Int32 user = *it;
            AutoPtr<IUserInfo> userInfo;
            // if it's still removed
            FAIL_RETURN(mSyncMgr->mUserManager->GetUserInfo(user, (IUserInfo**)&userInfo))
            if (NULL == userInfo) {
                FAIL_RETURN(mSyncMgr->OnUserRemoved(user))
            }
        }
    }

    // find the next operation to dispatch, if one is ready
    // iterate from the top, keep issuing (while potentially cancelling existing syncs)
    // until the quotas are filled.
    // once the quotas are filled iterate once more to find when the next one would be
    // (also considering pre-emption reasons).
//    if (isLoggable) Log.v(TAG, "sort the candidate operations, size " + operations.size());
//    Collections.sort(operations);
//    if (isLoggable) Log.v(TAG, "dispatch all ready sync operations");
    AutoPtr<ISyncOperation> candidate;
    Boolean candidateIsInitialization = FALSE;
    List<AutoPtr<ISyncOperation> >::Iterator it = operations.Begin();
    for (Int32 i = 0; it != operations.End(); it++, i++) {
        candidate = *it;
        FAIL_RETURN(candidate->IsInitialization(&candidateIsInitialization))
        Int32 numInit = 0;
        Int32 numRegular = 0;
        AutoPtr<CSyncManager::ActiveSyncContext> conflict = NULL;
        AutoPtr<CSyncManager::ActiveSyncContext> longRunning = NULL;
        AutoPtr<CSyncManager::ActiveSyncContext> toReschedule = NULL;
        AutoPtr<CSyncManager::ActiveSyncContext> oldestNonExpeditedRegular = NULL;

        AutoPtr<CSyncManager::ActiveSyncContext> activeSyncContext;
        List<AutoPtr<CSyncManager::ActiveSyncContext> >::Iterator iter = mSyncMgr->mActiveSyncContexts->Begin();
        Boolean result = FALSE;
        Boolean result2 = FALSE;
        Boolean result3 = FALSE;
        AutoPtr<IAccount> account;
        Int32 userId = 0;
        String authority;
        String type;
        String name;
        AutoPtr<IAccount> account2;
        Int32 userId2 = 0;
        String authority2;
        String type2;
        String name2;

        for (; iter != mSyncMgr->mActiveSyncContexts->End(); iter++) {
            activeSyncContext = *iter;
            AutoPtr<ISyncOperation> activeOp = activeSyncContext->mSyncOperation;
            FAIL_RETURN(activeOp->IsInitialization(&result))
            if (result) {
                numInit++;
            }
            else {
                numRegular++;
                FAIL_RETURN(activeOp->IsExpedited(&result))
                if (!result) {
                    if (oldestNonExpeditedRegular == NULL
                        || (oldestNonExpeditedRegular->mStartTime
                            > activeSyncContext->mStartTime)) {
                        oldestNonExpeditedRegular = activeSyncContext;
                    }
                }
            }

            account = NULL;
            account2 = NULL;
            FAIL_RETURN(activeOp->GetAccount((IAccount**)&account))
            FAIL_RETURN(activeOp->GetUserId(&userId))
            FAIL_RETURN(activeOp->GetAuthority(&authority))
            FAIL_RETURN(activeOp->AllowParallelSyncs(&result))
            FAIL_RETURN(account->GetType(&type))
            FAIL_RETURN(account->GetName(&name))
            FAIL_RETURN(candidate->GetAccount((IAccount**)&account2))
            FAIL_RETURN(candidate->GetUserId(&userId2))
            FAIL_RETURN(candidate->GetAuthority(&authority2))
            FAIL_RETURN(account2->GetType(&type2))
            FAIL_RETURN(account2->GetName(&name2))
            if (type.Equals(type2)
                    && authority.Equals(authority2)
                    && userId == userId2
                    && (!result || name.Equals(name2))) {
                conflict = activeSyncContext;
                // don't break out since we want to do a full count of the varieties
            }
            else {
                FAIL_RETURN(activeOp->IsInitialization(&result))
                if (candidateIsInitialization == result
                        && activeSyncContext->mStartTime + MAX_TIME_PER_SYNC < now) {
                    longRunning = activeSyncContext;
                    // don't break out since we want to do a full count of the varieties
                }
            }
        }

//        if (isLoggable) {
//            Log.v(TAG, "candidate " + (i + 1) + " of " + N + ": " + candidate);
//            Log.v(TAG, "  numActiveInit=" + numInit + ", numActiveRegular=" + numRegular);
//            Log.v(TAG, "  longRunning: " + longRunning);
//            Log.v(TAG, "  conflict: " + conflict);
//            Log.v(TAG, "  oldestNonExpeditedRegular: " + oldestNonExpeditedRegular);
//        }

        Boolean roomAvailable = candidateIsInitialization
                ? numInit < MAX_SIMULTANEOUS_INITIALIZATION_SYNCS
                : numRegular < MAX_SIMULTANEOUS_REGULAR_SYNCS;

        if (NULL != conflict) {
            FAIL_RETURN(conflict->mSyncOperation->IsInitialization(&result))
            if (candidateIsInitialization && !result
                    && numInit < MAX_SIMULTANEOUS_INITIALIZATION_SYNCS) {
                toReschedule = conflict;
//                if (Log.isLoggable(TAG, Log.VERBOSE)) {
//                    Log.v(TAG, "canceling and rescheduling sync since an initialization "
//                            + "takes higher priority, " + conflict);
//                }
            }
            else if ((candidate->GetExpedited(&result), result)
                    && !(conflict->mSyncOperation->GetExpedited(&result2), result2)
                    && (candidateIsInitialization
                        == (conflict->mSyncOperation->IsInitialization(&result3), result3))) {
                toReschedule = conflict;
//                if (Log.isLoggable(TAG, Log.VERBOSE)) {
//                    Log.v(TAG, "canceling and rescheduling sync since an expedited "
//                            + "takes higher priority, " + conflict);
//                }
            }
            else {
                continue;
            }
        }
        else if (roomAvailable) {
            // dispatch candidate
        }
        else if ((candidate->IsExpedited(&result), result) && NULL != oldestNonExpeditedRegular
                   && !candidateIsInitialization) {
            // We found an active, non-expedited regular sync. We also know that the
            // candidate doesn't conflict with this active sync since conflict
            // is null. Reschedule the active sync and start the candidate.
            toReschedule = oldestNonExpeditedRegular;
//            if (Log.isLoggable(TAG, Log.VERBOSE)) {
//                Log.v(TAG, "canceling and rescheduling sync since an expedited is ready to run, "
//                        + oldestNonExpeditedRegular);
//            }
        }
        else if (NULL != longRunning
                && (candidateIsInitialization
                    == (longRunning->mSyncOperation->IsInitialization(&result), result))) {
            // We found an active, long-running sync. Reschedule the active
            // sync and start the candidate.
            toReschedule = longRunning;
//            if (Log.isLoggable(TAG, Log.VERBOSE)) {
//                Log.v(TAG, "canceling and rescheduling sync since it ran roo long, "
//                      + longRunning);
//            }
        }
        else {
            // we were unable to find or make space to run this candidate, go on to
            // the next one
            continue;
        }

        if (NULL != toReschedule) {
            FAIL_RETURN(RunSyncFinishedOrCanceledLocked((ISyncResult*) NULL, toReschedule))
            FAIL_RETURN(mSyncMgr->ScheduleSyncOperation(toReschedule->mSyncOperation))
        }
        {
            AutoLock lock(mSyncMgr->mSyncQueueLock);
            FAIL_RETURN(mSyncMgr->mSyncQueue->Remove(candidate))
        }
        FAIL_RETURN(DispatchSyncOperation(candidate, &result))
    }

    *nextTime = nextReadyToRunTime;
    return NOERROR;
}

ECode CSyncManager::SyncHandler::DispatchSyncOperation(
    /* [in] */ ISyncOperation* op,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(op)
    VALIDATE_NOT_NULL(result)
//    if (Log.isLoggable(TAG, Log.VERBOSE)) {
//        Log.v(TAG, "dispatchSyncOperation: we are going to sync " + op);
//        Log.v(TAG, "num active syncs: " + mActiveSyncContexts.size());
//        for (ActiveSyncContext syncContext : mActiveSyncContexts) {
//            Log.v(TAG, syncContext.toString());
//        }
//    }

    AutoPtr<IAccount> account;
    Int32 userId = 0;
    String authority;
    String type;
    String name;
    FAIL_RETURN(op->GetAccount((IAccount**)&account))
    FAIL_RETURN(op->GetUserId(&userId))
    FAIL_RETURN(op->GetAuthority(&authority))
    FAIL_RETURN(account->GetType(&type))
    FAIL_RETURN(account->GetName(&name))
    // connect to the sync adapter
//    syncAdapterType = SyncAdapterType.newKey(authority, type);
//    final RegisteredServicesCache.ServiceInfo<SyncAdapterType> syncAdapterInfo;
//    syncAdapterInfo = mSyncAdapters.getServiceInfo(syncAdapterType, userId);
    AutoPtr<ISyncAdapterType> syncAdapterType;
    AutoPtr<RegisteredServicesCache::ServiceInfo> syncAdapterInfo;

    if (NULL == syncAdapterInfo) {
//        Log.d(TAG, "can't find a sync adapter for " + syncAdapterType
//                + ", removing settings for it");
        FAIL_RETURN(mSyncMgr->mSyncStorageEngine->RemoveAuthority(account, userId, authority))
        *result = FALSE;
        return NOERROR;
    }

    Int64 eventValue = 0;
    Int32 uid = 0;
    FAIL_RETURN(InsertStartSyncEvent(op, &eventValue))
    uid = syncAdapterInfo->mUid;
    AutoPtr<ActiveSyncContext> activeSyncContext = new ActiveSyncContext(op, eventValue, uid, mSyncMgr);
    FAIL_RETURN(mSyncMgr->mSyncStorageEngine->AddActiveSync(activeSyncContext->mSyncOperation, activeSyncContext->mStartTime,
            (ISyncInfo**)&(activeSyncContext->mSyncInfo)))
    mSyncMgr->mActiveSyncContexts->PushBack(activeSyncContext);
//    if (Log.isLoggable(TAG, Log.VERBOSE)) {
//        Log.v(TAG, "dispatchSyncOperation: starting " + activeSyncContext);
//    }
    Boolean ret = FALSE;
    FAIL_RETURN(activeSyncContext->BindToSyncAdapter(syncAdapterInfo, userId, &ret))
    if (!ret) {
//        Log.e(TAG, "Bind attempt failed to " + syncAdapterInfo);
        FAIL_RETURN(CloseActiveSyncContext(activeSyncContext))
        *result = FALSE;
        return NOERROR;
    }

    *result = TRUE;
    return NOERROR;
}

ECode CSyncManager::SyncHandler::RunBoundToSyncAdapter(
    /* [in] */ ActiveSyncContext* activeSyncContext,
    /* [in] */ IISyncAdapter* syncAdapter)
{
    activeSyncContext->mSyncAdapter = syncAdapter;
    AutoPtr<ISyncOperation> syncOperation = activeSyncContext->mSyncOperation;
    AutoPtr<ISyncOperation> tmpOp;
//    try {
        activeSyncContext->mIsLinkedToDeath = TRUE;
        AutoPtr< ::IProxy > proxy = (::IProxy*)syncAdapter->Probe(::EIID_IProxy);
        if (proxy != NULL) proxy->LinkToDeath(activeSyncContext, 0);
        String authority;
        AutoPtr<IAccount> account;
        AutoPtr<IBundle> extras;
        ECode ec = syncOperation->GetAccount((IAccount**)&account);
        if (FAILED(ec)) goto EXIT;
        ec = syncOperation->GetAuthority(&authority);
        if (FAILED(ec)) goto EXIT;
        ec = syncOperation->GetExtras((IBundle**)&extras);
        if (FAILED(ec)) goto EXIT;
        ec = syncAdapter->StartSync(activeSyncContext, authority, account, extras);
        if (FAILED(ec)) goto EXIT;
//    } catch (RemoteException remoteExc) {
//        Log.d(TAG, "maybeStartNextSync: caught a RemoteException, rescheduling", remoteExc);
//        closeActiveSyncContext(activeSyncContext);
//        increaseBackoffSetting(syncOperation);
//        scheduleSyncOperation(new SyncOperation(syncOperation));
//    } catch (RuntimeException exc) {
//        closeActiveSyncContext(activeSyncContext);
//        Log.e(TAG, "Caught RuntimeException while starting the sync " + syncOperation, exc);
//    }
    EXIT:
        if (ec == E_RUNTIME_EXCEPTION) {
            CloseActiveSyncContext(activeSyncContext);
//            Log.e(TAG, "Caught RuntimeException while starting the sync " + syncOperation, exc);
            return E_RUNTIME_EXCEPTION;
        }
        else if (ec != NOERROR) {
//            Log.d(TAG, "maybeStartNextSync: caught a RemoteException, rescheduling", remoteExc);
            FAIL_RETURN(CloseActiveSyncContext(activeSyncContext))
            FAIL_RETURN(mSyncMgr->IncreaseBackoffSetting(syncOperation))
            FAIL_RETURN(CSyncOperation::New(syncOperation, (ISyncOperation**)&tmpOp))
            FAIL_RETURN(mSyncMgr->ScheduleSyncOperation(tmpOp))
            return ec;
        }
    return NOERROR;
}

ECode CSyncManager::SyncHandler::CancelActiveSyncLocked(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authority)
{
    AutoPtr<ActiveSyncContext> activeSyncContext;
    List<AutoPtr<CSyncManager::ActiveSyncContext> >::Iterator it = mSyncMgr->mActiveSyncContexts->Begin();
    for (; it != mSyncMgr->mActiveSyncContexts->End(); it++) {
        activeSyncContext = *it;
        if (NULL != activeSyncContext) {
            // if an account was specified then only cancel the sync if it matches
            AutoPtr<IAccount> act;
            FAIL_RETURN(activeSyncContext->mSyncOperation->GetAccount((IAccount**)&act))
            if (NULL != account) {
                if (!_CObject_Compare(account, act)) {
                    continue;
                }
            }
            // if an authority was specified then only cancel the sync if it matches
            String auth;
            FAIL_RETURN(activeSyncContext->mSyncOperation->GetAuthority(&auth))
            if (!authority.IsNull()) {
                if (!authority.Equals(auth)) {
                    continue;
                }
            }
            // check if the userid matches
            Int32 uid = 0;
            FAIL_RETURN(activeSyncContext->mSyncOperation->GetUserId(&uid))
            if (userId != IUserHandle::USER_ALL && userId != uid) {
                continue;
            }
            FAIL_RETURN(RunSyncFinishedOrCanceledLocked((ISyncResult*) NULL /* no result since this is a cancel */,
                    activeSyncContext))
        }
    }

    return NOERROR;
}

ECode CSyncManager::SyncHandler::RunSyncFinishedOrCanceledLocked(
    /* [in] */ ISyncResult* syncResult,
    /* [in] */ ActiveSyncContext* activeSyncContext)
{
//***    Boolean isLoggable = Logger::IsLoggable(CSyncManager::TAG, Logger::VERBOSE);

    if (activeSyncContext->mIsLinkedToDeath) {
        AutoPtr< ::IProxy > proxy = (::IProxy*)activeSyncContext->mSyncAdapter->Probe(::EIID_IProxy);
        Boolean result;
        proxy->UnlinkToDeath(activeSyncContext, 0, &result);
        activeSyncContext->mIsLinkedToDeath = FALSE;
    }
    FAIL_RETURN(CloseActiveSyncContext(activeSyncContext))
    AutoPtr<ISyncOperation> syncOperation = activeSyncContext->mSyncOperation;
    Int64 elapsedTime = 0;
//    elapsedTime = SystemClock.elapsedRealtime() - activeSyncContext->mStartTime;
    String historyMessage;
    Int32 downstreamActivity = 0;
    Int32 upstreamActivity = 0;
    Boolean ret = FALSE;

    if (NULL != syncResult) {
//        if (isLoggable) {
//            Log.v(TAG, "runSyncFinishedOrCanceled [finished]: "
//                    + syncOperation + ", result " + syncResult);
//        }
        FAIL_RETURN(syncResult->HasError(&ret))
        if (!ret) {
            historyMessage = ISyncStorageEngine::MESG_SUCCESS;
            // TODO: set these correctly when the SyncResult is extended to include it
            downstreamActivity = 0;
            upstreamActivity = 0;
            FAIL_RETURN(mSyncMgr->ClearBackoffSetting(syncOperation))
        }
        else {
//            Log.d(TAG, "failed sync operation " + syncOperation + ", " + syncResult);
            // the operation failed so increase the backoff time
            FAIL_RETURN(syncResult->GetSyncAlreadyInProgress(&ret))
            if (!ret) {
                FAIL_RETURN(mSyncMgr->IncreaseBackoffSetting(syncOperation))
            }
            // reschedule the sync if so indicated by the syncResult
            FAIL_RETURN(mSyncMgr->MaybeRescheduleSync(syncResult, syncOperation))
            Int32 errorNumber = 0;
            FAIL_RETURN(SyncResultToErrorNumber(syncResult, &errorNumber))
            historyMessage = StringUtils::Int32ToString(errorNumber);
            // TODO: set these correctly when the SyncResult is extended to include it
            downstreamActivity = 0;
            upstreamActivity = 0;
        }

        Int64 delayUntil = 0;
        FAIL_RETURN(syncResult->GetDelayUntil(&delayUntil))
        FAIL_RETURN(mSyncMgr->SetDelayUntilTime(syncOperation, delayUntil))
    }
    else {
//        if (isLoggable) {
//            Log.v(TAG, "runSyncFinishedOrCanceled [canceled]: " + syncOperation);
//        }
        if (NULL != activeSyncContext->mSyncAdapter) {
            FAIL_RETURN(activeSyncContext->mSyncAdapter->CancelSync((IISyncContext*) activeSyncContext))
        }
        historyMessage = ISyncStorageEngine::MESG_CANCELED;
        downstreamActivity = 0;
        upstreamActivity = 0;
    }

    FAIL_RETURN(StopSyncEvent(activeSyncContext->mHistoryRowId, syncOperation, historyMessage,
            upstreamActivity, downstreamActivity, elapsedTime))

    AutoPtr<IAccount> account;
    String authority;
    Int32 userId = 0;
    FAIL_RETURN(syncOperation->GetAccount((IAccount**)&account))
    FAIL_RETURN(syncOperation->GetAuthority(&authority))
    FAIL_RETURN(syncOperation->GetUserId(&userId))

    if (NULL != syncResult && (syncResult->GetTooManyDeletions(&ret), ret)) {

        AutoPtr<ISyncStats> stats;
        FAIL_RETURN(syncResult->GetStats((ISyncStats**)&stats))
        Int64 numDeletes = 0;
        FAIL_RETURN(stats->GetNumDeletes(&numDeletes))
        FAIL_RETURN(InstallHandleTooManyDeletesNotification(account, authority, numDeletes, userId))
    }
    else {
//        mNotificationMgr->CancelAsUser(String(NULL),
//                syncOperation.account.hashCode() ^ syncOperation.authority.hashCode(),
//                new UserHandle(userId));
    }

    if (NULL != syncResult && (syncResult->GetFullSyncRequested(&ret), ret)) {
        AutoPtr<ISyncOperation> tmpOp;
        AutoPtr<IBundle> tmpBundle;
//***        FAIL_RETURN(CBundle::New((IBundle**)&tmpBundle))
        Int32 syncSource = 0;
        Int64 backoff = 0;
        Int64 delayUntil = 0;
        FAIL_RETURN(syncOperation->GetSyncSource(&syncSource))
        FAIL_RETURN(syncOperation->GetBackoff(&backoff))
        FAIL_RETURN(syncOperation->GetDelayUntil(&delayUntil))
        FAIL_RETURN(syncOperation->AllowParallelSyncs(&ret))
        FAIL_RETURN(CSyncOperation::New(account, userId, syncSource, authority, tmpBundle, 0,
                backoff, delayUntil, ret, (ISyncOperation**)&tmpOp))
        FAIL_RETURN(mSyncMgr->ScheduleSyncOperation(tmpOp))
    }
    // no need to schedule an alarm, as that will be done by our caller.
    return NOERROR;
}

ECode CSyncManager::SyncHandler::CloseActiveSyncContext(
    /* [in] */ ActiveSyncContext* activeSyncContext)
{
    FAIL_RETURN(activeSyncContext->Close())
    mSyncMgr->mActiveSyncContexts->Remove(activeSyncContext);
    Int32 userId = 0;
    FAIL_RETURN(activeSyncContext->mSyncOperation->GetUserId(&userId))
    FAIL_RETURN(mSyncMgr->mSyncStorageEngine->RemoveActiveSync(activeSyncContext->mSyncInfo, userId))
    return NOERROR;
}

ECode CSyncManager::SyncHandler::SyncResultToErrorNumber(
    /* [in] */ ISyncResult* syncResult,
    /* [out] */ Int32* errorNumber)
{
    VALIDATE_NOT_NULL(errorNumber)
    Boolean ret = FALSE;
    FAIL_RETURN(syncResult->GetSyncAlreadyInProgress(&ret))
    if (ret) {
        *errorNumber = IContentResolver::SYNC_ERROR_SYNC_ALREADY_IN_PROGRESS;
        return NOERROR;
    }

    AutoPtr<ISyncStats> stats;
    FAIL_RETURN(syncResult->GetStats((ISyncStats**)&stats))
    Int64 numAuthExceptions = 0;
    FAIL_RETURN(stats->GetNumAuthExceptions(&numAuthExceptions))
    if (numAuthExceptions > 0) {
        *errorNumber = IContentResolver::SYNC_ERROR_AUTHENTICATION;
        return NOERROR;
    }

    Int64 numIoExceptions = 0;
    FAIL_RETURN(stats->GetNumIoExceptions(&numIoExceptions))
    if (numIoExceptions > 0) {
        *errorNumber = IContentResolver::SYNC_ERROR_IO;
        return NOERROR;
    }

    Int64 numParseExceptions = 0;
    FAIL_RETURN(stats->GetNumParseExceptions(&numParseExceptions))
    if (numParseExceptions > 0) {
        *errorNumber = IContentResolver::SYNC_ERROR_PARSE;
        return NOERROR;
    }

    Int64 numConflictDetectedExceptions = 0;
    FAIL_RETURN(stats->GetNumConflictDetectedExceptions(&numConflictDetectedExceptions))
    if (numConflictDetectedExceptions > 0) {
        *errorNumber = IContentResolver::SYNC_ERROR_CONFLICT;
        return NOERROR;
    }

    FAIL_RETURN(syncResult->GetTooManyDeletions(&ret))
    if (ret) {
        *errorNumber = IContentResolver::SYNC_ERROR_TOO_MANY_DELETIONS;
        return NOERROR;
    }

    FAIL_RETURN(syncResult->GetTooManyRetries(&ret))
    if (ret) {
        *errorNumber = IContentResolver::SYNC_ERROR_TOO_MANY_RETRIES;
        return NOERROR;
    }

    FAIL_RETURN(syncResult->GetDatabaseError(&ret))
    if (ret) {
        *errorNumber = IContentResolver::SYNC_ERROR_INTERNAL;
        return NOERROR;
    }

    // throw new IllegalStateException("we are not in an error state, " + syncResult);
    return E_ILLEGAL_STATE_EXCEPTION;
}

ECode CSyncManager::SyncHandler::ManageSyncNotificationLocked()
{
    Boolean shouldCancel = FALSE;
    Boolean shouldInstall = FALSE;

    if (mSyncMgr->mActiveSyncContexts->IsEmpty()) {
        mSyncNotificationInfo->startTime = 0;

        // we aren't syncing. if the notification is active then remember that we need
        // to cancel it and then clear out the info
        shouldCancel = mSyncNotificationInfo->isActive;
        shouldInstall = FALSE;
    }
    else {
        // we are syncing
        Int64 now = 0;
//        now = SystemClock.elapsedRealtime();
        if (mSyncNotificationInfo->startTime == 0) {
            mSyncNotificationInfo->startTime = now;
        }

        // there are three cases:
        // - the notification is up: do nothing
        // - the notification is not up but it isn't time yet: don't install
        // - the notification is not up and it is time: need to install

        if (mSyncNotificationInfo->isActive) {
            shouldInstall = shouldCancel = FALSE;
        }
        else {
            // it isn't currently up, so there is nothing to cancel
            shouldCancel = FALSE;

            Boolean timeToShowNotification =
                    now > mSyncNotificationInfo->startTime + SYNC_NOTIFICATION_DELAY;
            if (timeToShowNotification) {
                shouldInstall = TRUE;
            }
            else {
                // show the notification immediately if this is a manual sync
                shouldInstall = FALSE;
                AutoPtr<CSyncManager::ActiveSyncContext> activeSyncContext;
                List<AutoPtr<CSyncManager::ActiveSyncContext> >::Iterator it = mSyncMgr->mActiveSyncContexts->Begin();
                for (; it != mSyncMgr->mActiveSyncContexts->End(); it++) {
                    activeSyncContext = *it;
                    AutoPtr<IBundle> extras;
                    FAIL_RETURN(activeSyncContext->mSyncOperation->GetExtras((IBundle**)&extras))
                    Boolean manualSync = FALSE;
                    FAIL_RETURN(extras->GetBoolean(IContentResolver::SYNC_EXTRAS_MANUAL, FALSE, &manualSync))
                    if (manualSync) {
                        shouldInstall = TRUE;
                        break;
                    }
                }
            }
        }
    }

    if (shouldCancel && !shouldInstall) {
        mSyncMgr->mNeedSyncActiveNotification = FALSE;
        FAIL_RETURN(SendSyncStateIntent())
        mSyncNotificationInfo->isActive = FALSE;
    }

    if (shouldInstall) {
        mSyncMgr->mNeedSyncActiveNotification = TRUE;
        FAIL_RETURN(SendSyncStateIntent())
        mSyncNotificationInfo->isActive = TRUE;
    }
    return NOERROR;
}

ECode CSyncManager::SyncHandler::ManageSyncAlarmLocked(
    /* [in] */ Int64 nextPeriodicEventElapsedTime,
    /* [in] */ Int64 nextPendingEventElapsedTime)
{
    // in each of these cases the sync loop will be kicked, which will cause this
    // method to be called again
    if (!mSyncMgr->mDataConnectionIsConnected) return NOERROR;
    if (mSyncMgr->mStorageIsLow) return NOERROR;

    // When the status bar notification should be raised
    Int64 notificationTime =
            (!mSyncMgr->mSyncHandler->mSyncNotificationInfo->isActive
                    && mSyncMgr->mSyncHandler->mSyncNotificationInfo->startTime != 0)
                    ? mSyncMgr->mSyncHandler->mSyncNotificationInfo->startTime + SYNC_NOTIFICATION_DELAY
                    : 0x7FFFFFFFFFFFFFFFL /* Long.MAX_VALUE */;

    // When we should consider canceling an active sync
    Int64 earliestTimeoutTime = 0x7FFFFFFFFFFFFFFFL; // Long.MAX_VALUE
    AutoPtr<CSyncManager::ActiveSyncContext> currentSyncContext;
    List<AutoPtr<CSyncManager::ActiveSyncContext> >::Iterator it = mSyncMgr->mActiveSyncContexts->Begin();
    for (; it != mSyncMgr->mActiveSyncContexts->End(); it++) {
        currentSyncContext = *it;
        Int64 currentSyncTimeoutTime =
                currentSyncContext->mTimeoutStartTime + MAX_TIME_PER_SYNC;
//        if (Log.isLoggable(TAG, Log.VERBOSE)) {
//            Log.v(TAG, "manageSyncAlarm: active sync, mTimeoutStartTime + MAX is "
//                    + currentSyncTimeoutTime);
//        }
        if (earliestTimeoutTime > currentSyncTimeoutTime) {
            earliestTimeoutTime = currentSyncTimeoutTime;
        }
    }

//    if (Log.isLoggable(TAG, Log.VERBOSE)) {
//        Log.v(TAG, "manageSyncAlarm: notificationTime is " + notificationTime);
//    }

//    if (Log.isLoggable(TAG, Log.VERBOSE)) {
//        Log.v(TAG, "manageSyncAlarm: earliestTimeoutTime is " + earliestTimeoutTime);
//    }

//    if (Log.isLoggable(TAG, Log.VERBOSE)) {
//        Log.v(TAG, "manageSyncAlarm: nextPeriodicEventElapsedTime is "
//                + nextPeriodicEventElapsedTime);
//    }
//    if (Log.isLoggable(TAG, Log.VERBOSE)) {
//        Log.v(TAG, "manageSyncAlarm: nextPendingEventElapsedTime is "
//                + nextPendingEventElapsedTime);
//    }

    // Math.min(notificationTime, earliestTimeoutTime);
    Int64 alarmTime = notificationTime < earliestTimeoutTime ? notificationTime : earliestTimeoutTime;
    alarmTime = alarmTime < nextPeriodicEventElapsedTime ? alarmTime : nextPeriodicEventElapsedTime;
    alarmTime = alarmTime < nextPendingEventElapsedTime ? alarmTime : nextPendingEventElapsedTime;

    // Bound the alarm time.
    Int64 now = 0;
//    now = SystemClock.elapsedRealtime();
    if (alarmTime < now + SYNC_ALARM_TIMEOUT_MIN) {
//        if (Log.isLoggable(TAG, Log.VERBOSE)) {
//            Log.v(TAG, "manageSyncAlarm: the alarmTime is too small, "
//                    + alarmTime + ", setting to " + (now + SYNC_ALARM_TIMEOUT_MIN));
//        }
        alarmTime = now + SYNC_ALARM_TIMEOUT_MIN;
    }
    else if (alarmTime > now + SYNC_ALARM_TIMEOUT_MAX) {
//        if (Log.isLoggable(TAG, Log.VERBOSE)) {
//            Log.v(TAG, "manageSyncAlarm: the alarmTime is too large, "
//                    + alarmTime + ", setting to " + (now + SYNC_ALARM_TIMEOUT_MIN));
//        }
        alarmTime = now + SYNC_ALARM_TIMEOUT_MAX;
    }

    // determine if we need to set or cancel the alarm
    Boolean shouldSet = FALSE;
    Boolean shouldCancel = FALSE;
    Boolean alarmIsActive = mAlarmScheduleTime != 0;
    Boolean needAlarm = alarmTime != 0x7FFFFFFFFFFFFFFFL; // Long.MAX_VALUE;
    if (needAlarm) {
        if (!alarmIsActive || alarmTime < mAlarmScheduleTime) {
            shouldSet = TRUE;
        }
    }
    else {
        shouldCancel = alarmIsActive;
    }

    // set or cancel the alarm as directed
    FAIL_RETURN(mSyncMgr->EnsureAlarmService())
    if (shouldSet) {
//        if (Log.isLoggable(TAG, Log.VERBOSE)) {
//            Log.v(TAG, "requesting that the alarm manager wake us up at elapsed time "
//                    + alarmTime + ", now is " + now + ", " + ((alarmTime - now) / 1000)
//                    + " secs from now");
//        }
        mAlarmScheduleTime = alarmTime;
        FAIL_RETURN(mSyncMgr->mAlarmService->Set(IAlarmManager::ELAPSED_REALTIME_WAKEUP, alarmTime,
                mSyncMgr->mSyncAlarmIntent))
    }
    else if (shouldCancel) {
        mAlarmScheduleTime = 0;
        FAIL_RETURN(mSyncMgr->mAlarmService->Cancel(mSyncMgr->mSyncAlarmIntent))
    }
    return NOERROR;
}

ECode CSyncManager::SyncHandler::SendSyncStateIntent()
{
    AutoPtr<IIntent> syncStateIntent;
    FAIL_RETURN(CIntent::New(IIntent::ACTION_SYNC_STATE_CHANGED, (IIntent**)&syncStateIntent))
    FAIL_RETURN(syncStateIntent->AddFlags(IIntent::FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT))
    FAIL_RETURN(syncStateIntent->PutBooleanExtra(String("active"), mSyncMgr->mNeedSyncActiveNotification))
    FAIL_RETURN(syncStateIntent->PutBooleanExtra(String("failing"), FALSE))
    AutoPtr<IUserHandle> userHandle;
//***    FAIL_RETURN(CUserHandle::New(IUserHandle::OWNER, (IUserHandle**)&userHandle))
    FAIL_RETURN(mSyncMgr->mContext->SendBroadcastAsUser(syncStateIntent, userHandle))
    return NOERROR;
}

ECode CSyncManager::SyncHandler::InstallHandleTooManyDeletesNotification(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Int64 numDeletes,
    /* [in] */ Int32 userId)
{
    if (NULL == mSyncMgr->mNotificationMgr) return NOERROR;

    AutoPtr<IPackageManager> pkgManager;
    FAIL_RETURN(mSyncMgr->mContext->GetPackageManager((IPackageManager**)&pkgManager))
    AutoPtr<IProviderInfo> providerInfo;
    FAIL_RETURN(pkgManager->ResolveContentProvider(authority, 0 /* flags */, (IProviderInfo**)&providerInfo))
    if (NULL == providerInfo) {
        return NOERROR;
    }

    AutoPtr<ICharSequence> authorityName;
    FAIL_RETURN(providerInfo->LoadLabel(pkgManager, (ICharSequence**)&authorityName))

    AutoPtr<IIntent> clickIntent;
//    CIntent::New(mSyncMgr->mContext, SyncActivityTooManyDeletes.class);
    AutoPtr<IParcelable> parcelable = (IParcelable*) account->Probe(EIID_IParcelable);
    FAIL_RETURN(clickIntent->PutParcelableExtra(String("account"), parcelable))
    FAIL_RETURN(clickIntent->PutStringExtra(String("authority"), authority))
    String str;
    FAIL_RETURN(authorityName->ToString(&str))
    FAIL_RETURN(clickIntent->PutStringExtra(String("provider"), str))
    FAIL_RETURN(clickIntent->PutInt64Extra(String("numDeletes"), numDeletes))

    Boolean ret = FALSE;
    FAIL_RETURN(IsActivityAvailable(clickIntent, &ret))
    if (!ret) {
//        Log.w(TAG, "No activity found to handle too many deletes.");
        return NOERROR;
    }

    AutoPtr<IPendingIntent> pendingIntent;
    AutoPtr<IUserHandle> userHandle;
//***    FAIL_RETURN(CUserHandle::New(userId, (IUserHandle**)&userHandle))
    AutoPtr<IPendingIntentHelper> pendingHelper;
//***    FAIL_RETURN(CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&pendingHelper))
    FAIL_RETURN(pendingHelper->GetActivityAsUser(mSyncMgr->mContext, 0, clickIntent, IPendingIntent::FLAG_CANCEL_CURRENT,
            (IBundle*) NULL, userHandle, (IPendingIntent**)&pendingIntent))
    AutoPtr<ICharSequence> tooManyDeletesDescFormat;
    AutoPtr<IResources> res;
    FAIL_RETURN(mSyncMgr->mContext->GetResources((IResources**)&res))
    FAIL_RETURN(res->GetText(0x010400d1 /* R.string.contentServiceTooManyDeletesNotificationDesc */,
            (ICharSequence**)&tooManyDeletesDescFormat))

    AutoPtr<INotification> notification;
//    notification = new Notification(R.drawable.stat_notify_sync_error,
//                mContext.getString(R.string.contentServiceSync),
//                System.currentTimeMillis());
    AutoPtr<ICharSequence> contentTitle;
    AutoPtr<ICharSequence> contentText;
    FAIL_RETURN(mSyncMgr->mContext->GetString(0x010400d0 /* R.string.contentServiceSyncNotificationTitle */, &str))
    FAIL_RETURN(CStringWrapper::New(str, (ICharSequence**)&contentTitle))
//    contentText = String.format(tooManyDeletesDescFormat.toString(), authorityName)
    FAIL_RETURN(notification->SetLatestEventInfo(mSyncMgr->mContext,
            contentTitle, contentText, pendingIntent))
    Int32 flags = 0;
    FAIL_RETURN(notification->GetFlags(&flags))
    flags |= INotification::FLAG_ONGOING_EVENT;
    FAIL_RETURN(notification->SetFlags(flags))
    Int32 id = 0;
//    id = account.hashCode() ^ authority.hashCode();
    FAIL_RETURN(mSyncMgr->mNotificationMgr->NotifyAsUser(String(NULL), id, notification, userHandle))
    return NOERROR;
}

ECode CSyncManager::SyncHandler::IsActivityAvailable(
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* isAvailable)
{
    VALIDATE_NOT_NULL(isAvailable)
    AutoPtr<IPackageManager> pkgManager;
    FAIL_RETURN(mSyncMgr->mContext->GetPackageManager((IPackageManager**)&pkgManager))

    AutoPtr<IObjectContainer> objContainer;
    AutoPtr<IObjectEnumerator> objEnumerator;
    FAIL_RETURN(pkgManager->QueryIntentActivities(intent, 0, (IObjectContainer**)&objContainer))

    if (NULL != objContainer) {
        FAIL_RETURN(objContainer->GetObjectEnumerator((IObjectEnumerator**)&objEnumerator))
        Boolean hasNext = FALSE;
        AutoPtr<IResolveInfo> resolveInfo;
        AutoPtr<IActivityInfo> activityInfo;
        AutoPtr<IApplicationInfo> applicationInfo;
        Int32 flags = 0;

        while ((objEnumerator->MoveNext(&hasNext), hasNext)) {
            resolveInfo = NULL;
            activityInfo = NULL;
            applicationInfo = NULL;
            FAIL_RETURN(objEnumerator->Current((IInterface**)&resolveInfo))
            FAIL_RETURN(resolveInfo->GetActivityInfo((IActivityInfo**)&activityInfo))
            FAIL_RETURN(activityInfo->GetApplicationInfo((IApplicationInfo**)&applicationInfo))
            FAIL_RETURN(applicationInfo->GetFlags(&flags))
            if ((flags & IApplicationInfo::FLAG_SYSTEM) != 0) {
                *isAvailable = TRUE;
                return NOERROR;
            }
        }

    }

    *isAvailable = FALSE;
    return NOERROR;
}

CSyncManager::AuthoritySyncStats::AuthoritySyncStats(
    /* [in] */ const String& name)
    : name(name)
    , elapsedTime(0)
    , times(0)
{
}

CSyncManager::AuthoritySyncStats::~AuthoritySyncStats()
{
}

CAR_INTERFACE_IMPL(CSyncManager::AuthoritySyncStats, IInterface)

CSyncManager::AccountSyncStats::AccountSyncStats(
    /* [in] */ const String& name)
    : name(name)
    , elapsedTime(0)
    , times(0)
{}

CSyncManager::AccountSyncStats::~AccountSyncStats()
{}

CAR_INTERFACE_IMPL(CSyncManager::AccountSyncStats, IInterface)

CSyncManager::SyncTimeTracker::SyncTimeTracker(
    /* [in] */ CSyncManager* syncMgr)
    : mLastWasSyncing(FALSE)
    , mWhenSyncStarted(0)
    , mTimeSpentSyncing(0)
    , mSyncMgr(syncMgr)
{}

CSyncManager::SyncTimeTracker::~SyncTimeTracker()
{}

CAR_INTERFACE_IMPL(CSyncManager::SyncTimeTracker, IInterface)

ECode CSyncManager::SyncTimeTracker::Update()
{
    AutoLock lock(mSyncTimeTrackerLock);
    Boolean isSyncInProgress = !mSyncMgr->mActiveSyncContexts->IsEmpty();

    if (isSyncInProgress == mLastWasSyncing) return NOERROR;
    Int64 now = 0;
//    now = SystemClock.elapsedRealtime();
    if (isSyncInProgress) {
        mWhenSyncStarted = now;
    }
    else {
        mTimeSpentSyncing += now - mWhenSyncStarted;
    }
    mLastWasSyncing = isSyncInProgress;
    return NOERROR;
}

ECode CSyncManager::SyncTimeTracker::TimeSpentSyncing(
    /* [out] */ Int64* syncTime)
{
    VALIDATE_NOT_NULL(syncTime)
    AutoLock lock(mSyncTimeTrackerLock);
    if (!mLastWasSyncing) {
        *syncTime = mTimeSpentSyncing;
        return NOERROR;
    }

    Int64 now = 0;
//    now = SystemClock.elapsedRealtime();
    *syncTime = mTimeSpentSyncing + (now - mWhenSyncStarted);
    return NOERROR;
}

CSyncManager::StorageIntentReceiver::StorageIntentReceiver(
    /* [in] */ CSyncManager* syncMgr)
    : mSyncMgr(syncMgr)
{}

CSyncManager::StorageIntentReceiver::~StorageIntentReceiver()
{}

ECode CSyncManager::StorageIntentReceiver::Initialize()
{
    return NOERROR;
}

ECode CSyncManager::StorageIntentReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(intent)
    String action;
    FAIL_RETURN(intent->GetAction(&action));

    if (IIntent::ACTION_DEVICE_STORAGE_LOW.Equals(action)) {
//***        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
//***            Logger::V(TAG, String("Internal storage is low."));
//***        }
        mSyncMgr->mStorageIsLow = TRUE;
        FAIL_RETURN(mSyncMgr->CancelActiveSync((IAccount*) NULL /* any account */, IUserHandle::USER_ALL,
                String(NULL) /* any authority */));
    }
    else if (IIntent::ACTION_DEVICE_STORAGE_OK.Equals(action)) {
//***        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
//***            Logger::V(TAG, String("Internal storage is ok."));
//***        }
        mSyncMgr->mStorageIsLow = FALSE;
        FAIL_RETURN(mSyncMgr->SendCheckAlarmsMessage());
    }

    return NOERROR;
}

CSyncManager::BootCompletedReceiver::BootCompletedReceiver(
    /* [in] */ CSyncManager* syncMgr)
    : mSyncMgr(syncMgr)
{}

CSyncManager::BootCompletedReceiver::~BootCompletedReceiver()
{}

ECode CSyncManager::BootCompletedReceiver::Initialize()
{
    return NOERROR;
}

ECode CSyncManager::BootCompletedReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return mSyncMgr->mSyncHandler->OnBootCompleted();
}

CSyncManager::BackgroundDataSettingChanged::BackgroundDataSettingChanged(
    /* [in] */ CSyncManager* syncMgr)
    : mSyncMgr(syncMgr)
{}

CSyncManager::BackgroundDataSettingChanged::~BackgroundDataSettingChanged()
{}

ECode CSyncManager::BackgroundDataSettingChanged::Initialize()
{
    return NOERROR;
}

ECode CSyncManager::BackgroundDataSettingChanged::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    AutoPtr<IConnectivityManager> connManager;
    FAIL_RETURN(mSyncMgr->GetConnectivityManager((IConnectivityManager**)&connManager));
    Boolean ret = FALSE;
    FAIL_RETURN(connManager->GetBackgroundDataSetting(&ret));
    if (ret) {
        AutoPtr<IBundle> bundle;
//***        FAIL_RETURN(CBundle::New((IBundle**)&bundle));
        FAIL_RETURN(mSyncMgr->ScheduleSync((IAccount*) NULL /* account */, IUserHandle::USER_ALL, String(NULL) /* authority */,
                bundle, 0 /* delay */, FALSE /* onlyThoseWithUnknownSyncableState */));
    }
    return NOERROR;
}

CSyncManager::AccountsUpdatedReceiver::AccountsUpdatedReceiver(
    /* [in] */ CSyncManager* syncMgr)
    : mSyncMgr(syncMgr)
{}

CSyncManager::AccountsUpdatedReceiver::~AccountsUpdatedReceiver()
{}

ECode CSyncManager::AccountsUpdatedReceiver::Initialize()
{
    return NOERROR;
}

ECode CSyncManager::AccountsUpdatedReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    FAIL_RETURN(mSyncMgr->UpdateRunningAccounts());
    // Kick off sync for everyone, since this was a radical account change
    return mSyncMgr->ScheduleSync((IAccount*) NULL, IUserHandle::USER_ALL, String(NULL), (IBundle*) NULL, 0 /* no delay */, FALSE);
}

CSyncManager::ConnectivityIntentReceiver::ConnectivityIntentReceiver(
    /* [in] */ CSyncManager* syncMgr)
    : mSyncMgr(syncMgr)
{}

CSyncManager::ConnectivityIntentReceiver::~ConnectivityIntentReceiver()
{}

ECode CSyncManager::ConnectivityIntentReceiver::Initialize()
{
    return NOERROR;
}

ECode CSyncManager::ConnectivityIntentReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    Boolean wasConnected = mSyncMgr->mDataConnectionIsConnected;
    // don't use the intent to figure out if network is connected, just check
    // ConnectivityManager directly.
    FAIL_RETURN(mSyncMgr->ReadDataConnectionState(&(mSyncMgr->mDataConnectionIsConnected)));
    if (mSyncMgr->mDataConnectionIsConnected) {
        if (!wasConnected) {
//***            if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
//***                Logger::V(TAG, String("Reconnection detected: clearing all backoffs"));
//***            }
            FAIL_RETURN(mSyncMgr->mSyncStorageEngine->ClearAllBackoffs(mSyncMgr->mSyncQueue));
        }
        FAIL_RETURN(mSyncMgr->SendCheckAlarmsMessage());
    }
    return NOERROR;
}

CSyncManager::ShutdownIntentReceiver::ShutdownIntentReceiver(
    /* [in] */ CSyncManager* syncMgr)
    : mSyncMgr(syncMgr)
{}

CSyncManager::ShutdownIntentReceiver::~ShutdownIntentReceiver()
{}

ECode CSyncManager::ShutdownIntentReceiver::Initialize()
{
    return NOERROR;
}

ECode CSyncManager::ShutdownIntentReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
//***    Logger::W(TAG, String("Writing sync state before shutdown..."));
    AutoPtr<ISyncStorageEngine> syncEngine;
    FAIL_RETURN(mSyncMgr->GetSyncStorageEngine((ISyncStorageEngine**)&syncEngine));
    return syncEngine->WriteAllState();
}

CSyncManager::UserIntentReceiver::UserIntentReceiver(
    /* [in] */ CSyncManager* syncMgr)
    : mSyncMgr(syncMgr)
{}

CSyncManager::UserIntentReceiver::~UserIntentReceiver()
{}

ECode CSyncManager::UserIntentReceiver::Initialize()
{
    return NOERROR;
}

ECode CSyncManager::UserIntentReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(intent)
    String action;
    FAIL_RETURN(intent->GetAction(&action));
    Int32 userId = 0;
    FAIL_RETURN(intent->GetInt32Extra(IIntent::EXTRA_USER_HANDLE, IUserHandle::USER_NULL, &userId));
    if (userId == IUserHandle::USER_NULL) return NOERROR;

    if (IIntent::ACTION_USER_REMOVED.Equals(action)) {
        FAIL_RETURN(mSyncMgr->OnUserRemoved(userId));
    }
    else if (IIntent::ACTION_USER_STARTING.Equals(action)) {
        FAIL_RETURN(mSyncMgr->OnUserStarting(userId));
    }
    else if (IIntent::ACTION_USER_STOPPING.Equals(action)) {
        FAIL_RETURN(mSyncMgr->OnUserStopping(userId));
    }

    return NOERROR;
}

CSyncManager::SyncStatusObserver::SyncStatusObserver(
    /* [in] */ CSyncManager* syncMgr)
    : mSyncMgr(syncMgr)
{}

CSyncManager::SyncStatusObserver::~SyncStatusObserver()
{}

CAR_INTERFACE_IMPL(CSyncManager::SyncStatusObserver, IISyncStatusObserver)

ECode CSyncManager::SyncStatusObserver::OnStatusChanged(
    /* [in] */ Int32 which)
{
    // force the sync loop to run if the settings change
    return mSyncMgr->SendCheckAlarmsMessage();
}

CSyncManager::SyncRequestListener::SyncRequestListener(
    /* [in] */ CSyncManager* syncMgr)
    : mSyncMgr(syncMgr)
{}

CSyncManager::SyncRequestListener::~SyncRequestListener()
{}

CAR_INTERFACE_IMPL(CSyncManager::SyncRequestListener, ISyncStorageEngineOnSyncRequestListener)

ECode CSyncManager::SyncRequestListener::OnSyncRequest(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras)
{
    return mSyncMgr->ScheduleSync(account, userId, authority, extras, 0, FALSE);
}

CSyncManager::CSyncManager()
    : mActiveSyncContexts(NULL)
    , mSyncAdapters(NULL)
    , mContext(NULL)
    , mRunningAccounts(INITIAL_ACCOUNTS_ARRAY)
    , mHandleAlarmWakeLock(NULL)
    , mSyncManagerWakeLock(NULL)
    , mDataConnectionIsConnected(FALSE)
    , mStorageIsLow(FALSE)
    , mNotificationMgr(NULL)
    , mAlarmService(NULL)
    , mSyncStorageEngine(NULL)
    , mSyncQueue(NULL)
    , mNeedSyncActiveNotification(FALSE)
    , mSyncAlarmIntent(NULL)
    , mConnManagerDoNotUseDirectly(NULL)
    , mStorageIntentReceiver(NULL)
    , mBootCompletedReceiver(NULL)
    , mBackgroundDataSettingChanged(NULL)
    , mAccountsUpdatedReceiver(NULL)
    , mPowerManager(NULL)
    , mSyncRandomOffsetMillis(0)
    , mUserManager(NULL)
    , mConnectivityIntentReceiver(NULL)
    , mShutdownIntentReceiver(NULL)
    , mUserIntentReceiver(NULL)
    , mSyncHandler(NULL)
    , mBootCompleted(FALSE)
{
    mActiveSyncContexts = new List<AutoPtr<ActiveSyncContext> >();
}

CSyncManager::~CSyncManager()
{
}

ECode CSyncManager::UpdateRunningAccounts()
{
    mRunningAccounts = NULL;
//    mRunningAccounts = AccountManagerService.getSingleton().getRunningAccounts();

    if (mBootCompleted) {
        FAIL_RETURN(DoDatabaseCleanup())
    }

    AutoPtr<ActiveSyncContext> currentSyncContext;
    List<AutoPtr<ActiveSyncContext> >::Iterator it;
    for (it = mActiveSyncContexts->Begin(); it != mActiveSyncContexts->End(); it++) {
        currentSyncContext = *it;
        AutoPtr<ISyncOperation> syncOperation = currentSyncContext->mSyncOperation;
        AutoPtr<IAccount> account;
        FAIL_RETURN(syncOperation->GetAccount((IAccount**)&account))
        Int32 userId = 0;
        FAIL_RETURN(syncOperation->GetUserId(&userId))
        Boolean isContain = FALSE;
        FAIL_RETURN(ContainsAccountAndUser(mRunningAccounts, account, userId, &isContain))
        if (!isContain) {
//***            Logger::D(TAG, "canceling sync since the account is no longer running");
            FAIL_RETURN(SendSyncFinishedOrCanceledMessage(currentSyncContext, (ISyncResult*) NULL /* no result since this is a cancel */));
        }
    }

    // we must do this since we don't bother scheduling alarms when
    // the accounts are not set yet
    FAIL_RETURN(SendCheckAlarmsMessage())
    return NOERROR;
}

ECode CSyncManager::GetSyncStorageEngine(
    /* [out] */ ISyncStorageEngine** engine)
{
    VALIDATE_NOT_NULL(engine)
    *engine = mSyncStorageEngine;
    REFCOUNT_ADD(*engine);
    return NOERROR;
}

ECode CSyncManager::ScheduleSync(
    /* [in] */ IAccount* requestedAccount,
    /* [in] */ Int32 userId,
    /* [in] */ const String& requestedAuthority,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 delay,
    /* [in] */ Boolean onlyThoseWithUnkownSyncableState)
{
//***    Boolean isLoggable = Logger::IsLoggable(TAG, Logger::VERBOSE);
    AutoPtr<IConnectivityManager> connManager;
    FAIL_RETURN(GetConnectivityManager((IConnectivityManager**)&connManager))
    Boolean ret = FALSE;
    FAIL_RETURN(connManager->GetBackgroundDataSetting(&ret))
    Boolean backgroundDataUsageAllowed = !mBootCompleted || ret;

    if (NULL == extras) {
//***        FAIL_RETURN(CBundle::New(&extras))
    }

    Boolean expedited = FALSE;
    FAIL_RETURN(extras->GetBoolean(IContentResolver::SYNC_EXTRAS_EXPEDITED, FALSE, &expedited))
    if (expedited) {
        delay = -1; // this means schedule at the front of the queue
    }

    AutoPtr<ArrayOf<IAccountAndUser *> > accounts;
    if (NULL != requestedAccount && userId != IUserHandle::USER_ALL) {
        accounts = ArrayOf<IAccountAndUser *>::Alloc(1);
        AutoPtr<IAccountAndUser> accountUser;
//***         FAIL_RETURN(CAccountAndUser::New(requestedAccount, userId, (IAccountAndUser**)&accountUser))
        accounts->Set(0, accountUser);
    }
    else {
        // if the accounts aren't configured yet then we can't support an account-less
        // sync request
        accounts = mRunningAccounts;
        if (accounts->GetLength() == 0) {
//***            if (isLoggable) {
//***                Logger::V(TAG, String("scheduleSync: no accounts configured, dropping"));
//***            }
            return NOERROR;
        }
    }

    Boolean uploadOnly = FALSE;
    FAIL_RETURN(extras->GetBoolean(IContentResolver::SYNC_EXTRAS_UPLOAD, FALSE, &uploadOnly))
    Boolean manualSync = FALSE;
    FAIL_RETURN(extras->GetBoolean(IContentResolver::SYNC_EXTRAS_MANUAL, FALSE, &manualSync))
    if (manualSync) {
        FAIL_RETURN(extras->PutBoolean(IContentResolver::SYNC_EXTRAS_IGNORE_BACKOFF, TRUE))
        FAIL_RETURN(extras->PutBoolean(IContentResolver::SYNC_EXTRAS_IGNORE_SETTINGS, TRUE))
    }
    Boolean ignoreSettings = FALSE;
    FAIL_RETURN(extras->GetBoolean(IContentResolver::SYNC_EXTRAS_IGNORE_SETTINGS, FALSE, &ignoreSettings))

    Int32 source = 0;
    if (uploadOnly) {
        source = ISyncStorageEngine::SOURCE_LOCAL;
    }
    else if (manualSync) {
        source = ISyncStorageEngine::SOURCE_USER;
    }
    else if (requestedAuthority.IsNull()) {
        source = ISyncStorageEngine::SOURCE_POLL;
    }
    else {
        // this isn't strictly server, since arbitrary callers can (and do) request
        // a non-forced two-way sync on a specific url
        source = ISyncStorageEngine::SOURCE_SERVER;
    }

    const Int32 N = accounts->GetLength();
    AutoPtr<IAccountAndUser> account;

    for (Int32 i = 0; i< N; i++) {
        account = (*accounts)[i];
        // Compile a list of authorities that have sync adapters.
        // For each authority sync each account that matches a sync adapter.
        AutoPtr<HashSet<String> > syncableAuthorities = new HashSet<String>();
//        for (RegisteredServicesCache.ServiceInfo<SyncAdapterType> syncAdapter :
//                mSyncAdapters.getAllServices(account.userId)) {
//            syncableAuthorities.add(syncAdapter.type.authority);
//        }

        // if the url was specified then replace the list of authorities
        // with just this authority or clear it if this authority isn't
        // syncable
        if (!requestedAuthority.IsNull()) {
            HashSet<String>::Iterator it = syncableAuthorities->Find(requestedAuthority);
            Boolean hasSyncAdapter = it != syncableAuthorities->End();
            syncableAuthorities->Clear();
            if (hasSyncAdapter) syncableAuthorities->Insert(requestedAuthority);
        }

        HashSet<String>::Iterator it = syncableAuthorities->Begin();
        for (; it != syncableAuthorities->End(); it++) {
            String authority = *it;
            Int32 isSyncable = 0;
            AutoPtr<IAccount> act;
            Int32 uId = 0;
            FAIL_RETURN(account->GetAccount((IAccount**)&act))
            FAIL_RETURN(account->GetUserId(&uId))
            FAIL_RETURN(mSyncStorageEngine->GetIsSyncable(act, uId, authority, &isSyncable))
            if (isSyncable == 0) {
                continue;
            }

//            final RegisteredServicesCache.ServiceInfo<SyncAdapterType> syncAdapterInfo;
//            syncAdapterInfo = mSyncAdapters.getServiceInfo(
//                    SyncAdapterType.newKey(authority, account.account.type), account.userId);
            AutoPtr<RegisteredServicesCache::ServiceInfo> syncAdapterInfo;
            if (NULL == syncAdapterInfo) {
                continue;
            }
            Boolean allowParallelSyncs = FALSE;
//            syncAdapterInfo.type.allowParallelSyncs();
            Boolean isAlwaysSyncable = FALSE;
//            syncAdapterInfo.type.isAlwaysSyncable();
            if (isSyncable < 0 && isAlwaysSyncable) {
                FAIL_RETURN(mSyncStorageEngine->SetIsSyncable(act, uId, authority, 1))
                isSyncable = 1;
            }
            if (onlyThoseWithUnkownSyncableState && isSyncable >= 0) {
                continue;
            }
//            if (!syncAdapterInfo.type.supportsUploading() && uploadOnly) {
//                continue;
//            }

            Boolean result = FALSE;
            // always allow if the isSyncable state is unknown
            const Boolean syncAllowed =
                    (isSyncable < 0)
                    || ignoreSettings
                    || (backgroundDataUsageAllowed
                            && (mSyncStorageEngine->GetMasterSyncAutomatically(uId, &result), result)
                            && (mSyncStorageEngine->GetSyncAutomatically(act, uId, authority, &result), result));
            if (!syncAllowed) {
//***                if (isLoggable) {
//                    Log.d(TAG, "scheduleSync: sync of " + account + ", " + authority
//                            + " is not allowed, dropping request");
//***                }
                continue;
            }

            AutoPtr<IPair> backoff; // Pair<Long, Long>
            FAIL_RETURN(mSyncStorageEngine->GetBackoff(act, uId, authority, (IPair**)&backoff))
            Int64 delayUntil = 0;
            FAIL_RETURN(mSyncStorageEngine->GetDelayUntilTime(act, uId, authority, &delayUntil))
            Int64 backoffTime = 0;
            if (NULL != backoff) {
                AutoPtr<IInteger64> first;
                FAIL_RETURN(backoff->GetFirst((IInterface**) (IInteger64**)&first))
                FAIL_RETURN(first->GetValue(&backoffTime))
            }
            if (isSyncable < 0) {
                AutoPtr<IBundle> newExtras;
//***                FAIL_RETURN(CBundle::New((IBundle**)&newExtras))
                FAIL_RETURN(newExtras->PutBoolean(IContentResolver::SYNC_EXTRAS_INITIALIZE, TRUE))
//                if (isLoggable) {
//                    Log.v(TAG, "scheduleSync:"
//                            + " delay " + delay
//                            + ", source " + source
//                            + ", account " + account
//                            + ", authority " + authority
//                            + ", extras " + newExtras);
//                }
                AutoPtr<ISyncOperation> syncOperation;
                FAIL_RETURN(CSyncOperation::New(act, uId, source, authority, newExtras, 0, backoffTime, delayUntil,
                        allowParallelSyncs, (ISyncOperation**)&syncOperation))
                FAIL_RETURN(ScheduleSyncOperation(syncOperation))
            }
            if (!onlyThoseWithUnkownSyncableState) {
//                if (isLoggable) {
//                    Log.v(TAG, "scheduleSync:"
//                            + " delay " + delay
//                            + ", source " + source
//                            + ", account " + account
//                            + ", authority " + authority
//                            + ", extras " + extras);
//                }
                AutoPtr<ISyncOperation> syncOperation;
                FAIL_RETURN(CSyncOperation::New(act, uId, source, authority, extras, delay, backoffTime, delayUntil,
                        allowParallelSyncs, (ISyncOperation**)&syncOperation))
                FAIL_RETURN(ScheduleSyncOperation(syncOperation))
            }
        }
    }
    return NOERROR;
}

ECode CSyncManager::ScheduleLocalSync(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authority)
{
    AutoPtr<IBundle> extras;
//***    FAIL_RETURN(CBundle::New((IBundle**)&extras))
    FAIL_RETURN(extras->PutBoolean(IContentResolver::SYNC_EXTRAS_UPLOAD, TRUE))
    return ScheduleSync(account, userId, authority, extras, LOCAL_SYNC_DELAY, FALSE /* onlyThoseWithUnkownSyncableState */);
}

ECode CSyncManager::GetSyncAdapterTypes(
    /* [in] */ Int32 userId,
    /* [out, callee] */ ArrayOf<ISyncAdapterType *>** syncAdapterTypes)
{
    VALIDATE_NOT_NULL(syncAdapterTypes)
    //final Collection<RegisteredServicesCache.ServiceInfo<SyncAdapterType>> serviceInfos;
    AutoPtr<ArrayOf<RegisteredServicesCache::ServiceInfo*> > serviceInfos;
//    serviceInfos = mSyncAdapters.getAllServices(userId);
    const Int32 N = serviceInfos->GetLength();
    AutoPtr<ArrayOf<ISyncAdapterType *> > types = ArrayOf<ISyncAdapterType *>::Alloc(N);
    AutoPtr<RegisteredServicesCache::ServiceInfo> seviceInfo;
    for (Int32 i = 0; i < N; i++) {
        seviceInfo = (*serviceInfos)[i];
        AutoPtr<ISyncAdapterType> type = ISyncAdapterType::Probe(seviceInfo->mType);
        types->Set(i, type.Get());
    }
    *syncAdapterTypes = types;
    REFCOUNT_ADD(*syncAdapterTypes);
    return NOERROR;
}

ECode CSyncManager::CancelActiveSync(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authority)
{
    return SendCancelSyncsMessage(account, userId, authority);
}

ECode CSyncManager::ScheduleSyncOperation(
    /* [in] */ ISyncOperation* syncOperation)
{
    Boolean queueChanged = FALSE;
    AutoLock lock(mSyncQueueLock);
    FAIL_RETURN(mSyncQueue->Add(syncOperation, &queueChanged))

    if (queueChanged) {
//***        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
//            Log.v(TAG, "scheduleSyncOperation: enqueued " + syncOperation);
//***        }
        FAIL_RETURN(SendCheckAlarmsMessage())
    }
    else {
//***        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
//            Log.v(TAG, "scheduleSyncOperation: dropping duplicate sync operation "
//                    + syncOperation);
//***        }
    }
    return NOERROR;
}

ECode CSyncManager::ClearScheduledSyncOperations(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authority)
{
    AutoLock lock(mSyncQueueLock);
    FAIL_RETURN(mSyncQueue->Remove(account, userId, authority))
    FAIL_RETURN(mSyncStorageEngine->SetBackoff(account, userId, authority,
            ISyncStorageEngine::NOT_IN_BACKOFF_MODE, ISyncStorageEngine::NOT_IN_BACKOFF_MODE))
    return NOERROR;
}

ECode CSyncManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Boolean factoryTest)
{
    // Initialize the SyncStorageEngine first, before registering observers
    // and creating threads and so on; it may fail if the disk is full.
    mContext = context;

    AutoPtr<ISyncStorageEngineHelper> engineHelper;
    FAIL_RETURN(CSyncStorageEngineHelper::AcquireSingleton((ISyncStorageEngineHelper**)&engineHelper))
    FAIL_RETURN(engineHelper->Init(context))
    FAIL_RETURN(engineHelper->GetSingleton((ISyncStorageEngine**)&mSyncStorageEngine))

    AutoPtr<ISyncStorageEngineOnSyncRequestListener> syncListener = new SyncRequestListener(this);
    FAIL_RETURN(((CSyncStorageEngine*) mSyncStorageEngine.Get())->SetOnSyncRequestListener(syncListener))

    mSyncAdapters = new SyncAdaptersCache(mContext);
    mSyncQueue = new SyncQueue(mSyncStorageEngine, mSyncAdapters);

//    HandlerThread syncThread = new HandlerThread("SyncHandlerThread",
//            Process.THREAD_PRIORITY_BACKGROUND);
//    syncThread.start();
    AutoPtr<ILooper> looper;
//    looper = syncThread.getLooper();
    mSyncHandler = new SyncHandler(looper, this);

//    mSyncAdapters.setListener(new RegisteredServicesCacheListener<SyncAdapterType>() {
//        @Override
//        public void onServiceChanged(SyncAdapterType type, int userId, boolean removed) {
//            if (!removed) {
//                scheduleSync(null, UserHandle.USER_ALL, type.authority, null, 0 /* no delay */,
//                        false /* onlyThoseWithUnkownSyncableState */);
//            }
//        }
//    }, mSyncHandler);

    AutoPtr<IPendingIntentHelper> pendingHelper;
//***    FAIL_RETURN(CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&pendingHelper))
    AutoPtr<IIntent> tmpIntent;
    FAIL_RETURN(CIntent::New(ACTION_SYNC_ALARM, (IIntent**)&tmpIntent))
    FAIL_RETURN(pendingHelper->GetBroadcast(mContext, 0 /* ignored */, tmpIntent, 0, (IPendingIntent**)&mSyncAlarmIntent))

    tmpIntent = NULL;
    AutoPtr<IIntentFilter> intentFilter;
    FAIL_RETURN(CIntentFilter::New(IConnectivityManager::CONNECTIVITY_ACTION, (IIntentFilter**)&intentFilter))
    FAIL_RETURN(context->RegisterReceiver(mConnectivityIntentReceiver, intentFilter, (IIntent**)&tmpIntent))

    if (!factoryTest) {
        intentFilter = NULL;
        tmpIntent = NULL;
        FAIL_RETURN(CIntentFilter::New(IIntent::ACTION_BOOT_COMPLETED, (IIntentFilter**)&intentFilter))
        FAIL_RETURN(context->RegisterReceiver(mBootCompletedReceiver, intentFilter, (IIntent**)&tmpIntent))
    }

    intentFilter = NULL;
    tmpIntent = NULL;
    FAIL_RETURN(CIntentFilter::New(IConnectivityManager::ACTION_BACKGROUND_DATA_SETTING_CHANGED, (IIntentFilter**)&intentFilter))
    FAIL_RETURN(context->RegisterReceiver(mBackgroundDataSettingChanged, intentFilter, (IIntent**)&tmpIntent))

    intentFilter = NULL;
    tmpIntent = NULL;
    FAIL_RETURN(CIntentFilter::New(IIntent::ACTION_DEVICE_STORAGE_LOW, (IIntentFilter**)&intentFilter))
    FAIL_RETURN(intentFilter->AddAction(IIntent::ACTION_DEVICE_STORAGE_OK))
    FAIL_RETURN(context->RegisterReceiver(mStorageIntentReceiver, intentFilter, (IIntent**)&tmpIntent))

    intentFilter = NULL;
    tmpIntent = NULL;
    FAIL_RETURN(CIntentFilter::New(IIntent::ACTION_SHUTDOWN, (IIntentFilter**)&intentFilter))
    FAIL_RETURN(intentFilter->SetPriority(100))
    FAIL_RETURN(context->RegisterReceiver(mShutdownIntentReceiver, intentFilter, (IIntent**)&tmpIntent))

    intentFilter = NULL;
    tmpIntent = NULL;
    FAIL_RETURN(CIntentFilter::New((IIntentFilter**)&intentFilter))
    FAIL_RETURN(intentFilter->AddAction(IIntent::ACTION_USER_REMOVED))
    FAIL_RETURN(intentFilter->AddAction(IIntent::ACTION_USER_STARTING))
    FAIL_RETURN(intentFilter->AddAction(IIntent::ACTION_USER_STOPPING))
    AutoPtr<IUserHandle> userHandle;
//***    FAIL_RETURN(CUserHandle::New(IUserHandle::USER_ALL, (IUserHandle**)&userHandle))
    FAIL_RETURN(mContext->RegisterReceiverAsUser(mUserIntentReceiver, userHandle, intentFilter, String(NULL),
            (IHandler*) NULL, (IIntent**)&tmpIntent))

    if (!factoryTest) {
        FAIL_RETURN(context->GetSystemService(IContext::NOTIFICATION_SERVICE, (IInterface**) (INotificationManager**)&mNotificationMgr))
        intentFilter = NULL;
        tmpIntent = NULL;
        FAIL_RETURN(CIntentFilter::New(String(ACTION_SYNC_ALARM), (IIntentFilter**)&intentFilter))
        AutoPtr<SyncAlarmIntentReceiver> receiver = new SyncAlarmIntentReceiver(this);
        FAIL_RETURN(context->RegisterReceiver(receiver, intentFilter,  (IIntent**)&tmpIntent))
    }
    else {
        mNotificationMgr = NULL;
    }
    FAIL_RETURN(context->GetSystemService(IContext::POWER_SERVICE, (IInterface**) (IPowerManager**)&mPowerManager))
    FAIL_RETURN(mContext->GetSystemService(IContext::USER_SERVICE, (IInterface**) (IUserManager**)&mUserManager))

    // This WakeLock is used to ensure that we stay awake between the time that we receive
    // a sync alarm notification and when we finish processing it. We need to do this
    // because we don't do the work in the alarm handler, rather we do it in a message
    // handler.
//    mHandleAlarmWakeLock = mPowerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK,
//            HANDLE_SYNC_ALARM_WAKE_LOCK);
//    mHandleAlarmWakeLock.setReferenceCounted(false);

    // This WakeLock is used to ensure that we stay awake while running the sync loop
    // message handler. Normally we will hold a sync adapter wake lock while it is being
    // synced but during the execution of the sync loop it might finish a sync for
    // one sync adapter before starting the sync for the other sync adapter and we
    // don't want the device to go to sleep during that window.
//    mSyncManagerWakeLock = mPowerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK,
//            SYNC_LOOP_WAKE_LOCK);
//    mSyncManagerWakeLock.setReferenceCounted(false);

    FAIL_RETURN(mSyncStorageEngine->AddStatusChangeListener(
            IContentResolver::SYNC_OBSERVER_TYPE_SETTINGS, new SyncStatusObserver(this)))

    if (!factoryTest) {
        intentFilter = NULL;
        tmpIntent = NULL;
        FAIL_RETURN(CIntentFilter::New(IAccountManager::LOGIN_ACCOUNTS_CHANGED_ACTION, (IIntentFilter**)&intentFilter))
        // Register for account list updates for all users
        FAIL_RETURN(mContext->RegisterReceiverAsUser(mAccountsUpdatedReceiver, userHandle,intentFilter,
                String(NULL), (IHandler*) NULL, (IIntent**)&tmpIntent))
    }

    // Pick a random second in a day to seed all periodic syncs
    Int32 offSet = 0;
    FAIL_RETURN(mSyncStorageEngine->GetSyncRandomOffset(&offSet))
    mSyncRandomOffsetMillis =  offSet * 1000;
    return NOERROR;
}

ECode CSyncManager::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw)
{
//    final IndentingPrintWriter ipw = new IndentingPrintWriter(pw, "  ");
//***    FAIL_RETURN(DumpSyncState(ipw))
//***    FAIL_RETURN(DumpSyncHistory(ipw))
//***    FAIL_RETURN(DumpSyncAdapters(ipw))
    return E_NOT_IMPLEMENTED;
}

ECode CSyncManager::DumpSyncState(
    /* [in] */ IPrintWriter* pw)
{
    pw->PrintString(String("data connected: "));
    pw->PrintBooleanln(mDataConnectionIsConnected);
    pw->PrintString(String("auto sync: "));
    AutoPtr<List<AutoPtr<IUserInfo> > > users;
    FAIL_RETURN(GetAllUsers((List<AutoPtr<IUserInfo> >**)&users))
    if (NULL != users) {
        AutoPtr<IUserInfo> user;
        List<AutoPtr<IUserInfo> >::Iterator it = users->Begin();
        for (;it != users->End(); it++) {
            user = *it;
            Int32 id = 0;
            FAIL_RETURN(user->GetId(&id))
            Boolean ret = FALSE;
            FAIL_RETURN(mSyncStorageEngine->GetMasterSyncAutomatically(id, &ret))
            String str("u");
            str += StringUtils::Int32ToString(id);
            str += StringUtils::BooleanToString(ret);
            str += " ";
            pw->PrintString(str);
        }
        pw->Println();
        users = NULL;
    }
    pw->PrintString(String("memory low: "));
    pw->PrintBooleanln(mStorageIsLow);

    AutoPtr<ArrayOf<IAccountAndUser *> > accounts;
//    accounts = AccountManagerService.getSingleton().getAllAccounts();

    pw->PrintString(String("accounts: "));
    if (accounts != INITIAL_ACCOUNTS_ARRAY) {
        pw->PrintInt32ln(accounts->GetLength());
    }
    else {
        pw->PrintString(String("not known yet"));
    }
    Int64 now = 0;
//    now = SystemClock.elapsedRealtime();
    pw->PrintString(String("now: "));
    pw->PrintInt64(now);
    String formatTime;
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 millis;
    system->GetCurrentTimeMillis(&millis);

//***    FAIL_RETURN(FormatTime(millis, &formatTime))
    String str(" (");
    str += formatTime;
    str += ")";
    pw->PrintString(str);
    pw->PrintString(String("offset: "));
//***    pw->PrintString(DateUtils::FormatElapsedTime(mSyncRandomOffsetMillis/1000));
    pw->PrintStringln(String(" (HH:MM:SS)"));
    pw->PrintString(String("uptime: "));
//***    pw->PrintString(DateUtils::FormatElapsedTime(now/1000));
    pw->PrintStringln(String(" (HH:MM:SS)"));
    pw->PrintString(String("time spent syncing: "));
//***    pw->PrintString(DateUtils::FormatElapsedTime(mSyncHandler->mSyncTimeTracker->TimeSpentSyncing() / 1000));
    pw->PrintString(String(" (HH:MM:SS), sync "));
    pw->PrintString(mSyncHandler->mSyncTimeTracker->mLastWasSyncing ? String("") : String("not "));
    pw->PrintStringln(String("in progress"));
    if (mSyncHandler->mAlarmScheduleTime != 0) {
        pw->PrintString(String("next alarm time: "));
        pw->PrintInt64(mSyncHandler->mAlarmScheduleTime);
        pw->PrintString(String(" ("));
//***        pw->PrintString(DateUtils::FormatElapsedTime((mSyncHandler->mAlarmScheduleTime-now)/1000));
        pw->PrintStringln(String(" (HH:MM:SS) from now)"));
    }
    else {
        pw->PrintStringln(String("no alarm is scheduled (there had better not be any pending syncs)"));
    }

    pw->PrintString(String("notification info: "));
    AutoPtr<IStringBuilder> sb = new StringBuilder();
    FAIL_RETURN(mSyncHandler->mSyncNotificationInfo->ToString(sb))
    String tmp;
    FAIL_RETURN(sb->ToString(&tmp))
    pw->PrintStringln(tmp);

    pw->Println();
    tmp = "Active Syncs: ";
    tmp = StringUtils::Int32ToString(mActiveSyncContexts->GetSize());
    pw->PrintStringln(tmp);
    List<AutoPtr<ActiveSyncContext> >::Iterator itor = mActiveSyncContexts->Begin();
    AutoPtr<ActiveSyncContext> activeSyncContext;
    for (; itor != mActiveSyncContexts->End(); itor++) {
        activeSyncContext = *itor;
        Int64 durationInSeconds = (now - activeSyncContext->mStartTime) / 1000;
        pw->PrintString(String("  "));
//***        pw->PrintString(DateUtils::FormatElapsedTime(durationInSeconds));
        pw->PrintString(String(" - "));
        String dump;
        FAIL_RETURN(activeSyncContext->mSyncOperation->Dump(FALSE, &dump))
        pw->PrintString(dump);
        pw->Println();
    }

    {
        AutoLock lock(mSyncQueueLock);
        sb->Reset(); // sb->SetLength(0);
        FAIL_RETURN(mSyncQueue->Dump(sb))
    }
    pw->Println();
    sb->ToString(&tmp);
    pw->PrintString(tmp);

    // join the installed sync adapter with the accounts list and emit for everything
    pw->Println();
    pw->PrintStringln(String("Sync Status"));
    const Int32 N = accounts->GetLength();
    AutoPtr<IAccountAndUser> account;
    AutoPtr<IAccount> act;
    String name;
    Int32 userId = 0;
    String type;

    for (Int32 i = 0; i < N; i++) {
        account = (*accounts)[i];
        pw->PrintString(String("  Account "));
        act = NULL;
        FAIL_RETURN(account->GetAccount((IAccount**)&act))
        FAIL_RETURN(act->GetName(&name))
        pw->PrintString(name);
        pw->PrintString(String(" u"));
        FAIL_RETURN(account->GetUserId(&userId))
        pw->PrintString(StringUtils::Int32ToString(userId));
        pw->PrintString(String(" "));
        FAIL_RETURN(act->GetType(&type))
        pw->PrintString(type);
        pw->PrintString(String(":"));
        AutoPtr<ISyncAdapterType> adapterType;
        AutoPtr<RegisteredServicesCache::ServiceInfo> syncAdapterType;
        AutoPtr<ArrayOf<RegisteredServicesCache::ServiceInfo *> > serviceInfos;
        const Int32 M = serviceInfos->GetLength();
//        for (RegisteredServicesCache.ServiceInfo<SyncAdapterType> syncAdapterType :
//                mSyncAdapters.getAllServices(userId)) {
        for (Int32 j = 0; j < M; j++) {
            syncAdapterType = (*serviceInfos)[j];
            adapterType = ISyncAdapterType::Probe(syncAdapterType->mType);
            String accountType;
            FAIL_RETURN(adapterType->GetAccountType(&accountType))
            if (!accountType.Equals(type)) {
                continue;
            }

            String authority;
            FAIL_RETURN(adapterType->GetAuthority(&authority))
            AutoPtr<ISyncStorageEngineAuthorityInfo> settings;
            FAIL_RETURN(mSyncStorageEngine->GetOrCreateAuthority(act, userId, authority,
                    (ISyncStorageEngineAuthorityInfo**)&settings))
            AutoPtr<ISyncStatusInfo> status;
            FAIL_RETURN(mSyncStorageEngine->GetOrCreateSyncStatus(settings, (ISyncStatusInfo**)&status))
            pw->PrintString(String("    "));
            FAIL_RETURN(settings->GetAuthority(&authority))
            pw->PrintString(authority);
            pw->PrintStringln(String(":"));
            pw->PrintString(String("      settings:"));
            Int32 syncable = 0;
            FAIL_RETURN(settings->GetSyncable(&syncable))
            tmp = " ";
            tmp += syncable > 0 ? "syncable" : (syncable == 0 ? "not syncable" : "not initialized");
            pw->PrintString(tmp);
            Boolean enabled = FALSE;
            FAIL_RETURN(settings->GetEnabled(&enabled))
            tmp = ", ";
            tmp += enabled ? "enabled" : "disabled";
            pw->PrintString(tmp);
            Int64 delayUntil = 0;
            FAIL_RETURN(settings->GetDelayUntil(&delayUntil))
            if (delayUntil > now) {
                tmp = ", delay for ";
                tmp += StringUtils::Int64ToString((delayUntil - now) / 1000);
                tmp += " sec";
                pw->PrintString(tmp);
            }
            Int64 backoffTime = 0;
            FAIL_RETURN(settings->GetBackoffTime(&backoffTime))
            if (backoffTime > now) {
                tmp = ", backoff for ";
                tmp += StringUtils::Int64ToString((backoffTime - now) / 1000);
                tmp += " sec";
                pw->PrintString(tmp);
            }
            Int64 backoffDelay = 0;
            FAIL_RETURN(settings->GetBackoffDelay(&backoffDelay))
            if (backoffDelay > 0) {
                tmp = ", the backoff increment is ";
                tmp += StringUtils::Int64ToString(backoffDelay / 1000);
                tmp += " sec";
                pw->PrintString(tmp);
            }
            pw->Println();

            AutoPtr<IObjectContainer> objContainer;
            AutoPtr<IObjectEnumerator> objEnumerator;
            FAIL_RETURN(settings->GetPeriodicSyncs((IObjectContainer**)&objContainer))

            if (NULL != objContainer) {
                FAIL_RETURN(objContainer->GetObjectEnumerator((IObjectEnumerator**)&objEnumerator))
                Boolean hasNext = FALSE;
                AutoPtr<IPair> info;
                Int32 periodicIndex = 0;
                AutoPtr<IBundle> bundle;
                AutoPtr<IInteger64> secondObj;
                Int64 second = 0;
                String str;

                while ((objEnumerator->MoveNext(&hasNext), hasNext)) {
                    info = NULL;
                    secondObj = NULL;
                    bundle = NULL;
                    FAIL_RETURN(objEnumerator->Current((IInterface**)&info))
                    Int64 lastPeriodicTime = 0;
                    FAIL_RETURN(status->GetPeriodicSyncTime(periodicIndex, &lastPeriodicTime))
                    FAIL_RETURN(info->GetSecond((IInterface**)&secondObj))
                    FAIL_RETURN(secondObj->GetValue(&second))
                    FAIL_RETURN(info->GetFirst((IInterface**)&bundle))
//                    FAIL_RETURN(bundle->ToString(&str))
                    Int64 nextPeriodicTime = lastPeriodicTime + second * 1000;
                    tmp = "      periodic period=";
                    tmp += StringUtils::Int64ToString(second);
                    tmp += ", extras=";
//                    tmp += str;
                    tmp += ", next=";
                    FAIL_RETURN(FormatTime(nextPeriodicTime, &str))
                    tmp += str;
                    pw->PrintStringln(tmp);
                    periodicIndex++;
                }

            }

            pw->PrintString(String("      count: local="));
            Int32 numSourceLocal = 0;
            FAIL_RETURN(status->GetNumSourceLocal(&numSourceLocal))
            pw->PrintString(StringUtils::Int32ToString(numSourceLocal));
            pw->PrintString(String(" poll="));
            Int32 numSourcePoll = 0;
            FAIL_RETURN(status->GetNumSourcePoll(&numSourcePoll))
            pw->PrintString(StringUtils::Int32ToString(numSourcePoll));
            pw->PrintString(String(" periodic="));
            Int32 numSourcePeriodic = 0;
            FAIL_RETURN(status->GetNumSourcePeriodic(&numSourcePeriodic))
            pw->PrintString(StringUtils::Int32ToString(numSourcePeriodic));
            pw->PrintString(String(" server="));
            Int32 numSourceServer = 0;
            FAIL_RETURN(status->GetNumSourceServer(&numSourceServer))
            pw->PrintString(StringUtils::Int32ToString(numSourceServer));
            pw->PrintString(String(" user="));
            Int32 numSourceUser = 0;
            FAIL_RETURN(status->GetNumSourceUser(&numSourceUser))
            pw->PrintString(StringUtils::Int32ToString(numSourceUser));
            pw->PrintString(String(" total="));
            Int32 numSyncs = 0;
            FAIL_RETURN(status->GetNumSyncs(&numSyncs))
            pw->PrintString(StringUtils::Int32ToString(numSyncs));
            pw->Println();
            pw->PrintString(String("      total duration: "));
            Int64 totalElapsedTime = 0;
            FAIL_RETURN(status->GetTotalElapsedTime(&totalElapsedTime))
//***            pw->PrintStringln(DateUtils::FormatElapsedTime(totalElapsedTime/1000));
            Int64 lastSuccessTime = 0;
            FAIL_RETURN(status->GetLastSuccessTime(&lastSuccessTime))

            AutoPtr<ISyncStorageEngineHelper> engineHelper;
            FAIL_RETURN(CSyncStorageEngineHelper::AcquireSingleton((ISyncStorageEngineHelper**)&engineHelper))
            String source;
            AutoPtr<ArrayOf<String> > sources;
            FAIL_RETURN(engineHelper->GetSOURCES((ArrayOf<String>**)&sources))

            if (lastSuccessTime != 0) {
                pw->PrintString(String("      SUCCESS: source="));
                Int32 lastSuccessSource = 0;
                FAIL_RETURN(status->GetLastSuccessSource(&lastSuccessSource))
                if (lastSuccessSource < sources->GetLength()) {
                    source = (*sources)[lastSuccessSource];
                }
                pw->PrintString(source);
                pw->PrintString(String(" time="));
                FAIL_RETURN(FormatTime(lastSuccessTime, &tmp))
                pw->PrintStringln(tmp);
            }
            Int64 lastFailureTime = 0;
            FAIL_RETURN(status->GetLastFailureTime(&lastFailureTime))
            if (lastFailureTime != 0) {
                pw->PrintString(String("      FAILURE: source="));
                Int32 lastFailureSource = 0;
                FAIL_RETURN(status->GetLastFailureSource(&lastFailureSource))
                if (lastFailureSource < sources->GetLength()) {
                    source = (*sources)[lastFailureSource];
                }
                pw->PrintString(source);
                pw->PrintString(String(" initialTime="));
                Int64 initialFailureTime = 0;
                FAIL_RETURN(status->GetInitialFailureTime(&initialFailureTime))
                FAIL_RETURN(FormatTime(initialFailureTime, &tmp))
                pw->PrintString(tmp);
                pw->PrintString(String(" lastTime="));
                FAIL_RETURN(FormatTime(lastFailureTime, &tmp))
                pw->PrintStringln(tmp);
                Int32 errCode = 0;
                FAIL_RETURN(status->GetLastFailureMesgAsInt(0, &errCode))
                pw->PrintString(String("      message: "));
                FAIL_RETURN(GetLastFailureMessage(errCode, &tmp))
                tmp += " (";
                tmp += StringUtils::Int32ToString(errCode);
                tmp += ")";
                pw->PrintStringln(tmp);
            }
        }
    }

    return NOERROR;
}

ECode CSyncManager::DumpSyncHistory(
    /* [in] */ IPrintWriter* pw)
{
    FAIL_RETURN(DumpRecentHistory(pw))
    FAIL_RETURN(DumpDayStatistics(pw))
    return NOERROR;
}

ECode CSyncManager::FormatTime(
    /* [in] */ Int64 time,
    /* [out] */ String* formatTime)
{
    VALIDATE_NOT_NULL(formatTime)
    AutoPtr<ITime> tobj;
//***    FAIL_RETURN(CTime((ITime**)&tobj))
    FAIL_RETURN(tobj->Set(time))
    return tobj->Format(String("%Y-%m-%d %H:%M:%S"), formatTime);
}

Boolean CSyncManager::InitStaticMembers()
{
    if(sIsStaticMemberInitialized){
        return TRUE;
    }

    return CActivityManager::IsLargeRAM();
}

ECode CSyncManager::GetAllUsers(
    /* [out, callee] */ List<AutoPtr<IUserInfo> >** allUsers)
{
    VALIDATE_NOT_NULL(allUsers)
    *allUsers = NULL;

    AutoPtr<IObjectContainer> objContainer;
    AutoPtr<IObjectEnumerator> objEnumerator;
    FAIL_RETURN(mUserManager->GetUsers((IObjectContainer**)&objContainer))
    if (NULL != objContainer) {
        FAIL_RETURN(objContainer->GetObjectEnumerator((IObjectEnumerator**)&objEnumerator))
        Boolean hasNext = FALSE;
        AutoPtr<IUserInfo> userInfo;

        AutoPtr<  List<AutoPtr<IUserInfo> > > users = new List<AutoPtr<IUserInfo> >();
        while ((objEnumerator->MoveNext(&hasNext), hasNext)) {
            userInfo = NULL;
            FAIL_RETURN(objEnumerator->Current((IInterface**)&userInfo))
            users->PushBack(userInfo);
        }

        *allUsers = users;
        REFCOUNT_ADD(*allUsers);
    }

    return NOERROR;
}

ECode CSyncManager::ContainsAccountAndUser(
    /* [in] */ ArrayOf<IAccountAndUser *>* accounts,
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    for (Int32 i = 0; i < accounts->GetLength(); i++) {
        Int32 uId = 0;
        AutoPtr<IAccount> act;
        FAIL_RETURN((*accounts)[i]->GetUserId(&uId))
        FAIL_RETURN((*accounts)[i]->GetAccount((IAccount**)&act))
        if (uId == userId && _CObject_Compare(act, account)) {
            *result = TRUE;
            break;
        }
    }

    return NOERROR;
}

ECode CSyncManager::DoDatabaseCleanup()
{
    AutoPtr<IObjectContainer> objContainer;
    AutoPtr<IObjectEnumerator> objEnumerator;
    FAIL_RETURN(mUserManager->GetUsers(TRUE, (IObjectContainer**)&objContainer))

    if (NULL != objContainer) {
        FAIL_RETURN(objContainer->GetObjectEnumerator((IObjectEnumerator**)&objEnumerator))
        Boolean hasNext = FALSE;
        AutoPtr<IUserInfo> userInfo;

        while ((objEnumerator->MoveNext(&hasNext), hasNext)) {
            userInfo = NULL;
            FAIL_RETURN(objEnumerator->Current((IInterface**)&userInfo))
            // Skip any partially created/removed users
            Boolean partial = FALSE;
            FAIL_RETURN(userInfo->GetPartial(&partial))
            if (partial) continue;
            AutoPtr<ArrayOf<IAccount *> > accountsForUser;
//            accountsForUser = AccountManagerService.getSingleton().getAccounts(user.id);
            Int32 id = 0;
            FAIL_RETURN(userInfo->GetId(&id))
            FAIL_RETURN(mSyncStorageEngine->DoDatabaseCleanup(accountsForUser, id))
        }

    }

    return NOERROR;
}

ECode CSyncManager::ReadDataConnectionState(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    AutoPtr<IConnectivityManager> connManager;
    FAIL_RETURN(GetConnectivityManager((IConnectivityManager**)&connManager))
    AutoPtr<INetworkInfo> networkInfo;
    FAIL_RETURN(connManager->GetActiveNetworkInfo((INetworkInfo**)&networkInfo))
    Boolean IsConnected = FALSE;
    *result = (NULL != networkInfo) && (networkInfo->IsConnected(&IsConnected), IsConnected);
    return NOERROR;
}

ECode CSyncManager::GetConnectivityManager(
    /* [out] */ IConnectivityManager** connManager)
{
    VALIDATE_NOT_NULL(connManager)
    AutoLock lock(mCSyncManagerLock);

    if (NULL == mConnManagerDoNotUseDirectly) {
        FAIL_RETURN(mContext->GetSystemService(IContext::CONNECTIVITY_SERVICE,
                (IInterface**)&mConnManagerDoNotUseDirectly))
    }

    *connManager = mConnManagerDoNotUseDirectly;
    REFCOUNT_ADD(*connManager);
    return NOERROR;
}

ECode CSyncManager::Jitterize(
    /* [in] */ Int64 minValue,
    /* [in] */ Int64 maxValue,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    Int64 seed = 0;
//    seed = SystemClock.elapsedRealtime();
//    Random random = new Random();
    srandom(seed);
    Int64 spread = maxValue - minValue;
    if (spread > 0x7FFFFFFF /* Integer.MAX_VALUE */) {
        //throw new IllegalArgumentException("the difference between the maxValue and the "
        //        + "minValue must be less than " + Integer.MAX_VALUE);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    //minValue + random.nextInt((int)spread);
    *value = minValue + random() % spread;
    return NOERROR;
}

ECode CSyncManager::EnsureAlarmService()
{
    if (NULL == mAlarmService) {
        FAIL_RETURN(mContext->GetSystemService(IContext::ALARM_SERVICE, (IInterface**)&mAlarmService))
    }
    return NOERROR;
}

ECode CSyncManager::SendSyncAlarmMessage()
{
//***    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) Logger::V(TAG, String("sending MESSAGE_SYNC_ALARM"));
    Boolean result;
    return mSyncHandler->SendEmptyMessage(SyncHandler::MESSAGE_SYNC_ALARM, &result);
}

ECode CSyncManager::SendCheckAlarmsMessage()
{
//***    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) Logger::V(TAG, String("sending MESSAGE_CHECK_ALARMS"));
    mSyncHandler->RemoveMessages(SyncHandler::MESSAGE_CHECK_ALARMS);
    Boolean result;
    return mSyncHandler->SendEmptyMessage(SyncHandler::MESSAGE_CHECK_ALARMS, &result);
}

ECode CSyncManager::SendSyncFinishedOrCanceledMessage(
    /* [in] */ ActiveSyncContext* syncContext,
    /* [in] */ ISyncResult* syncResult)
{
//***    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) Logger::V(TAG, String("sending MESSAGE_SYNC_FINISHED"));
    AutoPtr<SyncHandlerMessagePayload> msgPayload = new SyncHandlerMessagePayload(syncContext, syncResult);
    AutoPtr<IMessage> msg;
    mSyncHandler->ObtainMessage(SyncHandler::MESSAGE_SYNC_FINISHED,
        msgPayload.Get(), (IMessage**)&msg);
    Boolean result;
    return mSyncHandler->SendMessage(msg, &result);
}

ECode CSyncManager::SendCancelSyncsMessage(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authority)
{
//***    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) Logger::V(TAG, String("sending MESSAGE_CANCEL"));
    AutoPtr<ICharSequence> authCS;
    FAIL_RETURN(CStringWrapper::New(authority, (ICharSequence**)&authCS))
    AutoPtr<IPair> pair;
    FAIL_RETURN(CPair::New(account, authCS, (IPair**)&pair))

    AutoPtr<IMessage> msg;
    mSyncHandler->ObtainMessage(SyncHandler::MESSAGE_CANCEL,
        userId, 0, pair, (IMessage**)&msg);
    Boolean result;
    return mSyncHandler->SendMessage(msg, &result);
}

ECode CSyncManager::ClearBackoffSetting(
    /* [in] */ ISyncOperation* op)
{
    VALIDATE_NOT_NULL(op)
    AutoPtr<IAccount> account;
    Int32 userId = 0;
    String authority;
    FAIL_RETURN(op->GetAccount((IAccount**)&account))
    FAIL_RETURN(op->GetUserId(&userId))
    FAIL_RETURN(op->GetAuthority(&authority))
    FAIL_RETURN(mSyncStorageEngine->SetBackoff(account, userId, authority, ISyncStorageEngine::NOT_IN_BACKOFF_MODE,
            ISyncStorageEngine::NOT_IN_BACKOFF_MODE))

    AutoLock lock(mSyncQueueLock);
    FAIL_RETURN(mSyncQueue->OnBackoffChanged(account, userId, authority, 0))

    return NOERROR;
}

ECode CSyncManager::IncreaseBackoffSetting(
    /* [in] */ ISyncOperation* op)
{
    VALIDATE_NOT_NULL(op)
    // TODO: Use this function to align it to an already scheduled sync
    //       operation in the specified window
    Int64 now = 0;
//    now = SystemClock.elapsedRealtime();
    AutoPtr<IAccount> account;
    Int32 userId = 0;
    String authority;
    FAIL_RETURN(op->GetAccount((IAccount**)&account))
    FAIL_RETURN(op->GetUserId(&userId))
    FAIL_RETURN(op->GetAuthority(&authority))
    AutoPtr<IPair> previousSettings;
    FAIL_RETURN(mSyncStorageEngine->GetBackoff(account, userId, authority, (IPair**)&previousSettings))
    Int64 newDelayInMs = -1;
    if (NULL != previousSettings) {
        // don't increase backoff before current backoff is expired. This will happen for op's
        // with ignoreBackoff set.
        AutoPtr<IInteger64> firstObj;
        Int64 first = 0;
        FAIL_RETURN(previousSettings->GetFirst((IInterface**)&firstObj))
        FAIL_RETURN(firstObj->GetValue(&first))

        if (now < first) {
//***            if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
//                Log.v(TAG, "Still in backoff, do not increase it. "
//                    + "Remaining: " + ((previousSettings.first - now) / 1000) + " seconds.");
//***            }
            return NOERROR;
        }
        AutoPtr<IInteger64> secondObj;
        Int64 second = 0;
        FAIL_RETURN(previousSettings->GetSecond((IInterface**)&secondObj))
        FAIL_RETURN(secondObj->GetValue(&second))
        // Subsequent delays are the double of the previous delay
        newDelayInMs = second * 2;
    }
    if (newDelayInMs <= 0) {
        // The initial delay is the jitterized INITIAL_SYNC_RETRY_TIME_IN_MS
        FAIL_RETURN(Jitterize(INITIAL_SYNC_RETRY_TIME_IN_MS,
                (Int64)(INITIAL_SYNC_RETRY_TIME_IN_MS * 1.1), &newDelayInMs))
    }

    // Cap the delay
    Int64 maxSyncRetryTimeInSeconds = 0;
    AutoPtr<IContentResolver> contentResolver;
    FAIL_RETURN(mContext->GetContentResolver((IContentResolver**)&contentResolver))
//    maxSyncRetryTimeInSeconds = Settings.Global.getLong(contentResolver,
//            String("sync_max_retry_delay_in_seconds") /* Settings.Global.SYNC_MAX_RETRY_DELAY_IN_SECONDS */,
//            DEFAULT_MAX_SYNC_RETRY_TIME_IN_SECONDS);
    if (newDelayInMs > maxSyncRetryTimeInSeconds * 1000) {
        newDelayInMs = maxSyncRetryTimeInSeconds * 1000;
    }

    Int64 backoff = now + newDelayInMs;
    FAIL_RETURN(mSyncStorageEngine->SetBackoff(account, userId, authority, backoff, newDelayInMs))
    FAIL_RETURN(op->SetBackoff(backoff))
    FAIL_RETURN(op->UpdateEffectiveRunTime())

    AutoLock lock(mSyncQueueLock);
    FAIL_RETURN(mSyncQueue->OnBackoffChanged(account, userId, authority, backoff))
    return NOERROR;
}

ECode CSyncManager::SetDelayUntilTime(
    /* [in] */ ISyncOperation* op,
    /* [in] */ Int64 delayUntilSeconds)
{
    Int64 delayUntil = delayUntilSeconds * 1000;
    Int64 absoluteNow;
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    system->GetCurrentTimeMillis(&absoluteNow);
    Int64 newDelayUntilTime = 0;
    if (delayUntil > absoluteNow) {
        Int64 elapsedTime = 0;
//        elapsedTime = SystemClock.elapsedRealtime();
        newDelayUntilTime = elapsedTime + (delayUntil - absoluteNow);
    }
    AutoPtr<IAccount> account;
    Int32 userId = 0;
    String authority;
    FAIL_RETURN(op->GetAccount((IAccount**)&account))
    FAIL_RETURN(op->GetUserId(&userId))
    FAIL_RETURN(op->GetAuthority(&authority))
    FAIL_RETURN(mSyncStorageEngine->SetDelayUntilTime(account, userId, authority, newDelayUntilTime))
    AutoLock lock(mSyncQueueLock);
    FAIL_RETURN(mSyncQueue->OnDelayUntilTimeChanged(account, authority, newDelayUntilTime))
    return NOERROR;
}

ECode CSyncManager::OnUserStarting(
    /* [in] */ Int32 userId)
{
    // Make sure that accounts we're about to use are valid
//    AccountManagerService.getSingleton().validateAccounts(userId);

//    mSyncAdapters.invalidateCache(userId);

    FAIL_RETURN(UpdateRunningAccounts())

    AutoLock lock(mSyncQueueLock);
    FAIL_RETURN(mSyncQueue->AddPendingOperations(userId))

    // Schedule sync for any accounts under started user
    AutoPtr<ArrayOf<IAccount*> > accounts;
//    accounts = AccountManagerService.getSingleton().getAccounts(userId);
    AutoPtr<IAccount> account;
    const Int32 N = accounts->GetLength();

    for (Int32 i = 0; i < N; i++) {
        FAIL_RETURN(ScheduleSync((*accounts)[i], userId, String(NULL), (IBundle*) NULL, 0 /* no delay */,
                TRUE /* onlyThoseWithUnknownSyncableState */))
    }

    FAIL_RETURN(SendCheckAlarmsMessage())
    return NOERROR;
}

ECode CSyncManager::OnUserStopping(
    /* [in] */ Int32 userId)
{
    FAIL_RETURN(UpdateRunningAccounts())
    FAIL_RETURN(CancelActiveSync((IAccount*) NULL /* any account */, userId, String(NULL) /* any authority */))
    return NOERROR;
}

ECode CSyncManager::OnUserRemoved(
    /* [in] */ Int32 userId)
{
    FAIL_RETURN(UpdateRunningAccounts())
    // Clean up the storage engine database
    AutoPtr<ArrayOf<IAccount *> > accounts;
    FAIL_RETURN(mSyncStorageEngine->DoDatabaseCleanup(accounts, userId))
    AutoLock lock(mSyncQueueLock);
    FAIL_RETURN(mSyncQueue->RemoveUser(userId))
    return NOERROR;
}

ECode CSyncManager::GetLastFailureMessage(
    /* [in] */ Int32 code,
    /* [out] */ String* failureMessage)
{
    VALIDATE_NOT_NULL(failureMessage)
    switch (code) {
        case IContentResolver::SYNC_ERROR_SYNC_ALREADY_IN_PROGRESS:
            *failureMessage = "sync already in progress";
            break;

        case IContentResolver::SYNC_ERROR_AUTHENTICATION:
            *failureMessage = "authentication error";
            break;

        case IContentResolver::SYNC_ERROR_IO:
            *failureMessage = "I/O error";
            break;

        case IContentResolver::SYNC_ERROR_PARSE:
            *failureMessage = "parse error";
            break;

        case IContentResolver::SYNC_ERROR_CONFLICT:
            *failureMessage = "conflict error";
            break;

        case IContentResolver::SYNC_ERROR_TOO_MANY_DELETIONS:
            *failureMessage = "too many deletions error";
            break;

        case IContentResolver::SYNC_ERROR_TOO_MANY_RETRIES:
            *failureMessage = "too many retries error";
            break;

        case IContentResolver::SYNC_ERROR_INTERNAL:
            *failureMessage = "internal error";
            break;

        default:
            *failureMessage = "unknown";
            break;
    }
    return NOERROR;
}

ECode CSyncManager::DumpTimeSec(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ Int64 time)
{
    pw->PrintInt64(time/1000);
    pw->PrintChar('.');
    pw->PrintInt64((time/100)%10);
    pw->PrintChar('s');
    return NOERROR;
}

ECode CSyncManager::DumpDayStatistic(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ISyncStorageEngineDayStats* ds)
{
    pw->PrintString(String("Success ("));
    Int32 successCount = 0;
    FAIL_RETURN(ds->GetSuccessCount(&successCount))
    pw->PrintInt32(successCount);
    if (successCount > 0) {
        pw->PrintString(String(" for "));
        Int64 successTime = 0;
        FAIL_RETURN(ds->GetSuccessTime(&successTime))
        FAIL_RETURN(DumpTimeSec(pw, successTime))
        pw->PrintString(String(" avg="));
        FAIL_RETURN(DumpTimeSec(pw, successTime/successCount))
    }
    pw->PrintString(String(") Failure ("));
    Int32 failureCount = 0;
    FAIL_RETURN(ds->GetFailureCount(&failureCount))
    pw->PrintInt32(failureCount);
    if (failureCount > 0) {
        pw->PrintString(String(" for "));
        Int64 failureTime = 0;
        FAIL_RETURN(ds->GetFailureTime(&failureTime))
        FAIL_RETURN(DumpTimeSec(pw, failureTime))
        pw->PrintString(String(" avg="));
        FAIL_RETURN(DumpTimeSec(pw, failureTime/failureCount))
    }
    pw->PrintStringln(String(")"));
    return NOERROR;
}

ECode CSyncManager::DumpRecentHistory(
    /* [in] */ IPrintWriter* pw)
{
    List<AutoPtr<ISyncStorageEngineSyncHistoryItem> > items;
    AutoPtr<IObjectContainer> objContainer;
    AutoPtr<IObjectEnumerator> objEnumerator;
    FAIL_RETURN(mSyncStorageEngine->GetSyncHistory((IObjectContainer**)&objContainer))

    if (NULL != objContainer) {
        FAIL_RETURN(objContainer->GetObjectEnumerator((IObjectEnumerator**)&objEnumerator))
        Boolean hasNext = FALSE;
        AutoPtr<ISyncStorageEngineSyncHistoryItem> item;

        while ((objEnumerator->MoveNext(&hasNext), hasNext)) {
            item = NULL;
            FAIL_RETURN(objEnumerator->Current((IInterface**)&item))
            items.PushBack(item);
        }

    }

    if (!items.IsEmpty()) {
        HashMap<String, AutoPtr<AuthoritySyncStats> > authorityMap;
        Int64 totalElapsedTime = 0;
        Int64 totalTimes = 0;
        Int32 maxAuthority = 0;
        Int32 maxAccount = 0;
        List<AutoPtr<ISyncStorageEngineSyncHistoryItem> >::Iterator it;
        AutoPtr<ISyncStorageEngineSyncHistoryItem> item;
        for (it = items.Begin(); it != items.End(); it++) {
            item = *it;
            AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
            Int32 authorityId = 0;
            FAIL_RETURN(item->GetAuthorityId(&authorityId))
            FAIL_RETURN(mSyncStorageEngine->GetAuthority(authorityId, (ISyncStorageEngineAuthorityInfo**)&authority))
            String authorityName;
            String accountKey;
            if (NULL != authority) {
                FAIL_RETURN(authority->GetAuthority(&authorityName))
                AutoPtr<IAccount> account;
                FAIL_RETURN(authority->GetAccount((IAccount**)&account))
                Int32 userId = 0;
                FAIL_RETURN(authority->GetUserId(&userId))
                String name;
                String type;
                FAIL_RETURN(account->GetName(&name))
                FAIL_RETURN(account->GetType(&type))
                accountKey = name;
                accountKey += "/";
                accountKey += type;
                accountKey += " u";
                accountKey += StringUtils::Int32ToString(userId);
            }
            else {
                authorityName = "Unknown";
                accountKey = "Unknown";
            }

            Int32 length = authorityName.GetLength();
            if (length > maxAuthority) {
                maxAuthority = length;
            }
            length = accountKey.GetLength();
            if (length > maxAccount) {
                maxAccount = length;
            }

            Int64 elapsedTime = 0;
            FAIL_RETURN(item->GetElapsedTime(&elapsedTime))
            totalElapsedTime += elapsedTime;
            totalTimes++;
            AutoPtr<AuthoritySyncStats> authoritySyncStats = authorityMap[authorityName];
            if (NULL == authoritySyncStats) {
                authoritySyncStats = new AuthoritySyncStats(authorityName);
                authorityMap.Insert(HashMap<String, AutoPtr<AuthoritySyncStats> >::ValueType(authorityName, authoritySyncStats));
            }
            authoritySyncStats->elapsedTime += elapsedTime;
            authoritySyncStats->times++;
            HashMap<String, AutoPtr<AccountSyncStats> >& accountMap = authoritySyncStats->mAccountMap;
            AutoPtr<AccountSyncStats> accountSyncStats = accountMap[accountKey];
            if (NULL == accountSyncStats) {
                accountSyncStats = new AccountSyncStats(accountKey);
                accountMap.Insert(HashMap<String, AutoPtr<AccountSyncStats> >::ValueType(accountKey, accountSyncStats));
            }
            accountSyncStats->elapsedTime += elapsedTime;
            accountSyncStats->times++;

        }

        if (totalElapsedTime > 0) {
            pw->Println();
            String format("Detailed Statistics (Recent history):  %d (# of times) %ds (sync time)\n");
            AutoPtr<ArrayOf<IInterface *> > args = ArrayOf<IInterface *>::Alloc(2);
            AutoPtr<IInteger64> totalTimesObj;
            FAIL_RETURN(CInteger64::New(totalTimes, (IInteger64**)&totalTimesObj))
            Int64 tmpValue = 0;
            tmpValue = totalElapsedTime / 1000;
            AutoPtr<IInteger64> tmpValueObj;
            FAIL_RETURN(CInteger64::New(tmpValue, (IInteger64**)&tmpValueObj))
            args->Set(0, totalTimesObj);
            args->Set(1, tmpValueObj);
            pw->Printf(format, args);

            List<AutoPtr<AuthoritySyncStats> > sortedAuthorities;
            HashMap<String, AutoPtr<AuthoritySyncStats> >::Iterator it = authorityMap.Begin();
            for (; it != authorityMap.End(); it++) {
                sortedAuthorities.PushBack((*it).mSecond);
            }

            sortedAuthorities.Sort(ComparatorTemplate());
            Int32 maxLength = maxAuthority > (maxAccount + 3) ? maxAuthority : (maxAccount + 3);
            Int32 padLength = 2 + 2 + maxLength + 2 + 10 + 11;
            AutoPtr<ArrayOf<Char32> > chars = ArrayOf<Char32>::Alloc(padLength);
            for (Int32 i = 0; i < padLength; i++) {
                (*chars)[i] = '-';
            }

            String separator(*chars);
            String authorityFormat;
            String accountFormat;
//            authorityFormat = String.format("  %%-%ds: %%-9s  %%-11s\n", maxLength + 2);
            char buf1[50];
            sprintf(buf1, "  %%-%ds: %%-9s  %%-11s\n",  maxLength + 2);
            authorityFormat = buf1;
//            accountFormat = String.format("    %%-%ds:   %%-9s  %%-11s\n", maxLength);
            char buf2[50];
            sprintf(buf2, "    %%-%ds:   %%-9s  %%-11s\n", maxLength);
            accountFormat = buf2;
            pw->PrintStringln(separator);
            AutoPtr<ArrayOf<IInterface *> > args2 = ArrayOf<IInterface *>::Alloc(3);
            AutoPtr<AuthoritySyncStats> authoritySyncStats;
            List<AutoPtr<AuthoritySyncStats> >::Iterator itor = sortedAuthorities.Begin();
            for (; itor != sortedAuthorities.End(); itor++) {
                authoritySyncStats = *itor;
                String name = authoritySyncStats->name;
                Int64 elapsedTime = 0;
                Int32 times = 0;
                String timeStr;
                String timesStr;
                elapsedTime = authoritySyncStats->elapsedTime;
                times = authoritySyncStats->times;
//                timeStr = String.format("%ds/%d%%",
//                        elapsedTime / 1000,
//                        elapsedTime * 100 / totalElapsedTime);
                char buf1[30];
                sprintf(buf1, "%ds/%d%%", (Int32)(elapsedTime / 1000), (Int32)(elapsedTime * 100 / totalElapsedTime));
                timeStr = buf1;
//                timesStr = String.format("%d/%d%%",
//                        times,
//                        times * 100 / totalTimes);
                char buf2[30];
                sprintf(buf2, "%d/%d%%", times, (Int32)(times * 100 / totalTimes));
                timesStr = buf2;
                AutoPtr<ICharSequence> nameCS;
                AutoPtr<ICharSequence> timesStrCS;
                AutoPtr<ICharSequence> timeStrCS;
                FAIL_RETURN(CStringWrapper::New(name, (ICharSequence**)&nameCS))
                FAIL_RETURN(CStringWrapper::New(timesStr, (ICharSequence**)&timesStrCS))
                FAIL_RETURN(CStringWrapper::New(timeStr, (ICharSequence**)&timeStrCS))
                args2->Set(0, nameCS);
                args2->Set(1, timesStrCS);
                args2->Set(2, timeStrCS);
                pw->Printf(authorityFormat, args2);

                List<AutoPtr<AccountSyncStats> > sortedAccounts;
                HashMap<String, AutoPtr<AccountSyncStats> >& accountMap = authoritySyncStats->mAccountMap;
                HashMap<String, AutoPtr<AccountSyncStats> >::Iterator it = accountMap.Begin();
                for (; it != accountMap.End(); it++) {
                    sortedAccounts.PushBack((*it).mSecond);
                }

                sortedAccounts.Sort(ComparatorTemplate());
                AutoPtr<AccountSyncStats> stats;
                List<AutoPtr<AccountSyncStats> >::Iterator it2 = sortedAccounts.Begin();
                for (; it2 != sortedAccounts.End(); it2++) {
                    stats = *it2;
                    elapsedTime = stats->elapsedTime;
                    times = stats->times;
//                    timeStr = String.format("%ds/%d%%",
//                            elapsedTime / 1000,
//                            elapsedTime * 100 / totalElapsedTime);
                    char buf1[30];
                    sprintf(buf1, "%ds/%d%%", (Int32)(elapsedTime / 1000), (Int32)(elapsedTime * 100 / totalElapsedTime));
                    timeStr = buf1;
//                    timesStr = String.format("%d/%d%%",
//                            times,
//                            times * 100 / totalTimes);
                    char buf2[30];
                    sprintf(buf2, "%d/%d%%", times, (Int32)(times * 100 / totalTimes));
                    timesStr = buf2;
                    AutoPtr<ICharSequence> nameCS;
                    AutoPtr<ICharSequence> timesStrCS;
                    AutoPtr<ICharSequence> timeStrCS;
                    FAIL_RETURN(CStringWrapper::New(stats->name, (ICharSequence**)&nameCS))
                    FAIL_RETURN(CStringWrapper::New(timesStr, (ICharSequence**)&timesStrCS))
                    FAIL_RETURN(CStringWrapper::New(timeStr, (ICharSequence**)&timeStrCS))
                    args2->Set(0, nameCS);
                    args2->Set(1, timesStrCS);
                    args2->Set(2, timeStrCS);
                    pw->Printf(accountFormat, args2);
                }
                pw->PrintStringln(separator);
            }
        }

        pw->Println();
        pw->PrintStringln(String("Recent Sync History"));
        String format("  %-");
        format += StringUtils::Int32ToString(maxAccount);
        format += "s  %s\n";
        HashMap<String, Int64> lastTimeMap;
        Int32 index = 0;
        AutoPtr<ISyncStorageEngineHelper> engineHelper;
        FAIL_RETURN(CSyncStorageEngineHelper::AcquireSingleton((ISyncStorageEngineHelper**)&engineHelper))
        for (it = items.Begin(); it != items.End(); it++) {
            item = *it;
            AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
            Int32 authorityId = 0;
            FAIL_RETURN(item->GetAuthorityId(&authorityId))
            FAIL_RETURN(mSyncStorageEngine->GetAuthority(authorityId, (ISyncStorageEngineAuthorityInfo**)&authority))
            String authorityName;
            String accountKey;
            if (NULL != authority) {
                FAIL_RETURN(authority->GetAuthority(&authorityName))
                AutoPtr<IAccount> account;
                FAIL_RETURN(authority->GetAccount((IAccount**)&account))
                Int32 userId = 0;
                FAIL_RETURN(authority->GetUserId(&userId))
                String name;
                String type;
                FAIL_RETURN(account->GetName(&name))
                FAIL_RETURN(account->GetType(&type))
                accountKey = name;
                accountKey += "/";
                accountKey += type;
                accountKey += " u";
                accountKey += StringUtils::Int32ToString(userId);
            }
            else {
                authorityName = "Unknown";
                accountKey = "Unknown";
            }
            Int64 elapsedTime = 0;
            FAIL_RETURN(item->GetElapsedTime(&elapsedTime))
            AutoPtr<ITime> time;
//            time = new Time();
            Int64 eventTime = 0;
            FAIL_RETURN(item->GetEventTime(&eventTime))
            FAIL_RETURN(time->Set(eventTime))

            String key(authorityName);
            key += authorityName;
            key += "/";
            key += accountKey;
            Int64 lastEventTime = lastTimeMap[key];
            String diffString;
            if (lastEventTime == 0) {
                diffString = "";
            }
            else {
                Int64 diff = (lastEventTime - eventTime) / 1000;
                if (diff < 60) {
                    diffString = StringUtils::Int64ToString(diff);
                }
                else if (diff < 3600) {
//                    diffString = String.format("%02d:%02d", diff / 60, diff % 60);
                    char buf[20];
                    sprintf(buf, "%02d:%02d", (Int32) (diff / 60), (Int32) (diff % 60));
                    diffString = buf;
                }
                else {
                    Int64 sec = diff % 3600;
//                    diffString = String.format("%02d:%02d:%02d", diff / 3600, sec / 60, sec % 60);
                    char buf[30];
                    sprintf(buf, "%02d:%02d:%02d", (Int32) (diff / 3600), (Int32) (sec / 60), (Int32) (sec % 60));
                    diffString = buf;
                }
            }

            lastTimeMap.Insert(HashMap<String, Int64>::ValueType(key, eventTime));
            AutoPtr<ArrayOf<IInterface *> > args3 = ArrayOf<IInterface *>::Alloc(5);
            AutoPtr<IInteger32> intObj;
            FAIL_RETURN(CInteger32::New(index + 1, (IInteger32**)&intObj))
            String formatTime;
            FAIL_RETURN(FormatTime(eventTime, &formatTime))
            AutoPtr<ICharSequence> formatTimeCS;
            FAIL_RETURN(CStringWrapper::New(formatTime, (ICharSequence**)&formatTimeCS))
            Int32 src = 0;
            FAIL_RETURN(item->GetSource(&src))
            String source;
            AutoPtr<ArrayOf<String> > sources;
            FAIL_RETURN(engineHelper->GetSOURCES((ArrayOf<String>**)&sources))
            if (src < sources->GetLength()) {
                source = (*sources)[src];
            }
            AutoPtr<ICharSequence> sourceCS;
            FAIL_RETURN(CStringWrapper::New(source, (ICharSequence**)&sourceCS))
            AutoPtr<IFloat> elapsedObj;
            FAIL_RETURN(CFloat::New(((Float) elapsedTime) / 1000, (IFloat**)&elapsedObj))
            AutoPtr<ICharSequence> diffStringCS;
            FAIL_RETURN(CStringWrapper::New(diffString, (ICharSequence**)&diffStringCS))
            args3->Set(0, intObj);
            args3->Set(1, formatTimeCS);
            args3->Set(2, sourceCS);
            args3->Set(3, elapsedObj);
            args3->Set(4, diffStringCS);
            pw->Printf(String("  #%-3d: %s %8s  %5.1fs  %8s"), args3);

            AutoPtr<ArrayOf<IInterface *> > args4 = ArrayOf<IInterface *>::Alloc(2);
            AutoPtr<ICharSequence> accountKeyCS;
            AutoPtr<ICharSequence> authorityNameCS;
            FAIL_RETURN(CStringWrapper::New(accountKey, (ICharSequence**)&accountKeyCS))
            FAIL_RETURN(CStringWrapper::New(authorityName, (ICharSequence**)&authorityNameCS))
            args4->Set(0, accountKeyCS);
            args4->Set(1, authorityNameCS);
            pw->Printf(format, args4);

            Int32 event = 0;
            Int64 upstreamActivity = 0;
            Int64 downstreamActivity = 0;
            FAIL_RETURN(item->GetEvent(&event))
            FAIL_RETURN(item->GetUpstreamActivity(&upstreamActivity))
            FAIL_RETURN(item->GetDownstreamActivity(&downstreamActivity))
            if (event != ISyncStorageEngine::EVENT_STOP || upstreamActivity != 0
                    || downstreamActivity != 0) {
                AutoPtr<ArrayOf<IInterface *> > args5 = ArrayOf<IInterface *>::Alloc(3);
                AutoPtr<IInteger32> eventObj;
                AutoPtr<IInteger64> upstreamActivityObj;
                AutoPtr<IInteger64> downstreamActivityObj;
                FAIL_RETURN(CInteger32::New(event, (IInteger32**)&eventObj))
                FAIL_RETURN(CInteger64::New(upstreamActivity, (IInteger64**)&upstreamActivityObj))
                FAIL_RETURN(CInteger64::New(downstreamActivity, (IInteger64**)&downstreamActivityObj))
                args5->Set(0, eventObj);
                args5->Set(1, upstreamActivityObj);
                args5->Set(2, downstreamActivityObj);
                pw->Printf(String("    event=%d upstreamActivity=%d downstreamActivity=%d\n"), args5);
            }

            String mesg;
            FAIL_RETURN(item->GetMesg(&mesg))
            if (!mesg.IsNull() && !ISyncStorageEngine::MESG_SUCCESS.Equals(mesg)) {
                AutoPtr<ArrayOf<IInterface *> > args6 = ArrayOf<IInterface *>::Alloc(1);
                AutoPtr<ICharSequence> mesgCS;
                FAIL_RETURN(CStringWrapper::New(mesg, (ICharSequence**)&mesgCS))
                args6->Set(0, mesgCS);
                pw->Printf(String("    mesg=%s\n"), args6);
            }
            index++;
        }
    }
    return NOERROR;
}

ECode CSyncManager::DumpDayStatistics(
    /* [in] */ IPrintWriter* pw)
{
    AutoPtr<ArrayOf<ISyncStorageEngineDayStats *> > dses;
    FAIL_RETURN(mSyncStorageEngine->GetDayStatistics((ArrayOf<ISyncStorageEngineDayStats *>**)&dses))
    if (NULL != dses && NULL != (*dses)[0]) {
        pw->Println();
        pw->PrintStringln(String("Sync Statistics"));
        pw->PrintString(String("  Today:  "));
        FAIL_RETURN(DumpDayStatistic(pw, (*dses)[0]))
        Int32 today = 0;
        FAIL_RETURN((*dses)[0]->GetDay(&today))
        Int32 i = 0;
        AutoPtr<ISyncStorageEngineDayStats> ds;

        // Print each day in the current week.
        for (i=1; i<=6 && i < dses->GetLength(); i++) {
            ds = (*dses)[i];
            if (NULL == ds) break;
            Int32 day = 0;
            FAIL_RETURN(ds->GetDay(&day))
            Int32 delta = today - day;
            if (delta > 6) break;

            pw->PrintString(String("  Day-"));
            pw->PrintInt32(delta);
            pw->PrintString(String(":  "));
            FAIL_RETURN(DumpDayStatistic(pw, ds))
        }

        // Aggregate all following days into weeks and print totals.
        Int32 weekDay = today;
        while (i < dses->GetLength()) {
            AutoPtr<ISyncStorageEngineDayStats> aggr = NULL;
            weekDay -= 7;
            while (i < dses->GetLength()) {
                ds = (*dses)[i];
                if (NULL == ds) {
                    i = dses->GetLength();
                    break;
                }
                Int32 day = 0;
                FAIL_RETURN(ds->GetDay(&day))
                Int32 delta = weekDay-day;
                if (delta > 6) break;
                i++;

                if (NULL == aggr) {
                    FAIL_RETURN(CSyncStorageEngineDayStats::New(weekDay, (ISyncStorageEngineDayStats**)&aggr))
                }

                Int32 dsSC = 0;
                Int32 aggrSC = 0;
                FAIL_RETURN(ds->GetSuccessCount(&dsSC))
                FAIL_RETURN(aggr->GetSuccessCount(&aggrSC))
                aggrSC += dsSC;
                FAIL_RETURN(aggr->SetSuccessCount(aggrSC))

                Int64 dsST = 0;
                Int64 aggrST = 0;
                FAIL_RETURN(ds->GetSuccessTime(&dsST))
                FAIL_RETURN(aggr->GetSuccessTime(&aggrST))
                aggrST += dsST;
                FAIL_RETURN(aggr->SetSuccessTime(aggrST))

                Int32 dsFC = 0;
                Int32 aggrFC = 0;
                FAIL_RETURN(ds->GetFailureCount(&dsFC))
                FAIL_RETURN(aggr->GetFailureCount(&aggrFC))
                aggrFC += dsFC;
                FAIL_RETURN(aggr->SetFailureCount(aggrFC))

                Int64 dsFT = 0;
                Int64 aggrFT = 0;
                FAIL_RETURN(ds->GetFailureTime(&dsFT))
                FAIL_RETURN(aggr->GetFailureTime(&aggrFT))
                aggrFT += dsFT;
                FAIL_RETURN(aggr->SetFailureTime(aggrFT))
            }
            if (NULL != aggr) {
                pw->PrintString(String("  Week-"));
                pw->PrintInt32((today-weekDay)/7);
                pw->PrintString(String(": "));
                FAIL_RETURN(DumpDayStatistic(pw, aggr))
            }
        }
    }
    return NOERROR;
}

//ECode CSyncManager::DumpSyncAdapters(
//    /* [in] */ IIndentingPrintWriter* pw)
//{
//    return E_NOT_IMPLEMENTED;
//}

ECode CSyncManager::IsSyncStillActive(
    /* [in] */ ActiveSyncContext* activeSyncContext,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    List<AutoPtr<ActiveSyncContext> >::Iterator it = mActiveSyncContexts->Begin();
    AutoPtr<ActiveSyncContext> activeSync;

    for (; it != mActiveSyncContexts->End(); it++) {
        activeSync = *it;
        if ((ActiveSyncContext*) activeSync == activeSyncContext) {
            *result = TRUE;
            return NOERROR;
        }
    }

    *result = FALSE;
    return NOERROR;
}

ECode CSyncManager::MaybeRescheduleSync(
    /* [in] */ ISyncResult* syncResult,
    /* [in] */ ISyncOperation* operation)
{
//    boolean isLoggable = Log.isLoggable(TAG, Log.DEBUG);
//    if (isLoggable) {
//        Log.d(TAG, "encountered error(s) during the sync: " + syncResult + ", " + operation);
//    }

    AutoPtr<ISyncOperation> newOperation;
    FAIL_RETURN(CSyncOperation::New(operation, (ISyncOperation**)&newOperation))
    AutoPtr<IBundle> extras;
    Boolean ret = FALSE;
    Boolean syncAlreadyInProgress = FALSE;
    FAIL_RETURN(newOperation->GetExtras((IBundle**)&extras))

    // The SYNC_EXTRAS_IGNORE_BACKOFF only applies to the first attempt to sync a given
    // request. Retries of the request will always honor the backoff, so clear the
    // flag in case we retry this request.
    FAIL_RETURN(extras->GetBoolean(IContentResolver::SYNC_EXTRAS_IGNORE_BACKOFF, FALSE, &ret))
    if (ret) {
        FAIL_RETURN(extras->Remove(IContentResolver::SYNC_EXTRAS_IGNORE_BACKOFF))
    }

    // If this sync aborted because the internal sync loop retried too many times then
    //   don't reschedule. Otherwise we risk getting into a retry loop.
    // If the operation succeeded to some extent then retry immediately.
    // If this was a two-way sync then retry soft errors with an exponential backoff.
    // If this was an upward sync then schedule a two-way sync immediately.
    // Otherwise do not reschedule.
    if ((extras->GetBoolean(IContentResolver::SYNC_EXTRAS_DO_NOT_RETRY, FALSE, &ret), ret)) {
//        Log.d(TAG, "not retrying sync operation because SYNC_EXTRAS_DO_NOT_RETRY was specified "
//                + newOperation);
    }
    else if ((extras->GetBoolean(IContentResolver::SYNC_EXTRAS_UPLOAD, FALSE, &ret), ret)
            && !(syncResult->GetSyncAlreadyInProgress(&syncAlreadyInProgress), syncAlreadyInProgress)) {
        FAIL_RETURN(extras->Remove(IContentResolver::SYNC_EXTRAS_UPLOAD))
//        Log.d(TAG, "retrying sync operation as a two-way sync because an upload-only sync "
//                + "encountered an error: " + newOperation);
        FAIL_RETURN(ScheduleSyncOperation(newOperation))
    }
    else if ((syncResult->GetTooManyRetries(&ret), ret)) {
//        Log.d(TAG, "not retrying sync operation because it retried too many times: "
//                + newOperation);
    }
    else if ((syncResult->MadeSomeProgress(&ret), ret)) {
//        if (isLoggable) {
//            Log.d(TAG, "retrying sync operation because even though it had an error "
//                    + "it achieved some success");
//        }
        FAIL_RETURN(ScheduleSyncOperation(newOperation))
    }
    else if ((syncResult->GetSyncAlreadyInProgress(&syncAlreadyInProgress), syncAlreadyInProgress)) {
//        if (isLoggable) {
//            Log.d(TAG, "retrying sync operation that failed because there was already a "
//                    + "sync in progress: " + newOperation);
//        }
        AutoPtr<ISyncOperation> tmpOp;
        AutoPtr<IAccount> account;
        Int32 userId = 0;
        Int32 syncSource = 0;
        Int64 backoff = 0;
        Int64 delayUntil = 0;
        String authority;
        FAIL_RETURN(newOperation->GetAccount((IAccount**)&account))
        FAIL_RETURN(newOperation->GetUserId(&userId))
        FAIL_RETURN(newOperation->GetSyncSource(&syncSource))
        FAIL_RETURN(newOperation->GetBackoff(&backoff))
        FAIL_RETURN(newOperation->GetDelayUntil(&delayUntil))
        FAIL_RETURN(newOperation->GetAuthority(&authority))
        FAIL_RETURN(newOperation->AllowParallelSyncs(&ret))
        FAIL_RETURN(CSyncOperation::New(account, userId, syncSource, authority, extras,
                DELAY_RETRY_SYNC_IN_PROGRESS_IN_SECONDS * 1000, backoff, delayUntil, ret, (ISyncOperation**)&tmpOp))
        FAIL_RETURN(ScheduleSyncOperation(tmpOp))
    }
    else if ((syncResult->HasSoftError(&ret), ret)) {
//        if (isLoggable) {
//            Log.d(TAG, "retrying sync operation because it encountered a soft error: "
//                    + newOperation);
//        }
        FAIL_RETURN(ScheduleSyncOperation(newOperation))
    }
    else {
//        Log.d(TAG, "not retrying sync operation because the error is a hard error: "
//                + newOperation);
    }
    return NOERROR;
}

} // namespace Elastos
} // namespace Droid
} // namespace Content
