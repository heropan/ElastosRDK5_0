
#include "provider/CCalendarContractCalendarEntity.h"
#include "provider/ContactsContract.h"
#include "database/DatabaseUtils.h"
#include "content/CContentValues.h"
#include "content/CEntity.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;
using Elastos::Droid::Content::CEntity;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Content::CContentValues;
using Elastos::Droid::Content::EIID_ICursorEntityIterator;
using Elastos::Droid::Content::EIID_IEntityIterator;

namespace Elastos {
namespace Droid {
namespace Provider {

CCalendarContractCalendarEntity::EntityIteratorImpl::EntityIteratorImpl(
    /* [in] */ ICursor* cursor)
    : CursorEntityIterator(cursor)
{
}

CCalendarContractCalendarEntity::EntityIteratorImpl::~EntityIteratorImpl()
{
}

PInterface CCalendarContractCalendarEntity::EntityIteratorImpl::Probe(
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

UInt32 CCalendarContractCalendarEntity::EntityIteratorImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCalendarContractCalendarEntity::EntityIteratorImpl::Release()
{
    return ElRefBase::Release();
}

ECode CCalendarContractCalendarEntity::EntityIteratorImpl::GetInterfaceID(
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

ECode CCalendarContractCalendarEntity::EntityIteratorImpl::GetEntityAndIncrementCursor(
    /* [in] */ ICursor* cursor,
    /* [out] */ IEntity** entity)
{
    // we expect the cursor is already at the row we need to read from

    Int32 columnIndex;
    FAIL_RETURN(cursor->GetColumnIndexOrThrow(IBaseColumns::ID, &columnIndex))
    Int64 calendarId;
    FAIL_RETURN(cursor->GetInt64(columnIndex, &calendarId))

    // Create the content value
    AutoPtr<IContentValues> cv;
    FAIL_RETURN(CContentValues::New((IContentValues**)&cv))
    AutoPtr<IInteger64> id;
    FAIL_RETURN(CInteger64::New(calendarId, (IInteger64**)&id))
    FAIL_RETURN(cv->PutInt64(IBaseColumns::ID, id))

    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, ACCOUNT_NAME);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, ACCOUNT_TYPE);

    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, _SYNC_ID);
    DatabaseUtils::CursorInt64ToContentValuesIfPresent(cursor, cv, DIRTY);

    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, CAL_SYNC1);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, CAL_SYNC2);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, CAL_SYNC3);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, CAL_SYNC4);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, CAL_SYNC5);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, CAL_SYNC6);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, CAL_SYNC7);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, CAL_SYNC8);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, CAL_SYNC9);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, CAL_SYNC10);

    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, ICalendarContractCalendars::NAME);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, CALENDAR_DISPLAY_NAME);
    DatabaseUtils::CursorInt32ToContentValuesIfPresent(cursor, cv, CALENDAR_COLOR);
    DatabaseUtils::CursorInt32ToContentValuesIfPresent(cursor, cv, CALENDAR_ACCESS_LEVEL);
    DatabaseUtils::CursorInt32ToContentValuesIfPresent(cursor, cv, VISIBLE);
    DatabaseUtils::CursorInt32ToContentValuesIfPresent(cursor, cv, SYNC_EVENTS);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, ICalendarContractCalendars::CALENDAR_LOCATION);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, CALENDAR_TIME_ZONE);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, OWNER_ACCOUNT);
    DatabaseUtils::CursorInt32ToContentValuesIfPresent(cursor, cv, CAN_ORGANIZER_RESPOND);
    DatabaseUtils::CursorInt32ToContentValuesIfPresent(cursor, cv, CAN_MODIFY_TIME_ZONE);
    DatabaseUtils::CursorInt32ToContentValuesIfPresent(cursor, cv, MAX_REMINDERS);
    DatabaseUtils::CursorInt32ToContentValuesIfPresent(cursor, cv, CAN_PARTIALLY_UPDATE);
    DatabaseUtils::CursorStringToContentValuesIfPresent(cursor, cv, ALLOWED_REMINDERS);

    DatabaseUtils::CursorInt32ToContentValuesIfPresent(cursor, cv, DELETED);

    // Create the Entity from the ContentValue
    AutoPtr<IEntity> _entity;
    FAIL_RETURN(CEntity::New(cv, (IEntity**)&_entity))

    // Set cursor to next row
    Boolean result;
    FAIL_RETURN(cursor->MoveToNext(&result))

    // Return the created Entity
    *entity = _entity;
    REFCOUNT_ADD(*entity);
    return NOERROR;
}

ECode CCalendarContractCalendarEntity::constructor()
{
    return NOERROR;
}


ECode CCalendarContractCalendarEntity::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://") + ICalendarContract::AUTHORITY + String("/calendar_entities"), uri);
}


ECode CCalendarContractCalendarEntity::NewEntityIterator(
    /* [in] */ ICursor* cursor,
    /* [out] */ IEntityIterator** iter)
{
    VALIDATE_NOT_NULL(iter);

    AutoPtr<EntityIteratorImpl> impl = new EntityIteratorImpl(cursor);
    *iter = (IEntityIterator*)impl;
    REFCOUNT_ADD(*iter);
    return NOERROR;
}

} //Provider
} //Droid
} //Elastos