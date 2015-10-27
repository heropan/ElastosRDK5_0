
#include "elastos/droid/provider/ContactsContractContacts.h"
#include "elastos/droid/provider/ContactsContract.h"
#include "elastos/droid/net/Uri.h"
#include "elastos/droid/content/CContentUris.h"
#include "elastos/droid/content/CContentValues.h"
#include <elastos/coredef.h>

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::IInteger64;
using Elastos::Core::CInteger64;
using Elastos::IO::IFileInputStream;
using Elastos::IO::IByteArrayInputStream;
using Elastos::IO::CByteArrayInputStream;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Provider::IBaseColumns;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Content::CContentValues;
using Elastos::Droid::Content::CContentUris;
using Elastos::Droid::Content::IContentUris;
using Elastos::Droid::Content::Res::IAssetFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode ContactsContractContacts::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITY_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("contacts"), uri);
}

ECode ContactsContractContacts::GetCONTENT_LOOKUP_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("lookup"), uri);
}

ECode ContactsContractContacts::GetCONTENT_VCARD_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("as_vcard"), uri);
}

ECode ContactsContractContacts::GetCONTENT_MULTIVCARD_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("as_multi_vcard"), uri);
}

ECode ContactsContractContacts::GetCONTENT_FILTER_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("filter"), uri);
}

ECode ContactsContractContacts::GetCONTENT_STREQUENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("strequent"), uri);
}

ECode ContactsContractContacts::GetCONTENT_FREQUENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("frequent"), uri);
}

ECode ContactsContractContacts::GetCONTENT_STREQUENT_FILTER_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(GetCONTENT_STREQUENT_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("filter"), uri);
}

ECode ContactsContractContacts::GetCONTENT_GROUP_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("group"), uri);
}

ECode ContactsContractContacts::GetLookupUri(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IUri* contactUri,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<ICursor> c;
    AutoPtr<ArrayOf<String> > args = ArrayOf<String>::Alloc(2);
    (*args)[0] = IContactsContractContactsColumns::LOOKUP_KEY;
    (*args)[1] = IBaseColumns::ID;
    FAIL_RETURN(resolver->Query(contactUri, args, String(NULL), NULL, String(NULL), (ICursor**)&c))
    if (c == NULL) {
        *uri = NULL;
        return NOERROR;
    }

    //try {
    Boolean result;
    FAIL_GOTO(c->MoveToFirst(&result), EXIT)
    if (result) {
        String lookupKey;
        FAIL_GOTO(c->GetString(0, &lookupKey), EXIT)
        Int64 contactId;
        FAIL_GOTO(c->GetInt64(1, &contactId), EXIT)
        FAIL_GOTO(GetLookupUri(contactId, lookupKey, uri), EXIT)
        return c->Close();
    }
    //} finally {
EXIT:
    FAIL_RETURN(c->Close())
    //}
    *uri = NULL;
    return NOERROR;
}

ECode ContactsContractContacts::GetLookupUri(
    /* [in] */ Int64 contactId,
    /* [in] */ const String& lookupKey,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri, _uri;
    FAIL_RETURN(GetCONTENT_LOOKUP_URI((IUri**)&auUri))
    FAIL_RETURN(Uri::WithAppendedPath(auUri, lookupKey, (IUri**)&_uri))
    AutoPtr<IContentUris> contentUris;
    FAIL_RETURN(CContentUris::AcquireSingleton((IContentUris**)&contentUris))
    return contentUris->WithAppendedId(_uri, contactId, uri);
}

ECode ContactsContractContacts::LookupContact(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IUri* lookupUri,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    if (lookupUri == NULL) {
        *uri = NULL;
        return NOERROR;
    }

    AutoPtr<ICursor> c;
    AutoPtr<ArrayOf<String> > args = ArrayOf<String>::Alloc(1);
    (*args)[0] = IBaseColumns::ID;
    FAIL_RETURN(resolver->Query(lookupUri, args, String(NULL), NULL, String(NULL), (ICursor**)&c))
    if (c == NULL) {
        *uri = NULL;
        return NOERROR;
    }

    //try {
    Boolean result;
    FAIL_GOTO(c->MoveToFirst(&result), EXIT)
    if (result) {
        Int64 contactId;
        FAIL_GOTO(c->GetInt64(0, &contactId), EXIT)
        AutoPtr<IUri> auUri;
        FAIL_GOTO(GetCONTENT_URI((IUri**)&auUri), EXIT)
        AutoPtr<IContentUris> contentUris;
        FAIL_GOTO(CContentUris::AcquireSingleton((IContentUris**)&contentUris), EXIT)
        FAIL_GOTO(contentUris->WithAppendedId(auUri, contactId, uri), EXIT)
        return c->Close();
    }
    //} finally {
EXIT:
    FAIL_RETURN((c->Close()))
    //}
    *uri = NULL;
    return NOERROR;
}

