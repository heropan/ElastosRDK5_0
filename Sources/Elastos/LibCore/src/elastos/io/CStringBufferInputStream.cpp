
#include "coredef.h"
#include "CStringBufferInputStream.h"

namespace Elastos {
namespace IO {

ECode CStringBufferInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return StringBufferInputStream::Available(number);
}

ECode CStringBufferInputStream::Close()
{
    return StringBufferInputStream::Close();
}

ECode CStringBufferInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return StringBufferInputStream::Mark(readLimit);
}

ECode CStringBufferInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return StringBufferInputStream::IsMarkSupported(supported);
}

ECode CStringBufferInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return StringBufferInputStream::Read(value);
}

ECode CStringBufferInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return StringBufferInputStream::ReadBytes(buffer, number);
}

ECode CStringBufferInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return StringBufferInputStream::ReadBytes(buffer, offset, length, number);
}

ECode CStringBufferInputStream::Reset()
{
    return StringBufferInputStream::Reset();
}

ECode CStringBufferInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return StringBufferInputStream::Skip(count, number);
}

ECode CStringBufferInputStream::constructor(
    /* [in] */ const String& str)
{
    return StringBufferInputStream::Init(str);
}

PInterface CStringBufferInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CStringBufferInputStream::Probe(riid);
}

ECode CStringBufferInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = StringBufferInputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
