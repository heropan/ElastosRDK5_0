
#include "cmdef.h"
#include "CStringReader.h"

namespace Elastos {
namespace IO {

ECode CStringReader::Close()
{
    return StringReader::Close();
}

ECode CStringReader::Mark(
    /* [in] */ Int32 readLimit)
{
    return StringReader::Mark(readLimit);
}

ECode CStringReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return StringReader::IsMarkSupported(supported);
}

ECode CStringReader::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return StringReader::Read(value);
}

ECode CStringReader::ReadChars(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return StringReader::ReadChars(buffer, number);
}

ECode CStringReader::ReadCharsEx(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return StringReader::ReadCharsEx(buffer, offset, count, number);
}

ECode CStringReader::IsReady(
    /* [out] */ Boolean* ready)
{
    VALIDATE_NOT_NULL(ready);

    return StringReader::IsReady(ready);
}

ECode CStringReader::Reset()
{
    return StringReader::Reset();
}

ECode CStringReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return StringReader::Skip(count, number);
}

ECode CStringReader::ReadCharBuffer(
    /* [in] */ ICharBuffer* target,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return StringReader::ReadCharBuffer(target, number);
}

ECode CStringReader::constructor(
    /* [in] */ const String& str)
{
    return StringReader::Init(str);
}

ECode CStringReader::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = StringReader::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
