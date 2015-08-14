
#ifndef __ELASTOS_DROID_CONTENT_CSYNCSTORAGEENGINE_H__
#define __ELASTOS_DROID_CONTENT_CSYNCSTORAGEENGINE_H__

#include "_CSyncStorageEngine.h"
#include <ext/frameworkext.h>
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashMap.h>
#include "SparseArray.h"
#include "os/Handler.h"
#include "CSyncStorageEngineAuthorityInfo.h"
#include "CSyncStorageEngineHelper.h"
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>


using namespace Elastos::Core;
using Elastos::Droid::Accounts::IAccount;
using Elastos::Droid::Accounts::IAccountAndUser;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::IRemoteCallbackList;
using Elastos::Droid::Utility::IPair;
using Elastos::Droid::Utility::SparseArray;
using Elastos::IO::IFile;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::List;
using Org::Xmlpull::V1::IXmlPullParser;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CSyncStorageEngine), public Handler
{
    class AccountInfo
        : public ElRefBase
    {
    public:
        AccountInfo(
            /* [in] */ IAccountAndUser* accountAndUser);

        ~AccountInfo();

        CAR_INTERFACE_DECL()

    public:
        AutoPtr<IAccountAndUser> mAccountAndUser;
        AutoPtr<HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> > > mAuthorities;

    };

friend class CSyncStorageEngineAuthorityInfo;
friend class CSyncStorageEngineHelper;

public:
    IHANDLER_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CSyncStorageEngine();

    ~CSyncStorageEngine();

    CARAPI HandleMessage(
        /* [in] */ IMessage* msg);

    CARAPI GetSyncRandomOffset(
        /* [out] */ Int32* syncRandomOffset);

    CARAPI AddStatusChangeListener(
        /* [in] */ Int32 mask,
        /* [in] */ IISyncStatusObserver* syncStatusObserver);

    CARAPI RemoveStatusChangeListener(
        /* [in] */ IISyncStatusObserver* syncStatusObserver);

    CARAPI GetSyncAutomatically(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& providerName,
        /* [out] */ Boolean* result);

    CARAPI SetSyncAutomatically(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& providerName,
        /* [in] */ Boolean sync);

    CARAPI GetIsSyncable(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& providerName,
        /* [out] */ Int32* result);

    CARAPI SetIsSyncable(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& providerName,
        /* [in] */ Int32 syncable);

    CARAPI GetBackoff(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& providerName,
        /* [out] */ IPair** pair);

    CARAPI SetBackoff(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& providerName,
        /* [in] */ Int64 nextSyncTime,
        /* [in] */ Int64 nextDelay);

    CARAPI ClearAllBackoffs(
        /* [in] */ ISyncQueue* syncQueue);

    CARAPI SetDelayUntilTime(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& providerName,
        /* [in] */ Int64 delayUntil);

    CARAPI GetDelayUntilTime(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& providerName,
        /* [out] */ Int64* delayUntilTime);

    CARAPI AddPeriodicSync(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& providerName,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 pollFrequency);

    CARAPI RemovePeriodicSync(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& providerName,
        /* [in] */ IBundle* extras);

    CARAPI GetPeriodicSyncs(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& providerName,
        /* [out] */ IObjectContainer** periodicSyncList);

    CARAPI SetMasterSyncAutomatically(
        /* [in] */ Boolean flag,
        /* [in] */ Int32 userId);

    CARAPI GetMasterSyncAutomatically(
        /* [in] */ Int32 userId,
        /* [out] */ Boolean* result);

    CARAPI GetOrCreateAuthority(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authority,
        /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo);

    CARAPI RemoveAuthority(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authority);

    CARAPI GetAuthority(
        /* [in] */ Int32 authorityId,
        /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo);

    /**
     * Returns true if there is currently a sync operation for the given
     * account or authority actively being processed.
     */
    CARAPI IsSyncActive(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authority,
        /* [out] */ Boolean* isActive);

    CARAPI InsertIntoPending(
        /* [in] */ ISyncStorageEnginePendingOperation* op,
        /* [out] */ ISyncStorageEnginePendingOperation** pendingOperation);

    CARAPI DeleteFromPending(
        /* [in] */ ISyncStorageEnginePendingOperation* op,
        /* [out] */ Boolean* result);

    /**
     * Return a copy of the current array of pending operations.  The
     * PendingOperation objects are the real objects stored inside, so that
     * they can be used with deleteFromPending().
     */
    CARAPI GetPendingOperations(
        /* [out] */ IObjectContainer** pendingOperationList);

    /**
     * Return the number of currently pending operations.
     */
    CARAPI GetPendingOperationCount(
        /* [out] */ Int32 * pCount);

    /**
     * Called when the set of account has changed, given the new array of
     * active accounts.
     */
    CARAPI DoDatabaseCleanup(
        /* [in] */ ArrayOf<IAccount *>* accounts,
        /* [in] */ Int32 userId);

    /**
     * Called when a sync is starting. Supply a valid ActiveSyncContext with information
     * about the sync.
     */
    CARAPI AddActiveSync(
        /* [in] */ ISyncOperation* activeSyncContextSyncOperation,
        /* [in] */ Int64 activeSyncContextStartTime,
        /* [out] */ ISyncInfo** syncInfo);

    /**
     * Called to indicate that a previously active sync is no longer active.
     */
    CARAPI RemoveActiveSync(
        /* [in] */ ISyncInfo* syncInfo,
        /* [in] */ Int32 userId);

    /**
     * To allow others to send active change reports, to poke clients.
     */
    CARAPI ReportActiveChange();

    /**
     * Note that sync has started for the given account and authority.
     */
    CARAPI InsertStartSyncEvent(
        /* [in] */ IAccount* accountName,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authorityName,
        /* [in] */ Int64 now,
        /* [in] */ Int32 source,
        /* [in] */ Boolean initialization,
        /* [out] */ Int64* result);

    CARAPI StopSyncEvent(
        /* [in] */ Int64 historyId,
        /* [in] */ Int64 elapsedTime,
        /* [in] */ const String& resultMessage,
        /* [in] */ Int64 downstreamActivity,
        /* [in] */ Int64 upstreamActivity);

    /**
     * Return a list of the currently active syncs. Note that the returned items are the
     * real, live active sync objects, so be careful what you do with it.
     */
    CARAPI GetCurrentSyncs(
        /* [in] */ Int32 userId,
        /* [out] */ IObjectContainer** syncInfoList);

    /**
     * Return an array of the current sync status for all authorities.  Note
     * that the objects inside the array are the real, live status objects,
     * so be careful what you do with them.
     */
    CARAPI GetSyncStatus(
        /* [out] */ IObjectContainer** syncStatusInfoList);

    /**
     * Return an array of the current authorities. Note
     * that the objects inside the array are the real, live objects,
     * so be careful what you do with them.
     */
    CARAPI GetAuthorities(
        /* [out] */ IObjectContainer** authorityInfoList);

    /**
     * Returns the status that matches the authority and account.
     *
     * @param account the account we want to check
     * @param authority the authority whose row should be selected
     * @return the SyncStatusInfo for the authority
     */
    CARAPI GetStatusByAccountAndAuthority(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authority,
        /* [out] */ ISyncStatusInfo** result);

    /**
     * Return true if the pending status is true of any matching authorities.
     */
    CARAPI IsSyncPending(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authority,
        /* [out] */ Boolean* isPending);

    /**
     * Return an array of the current sync status for all authorities.  Note
     * that the objects inside the array are the real, live status objects,
     * so be careful what you do with them.
     */
    CARAPI GetSyncHistory(
        /* [out] */ IObjectContainer** syncHistoryItemList);

    /**
     * Return an array of the current per-day statistics.  Note
     * that the objects inside the array are the real, live status objects,
     * so be careful what you do with them.
     */
    CARAPI GetDayStatistics(
        /* [out, callee] */ ArrayOf<ISyncStorageEngineDayStats *>** dayStats);

    CARAPI GetOrCreateSyncStatus(
        /* [in] */ ISyncStorageEngineAuthorityInfo* authority,
        /* [out] */ ISyncStatusInfo** syncStatus);

    CARAPI WriteAllState();

    /**
     * public for testing
     */
    CARAPI ClearAndReadState();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IFile* dataDir);

