#include "CByteArrayBuffer.h"

namespace Elastos {
namespace IO {


CAR_OBJECT_IMPL(CByteArrayBuffer)

CByteArrayBuffer::CByteArrayBuffer()
    : mArrayOffset(0)
    , mIsReadOnly(0)
{}

ECode CByteArrayBuffer::constructor(
    /* [in] */ ArrayOf<Byte>* backingArray)
{
    return NOERROR;
}

ECode CByteArrayBuffer::constructor(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Byte>* backingArray,
    /* [in] */ Int32 arrayOffset,
    /* [in] */ Boolean isReadOnly)
{
    return NOERROR;
}

AutoPtr<IByteBuffer> Copy(
    /* [in] */ IByteBuffer* other,
    /* [in] */ Int32 markOfOther,
    /* [in] */ Boolean isReadOnly)
{
    return NULL;
}

ECode CByteArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IByteBuffer** buffer)
{
    return NOERROR;
}


ECode CByteArrayBuffer::Compact()
{
    return NOERROR;
}

ECode CByteArrayBuffer::Duplicate(
    /* [out] */ IByteBuffer** buffer)
{
    return NOERROR;
}

ECode CByteArrayBuffer::Slice(
    /* [out] */ IByteBuffer** buffer)
{
    return NOERROR;
}

ECode CByteArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return NOERROR;
}

ECode CByteArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return NOERROR;
}

ECode CByteArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return NOERROR;
}

ECode CByteArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    return NOERROR;
}

ECode CByteArrayBuffer::Get(
    /* [out] */ Byte* value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::Get(
    /* [in] */ ArrayOf<Byte>* dst)
{
    return NOERROR;
}

ECode CByteArrayBuffer::Get(
    /* [in] */ ArrayOf<Byte>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 byteCount)
{
    return NOERROR;
}

ECode CByteArrayBuffer::GetChar(
    /* [out] */ Char32* value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::GetChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::GetDouble(
    /* [out] */ Double* value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::GetDouble(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::GetFloat(
    /* [out] */ Float* value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::GetFloat(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::GetInt32(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::GetInt64(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::GetInt16(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return NOERROR;
}

ECode CByteArrayBuffer::Put(
    /* [in] */ Byte b)
{
    return NOERROR;
}

ECode CByteArrayBuffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Byte b)
{
    return NOERROR;
}

ECode CByteArrayBuffer::Put(
    /* [in] */ ArrayOf<Byte>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 byteCount)
{
    return NOERROR;
}

ECode CByteArrayBuffer::PutChar(
    /* [in] */ Char32 value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::PutChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::PutDouble(
    /* [in] */ Double value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::PutDouble(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::PutFloat(
    /* [in] */ Float value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::PutFloat(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::PutInt16(
    /* [in] */ Int16 value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::PutInt16(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::PutInt32(
    /* [in] */ Int32 value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::PutInt32(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::PutInt64(
    /* [in] */ Int64 value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::PutInt64(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    return NOERROR;
}

ECode CByteArrayBuffer::AsCharBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    return NOERROR;
}

ECode CByteArrayBuffer::AsDoubleBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    return NOERROR;
}

ECode CByteArrayBuffer::AsFloatBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    return NOERROR;
}

ECode CByteArrayBuffer::AsInt16Buffer(
    /* [out] */ IInt16Buffer** buffer)
{
    return NOERROR;
}

ECode CByteArrayBuffer::AsInt32Buffer(
    /* [out] */ IInt32Buffer** buffer)
{
    return NOERROR;
}

ECode CByteArrayBuffer::AsInt64Buffer(
    /* [out] */ IInt64Buffer** buffer)
{
    return NOERROR;
}

} // namespace IO
} // namespace Elastos