
#include "coredef.h"
#include "MappedByteBufferAdapter.h"
#include "elastos/core/StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

MappedByteBufferAdapter::MappedByteBufferAdapter(
    /* [in] */ MemoryBlock* block,
    /* [in] */ Int32 capa,
    /* [in] */ Int32 offset,
    /* [in] */ FileChannelMapMode mode)
    : MappedByteBuffer(block, capa, offset, mode)
{}

MappedByteBufferAdapter::MappedByteBufferAdapter(
    /* [in] */ IByteBuffer* buffer)
    : MappedByteBuffer(buffer)
{}

CAR_INTERFACE_IMPL(MappedByteBufferAdapter, Object, IMappedByteBuffer)

ECode MappedByteBufferAdapter::IsLoaded(
    /* [out] */ Boolean* isLoaded)
{
    return MappedByteBuffer::IsLoaded(isLoaded);
}

ECode MappedByteBufferAdapter::Load()
{
    return MappedByteBuffer::Load();
}

ECode MappedByteBufferAdapter::Force()
{
    return MappedByteBuffer::Force();
}

ECode MappedByteBufferAdapter::SetLimitImpl(
    /* [in] */ Int32 newLimit)
{
    FAIL_RETURN(Buffer::SetLimitImpl(newLimit))
    return MappedByteBuffer::mWrapped->SetLimit(newLimit);
}

ECode MappedByteBufferAdapter::SetPositionImpl(
    /* [in] */ Int32 newPosition)
{
    FAIL_RETURN(Buffer::SetPositionImpl(newPosition))
    return MappedByteBuffer::mWrapped->SetPosition(newPosition);
}

ECode MappedByteBufferAdapter::AsCharBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    return MappedByteBuffer::mWrapped->AsCharBuffer(buffer);
}

ECode MappedByteBufferAdapter::AsDoubleBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    return MappedByteBuffer::mWrapped->AsDoubleBuffer(buffer);
}

ECode MappedByteBufferAdapter::AsFloatBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    return MappedByteBuffer::mWrapped->AsFloatBuffer(buffer);
}

ECode MappedByteBufferAdapter::AsInt32Buffer(
    /* [out] */ IInt32Buffer** buffer)
{
    return MappedByteBuffer::mWrapped->AsInt32Buffer(buffer);
}

ECode MappedByteBufferAdapter::AsInt64Buffer(
    /* [out] */ IInt64Buffer** buffer)
{
    return MappedByteBuffer::mWrapped->AsInt64Buffer(buffer);
}

ECode MappedByteBufferAdapter::AsReadOnlyBuffer(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> byteBuf;
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->AsReadOnlyBuffer((IByteBuffer**)&byteBuf));
    AutoPtr<MappedByteBufferAdapter> result = new MappedByteBufferAdapter(byteBuf);
    result->mLimit = mLimit;
    result->mPosition = mPosition;
    result->mMark = mMark;
    *buffer = (IByteBuffer*)result->Probe(EIID_IByteBuffer);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode MappedByteBufferAdapter::AsInt16Buffer(
    /* [out] */ IInt16Buffer** buffer)
{
    return MappedByteBuffer::mWrapped->AsInt16Buffer(buffer);
}

ECode MappedByteBufferAdapter::Compact()
{
    Boolean isReadOnly = FALSE;
    if ((MappedByteBuffer::mWrapped->IsReadOnly(&isReadOnly), isReadOnly)) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    FAIL_RETURN(MappedByteBuffer::mWrapped->Compact())
    SetLimit(mCapacity);
    Int32 pos = 0;
    MappedByteBuffer::mWrapped->GetPosition(&pos);
    SetPosition(pos);
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode MappedByteBufferAdapter::Duplicate(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> byteBuf;
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->Duplicate((IByteBuffer**)&byteBuf))
    AutoPtr<MappedByteBufferAdapter> result = new MappedByteBufferAdapter(byteBuf);
    result->mLimit = mLimit;
    result->mPosition = mPosition;
    result->mMark = mMark;
    *buffer = (IByteBuffer*)result->Probe(EIID_IByteBuffer);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode MappedByteBufferAdapter::Get(
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value)
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->Get(value))
    ++mPosition;
    return NOERROR;
}

ECode MappedByteBufferAdapter::Get(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    return MappedByteBuffer::mWrapped->Get(index, value);
}

