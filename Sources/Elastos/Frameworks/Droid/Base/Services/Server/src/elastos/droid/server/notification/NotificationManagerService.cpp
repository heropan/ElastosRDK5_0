
#include "elastos/droid/server/notification/NotificationManagerService.h"
#include "elastos/droid/os/ServiceManager.h"
#include "elastos/droid/app/ActivityManagerNative.h"
#include "elastos/droid/app/AppGlobals.h"
#include "elastos/droid/os/Binder.h"
#include "elastos/droid/os/Process.h"
#include "CStatusBarManagerService.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/R.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include "util/Xml.h"

using Elastos::Droid::App::AppGlobals;
using Elastos::Droid::App::IActivityManagerHelper;
using Elastos::Droid::App::CActivityManagerHelper;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IIPackageManager;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Media::IAudioManager;
using Elastos::Droid::Media::IIRingtonePlayer;
using Elastos::Droid::Provider::ISettingsSystem;
using Elastos::Droid::Provider::CSettingsSystem;
using Elastos::Droid::Provider::ISettingsGlobal;
using Elastos::Droid::Provider::CSettingsGlobal;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::StatusBar::IStatusBarNotification;
using Elastos::Droid::StatusBar::CStatusBarNotification;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Utility::IAtomicFile;
using Elastos::Droid::Utility::CAtomicFile;
using Elastos::Droid::Utility::IFastXmlSerializer;
using Elastos::Droid::Utility::CFastXmlSerializer;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityManager;
using Elastos::Droid::View::Accessibility::CAccessibilityManager;
using Elastos::Droid::View::Accessibility::IAccessibilityManagerHelper;
using Elastos::Droid::View::Accessibility::CAccessibilityManagerHelper;
using Elastos::Droid::View::Accessibility::IAccessibilityEventHelper;
using Elastos::Droid::View::Accessibility::CAccessibilityEventHelper;
using Elastos::Droid::Widget::IRemoteViews;
using Elastos::Droid::Widget::IToast;
using Elastos::Core::StringBuilder;
using Elastos::Core::CString;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Core::StringUtils;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::IO::IIoUtils;
using Elastos::IO::CIoUtils;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::IFileInputStream;
using Elastos::Utility::CArrayList;
using Elastos::Utility::ICollection;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Logging::Slogger;
using Org::Xmlpull::V1::IXmlPullParser;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Notification {

const String NotificationManagerService::TAG("NotificationService");
const Boolean NotificationManagerService::DBG = TRUE;

const Int32 NotificationManagerService::MAX_PACKAGE_NOTIFICATIONS = 50;

const Int32 NotificationManagerService::MESSAGE_TIMEOUT;
const Int32 NotificationManagerService::MESSAGE_SAVE_POLICY_FILE;
const Int32 NotificationManagerService::MESSAGE_RECONSIDER_RANKING;
const Int32 NotificationManagerService::MESSAGE_RANKING_CONFIG_CHANGE;
const Int32 NotificationManagerService::MESSAGE_SEND_RANKING_UPDATE;
const Int32 NotificationManagerService::MESSAGE_LISTENER_HINTS_CHANGED;
const Int32 NotificationManagerService::MESSAGE_LISTENER_NOTIFICATION_FILTER_CHANGED;

const Int32 NotificationManagerService::LONG_DELAY = 3500; // 3.5 seconds
const Int32 NotificationManagerService::SHORT_DELAY = 2000; // 2 seconds

AutoPtr<ArrayOf<Int64> > InitDEFAULT_VIBRATE_PATTERN()
{
    AutoPtr<ArrayOf<Int64> > result = ArrayOf<Int64>::Alloc(4);
    result->Set(0, 0);
    result->Set(1, 250);
    result->Set(2, 250);
    result->Set(3, 250);
    return result;
}
const AutoPtr<ArrayOf<Int64> > NotificationManagerService::DEFAULT_VIBRATE_PATTERN = InitDEFAULT_VIBRATE_PATTERN();//{0, 250, 250, 250};
const Int32 NotificationManagerService::VIBRATE_PATTERN_MAXLEN = 8 * 2 + 1; // up to eight bumps

const Int32 NotificationManagerService::DEFAULT_STREAM_TYPE = IAudioManager::STREAM_NOTIFICATION;
const Boolean NotificationManagerService::SCORE_ONGOING_HIGHER = FALSE;

const Int32 NotificationManagerService::JUNK_SCORE = -1000;
const Int32 NotificationManagerService::NOTIFICATION_PRIORITY_MULTIPLIER = 10;
const Int32 NotificationManagerService::SCORE_DISPLAY_THRESHOLD = INotification::PRIORITY_MIN * 10 /*NotificationManagerService::NOTIFICATION_PRIORITY_MULTIPLIER*/;
const Int32 NotificationManagerService::SCORE_INTERRUPTION_THRESHOLD = INotification::PRIORITY_LOW * 10 /*NotificationManagerService::NOTIFICATION_PRIORITY_MULTIPLIER*/;

const Boolean NotificationManagerService::ENABLE_BLOCKED_NOTIFICATIONS = TRUE;
const Boolean NotificationManagerService::ENABLE_BLOCKED_TOASTS = TRUE;
const Int32 NotificationManagerService::MATCHES_CALL_FILTER_CONTACTS_TIMEOUT_MS = 3000;
const Float NotificationManagerService::MATCHES_CALL_FILTER_TIMEOUT_AFFINITY = ValidateNotificationPeople::STARRED_CONTACT;

const Int32 NotificationManagerService::DB_VERSION = 1;

const String NotificationManagerService::TAG_BODY("notification-policy");
const String NotificationManagerService::ATTR_VERSION("version");

const String NotificationManagerService::TAG_BLOCKED_PKGS("blocked-packages");
const String NotificationManagerService::TAG_PACKAGE("package");
const String NotificationManagerService::ATTR_NAME("name");

const Int32 NotificationManagerService::MY_UID = Process::MyUid();
const Int32 NotificationManagerService::MY_PID = Process::MyPid();
const Int32 NotificationManagerService::REASON_DELEGATE_CLICK = 1;
const Int32 NotificationManagerService::REASON_DELEGATE_CANCEL = 2;
const Int32 NotificationManagerService::REASON_DELEGATE_CANCEL_ALL = 3;
const Int32 NotificationManagerService::REASON_DELEGATE_ERROR = 4;
const Int32 NotificationManagerService::REASON_PACKAGE_CHANGED = 5;
const Int32 NotificationManagerService::REASON_USER_STOPPED = 6;
const Int32 NotificationManagerService::REASON_PACKAGE_BANNED = 7;
const Int32 NotificationManagerService::REASON_NOMAN_CANCEL = 8;
const Int32 NotificationManagerService::REASON_NOMAN_CANCEL_ALL = 9;
const Int32 NotificationManagerService::REASON_LISTENER_CANCEL = 10;
const Int32 NotificationManagerService::REASON_LISTENER_CANCEL_ALL = 11;
const Int32 NotificationManagerService::REASON_GROUP_SUMMARY_CANCELED = 12;

//===============================================================================
//                  NotificationManagerService::NotificationManagerStub
//===============================================================================

CAR_INTERFACE_IMPL_2(NotificationManagerService::NotificationManagerStub, Object, IINotificationManager, IBinder);

NotificationManagerService::NotificationManagerStub::NotificationManagerStub();

NotificationManagerService::NotificationManagerStub::~NotificationManagerStub();

ECode NotificationManagerService::NotificationManagerStub::constructor(
    /* [in] */ ISystemService* host)
{
    mHost = (NotificationManagerService*)service;
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::EnqueueToast(
    /* [in] */ const String& pkg,
    /* [in] */ IITransientNotification* callback,
    /* [in] */ Int32 duration)
{
    if (DBG) {
        Slogger::I(TAG, "enqueueToast pkg=%s, callback=%p, duration=%d", pkg.string(), callback, duration);
    }

    if (pkg.IsNull() || callback == NULL) {
        Slogger::E(TAG, "Not doing toast. pkg=%s, callback=%p", pkg.string(), callback);
        return NOERROR;
    }

    Boolean isSystemToast = mHost->IsCallerSystem() || pkg.Equals("android");

    if (ENABLE_BLOCKED_TOASTS && !mHost->NoteNotificationOp(pkg, Binder::GetCallingUid())) {
        if (!isSystemToast) {
            StringBuilder buf;
            buf += "Suppressing toast from package ";
            buf += pkg;
            buf += " by user request.";
            Slogger::E(TAG, buf.ToString());
            return NOERROR;
        }
    }

    synchronized(mToastQueue) {
        Int32 callingPid = Binder::GetCallingPid();
        Int64 callingId = Binder::ClearCallingIdentity();

        // try {
        AutoPtr<NotificationManagerService::ToastRecord> record;
        Int32 index = mHost->IndexOfToastLocked(pkg, callback);

        // If it's already in the queue, we update it in place, we don't
        // move it to the end of the queue.
        if (index >= 0) {
            AutoPtr<IInterface> obj;
            mHost->mToastQueue->Get(index, (IInterface**)&obj);
            record = (NotificationManagerService::ToastRecord*)IObject::Probe(obj);
            record->Update(duration);
        }
        else {
            // Limit the number of toasts that any given package except the android
            // package can enqueue.  Prevents DOS attacks and deals with leaks.
            if (!isSystemToast) {
                Int32 count = 0;
                Int32 N;
                mHost->mToastQueue->GetSize(&size);
                for (Int32 i = 0; i < size; i++) {
                    AutoPtr<IInterface> obj;
                    mToastQueue->Get(i, (IInterface**)&obj);
                    AutoPtr<NotificationManagerService::ToastRecord> r = (NotificationManagerService::ToastRecord*)IObject::Probe(obj);
                    if (r->mPkg.Equals(pkg)) {
                        count++;

                        if (count >= MAX_PACKAGE_NOTIFICATIONS) {
                            StringBuilder buf;
                            buf += "Package has already posted ";
                            buf += count;
                            buf += " toasts. Not showing more. Package=";
                            buf += pkg;
                            Logger::E(TAG, buf.ToString());
                            Binder::RestoreCallingIdentity(callingId);
                            return NOERROR;
                        }
                    }
                }
            }

            record = new NotificationManagerService::ToastRecord(callingPid, pkg, callback, duration);
            mHost->mToastQueue->Add(TO_IINTERFACE(record));
            mHost->mToastQueue->GetSize(&(index + 1));
            mHost->KeepProcessAliveLocked(callingPid);
        }

        // If it's at index 0, it's the current toast.  It doesn't matter if it's
        // new or just been updated.  Call back and tell it to show itself.
        // If the callback fails, this will remove it from the list, so don't
        // assume that it's valid after this.
        if (index == 0) {
            mHost->ShowNextToastLocked();
        }

        Binder::RestoreCallingIdentity(callingId);
    }

    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::CancelToast(
    /* [in] */ const String& pkg,
    /* [in] */ IITransientNotification* callback)
{
    Slogger::I(TAG, "cancelToast pkg=%s, callback=%p", pkg.string(), callback);

    if (pkg.IsNull() || callback == NULL) {
        Slogger::E(TAG, "Not cancelling notification. pkg=%s, callback=%p", pkg.string(), callback);
        return NOERROR;
    }

    synchronized(mToastQueue) {
        Int64 callingId = Binder::ClearCallingIdentity();
        // try {
        Int32 index = mHost->IndexOfToastLocked(pkg, callback);
        if (index >= 0) {
            mHost->CancelToastLocked(index);
        }
        else {
            Slogger::W(TAG, "Toast already cancelled. pkg=%s, callback=%p", pkg.string(), callback);
        }

        Binder::RestoreCallingIdentity(callingId);
    }

    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::EnqueueNotificationWithTag(
    /* [in] */ const String& pkg,
    /* [in] */ const String& opPkg,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification,
    /* [in] */ ArrayOf<Int32>* inIdReceived,
    /* [in] */ Int32 userId,
    /* [out, callee] */ ArrayOf<Int32>** outIdReceived)
{
    VALIDATE_NOT_NULL(outIdReceived);
    *outIdReceived = NULL;

    return mHost->EnqueueNotificationInternal(pkg, opPkg,
            Binder::GetCallingUid(),
            Binder::GetCallingPid(),
            tag, id, notification, idOut, userId, outIdReceived);
}

ECode NotificationManagerService::NotificationManagerStub::CancelNotificationWithTag(
    /* [in] */ const String& pkg,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ Int32 userId)
{
    mHost->CheckCallerIsSystemOrSameApp(pkg);

    AutoPtr<IActivityManagerHelper> activityMgrHelper;
    CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&activityMgrHelper);

    Int32 tmpUserId;
    activityMgrHelper->HandleIncomingUser(Binder::GetCallingPid(),
            Binder::GetCallingUid(), userId, TRUE, FALSE,
            String("cancelNotificationWithTag"), pkg, &tmpUserId);

    // Don't allow client applications to cancel foreground service notis.
    mHost->CancelNotification(Binder::GetCallingUid(),
            Binder::GetCallingPid(),
            ,pkg, tag, id, 0, Binder::GetCallingUid() == IProcess::SYSTEM_UID ?
            0 : INotification::FLAG_FOREGROUND_SERVICE,
            FALSE, tmpUserId, REASON_NOMAN_CANCEL, NULL);
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::CancelAllNotifications(
    /* [in] */ const String& pkg,
    /* [in] */ Int32 userId)
{
    mHost->CheckCallerIsSystemOrSameApp(pkg);

    AutoPtr<IActivityManagerHelper> activityMgrHelper;
    CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&activityMgrHelper);

    Int32 tmpUserId;
    activityMgrHelper->HandleIncomingUser(Binder::GetCallingPid(),
            Binder::GetCallingUid(), userId, TRUE, FALSE,
            String("cancelAllNotifications"), pkg, &tmpUserId);

    // Calling from user space, don't allow the canceling of actively
    // running foreground services.
    mHost->CancelAllNotificationsInt(Binder::GetCallingUid(),
            Binder::GetCallingPid(),
            pkg, 0, INotification::FLAG_FOREGROUND_SERVICE, TRUE, tmpUserId,
            REASON_NOMAN_CANCEL_ALL, NULL);
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::SetNotificationsEnabledForPackage(
    /* [in] */ const String& pkg,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean enabled)
{
    mHost->CheckCallerIsSystem();

    mHost->SetNotificationsEnabledForPackageImpl(pkg, uid, enabled);
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::AreNotificationsEnabledForPackage(
    /* [in] */ const String& pkg,
    /* [in] */ Int32 uid,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(result);

    mHost->CheckCallerIsSystem();
    Int32 data;
    mHost->mAppOps->CheckOpNoThrow(IAppOpsManager::OP_POST_NOTIFICATION, uid, pkg, &data);
    *result = data == IAppOpsManager::MODE_ALLOWED;
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::SetPackagePriority(
    /* [in] */ const String& pkg,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 priority)
{
    mHost->CheckCallerIsSystem();
    mHost->mRankingHelper->SetPackagePriority(pkg, uid, priority);
    mHost->SavePolicyFile();
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::GetPackagePriority(
    /* [in] */ const String& pkg,
    /* [in] */ Int32 uid,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    mHost->CheckCallerIsSystem();
    return mHost->mRankingHelper->GetPackagePriority(pkg, uid, result);
}

ECode NotificationManagerService::NotificationManagerStub::SetPackageVisibilityOverride(
    /* [in] */ const String& pkg,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 visibility)
{
    mHost->CheckCallerIsSystem();
    mHost->mRankingHelper->SetPackageVisibilityOverride(pkg, uid, visibility);
    mHost->SavePolicyFile();
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::GetPackageVisibilityOverride(
    /* [in] */ const String& pkg,
    /* [in] */ Int32 uid,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    mHost->CheckCallerIsSystem();
    return mHost->mRankingHelper->GetPackageVisibilityOverride(pkg, uid, result);
}

ECode NotificationManagerService::NotificationManagerStub::GetActiveNotifications(
    /* [in] */ const String& callingPkg,
    /* [out, callee] */ ArrayOf<IStatusBarNotification*>** ns)
{
    VALIDATE_NOT_NULL(ns);

    // enforce() will ensure the calling uid has the correct permission
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    context->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::ACCESS_NOTIFICATIONS,
            String("NotificationManagerService.getActiveNotifications"));

    AutoPtr< ArrayOf<IStatusBarNotification*> > tmp;
    Int32 uid = Binder::GetCallingUid();

    // noteOp will check to make sure the callingPkg matches the uid
    Int32 result;
    mHost->mAppOps->NoteOpNoThrow(IAppOpsManager::OP_ACCESS_NOTIFICATIONS, uid, callingPkg, &result);
    if (result == IAppOpsManager::MODE_ALLOWED) {
        synchronized(mNotificationList) {
            Int32 N;
            mHost->mNotificationList->GetSize(&N);
            tmp = ArrayOf<IStatusBarNotification*>::Alloc(N);
            for (Int32 i = 0; i < N; i++) {
                AutoPtr<IInterface> obj;
                mHost->mNotificationList->Get(i, (IInterface**)&obj);
                AutoPtr<NotificationRecord> record = (NotificationRecord*)IObject::Probe(obj);
                (*tmp)[i] = record->mSbn;
            }
        }
    }

    *ns = tmp;
    REFCOUNT_ADD(*ns);
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::GetHistoricalNotifications(
    /* [in] */ const String& callingPkg,
    /* [in] */ Int32 count,
    /* [out, callee] */ ArrayOf<IStatusBarNotification*>* ns)
{
    VALIDATE_NOT_NULL(ns);

    // enforce() will ensure the calling uid has the correct permission
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    context->EnforceCallingOrSelfPermission(
            Elastos::Droid::Manifest::permission::ACCESS_NOTIFICATIONS,
            String("NotificationManagerService.getHistoricalNotifications"));

    AutoPtr< ArrayOf<IStatusBarNotification*> > tmp;
    Int32 uid = Binder::GetCallingUid();

    // noteOp will check to make sure the callingPkg matches the uid
    Int32 result;
    mHost->mAppOps->NoteOpNoThrow(IAppOpsManager::OP_ACCESS_NOTIFICATIONS, uid, callingPkg, &result);
    if (result == IAppOpsManager::MODE_ALLOWED) {
        synchronized(mArchive) {
            mHost->mArchive->GetArray(count, (ArrayOf<IStatusBarNotification*>**)&tmp);
        }
    }
    *ns = tmp;
    REFCOUNT_ADD(*ns);
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::RegisterListener(
    /* [in] */ IINotificationListener* listener,
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 userid)
{
    EnforceSystemOrSystemUI(String("INotificationManager.registerListener"));
    mHost->mListeners->RegisterService(listener, component, userid);
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::UnregisterListener(
    /* [in] */ IINotificationListener* listener,
    /* [in] */ Int32 userid)
{
    mHost->mListeners->UnregisterService(listener, userid);
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::CancelNotificationsFromListener(
    /* [in] */ IINotificationListener* token,
    /* [in] */ ArrayOf<String>* keys)
{
    const Int32 callingUid = Binder::GetCallingUid();
    const Int32 callingPid = Binder::GetCallingPid();
    Int64 identity = Binder::ClearCallingIdentity();
    // try {
    synchronized(mNotificationList) {
        AutoPtr<ManagedService::ManagedServiceInfo> info;
        mHost->mListeners->CheckServiceTokenLocked(token, (ManagedService::ManagedServiceInfo**)&info);
        if (keys != NULL) {
            const Int32 N = keys->GetLength();
            for (Int32 i = 0; i < N; i++) {
                AutoPtr<IInterface> obj;
                mHost->mNotificationsByKey->Get(CoreUtils::Convert((*keys)[i]), (IInterface**)&obj);
                AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);
                if (r == NULL) continue;
                Int32 userId;
                r->mSbn->GetUserId(&userId);
                if (userId != info->mUserid && userId != IUserHandle::USER_ALL &&
                        !mHost->mUserProfiles->IsCurrentProfile(userId)) {
                    Binder::RestoreCallingIdentity(identity);
                    Slogger::E(TAG, "Disallowed call from listener: %p", info->mService.Get());
                    return E_SECURITY_EXCEPTION;
                    // throw new SecurityException("Disallowed call from listener: "
                    //         + info.service);
                }
                String pkg, tag;
                Int32 id;
                r->mSbn->GetPackageName(&pkg);
                r->mSbn->GetTag(&tag);
                r->mSbn->GetId(&id);
                CancelNotificationFromListenerLocked(info, callingUid, callingPid,
                        pkg, tag, id, userId);
            }
        }
        else {
            mHost->CancelAllLocked(callingUid, callingPid, info->mUserid,
                    REASON_LISTENER_CANCEL_ALL, info, info->SupportsProfiles());
        }
    }
    // } finally {
    Binder::RestoreCallingIdentity(identity);
    // }
    return NOERROR;
}

void NotificationManagerService::NotificationManagerStub::CancelNotificationFromListenerLocked(
    /* [in] */ ManagedServiceInfo* info,
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 callingPid,
    /* [in] */ const String& pkg,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ Int32 userId)
{
    mHost->CancelNotification(callingUid, callingPid, pkg, tag, id, 0,
            INotification::FLAG_ONGOING_EVENT | INotification::FLAG_FOREGROUND_SERVICE,
            TRUE, userId, REASON_LISTENER_CANCEL, info);
}

ECode NotificationManagerService::NotificationManagerStub::CancelNotificationFromListener(
    /* [in] */ IINotificationListener* token,
    /* [in] */ const String& pkg,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id)
{
    const Int32 callingUid = Binder::GetCallingUid();
    const Int32 callingPid = Binder::GetCallingPid();
    Int64 identity = Binder::ClearCallingIdentity();
    // try {
    synchronized(mNotificationList) {
        AutoPtr<ManagedService::ManagedServiceInfo> info;
        mHost->mListeners->CheckServiceTokenLocked(token, (ManagedService::ManagedServiceInfo**)&info);
        if (info->SupportsProfiles()) {
            Logger::E(TAG, "Ignoring deprecated cancelNotification(pkg, tag, id) from %p use cancelNotification(key) instead."
                    , info->mComponent.Get());
        }
        else {
            CancelNotificationFromListenerLocked(info, callingUid, callingPid,
                    pkg, tag, id, info->mUserid);
        }
    }
    // } finally {
    Binder::RestoreCallingIdentity(identity);
    // }
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::GetActiveNotificationsFromListener(
    /* [in] */ IINotificationListener* token,
    /* [in] */ ArrayOf<String>* keys,
    /* [in] */ Int32 trim,
    /* [out] */ IParceledListSlice** slice)
{
    VALIDATE_NOT_NULL(slice);

    synchronized(mNotificationList) {
        AutoPtr<ManagedService::ManagedServiceInfo> info;
        mHost->mListeners->CheckServiceTokenLocked(token, (ManagedService::ManagedServiceInfo**)&info);
        const Boolean getKeys = keys != NULL;
        Int32 size;
        mNotificationList->GetSize(&size);
        const Int32 N = getKeys ? keys->GetLength() : size;
        AutoPtr<IArrayList> list;
        CArrayList::New(N, (IArrayList**)&list);
        for (Int32 i = 0; i < N; i++) {
            if (getKeys) {
                AutoPtr<IInterface> obj;
                mHost->mNotificationsByKey->Get(CoreUtils::Convert((*keys)[i]), (IInterface**)&obj);
                AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);
            }
            else {
                AutoPtr<IInterface> obj;
                mHost->mNotificationList->Get(i, (IInterface**)&obj);
                AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);
            }
            if (r == NULL) continue;
            AutoPtr<IStatusBarNotification> sbn = r->mSbn;
            if (!IsVisibleToListener(sbn, info)) continue;
            AutoPtr<IStatusBarNotification> sbnToSend;
            if (trim != TRIM_FULL) {
                AutoPtr<IStatusBarNotification> action;
                sbn->CloneLight((IStatusBarNotification**)&action);
                sbnToSend = action;
            }
            else {
                sbnToSend = sbn;
            }
            list->Add(sbnToSend);
        }
        return CParceledListSlice::New(IList::Probe(list), slice);
    }
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::RequestHintsFromListener(
    /* [in] */ IINotificationListener* token,
    /* [in] */ Int32 hints)
{
    const Int64 identity = Binder::ClearCallingIdentity();
    // try {
    synchronized(mNotificationList) {
        AutoPtr<ManagedService::ManagedServiceInfo> info;
        mHost->mListeners->CheckServiceTokenLocked(token, (ManagedService::ManagedServiceInfo**)&info);
        const Boolean disableEffects = (hints & HINT_HOST_DISABLE_EFFECTS) != 0;
        if (disableEffects) {
            mHost->mListenersDisablingEffects->Add(TO_IINTERFACE(info));
        }
        else {
            mHost->mListenersDisablingEffects->Remove(TO_IINTERFACE(info));
        }
        mHost->UpdateListenerHintsLocked();
        mHost->UpdateEffectsSuppressorLocked();
    }
    // } finally {
    Binder::RestoreCallingIdentity(identity);
    // }
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::GetHintsFromListener(
    /* [in] */ IINotificationListener* token,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    synchronized(mNotificationList) {
        *result = mHost->mListenerHints;
        return NOERROR;
    }
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::RequestInterruptionFilterFromListener(
    /* [in] */ IINotificationListener* token,
    /* [in] */ Int32 interruptionFilter)
{
    const Int64 identity = Binder::ClearCallingIdentity();
    // try {
    synchronized(mNotificationList) {
        AutoPtr<ManagedService::ManagedServiceInfo> info;
        mHost->mListeners->CheckServiceTokenLocked(token, (ManagedService::ManagedServiceInfo**)&info);
        mHost->mZenModeHelper->RequestFromListener(interruptionFilter);
        mHost->UpdateInterruptionFilterLocked();
    }
    // } finally {
    Binder::RestoreCallingIdentity(identity);
    // }
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::GetInterruptionFilterFromListener(
    /* [in] */ IINotificationListener* token,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    synchronized (mNotificationLight) {
        *result = mInterruptionFilter;
        return NOERROR;
    }
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::SetOnNotificationPostedTrimFromListener(
    /* [in] */ IINotificationListener* token,
    /* [in] */ Int32 trim)
{
    synchronized (mNotificationList) {
        AutoPtr<ManagedService::ManagedServiceInfo> info;
        mHost->mListeners->CheckServiceTokenLocked(token, (ManagedService::ManagedServiceInfo**)&info);
        if (info == NULL) return NOERROR;
        mHost->mListeners->SetOnNotificationPostedTrimLocked(info, trim);
    }
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::GetZenModeConfig(
    /* [out] */ IZenModeConfig** config)
{
    VALIDATE_NOT_NULL(config);
    EnforceSystemOrSystemUI(String("INotificationManager.getZenModeConfig"));
    return mHost->mZenModeHelper->GetConfig(config);
}

ECode NotificationManagerService::NotificationManagerStub::SetZenModeConfig(
    /* [in] */ IZenModeConfig* config,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    mHost->CheckCallerIsSystem();
    return mHost->mZenModeHelper->SetConfig(config, result);
}

ECode NotificationManagerService::NotificationManagerStub::NotifyConditions(
    /* [in] */ const String& pkg,
    /* [in] */ IIConditionProvider* provider,
    /* [in] */ ArrayOf<ICondition*>* conditions)
{
    AutoPtr<ManagedService::ManagedServiceInfo> info;
    mHost->mConditionProviders->CheckServiceToken(provider, (ManagedService::ManagedServiceInfo**)&info);
    mHost->CheckCallerIsSystemOrSameApp(pkg);
    const Int64 identity = Binder::ClearCallingIdentity();
    // try {
    mHost->mConditionProviders->NotifyConditions(pkg, info, conditions);
    // } finally {
    Binder::RestoreCallingIdentity(identity);
    // }
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::RequestZenModeConditions(
    /* [in] */ IIConditionListener* callback,
    /* [in] */ Int32 relevance)
{
    EnforceSystemOrSystemUI(String("INotificationManager.requestZenModeConditions"));
    mHost->mConditionProviders->RequestZenModeConditions(callback, relevance);
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::SetZenModeCondition(
    /* [in] */ ICondition* condition)
{
    EnforceSystemOrSystemUI(String("INotificationManager.setZenModeCondition"));
    const Int64 identity = Binder::ClearCallingIdentity();
    // try {
    mHost->mConditionProviders->SetZenModeCondition(condition, String("binderCall"));
    // } finally {
    Binder::RestoreCallingIdentity(identity);
    // }
}

ECode NotificationManagerService::NotificationManagerStub::SetAutomaticZenModeConditions(
    /* [in] */ ArrayOf<IUri*>* conditionIds)
{
    EnforceSystemOrSystemUI(String("INotificationManager.setAutomaticZenModeConditions"));
    mHost->mConditionProviders->SetAutomaticZenModeConditions(conditionIds);
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::GetAutomaticZenModeConditions(
    /* [out, callee] */ ArrayOf<ICondition*>** condition)
{
    VALIDATE_NOT_NULL(condition);
    EnforceSystemOrSystemUI(String("INotificationManager.getAutomaticZenModeConditions"));
    return mHost->mConditionProviders->GetAutomaticZenModeConditions(condition);
}

void NotificationManagerService::NotificationManagerStub::EnforceSystemOrSystemUI(
    /* [in] */ const String& message)
{
    if (IsCallerSystem()) return;
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    context->EnforceCallingPermission(
            Elastos::android::Manifest::permission::STATUS_BAR_SERVICE, message);
}

void NotificationManagerService::NotificationManagerStub::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ArrayOf<String>* args)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    Int32 result;
    context->CheckCallingOrSelfPermission(
            Elastos::android::Manifest::permission::DUMP, &result);
    if (result != IPackageManager::PERMISSION_GRANTED) {
        StringBuilder builder;
        builder += "Permission Denial: can't dump NotificationManager from pid=";
        builder += Binder::GetCallingPid();
        builder += ", uid=";
        builder += Binder::GetCallingUid();

        pw->Println(builder.ToString());
        return;
    }

    DumpImpl(pw, DumpFilter::ParseFromArguments(args));
}

ECode NotificationManagerService::NotificationManagerStub::GetEffectsSuppressor(
    /* [out] */ IComponentName** name)
{
    VALIDATE_NOT_NULL(name);

    EnforceSystemOrSystemUI(String("INotificationManager.getEffectsSuppressor"));
    *name = mHost->mEffectsSuppressor;
    return NOERROR;
}

ECode NotificationManagerService::NotificationManagerStub::MatchesCallFilter(
    /* [in] */ IBundle* extras,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    EnforceSystemOrSystemUI(String("INotificationManager.matchesCallFilter"));
    return mHost->mZenModeHelper->MatchesCallFilter(
            UserHandle::GetCallingUserHandle(),
            extras,
            mHost->mRankingHelper->FindExtractor(ValidateNotificationPeople.class),
            MATCHES_CALL_FILTER_CONTACTS_TIMEOUT_MS,
            MATCHES_CALL_FILTER_TIMEOUT_AFFINITY, result);
}

ECode NotificationManagerService::NotificationManagerStub::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = "NotificationManagerService::NotificationManagerStub: ";
    str.AppendFormat("%p", this);
    return NOERROR;
}

//===============================================================================
//                  NotificationManagerService::NotificationListeners
//===============================================================================
NotificationManagerService::NotificationListeners::NotificationListeners(
    /* [in] */ NotificationManagerService* host)
    : mHost(host)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    ManagedServices::ManagedServices(context, mHost->mHandler, mHost->mNotificationList, mHost->mUserProfiles);
    CArraySet::New((IArraySet**)&mLightTrimListeners);
}

NotificationManagerService::NotificationListeners::~NotificationListeners()
{}

AutoPtr<Config> NotificationManagerService::NotificationListeners::GetConfig()
{
    AutoPtr<Config> c = new Config();
    c->mCaption = "notification listener";
    c->mServiceInterface = INotificationListenerService::SERVICE_INTERFACE;
    c->mSecureSettingName = ISettingsSecure::ENABLED_NOTIFICATION_LISTENERS;
    c->mBindPermission = Elastos::android::Manifest::permission::BIND_NOTIFICATION_LISTENER_SERVICE;
    c->mSettingsAction = ISettings::ACTION_NOTIFICATION_LISTENER_SETTINGS;
    c->mClientLabel = R::string::notification_listener_binding_label;
    return c;
}

ECode NotificationManagerService::NotificationListeners::OnServiceAdded(
/* [in] */ ManagedServiceInfo* info)
{
    AutoPtr<IINotificationListener> listener = IINotificationListener::Probe(info->mService);
    AutoPtr<INotificationRankingUpdate> update;
    synchronized(mNotificationList) {
        update = mHost->MakeRankingUpdateLocked(info);
    }
    // try {
    ECode ec = listener->OnListenerConnected(update);
    if (FAILED(ec)) {
        // we tried
    }
    // } catch (RemoteException e) {
    //     // we tried
    // }
    return NOERROR;
}

ECode NotificationManagerService::NotificationListeners::OnServiceRemovedLocked(
/* [in] */ ManagedServiceInfo* info)
{
    Boolean res;
    ISet::Probe(mHost->mListenersDisablingEffects)->Remove(removed, &res);
    if (res) {
        mHost->UpdateListenerHintsLocked();
    }
    ISet::Probe(mLightTrimListeners)->Remove(removed);
    return NOERROR;
}

void NotificationManagerService::NotificationListeners::SetOnNotificationPostedTrimLocked(
    /* [in] */ ManagedServiceInfo* info,
    /* [in] */ Int32 trim)
{
    if (trim == TRIM_LIGHT) {
        mLightTrimListeners->Add(TO_IINTERFACE(info));
    }
    else {
        mLightTrimListeners->Remove(TO_IINTERFACE(info));
    }
}

Int32 NotificationManagerService::NotificationListeners::GetOnNotificationPostedTrim(
    /* [in] */ ManagedServiceInfo* info)
{
    Boolean res;
    return (mLightTrimListeners->Contains(info, &res), res) ? TRIM_LIGHT : TRIM_FULL;
}

ECode NotificationManagerService::NotificationListeners::NotifyPostedLocked(
    /* [in] */ IStatusBarNotification* sbn,
    /* [in] */ IStatusBarNotification* oldSbn)
{
    // Lazily initialized snapshots of the notification.
    AutoPtr<IStatusBarNotification> sbnClone;
    AutoPtr<IStatusBarNotification> sbnCloneLight;

    Int32 size;
    mServices->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> obj;
        mServices->Get(i, (IInterface**)&obj);
        AutoPtr<ManagedServiceInfo> info = (ManagedServiceInfo*)IObject::Probe(obj);

        Boolean sbnVisible = mHost->IsVisibleToListener(sbn, info);
        Boolean oldSbnVisible = oldSbn != NULL ? mHost->IsVisibleToListener(oldSbn, info) : FALSE;
        // This notification hasn't been and still isn't visible -> ignore.
        if (!oldSbnVisible && !sbnVisible) {
            continue;
        }
        AutoPtr<INotificationRankingUpdate> update = mHost->MakeRankingUpdateLocked(info);

        AutoPtr<NotificationListenersRunnable> runnable;
        Boolean res;
        // This notification became invisible -> remove the old one.
        if (oldSbnVisible && !sbnVisible) {
            AutoPtr<IStatusBarNotification> oldSbnLightClone;
            oldSbn->CloneLight((IStatusBarNotification**)&oldSbnLightClone);
            runnable = new NotificationListenersRunnable(0,
                    this, mHost, info, oldSbnLightClone, update, 0);
            mHost->mHandler->Post(runnable, &res);
            continue;
        }

        Int32 trim = mHost->mListeners->GetOnNotificationPostedTrim(info);

        if (trim == TRIM_LIGHT && sbnCloneLight == NULL) {
            sbn->CloneLight((IStatusBarNotification**)&sbnCloneLight);
        }
        else if (trim == TRIM_FULL && sbnClone == NULL) {
            AutoPtr<IInterface> obj;
            sbn->Clone((IInterface**)&obj);
            sbnClone = IStatusBarNotification::Probe(obj);
        }
        AutoPtr<IStatusBarNotification> sbnToPost =
                (trim == TRIM_FULL) ? sbnClone : sbnCloneLight;

        runnable = NULL;
        runnable = new NotificationListenersRunnable(1, this, mHost, info, sbnToPost, update, 0);
        mHost->mHandler->Post(runnable, &res);
    }
    return NOERROR;
}

ECode NotificationManagerService::NotificationListeners::NotifyRemovedLocked(
    /* [in] */ IStatusBarNotification* sbn)
{
    // make a copy in case changes are made to the underlying Notification object
    // NOTE: this copy is lightweight: it doesn't include heavyweight parts of the
    // notification
    AutoPtr<IStatusBarNotification> sbnLight;
    sbn->CloneLight((IStatusBarNotification**)&sbnLight);

    Int32 size;
    mServices->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> obj;
        mServices->Get(i, (IInterface**)&obj);
        AutoPtr<ManagedServiceInfo> info = (ManagedServiceInfo*)IObject::Probe(obj);

        if (!mHost->IsVisibleToListener(sbn, info)) {
            continue;
        }
        AutoPtr<INotificationRankingUpdate> update = mHost->MakeRankingUpdateLocked(info);
        AutoPtr<NotificationListenersRunnable> runnable = new NotificationListenersRunnable(
                2, this, mHost, info, sbnLight, update, 0);
        Boolean res;
        mHost->mHandler->Post(runnable, &res);
    }
    return NOERROR;
}

ECode NotificationManagerService::NotificationListeners::NotifyRankingUpdateLocked()
{
    Int32 size;
    mServices->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> obj;
        mServices->Get(i, (IInterface**)&obj);
        AutoPtr<ManagedServiceInfo> serviceInfo = (ManagedServiceInfo*)IObject::Probe(obj);

        if (!serviceInfo->IsEnabledForCurrentProfiles()) {
            continue;
        }

        AutoPtr<INotificationRankingUpdate> update = mHost->MakeRankingUpdateLocked(serviceInfo);
        AutoPtr<NotificationListenersRunnable> runnable = new NotificationListenersRunnable(
                3, this, mHost, serviceInfo, NULL, update, 0);
        Boolean res;
        mHost->mHandler->Post(runnable, &res);
    }
    return NOERROR;
}

ECode NotificationManagerService::NotificationListeners::NotifyListenerHintsChangedLocked(
    /* [in] */ Int32 hints)
{
    Int32 size;
    mServices->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> obj;
        mServices->Get(i, (IInterface**)&obj);
        AutoPtr<ManagedServiceInfo> serviceInfo = (ManagedServiceInfo*)IObject::Probe(obj);

        if (!serviceInfo->IsEnabledForCurrentProfiles()) {
            continue;
        }

        AutoPtr<NotificationListenersRunnable> runnable = new NotificationListenersRunnable(
                4, this, mHost, serviceInfo, NULL, NULL, hints);
        Boolean res;
        mHost->mHandler->Post(runnable, &res);
    }
    return NOERROR;
}

ECode NotificationManagerService::NotificationListeners::NotifyInterruptionFilterChanged(
    /* [in] */ Int32 interruptionFilter)
{
    Int32 size;
    mServices->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> obj;
        mServices->Get(i, (IInterface**)&obj);
        AutoPtr<ManagedServiceInfo> serviceInfo = (ManagedServiceInfo*)IObject::Probe(obj);

        if (!serviceInfo->IsEnabledForCurrentProfiles()) {
            continue;
        }

        AutoPtr<NotificationListenersRunnable> runnable = new NotificationListenersRunnable(
                5, this, mHost, serviceInfo, NULL, NULL, interruptionFilter);
        Boolean res;
        mHost->mHandler->Post(runnable, &res);
    }
    return NOERROR;
}

ECode NotificationManagerService::NotificationListeners::NotifyPosted(
    /* [in] */ ManagedServiceInfo* info,
    /* [in] */ IStatusBarNotification* sbn,
    /* [in] */ INotificationRankingUpdate* rankingUpdate)
{
    AutoPtr<IINotificationListener> listener = IINotificationListener::Probe(info->mService);
    AutoPtr<IIStatusBarNotificationHolder> sbnHolder;
    CStatusBarNotificationHolder::New(sbn, (IIStatusBarNotificationHolder**)&sbnHolder);
    // try {
    ECode ec = listener->OnNotificationPosted(sbnHolder, rankingUpdate);
    if (FAILED(ec)) {
        Logger::E(TAG, "unable to notify listener (posted): %p", listener.Get());
    }
    // } catch (RemoteException ex) {
    //     Log.e(TAG, "unable to notify listener (posted): " + listener, ex);
    // }
    return NOERROR;
}

ECode NotificationManagerService::NotificationListeners::NotifyRemoved(
    /* [in] */ ManagedServiceInfo* info,
    /* [in] */ IStatusBarNotification* sbn,
    /* [in] */ INotificationRankingUpdate* rankingUpdate)
{
    Int32 id;
    sbn->GetUserId(&id);
    if (!info->EnabledAndUserMatches(id)) {
        return NOERROR;
    }

    AutoPtr<IINotificationListener> listener = IINotificationListener::Probe(info->mService);
    AutoPtr<IIStatusBarNotificationHolder> sbnHolder;
    CStatusBarNotificationHolder::New(sbn, (IIStatusBarNotificationHolder**)&sbnHolder);
    // try {
    ECode ec = listener->OnNotificationRemoved(sbnHolder, rankingUpdate);
    if (FAILED(ec)) {
        Logger::E(TAG, "unable to notify listener (removed): %p", listener.Get());
    }
    // } catch (RemoteException ex) {
    //     Log.e(TAG, "unable to notify listener (removed): " + listener, ex);
    // }
    return NOERROR;
}

ECode NotificationManagerService::NotificationListeners::NotifyRankingUpdate(
    /* [in] */ ManagedServiceInfo* info,
    /* [in] */ INotificationRankingUpdate* rankingUpdate)
{
    AutoPtr<IINotificationListener> listener = IINotificationListener::Probe(info->mService);
    // try {
    ECode ec = listener->OnNotificationRankingUpdate(rankingUpdate);
    if (FAILED(ec)) {
        Logger::E(TAG, "unable to notify listener (ranking update): %p", listener.Get());
    }
    // } catch (RemoteException ex) {
    //     Log.e(TAG, "unable to notify listener (ranking update): " + listener, ex);
    // }
    return NOERROR;
}

ECode NotificationManagerService::NotificationListeners::NotifyListenerHintsChanged(
    /* [in] */ ManagedServiceInfo* info,
    /* [in] */ Int32 hints)
{
    AutoPtr<IINotificationListener> listener = IINotificationListener::Probe(info->mService);
    // try {
    ECode ec = listener->OnListenerHintsChanged(hints);
    if (FAILED(ec)) {
        Logger::E(TAG, "unable to notify listener (listener hints): %p", listener.Get());
    }
    // } catch (RemoteException ex) {
    //     Log.e(TAG, "unable to notify listener (listener hints): " + listener, ex);
    // }
    return NOERROR;
}

ECode NotificationManagerService::NotificationListeners::NotifyInterruptionFilterChanged(
    /* [in] */ ManagedServiceInfo* info,
    /* [in] */ Int32 interruptionFilter)
{
    AutoPtr<IINotificationListener> listener = IINotificationListener::Probe(info->mService);
    // try {
    ECode ec = listener->OnInterruptionFilterChanged(interruptionFilter);
    if (FAILED(ec)) {
        Logger::E(TAG, "unable to notify listener (interruption filter): %p", listener.Get());
    }
    // } catch (RemoteException ex) {
    //     Log.e(TAG, "unable to notify listener (interruption filter): " + listener, ex);
    // }
    return NOERROR;
}

Boolean NotificationManagerService::NotificationListeners::IsListenerPackage(
    /* [in] */ const String& packageName)
{
    if (packageName.IsNull()) {
        return FALSE;
    }
    // TODO: clean up locking object later
    synchronized(mNotificationList) {
        Int32 size;
        mServices->GetSize(&size);
        for (Int32 i = 0; i < size; i++) {
            AutoPtr<IInterface> obj;
            mServices->Get(i, (IInterface**)&obj);
            AutoPtr<ManagedServiceInfo> serviceInfo = (ManagedServiceInfo*)IObject::Probe(obj);
            String name;
            serviceInfo->mComponent->GetPackageName(&name);
            if (packageName.Equals(name)) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

//===============================================================================
//                  NotificationManagerService::DumpFilter
//===============================================================================

NotificationManagerService::DumpFilter::DumpFilter()
{}

NotificationManagerService::DumpFilter::~DumpFilter();

AutoPtr<DumpFilter> NotificationManagerService::DumpFilter::ParseFromArguments(
    /* [in] */ ArrayOf<String>* args)
{
    if (args != NULL && args->GetLength() == 2 && String("p").Equals((*args)[0])
            && !(*args)[1].IsNull() && !(*args)[1].Trim().IsEmpty()) {
        AutoPtr<DumpFilter> filter = new DumpFilter();
        filter->mPkgFilter = (*args)[1].Trim().ToLowerCase();
        return filter;
    }
    if (args != NULL && args->GetLength() == 1 && String("zen").Equals((*args)[0])) {
        AutoPtr<DumpFilter> filter = new DumpFilter();
        filter->mZen = TRUE;
        return filter;
    }
    return NULL;
}

Boolean NotificationManagerService::DumpFilter::Matches(
    /* [in] */ IStatusBarNotification* sbn)
{
    String name, pkg;
    sbn->GetPackageName(&name);
    sbn->GetOpPkg(&pkg);
    return mZen ? TRUE : sbn != NULL && (Matches(name) || Matches(pkg));
}

Boolean NotificationManagerService::DumpFilter::Matches(
    /* [in] */ IComponentName* component)
{
    String name;
    component->GetPackageName(&name);
    return mZen ? TRUE : component != NULL && Matches(name);
}

Boolean NotificationManagerService::DumpFilter::Matches(
    /* [in] */ const String& pkg)
{
    return mZen ? TRUE : !pkg.IsNull() && pkg.ToLowerCase().Contains(mPkgFilter);
}

ECode NotificationManagerService::DumpFilter::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    if (mZen) {
        *str = "zen";
    }
    else {
        StringBuilder builder;
        builder += '\'';
        builder += mPkgFilter;
        builder += '\'';
        *str = builder.ToString();
    }
    return NOERROR;
}

//===============================================================================
//                  NotificationManagerService::StatusBarNotificationHolder
//===============================================================================

CAR_INTERFACE_IMPL_2(NotificationManagerService::StatusBarNotificationHolder, Object, IIStatusBarNotificationHolder, IBinder);

NotificationManagerService::StatusBarNotificationHolder::StatusBarNotificationHolder()
{}

NotificationManagerService::StatusBarNotificationHolder::~StatusBarNotificationHolder()
{}

ECode NotificationManagerService::StatusBarNotificationHolder::constructor(
    /* [in] */ IStatusBarNotification* value)
{
    mValue = value;
    return NOERROR;
}

ECode NotificationManagerService::StatusBarNotificationHolder::Get(
    /* [out] */ IStatusBarNotification** ret)
{
    VALIDATE_NOT_NULL(ret);

    AutoPtr<IStatusBarNotification> value = mValue;
    mValue = NULL;
    *ret = value;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode NotificationManagerService::StatusBarNotificationHolder::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = "NotificationManagerService::StatusBarNotificationHolder: ";
    str.AppendFormat("%p", this);
    return NOERROR;
}

//===============================================================================
//                  NotificationManagerService::SettingsObserver
//===============================================================================

NotificationManagerService::SettingsObserver::SettingsObserver(
    /* [in] */ NotificationManagerService* host,
    /* [in] */ IHandler* handler)
    : ContentObserver(handler)
    , mHost(host)
{
    AutoPtr<ISettingsSystem> systemSettings;
    CSettingsSystem::AcquireSingleton((ISettingsSystem**)&systemSettings);
    systemSettings->GetUriFor(ISettingsSystem::NOTIFICATION_LIGHT_PULSE,
            (IUri**)&NOTIFICATION_LIGHT_PULSE_URI);
}

NotificationManagerService::SettingsObserver::~SettingsObserver()
{}

void NotificationManagerService::SettingsObserver::Observe()
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    resolver->RegisterContentObserver(NOTIFICATION_LIGHT_PULSE_URI, FALSE,
            this, IUserHandle::USER_ALL);

    Update(NULL);
}

ECode NotificationManagerService::SettingsObserver::OnChange(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    Update(uri);
    return NOERROR;
}

void NotificationManagerService::SettingsObserver::Update(
    /* [in] */ IUri* uri)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);

    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);

    Boolean res;
    if (uri == NULL || (IObject::Probe(NOTIFICATION_LIGHT_PULSE_URI)->Equals(uri, &res), res)) {
        AutoPtr<ISettingsSystem> systemSettings;
        CSettingsSystem::AcquireSingleton((ISettingsSystem**)&systemSettings);

        Int32 value;
        systemSettings->GetInt32(resolver, ISettingsSystem::NOTIFICATION_LIGHT_PULSE, 0, &value);

        Boolean pulseEnabled = (value != 0);
        if (mHost->mNotificationPulseEnabled != pulseEnabled) {
            mHost->mNotificationPulseEnabled = pulseEnabled;
            mHost->UpdateNotificationPulse();
        }
    }
}

//===============================================================================
//                  NotificationManagerService::IteratorInner
//===============================================================================

CAR_INTERFACE_IMPL(NotificationManagerService::IteratorInner, Object, IIterator);

NotificationManagerService::IteratorInner::IteratorInner(
    /* [in] */ IIterator* iter,
    /* [in] */ const String& pkg,
    /* [in] */ Int32 userId)
    : mIter(iter)
    , mPkg(pkg)
    , mUserId(userId)
{
    mNext = FindNext();
}

NotificationManagerService::IteratorInner::~IteratorInner()
{}

AutoPtr<IStatusBarNotification> NotificationManagerService::IteratorInner::FindNext()
{
    Boolean res;
    while (mIter->HasNext(&res), res) {
        AutoPtr<IInterface> obj;
        mIter->GetNext((IInterface**)&obj);
        AutoPtr<IStatusBarNotification> nr = IStatusBarNotification::Probe(obj);
        String name;
        Int32 id;
        if ((mPkg.IsNull() || (nr->GetPackageName(&name), name) == pkg)
                && (mUserId == IUserHandle::USER_ALL || (nr->GetUserId(&id), id) == mUserId)) {
            return nr;
        }
    }
    return NULL;
}

ECode NotificationManagerService::IteratorInner::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mNext == NULL;
    return NOERROR;
}

ECode NotificationManagerService::IteratorInner::GetNext(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    *object = NULL;

    AutoPtr<IStatusBarNotification> next = mNext;
    if (next == NULL) {
        return E_NO_SUCH_ELEMENT_EXCEPTION;
        // throw new NoSuchElementException();
    }
    mNext = FindNext();
    *object = next;
    REFCOUNT_ADD(*object);
    return NOERROR;
}

ECode NotificationManagerService::IteratorInner::Remove();
{
    return mIter->Remove();
}

//===============================================================================
//                  NotificationManagerService::Archive
//===============================================================================

NotificationManagerService::Archive::Archive(
    /* [in] */ Int32 size)
    : mBufferSize(size)
{
    CArrayDeque::New(mBufferSize, (IArrayDeque**)&mBuffer);
}

NotificationManagerService::Archive::~Archive()
{}

ECode NotificationManagerService::Archive::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    StringBuilder sb;
    Int32 N;
    mBuffer->GetSize(&N);
    sb.Append("Archive (");
    sb.Append(N);
    sb.Append(" notification");
    sb.Append((N==1)?")":"s)");
    *str = sb.ToString();
    return NOERROR;
}

ECode NotificationManagerService::Archive::Record(
    /* [in] */ IStatusBarNotification* nr)
{
    Int32 size;
    mBuffer->GetSize(&size);
    if (size == mBufferSize) {
        AutoPtr<IInterface> obj;
        IDeque::Probe(mBuffer)->RemoveFirst((IInterface**)&obj);
    }

    // We don't want to store the heavy bits of the notification in the archive,
    // but other clients in the system process might be using the object, so we
    // store a (lightened) copy.
    AutoPtr<IStatusBarNotification> clone;
    nr->CloneLight((IStatusBarNotification**)&clone);
    IDeque::Probe(mBuffer)->AddLast(clone);
    return NOERROR;
}

ECode NotificationManagerService::Archive::Clear()
{
    mBuffer->Clear();
    return NOERROR;
}

ECode NotificationManagerService::Archive::DescendingIterator(
    /* [out] */ IIterator** iterator)
{
    VALIDATE_NOT_NULL(iterator);
    return IDeque::Probe(mBuffer)->GetDescendingIterator(iterator);
}

ECode NotificationManagerService::Archive::AscendingIterator(
    /* [out] */ IIterator** iterator)
{
    VALIDATE_NOT_NULL(iterator);
    return mBuffer->GetIterator(iterator);
}

ECode NotificationManagerService::Archive::Filter(
    /* [in] */ IIterator* iter,
    /* [in] */ const String& pkg,
    /* [in] */ Int32 userId,
    /* [out] */IIterator** iterator)
{
    VALIDATE_NOT_NULL(iterator);
    AutoPtr<IteratorInner> objIter = new IteratorInner(iter, pkg, userId);
    AutoPtr<IIterator> obj = (IIterator*)objIter.Get();
    *iterator = obj;
    REFCOUNT_ADD(&iterator);
    return NOERROR;
}

ECode NotificationManagerService::Archive::GetArray(
    /* [in] */ Int32 count,
    /* [out, callee] */ ArrayOf<IStatusBarNotification*>** ation)
{
    VALIDATE_NOT_NULL(ation);
    if (count == 0) {
        count = mBufferSize;
    }

    Int32 size;
    mBuffer->GetSize(&size);
    using Elastos::Core::Math;
    AutoPtr< ArrayOf<IStatusBarNotification*> > a = ArrayOf<IStatusBarNotification*>::Alloc(Math::Min(count, size));
    AutoPtr<IIterator> iter;
    DescendingIterator((IIterator**)&iter);

    Int32 i = 0;
    Boolean res;
    while ((iter->HasNext(&res) ,res) && i < count) {
        AutoPtr<IInterface> obj;
        iter->GetNext((IInterface**)&obj);
        AutoPtr<IStatusBarNotification> notification = IStatusBarNotification::Probe(obj);
        (*a)[i++] = notification;
    }
    *action = a;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

ECode NotificationManagerService::Archive::GetArray(
    /* [in] */ Int32 count,
    /* [in] */ const String& pkg,
    /* [in] */ Int32 userId,
    /* [out, callee] */ ArrayOf<IStatusBarNotification*>** ation)
{
    VALIDATE_NOT_NULL(ation);
    if (count == 0) {
        count = mBufferSize;
    }

    Int32 size;
    mBuffer->GetSize(&size);
    using Elastos::Core::Math;
    AutoPtr< ArrayOf<IStatusBarNotification*> > a = ArrayOf<IStatusBarNotification*>::Alloc(Math::Min(count, size));
    AutoPtr<IIterator> iterOther;
    DescendingIterator((IIterator**)&iterOther);
    AutoPtr<IIterator> iter;
    Filter(iterOther, pkg, userId, (IIterator**)&iter);

    Int32 i = 0;
    Boolean res;
    while ((iter->HasNext(&res) ,res) && i < count) {
        AutoPtr<IInterface> obj;
        iter->GetNext((IInterface**)&obj);
        AutoPtr<IStatusBarNotification> notification = IStatusBarNotification::Probe(obj);
        (*a)[i++] = notification;
    }
    *action = a;
    REFCOUNT_ADD(*action);
    return NOERROR;
}

//===============================================================================
//                  NotificationManagerService::ToastRecord
//===============================================================================

NotificationManagerService::ToastRecord::ToastRecord(
    /* [in] */ Int32 pid,
    /* [in] */ const String& pkg,
    /* [in] */ IITransientNotification* callback,
    /* [in] */ Int32 duration)
    : mPid(pid)
    , mPkg(pkg)
    , mCallback(callback)
    , mDuration(duration)
{}

NotificationManagerService::ToastRecord::~ToastRecord()
{}

void NotificationManagerService::ToastRecord::Update(
    /* [in] */ Int32 duration)
{
    mDuration = duration;
}

void NotificationManagerService::ToastRecord::Dump(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& prefix,
    /* [in] */ DumpFilter* filter)
{
    if (filter != NULL && !filter->Matches(pkg)) return;
    pw->Println(prefix + this);
}

ECode NotificationManagerService::ToastRecord::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    StringBuilder builder;
    builder += "ToastRecord{";

    AutoPtr<ISystem> sys;
    CSystem::AcquireSingleton((ISystem**)&sys);
    Int32 code;
    sys->IdentityHashCode((IInterface*)this, &code);

    builder += StringUtils::ToHexString(code);
    builder += " pkg=";
    builder += pkg;
    builder += " callback=";
    builder += callback;
    builder += " duration=";
    builder += duration;
    *str = builder.ToString();
    return NOERROR;
}

//===============================================================================
//                  NotificationManagerService::MyNotificationDelegate
//===============================================================================

CAR_INTERFACE_IMPL(NotificationManagerService::MyNotificationDelegate, Object, INotificationDelegate);

NotificationManagerService::MyNotificationDelegate::MyNotificationDelegate(
    /* [in] */ NotificationManagerService* host)
    : mHost(host)
{}

NotificationManagerService::MyNotificationDelegate::~MyNotificationDelegate()
{}

ECode NotificationManagerService::MyNotificationDelegate::OnSetDisabled(
    /* [in] */ Int32 status)
{
    synchronized(mNotificationList) {
        mHost->mDisableNotificationEffects =
                (status & IStatusBarManager::DISABLE_NOTIFICATION_ALERTS) != 0;
        if (!DisableNotificationEffects(NULL).IsNull()) {
            // cancel whatever's going on
            Int64 identity;
            Binder::ClearCallingIdentity(&identity);

            if (mHost->mAudioService != NULL) {
                AutoPtr<IIRingtonePlayer> player;
                mHost->mAudioManager->GetRingtonePlayer((IIRingtonePlayer**)&player);

                if (player != NULL) {
                    player->StopAsync();
                }
                Binder::RestoreCallingIdentity(identity);
            }

            Binder::ClearCallingIdentity(&identity);
            if (mHost->mVibrator != NULL) {
                mHost->mVibrator->Cancel();
                Binder::RestoreCallingIdentity(identity);
            }
        }
    }
    return NOERROR;
}

ECode NotificationManagerService::MyNotificationDelegate::OnClearAll(
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 userId)
{
    synchronized(mNotificationList) {
        mHost->CancelAllLocked(callingUid, callingPid, userId, REASON_DELEGATE_CANCEL_ALL, NULL,
                /*includeCurrentProfiles*/ TRUE);
    }
    return NOERROR;
}

ECode NotificationManagerService::MyNotificationDelegate::OnNotificationClick(
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 callingPid,
    /* [in] */ const String& key)
{
    synchronized(mNotificationList) {
        assert(0 && "TODO");
        // EventLogTags.writeNotificationClicked(key);
        AutoPtr<IInterface> obj;
        mHost->mNotificationsByKey->Get(CoreUtils::Convert(key), (IInterface**)&obj);
        AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);
        if (r == NULL) {
            Logger::W(TAG, "No notification with key: %s", key.string());
            return NOERROR;
        }
        AutoPtr<IStatusBarNotification> sbn = r->mSbn;
        String pkg, tag;
        Int32 id, userId;
        mHost->CancelNotification(callingUid, callingPid,
                (sbn->GetPackageName(&pkg), pkg),
                (sbn->GetTag(&tag), tag),
                (sbn->GetId(&id), id),
                INotification::FLAG_AUTO_CANCEL,
                INotification::FLAG_FOREGROUND_SERVICE, FALSE,
                r->GetUserId(),
                REASON_DELEGATE_CLICK, NULL);
    }
    return NOERROR;
}

ECode NotificationManagerService::MyNotificationDelegate::OnNotificationClear(
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 callingPid,
    /* [in] */ const String& pkg,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ Int32 userId)
{
    return mHost->CancelNotification(callingUid, callingPid, pkg, tag, id, 0,
            INotification::FLAG_ONGOING_EVENT | INotification::FLAG_FOREGROUND_SERVICE,
            TRUE, userId, REASON_DELEGATE_CANCEL, NULL);
}

ECode NotificationManagerService::MyNotificationDelegate::OnPanelRevealed()
{
    assert(0 && "TODO");
    // EventLogTags.writeNotificationPanelRevealed();
    synchronized (mNotificationList) {
        // sound
        mHost->mSoundNotification = NULL;

        Int64 identity;
        Binder::ClearCallingIdentity(&identity);
        if (mHost->mAudioService != NULL) {
            AutoPtr<IIRingtonePlayer> player;
            mHost->mAudioManager->GetRingtonePlayer((IIRingtonePlayer**)&player);

            if (player != NULL) {
                player->StopAsync();
            }
            Binder::RestoreCallingIdentity(identity);
        }

        // vibrate
        mHost->mVibrateNotification = NULL;
        Binder::ClearCallingIdentity(&identity);
        if (mHost->mVibrator != NULL) {
            mHost->mVibrator->Cancel();
            Binder::RestoreCallingIdentity(identity);
        }

        // light
        mHost->mLights.Clear();
        mHost->mLedNotification = NULL;
        mHost->UpdateLightsLocked();
    }
    return NOERROR;
}

ECode NotificationManagerService::MyNotificationDelegate::OnPanelHidden()
{
    assert(0 && "TODO");
    // EventLogTags.writeNotificationPanelHidden();
    return NOERROR;
}

ECode NotificationManagerService::MyNotificationDelegate::OnNotificationError(
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 callingPid,
    /* [in] */ const String& pkg,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 initialPid,
    /* [in] */ const String& message,
    /* [in] */ Int32 userId)
{
    StringBuilder buf;
    buf += "onNotification error pkg=";
    buf += pkg;
    buf += " tag=";
    buf += tag;
    buf += " id=";
    buf += id;
    buf += "; will crashApplication(uid=";
    buf += uid;
    buf += ", pid=";
    buf += initialPid;
    buf += ")";
    Logger::D(TAG, buf.ToString().string());

    // XXX to be totally correct, the caller should tell us which user
    // this is for.
    Int32 userId;
    UserHandle::GetUserId(uid, &userId);
    mHost->CancelNotification(callingUid, callingPid, pkg, tag, id, 0, 0, FALSE,
            userId, REASON_DELEGATE_ERROR, NULL);

    Int64 ident;
    Binder::ClearCallingIdentity(&ident);

    AutoPtr<IIActivityManager> activityMgr;
    mHost->mContext->GetSystemService(IContext::ACTIVITY_SERVICE, (IInterface**)&activityMgr);
    assert(activityMgr != NULL);

    StringBuilder msg;
    msg += "Bad notification posted from package ";
    msg += pkg;
    msg += ": ";
    msg += message;

    activityMgr->CrashApplication(uid, initialPid, pkg, msg.ToString());
    Binder::RestoreCallingIdentity(ident);
    return NOERROR;
}

ECode NotificationManagerService::MyNotificationDelegate::OnNotificationVisibilityChanged(
    /* [in] */ ArrayOf<String>* newlyVisibleKeys,
    /* [in] */ ArrayOf<String>* noLongerVisibleKeys)
{
    // Using ';' as separator since eventlogs uses ',' to separate
    // args.
    assert(0 && "TODO");
    // EventLogTags.writeNotificationVisibilityChanged(
    //         TextUtils.join(";", newlyVisibleKeys),
    //         TextUtils.join(";", noLongerVisibleKeys));
    synchronized(mNotificationList) {
        for (Int32 i = 0; i < newlyVisibleKeys->GetLength(); i++) {
            String key = (*newlyVisibleKeys)[i];
            AutoPtr<IInterface> obj;
            mHost->mNotificationsByKey->Get(CoreUtils::Convert(key), (IInterface**)&obj);
            AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);
            if (r == NULL) continue;
            r->mStats->OnVisibilityChanged(TRUE);
        }
        // Note that we might receive this event after notifications
        // have already left the system, e.g. after dismissing from the
        // shade. Hence not finding notifications in
        // mNotificationsByKey is not an exceptional condition.
        for (Int32 i = 0; i < noLongerVisibleKeys->GetLength(); i++) {
            String key = (*noLongerVisibleKeys)[i];
            AutoPtr<IInterface> obj;
            mHost->mNotificationsByKey->Get(CoreUtils::Convert(key), (IInterface**)&obj);
            AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);
            if (r == NULL) continue;
            r->mStats->OnVisibilityChanged(FALSE);
        }
    }
    return NOERROR;
}

ECode NotificationManagerService::MyNotificationDelegate::OnNotificationExpansionChanged(
    /* [in] */ const String& key,
    /* [in] */ Boolean userAction,
    /* [in] */ Boolean expanded);
{
    assert(0 && "TODO");
    // EventLogTags.writeNotificationExpansion(key, userAction ? 1 : 0, expanded ? 1 : 0);
    synchronized(mNotificationList) {
        AutoPtr<IInterface> obj;
        mHost->mNotificationsByKey->Get(CoreUtils::Convert(key), (IInterface**)&obj);
        AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);
        if (r != NULL) {
            r->mStats->OnExpansionChanged(userAction, expanded);
        }
    }
    return NOERROR;
}

//===============================================================================
//                  NotificationManagerService::MyBroadcastReceiver
//===============================================================================
NotificationManagerService::MyBroadcastReceiver::MyBroadcastReceiver(
    /* [in] */ NotificationManagerService* host)
    : mHost(host)
{
}

NotificationManagerService::MyBroadcastReceiver::MyBroadcastReceiver()
{}

ECode NotificationManagerService::MyBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    Logger::D("NotificationManagerService::MyBroadcastReceiver", "OnReceive %s", action.string());

    Boolean queryRestart = FALSE;
    Boolean queryRemove = FALSE;
    Boolean packageChanged = FALSE;
    Boolean cancelNotifications = TRUE;

    if (action.Equals(IIntent::ACTION_PACKAGE_ADDED)
            || (queryRemove == action.Equals(IIntent::ACTION_PACKAGE_REMOVED))
            || action.Equals(IIntent::ACTION_PACKAGE_RESTARTED)
            || (packageChanged = action.Equals(IIntent::ACTION_PACKAGE_CHANGED))
            || (queryRestart = action.Equals(IIntent::ACTION_QUERY_PACKAGE_RESTART))
            || action.Equals(IIntent::ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE))
    {
        AutoPtr< ArrayOf<String> > pkgList;
        Boolean res;
        Boolean queryReplace = queryRemove &&
                (intent->GetBooleanExtra(IIntent::EXTRA_REPLACING, FALSE, &res), res);
        if (DBG) {
            Slogger::I(TAG, "action=%s queryReplace=%d", action.string(), queryReplace);
        }

        if (action.Equals(IIntent::ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE)) {
            intent->GetStringArrayExtra(IIntent::EXTRA_CHANGED_PACKAGE_LIST, (ArrayOf<String>**)&pkgList);
        }
        else if (queryRestart) {
            intent->GetStringArrayExtra(IIntent::EXTRA_PACKAGES, (ArrayOf<String>**)&pkgList);
        }
        else {
            AutoPtr<IUri> uri;
            intent->GetData((IUri**)&uri);
            if (uri == NULL) {
                return NOERROR;
            }

            String pkgName;
            uri->GetSchemeSpecificPart(&pkgName);
            if (pkgName.IsNull()) {
                return NOERROR;
            }

            if (packageChanged) {
                // We cancel notifications for packages which have just been disabled
                // try {
                AutoPtr<IContext> context;
                GetContext((IContext**)&context);
                AutoPtr<IPackageManager> pkgMgr;
                context->GetPackageManager((IPackageManager**)&pkgMgr);
                Int32 enabled;
                pkgMgr->GetApplicationEnabledSetting(pkgName, &enabled);

                if (enabled == IPackageManager::COMPONENT_ENABLED_STATE_ENABLED
                    || enabled == IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT) {
                    cancelNotifications = FALSE;
                }
                // } catch (IllegalArgumentException e) {
                //     // Package doesn't exist; probably racing with uninstall.
                //     // cancelNotifications is already true, so nothing to do here.
                //     if (DBG) {
                //         Slog.i(TAG, "Exception trying to look up app enabled setting", e);
                //     }
                // }
            }

            pkgList = ArrayOf<String>::Alloc(1);
            (*pkgList)[0] = pkgName;
        }

        if (pkgList != NULL && (pkgList->GetLength() > 0)) {
            for (Int32 i = 0; i < pkgList->GetLength(); ++i) {
                String pkgName = (*pkgList)[i];
                if (cancelNotifications) {
                    mHost->CancelAllNotificationsInt(MY_UID, MY_PID, pkgName, 0, 0, !queryRestart,
                            IUserHandle::USER_ALL, REASON_PACKAGE_CHANGED, NULL);
                }
            }
        }
        mHost->mListeners->OnPackagesChanged(queryReplace, pkgList);
        mHost->mConditionProviders->OnPackagesChanged(queryReplace, pkgList);
    }
    else if (action.Equals(IIntent::ACTION_SCREEN_ON)) {
        // Keep track of screen on/off state, but do not turn off the notification light
        // until user passes through the lock screen or views the notification.
        mHost->mScreenOn = TRUE;
    }
    else if (action.Equals(IIntent::ACTION_SCREEN_OFF)) {
        mHost->mScreenOn = FALSE;
    }
    else if (action.Equals(ITelephonyManager::ACTION_PHONE_STATE_CHANGED)) {
        String value;
        intent->GetStringExtra(ITelephonyManager::EXTRA_STATE, &value);
        mHost->mInCall = ITelephonyManager::EXTRA_STATE_OFFHOOK.Equals(value);
        mHost->UpdateNotificationPulse();
    }
    else if (action.Equals(IIntent::ACTION_USER_STOPPED)) {
        Int32 userHandle;
        intent->GetInt32Extra(IIntent::EXTRA_USER_HANDLE, -1, &userHandle);
        if (userHandle >= 0) {
            mHost->CancelAllNotificationsInt(MY_UID, MY_PID, String(NULL), 0, 0, TRUE,
                    userHandle, REASON_USER_STOPPED, NULL);
        }
    }
    else if (action.Equals(IIntent::ACTION_USER_PRESENT)) {
        // turn off LED when user passes through lock screen
        mHost->mNotificationLight->TurnOff();
        mHost->mStatusBar->NotificationLightOff();
    }
    else if (action.Equals(IIntent::ACTION_USER_SWITCHED)) {
        // reload per-user settings
        mHost->mSettingsObserver->Update(NULL);
        mHost->mUserProfiles->UpdateCache(context);
        // Refresh managed services
        mHost->mConditionProviders.onUserSwitched();
        mHost->mListeners->OnUserSwitched();
    }
    else if (action.Equals(IIntent::ACTION_USER_ADDED)) {
        mHost->mUserProfiles->UpdateCache(context);
    }

    return NOERROR;
}

