#include "elastos/droid/server/content/SyncOperation.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Content {

const String SyncOperation::TAG("SyncManager");

const Int32 SyncOperation::REASON_BACKGROUND_DATA_SETTINGS_CHANGED = -1;
const Int32 SyncOperation::REASON_ACCOUNTS_UPDATED = -2;
const Int32 SyncOperation::REASON_SERVICE_CHANGED = -3;
const Int32 SyncOperation::REASON_PERIODIC = -4;
/** Sync started because it has just been set to isSyncable. */
const Int32 SyncOperation::REASON_IS_SYNCABLE = -5;
/** Sync started because it has just been set to sync automatically. */
const Int32 SyncOperation::REASON_SYNC_AUTO = -6;
/** Sync started because master sync automatically has been set to true. */
const Int32 SyncOperation::REASON_MASTER_SYNC_AUTO = -7;
const Int32 SyncOperation::REASON_USER_START = -8;

const Int32 SyncOperation::SYNC_TARGET_UNKNOWN = 0;
const Int32 SyncOperation::SYNC_TARGET_ADAPTER = 1;
const Int32 SyncOperation::SYNC_TARGET_SERVICE = 2;

AutoPtr<ArrayOf<String> > InitREASON_NAMES()
{
    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(8);
    array->Set(0, String("DataSettingsChanged"));
    array->Set(0, String("AccountsUpdated"));
    array->Set(0, String("ServiceChanged"));
    array->Set(0, String("Periodic"));
    array->Set(0, String("IsSyncable"));
    array->Set(0, String("AutoSync"));
    array->Set(0, String("MasterSyncAuto"));
    array->Set(0, String("UserStart"));
    return array;
}

AutoPtr<ArrayOf<String> > SyncOperation::REASON_NAMES = InitREASON_NAMES();

CAR_INTERFACE_IMPL(SyncOperation, Object, IComparable)

SyncOperation::SyncOperation(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ Int32 reason,
    /* [in] */ Int32 source,
    /* [in] */ String provider,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 runTimeFromNow,
    /* [in] */ Int64 flexTime,
    /* [in] */ Int64 backoff,
    /* [in] */ Int64 delayUntil,
    /* [in] */ Boolean allowParallelSyncs)
    : mReason(reason)
    , mSyncSource(source)
    , mAllowParallelSyncs(FALSE)
    , mExpedited(FALSE)
    , mLatestRunTime(0)
    , mBackoff(0)
    , mDelayUntil(0)
    , mEffectiveRunTime(0)
    , mFlexTime(0)
{
    Init(new SyncStorageEngine.EndPoint(account, provider, userId),
            reason, source, extras, runTimeFromNow, flexTime, backoff, delayUntil,
            allowParallelSyncs);
}

SyncOperation::SyncOperation(
    /* [in] */ IComponentName* service,
    /* [in] */ Int32 userId,
    /* [in] */ Int32 reason,
    /* [in] */ Int32 source,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 runTimeFromNow,
    /* [in] */ Int64 flexTime,
    /* [in] */ Int64 backoff,
    /* [in] */ Int64 delayUntil)
{
    Init(new SyncStorageEngine.EndPoint(service, userId), reason, source, extras,
            runTimeFromNow, flexTime, backoff, delayUntil, true /* allowParallelSyncs */);
}

ECode SyncOperation::Init(
    /* [in] */ SyncStorageEngineEndPoint* info,
    /* [in] */ Int32 reason,
    /* [in] */ Int32 source,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 runTimeFromNow,
    /* [in] */ Int64 flexTime,
    /* [in] */ Int64 backoff,
    /* [in] */ Int64 delayUntil,
    /* [in] */ Boolean allowParallelSyncs)
{
    mtarget = info;
    mreason = reason;
    msyncSource = source;
    mextras = new Bundle(extras);
    cleanBundle(mextras);
    mdelayUntil = delayUntil;
    mbackoff = backoff;
    mallowParallelSyncs = allowParallelSyncs;
    final Int64 now = SystemClock.elapsedRealtime();
    // Set expedited based on runTimeFromNow. The SyncManager specifies whether the op is
    // expedited (Not done solely based on bundle).
    if (runTimeFromNow < 0) {
        mexpedited = true;
        // Sanity check: Will always be true.
        if (!mextras.getBoolean(ContentResolver.SYNC_EXTRAS_EXPEDITED, false)) {
            mextras.putBoolean(ContentResolver.SYNC_EXTRAS_EXPEDITED, true);
        }
        mlatestRunTime = now;
        mflexTime = 0;
    } else {
        mexpedited = false;
        mextras.remove(ContentResolver.SYNC_EXTRAS_EXPEDITED);
        mlatestRunTime = now + runTimeFromNow;
        mflexTime = flexTime;
    }
    UpdateEffectiveRunTime();
    mkey = toKey(info, mextras);
}

