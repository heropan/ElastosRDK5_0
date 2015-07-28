
#include "provider/CBrowserContractSyncState.h"


ECode CBrowserContractSyncState::constructor()
{
    return NOERROR;
}

ECode CBrowserContractSyncState::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
//    *uri = Uri.withAppendedPath(AUTHORITY_URI, CONTENT_DIRECTORY);
}

ECode CBrowserContractSyncState::Get(
    /* [in] */ IContentProviderClient* provider,
    /* [in] */ IAccount* account,
    /* [out] */ ArrayOf<Byte>* value)
{
//    return SyncStateContract.Helpers.get(provider, CONTENT_URI, account);
}

ECode CBrowserContractSyncState::GetWithUri(
    /* [in] */ IContentProviderClient* provider,
    /* [in] */ IAccount* account,
    /* [out] */ Pair<IUri*, ArrayOf<Byte> >* value)
{
//    return SyncStateContract.Helpers.getWithUri(provider, CONTENT_URI, account);
}

ECode CBrowserContractSyncState::Set(
    /* [in] */ IContentProviderClient* provider,
    /* [in] */ IAccount* account,
    /* [in] */ ArrayOf<Byte> data)
{
//    SyncStateContract.Helpers.set(provider, CONTENT_URI, account, data);
}

/**
 * @see android.provider.SyncStateContract.Helpers#newSetOperation
 */
ECode CBrowserContractSyncState::NewSetOperation(
    /* [in] */ IAccount* account,
    /* [in] */ ArrayOf<Byte> data,
    /* [out] */ IContentProviderOperation** operation)
{
//    return SyncStateContract.Helpers.newSetOperation(CONTENT_URI, account, data);
}