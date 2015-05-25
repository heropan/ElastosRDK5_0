
#ifndef __TABLERESULT_H__
#define __TABLERESULT_H__

#include <coredef.h>


using Elastos::Utility::Vector;

namespace Elastos {
namespace Sql {
namespace SQLite {

class TableResult
{
public:

    CARAPI Columns(
        /* [in] */ const ArrayOf<String> & coldata);

    CARAPI Types(
        /* [in] */ const ArrayOf<String> & intypes);

    CARAPI_(Boolean) Newrow(
        /* [in] */ const ArrayOf<String> & rowdata);

    CARAPI_(String) ToString();

    TableResult();

    TableResult(
        /* [in] */ Int32 maxrows);

    // ~TableResult();

    CARAPI Clear();

    CARAPI_(void) Init();

    CARAPI_(void) Init(
        /* [in] */ Int32 maxrows);

public:
    Int32 ncolumns;
    Int32 nrows;
    AutoPtr<ArrayOf<String> > column;
    AutoPtr<ArrayOf<String> > types;
    Int32 maxrows ;
    Vector<AutoPtr<ArrayOf<String> > > rows;
    Boolean atmaxrows;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __TABLERESULT_H__
