#ifndef __ELASTOS_DROID_DATABASE_CCURSORWINDOW_H__
#define __ELASTOS_DROID_DATABASE_CCURSORWINDOW_H__

#include "_Elastos_Droid_Database_CCursorWindow.h"
#include "database/CursorWindow.h"

namespace Elastos {
namespace Droid {
namespace Database {

CarClass(CCursorWindow), public CursorWindow
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI constructor(
        /* [in] */ Boolean localWindow);

    CARAPI AcquireReference();

    CARAPI ReleaseReference();

    CARAPI ReleaseReferenceFromContainer();

    CARAPI Close();

    CARAPI Clear();

    CARAPI GetStartPosition(
        /* [out] */ Int32* pos);

    CARAPI SetStartPosition(
        /* [in] */ Int32 pos);

    CARAPI GetNumRows(
        /* [out] */ Int32* num);

    CARAPI SetNumColumns(
        /* [in] */ Int32 columnNum,
        /* [out] */ Boolean* succeeded);

    CARAPI AllocRow(
        /* [out] */ Boolean* succeeded);

    CARAPI FreeLastRow();

    CARAPI IsNull(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* isNull);

    CARAPI IsBlob(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* isBlob);

    CARAPI IsInt64(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* isInt64);

    CARAPI IsFloat(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* isFloat);

    CARAPI IsString(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* isString);

    CARAPI GetType(
        /* [in] */ Int32 row,
        /* [in] */ Int32 column,
        /* [out] */ Int32* type);

    CARAPI GetBlob(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out, callee] */ ArrayOf<Byte>** blob);

    CARAPI GetString(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ String* str);

    CARAPI CopyStringToBuffer(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [in] */ ICharArrayBuffer* buffer);

    CARAPI GetInt64(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Int64* value);

    CARAPI GetDouble(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Double* value);

    CARAPI GetInt16(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Int16* value);

    CARAPI GetInt32(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Int32* value);

    CARAPI GetFloat(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Float* value);

    CARAPI PutBlob(
        /* [in] */ const ArrayOf<Byte>& value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* succeeded);

    CARAPI PutString(
        /* [in] */ const String& value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* succeeded);

    CARAPI PutInt64(
        /* [in] */ Int64 value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* succeeded);

    CARAPI PutDouble(
        /* [in] */ Double value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* succeeded);

    CARAPI PutNull(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* succeeded);

    CARAPI DescribeContents(
        /* [out] */ Int32* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ToString(
        /* [out] */ String* str);
};

} //Database
} //Droid
} //Elastos

#endif // __ELASTOS_DROID_DATABASE_CCURSORWINDOW_H__
