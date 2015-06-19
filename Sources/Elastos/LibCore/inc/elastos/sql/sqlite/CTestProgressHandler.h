
#ifndef __CTESTPROGRESSHANDLER_H__
#define __CTESTPROGRESSHANDLER_H__

#include "_Elastos_Sql_SQLite_CTestProgressHandler.h"
#include <elastos/core/Object.h>


namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CTestProgressHandler)
    , public Object
    , public IProgressHandler
    , public ICallback
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI Progress(
        /* [out] */ Boolean* value);

    CARAPI Columns(
        /* [in] */ const ArrayOf<String>& coldata);

    CARAPI Types(
        /* [in] */ const ArrayOf<String>& types);

    CARAPI Newrow(
        /* [in] */ const ArrayOf<String>& rowdata,
        /* [out] */ Boolean* value);

    CARAPI constructor();

    CARAPI_(Int32) GetCounts();

private:
    Boolean progressed;
    Int32 counter;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CTESTPROGRESSHANDLER_H__
