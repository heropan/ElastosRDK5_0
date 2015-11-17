
#include "elastos/droid/app/AppImportanceMonitor.h"
#include "elastos/droid/app/CActivityManagerRunningAppProcessInfo.h"
#include "elastos/droid/app/ActivityManagerNative.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IMessage;

namespace Elastos {
namespace Droid {
namespace App {

//=================================================================================
// AppImportanceMonitor::AppEntry
//=================================================================================

AppImportanceMonitor::AppEntry::AppEntry(
    /* [in] */ Int32 uid)
    : mUid(uid)
    , mImportance(IActivityManagerRunningAppProcessInfo.IMPORTANCE_GONE)
{
}

//=================================================================================
// AppImportanceMonitor::ProcessObserver
//=================================================================================
CAR_INTERFACE_IMPL_2(AppImportanceMonitor::ProcessObserver, Object, IProcessObserver, IBinder)

AppImportanceMonitor::ProcessObserver::ProcessObserver(
    /* [in] */ AppImportanceMonitor* host)
    : mHost(host)
{}

AppImportanceMonitor::ProcessObserver::~ProcessObserver()
{}

ECode AppImportanceMonitor::ProcessObserver::OnForegroundActivitiesChanged(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean foregroundActivities)
{
    return NOERROR;
}

ECode AppImportanceMonitor::ProcessObserver::OnProcessStateChanged(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 procState)
{
    Object& lock = mHost->mAppsLock;
    synchronized(lock) {
        mHost->UpdateImportanceLocked(pid, uid,
            CActivityManagerRunningAppProcessInfo::procStateToImportance(procState),
            TRUE);
    }
    return NOERROR;
}

ECode AppImportanceMonitor::ProcessObserver::OnProcessDied(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid)
{
    synchronized(mApps) {
        mHost->UpdateImportanceLocked(pid, uid,
            IActivityManagerRunningAppProcessInfo::IMPORTANCE_GONE, TRUE);
    }
    return NOERROR;
}

//=================================================================================
// AppImportanceMonitor::MyHandler
//=================================================================================
AppImportanceMonitor::MyHandler::MyHandler(
    /* [in] */ AppImportanceMonitor* host,
    /* [in] */ ILooper* looper)
    : mHost(host)
    , Handler(looper)
{}

ECode AppImportanceMonitor::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);

    switch (what) {
        case AppImportanceMonitor::MSG_UPDATE:
            mHost->OnImportanceChanged(arg1, arg2 & 0xffff, arg2 >> 16);
            break;
        default:
            return Handler::HandleMessage(msg);
    }
    return NOERROR;
}

//=================================================================================
// AppImportanceMonitor
//=================================================================================
static const Int32 AppImportanceMonitor::MSG_UPDATE = 1;

CAR_INTERFACE_IMPL(AppImportanceMonitor, Object, IAppImportanceMonitor)

AppImportanceMonitor::AppImportanceMonitor()
{}

AppImportanceMonitor::~AppImportanceMonitor()
{}

ECode AppImportanceMonitor::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ILooper* looper)
{
    mContext = context;
    mHandler = new MyHandler(looper);

    mProcessObserver = new ProcessObserver(this);

    AutoPtr<IInterface> obj;
    context->GetSystemService(IContext::ACTIVITY_SERVICE, (IInterface**)&obj);
    AutoPtr<IActivityManager> am = IActivityManager::Probe(obj);

    // try {
    ActivityManagerNative::GetDefault()::RegisterProcessObserver(mProcessObserver);
    // } catch (RemoteException e) {
    // }
    AutoPtr<IList> apps;
    am->GetRunningAppProcesses((IList**)&apps);

    if (apps != NULL) {
        Int32 size;
        apps->GetSize(&size);
        Int32 uid, pid, importance;
        for (Int32 i = 0; i < size; i++) {
            AutoPtr<IInterface> obj;
            apps->Get(i, (IInterface**)&obj);
            IActivityManagerRunningAppProcessInfo* app = IActivityManagerRunningAppProcessInfo::Probe(obj);
            app->GetUid(&uid);
            app->GetPid(&pid);
            app->GetImportance(&importance);
            UpdateImportanceLocked(uid, pid, importance, FALSE);
        }
    }
    return NOERROR;
}

ECode AppImportanceMonitor::GetImportance(
    /* [in] */ Int32 uid,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    HashMap<Int32, AutoPtr<AppEntry> >::Iterator it = mApps.Find(uid);
    if (it == mApps.End()) {
        *result = IActivityManagerRunningAppProcessInfo::IMPORTANCE_GONE;
        return NOERROR;
    }
    *result = it->mSecond->mImportance;
    return NOERROR;
}

ECode AppImportanceMonitor::OnImportanceChanged(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 importance,
    /* [in] */ Int32 oldImportance)
{
    return NOERROR;
}

void AppImportanceMonitor::UpdateImportanceLocked(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 importance,
    /* [in] */ Boolean repChange)
{
    AutoPtr<AppEntry> ent;

    HashMap<Int32, AutoPtr<AppEntry> >::Iterator it = mApps.Find(uid);
    if (it == mApps.End()) {
        ent = new AppEntry(uid);
        mApps[uid] = ent;
    }
    if (importance >= IActivityManagerRunningAppProcessInfo::IMPORTANCE_GONE) {
        ent->mProcs.Remove(pid);
    } else {
        ent->procs[pid] = importance;
    }
    UpdateImportanceLocked(ent, repChange);
}

void AppImportanceMonitor::UpdateImportanceLocked(
    /* [in] */ AppEntry* ent,
    /* [in] */ Boolean repChange)
{
    Int32 appImp = IActivityManagerRunningAppProcessInfo::IMPORTANCE_GONE;
    HashMap<Int32, Int32>::Iterator it = ent->mProcs.Begin();
    for (; it != ent->mProcs.End(); ++it) {
        Int32 procImp = it->mSecond;
        if (procImp < appImp) {
            appImp = procImp;
        }
    }

    if (appImp != ent->mImportance) {
        Int32 impCode = appImp | (ent->mImportance << 16);
        ent->mImportance = appImp;
        if (appImp >= IActivityManagerRunningAppProcessInfo::IMPORTANCE_GONE) {
            mApps->Remove(ent->mUid);
        }

        if (repChange) {
            AutoPtr<IMessage> msg;
            mHandler->ObtainMessage(MSG_UPDATE, ent->mUid, impCode, (IMessage**)&msg);
            Boolean result;
            msg->SendToTarget(&result);
        }
    }
}

} // namespace App
} // namespace Droid
} // namespace Elastos