public:
    CARAPI SetOnSyncRequestListener(
        /* [in] */ ISyncStorageEngineOnSyncRequestListener* listener);

public:
    static CARAPI GetIntColumn(
        /* [in] */ ICursor* cursor,
        /* [in] */ const String& name,
        /* [out] */ Int32* result);

    static CARAPI GetLongColumn(
        /* [in] */ ICursor* cursor,
        /* [in] */ const String& name,
        /* [out] */ Int64* result);

private:
    static CARAPI FlattenBundle(
        /* [in] */ IBundle* bundle,
        /* [out, callee] */ ArrayOf<Byte>** flattenBundleArray);

    static CARAPI UnflattenBundle(
        /* [in] */ ArrayOf<Byte>* flatData,
        /* [out] */ IBundle** bundle);

    static CARAPI_(Boolean) InitStaticBlock();

private:
    CARAPI ReportChange(
        /* [in] */ Int32 which);

    CARAPI UpdateOrRemovePeriodicSync(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& providerName,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 period,
        /* [in] */ Boolean add);

    CARAPI GetCurrentDayLocked(
        /* [out] */ Int32* result);

    /**
     * Retrieve an authority, returning null if one does not exist.
     *
     * @param accountName The name of the account for the authority.
     * @param authorityName The name of the authority itself.
     * @param tag If non-null, this will be used in a log message if the
     * requested authority does not exist.
     */
    CARAPI GetAuthorityLocked(
        /* [in] */ IAccount* accountName,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authorityName,
        /* [in] */ const String& tag,
        /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo);

    CARAPI GetOrCreateAuthorityLocked(
        /* [in] */ IAccount* accountName,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authorityName,
        /* [in] */ Int32 ident,
        /* [in] */ Boolean doWrite,
        /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo);

    CARAPI RemoveAuthorityLocked(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authorityName,
        /* [in] */ Boolean doWrite);

    CARAPI GetOrCreateSyncStatusLocked(
        /* [in] */ Int32 authorityId,
        /* [out] */ ISyncStatusInfo** syncStatusInfo);

    /**
     * Read all account information back in to the initial engine state.
     */
    CARAPI ReadAccountInfoLocked();

    /**
     * some authority names have changed. copy over their settings and delete the old ones
     * @return true if a change was made
     */
    CARAPI MaybeMigrateSettingsForRenamedAuthorities(
        /* [out] */ Boolean* result);

    CARAPI ParseListenForTickles(
        /* [in] */ IXmlPullParser* parser);

    CARAPI ParseAuthority(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ Int32 version,
        /* [out] */ ISyncStorageEngineAuthorityInfo** authority);

    CARAPI ParsePeriodicSync(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ ISyncStorageEngineAuthorityInfo* authority,
        /* [out] */ IPair** pair);

    CARAPI ParseExtra(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IPair* periodicSync);

    /**
     * Write all account information to the account file.
     */
    CARAPI WriteAccountInfoLocked();

    /**
     * Load sync engine state from the old syncmanager database, and then
     * erase it.  Note that we don't deal with pending operations, active
     * sync, or history.
     */
    CARAPI ReadAndDeleteLegacyAccountInfoLocked();

    /**
     * Read all sync status back in to the initial engine state.
     */
    CARAPI ReadStatusLocked();

    /**
     * Write all sync status to the sync status file.
     */
    CARAPI WriteStatusLocked();

    /**
     * Read all pending operations back in to the initial engine state.
     */
    CARAPI ReadPendingOperationsLocked();

    CARAPI WritePendingOperationLocked(
        /* [in] */ ISyncStorageEnginePendingOperation* op,
        /* [in] */ IParcel* out);

    /**
     * Write all currently pending ops to the pending ops file.
     */
    CARAPI WritePendingOperationsLocked();

    /**
     * Append the given operation to the pending ops file; if unable to,
     * write all pending ops.
     */
    CARAPI AppendPendingOperationLocked(
        /* [in] */ ISyncStorageEnginePendingOperation* op);

    CARAPI RequestSync(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authority,
        /* [in] */ IBundle* extras);

    /**
     * Read all sync statistics back in to the initial engine state.
     */
    CARAPI ReadStatisticsLocked();

    /**
     * Write all sync statistics to the sync status file.
     */
    CARAPI WriteStatisticsLocked();