//===============================================================================
//                  NotificationManagerService::MyRunnable
//===============================================================================

NotificationManagerService::MyRunnable::MyRunnable(
    /* [in] */ PowerManagerService* host)
    : mHost(host)
{}

NotificationManagerService::MyRunnable::~MyRunnable()
{}

ECode NotificationManagerService::MyRunnable::Run()
{
    return mHost->mStatusBar->BuzzBeepBlinked();
}

//===============================================================================
//                  NotificationManagerService::MyNotificationManagerInternal
//===============================================================================

CAR_INTERFACE_IMPL(NotificationManagerService::MyNotificationManagerInternal, Object, INotificationManagerInternal);

NotificationManagerService::MyNotificationManagerInternal::MyNotificationManagerInternal(
    /* [in] */ NotificationManagerService* host)
    : mHost(host)
{}

NotificationManagerService::MyNotificationManagerInternal::~MyNotificationManagerInternal()
{}

ECode NotificationManagerService::MyNotificationManagerInternal::EnqueueNotification(
    /* [in] */ const String& pkg,
    /* [in] */ const String& opPkg,
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 callingPid,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification,
    /* [in] */ ArrayOf<Int32>* idReceived,
    /* [in] */ Int32 userId)
{
    return mHost->EnqueueNotificationInternal(pkg, opPkg, callingUid, callingPid, tag, id, notification,
                    idReceived, userId);
}

