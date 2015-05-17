
#include "cmdef.h"
#include "CLineNumberInputStream.h"

namespace Elastos {
namespace IO {

ECode CLineNumberInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return LineNumberInputStream::Available(number);
}

ECode CLineNumberInputStream::Close()
{
    return LineNumberInputStream::Close();
}

ECode CLineNumberInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return LineNumberInputStream::Mark(readLimit);
}

ECode CLineNumberInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return LineNumberInputStream::IsMarkSupported(supported);
}

ECode CLineNumberInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return LineNumberInputStream::Read(value);
}

ECode CLineNumberInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return LineNumberInputStream::ReadBytes(buffer, number);
}

ECode CLineNumberInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return LineNumberInputStream::ReadBytesEx(buffer, offset, length, number);
}

ECode CLineNumberInputStream::Reset()
{
    return LineNumberInputStream::Reset();
}

ECode CLineNumberInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return LineNumberInputStream::Skip(count, number);
}

ECode CLineNumberInputStream::GetLineNumber(
    /* [out] */ Int32* lineNumber)
{
    VALIDATE_NOT_NULL(lineNumber);

    return LineNumberInputStream::GetLineNumber(lineNumber);
}

ECode CLineNumberInputStream::SetLineNumber(
    /* [in] */ Int32 lineNumber)
{
    return LineNumberInputStream::SetLineNumber(lineNumber);
}

ECode CLineNumberInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return LineNumberInputStream::Init(is);
}

PInterface CLineNumberInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CLineNumberInputStream::Probe(riid);
}

ECode CLineNumberInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = LineNumberInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