private:
    static const String TAG;

    static const Boolean DEBUG_FILE;

    static const String XML_ATTR_NEXT_AUTHORITY_ID;
    static const String XML_ATTR_LISTEN_FOR_TICKLES;
    static const String XML_ATTR_SYNC_RANDOM_OFFSET;
    static const String XML_ATTR_ENABLED;
    static const String XML_ATTR_USER;
    static const String XML_TAG_LISTEN_FOR_TICKLES;

    static const Int64 DEFAULT_POLL_FREQUENCY_SECONDS;

    // @VisibleForTesting
    static const Int64 MILLIS_IN_4WEEKS;

    static const Int32 MSG_WRITE_STATUS;

    static const Int64 WRITE_STATUS_DELAY; // 10 minutes

    static const Int32 MSG_WRITE_STATISTICS;

    static const Int64 WRITE_STATISTICS_DELAY; // 1/2 hour

    static const Boolean SYNC_ENABLED_DEFAULT;

    // the version of the accounts xml file format
    static const Int32 ACCOUNTS_VERSION;

    static AutoPtr<HashMap<String, String> > sAuthorityRenames;

    static Boolean sIsInitialization;

    static volatile ISyncStorageEngine* sSyncStorageEngine;

    static const Int32 PENDING_FINISH_TO_WRITE;

