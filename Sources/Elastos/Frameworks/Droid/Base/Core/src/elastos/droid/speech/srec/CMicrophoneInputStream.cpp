
#include "elastos/droid/speech/srec/CMicrophoneInputStream.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

ECode CMicrophoneInputStream::Close()
{
    return MicrophoneInputStream::Close();
}

ECode CMicrophoneInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return MicrophoneInputStream::Available(number);
}

ECode CMicrophoneInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    VALIDATE_NOT_NULL(readLimit);
    return MicrophoneInputStream::Mark(readLimit);
}

ECode CMicrophoneInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    return MicrophoneInputStream::IsMarkSupported(supported);
}

ECode CMicrophoneInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return MicrophoneInputStream::Read(value);
}

ECode CMicrophoneInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);
    return MicrophoneInputStream::ReadBytes(buffer, number);
}

ECode CMicrophoneInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);
    return MicrophoneInputStream::ReadBytes(buffer, offset, length, number);
}

ECode CMicrophoneInputStream::Reset()
{
    return MicrophoneInputStream::Reset();
}

ECode CMicrophoneInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    return MicrophoneInputStream::Skip(byteCount, number);
}

PInterface CMicrophoneInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CMicrophoneInputStream::Probe(riid);
}

ECode CMicrophoneInputStream::constructor(
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 fifoDepth)
{
    return Init(sampleRate, fifoDepth);
}

ECode CMicrophoneInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = MicrophoneInputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

}//namespace Srec
}//namespace Speech
}//namespace Droid
}//namespace Elastos
