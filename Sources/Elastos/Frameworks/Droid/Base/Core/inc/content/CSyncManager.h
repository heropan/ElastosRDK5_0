
#ifndef __CSYNCMANAGER_H__
#define __CSYNCMANAGER_H__

#include "_CSyncManager.h"
#include <ext/frameworkext.h>
#include <elastos/List.h>
#include <elastos/HashMap.h>
#include <elastos/Mutex.h>
#include <elastos/StringBuilder.h>
#include "BroadcastReceiver.h"
#include "SyncAdaptersCache.h"
#include "SyncQueue.h"
#include "os/HandlerBase.h"
//***#include "../text/format/DateUtils.h"
//***#include "../os/SystemProperties.h"
//***#include <Logger.h>

using namespace Elastos::IO;
using namespace Elastos::Core;
using namespace Elastos::Utility;
using namespace Elastos::Droid::Accounts;
using namespace Elastos::Droid::App;
using namespace Elastos::Droid::Net;
using namespace Elastos::Droid::Os;
using namespace Elastos::Droid::Utility;
using namespace Elastos::Droid::Content::Pm;
using namespace Elastos::Droid::Text::Format;
using Elastos::Utility::Concurrent::ICountDownLatch;
//***using namespace Elastos::Utility::Logging;

// TODO please define in frameworkhash.h
//
_ETL_NAMESPACE_BEGIN
#ifndef AUTOPTR_IPAIR_HASH_FUNC
#define AUTOPTR_IPAIR_HASH_FUNC
template<> struct Hash< AutoPtr<IPair> >
{
    size_t operator()(AutoPtr<IPair> name) const
    {
        assert(name != NULL);
        return (size_t)name.Get();
    }
};

template<> struct EqualTo< AutoPtr<IPair> >
{
    Boolean operator()(const AutoPtr<IPair>& x,
                         const AutoPtr<IPair>& y) const
    {
        assert(x != NULL && y != NULL);
        return x.Get() == y.Get();
    }
};
#endif
_ETL_NAMESPACE_END

