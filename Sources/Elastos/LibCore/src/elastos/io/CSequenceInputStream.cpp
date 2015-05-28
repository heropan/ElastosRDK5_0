
#include "coredef.h"
#include "CSequenceInputStream.h"

namespace Elastos {
namespace IO {

ECode CSequenceInputStream::constructor(
    /* [in] */ IInputStream* s1,
    /* [in] */ IInputStream* s2)
{
    return Init(s1, s2);
}

ECode CSequenceInputStream::constructor(
    /* [in] */ IObjectEnumerator* e)
{
    return Init(e);
}

ECode CSequenceInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return SequenceInputStream::Available(number);
}

ECode CSequenceInputStream::Close()
{
    return SequenceInputStream::Close();
}

ECode CSequenceInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return SequenceInputStream::Read(value);
}

ECode CSequenceInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    VALIDATE_NOT_NULL(buffer);
    return SequenceInputStream::ReadBytes(buffer, offset, length, number);
}

ECode CSequenceInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return SequenceInputStream::Mark(readLimit);
}

ECode CSequenceInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);
    return SequenceInputStream::IsMarkSupported(supported);
}

ECode CSequenceInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    assert(buffer != NULL);
    assert(number != NULL);
    return ReadBytes(buffer, 0, buffer->GetLength(), number);
}

ECode CSequenceInputStream::Reset()
{
    return SequenceInputStream::Reset();
}

ECode CSequenceInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);
    return SequenceInputStream::Skip(count, number);
}

PInterface CSequenceInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CSequenceInputStream::Probe(riid);
}

ECode CSequenceInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = SequenceInputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
