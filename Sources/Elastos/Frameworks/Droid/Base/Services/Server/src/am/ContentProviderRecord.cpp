
#include "am/CActivityManagerService.h"
#include "am/ContentProviderRecord.h"
#include "am/ProcessRecord.h"
#include "am/CContentProviderConnection.h"
#include "elastos/StringUtils.h"
#include "elastos/StringBuilder.h"
#include "os/UserHandle.h"
#include <elastos/Slogger.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::App::CContentProviderHolder;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

//=============================================================================
// ContentProviderRecord::ExternalProcessHandle
//=============================================================================

const String ContentProviderRecord::ExternalProcessHandle::TAG = String("ExternalProcessHanldle");

CAR_INTERFACE_IMPL(ContentProviderRecord::ExternalProcessHandle, IProxyDeathRecipient)

ContentProviderRecord::ExternalProcessHandle::ExternalProcessHandle(
    /* [in] */ IBinder* token,
    /* [in] */ ContentProviderRecord* owner)
    : mAcquisitionCount(0)
    , mToken(token)
    , mOwner(owner)
{
    AutoPtr<IProxy> proxy = (IProxy*)token->Probe(EIID_IProxy);
    assert(proxy != NULL);
    if (FAILED(proxy->LinkToDeath(this, 0))) {
        Slogger::E(TAG, "Couldn't register for death for token: %p", mToken.Get());
    }
}

ECode ContentProviderRecord::ExternalProcessHandle::UnlinkFromOwnDeathLocked()
{
    AutoPtr<IProxy> proxy = (IProxy*)mToken->Probe(EIID_IProxy);
    assert(proxy != NULL);
    Boolean res;
    return proxy->UnlinkToDeath(this, 0, &res);
}

// @Override
ECode ContentProviderRecord::ExternalProcessHandle::ProxyDied()
{
    PFL_EX("ProxyDied()");
    Object::Autolock lock(mOwner->mService->mLock);
    if (mOwner->HasExternalProcessHandles() &&
        mOwner->mExternalProcessTokenToHandle.Find(mToken)
        != mOwner->mExternalProcessTokenToHandle.End()) {
        mOwner->RemoveExternalProcessHandleInternalLocked(mToken);
    }
    return NOERROR;
}

//=============================================================================
// ContentProviderRecord
//=============================================================================


ContentProviderRecord::ContentProviderRecord(
    /* [in] */ CActivityManagerService* service,
    /* [in] */ IProviderInfo* info,
    /* [in] */ IApplicationInfo* ai,
    /* [in] */ IComponentName* name,
    /* [in] */ Boolean singleton)
    : mService(service)
    , mUid(0)
    , mSingleton(singleton)
    , mNoReleaseNeeded(FALSE)
    , mExternalProcessNoHandleCount(0)
    , mProc(NULL)
    , mLaunchingApp(NULL)
{
    mInfo = info;
    mName = name;
    ai->GetUid(&mUid);
    mAppInfo = ai;
    mNoReleaseNeeded = mUid == 0 || mUid == IProcess::SYSTEM_UID;
}

ContentProviderRecord::ContentProviderRecord(
    /* [in] */ ContentProviderRecord* cpr)
    : mLaunchingApp(NULL)
{
    mService = cpr->mService;
    mInfo = cpr->mInfo;
    mUid = cpr->mUid;
    mAppInfo = cpr->mAppInfo;
    mName = cpr->mName;
    mSingleton = cpr->mSingleton;
    mNoReleaseNeeded = cpr->mNoReleaseNeeded;
    mName = cpr->mName;
}

ContentProviderRecord::~ContentProviderRecord()
{}

CAR_INTERFACE_IMPL(ContentProviderRecord, ISynchronize);

AutoPtr<IContentProviderHolder> ContentProviderRecord::NewHolder(
    /* [in] */ CContentProviderConnection* conn)
{
     AutoPtr<IContentProviderHolder> holder;
     CContentProviderHolder::New(mInfo, (IContentProviderHolder**)&holder);
     holder->SetContentProvider(mProvider);
     holder->SetNoReleaseNeeded(mNoReleaseNeeded);
     holder->SetConnection(conn);
     return holder;
}

Boolean ContentProviderRecord::CanRunHere(
    /* [in] */ ProcessRecord* app)
{
    Boolean multiprocess;
    mInfo->GetMultiprocess(&multiprocess);
    String processName;
    mInfo->GetProcessName(&processName);

    Int32 appUid;
    app->mInfo->GetUid(&appUid);
    return (multiprocess || processName.Equals(app->mProcessName)) && mUid == appUid;
}

void ContentProviderRecord::AddExternalProcessHandleLocked(
    /* [in] */ IBinder* token)
{
    if (token == NULL) {
        mExternalProcessNoHandleCount++;
    }
    else {
        HashMap<AutoPtr<IBinder>, AutoPtr<ExternalProcessHandle> >::Iterator it;
        it = mExternalProcessTokenToHandle.Find(token);

        AutoPtr<ExternalProcessHandle> handle;
        if (it == mExternalProcessTokenToHandle.End()) {
            handle = new ExternalProcessHandle(token, this);
            mExternalProcessTokenToHandle.Insert(ProcessHandleValueType(token, handle));
        }
        else {
            handle = it->mSecond;
        }

        handle->mAcquisitionCount++;
    }
}

