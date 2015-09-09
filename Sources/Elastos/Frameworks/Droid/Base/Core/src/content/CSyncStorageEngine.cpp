
#include "content/CContentResolverHelper.h"
#include "content/CIntent.h"
#include "content/CPeriodicSync.h"
#include "content/CSyncInfo.h"
#include "content/CSyncStatusInfo.h"
#include "content/CSyncStorageEngine.h"
#include "content/CSyncStorageEngineAuthorityInfo.h"
#include "content/CSyncStorageEngineDayStats.h"
#include "content/CSyncStorageEnginePendingOperation.h"
#include "content/CSyncStorageEngineSyncHistoryItem.h"
#include "util/CPair.h"
#include "os/CBundle.h"
//***#include "os/CParcel.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/Math.h>
#include <elastos/core/StringUtils.h>
#include <Elastos.CoreLibrary.h>

using Elastos::IO::CFile;
using Elastos::IO::IFile;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::IFileInputStream;
using Elastos::Droid::Os::CBundle;
//***using Elastos::Droid::Os::CParcel;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabase;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabaseHelper;
using Elastos::Droid::Database::Sqlite::ISQLiteQueryBuilder;
using Elastos::Droid::Utility::CPair;
using Elastos::Utility::Logging::Logger;
using Elastos::Core::CObjectContainer;
using Elastos::Core::StringUtils;
using Elastos::Utility::IObjectStringMap;
using Elastos::Utility::CObjectStringMap;
//***using Org::Xmlpull::V1::IXmlPullParserFactory;
using Org::Xmlpull::V1::IXmlSerializer;

_ETL_NAMESPACE_BEGIN
template<> struct Hash<AutoPtr<IAccountAndUser> > {
    size_t operator()(const AutoPtr<IAccountAndUser>& au) const
    {
        Int32 hashCode;
        au->GetHashCode(&hashCode);
        return (size_t)hashCode;
    }
};
_ETL_NAMESPACE_END

namespace Elastos {
namespace Droid {
namespace Content {

#define FAIL_DOFINAL(expr)                                 \
    do {                                           \
        ECode ec = expr;                           \
        if (FAILED(ec)) {                          \
            FAIL_RETURN(WriteAccountInfoLocked())  \
            FAIL_RETURN(WriteStatusLocked())       \
        }                                          \
    } while(0);

const String CSyncStorageEngine::TAG("SyncManager");
const Boolean CSyncStorageEngine::DEBUG_FILE = FALSE;
const String CSyncStorageEngine::XML_ATTR_NEXT_AUTHORITY_ID = String("nextAuthorityId");
const String CSyncStorageEngine::XML_ATTR_LISTEN_FOR_TICKLES = String("listen-for-tickles");
const String CSyncStorageEngine::XML_ATTR_SYNC_RANDOM_OFFSET = String("offsetInSeconds");
const String CSyncStorageEngine::XML_ATTR_ENABLED = String("enabled");
const String CSyncStorageEngine::XML_ATTR_USER = String("user");
const String CSyncStorageEngine::XML_TAG_LISTEN_FOR_TICKLES = String("listenForTickles");
const Int64 CSyncStorageEngine::DEFAULT_POLL_FREQUENCY_SECONDS = 60 * 60 * 24; // One day
const Int64 CSyncStorageEngine::MILLIS_IN_4WEEKS = 1000 * 60 * 60 * 24 * 7 * 4LL;
const Int32 CSyncStorageEngine::MSG_WRITE_STATUS = 1;
const Int64 CSyncStorageEngine::WRITE_STATUS_DELAY = 1000 * 60 * 10; // 10 minutes
const Int32 CSyncStorageEngine::MSG_WRITE_STATISTICS = 2;
const Int64 CSyncStorageEngine::WRITE_STATISTICS_DELAY = 1000 * 60 * 30; // 1/2 hour
const Boolean CSyncStorageEngine::SYNC_ENABLED_DEFAULT = FALSE;
const Int32 CSyncStorageEngine::ACCOUNTS_VERSION = 2;
const Int32 CSyncStorageEngine::PENDING_FINISH_TO_WRITE = 4;
AutoPtr<HashMap<String, String> > CSyncStorageEngine::sAuthorityRenames = new HashMap<String, String>();
Boolean CSyncStorageEngine::sIsInitialization = CSyncStorageEngine::InitStaticBlock();
volatile ISyncStorageEngine* CSyncStorageEngine::sSyncStorageEngine = NULL;

CSyncStorageEngine::AccountInfo::AccountInfo(
    /* [in] */ IAccountAndUser* accountAndUser)
    : mAccountAndUser(accountAndUser)
{
}

CSyncStorageEngine::AccountInfo::~AccountInfo()
{
}

CAR_INTERFACE_IMPL(CSyncStorageEngine::AccountInfo, IInterface)

IHANDLER_METHODS_IMPL(CSyncStorageEngine, Handler)

PInterface CSyncStorageEngine::Probe(
    /* [in] */ REIID riid)
{
    return _CSyncStorageEngine::Probe(riid);
}

CSyncStorageEngine::CSyncStorageEngine()
    : mAuthorities(NULL)
    , mSyncStatus(NULL)
    , mChangeListeners(NULL)
    , mNextAuthorityId(0)
    , mYear(0)
    , mYearInDays(0)
    , mContext(NULL)
    , mSyncRandomOffset(0)
    , mNumPendingFinished(0)
    , mNextHistoryId(0)
    , mMasterSyncAutomatically(NULL)
    , mDefaultMasterSyncAutomatically(FALSE)
    , mSyncRequestListener(NULL)
{
    mAuthorities = new SparseArray();
    mSyncStatus = new SparseArray();
//***    CRemoteCallbackList::New((IRemoteCallbackList**)&mChangeListeners);
    mDayStats = ArrayOf<ISyncStorageEngineDayStats*>::Alloc(7*4);
    mMasterSyncAutomatically = new SparseArray();
}

CSyncStorageEngine::~CSyncStorageEngine()
{
    mAuthorities = NULL;
    mAccounts->Clear();
    mPendingOperations->Clear();
    mCurrentSyncs->Clear();

    mSyncStatus = NULL;
    mSyncHistory->Clear();
    mMasterSyncAutomatically = NULL;
}

ECode CSyncStorageEngine::GetSyncRandomOffset(
    /* [out] */ Int32* syncRandomOffset)
{
    VALIDATE_NOT_NULL(syncRandomOffset)
    *syncRandomOffset = mSyncRandomOffset;
    return NOERROR;
}

ECode CSyncStorageEngine::AddStatusChangeListener(
    /* [in] */ Int32 mask,
    /* [in] */ IISyncStatusObserver* syncStatusObserver)
{
    AutoLock lock(mAuthoritiesLock);
    Boolean ret = FALSE;
    AutoPtr<IInteger32> maskObj;
    FAIL_RETURN(CInteger32::New(mask, (IInteger32**)&maskObj))
    FAIL_RETURN(mChangeListeners->Register((IInterface*) syncStatusObserver, (IInterface*) maskObj, &ret))
    return NOERROR;
}

ECode CSyncStorageEngine::RemoveStatusChangeListener(
    /* [in] */ IISyncStatusObserver* syncStatusObserver)
{
    AutoLock lock(mAuthoritiesLock);
    Boolean ret = FALSE;
    FAIL_RETURN(mChangeListeners->Unregister(syncStatusObserver, &ret))
    return NOERROR;
}

ECode CSyncStorageEngine::GetSyncAutomatically(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& providerName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    AutoLock lock(mAuthoritiesLock);

    if (NULL != account) {
        AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
        FAIL_RETURN(GetAuthorityLocked(account, userId, providerName, String("getSyncAutomatically"),
                (ISyncStorageEngineAuthorityInfo**)&authority))
        if (NULL != authority) {
            Boolean isEnable = FALSE;
            FAIL_RETURN(authority->GetEnabled(&isEnable))
            if (isEnable) {
                *result = TRUE;
                return NOERROR;
            }
        }
        *result = FALSE;
        return NOERROR;
    }

    Int32 i = mAuthorities->Size();
    AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
    String strAuthority;
    Int32 uId = 0;
    Boolean isEnable = FALSE;

    while (i > 0) {
        i--;
        authorityInfo = (ISyncStorageEngineAuthorityInfo*) mAuthorities->ValueAt(i);
        FAIL_RETURN(authorityInfo->GetAuthority(&strAuthority))
        FAIL_RETURN(authorityInfo->GetUserId(&uId))
        FAIL_RETURN(authorityInfo->GetEnabled(&isEnable))

        if (strAuthority.Equals(providerName) && uId == userId && isEnable) {
            *result = TRUE;
            return NOERROR;
        }

    }

    *result = FALSE;
    return NOERROR;
}

ECode CSyncStorageEngine::SetSyncAutomatically(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& providerName,
    /* [in] */ Boolean sync)
{
    String str("setSyncAutomatically: ");
    str.Append(", provider ");
    str.Append(providerName);
    str.Append(", user ");
    str.Append(StringUtils::Int32ToString(userId));
    str.Append(" -> ");
    str.Append(sync ? "true" : "false");
    Logger::D(TAG, str);

    {
        AutoLock lock(mAuthoritiesLock);
        AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
        FAIL_RETURN(GetOrCreateAuthorityLocked(account, userId, providerName, -1, FALSE,
                (ISyncStorageEngineAuthorityInfo**)&authorityInfo))
        Boolean isEnable = FALSE;
        FAIL_RETURN(authorityInfo->GetEnabled(&isEnable))

        if (isEnable == sync) {
            String info("setSyncAutomatically: already set to ");
            info.Append(sync ? "true" : "false");
            info.Append(", doing nothing");
            Logger::D(TAG, info);
            return NOERROR;
        }

        FAIL_RETURN(authorityInfo->SetEnabled(sync))
        FAIL_RETURN(WriteAccountInfoLocked())
    }

    if (sync) {
        AutoPtr<IBundle> bundle;
        FAIL_RETURN(CBundle::New((IBundle**)&bundle))
        FAIL_RETURN(RequestSync(account, userId, providerName, bundle))
    }

    FAIL_RETURN(ReportChange(IContentResolver::SYNC_OBSERVER_TYPE_SETTINGS))
    return NOERROR;
}

ECode CSyncStorageEngine::GetIsSyncable(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& providerName,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    AutoLock lock(mAuthoritiesLock);

    if (NULL != account) {
        AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
        FAIL_RETURN(GetAuthorityLocked(account, userId, providerName, String("getIsSyncable"),
                (ISyncStorageEngineAuthorityInfo**)&authority))
        if (NULL == authority) {
            *result = -1;
            return NOERROR;
        }
        Int32 syncable;
        FAIL_RETURN(authority->GetSyncable(&syncable))
        *result = syncable;
        return NOERROR;
    }

    Int32 i = mAuthorities->Size();
    AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
    String strAuthority;

    while (i > 0) {
        i--;
        authorityInfo = (ISyncStorageEngineAuthorityInfo*) mAuthorities->ValueAt(i);
        FAIL_RETURN(authorityInfo->GetAuthority(&strAuthority))

        if (strAuthority.Equals(providerName)) {
            Int32 syncable;
            FAIL_RETURN(authorityInfo->GetSyncable(&syncable))
            *result = syncable;
            return NOERROR;
        }
    }

    *result = -1;
    return NOERROR;
}

ECode CSyncStorageEngine::SetIsSyncable(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& providerName,
    /* [in] */ Int32 syncable)
{
    if (syncable > 1) {
        syncable = 1;
    }
    else if (syncable < -1) {
        syncable = -1;
    }

    String str("setIsSyncable: ");
    if (NULL != account) {
        String tmp;
        FAIL_RETURN(account->ToString(&tmp))
        str.Append(tmp);
    }
    else {
        str.Append(" null ");
    }
    str.Append(", provider ");
    str.Append(providerName);
    str.Append(", user ");
    str.Append(StringUtils::Int32ToString(userId));
    str.Append(" -> ");
    String strSyncable = StringUtils::ToString(syncable);
    str.Append(strSyncable);
    Logger::D(TAG, str);

    {
        AutoLock lock(mAuthoritiesLock);
        AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
        FAIL_RETURN(GetOrCreateAuthorityLocked(account, userId, providerName, -1, FALSE,
                (ISyncStorageEngineAuthorityInfo**)&authorityInfo))
        Int32 newSyncable = 0;
        FAIL_RETURN(authorityInfo->GetSyncable(&newSyncable))

        if (newSyncable == syncable) {
            String info("setIsSyncable: already set to ");
            info.Append(strSyncable);
            info.Append(", doing nothing");
            Logger::D(TAG, info);
            return NOERROR;
        }

        FAIL_RETURN(authorityInfo->SetSyncable(syncable))
        FAIL_RETURN(WriteAccountInfoLocked())
    }

    if (syncable > 0) {
        AutoPtr<IBundle> bundle;
        FAIL_RETURN(CBundle::New((IBundle**)&bundle))
        FAIL_RETURN(RequestSync(account, userId, providerName, bundle))
    }

    FAIL_RETURN(ReportChange(IContentResolver::SYNC_OBSERVER_TYPE_SETTINGS))
    return NOERROR;
}

ECode CSyncStorageEngine::GetBackoff(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& providerName,
    /* [out] */ IPair** pair)
{
    VALIDATE_NOT_NULL(pair)
    *pair = NULL;

    AutoLock lock(mAuthoritiesLock);
    AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
    FAIL_RETURN(GetAuthorityLocked(account, userId, providerName, String("getBackoff"),
            (ISyncStorageEngineAuthorityInfo**)&authority))
    Int64 backoffTime = 0;

    if (NULL == authority || (authority->GetBackoffTime(&backoffTime), backoffTime) < 0) {
        return NOERROR;
    }

    Int64 backoffDelay = 0;
    FAIL_RETURN(authority->GetBackoffDelay(&backoffDelay))
    AutoPtr<IInteger64> backoffTimeObj;
    AutoPtr<IInteger64> backoffDelayObj;
    FAIL_RETURN(CInteger64::New(backoffTime, (IInteger64**)&backoffTimeObj))
    FAIL_RETURN(CInteger64::New(backoffDelay, (IInteger64**)&backoffDelayObj))
    return CPair::New(backoffTimeObj, backoffDelayObj, pair);
}

ECode CSyncStorageEngine::SetBackoff(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& providerName,
    /* [in] */ Int64 nextSyncTime,
    /* [in] */ Int64 nextDelay)
{
    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        String str("setBackoff: ");
        if (NULL != account) {
            String tmp;
            FAIL_RETURN(account->ToString(&tmp))
            str.Append(tmp);
        }
        else {
            str.Append(" null ");
        }
        str.Append(", provider ");
        str.Append(providerName);
        str.Append(", user ");
        str.Append(StringUtils::Int32ToString(userId));
        str.Append(" -> nextSyncTime ");
        String strNextSyncTime = StringUtils::ToString(nextSyncTime);
        str.Append(strNextSyncTime);
        str.Append(", nextDelay ");
        String strNextDelay = StringUtils::ToString(nextDelay);
        str.Append(strNextDelay);
        Logger::V(TAG, str);
    }

    Boolean changed = FALSE;
    AutoLock lock(mAuthoritiesLock);

    if (NULL == account || providerName.IsNull()) {
        HashMap<AutoPtr<IAccountAndUser>, AutoPtr<CSyncStorageEngine::AccountInfo> >::Iterator it = mAccounts->Begin();
        AutoPtr<CSyncStorageEngine::AccountInfo> accountInfo;
        AutoPtr<IAccountAndUser> accountAndUser;
        AutoPtr<IAccount> act;
        Int32 uId = 0;

        for (; it != mAccounts->End(); ++it) {
            accountInfo = it->mSecond;
            accountAndUser = accountInfo->mAccountAndUser;
            act = NULL;
            FAIL_RETURN(accountAndUser->GetAccount((IAccount**)&act))
            FAIL_RETURN(accountAndUser->GetUserId(&uId))
            if (NULL != account && !_CObject_Compare(account, act) && userId != uId) continue;
            HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >::Iterator iter = accountInfo->mAuthorities->Begin();
            AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;

            for (; iter != accountInfo->mAuthorities->End(); ++iter) {
                authorityInfo = iter->mSecond;
                String authority;
                FAIL_RETURN(authorityInfo->GetAuthority(&authority))
                if (!providerName.IsNull() && !providerName.Equals(authority)) continue;
                Int64 backoffTime = 0;
                Int64 backoffDelay = 0;
                FAIL_RETURN(authorityInfo->GetBackoffTime(&backoffTime))
                FAIL_RETURN(authorityInfo->GetBackoffDelay(&backoffDelay))
                if (backoffTime != nextSyncTime || backoffDelay != nextDelay) {
                    FAIL_RETURN(authorityInfo->SetBackoffTime(nextSyncTime))
                    FAIL_RETURN(authorityInfo->SetBackoffDelay(nextDelay))
                    changed = TRUE;
                }
            }

        }

    }
    else {
        AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
        FAIL_RETURN(GetOrCreateAuthorityLocked(account, userId, providerName, -1, TRUE,
                (ISyncStorageEngineAuthorityInfo**)&authorityInfo))
        Int64 backoffTime = 0;
        Int64 backoffDelay = 0;
        FAIL_RETURN(authorityInfo->GetBackoffTime(&backoffTime))
        FAIL_RETURN(authorityInfo->GetBackoffDelay(&backoffDelay))

        if (backoffTime == nextSyncTime && backoffDelay == nextDelay) {
            return NOERROR;
        }

        FAIL_RETURN(authorityInfo->SetBackoffTime(nextSyncTime))
        FAIL_RETURN(authorityInfo->SetBackoffDelay(nextDelay))
        changed = TRUE;
    }

    if (changed) {
        FAIL_RETURN(ReportChange(IContentResolver::SYNC_OBSERVER_TYPE_SETTINGS))
    }

