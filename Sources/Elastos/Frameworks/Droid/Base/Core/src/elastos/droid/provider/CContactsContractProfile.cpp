
#include "elastos/droid/provider/CContactsContractProfile.h"
#include "elastos/droid/provider/ContactsContract.h"

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractProfile::constructor()
{
    return NOERROR;
}

ECode CContactsContractProfile::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractProfile::GetCONTENT_URI(uri);
}

ECode CContactsContractProfile::GetCONTENT_VCARD_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractProfile::GetCONTENT_VCARD_URI(uri);

}

ECode CContactsContractProfile::GetCONTENTRAWCONTACTSURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractProfile::GetCONTENTRAWCONTACTSURI(uri);
}

ECode CContactsContractProfile::GetMINID(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);

    return ContactsContractProfile::GetMINID(id);
}

} //Provider
} //Droid
} //Elastos