
#include "ReadWriteCharArrayBuffer.h"
#include "coredef.h"
#include "ReadOnlyCharArrayBuffer.h"

using Elastos::Core::EIID_ICharSequence;

namespace Elastos {
namespace IO {

ReadWriteCharArrayBuffer::ReadWriteCharArrayBuffer(
    /* [in] */ ArrayOf<Char32>* array)
    : CharArrayBuffer(array)
{}

ReadWriteCharArrayBuffer::ReadWriteCharArrayBuffer(
    /* [in] */ Int32 capacity)
    : CharArrayBuffer(capacity)
{}

ReadWriteCharArrayBuffer::ReadWriteCharArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Char32>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : CharArrayBuffer(capacity, backingArray, arrayOffset)
{}

AutoPtr<ReadWriteCharArrayBuffer> ReadWriteCharArrayBuffer::Copy(
    /* [in] */ CharArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    AutoPtr<ReadWriteCharArrayBuffer> buf = new ReadWriteCharArrayBuffer(other->mCapacity,
            other->mBackingArray, other->mOffset);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = markOfOther;
    return buf;
}

PInterface ReadWriteCharArrayBuffer::Probe(
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

UInt32 ReadWriteCharArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteCharArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteCharArrayBuffer::GetInterfaceID(
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

ECode ReadWriteCharArrayBuffer::GetPrimitiveArray(
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

ECode ReadWriteCharArrayBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    return CharArrayBuffer::GetArray(array);
}

ECode ReadWriteCharArrayBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadWriteCharArrayBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadWriteCharArrayBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return CharArrayBuffer::GetArrayOffset(offset);
}

ECode ReadWriteCharArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<ReadOnlyCharArrayBuffer> buf = ReadOnlyCharArrayBuffer::Copy((CharArrayBuffer*)this, mMark);
    *buffer = (ICharBuffer*)buf.Get();
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::Compact()
{
    Int32 remaining = 0;
    GetRemaining(&remaining);
    mBackingArray->Copy(mOffset, mBackingArray, mPosition + mOffset, remaining);
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::CompareTo(
    /* [in] */ ICharBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return CharArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteCharArrayBuffer::Duplicate(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<ICharBuffer> buf = Copy((CharArrayBuffer*)this, mMark);
    *buffer = buf;
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::GetChar(
    /* [out] */ Char32* value)
{
    return CharArrayBuffer::GetChar(value);
}

ECode ReadWriteCharArrayBuffer::GetChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return CharArrayBuffer::GetChar(index, value);
}

ECode ReadWriteCharArrayBuffer::GetChars(
    /* [out] */ ArrayOf<Char32>* dst)
{
    return CharArrayBuffer::GetChars(dst);
}

ECode ReadWriteCharArrayBuffer::GetChars(
    /* [out] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 byteCount)
{
    return CharArrayBuffer::GetChars(dst, dstOffset, byteCount);
}

ECode ReadWriteCharArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return CharArrayBuffer::GetOrder(order);
}

CARAPI ReadWriteCharArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = mBackingArray;
    REFCOUNT_ADD(*array)
    return NOERROR;
}

CARAPI ReadWriteCharArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mOffset;
    return NOERROR;
}

CARAPI ReadWriteCharArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::PutChar(
    /* [in] */ Char32 c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    (*mBackingArray)[mOffset + mPosition++] = c;
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::PutChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 c)
{
    FAIL_RETURN(CheckIndex(index))
    (*mBackingArray)[mOffset + index] = c;
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::PutChars(
    /* [in] */ const ArrayOf<Char32>& src)
{
    return CharArrayBuffer::PutChars(src);
}

ECode ReadWriteCharArrayBuffer::PutChars(
    /* [in] */ const ArrayOf<Char32>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (charCount > remaining) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }

    mBackingArray->Copy(mOffset + mPosition, &src, srcOffset, charCount);
    mPosition += charCount;
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::PutCharBuffer(
    /* [in] */ ICharBuffer* src)
{
    return CharArrayBuffer::PutCharBuffer(src);
}

ECode ReadWriteCharArrayBuffer::PutString(
    /* [in] */ const String& str)
{
    return CharArrayBuffer::PutString(str);
}

ECode ReadWriteCharArrayBuffer::PutString(
    /* [in] */ const String& str,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return CharArrayBuffer::PutString(str, start, end);
}

ECode ReadWriteCharArrayBuffer::Slice(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    *buffer = (ICharBuffer*)new ReadWriteCharArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::AppendChar(
    /* [in] */ Char32 c)
{
    return CharArrayBuffer::AppendChar(c);
}

ECode ReadWriteCharArrayBuffer::AppendChars(
    /* [in] */ ICharSequence* csq)
{
    return CharArrayBuffer::AppendChars(csq);
}

ECode ReadWriteCharArrayBuffer::AppendChars(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return CharArrayBuffer::AppendChars(csq, start, end);
}

ECode ReadWriteCharArrayBuffer::Read(
    /* [in] */ ICharBuffer* target,
    /* [out] */ Int32* number)
{
    return CharArrayBuffer::Read(target, number);
}

ECode ReadWriteCharArrayBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    return CharArrayBuffer::GetCapacity(cap);
}

ECode ReadWriteCharArrayBuffer::Clear()
{
    return CharArrayBuffer::Clear();
}

ECode ReadWriteCharArrayBuffer::Flip()
{
    return CharArrayBuffer::Flip();
}

ECode ReadWriteCharArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return CharArrayBuffer::HasArray(hasArray);
}

ECode ReadWriteCharArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return CharArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteCharArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return CharArrayBuffer::IsDirect(isDirect);
}

ECode ReadWriteCharArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return CharArrayBuffer::GetLimit(limit);
}

ECode ReadWriteCharArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return CharArrayBuffer::SetLimit(newLimit);
}

ECode ReadWriteCharArrayBuffer::Mark()
{
    return CharArrayBuffer::Mark();
}

ECode ReadWriteCharArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return CharArrayBuffer::GetPosition(pos);
}

ECode ReadWriteCharArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return CharArrayBuffer::SetPosition(newPosition);
}

ECode ReadWriteCharArrayBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return CharArrayBuffer::GetRemaining(remaining);
}

ECode ReadWriteCharArrayBuffer::Reset()
{
    return CharArrayBuffer::Reset();
}

ECode ReadWriteCharArrayBuffer::Rewind()
{
    return CharArrayBuffer::Rewind();
}

ECode ReadWriteCharArrayBuffer::GetLength(
    /* [out] */ Int32* number)
{
    return CharArrayBuffer::GetLength(number);
}

ECode ReadWriteCharArrayBuffer::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    return CharArrayBuffer::GetCharAt(index, c);
}

ECode ReadWriteCharArrayBuffer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    return CharArrayBuffer::SubSequence(start, end, csq);
}

ECode ReadWriteCharArrayBuffer::ToString(
    /* [out] */ String* str)
{
    return CharArrayBuffer::ToString(str);
}

ECode ReadWriteCharArrayBuffer::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* rst)
{
    return CharArrayBuffer::Equals(other, rst);
}

} // namespace IO
} // namespace Elastos