private:
    // Primary list of all syncable authorities.  Also our global lock.
    AutoPtr<SparseArray> mAuthorities;

    AutoPtr<HashMap<AutoPtr<IAccountAndUser>, AutoPtr<CSyncStorageEngine::AccountInfo> > > mAccounts;
    AutoPtr<List<AutoPtr<ISyncStorageEnginePendingOperation> > > mPendingOperations;
    typedef List<AutoPtr<ISyncInfo> > SyncInfoList;
    typedef SyncInfoList::Iterator SyncInfoListIterator;
    AutoPtr<HashMap<Int32, AutoPtr<SyncInfoList> > > mCurrentSyncs; // SparseArray<ArrayList<SyncInfo>>
    AutoPtr<SparseArray> mSyncStatus;
    AutoPtr<List<AutoPtr<ISyncStorageEngineSyncHistoryItem> > > mSyncHistory;
    AutoPtr<IRemoteCallbackList> mChangeListeners;
    Int32 mNextAuthorityId;

    // We keep 4 weeks of stats.
    AutoPtr<ArrayOf<ISyncStorageEngineDayStats*> > mDayStats;

    //TODO: private final Calendar mCal;

    Int32 mYear;
    Int32 mYearInDays;
    AutoPtr<IContext> mContext;
    Int32 mSyncRandomOffset;

    /**
     * This file contains the core engine state: all accounts and the
     * settings for them.  It must never be lost, and should be changed
     * infrequently, so it is stored as an XML file.
     */
    //TODO: private final AtomicFile mAccountInfoFile;

    /**
     * This file contains the current sync status.  We would like to retain
     * it across boots, but its loss is not the end of the world, so we store
     * this information as binary data.
     */
    //TODO: private final AtomicFile mStatusFile;

    /**
     * This file contains sync statistics.  This is purely debugging information
     * so is written infrequently and can be thrown away at any time.
     */
    //TODO: private final AtomicFile mStatisticsFile;

    /**
     * This file contains the pending sync operations.  It is a binary file,
     * which must be updated every time an operation is added or removed,
     * so we have special handling of it.
     */
    //TODO: private final AtomicFile mPendingFile;

    Int32 mNumPendingFinished;
    Int32 mNextHistoryId;
    AutoPtr<SparseArray> mMasterSyncAutomatically;
    Boolean mDefaultMasterSyncAutomatically;
    AutoPtr<ISyncStorageEngineOnSyncRequestListener> mSyncRequestListener;
    Object mAuthoritiesLock;
};

}
}
}

#endif // __ELASTOS_DROID_CONTENT_CSYNCSTORAGEENGINE_H__
