
#include "provider/CContactsContractProfileSyncState.h"
#include "provider/SyncStateContractHelpers.h"
#include "provider/ContactsContractProfile.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;
using Elastos::Droid::Provider::ISyncStateContractHelpers;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractProfileSyncState::constructor()
{
    return NOERROR;
}

ECode CContactsContractProfileSyncState::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContractProfile::GetCONTENT_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, IContactsContractProfileSyncState::CONTENT_DIRECTORY, uri);
}

ECode CContactsContractProfileSyncState::Get(
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

/**
 * @see android.provider.SyncStateContract.Helpers#set
 */
ECode CContactsContractProfileSyncState::Set(
    /* [in] */ IContentProviderClient* provider,
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<Byte>& data)
{
    AutoPtr<IUri> uri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&uri))
    return SyncStateContractHelpers::Set(provider, uri, account, data);
}

/**
 * @see android.provider.SyncStateContract.Helpers#newSetOperation
 */
ECode CContactsContractProfileSyncState::NewSetOperation(
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