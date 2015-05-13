
#ifndef __CCONTENTSERVICE_H__
#define __CCONTENTSERVICE_H__

#include "_CContentService.h"
#include "ext/frameworkdef.h"
#include <elastos/Mutex.h>
#include <elastos/List.h>
#include <elastos/HashMap.h>

using Elastos::Utility::List;
using Elastos::Utility::HashMap;
using Elastos::Core::Threading::Mutex;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IPrintWriter;
using Elastos::Droid::Accounts::IAccount;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Database::IIContentObserver;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::ISyncAdapterType;
using Elastos::Droid::Content::ISyncManager;
using Elastos::Droid::Content::ISyncStatusInfo;
using Elastos::Droid::Content::IISyncStatusObserver;

namespace Elastos {
namespace Droid {
namespace Server {

CarClass(CContentService)
{
public:
    class ObserverNode;
    /**
     * Hide this class since it is not part of api,
     * but current unittest framework requires it to be public
     * @hide
     *
     */
    class ObserverCall: public ElRefBase
    {
    public:
        ObserverCall(
            /* [in] */ ObserverNode* node,
            /* [in] */ IIContentObserver* observer,
            /* [in] */ Boolean selfChange);

    public:
        AutoPtr<ObserverNode> mNode;
        AutoPtr<IIContentObserver> mObserver;
        Boolean mSelfChange;
    };

    /**
     * Hide this class since it is not part of api,
     * but current unittest framework requires it to be public
     * @hide
     */
    class ObserverNode: public ElRefBase
    {
    public:
        class ObserverEntry
            : public ElRefBase
            , public IProxyDeathRecipient
        {
        public:
            ObserverEntry(
                /* [in] */ ObserverNode* host,
                /* [in] */ IIContentObserver* observer,
                /* [in] */ Boolean notify,
                /* [in] */ Mutex* observersLock,
                /* [in] */ Int32 uid,
                /* [in] */ Int32 pid,
                /* [in] */ Int32 userHandle);

            CAR_INTERFACE_DECL()

            CARAPI ProxyDied();

            CARAPI_(void) DumpLocked(
                /* [in] */ IFileDescriptor* fd,
                /* [in] */ IPrintWriter* pw,
                /* [in] */ ArrayOf<String>* args,
                /* [in] */ const String& name,
                /* [in] */ const String& prefix,
                /* [in] */ HashMap<Int32, Int32>* pidCounts);

        public:
            AutoPtr<IIContentObserver> mObserver;
            Int32 mUid;
            Int32 mPid;
            Boolean mNotifyForDescendants;
            Int32 mUserHandle;

        private:
            ObserverNode* mHost;
            Mutex* mObserversLock;
        };

    public:
        ObserverNode(
            /* [in] */ const String& name);

        CARAPI_(void) DumpLocked(
            /* [in] */ IFileDescriptor* fd,
            /* [in] */ IPrintWriter* pw,
            /* [in] */ ArrayOf<String>* args,
            /* [in] */ const String& name,
            /* [in] */ const String& prefix,
            /* [in] */ ArrayOf<Int32>* counts,
            /* [in] */ HashMap<Int32, Int32>* pidCounts);

        // Invariant:  userHandle is either a hard user number or is USER_ALL
        CARAPI AddObserverLocked(
            /* [in] */ IUri* uri,
            /* [in] */ IIContentObserver* observer,
            /* [in] */ Boolean notifyForDescendants,
            /* [in] */ Mutex* observersLock,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 pid,
            /* [in] */ Int32 userHandle);

        CARAPI_(Boolean) RemoveObserverLocked(
            /* [in] */ IIContentObserver* observer);

        /**
         * targetUserHandle is either a hard user handle or is USER_ALL
         */
        CARAPI_(void) CollectObserversLocked(
            /* [in] */ IUri* uri,
            /* [in] */ Int32 index,
            /* [in] */ IIContentObserver* observer,
            /* [in] */ Boolean observerWantsSelfNotifications,
            /* [in] */ Int32 targetUserHandle,
            /* [in] */ List<AutoPtr<ObserverCall> >* calls);

    private:
        CARAPI_(String) GetUriSegment(
            /* [in] */ IUri* uri,
            /* [in] */ Int32 index);

        CARAPI_(Int32) CountUriSegments(
            /* [in] */ IUri* uri);

        CARAPI AddObserverLocked(
            /* [in] */ IUri* uri,
            /* [in] */ Int32 index,
            /* [in] */ IIContentObserver* observer,
            /* [in] */ Boolean notifyForDescendants,
            /* [in] */ Mutex* observersLock,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 pid,
            /* [in] */ Int32 userHandle);

        CARAPI_(void) CollectMyObserversLocked(
            /* [in] */ Boolean leaf,
            /* [in] */ IIContentObserver* observer,
            /* [in] */ Boolean observerWantsSelfNotifications,
            /* [in] */ Int32 targetUserHandle,
            /* [in] */ List<AutoPtr<ObserverCall> >* calls);

    public:
        static const Int32 INSERT_TYPE = 0;
        static const Int32 UPDATE_TYPE = 1;
        static const Int32 DELETE_TYPE = 2;

    public:
        String mName;
        List<AutoPtr<ObserverNode> > mChildren;
        List<AutoPtr<ObserverEntry> > mObservers;
    };

public:
    CContentService();

    CARAPI Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args);