ECode NotificationManagerService::MyNotificationManagerInternal::RemoveForegroundServiceFlagFromNotification(
    /* [in] */ const String& pkg,
    /* [in] */ Int32 notificationId,
    /* [in] */ Int32 userId)
{
    mHost->CheckCallerIsSystem();
    synchronized(mNotificationList) {
        Int32 i = mHost->IndexOfNotificationLocked(pkg, String(NULL), notificationId, userId);
        if (i < 0) {
            Logger::D(TAG, "stripForegroundServiceFlag: Could not find notification with pkg=%s / id=%d / userId=%d",
                    pkg.string(), notificationId, userId);
            return NOERROR;
        }
        AutoPtr<IInterface> obj;
        mHost->mNotificationList->Get(i, (IInterface**)&obj);
        AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);
        AutoPtr<IStatusBarNotification> sbn = r->mSbn;
        // NoMan adds flags FLAG_NO_CLEAR and FLAG_ONGOING_EVENT when it sees
        // FLAG_FOREGROUND_SERVICE. Hence it's not enough to remove FLAG_FOREGROUND_SERVICE,
        // we have to revert to the flags we received initially *and* force remove
        // FLAG_FOREGROUND_SERVICE.
        AutoPtr<INotification> notification;
        sbn->GetNotification((INotification**)&notification);
        notification->SetFlags(r->mOriginalFlags & ~INotification::FLAG_FOREGROUND_SERVICE);
        mHost->mRankingHelper->Sort(mNotificationList);
        mHost->mListeners->NotifyPostedLocked(sbn, sbn /* oldSbn */);
    }
    return NOERROR;
}

