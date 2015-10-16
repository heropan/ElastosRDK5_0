
#include "elastos/droid/provider/CContactsContractRawContacts.h"
#include "elastos/droid/provider/ContactsContract.h"
#include "elastos/droid/provider/ContactsContractData.h"
#include "elastos/droid/provider/ContactsContractContacts.h"
#include "elastos/droid/database/DatabaseUtils.h"
#include "elastos/droid/net/Uri.h"
#include "elastos/droid/content/CContentValues.h"
#include "elastos/droid/content/CEntity.h"

using Elastos::Core::IInteger64;
using Elastos::Core::CInteger64;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Core::EIID_IByte;
using Elastos::Core::IArrayOf;
using Elastos::Core::CArrayOf;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Content::CContentValues;
using Elastos::Droid::Content::CEntity;
using Elastos::Droid::Content::EIID_ICursorEntityIterator;
using Elastos::Droid::Content::EIID_IEntityIterator;
using Elastos::Droid::Database::DatabaseUtils;
using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractRawContacts::constructor()
{
    return NOERROR;
}

ECode CContactsContractRawContacts::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITY_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("raw_contacts"), uri);
}

ECode CContactsContractRawContacts::GetContactLookupUri(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IUri* rawContactUri,
    /* [out] */ IUri** uri)
{
    // TODO: use a lighter query by joining rawcontacts with contacts in provider
    AutoPtr<IUri> dataUri;
    FAIL_RETURN(Uri::WithAppendedPath(rawContactUri, IContactsContractContactsData::CONTENT_DIRECTORY, (IUri**)&dataUri))
    AutoPtr<ICursor> cursor;
    AutoPtr<ArrayOf<String> > args = ArrayOf<String>::Alloc(2);
    (*args)[0] = IContactsContractRawContactsColumns::CONTACT_ID;
    (*args)[1] = IContactsContractContactsColumns::LOOKUP_KEY;
    FAIL_RETURN(resolver->Query(dataUri, args, String(NULL), NULL, String(NULL), (ICursor**)&cursor))

    //AutoPtr<IUri> lookupUri;
    //try {
    Boolean result;
    if (cursor != NULL && (cursor->MoveToFirst(&result), result)) {
        Int64 contactId;
        FAIL_GOTO(cursor->GetInt64(0, &contactId), EXIT)
        String lookupKey;
        FAIL_GOTO(cursor->GetString(1, &lookupKey), EXIT)
        FAIL_GOTO(ContactsContractContacts::GetLookupUri(contactId, lookupKey, uri), EXIT)
        return cursor->Close();
    }
    //} finally {
EXIT:
    if (cursor != NULL) {
        FAIL_RETURN(cursor->Close())
    }
    //}
    *uri = NULL;
    return NOERROR;
}

ECode CContactsContractRawContacts::NewEntityIterator(
    /* [in] */ ICursor* cursor,
    /* [out] */ IEntityIterator** iterator)
{
    VALIDATE_NOT_NULL(iterator);

    AutoPtr<EntityIteratorImpl> impl = new EntityIteratorImpl(cursor);
    *iterator = (IEntityIterator*)impl;
    REFCOUNT_ADD(*iterator);
    return NOERROR;
}

static AutoPtr<ArrayOf<String> > initDATAKEYS()
{
    AutoPtr<ArrayOf<String> > args = ArrayOf<String>::Alloc(19);
    (*args)[0] = IContactsContractDataColumns::DATA1;
    (*args)[1] = IContactsContractDataColumns::DATA2;
    (*args)[2] = IContactsContractDataColumns::DATA3;
    (*args)[3] = IContactsContractDataColumns::DATA4;
    (*args)[4] = IContactsContractDataColumns::DATA5;
    (*args)[5] = IContactsContractDataColumns::DATA6;
    (*args)[6] = IContactsContractDataColumns::DATA7;
    (*args)[7] = IContactsContractDataColumns::DATA8;
    (*args)[8] = IContactsContractDataColumns::DATA9;
    (*args)[9] = IContactsContractDataColumns::DATA10;
    (*args)[10] = IContactsContractDataColumns::DATA11;
    (*args)[11] = IContactsContractDataColumns::DATA12;
    (*args)[12] = IContactsContractDataColumns::DATA13;
    (*args)[13] = IContactsContractDataColumns::DATA14;
    (*args)[14] = IContactsContractDataColumns::DATA15;
    (*args)[15] = IContactsContractDataColumns::SYNC1;
    (*args)[16] = IContactsContractDataColumns::SYNC2;
    (*args)[17] = IContactsContractDataColumns::SYNC3;
    (*args)[18] = IContactsContractDataColumns::SYNC4;
    return args;
}

AutoPtr<ArrayOf<String> > CContactsContractRawContacts::EntityIteratorImpl::DATA_KEYS = initDATAKEYS();

CContactsContractRawContacts::EntityIteratorImpl::EntityIteratorImpl(
    /* [in] */ ICursor* cursor)
    : CursorEntityIterator(cursor)
{
}

CContactsContractRawContacts::EntityIteratorImpl::~EntityIteratorImpl()
{
}

PInterface CContactsContractRawContacts::EntityIteratorImpl::Probe(
    /* [in]  */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_ICursorEntityIterator) {
        return (PInterface)(ICursorEntityIterator*)this;
    }
    else if (riid == EIID_IEntityIterator) {
        return (IEntityIterator*)this;
    }
    return NULL;
}

UInt32 CContactsContractRawContacts::EntityIteratorImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CContactsContractRawContacts::EntityIteratorImpl::Release()
{
    return ElRefBase::Release();
}

