
#include "CDBDump.h"
#include "CTableResult.h"
#include <elastos/StringBuffer.h>
#include <elastos/StringUtils.h>

using Elastos::IO::IFlushable;
using Elastos::IO::EIID_IFlushable;
using Elastos::Core::StringBuffer;
using Elastos::Core::StringUtils;

namespace Elastos {
namespace Sql {
namespace SQLite {

ECode CDBDump::Columns(
    /* [in] */ const ArrayOf<String> & coldata)
{
    return NOERROR;
}

ECode CDBDump::Types(
    /* [in] */ const ArrayOf<String> & types)
{
    return NOERROR;
}

ECode CDBDump::Newrow(
    /* [in] */ const ArrayOf<String> & args,
    /* [out] */ Boolean * value)
{
    if (args.GetLength() != 3) {
        *value = TRUE;
    }
    s->pw->PrintStringln(args[2] + String(";"));
    if (args[1].Equals("table") == 0) {
        CShell s2 ;
        s->Clone(s2);
        s2.mode = CShell::MODE_Insert;
        s2.SetTableName(args[0]);
        AutoPtr<ArrayOf<String> > qargs = ArrayOf<String>::Alloc(1);
        (*qargs)[0] = args[0];
        Boolean isflag = FALSE;
        s2.db->Is3(&isflag);
        ECode ec = NOERROR;
        if (isflag) {
            AutoPtr<ITableResult> t ;
            s2.db->GetTableEx3(String("PRAGMA table_info('%q')")  , *qargs,(ITableResult **)&t);
            String query;
            if (t != NULL) {
                StringBuffer sb;
                String sep = String("");

                sb.AppendCStr("SELECT ");
                for (UInt32 i = 0; i < ((CTableResult *)t.Get())->nrows; i++) {
                    String col = (*(((CTableResult *)t.Get())->rows)[i])[1];
                    sb.AppendCStr(sep + "quote(" + CShell::SqlQuoteDbl(col) + ")");
                    sep = ",";
                }
                sb.AppendCStr(" from '%q'");
                query = sb.ToString();
                s2.mode = CShell::MODE_Insert2;
            }
            else {
                query = String("SELECT * from '%q'");
            }
            ec = s2.db->ExecEx(query, &s2, *qargs);
        }
        else {
            ec = s2.db->ExecEx(String("SELECT * from '%q'"), &s2, *qargs);
        }
        if (ec != NOERROR) {
            s->err->PrintStringln(String("SQL Error: ")+StringUtils::Int32ToString(ec,16));
            AutoPtr<IFlushable> iflush = (IFlushable*)s->err->Probe(EIID_IFlushable);
            iflush->Flush();
            *value = TRUE;
        }
    }
    *value = FALSE;
    return NOERROR;
}

ECode CDBDump::constructor(
    /* [in] */ IShell * ms,
    /* [in] */ const ArrayOf<String> & tables)
{
    s = (CShell *)ms;
    s->pw->PrintStringln(String("BEGIN TRANSACTION;"));
    AutoPtr<IFlushable> iflush = (IFlushable*)s->err->Probe(EIID_IFlushable);
    if (tables.GetLength() == 0) {
        ECode  ec = s->db->Exec(String("SELECT name, type, sql FROM sqlite_master ") +
                        String("WHERE type!='meta' AND sql NOT NULL ") +
                        String("ORDER BY substr(type,2,1), name"), (ICallback *)this);
        if (ec != NOERROR)
        {
            s->err->PrintStringln(String("SQL Error: ")+StringUtils::Int32ToString(ec,16));
            iflush->Flush();
        }
    } else {
        AutoPtr<ArrayOf<String> > arg = ArrayOf<String>::Alloc(1);
        for (UInt32 i = 0; i < tables.GetLength(); i++) {
            (*arg)[0] = tables[i];
            ECode ec = s->db->ExecEx(String("SELECT name, type, sql FROM sqlite_master ") +
                                   String("WHERE tbl_name LIKE '%q' AND type!='meta' ") +
                                   String(" AND sql NOT NULL ") +
                                   String(" ORDER BY substr(type,2,1), name"),
                                   (ICallback *)this, *arg);
         if(ec != NOERROR) {
            s->err->PrintStringln(String("SQL Error: ")+StringUtils::Int32ToString(ec,16));
            iflush->Flush();
        }
        }
    }
    s->pw->PrintStringln(String("COMMIT;"));
    return NOERROR;
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