SyncOperation::SyncOperation(
    /* [in] */ SyncOperation* other,
    /* [in] */ Int64 newRunTimeFromNow)
{
    this(other.target, other.reason, other.syncSource, new Bundle(other.extras),
            newRunTimeFromNow,
            0L /* In back-off so no flex */,
            other.backoff,
            other.delayUntil,
            other.allowParallelSyncs);
}

Boolean SyncOperation::MatchesAuthority(
    /* [in] */ SyncOperation* other)
{
    return mtarget.matchesSpec(other.target);
}

void SyncOperation::CleanBundle(
    /* [in] */ IBundle* bundle)
{
    RemoveFalseExtra(bundle, ContentResolver.SYNC_EXTRAS_UPLOAD);
    RemoveFalseExtra(bundle, ContentResolver.SYNC_EXTRAS_MANUAL);
    RemoveFalseExtra(bundle, ContentResolver.SYNC_EXTRAS_IGNORE_SETTINGS);
    RemoveFalseExtra(bundle, ContentResolver.SYNC_EXTRAS_IGNORE_BACKOFF);
    RemoveFalseExtra(bundle, ContentResolver.SYNC_EXTRAS_DO_NOT_RETRY);
    RemoveFalseExtra(bundle, ContentResolver.SYNC_EXTRAS_DISCARD_LOCAL_DELETIONS);
    RemoveFalseExtra(bundle, ContentResolver.SYNC_EXTRAS_EXPEDITED);
    RemoveFalseExtra(bundle, ContentResolver.SYNC_EXTRAS_OVERRIDE_TOO_MANY_DELETIONS);
    RemoveFalseExtra(bundle, ContentResolver.SYNC_EXTRAS_DISALLOW_METERED);
}

void SyncOperation::RemoveFalseExtra(
    /* [in] */ IBundle* bundle,
    /* [in] */ const String& extraName)
{
    if (!bundle.getBoolean(extraName, false)) {
        bundle.remove(extraName);
    }
}

Boolean SyncOperation::IsConflict(
    /* [in] */ SyncOperation* toRun)
{
    final SyncStorageEngine.EndPoint other = toRun.target;
    if (target.target_provider) {
        return target.account.type.equals(other.account.type)
                && target.provider.equals(other.provider)
                && target.userId == other.userId
                && (!allowParallelSyncs
                        || target.account.name.equals(other.account.name));
    } else {
        // Ops that target a service default to allow parallel syncs, which is handled by the
        // service returning SYNC_IN_PROGRESS if they don't.
        return target.service.equals(other.service) && !allowParallelSyncs;
    }
}

// @Override
ECode SyncOperation::ToString(
    /* [out] */ String* str)
{
    return dump(null, true);
}

String SyncOperation::Dump(
    /* [in ]*/ IPackageManager* pm,
    /* [in] */ Boolean useOneLine)
{
    StringBuilder sb = new StringBuilder();
    if (target.target_provider) {
        sb.Append(target.account.name)
            .append(" u")
            .append(target.userId).append(" (")
            .append(target.account.type)
            .append(")")
            .append(", ")
            .append(target.provider)
            .append(", ");
    } else if (target.target_service) {
        sb.Append(target.service.getPackageName())
            .append(" u")
            .append(target.userId).append(" (")
            .append(target.service.getClassName()).append(")")
            .append(", ");
    }
    sb.Append(SyncStorageEngine.SOURCES[syncSource])
        .append(", currentRunTime ")
        .append(effectiveRunTime);
    if (expedited) {
        sb.Append(", EXPEDITED");
    }
    sb.Append(", reason: ");
    sb.Append(reasonToString(pm, reason));
    if (!useOneLine && !extras.keySet().isEmpty()) {
        sb.Append("\n    ");
        ExtrasToStringBuilder(extras, sb);
    }
    return sb.toString();
}

String SyncOperation::ReasonToString(
    /* [in] */ IPackageManager* pm,
    /* [in] */ Int32 reason)
{
    if (reason >= 0) {
        if (pm != null) {
            final String[] packages = pm.getPackagesForUid(reason);
            if (packages != null && packages.length == 1) {
                return packages[0];
            }
            final String name = pm.getNameForUid(reason);
            if (name != null) {
                return name;
            }
            return String.valueOf(reason);
        } else {
            return String.valueOf(reason);
        }
    } else {
        final Int32 index = -reason - 1;
        if (index >= REASON_NAMES.length) {
            return String.valueOf(reason);
        } else {
            return REASON_NAMES[index];
        }
    }
}

