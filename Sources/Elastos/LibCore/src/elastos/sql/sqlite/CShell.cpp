
#include "cmdef.h"
#include "CShell.h"
#include <elastos/StringBuffer.h>
#include "CPrintWriter.h"

using Elastos::IO::CPrintWriter;
using Elastos::Core::StringBuffer;

namespace Elastos {
namespace Sql {
namespace SQLite {

CAR_OBJECT_IMPL(CShell)

CARAPI CShell::constructor(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ IPrintWriter* err)
{
    pw = pw;
    err = err;
    return NOERROR;
}

CARAPI CShell::constructor(
    /* [in] */ IPrintStream* ps,
    /* [in] */ IPrintStream* errs)
{
    FAIL_RETURN(CPrintWriter::New(ps,(IPrintWriter **)&pw));
    FAIL_RETURN(CPrintWriter::New(errs,(IPrintWriter **)&err));
    return NOERROR;
}
void CShell::Clone(
    /* [out] */ CShell& s)
{
    s.db = db;
    s.echo = echo;
    s.mode = mode;
    s.count = 0;
    s.showHeader = showHeader;
    s.tableName = tableName;
    s.sep = sep;
    s.colwidth = colwidth;
}

ECode CShell::Columns(
    /* [in] */ const ArrayOf<String> & args)
{
    cols=args.Clone();
    return NOERROR;
}

ECode CShell::Types(
    /* [in] */ const ArrayOf<String> & types)
{
    /* Empty body to satisfy SQLite.Callback Int32erface. */
    return NOERROR;
}

ECode CShell::Newrow(
    /* [in] */ const ArrayOf<String> & args,
    /* [out] */ Boolean * pValue)
{
    Int32 i = 0;
    String tname(NULL);
    switch (mode) {
    case MODE_Line:
        {
            if (args.GetLength() == 0) {
                break;
            }
            if (count++ > 0) {
                pw->PrintStringln(String(""));
            }
            for (i = 0; i < args.GetLength(); i++) {
                pw->PrintStringln((*cols)[i] + String(" = ") + (args[i] == NULL ? String("NULL") : args[i]));
            }
            break;
        }

    case MODE_Column:
        {
            String csep(NULL);
            if (count++ == 0) {
                colwidth = ArrayOf<Int32>::Alloc(args.GetLength());
                for (i = 0; i < args.GetLength(); i++) {
                    Int32 w = 0 ;
                    w = ((*cols)[i]).GetLength();
                    if (w < 10) {
                        w = 10;
                    }
                    (*colwidth)[i] = w;
                    if (showHeader) {
                        pw->PrintStringln(csep + (*cols)[i]);
                        csep = String(" ");
                    }
                }
                if (showHeader) {
                    pw->PrintStringln(String(""));
                }
            }
            if (args.GetLength() == 0) {
                break;
            }
            csep = String("");
            for (i = 0; i < args.GetLength(); i++) {
                pw->PrintStringln(csep + (args[i] == NULL ? "NULL" : args[i]));
                csep = String(" ");
            }
            pw->PrintStringln(String(""));
            break;
        }

    case MODE_Semi:
    case MODE_List:
        {
            if (count++ == 0 && showHeader) {
            for (i = 0; i < args.GetLength(); i++) {
                pw->PrintStringln((*cols)[i] +
                     (i == args.GetLength() - 1 ? "\n" : sep));
            }
            }
            if (args.GetLength() == 0) {
            break;
            }
            for (i = 0; i < args.GetLength(); i++) {
            pw->PrintStringln(args[i] == NULL ? String("NULL") : args[i]);
            if (mode == MODE_Semi) {
            pw->PrintStringln(String(";"));
            } else if (i < args.GetLength() - 1) {
                pw->PrintStringln(sep);
            }
            }
            pw->PrintStringln(String(""));
            break;
        }

    case MODE_Html:
        {
            if (count++ == 0 && showHeader) {
                pw->PrintStringln(String("<TR>"));
                for (i = 0; i < args.GetLength(); i++) {
                    pw->PrintStringln(String("<TH>") + HtmlQuote((*cols)[i]) + String("</TH>"));
            }
            pw->PrintStringln(String("</TR>"));
            }
            if (args.GetLength() == 0) {
            break;
            }
            pw->PrintStringln(String("<TR>"));
            for (i = 0; i < args.GetLength(); i++) {
                pw->PrintStringln(String("<TD>") + HtmlQuote(args[i]) + String("</TD>"));
            }
            pw->PrintStringln(String("</TR>"));
            break;
        }
    case MODE_Insert:
        {
            if (args.GetLength() == 0) {
            break;
            }
            tname = tableName;
            if (destTable != NULL) {
                tname = destTable;
            }
            pw->PrintStringln(String("INSERT Int32O ") + tname + String(" VALUES("));
            for (i = 0; i < args.GetLength(); i++) {
                String tsep = i > 0 ? String(",") : String("");
            if (args[i] == NULL) {
                pw->PrintStringln(tsep + String("NULL"));
            }
            else if (IsNumeric(args[i])) {
                 pw->PrintStringln(tsep + args[i]);
            }
            else {
                pw->PrintStringln(tsep + SqlQuote(args[i]));
            }
            }
            pw->PrintStringln(String(");"));
            break;
        }

    case MODE_Insert2:
        {
            if (args.GetLength() == 0) {
            break;
            }
            tname = tableName;
            if (destTable != NULL) {
                tname = destTable;
            }
            pw->PrintStringln(String("INSERT Int32O ") + tname + String(" VALUES("));
            for (i = 0; i < args.GetLength(); i++) {
                String tsep = i > 0 ? String(",") : String("");
            pw->PrintStringln(tsep + args[i]);
            }
            pw->PrintStringln(String(");"));
            break;
        }
    }
    *pValue = false;
    return NOERROR;
}

String CShell::HtmlQuote(
    /* [in] */ const String& str)
{
    if (str == NULL) {
        return String("NULL");
    }

    StringBuffer sb;
    Char32 c;
    AutoPtr<ArrayOf<Char32> > charArray = str.GetChars();
    for (UInt32 i = 0; i < charArray->GetLength(); i++) {
        c = (*charArray)[i];
        if (c == '<') {
            sb.AppendCStr("&lt;");
        }
        else if (c == '>') {
            sb.AppendCStr("&gt;");
        }
        else if (c == '&') {
            sb.AppendCStr("&amp;");
        }
        else {
            Int32 x = c;
            if (x < 32 || x > 127) {
                sb.AppendCStr("&#");
                sb.AppendChar(c);
                sb.AppendCStr(";");

            } else {
                sb.AppendChar(c);
            }
        }
    }
    return sb.ToString();
}

Boolean CShell::IsNumeric(
    /* [in] */ const String& str) {
    // try {
    //     Double d = Double.valueOf(str);
    // } catch (java.lang.Exception e) {
    //     return false;
    // }
    return TRUE;
}

String CShell::SqlQuoteDbl(
    /* [in] */ const String& str)
{
    if (str == NULL) {
        return String("NULL");
    }

    Char32 c;
    AutoPtr<ArrayOf<Char32> > charArray = str.GetChars();
    UInt32 i = 0, single = 0, dbl = 0;
    for (i = 0; i < charArray->GetLength(); i++) {
        c = (*charArray)[i];
        if (c == '\'') {
            single++;
        }
        else if (c == '"') {
            dbl++;
        }
    }

    if (dbl == 0) {
        return String(String("\"") + str + String("\""));
    }

    StringBuffer sb("\"");
    for (i = 0; i < charArray->GetLength(); i++) {
        c = (*charArray)[i];
        if (c == '"') {
            sb += ("\"\"");
        }
        else {
            sb.AppendChar(c);
        }
    }
    return sb.ToString();
}

String CShell::SqlQuote(
    /* [in] */ const String& str)
{
    if (str.IsNull()) {
        return String("NULL");
    }

    AutoPtr<ArrayOf<Char32> > charArray = str.GetChars();
    UInt32 i, single = 0, dbl = 0;
    Char32 ch;
    for (i = 0; i < charArray->GetLength(); i++) {
        ch = (*charArray)[i];
        if (ch == '\'') {
            single++;
        } else if (ch == '"') {
            dbl++;
        }
    }
    if (single == 0) {
        return String("'") + str + String("'");
    }
    if (dbl == 0) {
        return String("\"") + str + String("\"");
    }

    StringBuffer sb("'");
    for (i = 0; i < charArray->GetLength(); i++) {
        ch = (*charArray)[i];
        if (ch == '\'') {
            sb += ("''");
        }
        else {
            sb.AppendChar(ch);
        }
    }
    return sb.ToString();
}

void CShell::SetTableName(
    /* [out] */ String str)
{
    if (str == NULL) {
        tableName = String("");
        return;
    }
    Boolean isflag = FALSE;
    db->Is3(&isflag);
    if (isflag) {
        tableName = SqlQuoteDbl(str);
    } else {
        tableName = SqlQuote(str);
    }
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
