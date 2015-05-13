
#include "provider/CContactsContractGroups.h"
#include "provider/ContactsContract.h"
#include "database/DatabaseUtils.h"
#include "content/CContentValues.h"
#include "content/CEntity.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;
using Elastos::Droid::Database::DatabaseUtils;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Content::CContentValues;
using Elastos::Droid::Content::EIID_ICursorEntityIterator;
using Elastos::Droid::Content::EIID_IEntityIterator;
using Elastos::Droid::Content::CEntity;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractGroups::constructor()
{
    return NOERROR;
}

ECode CContactsContractGroups::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITYURI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("groups"), uri);
}

ECode CContactsContractGroups::GetCONTENTSUMMARYURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContract::GetAUTHORITYURI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("groups_summary"), uri);
}

ECode CContactsContractGroups::NewEntityIterator(
    /* [in] */ ICursor* cursor,
    /* [out] */ IEntityIterator** iterator)
{
    VALIDATE_NOT_NULL(iterator);

    AutoPtr<EntityIteratorImpl> impl = new EntityIteratorImpl(cursor);
    *iterator = (IEntityIterator*)impl;
    INTERFACE_ADDREF(*iterator);
    return NOERROR;
}

CContactsContractGroups::EntityIteratorImpl::EntityIteratorImpl(
    /* [in] */ ICursor* cursor)
    : CursorEntityIterator(cursor)
{
}

CContactsContractGroups::EntityIteratorImpl::~EntityIteratorImpl()
{
}

PInterface CContactsContractGroups::EntityIteratorImpl::Probe(
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

UInt32 CContactsContractGroups::EntityIteratorImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CContactsContractGroups::EntityIteratorImpl::Release()
{
    return ElRefBase::Release();
}

ECode CContactsContractGroups::EntityIteratorImpl::GetInterfaceID(
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

ECode CContactsContractGroups::EntityIteratorImpl::GetEntityAndIncrementCursor(
    /* [in] */ ICursor* cursor,
    /* [out] */ IEntity** entity)
{
    VALIDATE_NOT_NULL(entity);

    // we expect the cursor is already at the row we need to read from
    AutoPtr<IContentValues> values;
    FAIL_RETURN(CContentValues::New((IContentValues**)&values))

    DatabaseUtils::CursorInt64ToContentValuesIfPresent(cursor, values, IBaseColumns::ID);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, values, ACCOUNT_NAME);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, values, ACCOUNT_TYPE);
    DatabaseUtils::CursorInt64ToContentValuesIfPresent(cursor, values, DIRTY);
    DatabaseUtils::CursorInt64ToContentValuesIfPresent(cursor, values, VERSION);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, values, SOURCE_ID);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, values, RES_PACKAGE);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, values, TITLE);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, values, TITLE_RES);
    DatabaseUtils::CursorInt64ToContentValuesIfPresent(cursor, values, GROUP_VISIBLE);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, values, SYNC1);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, values, SYNC2);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, values, SYNC3);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, values, SYNC4);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, values, SYSTEM_ID);
    DatabaseUtils::CursorInt64ToContentValuesIfPresent(cursor, values, DELETED);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, values, NOTES);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, values, SHOULD_SYNC);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, values, FAVORITES);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, values, AUTO_ADD);
    Boolean result;
    FAIL_RETURN(cursor->MoveToNext(&result))
    return CEntity::New(values, (IEntity**)&entity);
}

} //Provider
} //Droid
} //Elastos