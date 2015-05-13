
#include "CTestBusyHandler.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

ECode CTestBusyHandler::Busy(
    /* [in] */ const String& table,
    /* [in] */ Int32 count,
    /* [out] */ Boolean* value)
{
    *value = TRUE;
    return NOERROR;
}

ECode CTestBusyHandler::Columns(
    /* [in] */ const ArrayOf<String>& coldata)
{
    return NOERROR;
}

ECode CTestBusyHandler::Types(
    /* [in] */ const ArrayOf<String>& types)
{
    return NOERROR;
}

ECode CTestBusyHandler::Newrow(
    /* [in] */ const ArrayOf<String>& rowdata,
    /* [out] */ Boolean* value)
{
    *value = FALSE;
    return NOERROR;
}

ECode CTestBusyHandler::constructor()
{
    return NOERROR;
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos