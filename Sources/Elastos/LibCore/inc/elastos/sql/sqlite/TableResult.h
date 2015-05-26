
#ifndef __TABLERESULT_H__
#define __TABLERESULT_H__

#include <coredef.h>


using Elastos::Utility::Vector;

namespace Elastos {
namespace Sql {
namespace SQLite {

class TableResult
    : public Object
    , public ITableResult
    , public ICallback
{
public:
    CAR_INTERFACE_DECL();

    virtual CARAPI Columns(
        /* [in] */ const ArrayOf<String> & coldata);

    virtual CARAPI Types(
        /* [in] */ const ArrayOf<String> & intypes);

    virtual CARAPI Newrow(
        /* [in] */ const ArrayOf<String> & rowdata);

    virtual CARAPI ToString(
        /* [out] */ String* value);

    TableResult();

    TableResult(
        /* [in] */ Int32 maxrows);

    virtual CARAPI Clear();

    CARAPI Init();

    CARAPI Init(
        /* [in] */ Int32 maxrows);

public:
    Int32 ncolumns;
    Int32 nrows;
    AutoPtr<ArrayOf<String> > column;
    AutoPtr<ArrayOf<String> > types;
    Int32 maxrows;
    Vector<AutoPtr<ArrayOf<String> > > rows;
    Boolean atmaxrows;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __TABLERESULT_H__