//===============================================================================
//                  NotificationManagerService::WorkerHandler
//===============================================================================

NotificationManagerService::WorkerHandler::WorkerHandler(
    /* [in] */ NotificationManagerService* host)
    : mHost(host)
{}

NotificationManagerService::WorkerHandler::~WorkerHandler()
{}

ECode NotificationManagerService::WorkerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case NotificationManagerService::MESSAGE_TIMEOUT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            NotificationManagerService::ToastRecord* record = (NotificationManagerService::ToastRecord*)obj.Get();
            mHost->HandleTimeout(record);
            break;
        }
        case MESSAGE_SAVE_POLICY_FILE:
            mHost->HandleSavePolicyFile();
            break;
        case MESSAGE_SEND_RANKING_UPDATE:
            mHost->HandleSendRankingUpdate();
            break;
        case MESSAGE_LISTENER_HINTS_CHANGED: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mHost->HandleListenerHintsChanged(arg1);
            break;
        }
        case MESSAGE_LISTENER_NOTIFICATION_FILTER_CHANGED:{
            Int32 arg1;
            msg->GetArg1(&arg1);
            mHost->HandleListenerInterruptionFilterChanged(arg1);
            break;
        }
    }
    return NOERROR;
}

//===============================================================================
//                  NotificationManagerService::WorkerHandler
//===============================================================================

NotificationManagerService::RankingWorkerHandler::RankingWorkerHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ NotificationManagerService* host)
    : Handler(looper)
    , mHost(host)
{}

NotificationManagerService::RankingWorkerHandler::~RankingWorkerHandler()
{}

ECode NotificationManagerService::RankingWorkerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case MESSAGE_RECONSIDER_RANKING:
            mHost->HandleRankingReconsideration(msg);
            break;
        case MESSAGE_RANKING_CONFIG_CHANGE:
            mHost->HandleRankingConfigChange();
            break;
    }
    return NOERROR;
}

//===============================================================================
//                  NotificationManagerService::NotificationListenersRunnable
//===============================================================================

NotificationManagerService::NotificationListenersRunnable::NotificationListenersRunnable(
    /* [in] */ Int32 id,
    /* [in] */ NotificationListeners* host,
    /* [in] */ PowerManagerService* mainHost,
    /* [in] */ ManagedServiceInfo* info,
    /* [in] */ IStatusBarNotification* sbn,
    /* [in] */ INotificationRankingUpdate* update,
    /* [in] */ Int32 data)
    : mId(id)
    , mHost(host)
    , mMainHost(mainHost)
    , mInfo(info)
    , mSbn(sbn)
    , mUpdate(update)
    , mData(data)
{}

NotificationManagerService::NotificationListenersRunnable::~NotificationListenersRunnable()
{}

ECode NotificationManagerService::NotificationListenersRunnable::Run()
{
    switch (mId) {
        case 0:
            mHost->NotifyRemoved(mInfo, mSbn, mUpdate);
            break;
        case 1:
            mHost->NotifyPosted(mInfo, mSbn, mUpdate);
            break;
        case 2:
            mHost->NotifyRemoved(mInfo, mSbn, mUpdate);
            break;
        case 3:
            mHost->NotifyRankingUpdate(mInfo, mUpdate);
            break;
        case 4:
            mHost->NotifyListenerHintsChanged(mInfo, mData);
            break;
        case 5:
            mHost->NotifyInterruptionFilterChanged(mInfo, mData);
            break;
    }
    return NOERROR;
}

//===============================================================================
//                  NotificationManagerService::EnqueueNotificationInternalRunnable
//===============================================================================

NotificationManagerService::EnqueueNotificationInternalRunnable::EnqueueNotificationInternalRunnable(
    /* [in] */ PowerManagerService* host,
    /* [in] */ const String& pkg,
    /* [in] */ const String& opPkg,
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 callingPid,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification,
    /* [in] */ IUserHandle* user,
    /* [in] */ Boolean isSystemNotification)
    : mHost(host)
    , mPkg(pkg)
    , mOpPkg(opPkg)
    , mCallingUid(callingUid)
    , mCallingPid(callingPid)
    , mTag(tag)
    , mId(id)
    , mNotification(notification)
    , mUser(user)
    , mIsSystemNotification(isSystemNotification)
{}

NotificationManagerService::EnqueueNotificationInternalRunnable::~EnqueueNotificationInternalRunnable()
{}

