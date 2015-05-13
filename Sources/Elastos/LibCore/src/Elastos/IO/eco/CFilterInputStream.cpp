
#include "cmdef.h"
#include "CFilterInputStream.h"

namespace Elastos {
namespace IO {

CFilterInputStream::CFilterInputStream()
{
}

CFilterInputStream::~CFilterInputStream()
{
}

ECode CFilterInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return FilterInputStream::Available(number);
}

ECode CFilterInputStream::Close()
{
    return FilterInputStream::Close();
}

ECode CFilterInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    Object::Autolock lock(mLock);

    return FilterInputStream::Mark(readLimit);
}

ECode CFilterInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return FilterInputStream::IsMarkSupported(supported);
}

ECode CFilterInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return FilterInputStream::Read(value);
}

ECode CFilterInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return FilterInputStream::ReadBytes(buffer, number);
}

ECode CFilterInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return FilterInputStream::ReadBytesEx(buffer, offset, length, number);
}

ECode CFilterInputStream::Reset()
{
    Object::Autolock lock(mLock);

    return FilterInputStream::Reset();
}

ECode CFilterInputStream::Skip(
    /* [in] */ Int64 length,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    return FilterInputStream::Skip(length, value);
}

ECode CFilterInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return FilterInputStream::Init(is);
}

PInterface CFilterInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CFilterInputStream::Probe(riid);
}

ECode CFilterInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = FilterInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