Boolean ContentProviderRecord::RemoveExternalProcessHandleLocked(
    /* [in] */ IBinder* token)
{
    if (HasExternalProcessHandles()) {
        Boolean hasHandle = FALSE;
        HashMap<AutoPtr<IBinder>, AutoPtr<ExternalProcessHandle> >::Iterator it;
        it = mExternalProcessTokenToHandle.Find(token);

        if (it != mExternalProcessTokenToHandle.End()) {
            AutoPtr<ExternalProcessHandle> handle = it->mSecond;
            hasHandle = TRUE;
            handle->mAcquisitionCount--;
            if (handle->mAcquisitionCount == 0) {
                RemoveExternalProcessHandleInternalLocked(token);
                return TRUE;
            }
        }

        if (!hasHandle) {
            mExternalProcessNoHandleCount--;
            return TRUE;
        }
    }
    return FALSE;
}

void ContentProviderRecord::RemoveExternalProcessHandleInternalLocked(
    /* [in] */ IBinder* token)
{
    HashMap<AutoPtr<IBinder>, AutoPtr<ExternalProcessHandle> >::Iterator it;
    it = mExternalProcessTokenToHandle.Find(token);
    if (it != mExternalProcessTokenToHandle.End()) {
        AutoPtr<ExternalProcessHandle> handle = it->mSecond;
        handle->UnlinkFromOwnDeathLocked();
    }
    mExternalProcessTokenToHandle.Erase(token);
}

Boolean ContentProviderRecord::HasExternalProcessHandles()
{
    return (mExternalProcessTokenToHandle.IsEmpty() == FALSE || mExternalProcessNoHandleCount > 0);
}

void ContentProviderRecord::Dump(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& prefix,
    /* [in] */ Boolean full)
{
//     if (full) {
//         pw.print(prefix); pw.print("package=");
//                 pw.print(info.applicationInfo.packageName);
//                 pw.print(" process="); pw.println(info.processName);
//     }
//     pw.print(prefix); pw.print("proc="); pw.println(proc);
//     if (launchingApp != null) {
//         pw.print(prefix); pw.print("launchingApp="); pw.println(launchingApp);
//     }
//     if (full) {
//         pw.print(prefix); pw.print("uid="); pw.print(uid);
//                 pw.print(" provider="); pw.println(provider);
//     }
//     if (singleton) {
//         pw.print(prefix); pw.print("singleton="); pw.println(singleton);
//     }
//     pw.print(prefix); pw.print("authority="); pw.println(info.authority);
//     if (full) {
//         if (info.isSyncable || info.multiprocess || info.initOrder != 0) {
//             pw.print(prefix); pw.print("isSyncable="); pw.print(info.isSyncable);
//                     pw.print(" multiprocess="); pw.print(info.multiprocess);
//                     pw.print(" initOrder="); pw.println(info.initOrder);
//         }
//     }
//     if (full) {
//         if (hasExternalProcessHandles()) {
//             pw.print(prefix); pw.print("externals=");
//                     pw.println(mExternalProcessTokenToHandle.size());
//         }
//     } else {
//         if (connections.size() > 0 || mExternalProcessNoHandleCount > 0) {
//             pw.print(prefix); pw.print(connections.size());
//                     pw.print(" connections, "); pw.print(mExternalProcessNoHandleCount);
//                     pw.println(" external handles");
//         }
//     }
//     if (connections.size() > 0) {
//         if (full) {
//             pw.print(prefix); pw.println("Connections:");
//         }
//         for (int i=0; i<connections.size(); i++) {
//             CContentProviderConnection conn = connections.get(i);
//             pw.print(prefix); pw.print("  -> "); pw.println(conn.toClientString());
//             if (conn.provider != this) {
//                 pw.print(prefix); pw.print("    *** WRONG PROVIDER: ");
//                         pw.println(conn.provider);
//             }
//         }
//     }
}

String ContentProviderRecord::ToString()
{
    if (!mStringName.IsNull()) {
        return mStringName;
    }

    String nameStr;
    mName->FlattenToShortString(&nameStr);

    StringBuilder sb(128);
    sb += ("ContentProviderRecord{");
    sb += StringUtils::Int32ToHexString((Int32)this);
    sb += (" u");
    sb += (UserHandle::GetUserId(mUid));
    sb += (' ');
    sb += nameStr;
    sb += ('}');
    return mStringName = sb.ToString();
}

String ContentProviderRecord::ToShortString()
{
    if (!mShortStringName.IsNull()) {
        return mShortStringName;
    }

    String nameStr;
    mName->FlattenToShortString(&nameStr);

    StringBuilder sb (128);
    sb += StringUtils::Int32ToHexString((Int32)this);
    sb += ('/');
    sb += nameStr;
    return mShortStringName = sb.ToString();
}

ECode ContentProviderRecord::Lock()
{
    return Object::Lock();
}

ECode ContentProviderRecord::Unlock()
{
    return Object::Unlock();
}

ECode ContentProviderRecord::Wait()
{
    return Object::Wait();
}

ECode ContentProviderRecord::WaitEx(
    /* [in] */ Int64 millis)
{
    return Object::Wait(millis);
}

ECode ContentProviderRecord::WaitEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Object::Wait(millis, nanos);
}

ECode ContentProviderRecord::Notify()
{
    return Object::Notify();
}

ECode ContentProviderRecord::NotifyAll()
{
    return Object::NotifyAll();
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
