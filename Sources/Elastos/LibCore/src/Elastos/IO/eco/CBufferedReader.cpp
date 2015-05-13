
#include "cmdef.h"
#include "CBufferedReader.h"

namespace Elastos {
namespace IO {

ECode CBufferedReader::Close()
{
    return BufferedReader::Close();
}

ECode CBufferedReader::Mark(
    /* [in] */ Int32 readLimit)
{
    return BufferedReader::Mark(readLimit);
}

ECode CBufferedReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return BufferedReader::IsMarkSupported(supported);
}

ECode CBufferedReader::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return BufferedReader::Read(value);
}

ECode CBufferedReader::ReadChars(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return BufferedReader::ReadChars(buffer, number);
}

ECode CBufferedReader::ReadCharsEx(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return BufferedReader::ReadCharsEx(buffer, offset, count, number);
}

ECode CBufferedReader::ReadLine(
    /* [out] */ String* contents)
{
    VALIDATE_NOT_NULL(contents);

    return BufferedReader::ReadLine(contents);
}

ECode CBufferedReader::IsReady(
    /* [out] */ Boolean* ready)
{
    VALIDATE_NOT_NULL(ready);

    return BufferedReader::IsReady(ready);
}

ECode CBufferedReader::Reset()
{
    return BufferedReader::Reset();
}

ECode CBufferedReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return BufferedReader::Skip(count, number);
}

ECode CBufferedReader::ReadCharBuffer(
    /* [in] */ ICharBuffer* target,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return BufferedReader::ReadCharBuffer(target, number);
}

ECode CBufferedReader::constructor(
    /* [in] */ IReader* rin)
{
    return BufferedReader::Init(rin);
}

ECode CBufferedReader::constructor(
    /* [in] */ IReader* rin,
    /* [in] */ Int32 size)
{
    return BufferedReader::Init(rin, size);
}

ECode CBufferedReader::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = BufferedReader::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
