
#ifndef __ELASTOS_DROID_CONTENT_CCONTENTRESOLVERHELPER_H__
#define __ELASTOS_DROID_CONTENT_CCONTENTRESOLVERHELPER_H__

#include "_Elastos_Droid_Content_CContentResolverHelper.h"
#include "ContentResolver.h"

using Elastos::Droid::Accounts::IAccount;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CContentResolverHelper)
{
public:
    CARAPI ModeToMode(
        /* [in] */ IUri* uri,
        /* [in] */ const String& mode,
        /* [out] */ Int32* modeBits);

    CARAPI RequestSync(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [in] */ IBundle* extras);

    CARAPI ValidateSyncExtrasBundle(
        /* [in] */ IBundle* extras);

    CARAPI CancelSync(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority);

    CARAPI GetSyncAdapterTypes(
        /* [out, callee] */ ArrayOf<ISyncAdapterType *>** types);

    CARAPI GetSyncAutomatically(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [out] */ Boolean* isSynced);

    CARAPI SetSyncAutomatically(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [in] */ Boolean sync);

    CARAPI AddPeriodicSync(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 pollFrequency);

    CARAPI RemovePeriodicSync(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [in] */ IBundle* extras);

    CARAPI GetPeriodicSyncs(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [out] */ IObjectContainer** periodicSyncs);

    CARAPI GetIsSyncable(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [out] */ Int32* isSyncable);

    CARAPI SetIsSyncable(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [in] */ Int32 syncable);

    CARAPI GetMasterSyncAutomatically(
        /* [out] */ Boolean* result);

    CARAPI SetMasterSyncAutomatically(
        /* [in] */ Boolean sync);

    CARAPI IsSyncActive(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [out] */ Boolean* isSyncActive);

    CARAPI GetCurrentSync(
        /* [out] */ ISyncInfo** syncInfo);

    CARAPI GetCurrentSyncs(
        /* [out] */ IObjectContainer** syncInfoList);

    CARAPI GetSyncStatus(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [out] */ ISyncStatusInfo** syncStatusInfo);

    CARAPI IsSyncPending(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [out] */ Boolean* isSyncPending);

    CARAPI AddStatusChangeListener(
        /* [in] */ Int32 mask,
        /* [in] */ ISyncStatusObserver* observerCallback,
        /* [out] */ IInterface** item);

    CARAPI RemoveStatusChangeListener(
        /* [in] */ IInterface* handle);

    CARAPI GetContentService(
        /* [out] */ IContentService** contentService);

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __ELASTOS_DROID_CONTENT_CCONTENTRESOLVERHELPER_H__
