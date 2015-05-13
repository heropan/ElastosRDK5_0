
#include "provider/CContactsContractContacts.h"

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractContacts::constructor()
{
    return NOERROR;
}

ECode CContactsContractContacts::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENTURI(uri);
}

ECode CContactsContractContacts::GetCONTENTLOOKUPURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENTLOOKUPURI(uri);
}

ECode CContactsContractContacts::GetCONTENTVCARDURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENTVCARDURI(uri);
}

ECode CContactsContractContacts::GetCONTENTMULTIVCARDURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENTMULTIVCARDURI(uri);
}

ECode CContactsContractContacts::GetCONTENTFILTERURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENTFILTERURI(uri);
}

ECode CContactsContractContacts::GetCONTENTSTREQUENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENTSTREQUENTURI(uri);
}

ECode CContactsContractContacts::GetCONTENTFREQUENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENTFREQUENTURI(uri);
}

ECode CContactsContractContacts::GetCONTENTSTREQUENTFILTERURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENTSTREQUENTFILTERURI(uri);
}

ECode CContactsContractContacts::GetCONTENTGROUPURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENTGROUPURI(uri);
}

ECode CContactsContractContacts::GetLookupUri(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IUri* contactUri,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetLookupUri(resolver, contactUri, uri);
}

ECode CContactsContractContacts::GetLookupUriEx(
    /* [in] */ Int64 contactId,
    /* [in] */ const String& lookupKey,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetLookupUriEx(contactId, lookupKey, uri);
}

ECode CContactsContractContacts::LookupContact(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IUri* lookupUri,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::LookupContact(resolver, lookupUri, uri);
}

ECode CContactsContractContacts::MarkAsContacted(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ Int64 contactId)
{
    return ContactsContractContacts::MarkAsContacted(resolver, contactId);
}

ECode CContactsContractContacts::OpenContactPhotoInputStream(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* contactUri,
    /* [in] */ Boolean preferHighres,
    /* [out] */ IInputStream** stream)
{
    VALIDATE_NOT_NULL(stream);

    return ContactsContractContacts::OpenContactPhotoInputStream(cr, contactUri, preferHighres, stream);
}

ECode CContactsContractContacts::OpenContactPhotoInputStreamEx(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* contactUri,
    /* [out] */ IInputStream** stream)
{
    VALIDATE_NOT_NULL(stream);

    return ContactsContractContacts::OpenContactPhotoInputStreamEx(cr, contactUri, stream);
}

} //Provider
} //Droid
} //Elastos