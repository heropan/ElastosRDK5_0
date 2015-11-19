
#include "CContentService.h"
#include "elastos/droid/os/UserHandle.h"
#include "elastos/droid/os/Binder.h"
#include "Manifest.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::App::CActivityManagerHelper;
using Elastos::Droid::App::IActivityManagerHelper;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Content::IContentResolverHelper;
using Elastos::Droid::Content::CContentResolverHelper;
using Elastos::Droid::Content::ISyncStorageEngine;

namespace Elastos {
namespace Droid {
namespace Server {

const String CContentService::TAG("CContentService");
const Boolean CContentService::DBG = FALSE;

CContentService::ObserverCall::ObserverCall(
    /* [in] */ ObserverNode* node,
    /* [in] */ IIContentObserver* observer,
    /* [in] */ Boolean selfChange)
    : mNode(node)
    , mObserver(observer)
    , mSelfChange(selfChange)
{}

CContentService::ObserverNode::ObserverEntry::ObserverEntry(
    /* [in] */ ObserverNode* host,
    /* [in] */ IIContentObserver* observer,
    /* [in] */ Boolean notify,
    /* [in] */ Object* observersLock,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 userHandle)
    : mObserver(observer)
    , mUid(uid)
    , mPid(pid)
    , mNotifyForDescendants(notify)
    , mUserHandle(userHandle)
    , mHost(host)
    , mObserversLock(observersLock)
{
    // try {
    AutoPtr<IProxy> proxy = (IProxy*)observer->Probe(EIID_IProxy);
    if (proxy != NULL) {
        if (FAILED(proxy->LinkToDeath(this, 0))) {
            ProxyDied();
        }
    }
    // } catch (RemoteException e) {
    //     binderDied();
    // }
}

CAR_INTERFACE_IMPL(CContentService::ObserverNode::ObserverEntry, IProxyDeathRecipient)

ECode CContentService::ObserverNode::ObserverEntry::ProxyDied()
{
    AutoLock lock(mObserversLock);
    Boolean isRemove = mHost->RemoveObserverLocked(mObserver);
    return NOERROR;
}

void CContentService::ObserverNode::ObserverEntry::DumpLocked(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ArrayOf<String>* args,
    /* [in] */ const String& name,
    /* [in] */ const String& prefix,
    /* [in] */ HashMap<Int32, Int32>* pidCounts)
{
    (*pidCounts)[mPid] = (*pidCounts)[mPid] + 1;
    pw->PrintString(prefix);
    pw->PrintString(name);
    pw->PrintString(String(": pid="));
    pw->PrintInt32(mPid);
    pw->PrintString(String(" uid="));
    pw->PrintInt32(mUid);
    pw->PrintString(String(" user="));
    pw->PrintInt32(mUserHandle);
    pw->PrintString(String(" target="));
    String info;
    if (mObserver != NULL) {
        mObserver->ToString(&info);
    }
    pw->PrintString(info);
}

CContentService::ObserverNode::ObserverNode(
    /* [in] */ const String& name)
    : mName(name)
{}

void CContentService::ObserverNode::DumpLocked(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ArrayOf<String>* args,
    /* [in] */ const String& name,
    /* [in] */ const String& prefix,
    /* [in] */ ArrayOf<Int32>* counts,
    /* [in] */ HashMap<Int32, Int32>* pidCounts)
{
    String innerName;
    if (!mObservers.IsEmpty()) {
        if (name.Equals("")) {
            innerName = mName;
        }
        else {
            innerName = name;
            innerName += "/";
            innerName += mName;
        }
        List<AutoPtr<ObserverEntry> >::Iterator it1 = mObservers.Begin();
        for (; it1 != mObservers.End(); ++it1) {
            (*counts)[1]++;
            (*it1)->DumpLocked(fd, pw, args, innerName, prefix, pidCounts);
        }
    }

    if (!mChildren.IsEmpty()) {
        if (innerName.IsNull()) {
            if (String("").Equals(name)) {
                innerName = mName;
            }
            else {
                innerName = name;
                innerName += "/";
                innerName += mName;
            }
        }
        List<AutoPtr<ObserverNode> >::Iterator it2 = mChildren.Begin();
        for (; it2 != mChildren.End(); ++it2) {
            (*counts)[0]++;
            (*it2)->DumpLocked(fd, pw, args, innerName, prefix, counts, pidCounts);
        }
    }
}

String CContentService::ObserverNode::GetUriSegment(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 index)
{
    if (uri != NULL) {
        if (index == 0) {
            String s;
            uri->GetAuthority(&s);
            return s;
        }
        else {
            AutoPtr< ArrayOf<String> > pathSegments;
            ASSERT_SUCCEEDED(uri->GetPathSegments((ArrayOf<String>**)&pathSegments))
            assert(index > 0 && index <= pathSegments->GetLength());
            return (*pathSegments)[index - 1];
        }
    }
    else {
        return String(NULL);
    }
}

Int32 CContentService::ObserverNode::CountUriSegments(
    /* [in] */ IUri* uri)
{
    if (uri == NULL) {
        return 0;
    }
    AutoPtr< ArrayOf<String> > pathSegments;
    ASSERT_SUCCEEDED(uri->GetPathSegments((ArrayOf<String>**)&pathSegments))
    return pathSegments->GetLength() + 1;
}

ECode CContentService::ObserverNode::AddObserverLocked(
    /* [in] */ IUri* uri,
    /* [in] */ IIContentObserver* observer,
    /* [in] */ Boolean notifyForDescendants,
    /* [in] */ Object* observersLock,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 userHandle)
{
    return AddObserverLocked(uri, 0, observer, notifyForDescendants, observersLock, uid, pid, userHandle);
}

ECode CContentService::ObserverNode::AddObserverLocked(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 index,
    /* [in] */ IIContentObserver* observer,
    /* [in] */ Boolean notifyForDescendants,
    /* [in] */ Object* observersLock,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 userHandle)
{
    VALIDATE_NOT_NULL(observer);

    // If this is the leaf node add the observer
    if (index == CountUriSegments(uri)) {
        AutoPtr<ObserverEntry> oberverEntry = new ObserverEntry(
            this, observer, notifyForDescendants, observersLock, uid, pid, userHandle);
        mObservers.PushBack(oberverEntry);
        return NOERROR;
    }

    // Look to see if the proper child already exists
    String segment = GetUriSegment(uri, index);
    if (segment.IsNull()) {
        String uriStr, observerStr;
        uri->ToString(&uriStr);
        observer->ToString(&observerStr);
        Slogger::E(TAG, "Invalid Uri [%s] used for observer [%s]", uriStr.string(), observerStr.string());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    List<AutoPtr<ObserverNode> >::Iterator it = mChildren.Begin();
    for (; it != mChildren.End(); ++it) {
        AutoPtr<ObserverNode> node = *it;
        if (node->mName.Equals(segment)) {
            return node->AddObserverLocked(uri, index + 1, observer, notifyForDescendants,
                observersLock, uid, pid, userHandle);
        }
    }

    // No child found, create one
    AutoPtr<ObserverNode> node = new ObserverNode(segment);
    mChildren.PushBack(node);
    return node->AddObserverLocked(uri, index + 1, observer, notifyForDescendants,
        observersLock, uid, pid, userHandle);
}

Boolean CContentService::ObserverNode::RemoveObserverLocked(
    /* [in] */ IIContentObserver* observer)
{
    assert(observer != NULL);

    List<AutoPtr<ObserverNode> >::Iterator it = mChildren.Begin();
    while (it != mChildren.End()) {
        Boolean empty = (*it)->RemoveObserverLocked(observer);
        if (empty) {
            it = mChildren.Erase(it);
        }
        else {
            ++it;
        }
    }

    AutoPtr<IProxy> proxy = (IProxy*)observer->Probe(EIID_IProxy);
    if (proxy != NULL) {
        Boolean result;
        List<AutoPtr<ObserverEntry> >::Iterator it2 = mObservers.Begin();
        for (; it2 != mObservers.End();) {
            AutoPtr<ObserverEntry> entry = *it2;
            if (entry->mObserver.Get() == observer) {
                // We no longer need to listen for death notifications. Remove it.
                proxy->UnlinkToDeath(entry, 0, &result);
                it2 = mObservers.Erase(it2);
            }
            else {
                ++it2;
            }
        }
    }

    if (mChildren.IsEmpty() && mObservers.IsEmpty()) {
        return TRUE;
    }
    return FALSE;
}

void CContentService::ObserverNode::CollectMyObserversLocked(
    /* [in] */ Boolean leaf,
    /* [in] */ IIContentObserver* observer,
    /* [in] */ Boolean observerWantsSelfNotifications,
    /* [in] */ Int32 targetUserHandle,
    /* [in] */ List<AutoPtr<ObserverCall> >* calls)
{
    List<AutoPtr<ObserverEntry> >::Iterator it = mObservers.Begin();
    for (; it != mObservers.End(); ++it) {
        AutoPtr<ObserverEntry> entry = *it;

        // Don't notify the observer if it sent the notification and isn't interested
        // in self notifications
        Boolean selfChange = (entry->mObserver.Get() == observer);
        if (selfChange && !observerWantsSelfNotifications) {
            continue;
        }

        // Does this observer match the target user?
        if (targetUserHandle == IUserHandle::USER_ALL
                || entry->mUserHandle == IUserHandle::USER_ALL
                || targetUserHandle == entry->mUserHandle) {
            // Make sure the observer is interested in the notification
            if (leaf || (!leaf && entry->mNotifyForDescendants)) {
                AutoPtr<ObserverCall> call = new ObserverCall(this, entry->mObserver, selfChange);
                calls->PushBack(call);
            }
        }
    }
}

void CContentService::ObserverNode::CollectObserversLocked(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 index,
    /* [in] */ IIContentObserver* observer,
    /* [in] */ Boolean observerWantsSelfNotifications,
    /* [in] */ Int32 targetUserHandle,
    /* [in] */ List<AutoPtr<ObserverCall> >* calls)
{
    String segment;
    Int32 segmentCount = CountUriSegments(uri);
    List<AutoPtr<ObserverNode> >::Iterator it;
    if (index >= segmentCount) {
        // This is the leaf node, notify all observers
        CollectMyObserversLocked(TRUE, observer, observerWantsSelfNotifications, targetUserHandle, calls);
    }
    else if (index < segmentCount) {
        segment = GetUriSegment(uri, index);
        // Notify any observers at this level who are interested in descendants
        CollectMyObserversLocked(FALSE, observer, observerWantsSelfNotifications, targetUserHandle, calls);
    }

    List<AutoPtr<ObserverNode> >::Iterator it2 = mChildren.Begin();
    for (; it2 != mChildren.End(); it2++) {
        AutoPtr<ObserverNode> node = *it2;
        if (segment.IsNull() || node->mName.Equals(segment)) {
            node->CollectObserversLocked(uri, index + 1,
                    observer, observerWantsSelfNotifications, targetUserHandle, calls);
            if (!segment.IsNull()) {
                break;
            }
        }
    }
}

CContentService::CContentService()
    : mFactoryTest(FALSE)
{
    mRootNode = new ObserverNode(String(""));
}

AutoPtr<ISyncManager> CContentService::GetSyncManager()
{
    AutoLock lock(mSyncManagerLock);
    // try {
        // Try to create the SyncManager, return null if it fails (e.g. the disk is full).
    if (mSyncManager == NULL) {
        // mSyncManager = new SyncManager(mContext, mFactoryTest);
        Slogger::W(TAG, "TODO, SyncManager not implemented");
    }
    // } catch (SQLiteException e) {
    //     Log.e(TAG, "Can't create SyncManager", e);
    // }
    return mSyncManager;
}

ECode CContentService::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ArrayOf<String>* args)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

void CContentService::SystemReady()
{
    GetSyncManager();
}

ECode CContentService::RegisterContentObserver(
    /* [in] */ IUri* uri,
    /* [in] */ Boolean notifyForDescendants,
    /* [in] */ IIContentObserver* observer,
    /* [in] */ Int32 userHandle)
{
    if (NULL == observer || NULL == uri) {
        String uriStr, observerStr;
        if (uri) uri->ToString(&uriStr);
        if (observer) observer->ToString(&observerStr);
        Slogger::E(TAG, "RegisterContentObserver: You must pass a valid uri(%s) and observer(%s)",
            uriStr.string(), observerStr.string());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (DBG) {
        String uriStr, observerStr;
        uri->ToString(&uriStr);
        observer->ToString(&observerStr);
        Logger::V(TAG, "Registered observer %p : [%s] at [%s] with notifyForDescendants %d",
            observer, observerStr.string(), uriStr.string(), notifyForDescendants);
    }

    Int32 callingUser = UserHandle::GetCallingUserId();
    if (callingUser != userHandle) {
        FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::INTERACT_ACROSS_USERS_FULL,
            String("no permission to observe other users' provider view")));
    }

