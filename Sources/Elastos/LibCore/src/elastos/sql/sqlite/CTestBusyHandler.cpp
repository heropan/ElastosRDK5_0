
#include "CTestBusyHandler.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

CAR_OBJECT_IMPL(CTestBusyHandler);

CAR_INTERFACE_IMPL_2(CTestBusyHandler, Object, IBusyHandler, ICallback);

ECode CTestBusyHandler::Busy(
    /* [in] */ const String& table,
    /* [in] */ Int32 count,
    /* [out] */ Boolean* value)
{
    *value = TRUE;
    return NOERROR;
}

ECode CTestBusyHandler::Columns(
    /* [in] */ ArrayOf<String> * coldata)
{
    return NOERROR;
}

ECode CTestBusyHandler::Types(
    /* [in] */ ArrayOf<String> * types)
{
    return NOERROR;
}

ECode CTestBusyHandler::Newrow(
    /* [in] */ ArrayOf<String> * rowdata,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
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