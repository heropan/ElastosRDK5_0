
#include "speech/srec/CUlawEncoderInputStream.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

ECode CUlawEncoderInputStream::Close()
{
    return UlawEncoderInputStream::Close();
}

ECode CUlawEncoderInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return UlawEncoderInputStream::Available(number);
}

ECode CUlawEncoderInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    VALIDATE_NOT_NULL(readLimit);
    return UlawEncoderInputStream::Mark(readLimit);
}

ECode CUlawEncoderInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    return UlawEncoderInputStream::IsMarkSupported(supported);
}

ECode CUlawEncoderInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return UlawEncoderInputStream::Read(value);
}

ECode CUlawEncoderInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);
    return UlawEncoderInputStream::ReadBytes(buffer, number);
}

ECode CUlawEncoderInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);
    return UlawEncoderInputStream::ReadBytes(buffer, offset, length, number);
}

ECode CUlawEncoderInputStream::Reset()
{
    return UlawEncoderInputStream::Reset();
}

ECode CUlawEncoderInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    return UlawEncoderInputStream::Skip(byteCount, number);
}

PInterface CUlawEncoderInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CUlawEncoderInputStream::Probe(riid);
}

ECode CUlawEncoderInputStream::constructor(
    /* [in] */ IInputStream* in,
    /* [in] */ Int32 max)
{
    Init(in, max);
    return NOERROR;
}

ECode CUlawEncoderInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = UlawEncoderInputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

}//namespace Srec
}//namespace Speech
}//namespace Droid
}//namespace Elastos

