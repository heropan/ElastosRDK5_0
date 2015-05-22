
#ifndef __CTABLERESULT_H__
#define __CTABLERESULT_H__

#include "_Elastos_Sql_SQLite_CTableResult.h"
#include "TableResult.h"
#include "coredef.h"
#include <elstring.h>

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CTableResult) , public TableResult
{
public:
    CARAPI Columns(
        /* [in] */ const ArrayOf<String> & coldata);

    CARAPI Types(
        /* [in] */ const ArrayOf<String> & intypes);

    CARAPI Newrow(
        /* [in] */ const ArrayOf<String> & rowdata,
        /* [out] */ Boolean * value);

    CARAPI ToString(
        /* [out] */ String * str);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 maxrows);

    CARAPI Clear();
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CTABLERESULT_H__
