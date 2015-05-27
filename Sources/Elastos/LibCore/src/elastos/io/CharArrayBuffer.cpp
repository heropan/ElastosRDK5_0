
#include "coredef.h"
#include "CharArrayBuffer.h"
#include "CByteOrderHelper.h"
#include <elastos/StringBuilder.h>

using Elastos::Core::EIID_ICharSequence;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

CharArrayBuffer::CharArrayBuffer(
    /* [in] */ ArrayOf<Char32>* array)
    : CharBuffer(array->GetLength())
    , mBackingArray(array)
    , mOffset(0)
{}

CharArrayBuffer::CharArrayBuffer(
    /* [in] */ Int32 capacity)
    : CharBuffer(capacity)
    , mBackingArray(ArrayOf<Char32>::Alloc(capacity))
    , mOffset(0)
{}

CharArrayBuffer::CharArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Char32>* backingArray,
    /* [in] */ Int32 offset)
    : CharBuffer(capacity)
    , mBackingArray(backingArray)
    , mOffset(0)
{}

ECode CharArrayBuffer::GetChar(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value)
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = (*mBackingArray)[mOffset + mPosition++];
    return NOERROR;
}

ECode CharArrayBuffer::GetChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value)
    FAIL_RETURN(CheckIndex(index))
    *value = (*mBackingArray)[mOffset + index];
    return NOERROR;
}

ECode CharArrayBuffer::GetChars(
    /* [out] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    Int32 remaining;
    GetRemaining(&remaining);
    if (charCount > remaining) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    dst->Copy(dstOffset, mBackingArray, mOffset + mPosition, charCount);
    mPosition += charCount;
    return NOERROR;
}

ECode CharArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect)
    *isDirect = FALSE;
    return NOERROR;
}

ECode CharArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    VALIDATE_NOT_NULL(byteOrder)
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    return helper->GetNativeOrder(byteOrder);
}

ECode CharArrayBuffer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq)
    FAIL_RETURN(CheckStartEndRemaining(start, end))
    AutoPtr<ICharBuffer> result;
    Duplicate((ICharBuffer**)&result);
    result->SetLimit(mPosition + end);
    result->SetPosition(mPosition + start);
    *csq = ICharSequence::Probe(result);
    INTERFACE_ADDREF(*csq);
    return NOERROR;
}

ECode CharArrayBuffer::ToString(
    /* [out] */ String* str)
{
    // return String.copyValueOf(backingArray, offset + position, remaining());
    Int32 remaining = 0;
    Int32 offset = mOffset + mPosition;
    StringBuilder sb;
    GetRemaining(&remaining);
    for (Int32 i = 0; i < remaining; ++i) {
        sb += (*mBackingArray)[offset + i];
    }

    return sb.ToString(str);
}

} // namespace IO
} // namespace Elastos