ECode MappedByteBufferAdapter::Get(
    /* [out] */ ArrayOf<Byte>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 byteCount)
{
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->Get(dst, dstOffset, byteCount))
    mPosition += byteCount;
    return NOERROR;
}

ECode MappedByteBufferAdapter::GetChar(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value)
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->GetChar(value))
    mPosition += sizeof(Char32); // SizeOf.CHAR;
    return NOERROR;
}

ECode MappedByteBufferAdapter::GetChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return MappedByteBuffer::mWrapped->GetChar(index, value);
}

ECode MappedByteBufferAdapter::GetDouble(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value)
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->GetDouble(value))
    mPosition += sizeof(Double); // SizeOf.DOUBLE;
    return NOERROR;
}

ECode MappedByteBufferAdapter::GetDouble(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return MappedByteBuffer::mWrapped->GetDouble(index, value);
}

ECode MappedByteBufferAdapter::GetFloat(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->GetFloat(value))
    mPosition += sizeof(Float); // SizeOf.FLOAT;
    return NOERROR;
}

ECode MappedByteBufferAdapter::GetFloat(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return MappedByteBuffer::mWrapped->GetFloat(index, value);
}

ECode MappedByteBufferAdapter::GetInt32(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->GetInt32(value))
    mPosition += sizeof(Int32); // SizeOf.INT;
    return NOERROR;
}

ECode MappedByteBufferAdapter::GetInt32(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return MappedByteBuffer::mWrapped->GetInt32(index, value);
}

ECode MappedByteBufferAdapter::GetInt64(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->GetInt64(value))
    mPosition += sizeof(Int64); // SizeOf.LONG;
    return NOERROR;
}

ECode MappedByteBufferAdapter::GetInt64(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return MappedByteBuffer::mWrapped->GetInt64(index, value);
}

ECode MappedByteBufferAdapter::GetInt16(
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value)
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->GetInt16(value))
    mPosition += sizeof(Int16); // SizeOf.SHORT;
    return NOERROR;
}

ECode MappedByteBufferAdapter::GetInt16(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return MappedByteBuffer::mWrapped->GetInt16(index, value);
}

ECode MappedByteBufferAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect)
    *isDirect = TRUE;
    return NOERROR;
}

ECode MappedByteBufferAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return MappedByteBuffer::mWrapped->IsReadOnly(isReadOnly);
}

ECode MappedByteBufferAdapter::SetOrderImpl(
    /* [in] */ ByteOrder byteOrder)
{
    ByteBuffer::SetOrderImpl(byteOrder);
    return MappedByteBuffer::mWrapped->SetOrder(byteOrder);
}

ECode MappedByteBufferAdapter::Put(
    /* [in] */ Byte b)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->Put(b))
    mPosition++;
    return NOERROR;
}

ECode MappedByteBufferAdapter::Put(
    /* [in] */ Int32 index,
    /* [in] */ Byte b)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    return MappedByteBuffer::mWrapped->Put(index, b);
}

ECode MappedByteBufferAdapter::Put(
    /* [in] */ ArrayOf<Byte>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 byteCount)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->Put(src, srcOffset, byteCount))
    mPosition += byteCount;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutChar(
    /* [in] */ Char32 value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->PutChar(value))
    mPosition += sizeof(Char32); // SizeOf.CHAR;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    return MappedByteBuffer::mWrapped->PutChar(index, value);
}

ECode MappedByteBufferAdapter::PutDouble(
    /* [in] */ Double value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->PutDouble(value))
    mPosition += sizeof(Double); // SizeOf.DOUBLE;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutDouble(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    return MappedByteBuffer::mWrapped->PutDouble(index, value);
}

ECode MappedByteBufferAdapter::PutFloat(
    /* [in] */ Float value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->PutFloat(value))
    mPosition += sizeof(Float); // SizeOf.FLOAT;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutFloat(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    return MappedByteBuffer::mWrapped->PutFloat(index, value);
}

ECode MappedByteBufferAdapter::PutInt32(
    /* [in] */ Int32 value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->PutInt32(value))
    mPosition += sizeof(Int32); // SizeOf.INT;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutInt32(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    return MappedByteBuffer::mWrapped->PutInt32(index, value);
}

ECode MappedByteBufferAdapter::PutInt64(
    /* [in] */ Int64 value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->PutInt64(value))
    mPosition += sizeof(Int64); // SizeOf.LONG;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutInt64(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    return MappedByteBuffer::mWrapped->PutInt64(index, value);
}

