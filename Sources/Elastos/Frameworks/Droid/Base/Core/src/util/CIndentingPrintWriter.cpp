
#include "cmdef.h"
#include "util/CIndentingPrintWriter.h"

using Elastos::Core::EIID_IAppendable;
using Elastos::IO::EIID_ICloseable;
using Elastos::IO::EIID_IFlushable;
using Elastos::IO::EIID_IWriter;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

CIndentingPrintWriter::CIndentingPrintWriter()
    : mEmptyLine(TRUE)
{}

ECode CIndentingPrintWriter::constructor(
    /* [in] */ IWriter* writer,
    /* [in] */ const String& indent)
{
    FAIL_RETURN(PrintWriter::Init(writer))
    mIndent = indent;
    return NOERROR;
}

PInterface CIndentingPrintWriter::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IIndentingPrintWriter*)this;
    }
    else if (riid == EIID_IIndentingPrintWriter) {
        return (IIndentingPrintWriter*)this;
    }
    else if (riid == EIID_ICloseable) {
        return (ICloseable*)this;
    }
    else if (riid == EIID_IFlushable) {
        return (IFlushable*)this;
    }
    else if (riid == EIID_IAppendable) {
        return (IAppendable*)this;
    }
    else if (riid == EIID_IWriter) {
        return (IWriter*)this;
    }

    return NULL;
}

ECode CIndentingPrintWriter::IncreaseIndent()
{
    FAIL_RETURN(mBuilder.AppendString(mIndent))
    mCurrent = NULL;
    return NOERROR;
}

ECode CIndentingPrintWriter::DecreaseIndent()
{
    FAIL_RETURN(mBuilder.Delete(0, mIndent.GetLength()))
    mCurrent = NULL;
    return NOERROR;
}

ECode CIndentingPrintWriter::PrintPair(
    /* [in] */ const String& key,
    /* [in] */ IInterface* value)
{
    String str;
    FAIL_RETURN(IObject::Probe(value)->ToString(&str))

    StringBuilder sb;
    sb += String(key);
    sb += String("=");
    sb += str;
    sb += String(" ");
    return PrintString(sb.ToString());
}

ECode CIndentingPrintWriter::WriteCharsEx(
    /* [in] */ const ArrayOf<Char32>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    const Int32 bufferEnd = offset + count;
    Int32 lineStart = offset;
    Int32 lineEnd = offset;
    while (lineEnd < bufferEnd) {
        Char32 ch = buffer[lineEnd++];
        if (ch == '\n') {
            FAIL_RETURN(WriteIndent())
            FAIL_RETURN(PrintWriter::WriteCharsEx(buffer, lineStart, lineEnd - lineStart))
            lineStart = lineEnd;
            mEmptyLine = TRUE;
        }
    }

    if (lineStart != lineEnd) {
        FAIL_RETURN(WriteIndent())
        FAIL_RETURN(PrintWriter::WriteCharsEx(buffer, lineStart, lineEnd - lineStart))
    }

    return NOERROR;
}

ECode CIndentingPrintWriter::WriteIndent()
{
    if (mEmptyLine) {
        mEmptyLine = FALSE;
        if (mBuilder.GetLength() != 0) {
            if (mCurrent == NULL) {
                String str = mBuilder.ToString();
                Int32 len = str.GetLength();
                mCurrent = ArrayOf<Char32>::Alloc(len);
                for (Int32 i = 0; i < len; i++) {
                    (*mCurrent)[i] = str.GetChar(i);
                }
            }
            return PrintWriter::WriteCharsEx(*mCurrent, 0, mCurrent->GetLength());
        }
    }

    return NOERROR;
}

ECode CIndentingPrintWriter::AppendChar(
    /* [in] */ Char32 c)
{
    return PrintWriter::AppendChar(c);
}

ECode CIndentingPrintWriter::AppendCharSequence(
    /* [in] */ ICharSequence* csq)
{
    return PrintWriter::AppendCharSequence(csq);
}

ECode CIndentingPrintWriter::AppendCharSequenceEx(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return PrintWriter::AppendCharSequenceEx(csq, start, end);
}

ECode CIndentingPrintWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    return PrintWriter::Write(oneChar32);
}

ECode CIndentingPrintWriter::WriteChars(
    /* [in] */ const ArrayOf<Char32>& buffer)
{
    return PrintWriter::WriteChars(buffer);
}

