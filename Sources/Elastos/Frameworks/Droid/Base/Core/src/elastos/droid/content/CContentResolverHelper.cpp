
#include "elastos/droid/content/CContentResolverHelper.h"
#include "elastos/droid/content/ContentResolver.h"

namespace Elastos {
namespace Droid {
namespace Content {

CAR_INTERFACE_IMPL(CContentResolverHelper, Singleton, IContentResolverHelper)

CAR_SINGLETON_IMPL(CContentResolverHelper)

ECode CContentResolverHelper::RequestSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras)
{
    return ContentResolver::RequestSync(account, authority, extras);
}

ECode CContentResolverHelper::RequestSync(
    /* [in] */ ISyncRequest* request)
{
    return ContentResolver::RequestSync(request);
}

ECode CContentResolverHelper::ValidateSyncExtrasBundle(
    /* [in] */ IBundle* extras)
{
    return ContentResolver::ValidateSyncExtrasBundle(extras);
}

ECode CContentResolverHelper::CancelSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority)
{
    return ContentResolver::CancelSync(account, authority);
}

ECode CContentResolverHelper::GetSyncAdapterTypes(
    /* [out, callee] */ ArrayOf<ISyncAdapterType *>** types)
{
    return ContentResolver::GetSyncAdapterTypes(types);
}

ECode CContentResolverHelper::GetSyncAutomatically(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ Boolean* isSynced)
{
    return ContentResolver::GetSyncAutomatically(account, authority, isSynced);
}

ECode CContentResolverHelper::SetSyncAutomatically(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Boolean sync)
{
    return ContentResolver::SetSyncAutomatically(account, authority, sync);
}

ECode CContentResolverHelper::AddPeriodicSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 pollFrequency)
{
    return ContentResolver::AddPeriodicSync(account, authority, extras, pollFrequency);
}

ECode CContentResolverHelper::CancelSync(
    /* [in] */ ISyncRequest* request)
{
    return ContentResolver::CancelSync(request);
}

ECode CContentResolverHelper::RemovePeriodicSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras)
{
    return ContentResolver::RemovePeriodicSync(account, authority, extras);
}

ECode CContentResolverHelper::GetPeriodicSyncs(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ IList** periodicSyncs)
{
    return ContentResolver::GetPeriodicSyncs(account, authority, periodicSyncs);
}

ECode CContentResolverHelper::GetIsSyncable(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ Int32* isSyncable)
{
    return ContentResolver::GetIsSyncable(account, authority, isSyncable);
}

ECode CContentResolverHelper::SetIsSyncable(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Int32 syncable)
{
    return ContentResolver::SetIsSyncable(account, authority, syncable);
}

ECode CContentResolverHelper::GetMasterSyncAutomatically(
    /* [out] */ Boolean* result)
{
    return ContentResolver::GetMasterSyncAutomatically(result);
}

ECode CContentResolverHelper::SetMasterSyncAutomatically(
    /* [in] */ Boolean sync)
{
    return ContentResolver::SetMasterSyncAutomatically(sync);
}

ECode CContentResolverHelper::IsSyncActive(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ Boolean* isSyncActive)
{
    return ContentResolver::IsSyncActive(account, authority, isSyncActive);
}

ECode CContentResolverHelper::GetCurrentSync(
    /* [out] */ ISyncInfo** syncInfo)
{
    return ContentResolver::GetCurrentSync(syncInfo);
}

ECode CContentResolverHelper::GetCurrentSyncs(
    /* [out] */ IList** syncInfoList)
{
    return ContentResolver::GetCurrentSyncs(syncInfoList);
}

ECode CContentResolverHelper::GetSyncStatus(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ ISyncStatusInfo** syncStatusInfo)
{
    return ContentResolver::GetSyncStatus(account, authority, syncStatusInfo);
}

ECode CContentResolverHelper::IsSyncPending(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ Boolean* isSyncPending)
{
    return ContentResolver::IsSyncPending(account, authority, isSyncPending);
}

ECode CContentResolverHelper::AddStatusChangeListener(
    /* [in] */ Int32 mask,
    /* [in] */ ISyncStatusObserver* observerCallback,
    /* [out] */ IInterface** item)
{
    return ContentResolver::AddStatusChangeListener(mask, observerCallback, item);
}

ECode CContentResolverHelper::RemoveStatusChangeListener(
    /* [in] */ IInterface* handle)
{
    return ContentResolver::RemoveStatusChangeListener(handle);
}

ECode CContentResolverHelper::GetContentService(
    /* [out] */ IIContentService** contentService)
{
    return ContentResolver::GetContentService(contentService);
}

ECode CContentResolverHelper::SyncErrorToString(
    /* [in] */ Int32 error,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = ContentResolver::SyncErrorToString(error);
    return NOERROR;
}

}
}
}

