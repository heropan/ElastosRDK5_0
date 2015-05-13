
#include "cmdef.h"
#include "HeapByteBuffer.h"
#include <elastos/Math.h>
#include "Memory.h"
#include "CByteOrderHelper.h"

namespace Elastos {
namespace IO {

// {5F67E9B8-5F51-D90B-15DA-B034EA86B386}
extern "C" const InterfaceID EIID_HeapByteBuffer =
    { 0x5f67e9b8, 0x5f51, 0xd90b, { 0x15, 0xda, 0xb0, 0x34, 0xea, 0x86, 0xb3, 0x86 } };

HeapByteBuffer::HeapByteBuffer(
    /* [in] */ ArrayOf<Byte>* backingArray)
    : BaseByteBuffer(backingArray->GetLength(), NULL)
    , mBackingArray(backingArray)
    , mOffset(0)
{}

HeapByteBuffer::HeapByteBuffer(
    /* [in] */ Int32 capacity)
    : BaseByteBuffer(capacity, NULL)
    , mBackingArray(ArrayOf<Byte>::Alloc(capacity))
    , mOffset(0)
{}

HeapByteBuffer::HeapByteBuffer(
    /* [in] */ ArrayOf<Byte>* backingArray,
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 offset)
    : BaseByteBuffer(capacity, NULL)
    , mBackingArray(backingArray)
    , mOffset(offset)
{
    assert(offset + capacity <= backingArray->GetLength());
}

ECode HeapByteBuffer::GetBytesEx(
    /* [out] */ ArrayOf<Byte>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 byteCount)
{
    VALIDATE_NOT_NULL(dst);

    Int32 bounds;
    FAIL_RETURN(CheckGetBounds(1, dst->GetLength(), dstOffset, byteCount, &bounds));
    dst->Copy(dstOffset, mBackingArray, mOffset + mPosition, byteCount);
    mPosition += byteCount;
    return NOERROR;
}

ECode HeapByteBuffer::GetCharsEx(
    /* [out] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    VALIDATE_NOT_NULL(dst);

    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Char32), dst->GetLength(), dstOffset, charCount, &byteCount));
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    FAIL_RETURN(Memory::UnsafeBulkGet((Byte*)dst->GetPayload(), dstOffset, byteCount, const_cast<ArrayOf<Byte>&>(*mBackingArray),
            mOffset + mPosition, sizeof(Char32), needsSwap))
    mPosition += byteCount;
    return NOERROR;
}

ECode HeapByteBuffer::GetDoublesEx(
    /* [out] */ ArrayOf<Double>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 doubleCount)
{
    VALIDATE_NOT_NULL(dst);

    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Double), dst->GetLength(), dstOffset, doubleCount, &byteCount));
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    FAIL_RETURN(Memory::UnsafeBulkGet((Byte*)dst->GetPayload(), dstOffset, byteCount, const_cast<ArrayOf<Byte>&>(*mBackingArray),
            mOffset + mPosition, sizeof(Double), needsSwap))
    mPosition += byteCount;
    return NOERROR;
}

ECode HeapByteBuffer::GetFloatsEx(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 floatCount)
{
    VALIDATE_NOT_NULL(dst);

    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Float), dst->GetLength(), dstOffset, floatCount, &byteCount));
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    FAIL_RETURN(Memory::UnsafeBulkGet((Byte*)dst->GetPayload(), dstOffset, byteCount, const_cast<ArrayOf<Byte>&>(*mBackingArray),
            mOffset + mPosition, sizeof(Float), needsSwap))
    mPosition += byteCount;
    return NOERROR;
}

ECode HeapByteBuffer::GetInt16sEx(
    /* [out] */ ArrayOf<Int16>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int16Count)
{
    VALIDATE_NOT_NULL(dst);

    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Int16), dst->GetLength(), dstOffset, int16Count, &byteCount));
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    FAIL_RETURN(Memory::UnsafeBulkGet((Byte*)dst->GetPayload(), dstOffset, byteCount, const_cast<ArrayOf<Byte>&>(*mBackingArray),
            mOffset + mPosition, sizeof(Int16), needsSwap))
    mPosition += byteCount;
    return NOERROR;
}

ECode HeapByteBuffer::GetInt32sEx(
    /* [out] */ ArrayOf<Int32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int32Count)
{
    VALIDATE_NOT_NULL(dst);

    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Int32), dst->GetLength(), dstOffset, int32Count, &byteCount));
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    FAIL_RETURN(Memory::UnsafeBulkGet((Byte*)dst->GetPayload(), dstOffset, byteCount, const_cast<ArrayOf<Byte>&>(*mBackingArray),
            mOffset + mPosition, sizeof(Int32), needsSwap))
    mPosition += byteCount;
    return NOERROR;
}

