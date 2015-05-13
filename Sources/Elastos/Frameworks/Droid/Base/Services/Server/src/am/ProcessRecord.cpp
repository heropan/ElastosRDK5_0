
#include "am/ProcessRecord.h"
#include "am/ProcessList.h"
#include "os/UserHandle.h"
#include <elastos/StringUtils.h>

#include <elastos/Slogger.h>

using Elastos::Utility::Logging::Slogger;

using Elastos::Core::StringUtils;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

// {88C667C8-D324-445A-9210-1BDC6FD27F2C}
extern const InterfaceID EIID_ProcessRecord =
    { 0x88c667c8, 0xd324, 0x445a, { 0x92, 0x10, 0x1b, 0xdc, 0x6f, 0xd2, 0x7f, 0x2c } };

CAR_INTERFACE_IMPL_WITH_CPP_CAST(ProcessRecord, IInterface, ProcessRecord)

ProcessRecord::ProcessRecord(
    /* [in] */ BatteryStatsImpl::Uid::Proc* batteryStats,
    /* [in] */ IApplicationThread* appThread,
    /* [in] */ IApplicationInfo* info,
    /* [in] */ const String& processName,
    /* [in] */ Int32 uid)
    : mBatteryStats(batteryStats)
    , mInfo(info)
    , mIsolated(FALSE)
    , mUid(uid)
    , mUserId(0)
    , mProcessName(processName)
    , mThread(appThread)
    , mPid(0)
    , mStarting(FALSE)
    , mLastActivityTime(0)
    , mLruWeight(FALSE)
    , mMaxAdj(ProcessList::HIDDEN_APP_MAX_ADJ)
    , mHiddenAdj(ProcessList::HIDDEN_APP_MIN_ADJ)
    , mClientHiddenAdj(ProcessList::HIDDEN_APP_MIN_ADJ)
    , mEmptyAdj(ProcessList::HIDDEN_APP_MIN_ADJ)
    , mCurRawAdj(-100)
    , mSetRawAdj(-100)
    , mNonStoppingAdj(0)
    , mCurAdj(-100)
    , mSetAdj(-100)
    , mCurSchedGroup(0)
    , mTrimMemoryLevel(0)
    , mMemImportance(0)
    , mServiceb(FALSE)
    , mKeeping(FALSE)
    , mSetIsForeground(FALSE)
    , mHasActivities(FALSE)
    , mHasClientActivities(FALSE)
    , mForegroundServices(FALSE)
    , mForegroundActivities(FALSE)
    , mSystemNoUi(FALSE)
    , mHasShownUi(FALSE)
    , mPendingUiClean(FALSE)
    , mHasAboveClient(FALSE)
    , mBad(FALSE)
    , mKilledBackground(FALSE)
    , mAdjSeq(0)
    , mLruSeq(0)
    , mUsingWrapper(0)
    , mLastWakeTime(0)
    , mLastCpuTime(0)
    , mCurCpuTime(0)
    , mLastRequestedGc(0)
    , mLastLowMemory(0)
    , mReportLowMemory(0)
    , mEmpty(0)
    , mHidden(0)
    , mLastPss(0)
    , mAdjTypeCode(0)
    , mAdjSourceOom(0)
    , mPersistent(0)
    , mCrashing(0)
    , mNotResponding(0)
    , mRemoved(0)
    , mDebugging(0)
    , mWaitedForDebugger(0)
{
    assert(info != NULL && "IApplicationInfo can not be null when creating ProcessRecord.");

    Int32 appUid;
    info->GetUid(&appUid);
    String pkgName;
    info->GetPackageName(&pkgName);

    mIsolated = appUid != mUid;
    mUserId = UserHandle::GetUserId(uid);
    mPkgList.Insert(pkgName);
}

ProcessRecord::~ProcessRecord()
{
    mPkgList.Clear();
    mActivities.Clear();
    mServices.Clear();
    mExecutingServices.Clear();
    mConnections.Clear();
    mReceivers.Clear();
    mPubProviders.Clear();
    mConProviders.Clear();
    Slogger::D("ProcessRecord", " =========== ~ProcessRecord(): %s", ToString().string());
}

