
#include "elastos/droid/provider/CContactsContractContacts.h"

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractContacts::constructor()
{
    return NOERROR;
}

ECode CContactsContractContacts::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENT_URI(uri);
}

ECode CContactsContractContacts::GetCONTENT_LOOKUP_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENT_LOOKUP_URI(uri);
}

ECode CContactsContractContacts::GetCONTENT_VCARD_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENT_VCARD_URI(uri);
}

ECode CContactsContractContacts::GetCONTENT_MULTIVCARD_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENT_MULTIVCARD_URI(uri);
}

ECode CContactsContractContacts::GetCONTENT_FILTER_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENT_FILTER_URI(uri);
}

ECode CContactsContractContacts::GetCONTENT_STREQUENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENT_STREQUENT_URI(uri);
}

ECode CContactsContractContacts::GetCONTENT_FREQUENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENT_FREQUENT_URI(uri);
}

ECode CContactsContractContacts::GetCONTENT_STREQUENT_FILTER_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENT_STREQUENT_FILTER_URI(uri);
}

ECode CContactsContractContacts::GetCONTENT_GROUP_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetCONTENT_GROUP_URI(uri);
}

ECode CContactsContractContacts::GetLookupUri(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IUri* contactUri,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetLookupUri(resolver, contactUri, uri);
}

ECode CContactsContractContacts::GetLookupUri(
    /* [in] */ Int64 contactId,
    /* [in] */ const String& lookupKey,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContractContacts::GetLookupUri(contactId, lookupKey, uri);
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

ECode CContactsContractContacts::OpenContactPhotoInputStream(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* contactUri,
    /* [out] */ IInputStream** stream)
{
    VALIDATE_NOT_NULL(stream);

    return ContactsContractContacts::OpenContactPhotoInputStream(cr, contactUri, stream);
}

} //Provider
} //Droid
} //Elastos