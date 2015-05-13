
#include "cmdef.h"
#include "COutputStreamWriter.h"

namespace Elastos {
namespace IO {

ECode COutputStreamWriter::Close()
{
    return OutputStreamWriter::Close();
}

ECode COutputStreamWriter::Flush()
{
    return OutputStreamWriter::Flush();
}

ECode COutputStreamWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    return OutputStreamWriter::Write(oneChar32);
}

ECode COutputStreamWriter::WriteChars(
    /* [in] */ const ArrayOf<Char32> &buffer)
{
    return OutputStreamWriter::WriteChars(buffer);
}

ECode COutputStreamWriter::WriteCharsEx(
    /* [in] */ const ArrayOf<Char32>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return OutputStreamWriter::WriteCharsEx(buffer, offset, count);
}

ECode COutputStreamWriter::WriteString(
    /* [in] */ const String& str)
{
    return OutputStreamWriter::WriteString(str);
}

ECode COutputStreamWriter::WriteStringEx(
    /* [in] */ const String& str,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return OutputStreamWriter::WriteStringEx(str, offset, count);
}

ECode COutputStreamWriter::AppendChar(
    /* [in] */ Char32 c)
{
    return OutputStreamWriter::AppendChar(c);
}

ECode COutputStreamWriter::AppendCharSequence(
    /* [in] */ ICharSequence *csq)
{
    VALIDATE_NOT_NULL(csq);

    return OutputStreamWriter::AppendCharSequence(csq);
}

ECode COutputStreamWriter::AppendCharSequenceEx(
    /* [in] */ ICharSequence *csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    VALIDATE_NOT_NULL(csq);

    return OutputStreamWriter::AppendCharSequenceEx(csq, start, end);
}

ECode COutputStreamWriter::CheckError(
    /* [out] */ Boolean *hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return OutputStreamWriter::CheckError(hasError);
}

ECode COutputStreamWriter::GetEncoding(
    /* [out] */ String *encoding)
{
    VALIDATE_NOT_NULL(encoding);

    return OutputStreamWriter::GetEncoding(encoding);
}

ECode COutputStreamWriter::constructor(
    /* [in] */ IOutputStream *os)
{
    return OutputStreamWriter::Init(os);
}

ECode COutputStreamWriter::constructor(
    /* [in] */ IOutputStream *os,
    /* [in] */ const String &enc)
{
    return OutputStreamWriter::Init(os, enc);
}

ECode COutputStreamWriter::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = OutputStreamWriter::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
