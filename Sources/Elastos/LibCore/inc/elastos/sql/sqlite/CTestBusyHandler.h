
#ifndef __CTESTBUSYHANDLER_H__
#define __CTESTBUSYHANDLER_H__

#include "_Elastos_Sql_SQLite_CTestBusyHandler.h"
#include "Object.h"


namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CTestBusyHandler)
    , public Object
    , public IBusyHandler
    , public ICallback
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI Busy(
        /* [in] */ const String& table,
        /* [in] */ Int32 count,
        /* [out] */ Boolean* value);

    CARAPI Columns(
        /* [in] */ ArrayOf<String> * coldata);

    CARAPI Types(
        /* [in] */ ArrayOf<String> * types);

    CARAPI Newrow(
        /* [in] */ ArrayOf<String> * rowdata,
        /* [out] */ Boolean* value);

    CARAPI constructor();
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CTESTBUSYHANDLER_H__