ECode NotificationManagerService::EnqueueNotificationInternalRunnable::Run()
{
    synchronized(mNotificationList) {

        // === Scoring ===

        // 0. Sanitize inputs

        Int32 priority;
        mNotification->GetPriority(&priority);
        mNotification->SetPriority(Clamp(priority, INotification::PRIORITY_MIN, INotification::PRIORITY_MAX));
        // Migrate notification flags to scores
        Int32 flags;
        mNotification->GetFlags(&flags);

        if (0 != (flags & INotification::FLAG_HIGH_PRIORITY)) {
            Int32 tmpPriority;
            mNotification->GetPriority(&tmpPriority);

            if (tmpPriority < INotification::PRIORITY_MAX) {
                mNotification->SetPriority(INotification::PRIORITY_MAX);
            }
        }
        else if (SCORE_ONGOING_HIGHER &&
                0 != (flags & INotification::FLAG_ONGOING_EVENT)) {
            Int32 tmpPriority;
            mNotification->GetPriority(&tmpPriority);

            if (tmpPriority < INotification::PRIORITY_HIGH) {
                mNotification->SetPriority(INotification::PRIORITY_HIGH);
            }
        }

        // 1. initial score: buckets of 10, around the app [-20..20]
        mNotification->GetPriority(&priority);
        Int32 score = priority * NOTIFICATION_PRIORITY_MULTIPLIER;

        // 2. extract ranking signals from the notification data
        AutoPtr<IStatusBarNotification> n;
        CStatusBarNotification::New(mPkg, mOpPkg, mId, mTag,
                mCallingUid, mCallingPid, score, mNotification, mUser,
                (IStatusBarNotification**)&n);

        AutoPtr<NotificationRecord> r = new NotificationRecord(n, score);
        String key;
        n->GetKey(&key);
        AutoPtr<IInterface> obj;
        mHost->mNotificationsByKey->Get(CoreUtils::Convert(key), (IInterface**)&obj);
        AutoPtr<NotificationRecord> old = (NotificationRecord*)IObject::Probe(obj);
        if (old != NULL) {
            // Retain ranking information from previous record
            r->CopyRankingInformation(old);
        }
        mHost->mRankingHelper->ExtractSignals(r);

        // 3. Apply local rules

        // blocked apps
        if (ENABLE_BLOCKED_NOTIFICATIONS && !mHost->NoteNotificationOp(mPkg, mCallingUid)) {
            if (!mIsSystemNotification) {
                r->mScore = JUNK_SCORE;
                Logger::E(TAG, "Suppressing notification from package %s by user request.", pkg.string());
            }
        }

        if (r->mScore < SCORE_DISPLAY_THRESHOLD) {
            // Notification will be blocked because the score is too low.
            return NOERROR;
        }

        // Clear out group children of the old notification if the update causes the
        // group summary to go away. This happens when the old notification was a
        // summary and the new one isn't, or when the old notification was a summary
        // and its group key changed.
        Boolean res;
        if (old != NULL && (old->GetNotification()->IsGroupSummary(&res), res) &&
                ((mNotification->IsGroupSummary(&res), !res) ||
                        !old->GetGroupKey().Equals(r->GetGroupKey()))) {
            mHost->CancelGroupChildrenLocked(old, mCallingUid, mCallingPid, String(NULL));
        }

        key = String(NULL);
        n->GetKey(&key);
        Int32 index = IndexOfNotificationLocked(key);
        if (index < 0) {
            mHost->mNotificationList->Add(TO_IINTERFACE(r));
            mHost->mUsageStats->RegisterPostedByApp(r);
        }
        else {
            AutoPtr<IInterface> obj;
            mHost->mNotificationList->Get(index, (IInterface**)&obj);
            old = (NotificationRecord*)IObject::Probe(obj);
            mHost->mNotificationList->Set(index, TO_IINTERFACE(r));
            mHost->mUsageStats->RegisterUpdatedByApp(r, old);
            // Make sure we don't lose the foreground service state.

            mNotification->GetFlags(&flags);
            Int32 oldFlags;
            old->mNotification->GetFlags(&oldFlags);
            mNotification->SetFlags(flags |= oldFlags & INotification::FLAG_FOREGROUND_SERVICE);

            r->mIsUpdate = TRUE;
        }

        key = String(NULL);
        n->GetKey(&key);
        mHost->mNotificationsByKey->Put(CoreUtils::Convert(key), TO_IINTERFACE(r));

        // Ensure if this is a foreground service that the proper additional
        // flags are set.
        mNotification->GetFlags(&flags);
        if ((flags & INotification::FLAG_FOREGROUND_SERVICE) != 0) {
            mNotification->SetFlags(flags |= INotification::FLAG_ONGOING_EVENT
                    | INotification::FLAG_NO_CLEAR);
        }

        mHost->ApplyZenModeLocked(r);
        mHost->mRankingHelper->Sort(mHost->mNotificationList);

        Int32 icon;
        mNotification->GetIcon(&icon);
        if (icon != 0) {
            AutoPtr<IStatusBarNotification> oldSbn = (old != NULL) ? old->mSbn : NULL;
            mHost->mListeners->NotifyPostedLocked(n, oldSbn);
        }
        else {
            Slogger::E(TAG, "Not posting notification with icon==0: %p", mNotification.Get());
            if (old != NULL && !old->mIsCanceled) {
                mHost->mListeners->NotifyRemovedLocked(n);
            }
            // ATTENTION: in a future release we will bail out here
            // so that we do not play sounds, show lights, etc. for invalid
            // notifications
            String str;
            n->GetPackageName(&str);
            Slogger::E(TAG, "WARNING: In a future release this will crash the app: %s", str.string());
        }

        mHost->BuzzBeepBlinkLocked(r);
    }
    return NOERROR;
}

//===============================================================================
//                  NotificationManagerService::CancelNotificationRunnable
//===============================================================================

NotificationManagerService::CancelNotificationRunnable::CancelNotificationRunnable(
    /* [in] */ PowerManagerService* host,
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 callingPid,
    /* [in] */ const String& pkg,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ Int32 mustHaveFlags,
    /* [in] */ Int32 mustNotHaveFlags,
    /* [in] */ Boolean sendDelete,
    /* [in] */ Int32 userId,
    /* [in] */ Int32 reason,
    /* [in] */ ManagedServiceInfo* listener)
    : mHost(host)
    , mCallingUid(callingUid)
    , mCallingPid(callingPid)
    , mPkg(pkg)
    , mTag(tag)
    , mId(id)
    , mMustHaveFlags(mustHaveFlags)
    , mMustNotHaveFlags(mustNotHaveFlags)
    , mSendDelete(sendDelete)
    , mUserId(userId)
    , mReason(reason)
    , mListener(listener)
{}

NotificationManagerService::CancelNotificationRunnable::~CancelNotificationRunnable()
{}

ECode NotificationManagerService::CancelNotificationRunnable::Run()
{
    String listenerName;
    if (mListener == NULL){
        listenerName = String(NULL);
    }
    else {
        mListener->mComponent->ToShortString(&listenerName);
    }

    EventLogTags::WriteNotificationCancel(mCallingUid, mCallingPid, mPkg, mId, mTag, mUserId,
            mMustHaveFlags, mMustNotHaveFlags, mReason, listenerName);

    synchronized (mNotificationList) {
        Int32 index = mHost->IndexOfNotificationLocked(mPkg, mTag, mId, mUserId);
        if (index >= 0) {
            AutoPtr<IInterface> obj;
            mHost->mNotificationList->Get(index, (IInterface**)&obj);
            AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);

            // Ideally we'd do this in the caller of this method. However, that would
            // require the caller to also find the notification.
            if (reason == REASON_DELEGATE_CLICK) {
                mHost->mUsageStats->RegisterClickedByUser(r);
            }

            Int32 flags;
            r->GetNotification()->GetFlags(&flags);
            if ((flags & mustHaveFlags) != mustHaveFlags) {
                return NOERROR;
            }
            if ((flags & mustNotHaveFlags) != 0) {
                return NOERROR;
            }

            mHost->mNotificationList->Remove(index);

            mHost->CancelNotificationLocked(r, mSendDelete, mReason);
            mHost->CancelGroupChildrenLocked(r, mCallingUid, mCallingPid, listenerName);
            mHost->UpdateLightsLocked();
        }
    }
    return NOERROR;
}

//===============================================================================
//                  NotificationManagerService
//===============================================================================

NotificationManagerService::NotificationManagerService()
    : mDefaultNotificationColor(0)
    , mDefaultNotificationLedOn(0)
    , mDefaultNotificationLedOff(0)
    , mUseAttentionLight(FALSE)
    , mSystemReady(FALSE)
    , mDisableNotificationEffects(FALSE)
    , mCallState(0)
    , mListenerHints(0)
    , mInterruptionFilter(0)
    , mScreenOn(TRUE)
    , mInCall(FALSE)
    , mNotificationPulseEnabled(FALSE)
{
    CBinder::New((IBinder**)&mForegroundToken);
    CArrayList::New((IArrayList**)&mNotificationList);
    CArrayMap::New((IArrayMap**)&mNotificationsByKey);
    CArrayList::New((IArrayList**)&mToastQueue);
    CArrayList::New((IArrayList**)&mLights);

    CHandlerThread::New(String("ranker"),
            IProcess::THREAD_PRIORITY_BACKGROUND, (IHandlerThread**)&mRankingThread);

    CArraySet::New((IArraySet**)&mListenersDisablingEffects);
    CHashSet::New((IHashSet**)&mBlockedPackages);
    mUserProfiles = new ManagedServices::UserProfiles();

    mNotificationDelegate = new MyNotificationDelegate(this);
    mIntentReceiver = new MyBroadcastReceiver(this);
    mBuzzBeepBlinked = new MyRunnable(this);

    CNotificationManagerStub::New((ISystemService*)this, (IBinder**)&mService);
    mInternalService = new MyNotificationManagerInternal(this);
}

ECode NotificationManagerService::constructor(
    /* [in] */ IContext* context)
{
    FAILED_RETURN(SystemService::constructor(context));
    return NOERROR;
}

void NotificationManagerService::LoadPolicyFile()
{
    synchronized(mPolicyFile) {
        mBlockedPackages->Clear();

        // try {
        AutoPtr<IFileInputStream> infile;
        mPolicyFile->OpenRead((IFileInputStream**)&infile);

        String nullStr;
        AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
        parser->SetInput(infile, nullStr);

        Int32 type;
        String tag;
        Int32 version = DB_VERSION;

        while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT) {
            if (type != IXmlPullParser::START_TAG) {
                continue;
            }

            parser->GetName(&tag);

            if (TAG_BODY.Equals(tag)) {
                String value;
                parser->GetAttributeValue(nullStr, ATTR_VERSION, &value);
                version = StringUtils::ParseInt32(value);
            }
            else if (TAG_BLOCKED_PKGS.Equals(tag)) {
                parser->Next(&type);

                while (type != IXmlPullParser::END_DOCUMENT) {
                    parser->GetName(&tag);

                    if (TAG_PACKAGE.Equals(tag)) {
                        String value;
                        parser->GetAttributeValue(nullStr, ATTR_NAME, &value);
                        mBlockedPackages->Add(CoreUtils::Convert(value));
                    }
                    else if (TAG_BLOCKED_PKGS.Equals(tag) && type == IXmlPullParser::END_TAG) {
                        break;
                    }
                }
            }
            assert(0 && "TODO");
            // mZenModeHelper.readXml(parser);
            // mRankingHelper.readXml(parser);
        }

        // } catch (FileNotFoundException e) {
        //     // No data yet
        // } catch (IOException e) {
        //     Log.wtf(TAG, "Unable to read notification policy", e);
        // } catch (NumberFormatException e) {
        //     Log.wtf(TAG, "Unable to parse notification policy", e);
        // } catch (XmlPullParserException e) {
        //     Log.wtf(TAG, "Unable to parse notification policy", e);
        // } finally {
        AutoPtr<IIoUtils> ioUtils;
        CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
        ioUtils->CloseQuietly(infile);
        // }
    }
}

ECode NotificationManagerService::SavePolicyFile()
{
    mHandler->RemoveMessages(MESSAGE_SAVE_POLICY_FILE);
    mHandler->SendEmptyMessage(MESSAGE_SAVE_POLICY_FILE);
    return NOERROR;
}

void NotificationManagerService::HandleSavePolicyFile()
{
    Slogger::D(TAG, "handleSavePolicyFile");
    synchronized(mPolicyFile) {
        AutoPtr<IFileOutputStream> stream;
        // try {
        ECode ec = mPolicyFile->StartWrite((IFileOutputStream**)&stream);
        if (ec == (ECode)E_IO_EXCEPTION) {
            Slogger::W(TAG, "Failed to save policy file");
            return;
        }
        // } catch (IOException e) {
        //     Slog.w(TAG, "Failed to save policy file", e);
        //     return;
        // }

        // try {
        ECode ec = NOERROR;
        HashSet<String>::Iterator it;
        AutoPtr<IBoolean> standalone;
        String nullStr;
        String emptyStr("");
        AutoPtr<IXmlSerializer> out;
        ec = CFastXmlSerializer::New((IFastXmlSerializer**)&out);
        FAIL_GOTO(ec, _EXIT_)

        ec = out->SetOutput(stream, String("utf-8"));
        FAIL_GOTO(ec, _EXIT_)

        CBoolean::New(TRUE, (IBoolean**)&standalone);
        ec = out->StartDocument(nullStr, standalone);
        FAIL_GOTO(ec, _EXIT_)

        ec = out->WriteStartTag(nullStr, TAG_BODY);
        FAIL_GOTO(ec, _EXIT_)

        ec = out->WriteAttribute(emptyStr, ATTR_VERSION, StringUtils::Int32ToString(DB_VERSION));
        FAIL_GOTO(ec, _EXIT_)

        assert(0 && "TODO");
        // mZenModeHelper.writeXml(out);
        // mRankingHelper.writeXml(out);

        ec = out->WriteEndTag(nullStr, TAG_BODY);
        FAIL_GOTO(ec, _EXIT_)

        ec = out->EndDocument();
        FAIL_GOTO(ec, _EXIT_)

        ec = mPolicyFile->FinishWrite(stream);
        FAIL_GOTO(ec, _EXIT_)
        // } catch (IOException e) {
        //     Slog.w(TAG, "Failed to save policy file, restoring backup", e);
        //     mPolicyFile.failWrite(stream);
        // }

        if (stream) {
            stream->Close();
        }
        return;

    _EXIT_:
        Slogger::W(TAG, "Failed to save policy file, restoring backup");
        mPolicyFile->FailWrite(stream);
        stream->Close();
    }
}

Boolean NotificationManagerService::NoteNotificationOp(
    /* [in] */ const String& pkg,
    /* [in] */ Int32 uid)
{
    Int32 result;
    mAppOps->NoteOpNoThrow(IAppOpsManager::OP_POST_NOTIFICATION, uid, pkg, &result);
    if (result != IAppOpsManager::MODE_ALLOWED) {
        Slogger::V(TAG, "notifications are disabled by AppOps for %s", pkg.string());
        return FALSE;
    }
    return TRUE;
}


AutoPtr< ArrayOf<Int64> > NotificationManagerService::GetLongArray(
    /* [in] */ IResources* res,
    /* [in] */ Int32 resid,
    /* [in] */ Int32 maxlen,
    /* [in] */ ArrayOf<Int64>* def)
{
    AutoPtr< ArrayOf<Int32> > ar;
    res->GetInt32Array(resid, (ArrayOf<Int32>**)&ar);

    AutoPtr< ArrayOf<Int64> > result;
    if (ar == NULL) {
        if (def != NULL) result = def->Clone();
        return result;
    }

    Int32 len = ar->GetLength() > maxlen ? maxlen : ar->GetLength();
    result = ArrayOf<Int64>::Alloc(len);
    for (Int32 i = 0; i < len; i++) {
        (*result)[i] = (*ar)[i];
    }

    return result;
}

ECode NotificationManagerService::OnStart()
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);

    mAm = ActivityManagerNative::GetDefault();
    AutoPtr<IInterface> obj;
    context->GetSystemService(IContext::APP_OPS_SERVICE, (IInterface**)&obj);
    mAppOps = IAppOpsManager::Probe(obj);
    context->GetSystemService(IContext::VIBRATOR_SERVICE, (IInterface**)&obj);
    mVibrator = IVibrator::Probe(obj)

    mHandler = new WorkerHandler(this);
    mRankingThread->Start();
    AutoPtr< ArrayOf<String> > extractorNames;
    // try {
    ECode ec = resources->GetStringArray(
            R::array::config_notificationSignalExtractors,
            (ArrayOf<String>**)&extractorNames);
    if (ec == (ECode)E_RESOURCES_NOT_FOUND_EXCEPTION) {
        extractorNames = ArrayOf<String>::Alloc(0);
    }
    // } catch (Resources.NotFoundException e) {
    //     extractorNames = new String[0];
    // }

    assert(0 && "TODO");
    // mRankingHelper = new RankingHelper(getContext(),
    //         new RankingWorkerHandler(mRankingThread.getLooper()),
    //         extractorNames);
    // mZenModeHelper = new ZenModeHelper(getContext(), mHandler);
    // mZenModeHelper.addCallback(new ZenModeHelper.Callback() {
    //     @Override
    //     public void onConfigChanged() {
    //         savePolicyFile();
    //     }

    //     @Override
    //     void onZenModeChanged() {
    //         synchronized(mNotificationList) {
    //             updateInterruptionFilterLocked();
    //         }
    //     }
    // });


    AutoPtr<IFile> systemDir;
    CFile::New(Environment::GetDataDirectory(), String("system"), (IFile**)&systemDir);
    AutoPtr<IFile> other;
    CFile::New(systemDir, String("notification_policy.xml"), (IFile**)&other);
    CAtomicFile::New(other, (IAtomicFile**)&mPolicyFile);
    assert(0 && "TODO");
    // mUsageStats = new NotificationUsageStats(getContext());

    ImportOldBlockDb();

    mListeners = new NotificationListeners(this);
    assert(0 && "TODO");
    // mConditionProviders = new ConditionProviders(getContext(),
    //         mHandler, mUserProfiles, mZenModeHelper);
    mStatusBar = GetLocalService(EIID_IStatusBarManagerInternal);
    mStatusBar->SetNotificationDelegate(mNotificationDelegate);

    obj = GetLocalService(EIID_ILightsManager);
    AutoPtr<LightsManager> lights = (LightsManager*)ILightsManager::Probe(obj).Get();
    mNotificationLight = lights->GetLight(LightsManager::LIGHT_ID_NOTIFICATIONS);
    mAttentionLight = lights->GetLight(LightsManager::LIGHT_ID_ATTENTION);

    resources->GetColor(R::color::config_defaultNotificationColor, &mDefaultNotificationColor);
    resources->GetInteger(R::integer::config_defaultNotificationLedOn, &mDefaultNotificationLedOn);
    resources->GetInteger(R::integer::config_defaultNotificationLedOff, &mDefaultNotificationLedOff);

    mDefaultVibrationPattern = GetLongArray(resources,
        R::array::config_defaultNotificationVibePattern, VIBRATE_PATTERN_MAXLEN, DEFAULT_VIBRATE_PATTERN);
    mFallbackVibrationPattern = GetLongArray(resources,
        R::array::config_notificationFallbackVibePattern, VIBRATE_PATTERN_MAXLEN, DEFAULT_VIBRATE_PATTERN);

    resources->GetBoolean(R::bool::config_useAttentionLight, &mUseAttentionLight);

    // Don't start allowing notifications until the setup wizard has run once.
    // After that, including subsequent boots, init with notifications turned on.
    // This works on the first boot because the setup wizard will toggle this
    // flag at least once and we'll go back to 0 after that.
    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);

    AutoPtr<ISettingsGlobal> globalSettings;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&globalSettings);

    Int32 value;
    globalSettings->GetInt32(resolver, ISettingsGlobal::DEVICE_PROVISIONED, 0, &value);

    if (value == 0) {
        mDisableNotificationEffects = TRUE;
    }

    assert(0 && "TODO");
    // mZenModeHelper.updateZenMode();

    mUserProfiles->UpdateCache(context);
    ListenForCallState();

    // register for various Intents
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(IIntent::ACTION_SCREEN_ON);
    filter->AddAction(IIntent::ACTION_SCREEN_OFF);
    // filter->AddAction(ITelephonyManager::ACTION_PHONE_STATE_CHANGED);
    filter->AddAction(IIntent::ACTION_USER_PRESENT);
    filter->AddAction(IIntent::ACTION_USER_STOPPED);
    filter->AddAction(IIntent::ACTION_USER_SWITCHED);
    filter->AddAction(IIntent::ACTION_USER_ADDED);

    AutoPtr<IIntent> stickyIntent;
    context->RegisterReceiver(mIntentReceiver, filter, (IIntent**)&stickyIntent);

    AutoPtr<IIntentFilter> pkgFilter;
    CIntentFilter::New((IIntentFilter**)&pkgFilter);
    pkgFilter->AddAction(IIntent::ACTION_PACKAGE_ADDED);
    pkgFilter->AddAction(IIntent::ACTION_PACKAGE_REMOVED);
    pkgFilter->AddAction(IIntent::ACTION_PACKAGE_CHANGED);
    pkgFilter->AddAction(IIntent::ACTION_PACKAGE_RESTARTED);
    pkgFilter->AddAction(IIntent::ACTION_QUERY_PACKAGE_RESTART);
    pkgFilter->AddDataScheme(String("package"));

    stickyIntent = NULL;
    context->RegisterReceiver(mIntentReceiver, pkgFilter, (IIntent**)&stickyIntent);

    AutoPtr<IIntentFilter> sdFilter;
    CIntentFilter::New(IIntent::ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE, (IIntentFilter**)&sdFilter);

    stickyIntent = NULL;
    context->RegisterReceiver(mIntentReceiver, sdFilter, (IIntent**)&stickyIntent);

    mSettingsObserver = new SettingsObserver(this, mHandler);

    resources->GetInteger(R::integer::config_notificationServiceArchiveSize, &value);
    mArchive = new Archive(value);

    PublishBinderService(IContext::NOTIFICATION_SERVICE, mService);
    PublishLocalService(EIID_INotificationManagerInternal, mInternalService);

    return NOERROR;
}

