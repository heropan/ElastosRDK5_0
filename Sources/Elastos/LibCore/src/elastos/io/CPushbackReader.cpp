
#include "coredef.h"
#include "CPushbackReader.h"

namespace Elastos {
namespace IO {

ECode CPushbackReader::Close()
{
    return PushbackReader::Close();
}

ECode CPushbackReader::Mark(
    /* [in] */ Int32 readLimit)
{
    return PushbackReader::Mark(readLimit);
}

ECode CPushbackReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return PushbackReader::IsMarkSupported(supported);
}

ECode CPushbackReader::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return PushbackReader::Read(value);
}

ECode CPushbackReader::ReadChars(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    VALIDATE_NOT_NULL(number);

    return PushbackReader::ReadChars(buffer, number);
}

ECode CPushbackReader::ReadChars(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    VALIDATE_NOT_NULL(number);

    return PushbackReader::ReadChars(buffer, offset, count, number);
}

ECode CPushbackReader::IsReady(
    /* [out] */ Boolean* ready)
{
    VALIDATE_NOT_NULL(ready);

    return PushbackReader::IsReady(ready);
}

ECode CPushbackReader::Reset()
{
    return PushbackReader::Reset();
}

ECode CPushbackReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return PushbackReader::Skip(count, number);
}

ECode CPushbackReader::ReadCharBuffer(
    /* [in] */ ICharBuffer* target,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return PushbackReader::ReadCharBuffer(target, number);
}

ECode CPushbackReader::Unread(
    /* [in] */ Int32 oneChar)
{
    return PushbackReader::Unread(oneChar);
}

ECode CPushbackReader::UnreadChars(
    /* [in] */ const ArrayOf<Char32>& buffer)
{
    return PushbackReader::UnreadChars(buffer);
}

ECode CPushbackReader::UnreadChars(
    /* [in] */ const ArrayOf<Char32>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return PushbackReader::UnreadChars(buffer, offset, length);
}

ECode CPushbackReader::constructor(
    /* [in] */ IReader* in)
{
    return PushbackReader::Init(in);
}

ECode CPushbackReader::constructor(
    /* [in] */ IReader* in,
    /* [in] */ Int32 size)
{
    return PushbackReader::Init(in, size);
}

ECode CPushbackReader::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = PushbackReader::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