    return NOERROR;
}

ECode CSyncStorageEngine::ClearAllBackoffs(
    /* [in] */ ISyncQueue* syncQueue)
{
    Boolean changed = FALSE;
    {
        AutoLock lock(mAuthoritiesLock);
        Object syncQueueLock;
        {
            AutoLock lock(syncQueueLock);
            HashMap<AutoPtr<IAccountAndUser>, AutoPtr<CSyncStorageEngine::AccountInfo> >::Iterator it = mAccounts->Begin();
            AutoPtr<CSyncStorageEngine::AccountInfo> accountInfo;

            for (; it != mAccounts->End(); ++it) {
                accountInfo = it->mSecond;
                HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >::Iterator iter = accountInfo->mAuthorities->Begin();
                AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
                Int64 backoffTime = 0;
                Int64 backoffDelay = 0;

                for (; iter != accountInfo->mAuthorities->End(); ++iter) {
                    authorityInfo = iter->mSecond;
                    FAIL_RETURN(authorityInfo->GetBackoffTime(&backoffTime))
                    FAIL_RETURN(authorityInfo->GetBackoffDelay(&backoffDelay))

                    if(ISyncStorageEngine::NOT_IN_BACKOFF_MODE != backoffTime
                            || ISyncStorageEngine::NOT_IN_BACKOFF_MODE != backoffDelay) {
                        AutoPtr<IAccountAndUser> accountAndUser = accountInfo->mAccountAndUser;
                        AutoPtr<IAccount> account;
                        Int32 userId = 0;
                        String authority;
                        FAIL_RETURN(accountAndUser->GetAccount((IAccount**)&account))
                        FAIL_RETURN(accountAndUser->GetUserId(&userId))
                        FAIL_RETURN(authorityInfo->GetAuthority(&authority))

                        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                            String str("clearAllBackoffs:");
                            str.Append(" authority:");
                            str.Append(authority);
                            str.Append(" account:");
                            String accountName;
                            FAIL_RETURN(account->GetName(&accountName));
                            str.Append(accountName);
                            str.Append(" user:");
                            str.Append(StringUtils::Int32ToString(userId));
                            str.Append(" backoffTime was: ");
                            String strBackoffTime = StringUtils::ToString(backoffTime);
                            str.Append(strBackoffTime);
                            String strBackoffDelay = StringUtils::ToString(backoffDelay);
                            str.Append(strBackoffDelay);
                            Logger::V(TAG, str);
                        }

                        FAIL_RETURN(authorityInfo->SetBackoffTime(ISyncStorageEngine::NOT_IN_BACKOFF_MODE))
                        FAIL_RETURN(authorityInfo->SetBackoffDelay(ISyncStorageEngine::NOT_IN_BACKOFF_MODE))
                        FAIL_RETURN(syncQueue->OnBackoffChanged(account, userId, authority, 0))
                        changed = TRUE;
                    }

                }

            }
        }
    }

    if (changed) {
        FAIL_RETURN(ReportChange(IContentResolver::SYNC_OBSERVER_TYPE_SETTINGS))
    }

    return NOERROR;
}

ECode CSyncStorageEngine::SetDelayUntilTime(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& providerName,
    /* [in] */ Int64 delayUntil)
{
    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        String str("setDelayUntil: ");
        if (NULL != account) {
            String tmp;
            FAIL_RETURN(account->ToString(&tmp))
            str.Append(tmp);
        }
        else {
            str.Append(" null ");
        }
        str.Append(", provider ");
        str.Append(providerName);
        str.Append(", user ");
        str.Append(StringUtils::Int32ToString(userId));
        str.Append(" -> delayUntil ");
        str.Append(StringUtils::Int64ToString(delayUntil));
        Logger::V(TAG, str);
    }

    {
        AutoLock lock(mAuthoritiesLock);
        AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
        FAIL_RETURN(GetOrCreateAuthorityLocked(account, userId, providerName, -1 /* ident */, TRUE,
                (ISyncStorageEngineAuthorityInfo**)&authorityInfo))
        Int64 tmpDelayUntil = 0;
        FAIL_RETURN(authorityInfo->GetDelayUntil(&tmpDelayUntil))

        if (tmpDelayUntil == delayUntil) {
            return NOERROR;
        }

        FAIL_RETURN(authorityInfo->SetDelayUntil(delayUntil))
    }

    return ReportChange(IContentResolver::SYNC_OBSERVER_TYPE_SETTINGS);
}

ECode CSyncStorageEngine::GetDelayUntilTime(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& providerName,
    /* [out] */ Int64* delayUntilTime)
{
    VALIDATE_NOT_NULL(delayUntilTime);
    AutoLock lock(mAuthoritiesLock);
    AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
    FAIL_RETURN(GetAuthorityLocked(account, userId, providerName, String("getDelayUntil"),
            (ISyncStorageEngineAuthorityInfo**)&authority))

    if (NULL == authority) {
        *delayUntilTime = 0;
        return NOERROR;
    }

    Int64 delayUntil = 0;
    FAIL_RETURN(authority->GetDelayUntil(&delayUntil))
    *delayUntilTime = delayUntil;
    return NOERROR;
}

ECode CSyncStorageEngine::AddPeriodicSync(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& providerName,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 pollFrequency)
{
    return UpdateOrRemovePeriodicSync(account, userId, providerName, extras, pollFrequency, TRUE /* add */);
}

ECode CSyncStorageEngine::RemovePeriodicSync(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& providerName,
    /* [in] */ IBundle* extras)
{
    return UpdateOrRemovePeriodicSync(account, userId, providerName, extras, 0 /* period, ignored */, FALSE /* remove */);
}

ECode CSyncStorageEngine::GetPeriodicSyncs(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& providerName,
    /* [out] */ IObjectContainer** periodicSyncList)
{
    VALIDATE_NOT_NULL(periodicSyncList)
    AutoLock lock(mAuthoritiesLock);
    FAIL_RETURN(CObjectContainer::New(periodicSyncList))
    AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
    FAIL_RETURN(GetAuthorityLocked(account, userId, providerName, String("getPeriodicSyncs"),
            (ISyncStorageEngineAuthorityInfo**)&authority))

    if (NULL == authority) {
        AutoPtr<IObjectContainer> objContainer;
        AutoPtr<IObjectEnumerator> objEnumerator;
        FAIL_RETURN(authority->GetPeriodicSyncs((IObjectContainer**)&objContainer))
        if (NULL != objContainer) {
            FAIL_RETURN(objContainer->GetObjectEnumerator((IObjectEnumerator**)&objEnumerator))
            Boolean hasNext = FALSE;
            AutoPtr<IPair> pair;
            AutoPtr<IBundle> bundle;
            AutoPtr<IInteger64> secondObj;
            AutoPtr<IPeriodicSync> periodicSync;
            Int64 second = 0;

            while ((objEnumerator->MoveNext(&hasNext), hasNext)) {
                pair = NULL;
                bundle = NULL;
                secondObj = NULL;
                periodicSync = NULL;

                FAIL_RETURN(objEnumerator->Current((IInterface**)&pair))
                FAIL_RETURN(pair->GetFirst((IInterface**)&bundle))
                FAIL_RETURN(pair->GetSecond((IInterface**)&secondObj))
                FAIL_RETURN(secondObj->GetValue(&second))
                FAIL_RETURN(CPeriodicSync::New(account, providerName, bundle, second, (IPeriodicSync**)&periodicSync))
                FAIL_RETURN((*periodicSyncList)->Add(periodicSync))
            }

        }
    }

    return NOERROR;
}

ECode CSyncStorageEngine::SetMasterSyncAutomatically(
    /* [in] */ Boolean flag,
    /* [in] */ Int32 userId)
{
    {
        AutoLock lock(mAuthoritiesLock);
        AutoPtr<IBoolean> isAuto = (IBoolean*) mMasterSyncAutomatically->Get(userId);
        Boolean ret = FALSE;

        if (NULL != isAuto && (isAuto->GetValue(&ret), ret) == flag) {
            return NOERROR;
        }

        AutoPtr<IBoolean> flagObj;
        FAIL_RETURN(CBoolean::New(flag, (IBoolean**)&flagObj))
        mMasterSyncAutomatically->Put(userId, (IBoolean*) flagObj);
        FAIL_RETURN(WriteAccountInfoLocked())
    }

    if (flag) {
        AutoPtr<IBundle> bundle;
        FAIL_RETURN(CBundle::New((IBundle**)&bundle))
        FAIL_RETURN(RequestSync((IAccount*) NULL, userId, String(NULL), bundle))
    }

    FAIL_RETURN(ReportChange(IContentResolver::SYNC_OBSERVER_TYPE_SETTINGS))
    AutoPtr<IIntent> intent;
    String action("com.android.sync.SYNC_CONN_STATUS_CHANGED");
    FAIL_RETURN(CIntent::New(action, (IIntent**)&intent))
    FAIL_RETURN(mContext->SendBroadcast(intent))
    return NOERROR;
}

ECode CSyncStorageEngine::GetMasterSyncAutomatically(
    /* [in] */ Int32 userId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    AutoLock lock(mAuthoritiesLock);
    AutoPtr<IBoolean> isAuto = (IBoolean*) mMasterSyncAutomatically->Get(userId);

    if (NULL == isAuto) {
        *result = mDefaultMasterSyncAutomatically;
    }
    else {
        FAIL_RETURN(isAuto->GetValue(result))
    }
    return NOERROR;
}

ECode CSyncStorageEngine::GetOrCreateAuthority(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authority,
    /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo)
{
    VALIDATE_NOT_NULL(authorityInfo);
    AutoLock lock(mAuthoritiesLock);
    FAIL_RETURN(GetOrCreateAuthorityLocked(account, userId, authority, -1/* assign a new identifier if creating a new authority */,
            TRUE /* write to storage if this results in a change */, authorityInfo))
    return NOERROR;
}

ECode CSyncStorageEngine::RemoveAuthority(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authority)
{
    AutoLock lock(mAuthoritiesLock);
    FAIL_RETURN(RemoveAuthorityLocked(account, userId, authority, TRUE /* doWrite */))
    return NOERROR;
}

ECode CSyncStorageEngine::GetAuthority(
    /* [in] */ Int32 authorityId,
    /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo)
{
    VALIDATE_NOT_NULL(authorityInfo)
    AutoLock lock(mAuthoritiesLock);
    AutoPtr<ISyncStorageEngineAuthorityInfo> temp = (ISyncStorageEngineAuthorityInfo*) mAuthorities->Get(authorityId);
    *authorityInfo = temp;
    REFCOUNT_ADD(*authorityInfo);
    return NOERROR;
}

ECode CSyncStorageEngine::IsSyncActive(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authority,
    /* [out] */ Boolean* isActive)
{
    VALIDATE_NOT_NULL(isActive)
    AutoLock lock(mAuthoritiesLock);
    AutoPtr<IObjectContainer> objContainer;
    AutoPtr<IObjectEnumerator> objEnumerator;
    FAIL_RETURN(GetCurrentSyncs(userId, (IObjectContainer**)&objContainer))

    if (NULL != objContainer) {
        FAIL_RETURN(objContainer->GetObjectEnumerator((IObjectEnumerator**)&objEnumerator))
        Boolean hasNext = FALSE;
        AutoPtr<ISyncInfo> syncInfo;
        AutoPtr<ISyncStorageEngineAuthorityInfo> ainfo;
        Int32 authorityId = 0;
        while ((objEnumerator->MoveNext(&hasNext), hasNext)) {
            syncInfo = NULL;
            ainfo = NULL;
            FAIL_RETURN(objEnumerator->Current((IInterface**)&syncInfo))
            FAIL_RETURN(syncInfo->GetAuthorityId(&authorityId))
            FAIL_RETURN(GetAuthority(authorityId, (ISyncStorageEngineAuthorityInfo**)&ainfo))
            if (NULL != ainfo) {
                AutoPtr<IAccount> newAccount;
                String newAuthority;
                Int32 uId = 0;
                FAIL_RETURN(ainfo->GetAccount((IAccount**)&newAccount))
                FAIL_RETURN(ainfo->GetAuthority(&newAuthority))
                FAIL_RETURN(ainfo->GetUserId(&uId))
                if (_CObject_Compare(newAccount, account) && newAuthority.Equals(authority) && userId == uId) {
                    *isActive = TRUE;
                    return NOERROR;
                }
            }
        }
    }

    *isActive = FALSE;
    return NOERROR;
}

ECode CSyncStorageEngine::InsertIntoPending(
    /* [in] */ ISyncStorageEnginePendingOperation* op,
    /* [out] */ ISyncStorageEnginePendingOperation** pendingOperation)
{
    VALIDATE_NOT_NULL(pendingOperation)
    {
        AutoLock lock(mAuthoritiesLock);
        AutoPtr<IAccount> account;
        String authority;
        Int32 userId = 0;
        FAIL_RETURN(op->GetAccount((IAccount**)&account))
        FAIL_RETURN(op->GetAuthority(&authority))
        FAIL_RETURN(op->GetUserId(&userId))

        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
            Int32 syncSource = 0;
            AutoPtr<IBundle> extras;
            FAIL_RETURN(op->GetSyncSource(&syncSource))
            FAIL_RETURN(op->GetExtras((IBundle**)&extras))
            String strSyncSource = StringUtils::ToString(syncSource);
            String strExtras;
            FAIL_RETURN(extras->ToString(&strExtras))
            String str("insertIntoPending: account=");
            if (NULL != account) {
                String tmp;
                FAIL_RETURN(account->ToString(&tmp))
                str.Append(tmp);
            }
            else {
                str.Append(" null ");
            }
            str.Append(" user=");
            str.Append(StringUtils::Int32ToString(userId));
            str.Append(" auth=");
            str.Append(authority);
            str.Append(" src=");
            str.Append(strSyncSource);
            str.Append(" extras=");
            str.Append(strExtras);
            Logger::V(TAG, str);
        }

        AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
        FAIL_RETURN(GetOrCreateAuthorityLocked(account, userId, authority, -1 /* desired identifier */,
                TRUE /* write accounts to storage */, (ISyncStorageEngineAuthorityInfo**)&authorityInfo))

        if (NULL == authorityInfo) {
            *pendingOperation = NULL;
            return NOERROR;
        }

        FAIL_RETURN(CSyncStorageEnginePendingOperation::New(op, pendingOperation))
        Int32 ident = 0;
        FAIL_RETURN(authorityInfo->GetIdent(&ident))
        FAIL_RETURN((*pendingOperation)->SetAuthorityId(ident))
        mPendingOperations->PushBack(*pendingOperation);
        FAIL_RETURN(AppendPendingOperationLocked(*pendingOperation))
        AutoPtr<ISyncStatusInfo> statusInfo;
        FAIL_RETURN(GetOrCreateSyncStatusLocked(ident, (ISyncStatusInfo**)&statusInfo))
        FAIL_RETURN(statusInfo->SetPending(TRUE))
    }

    FAIL_RETURN(ReportChange(IContentResolver::SYNC_OBSERVER_TYPE_PENDING))
    return NOERROR;
}

ECode CSyncStorageEngine::DeleteFromPending(
    /* [in] */ ISyncStorageEnginePendingOperation* op,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    {
        AutoLock lock(mAuthoritiesLock);
        AutoPtr<IAccount> account;
        String authority;
        Int32 userId = 0;
        FAIL_RETURN(op->GetAccount((IAccount**)&account))
        FAIL_RETURN(op->GetAuthority(&authority))
        FAIL_RETURN(op->GetUserId(&userId))

        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
            Int32 syncSource = 0;
            AutoPtr<IBundle> extras;
            FAIL_RETURN(op->GetSyncSource(&syncSource))
            FAIL_RETURN(op->GetExtras((IBundle**)&extras))
            String strSyncSource = StringUtils::ToString(syncSource);
            String strExtras;
            FAIL_RETURN(extras->ToString(&strExtras))
            String str("deleteFromPending: account=");
            if (NULL != account) {
                String tmp;
                FAIL_RETURN(account->ToString(&tmp))
                str.Append(tmp);
            }
            else {
                str.Append(" null ");
            }
            str.Append(" user=");
            str.Append(StringUtils::Int32ToString(userId));
            str.Append(" auth=");
            str.Append(authority);
            str.Append(" src=");
            str.Append(strSyncSource);
            str.Append(" extras=");
            str.Append(strExtras);
            Logger::V(TAG, str);
        }

        Boolean isRemove = FALSE;
        AutoPtr<ISyncStorageEnginePendingOperation> tmpOp = op;
        List<AutoPtr<ISyncStorageEnginePendingOperation> >::Iterator it = Find(mPendingOperations->Begin(),
                mPendingOperations->End(), tmpOp);
        if (it != mPendingOperations->End()) {
            mPendingOperations->Erase(it);
            isRemove = TRUE;
        }

        if (isRemove) {
            if (mPendingOperations->IsEmpty()
                    || mNumPendingFinished >= PENDING_FINISH_TO_WRITE) {
                FAIL_RETURN(WritePendingOperationsLocked())
                mNumPendingFinished = 0;
            }
            else {
                mNumPendingFinished++;
            }

            AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
            FAIL_RETURN(GetAuthorityLocked(account, userId, authority, String("deleteFromPending"),
                    (ISyncStorageEngineAuthorityInfo**)&authorityInfo))

            if (NULL != authorityInfo) {
                if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                    String str("removing - authorityInfo");
//                    authorityInfo->ToString();
                    Logger::V(TAG, str);
                }
                Boolean morePending = FALSE;
                AutoPtr<ISyncStorageEnginePendingOperation> cur;
                AutoPtr<IAccount> newAccount;
                String newAuthority;
                Int32 uId = 0;
                const Int32 N = mPendingOperations->GetSize();

                for (Int32 i = 0; i < N; i++) {
                    cur = (*mPendingOperations)[i];
                    newAccount = NULL;
                    FAIL_RETURN(cur->GetAccount((IAccount**)&newAccount))
                    FAIL_RETURN(cur->GetAuthority(&newAuthority))
                    FAIL_RETURN(cur->GetUserId(&uId))

                    if (_CObject_Compare(newAccount, account) && newAuthority.Equals(authority) && uId == userId){
                        morePending = TRUE;
                        break;
                    }

                }

                if (!morePending) {
                    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                        String str("no more pending!");
                        Logger::V(TAG, str);
                    }

                    Int32 ident = 0;
                    AutoPtr<ISyncStatusInfo> statusInfo;
                    FAIL_RETURN(authorityInfo->GetIdent(&ident))
                    FAIL_RETURN(GetOrCreateSyncStatusLocked(ident, (ISyncStatusInfo**)&statusInfo))
                    FAIL_RETURN(statusInfo->SetPending(FALSE))
                }

            }

            *result = TRUE;
        }
    }

    FAIL_RETURN(ReportChange(IContentResolver::SYNC_OBSERVER_TYPE_PENDING))
    return NOERROR;
}

