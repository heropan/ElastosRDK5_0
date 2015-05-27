
#include "coredef.h"
#include "CharToByteBufferAdapter.h"
#include "ByteBuffer.h"
#include "DirectByteBuffer.h"
#include "HeapByteBuffer.h"
#include "ReadWriteDirectByteBuffer.h"
#include "ReadWriteHeapByteBuffer.h"

using Elastos::Core::EIID_ICharSequence;

namespace Elastos {
namespace IO {

CharToByteBufferAdapter::CharToByteBufferAdapter(
    /* [in] */ IByteBuffer* byteBuffer)
    : CharBuffer(((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mCapacity / sizeof(Char32))
    , mByteBuffer(byteBuffer)
{
    mByteBuffer->Clear();
    mEffectiveDirectAddress = ((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mEffectiveDirectAddress;
}

ECode CharToByteBufferAdapter::AsCharBuffer(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ ICharBuffer** charBuffer)
{
    VALIDATE_NOT_NULL(charBuffer)
    assert(byteBuffer != NULL);
    AutoPtr<IByteBuffer> slice;
    FAIL_RETURN(byteBuffer->Slice((IByteBuffer**)&slice))
    slice->SetOrder(((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *charBuffer = (ICharBuffer*) new CharToByteBufferAdapter(slice);
    INTERFACE_ADDREF(*charBuffer);
    return NOERROR;
}

PInterface CharToByteBufferAdapter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ICharBuffer*)this;
    }
    else if (riid == EIID_ICharBuffer) {
        return (ICharBuffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_ICharSequence) {
        return (ICharSequence*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_CharBuffer) {
        return reinterpret_cast<PInterface>((CharBuffer*)this);
    }

    return NULL;
}

UInt32 CharToByteBufferAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CharToByteBufferAdapter::Release()
{
    return ElRefBase::Release();
}

ECode CharToByteBufferAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(ICharBuffer*)this) {
        *pIID = EIID_ICharBuffer;
    }
    else if (pObject == (IInterface*)(ICharSequence*)this) {
        *pIID = EIID_ICharSequence;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CharToByteBufferAdapter::GetPrimitiveArray(
    /* [out] */ Handle32* arrayHandle)
{
    mArrayTemp = NULL;
    GetArray((ArrayOf<Char32>**)&mArrayTemp);
    if (mArrayTemp == NULL)
    {
        *arrayHandle = 0;
        return NOERROR;
    }
    Char32* primitiveArray = mArrayTemp->GetPayload();
    *arrayHandle = reinterpret_cast<Handle32>(primitiveArray);
    return NOERROR;
}

ECode CharToByteBufferAdapter::GetArray(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    return CharBuffer::GetArray(array);
}

ECode CharToByteBufferAdapter::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode CharToByteBufferAdapter::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode CharToByteBufferAdapter::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return CharBuffer::GetArrayOffset(offset);
}

ECode CharToByteBufferAdapter::AsReadOnlyBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> byteBuf;
    FAIL_RETURN(mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&byteBuf))
    AutoPtr<CharToByteBufferAdapter> buf = new CharToByteBufferAdapter(byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    buf->mByteBuffer->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *buffer = (ICharBuffer*)buf->Probe(EIID_ICharBuffer);
    INTERFACE_ADDREF(*buffer)
    return NOERROR;
}

ECode CharToByteBufferAdapter::Compact()
{
    Boolean isReadOnly = FALSE;
    mByteBuffer->IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    mByteBuffer->SetLimit(mLimit * sizeof(Char32));
    mByteBuffer->SetPosition(mPosition * sizeof(Char32));
    mByteBuffer->Compact();
    mByteBuffer->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode CharToByteBufferAdapter::CompareTo(
    /* [in] */ ICharBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return CharBuffer::CompareTo(otherBuffer, result);
}

ECode CharToByteBufferAdapter::Duplicate(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> bb;
    FAIL_RETURN(mByteBuffer->Duplicate((IByteBuffer**)&bb))
    bb->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    AutoPtr<CharToByteBufferAdapter> buf = new CharToByteBufferAdapter(bb);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    *buffer = (ICharBuffer*)buf->Probe(EIID_ICharBuffer);
    INTERFACE_ADDREF(*buffer)
    return NOERROR;
}

ECode CharToByteBufferAdapter::GetChar(
    /* [out] */ Char32* value)
{
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    return mByteBuffer->GetCharEx(mPosition++ * sizeof(Char32), value);
}

ECode CharToByteBufferAdapter::GetChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    FAIL_RETURN(CheckIndex(index))
    return mByteBuffer->GetCharEx(index * sizeof(Char32), value);
}

ECode CharToByteBufferAdapter::GetChars(
    /* [out] */ ArrayOf<Char32>* dst)
{
    return CharBuffer::GetChars(dst);
}

ECode CharToByteBufferAdapter::GetChars(
    /* [out] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    mByteBuffer->SetLimit(mLimit * sizeof(Char32));
    mByteBuffer->SetPosition(mPosition * sizeof(Char32));
    if (mByteBuffer->Probe(EIID_DirectByteBuffer) != NULL) {
        FAIL_RETURN(((DirectByteBuffer*)mByteBuffer->Probe(EIID_DirectByteBuffer))->GetCharsEx(dst, dstOffset, charCount))
    }
    else {
        FAIL_RETURN(((HeapByteBuffer*)mByteBuffer->Probe(EIID_HeapByteBuffer))->GetCharsEx(dst, dstOffset, charCount))
    }
    mPosition += charCount;
    return NOERROR;
}

ECode CharToByteBufferAdapter::GetLength(
    /* [out] */ Int32* number)
{
    return CharBuffer::GetLength(number);
}

ECode CharToByteBufferAdapter::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return mByteBuffer->GetOrder(order);
}

ECode CharToByteBufferAdapter::PutChar(
    /* [in] */ Char32 c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    return mByteBuffer->PutCharEx(mPosition++ * sizeof(Char32), c);
}

ECode CharToByteBufferAdapter::PutChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 c)
{
    FAIL_RETURN(CheckIndex(index))
    return mByteBuffer->PutCharEx(index * sizeof(Char32), c);
}

ECode CharToByteBufferAdapter::PutChars(
    /* [in] */ const ArrayOf<Char32>& src)
{
    return CharBuffer::PutChars(src);
}

ECode CharToByteBufferAdapter::PutChars(
    /* [in] */ const ArrayOf<Char32>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    mByteBuffer->SetLimit(mLimit * sizeof(Char32));
    mByteBuffer->SetPosition(mPosition * sizeof(Char32));
    if (mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer) != NULL ) {
        FAIL_RETURN(((ReadWriteDirectByteBuffer*)mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer))->PutChar32s(
                src, srcOffset, charCount))
    }
    else {
        FAIL_RETURN(((ReadWriteHeapByteBuffer*)mByteBuffer.Get())->PutChar32s(src, srcOffset, charCount))
    }
    mPosition += charCount;
    return NOERROR;
}

ECode CharToByteBufferAdapter::PutCharBuffer(
    /* [in] */ ICharBuffer* src)
{
    return CharBuffer::PutCharBuffer(src);
}

ECode CharToByteBufferAdapter::PutString(
    /* [in] */ const String& str)
{
    return CharBuffer::PutString(str);
}

ECode CharToByteBufferAdapter::PutString(
    /* [in] */ const String& str,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return CharBuffer::PutStringEx(str, start, end);
}

ECode CharToByteBufferAdapter::Slice(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    mByteBuffer->SetLimit(mLimit * sizeof(Char32));
    mByteBuffer->SetPosition(mPosition * sizeof(Char32));
    AutoPtr<IByteBuffer> bb;
    FAIL_RETURN(mByteBuffer->Slice((IByteBuffer**)&bb))
    bb->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *buffer = (ICharBuffer*) new CharToByteBufferAdapter(bb);
    INTERFACE_ADDREF(*buffer);
    mByteBuffer->Clear();
    return NOERROR;
}

ECode CharToByteBufferAdapter::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    return CharBuffer::GetCharAt(index, c);
}

ECode CharToByteBufferAdapter::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);
    FAIL_RETURN(CheckStartEndRemaining(start, end))
    AutoPtr<ICharBuffer> result;
    Duplicate((ICharBuffer**)&result);
    result->SetLimit(mPosition + end);
    result->SetPosition(mPosition + start);
    *csq = (ICharSequence*)result->Probe(EIID_ICharSequence);
    INTERFACE_ADDREF(*csq)
    return NOERROR;
}