ECode ContactsContractContacts::MarkAsContacted(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ Int64 contactId)
{
    AutoPtr<IUri> uri, auUri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&auUri))
    AutoPtr<IContentUris> contentUris;
    FAIL_RETURN(CContentUris::AcquireSingleton((IContentUris**)&contentUris))
    FAIL_RETURN(contentUris->WithAppendedId(auUri, contactId, (IUri**)&uri))
    AutoPtr<IContentValues> values;
    FAIL_RETURN(CContentValues::New((IContentValues**)&values))

    // TIMES_CONTACTED will be incremented when LAST_TIME_CONTACTED is modified.
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 now;
    system->GetCurrentTimeMillis(&now);
    AutoPtr<IInteger64> timeMillis;
    CInteger64::New(now, (IInteger64**)&timeMillis);
    FAIL_RETURN(values->PutInt64(IContactsContractContactOptionsColumns::LAST_TIME_CONTACTED, timeMillis))
    Int32 result;
    return resolver->Update(uri, values, String(NULL), NULL, &result);
}

ECode ContactsContractContacts::OpenContactPhotoInputStream(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* contactUri,
    /* [in] */ Boolean preferHighres,
    /* [out] */ IInputStream** stream)
{
    VALIDATE_NOT_NULL(stream);

    if (preferHighres) {
        AutoPtr<IUri> displayPhotoUri;
        FAIL_RETURN(Uri::WithAppendedPath(contactUri, IContactsContractContactsPhoto::DISPLAY_PHOTO, (IUri**)&displayPhotoUri))
        AutoPtr<IInputStream> inputStream;
        //try {
            AutoPtr<IAssetFileDescriptor> fd;
            FAIL_RETURN(cr->OpenAssetFileDescriptor(displayPhotoUri, String("r"), (IAssetFileDescriptor**)&fd))
            AutoPtr<IFileInputStream> fstream;
            FAIL_RETURN(fd->CreateInputStream((IFileInputStream**)&fstream))
            *stream = (IInputStream*)fstream;
            REFCOUNT_ADD(*stream);
            return NOERROR;
        //} catch (IOException e) {
            // fallback to the thumbnail code
        //}
    }

    AutoPtr<IUri> photoUri;
    FAIL_RETURN(Uri::WithAppendedPath(contactUri, IContactsContractContactsPhoto::CONTENT_DIRECTORY, (IUri**)&photoUri))
    if (photoUri == NULL) {
        *stream = NULL;
        return NOERROR;
    }
    AutoPtr<ICursor> cursor;
    AutoPtr<ArrayOf<String> > args = ArrayOf<String>::Alloc(1);
    (*args)[0] = IContactsContractCommonDataKindsPhoto::PHOTO;
    FAIL_RETURN(cr->Query(photoUri, args, String(NULL), NULL, String(NULL), (ICursor**)&cursor))

    //try {
    Boolean result;
    AutoPtr<ArrayOf<Byte> > data;
    AutoPtr<IByteArrayInputStream> _stream;
    if (cursor == NULL || (cursor->MoveToNext(&result), !result)) {
        *stream = NULL;
        goto FINALLY;
    }
    FAIL_GOTO(cursor->GetBlob(0, (ArrayOf<Byte>**)&data), FINALLY)
    if (data == NULL) {
        *stream = NULL;
        goto FINALLY;
    }
    FAIL_GOTO(CByteArrayInputStream::New(data, (IByteArrayInputStream**)&_stream), FINALLY)
    *stream = (IInputStream*)_stream;
    REFCOUNT_ADD(*stream);
    goto FINALLY;
    //} finally {
FINALLY:
    if (cursor != NULL) {
        FAIL_RETURN(cursor->Close())
    }
    //}
    return NOERROR;
}

ECode ContactsContractContacts::OpenContactPhotoInputStream(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* contactUri,
    /* [out] */ IInputStream** stream)
{
    return OpenContactPhotoInputStream(cr, contactUri, FALSE, stream);
}

} //Provider
} //Droid
} //Elastos