ECode CIndentingPrintWriter::WriteString(
    /* [in] */ const String& str)
{
    return PrintWriter::WriteString(str);
}

ECode CIndentingPrintWriter::WriteStringEx(
    /* [in] */ const String& str,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return PrintWriter::WriteStringEx(str, offset, count);
}

ECode CIndentingPrintWriter::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return PrintWriter::CheckError(hasError);
}

ECode CIndentingPrintWriter::Format(
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args)
{
    return PrintWriter::Format(format, args);
}

ECode CIndentingPrintWriter::FormatEx(
    /* [in] */ ILocale* l,
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args)
{
    return PrintWriter::FormatEx(l, format, args);
}

ECode CIndentingPrintWriter::Printf(
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args)
{
    return PrintWriter::Printf(format, args);
}

ECode CIndentingPrintWriter::PrintfEx(
    /* [in] */ ILocale* l,
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args)
{
    return PrintWriter::PrintfEx(l, format, args);
}

ECode CIndentingPrintWriter::PrintChars(
    /* [in] */ const ArrayOf<Char32>& charArray)
{
    return PrintWriter::PrintChars(charArray);
}

ECode CIndentingPrintWriter::PrintChar(
    /* [in] */ Char32 ch)
{
    return PrintWriter::PrintChar(ch);
}

ECode CIndentingPrintWriter::PrintDouble(
    /* [in] */ Double dnum)
{
    return PrintWriter::PrintDouble(dnum);
}

ECode CIndentingPrintWriter::PrintFloat(
    /* [in] */ Float fnum)
{
    return PrintWriter::PrintFloat(fnum);
}

ECode CIndentingPrintWriter::PrintInt32(
    /* [in] */ Int32 inum)
{
    return PrintWriter::PrintInt32(inum);
}

ECode CIndentingPrintWriter::PrintInt64(
    /* [in] */ Int64 lnum)
{
    return PrintWriter::PrintInt64(lnum);
}

ECode CIndentingPrintWriter::PrintObject(
    /* [in] */ IInterface* obj)
{
    return PrintWriter::PrintObject(obj);
}

ECode CIndentingPrintWriter::PrintString(
    /* [in] */ const String& str)
{
    return PrintWriter::PrintString(str);
}

ECode CIndentingPrintWriter::PrintBoolean(
    /* [in] */ Boolean result)
{
    return PrintWriter::PrintBoolean(result);
}

ECode CIndentingPrintWriter::Println()
{
    return PrintWriter::Println();
}

ECode CIndentingPrintWriter::PrintCharsln(
    /* [in] */ const ArrayOf<Char32>& charArray)
{
    return PrintWriter::PrintCharsln(charArray);
}

ECode CIndentingPrintWriter::PrintCharln(
    /* [in] */ Char32 ch)
{
    return PrintWriter::PrintCharln(ch);
}

ECode CIndentingPrintWriter::PrintDoubleln(
    /* [in] */ Double dnum)
{
    return PrintWriter::PrintDoubleln(dnum);
}

ECode CIndentingPrintWriter::PrintFloatln(
    /* [in] */ Float fnum)
{
    return PrintWriter::PrintFloatln(fnum);
}

ECode CIndentingPrintWriter::PrintInt32ln(
    /* [in] */ Int32 inum)
{
    return PrintWriter::PrintInt32ln(inum);
}

ECode CIndentingPrintWriter::PrintInt64ln(
    /* [in] */ Int64 lnum)
{
    return PrintWriter::PrintInt64ln(lnum);
}

ECode CIndentingPrintWriter::PrintObjectln(
    /* [in] */ IInterface* obj)
{
    return PrintWriter::PrintObjectln(obj);
}

ECode CIndentingPrintWriter::PrintStringln(
    /* [in] */ const String& str)
{
    return PrintWriter::PrintStringln(str);
}

ECode CIndentingPrintWriter::PrintBooleanln(
    /* [in] */ Boolean result)
{
    return PrintWriter::PrintBooleanln(result);
}

ECode CIndentingPrintWriter::Close()
{
    return PrintWriter::Close();
}

ECode CIndentingPrintWriter::Flush()
{
    return PrintWriter::Flush();
}

ECode CIndentingPrintWriter::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);
    AutoPtr<IInterface> obj = PrintWriter::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos
