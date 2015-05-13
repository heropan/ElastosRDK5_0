
#include "content/CContentResolverHelper.h"

namespace Elastos {
namespace Droid {
namespace Content {

ECode CContentResolverHelper::ModeToMode(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ Int32* modeBits)
{
    return ContentResolver::ModeToMode(uri, mode, modeBits);
}

ECode CContentResolverHelper::RequestSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras)
{
    return ContentResolver::RequestSync(account, authority, extras);
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
    /* [out] */ IObjectContainer** periodicSyncs)
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
    /* [out] */ IObjectContainer** syncInfoList)
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
    /* [out] */ IContentService** contentService)
{
    return ContentResolver::GetContentService(contentService);
}

}
}
}

