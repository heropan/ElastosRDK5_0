
#include "cmdef.h"
#include "CStringWriter.h"

namespace Elastos {
namespace IO {

ECode CStringWriter::Close()
{
    return StringWriter::Close();
}

ECode CStringWriter::Flush()
{
    return StringWriter::Flush();
}

ECode CStringWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    return StringWriter::Write(oneChar32);
}

ECode CStringWriter::WriteChars(
    /* [in] */ const ArrayOf<Char32> &buffer)
{
    return StringWriter::WriteChars(buffer);
}

ECode CStringWriter::WriteCharsEx(
    /* [in] */ const ArrayOf<Char32>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return StringWriter::WriteCharsEx(buffer, offset, count);
}

ECode CStringWriter::WriteString(
    /* [in] */ const String& str)
{
    return StringWriter::WriteString(str);
}

ECode CStringWriter::WriteStringEx(
    /* [in] */ const String& str,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return StringWriter::WriteStringEx(str, offset, count);
}

ECode CStringWriter::GetBuffer(
    /* [out] */ IStringBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    return StringWriter::GetBuffer(buf);
}

ECode CStringWriter::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    return StringWriter::ToString(str);
}

ECode CStringWriter::AppendChar(
    /* [in] */ Char32 c)
{
    return StringWriter::AppendChar(c);
}

ECode CStringWriter::AppendCharSequence(
    /* [in] */ ICharSequence* csq)
{
    VALIDATE_NOT_NULL(csq);

    return StringWriter::AppendCharSequence(csq);
}

ECode CStringWriter::AppendCharSequenceEx(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    VALIDATE_NOT_NULL(csq);

    return StringWriter::AppendCharSequenceEx(csq, start, end);
}

ECode CStringWriter::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return StringWriter::CheckError(hasError);
}

ECode CStringWriter::constructor()
{
    return StringWriter::Init();
}

ECode CStringWriter::constructor(
    /* [in] */ Int32 initialSize)
{
    return StringWriter::Init(initialSize);
}

ECode CStringWriter::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = StringWriter::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
