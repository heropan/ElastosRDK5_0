
#ifndef __CDATABASEX_H__
#define __CDATABASEX_H__

#include "_CDatabaseX.h"
#include "cmdef.h"
#include "Database.h"
#include "DatabaseMacro.h"
#include <elastos/Object.h>

using Elastos::Core::Threading::Object;

namespace Elastos {
namespace Sql {
namespace SQLite {
namespace JDBC{

CarClass(CDatabaseX) , public Database
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    DATABASE_METHODS_DECL()

    CARAPI constructor();

    CARAPI ExecEx2(
        /* [in] */ const String& sql,
        /* [in] */ ICallback * cb);

    CARAPI ExecEx3(
        /* [in] */ const String& sql,
        /* [in] */ ICallback * cb,
        /* [in] */ const ArrayOf<String>& args);

    CARAPI GetTableEx5(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String>& args,
        /* [out] */ ITableResult ** tbl);

    CARAPI GetTableEx6(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String>& args,
        /* [in] */ ITableResult * tbl);

protected:
    CARAPI_(void) Wait(Int32 ms);

private:
    Object lock;
};

} // namespace JDBC
} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CDATABASEX_H__