ECode CSyncStorageEngine::GetPendingOperations(
    /* [out] */ IObjectContainer** pendingOperationList)
{
    VALIDATE_NOT_NULL(pendingOperationList)
    AutoLock lock(mAuthoritiesLock);
    FAIL_RETURN(CObjectContainer::New(pendingOperationList))
    List<AutoPtr<ISyncStorageEnginePendingOperation> >::Iterator iter = mPendingOperations->Begin();
    AutoPtr<ISyncStorageEnginePendingOperation> operation;

    for (; iter != mPendingOperations->End(); ++iter) {
        operation = *iter;
        FAIL_RETURN((*pendingOperationList)->Add(operation))
    }

    return NOERROR;
}

ECode CSyncStorageEngine::GetPendingOperationCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    AutoLock lock(mAuthoritiesLock);
    *count = mPendingOperations->GetSize();
    return NOERROR;
}

ECode CSyncStorageEngine::DoDatabaseCleanup(
    /* [in] */ ArrayOf<IAccount *>* accounts,
    /* [in] */ Int32 userId)
{
    VALIDATE_NOT_NULL(accounts)
    AutoLock lock(mAuthoritiesLock);

    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        String str("Updating for new accounts...");
        Logger::W(TAG, str);
    }

    AutoPtr<SparseArray> removing = new SparseArray();
    Int32 len = accounts->GetLength();
    HashMap<AutoPtr<IAccountAndUser>, AutoPtr<CSyncStorageEngine::AccountInfo> >::Iterator it = mAccounts->Begin();
    AutoPtr<CSyncStorageEngine::AccountInfo> accountInfo;
    AutoPtr<IAccountAndUser> accountAndUser;

    Int32 uId = 0;
    for (; it != mAccounts->End();) {
        accountInfo = it->mSecond;
        accountAndUser = accountInfo->mAccountAndUser;
        AutoPtr<IAccount> account;
        FAIL_RETURN(accountAndUser->GetAccount((IAccount**)&account))
        FAIL_RETURN(accountAndUser->GetUserId(&uId))
        Boolean isContain = FALSE;

        for (Int32 i = 0; i != len; i++) {
            if (_CObject_Compare((*accounts)[i], account) && uId == userId) {
                isContain = TRUE;
                break;
            }
        }

        if (!isContain) {
            if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                String str("Account removed: ");
                String tmp;
                FAIL_RETURN(accountAndUser->ToString(&tmp))
                str.Append(tmp);
                Logger::W(TAG, str);
            }
            HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >::Iterator iter = accountInfo->mAuthorities->Begin();
            AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
            Int32 ident = 0;
            for (; iter != accountInfo->mAuthorities->End(); ++iter) {
                authorityInfo = iter->mSecond;
                FAIL_RETURN(authorityInfo->GetIdent(&ident))
                removing->Put(ident, authorityInfo);
            }

            mAccounts->Erase(it++);
        }
        else {
            ++it;
        }
    }

    // Clean out all data structures.
    Int32 i = removing->Size();
    if (i > 0) {
        while (i > 0) {
            i--;
            Int32 ident = removing->KeyAt(i);
            mAuthorities->Remove(ident);
            Int32 j = mSyncStatus->Size();
            while (j > 0) {
                j--;
                if (mSyncStatus->KeyAt(j) == ident) {
                    mSyncStatus->Remove(mSyncStatus->KeyAt(j));
                }
            }
            j = mSyncHistory->GetSize();
            while (j > 0) {
                j--;
                List<AutoPtr<ISyncStorageEngineSyncHistoryItem> >::Iterator iter = mSyncHistory->Begin();
                AutoPtr<ISyncStorageEngineSyncHistoryItem> historyItem;
                Int32 count = 0;
                while (count != j) {
                    count++;
                    iter++;
                }
                historyItem = *iter;
                Int32 authorityId;
                FAIL_RETURN(historyItem->GetAuthorityId(&authorityId));
                if (authorityId == ident) {
                    mSyncHistory->Remove(j);
                }
            }
        }

        FAIL_RETURN(WriteAccountInfoLocked())
        FAIL_RETURN(WriteStatusLocked())
        FAIL_RETURN(WritePendingOperationsLocked())
        FAIL_RETURN(WriteStatisticsLocked())
    }

    return NOERROR;
}

ECode CSyncStorageEngine::AddActiveSync(
    /* [in] */ ISyncOperation* activeSyncContextSyncOperation,
    /* [in] */ Int64 activeSyncContextStartTime,
    /* [out] */ ISyncInfo** syncInfo)
{
    VALIDATE_NOT_NULL(syncInfo)
    *syncInfo = NULL;

    {
        AutoLock lock(mAuthoritiesLock);
        AutoPtr<IAccount> account;
        AutoPtr<IBundle> extras;
        String authority;
        Int32 syncSource = 0;
        Int32 userId = 0;
        FAIL_RETURN(activeSyncContextSyncOperation->GetAccount((IAccount**)&account))
        FAIL_RETURN(activeSyncContextSyncOperation->GetUserId(&userId))
        FAIL_RETURN(activeSyncContextSyncOperation->GetSyncSource(&syncSource))
        FAIL_RETURN(activeSyncContextSyncOperation->GetExtras((IBundle**)&extras))

        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
            String str("setActiveSync: account=");
            if (NULL != account) {
                String tmp;
                FAIL_RETURN(account->ToString(&tmp))
                str.Append(tmp);
            }
            else {
                str.Append(" null ");
            }
            str.Append(" auth=");
            str.Append(authority);
            str.Append(" src=");
            str.Append(StringUtils::Int32ToString(syncSource));
            str.Append(" extras=");
            if (NULL != extras) {
                String tmp;
                FAIL_RETURN(extras->ToString(&tmp))
                str.Append(tmp);
            }
            else {
                str.Append(" null ");
            }
            Logger::V(TAG, str);
        }

        AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
        FAIL_RETURN(GetOrCreateAuthorityLocked(account, userId, authority,
                -1 /* assign a new identifier if creating a new authority */,
                TRUE /* write to storage if this results in a change */, (ISyncStorageEngineAuthorityInfo**)&authorityInfo))
        Int32 ident = 0;
        AutoPtr<IAccount> act;
        String auth;
        FAIL_RETURN(authorityInfo->GetIdent(&ident))
        FAIL_RETURN(authorityInfo->GetAccount((IAccount**)&act))
        FAIL_RETURN(authorityInfo->GetAuthority(&auth))
        FAIL_RETURN(CSyncInfo::New(ident, act, auth, activeSyncContextStartTime, syncInfo))
        Int32 uId = 0;
        FAIL_RETURN(authorityInfo->GetUserId(&uId))
        HashMap<Int32, AutoPtr<SyncInfoList> >::Iterator it = mCurrentSyncs->Find(uId);
        AutoPtr<SyncInfoList> syncList = it->mSecond;
        if (NULL != syncList) {
            syncList->PushBack(*syncInfo); //getCurrentSyncs(authority.userId).add(syncInfo);
        }
    }

    FAIL_RETURN(ReportActiveChange())
    return NOERROR;
}

ECode CSyncStorageEngine::RemoveActiveSync(
    /* [in] */ ISyncInfo* syncInfo,
    /* [in] */ Int32 userId)
{
    {
        AutoLock lock(mAuthoritiesLock);
        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
            AutoPtr<IAccount> account;
            String authority;
            FAIL_RETURN(syncInfo->GetAccount((IAccount**)&account))
            FAIL_RETURN(syncInfo->GetAuthority(&authority))
            String str("removeActiveSync: account=");
            if (NULL != account) {
                String tmp;
                FAIL_RETURN(account->ToString(&tmp))
                str.Append(tmp);
            }
            else {
                str.Append(" null ");
            }
            str.Append(" user=");
            str.Append(StringUtils::Int32ToString(userId));
            str.Append(" auth=");
            str.Append(authority);
            Logger::V(TAG, str);
        }

        HashMap<Int32, AutoPtr<SyncInfoList> >::Iterator it = mCurrentSyncs->Find(userId);
        AutoPtr<SyncInfoList> syncList = it->mSecond;
        if (NULL != syncList) {
            syncList->Remove(syncInfo); // getCurrentSyncs(userId).remove(syncInfo);
        }
    }

    FAIL_RETURN(ReportActiveChange())
    return NOERROR;
}

ECode CSyncStorageEngine::ReportActiveChange()
{
    FAIL_RETURN(ReportChange(IContentResolver::SYNC_OBSERVER_TYPE_ACTIVE))
    return NOERROR;
}

ECode CSyncStorageEngine::InsertStartSyncEvent(
    /* [in] */ IAccount* accountName,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authorityName,
    /* [in] */ Int64 now,
    /* [in] */ Int32 source,
    /* [in] */ Boolean initialization,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)
    Int64 id = 0;

    {
        AutoLock lock(mAuthoritiesLock);

        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
            String str("insertStartSyncEvent: account=");
            if (NULL != accountName) {
                String tmp;
                FAIL_RETURN(accountName->ToString(&tmp))
                str.Append(tmp);
            }
            else {
                str.Append(" null ");
            }
            str.Append(" user=");
            str.Append(StringUtils::Int32ToString(userId));
            str.Append(" auth=");
            str.Append(authorityName);
            str.Append(" source=");
            String strSource = StringUtils::ToString(source);
            str.Append(strSource);
            Logger::V(TAG, str);
        }

        AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
        FAIL_RETURN(GetAuthorityLocked(accountName, userId, authorityName, String("insertStartSyncEvent"),
                (ISyncStorageEngineAuthorityInfo**)&authority))

        if (NULL == authority) {
            *result = -1;
            return NOERROR;
        }

        AutoPtr<ISyncStorageEngineSyncHistoryItem> item;
        FAIL_RETURN(CSyncStorageEngineSyncHistoryItem::New((ISyncStorageEngineSyncHistoryItem**)&item))
        Int32 ident = 0;
        FAIL_RETURN(item->SetInitialization(initialization))
        FAIL_RETURN(authority->GetIdent(&ident))
        FAIL_RETURN(item->SetAuthorityId(ident))
        FAIL_RETURN(item->SetHistoryId(mNextHistoryId++))
        if (mNextHistoryId < 0) mNextHistoryId = 0;
        FAIL_RETURN(item->SetEventTime(now))
        FAIL_RETURN(item->SetSource(source))
        FAIL_RETURN(item->SetEvent(ISyncStorageEngine::EVENT_START))
        mSyncHistory->PushFront(item);

        while (mSyncHistory->GetSize() > ISyncStorageEngine::MAX_HISTORY) {
            mSyncHistory->Remove(mSyncHistory->GetSize() - 1);
        }

        Int32 historyId = 0;
        FAIL_RETURN(item->GetHistoryId(&historyId))
        id = historyId;

        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
            String str("returning historyId ");
            str.Append(StringUtils::Int64ToString(id));
            Logger::V(TAG, str);
        }
    }

    FAIL_RETURN(ReportChange(IContentResolver::SYNC_OBSERVER_TYPE_STATUS))
    *result = id;
    return NOERROR;
}

ECode CSyncStorageEngine::StopSyncEvent(
    /* [in] */ Int64 historyId,
    /* [in] */ Int64 elapsedTime,
    /* [in] */ const String& resultMessage,
    /* [in] */ Int64 downstreamActivity,
    /* [in] */ Int64 upstreamActivity)
{
    {
        AutoLock lock(mAuthoritiesLock);

        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
            String str("stopSyncEvent: historyId=");
            str.Append(StringUtils::Int64ToString(historyId));
            Logger::V(TAG, str);
        }

        AutoPtr<ISyncStorageEngineSyncHistoryItem> item;
        Int32 i = mSyncHistory->GetSize();

        while (i > 0) {
            i--;
            item = (*mSyncHistory)[i];
            Int32 tmpHistoryId = 0;
            FAIL_RETURN(item->GetHistoryId(&tmpHistoryId))
            if (tmpHistoryId == historyId) {
                break;
            }
            item = NULL;
        }

        if (NULL == item) {
            String str("stopSyncEvent: no history for id ");
            str.Append(StringUtils::Int64ToString(historyId));
            Logger::W(TAG, str);
            return NOERROR;
        }

        FAIL_RETURN(item->SetElapsedTime(elapsedTime))
        FAIL_RETURN(item->SetEvent(ISyncStorageEngine::EVENT_STOP))
        FAIL_RETURN(item->SetMesg(resultMessage))
        FAIL_RETURN(item->SetDownstreamActivity(downstreamActivity))
        FAIL_RETURN(item->SetUpstreamActivity(upstreamActivity))
        Int32 authorityId = 0;
        FAIL_RETURN(item->GetAuthorityId(&authorityId))
        AutoPtr<ISyncStatusInfo> statusInfo;
        FAIL_RETURN(GetOrCreateSyncStatusLocked(authorityId, (ISyncStatusInfo**)&statusInfo))
        Int32 numSyncs = 0;
        FAIL_RETURN(statusInfo->GetNumSyncs(&numSyncs))
        numSyncs++;
        FAIL_RETURN(statusInfo->SetNumSyncs(numSyncs))
        Int64 totalElapsedTime = 0;
        FAIL_RETURN(statusInfo->GetTotalElapsedTime(&totalElapsedTime))
        totalElapsedTime += elapsedTime;
        FAIL_RETURN(statusInfo->SetTotalElapsedTime(totalElapsedTime))
        Int32 source = 0;
        FAIL_RETURN(item->GetSource(&source))

        switch (source) {
            case ISyncStorageEngine::SOURCE_LOCAL:
            {
                Int32 numSourceLocal = 0;
                FAIL_RETURN(statusInfo->GetNumSourceLocal(&numSourceLocal))
                numSourceLocal++;
                FAIL_RETURN(statusInfo->SetNumSourceLocal(numSourceLocal))
                break;
            }
            case ISyncStorageEngine::SOURCE_POLL:
            {
                Int32 numSourcePoll = 0;
                FAIL_RETURN(statusInfo->GetNumSourcePoll(&numSourcePoll))
                numSourcePoll++;
                FAIL_RETURN(statusInfo->SetNumSourcePoll(numSourcePoll))
                break;
            }
            case ISyncStorageEngine::SOURCE_USER:
            {
                Int32 numSourceUser = 0;
                FAIL_RETURN(statusInfo->GetNumSourceUser(&numSourceUser))
                numSourceUser++;
                FAIL_RETURN(statusInfo->SetNumSourceUser(numSourceUser))
                break;
            }
            case ISyncStorageEngine::SOURCE_SERVER:
            {
                Int32 numSourceServer = 0;
                FAIL_RETURN(statusInfo->GetNumSourceServer(&numSourceServer))
                numSourceServer++;
                FAIL_RETURN(statusInfo->SetNumSourceServer(numSourceServer))
                break;
            }
            case ISyncStorageEngine::SOURCE_PERIODIC:
            {
                Int32 numSourcePeriodic = 0;
                FAIL_RETURN(statusInfo->GetNumSourcePeriodic(&numSourcePeriodic))
                numSourcePeriodic++;
                FAIL_RETURN(statusInfo->SetNumSourcePeriodic(numSourcePeriodic))
                break;
            }
        }

        Boolean writeStatisticsNow = FALSE;
        Int32 day = 0;
        Int32 tmpDay = 0;
        FAIL_RETURN(GetCurrentDayLocked(&day))
        AutoPtr<ISyncStorageEngineDayStats> dayStats;

        if (NULL == (*mDayStats)[0]) {
            FAIL_RETURN(CSyncStorageEngineDayStats::New(day, (ISyncStorageEngineDayStats**)&dayStats))
            mDayStats->Set(0, dayStats);
        }
        else if (day != ((*mDayStats)[0]->GetDay(&tmpDay), tmpDay)) {
            mDayStats->Copy(1, mDayStats, 0, mDayStats->GetLength() - 1); //System.arraycopy(mDayStats, 0, mDayStats, 1, mDayStats.length-1);
            FAIL_RETURN(CSyncStorageEngineDayStats::New(day, (ISyncStorageEngineDayStats**)&dayStats))
            mDayStats->Set(0, dayStats);
            writeStatisticsNow = TRUE;
        }

        const AutoPtr<ISyncStorageEngineDayStats> ds = (*mDayStats)[0];
        Int64 eventTime = 0;
        FAIL_RETURN(item->GetEventTime(&eventTime))
        const Int64 lastSyncTime = (eventTime + elapsedTime);
        Boolean writeStatusNow = FALSE;
        Int64 lastFailureTime = 0;
        FAIL_RETURN(statusInfo->GetLastFailureTime(&lastFailureTime))

        if (ISyncStorageEngine::MESG_SUCCESS.Equals(resultMessage)) {
            // - if successful, update the successful columns
            Int64 lastSuccessTime = 0;
            FAIL_RETURN(statusInfo->GetLastSuccessTime(&lastSuccessTime))
            if (0 == lastSuccessTime || 0 != lastFailureTime) {
                writeStatusNow = TRUE;
            }
            FAIL_RETURN(statusInfo->SetLastSuccessTime(lastSyncTime))
            FAIL_RETURN(statusInfo->SetLastSuccessSource(source))
            FAIL_RETURN(statusInfo->SetLastFailureTime(0))
            FAIL_RETURN(statusInfo->SetLastFailureSource(-1))
            FAIL_RETURN(statusInfo->SetLastFailureMesg(String(NULL)))
            FAIL_RETURN(statusInfo->SetInitialFailureTime(0))
            Int32 successCount = 0;
            FAIL_RETURN(ds->GetSuccessCount(&successCount))
            successCount++;
            FAIL_RETURN(ds->SetSuccessCount(successCount))
            Int64 successTime = 0;
            FAIL_RETURN(ds->GetSuccessTime(&successTime))
            successTime += elapsedTime;
            FAIL_RETURN(ds->SetSuccessTime(successTime))
        }
        else if (!ISyncStorageEngine::MESG_CANCELED.Equals(resultMessage)) {
            if (0 == lastFailureTime) {
                writeStatusNow = TRUE;
            }
            FAIL_RETURN(statusInfo->SetLastFailureTime(lastSyncTime))
            FAIL_RETURN(statusInfo->SetLastFailureSource(source))
            FAIL_RETURN(statusInfo->SetLastFailureMesg(resultMessage))
            Int64 initialFailureTime = 0;
            FAIL_RETURN(statusInfo->GetInitialFailureTime(&initialFailureTime))
            if (0 == initialFailureTime) {
                FAIL_RETURN(statusInfo->SetInitialFailureTime(lastSyncTime))
            }
            Int32 failureCount = 0;
            FAIL_RETURN(ds->GetFailureCount(&failureCount))
            failureCount++;
            FAIL_RETURN(ds->SetFailureCount(failureCount))
            Int64 failureTime = 0;
            FAIL_RETURN(ds->GetFailureTime(&failureTime))
            failureTime += elapsedTime;
            FAIL_RETURN(ds->SetFailureTime(failureTime))
        }

        Boolean bval = FALSE;
        if (writeStatusNow) {
            FAIL_RETURN(WriteStatusLocked())
        }
        else if (HasMessages(MSG_WRITE_STATUS, &bval), !bval) {
            SendEmptyMessageDelayed(MSG_WRITE_STATUS, WRITE_STATUS_DELAY, &bval);
        }

        if (writeStatisticsNow) {
            FAIL_RETURN(WriteStatisticsLocked())
        }
        else if (HasMessages(MSG_WRITE_STATISTICS, &bval), !bval) {
            SendEmptyMessageDelayed(MSG_WRITE_STATISTICS, WRITE_STATISTICS_DELAY, &bval);
        }
    }

    FAIL_RETURN(ReportChange(IContentResolver::SYNC_OBSERVER_TYPE_STATUS))
    return NOERROR;
}

