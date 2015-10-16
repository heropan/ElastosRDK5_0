
#include "elastos/droid/provider/CContactsContractStatusUpdates.h"
#include "elastos/droid/provider/ContactsContract.h"
#include "elastos/droid/provider/ContactsContractProfile.h"
#include "elastos/droid/net/Uri.h"
#include "elastos/droid/R.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractStatusUpdates::constructor()
{
    return NOERROR;
}

ECode CContactsContractStatusUpdates::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITY_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("status_updates"), uri);
}

ECode CContactsContractStatusUpdates::GetPROFILE_CONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> conUri;
    FAIL_RETURN(ContactsContractProfile::GetCONTENT_URI((IUri**)&conUri))
    return Uri::WithAppendedPath(conUri, String("status_updates"), uri);
}

ECode CContactsContractStatusUpdates::GetPresenceIconResourceId(
    /* [in] */ Int32 status,
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);

    switch (status) {
        case AVAILABLE:
            *id = Elastos::Droid::R::drawable::presence_online;
            return NOERROR;
        case IDLE:
        case AWAY:
            *id = Elastos::Droid::R::drawable::presence_away;
            return NOERROR;
        case DO_NOT_DISTURB:
            *id = Elastos::Droid::R::drawable::presence_busy;
            return NOERROR;
        case INVISIBLE:
            *id = Elastos::Droid::R::drawable::presence_invisible;
            return NOERROR;
        case OFFLINE:
        default:
            *id = Elastos::Droid::R::drawable::presence_offline;
            return NOERROR;
    }
    return NOERROR;
}

ECode CContactsContractStatusUpdates::GetPresencePrecedence(
    /* [in] */ Int32 status,
    /* [out] */ Int32* precedence)
{
    VALIDATE_NOT_NULL(precedence);
    // Keep this function here incase we want to enforce a different precedence than the
    // natural order of the status constants.
    *precedence = status;
    return NOERROR;
}

} //Provider
} //Droid
} //Elastos