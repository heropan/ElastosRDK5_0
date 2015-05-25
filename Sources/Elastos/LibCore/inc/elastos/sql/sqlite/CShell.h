
#ifndef __CSHELL_H__
#define __CSHELL_H__

#include "_Elastos_Sql_SQLite_CShell.h"
#include "CDatabase.h"

using Elastos::IO::IPrintWriter;
using Elastos::IO::IPrintStream;

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CShell)
    , public Object
    , public IShell
    , public ICallback
{
public:
    CAR_OBJECT_DECL();

    CARAPI Columns(
        /* [in] */ const ArrayOf<String> & args);

    CARAPI Types(
        /* [in] */ const ArrayOf<String> & types);

    CARAPI Newrow(
        /* [in] */ const ArrayOf<String> & args,
        /* [out] */ Boolean * pValue);

    CARAPI constructor(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ IPrintWriter* err);

    CARAPI constructor(
        /* [in] */ IPrintStream* pw,
        /* [in] */ IPrintStream* errs);

public:
    CARAPI_(void) Clone(
        /* [out] */ CShell& s);

    CARAPI_(void) SetTableName(
        /* [out] */ String str);

    static CARAPI_(String) HtmlQuote(
        /* [in] */ const String& str);

    static CARAPI_(Boolean) IsNumeric(
        /* [in] */ const String& str);

    static CARAPI_(String) SqlQuoteDbl(
        /* [in] */ const String& str);

    static CARAPI_(String) SqlQuote(
        /* [in] */ const String& str);

public:
    AutoPtr<CDatabase> db;
    Boolean echo;
    Int32 count;
    Int32 mode;
    Boolean showHeader;
    String tableName;
    String sep;
    AutoPtr<ArrayOf<String> > cols;
    AutoPtr<ArrayOf<Int32> > colwidth;
    String destTable;
    AutoPtr<IPrintWriter> pw;
    AutoPtr<IPrintWriter> err;

    const static Int32 MODE_Line = 0;
    const static Int32 MODE_Column = 1;
    const static Int32 MODE_List = 2;
    const static Int32 MODE_Semi = 3;
    const static Int32 MODE_Html = 4;
    const static Int32 MODE_Insert = 5;
    const static Int32 MODE_Insert2 = 6;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CSHELL_H__
