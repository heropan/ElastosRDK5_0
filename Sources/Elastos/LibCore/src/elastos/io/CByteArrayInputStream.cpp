
#include "cmdef.h"
#include "CByteArrayInputStream.h"

namespace Elastos {
namespace IO {

ECode CByteArrayInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Object::Autolock lock(mLock);

    return ByteArrayInputStream::Available(number);
}

ECode CByteArrayInputStream::Close()
{
    return ByteArrayInputStream::Close();
}

ECode CByteArrayInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    Object::Autolock lock(mLock);

    return ByteArrayInputStream::Mark(readLimit);
}

ECode CByteArrayInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return ByteArrayInputStream::IsMarkSupported(supported);
}

ECode CByteArrayInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    Object::Autolock lock(mLock);

    return ByteArrayInputStream::Read(value);
}

ECode CByteArrayInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    Object::Autolock lock(mLock);

    return ByteArrayInputStream::ReadBytes(buffer, number);
}

ECode CByteArrayInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    Object::Autolock lock(mLock);

    return ByteArrayInputStream::ReadBytesEx(buffer, offset, length, number);
}

ECode CByteArrayInputStream::Reset()
{
    Object::Autolock lock(mLock);

    return ByteArrayInputStream::Reset();
}

ECode CByteArrayInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    Object::Autolock lock(mLock);

    return ByteArrayInputStream::Skip(count, number);
}

ECode CByteArrayInputStream::constructor(
    /* [in] */ ArrayOf<Byte>* buffer)
{
    return ByteArrayInputStream::Init(buffer);
}

ECode CByteArrayInputStream::constructor(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return ByteArrayInputStream::Init(buffer, offset, length);
}

PInterface CByteArrayInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CByteArrayInputStream::Probe(riid);
}

ECode CByteArrayInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = ByteArrayInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
