
#ifndef __CTABLERESULTX_H__
#define __CTABLERESULTX_H__

#include "_Elastos_Sql_SQLite_JDBC_CTableResultX.h"
#include "TableResult.h"

namespace Elastos {
namespace Sql {
namespace SQLite {
namespace JDBC{

CarClass(CTableResultX)
    , public TableResult
    , public ITableResultX
{
public:
    CAR_OBJECT_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Columns(
        /* [in] */ const ArrayOf<String>& coldata);

    CARAPI Types(
        /* [in] */ const ArrayOf<String>& intypes);

    CARAPI Newrow(
        /* [in] */ const ArrayOf<String>& rowdata,
        /* [out] */ Boolean * value);

    CARAPI_(void) Sql_types(
        /* [in] */ ArrayOf<Int32>* types);

public:
    AutoPtr<ArrayOf<Int32> > sql_type;
};

} // namespace JDBC
} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CTABLERESULTX_H__