ECode CSyncStorageEngine::GetCurrentSyncs(
    /* [in] */ Int32 userId,
    /* [out] */ IObjectContainer** syncInfoList)
{
    VALIDATE_NOT_NULL(syncInfoList)
    AutoLock lock(mAuthoritiesLock);
    HashMap<Int32, AutoPtr<SyncInfoList> >::Iterator it = mCurrentSyncs->Find(userId);
    AutoPtr<SyncInfoList> syncList = it->mSecond;

    if (NULL == syncList) {
        syncList = new SyncInfoList();
        (*mCurrentSyncs)[userId] = syncList;
    }

    FAIL_RETURN(CObjectContainer::New(syncInfoList))
    SyncInfoListIterator iter = syncList->Begin();
    for (; iter != syncList->End(); iter++) {
        FAIL_RETURN((*syncInfoList)->Add(*iter))
    }

    return NOERROR;
}

ECode CSyncStorageEngine::GetSyncStatus(
    /* [out] */ IObjectContainer** syncStatusInfoList)
{
    VALIDATE_NOT_NULL(syncStatusInfoList)
    AutoLock lock(mAuthoritiesLock);
    const Int32 N = mSyncStatus->Size();
    FAIL_RETURN(CObjectContainer::New(syncStatusInfoList))

    for (Int32 i = 0; i < N; i++) {
        FAIL_RETURN((*syncStatusInfoList)->Add(mSyncStatus->ValueAt(i)))
    }

    return NOERROR;
}

ECode CSyncStorageEngine::GetAuthorities(
    /* [out] */ IObjectContainer** authorityInfoList)
{
    VALIDATE_NOT_NULL(authorityInfoList)
    AutoLock lock(mAuthoritiesLock);
    const Int32 N = mAuthorities->Size();
    FAIL_RETURN(CObjectContainer::New(authorityInfoList))

    for (Int32 i = 0; i < N; i++) {
        // Make deep copy because AuthorityInfo syncs are liable to change.
        AutoPtr<ISyncStorageEngineAuthorityInfo> newAuthorityInfo;
        AutoPtr<ISyncStorageEngineAuthorityInfo> tmp = (ISyncStorageEngineAuthorityInfo*) mAuthorities->ValueAt(i);
        FAIL_RETURN(CSyncStorageEngineAuthorityInfo::New(tmp, (ISyncStorageEngineAuthorityInfo**)&newAuthorityInfo))
        FAIL_RETURN((*authorityInfoList)->Add(newAuthorityInfo))
    }

    return NOERROR;
}

ECode CSyncStorageEngine::GetStatusByAccountAndAuthority(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authority,
    /* [out] */ ISyncStatusInfo** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    if (NULL == account || authority.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoLock lock(mAuthoritiesLock);
    const Int32 N = mSyncStatus->Size();
    AutoPtr<ISyncStatusInfo> cur;
    AutoPtr<ISyncStorageEngineAuthorityInfo> ainfo;
    Int32 authorityId = 0;

    for (Int32 i = 0; i < N; i++) {
        cur = (ISyncStatusInfo*) mSyncStatus->ValueAt(i);
        FAIL_RETURN(cur->GetAuthorityId(&authorityId))
        ainfo = (ISyncStorageEngineAuthorityInfo*) mAuthorities->Get(authorityId);

        if (NULL != ainfo) {
            String tmpAuthority;
            Int32 uId = 0;
            AutoPtr<IAccount> tmpAccount;
            FAIL_RETURN(ainfo->GetAuthority(&tmpAuthority))
            FAIL_RETURN(ainfo->GetUserId(&uId))
            FAIL_RETURN(ainfo->GetAccount((IAccount**)&tmpAccount))
            if (tmpAuthority.Equals(authority) && uId == userId && _CObject_Compare(account, tmpAccount)) {
                *result = cur;
                REFCOUNT_ADD(*result);
                return NOERROR;
            }
        }

    }

    return NOERROR;
}

ECode CSyncStorageEngine::IsSyncPending(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authority,
    /* [out] */ Boolean* isPending)
{
    VALIDATE_NOT_NULL(isPending)
    AutoLock lock(mAuthoritiesLock);
    const Int32 N = mSyncStatus->Size();
    AutoPtr<ISyncStatusInfo> cur;
    AutoPtr<ISyncStorageEngineAuthorityInfo> ainfo;
    Int32 authorityId = 0;
    Int32 uId = 0;

    for (Int32 i = 0; i < N; i++) {
        cur = (ISyncStatusInfo*) mSyncStatus->ValueAt(i);
        FAIL_RETURN(cur->GetAuthorityId(&authorityId));
        ainfo = (ISyncStorageEngineAuthorityInfo*) mAuthorities->Get(authorityId);
        if (NULL == ainfo) continue;

        FAIL_RETURN(ainfo->GetUserId(&uId))
        if (userId != uId) continue;

        if (NULL != account) {
            AutoPtr<IAccount> tmpAccount;
            FAIL_RETURN(ainfo->GetAccount((IAccount**)&tmpAccount));
            if (_CObject_Compare(tmpAccount, account)) continue;
        }

        String strAuthority;
        FAIL_RETURN(ainfo->GetAuthority(&strAuthority))
        Boolean pending = FALSE;
        FAIL_RETURN(cur->GetPending(&pending))
        if (strAuthority.Equals(authority) && pending) {
            *isPending = TRUE;
            return NOERROR;
        }
    }

    *isPending = FALSE;
    return NOERROR;
}

ECode CSyncStorageEngine::GetSyncHistory(
    /* [out] */ IObjectContainer** syncHistoryItemList)
{
    VALIDATE_NOT_NULL(syncHistoryItemList)
    AutoLock lock(mAuthoritiesLock);
    FAIL_RETURN(CObjectContainer::New(syncHistoryItemList))
    List<AutoPtr<ISyncStorageEngineSyncHistoryItem> >::Iterator iter = mSyncHistory->Begin();
    AutoPtr<ISyncStorageEngineSyncHistoryItem> historyItem;

    for (; iter != mSyncHistory->End(); ++iter) {
        historyItem = *iter;
        FAIL_RETURN((*syncHistoryItemList)->Add(historyItem))
    }

    return NOERROR;
}

ECode CSyncStorageEngine::GetDayStatistics(
    /* [out, callee] */ ArrayOf<ISyncStorageEngineDayStats *>** dayStats)
{
    VALIDATE_NOT_NULL(dayStats);
    AutoLock lock(mAuthoritiesLock);
    Int32 N = mDayStats->GetLength();
    AutoPtr<ArrayOf<ISyncStorageEngineDayStats*> > tmpArray = ArrayOf<ISyncStorageEngineDayStats*>::Alloc(N);
    tmpArray->Copy(mDayStats);
    *dayStats = tmpArray;
    REFCOUNT_ADD(*dayStats);
    return NOERROR;
}

ECode CSyncStorageEngine::GetOrCreateSyncStatus(
    /* [in] */ ISyncStorageEngineAuthorityInfo* authority,
    /* [out] */ ISyncStatusInfo** syncStatus)
{
    VALIDATE_NOT_NULL(authority)
    VALIDATE_NOT_NULL(syncStatus)
    AutoLock lock(mAuthoritiesLock);
    Int32 ident = 0;
    FAIL_RETURN(authority->GetIdent(&ident))
    FAIL_RETURN(GetOrCreateSyncStatusLocked(ident, syncStatus))
    return NOERROR;
}

ECode CSyncStorageEngine::WriteAllState()
{
    AutoLock lock(mAuthoritiesLock);
    // Account info is always written so no need to do it here.

    if (mNumPendingFinished > 0) {
        // Only write these if they are out of date.
        FAIL_RETURN(WritePendingOperationsLocked())
    }

    // Just always write these...  they are likely out of date.
    FAIL_RETURN(WriteStatusLocked())
    FAIL_RETURN(WriteStatisticsLocked())
    return NOERROR;
}

ECode CSyncStorageEngine::ClearAndReadState()
{
    AutoLock lock(mAuthoritiesLock);

    if (NULL != mAuthorities) mAuthorities->Clear();
    mAccounts->Clear();
    mPendingOperations->Clear();
    if (NULL != mSyncStatus) mSyncStatus->Clear();
    mSyncHistory->Clear();

    FAIL_RETURN(ReadAccountInfoLocked())
    FAIL_RETURN(ReadStatusLocked())
    FAIL_RETURN(ReadPendingOperationsLocked())
    FAIL_RETURN(ReadStatisticsLocked())
    FAIL_RETURN(ReadAndDeleteLegacyAccountInfoLocked())
    FAIL_RETURN(WriteAccountInfoLocked())
    FAIL_RETURN(WriteStatusLocked())
    FAIL_RETURN(WritePendingOperationsLocked())
    FAIL_RETURN(WriteStatisticsLocked())

    return NOERROR;
}

ECode CSyncStorageEngine::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IFile* dataDir)
{
    VALIDATE_NOT_NULL(context)

    Handler::Init();

    mContext = context;
    sSyncStorageEngine = (ISyncStorageEngine*) this;

//    mCal = Calendar.getInstance(TimeZone.getTimeZone("GMT+0"));

    AutoPtr<IResources> res;
    FAIL_RETURN(mContext->GetResources((IResources**)&res))
    FAIL_RETURN(res->GetBoolean(0x01110046 /* com.android.internal.R.bool.config_ISyncStorageEngine::masterSyncAutomatically */,
            &mDefaultMasterSyncAutomatically))
    AutoPtr<IFile> systemDir;
    FAIL_RETURN(CFile::New(dataDir, String("system"), (IFile**)&systemDir))
    AutoPtr<IFile> syncDir;
    FAIL_RETURN(CFile::New(systemDir, String("sync"), (IFile**)&syncDir))
    Boolean succeeded = FALSE;
    FAIL_RETURN(syncDir->Mkdirs(&succeeded))

//    mAccountInfoFile = new AtomicFile(new File(syncDir, "accounts.xml"));
//    mStatusFile = new AtomicFile(new File(syncDir, "status.bin"));
//    mPendingFile = new AtomicFile(new File(syncDir, "pending.bin"));
//    mStatisticsFile = new AtomicFile(new File(syncDir, "stats.bin"));

    FAIL_RETURN(ReadAccountInfoLocked())
    FAIL_RETURN(ReadStatusLocked())
    FAIL_RETURN(ReadPendingOperationsLocked())
    FAIL_RETURN(ReadStatisticsLocked())
    FAIL_RETURN(ReadAndDeleteLegacyAccountInfoLocked())
    FAIL_RETURN(WriteAccountInfoLocked())
    FAIL_RETURN(WriteStatusLocked())
    FAIL_RETURN(WritePendingOperationsLocked())
    FAIL_RETURN(WriteStatisticsLocked())
    return NOERROR;
}

ECode CSyncStorageEngine::SetOnSyncRequestListener(
    /* [in] */ ISyncStorageEngineOnSyncRequestListener* listener)
{
    if (NULL == mSyncRequestListener) {
        mSyncRequestListener = listener;
    }
    return NOERROR;
}

ECode CSyncStorageEngine::GetIntColumn(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& name,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(cursor)
    VALIDATE_NOT_NULL(result)
    Int32 columnIndex = 0;
    FAIL_RETURN(cursor->GetColumnIndex(name, &columnIndex))
    FAIL_RETURN(cursor->GetInt32(columnIndex, result))
    return NOERROR;
}

ECode CSyncStorageEngine::GetLongColumn(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& name,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(cursor)
    VALIDATE_NOT_NULL(result)
    Int32 columnIndex = 0;
    FAIL_RETURN(cursor->GetColumnIndex(name, &columnIndex))
    FAIL_RETURN(cursor->GetInt64(columnIndex, result))
    return NOERROR;
}

ECode CSyncStorageEngine::FlattenBundle(
    /* [in] */ IBundle* bundle,
    /* [out, callee] */ ArrayOf<Byte>** flattenBundleArray)
{
    VALIDATE_NOT_NULL(bundle)
    VALIDATE_NOT_NULL(flattenBundleArray)
    AutoPtr<IParcel> parcel;
    // Parcel parcel = Parcel.obtain();
//***    FAIL_RETURN(CParcel::New((IParcel**)&parcel))
    ECode ecode = NOERROR;
    ecode = IParcelable::Probe(bundle)->WriteToParcel(parcel);
    if (FAILED(ecode)) goto EXIT;
//    *flattenBundleArray = parcel.marshall();
EXIT:
//    parcel.recycle();
    FAIL_RETURN(ecode)
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::UnflattenBundle(
    /* [in] */ ArrayOf<Byte>* flatData,
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(flatData)
    VALIDATE_NOT_NULL(bundle)
    AutoPtr<IParcel> parcel; // Parcel parcel = Parcel.obtain();
//***    FAIL_RETURN(CParcel::New((IParcel**)&parcel))
    ECode ecode = NOERROR;
//    parcel.unmarshall(flatData, 0, flatData.length);
    ecode = parcel->SetDataPosition(0);
    if (FAILED(ecode)) goto EXIT;
//    *bundle = parcel.readBundle();
EXIT:
    if (E_RUNTIME_EXCEPTION == ecode) {
        // A RuntimeException is thrown if we were unable to parse the parcel.
        // Create an empty parcel in this case.
        FAIL_RETURN(CBundle::New(bundle))
    }
//    parcel.recycle();
    FAIL_RETURN(ecode)
    return E_NOT_IMPLEMENTED;
}

Boolean CSyncStorageEngine::InitStaticBlock()
{
    if (sIsInitialization) return TRUE;
    (*sAuthorityRenames)[String("contacts")] = String("com.android.contacts");
    (*sAuthorityRenames)[String("calendar")] = String("com.android.calendar");
    return TRUE;
}

ECode CSyncStorageEngine::ReportChange(
    /* [in] */ Int32 which)
{
    AutoPtr<List<AutoPtr<ISyncStatusObserver> > > reports = new List<AutoPtr<ISyncStatusObserver> >;
    AutoPtr<ISyncStatusObserver> observer;

    {
        AutoLock lock(mAuthoritiesLock);
        Int32 i = 0;
        FAIL_RETURN(mChangeListeners->BeginBroadcast(&i))

        while (i > 0) {
            i--;
            AutoPtr<IInteger32> mask;
            FAIL_RETURN(mChangeListeners->GetBroadcastCookie(i, (IInterface**)&mask))
            Int32 intValue = 0;
            FAIL_RETURN(mask->GetValue(&intValue))
            if ((which & intValue) == 0) {
                continue;
            }

            observer = NULL;
            FAIL_RETURN(mChangeListeners->GetBroadcastItem(i, (IInterface**)&observer))
            reports->PushBack(observer);
        }

        FAIL_RETURN(mChangeListeners->FinishBroadcast())
    }

    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        String str("reportChange ");
        str.Append(StringUtils::Int32ToString(which));
        str.Append(" to: ");
//        reports.toString();
        Logger::V(TAG, str);
    }

    if (NULL != reports) {
        Int32 i = reports->GetSize();
        while (i > 0) {
            i--;
            FAIL_RETURN((*reports)[i]->OnStatusChanged(which))
        }
    }

    return NOERROR;
}