void NotificationManagerService::ImportOldBlockDb()
{
    LoadPolicyFile();

    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IPackageManager> pm;
    context->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IIterator> iterator;
    ISet::Probe(mBlockedPackages)->GetIterator((IIterator**)&iterator);
    Boolean hasNext;
    while (iterator->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> obj;
        iterator->GetNext((IInterface**)&obj);
        String pkg;
        ICharSequence::Probe(obj)->ToString(&pkg);
        AutoPtr<IPackageInfo> info;
        // try {
        pm->GetPackageInfo(pkg, 0, (IPackageInfo**)&info);
        AutoPtr<IApplicationInfo> applicationInfo;
        info->GetApplicationInfo((IApplicationInfo**)&applicationInfo);
        Int32 uid;
        applicationInfo->GetUid(&uid);
        SetNotificationsEnabledForPackageImpl(pkg, uid, FALSE);
        // } catch (NameNotFoundException e) {
        //     // forget you
        // }
    }
    mBlockedPackages->Clear();
}

ECode NotificationManagerService::OnBootPhase(
    /* [in] */ Int32 phase)
{
    if (phase == ISystemService::PHASE_SYSTEM_SERVICES_READY) {
        // no beeping until we're basically done booting
        mSystemReady = TRUE;

        // Grab our optional AudioService
        AutoPtr<IContext> context;
        GetContext((IContext**)&context);
        AutoPtr<IInterface> obj;
        context->GetSystemService(IContext::AUDIO_SERVICE, (IInterface**)&obj);
        mAudioManager = IAudioManager::Probe(obj);
        mZenModeHelper->SetAudioManager(mAudioManager);
    }
    else if (phase == ISystemService::PHASE_THIRD_PARTY_APPS_CAN_START) {
        // This observer will force an update when observe is called, causing us to
        // bind to listener services.
        mSettingsObserver->Observe();
        mListeners->OnBootPhaseAppsCanStart();
        mConditionProviders->OnBootPhaseAppsCanStart();
    }
    return NOERROR;
}

void NotificationManagerService::SetNotificationsEnabledForPackageImpl(
    /* [in] */ const String& pkg,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean enabled)
{
    StringBuilder builder;
    builder += enabled ? "en":"dis";
    builder += "abling notifications for ";
    builder += pkg;

    Slogger::V(TAG, builder.ToString());

    mAppOps->SetMode(IAppOpsManager::OP_POST_NOTIFICATION, uid, pkg,
            enabled ? IAppOpsManager::MODE_ALLOWED : IAppOpsManager::MODE_IGNORED);

    // Now, cancel any outstanding notifications that are part of a just-disabled app
    if (ENABLE_BLOCKED_NOTIFICATIONS && !enabled) {
        CancelAllNotificationsInt(MY_UID, MY_PID, pkg, 0, 0, TRUE, UserHandle::GetUserId(uid),
                REASON_PACKAGE_BANNED, NULL);
    }
}

void NotificationManagerService::UpdateListenerHintsLocked()
{
    Boolean res;
    ISet::Probe(mListenersDisablingEffects)->IsEmpty(&res);
    const Int32 hints = res ? 0 : HINT_HOST_DISABLE_EFFECTS;
    if (hints == mListenerHints) return;
    mListenerHints = hints;
    ScheduleListenerHintsChanged(hints);
}

void NotificationManagerService::UpdateEffectsSuppressorLocked()
{
    Boolean res;
    ISet::Probe(mListenersDisablingEffects)->IsEmpty(&res);
    AutoPtr<IComponentName> suppressor;
    if (!res) {
        AutoPtr<IInterface> obj;
        mListenersDisablingEffects->GetValueAt(0, (IInterface**)&obj);
        AutoPtr<ManagedServiceInfo> info = (ManagedServiceInfo*)IObject::Probe(obj);
        suppressor = info->mComponent;
    }
    if (Objects::Equals(suppressor, mEffectsSuppressor)) return;
    mEffectsSuppressor = suppressor;
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IIntent> intent;
    CIntent::New(INotificationManager::ACTION_EFFECTS_SUPPRESSOR_CHANGED, (IIntent**)&intent);
    intent->AddFlags(IIntent::FLAG_RECEIVER_REGISTERED_ONLY);
    context->SendBroadcast(intent);
}

void NotificationManagerService::UpdateInterruptionFilterLocked()
{
    Int32 interruptionFilter;
    assert(0 && "TODO");
    // interruptionFilter = mZenModeHelper->GetZenModeListenerInterruptionFilter();
    if (interruptionFilter == mInterruptionFilter) return;
    mInterruptionFilter = interruptionFilter;
    ScheduleInterruptionFilterChanged(interruptionFilter);
}

AutoPtr< ArrayOf<String> > NotificationManagerService::GetActiveNotificationKeys(
    /* [in] */ IINotificationListener* token)
{
    AutoPtr<ManagedServiceInfo> info;
    mListeners->CheckServiceTokenLocked(token, (ManagedServiceInfo**)&info);
    AutoPtr<IArrayList> keys;
    CArrayList::New((IArrayList**)&keys);
    if (info->IsEnabledForCurrentProfiles()) {
        synchronized(mNotificationList) {
            Int32 N;
            mNotificationList->GetSize(&N);
            for (Int32 i = 0; i < N; i++) {
                AutoPtr<IInterface> obj;
                mNotificationList->Get(i, (IInterface**)&obj);
                AutoPtr<NotificationRecord> record = (NotificationRecord*)IObject::Probe(obj);
                AutoPtr<IStatusBarNotification> sbn = record->mSbn;
                Int32 id;
                sbn->GetUserId(&id);
                if (info->EnabledAndUserMatches(id)) {
                    String key;
                    sbn->GetKey(&key);
                    keys->Add(CoreUtils::Convert(key));
                }
            }
        }
    }
    Int32 size;
    keys->GetSize(&size);
    AutoPtr< ArrayOf<String> > args = ArrayOf<String>::Alloc(size);
    AutoPtr< ArrayOf<String> > result;
    keys->ToArray(args, (ArrayOf<String>**)&result);
    return keys.toArray(new String[keys.size()]);
}

String NotificationManagerService::DisableNotificationEffects(
    /* [in] */ NotificationRecord* record)
{
    if (mDisableNotificationEffects) {
        return String("booleanState");
    }
    if ((mListenerHints & HINT_HOST_DISABLE_EFFECTS) != 0) {
        return String("listenerHints");
    }
    if (mCallState != ITelephonyManager::CALL_STATE_IDLE && !mZenModeHelper->IsCall(record)) {
        return String("callState");
    }
    return String(NULL);
}

void NotificationManagerService::DumpImpl(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ DumpFilter* filter)
{
    pw->Print(String("Current Notification Manager state"));
    if (filter != NULL) {
        pw->Print(String(" (filtered to "));
        pw->Print(filter);
        pw->Print(String(")"));
    }
    pw->PrintCharln(':');
    Int32 N;
    final Boolean zenOnly = filter != NULL && filter->mZen;

    if (!zenOnly) {
        synchronized(mToastQueue) {
            mToastQueue->GetSize(&N);
            if (N > 0) {
                pw->Println(String("  Toast Queue:"));
                for (Int32 i = 0; i < N; i++) {
                    AutoPtr<IInterface> obj;
                    mToastQueue->Get(i, (IInterface**)&obj);
                    AutoPtr<ToastRecord> record = (ToastRecord*)IObject::Probe(obj);
                    record->Dump(pw, String("    "), filter);
                }
                pw->Println(String("  "));
            }
        }
    }

    synchronized(mNotificationList) {
        if (!zenOnly) {
            mNotificationList->GetSize(&N);
            if (N > 0) {
                pw->Println(String("  Notification List:"));
                for (Int32 i = 0; i < N; i++) {
                    AutoPtr<IInterface> obj;
                    mNotificationList->Get(i, (IInterface**)&obj);
                    AutoPtr<NotificationRecord> nr = (NotificationRecord*)IObject::Probe(obj);
                    if (filter != NULL && !filter->Matches(nr->mSbn)) continue;
                    AutoPtr<IContext> context;
                    GetContext((IContext**)&context);
                    nr->Dump(pw, String("    "), context);
                }
                pw->Println(String("  "));
            }

            if (filter == NULL) {
                mLights->GetSize(&N);
                if (N > 0) {
                    pw->Println(String("  Lights List:"));
                    for (Int32 i = 0; i < N; i++) {
                        AutoPtr<IInterface> obj;
                        mLights->Get(i, (IInterface**)&obj);
                        String str;
                        ICharSequence::Probe(obj)->ToString(&str);
                        pw->Println(String("    ") + str);
                    }
                    pw->Println(String("  "));
                }
                pw->Println(String("  mUseAttentionLight=") + StringUtils::BooleanToString(mUseAttentionLight));
                pw->Println(String("  mNotificationPulseEnabled=") + StringUtils::BooleanToString(mNotificationPulseEnabled));
                StringBuilder builder;
                builder += "  mSoundNotification=";
                builder += mSoundNotification;
                pw->Println(builder.ToString());

                builder.Reset();
                builder += "  mVibrateNotification=";
                builder += mVibrateNotification;
                pw->Println(builder.ToString());
                pw->Println(String("  mDisableNotificationEffects=") + StringUtils::BooleanToString(mDisableNotificationEffects));
                pw->Println(String("  mCallState=") + CallStateToString(mCallState));
                pw->Println(String("  mSystemReady=") + StringUtils::BooleanToString(mSystemReady));
            }
            String str;
            mArchive->ToString(&str);
            pw->Println(String("  mArchive=") + str);

            AutoPtr<IIterator> iter;
            mArchive->DescendingIterator((IIterator**)&iter);
            Int32 i = 0;
            Boolean res;
            while (iter->HasNext(&res), res) {
                AutoPtr<IInterface> obj;
                iter->GetNext((IInterface**)&obj);
                AutoPtr<IStatusBarNotification> sbn = IStatusBarNotification::Probe(obj);
                if (filter != NULL && !filter->Matches(sbn)) continue;
                StringBuilder builder;
                builder += "    ";
                builder += sbn;
                pw->Println(builder.ToString());
                if (++i >= 5) {
                    if (iter->HasNext(&res), res) {
                        pw->Println(String("    ..."));
                    }
                    break;
                }
            }
        }

        if (!zenOnly) {
            pw->Println(String("\n  Usage Stats:"));
            mUsageStats->Dump(pw, String("    "), filter);
        }

        if (filter == NULL || zenOnly) {
            pw->Println(String("\n  Zen Mode:"));
            mZenModeHelper->Dump(pw, String("    "));

            pw->Println(String("\n  Zen Log:"));
            ZenLog->Dump(pw, String("    "));
        }

        if (!zenOnly) {
            pw->Println(String("\n  Ranking Config:"));
            mRankingHelper->Dump(pw, String("    "), filter);

            pw->Println(String("\n  Notification listeners:"));
            mListeners->Dump(pw, filter);
            pw->Print(String("    mListenerHints: "));
            pw->Println(mListenerHints);
            pw->Print(String("    mListenersDisablingEffects: ("));
            mListenersDisablingEffects->GetSize(&N);
            for (Int32 i = 0; i < N; i++) {
                AutoPtr<IInterface> obj;
                mListenersDisablingEffects->GetValueAt(i, (IInterface**)&obj);
                AutoPtr<ManagedServiceInfo> listener = (ManagedServiceInfo*)IObject::Probe(obj);
                if (i > 0) pw->PrintCharln(',');
                pw->Print(listener->mComponent);
            }
            pw->PrintCharln(')');
        }

        pw->Println(String("\n  Condition providers:"));
        mConditionProviders->Dump(pw, filter);
    }
}

ECode NotificationManagerService::EnqueueNotificationInternal(
    /* [in] */ const String& pkg,
    /* [in] */ const String& opPkg,
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 callingPid,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification,
    /* [in] */ ArrayOf<Int32>* idOut,
    /* [in] */ Int32 incomingUserId,
    /* [out, callee] */ ArrayOf<Int32>** outIdReceived)
{
    VALIDATE_NOT_NULL(outIdReceived);

    AutoPtr< ArrayOf<Int32> > cloneIdout;
    if (idOut != NULL) {
        cloneIdout = idOut->Clone();
    }
    else {
        cloneIdout = ArrayOf<Int32>::Alloc(1);
    }

    if (DBG) {
        AutoPtr<ICharSequence> text;
        notification->GetTickerText((ICharSequence**)&text);
        String str;
        if(text != NULL){
            text->ToString(&str);
        }

        Logger::V(TAG, "enqueueNotificationInternal: pkg=[%s] id=%d userId=%d notification=[%s]",
            pkg.string(), id, userId, str.string());
    }

    CheckCallerIsSystemOrSameApp(pkg);
    Boolean isSystemNotification = IsUidSystem(callingUid)|| pkg.Equals("android");
    Boolean isNotificationFromListener = mListeners->IsListenerPackage(pkg);

    AutoPtr<IActivityManagerHelper> activityMgrHelper;
    CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&activityMgrHelper);
    Int32 userId;
    activityMgrHelper->HandleIncomingUser(callingPid, callingUid, incomingUserId, TRUE, FALSE,
            String("enqueueNotification"), pkg, &userId);

    AutoPtr<IUserHandle> user;
    CUserHandle::New(userId, (IUserHandle**)&user);

    // Limit the number of notifications that any given package except the android
    // package or a registered listener can enqueue.  Prevents DOS attacks and deals with leaks.
    if (!isSystemNotification && !isNotificationFromListener) {
        synchronized(mNotificationList) {
            Int32 count = 0;
            Int32 N;
            mNotificationList->GetSize(&N);
            for (Int32 i = 0; i < N; ++i) {
                AutoPtr<IInterface> obj;
                mNotificationList->Get(i, (IInterface**)&obj);
                AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);
                String packageName;
                r->mSbn->GetPackageName(&packageName);
                Int32 _userid;
                r->mSbn->GetUserId(&_userid);
                if (packageName.Equals(pkg) && _userid == userId) {
                    count++;
                    if (count >= MAX_PACKAGE_NOTIFICATIONS) {
                        Logger::E(TAG, "Package has already posted %d notifications.  Not showing more.  package=%s",
                            count, pkg.string());
                        return NOERROR;
                    }
                }
            }
        }
    }

    // This conditional is a dirty hack to limit the logging done on
    //     behalf of the download manager without affecting other apps.
    // if (!pkg.equals("com.android.providers.downloads")
    //         || Log.isLoggable("DownloadManager", Log.VERBOSE)) {
    //     EventLogTags.writeNotificationEnqueue(callingUid, callingPid,
    //             pkg, id, tag, userId, notification.toString());
    // }

    if (pkg.IsNull() || notification == NULL) {
        Logger::E(TAG, "null not allowed: pkg=%s, id=%d, notification=%p", pkg.string(), id, notification);
        //throw new IllegalArgumentException("null not allowed: pkg=" + pkg + " id=" + id + " notification=" + notification);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 icon;
    notification->GetIcon(&icon);
    Boolean res;
    if (icon != 0) {
        if (notification->IsValid(&res), !res) {
            Logger::E(TAG, "Invalid notification: pkg=%s, id=%d, notification=[%p]", pkg.string(), id, notification);
            //throw new IllegalArgumentException("Invalid notification: pkg=" + pkg + " id=" + id + " notification=" + notification);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    AutoPtr<EnqueueNotificationInternalRunnable> runnable = new EnqueueNotificationInternalRunnable(
            this, pkg, opPkg, callingUid, callingPid, tag, id, notification, user, isSystemNotification);
    mHandler->Post(runnable, &res);

    (*cloneIdout)[0] = id;
    *outIdReceived = cloneIdout;
    REFCOUNT_ADD(*outIdReceived);
    return NOERROR;
}

ECode NotificationManagerService::BuzzBeepBlinkLocked(
    /* [in] */ NotificationRecord* record)
{
    Boolean buzzBeepBlinked = FALSE;
    AutoPtr<INotification> notification;
    record->mSbn->GetNotification((INotification**)&notification);

    // Should this notification make noise, vibe, or use the LED?
    const Boolean canInterrupt = (record->mScore >= SCORE_INTERRUPTION_THRESHOLD) &&
            !record->IsIntercepted();
    if (DBG || record->IsIntercepted()) {
        String packageName;
        record->mSbn->GetPackageName(&packageName);
        Slogger::V(TAG, "pkg=%s canInterrupt=%d intercept=%d", packageName.string(), canInterrupt, record.isIntercepted());
    }

    Int32 currentUser;
    const Int64 token = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<IActivityManagerHelper> activityMgrHelper;
    CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&activityMgrHelper);
    activityMgrHelper->GetCurrentUser(&currentUser);
    // } finally {
    Binder::RestoreCallingIdentity(token);
    // }

    // If we're not supposed to beep, vibrate, etc. then don't.
    String disableEffects = DisableNotificationEffects(record);
    if (!disableEffects.IsNull()) {
        ZenLog::TraceDisableEffects(record, disableEffects);
    }
    Int32 flags;
    notification->GetFlags(&flags);
    if (disableEffects.IsNull()
            && (!(record->mIsUpdate
                && (flags & INotification::FLAG_ONLY_ALERT_ONCE) != 0 ))
            && (record->GetUserId() == IUserHandle::USER_ALL ||
                record->GetUserId() == currentUser ||
                mUserProfiles->IsCurrentProfile(record->GetUserId()))
            && canInterrupt
            && mSystemReady
            && mAudioManager != NULL) {
        if (DBG) {
            Slogger::V(TAG, "Interrupting!");
        }

        String packageName;
        record->mSbn->GetPackageName(&packageName);
        SendAccessibilityEvent(notification, packageName);

        // sound

        // should we use the default notification sound? (indicated either by
        // DEFAULT_SOUND or because notification.sound is pointing at
        // Settings.System.NOTIFICATION_SOUND)

        Int32 defaults;
        notification->GetDefaults(&defaults);

        AutoPtr<ISettingsSystem> settingsSystem;
        CSettingsSystem::AcquireSingleton((ISettingsSystem**)&settingsSystem);
        AutoPtr<IUri> notificationUri;
        settingsSystem->GetDefaultNotificationUri((IUri**)&notificationUri);
        AutoPtr<IUri> sound;
        notification->GetSound((IUri**)&sound);
        Boolean res;
        notificationUri->Equals(sound, &res);

        Boolean useDefaultSound = (defaults & INotification::DEFAULT_SOUND) != 0 || res;

        AutoPtr<IUri> soundUri;
        Boolean hasValidSound = FALSE;

        if (useDefaultSound) {
            soundUri = notificationUri;

            // check to see if the default notification sound is silent
            AutoPtr<IContext> context;
            GetContext((IContext**)&context);
            AutoPtr<IContentResolver> resolver;
            context->GetContentResolver((IContentResolver**)&resolver);
            String str;
            settingsSystem->GetString(resolver, ISettingsSystem::NOTIFICATION_SOUND, &str);
            hasValidSound = !str.IsNull();
        }
        else if (sound != NULL){
            soundUri = sound;
            hasValidSound = (soundUri != NULL);
        }

        if (hasValidSound) {
            notification->GetFlags(&flags);
            Boolean looping = (flags & INotification::FLAG_INSISTENT) != 0;
            AutoPtr<IAudioAttributes> attr;
            notification->GetAudioAttributes((IAudioAttributes**)&attr);
            AutoPtr<IAudioAttributes> audioAttributes;
            if (attr != NULL) {
                audioAttributes = attr;
            }
            else {
                audioAttributes = Notification::AUDIO_ATTRIBUTES_DEFAULT;
            }
            mSoundNotification = record;
            // do not play notifications if stream volume is 0 (typically because
            // ringer mode is silent) or if there is a user of exclusive audio focus
            AutoPtr<IAudioAttributesHelper> helper;
            CAudioAttributesHelper::AcquireSingleton((IAudioAttributesHelper**)&helper);
            Int32 result;
            helper->ToLegacyStreamType(audioAttributes, &result);
            Int32 data;
            mAudioManager->GetStreamVolume(result, &data);
            Boolean res;
            mAudioManager->IsAudioFocusExclusive(&res);
            if ((data != 0) && !res) {
                const Int64 identity = Binder::ClearCallingIdentity();
                // try {
                AutoPtr<IIRingtonePlayer> player;
                mAudioManager->GetRingtonePlayer((IIRingtonePlayer**)&player);
                if (player != NULL) {
                    if (DBG) {
                        Slogger::V(TAG, "Playing sound %p with attributes %p", soundUri.Get(), audioAttributes.Get());
                    }
                    AutoPtr<IUserHandle> user;
                    record->mSbn->GetUser((IUserHandle**)&user);
                    player->PlayAsync(soundUri, user, looping, audioAttributes);
                    buzzBeepBlinked = TRUE;
                }
                // } catch (RemoteException e) {
                // } finally {
                Binder::RestoreCallingIdentity(identity);
                // }
            }
        }

        // vibrate
        // Does the notification want to specify its own vibration?
        AutoPtr< ArrayOf<Int64> > vibrate;
        notification->GetVibrate((ArrayOf<Int64>**)&vibrate);
        Boolean hasCustomVibrate = vibrate != NULL;

        // new in 4.2: if there was supposed to be a sound and we're in vibrate
        // mode, and no other vibration is specified, we fall back to vibration
        Int32 mode;
        mAudioManager->GetRingerMode(&mode);

        Boolean convertSoundToVibration =
            !hasCustomVibrate
            && hasValidSound
            && (mode == IAudioManager::RINGER_MODE_VIBRATE);

        // The DEFAULT_VIBRATE flag trumps any custom vibration AND the fallback.
        notification->GetDefaults(&defaults);
        Boolean useDefaultVibrate =
                (defaults & INotification::DEFAULT_VIBRATE) != 0;

        if ((useDefaultVibrate || convertSoundToVibration || hasCustomVibrate)
                && !(mode == IAudioManager::RINGER_MODE_SILENT)) {
            mVibrateNotification = record;

            if (useDefaultVibrate || convertSoundToVibration) {
                // Escalate privileges so we can use the vibrator even if the
                // notifying app does not have the VIBRATE permission.
                Int64 identity;
                Binder::ClearCallingIdentity(&identity);
                // try {
                Int32 uid;
                record->mSbn->GetUid(&uid);
                String opPkg;
                record->mSbn->GetOpPkg(&opPkg);
                mVibrator->Vibrate(uid, opPkg,
                        useDefaultVibrate ? *mDefaultVibrationPattern : *mFallbackVibrationPattern,
                        ((flags & INotification::FLAG_INSISTENT) != 0) ? 0: -1,
                        AudioAttributesForNotification(notification));
                buzzBeepBlinked = TRUE;

                Binder::RestoreCallingIdentity(identity);
            }
            else if (vibrate->GetLength() > 1) {
                // If you want your own vibration pattern, you need the VIBRATE
                // permission
                Int32 uid;
                record->mSbn->GetUid(&uid);
                String opPkg;
                record->mSbn->GetOpPkg(&opPkg);
                mVibrator->Vibrate(uid, opPkg,
                        *vibrate,
                        ((flags & INotification::FLAG_INSISTENT) != 0) ? 0: -1,
                        AudioAttributesForNotification(notification));
                buzzBeepBlinked = TRUE;
            }
        }
    }

    // light
    // release the light
    String key = record->GetKey();
    Boolean wasShowLights;
    mLights->Remove(CoreUtils::Convert(key), &wasShowLights);
    if (mLedNotification != NULL && key.Equals(mLedNotification->GetKey())) {
        mLedNotification = NULL;
    }
    notification->GetFlags(&flags);
    if ((flags & INotification::FLAG_SHOW_LIGHTS) != 0 && canInterrupt) {
        mLights->Add(CoreUtils::Convert(key));
        UpdateLightsLocked();
        if (mUseAttentionLight) {
            mAttentionLight->Pulse();
        }
        buzzBeepBlinked = TRUE;
    }
    else if (wasShowLights) {
        UpdateLightsLocked();
    }
    if (buzzBeepBlinked) {
        Boolean res;
        mHandler->Post(mBuzzBeepBlinked, &res);
    }

    return NOERROR;
}

