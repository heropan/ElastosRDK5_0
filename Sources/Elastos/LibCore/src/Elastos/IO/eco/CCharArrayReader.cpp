
#include "cmdef.h"
#include "CCharArrayReader.h"

namespace Elastos {
namespace IO {

ECode CCharArrayReader::Close()
{
    return CharArrayReader::Close();
}

ECode CCharArrayReader::Mark(
    /* [in] */ Int32 readLimit)
{
    return CharArrayReader::Mark(readLimit);
}

ECode CCharArrayReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return CharArrayReader::IsMarkSupported(supported);
}

ECode CCharArrayReader::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return CharArrayReader::Read(value);
}

ECode CCharArrayReader::ReadChars(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return CharArrayReader::ReadChars(buffer, number);
}

ECode CCharArrayReader::ReadCharsEx(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return CharArrayReader::ReadCharsEx(buffer, offset, length, number);
}

ECode CCharArrayReader::IsReady(
    /* [out] */ Boolean* ready)
{
    VALIDATE_NOT_NULL(ready);

    return CharArrayReader::IsReady(ready);
}

ECode CCharArrayReader::Reset()
{
    return CharArrayReader::Reset();
}

ECode CCharArrayReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return CharArrayReader::Skip(count, number);
}

ECode CCharArrayReader::ReadCharBuffer(
    /* [in] */ ICharBuffer* target,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return CharArrayReader::ReadCharBuffer(target, number);
}

ECode CCharArrayReader::constructor(
    /* [in] */ ArrayOf<Char32>* buf)
{
    return CharArrayReader::Init(buf);
}

ECode CCharArrayReader::constructor(
    /* [in] */ ArrayOf<Char32>* buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return CharArrayReader::Init(buf, offset, length);
}

ECode CCharArrayReader::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = CharArrayReader::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