ECode HeapByteBuffer::GetInt64sEx(
    /* [out] */ ArrayOf<Int64>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int64Count)
{
    VALIDATE_NOT_NULL(dst);

    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Int64), dst->GetLength(), dstOffset, int64Count, &byteCount));
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    FAIL_RETURN(Memory::UnsafeBulkGet((Byte*)dst->GetPayload(), dstOffset, byteCount, const_cast<ArrayOf<Byte>&>(*mBackingArray),
            mOffset + mPosition, sizeof(Int64), needsSwap))
    mPosition += byteCount;
    return NOERROR;
}

ECode HeapByteBuffer::GetByte(
    /* [out] */ Byte* value)
{
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = (*mBackingArray)[mOffset + mPosition++];
    return NOERROR;
}

ECode HeapByteBuffer::GetByteEx(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    FAIL_RETURN(CheckIndex(index));
    *value = (*mBackingArray)[mOffset + index];
    return NOERROR;
}

ECode HeapByteBuffer::GetChar32(
    /* [out] */ Char32* c)
{
    Int32 newPosition = mPosition + sizeof(Char32);
    if (newPosition > mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *c = (Char32) Memory::PeekInt32(const_cast<ArrayOf<Byte>&>(*mBackingArray), mOffset + mPosition, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode HeapByteBuffer::GetChar32Ex(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    FAIL_RETURN(CheckIndex(index));
    *c = (Char32) Memory::PeekInt32(const_cast<ArrayOf<Byte>&>(*mBackingArray), mOffset + index, mOrder);
    return NOERROR;
}

ECode HeapByteBuffer::GetDouble(
    /* [out] */ Double* value)
{
    Int64 val = 0;
    FAIL_RETURN(GetInt64(&val));
    *value = Elastos::Core::Math::Int64BitsToDouble(val);
    return NOERROR;
}

ECode HeapByteBuffer::GetDoubleEx(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    Int64 val = 0;
    FAIL_RETURN(GetInt64Ex(index, &val));
    *value = Elastos::Core::Math::Int64BitsToDouble(val);
    return NOERROR;
}

ECode HeapByteBuffer::GetFloat(
    /* [out] */ Float* value)
{
    Int32 val = 0;
    FAIL_RETURN(GetInt32(&val));
    *value = Elastos::Core::Math::Int32BitsToFloat(val);
    return NOERROR;
}

ECode HeapByteBuffer::GetFloatEx(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    Int32 val = 0;
    FAIL_RETURN(GetInt32Ex(index, &val));
    *value = Elastos::Core::Math::Int32BitsToFloat(val);
    return NOERROR;
}

ECode HeapByteBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    Int32 newPosition = mPosition + sizeof(Int32);
    if (newPosition > mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = Memory::PeekInt32(const_cast<ArrayOf<Byte>&>(*mBackingArray), mOffset + mPosition, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode HeapByteBuffer::GetInt32Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    FAIL_RETURN(CheckIndex(index, sizeof(Int32)));
    *value = Memory::PeekInt32(const_cast<ArrayOf<Byte>&>(*mBackingArray), mOffset + index, mOrder);
    return NOERROR;
}

ECode HeapByteBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    Int32 newPosition = mPosition + sizeof(Int64);
    if (newPosition > mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = Memory::PeekInt64(const_cast<ArrayOf<Byte>&>(*mBackingArray), mOffset + mPosition, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode HeapByteBuffer::GetInt64Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    FAIL_RETURN(CheckIndex(index, sizeof(Int64)));
    *value = Memory::PeekInt64(const_cast<ArrayOf<Byte>&>(*mBackingArray), mOffset + index, mOrder);
    return NOERROR;
}

ECode HeapByteBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    Int32 newPosition = mPosition + sizeof(Int16);
    if (newPosition > mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = Memory::PeekInt16(const_cast<ArrayOf<Byte>&>(*mBackingArray), mOffset + mPosition, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode HeapByteBuffer::GetInt16Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    FAIL_RETURN(CheckIndex(index, sizeof(Int16)));
    *value = Memory::PeekInt16(const_cast<ArrayOf<Byte>&>(*mBackingArray), mOffset + index, mOrder);
    return NOERROR;
}

ECode HeapByteBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    *isDirect = FALSE;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