ECode CSyncStorageEngine::UpdateOrRemovePeriodicSync(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& providerName,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 period,
    /* [in] */ Boolean add)
{
    if (period <= 0) {
        period = 0;
    }
    if (NULL == extras) {
        FAIL_RETURN(CBundle::New(&extras))
    }

    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        String str("addOrRemovePeriodicSync: ");
        if (NULL != account) {
            String tmp;
            FAIL_RETURN(account->ToString(&tmp))
            str.Append(tmp);
        }
        else {
            str.Append(" null ");
        }
        str.Append(", user ");
        str.Append(StringUtils::Int32ToString(userId));
        str.Append(", provider ");
        str.Append(providerName);
        str.Append(" -> period ");
        String strPeriod = StringUtils::ToString(period);
        str.Append(strPeriod);
        str.Append(", extras ");
        String tmp;
        FAIL_RETURN(extras->ToString(&tmp))
        str.Append(tmp);
        Logger::V(TAG, str);
    }

    {
        AutoLock lock(mAuthoritiesLock);
        AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
        FAIL_DOFINAL(GetOrCreateAuthorityLocked(account, userId, providerName, -1, FALSE,
                (ISyncStorageEngineAuthorityInfo**)&authority))
        Int32 ident = 0;
        FAIL_DOFINAL(authority->GetIdent(&ident))
        AutoPtr<ISyncStatusInfo> status;
        AutoPtr< List<AutoPtr<IPair> > > periodicSyncs;
        AutoPtr<IPair> syncInfo;
        AutoPtr<IBundle> existingExtras;
        AutoPtr<IInteger64> secondObj;
        AutoPtr<IPeriodicSync> periodicSync;
        Int64 second = 0;
        Int32 index = 0;
        FAIL_DOFINAL(((CSyncStorageEngineAuthorityInfo*) authority.Get())->GetPeriodicSyncs((List<AutoPtr<IPair> >**)&periodicSyncs))
        List<AutoPtr<IPair> >::Iterator it = periodicSyncs->Begin();

        if (add) {
            // add this periodic sync if one with the same extras doesn't already
            // exist in the periodicSyncs array
            Boolean alreadyPresent = FALSE;

            for (; it != periodicSyncs->End(); ++it) {
                syncInfo = *it;
                existingExtras = NULL;
                secondObj = NULL;
                FAIL_DOFINAL(syncInfo->GetFirst((IInterface**)&existingExtras))
                FAIL_DOFINAL(syncInfo->GetSecond((IInterface**)&secondObj))
                FAIL_DOFINAL(secondObj->GetValue(&second))
                if(_CObject_Compare(existingExtras, extras)) {
                    if (second == period) {
                        return NOERROR;
                    }
                    AutoPtr<IPair> temPair;
                    AutoPtr<IInteger64> periodObj;
                    FAIL_DOFINAL(CInteger64::New(period, (IInteger64**)&periodObj))
                    FAIL_DOFINAL(CPair::New(extras, periodObj, (IPair**)&temPair))
                    periodicSyncs->Insert(index, temPair);
                    alreadyPresent = TRUE;
                    break;
                }
                index++;
            }

            // if we added an entry to the periodicSyncs array also add an entry to
            // the periodic syncs status to correspond to it
            if (!alreadyPresent) {
                AutoPtr<IPair> temPair;
                AutoPtr<IInteger64> periodObj;
                FAIL_DOFINAL(CInteger64::New(period, (IInteger64**)&periodObj))
                FAIL_DOFINAL(CPair::New(extras, periodObj, (IPair**)&temPair))
                periodicSyncs->PushBack(temPair);
                FAIL_DOFINAL(GetOrCreateSyncStatusLocked(ident, (ISyncStatusInfo**)&status))
                Int32 size = periodicSyncs->GetSize();
                FAIL_DOFINAL(status->SetPeriodicSyncTime(size - 1, 0))
            }
        }
        else {
            // remove any periodic syncs that match the authority and extras
            status = (ISyncStatusInfo*) mSyncStatus->Get(ident);
            Boolean changed = FALSE;
            index = 0;

            for (; it != periodicSyncs->End(); ++index) {
                syncInfo = *it;
                existingExtras = NULL;
                FAIL_DOFINAL(syncInfo->GetFirst((IInterface**)&existingExtras))
                if(_CObject_Compare(existingExtras, extras)) {
                    it = periodicSyncs->Erase(it);
                    changed = TRUE;
                    // if we removed an entry from the periodicSyncs array also
                    // remove the corresponding entry from the status
                    if (NULL != status) {
                        FAIL_DOFINAL(status->RemovePeriodicSyncTime(index))
                    }
                }
                else {
                    ++it;
                }
            }

            if (!changed) {
                return NOERROR;
            }
        }

        FAIL_RETURN(WriteAccountInfoLocked())
        FAIL_RETURN(WriteStatusLocked())
    }

    FAIL_RETURN(ReportChange(IContentResolver::SYNC_OBSERVER_TYPE_SETTINGS))
    return NOERROR;
}

ECode CSyncStorageEngine::GetCurrentDayLocked(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
//    mCal.setTimeInMillis(System.currentTimeMillis());
//    final int dayOfYear = mCal.get(Calendar.DAY_OF_YEAR);
//    if (mYear != mCal.get(Calendar.YEAR)) {
//        mYear = mCal.get(Calendar.YEAR);
//        mCal.clear();
//        mCal.set(Calendar.YEAR, mYear);
//        mYearInDays = (int)(mCal.getTimeInMillis()/86400000);
//    }
//    return dayOfYear + mYearInDays;
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetAuthorityLocked(
    /* [in] */ IAccount* accountName,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authorityName,
    /* [in] */ const String& tag,
    /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo)
{
    VALIDATE_NOT_NULL(authorityInfo)
    *authorityInfo = NULL;

    AutoPtr<IAccountAndUser> au;
//***    FAIL_RETURN(CAccountAndUser::New(accountName, userId, (IAccountAndUser**)&au))
    HashMap<AutoPtr<IAccountAndUser>, AutoPtr<CSyncStorageEngine::AccountInfo> >::Iterator it = mAccounts->Find(au);
    AutoPtr<CSyncStorageEngine::AccountInfo> accountInfo = it->mSecond;

    if (NULL == accountInfo) {
        if (!tag.IsNull()) {
            if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                String str(tag);
                str.Append(": unknown account ");
                if (NULL != au) {
                    String tmp;
                    FAIL_RETURN(au->ToString(&tmp))
                    str.Append(tmp);
                }
                else {
                    str.Append(" null ");
                }
                Logger::V(TAG, str);
            }
        }

        return NOERROR;
    }

    AutoPtr<HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> > > authorities = accountInfo->mAuthorities;
    HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >::Iterator iter = authorities->Find(authorityName);
    *authorityInfo = iter->mSecond;
    REFCOUNT_ADD(*authorityInfo);

    if (NULL == *authorityInfo) {
        if (!tag.IsNull()) {
            if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                String str(tag);
                str.Append(": unknown authority ");
                str.Append(authorityName);
                Logger::V(TAG, str);
            }
        }
        return NOERROR;
    }

    return NOERROR;
}

ECode CSyncStorageEngine::GetOrCreateAuthorityLocked(
    /* [in] */ IAccount* accountName,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authorityName,
    /* [in] */ Int32 ident,
    /* [in] */ Boolean doWrite,
    /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo)
{
    VALIDATE_NOT_NULL(authorityInfo)
    *authorityInfo = NULL;

    AutoPtr<IAccountAndUser> au;
//***    FAIL_RETURN(CAccountAndUser::New(accountName, userId, (IAccountAndUser**)&au))
    HashMap<AutoPtr<IAccountAndUser>, AutoPtr<CSyncStorageEngine::AccountInfo> >::Iterator it = mAccounts->Find(au);
    AutoPtr<CSyncStorageEngine::AccountInfo> accountInfo = it->mSecond;

    if (NULL == accountInfo) {
        accountInfo = new CSyncStorageEngine::AccountInfo(au);
        (*mAccounts)[au] = accountInfo;
    }

    AutoPtr<HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> > > authorities = accountInfo->mAuthorities;
    HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >::Iterator iter = authorities->Find(authorityName);
    *authorityInfo = iter->mSecond;
    REFCOUNT_ADD(*authorityInfo);

    if (NULL == *authorityInfo) {
        if (ident < 0) {
            ident = mNextAuthorityId;
            mNextAuthorityId++;
            doWrite = TRUE;
        }

        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
            String str("created a new AuthorityInfo for ");
            if (NULL != accountName) {
                String tmp;
                FAIL_RETURN(accountName->ToString(&tmp))
                str.Append(tmp);
            }
            else {
                str.Append(" null ");
            }
            str.Append(", user ");
            str.Append(StringUtils::Int32ToString(userId));
            str.Append(", provider ");
            str.Append(authorityName);
            Logger::V(TAG, str);
        }

        FAIL_RETURN(CSyncStorageEngineAuthorityInfo::New(accountName, userId, authorityName, ident, authorityInfo))
        (*authorities)[authorityName] = *authorityInfo;
        mAuthorities->Put(ident, *authorityInfo);
        if (doWrite) {
            FAIL_RETURN(WriteAccountInfoLocked());
        }
    }

    return NOERROR;
}

ECode CSyncStorageEngine::RemoveAuthorityLocked(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authorityName,
    /* [in] */ Boolean doWrite)
{
    AutoPtr<IAccountAndUser> au;
//***    FAIL_RETURN(CAccountAndUser::New(account, userId, (IAccountAndUser**)&au))
    HashMap<AutoPtr<IAccountAndUser>, AutoPtr<CSyncStorageEngine::AccountInfo> >::Iterator it = mAccounts->Find(au);
    AutoPtr<CSyncStorageEngine::AccountInfo> accountInfo = it->mSecond;

    if (NULL != accountInfo) {
        AutoPtr<HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> > > authorities = accountInfo->mAuthorities;
        HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >::Iterator iter = authorities->Find(authorityName);
        AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo = iter->mSecond;
        authorities->Erase(authorityName);
        if (NULL != authorityInfo) {
            Int32 ident;
            FAIL_RETURN(authorityInfo->GetIdent(&ident))
            mAuthorities->Remove(ident);
            if (doWrite) {
                FAIL_RETURN(WriteAccountInfoLocked())
            }
        }
    }

    return NOERROR;
}

ECode CSyncStorageEngine::GetOrCreateSyncStatusLocked(
    /* [in] */ Int32 authorityId,
    /* [out] */ ISyncStatusInfo** syncStatusInfo)
{
    VALIDATE_NOT_NULL(syncStatusInfo)
    *syncStatusInfo = (ISyncStatusInfo*) mSyncStatus->Get(authorityId);
    REFCOUNT_ADD(*syncStatusInfo);

    if (NULL == *syncStatusInfo) {
        FAIL_RETURN(CSyncStatusInfo::New(authorityId, syncStatusInfo))
        mSyncStatus->Put(authorityId, *syncStatusInfo);
    }
    return NOERROR;
}

ECode CSyncStorageEngine::ReadAccountInfoLocked()
{
    Int32 highestAuthorityId = -1;
    AutoPtr<IFileInputStream> fis;
    ECode ecode = NOERROR;
    {
//        fis = mAccountInfoFile.openRead();
        if (DEBUG_FILE) {
//            Log.v(TAG, "Reading " + mAccountInfoFile.getBaseFile());
        }
        AutoPtr<IXmlPullParser> parser;
//***        AutoPtr<IXmlPullParserFactory> parserFactory;
//***        ecode = CXmlPullParserFactory::New((IXmlPullParserFactory**)&parserFactory);
        FAIL_GOTO(ecode, EXIT)
//***        ecode = parserFactory->NewPullParser((IXmlPullParser**)&parser);
        FAIL_GOTO(ecode, EXIT)
        String inputEncoding;
        ecode = parser->SetInput(fis, inputEncoding);
        FAIL_GOTO(ecode, EXIT)
        Int32 eventType = 0;
        ecode = parser->GetEventType(&eventType);
        FAIL_GOTO(ecode, EXIT)
        //change by AW:lisidong
        //in some case, the file was empty and bad, it may loop forever
        while (eventType != IXmlPullParser::START_TAG) {
            ecode = parser->Next(&eventType);
            FAIL_GOTO(ecode, EXIT)
            if (eventType == IXmlPullParser::END_DOCUMENT) {
                //delete the error file for recreate it;
                // TODO mAccountInfoFile->GetBaseFile().delete();
                Logger::W(TAG, "Error reading accounts, remove accounts.xml");
                if (NULL != fis) {
                    FAIL_RETURN(fis->Close())
                }
                return NOERROR;
            }
        }
        String tagName;
        ecode = parser->GetName(&tagName);
        FAIL_GOTO(ecode, EXIT)

        if (String("accounts").Equals(tagName)) {
            String listen;
            ecode = parser->GetAttributeValue(String(NULL), XML_ATTR_LISTEN_FOR_TICKLES, &listen);
            FAIL_GOTO(ecode, EXIT)
            String versionString;
            ecode = parser->GetAttributeValue(String(NULL), String("version"), &versionString);
            FAIL_GOTO(ecode, EXIT)
            Int32 version = 0;
            version = versionString.IsNull() ? 0 : StringUtils::ParseInt32(versionString);
            String nextIdString;
            ecode = parser->GetAttributeValue(String(NULL), XML_ATTR_NEXT_AUTHORITY_ID, &nextIdString);
            FAIL_GOTO(ecode, EXIT)
            Int32 id = nextIdString.IsNull() ? 0 : StringUtils::ParseInt32(nextIdString);
            mNextAuthorityId = Elastos::Core::Math::Max(mNextAuthorityId, id);
            String offsetString;
            ecode = parser->GetAttributeValue(String(NULL), XML_ATTR_SYNC_RANDOM_OFFSET, &offsetString);
            FAIL_GOTO(ecode, EXIT)
            mSyncRandomOffset = offsetString.IsNull() ? 0 : StringUtils::ParseInt32(offsetString);
            if (mSyncRandomOffset == 0) {
                AutoPtr<ISystem> system;
                Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
                // Random random = new Random(System.currentTimeMillis());
                // mSyncRandomOffset = random.nextInt(86400);
                Int64 seed;
                system->GetCurrentTimeMillis(&seed);

                srandom(seed);
                mSyncRandomOffset = random() % 86400;
            }
            AutoPtr<IBoolean> boolObj;
            ecode = CBoolean::New(listen.IsNull() || listen.Equals("TRUE"), (IBoolean**)&boolObj);
            FAIL_GOTO(ecode, EXIT)
            mMasterSyncAutomatically->Put(0, boolObj);
            ecode = parser->Next(&eventType);
            FAIL_GOTO(ecode, EXIT)
            AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
            AutoPtr<IPair> periodicSync = NULL;
            Int32 depth = 0;

            do {
                if (eventType == IXmlPullParser::START_TAG) {
                    ecode = parser->GetName(&tagName);
                    FAIL_GOTO(ecode, EXIT)
                    ecode = parser->GetDepth(&depth);
                    FAIL_GOTO(ecode, EXIT)
                    if (2 == depth) {
                        if (String("authority").Equals(tagName)) {
                            authority = NULL;
                            ecode = ParseAuthority(parser, version, (ISyncStorageEngineAuthorityInfo**)&authority);
                            FAIL_GOTO(ecode, EXIT)
                            periodicSync = NULL;
                            Int32 ident = 0;
                            ecode = authority->GetIdent(&ident);
                            FAIL_GOTO(ecode, EXIT)
                            if (ident > highestAuthorityId) {
                                highestAuthorityId = ident;
                            }
                        }
                        else if (XML_TAG_LISTEN_FOR_TICKLES.Equals(tagName)) {
                            ecode = ParseListenForTickles(parser);
                            FAIL_GOTO(ecode, EXIT)
                        }
                    }
                    else if (3 == depth) {
                        if (String("periodicSync").Equals(tagName) && NULL != authority) {
                            periodicSync = NULL;
                            ecode = ParsePeriodicSync(parser, authority, (IPair**)&periodicSync);
                            FAIL_GOTO(ecode, EXIT)
                        }
                    }
                    else if (4 == depth && NULL != periodicSync) {
                        if (String("extra").Equals(tagName)) {
                            ecode = ParseExtra(parser, periodicSync);
                            FAIL_GOTO(ecode, EXIT)
                        }
                    }
                }
                ecode = parser->Next(&eventType);
                FAIL_GOTO(ecode, EXIT)
            } while (eventType != IXmlPullParser::END_DOCUMENT);
        }
    }
EXIT:
    if (E_XML_PULL_PARSER_EXCEPTION == ecode) {
        String str("Error reading accounts ecode = XmlPullParserException");
        Logger::W(TAG, str);
    }
    else if (E_IO_EXCEPTION == ecode) {
        if (NULL == fis) {
            String str("No initial accounts");
            Logger::I(TAG, str);
        }
        else {
            String str("Error reading accounts ecode = E_IO_EXCEPTION");
            Logger::W(TAG, str);
        }
    }

    mNextAuthorityId = Elastos::Core::Math::Max(highestAuthorityId + 1, mNextAuthorityId);
    if (NULL != fis) {
        FAIL_RETURN(fis->Close())
    }

    FAIL_RETURN(ecode)
    Boolean isChange = FALSE;
    FAIL_RETURN(MaybeMigrateSettingsForRenamedAuthorities(&isChange))
    return NOERROR;
}

