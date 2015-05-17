
#include "cmdef.h"
#include "CPrintWriter.h"

using Elastos::Core::EIID_IAppendable;

namespace Elastos {
namespace IO {

ECode CPrintWriter::constructor(
    /* [in] */ IOutputStream* outs)
{
    return PrintWriter::Init(outs);
}

ECode CPrintWriter::constructor(
    /* [in] */ IOutputStream* outs,
    /* [in] */ Boolean autoflush)
{
    return PrintWriter::Init(outs, autoflush);
}

ECode CPrintWriter::constructor(
    /* [in] */ IWriter* wr)
{
    return PrintWriter::Init(wr);
}

ECode CPrintWriter::constructor(
    /* [in] */ IWriter* wr,
    /* [in] */ Boolean autoflush)
{
    return PrintWriter::Init(wr, autoflush);
}

ECode CPrintWriter::constructor(
    /* [in] */ IFile* file)
{
    return PrintWriter::Init(file);
}

ECode CPrintWriter::constructor(
    /* [in] */ IFile* file,
    /* [in] */ const String& csn)
{
    return PrintWriter::Init(file, csn);
}

ECode CPrintWriter::constructor(
    /* [in] */ const String& fileName)
{
    return PrintWriter::Init(fileName);
}

ECode CPrintWriter::constructor(
    /* [in] */ const String& fileName,
    /* [in] */ const String& csn)
{
    return PrintWriter::Init(fileName, csn);
}

PInterface CPrintWriter::Probe(
    /* [in]  */ REIID riid)
{
    return _CPrintWriter::Probe(riid);
}

ECode CPrintWriter::CheckError(
    /* [out] */ Boolean* hasError)
{
    return PrintWriter::CheckError(hasError);
}

ECode CPrintWriter::Close()
{
    return PrintWriter::Close();
}

ECode CPrintWriter::Flush()
{
    return PrintWriter::Flush();
}

ECode CPrintWriter::Format(
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args)
{
    return PrintWriter::Format(format, args);
}

ECode CPrintWriter::FormatEx(
    /* [in] */ ILocale* l,
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args)
{
    return PrintWriter::FormatEx(l, format, args);
}

ECode CPrintWriter::Printf(
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args)
{
    return PrintWriter::Printf(format, args);
}

ECode CPrintWriter::PrintfEx(
    /* [in] */ ILocale* l,
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args)
{
    return PrintWriter::PrintfEx(l, format, args);
}

ECode CPrintWriter::PrintChars(
    /* [in] */ const ArrayOf<Char32>& charArray)
{
    return PrintWriter::PrintChars(charArray);
}

ECode CPrintWriter::PrintChar(
    /* [in] */ Char32 ch)
{
    return PrintWriter::PrintChar(ch);
}

ECode CPrintWriter::PrintDouble(
    /* [in] */ Double dnum)
{
    return PrintWriter::PrintDouble(dnum);
}

ECode CPrintWriter::PrintFloat(
    /* [in] */ Float fnum)
{
    return PrintWriter::PrintFloat(fnum);
}

ECode CPrintWriter::PrintInt32(
    /* [in] */ Int32 inum)
{
    return PrintWriter::PrintInt32(inum);
}

ECode CPrintWriter::PrintInt64(
    /* [in] */ Int64 lnum)
{
    return PrintWriter::PrintInt64(lnum);
}

ECode CPrintWriter::PrintObject(
    /* [in] */ IInterface* obj)
{
    return PrintWriter::PrintObject(obj);
}

ECode CPrintWriter::PrintString(
    /* [in] */ const String& str)
{
    return PrintWriter::PrintString(str);
}

ECode CPrintWriter::PrintBoolean(
    /* [in] */ Boolean result)
{
    return PrintWriter::PrintBoolean(result);
}

ECode CPrintWriter::Println()
{
    return PrintWriter::Println();
}

ECode CPrintWriter::PrintCharsln(
    /* [in] */ const ArrayOf<Char32>& charArray)
{
    return PrintWriter::PrintCharsln(charArray);
}

ECode CPrintWriter::PrintCharln(
    /* [in] */ Char32 ch)
{
    return PrintWriter::PrintCharln(ch);
}

ECode CPrintWriter::PrintDoubleln(
    /* [in] */ Double dnum)
{
    return PrintWriter::PrintDoubleln(dnum);
}

ECode CPrintWriter::PrintFloatln(
    /* [in] */ Float fnum)
{
    return PrintWriter::PrintFloatln(fnum);
}

ECode CPrintWriter::PrintInt32ln(
    /* [in] */ Int32 inum)
{
    return PrintWriter::PrintInt32ln(inum);
}

ECode CPrintWriter::PrintInt64ln(
    /* [in] */ Int64 lnum)
{
    return PrintWriter::PrintInt64ln(lnum);
}

ECode CPrintWriter::PrintObjectln(
    /* [in] */ IInterface* obj)
{
    return PrintWriter::PrintObjectln(obj);
}

ECode CPrintWriter::PrintStringln(
    /* [in] */ const String& str)
{
    Mutex::Autolock lock(_m_syncLock);

    return PrintWriter::PrintStringln(str);
}

ECode CPrintWriter::PrintBooleanln(
    /* [in] */ Boolean result)
{
    return PrintWriter::PrintBooleanln(result);
}

ECode CPrintWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    return PrintWriter::Write(oneChar32);
}

ECode CPrintWriter::WriteChars(
    /* [in] */ const ArrayOf<Char32>& buffer)
{
    return PrintWriter::WriteChars(buffer);
}

ECode CPrintWriter::WriteCharsEx(
    /* [in] */ const ArrayOf<Char32>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return PrintWriter::WriteCharsEx(buffer, offset, count);
}

ECode CPrintWriter::WriteString(
    /* [in] */ const String& str)
{
    return PrintWriter::WriteString(str);
}

ECode CPrintWriter::WriteStringEx(
    /* [in] */ const String& str,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return PrintWriter::WriteStringEx(str, offset, count);
}

ECode CPrintWriter::AppendChar(
    /* [in] */ Char32 c)
{
    return PrintWriter::AppendChar(c);
}

ECode CPrintWriter::AppendCharSequence(
    /* [in] */ ICharSequence* csq)
{
    return PrintWriter::AppendCharSequence(csq);
}

ECode CPrintWriter::AppendCharSequenceEx(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return PrintWriter::AppendCharSequenceEx(csq, start, end);
}

ECode CPrintWriter::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);
    AutoPtr<IInterface> obj = PrintWriter::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}


} // namespace IO
} // namespace Elastos
