#ifndef __ELASTOS_DROID_DATABASE_CDATABASEINSERTHELPER_H__
#define __ELASTOS_DROID_DATABASE_CDATABASEINSERTHELPER_H__

#include "_CDatabaseInsertHelper.h"
#include "database/DatabaseUtils.h"

namespace Elastos {
namespace Droid {
namespace Database {

CarClass(CDatabaseInsertHelper) , public DatabaseUtils::InsertHelper
{
public:
    CARAPI constructor(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ const String& tableName);

    CARAPI GetColumnIndex(
        /* [in] */ const String& key,
        /* [out] */ Int32* index);

    CARAPI BindDouble(
        /* [in] */ Int32 index,
        /* [in] */ Double value);

    CARAPI BindFloat(
        /* [in] */ Int32 index,
        /* [in] */ Float value);

    CARAPI BindInt64(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value);

    CARAPI BindInt32(
        /* [in] */ Int32 index,
        /* [in] */ Int32 value);

    CARAPI BindBoolean(
        /* [in] */ Int32 index,
        /* [in] */ Boolean value);

    CARAPI BindNull(
        /* [in] */ Int32 index);

    CARAPI BindBytes(
        /* [in] */ Int32 index,
        /* [in] */ ArrayOf<Byte>* value);

    CARAPI BindString(
        /* [in] */ Int32 index,
        /* [in] */ const String& value);

    CARAPI Insert(
        /* [in] */ IContentValues* values,
        /* [out] */ Int64* result);

    CARAPI Execute(
        /* [out] */ Int64* result);

    CARAPI PrepareForInsert();

    CARAPI PrepareForReplace();

    CARAPI Replace(
        /* [in] */ IContentValues* values,
        /* [out] */ Int64* result);

    CARAPI Close();

};

} //Database
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_DATABASE_CDATABASEINSERTHELPER_H__