    CARAPI_(void) SystemReady();

    /**
     * Register a content observer tied to a specific user's view of the provider.
     * @param userHandle the user whose view of the provider is to be observed.  May be
     *     the calling user without requiring any permission, otherwise the caller needs to
     *     hold the INTERACT_ACROSS_USERS_FULL permission.  Pseudousers USER_ALL and
     *     USER_CURRENT are properly handled; all other pseudousers are forbidden.
     */
    CARAPI RegisterContentObserver(
        /* [in] */ IUri* uri,
        /* [in] */ Boolean notifyForDescendants,
        /* [in] */ IIContentObserver* observer,
        /* [in] */ Int32 userHandle);

    CARAPI RegisterContentObserver(
        /* [in] */ IUri* uri,
        /* [in] */ Boolean notifyForDescendants,
        /* [in] */ IIContentObserver* observer);

    CARAPI UnregisterContentObserver(
        /* [in] */ IIContentObserver* observer);

    /**
     * Notify observers of a particular user's view of the provider.
     * @param userHandle the user whose view of the provider is to be notified.  May be
     *     the calling user without requiring any permission, otherwise the caller needs to
     *     hold the INTERACT_ACROSS_USERS_FULL permission.  Pseudousers USER_ALL and
     *     USER_CURRENT are properly interpreted; no other pseudousers are allowed.
     */
    CARAPI NotifyChange(
        /* [in] */ IUri* uri,
        /* [in] */ IIContentObserver* observer,
        /* [in] */ Boolean observerWantsSelfNotifications,
        /* [in] */ Boolean syncToNetwork,
        /* [in] */ Int32 userHandle);

    CARAPI NotifyChange(
        /* [in] */ IUri* uri,
        /* [in] */ IIContentObserver* observer,
        /* [in] */ Boolean observerWantsSelfNotifications,
        /* [in] */ Boolean syncToNetwork);

    CARAPI RequestSync(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [in] */ IBundle* extras);

    /**
     * Clear all scheduled sync operations that match the uri and cancel the active sync
     * if they match the authority and account, if they are present.
     * @param account filter the pending and active syncs to cancel using this account
     * @param authority filter the pending and active syncs to cancel using this authority
     */
    CARAPI CancelSync(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority);

    /**
     * Get information about the SyncAdapters that are known to the system.
     * @return an array of SyncAdapters that have registered with the system
     */
    CARAPI GetSyncAdapterTypes(
        /* [out, callee] */ ArrayOf<ISyncAdapterType*>** result);

    CARAPI GetSyncAutomatically(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [out] */ Boolean* result);

    CARAPI SetSyncAutomatically(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [in] */ Boolean sync);

    CARAPI AddPeriodicSync(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 pollFrequency);

    CARAPI RemovePeriodicSync(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [in] */ IBundle* extras);

    CARAPI GetPeriodicSyncs(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [out] */ IObjectContainer** periodicSyncList);

    CARAPI GetIsSyncable(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [out] */ Int32* value);

    CARAPI SetIsSyncable(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [in] */ Int32 syncable);

    CARAPI GetMasterSyncAutomatically(
        /* [out] */ Boolean* result);

    CARAPI SetMasterSyncAutomatically(
        /* [in] */ Boolean flag);

    CARAPI IsSyncActive(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [out] */ Boolean* isActive);

    CARAPI GetCurrentSyncs(
        /* [out] */ IObjectContainer** syncInfoList);

    CARAPI GetSyncStatus(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [out] */ ISyncStatusInfo** result);

    CARAPI IsSyncPending(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [out] */ Boolean* isPending);

    CARAPI AddStatusChangeListener(
        /* [in] */ Int32 mask,
        /* [in] */ IISyncStatusObserver* observer);

    CARAPI RemoveStatusChangeListener(
        /* [in] */ IISyncStatusObserver* observer);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Boolean factoryTest);

private:
    CARAPI_(AutoPtr<ISyncManager>) GetSyncManager();

private:
    static const String TAG;
    static const Boolean DBG;

private:
    AutoPtr<IContext> mContext;
    Boolean mFactoryTest;
    AutoPtr<ObserverNode> mRootNode;
    Mutex mRootNodeLock;
    AutoPtr<ISyncManager> mSyncManager;
    Mutex mSyncManagerLock;
};

}
}
}

#endif // __CCONTENTSERVICE_H__
