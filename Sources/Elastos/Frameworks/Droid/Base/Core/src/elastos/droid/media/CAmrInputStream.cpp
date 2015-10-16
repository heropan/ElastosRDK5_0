#include "elastos/droid/media/CAmrInputStream.h"

namespace Elastos {
namespace Droid {
namespace Media {

ECode CAmrInputStream::constructor(
    /* [in] */ IInputStream* inputStream)
{
    return AmrInputStream::Init(inputStream);
}

PInterface CAmrInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CAmrInputStream::Probe(riid);
}

ECode CAmrInputStream::Available(
    /* [out] */ Int32* number)
{
    return AmrInputStream::Available(number);
}

ECode CAmrInputStream::Close()
{
    return AmrInputStream::Close();
}

ECode CAmrInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return AmrInputStream::Mark(readLimit);
}

ECode CAmrInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return AmrInputStream::IsMarkSupported(supported);
}

ECode CAmrInputStream::Read(
    /* [out] */ Int32* value)
{
    return AmrInputStream::Read(value);
}

ECode CAmrInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return AmrInputStream::ReadBytes(buffer, number);
}

ECode CAmrInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    return AmrInputStream::ReadBytes(buffer, byteOffset, byteCount, number);
}

ECode CAmrInputStream::Reset()
{
    return AmrInputStream::Reset();
}

ECode CAmrInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    return AmrInputStream::Skip(byteCount, number);
}

ECode CAmrInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = AmrInputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}


} // namespace Media
} // namepsace Droid
} // namespace Elastos