ECode CSyncStorageEngine::MaybeMigrateSettingsForRenamedAuthorities(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Boolean writeNeeded = FALSE;
    List<AutoPtr<ISyncStorageEngineAuthorityInfo> > authoritiesToRemove;
    AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
    const Int32 N = mAuthorities->Size();

    for (Int32 i = 0; i < N; i++) {
        authority = (ISyncStorageEngineAuthorityInfo*) mAuthorities->ValueAt(i);
        String strAuthority;
        FAIL_RETURN(authority->GetAuthority(&strAuthority))
        // skip this authority if it isn't one of the renamed ones
        HashMap<String, String>::Iterator it = sAuthorityRenames->Find(strAuthority);
        String newAuthorityName;
        newAuthorityName = it->mSecond;

        if (newAuthorityName.IsNull()) {
            continue;
        }

        // remember this authority so we can remove it later. we can't remove it
        // now without messing up this loop iteration
        authoritiesToRemove.PushBack(authority);

        // this authority isn't enabled, no need to copy it to the new authority name since
        // the default is "disabled"
        Boolean enabled = FALSE;
        FAIL_RETURN(authority->GetEnabled(&enabled))
        if (!enabled) {
            continue;
        }

        // if we already have a record of this new authority then don't copy over the settings
        AutoPtr<IAccount> account;
        Int32 userId = 0;
        FAIL_RETURN(authority->GetAccount((IAccount**)&account))
        FAIL_RETURN(authority->GetUserId(&userId))
        AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
        FAIL_RETURN(GetAuthorityLocked(account, userId, newAuthorityName, String("cleanup"),
                (ISyncStorageEngineAuthorityInfo**)&authorityInfo))
        if (NULL != authorityInfo) {
            continue;
        }

        AutoPtr<ISyncStorageEngineAuthorityInfo> newAuthority;
        FAIL_RETURN(GetOrCreateAuthorityLocked(account, userId, newAuthorityName, -1 /* ident */,
                FALSE /* doWrite */, (ISyncStorageEngineAuthorityInfo**)&newAuthority))
        FAIL_RETURN(newAuthority->SetEnabled(TRUE))
        writeNeeded = TRUE;
    }

    List<AutoPtr<ISyncStorageEngineAuthorityInfo> >::Iterator iter = authoritiesToRemove.Begin();
    AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
    AutoPtr<IAccount> account;
    String strAuthority;
    Int32 userId = 0;

    for (; iter != authoritiesToRemove.End(); ++iter) {
        authorityInfo = *iter;
        account = NULL;
        FAIL_RETURN(authorityInfo->GetAccount((IAccount**)&account))
        FAIL_RETURN(authorityInfo->GetAuthority(&strAuthority))
        FAIL_RETURN(authorityInfo->GetUserId(&userId))
        FAIL_RETURN(RemoveAuthorityLocked(account, userId, strAuthority, FALSE /* doWrite */))
        writeNeeded = TRUE;
    }

    *result = writeNeeded;
    return NOERROR;
}

ECode CSyncStorageEngine::ParseListenForTickles(
    /* [in] */ IXmlPullParser* parser)
{
    String user;
    FAIL_RETURN(parser->GetAttributeValue(String(NULL), XML_ATTR_USER, &user))
    Int32 userId = 0;
    // try {
    userId = StringUtils::ParseInt32(user);
    // } catch (NumberFormatException e) {
    //     Log.e(TAG, "error parsing the user for listen-for-tickles", e);
    // } catch (NullPointerException e) {
    //     Log.e(TAG, "the user in listen-for-tickles is null", e);
    // }
    String enabled;
    FAIL_RETURN(parser->GetAttributeValue(String(NULL), XML_ATTR_ENABLED, &enabled))
    Boolean listen = enabled.IsNull() || enabled.Equals("TRUE");
    AutoPtr<IBoolean> boolObj;
    FAIL_RETURN(CBoolean::New(listen, (IBoolean**)&boolObj))
    mMasterSyncAutomatically->Put(userId, boolObj);
    return NOERROR;
}

ECode CSyncStorageEngine::ParseAuthority(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ Int32 version,
    /* [out] */ ISyncStorageEngineAuthorityInfo** authority)
{
    VALIDATE_NOT_NULL(authority)
    *authority = NULL;
    if (NULL == parser) {
        String str("the id of the authority is null");
        Logger::E(TAG, str);
        return E_NULL_POINTER_EXCEPTION;
    }
    Int32 id = -1;
    String strValue;
    FAIL_RETURN(parser->GetAttributeValue(String(NULL), String("id"), &strValue));

    if (strValue.IsNull()) {
        String str("error parsing the id of the authority, the id of the authority is null");
        Logger::E(TAG, str);
    }
    else {
        id = StringUtils::ParseInt32(strValue);
    }

    if (id >= 0) {
        String authorityName;
        FAIL_RETURN(parser->GetAttributeValue(String(NULL), String("authority"), &authorityName))
        String enabled;
        FAIL_RETURN(parser->GetAttributeValue(String(NULL), XML_ATTR_ENABLED, &enabled))
        String syncable;
        FAIL_RETURN(parser->GetAttributeValue(String(NULL), String("syncable"), &syncable))
        String accountName;
        FAIL_RETURN(parser->GetAttributeValue(String(NULL), String("account"), &accountName))
        String accountType;
        FAIL_RETURN(parser->GetAttributeValue(String(NULL), String("type"), &accountType))
        String user;
        FAIL_RETURN(parser->GetAttributeValue(String(NULL), XML_ATTR_USER, &user))
        Int32 userId = user.IsNull() ? 0 : StringUtils::ParseInt32(user);

        if (accountType.IsNull()) {
            accountType = "com.google";
            syncable = "unknown";
        }

        *authority = (ISyncStorageEngineAuthorityInfo*) mAuthorities->Get(id);
        REFCOUNT_ADD(*authority);

        if (CSyncStorageEngine::DEBUG_FILE) {
            String str("Adding authority: account=");
            str.Append(accountName);
            str.Append(" auth=");
            str.Append(authorityName);
            str.Append(" user=");
            str.Append(StringUtils::Int32ToString(userId));
            str.Append(" enabled=");
            str.Append(enabled);
            str.Append(" syncable=");
            str.Append(syncable);
            Logger::V(TAG, str);
        }

        if (NULL == *authority) {
            if (CSyncStorageEngine::DEBUG_FILE) {
                String str("Creating entry");
                Logger::V(TAG, str);
            }
            AutoPtr<IAccount> account;
//***            FAIL_RETURN(CAccount::New(accountName, accountType, (IAccount**)&account))
            FAIL_RETURN(GetOrCreateAuthorityLocked(account, userId, authorityName, id, FALSE /* doWrite */, authority))
            // If the version is 0 then we are upgrading from a file format that did not
            // know about periodic syncs. In that case don't clear the list since we
            // want the default, which is a daily periodioc sync.
            // Otherwise clear out this default list since we will populate it later with
            // the periodic sync descriptions that are read from the configuration file.
            if (version > 0) {
                AutoPtr< List<AutoPtr<IPair> > > periodicSyncs;
                FAIL_RETURN(((CSyncStorageEngineAuthorityInfo*) (*authority))->GetPeriodicSyncs((List<AutoPtr<IPair> > **)&periodicSyncs))
                if (NULL != periodicSyncs) {
                    periodicSyncs->Clear();
                }
            }
        }

        if (NULL != *authority) {
            FAIL_RETURN((*authority)->SetEnabled(enabled.IsNull() || enabled.Equals("TRUE")))
            if (String("unknown").Equals(syncable)) {
                FAIL_RETURN((*authority)->SetSyncable(-1))
            }
            else {
                FAIL_RETURN((*authority)->SetSyncable((syncable.IsNull() || syncable.Equals("TRUE")) ? 1 : 0))
            }
        }
        else {
            String str("Failure adding authority: account=");
            str.Append(accountName);
            str.Append(" auth=");
            str.Append(authorityName);
            str.Append(" enabled=");
            str.Append(enabled);
            str.Append(" syncable=");
            str.Append(syncable);
            Logger::W(TAG, str);
        }

    }

    return NOERROR;
}

ECode CSyncStorageEngine::ParsePeriodicSync(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ ISyncStorageEngineAuthorityInfo* authority,
    /* [out] */ IPair** pair)
{
    VALIDATE_NOT_NULL(pair)
    *pair = NULL;

    AutoPtr<IBundle> extras;
    FAIL_RETURN(CBundle::New((IBundle**)&extras))
    String periodValue;
    FAIL_RETURN(parser->GetAttributeValue(String(NULL), String("period"), &periodValue))
    Int64 period = 0;

    if (periodValue.IsNull()) {
        String str("error parsing the period of a periodic sync. the period of a periodic sync is null");
        Logger::E(TAG, str);
        return NOERROR;
    }
    else {
        period = StringUtils::ParseInt64(periodValue);
    }

    AutoPtr<IPair> periodicSync;
    AutoPtr<IInteger64> periodObj;
    FAIL_DOFINAL(CInteger64::New(period, (IInteger64**)&periodObj))
    FAIL_DOFINAL(CPair::New(extras, periodObj, (IPair**)&periodicSync))
    AutoPtr< List<AutoPtr<IPair> > > periodicSyncs;
    FAIL_RETURN(((CSyncStorageEngineAuthorityInfo*) authority)->GetPeriodicSyncs((List<AutoPtr<IPair> > **)&periodicSyncs))
    if (NULL != periodicSyncs) {
        periodicSyncs->PushBack(periodicSync);
    }

    *pair = periodicSync;
    REFCOUNT_ADD(*pair);
    return NOERROR;
}

ECode CSyncStorageEngine::ParseExtra(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IPair* periodicSync)
{
    AutoPtr<IBundle> extras;
    FAIL_RETURN(periodicSync->GetFirst((IInterface**)&extras))
    String name;
    FAIL_RETURN(parser->GetAttributeValue(String(NULL), String("name"), &name))
    String type;
    FAIL_RETURN(parser->GetAttributeValue(String(NULL), String("type"), &type))
    String strValue1;
    FAIL_RETURN(parser->GetAttributeValue(String(NULL), String("value1"), &strValue1))
    String strValue2;
    FAIL_RETURN(parser->GetAttributeValue(String(NULL), String("value2"), &strValue2))

    if (String("long").Equals(type)) {
        if (strValue1.IsNull()) {
            String str("error parsing bundle value. strValue1 is null");
            Logger::E(TAG, str);
        }
        else {
            Int64 value1 = StringUtils::ParseInt64(strValue1);
            FAIL_RETURN(extras->PutInt64(name, value1))
        }
    }
    else if (String("integer").Equals(type)) {
        if (strValue1.IsNull()) {
            String str("error parsing bundle value. strValue1 is null");
            Logger::E(TAG, str);
        }
        else {
            Int32 value1 = StringUtils::ParseInt32(strValue1);
            FAIL_RETURN(extras->PutInt32(name, value1))
        }
    }
    else if (String("double").Equals(type)) {
        if (strValue1.IsNull()) {
            String str("error parsing bundle value. strValue1 is null");
            Logger::E(TAG, str);
        }
        else {
            Double value1 = StringUtils::ParseDouble(strValue1);
            FAIL_RETURN(extras->PutDouble(name, value1))
        }
    }
    else if (String("float").Equals(type)) {
        if (strValue1.IsNull()) {
            String str("error parsing bundle value. strValue1 is null");
            Logger::E(TAG, str);
        }
        else {
            Float value1 = StringUtils::ParseFloat(strValue1);
            FAIL_RETURN(extras->PutFloat(name, value1))
        }
    }
    else if (String("boolean").Equals(type)) {
        if (strValue1.IsNull()) {
            String str("error parsing bundle value. strValue1 is null");
            Logger::E(TAG, str);
        }
        else {
            Boolean value1 = strValue1.EqualsIgnoreCase("TRUE");
            FAIL_RETURN(extras->PutBoolean(name, value1))
        }
    }
    else if (String("string").Equals(type)) {
        FAIL_RETURN(extras->PutString(name, strValue1))
    }
    else if (String("account").Equals(type)) {
        AutoPtr<IAccount> account;
//***        FAIL_RETURN(CAccount::New(strValue1, strValue2, (IAccount**)&account))
        FAIL_RETURN(extras->PutParcelable(name, (IParcelable*) account->Probe(EIID_IParcelable)))
    }

    return NOERROR;
}

ECode CSyncStorageEngine::WriteAccountInfoLocked()
{
    if (DEBUG_FILE) {
        String str("Writing new ");
//        mAccountInfoFile.getBaseFile();
        Logger::V(TAG, str);
    }

    AutoPtr<IFileOutputStream> fos;
    ECode ecode = NOERROR;
    {
//        fos = mAccountInfoFile.startWrite();
        AutoPtr<IXmlSerializer> out;
        // XmlSerializer out = new FastXmlSerializer();
//***        ecode = CXmlSerializer::New((IXmlSerializer**)&out);
        FAIL_GOTO(ecode, EXIT)
        ecode = out->SetOutput(fos, String("utf-8"));
        FAIL_GOTO(ecode, EXIT)
        AutoPtr<IBoolean> boolObj;
        ecode = CBoolean::New(TRUE, (IBoolean**)&boolObj);
        FAIL_GOTO(ecode, EXIT)
        ecode = out->StartDocument(String(NULL), boolObj);
        FAIL_GOTO(ecode, EXIT)
        ecode = out->SetFeature(String("http://xmlpull.org/v1/doc/features.html#indent-output"), TRUE);
        FAIL_GOTO(ecode, EXIT)
        ecode = out->WriteStartTag(String(NULL), String("accounts"));
        FAIL_GOTO(ecode, EXIT)
        ecode = out->WriteAttribute(String(NULL), String("version"), StringUtils::Int32ToString(ACCOUNTS_VERSION));
        FAIL_GOTO(ecode, EXIT)
        ecode = out->WriteAttribute(String(NULL), XML_ATTR_NEXT_AUTHORITY_ID, StringUtils::Int32ToString(mNextAuthorityId));
        FAIL_GOTO(ecode, EXIT)
        ecode = out->WriteAttribute(String(NULL), XML_ATTR_SYNC_RANDOM_OFFSET, StringUtils::Int32ToString(mSyncRandomOffset));
        FAIL_GOTO(ecode, EXIT)

        // Write the Sync Automatically flags for each user
        const Int32 M = mMasterSyncAutomatically->Size();
        for (Int32 m = 0; m < M; m++) {
            Int32 userId = mMasterSyncAutomatically->KeyAt(m);
            AutoPtr<IBoolean> listenObj = (IBoolean*) mMasterSyncAutomatically->ValueAt(m);
            Boolean listen = FALSE;
            ecode = listenObj->GetValue(&listen);
            FAIL_GOTO(ecode, EXIT)
            ecode = out->WriteStartTag(String(NULL), XML_TAG_LISTEN_FOR_TICKLES);
            FAIL_GOTO(ecode, EXIT)
            ecode = out->WriteAttribute(String(NULL), XML_ATTR_USER, StringUtils::Int32ToString(userId));
            FAIL_GOTO(ecode, EXIT)
            ecode = out->WriteAttribute(String(NULL), XML_ATTR_ENABLED, StringUtils::BooleanToString(listen));
            FAIL_GOTO(ecode, EXIT)
            ecode = out->WriteEndTag(String(NULL), XML_TAG_LISTEN_FOR_TICKLES);
            FAIL_GOTO(ecode, EXIT)
        }

        const Int32 N = mAuthorities->Size();
        AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
        AutoPtr<IAccount> account;
        String name;
        String type;
        String auth;
        Boolean enabled = FALSE;
        Int32 ident = 0;
        Int32 userId = 0;
        Int32 syncable = 0;

        AutoPtr< List<AutoPtr<IPair> > > periodicSyncs;
        for (Int32 i = 0; i < N; i++) {
            authority = (ISyncStorageEngineAuthorityInfo*) mAuthorities->ValueAt(i);
            ecode = out->WriteStartTag(String(NULL), String("authority"));
            FAIL_GOTO(ecode, EXIT)
            ecode = authority->GetIdent(&ident);
            FAIL_GOTO(ecode, EXIT)
            ecode = out->WriteAttribute(String(NULL), String("id"), StringUtils::Int32ToString(ident));
            FAIL_GOTO(ecode, EXIT)
            ecode = authority->GetAccount((IAccount**)&account);
            FAIL_GOTO(ecode, EXIT)
            ecode = account->GetName(&name);
            FAIL_GOTO(ecode, EXIT)
            ecode = out->WriteAttribute(String(NULL), String("account"), name);
            FAIL_GOTO(ecode, EXIT)
            ecode = authority->GetUserId(&userId);
            FAIL_GOTO(ecode, EXIT)
            ecode = out->WriteAttribute(String(NULL), XML_ATTR_USER, StringUtils::Int32ToString(userId));
            FAIL_GOTO(ecode, EXIT)
            ecode = account->GetType(&type);
            FAIL_GOTO(ecode, EXIT)
            ecode = out->WriteAttribute(String(NULL), String("type"), type);
            FAIL_GOTO(ecode, EXIT)
            ecode = authority->GetAuthority(&auth);
            FAIL_GOTO(ecode, EXIT)
            ecode = out->WriteAttribute(String(NULL), String("authority"), auth);
            FAIL_GOTO(ecode, EXIT)
            ecode = authority->GetEnabled(&enabled);
            FAIL_GOTO(ecode, EXIT)
            ecode = out->WriteAttribute(String(NULL), XML_ATTR_ENABLED, StringUtils::BooleanToString(enabled));
            FAIL_GOTO(ecode, EXIT)
            ecode = authority->GetSyncable(&syncable);
            FAIL_GOTO(ecode, EXIT)
            if (syncable < 0) {
                ecode = out->WriteAttribute(String(NULL), String("syncable"), String("unknown"));
                FAIL_GOTO(ecode, EXIT)
            }
            else {
                ecode = out->WriteAttribute(String(NULL), String("syncable"), StringUtils::BooleanToString(syncable != 0));
                FAIL_GOTO(ecode, EXIT)
            }

            ecode = ((CSyncStorageEngineAuthorityInfo*) authority.Get())->GetPeriodicSyncs((List<AutoPtr<IPair> > **)&periodicSyncs);
            FAIL_GOTO(ecode, EXIT)
            List<AutoPtr<IPair> >::Iterator iter = periodicSyncs->Begin();
            AutoPtr<IPair> item;
            AutoPtr<IBundle> extras;
            AutoPtr<IInteger64> secondObj;
            Int64 second = 0;

            for (; iter != periodicSyncs->End(); ++iter) {
                item = *iter;
                extras = NULL;
                ecode = item->GetFirst((IInterface**)&extras);
                FAIL_GOTO(ecode, EXIT)
                secondObj = NULL;
                ecode = item->GetSecond((IInterface**)&secondObj);
                FAIL_GOTO(ecode, EXIT)
                ecode = secondObj->GetValue(&second);
                FAIL_GOTO(ecode, EXIT)
                ecode = out->WriteStartTag(String(NULL), String("periodicSync"));
                FAIL_GOTO(ecode, EXIT)
                ecode = out->WriteAttribute(String(NULL), String("period"), StringUtils::Int64ToString(second));
                FAIL_GOTO(ecode, EXIT)
                AutoPtr<IObjectContainer> stringSet;
                AutoPtr<IObjectEnumerator> ObjEnumerator;
                ecode = extras->KeySet((IObjectContainer**)&stringSet);
                FAIL_GOTO(ecode, EXIT)
                if (NULL != stringSet) {
                    AutoPtr<ICharSequence> stringKey;
                    AutoPtr<IInterface> value;
                    String key;
                    Boolean hasNext = FALSE;
                    ecode = stringSet->GetObjectEnumerator((IObjectEnumerator**)&ObjEnumerator);
                    FAIL_GOTO(ecode, EXIT)
                    while ((ObjEnumerator->MoveNext(&hasNext), hasNext)) {
                        stringKey = NULL;
                        ecode = ObjEnumerator->Current((IInterface**)&stringKey);
                        FAIL_GOTO(ecode, EXIT)
                        ecode = stringKey->ToString(&key);
                        FAIL_GOTO(ecode, EXIT)
                        ecode = out->WriteStartTag(String(NULL), String("extra"));
                        FAIL_GOTO(ecode, EXIT)
                        ecode = out->WriteAttribute(String(NULL), String("name"), key);
                        FAIL_GOTO(ecode, EXIT)
                        value = NULL;
                        ecode = extras->Get(key, (IInterface**)&value);
                        FAIL_GOTO(ecode, EXIT)
                        if (IInteger64::Probe(value) != NULL) {
                            ecode = out->WriteAttribute(String(NULL), String("type"), String("long"));
                            FAIL_GOTO(ecode, EXIT)
                            Int64 tmp = 0;
                            ecode = ((IInteger64*) (IInterface*) value)->GetValue(&tmp);
                            FAIL_GOTO(ecode, EXIT)
                            ecode = out->WriteAttribute(String(NULL), String("value1"), StringUtils::Int64ToString(tmp));
                            FAIL_GOTO(ecode, EXIT)
                        }
                        else if (IInteger32::Probe(value) != NULL) {
                            ecode = out->WriteAttribute(String(NULL), String("type"), String("integer"));
                            FAIL_GOTO(ecode, EXIT)
                            Int32 tmp = 0;
                            ecode = ((IInteger32*) (IInterface*) value)->GetValue(&tmp);
                            FAIL_GOTO(ecode, EXIT)
                            ecode = out->WriteAttribute(String(NULL), String("value1"), StringUtils::Int32ToString(tmp));
                            FAIL_GOTO(ecode, EXIT)
                        }
                        else if (IBoolean::Probe(value) != NULL) {
                            ecode = out->WriteAttribute(String(NULL), String("type"), String("boolean"));
                            FAIL_GOTO(ecode, EXIT)
                            Boolean tmp = FALSE;
                            ecode = ((IBoolean*) (IInterface*) value)->GetValue(&tmp);
                            FAIL_GOTO(ecode, EXIT)
                            ecode = out->WriteAttribute(String(NULL), String("value1"), StringUtils::BooleanToString(tmp));
                            FAIL_GOTO(ecode, EXIT)
                        }
                        else if (IFloat::Probe(value) != NULL) {
                            ecode = out->WriteAttribute(String(NULL), String("type"), String("float"));
                            FAIL_GOTO(ecode, EXIT)
                            Float tmp = 0.0f;
                            ecode = ((IFloat*) (IInterface*) value)->GetValue(&tmp);
                            FAIL_GOTO(ecode, EXIT)
                            ecode = out->WriteAttribute(String(NULL), String("value1"), StringUtils::DoubleToString((Double)tmp));
                            FAIL_GOTO(ecode, EXIT)
                        }
                        else if (IDouble::Probe(value) != NULL) {
                            ecode = out->WriteAttribute(String(NULL), String("type"), String("double"));
                            FAIL_GOTO(ecode, EXIT)
                            Double tmp = 0.0;
                            ecode = ((IDouble*) (IInterface*) value)->GetValue(&tmp);
                            FAIL_GOTO(ecode, EXIT)
                            ecode = out->WriteAttribute(String(NULL), String("value1"), StringUtils::DoubleToString(tmp));
                            FAIL_GOTO(ecode, EXIT)
                        }
                        else if (ICharSequence::Probe(value) != NULL) {
                            ecode = out->WriteAttribute(String(NULL), String("type"), String("string"));
                            FAIL_GOTO(ecode, EXIT)
                            String tmp;
                            ecode = ((ICharSequence*) (IInterface*) value)->ToString(&tmp);
                            FAIL_GOTO(ecode, EXIT)
                            ecode = out->WriteAttribute(String(NULL), String("value1"), tmp);
                            FAIL_GOTO(ecode, EXIT)
                        }
                        else if (IAccount::Probe(value) != NULL) {
                            ecode = out->WriteAttribute(String(NULL), String("type"), String("account"));
                            FAIL_GOTO(ecode, EXIT)
                            String tmpName;
                            String tmpType;
                            ecode = ((IAccount*) (IInterface*) value)->GetName(&tmpName);
                            FAIL_GOTO(ecode, EXIT)
                            ecode = ((IAccount*) (IInterface*) value)->GetType(&tmpType);
                            FAIL_GOTO(ecode, EXIT)
                            ecode = out->WriteAttribute(String(NULL), String("value1"), tmpName);
                            FAIL_GOTO(ecode, EXIT)
                            ecode = out->WriteAttribute(String(NULL), String("value2"), tmpType);
                            FAIL_GOTO(ecode, EXIT)
                        }
                        ecode = out->WriteEndTag(String(NULL), String("extra"));
                        FAIL_GOTO(ecode, EXIT)
                    }
                    ecode = out->WriteEndTag(String(NULL), String("periodicSync"));
                    FAIL_GOTO(ecode, EXIT)
                }
            }

            ecode = out->WriteEndTag(String(NULL), String("authority"));
            FAIL_GOTO(ecode, EXIT)
        }
        ecode = out->WriteEndTag(String(NULL), String("accounts"));
        FAIL_GOTO(ecode, EXIT)
        ecode = out->EndDocument();
        FAIL_GOTO(ecode, EXIT)
//        mAccountInfoFile.finishWrite(fos);
    }
EXIT:
    if (E_IO_EXCEPTION == ecode) {
        String str("Error writing accounts, ecode == E_IO_EXCEPTION");
        Logger::W(TAG, str);
        if (NULL != fos) {
//            mAccountInfoFile.failWrite(fos);
        }
    }
    FAIL_RETURN(ecode)
    return NOERROR;
}

