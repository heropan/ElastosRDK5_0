
#include "CShell.h"
#include <elastos/core/StringBuffer.h>
//#include "CPrintWriter.h"

//using Elastos::IO::CPrintWriter;
using Elastos::Core::StringBuffer;

namespace Elastos {
namespace Sql {
namespace SQLite {

CAR_OBJECT_IMPL(CShell);

CAR_INTERFACE_IMPL_2(CShell, Object, IShell, ICallback);

CShell::CShell()
    : echo(FALSE)
    , count(0)
    , mode(0)
    , showHeader(FALSE)
{}

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
    //TODO
    assert(0);
    // FAIL_RETURN(CPrintWriter::New(ps,(IPrintWriter **)&pw));
    // FAIL_RETURN(CPrintWriter::New(errs,(IPrintWriter **)&err));
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
    /* [in] */ ArrayOf<String> * args)
{
    cols = NULL;
    if (args) {
        cols = args->Clone();
    }
    return NOERROR;
}

ECode CShell::Types(
    /* [in] */ ArrayOf<String> * types)
{
    /* Empty body to satisfy SQLite.Callback Int32erface. */
    return NOERROR;
}

ECode CShell::Newrow(
    /* [in] */ ArrayOf<String> * args,
    /* [out] */ Boolean * pValue)
{
    Int32 i = 0;
    String tname(NULL);
    switch (mode) {
    case MODE_Line:
        {
            if (args == NULL || args->GetLength() == 0) {
                break;
            }
            if (count++ > 0) {
                pw->Print(String(""));
            }
            for (i = 0; i < args->GetLength(); i++) {
                pw->Print((*cols)[i] + String(" = ") + ((*args)[i] == NULL ? String("NULL") : (*args)[i]));
            }
            break;
        }

    case MODE_Column:
        {
            String csep(NULL);
            if (count++ == 0) {
                colwidth = ArrayOf<Int32>::Alloc(args->GetLength());
                for (i = 0; i < args->GetLength(); i++) {
                    Int32 w = 0 ;
                    w = ((*cols)[i]).GetLength();
                    if (w < 10) {
                        w = 10;
                    }
                    (*colwidth)[i] = w;
                    if (showHeader) {
                        pw->Print(csep + (*cols)[i]);
                        csep = String(" ");
                    }
                }
                if (showHeader) {
                    pw->Print(String(""));
                }
            }
            if (args->GetLength() == 0) {
                break;
            }
            csep = String("");
            for (i = 0; i < args->GetLength(); i++) {
                pw->Print(csep + ((*args)[i] == NULL ? "NULL" : (*args)[i]));
                csep = String(" ");
            }
            pw->Print(String(""));
            break;
        }

    case MODE_Semi:
    case MODE_List:
        {
            if (count++ == 0 && showHeader) {
            for (i = 0; i < args->GetLength(); i++) {
                pw->Print((*cols)[i] +
                     (i == args->GetLength() - 1 ? "\n" : sep));
            }
            }
            if (args->GetLength() == 0) {
            break;
            }
            for (i = 0; i < args->GetLength(); i++) {
            pw->Print((*args)[i] == NULL ? String("NULL") : (*args)[i]);
            if (mode == MODE_Semi) {
            pw->Print(String(";"));
            } else if (i < args->GetLength() - 1) {
                pw->Print(sep);
            }
            }
            pw->Print(String(""));
            break;
        }

    case MODE_Html:
        {
            if (count++ == 0 && showHeader) {
                pw->Print(String("<TR>"));
                for (i = 0; i < args->GetLength(); i++) {
                    pw->Print(String("<TH>") + HtmlQuote((*cols)[i]) + String("</TH>"));
            }
            pw->Print(String("</TR>"));
            }
            if (args->GetLength() == 0) {
            break;
            }
            pw->Print(String("<TR>"));
            for (i = 0; i < args->GetLength(); i++) {
                pw->Print(String("<TD>") + HtmlQuote((*args)[i]) + String("</TD>"));
            }
            pw->Print(String("</TR>"));
            break;
        }
    case MODE_Insert:
        {
            if (args->GetLength() == 0) {
            break;
            }
            tname = tableName;
            if (destTable != NULL) {
                tname = destTable;
            }
            pw->Print(String("INSERT Int32O ") + tname + String(" VALUES("));
            for (i = 0; i < args->GetLength(); i++) {
                String tsep = i > 0 ? String(",") : String("");
            if ((*args)[i] == NULL) {
                pw->Print(tsep + String("NULL"));
            }
            else if (IsNumeric((*args)[i])) {
                 pw->Print(tsep + (*args)[i]);
            }
            else {
                pw->Print(tsep + SqlQuote((*args)[i]));
            }
            }
            pw->Print(String(");"));
            break;
        }

    case MODE_Insert2:
        {
            if (args->GetLength() == 0) {
            break;
            }
            tname = tableName;
            if (destTable != NULL) {
                tname = destTable;
            }
            pw->Print(String("INSERT Int32O ") + tname + String(" VALUES("));
            for (i = 0; i < args->GetLength(); i++) {
                String tsep = i > 0 ? String(",") : String("");
            pw->Print(tsep + (*args)[i]);
            }
            pw->Print(String(");"));
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
    for (Int32 i = 0; i < charArray->GetLength(); i++) {
        c = (*charArray)[i];
        if (c == '<') {
            sb.Append("&lt;");
        }
        else if (c == '>') {
            sb.Append("&gt;");
        }
        else if (c == '&') {
            sb.Append("&amp;");
        }
        else {
            Int32 x = c;
            if (x < 32 || x > 127) {
                sb.Append("&#");
                sb.AppendChar(c);
                sb.Append(";");

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
    Int32 i = 0, single = 0, dbl = 0;
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
    Int32 i, single = 0, dbl = 0;
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
