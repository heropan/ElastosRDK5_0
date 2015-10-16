
#include "elastos/droid/provider/CContactsContractSyncState.h"
#include "elastos/droid/provider/ContactsContract.h"
#include "elastos/droid/provider/SyncStateContractHelpers.h"
#include "elastos/droid/net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractSyncState::constructor()
{
    return NOERROR;
}

ECode CContactsContractSyncState::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITY_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, IContactsContractSyncState::CONTENT_DIRECTORY, uri);
}

ECode CContactsContractSyncState::Get(
    /* [in] */ IContentProviderClient* provider,
    /* [in] */ IAccount* account,
    /* [out] */ ArrayOf<Byte>** value)
{
    VALIDATE_NOT_NULL(value);

    AutoPtr<IUri> uri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&uri))
    return SyncStateContractHelpers::Get(provider, uri, account, value);
}

/**
 * @see android.provider.SyncStateContract.Helpers#get
 */
// public static Pair<Uri, byte[]> getWithUri(ContentProviderClient provider, Account account)
//         throws RemoteException {
//     return SyncStateContract.Helpers.getWithUri(provider, CONTENT_URI, account);
// }

ECode CContactsContractSyncState::Set(
    /* [in] */ IContentProviderClient* provider,
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<Byte>& data)
{
    AutoPtr<IUri> uri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&uri))
    return SyncStateContractHelpers::Set(provider, uri, account, data);
}

ECode CContactsContractSyncState::NewSetOperation(
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<Byte>& data,
    /* [out] */ IContentProviderOperation** operation)
{
    VALIDATE_NOT_NULL(operation);

    AutoPtr<IUri> uri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&uri))
    return SyncStateContractHelpers::NewSetOperation(uri, account, data, operation);
}

} //Provider
} //Droid
} //Elastos