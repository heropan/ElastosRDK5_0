
#include "CTableResult.h"
#include <elastos/StringBuffer.h>

using Elastos::Core::StringBuffer;

namespace Elastos {
namespace Sql {
namespace SQLite {

ECode CTableResult::Columns(
    /* [in] */ const ArrayOf<String> & coldata)
{
    TableResult::Columns(coldata);
    return NOERROR;
}

ECode CTableResult::Types(
    /* [in] */ const ArrayOf<String> & intypes)
{
    TableResult::Types(intypes);
    return NOERROR;
}

ECode CTableResult::Newrow(
    /* [in] */ const ArrayOf<String> & rowdata,
    /* [out] */ Boolean * value)
{
    *value = TableResult::Newrow(rowdata);
    return NOERROR;
}

ECode CTableResult::ToString(
    /* [out] */ String * str)
{
    *str = TableResult::ToString();
    return NOERROR;
}

ECode CTableResult::constructor()
{
    Clear();
    return NOERROR;
}

ECode CTableResult::constructor(
    /* [in] */ Int32 maxrows)
{
    maxrows = maxrows;
    Clear();
    return NOERROR;
}

ECode CTableResult::Clear()
{
    TableResult::Clear();
    return NOERROR;
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
