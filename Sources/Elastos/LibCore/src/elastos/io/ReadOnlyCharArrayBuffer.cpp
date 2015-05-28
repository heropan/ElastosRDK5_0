
#include "coredef.h"
#include "ReadOnlyCharArrayBuffer.h"

using Elastos::Core::EIID_ICharSequence;

namespace Elastos {
namespace IO {

ReadOnlyCharArrayBuffer::ReadOnlyCharArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Char32>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : CharArrayBuffer(capacity, backingArray, arrayOffset)
{}

AutoPtr<ReadOnlyCharArrayBuffer> ReadOnlyCharArrayBuffer::Copy(
    /* [in] */ CharArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    AutoPtr<ReadOnlyCharArrayBuffer> buf = new ReadOnlyCharArrayBuffer(other->mCapacity,
            other->mBackingArray, other->mOffset);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = markOfOther;
    return buf;
}

PInterface ReadOnlyCharArrayBuffer::Probe(
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


UInt32 ReadOnlyCharArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadOnlyCharArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadOnlyCharArrayBuffer::GetInterfaceID(
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

ECode ReadOnlyCharArrayBuffer::GetPrimitiveArray(
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

ECode ReadOnlyCharArrayBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    return CharArrayBuffer::GetArray(array);
}

ECode ReadOnlyCharArrayBuffer::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode ReadOnlyCharArrayBuffer::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode ReadOnlyCharArrayBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return CharArrayBuffer::GetArrayOffset(offset);
}

ECode ReadOnlyCharArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    return Duplicate(buffer);
}

ECode ReadOnlyCharArrayBuffer::Compact()
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyCharArrayBuffer::CompareTo(
    /* [in] */ ICharBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return CharArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyCharArrayBuffer::Duplicate(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<ReadOnlyCharArrayBuffer> buf = Copy((CharArrayBuffer*)this, mMark);
    *buffer = (ICharBuffer*)buf.Get();
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadOnlyCharArrayBuffer::GetChar(
    /* [out] */ Char32* value)
{
    return CharArrayBuffer::GetChar(value);
}

ECode ReadOnlyCharArrayBuffer::GetChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return CharArrayBuffer::GetChar(index, value);
}

ECode ReadOnlyCharArrayBuffer::GetChars(
    /* [out] */ ArrayOf<Char32>* dst)
{
    return CharArrayBuffer::GetChars(dst);
}

ECode ReadOnlyCharArrayBuffer::GetChars(
    /* [out] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 byteCount)
{
    return CharArrayBuffer::GetChars(dst, dstOffset, byteCount);
}

ECode ReadOnlyCharArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return CharArrayBuffer::GetOrder(order);
}

CARAPI ReadOnlyCharArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

CARAPI ReadOnlyCharArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

CARAPI ReadOnlyCharArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode ReadOnlyCharArrayBuffer::PutChar(
    /* [in] */ Char32 c)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyCharArrayBuffer::PutChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 c)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyCharArrayBuffer::PutChars(
    /* [in] */ const ArrayOf<Char32>& src)
{
    return CharArrayBuffer::PutChars(src);
}

ECode ReadOnlyCharArrayBuffer::PutChars(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Char32>& src)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyCharArrayBuffer::PutCharBuffer(
    /* [in] */ ICharBuffer* src)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode ReadOnlyCharArrayBuffer::PutString(
    /* [in] */ const String& str)
{
    return CharArrayBuffer::PutString(str);
}

ECode ReadOnlyCharArrayBuffer::PutString(
    /* [in] */ const String& str,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return CharArrayBuffer::PutString(str, start, end);
}

ECode ReadOnlyCharArrayBuffer::Slice(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    *buffer = (ICharBuffer*)new ReadOnlyCharArrayBuffer(remaining, mBackingArray, mOffset + mPosition);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode ReadOnlyCharArrayBuffer::AppendChar(
    /* [in] */ Char32 c)
{
    return CharArrayBuffer::AppendChar(c);
}

ECode ReadOnlyCharArrayBuffer::AppendChars(
    /* [in] */ ICharSequence* csq)
{
    return CharArrayBuffer::AppendChars(csq);
}

ECode ReadOnlyCharArrayBuffer::AppendChars(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return CharArrayBuffer::AppendChars(csq, start, end);
}

ECode ReadOnlyCharArrayBuffer::Read(
    /* [in] */ ICharBuffer* target,
    /* [out] */ Int32* number)
{
    return CharArrayBuffer::Read(target, number);
}

ECode ReadOnlyCharArrayBuffer::GetCapacity(
    /* [out] */ Int32* cap)
{
    return CharArrayBuffer::GetCapacity(cap);
}

ECode ReadOnlyCharArrayBuffer::Clear()
{
    return CharArrayBuffer::Clear();
}

ECode ReadOnlyCharArrayBuffer::Flip()
{
    return CharArrayBuffer::Flip();
}

ECode ReadOnlyCharArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return CharArrayBuffer::HasArray(hasArray);
}

ECode ReadOnlyCharArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return CharArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyCharArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return CharArrayBuffer::IsDirect(isDirect);
}

ECode ReadOnlyCharArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

ECode ReadOnlyCharArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return CharArrayBuffer::GetLimit(limit);
}

ECode ReadOnlyCharArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return CharArrayBuffer::SetLimit(newLimit);
}

ECode ReadOnlyCharArrayBuffer::Mark()
{
    return CharArrayBuffer::Mark();
}

ECode ReadOnlyCharArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return CharArrayBuffer::GetPosition(pos);
}

ECode ReadOnlyCharArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return CharArrayBuffer::SetPosition(newPosition);
}

ECode ReadOnlyCharArrayBuffer::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return CharArrayBuffer::GetRemaining(remaining);
}

ECode ReadOnlyCharArrayBuffer::Reset()
{
    return CharArrayBuffer::Reset();
}

ECode ReadOnlyCharArrayBuffer::Rewind()
{
    return CharArrayBuffer::Rewind();
}

ECode ReadOnlyCharArrayBuffer::GetLength(
    /* [out] */ Int32* number)
{
    return CharArrayBuffer::GetLength(number);
}

ECode ReadOnlyCharArrayBuffer::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    return CharArrayBuffer::GetCharAt(index, c);
}

ECode ReadOnlyCharArrayBuffer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    return CharArrayBuffer::SubSequence(start, end, csq);
}

ECode ReadOnlyCharArrayBuffer::ToString(
    /* [out] */ String* str)
{
    return CharArrayBuffer::ToString(str);
}

ECode ReadOnlyCharArrayBuffer::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* rst)
{
    return CharArrayBuffer::Equals(object, rst);
}

ECode ReadOnlyCharArrayBuffer::PutChars(
    /* [in] */ const ArrayOf<Char32>& src,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len)
{
    return CharArrayBuffer::PutChars(src, off, len);
}

} // namespace IO
} // namespace Elastos
