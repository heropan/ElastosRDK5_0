
#include "TableResult.h"
#include <elastos/StringBuffer.h>

using Elastos::Core::StringBuffer;

namespace Elastos {
namespace Sql {
namespace SQLite {

CAR_INTERFACE_IMPL_2(TableResult, Object, ITableResult, ICallback)

ECode TableResult::Columns(
    /* [in] */ const ArrayOf<String> & coldata)
{
    column = const_cast<ArrayOf<String> * >(&coldata);
    ncolumns = column->GetLength();
    return NOERROR;
}

ECode TableResult::Types(
    /* [in] */ const ArrayOf<String> & intypes)
{
    types = const_cast<ArrayOf<String> * >(&intypes);
    return NOERROR;
}

Boolean TableResult::Newrow(
    /* [in] */ const ArrayOf<String> & rowdata,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    if (rowdata.GetLength()>0) {
        if (maxrows > 0 && nrows >= maxrows) {
            atmaxrows = TRUE;
            *value = TRUE;
            return NOERROR;
        }
        rows.PushBack(const_cast<ArrayOf<String> *>(&rowdata));
        nrows++;
    }

    *value = FALSE;
    return NOERROR;
}

ECode TableResult::ToString(
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    StringBuffer sb;
    Int32 i = 0;
    for (i = 0; i < ncolumns; i++) {
        sb.AppendString((*column)[i].IsNull() ? String("NULL") : (*column)[i]);
        sb.AppendChar('|');
    }
    sb.AppendChar('\n');
    for (i = 0; i < nrows; i++) {
        Int32 k = 0;
        AutoPtr<ArrayOf<String> > row = rows[i];
        for (k = 0; k < ncolumns; k++) {
            sb.AppendString((*row)[k].IsNull() ? String("NULL") : (*row)[k]);
            sb.AppendChar('|');
        }
        sb.AppendChar('\n');
    }
    *value = sb.ToString();
    return NOERROR;
}

TableResult::TableResult()
    : ncolumns(0)
    , nrows(0)
    , maxrows(0)
    , atmaxrows(FALSE)
{
}

TableResult::TableResult(
    /* [in] */ Int32 maxrows)
    : ncolumns(0)
    , nrows(0)
    , maxrows(0)
    , atmaxrows(FALSE)
{
}

ECode TableResult::Clear()
{
    column = ArrayOf<String>::Alloc(1);
    types = NULL;
    // rows = new Vector<AutoPtr<ArrayOf<String> > >();
    ncolumns = nrows = 0;
    atmaxrows = FALSE;
    return NOERROR;
}

ECode TableResult::Init()
{
    return Clear();
}

ECode TableResult::Init(Int32 maxrows)
{
    maxrows = maxrows;
    return Clear();
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
