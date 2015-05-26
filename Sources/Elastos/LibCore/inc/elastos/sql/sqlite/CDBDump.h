
#ifndef __CDBDUMP_H__
#define __CDBDUMP_H__

#include "_Elastos_Sql_SQLite_CDBDump.h"
#include "CShell.h"
#include <coredef.h>

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CDBDump)
    , public Object
    , public IDBDump
    , public ICallback
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI Columns(
        /* [in] */ const ArrayOf<String> & coldata);

    CARAPI Types(
        /* [in] */ const ArrayOf<String> & types);

    CARAPI Newrow(
        /* [in] */ const ArrayOf<String> & args,
        /* [out] */ Boolean * value);

    CARAPI constructor(
        /* [in] */ IShell * ms,
        /* [in] */ const ArrayOf<String> & tables);

private:
    AutoPtr<CShell> s;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CDBDUMP_H__