    if (userHandle < 0) {
        if (userHandle == IUserHandle::USER_CURRENT) {
            AutoPtr<IActivityManagerHelper> amHelper;
            CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&amHelper);
            FAIL_RETURN(amHelper->GetCurrentUser(&userHandle))
        }
        else if (userHandle != IUserHandle::USER_ALL) {
            Slogger::E(TAG, "Bad user handle for registerContentObserver: %d", userHandle);
            return E_INVALID_ARGUMENT;
        }
    }

    AutoLock lock(mRootNodeLock);
    FAIL_RETURN(mRootNode->AddObserverLocked(uri, observer, notifyForDescendants,
            &mRootNodeLock, Binder::GetCallingUid(), Binder::GetCallingPid(), userHandle));
    return NOERROR;
}

ECode CContentService::RegisterContentObserver(
    /* [in] */ IUri* uri,
    /* [in] */ Boolean notifyForDescendants,
    /* [in] */ IIContentObserver* observer)
{
    return RegisterContentObserver(uri, notifyForDescendants, observer,
            UserHandle::GetCallingUserId());
}

ECode CContentService::UnregisterContentObserver(
    /* [in] */ IIContentObserver* observer)
{
    if (observer == NULL) {
        Slogger::E(TAG, "UnregisterContentObserver: You must pass a valid observer.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (DBG) {
        String info;
        observer->ToString(&info);
        Slogger::V(TAG, "Unregistered observer %p : [%s]", observer, info.string());
    }

    AutoLock lock(mRootNodeLock);
    mRootNode->RemoveObserverLocked(observer);
    return NOERROR;
}

ECode CContentService::NotifyChange(
    /* [in] */ IUri* uri,
    /* [in] */ IIContentObserver* observer,
    /* [in] */ Boolean observerWantsSelfNotifications,
    /* [in] */ Boolean syncToNetwork,
    /* [in] */ Int32 userHandle)
{
    if (DBG) {
        String uriStr, observerStr;
        if (uri) uri->ToString(&uriStr);
        if (observer) observer->ToString(&observerStr);
        Logger::V(TAG, "Notifying update of %s for user %d from observer %s, syncToNetwork %d",
            uriStr.string(), userHandle, observerStr.string(), syncToNetwork);
    }

    // Notify for any user other than the caller's own requires permission.
    Int32 callingUserHandle = UserHandle::GetCallingUserId();
    if (userHandle != callingUserHandle) {
        FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::INTERACT_ACROSS_USERS_FULL,
                String("no permission to notify other users")))
    }

