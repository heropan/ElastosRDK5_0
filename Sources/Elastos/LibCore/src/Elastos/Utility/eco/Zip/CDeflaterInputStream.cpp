
#include "cmdef.h"
#include "CDeflaterInputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {


PInterface CDeflaterInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CDeflaterInputStream::Probe(riid);
}

ECode CDeflaterInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = DeflaterInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

ECode CDeflaterInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return DeflaterInputStream::Available(number);
}

ECode CDeflaterInputStream::Close()
{
    return DeflaterInputStream::Close();
}

ECode CDeflaterInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return DeflaterInputStream::Mark(readLimit);
}

ECode CDeflaterInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    return DeflaterInputStream::IsMarkSupported(supported);
}

ECode CDeflaterInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return DeflaterInputStream::Read(value);
}

ECode CDeflaterInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return DeflaterInputStream::ReadBytes(buffer, number);
}

ECode CDeflaterInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return DeflaterInputStream::ReadBytesEx(buffer, offset, length, number);
}

ECode CDeflaterInputStream::Reset()
{
    return DeflaterInputStream::Reset();
}

ECode CDeflaterInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    return DeflaterInputStream::Skip(count, number);
}

ECode CDeflaterInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return DeflaterInputStream::Init(is);
}

ECode CDeflaterInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IDeflater* deflater)
{
    return DeflaterInputStream::Init(is, deflater);
}

ECode CDeflaterInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IDeflater* deflater,
    /* [in] */ Int32 bufferSize)
{
    return DeflaterInputStream::Init(is, deflater, bufferSize);
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
