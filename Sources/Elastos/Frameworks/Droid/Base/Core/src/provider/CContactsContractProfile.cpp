
#include "provider/CContactsContractProfile.h"
#include "provider/ContactsContract.h"

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractProfile::constructor()
{
    return NOERROR;
}

ECode CContactsContractProfile::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractProfile::GetCONTENTURI(uri);
}

ECode CContactsContractProfile::GetCONTENTVCARDURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractProfile::GetCONTENTVCARDURI(uri);

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