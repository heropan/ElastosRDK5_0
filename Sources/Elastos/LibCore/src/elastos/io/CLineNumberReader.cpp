
#include "cmdef.h"
#include "CLineNumberReader.h"

namespace Elastos {
namespace IO {

ECode CLineNumberReader::Close()
{
    return LineNumberReader::Close();
}

ECode CLineNumberReader::Mark(
    /* [in] */ Int32 readLimit)
{
    return LineNumberReader::Mark(readLimit);
}

ECode CLineNumberReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return LineNumberReader::IsMarkSupported(supported);
}

ECode CLineNumberReader::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return LineNumberReader::Read(value);
}

ECode CLineNumberReader::ReadChars(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return LineNumberReader::ReadChars(buffer, number);
}

ECode CLineNumberReader::ReadCharsEx(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return LineNumberReader::ReadCharsEx(buffer, offset, count, number);
}

ECode CLineNumberReader::IsReady(
    /* [out] */ Boolean* ready)
{
    VALIDATE_NOT_NULL(ready);

    return LineNumberReader::IsReady(ready);
}

ECode CLineNumberReader::Reset()
{
    return LineNumberReader::Reset();
}

ECode CLineNumberReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return LineNumberReader::Skip(count, number);
}

ECode CLineNumberReader::ReadCharBuffer(
    /* [in] */ ICharBuffer* target,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return LineNumberReader::ReadCharBuffer(target, number);
}

ECode CLineNumberReader::ReadLine(
    /* [out] */ String * contents)
{
    VALIDATE_NOT_NULL(contents);

    return LineNumberReader::ReadLine(contents);
}

ECode CLineNumberReader::GetLineNumber(
    /* [out] */ Int32* lineNumber)
{
    VALIDATE_NOT_NULL(lineNumber);

    return LineNumberReader::GetLineNumber(lineNumber);
}

ECode CLineNumberReader::SetLineNumber(
    /* [in] */ Int32 lineNumber)
{
    return LineNumberReader::SetLineNumber(lineNumber);
}

ECode CLineNumberReader::constructor(
    /* [in] */ IReader* ir)
{
    return LineNumberReader::Init(ir);
}

ECode CLineNumberReader::constructor(
    /* [in] */ IReader* ir,
    /* [in] */ Int32 size)
{
    return LineNumberReader::Init(ir, size);
}

ECode CLineNumberReader::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = LineNumberReader::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