AutoPtr<IAudioAttributes> NotificationManagerService::AudioAttributesForNotification(
    /* [in] */ INotification* n)
{
    AutoPtr<IAudioAttributes> attr;
    n->GetAudioAttributes((IAudioAttributes**)&attr);
    Int32 type;
    n->GetAudioStreamType(&type);
    if (attr != NULL) {
        return attr;
    }

    assert(0 && "TODO";)

    // else if (type >= 0 && type < AudioSystem.getNumStreamTypes()) {
    //     // the stream type is valid, use it
    //     AutoPtr<IAudioAttributesBuilder> builder;
    //     CAudioAttributesBuilder::New((IAudioAttributesBuilder**)&builder);
    //     builder->SetInternalLegacyStreamType(type);
    //     AutoPtr<IAudioAttributes> result;
    //     builder->Build((IAudioAttributes**)&result);
    //     return result;
    // }
    // else if (type == IAudioSystem::STREAM_DEFAULT) {
    //     return Notification.AUDIO_ATTRIBUTES_DEFAULT;
    // }
    // else {
    //     String str("");
    //     Logger::W(TAG, str.AppendFormat("Invalid stream type: %d", type));
    //     return Notification.AUDIO_ATTRIBUTES_DEFAULT;
    // }
}

ECode NotificationManagerService::ShowNextToastLocked()
{
    AutoPtr<IInterface> obj;
    mToastQueue->Get(0, (IInterface**)&obj);
    AutoPtr<ToastRecord> record = (ToastRecord*)IObject::Probe(obj);
    while (record != NULL) {
        if (DBG) Slogger::D(TAG, "Show pkg= %s callback=%p",
                record->mPkg.string(), record->mCallback.Get());
        // try {
        ECode ec = record->mCallback->Show();
        if (SUCCEEDED(ec)) {
            ScheduleTimeoutLocked(record);
            return NOERROR;
        }
        // } catch (RemoteException e) {
        Slogger::W(TAG, "Object died trying to show notification %p in package %s",
                record->mCallback.Get(), record->mPkg.string());
        // remove it from the list and let the process die
        Int32 index;
        mToastQueue->IndexOf((IObject*)record, &index);
        if (index >= 0) {
            mToastQueue->Remove(index);
        }
        KeepProcessAliveLocked(record->mPid);
        Int32 size;
        if ((mToastQueue->GetSize(&size), size) > 0) {
            obj = NULL;
            mToastQueue->Get(0, (IInterface**)&obj);
            record = (ToastRecord*)IObject::Probe(obj);
        }
        else {
            record = NULL;
        }
        // }
    }
    return NOERROR;
}

ECode NotificationManagerService::CancelToastLocked(
    /* [in] */ Int32 index)
{
    AutoPtr<IInterface> obj;
    mToastQueue->Get(index, (IInterface**)&obj);
    AutoPtr<ToastRecord> record = (ToastRecord*)IObject::Probe(obj);
    // try {
    ECode ec = record->mCallback->Hide();
    // }
    // catch (RemoteException e) {
    if (FAILED(ec)) {
        Slogger::W(TAG, "Object died trying to hide notification %p in package %s"
                , record->mCallback.Get(), record->mPkg.string());
        // don't worry about this, we're about to remove it from
        // the list anyway
    }
    // }
    mToastQueue->Remove(index);
    KeepProcessAliveLocked(record->mPid);
    Int32 size;
    if ((mToastQueue->GetSize(&size), size) > 0) {
        // Show the next one. If the callback fails, this will remove
        // it from the list, so don't assume that the list hasn't changed
        // after this point.
        ShowNextToastLocked();
    }
    return NOERROR;
}

void NotificationManagerService::ScheduleTimeoutLocked(
    /* [in] */ ToastRecord* r)
{
    assert(r);
    mHandler->RemoveCallbacksAndMessages((IInterface*)r);

    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> m;
    helper->Obtain(mHandler, MESSAGE_TIMEOUT, (IInterface*)r, (IMessage**)&m);
    Int64 delay = r->mDuration == IToast::LENGTH_LONG ? LONG_DELAY : SHORT_DELAY;
    Boolean result;
    mHandler->SendMessageDelayed(m, delay, &result);
}

void NotificationManagerService::HandleTimeout(
    /* [in] */ ToastRecord* record)
{
    if (DBG) Slogger::D(TAG, "Timeout pkg=%s, callback=%p",
            record->mPkg.string(), record->mCallback.Get());

    synchronized(mToastQueue) {
        Int32 index = IndexOfToastLocked(record->mPkg, record->mCallback);
        if (index >= 0) {
            CancelToastLocked(index);
        }
    }
}

Int32 NotificationManagerService::IndexOfToastLocked(
    /* [in] */ const String& pkg,
    /* [in] */ IITransientNotification* callback)
{
    AutoPtr<IBinder> cbak = IBinder::Probe(callback);

    AutoPtr<IArrayList> list = mToastQueue;
    Int32 len;
    list->GetSize(&len);
    for (Int32 i = 0; i < len; i++) {
        AutoPtr<IInterface> obj;
        list->Get(i, (IInterface**)&obj);
        AutoPtr<ToastRecord> r = (ToastRecord*)IObject::Probe(obj);
        AutoPtr<IBinder> tmpCbak = IBinder::Probe(r->mCallback);
        if (r->mPkg.Equals(pkg) && tmpCbak == cbak) {
            return index;
        }
    }

    return -1;
}

void NotificationManagerService::KeepProcessAliveLocked(
    /* [in] */ Int32 pid)
{
    Int32 toastCount = 0; // toasts from this pid
    AutoPtr<IArrayList> list = mToastQueue;
    Int32 N;
    list->GetSize(&N);
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<IInterface> obj;
        list->Get(i, (IInterface**)&obj);
        AutoPtr<ToastRecord> r = (ToastRecord*)IObject::Probe(obj);
        if (r->mPid == pid) {
            toastCount++;
        }
    }
    // try {
    mAm->SetProcessForeground(mForegroundToken, pid, toastCount > 0);
    // } catch (RemoteException e) {
    //     // Shouldn't happen.
    // }
}

void NotificationManagerService::HandleRankingReconsideration(
    /* [in] */ IMessage* message)
{
    AutoPtr<IInterface> obj;
    message->GetObj((IInterface**)&obj);
    if (IRankingReconsideration::Probe(obj) != NULL) return;
    AutoPtr<RankingReconsideration> recon = (RankingReconsideration*)IRankingReconsideration::Probe(obj);
    recon->Run();
    Boolean changed;
    synchronized(mNotificationList) {
        AutoPtr<IInterface> obj;
        mNotificationsByKey->Get(CoreUtils::Convert(recon->GetKey()), (IInterface**)&obj);
        AutoPtr<NotificationRecord> record = (NotificationRecord*)IObject::Probe(obj);
        if (record == NULL) {
            return;
        }
        Int32 indexBefore = FindNotificationRecordIndexLocked(record);
        Boolean interceptBefore = record->IsIntercepted();
        Int32 visibilityBefore = record->GetPackageVisibilityOverride();
        recon->ApplyChangesLocked(record);
        ApplyZenModeLocked(record);
        mRankingHelper->Sort(mNotificationList);
        Int32 indexAfter = FindNotificationRecordIndexLocked(record);
        Boolean interceptAfter = record->IsIntercepted();
        Int32 visibilityAfter = record->GetPackageVisibilityOverride();
        changed = indexBefore != indexAfter || interceptBefore != interceptAfter
                || visibilityBefore != visibilityAfter;
        if (interceptBefore && !interceptAfter) {
            BuzzBeepBlinkLocked(record);
        }
    }
    if (changed) {
        ScheduleSendRankingUpdate();
    }
}

void NotificationManagerService::HandleRankingConfigChange()
{
    synchronized(mNotificationList) {
        Int32 N;
        mNotificationList->GetSize(&N);
        AutoPtr<IArrayList> orderBefore;
        CArrayList::New(N, (IArrayList**)&orderBefore);
        AutoPtr< ArrayOf<Int32> > visibilities = ArrayOf<Int32>::Alloc(N);
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IInterface> obj;
            mNotificationList->Get(i, (IInterface**)&obj);
            AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);
            orderBefore->Add(CoreUtils::Convert(r->GetKey()));
            (*visibilities)[i] = r->GetPackageVisibilityOverride();
            mRankingHelper->ExtractSignals(r);
        }

        for (Int32 i = 0; i < N; i++) {
            mRankingHelper->Sort(mNotificationList);
            AutoPtr<IInterface> obj;
            mNotificationList->Get(i, (IInterface**)&obj);
            AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);
            obj = NULL;
            orderBefore->Get(i, (IInterface**)&obj);
            String str;
            ICharSequence::Probe(IInterface)->ToString(&str);
            if (!str.Equals(r->GetKey())
                    || (*visibilities)[i] != r->GetPackageVisibilityOverride()) {
                ScheduleSendRankingUpdate();
                return;
            }
        }
    }
}

void NotificationManagerService::ApplyZenModeLocked(
    /* [in] */ NotificationRecord* record)
{
    record->SetIntercepted(mZenModeHelper->ShouldIntercept(record));
}

Int32 NotificationManagerService::FindNotificationRecordIndexLocked(
    /* [in] */ NotificationRecord* target)
{
    return mRankingHelper->IndexOf(mNotificationList, target);
}

void NotificationManagerService::ScheduleSendRankingUpdate()
{
    mHandler->RemoveMessages(MESSAGE_SEND_RANKING_UPDATE);

    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> m;
    helper->Obtain(mHandler, MESSAGE_SEND_RANKING_UPDATE, (IMessage**)&m);
    mHandler->SendMessage(m);
}

void NotificationManagerService::HandleSendRankingUpdate()
{
    synchronized(mNotificationList) {
        mListeners->NotifyRankingUpdateLocked();
    }
}

void NotificationManagerService::ScheduleListenerHintsChanged(
    /* [in] */ Int32 state)
{
    mHandler->RemoveMessages(MESSAGE_LISTENER_HINTS_CHANGED);
    AutoPtr<IMessage> m;
    mHandler->ObtainMessage(MESSAGE_LISTENER_HINTS_CHANGED, state, 0, (IMessage**)&m);
    m->SendToTarget();
}

void NotificationManagerService::ScheduleInterruptionFilterChanged(
    /* [in] */ Int32 listenerInterruptionFilter)
{
    mHandler->RemoveMessages(MESSAGE_LISTENER_NOTIFICATION_FILTER_CHANGED);
    AutoPtr<IMessage> m;
    mHandler->ObtainMessage(
            MESSAGE_LISTENER_NOTIFICATION_FILTER_CHANGED,
            listenerInterruptionFilter,
            0, (IMessage**)&m);
    m->SendToTarget();
}

void NotificationManagerService::HandleListenerHintsChanged(
    /* [in] */ Int32 hints)
{
    synchronized(mNotificationList) {
        mListeners->NotifyListenerHintsChangedLocked(hints);
    }
}

void NotificationManagerService::HandleListenerInterruptionFilterChanged(
    /* [in] */ Int32 interruptionFilter)
{
    synchronized(mNotificationList) {
        mListeners->NotifyInterruptionFilterChanged(interruptionFilter);
    }
}

// Notifications
// ============================================================================

Int32 NotificationManagerService::Clamp(
    /* [in] */ Int32 x,
    /* [in] */ Int32 low,
    /* [in] */ Int32 high)
{
    return (x < low) ? low : ((x > high) ? high : x);
}

void NotificationManagerService::SendAccessibilityEvent(
    /* [in] */ INotification* notification,
    /* [in] */ ICharSequence* packageName)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IAccessibilityManagerHelper> amHelper;
    CAccessibilityManagerHelper::AcquireSingleton((IAccessibilityManagerHelper**)&amHelper);
    AutoPtr<IAccessibilityManager> manager;
    amHelper->GetInstance(context, (IAccessibilityManager**)&manager);

    Boolean isEnabled;
    manager->IsEnabled(&isEnabled);
    if (isEnabled == FALSE) {
        return;
    }

    AutoPtr<IAccessibilityEventHelper> eventHelper;
    CAccessibilityEventHelper::AcquireSingleton((IAccessibilityEventHelper**)&eventHelper);

    AutoPtr<IAccessibilityEvent> event;
    eventHelper->Obtain(IAccessibilityEvent::TYPE_NOTIFICATION_STATE_CHANGED, (IAccessibilityEvent**)&event);

    AutoPtr<ICharSequence> clsSeq;
    CString::New(String("CNotification"), (ICharSequence**)&clsSeq);
    event->SetPackageName(packageName);
    event->SetClassName(clsSeq);
    event->SetParcelableData(IParcelable::Probe(notification));

    AutoPtr<ICharSequence> tickerText;
    notification->GetTickerText((ICharSequence**)&tickerText);

    if (!TextUtils::IsEmpty(tickerText)) {
        AutoPtr<IList> list;
        IAccessibilityRecord::Probe(event)->GetText((List**)&list);
        list->Add(tickerText);
    }

    manager->SendAccessibilityEvent(event);
}