    // We passed the permission check; resolve pseudouser targets as appropriate
    if (userHandle < 0) {
        if (userHandle == IUserHandle::USER_CURRENT) {
            AutoPtr<IActivityManagerHelper> amHelper;
            FAIL_RETURN(CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&amHelper))
            FAIL_RETURN(amHelper->GetCurrentUser(&userHandle))
        }
        else if (userHandle != IUserHandle::USER_ALL) {
            Slogger::E(TAG, "Bad user handle for notifyChange: %d", userHandle);
            return E_INVALID_ARGUMENT;
            // throw new InvalidParameterException("Bad user handle for notifyChange: "
            //         + userHandle);
        }
    }

    // This makes it so that future permission checks will be in the context of this
    // process rather than the caller's process. We will restore this before returning.
    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    List<AutoPtr<ObserverCall> > calls;
    {
        AutoLock lock(mRootNodeLock);
        mRootNode->CollectObserversLocked(uri, 0, observer, observerWantsSelfNotifications,
                userHandle, &calls);
    }
    List<AutoPtr<ObserverCall> >::Iterator it = calls.Begin();
    for (; it != calls.End(); ++it) {
        AutoPtr<ObserverCall> oc = *it;
        // try {
        if (FAILED(oc->mObserver->OnChange(oc->mSelfChange, uri))) {
            AutoLock lock(mRootNodeLock);
            Slogger::W(TAG, "Found dead observer, removing");
            List<AutoPtr<ObserverNode::ObserverEntry> >& list = oc->mNode->mObservers;
            List<AutoPtr<ObserverNode::ObserverEntry> >::Iterator it2 = list.Begin();
            while (it2 != list.End()) {
                AutoPtr<ObserverNode::ObserverEntry> oe = *it2;
                if (oe->mObserver == oc->mObserver) {
                    it2 = list.Erase(it2);
                }
                else {
                    ++it2;
                }
            }
        }
        // if (Log.isLoggable(TAG, Log.VERBOSE)) {
        //     Log.v(TAG, "Notified " + oc.mObserver + " of " + "update at " + uri);
        // }
        // } catch (RemoteException ex) {
        //     synchronized(mRootNode) {
        //         Log.w(TAG, "Found dead observer, removing");
        //         IBinder binder = oc.mObserver.asBinder();
        //         final ArrayList<ObserverNode.ObserverEntry> list
        //                 = oc.mNode.mObservers;
        //         int numList = list.size();
        //         for (int j=0; j<numList; j++) {
        //             ObserverNode.ObserverEntry oe = list.get(j);
        //             if (oe.observer.asBinder() == binder) {
        //                 list.remove(j);
        //                 j--;
        //                 numList--;
        //             }
        //         }
        //     }
        // }
    }
    if (syncToNetwork) {
        AutoPtr<ISyncManager> syncManager = GetSyncManager();
        if (syncManager != NULL) {
            String auth;
            uri->GetAuthority(&auth);
            ECode ec = syncManager->ScheduleLocalSync(NULL /* all accounts */, callingUserHandle, auth);
            if (FAILED(ec)) {
                Binder::RestoreCallingIdentity(identityToken);
                return ec;
            }
        }
    }
    // } finally {
    Binder::RestoreCallingIdentity(identityToken);
    // }
    return NOERROR;
}

