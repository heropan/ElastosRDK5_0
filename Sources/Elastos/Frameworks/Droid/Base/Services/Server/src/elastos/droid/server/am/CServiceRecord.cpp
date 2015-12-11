
#include "CNotificationManagerService.h"
#include "elastos/droid/server/am/CServiceRecord.h"
#include <elastos/core/StringUtils.h>
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/os/UserHandle.h"
#include "elastos/droid/server/am/AppBindRecord.h"
#include "elastos/droid/server/am/CActivityManagerService.h"
#include "elastos/droid/server/am/IntentBindRecord.h"
#include "elastos/droid/server/am/ProcessRecord.h"
#include "elastos/droid/server/am/UriPermissionOwner.h"
#include "elastos/droid/app/NotificationManager.h"

using Elastos::Core::StringUtils;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::App::NotificationManager;
using Elastos::Droid::Content::CIntentFilterComparison;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

const Int32 CServiceRecord::MAX_DELIVERY_COUNT;
const Int32 CServiceRecord::MAX_DONE_EXECUTING_COUNT;

//==============================================================================
// CServiceRecord::StartItem
//==============================================================================

CServiceRecord::StartItem::StartItem(
    /* [in] */ CServiceRecord* sr,
    /* [in] */ Boolean taskRemoved,
    /* [in] */ Int32 id,
    /* [in] */ IIntent* intent,
    /* [in] */ NeededUriGrants* neededGrants)
    : mSr(sr)
    , mTaskRemoved(taskRemoved)
    , mId(id)
    , mIntent(intent)
    , mNeededGrants(neededGrants)
    , mDeliveredTime(0)
    , mDeliveryCount(0)
    , mDoneExecutingCount(0)
{
}

AutoPtr<UriPermissionOwner> CServiceRecord::StartItem::GetUriPermissionsLocked()
{
    if (mUriPermissions == NULL) {
        mUriPermissions = new UriPermissionOwner(mSr->mAms, (Handle32)this);
    }
    return mUriPermissions;
}

ECode CServiceRecord::StartItem::RemoveUriPermissionsLocked()
{
    if (mUriPermissions != NULL) {
        mUriPermissions->RemoveUriPermissionsLocked();
        mUriPermissions = NULL;
    }
    return NOERROR;
}

//==============================================================================
// CServiceRecord::PostNotificationRunnable
//==============================================================================

