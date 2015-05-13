
#include "cmdef.h"
#include "CCharArrayWriter.h"

namespace Elastos {
namespace IO {

ECode CCharArrayWriter::Close()
{
    return CharArrayWriter::Close();
}

ECode CCharArrayWriter::Flush()
{
    return CharArrayWriter::Flush();
}

ECode CCharArrayWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    return CharArrayWriter::Write(oneChar32);
}

ECode CCharArrayWriter::WriteChars(
    /* [in] */ const ArrayOf<Char32>& buffer)
{
    return CharArrayWriter::WriteChars(buffer);
}

ECode CCharArrayWriter::WriteCharsEx(
    /* [in] */ const ArrayOf<Char32>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return CharArrayWriter::WriteCharsEx(buffer, offset, count);
}

ECode CCharArrayWriter::WriteString(
    /* [in] */ const String& str)
{
    return CharArrayWriter::WriteString(str);
}

ECode CCharArrayWriter::WriteStringEx(
    /* [in] */ const String& str,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return CharArrayWriter::WriteStringEx(str, offset, count);
}

ECode CCharArrayWriter::AppendChar(
    /* [in] */ Char32 c)
{
    return CharArrayWriter::AppendChar(c);
}

ECode CCharArrayWriter::AppendCharSequence(
    /* [in] */ ICharSequence* csq)
{
    return CharArrayWriter::AppendCharSequence(csq);
}

ECode CCharArrayWriter::AppendCharSequenceEx(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return CharArrayWriter::AppendCharSequenceEx(csq, start, end);
}

ECode CCharArrayWriter::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return CharArrayWriter::CheckError(hasError);
}

ECode CCharArrayWriter::Reset()
{
    return CharArrayWriter::Reset();
}

ECode CCharArrayWriter::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    return CharArrayWriter::GetSize(size);
}

ECode CCharArrayWriter::ToCharArray(
    /* [out, callee] */ ArrayOf<Char32>** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    return CharArrayWriter::ToCharArray(buffer);
}

ECode CCharArrayWriter::WriteTo(
    /* [in] */ IWriter* ot)
{
    return CharArrayWriter::WriteTo(ot);
}

ECode CCharArrayWriter::constructor()
{
    return CharArrayWriter::Init();
}

ECode CCharArrayWriter::constructor(
    /* [in] */ Int32 initialSize)
{
    return CharArrayWriter::Init(initialSize);
}

ECode CCharArrayWriter::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = CharArrayWriter::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
