
#ifndef __ELASTOS_DROID_DATABASE_CURSORWRAPPER_H__
#define __ELASTOS_DROID_DATABASE_CURSORWRAPPER_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Content::IContentResolver;
using Elastos::IO::ICloseable;

namespace Elastos {
namespace Droid {
namespace Database {
/**
 * Wrapper class for Cursor that delegates all calls to the actual cursor object
 */
class CursorWrapper
    : public Object
    , public ICursorWrapper
    , public ICursor
    , public ICloseable
{
public:
    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ ICursor* cursor);

    /**
     * Gets the underlying cursor that is wrapped by this instance.
     *
     * @return The wrapped cursor.
     */
    virtual CARAPI GetWrappedCursor(
        /* [out] */ ICursor** cursor);

    virtual CARAPI Close();

    virtual CARAPI IsClosed(
        /* [out] */ Boolean* isClosed);

    virtual CARAPI GetCount(
        /* [out] */ Int32* count);

    virtual CARAPI Deactivate();

    virtual CARAPI MoveToFirst(
        /* [out] */ Boolean* succeeded);

    virtual CARAPI GetColumnCount(
        /* [out] */ Int32* count);

    virtual CARAPI GetColumnIndex(
        /* [in] */ const String& columnName,
        /* [out] */ Int32* index);

    virtual CARAPI GetColumnIndexOrThrow(
        /* [in] */  const String& columnName,
        /* [out] */ Int32* columnIndex);

    virtual CARAPI GetColumnName(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* name);

    virtual CARAPI GetColumnNames(
        /* [out, callee] */ ArrayOf<String>** columnNames);

    virtual CARAPI GetDouble(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Double* value);

    virtual CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    virtual CARAPI GetFloat(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Float* value);

    virtual CARAPI GetInt32(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int32* value);

    virtual CARAPI GetInt64(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int64* value);

    virtual CARAPI GetInt16(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int16* value);

    virtual CARAPI GetString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* value);

    virtual CARAPI CopyStringToBuffer(
        /* [in] */ Int32 columnIndex,
        /* [in] */ ICharArrayBuffer* buffer);

    virtual CARAPI GetBlob(
        /* [in] */  Int32 columnIndex,
        /* [out,callee] */ ArrayOf<Byte>** blob);

    virtual CARAPI GetWantsAllOnMoveCalls(
        /* [out] */ Boolean* value);

    virtual CARAPI IsAfterLast(
        /* [out] */ Boolean* result);

    virtual CARAPI IsBeforeFirst(
        /* [out] */ Boolean* result);

    virtual CARAPI IsFirst(
        /* [out] */ Boolean* result);

    virtual CARAPI IsLast(
        /* [out] */ Boolean* result);

    virtual CARAPI GetType(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int32* type);

    virtual CARAPI IsNull(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* result);

    virtual CARAPI MoveToLast(
        /* [out] */ Boolean* succeeded);

    virtual CARAPI Move(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean* succeeded);

    virtual CARAPI MoveToPosition(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* succeeded);

    virtual CARAPI MoveToNext(
        /* [out] */ Boolean* succeeded);

    virtual CARAPI GetPosition(
        /* [out] */ Int32* position);

    virtual CARAPI MoveToPrevious(
        /* [out] */ Boolean* succeeded);

    virtual CARAPI RegisterContentObserver(
        /* [in] */ IContentObserver* observer);

    virtual CARAPI RegisterDataSetObserver(
        /* [in] */IDataSetObserver* observer);

    virtual CARAPI Requery(
        /* [out] */ Boolean* succeeded);

    virtual CARAPI Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** bundle);

    virtual CARAPI SetNotificationUri(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IUri* uri);

    virtual CARAPI GetNotificationUri(
        /* [out] */ IUri** uri);

    virtual CARAPI UnregisterContentObserver(
        /* [in] */ IContentObserver* observer);

    virtual CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

protected:
    AutoPtr<ICursor> mCursor;
};

} //Database
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_DATABASE_CURSORWRAPPER_H__