ECode CContentService::NotifyChange(
    /* [in] */ IUri* uri,
    /* [in] */ IIContentObserver* observer,
    /* [in] */ Boolean observerWantsSelfNotifications,
    /* [in] */ Boolean syncToNetwork)
{
    return NotifyChange(uri, observer, observerWantsSelfNotifications, syncToNetwork,
            UserHandle::GetCallingUserId());
}

ECode CContentService::RequestSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras)
{
    AutoPtr<IContentResolverHelper> contentResolverHelper;
    FAIL_RETURN(CContentResolverHelper::AcquireSingleton((IContentResolverHelper**)&contentResolverHelper))
    FAIL_RETURN(contentResolverHelper->ValidateSyncExtrasBundle(extras))
    Int32 userId = UserHandle::GetCallingUserId();

    // This makes it so that future permission checks will be in the context of this
    // process rather than the caller's process. We will restore this before returning.
    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        ec = syncManager->ScheduleSync(account, userId, authority, extras,
                0 /* no delay */, FALSE /* onlyThoseWithUnkownSyncableState */);
    }
    // } finally {
    //     restoreCallingIdentity(identityToken);
    // }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::CancelSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority)
{
    Int32 userId = UserHandle::GetCallingUserId();

    // This makes it so that future permission checks will be in the context of this
    // process rather than the caller's process. We will restore this before returning.
    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        ec = syncManager->ClearScheduledSyncOperations(account, userId, authority);
        ec = syncManager->CancelActiveSync(account, userId, authority);
    }
    // } finally {
    // RestoreCallingIdentity(identityToken);
    // }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::GetSyncAdapterTypes(
    /* [out, callee] */ ArrayOf<ISyncAdapterType*>** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;
    // This makes it so that future permission checks will be in the context of this
    // process rather than the caller's process. We will restore this before returning.
    Int32 userId = UserHandle::GetCallingUserId();
    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        ec = syncManager->GetSyncAdapterTypes(userId, result);
    }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
    // } finally {
    //     restoreCallingIdentity(identityToken);
    // }
}

