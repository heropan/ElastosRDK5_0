
#include "provider/CContactsContractData.h"

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractData::constructor()
{
    return NOERROR;
}

ECode CContactsContractData::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractData::GetCONTENTURI(uri);
}

ECode CContactsContractData::GetContactLookupUri(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IUri* dataUri,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractData::GetContactLookupUri(resolver, dataUri, uri);
}

} //Provider
} //Droid
} //Elastos