ECode CContactsContractRawContacts::EntityIteratorImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ICursorEntityIterator*)this) {
        *pIID = EIID_ICursorEntityIterator;
    }
    else if (pObject == (IInterface*)(IEntityIterator*)this) {
        *pIID = EIID_IEntityIterator;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return  NOERROR;
}

ECode CContactsContractRawContacts::EntityIteratorImpl::GetEntityAndIncrementCursor(
    /* [in] */ ICursor* cursor,
    /* [out] */ IEntity** entity)
{
    VALIDATE_NOT_NULL(entity);

    Int32 columnRawContactId;
    FAIL_RETURN(cursor->GetColumnIndexOrThrow(IBaseColumns::ID, &columnRawContactId))
    Int64 rawContactId;
    FAIL_RETURN(cursor->GetInt64(columnRawContactId, &rawContactId))

    // we expect the cursor is already at the row we need to read from
    AutoPtr<IContentValues> cv;
    FAIL_RETURN(CContentValues::New((IContentValues**)&cv))
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, ACCOUNT_NAME);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, ACCOUNT_TYPE);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, DATA_SET);
    DatabaseUtils::CursorInt64ToContentValuesIfPresent(cursor, cv, IBaseColumns::ID);
    DatabaseUtils::CursorInt64ToContentValuesIfPresent(cursor, cv, DIRTY);
    DatabaseUtils::CursorInt64ToContentValuesIfPresent(cursor, cv, VERSION);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, SOURCE_ID);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, SYNC1);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, SYNC2);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, SYNC3);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, SYNC4);
    DatabaseUtils::CursorInt64ToContentValuesIfPresent(cursor, cv, DELETED);
    DatabaseUtils::CursorInt64ToContentValuesIfPresent(cursor, cv, CONTACT_ID);
    DatabaseUtils::CursorInt64ToContentValuesIfPresent(cursor, cv, STARRED);
    DatabaseUtils::CursorInt32ToContentValuesIfPresent(cursor, cv, NAME_VERIFIED);
    AutoPtr<IEntity> contact;
    FAIL_RETURN(CEntity::New(cv, (IEntity**)&contact))

    // read data rows until the contact id changes
    Boolean result;
    do {
        Int64 id;
        FAIL_RETURN(cursor->GetInt64(columnRawContactId, &id))
        if (rawContactId != id) {
            break;
        }
        // add the data to to the contact
        FAIL_RETURN(CContentValues::New((IContentValues**)&cv))
        Int32 columnIndex;
        FAIL_RETURN(cursor->GetColumnIndexOrThrow(IContactsContractContactsEntity::DATA_ID, &columnIndex))
        Int64 columnValue;
        FAIL_RETURN(cursor->GetInt64(columnIndex, &columnValue))
        AutoPtr<IInteger64> value;
        FAIL_RETURN(CInteger64::New(columnValue, (IInteger64**)&value))
        FAIL_RETURN(cv->PutInt64(IBaseColumns::ID, value))
        DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, IContactsContractDataColumns::RES_PACKAGE);
        DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, IContactsContractDataColumns::MIMETYPE);
        DatabaseUtils::CursorInt64ToContentValuesIfPresent(cursor, cv, IContactsContractDataColumns::IS_PRIMARY);
        DatabaseUtils::CursorInt64ToContentValuesIfPresent(cursor, cv, IContactsContractDataColumns::IS_SUPER_PRIMARY);
        DatabaseUtils::CursorInt64ToContentValuesIfPresent(cursor, cv, IContactsContractDataColumns::DATA_VERSION);
        DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, IContactsContractCommonDataKindsGroupMembership::GROUP_SOURCE_ID);
        DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, IContactsContractDataColumns::DATA_VERSION);
        for (Int32 i = 0; i < DATA_KEYS->GetLength(); i++) {
            String key = (*DATA_KEYS)[i];
            Int32 columnIndex;
            FAIL_RETURN(cursor->GetColumnIndexOrThrow(key, &columnIndex))
            Int32 type;
            FAIL_RETURN(cursor->GetType(columnIndex, &type))
            String str;
            AutoPtr<ICharSequence> cstr;
            AutoPtr<ArrayOf<Byte> > blob;
            AutoPtr<IArrayOf> array;
            switch (type) {
                case ICursor::FIELD_TYPE_NULL:
                    // don't put anything
                    break;
                case ICursor::FIELD_TYPE_INTEGER:
                case ICursor::FIELD_TYPE_FLOAT:
                case ICursor::FIELD_TYPE_STRING:
                    FAIL_RETURN(cursor->GetString(columnIndex, &str))
                    FAIL_RETURN(CString::New(str, (ICharSequence**)&cstr))
                    FAIL_RETURN(cv->PutString(key, cstr))
                    break;
                case ICursor::FIELD_TYPE_BLOB:
                    FAIL_RETURN(cursor->GetBlob(columnIndex, (ArrayOf<Byte>**)&blob))
                    FAIL_RETURN(CArrayOf::New(EIID_IByte, blob->GetLength(), (IArrayOf**)&array))
                    FAIL_RETURN(cv->PutBytes(key, array))
                    break;
                default:
                    //throw new IllegalStateException("Invalid or unhandled data type");
                    return E_ILLEGAL_STATE_EXCEPTION;
            }
        }
        AutoPtr<IUri> _uri;
        FAIL_RETURN(ContactsContractData::GetCONTENT_URI((IUri**)&_uri))
        FAIL_RETURN(contact->AddSubValue(_uri, cv))
    } while ((cursor->MoveToNext(&result), result));

    return NOERROR;
}

} //Provider
} //Droid
} //Elastos