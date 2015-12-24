
#include <elastos/droid/server/SyncQueue.h>

namespace Elastos {
namespace Droid {
namespace Server {
namespace Content {

const String SyncQueue::TAG("SyncManager");

SyncQueue::SyncQueue(
    /* [in] */ IPackageManager* packageManager,
    /* [in] */ SyncStorageEngine* syncStorageEngine,
    /* [in] */ SyncAdaptersCache* syncAdapters)
{
    mPackageManager = packageManager;
    mSyncStorageEngine = syncStorageEngine;
    mSyncAdapters = syncAdapters;
}

void SyncQueue::AddPendingOperations(
    /* [in] */ Int32 userId)
{
    for (SyncStorageEngine.PendingOperation op : mSyncStorageEngine.getPendingOperations()) {
        final SyncStorageEngine.EndPoint info = op.target;
        if (info.userId != userId) continue;

        final Pair<Long, Long> backoff = mSyncStorageEngine.getBackoff(info);
        SyncOperation operationToAdd;
        if (info.target_provider) {
            final ServiceInfo<SyncAdapterType> syncAdapterInfo = mSyncAdapters.getServiceInfo(
                    SyncAdapterType.newKey(info.provider, info.account.type), info.userId);
            if (syncAdapterInfo == null) {
                if (Log.isLoggable(TAG, Log.VERBOSE)) {
                    Log.v(TAG, "Missing sync adapter info for authority " + op.target);
                }
                continue;
            }
            operationToAdd = new SyncOperation(
                    info.account, info.userId, op.reason, op.syncSource, info.provider,
                    op.extras,
                    op.expedited ? -1 : 0 /* delay */,
                    0 /* flex */,
                    backoff != null ? backoff.first : 0L,
                    mSyncStorageEngine.getDelayUntilTime(info),
                    syncAdapterInfo.type.allowParallelSyncs());
            operationToAdd.pendingOperation = op;
            add(operationToAdd, op);
        } else if (info.target_service) {
            try {
                mPackageManager.getServiceInfo(info.service, 0);
            } catch (PackageManager.NameNotFoundException e) {
                if (Log.isLoggable(TAG, Log.VERBOSE)) {
                    Log.w(TAG, "Missing sync service for authority " + op.target);
                }
                continue;
            }
            operationToAdd = new SyncOperation(
                    info.service, info.userId, op.reason, op.syncSource,
                    op.extras,
                    op.expedited ? -1 : 0 /* delay */,
                    0 /* flex */,
                    backoff != null ? backoff.first : 0,
                    mSyncStorageEngine.getDelayUntilTime(info));
            operationToAdd.pendingOperation = op;
            add(operationToAdd, op);
        }
    }
}

Boolean SyncQueue::Add(
    /* [in] */ SyncOperation* operation)
{
    return add(operation, null /* this is not coming from the database */);
}

Boolean SyncQueue::Add(
    /* [in] */ SyncOperation* operation,
    /* [in] */ PendingOperation* pop)
{
    // If an operation with the same key exists and this one should run sooner/overlaps,
    // replace the run Int32erval of the existing operation with this new one.
    // Complications: what if the existing operation is expedited but the new operation has an
    // earlier run time? Will not be a problem for periodic syncs (no expedited flag), and for
    // one-off syncs we only change it if the new sync is sooner.
    final String operationKey = operation.key;
    final SyncOperation existingOperation = mOperationsMap.get(operationKey);

    if (existingOperation != null) {
        Boolean changed = false;
        if (operation.compareTo(existingOperation) <= 0 ) {
            Int64 newRunTime =
                    Math.min(existingOperation.latestRunTime, operation.latestRunTime);
            // Take smaller runtime.
            existingOperation.latestRunTime = newRunTime;
            // Take newer flextime.
            existingOperation.flexTime = operation.flexTime;
            changed = true;
        }
        return changed;
    }

    operation.pendingOperation = pop;
    // Don't update the PendingOp if one already exists. This really is just a placeholder,
    // no actual scheduling info is placed here.
    if (operation.pendingOperation == null) {
        pop = mSyncStorageEngine.insertIntoPending(operation);
        if (pop == null) {
            throw new IllegalStateException("error adding pending sync operation "
                    + operation);
        }
        operation.pendingOperation = pop;
    }

    mOperationsMap.put(operationKey, operation);
    return true;
}

void SyncQueue::RemoveUserLocked(
    /* [in] */ Int32 userId)
{
    ArrayList<SyncOperation> opsToRemove = new ArrayList<SyncOperation>();
    for (SyncOperation op : mOperationsMap.values()) {
        if (op.target.userId == userId) {
            opsToRemove.add(op);
        }
    }
        for (SyncOperation op : opsToRemove) {
            remove(op);
        }
}

void SyncQueue::Remove(
    /* [in] */ SyncOperation* operation)
{
    Boolean isLoggable = Log.isLoggable(TAG, Log.VERBOSE);
    SyncOperation operationToRemove = mOperationsMap.remove(operation.key);
    if (isLoggable) {
        Log.v(TAG, "Attempting to remove: " + operation.key);
    }
    if (operationToRemove == null) {
        if (isLoggable) {
            Log.v(TAG, "Could not find: " + operation.key);
        }
        return;
    }
    if (!mSyncStorageEngine.deleteFromPending(operationToRemove.pendingOperation)) {
        final String errorMessage = "unable to find pending row for " + operationToRemove;
        Log.e(TAG, errorMessage, new IllegalStateException(errorMessage));
    }
}

void SyncQueue::ClearBackoffs()
{
    for (SyncOperation op : mOperationsMap.values()) {
        op.backoff = 0L;
        op.updateEffectiveRunTime();
    }
}

void SyncQueue::OnBackoffChanged(
    /* [in] */ EndPoint* target,
    /* [in] */ Int64 backoff)
{
    // For each op that matches the target of the changed op, update its
    // backoff and effectiveStartTime
    for (SyncOperation op : mOperationsMap.values()) {
        if (op.target.matchesSpec(target)) {
            op.backoff = backoff;
            op.updateEffectiveRunTime();
        }
    }
}

void SyncQueue::OnDelayUntilTimeChanged(
    /* [in] */ EndPoint* target,
    /* [in] */ Int64 delayUntil)
{
    // for each op that matches the target info of the provided op, change the delay time.
    for (SyncOperation op : mOperationsMap.values()) {
        if (op.target.matchesSpec(target)) {
            op.delayUntil = delayUntil;
            op.updateEffectiveRunTime();
        }
    }
}

void SyncQueue::Remove(
    /* [in] */ EndPoint* info,
    /* [in] */ IBundle* extras)
{
    Iterator<Map.Entry<String, SyncOperation>> entries = mOperationsMap.entrySet().iterator();
    while (entries.hasNext()) {
        Map.Entry<String, SyncOperation> entry = entries.next();
        SyncOperation syncOperation = entry.getValue();
        final SyncStorageEngine.EndPoint opInfo = syncOperation.target;
        if (!opInfo.matchesSpec(info)) {
            continue;
        }
        if (extras != null
                && !SyncManager.syncExtrasEquals(
                    syncOperation.extras,
                    extras,
                    false /* no config flags*/)) {
            continue;
        }
        entries.remove();
        if (!mSyncStorageEngine.deleteFromPending(syncOperation.pendingOperation)) {
            final String errorMessage = "unable to find pending row for " + syncOperation;
            Log.e(TAG, errorMessage, new IllegalStateException(errorMessage));
        }
    }
}

AutoPtr<List<AutoPtr<SyncOperation> > SyncQueue::GetOperations()
{
    return mOperationsMap.values();
}

void SyncQueue::Dump(
    /* [in] */ StringBuilder* sb)
{
    final Int64 now = SystemClock.elapsedRealtime();
    sb.append("SyncQueue: ").append(mOperationsMap.size()).append(" operation(s)\n");
    for (SyncOperation operation : mOperationsMap.values()) {
        sb.append("  ");
        if (operation.effectiveRunTime <= now) {
            sb.append("READY");
        } else {
            sb.append(DateUtils.formatElapsedTime((operation.effectiveRunTime - now) / 1000));
        }
        sb.append(" - ");
        sb.append(operation.dump(mPackageManager, false)).append("\n");
    }
}


} // namespace Content
} // namespace Server
} // namespace Droid
} // namespace Elastos
