
#include "cmdef.h"
#include "MappedByteBufferAdapter.h"
#include "elastos/StringBuilder.h"

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

PInterface MappedByteBufferAdapter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_IByteBuffer) {
        return (IByteBuffer*)this;
    }
    else if (riid == EIID_MappedByteBuffer) {
        return reinterpret_cast<PInterface>((MappedByteBuffer*)this);
    }
    else if (riid == EIID_IMappedByteBuffer) {
        return (IMappedByteBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_ByteBuffer) {
        return reinterpret_cast<PInterface>((ByteBuffer*)this);
    }

    return NULL;
}


UInt32 MappedByteBufferAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MappedByteBufferAdapter::Release()
{
    return ElRefBase::Release();
}

ECode MappedByteBufferAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(IMappedByteBuffer *)this) {
        *pIID = EIID_IMappedByteBuffer ;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

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
    INTERFACE_ADDREF(*buffer)
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
    INTERFACE_ADDREF(*buffer)
    return NOERROR;
}

ECode MappedByteBufferAdapter::GetByte(
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value)
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->GetByte(value))
    ++mPosition;
    return NOERROR;
}

ECode MappedByteBufferAdapter::GetByteEx(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    return MappedByteBuffer::mWrapped->GetByteEx(index, value);
}

ECode MappedByteBufferAdapter::GetBytesEx(
    /* [out] */ ArrayOf<Byte>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 byteCount)
{
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->GetBytesEx(dst, dstOffset, byteCount))
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

ECode MappedByteBufferAdapter::GetCharEx(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return MappedByteBuffer::mWrapped->GetCharEx(index, value);
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

ECode MappedByteBufferAdapter::GetDoubleEx(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return MappedByteBuffer::mWrapped->GetDoubleEx(index, value);
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

ECode MappedByteBufferAdapter::GetFloatEx(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return MappedByteBuffer::mWrapped->GetFloatEx(index, value);
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

ECode MappedByteBufferAdapter::GetInt32Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return MappedByteBuffer::mWrapped->GetInt32Ex(index, value);
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

ECode MappedByteBufferAdapter::GetInt64Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return MappedByteBuffer::mWrapped->GetInt64Ex(index, value);
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

ECode MappedByteBufferAdapter::GetInt16Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return MappedByteBuffer::mWrapped->GetInt16Ex(index, value);
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

ECode MappedByteBufferAdapter::PutByte(
    /* [in] */ Byte b)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->PutByte(b))
    mPosition++;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutByteEx(
    /* [in] */ Int32 index,
    /* [in] */ Byte b)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    return MappedByteBuffer::mWrapped->PutByteEx(index, b);
}

ECode MappedByteBufferAdapter::PutBytesEx(
    /* [in] */ const ArrayOf<Byte>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 byteCount)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->PutBytesEx(src, srcOffset, byteCount))
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

ECode MappedByteBufferAdapter::PutCharEx(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    return MappedByteBuffer::mWrapped->PutCharEx(index, value);
}

ECode MappedByteBufferAdapter::PutDouble(
    /* [in] */ Double value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->PutDouble(value))
    mPosition += sizeof(Double); // SizeOf.DOUBLE;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutDoubleEx(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    return MappedByteBuffer::mWrapped->PutDoubleEx(index, value);
}

ECode MappedByteBufferAdapter::PutFloat(
    /* [in] */ Float value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->PutFloat(value))
    mPosition += sizeof(Float); // SizeOf.FLOAT;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutFloatEx(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    return MappedByteBuffer::mWrapped->PutFloatEx(index, value);
}

ECode MappedByteBufferAdapter::PutInt32(
    /* [in] */ Int32 value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->PutInt32(value))
    mPosition += sizeof(Int32); // SizeOf.INT;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutInt32Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    return MappedByteBuffer::mWrapped->PutInt32Ex(index, value);
}

ECode MappedByteBufferAdapter::PutInt64(
    /* [in] */ Int64 value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->PutInt64(value))
    mPosition += sizeof(Int64); // SizeOf.LONG;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutInt64Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    return MappedByteBuffer::mWrapped->PutInt64Ex(index, value);
}

ECode MappedByteBufferAdapter::PutInt16(
    /* [in] */ Int16 value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    ASSERT_SUCCEEDED(MappedByteBuffer::mWrapped->PutInt16(value))
    mPosition += sizeof(Int16); // SizeOf.SHORT;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutInt16Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    MappedByteBuffer::mWrapped->SetPosition(mPosition);
    return MappedByteBuffer::mWrapped->PutInt16Ex(index, value);
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
    INTERFACE_ADDREF(*buffer)
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

ECode MappedByteBufferAdapter::GetBytes(
    /* [out] */ ArrayOf<Byte>* dst)
{
    return ByteBuffer::GetBytes(dst);
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

ECode MappedByteBufferAdapter::PutBytes(
    /* [in] */ const ArrayOf<Byte>& src)
{
    return ByteBuffer::PutBytes(src);
}

ECode MappedByteBufferAdapter::PutByteBuffer(
    /* [in] */ IByteBuffer* src)
{
    return ByteBuffer::PutByteBuffer(src);
}

} // namespace IO
} // namespace Elastos