Boolean SyncOperation::IsInitialization() {
    return extras.getBoolean(ContentResolver.SYNC_EXTRAS_INITIALIZE, false);
}

Boolean SyncOperation::IsExpedited() {
    return expedited;
}

Boolean SyncOperation::IgnoreBackoff() {
    return extras.getBoolean(ContentResolver.SYNC_EXTRAS_IGNORE_BACKOFF, false);
}

Boolean SyncOperation::IsNotAllowedOnMetered() {
    return extras.getBoolean(ContentResolver.SYNC_EXTRAS_DISALLOW_METERED, false);
}

Boolean SyncOperation::IsManual() {
    return extras.getBoolean(ContentResolver.SYNC_EXTRAS_MANUAL, false);
}

Boolean SyncOperation::IsIgnoreSettings() {
    return extras.getBoolean(ContentResolver.SYNC_EXTRAS_IGNORE_SETTINGS, false);
}

/** Changed in V3. */
static String SyncOperation::ToKey(
    /* [in] */ SyncStorageEngineEndPoint* info,
    /* [in] */ IBundle* extras)
{
    StringBuilder sb = new StringBuilder();
    if (info.target_provider) {
        sb.Append("provider: ");
        sb.Append(info.provider);
        sb.Append(" account {name=" + info.account.name
                + ", user="
                + info.userId
                + ", type="
                + info.account.type
                + "}");
    } else if (info.target_service) {
        sb.Append("service {package=" );
        sb.Append(info.service.getPackageName());
        sb.Append(" user=");
        sb.Append(info.userId);
        sb.Append(", class=");
        sb.Append(info.service.getClassName());
        sb.Append("}");
    } else {
        Log.v(TAG, "Converting SyncOperaton to key, invalid target: " + info.toString());
        return "";
    }
    sb.Append(" extras: ");
    ExtrasToStringBuilder(extras, sb);
    return sb.toString();
}

void SyncOperation::ExtrasToStringBuilder(
    /* [in] */ IBundle* bundle,
    /* [in] */ StringBuilder& sb)
{
    sb.Append("[");
    for (String key : bundle.keySet()) {
        sb.Append(key).append("=").append(bundle.get(key)).append(" ");
    }
    sb.Append("]");
}

String SyncOperation::WakeLockName()
{
    if (wakeLockName != null) {
        return wakeLockName;
    }
    if (target.target_provider) {
        return (wakeLockName = target.provider
                + "/" + target.account.type
                + "/" + target.account.name);
    } else if (target.target_service) {
        return (wakeLockName = target.service.getPackageName()
                + "/" + target.service.getClassName());
    } else {
        Log.wtf(TAG, "Invalid target getting wakelock name for operation - " + key);
        return null;
    }
}

void SyncOperation::UpdateEffectiveRunTime()
{
    // Regardless of whether we're in backoff or honouring a delayUntil, we still incorporate
    // the flex time provided by the developer.
    effectiveRunTime = ignoreBackoff() ?
            latestRunTime :
                Math.max(Math.max(latestRunTime, delayUntil), backoff);
}

ECode SyncOperation::CompareTo(
    /* [in] */ IInterface* o,
    /* [out] */ Int32* result)
{
    SyncOperation other = (SyncOperation) o;
    if (expedited != other.expedited) {
        return expedited ? -1 : 1;
    }
    Int64 thisIntervalStart = Math.max(effectiveRunTime - flexTime, 0);
    Int64 otherIntervalStart = Math.max(
        other.effectiveRunTime - other.flexTime, 0);
    if (thisIntervalStart < otherIntervalStart) {
        return -1;
    } else if (otherIntervalStart < thisIntervalStart) {
        return 1;
    } else {
        return 0;
    }
}

// TODO: Test this to make sure that casting to object doesn't lose the type info for EventLog.
AutoPtr<ArrayOf<IInterface*> > SyncOperation::ToEventLog(
    /* [in] */ Int32 event)
{
    Object[] logArray = new Object[4];
    logArray[1] = event;
    logArray[2] = syncSource;
    if (target.target_provider) {
        logArray[0] = target.provider;
        logArray[3] = target.account.name.hashCode();
    } else if (target.target_service) {
        logArray[0] = target.service.getPackageName();
        logArray[3] = target.service.hashCode();
    } else {
        Log.wtf(TAG, "sync op with invalid target: " + key);
    }
    return logArray;
}

} // namespace Content
} // namespace Server
} // namespace Droid
} // namespace Elastos
