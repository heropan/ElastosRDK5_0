
#include "content/SyncQueue.h"
#include <elastos/Logger.h>
#include <elastos/Math.h>
#include "os/SystemClock.h"
//***#include "text/format/DateUtils.h"
#include "content/CSyncAdapterTypeHelper.h"
#include "content/CSyncStorageEnginePendingOperation.h"
#include <elastos/HashMap.h>
#include <elastos/List.h>
#include "content/CSyncOperation.h"
#include "content/pm/RegisteredServicesCache.h"
#include "elastos/StringUtils.h"

using Elastos::Core::IStringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Utility::Iterator;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::HashMap;
using Elastos::Utility::List;
using Elastos::Droid::Content::Pm::RegisteredServicesCache;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Utility::IPair;

namespace Elastos {
namespace Droid {
namespace Content {

const String SyncQueue::TAG = String("SyncManager");

SyncQueue::SyncQueue(
    /* [in] */ ISyncStorageEngine* engine,
    /* [in] */ SyncAdaptersCache* syncAdapters)
    : mSyncStorageEngine(engine)
    , mSyncAdapters(syncAdapters)
{}

SyncQueue::~SyncQueue()
{
}

CAR_INTERFACE_IMPL(SyncQueue, ISyncQueue)

ECode SyncQueue::AddPendingOperations(
    /* [in] */ Int32 userId)
{
    AutoPtr<IObjectContainer> objContainer;
    AutoPtr<IObjectEnumerator> objEnumerator;
    FAIL_RETURN(mSyncStorageEngine->GetPendingOperations((IObjectContainer**)&objContainer))

    if (NULL != objContainer) {
        FAIL_RETURN(objContainer->GetObjectEnumerator((IObjectEnumerator**)&objEnumerator))
        Boolean hasNext = FALSE;
        Boolean ret = FALSE;
        AutoPtr<ISyncStorageEnginePendingOperation> op;
        AutoPtr<IAccount> account;
        String type;
        Int32 uId = 0;
        String authority;

        while ((objEnumerator->MoveNext(&hasNext), hasNext)) {
            op = NULL;
            account = NULL;
            FAIL_RETURN(objEnumerator->Current((IInterface**)&op))
            FAIL_RETURN(op->GetUserId(&uId))
            if (uId != userId) continue;
            FAIL_RETURN(op->GetAccount((IAccount**)&account))
            FAIL_RETURN(op->GetAuthority(&authority))
            AutoPtr<IPair> backoff;
            FAIL_RETURN(mSyncStorageEngine->GetBackoff(account, uId, authority, (IPair**)&backoff))
            AutoPtr<ISyncAdapterTypeHelper> typeHelper;
            FAIL_RETURN(CSyncAdapterTypeHelper::AcquireSingleton((ISyncAdapterTypeHelper**)&typeHelper))
            FAIL_RETURN(account->GetType(&type))
            AutoPtr<ISyncAdapterType> adapterType;
            FAIL_RETURN(typeHelper->NewKey(authority, type, (ISyncAdapterType**)&adapterType))
            AutoPtr<RegisteredServicesCache::ServiceInfo> syncAdapterInfo = mSyncAdapters->GetServiceInfo((IInterface*) adapterType, uId);

            if (NULL == syncAdapterInfo) {
                Logger::W(TAG, String("Missing sync adapter info for authority ") + authority + String(", userId ")
                    + StringUtils::Int32ToString(uId));
                continue;
            }

            AutoPtr<IBundle> extras;
            Int32 syncSource = 0;
            AutoPtr<ISyncOperation> syncOperation;
            FAIL_RETURN(op->GetExtras((IBundle**)&extras))
            FAIL_RETURN(op->GetSyncSource(&syncSource))
            Int64 first = 0;
            if (NULL != backoff) {
                AutoPtr<IInteger64> backoffFirst;
                FAIL_RETURN(backoff->GetFirst((IInterface**)&backoffFirst))
                FAIL_RETURN(backoffFirst->GetValue(&first))
            }
            Int64 delayUntilTime = 0;
            FAIL_RETURN(mSyncStorageEngine->GetDelayUntilTime(account, uId, authority,
                    &delayUntilTime))
            adapterType = ISyncAdapterType::Probe(syncAdapterInfo->mType);
            Boolean isAllow = FALSE;
            FAIL_RETURN(adapterType->AllowParallelSyncs(&isAllow))
            FAIL_RETURN(CSyncOperation::New(account, uId, syncSource, authority, extras, 0 /* delay */,
                    first, delayUntilTime, isAllow, (ISyncOperation**)&syncOperation))
            Boolean expedited = FALSE;
            FAIL_RETURN(op->GetExpedited(&expedited))
            FAIL_RETURN(syncOperation->SetExpedited(expedited))
            FAIL_RETURN(syncOperation->SetPendingOperation(op))
            FAIL_RETURN(Add(syncOperation, op, &ret))
        }
    }

    return NOERROR;
}

ECode SyncQueue::Add(
    /* [in] */ ISyncOperation* operation,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return Add(operation, (ISyncStorageEnginePendingOperation*) NULL /* this is not coming from the database */, result);
}

ECode SyncQueue::RemoveUser(
    /* [in] */ Int32 userId)
{
    AutoPtr<List<AutoPtr<ISyncOperation> > > opsToRemove;
    AutoPtr<ISyncOperation> op;
    Int32 uId = 0;
    HashMap<String, AutoPtr<ISyncOperation> >::Iterator it = mOperationsMap->Begin();

    for (; it != mOperationsMap->End(); it++) {
        op = (*it).mSecond;
        FAIL_RETURN(op->GetUserId(&uId))
        if (uId == userId) {
            opsToRemove->PushBack(op);
        }
    }

    List<AutoPtr<ISyncOperation> >::Iterator iter = opsToRemove->Begin();
    for (; iter != opsToRemove->End(); iter++) {
        op = *iter;
        FAIL_RETURN(Remove(op))
    }

    return NOERROR;
}

ECode SyncQueue::Remove(
    /* [in] */ ISyncOperation* operation)
{
    AutoPtr<ISyncOperation> operationToRemove;
    String key;
    FAIL_RETURN(operation->GetKey(&key))
    HashMap<String, AutoPtr<ISyncOperation> >::Iterator it = mOperationsMap->Find(key);
    operationToRemove = it->mSecond;
    mOperationsMap->Erase(key);

    if (NULL == operationToRemove) {
        return NOERROR;
    }

    AutoPtr<ISyncStorageEnginePendingOperation> op;
    FAIL_RETURN(operationToRemove->GetPendingOperation((ISyncStorageEnginePendingOperation**)&op))
    Boolean ret = FALSE;
    FAIL_RETURN(mSyncStorageEngine->DeleteFromPending(op, &ret))

    if (!ret) {
        String errorMessage("unable to find pending row for ");
        String str;
        FAIL_RETURN(operationToRemove->ToString(&str))
        errorMessage += str;
        errorMessage += " IllegalStateException";
        //Log.e(TAG, errorMessage, new IllegalStateException(errorMessage));
        Logger::E(TAG, errorMessage);
    }

    return NOERROR;
}

ECode SyncQueue::OnBackoffChanged(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& providerName,
    /* [in] */ Int64 backoff)
{
    // for each op that matches the account and provider update its
    // backoff and effectiveStartTime
    AutoPtr<ISyncOperation> op;
    AutoPtr<IAccount> act;
    Int32 uId = 0;
    String authority;
    HashMap<String, AutoPtr<ISyncOperation> >::Iterator it = mOperationsMap->Begin();

    for (; it != mOperationsMap->End(); it++) {
        op = it->mSecond;
        act = NULL;
        FAIL_RETURN(op->GetUserId(&uId))
        FAIL_RETURN(op->GetAccount((IAccount**)&act))
        FAIL_RETURN(op->GetAuthority(&authority))
        if (_CObject_Compare(act, account) && authority.Equals(providerName) && uId == userId) {
            FAIL_RETURN(op->SetBackoff(backoff))
            FAIL_RETURN(op->UpdateEffectiveRunTime())
        }
    }

    return NOERROR;
}

ECode SyncQueue::OnDelayUntilTimeChanged(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ Int64 delayUntil)
{
    // for each op that matches the account and provider update its
    // delayUntilTime and effectiveStartTime
    AutoPtr<ISyncOperation> op;
    AutoPtr<IAccount> act;
    String authority;
    HashMap<String, AutoPtr<ISyncOperation> >::Iterator it = mOperationsMap->Begin();

    for (; it != mOperationsMap->End(); it++) {
        op = it->mSecond;
        act = NULL;
        FAIL_RETURN(op->GetAccount((IAccount**)&act))
        FAIL_RETURN(op->GetAuthority(&authority))
        if (_CObject_Compare(act, account) && authority.Equals(providerName)) {
            FAIL_RETURN(op->SetDelayUntil(delayUntil))
            FAIL_RETURN(op->UpdateEffectiveRunTime())
        }
    }

    return E_NOT_IMPLEMENTED;
}

ECode SyncQueue::RemoveEx(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authority)
{
    AutoPtr<ISyncOperation> syncOperation;
    AutoPtr<IAccount> act;
    Int32 uId = 0;
    String auth;
    String key;
    HashMap<String, AutoPtr<ISyncOperation> >::Iterator it = mOperationsMap->Begin();

    for (; it != mOperationsMap->End(); it++) {
        syncOperation = it->mSecond;
        act = NULL;
        FAIL_RETURN(syncOperation->GetAccount((IAccount**)&act))
        if (NULL != account && !_CObject_Compare(act, account)) {
            continue;
        }

        FAIL_RETURN(syncOperation->GetAuthority(&auth))
        if (!authority.IsNull() && authority.Equals(auth)) {
            continue;
        }

        FAIL_RETURN(syncOperation->GetUserId(&uId))
        if (userId != uId) {
            continue;
        }

        key = it->mFirst;
        mOperationsMap->Erase(key);
        AutoPtr<ISyncStorageEnginePendingOperation> pop;
        FAIL_RETURN(syncOperation->GetPendingOperation((ISyncStorageEnginePendingOperation**)&pop))
        Boolean ret = FALSE;
        FAIL_RETURN(mSyncStorageEngine->DeleteFromPending(pop, &ret))

        if (!ret) {
            String errorMessage("unable to find pending row for ");
            String str;
            FAIL_RETURN(syncOperation->ToString(&str))
            errorMessage += str;
            errorMessage += " IllegalStateException";
            //Log.e(TAG, errorMessage, new IllegalStateException(errorMessage));
            Logger::E(TAG, errorMessage);
        }
    }

    return NOERROR;
}

ECode SyncQueue::GetOperations(
    /* [out, callee] */ ArrayOf<ISyncOperation *>** operations)
{
    VALIDATE_NOT_NULL(operations)
    Int32 size = mOperationsMap->GetSize();
    AutoPtr<ArrayOf<ISyncOperation *> > tmpArray = ArrayOf<ISyncOperation *>::Alloc(size);
    HashMap<String, AutoPtr<ISyncOperation> >::Iterator it = mOperationsMap->Begin();
    Int32 index = 0;
    for (; it != mOperationsMap->End(); it++, index++) {
        tmpArray->Set(index, it->mSecond);
    }

    *operations = tmpArray;
    INTERFACE_ADDREF(*operations);
    return NOERROR;
}

ECode SyncQueue::Dump(
    /* [in] */ IStringBuilder* sb)
{
    Int64 now = SystemClock::GetElapsedRealtime();
    sb->AppendString(String("SyncQueue: "));
    sb->AppendInt32(mOperationsMap->GetSize());
    sb->AppendString(String(" operation(s)\n"));

    AutoPtr<ISyncOperation> operation;
    AutoPtr<IAccount> act;
    String auth;
    String key;
    HashMap<String, AutoPtr<ISyncOperation> >::Iterator it = mOperationsMap->Begin();

    for (; it != mOperationsMap->End(); it++) {
        sb->AppendString(String("  "));
        operation = it->mSecond;
        Int64 effectiveRunTime = 0;
        FAIL_RETURN(operation->GetEffectiveRunTime(&effectiveRunTime))
        if (effectiveRunTime <= now) {
            sb->AppendString(String("READY"));
        }
        else {
//***            sb->AppendString(DateUtils::FormatElapsedTime((effectiveRunTime - now) / 1000));
        }
        sb->AppendString(String(" - "));
        String str;
        FAIL_RETURN(operation->Dump(FALSE, &str))
        sb->AppendString(str);
        sb->AppendString(String("\n"));
    }

    return NOERROR;
}

ECode SyncQueue::Add(
    /* [in] */ ISyncOperation* operation,
    /* [in] */ ISyncStorageEnginePendingOperation* pop,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    // - if an operation with the same key exists and this one should run earlier,
    //   update the earliestRunTime of the existing to the new time
    // - if an operation with the same key exists and if this one should run
    //   later, ignore it
    // - if no operation exists then add the new one
    String operationKey;
    FAIL_RETURN(operation->GetKey(&operationKey))
    AutoPtr<ISyncOperation> existingOperation;
    HashMap<String, AutoPtr<ISyncOperation> >::Iterator it = mOperationsMap->Find(operationKey);
    existingOperation = it->mSecond;

    if (NULL != existingOperation) {
        Boolean changed = FALSE;
        Boolean expedited = FALSE;
        Boolean expedited2 = FALSE;
        FAIL_RETURN(existingOperation->GetExpedited(&expedited))
        FAIL_RETURN(operation->GetExpedited(&expedited2))

        if (expedited == expedited2) {
            Int64 earliestRunTime = 0;
            Int64 earliestRunTime2 = 0;
            FAIL_RETURN(existingOperation->GetEarliestRunTime(&earliestRunTime))
            FAIL_RETURN(operation->GetEarliestRunTime(&earliestRunTime2))
            Int64 newRunTime = Elastos::Core::Math::Min(earliestRunTime, earliestRunTime2);
            if (earliestRunTime != newRunTime) {
                FAIL_RETURN(existingOperation->SetEarliestRunTime(newRunTime))
                changed = TRUE;
            }
        }
        else {
            if (expedited2) {
                FAIL_RETURN(existingOperation->SetExpedited(TRUE))
                changed = TRUE;
            }
        }

        *result = changed;
        return NOERROR;
    }

    FAIL_RETURN(operation->SetPendingOperation(pop))
    AutoPtr<ISyncStorageEnginePendingOperation> pendingOperation;
    FAIL_RETURN(operation->GetPendingOperation((ISyncStorageEnginePendingOperation**)&pendingOperation))

    if (NULL == pendingOperation) {
        AutoPtr<IAccount> account;
        Int32 userId = 0;
        Int32 syncSource = 0;
        String authority;
        AutoPtr<IBundle> extras;
        Boolean expedited = FALSE;
        FAIL_RETURN(operation->GetAccount((IAccount**)&account))
        FAIL_RETURN(operation->GetUserId(&userId))
        FAIL_RETURN(operation->GetSyncSource(&syncSource))
        FAIL_RETURN(operation->GetAuthority(&authority))
        FAIL_RETURN(operation->GetExtras((IBundle**)&extras))
        FAIL_RETURN(operation->GetExpedited(&expedited))
        FAIL_RETURN(CSyncStorageEnginePendingOperation::New(account, userId, syncSource, authority, extras, expedited,
                (ISyncStorageEnginePendingOperation**)&pendingOperation))
        FAIL_RETURN(mSyncStorageEngine->InsertIntoPending(pendingOperation,
                (ISyncStorageEnginePendingOperation**)&pendingOperation))

        if (NULL == pendingOperation) {
            //throw new IllegalStateException("error adding pending sync operation "  + operation);
            return E_ILLEGAL_STATE_EXCEPTION;
        }

        FAIL_RETURN(operation->SetPendingOperation(pendingOperation))
    }

    (*mOperationsMap)[operationKey] = operation;
    *result = TRUE;
    return NOERROR;
}

}
}
}

