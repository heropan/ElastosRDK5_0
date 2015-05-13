
#include "provider/CContactsContractSyncState.h"
#include "provider/ContactsContract.h"
#include "provider/SyncStateContractHelpers.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractSyncState::constructor()
{
    return NOERROR;
}

ECode CContactsContractSyncState::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITYURI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, IContactsContractSyncState::CONTENT_DIRECTORY, uri);
}

ECode CContactsContractSyncState::Get(
    /* [in] */ IContentProviderClient* provider,
    /* [in] */ IAccount* account,
    /* [out] */ ArrayOf<Byte>** value)
{
    VALIDATE_NOT_NULL(value);

    AutoPtr<IUri> uri;
    FAIL_RETURN(GetCONTENTURI((IUri**)&uri))
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
    FAIL_RETURN(GetCONTENTURI((IUri**)&uri))
    return SyncStateContractHelpers::Set(provider, uri, account, data);
}

ECode CContactsContractSyncState::NewSetOperation(
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<Byte>& data,
    /* [out] */ IContentProviderOperation** operation)
{
    VALIDATE_NOT_NULL(operation);

    AutoPtr<IUri> uri;
    FAIL_RETURN(GetCONTENTURI((IUri**)&uri))
    return SyncStateContractHelpers::NewSetOperation(uri, account, data, operation);
}

} //Provider
} //Droid
} //Elastos