ECode CharToByteBufferAdapter::AppendChar(
    /* [in] */ Char32 c)
{
    return CharBuffer::AppendChar(c);
}

ECode CharToByteBufferAdapter::AppendChars(
    /* [in] */ ICharSequence* csq)
{
    return CharBuffer::AppendChars(csq);
}

ECode CharToByteBufferAdapter::AppendChars(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return CharBuffer::AppendCharsEx(csq, start, end);
}

ECode CharToByteBufferAdapter::Read(
    /* [in] */ ICharBuffer* target,
    /* [out] */ Int32* number)
{
    return CharBuffer::Read(target, number);
}

ECode CharToByteBufferAdapter::GetCapacity(
    /* [out] */ Int32* cap)
{
    return CharBuffer::GetCapacity(cap);
}

ECode CharToByteBufferAdapter::Clear()
{
    return CharBuffer::Clear();
}

ECode CharToByteBufferAdapter::Flip()
{
    return CharBuffer::Flip();
}

ECode CharToByteBufferAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return CharBuffer::HasArray(hasArray);
}

ECode CharToByteBufferAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return CharBuffer::HasRemaining(hasRemaining);
}

ECode CharToByteBufferAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return mByteBuffer->IsDirect(isDirect);
}

ECode CharToByteBufferAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return mByteBuffer->IsReadOnly(isReadOnly);
}

ECode CharToByteBufferAdapter::CharToByteBufferAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CharToByteBufferAdapter::CharToByteBufferAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CharToByteBufferAdapter::CharToByteBufferAdapter::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode CharToByteBufferAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return CharBuffer::GetLimit(limit);
}

ECode CharToByteBufferAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return CharBuffer::SetLimit(newLimit);
}

ECode CharToByteBufferAdapter::Mark()
{
    return CharBuffer::Mark();
}

ECode CharToByteBufferAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return CharBuffer::GetPosition(pos);
}

ECode CharToByteBufferAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return CharBuffer::SetPosition(newPosition);
}

ECode CharToByteBufferAdapter::Remaining(
    /* [out] */ Int32* remaining)
{
    return CharBuffer::GetRemaining(remaining);
}

ECode CharToByteBufferAdapter::Reset()
{
    return CharBuffer::Reset();
}

ECode CharToByteBufferAdapter::Rewind()
{
    return CharBuffer::Rewind();
}

ECode CharToByteBufferAdapter::ToString(
    /* [out] */ String* str)
{
    return CharBuffer::ToString(str);
}

ECode CharToByteBufferAdapter::GetRemaining(
    /* [out] */ Int32* remaining)
{
    VALIDATE_NOT_NULL(remaining);
    *remaining = mLimit - mPosition;
    return NOERROR;
}

ECode CharToByteBufferAdapter::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* rst)
{
    return CharBuffer::Equals(object, rst);
}

} // namespace IO
} // namespace Elastos