ECode CContentService::GetSyncAutomatically(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::READ_SYNC_SETTINGS,
            String("no permission to read the sync settings")));
    Int32 userId = UserHandle::GetCallingUserId();

    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        AutoPtr<ISyncStorageEngine> engine;
        ec = syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine);
        ec = engine->GetSyncAutomatically(account, userId, providerName, result);
    }
    // } finally {
    //     restoreCallingIdentity(identityToken);
    // }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::SetSyncAutomatically(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ Boolean sync)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::WRITE_SYNC_SETTINGS,
            String("no permission to write the sync settings")))
    Int32 userId = UserHandle::GetCallingUserId();

    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        AutoPtr<ISyncStorageEngine> engine;
        ec = syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine);
        ec = engine->SetSyncAutomatically(account, userId, providerName, sync);
    }
    // } finally {
    //     restoreCallingIdentity(identityToken);
    // }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::AddPeriodicSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 pollFrequency)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::WRITE_SYNC_SETTINGS,
            String("no permission to write the sync settings")))
    Int32 userId = UserHandle::GetCallingUserId();

    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        AutoPtr<ISyncStorageEngine> engine;
        ec = syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine);
        ec = engine->AddPeriodicSync(account, userId, authority, extras, pollFrequency);
    }
    // } finally {
    //     restoreCallingIdentity(identityToken);
    // }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::RemovePeriodicSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::WRITE_SYNC_SETTINGS,
            String("no permission to write the sync settings")))
    Int32 userId = UserHandle::GetCallingUserId();

    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        AutoPtr<ISyncStorageEngine> engine;
        ec = syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine);
        ec = engine->RemovePeriodicSync(account, userId, authority, extras);
    }
    // } finally {
    //     restoreCallingIdentity(identityToken);
    // }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::GetPeriodicSyncs(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [out] */ IObjectContainer** periodicSyncList)
{
    VALIDATE_NOT_NULL(periodicSyncList);
    *periodicSyncList = NULL;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::READ_SYNC_SETTINGS,
            String("no permission to read the sync settings")))
    Int32 userId = UserHandle::GetCallingUserId();

    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        AutoPtr<ISyncStorageEngine> engine;
        ec = syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine);
        ec = engine->GetPeriodicSyncs(account, userId, providerName, periodicSyncList);
    }
    // } finally {
    //     restoreCallingIdentity(identityToken);
    // }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::GetIsSyncable(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    *value = -1;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::READ_SYNC_SETTINGS,
            String("no permission to read the sync settings")))
    Int32 userId = UserHandle::GetCallingUserId();

    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        AutoPtr<ISyncStorageEngine> engine;
        ec = syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine);
        ec = engine->GetIsSyncable(account, userId, providerName, value);
    }
    // } finally {
    //     restoreCallingIdentity(identityToken);
    // }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::SetIsSyncable(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ Int32 syncable)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::WRITE_SYNC_SETTINGS,
            String("no permission to write the sync settings")))
    Int32 userId = UserHandle::GetCallingUserId();

    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        AutoPtr<ISyncStorageEngine> engine;
        ec = syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine);
        ec = engine->SetIsSyncable(account, userId, providerName, syncable);
    }
    // } finally {
    //     restoreCallingIdentity(identityToken);
    // }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::GetMasterSyncAutomatically(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::READ_SYNC_SETTINGS,
            String("no permission to read the sync settings")))
    Int32 userId = UserHandle::GetCallingUserId();

    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        AutoPtr<ISyncStorageEngine> engine;
        ec = syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine);
        ec = engine->GetMasterSyncAutomatically(userId, result);
    }
    // } finally {
    //     restoreCallingIdentity(identityToken);
    // }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::SetMasterSyncAutomatically(
    /* [in] */ Boolean flag)
{
    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::WRITE_SYNC_SETTINGS,
            String("no permission to write the sync settings")))
    Int32 userId = UserHandle::GetCallingUserId();

    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        AutoPtr<ISyncStorageEngine> engine;
        ec = syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine);
        ec = engine->SetMasterSyncAutomatically(flag, userId);
    }
    // } finally {
    //     restoreCallingIdentity(identityToken);
    // }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::IsSyncActive(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ Boolean* isActive)
{
    VALIDATE_NOT_NULL(isActive)
    *isActive = FALSE;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::READ_SYNC_STATS,
            String("no permission to read the sync stats")))
    Int32 userId = UserHandle::GetCallingUserId();

    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        AutoPtr<ISyncStorageEngine> engine;
        ec = syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine);
        ec = engine->IsSyncActive(account, userId, authority, isActive);
    }
    // } finally {
    //     restoreCallingIdentity(identityToken);
    // }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::GetCurrentSyncs(
    /* [out] */ IObjectContainer** syncInfoList)
{
    VALIDATE_NOT_NULL(syncInfoList)
    *syncInfoList = NULL;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::READ_SYNC_STATS,
            String("no permission to read the sync stats")))
    Int32 userId = UserHandle::GetCallingUserId();

    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        AutoPtr<ISyncStorageEngine> engine;
        ec = syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine);
        ec = engine->GetCurrentSyncs(userId, syncInfoList);
    }
    // } finally {
    //     restoreCallingIdentity(identityToken);
    // }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::GetSyncStatus(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ ISyncStatusInfo** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::READ_SYNC_STATS,
            String("no permission to read the sync stats")))
    Int32 userId = UserHandle::GetCallingUserId();

    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        AutoPtr<ISyncStorageEngine> engine;
        ec = syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine);
        ec = engine->GetStatusByAccountAndAuthority(account, userId, authority, result);
    }
    // } finally {
    //     restoreCallingIdentity(identityToken);
    // }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::IsSyncPending(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ Boolean* isPending)
{
    VALIDATE_NOT_NULL(isPending)
    *isPending = FALSE;

    FAIL_RETURN(mContext->EnforceCallingOrSelfPermission(Elastos::Droid::Manifest::permission::READ_SYNC_STATS,
            String("no permission to read the sync stats")))
    Int32 userId = UserHandle::GetCallingUserId();

    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL) {
        AutoPtr<ISyncStorageEngine> engine;
        ec = syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine);
        ec = engine->IsSyncPending(account, userId, authority, isPending);
    }
    // } finally {
    //     restoreCallingIdentity(identityToken);
    // }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::AddStatusChangeListener(
    /* [in] */ Int32 mask,
    /* [in] */ IISyncStatusObserver* callback)
{
    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL && callback != NULL) {
        AutoPtr<ISyncStorageEngine> engine;
        ec = syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine);
        ec = engine->AddStatusChangeListener(mask, callback);
    }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::RemoveStatusChangeListener(
    /* [in] */ IISyncStatusObserver* callback)
{
    Int64 identityToken = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    ECode ec = NOERROR;
    if (syncManager != NULL && callback != NULL) {
        AutoPtr<ISyncStorageEngine> engine;
        ec = syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine);
        ec = engine->RemoveStatusChangeListener(callback);
    }
    Binder::RestoreCallingIdentity(identityToken);
    return ec;
}

ECode CContentService::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Boolean factoryTest)
{
    mContext = context;
    mFactoryTest = factoryTest;
    return NOERROR;
}

}
}
}

