
#include "cmdef.h"
#include "CBufferedWriter.h"

namespace Elastos {
namespace IO {

ECode CBufferedWriter::Close()
{
    return BufferedWriter::Close();
}

ECode CBufferedWriter::Flush()
{
    return BufferedWriter::Flush();
}

ECode CBufferedWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    return BufferedWriter::Write(oneChar32);
}

ECode CBufferedWriter::WriteChars(
    /* [in] */ const ArrayOf<Char32>& buffer)
{
    return BufferedWriter::WriteChars(buffer);
}

ECode CBufferedWriter::WriteCharsEx(
    /* [in] */ const ArrayOf<Char32>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return BufferedWriter::WriteCharsEx(buffer, offset, count);
}

ECode CBufferedWriter::WriteString(
    /* [in] */ const String& str)
{
    return BufferedWriter::WriteString(str);
}

ECode CBufferedWriter::WriteStringEx(
    /* [in] */ const String& str,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return BufferedWriter::WriteStringEx(str, offset, count);
}

ECode CBufferedWriter::AppendChar(
    /* [in] */ Char32 c)
{
    return BufferedWriter::AppendChar(c);
}

ECode CBufferedWriter::AppendCharSequence(
    /* [in] */ ICharSequence* csq)
{
    return BufferedWriter::AppendCharSequence(csq);
}

ECode CBufferedWriter::AppendCharSequenceEx(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return BufferedWriter::AppendCharSequenceEx(csq, start, end);
}

ECode CBufferedWriter::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return BufferedWriter::CheckError(hasError);
}

ECode CBufferedWriter::NewLine()
{
    return BufferedWriter::NewLine();
}

ECode CBufferedWriter::constructor(
    /* [in] */ IWriter* wout)
{
    return BufferedWriter::Init(wout);
}

ECode CBufferedWriter::constructor(
    /* [in] */ IWriter* wout,
    /* [in] */ Int32 size)
{
    return BufferedWriter::Init(wout, size);
}

ECode CBufferedWriter::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = BufferedWriter::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
