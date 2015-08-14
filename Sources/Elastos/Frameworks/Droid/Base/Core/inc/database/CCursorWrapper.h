#ifndef __ELASTOS_DROID_DATABASE_CCURSORWRAPPER_H__
#define __ELASTOS_DROID_DATABASE_CCURSORWRAPPER_H__

#include "_CCursorWrapper.h"
#include "database/CursorWrapper.h"

namespace Elastos {
namespace Droid {
namespace Database {

CarClass(CCursorWrapper), public CursorWrapper
{
public:
    CARAPI constructor(
        /* [in] */ ICursor* cursor);

    CARAPI GetWrappedCursor(
        /* [out] */ ICursor** cursor);

    CARAPI GetCount(
        /* [out] */ Int32* count);

    CARAPI GetPosition(
        /* [out] */ Int32* position);

    CARAPI Move(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean* succeeded);

    CARAPI MoveToPosition(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* succeeded);

    CARAPI MoveToFirst(
        /* [out] */ Boolean* succeeded);

    CARAPI MoveToLast(
        /* [out] */ Boolean* succeeded);

    CARAPI MoveToNext(
        /* [out] */ Boolean* succeeded);

    CARAPI MoveToPrevious(
        /* [out] */ Boolean* succeeded);

    CARAPI IsFirst(
        /* [out] */ Boolean* isFirst);

    CARAPI IsLast(
        /* [out] */ Boolean* isLast);

    CARAPI IsBeforeFirst(
        /* [out] */ Boolean* isBeforeFirst);

    CARAPI IsAfterLast(
        /* [out] */ Boolean* isAfterLast);

    CARAPI GetColumnIndex(
        /* [in] */ const String& columnName,
        /* [out] */ Int32* columnIndex);

    CARAPI GetColumnIndexOrThrow(
        /* [in] */ const String& columnName,
        /* [out] */ Int32* columnIndex);

    CARAPI GetColumnName(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* columnName);

    CARAPI GetColumnNames(
        /* [out,callee] */ ArrayOf<String>** columnNames);

    CARAPI GetColumnCount(
        /* [out] */ Int32* columnCount);

    CARAPI GetBlob(
        /* [in] */ Int32 columnIndex,
         /* [out,callee] */ ArrayOf<Byte>** blob);

    CARAPI GetString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* columnValue);

    CARAPI CopyStringToBuffer(
        /* [in] */ Int32 columnIndex,
        /* [in ] */ ICharArrayBuffer* buf);

    CARAPI GetInt16(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int16* columnValue);

    CARAPI GetInt32(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int32* columnValue);

    CARAPI GetInt64(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int64* columnValue);

    CARAPI GetFloat(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Float* columnValue);

    CARAPI GetDouble(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Double* columnValue);

    CARAPI GetType(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int32* index);

    CARAPI IsNull(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* isNull);

    CARAPI Deactivate();

    CARAPI Requery(
        /* [out] */ Boolean* succeeded);

    CARAPI Close();

    CARAPI IsClosed(
        /* [out] */ Boolean* closed);

    CARAPI RegisterContentObserver(
        /* [in] */ IContentObserver* observer);

    CARAPI UnregisterContentObserver(
        /* [in] */ IContentObserver* observer);

    CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI SetNotificationUri(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IUri* uri);

    CARAPI GetWantsAllOnMoveCalls(
        /* [out] */ Boolean* result);

    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** result);

private:
    // TODO: Add your private member variables here.
};

} //Database
} //Droid
} //Elastos

#endif // __ELASTOS_DROID_DATABASE_CCURSORWRAPPER_H__