ECode ProcessRecord::SetPid(
    /* [in] */ Int32 pid)
{
    mPid = pid;
    mShortStringName = NULL;
    mStringName = NULL;
    return NOERROR;
}

Boolean ProcessRecord::IsInterestingToUserLocked()
{
    Int32 size = mActivities.GetSize();
    for (Int32 i = 0 ; i < size ; i++) {
        AutoPtr<ActivityRecord> r = mActivities[i];
        if (r->IsInterestingToUserLocked()) {
            return TRUE;
        }
    }

    return FALSE;
}

ECode ProcessRecord::StopFreezingAllLocked()
{
    Int32 i = mActivities.GetSize();
    while (i > 0) {
        --i;
        AutoPtr<ActivityRecord> r = mActivities[i];
        r->StopFreezingScreenLocked(TRUE);
    }
    return NOERROR;
}

ECode ProcessRecord::UnlinkDeathRecipient()
{
    if (mDeathRecipient != NULL && mThread != NULL) {
        AutoPtr<IProxy> proxy = (IProxy*)mThread->Probe(EIID_IProxy);
        assert(proxy != NULL);
        Boolean res;
        proxy->UnlinkToDeath(mDeathRecipient, 0, &res);
    }
    mDeathRecipient = NULL;
    return NOERROR;
}

ECode ProcessRecord::UpdateHasAboveClientLocked()
{
    mHasAboveClient = FALSE;
    if (mConnections.IsEmpty() == FALSE) {
        HashSet< AutoPtr<ConnectionRecord> >::Iterator it = mConnections.Begin();
        for (; it != mConnections.End(); ++it) {
            AutoPtr<ConnectionRecord> cr = *it;

            if ((cr->mFlags&IContext::BIND_ABOVE_CLIENT) != 0) {
                mHasAboveClient = TRUE;
                break;
            }
        }
    }
    return NOERROR;
}

String ProcessRecord::ToShortString()
{
    if (!mShortStringName.IsNull()) {
        return mShortStringName;
    }
    StringBuilder sb;
    ToShortString(sb);
    mShortStringName = sb.ToString();
    return mShortStringName;
}

ECode ProcessRecord::ToShortString(
    /* [in] */ StringBuilder& sb)
{
    sb += StringUtils::Int32ToHexString((Int32)this);
    Int32 uid;
    mInfo->GetUid(&uid);
    sb += " ";
    sb += mPid;
    sb += ":";
    sb += mProcessName;
    sb += "/";
    sb += uid;
    return NOERROR;
}

String ProcessRecord::ToString()
{
    if (!mStringName.IsNull()) {
        return mStringName;
    }
    StringBuilder sb(128);
    sb += ("ProcessRecord{");
    sb += StringUtils::Int32ToHexString((Int32)this);
    sb += (' ');
    ToShortString(sb);
    sb += ('}');
    return mStringName = sb.ToString();
}

/*
 *  Return true if package has been added false if not
 */
Boolean ProcessRecord::AddPackage(
    /* [in] */ const String& pkg)
{
    HashSet<String>::Iterator it = mPkgList.Find(pkg);
    if (it == mPkgList.End()) return FALSE;
    mPkgList.Insert(pkg);
    return TRUE;
}

ECode ProcessRecord::ResetPackageList()
{
    String pkgName;
    mInfo->GetPackageName(&pkgName);

    mPkgList.Clear();
    mPkgList.Insert(pkgName);
    return NOERROR;
}

AutoPtr<ArrayOf<String> > ProcessRecord::GetPackageList()
{
    AutoPtr< ArrayOf<String> > list;
    Int32 size = mPkgList.GetSize();
    if (size == 0) {
        return list;
    }

    list = ArrayOf<String>::Alloc(size);
    HashSet<String>::Iterator it = mPkgList.Begin();
    for (Int32 i = 0; it != mPkgList.End(); ++it, ++i) {
        (*list)[i] = *it;
    }
    return list;
}


} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