ECode CSyncStorageEngine::ReadAndDeleteLegacyAccountInfoLocked()
{
    // Look for old database to initialize from.
    AutoPtr<IFile> file;
    //TODO: file = mContext.getDatabasePath("syncmanager.db");
    Boolean isExist = FALSE;
    FAIL_RETURN(file->Exists(&isExist))
    if (!isExist) {
        return NOERROR;
    }
    String path;
    FAIL_RETURN(file->GetPath(&path))
    AutoPtr<ISQLiteDatabase> db;
    AutoPtr<ISQLiteDatabaseHelper> databaseHelper;
//***    FAIL_RETURN(CSQLiteDatabaseHelper::AcquireSingleton((ISQLiteDatabaseHelper**)&databaseHelper))
    FAIL_RETURN(databaseHelper->OpenDatabase(path, NULL, ISQLiteDatabase::OPEN_READONLY, (ISQLiteDatabase**)&db))

    if (NULL != db) {
        Int32 versions = 0;
        FAIL_RETURN(db->GetVersion(&versions))
        const Boolean hasType = versions >= 11;
        // Copy in all of the status information, as well as accounts.
        if (CSyncStorageEngine::DEBUG_FILE) {
            String str("Reading legacy sync accounts db");
            Logger::V(TAG, str);
        }
        AutoPtr<ISQLiteQueryBuilder> qb;
//***        FAIL_RETURN(CSQLiteQueryBuilder::New((ISQLiteQueryBuilder**)&qb))
        FAIL_RETURN(qb->SetTables(String("stats, status")))
        AutoPtr<IObjectStringMap> objMap;
        AutoPtr<ICharSequence> value;
        FAIL_RETURN(CObjectStringMap::New((IObjectStringMap**)&objMap))
        FAIL_RETURN(CString::New(String("status._id as _id"), (ICharSequence**)&value))
        FAIL_RETURN(objMap->Put(String("_id"), (IInterface*) value))
        value = NULL;
        FAIL_RETURN(CString::New(String("stats.account as account"), (ICharSequence**)&value))
        FAIL_RETURN(objMap->Put(String("account"), (IInterface*) value))
        if (hasType) {
            value = NULL;
            FAIL_RETURN(CString::New(String("stats.account_type as account_type"), (ICharSequence**)&value))
            FAIL_RETURN(objMap->Put(String("account_type"), (IInterface*) value))
        }
        value = NULL;
        FAIL_RETURN(CString::New(String("stats.authority as authority"), (ICharSequence**)&value))
        FAIL_RETURN(objMap->Put(String("authority"), (IInterface*) value))
        value = NULL;
        FAIL_RETURN(CString::New(String("totalElapsedTime"), (ICharSequence**)&value))
        FAIL_RETURN(objMap->Put(String("totalElapsedTime"), (IInterface*) value))
        value = NULL;
        FAIL_RETURN(CString::New(String("numSyncs"), (ICharSequence**)&value))
        FAIL_RETURN(objMap->Put(String("numSyncs"), (IInterface*) value))
        value = NULL;
        FAIL_RETURN(CString::New(String("numSourceLocal"), (ICharSequence**)&value))
        FAIL_RETURN(objMap->Put(String("numSourceLocal"), (IInterface*) value))
        value = NULL;
        FAIL_RETURN(CString::New(String("numSourcePoll"), (ICharSequence**)&value))
        FAIL_RETURN(objMap->Put(String("numSourcePoll"), (IInterface*) value))
        value = NULL;
        FAIL_RETURN(CString::New(String("numSourceServer"), (ICharSequence**)&value))
        FAIL_RETURN(objMap->Put(String("numSourceServer"), (IInterface*) value))
        value = NULL;
        FAIL_RETURN(CString::New(String("numSourceUser"), (ICharSequence**)&value))
        FAIL_RETURN(objMap->Put(String("numSourceUser"), (IInterface*) value))
        value = NULL;
        FAIL_RETURN(CString::New(String("lastSuccessSource"), (ICharSequence**)&value))
        FAIL_RETURN(objMap->Put(String("lastSuccessSource"), (IInterface*) value))
        value = NULL;
        FAIL_RETURN(CString::New(String("lastSuccessTime"), (ICharSequence**)&value))
        FAIL_RETURN(objMap->Put(String("lastSuccessTime"), (IInterface*) value))
        value = NULL;
        FAIL_RETURN(CString::New(String("lastFailureSource"), (ICharSequence**)&value))
        FAIL_RETURN(objMap->Put(String("lastFailureSource"), (IInterface*) value))
        value = NULL;
        FAIL_RETURN(CString::New(String("lastFailureTime"), (ICharSequence**)&value))
        FAIL_RETURN(objMap->Put(String("lastFailureTime"), (IInterface*) value))
        value = NULL;
        FAIL_RETURN(CString::New(String("lastFailureMesg"), (ICharSequence**)&value))
        FAIL_RETURN(objMap->Put(String("lastFailureMesg"), (IInterface*) value))
        value = NULL;
        FAIL_RETURN(CString::New(String("pending"), (ICharSequence**)&value))
        FAIL_RETURN(objMap->Put(String("pending"), (IInterface*) value))
        FAIL_RETURN(qb->SetProjectionMap(objMap))
        String where("stats._id = status.stats_id");
        AutoPtr<ICharSequence> inWhere;
        FAIL_RETURN(CString::New(where, (ICharSequence**)&inWhere))
        FAIL_RETURN(qb->AppendWhere(inWhere))
        AutoPtr<ICursor> c;
        FAIL_RETURN(qb->Query(db, NULL, String(NULL), NULL, String(NULL), String(NULL), String(NULL), (ICursor**)&c))
        Boolean hasNext = FALSE;
        Int32 columnIndex = 0;

        while ((c->MoveToNext(&hasNext), hasNext)) {
            String accountName;
            FAIL_RETURN(c->GetColumnIndex(String("account"), &columnIndex))
            FAIL_RETURN(c->GetString(columnIndex, &accountName))
            String accountType;

            if(hasType){
                FAIL_RETURN(c->GetColumnIndex(String("account_type"), &columnIndex))
                FAIL_RETURN(c->GetString(columnIndex, &accountType))
            }
            if (accountType.IsNull()) {
                accountType = "com.google";
            }

            String authorityName;
            FAIL_RETURN(c->GetColumnIndex(String("authority"), &columnIndex))
            FAIL_RETURN(c->GetString(columnIndex, &authorityName))
            AutoPtr<IAccount> newAccount;
//***            FAIL_RETURN(CAccount::New(accountName, accountType, (IAccount**)&newAccount))
            AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
            FAIL_RETURN(GetOrCreateAuthorityLocked(newAccount, 0 /* legacy is single-user */, authorityName, -1,
                    FALSE, (ISyncStorageEngineAuthorityInfo**)&authority))

            if (NULL != authority) {
                Int32 i = mSyncStatus->Size();
                Boolean found = FALSE;
                AutoPtr<ISyncStatusInfo> st;
                Int32 ident = 0;
                Int32 authorityId = 0;
                while (i > 0) {
                    i--;
                    st = (ISyncStatusInfo*) mSyncStatus->ValueAt(i);
                    FAIL_RETURN(st->GetAuthorityId(&authorityId))
                    FAIL_RETURN(authority->GetIdent(&ident))
                    if (authorityId == ident) {
                        found = TRUE;
                        break;
                    }
                }
                if (!found) {
                    FAIL_RETURN(authority->GetIdent(&ident))
                    st = NULL;
                    FAIL_RETURN(CSyncStatusInfo::New(ident, (ISyncStatusInfo**)&st))
                    mSyncStatus->Put(ident, st);
                }
                Int32 intResult = 0;
                Int64 longResult = 0;
                FAIL_RETURN(GetLongColumn(c, String("totalElapsedTime"), &longResult))
                FAIL_RETURN(st->SetTotalElapsedTime(longResult))
                FAIL_RETURN(GetIntColumn(c, String("numSyncs"), &intResult))
                FAIL_RETURN(st->SetNumSyncs(intResult))
                FAIL_RETURN(GetIntColumn(c, String("numSourceLocal"), &intResult))
                FAIL_RETURN(st->SetNumSourceLocal(intResult))
                FAIL_RETURN(GetIntColumn(c, String("numSourcePoll"), &intResult))
                FAIL_RETURN(st->SetNumSourcePoll(intResult));
                FAIL_RETURN(GetIntColumn(c, String("numSourceServer"), &intResult))
                FAIL_RETURN(st->SetNumSourceServer(intResult))
                FAIL_RETURN(GetIntColumn(c, String("numSourceUser"), &intResult))
                FAIL_RETURN(st->SetNumSourceUser(intResult))
                FAIL_RETURN(st->SetNumSourcePeriodic(0))
                FAIL_RETURN(GetIntColumn(c, String("lastSuccessSource"), &intResult))
                FAIL_RETURN(st->SetLastSuccessSource(intResult))
                FAIL_RETURN(GetLongColumn(c, String("lastSuccessTime"), &longResult))
                FAIL_RETURN(st->SetLastSuccessTime(longResult))
                FAIL_RETURN(GetIntColumn(c, String("lastFailureSource"), &intResult))
                FAIL_RETURN(st->SetLastFailureSource(intResult))
                FAIL_RETURN(GetLongColumn(c, String("lastFailureTime"), &longResult))
                FAIL_RETURN(st->SetLastFailureTime(longResult))
                String lastFailureMesg;
                FAIL_RETURN(c->GetColumnIndex(String("lastFailureMesg"), &columnIndex))
                FAIL_RETURN(c->GetString(columnIndex, &lastFailureMesg))
                FAIL_RETURN(st->SetLastFailureMesg(lastFailureMesg))
                FAIL_RETURN(GetIntColumn(c, String("pending"), &intResult))
                FAIL_RETURN(st->SetPending(intResult != 0))
            }
        }

        FAIL_RETURN(c->Close())
        // Retrieve the settings.
//***        FAIL_RETURN(CSQLiteQueryBuilder::New((ISQLiteQueryBuilder**)&qb))
        FAIL_RETURN(qb->SetTables(String("settings")))
        c = NULL;
        FAIL_RETURN(qb->Query(db, NULL, String(NULL), NULL, String(NULL), String(NULL), String(NULL), (ICursor**)&c))

        String prefix("sync_provider");
        Int32 prefixLength = prefix.GetLength();
        while ((c->MoveToNext(&hasNext),hasNext)) {
            String name;
            String value;
            FAIL_RETURN(c->GetColumnIndex(String("name"), &columnIndex))
            FAIL_RETURN(c->GetString(columnIndex, &name))
            FAIL_RETURN(c->GetColumnIndex(String("value"), &columnIndex))
            FAIL_RETURN(c->GetString(columnIndex, &value))
            if (name.IsNull()) continue;
            if (name.Equals(String("listen_for_tickles"))) {
                FAIL_RETURN(SetMasterSyncAutomatically(value.IsNull() || value.Equals("TRUE"), 0))
            }
            else if (name.StartWith(prefix)) {
                String provider = name.Substring(prefixLength, name.GetLength());
                Int32 i = mAuthorities->Size();
                while (i > 0) {
                    i--;
                    AutoPtr<ISyncStorageEngineAuthorityInfo> authority = (ISyncStorageEngineAuthorityInfo*) mAuthorities->ValueAt(i);
                    String strAuthority;
                    FAIL_RETURN(authority->GetAuthority(&strAuthority))
                    if (strAuthority.Equals(provider)) {
                        FAIL_RETURN(authority->SetEnabled(value.IsNull() || value.Equals("TRUE")))
                        FAIL_RETURN(authority->SetSyncable(1))
                    }
                }
            }
        }

        FAIL_RETURN(c->Close())
        FAIL_RETURN(db->Close())
        AutoPtr<IFile> newFile;
        Boolean isDelete = FALSE;
        FAIL_RETURN(CFile::New(path, (IFile**)&newFile))
        FAIL_RETURN(newFile->Delete(&isDelete))
    }

    return NOERROR;
}

