
#include "CharSequenceAdapter.h"
#include "CByteOrderHelper.h"

using Elastos::Core::EIID_ICharSequence;

namespace Elastos {
namespace IO {

CAR_INTERFACE_IMPL(CharSequenceAdapter, CharBuffer, ICharSequence)

CharSequenceAdapter::CharSequenceAdapter(
    /* [in] */ Int32 capacity,
    /* [in] */ ICharSequence* chseq)
    : CharBuffer(capacity, 0)
    , mSequence(chseq)
{}

AutoPtr<CharSequenceAdapter> CharSequenceAdapter::Copy(
    /* [in] */ CharSequenceAdapter* other)
{
    Int32 len = 0;
    other->mSequence->GetLength(&len);
    AutoPtr<CharSequenceAdapter> buf = new CharSequenceAdapter(len, other->mSequence);
    buf->mLimit = other->mLimit;
    buf->mPosition = other->mPosition;
    buf->mMark = other->mMark;
    return buf;
}

ECode CharSequenceAdapter::GetPrimitiveArray(
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

ECode CharSequenceAdapter::GetArray(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    return CharBuffer::GetArray(array);
}

ECode CharSequenceAdapter::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode CharSequenceAdapter::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode CharSequenceAdapter::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return CharBuffer::GetArrayOffset(offset);
}

ECode CharSequenceAdapter::AsReadOnlyBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    return Duplicate(buffer);
}

ECode CharSequenceAdapter::Compact()
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode CharSequenceAdapter::CompareTo(
    /* [in] */ ICharBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return CharBuffer::CompareTo(otherBuffer, result);
}

ECode CharSequenceAdapter::Duplicate(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<CharSequenceAdapter> buf = Copy(this);
    *buffer = (ICharBuffer*)buf.Get();
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode CharSequenceAdapter::Get(
    /* [out] */ Char32* value)
{
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }

    return mSequence->GetCharAt(mPosition++, value);
}

ECode CharSequenceAdapter::Get(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    FAIL_RETURN(CheckIndex(index))
    return mSequence->GetCharAt(index, value);
}

ECode CharSequenceAdapter::Get(
    /* [out] */ ArrayOf<Char32>* dst)
{
    return CharBuffer::Get(dst);
}

ECode CharSequenceAdapter::Get(
    /* [out] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    VALIDATE_NOT_NULL(dst);
    Int32 length = dst->GetLength();
    if ((dstOffset | charCount) < 0 || dstOffset > length || length - dstOffset < charCount) {
        // throw new ArrayIndexOutOfBoundsException(length, offset, count);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (charCount > remaining) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }

    // sequence.toString().getChars(position, newPosition, dst, dstOffset);
    Int32 newPosition = mPosition + charCount;
    String s;
    mSequence->ToString(&s);

    // Note: last character not copied!
    if (mPosition >= 0 && mPosition <= newPosition && newPosition <= (Int32)s.GetLength()) {
        // System.arraycopy(value, mPosition, dst, dstOffset, newPosition - mPosition);
        AutoPtr<ArrayOf<Char32> > charArray = s.GetChars(mPosition, newPosition);
        dst->Copy(dstOffset, charArray, 0, charArray->GetLength());
    }
    else {
        // We throw StringIndexOutOfBoundsException rather than System.arraycopy's AIOOBE.
        // throw startEndAndLength(start, end);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    mPosition = newPosition;
    return NOERROR;
}

ECode CharSequenceAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = FALSE;
    return NOERROR;
}

ECode CharSequenceAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

ECode CharSequenceAdapter::GetOrder(
    /* [out] */ ByteOrder* order)
{
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    return helper->GetNativeOrder(order);
}

ECode CharSequenceAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CharSequenceAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CharSequenceAdapter::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode CharSequenceAdapter::Put(
    /* [in] */ Char32 c)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode CharSequenceAdapter::Put(
    /* [in] */ Int32 index,
    /* [in] */ Char32 c)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode CharSequenceAdapter::Put(
    /* [in] */ ArrayOf<Char32>* src)
{
    return CharBuffer::Put(src);
}

ECode CharSequenceAdapter::Put(
    /* [in] */ ArrayOf<Char32>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode CharSequenceAdapter::Put(
    /* [in] */ ICharBuffer* src)
{
    return CharBuffer::Put(src);
}

ECode CharSequenceAdapter::Put(
    /* [in] */ const String& str)
{
    return CharBuffer::Put(str);
}

ECode CharSequenceAdapter::Put(
    /* [in] */ const String& str,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    // throw new ReadOnlyBufferException();
    return E_READ_ONLY_BUFFER_EXCEPTION;
}

ECode CharSequenceAdapter::Slice(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(mSequence->SubSequence(mPosition, mLimit, (ICharSequence**)&seq))
    Int32 len = 0;
    seq->GetLength(&len);
    *buffer = (ICharBuffer*)new CharSequenceAdapter(len, seq);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode CharSequenceAdapter::GetLength(
    /* [out] */ Int32* number)
{
    return CharBuffer::GetLength(number);
}

ECode CharSequenceAdapter::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    return CharBuffer::GetCharAt(index, c);
}

ECode CharSequenceAdapter::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);
    FAIL_RETURN(CheckStartEndRemaining(start, end))
    AutoPtr<CharSequenceAdapter> result = Copy(this);
    result->mPosition = mPosition + start;
    result->mLimit = mPosition + end;
    *csq = (ICharSequence*)result.Get();
    REFCOUNT_ADD(*csq)
    return NOERROR;
}

ECode CharSequenceAdapter::ToString(
    /* [out] */ String* str)
{
    return CharBuffer::ToString(str);
}

} // namespace IO
} // namespace Elastos