ECode CServiceRecord::PostNotificationRunnable::Run()
{
    AutoPtr<CNotificationManagerService> nm = (CNotificationManagerService*)(IINotificationManager*)NotificationManager::GetService();
    if (nm == NULL) {
        return NOERROR;
    }
    // try {
    AutoPtr<ArrayOf<Int32> > outId;
    ECode ec = nm->EnqueueNotificationInternal(mLocalPackageName, mAppUid, mAppPid,
           String(NULL), mLocalForegroundId, mLocalForegroundNoti, (ArrayOf<Int32>**)&outId, mHost->mUserId);
    if (FAILED(ec)) {
    // } catch (RuntimeException e) {
        Slogger::W(CActivityManagerService::TAG,
                "Error showing notification for service");
        // If it gave us a garbage notification, it doesn't
        // get to be foreground.
        mHost->mAms->SetServiceForeground(mHost->mName, mHost,
                0, NULL, TRUE);
        StringBuilder errorsb("Bad notification for startForeground: ");
        errorsb += (Int32)ec;
        mHost->mAms->CrashApplication(mAppUid, mAppPid, mLocalPackageName,
                errorsb.ToString());
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL(CServiceRecord::PostNotificationRunnable, IRunnable)

//==============================================================================
// CServiceRecord::CancelNotificationRunnable
//==============================================================================

ECode CServiceRecord::CancelNotificationRunnable::Run()
{
    AutoPtr<IINotificationManager> inm = NotificationManager::GetService();
    if (inm == NULL) {
        return NOERROR;
    }
    // try {
    ECode ec = inm->CancelNotificationWithTag(mLocalPackageName, String(NULL),
                   mLocalForegroundId, mHost->mUserId);
    if (FAILED(ec)) {
    // } catch (RuntimeException e) {
    Slogger::W(CActivityManagerService::TAG,
               "Error canceling notification for service");
    // } catch (RemoteException e) {
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL(CServiceRecord::CancelNotificationRunnable, IRunnable)

//==============================================================================
// CServiceRecord
//==============================================================================
CServiceRecord::CServiceRecord()
    : mAms(NULL)
    , mUserId(0)
    , mExported(FALSE)
    , mCreateTime(0)
    , mApp(0)
    , mIsolatedProc(0)
    , mIsForeground(FALSE)
    , mForegroundId(0)
    , mLastActivity(0)
    , mStartRequested(FALSE)
    , mStopIfKilled(FALSE)
    , mCallStart(FALSE)
    , mExecuteNesting(0)
    , mExecutingStart(0)
    , mCrashCount(0)
    , mTotalRestartCount(0)
    , mRestartCount(0)
    , mRestartDelay(0)
    , mRestartTime(0)
    , mNextRestartTime(0)
    , mLastStartId(0)
{
}

CServiceRecord::~CServiceRecord()
{
    mDeliveredStarts.Clear();
    mPendingStarts.Clear();
}

ECode CServiceRecord::Init(
    /* [in] */ CActivityManagerService* ams,
    /* [in] */ BatteryStatsImpl::Uid::Pkg::Serv* servStats,
    /* [in] */ IComponentName* name,
    /* [in] */ IIntentFilterComparison* intent,
    /* [in] */ IServiceInfo* sInfo,
    /* [in] */ IRunnable* restarter)
{
    VALIDATE_NOT_NULL(name);
    VALIDATE_NOT_NULL(sInfo);

    mAms = ams;
    mStats = servStats;
    mName = name;
    name->FlattenToShortString(&mShortName);
    mIntent = intent;
    mServiceInfo = sInfo;
    sInfo->GetApplicationInfo((IApplicationInfo**)&mAppInfo);
    mAppInfo->GetPackageName(&mPackageName);
    sInfo->GetProcessName(&mProcessName);
    sInfo->GetPermission(&mPermission);
    Int32 uid;
    mAppInfo->GetUid(&uid);
    mAppInfo->GetSourceDir(&mBaseDir);
    mAppInfo->GetPublicSourceDir(&mResDir);
    mAppInfo->GetDataDir(&mDataDir);
    sInfo->GetExported(&mExported);
    mRestarter = restarter;
    mCreateTime = SystemClock::GetElapsedRealtime();
    mLastActivity = SystemClock::GetUptimeMillis();
    mUserId = UserHandle::GetUserId(uid);
    return NOERROR;
}

AutoPtr<AppBindRecord> CServiceRecord::RetrieveAppBindingLocked(
    /* [in] */ IIntent* intent,
    /* [in] */ ProcessRecord* app)
{
    AutoPtr<IIntentFilterComparison> filter;
    CIntentFilterComparison::New(intent, (IIntentFilterComparison**)&filter);
    AutoPtr<IntentBindRecord> i = mBindings[filter];
    if (i == NULL) {
        i = new IntentBindRecord(this, filter);
        mBindings[filter] = i;
    }
    AutoPtr<AppBindRecord> a = i->mApps[app];
    if (a != NULL) {
        return a;
    }
    a = new AppBindRecord(this, i, app);
    i->mApps[app] = a;
    return a;
}

ECode CServiceRecord::ResetRestartCounter()
{
    mRestartCount = 0;
    mRestartDelay = 0;
    mRestartTime = 0;
    return NOERROR;
}

AutoPtr<CServiceRecord::StartItem> CServiceRecord::FindDeliveredStart(
    /* [in] */ Int32 id,
    /* [in] */ Boolean remove)
{
    List<AutoPtr<StartItem> >::Iterator it = mDeliveredStarts.Begin();
    for (; it != mDeliveredStarts.End(); ++it) {
        AutoPtr<StartItem> si = *it;
        if (si->mId == id) {
            if (remove) mDeliveredStarts.Erase(it);
            return si;
        }
    }

    AutoPtr<StartItem> item;
    return item;
}

Int32 CServiceRecord::GetLastStartId()
{
    return mLastStartId;
}

Int32 CServiceRecord::MakeNextStartId()
{
    mLastStartId++;
    if (mLastStartId < 1) {
        mLastStartId = 1;
    }
    return mLastStartId;
}

ECode CServiceRecord::PostNotification()
{
    Int32 appUid;
    mAppInfo->GetUid(&appUid);
    const Int32 appPid = mApp->mPid;
    if (mForegroundId != 0 && mForegroundNoti != NULL) {
        // Do asynchronous communication with notification manager to
        // avoid deadlocks.
        const String localPackageName = mPackageName;
        const Int32 localForegroundId = mForegroundId;
        AutoPtr<INotification> localForegroundNoti = mForegroundNoti;
        AutoPtr<IRunnable> runnable = new PostNotificationRunnable(
            this, localPackageName, appUid, appPid, localForegroundId, localForegroundNoti);
        Boolean posted;
        mAms->mHandler->Post(runnable, &posted);
    }
    return NOERROR;
}

ECode CServiceRecord::CancelNotification()
{
    if (mForegroundId != 0) {
        // Do asynchronous communication with notification manager to
        // avoid deadlocks.
        const String localPackageName = mPackageName;
        const Int32 localForegroundId = mForegroundId;
        AutoPtr<IRunnable> runnable = new CancelNotificationRunnable(this, localPackageName, localForegroundId);
        Boolean posted;
        mAms->mHandler->Post(runnable, &posted);
    }
    return NOERROR;
}

ECode CServiceRecord::ClearDeliveredStartsLocked()
{
    List<AutoPtr<StartItem> >::Iterator it = mDeliveredStarts.Begin();
    for (; it != mDeliveredStarts.End(); ++it) {
        AutoPtr<StartItem> si = *it;
        si->RemoveUriPermissionsLocked();
    }

    mDeliveredStarts.Clear();
    return NOERROR;
}

ECode CServiceRecord::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    if (!mStringName.IsNull()) {
        *str = mStringName;
        return NOERROR;
    }

    StringBuilder sb(128);
    sb += ("ServiceRecord{");
    sb += (StringUtils::Int32ToHexString(Int32(this)));
    sb += (" u");
    sb += (mUserId);
    sb += (' ');
    sb += (mShortName);
    sb += ('}');
    *str = mStringName = sb.ToString();

    return NOERROR;
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
