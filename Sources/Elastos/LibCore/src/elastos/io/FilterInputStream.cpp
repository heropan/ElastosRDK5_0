
#include "FilterInputStream.h"

namespace Elastos {
namespace IO {

extern "C" const InterfaceID EIID_FilterInputStream =
    { 0x71a922d3, 0x24c8, 0x4f4f, { 0xac, 0x4e, 0x61, 0x22, 0x9c, 0xb8, 0xdb, 0xb7 } };

CAR_INTERFACE_IMPL(FilterInputStream, IFilterInputStream)

FilterInputStream::FilterInputStream()
{
}

FilterInputStream::~FilterInputStream()
{
}

ECode FilterInputStream::constructor(
    /* [in] */ IInputStream* in)
{
    mIn = in;
    return NOERROR;
}

ECode FilterInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return mIn->Available(number);
}

ECode FilterInputStream::Close()
{
    return mIn->Close();
}

ECode FilterInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    Object::Autolock lock(mLock);
    return mIn->Mark(readLimit);
}

ECode FilterInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    return mIn->IsMarkSupported(supported);
}

ECode FilterInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return mIn->Read(value);
}

ECode FilterInputStream::Read(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);
    return mIn->Read(buffer, byteOffset, byteCount, number);
}

ECode FilterInputStream::Reset()
{
    Object::Autolock lock(mLock);
    return mIn->Reset();
}

ECode FilterInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(value);
    return mIn->Skip(byteCount, number);
}

ECode FilterInputStream::ToString(
        /* [out] */ String* s)
{
    assert(s != NULL);
    *s = String("FilterInputStream");
    return NOERROR;
}

ECode FilterInputStream::GetClassID(
        /* [out] */ ClassID *pCLSID)
{
    assert(pCLSID != NULL);
    *pCLSID = EIID_FilterInputStream;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