void NotificationManagerService::CancelNotificationLocked(
    /* [in] */ NotificationRecord* record,
    /* [in] */ Boolean sendDelete,
    /* [in] */ Int32 reason)
{
    // tell the app
    AutoPtr<INotification> notification;
    record->GetNotification((INotification**)&notification);
    if (sendDelete) {
        AutoPtr<IPendingIntent> intent;
        notification->GetDeleteIntent((IPendingIntent**)&intent);

        if (intent != NULL) {
            ECode ec = intent->Send();

            if (FAILED(ec)) {
                // do nothing - there's no relevant way to recover, and
                //     no reason to let this propagate
                String str;
                record->mSbn->GetPackageName(&str);
                Logger::W(TAG, "canceled PendingIntent for %s", str);
            }
        }
    }

    // status bar
    Int32 icon;
    notification->GetIcon(&icon);

    if (icon != 0) {
        r->IsCanceled = TRUE;
        mListeners->NotifyRemovedLocked(record->mSbn);
    }

    // sound
    if (mSoundNotification.Get() == record) {
        mSoundNotification = NULL;

        Int64 identity;
        Binder::ClearCallingIdentity(&identity);
        AutoPtr<IIRingtonePlayer> player;
        mAudioManager->GetRingtonePlayer((IIRingtonePlayer**)&player);
        if (player != NULL) {
            player->StopAsync();
        }
        Binder::RestoreCallingIdentity(identity);
    }

    // vibrate
    if (mVibrateNotification.Get() == record) {
        mVibrateNotification = NULL;

        Int64 identity;
        Binder::ClearCallingIdentity(&identity);
        mVibrator->Cancel();
        Binder::RestoreCallingIdentity(identity);
    }

    // light
    mLights.Remove(CoreUtils::Convert(record->GetKey()));
    if (mLedNotification.Get() == record) {
        mLedNotification = NULL;
    }

    // Record usage stats
    switch (reason) {
        case REASON_DELEGATE_CANCEL:
        case REASON_DELEGATE_CANCEL_ALL:
        case REASON_LISTENER_CANCEL:
        case REASON_LISTENER_CANCEL_ALL:
            mUsageStats->RegisterDismissedByUser(record);
            break;
        case REASON_NOMAN_CANCEL:
        case REASON_NOMAN_CANCEL_ALL:
            mUsageStats->RegisterRemovedByApp(record);
            break;
        case REASON_DELEGATE_CLICK:
            mUsageStats->RegisterCancelDueToClick(record);
            break;
        default:
            mUsageStats->RegisterCancelUnknown(record);
            break;
    }

    mNotificationsByKey->Remove(CoreUtils::Convert(record->mSbn->GetKey()));

    // Save it for users of getHistoricalNotifications()
    mArchive->Record(record->mSbn);
}

void NotificationManagerService::CancelNotification(
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 callingPid,
    /* [in] */ const String& pkg,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ Int32 mustHaveFlags,
    /* [in] */ Int32 mustNotHaveFlags,
    /* [in] */ Boolean sendDelete,
    /* [in] */ Int32 userId,
    /* [in] */ Int32 reason,
    /* [in] */ ManagedServiceInfo* listener)
{
    // In enqueueNotificationInternal notifications are added by scheduling the
    // work on the worker handler. Hence, we also schedule the cancel on this
    // handler to avoid a scenario where an add notification call followed by a
    // remove notification call ends up in not removing the notification.
    AutoPtr<CancelNotificationRunnable> runnable = new CancelNotificationRunnable(
            this, callingUid, callingPid, pkg, tag, id, mustHaveFlags,
            mustNotHaveFlags, sendDelete, userId, reason, listener);
    Boolean res;
    mHost->mHandler->Post(runnable, &res);
}

Boolean NotificationManagerService::NotificationMatchesUserId(
    /* [in] */ NotificationRecord* record,
    /* [in] */ Int32 userId)
{
    return
      // looking for USER_ALL notifications? match everything
      userId == IUserHandle::USER_ALL
      // a notification sent to USER_ALL matches any query
      || record->GetUserId() == IUserHandle::USER_ALL
      // an exact user match
      || record->GetUserId() == userId;
}

Boolean NotificationManagerService::NotificationMatchesCurrentProfiles(
    /* [in] */ NotificationRecord* record,
    /* [in] */ Int32 userId)
{
    return NotificationMatchesUserId(record, userId)
            || mUserProfiles->IsCurrentProfile(record->GetUserId());
}

Boolean NotificationManagerService::CancelAllNotificationsInt(
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 callingPid,
    /* [in] */ const String& pkg,
    /* [in] */ Int32 mustHaveFlags,
    /* [in] */ Int32 mustNotHaveFlags,
    /* [in] */ Boolean doit,
    /* [in] */ Int32 userId,
    /* [in] */ Int32 reason,
    /* [in] */ ManagedServiceInfo* listener)
{
    String listenerName;
    if (listener == NULL) {
        listenerName = String(NULL);
    }
    else {
        listener->mComponent->ToShortString(&listenerName);
    }

    EventLogTags::WriteNotificationCancelAll(callingUid, callingPid,
            pkg, userId, mustHaveFlags, mustNotHaveFlags, reason,
            listenerName);

    synchronized(mNotificationList) {
        Int32 N;
        mNotificationList->GetSize(&N);
        AutoPtr<IArrayList> canceledNotifications;
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IInterface> obj;
            mNotificationList->Get(i, (IInterface**)&obj);
            AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);

            if (!NotificationMatchesUserId(r, userId)) {
                continue;
            }

            // Don't remove notifications to all, if there's no package name specified
            if (r->GetUserId() == IUserHandle::USER_ALL && pkg.IsNull()) {
                continue;
            }

            if ((r->GetFlags() & mustHaveFlags) != mustHaveFlags) {
                continue;
            }

            if ((r->GetFlags() & mustNotHaveFlags) != 0) {
                continue;
            }

            String packageName;
            r->mSbn->GetPackageName(&packageName);
            if (!pkg.IsNull() && !packageName.Equals(pkg)) {
                continue;
            }

            if (canceledNotifications == NULL) {
                CArrayList::New((IArrayList**)&canceledNotifications);
            }
            canceledNotifications->Add((IObject*)r);

            if (!doit) {
                return TRUE;
            }

            mNotificationList->Remove(i);
            CancelNotificationLocked(r, FAILED, reason);
        }

        if (doit && canceledNotifications != NULL) {
            Int32 M;
            canceledNotifications->GetSize(&M);
            for (Int32 i = 0; i < M; i++) {
                AutoPtr<IInterface> obj;
                canceledNotifications->Get(i, (IInterface**)&obj);
                AutoPtr<NotificationRecord> record = (NotificationRecord*)IObject::Probe(obj);
                CancelGroupChildrenLocked(record, callingUid, callingPid, listenerName);
            }
        }

        if (canceledNotifications) {
            UpdateLightsLocked();
        }

        return canceledNotifications != NULL;
    }
}

void NotificationManagerService::CancelAllLocked(
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 userId,
    /* [in] */ Int32 reason,
    /* [in] */ ManagedServiceInfo* listener,
    /* [in] */ Boolean includeCurrentProfiles)
{
    String listenerName;
    if (listener == NULL) {
        listenerName = String(NULL);
    }
    else {
        listener->mComponent->ToShortString(&listenerName);
    }

    EventLogTags::WriteNotificationCancelAll(callingUid, callingPid,
            null, userId, 0, 0, reason, listenerName);

    AutoPtr<IArrayList> canceledNotifications;
    Int32 N;
    mNotificationList->GetSize(&N);
    for (Int32 i = N-1; i >= 0; i--) {
        AutoPtr<IInterface> obj;
        mNotificationList->Get(i, (IInterface**)&obj);
        AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);
        if (includeCurrentProfiles) {
            if (!NotificationMatchesCurrentProfiles(r, userId)) {
                continue;
            }
        }
        else {
            if (!NotificationMatchesUserId(r, userId)) {
                continue;
            }
        }

        if ((r->GetFlags() & (INotification::FLAG_ONGOING_EVENT
                        | INotification::FLAG_NO_CLEAR)) == 0) {
            mNotificationList->Remove(i);
            CancelNotificationLocked(r, TRUE, reason);
            // Make a note so we can cancel children later.
            if (canceledNotifications == NULL) {
                CArrayList::New((IArrayList**)&canceledNotifications);
            }
            canceledNotifications->Add(r);
        }
    }
    Int32 M = 0;
    if (canceledNotifications != NULL) {
        canceledNotifications->GetSize(&M);
    }
    for (Int32 i = 0; i < M; i++) {
        AutoPtr<IInterface> obj;
        canceledNotifications->Get(i, (IInterface**)&obj);
        AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);
        CancelGroupChildrenLocked(r, callingUid, callingPid,
                listenerName);
    }
    UpdateLightsLocked();
}

void NotificationManagerService::CancelGroupChildrenLocked(
    /* [in] */ NotificationRecord* r;
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 callingPid,
    /* [in] */ const String& listenerName)
{
    AutoPtr<INotification> n = r->GetNotification();
    Boolean res;
    if (n->IsGroupSummary(&res), !res) {
        return;
    }

    String pkg;
    r->mSbn->GetPackageName(&pkg);
    Int32 userId = r->GetUserId();

    if (pkg.IsNull()) {
        if (DBG) {
            Logger::E(TAG, "No package for group summary: %s", r->GetKey().string());
        }
        return;
    }

    Int32 N;
    mNotificationList->GetSize(&N);
    for (Int32 i = N - 1; i >= 0; i--) {
        AutoPtr<IInterface> obj;
        mNotificationList->Get(i, (IInterface**)&obj);
        AutoPtr<NotificationRecord> childR = (NotificationRecord*)IObject::Probe(obj);
        AutoPtr<IStatusBarNotification> childSbn = childR->mSbn;
        if ((childR->GetNotification()->IsGroupChild(&res), res) &&
                childR->GetGroupKey().Equals(r->GetGroupKey())) {
            //TODO
            // EventLogTags.WriteNotificationCancel(callingUid, callingPid,
            //         pkg, childSbn.getId(), childSbn.getTag(), userId, 0, 0,
            //         REASON_GROUP_SUMMARY_CANCELED, listenerName);
            mNotificationList->Remove(i);
            CancelNotificationLocked(childR, FALSE, REASON_GROUP_SUMMARY_CANCELED);
        }
    }
}

void NotificationManagerService::UpdateLightsLocked()
{
    // handle notification lights
    if (mLedNotification == NULL) {
        // get next notification, if any
        Int32 n;
        mLights->GetSize(&n);
        if (n > 0) {
            AutoPtr<IInterface> obj;
            mLights->Get(n - 1, (IInterface**)&obj);
            AutoPtr<IInterface> object;
            mNotificationsByKey->Get(obj, (IInterface**)&object);
            mLedNotification = (NotificationRecord*)IObject::Probe(object);
        }
    }

    // Don't flash while we are in a call or screen is on
    if (mLedNotification == NULL || mInCall || mScreenOn) {
        mNotificationLight->TurnOff();
        mStatusBar->NotificationLightOff();
    }
    else {
        AutoPtr<INotification> ledno;
        mLedNotification->mSbn->GetNotification((INotification**)&ledno);

        Int32 ledARGB, ledOnMS, ledOffMS, defaults;
        ledno->GetLedARGB(&ledARGB);
        ledno->GetLedOnMS(&ledOnMS);
        ledno->GetLedOffMS(&ledOffMS);
        ledno->GetDefaults(&defaults);

        if ((defaults & INotification::DEFAULT_LIGHTS) != 0) {
            ledARGB = mDefaultNotificationColor;
            ledOnMS = mDefaultNotificationLedOn;
            ledOffMS = mDefaultNotificationLedOff;
        }

        if (mNotificationPulseEnabled) {
            // pulse repeatedly
            mNotificationLight->SetFlashing(ledARGB, Light::LIGHT_FLASH_TIMED, ledOnMS, ledOffMS);
        }
        // let SystemUI make an independent decision
        mStatusBar->NotificationLightPulse(ledARGB, ledOnMS, ledOffMS);
    }
}

Int32 NotificationManagerService::IndexOfNotificationLocked(
    /* [in] */ const String& pkg,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ Int32 userId)
{
    AutoPtr<IArrayList> list = mNotificationLight;
    Int32 len;
    list->GetSize(&len);

    for (Int32 i = 0; i < len; i++) {
        AutoPtr<IInterface> obj;
        list->Get(i, (IInterface**)&obj);
        AutoPtr<NotificationRecord> r = (NotificationRecord*)IObject::Probe(obj);

        Int32 _id;
        r->mSbn->GetId(&id);
        if (!NotificationMatchesUserId(r, userId) || _id != id) {
            continue;
        }

        String _tag;
        r->mSbn->GetTag(&_tag);
        if (tag.IsNull()) {
            if (!_tag.IsNull()) {
                continue;
            }
        } else {
            if (!tag.Equals(_tag)) {
                continue;
            }
        }

        String _pkg;
        r->mSbn->GetPackageName(&_pkg);
        if (pkg.Equals(_pkg)) {
            return i;
        }
    }

    return -1;
}

Int32 NotificationManagerService::IndexOfNotificationLocked(
    /* [in] */ const String& key)
{
    Int32 N;
    mNotificationList->GetSize(&N);
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<IInterface> obj;
        mNotificationList->Get(i, (IInterface**)&obj);
        AutoPtr<NotificationRecord> record = (NotificationRecord*)IObject::Probe(obj);
        if (key.Equals(record->GetKey())) {
            return i;
        }
    }
    return -1;
}

void NotificationManagerService::UpdateNotificationPulse()
{
    synchronized(mNotificationList) {
        UpdateLightsLocked();
    }
}

Boolean NotificationManagerService::IsUidSystem(
    /* [in] */ Int32 uid)
{
    Int32 appid = UserHandle::GetAppId(uid);
    return (appid == IProcess::SYSTEM_UID || appid == IProcess::PHONE_UID || uid == 0);
}

Boolean NotificationManagerService::IsCallerSystem()
{
    return IsUidSystem(Binder::GetCallingUid());
}

ECode NotificationManagerService::CheckCallerIsSystem()
{
    if (IsCallerSystem()) {
        return NOERROR;
    }
    Slogger::E(TAG, "Disallowed call for uid %d", Binder::GetCallingUid());
    return E_SECURITY_EXCEPTION;
    // throw new SecurityException("Disallowed call for uid " + Binder::GetCallingUid());
}

ECode NotificationManagerService::CheckCallerIsSystemOrSameApp(
    /* [in] */ const String& pkg)
{
    if (IsCallerSystem()) {
        return NOERROR;
    }

    const Int32 uid = Binder::GetCallingUid();

    // try {
    AutoPtr<IIPackageManager> pm = AppGlobals::GetPackageManager();
    AutoPtr<IApplicationInfo> ai;
    ECode ec = pm->GetApplicationInfo(pkg, 0, UserHandle::GetCallingUserId(), (IApplicationInfo**)&ai);
    if (FAILED(ec)) {
        Slogger::E(TAG, "Unknown package %s, 0x%08x", pkg.string(), ec);
        return E_SECURITY_EXCEPTION;
        // } catch (RemoteException re) {
        //     throw new SecurityException("Unknown package " + pkg + "\n" + re);
        // }
    }
    if (ai == NULL) {
        Slogger::E(TAG, "Unknown package %s", pkg.string());
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Unknown package " + pkg);
    }
    Int32 appUid;
    ai->GetUid(&appUid);
    if (!UserHandle::IsSameApp(appUid, uid)) {
        Slogger::E(TAG, "Calling uid %d gave package %s which is owned by uid %d",
                uid, pkg.string(), appUid);
        return E_SECURITY_EXCEPTION;
        // throw new SecurityException("Calling uid " + uid + " gave package"
        //         + pkg + " which is owned by uid " + ai.uid);
    }

    return NOERROR;
}

String NotificationManagerService::CallStateToString(
    /* [in] */ Int32 state)
{
    switch (state) {
        case ITelephonyManager::CALL_STATE_IDLE:
            return String("CALL_STATE_IDLE");
        case ITelephonyManager::CALL_STATE_RINGING:
            return String("CALL_STATE_RINGING");
        case ITelephonyManager::CALL_STATE_OFFHOOK:
            return String("CALL_STATE_OFFHOOK");
        default:
            return String("CALL_STATE_UNKNOWN_") + StringUtils::Int32ToString(state);
    }
}

void NotificationManagerService::ListenForCallState()
{
    assert(0 && "TODO");
    // TelephonyManager.from(getContext()).listen(new PhoneStateListener() {
    //     @Override
    //     public void onCallStateChanged(int state, String incomingNumber) {
    //         if (mCallState == state) return;
    //         if (DBG) Slog.d(TAG, "Call state changed: " + callStateToString(state));
    //         mCallState = state;
    //     }
    // }, IPhoneStateListener::LISTEN_CALL_STATE);
}

AutoPtr<INotificationRankingUpdate> NotificationManagerService::MakeRankingUpdateLocked(
    /* [in] */ ManagedServiceInfo* info)
{
    Int32 speedBumpIndex = -1;
    Int32 N;
    mNotificationList->GetSize(&N);
    AutoPtr<IArrayList> keys;
    CArrayList::New(N, (IArrayList**)&keys);
    AutoPtr<IArrayList> interceptedKeys;
    CArrayList::New(N, (IArrayList**)&interceptedKeys);
    AutoPtr<IBundle> visibilityOverrides;
    CBundle::New((IBundle**)&visibilityOverrides);
    Int32 size;
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<IInterface> obj;
        mNotificationList->Get(i, (IInterface**)&obj);
        AutoPtr<NotificationRecord> record = (NotificationRecord*)IObject::Probe(obj);
        if (!IsVisibleToListener(record->mSbn, info)) {
            continue;
        }
        keys->Add(CoreUtils::Convert(record->mSbn->GetKey()));
        if (record->IsIntercepted()) {
            interceptedKeys->Add(CoreUtils::Convert(record->mSbn->GetKey()));
        }
        if (record->GetPackageVisibilityOverride()
                != INotificationListenerServiceRanking::VISIBILITY_NO_OVERRIDE) {
            visibilityOverrides->PutInt32(record->mSbn->GetKey(),
                    record->GetPackageVisibilityOverride());
        }
        // Find first min-prio notification for speedbump placement.
        AutoPtr<INotification> notification;
        record->mSbn->GetNotification((INotification**)&notification);
        Int32 priority;
        notification->GetPriority(&priority);
        if (speedBumpIndex == -1 &&
                // Intrusiveness trumps priority, hence ignore intrusives.
                !record->IsRecentlyIntrusive() &&
                // Currently, package priority is either PRIORITY_DEFAULT or PRIORITY_MAX, so
                // scanning for PRIORITY_MIN within the package bucket PRIORITY_DEFAULT
                // (or lower as a safeguard) is sufficient to find the speedbump index.
                // We'll have to revisit this when more package priority buckets are introduced.
                record->GetPackagePriority() <= INotification::PRIORITY_DEFAULT &&
                priority == INotification::PRIORITY_MIN) {
            keys->GetSize(&size);
            speedBumpIndex = size - 1;
        }
    }

    keys->GetSize(&size);
    AutoPtr< ArrayOf<IInterface*> > inArgs = ArrayOf<IInterface*>::Alloc(size);
    AutoPtr< ArrayOf<IInterface*> > outArgs;
    keys->ToArray(inArgs, (ArrayOf<IInterface*>**)&outArgs);
    outArgs->GetSize(&size);
    AutoPtr< ArrayOf<String> > keysAr = ArrayOf<String>::Alloc(size);
    for (Int32 i = 0; i < size; i++) {
        ICharSequence::Probe(*outArgs)[i]->ToString(&(*keysAr)[i]);
    }

    interceptedKeys->GetSize(&size);
    AutoPtr< ArrayOf<IInterface*> > inArray = ArrayOf<IInterface*>::Alloc(size);
    AutoPtr< ArrayOf<IInterface*> > outArray;
    interceptedKeys->ToArray(inArray, (ArrayOf<IInterface*>**)&outArray);
    outArray->GetSize(&size);
    AutoPtr< ArrayOf<String> > interceptedKeysAr = ArrayOf<String>::Alloc(size);
    for (Int32 i = 0; i < size; i++) {
        ICharSequence::Probe(*outArray)[i]->ToString(&(*interceptedKeysAr)[i]);
    }

    AutoPtr<INotificationRankingUpdate> update;
    CNotificationRankingUpdate::New(keysAr, interceptedKeysAr, visibilityOverrides,
            speedBumpIndex, (INotificationRankingUpdate**)&update);
    return update;
}

Boolean NotificationManagerService::IsVisibleToListener(
    /* [in] */ IStatusBarNotification* sbn,
    /* [in] */ ManagedServiceInfo* listener)
{
    Int32 uid;
    sbn->GetUserId(&uid);
    if (!listener->EnabledAndUserMatches(uid)) {
        return FALSE;
    }
    // TODO: remove this for older listeners.
    return TRUE;
}

} // Notification
} // Server
} // Droid
} // Elastos
