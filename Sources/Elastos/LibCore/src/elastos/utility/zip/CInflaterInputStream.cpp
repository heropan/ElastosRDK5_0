
#include "CInflaterInputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {


PInterface CInflaterInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CInflaterInputStream::Probe(riid);
}

ECode CInflaterInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = InflaterInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}


ECode CInflaterInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return InflaterInputStream::Available(number);
}

ECode CInflaterInputStream::Close()
{
    return InflaterInputStream::Close();
}

ECode CInflaterInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return InflaterInputStream::Mark(readLimit);
}

ECode CInflaterInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    return InflaterInputStream::IsMarkSupported(supported);
}

ECode CInflaterInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return InflaterInputStream::Read(value);
}

ECode CInflaterInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return InflaterInputStream::ReadBytes(buffer, number);
}

ECode CInflaterInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return InflaterInputStream::ReadBytes(buffer, offset, length, number);
}

ECode CInflaterInputStream::Reset()
{
    return InflaterInputStream::Reset();
}

ECode CInflaterInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    return InflaterInputStream::Skip(count, number);
}

ECode CInflaterInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return InflaterInputStream::Init(is);
}

ECode CInflaterInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IInflater* inflater)
{
    return InflaterInputStream::Init(is, inflater);
}

ECode CInflaterInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IInflater* inflater,
    /* [in] */ Int32 bsize)
{
    return InflaterInputStream::Init(is, inflater, bsize);
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
