
#include "FilterInputStream.h"

namespace Elastos {
namespace IO {

FilterInputStream::FilterInputStream()
{
}

FilterInputStream::~FilterInputStream()
{
}

ECode FilterInputStream::Init(
    /* [in] */ IInputStream* in)
{
    mIn = in;
    return NOERROR;
}

ECode FilterInputStream::Available(
    /* [out] */ Int32* number)
{
    return mIn->Available(number);
}

ECode FilterInputStream::Close()
{
    return mIn->Close();
}

ECode FilterInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return mIn->Mark(readLimit);
}

ECode FilterInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return mIn->IsMarkSupported(supported);
}

ECode FilterInputStream::Read(
    /* [out] */ Int32* value)
{
    return mIn->Read(value);
}

ECode FilterInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    return mIn->ReadBytesEx(buffer, offset, length, number);
}

ECode FilterInputStream::Reset()
{
    return mIn->Reset();
}

ECode FilterInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    return mIn->Skip(byteCount, number);
}

} // namespace IO
} // namespace Elastos