ECode MappedByteBufferAdapter::PutInt16(
    /* [in] */ Int16 value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->PutInt16(value))
    mPosition += sizeof(Int16); // SizeOf.SHORT;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutInt16(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    return MappedByteBuffer::mWrapped->PutInt16(index, value);
}

ECode MappedByteBufferAdapter::Slice(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    AutoPtr<IByteBuffer> byteBuf;
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->Slice((IByteBuffer**)&byteBuf))
    AutoPtr<MappedByteBufferAdapter> result = new MappedByteBufferAdapter(byteBuf);
    MappedByteBuffer::mWrapped->Clear();
    *buffer = (IByteBuffer*)result->Probe(EIID_IByteBuffer);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode MappedByteBufferAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return MappedByteBuffer::mWrapped->ProtectedArray(array);
}

ECode MappedByteBufferAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return MappedByteBuffer::mWrapped->ProtectedArrayOffset(offset);
}

ECode MappedByteBufferAdapter::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    return MappedByteBuffer::mWrapped->ProtectedHasArray(hasArray);
}

void MappedByteBufferAdapter::Free()
{
    MappedByteBuffer::mWrapped->Free();
}

ECode MappedByteBufferAdapter::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return ByteBuffer::GetArrayOffset(offset);
}

ECode MappedByteBufferAdapter::GetCapacity(
    /* [out] */ Int32* cap)
{
    return Buffer::GetCapacity(cap);
}

ECode MappedByteBufferAdapter::Clear()
{
    return Buffer::Clear();
}

ECode MappedByteBufferAdapter::Flip()
{
    return Buffer::Flip();
}

ECode MappedByteBufferAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return ByteBuffer::HasArray(hasArray);
}

ECode MappedByteBufferAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Buffer::HasRemaining(hasRemaining);
}

ECode MappedByteBufferAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return Buffer::GetLimit(limit);
}

ECode MappedByteBufferAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Buffer::SetLimit(newLimit);
}

ECode MappedByteBufferAdapter::Mark()
{
    return Buffer::Mark();
}

ECode MappedByteBufferAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return Buffer::GetPosition(pos);
}

ECode MappedByteBufferAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Buffer::SetPosition(newPosition);
}

ECode MappedByteBufferAdapter::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return Buffer::GetRemaining(remaining);
}

ECode MappedByteBufferAdapter::Reset()
{
    return Buffer::Reset();
}

ECode MappedByteBufferAdapter::Rewind()
{
    return Buffer::Rewind();
}

ECode MappedByteBufferAdapter::GetPrimitiveArray(
    /* [out] */ Handle32* arrayHandle)
{
    mArrayTemp = NULL;
    GetArray((ArrayOf<Byte>**)&mArrayTemp);
    if (mArrayTemp == NULL)
    {
        *arrayHandle = 0;
        return NOERROR;
    }
    Byte* primitiveArray = mArrayTemp->GetPayload();
    *arrayHandle = reinterpret_cast<Handle32>(primitiveArray);
    return NOERROR;
}

ECode MappedByteBufferAdapter::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "MappedByteBufferAdapter";
    buf += ", status: capacity=";
    buf += mCapacity;
    buf += " position=";
    buf += mPosition;
    buf += " limit=";
    buf += mLimit;
    return buf.ToString(str);
}

ECode MappedByteBufferAdapter::GetArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return ByteBuffer::GetArray(array);
}

ECode MappedByteBufferAdapter::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode MappedByteBufferAdapter::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode MappedByteBufferAdapter::CompareTo(
    /* [in] */ IByteBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return ByteBuffer::CompareTo(otherBuffer, result);
}

ECode MappedByteBufferAdapter::Get(
    /* [out] */ ArrayOf<Byte>* dst)
{
    return ByteBuffer::Get(dst);
}

ECode MappedByteBufferAdapter::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    return ByteBuffer::GetOrder(byteOrder);
}

ECode MappedByteBufferAdapter::SetOrder(
    /* [in] */ ByteOrder byteOrder)
{
    return ByteBuffer::SetOrder(byteOrder);
}

ECode MappedByteBufferAdapter::Put(
    /* [in] */ ArrayOf<Byte>* src)
{
    return ByteBuffer::Put(src);
}

ECode MappedByteBufferAdapter::Put(
    /* [in] */ IByteBuffer* src)
{
    return ByteBuffer::Put(src);
}

} // namespace IO
} // namespace Elastos