ECode CSyncStorageEngine::ReadStatusLocked()
{
    if (DEBUG_FILE) {
        String str("Reading ");
//        mStatusFile.getBaseFile();
        Logger::V(TAG, str);
    }

    AutoPtr<ArrayOf<Byte> > data;
//    mStatusFile.readFully();
    AutoPtr<IParcel> in;
    // in = Parcel.obtain();
//***    FAIL_RETURN(CParcel::New((IParcel**)&in))
//    in.unmarshall(data, 0, data.length);
    FAIL_RETURN(in->SetDataPosition(0))
    Int32 token = 0;

    while ((in->ReadInt32(&token), token) != ISyncStorageEngine::STATUS_FILE_END) {
        if (token == ISyncStorageEngine::STATUS_FILE_ITEM) {
            AutoPtr<ISyncStatusInfo> status;
            FAIL_RETURN(CSyncStatusInfo::New(0, (ISyncStatusInfo**)&status))
            FAIL_RETURN(((IParcelable*) status->Probe(EIID_IParcelable))->ReadFromParcel(in))
            Int32 authorityId = 0;
            FAIL_RETURN(status->GetAuthorityId(&authorityId))
            if (mAuthorities->IndexOfKey(authorityId) >= 0) {
                FAIL_RETURN(status->SetPending(FALSE))
                if (DEBUG_FILE) {
                    String str("Adding status for id ");
                    str.Append(StringUtils::Int32ToString(authorityId));
                    Logger::V(TAG, str);
                }
                mSyncStatus->Put(authorityId, status);
            }
        }
        else {
            // Ooops.
            String str("Unknown status token: ");
            str.Append(StringUtils::Int32ToString(token));
            Logger::W(TAG, str);
            break;
        }
    }

    return NOERROR;
}

ECode CSyncStorageEngine::WriteStatusLocked()
{
    if (CSyncStorageEngine::DEBUG_FILE){
        String str("Writing new ");
        //TODO: mStatusFile.getBaseFile();
        Logger::V(TAG, str);
    }

    // The file is being written, so we don't need to have a scheduled
    // write until the next change.
    RemoveMessages(MSG_WRITE_STATUS);

    AutoPtr<IFileOutputStream> fos;
    ECode ecode = NOERROR;
//    fos = mStatusFile.startWrite();
    AutoPtr<IParcel> out;
    AutoPtr<ISyncStatusInfo> status;
    const Int32 N = mSyncStatus->Size();
    // out = Parcel.obtain();
//***    ecode = CParcel::New((IParcel**)&in);
    FAIL_GOTO(ecode, EXIT)

    for (Int32 i = 0; i < N; i++) {
        status = (ISyncStatusInfo*) mSyncStatus->ValueAt(i);
        ecode = out->WriteInt32(ISyncStorageEngine::STATUS_FILE_ITEM);
        FAIL_GOTO(ecode, EXIT)
        ecode = IParcelable::Probe(status)->WriteToParcel(out);
        FAIL_GOTO(ecode, EXIT)
    }

    ecode = out->WriteInt32(ISyncStorageEngine::STATUS_FILE_END);
    FAIL_GOTO(ecode, EXIT)
    //TODO: fos->WriteBuffer(out.marshall());
    //TODO: out.recycle();
    //TODO: mStatusFile.finishWrite(fos);

EXIT:
    if (E_IO_EXCEPTION == ecode) {
        String str("Error writing status, ecode == E_IO_EXCEPTION");
        Logger::W(TAG, str);
        if (NULL != fos) {
//            mStatusFile.failWrite(fos);
        }
    }
    FAIL_RETURN(ecode)
    return NOERROR;
}

ECode CSyncStorageEngine::ReadPendingOperationsLocked()
{
    if (DEBUG_FILE) {
        String str("Reading ");
//        mPendingFile.getBaseFile();
        Logger::V(TAG, str);
    }

    AutoPtr<ArrayOf<Byte> > data;
//    data = mPendingFile.readFully();
    AutoPtr<IParcel> in;
    // in = Parcel.obtain();
//***    FAIL_RETURN(CParcel::New((IParcel**)&in))
//    in.unmarshall(data, 0, data.length);
    FAIL_RETURN(in->SetDataPosition(0))
    Int32 SIZE = 0;
    FAIL_RETURN(in->GetElementSize(&SIZE))
    Int32 position = 0;

    while ((in->GetDataPosition(&position), position) < SIZE) {
        Int32 version = 0;
        FAIL_RETURN(in->ReadInt32(&version))

        if (version != ISyncStorageEngine::PENDING_OPERATION_VERSION && version != 1) {
            String str("Unknown pending operation version ");
            str.Append(StringUtils::Int32ToString(version));
            str.Append("; dropping all ops");
            Logger::W(TAG, str);
            break;
        }

        Int32 authorityId = 0;
        FAIL_RETURN(in->ReadInt32(&authorityId))
        Int32 syncSource = 0;
        FAIL_RETURN(in->ReadInt32(&syncSource))
        AutoPtr<ArrayOf<Byte> > flatExtras;
//        flatExtras = in.createByteArray();
        Boolean expedited = FALSE;

        if (version == ISyncStorageEngine::PENDING_OPERATION_VERSION) {
            Int32 tmp = 0;
            FAIL_RETURN(in->ReadInt32(&tmp))
            expedited =  tmp != 0;
        }
        else {
            expedited = FALSE;
        }

        AutoPtr<ISyncStorageEngineAuthorityInfo> authority = (ISyncStorageEngineAuthorityInfo*) mAuthorities->Get(authorityId);
        if (NULL != authority) {
            AutoPtr<IBundle> extras;
            if (NULL != flatExtras) {
                FAIL_RETURN(UnflattenBundle(flatExtras, (IBundle**)&extras))
            }
            else {
                // if we are unable to parse the extras for whatever reason convert this
                // to a regular sync by creating an empty extras
                FAIL_RETURN(CBundle::New((IBundle**)&extras))
            }
            AutoPtr<IAccount> account;
            Int32 userId = 0;
            String strAuthority;
            FAIL_RETURN(authority->GetAccount((IAccount**)&account))
            FAIL_RETURN(authority->GetUserId(&userId))
            FAIL_RETURN(authority->GetAuthority(&strAuthority))
            AutoPtr<ISyncStorageEnginePendingOperation> op;
            FAIL_RETURN(CSyncStorageEnginePendingOperation::New(account, userId, syncSource, strAuthority, extras, expedited,
                    (ISyncStorageEnginePendingOperation**)&op))
            FAIL_RETURN(op->SetAuthorityId(authorityId))
            FAIL_RETURN(op->SetFlatExtras(flatExtras))

            if (DEBUG_FILE){
                String str("Adding pending op: account=");
                if(NULL != account){
                    String tmp;
                    FAIL_RETURN(account->ToString(&tmp))
                    str.Append(tmp);
                }
                else {
                    str.Append(" null ");
                }
                str.Append(" auth=");
                str.Append(strAuthority);
                str.Append(" src=");
                str.Append(StringUtils::Int32ToString(syncSource));
                str.Append(" expedited=");
                if(expedited) {
                    str.Append(" true ");
                }
                else {
                    str.Append(" false ");
                }
                str.Append(" extras=");
                extras = NULL;
                FAIL_RETURN(op->GetExtras((IBundle**)&extras))
                String tmp;
                FAIL_RETURN(extras->ToString(&tmp))
                str.Append(tmp);
                Logger::V(TAG, str);
            }

            mPendingOperations->PushBack(op);
        }
    }

    return NOERROR;
}

ECode CSyncStorageEngine::WritePendingOperationLocked(
    /* [in] */ ISyncStorageEnginePendingOperation* op,
    /* [in] */ IParcel* out)
{
    FAIL_RETURN(out->WriteInt32(ISyncStorageEngine::PENDING_OPERATION_VERSION))
    Int32 authorityId = 0;
    FAIL_RETURN(op->GetAuthorityId(&authorityId))
    FAIL_RETURN(out->WriteInt32(authorityId))
    Int32 syncSource = 0;
    FAIL_RETURN(op->GetSyncSource(&syncSource))
    FAIL_RETURN(out->WriteInt32(syncSource))
    AutoPtr<ArrayOf<Byte> > flatExtras;
    FAIL_RETURN(op->GetFlatExtras((ArrayOf<Byte>**)&flatExtras))
    AutoPtr<IBundle> extras;
    FAIL_RETURN(op->GetExtras((IBundle**)&extras))
    if (NULL == flatExtras && NULL != extras) {
        FAIL_RETURN(FlattenBundle(extras, (ArrayOf<Byte>**)&flatExtras))
        FAIL_RETURN(op->SetFlatExtras(flatExtras))
    }
    FAIL_RETURN(out->WriteArrayOf((Handle32)flatExtras.Get()))
    Boolean expedited = FALSE;
    FAIL_RETURN(op->GetExpedited(&expedited))
    FAIL_RETURN(out->WriteInt32(expedited ? 1 : 0))
    return NOERROR;
}

ECode CSyncStorageEngine::WritePendingOperationsLocked()
{
    const Int32 N = mPendingOperations->GetSize();
    AutoPtr<IFileOutputStream> fos;
    ECode ecode = NOERROR;

    if (N == 0) {
        if (DEBUG_FILE) {
            String str("Truncating ");
//            mPendingFile.getBaseFile();
            Logger::V(TAG, str);
        }
//        mPendingFile.truncate();
        return NOERROR;
    }

    if (DEBUG_FILE) {
        String str("Writing new ");
        //TODO: mPendingFile.getBaseFile();
        Logger::V(TAG, str);
    }

//    fos = mPendingFile.startWrite();
    AutoPtr<IParcel> out;
    AutoPtr<ISyncStorageEnginePendingOperation> op;
    // out = Parcel.obtain();
//***    ecode = CParcel::New((IParcel**)&out);
    FAIL_GOTO(ecode, EXIT)
    for (Int32 i = 0; i < N; i++) {
        op = (*mPendingOperations)[i];
        ecode = WritePendingOperationLocked(op, out);
        FAIL_GOTO(ecode, EXIT)
    }

//    fos->WriteBuffer(out.marshall());
//    out.recycle();
//    mPendingFile.finishWrite(fos);

EXIT:
    if (E_IO_EXCEPTION == ecode) {
        String str("Error writing pending operations, ecode == E_IO_EXCEPTION");
        Logger::W(TAG, str);
        if (NULL != fos) {
//            mPendingFile.failWrite(fos);
        }
    }
    FAIL_RETURN(ecode)
    return NOERROR;
}

ECode CSyncStorageEngine::AppendPendingOperationLocked(
    /* [in] */ ISyncStorageEnginePendingOperation* op)
{
    if (CSyncStorageEngine::DEBUG_FILE) {
        String str("Appending to ");
//        mPendingFile.getBaseFile();
        Logger::V(TAG, str);
    }

    AutoPtr<IFileOutputStream> fos;
    ECode ecode = NOERROR;
//    fos = mPendingFile.openAppend();
    if (E_IO_EXCEPTION == ecode) {
        if (DEBUG_FILE){
            String str("Failed append; writing full file");
            Logger::W(TAG, str);
        }
        FAIL_RETURN(WritePendingOperationsLocked())
        return ecode;
    }

    FAIL_RETURN(ecode)
    AutoPtr<IParcel> out;
    // out = Parcel.obtain();
//***    ecode = CParcel::New((IParcel**)&out);
    FAIL_GOTO(ecode, EXIT)
    ecode = WritePendingOperationLocked(op, out);
    FAIL_GOTO(ecode, EXIT);
//    fos->WriteBuffer(out.marshall());
//    out.recycle();

EXIT:
    if (E_IO_EXCEPTION == ecode) {
        String str("Error writing pending operations, ecode == E_IO_EXCEPTION");
        Logger::W(TAG, str);
    }
    if (NULL != fos) {
        FAIL_RETURN(fos->Close())
    }
    FAIL_RETURN(ecode)
    return NOERROR;
}

ECode CSyncStorageEngine::RequestSync(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras)
{
    // If this is happening in the system process, then call the syncrequest listener
    // to make a request back to the SyncManager directly.
    // If this is probably a test instance, then call back through the ContentResolver
    // which will know which userId to apply based on the Binder id.
    Int32 uid = 0;
//    uid = android.os.Process.myUid();
    if (uid == 1000 /* android.os.Process.SYSTEM_UID */ && mSyncRequestListener != NULL) {
        FAIL_RETURN(mSyncRequestListener->OnSyncRequest(account, userId, authority, extras))
    }
    else {
        AutoPtr<IContentResolverHelper> resolverHelper;
        FAIL_RETURN(CContentResolverHelper::AcquireSingleton((IContentResolverHelper**)&resolverHelper))
        FAIL_RETURN(resolverHelper->RequestSync(account, authority, extras))
    }
    return NOERROR;
}

ECode CSyncStorageEngine::ReadStatisticsLocked()
{
    AutoPtr<ArrayOf<Byte> > data;
//    data = mStatisticsFile.readFully();
    AutoPtr<IParcel> in;
    // in = Parcel.obtain();
//***    FAIL_RETURN(CParcel::New((IParcel**)&in))
//    in.unmarshall(data, 0, data.length);
    FAIL_RETURN(in->SetDataPosition(0))
    Int32 token = 0;
    Int32 index = 0;

    while ((in->ReadInt32(&token), token) != ISyncStorageEngine::STATISTICS_FILE_END) {
        if (token == ISyncStorageEngine::STATISTICS_FILE_ITEM || token == ISyncStorageEngine::STATISTICS_FILE_ITEM_OLD) {
            Int32 day = 0;
            FAIL_RETURN(in->ReadInt32(&day))
            if (token == ISyncStorageEngine::STATISTICS_FILE_ITEM_OLD) {
                day = day - 2009 + 14245;  // Magic!
            }
            AutoPtr<ISyncStorageEngineDayStats> ds;
            FAIL_RETURN(CSyncStorageEngineDayStats::New(day, (ISyncStorageEngineDayStats**)&ds))
            Int32 intValue = 0;
            Int64 longValue = 0;
            FAIL_RETURN(in->ReadInt32(&intValue))
            FAIL_RETURN(ds->SetSuccessCount(intValue))
            FAIL_RETURN(in->ReadInt64(&longValue))
            FAIL_RETURN(ds->SetSuccessTime(longValue))
            FAIL_RETURN(in->ReadInt32(&intValue))
            FAIL_RETURN(ds->SetFailureCount(intValue))
            FAIL_RETURN(in->ReadInt64(&longValue))
            FAIL_RETURN(ds->SetFailureTime(longValue))
            if (index < mDayStats->GetLength()) {
                (*mDayStats)[index] = ds;
                index++;
            }
        }
        else {
            // Ooops.
            String str("Unknown stats token: ");
            str.Append(StringUtils::Int32ToString(token));
            Logger::W(TAG, str);
            break;
        }
    }

    return NOERROR;
}

ECode CSyncStorageEngine::WriteStatisticsLocked()
{
    if (DEBUG_FILE) {
        String str("Writing new ");
//        mStatisticsFile.getBaseFile();
        Logger::V(TAG, str);
    }

    // The file is being written, so we don't need to have a scheduled
    // write until the next change.
    RemoveMessages(MSG_WRITE_STATISTICS);

    AutoPtr<IFileOutputStream> fos;
    ECode ecode = NOERROR;
//    fos = mStatisticsFile.startWrite();
    AutoPtr<IParcel> out;
    // out = Parcel.obtain();
//***    FAIL_RETURN(CParcel::New((IParcel**)&out))
    const Int32 N = mDayStats->GetLength();
    AutoPtr<ISyncStorageEngineDayStats> ds;

    for (Int32 i = 0; i < N; i++) {
        ds = (*mDayStats)[i];
        if (NULL == ds) {
            break;
        }
        ecode = out->WriteInt32(ISyncStorageEngine::STATISTICS_FILE_ITEM);
        FAIL_GOTO(ecode, EXIT)
        Int32 intValue = 0;
        Int64 longValue = 0;
        ecode = ds->GetDay(&intValue);
        FAIL_GOTO(ecode, EXIT)
        ecode = out->WriteInt32(intValue);
        FAIL_GOTO(ecode, EXIT)
        ecode = ds->GetSuccessCount(&intValue);
        FAIL_GOTO(ecode, EXIT)
        ecode = out->WriteInt32(intValue);
        FAIL_GOTO(ecode, EXIT)
        ecode = ds->GetSuccessTime(&longValue);
        FAIL_GOTO(ecode, EXIT)
        ecode = out->WriteInt64(longValue);
        FAIL_GOTO(ecode, EXIT)
        ecode = ds->GetFailureCount(&intValue);
        FAIL_GOTO(ecode, EXIT)
        ecode = out->WriteInt32(intValue);
        FAIL_GOTO(ecode, EXIT)
        ecode = ds->GetFailureTime(&longValue);
        FAIL_GOTO(ecode, EXIT)
        ecode = out->WriteInt64(longValue);
        FAIL_GOTO(ecode, EXIT)
    }

    ecode = out->WriteInt32(ISyncStorageEngine::STATISTICS_FILE_END);
    FAIL_GOTO(ecode, EXIT)
//    fos->WriteBuffer(out.marshall());
//    out.recycle();
//    mStatisticsFile.finishWrite(fos);

EXIT:
    if (E_IO_EXCEPTION == ecode) {
        String str("Error writing stats, ecode == E_IO_EXCEPTION");
        Logger::W(TAG, str);
        if (NULL != fos) {
//            mStatisticsFile.failWrite(fos);
        }
    }
    FAIL_RETURN(ecode)
    return NOERROR;
}

ECode CSyncStorageEngine::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    if (what == MSG_WRITE_STATUS) {
        AutoLock lock(mAuthoritiesLock);
        FAIL_RETURN(WriteStatusLocked())
    }
    else if (what == MSG_WRITE_STATISTICS) {
        AutoLock lock(mAuthoritiesLock);
        FAIL_RETURN(WriteStatisticsLocked())
    }
    return NOERROR;
}

} // namespace Content
} // namespace Droid
} // namespace Elastos

