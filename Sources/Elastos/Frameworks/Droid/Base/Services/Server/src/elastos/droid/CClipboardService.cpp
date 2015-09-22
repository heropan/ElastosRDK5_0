
#include "CClipboardService.h"
#include "app/ActivityManagerNative.h"
#include "app/AppGlobals.h"
#include "os/Process.h"
#include "os/Binder.h"
#include "os/UserHandle.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::ICharSequence;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::App::ActivityManagerNative;
using Elastos::Droid::App::AppGlobals;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::Pm::IIPackageManager;
using Elastos::Droid::Content::Pm::IPackageInfo;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::CRemoteCallbackList;

namespace Elastos {
namespace Droid {
namespace Server {

CClipboardService::PerUserClipboard::PerUserClipboard(
    /* [in] */ Int32 userId)
    : mUserId(userId)
{
    ASSERT_SUCCEEDED(CRemoteCallbackList::New((IRemoteCallbackList**)&mPrimaryClipListeners));
}


CClipboardService::MyBroadcastReceiver::MyBroadcastReceiver(
    /* [in] */ CClipboardService* host)
    : mHost(host)
{}

ECode CClipboardService::MyBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (IIntent::ACTION_USER_REMOVED.Equals(action)) {
        Int32 value = 0;
        intent->GetInt32Extra(IIntent::EXTRA_USER_HANDLE, 0, &value);
        mHost->RemoveClipboard(value);
    }
    return NOERROR;
}


const String CClipboardService::TAG("ClipboardService");

ECode CClipboardService::constructor(
    /* [in] */ IContext *context)
{
    mContext = context;
    mAm = ActivityManagerNative::GetDefault();
    context->GetPackageManager((IPackageManager**)&mPm);
    AutoPtr<IBinder> permOwner;

    if (FAILED(mAm->NewUriPermissionOwner(String("clipboard"), (IBinder**)&permOwner))) {
        Slogger::W("clipboard", "AM dead");
    }

    mPermissionOwner = permOwner;

    // Remove the clipboard if a user is removed
    AutoPtr<IIntentFilter> userFilter;
    CIntentFilter::New((IIntentFilter**)&userFilter);
    userFilter->AddAction(IIntent::ACTION_USER_REMOVED);
    AutoPtr<IBroadcastReceiver> myBR = (IBroadcastReceiver*)new MyBroadcastReceiver(this);
    AutoPtr<IIntent> intent;
    mContext->RegisterReceiver(myBR, userFilter, (IIntent**)&intent);
    return NOERROR;
}

AutoPtr<CClipboardService::PerUserClipboard> CClipboardService::GetClipboard()
{
    return GetClipboard(UserHandle::GetCallingUserId());
}

AutoPtr<CClipboardService::PerUserClipboard> CClipboardService::GetClipboard(
    /* [in] */ Int32 userId)
{
    AutoLock Lock(mClipboardsLock);
    AutoPtr<PerUserClipboard> puc;
    HashMap<Int32, AutoPtr<PerUserClipboard> >::Iterator it = mClipboards.Find(userId);
    if (it != mClipboards.End()) {
        puc = it->mSecond;
    }
    if (puc == NULL) {
        puc = new PerUserClipboard(userId);
        mClipboards[userId] = puc;
    }
    return puc;
}

void CClipboardService::RemoveClipboard(
    /* [in] */ Int32 userId)
{
    AutoLock Lock(mClipboardsLock);
    mClipboards.Erase(userId);
}

ECode CClipboardService::SetPrimaryClip(
    /* [in] */ IClipData* clip)
{
    AutoLock Lock(_m_syncLock);
    Int32 itemCount;
    if (clip == NULL || (clip->GetItemCount(&itemCount), itemCount <= 0)) {
        Slogger::E(TAG, "No items");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("No items");
    }
    FAIL_RETURN(CheckDataOwnerLocked(clip, Binder::GetCallingUid()));
    ClearActiveOwnersLocked();
    AutoPtr<PerUserClipboard> clipboard = GetClipboard();
    clipboard->mPrimaryClip = clip;
    Int32 n;
    clipboard->mPrimaryClipListeners->BeginBroadcast(&n);
    for (Int32 i = 0; i < n; i++) {
        //try {
        AutoPtr<IOnPrimaryClipChangedListener> listener;
        clipboard->mPrimaryClipListeners->GetBroadcastItem(i, (IInterface**)&listener);
        listener->DispatchPrimaryClipChanged();
        //} catch (RemoteException e) {

            // The RemoteCallbackList will take care of removing
            // the dead object for us.
        //}
    }
    return clipboard->mPrimaryClipListeners->FinishBroadcast();
}

ECode CClipboardService::GetPrimaryClip(
    /* [in] */ const String& pkg,
    /* [out] */ IClipData** clip)
{
    VALIDATE_NOT_NULL(clip)
    *clip = NULL;

    AutoLock Lock(_m_syncLock);
    FAIL_RETURN(AddActiveOwnerLocked(Binder::GetCallingUid(), pkg));
    *clip = GetClipboard()->mPrimaryClip;
    REFCOUNT_ADD(*clip)

    return NOERROR;
}

ECode CClipboardService::GetPrimaryClipDescription(
    /* [out] */ IClipDescription** description)
{
    VALIDATE_NOT_NULL(description)
    *description = NULL;

    AutoLock Lock(_m_syncLock);
    AutoPtr<PerUserClipboard> clipboard = GetClipboard();
    if (clipboard->mPrimaryClip != NULL) {
        AutoPtr<IClipDescription> des;
        FAIL_RETURN(clipboard->mPrimaryClip->GetDescription((IClipDescription**)&des));
        *description = des;
        REFCOUNT_ADD(*description)
    }

    return NOERROR;
}

ECode CClipboardService::HasPrimaryClip(
    /* [out] */ Boolean* hasPrimaryClip)
{
    VALIDATE_NOT_NULL(hasPrimaryClip);
    AutoLock Lock(_m_syncLock);
    *hasPrimaryClip = GetClipboard()->mPrimaryClip != NULL;
    return NOERROR;
}

ECode CClipboardService::AddPrimaryClipChangedListener(
    /* [in] */ IOnPrimaryClipChangedListener* listener)
{
    AutoLock Lock(_m_syncLock);
    Boolean result;
    return GetClipboard()->mPrimaryClipListeners->Register(listener, &result);
}

ECode CClipboardService::RemovePrimaryClipChangedListener(
    /* [in] */ IOnPrimaryClipChangedListener* listener)
{
    AutoLock Lock(mClipboardsLock);
    Boolean result;
    return GetClipboard()->mPrimaryClipListeners->Unregister(listener, &result);
}

ECode CClipboardService::HasClipboardText(
    /* [out] */ Boolean* hasClipboardText)
{
    VALIDATE_NOT_NULL(hasClipboardText);

    AutoLock Lock(_m_syncLock);
    AutoPtr<PerUserClipboard> clipboard = GetClipboard();
    if (clipboard->mPrimaryClip != NULL) {
        AutoPtr<IClipDataItem> item;
        FAIL_RETURN(clipboard->mPrimaryClip->GetItemAt(0, (IClipDataItem**)&item))
        AutoPtr<ICharSequence> text;
        item->GetText((ICharSequence**)&text);
        if (text != NULL) {
            Int32 length;
            text->GetLength(&length);
            *hasClipboardText = length > 0;
            return NOERROR;
        }
    }
    *hasClipboardText = FALSE;
    return NOERROR;
}
//begin from this
ECode CClipboardService::CheckUriOwnerLocked(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 uid)
{
    String scheme;
    uri->GetScheme(&scheme);
    if (!String("content").Equals(scheme)) {
        return NOERROR;
    }
    Int64 ident = Binder::ClearCallingIdentity();
    //try {
    // This will throw SecurityException for us.
    Int32 res;
    ECode ec = mAm->CheckGrantUriPermission(uid, String(NULL), uri,
            IIntent::FLAG_GRANT_READ_URI_PERMISSION, &res);
    Binder::RestoreCallingIdentity(ident);
    return ec;
    //} catch (RemoteException e) {
    //} finally {
        // Binder::RestoreCallingIdentity(ident);
    //}
}

ECode CClipboardService::CheckItemOwnerLocked(
    /* [in] */ IClipDataItem* item,
    /* [in] */ Int32 uid)
{
    AutoPtr<IUri> uri;
    item->GetUri((IUri**)&uri);
    if (uri != NULL) {
        FAIL_RETURN(CheckUriOwnerLocked(uri, uid))
    }
    AutoPtr<IIntent> intent;
    item->GetIntent((IIntent**)&intent);
    uri = NULL;
    if (intent != NULL && (intent->GetData((IUri**)&uri), uri != NULL)) {
        return CheckUriOwnerLocked(uri, uid);
    }
    return NOERROR;
}

ECode CClipboardService::CheckDataOwnerLocked(
    /* [in] */ IClipData* data,
    /* [in] */ Int32 uid)
{
    VALIDATE_NOT_NULL(data);
    Int32 N;
    data->GetItemCount(&N);
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<IClipDataItem> item;
        data->GetItemAt(i, (IClipDataItem**)&item);
        FAIL_RETURN(CheckItemOwnerLocked(item, uid));
    }
    return NOERROR;
}

void CClipboardService::GrantUriLocked(
    /* [in] */ IUri* uri,
    /* [in] */ const String& pkg)
{
    Int64 ident = Binder::ClearCallingIdentity();
    //try {
    mAm->GrantUriPermissionFromOwner(mPermissionOwner, Process::MyUid(), pkg, uri,
            IIntent::FLAG_GRANT_READ_URI_PERMISSION);
    Binder::RestoreCallingIdentity(ident);
    //} catch (RemoteException e) {
    //} finally {
        // Binder::RestoreCallingIdentity(ident);
    //}
}

void CClipboardService::GrantItemLocked(
    /* [in] */ IClipDataItem* item,
    /* [in] */ const String& pkg)
{
    AutoPtr<IUri> uri;
    if (item->GetUri((IUri**)&uri), uri != NULL) {
        GrantUriLocked(uri, pkg);
    }
    AutoPtr<IIntent> intent;
    item->GetIntent((IIntent**)&intent);
    uri = NULL;
    if (intent != NULL && (intent->GetData((IUri**)&uri), uri != NULL)) {
        GrantUriLocked(uri, pkg);
    }
}

ECode CClipboardService::AddActiveOwnerLocked(
    /* [in] */ Int32 uid,
    /* [in] */ const String& pkg)
{
    AutoPtr<IIPackageManager> pm = AppGlobals::GetPackageManager();
    Int32 targetUserHandle = UserHandle::GetCallingUserId();
    Int64 oldIdentity = Binder::ClearCallingIdentity();
    //try {
    AutoPtr<IPackageInfo> pi;
    pm->GetPackageInfo(pkg, 0, targetUserHandle, (IPackageInfo**)&pi);
    if (pi == NULL) {
        //throw new IllegalArgumentException("Unknown package " + pkg);
        Slogger::E("clipboard", "Unknown package %s", pkg.string());
        Binder::RestoreCallingIdentity(oldIdentity);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IApplicationInfo> info;
    pi->GetApplicationInfo((IApplicationInfo**)&info);
    Int32 i;
    info->GetUid(&i);
    if (!UserHandle::IsSameApp(i, uid)) {
        //throw new SecurityException("Calling uid " + uid
        //        + " does not own package " + pkg);
        Slogger::E("clipboard", "Calling uid %d does not own package %s", uid, pkg.string());
        Binder::RestoreCallingIdentity(oldIdentity);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    //} catch (RemoteException e) {
        // Can't happen; the package manager is in the same process
    //} finally {
    Binder::RestoreCallingIdentity(oldIdentity);
    //}
    AutoPtr<PerUserClipboard> clipboard = GetClipboard();

    if (clipboard->mPrimaryClip != NULL
            && clipboard->mActivePermissionOwners.Find(pkg) == clipboard->mActivePermissionOwners.End()) {
        Int32 N;
        clipboard->mPrimaryClip->GetItemCount(&N);
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IClipDataItem> item;
            clipboard->mPrimaryClip->GetItemAt(i, (IClipDataItem**)&item);
            GrantItemLocked(item, pkg);
        }
        clipboard->mActivePermissionOwners.Insert(pkg);
    }

    return NOERROR;
}

void CClipboardService::RevokeUriLocked(
    /* [in] */ IUri* uri)
{
    Int64 ident = Binder::ClearCallingIdentity();
    //try {
    mAm->RevokeUriPermissionFromOwner(mPermissionOwner, uri,
            IIntent::FLAG_GRANT_READ_URI_PERMISSION
            | IIntent::FLAG_GRANT_WRITE_URI_PERMISSION);
    //} catch (RemoteException e) {
    //} finally {
    Binder::RestoreCallingIdentity(ident);
    //}
}

void CClipboardService::RevokeItemLocked(
    /* [in] */ IClipDataItem* item)
{
    AutoPtr<IUri> uri;
    item->GetUri((IUri**)&uri);
    if (uri != NULL) {
        RevokeUriLocked(uri);
    }
    AutoPtr<IIntent> intent;
    item->GetIntent((IIntent**)&intent);
    uri = NULL;
    if (intent != NULL && (intent->GetData((IUri**)&uri), uri != NULL)) {
        RevokeUriLocked(uri);
    }
}

void CClipboardService::ClearActiveOwnersLocked()
{
    AutoPtr<PerUserClipboard> clipboard = GetClipboard();
    clipboard->mActivePermissionOwners.Clear();
    if (clipboard->mPrimaryClip == NULL) {
        return;
    }
    Int32 N;
    clipboard->mPrimaryClip->GetItemCount(&N);
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<IClipDataItem> item;
        clipboard->mPrimaryClip->GetItemAt(i, (IClipDataItem**)&item);
        RevokeItemLocked(item);
    }
}


}//namespace Server
}//namesapce Droid
}//namespace Elastos
