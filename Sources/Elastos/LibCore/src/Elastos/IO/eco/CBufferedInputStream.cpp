
#include "cmdef.h"
#include "CBufferedInputStream.h"

namespace Elastos {
namespace IO {

ECode CBufferedInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Object::Autolock lock(mLock);

    return BufferedInputStream::Available(number);
}

ECode CBufferedInputStream::Close()
{
    return BufferedInputStream::Close();
}

ECode CBufferedInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    Object::Autolock lock(mLock);

    return BufferedInputStream::Mark(readLimit);
}

ECode CBufferedInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return BufferedInputStream::IsMarkSupported(supported);
}

ECode CBufferedInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    Object::Autolock lock(mLock);

    return BufferedInputStream::Read(value);
}

ECode CBufferedInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return BufferedInputStream::ReadBytes(buffer, number);
}

ECode CBufferedInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Object::Autolock lock(mLock);

    return BufferedInputStream::ReadBytesEx(buffer, offset, length, number);
}

ECode CBufferedInputStream::Reset()
{
    Object::Autolock lock(mLock);

    return BufferedInputStream::Reset();
}

ECode CBufferedInputStream::Skip(
    /* [in] */ Int64 length,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    Object::Autolock lock(mLock);

    return BufferedInputStream::Skip(length, number);
}

ECode CBufferedInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return BufferedInputStream::Init(is, 8192);
}

ECode CBufferedInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ Int32 size)
{
    return BufferedInputStream::Init(is, size);
}

PInterface CBufferedInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CBufferedInputStream::Probe(riid);
}

ECode CBufferedInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = BufferedInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