namespace Elastos {
namespace Droid {
namespace Content {

struct ComparatorTemplate
{
public:
    template<typename T> int operator () (const T& lhs, const T& rhs);
};

template<typename T>
int ComparatorTemplate::operator () (const T& lhs, const T& rhs)
{
    // reverse order
    int compare = lhs.Get()->times < rhs.Get()->times ? -1 : (lhs.Get()->times == rhs.Get()->times ? 0 : 1);
    if (compare == 0) {
        compare = lhs.Get()->elapsedTime < rhs.Get()->elapsedTime ? -1 : (lhs.Get()->elapsedTime == rhs.Get()->elapsedTime ? 0 : 1);
    }
    return compare;
}

CarClass(CSyncManager)
{
    friend class SyncHandlerMessagePayload;
    friend class SyncAlarmIntentReceiver;
    friend class ActiveSyncContext;
    friend class ServiceConnectionData;
    friend class SyncHandler;
    friend class AuthoritySyncStats;
    friend class AccountSyncStats;
    friend class SyncTimeTracker;
    friend class StorageIntentReceiver;
    friend class BootCompletedReceiver;
    friend class BackgroundDataSettingChanged;
    friend class AccountsUpdatedReceiver;
    friend class ConnectivityIntentReceiver;
    friend class ShutdownIntentReceiver;
    friend class UserIntentReceiver;
    friend class SyncStatusObserver;
    friend class SyncRequestListener;

    class SyncTimeTracker;
    class AccountSyncStats;

public:
    class SyncHandler;
    class ActiveSyncContext;

    class SyncHandlerMessagePayload
        : public IInterface
        , public ElRefBase
    {
    public:
        SyncHandlerMessagePayload(
            /* [in] */ ActiveSyncContext* syncContext,
            /* [in] */ ISyncResult* syncRet);

        ~SyncHandlerMessagePayload();

        CAR_INTERFACE_DECL()

    public:
        AutoPtr<ActiveSyncContext> activeSyncContext;
        AutoPtr<ISyncResult> syncResult;

    };

    class SyncAlarmIntentReceiver
        : public BroadcastReceiver
    {
    public:
        SyncAlarmIntentReceiver(
            /* [in] */ CSyncManager* syncMgr);

        ~SyncAlarmIntentReceiver();

        CARAPI Initialize();

    protected:
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("SyncAlarmIntentReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        AutoPtr<CSyncManager> mSyncMgr;

    };

    /**
     * @hide
     */
    class ActiveSyncContext
        : public ElRefBase
        , public IISyncContext
        , public IServiceConnection
        , public IProxyDeathRecipient
    {
    public:
        friend class SyncHandler;
        /**
         * Create an ActiveSyncContext for an impending sync and grab the wakelock for that
         * sync adapter. Since this grabs the wakelock you need to be sure to call
         * close() when you are done with this ActiveSyncContext, whether the sync succeeded
         * or not.
         * @param syncOperation the SyncOperation we are about to sync
         * @param historyRowId the row in which to record the history info for this sync
         * @param syncAdapterUid the UID of the application that contains the sync adapter
         * for this sync. This is used to attribute the wakelock hold to that application.
         */
        ActiveSyncContext(
            /* [in] */ ISyncOperation* syncOperation,
            /* [in] */ Int64 historyRowId,
            /* [in] */ Int32 syncAdapterUid,
            /* [in] */ CSyncManager* syncMgr);

        ~ActiveSyncContext();

        CAR_INTERFACE_DECL()

        CARAPI SendHeartbeat();

        CARAPI OnFinished(
            /* [in] */ ISyncResult* result);

        CARAPI ToString(
            /* [in] */ IStringBuilder* sb);

        CARAPI OnServiceConnected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI OnServiceDisconnected(
            /* [in] */ IComponentName* name);

        CARAPI BindToSyncAdapter(
            /* [in] */ RegisteredServicesCache::ServiceInfo* info,
            /* [in] */ Int32 userId,
            /* [out] */ Boolean* result);

        CARAPI ToString(
            /* [out] */ String* str);

        CARAPI ProxyDied();

    protected:
        /**
         * Performs the required cleanup, which is the releasing of the wakelock and
         * unbinding from the sync adapter (if actually bound).
         */
        CARAPI Close();

    public:
        AutoPtr<ISyncOperation> mSyncOperation;
        Int64 mHistoryRowId;
        AutoPtr<IISyncAdapter> mSyncAdapter;
        Int64 mStartTime;
        Int64 mTimeoutStartTime;
        Boolean mBound;
        AutoPtr<IPowerManagerWakeLock> mSyncWakeLock;
        Int32 mSyncAdapterUid;
        AutoPtr<ISyncInfo> mSyncInfo;
        Boolean mIsLinkedToDeath;
        AutoPtr<CSyncManager> mSyncMgr;

    };

    class ServiceConnectionData
        : public IInterface
        , public ElRefBase
    {
    public:
        ServiceConnectionData(
            /* [in] */ ActiveSyncContext* syncContext,
            /* [in] */ IISyncAdapter* adapter);

        ~ServiceConnectionData();

        CAR_INTERFACE_DECL()

    public:
        AutoPtr<ActiveSyncContext> activeSyncContext;
        AutoPtr<IISyncAdapter> syncAdapter;

    };

    /**
     * Handles SyncOperation Messages that are posted to the associated
     * HandlerThread.
     */
    class SyncHandler
        : public HandlerBase
    {
    public:
        friend class ActiveSyncContext;
        friend class CSyncManager;

        /**
         * Used to keep track of whether a sync notification is active and who it is for.
         */
        class SyncNotificationInfo
            : public ElRefBase
        {
        public:
            SyncNotificationInfo();

            ~SyncNotificationInfo();

            CAR_INTERFACE_DECL()

            CARAPI ToString(
                /* [in] */ IStringBuilder* sb);

            CARAPI ToString(
                /* [out] */ String* str);

        public:
            // true iff the notification manager has been asked to send the notification
            Boolean isActive;

            // Set when we transition from not running a sync to running a sync, and cleared on
            // the opposite transition.
            Int64 startTime;
        };

    public:
        SyncHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CSyncManager* syncMgr);

        ~SyncHandler();

        CARAPI OnBootCompleted();

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI InsertStartSyncEvent(
            /* [in] */ ISyncOperation* syncOperation,
            /* [out] */ Int64* result);

        CARAPI StopSyncEvent(
            /* [in] */ Int64 rowId,
            /* [in] */ ISyncOperation* syncOperation,
            /* [in] */ const String& resultMessage,
            /* [in] */ Int32 upstreamActivity,
            /* [in] */ Int32 downstreamActivity,
            /* [in] */ Int64 elapsedTime);

    private:
        CARAPI GetSyncWakeLock(
            /* [in] */ IAccount* account,
            /* [in] */ const String& authority,
            /* [out] */ IPowerManagerWakeLock** wakeLock);

        CARAPI WaitUntilReadyToRun();

        /**
         * Turn any periodic sync operations that are ready to run into pending sync operations.
         * @return the desired start time of the earliest future  periodic sync operation,
         * in milliseconds since boot
         */
        CARAPI ScheduleReadyPeriodicSyncs(
            /* [out] */ Int64* startTime);

        CARAPI MaybeStartNextSyncLocked(
            /* [out] */ Int64* nextTime);

        CARAPI DispatchSyncOperation(
            /* [in] */ ISyncOperation* op,
            /* [out] */ Boolean* result);

        CARAPI RunBoundToSyncAdapter(
            /* [in] */ ActiveSyncContext* activeSyncContext,
            /* [in] */ IISyncAdapter* syncAdapter);

        CARAPI CancelActiveSyncLocked(
            /* [in] */ IAccount* account,
            /* [in] */ Int32 userId,
            /* [in] */ const String& authority);

        CARAPI RunSyncFinishedOrCanceledLocked(
            /* [in] */ ISyncResult* syncResult,
            /* [in] */ ActiveSyncContext* activeSyncContext);

        CARAPI CloseActiveSyncContext(
            /* [in] */ ActiveSyncContext* activeSyncContext);

        /**
         * Convert the error-containing SyncResult into the Sync.History error number. Since
         * the SyncResult may indicate multiple errors at once, this method just returns the
         * most "serious" error.
         * @param syncResult the SyncResult from which to read
         * @return the most "serious" error set in the SyncResult
         * @throws IllegalStateException if the SyncResult does not indicate any errors.
         *   If SyncResult.error() is true then it is safe to call this.
         */
        CARAPI SyncResultToErrorNumber(
            /* [in] */ ISyncResult* syncResult,
            /* [out] */ Int32* errorNumber);

        CARAPI ManageSyncNotificationLocked();

        CARAPI ManageSyncAlarmLocked(
            /* [in] */ Int64 nextPeriodicEventElapsedTime,
            /* [in] */ Int64 nextPendingEventElapsedTime);

        CARAPI SendSyncStateIntent();

        CARAPI InstallHandleTooManyDeletesNotification(
            /* [in] */ IAccount* account,
            /* [in] */ const String& authority,
            /* [in] */ Int64 numDeletes,
            /* [in] */ Int32 userId);

        /**
         * Checks whether an activity exists on the system image for the given intent.
         *
         * @param intent The intent for an activity.
         * @return Whether or not an activity exists.
         */
        CARAPI IsActivityAvailable(
            /* [in] */ IIntent* intent,
            /* [out] */ Boolean* isAvailable);

        CARAPI HandleCancleMessage();
        CARAPI HandleSyncFinishedMessage();
        CARAPI HandleServiceConnectedMessage();
        CARAPI HandleServiceDisconnectedMessage();
        CARAPI HandleSyncAlarmMessage();
        CARAPI HandleCheckAlarmsMessage();

    public:
        AutoPtr<SyncNotificationInfo> mSyncNotificationInfo;
        AutoPtr<SyncTimeTracker> mSyncTimeTracker;

    private:
        // Messages that can be sent on mHandler
        static const Int32 MESSAGE_SYNC_FINISHED = 1;
        static const Int32 MESSAGE_SYNC_ALARM = 2;
        static const Int32 MESSAGE_CHECK_ALARMS = 3;
        static const Int32 MESSAGE_SERVICE_CONNECTED = 4;
        static const Int32 MESSAGE_SERVICE_DISCONNECTED = 5;
        static const Int32 MESSAGE_CANCEL = 6;

    private:
        Int64 mAlarmScheduleTime;
        HashMap<AutoPtr<IPair>, AutoPtr<IPowerManagerWakeLock> > mWakeLocks;
        /* volatile */ AutoPtr<ICountDownLatch> mReadyToRunLatch;
        AutoPtr<CSyncManager> mSyncMgr;

    };

private:
    class AuthoritySyncStats
        : public ElRefBase
    {
    public:
        AuthoritySyncStats(
            /* [in] */ const String& name);

        ~AuthoritySyncStats();

        CAR_INTERFACE_DECL()

    public:
        String name;
        Int64 elapsedTime;
        Int32 times;
        HashMap<String, AutoPtr<AccountSyncStats> > mAccountMap;

    };

    class AccountSyncStats
        : public ElRefBase
    {
    public:
        AccountSyncStats(
            /* [in] */ const String& name);

        ~AccountSyncStats();

        CAR_INTERFACE_DECL()

    public:
        String name;
        Int64 elapsedTime;
        Int32 times;

    };

    /**
     * A helper object to keep track of the time we have spent syncing since the last boot
     */
    class SyncTimeTracker
        : public ElRefBase
    {
    public:
        SyncTimeTracker(
            /* [in] */ CSyncManager* syncMgr);

        ~SyncTimeTracker();

        CAR_INTERFACE_DECL()

        /** Call to let the tracker know that the sync state may have changed */
        CARAPI Update();

        /** Get how long we have been syncing, in ms */
        CARAPI TimeSpentSyncing(
            /* [out] */ Int64* syncTime);

    public:
        /** True if a sync was in progress on the most recent call to update() */
        Boolean mLastWasSyncing;
        /** Used to track when lastWasSyncing was last set */
        Int64 mWhenSyncStarted;

    private:
        /** The cumulative time we have spent syncing */
        Int64 mTimeSpentSyncing;
        Mutex mSyncTimeTrackerLock;
        AutoPtr<CSyncManager> mSyncMgr;

    };

    class StorageIntentReceiver
        : public BroadcastReceiver
    {
    public:
        StorageIntentReceiver(
            /* [in] */ CSyncManager* syncMgr);

        ~StorageIntentReceiver();

        CARAPI Initialize();

    protected:
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("StorageIntentReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        AutoPtr<CSyncManager> mSyncMgr;

    };

    class BootCompletedReceiver
        : public BroadcastReceiver
    {
    public:
        BootCompletedReceiver(
            /* [in] */ CSyncManager* syncMgr);

        ~BootCompletedReceiver();

        CARAPI Initialize();

    protected:
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("BootCompletedReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        AutoPtr<CSyncManager> mSyncMgr;

    };

    class BackgroundDataSettingChanged
        : public BroadcastReceiver
    {
    public:
        BackgroundDataSettingChanged(
            /* [in] */ CSyncManager* syncMgr);

        ~BackgroundDataSettingChanged();

        CARAPI Initialize();

    protected:
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("BackgroundDataSettingChanged:");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        AutoPtr<CSyncManager> mSyncMgr;

    };

    class AccountsUpdatedReceiver
        : public BroadcastReceiver
    {
    public:
        AccountsUpdatedReceiver(
            /* [in] */ CSyncManager* syncMgr);

        ~AccountsUpdatedReceiver();

        CARAPI Initialize();

    protected:
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("AccountsUpdatedReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        AutoPtr<CSyncManager> mSyncMgr;

    };

    class ConnectivityIntentReceiver
        : public BroadcastReceiver
    {
    public:
        ConnectivityIntentReceiver(
            /* [in] */ CSyncManager* syncMgr);

        ~ConnectivityIntentReceiver();

        CARAPI Initialize();

    protected:
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("ConnectivityIntentReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        AutoPtr<CSyncManager> mSyncMgr;

    };

    class ShutdownIntentReceiver
        : public BroadcastReceiver
    {
    public:
        ShutdownIntentReceiver(
            /* [in] */ CSyncManager* syncMgr);

        ~ShutdownIntentReceiver();

        CARAPI Initialize();

    protected:
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("ShutdownIntentReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        AutoPtr<CSyncManager> mSyncMgr;

    };

    class UserIntentReceiver
        : public BroadcastReceiver
    {
    public:
        UserIntentReceiver(
            /* [in] */ CSyncManager* syncMgr);

        ~UserIntentReceiver();

        CARAPI Initialize();

    protected:
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("UserIntentReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        AutoPtr<CSyncManager> mSyncMgr;

    };

    class SyncStatusObserver
        : public ElRefBase
        , public IISyncStatusObserver
    {
    public:
        SyncStatusObserver(
            /* [in] */ CSyncManager* syncMgr);

        ~SyncStatusObserver();

        CAR_INTERFACE_DECL()

        CARAPI OnStatusChanged(
            /* [in] */ Int32 which);

    private:
        AutoPtr<CSyncManager> mSyncMgr;

    };

    class SyncRequestListener
        : public ElRefBase
        , public ISyncStorageEngineOnSyncRequestListener
    {
    public:
        SyncRequestListener(
            /* [in] */ CSyncManager* syncMgr);

        ~SyncRequestListener();

        CAR_INTERFACE_DECL()

        CARAPI OnSyncRequest(
            /* [in] */ IAccount* account,
            /* [in] */ Int32 userId,
            /* [in] */ const String& authority,
            /* [in] */ IBundle* extras);

    private:
        AutoPtr<CSyncManager> mSyncMgr;

    };

public:

    CSyncManager();

    ~CSyncManager();

    CARAPI UpdateRunningAccounts();

    CARAPI GetSyncStorageEngine(
        /* [out] */ ISyncStorageEngine** engine);

    /**
     * Initiate a sync. This can start a sync for all providers
     * (pass null to url, set onlyTicklable to false), only those
     * providers that are marked as ticklable (pass null to url,
     * set onlyTicklable to true), or a specific provider (set url
     * to the content url of the provider).
     *
     * <p>If the ContentResolver.SYNC_EXTRAS_UPLOAD boolean in extras is
     * true then initiate a sync that just checks for local changes to send
     * to the server, otherwise initiate a sync that first gets any
     * changes from the server before sending local changes back to
     * the server.
     *
     * <p>If a specific provider is being synced (the url is non-null)
     * then the extras can contain SyncAdapter-specific information
     * to control what gets synced (e.g. which specific feed to sync).
     *
     * <p>You'll start getting callbacks after this.
     *
     * @param requestedAccount the account to sync, may be null to signify all accounts
     * @param userId the id of the user whose accounts are to be synced. If userId is USER_ALL,
     *          then all users' accounts are considered.
     * @param requestedAuthority the authority to sync, may be null to indicate all authorities
     * @param extras a Map of SyncAdapter-specific information to control
     *          syncs of a specific provider. Can be null. Is ignored
     *          if the url is null.
     * @param delay how many milliseconds in the future to wait before performing this
     * @param onlyThoseWithUnkownSyncableState
     */
    CARAPI ScheduleSync(
        /* [in] */ IAccount* requestedAccount,
        /* [in] */ Int32 userId,
        /* [in] */ const String& requestedAuthority,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 delay,
        /* [in] */ Boolean onlyThoseWithUnkownSyncableState);

    CARAPI ScheduleLocalSync(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authority);

    CARAPI GetSyncAdapterTypes(
        /* [in] */ Int32 userId,
        /* [out, callee] */ ArrayOf<ISyncAdapterType *>** syncAdapterTypes);

    /**
     * Cancel the active sync if it matches the authority and account.
     * @param account limit the cancelations to syncs with this account, if non-null
     * @param authority limit the cancelations to syncs with this authority, if non-null
     */
    CARAPI CancelActiveSync(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authority);

    /**
     * Create and schedule a SyncOperation.
     *
     * @param syncOperation the SyncOperation to schedule
     */
    CARAPI ScheduleSyncOperation(
        /* [in] */ ISyncOperation* syncOperation);

    /**
     * Remove scheduled sync operations.
     * @param account limit the removals to operations with this account, if non-null
     * @param authority limit the removals to operations with this authority, if non-null
     */
    CARAPI ClearScheduledSyncOperations(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authority);

    /**
     * Should only be created after {@link ContentService#systemReady()} so that
     * {@link PackageManager} is ready to query.
     */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Boolean factoryTest);

protected:
    CARAPI Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw);

    CARAPI DumpSyncState(
        /* [in] */ IPrintWriter* pw);

    CARAPI DumpSyncHistory(
        /* [in] */ IPrintWriter* pw);

private:
    static FormatTime(
        /* [in] */ Int64 time,
        /* [out] */ String* formatTime);

    static CARAPI_(Boolean) InitStaticMembers();

private:
    CARAPI GetAllUsers(
        /* [out, callee] */ List<AutoPtr<IUserInfo> >** allUsers);

    CARAPI ContainsAccountAndUser(
        /* [in] */ ArrayOf<IAccountAndUser *>* accounts,
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [out] */ Boolean* result);

    CARAPI DoDatabaseCleanup();

    CARAPI ReadDataConnectionState(
        /* [out] */ Boolean* result);

    CARAPI GetConnectivityManager(
        /* [out] */ IConnectivityManager** connManager);

    /**
     * Return a random value v that satisfies minValue <= v < maxValue. The difference between
     * maxValue and minValue must be less than Integer.MAX_VALUE.
     */
    CARAPI Jitterize(
        /* [in] */ Int64 minValue,
        /* [in] */ Int64 maxValue,
        /* [out] */ Int64* value);

    CARAPI EnsureAlarmService();

    CARAPI SendSyncAlarmMessage();

    CARAPI SendCheckAlarmsMessage();

    CARAPI SendSyncFinishedOrCanceledMessage(
        /* [in] */ ActiveSyncContext* syncContext,
        /* [in] */ ISyncResult* syncResult);

    CARAPI SendCancelSyncsMessage(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authority);

    CARAPI ClearBackoffSetting(
        /* [in] */ ISyncOperation* op);

    CARAPI IncreaseBackoffSetting(
        /* [in] */ ISyncOperation* op);

    CARAPI SetDelayUntilTime(
        /* [in] */ ISyncOperation* op,
        /* [in] */ Int64 delayUntilSeconds);

    CARAPI OnUserStarting(
        /* [in] */ Int32 userId);

    CARAPI OnUserStopping(
        /* [in] */ Int32 userId);

    CARAPI OnUserRemoved(
        /* [in] */ Int32 userId);

    CARAPI GetLastFailureMessage(
        /* [in] */ Int32 code,
        /* [out] */ String* failureMessage);

    CARAPI DumpTimeSec(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int64 time);

    CARAPI DumpDayStatistic(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ISyncStorageEngineDayStats* ds);

    CARAPI DumpRecentHistory(
        /* [in] */ IPrintWriter* pw);

    CARAPI DumpDayStatistics(
        /* [in] */ IPrintWriter* pw);

//    CARAPI DumpSyncAdapters(
//        /* [in] */ IIndentingPrintWriter* pw);

    CARAPI IsSyncStillActive(
        /* [in] */ ActiveSyncContext* activeSyncContext,
        /* [out] */ Boolean* result);

    CARAPI MaybeRescheduleSync(
        /* [in] */ ISyncResult* syncResult,
        /* [in] */ ISyncOperation* operation);

protected:
    AutoPtr< List<AutoPtr<ActiveSyncContext> > > mActiveSyncContexts;

    AutoPtr<SyncAdaptersCache> mSyncAdapters;

private:
    static const String TAG;
    /** Delay a sync due to local changes this long. In milliseconds */
    static const Int64 LOCAL_SYNC_DELAY;
    /**
     * If a sync takes longer than this and the sync queue is not empty then we will
     * cancel it and add it back to the end of the sync queue. In milliseconds.
     */
    static const Int64 MAX_TIME_PER_SYNC;
    static const Int64 SYNC_NOTIFICATION_DELAY;
    /**
     * When retrying a sync for the first time use this delay. After that
     * the retry time will double until it reached MAX_SYNC_RETRY_TIME.
     * In milliseconds.
     */
    static const Int64 INITIAL_SYNC_RETRY_TIME_IN_MS = 30 * 1000; // 30 seconds

    /**
     * Default the max sync retry time to this value.
     */
    static const Int64 DEFAULT_MAX_SYNC_RETRY_TIME_IN_SECONDS = 60 * 60; // one hour

    /**
     * How long to wait before retrying a sync that failed due to one already being in progress.
     */
    static const Int32 DELAY_RETRY_SYNC_IN_PROGRESS_IN_SECONDS = 10;

    static const Int32 INITIALIZATION_UNBIND_DELAY_MS = 5000;

    static const String SYNC_WAKE_LOCK_PREFIX;
    static const String HANDLE_SYNC_ALARM_WAKE_LOCK ;
    static const String SYNC_LOOP_WAKE_LOCK;

    static const Int32 MAX_SIMULTANEOUS_REGULAR_SYNCS;
    static const Int32 MAX_SIMULTANEOUS_INITIALIZATION_SYNCS;
    static /* const */ AutoPtr<ArrayOf<IAccountAndUser *> > INITIAL_ACCOUNTS_ARRAY;
    static const Int64 SYNC_ALARM_TIMEOUT_MIN = 30 * 1000; // 30 seconds
    static const Int64 SYNC_ALARM_TIMEOUT_MAX = 2 * 60 * 60 * 1000; // two hours
    static const String ACTION_SYNC_ALARM;
    static Boolean sIsStaticMemberInitialized;
    static Boolean sIsLargeRAM;

private:
    AutoPtr<IContext> mContext;
    // TODO: add better locking around mRunningAccounts
    /* volatile */ AutoPtr<ArrayOf<IAccountAndUser *> > mRunningAccounts;

    /* volatile */ AutoPtr<IPowerManagerWakeLock> mHandleAlarmWakeLock;
    /* volatile */ AutoPtr<IPowerManagerWakeLock> mSyncManagerWakeLock;
    /* volatile */ Boolean mDataConnectionIsConnected;
    /* volatile */ Boolean mStorageIsLow;

    AutoPtr<INotificationManager> mNotificationMgr;
    AutoPtr<IAlarmManager> mAlarmService;
    AutoPtr<ISyncStorageEngine> mSyncStorageEngine;

    // @GuardedBy("mSyncQueue")
    AutoPtr<ISyncQueue> mSyncQueue;

    // set if the sync active indicator should be reported
    Boolean mNeedSyncActiveNotification;

    AutoPtr<IPendingIntent> mSyncAlarmIntent;
    // Synchronized on "this". Instead of using this directly one should instead call
    // its accessor, getConnManager().
    AutoPtr<IConnectivityManager> mConnManagerDoNotUseDirectly;

    AutoPtr<IBroadcastReceiver> mStorageIntentReceiver;
    AutoPtr<IBroadcastReceiver> mBootCompletedReceiver;
    AutoPtr<IBroadcastReceiver> mBackgroundDataSettingChanged;
    AutoPtr<IBroadcastReceiver> mAccountsUpdatedReceiver;
    AutoPtr<IPowerManager> mPowerManager;

    // Use this as a random offset to seed all periodic syncs
    Int32 mSyncRandomOffsetMillis;

    AutoPtr<IUserManager> mUserManager;
    AutoPtr<IBroadcastReceiver> mConnectivityIntentReceiver;
    AutoPtr<IBroadcastReceiver> mShutdownIntentReceiver;
    AutoPtr<IBroadcastReceiver> mUserIntentReceiver;
    AutoPtr<SyncHandler> mSyncHandler;

    /* volatile */ Boolean mBootCompleted;

    Mutex mSyncQueueLock;
    Mutex mCSyncManagerLock;
};

}
}
}

#endif // __CSYNCMANAGER_H__
