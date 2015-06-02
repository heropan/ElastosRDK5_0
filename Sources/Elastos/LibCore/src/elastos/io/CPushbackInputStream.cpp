
#include "coredef.h"
#include "CPushbackInputStream.h"

namespace Elastos {
namespace IO {

ECode CPushbackInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return PushbackInputStream::Available(number);
}

ECode CPushbackInputStream::Close()
{
    return PushbackInputStream::Close();
}

ECode CPushbackInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return PushbackInputStream::Mark(readLimit);
}

ECode CPushbackInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return PushbackInputStream::IsMarkSupported(supported);
}

ECode CPushbackInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return PushbackInputStream::Read(value);
}

ECode CPushbackInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return InputStream::Read(buffer, number);
}

ECode CPushbackInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return PushbackInputStream::ReadBytes(buffer, offset, length, number);
}

ECode CPushbackInputStream::Reset()
{
    return PushbackInputStream::Reset();
}

ECode CPushbackInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return PushbackInputStream::Skip(count, number);
}

ECode CPushbackInputStream::Unread(
    /* [in] */ Int32 oneByte)
{
    return PushbackInputStream::Unread(oneByte);
}

ECode CPushbackInputStream::UnreadBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return PushbackInputStream::UnreadBytes(buffer);
}

ECode CPushbackInputStream::UnreadBytes(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return PushbackInputStream::UnreadBytes(buffer, offset, length);
}

ECode CPushbackInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return PushbackInputStream::Init(is);
}

ECode CPushbackInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ Int32 size)
{
    return PushbackInputStream::Init(is, size);
}

} // namespace IO
} // namespace Elastos
