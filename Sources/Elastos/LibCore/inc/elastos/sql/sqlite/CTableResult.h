
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
    CAR_OBJECT_DECL();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 maxrows);
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CTABLERESULT_H__
