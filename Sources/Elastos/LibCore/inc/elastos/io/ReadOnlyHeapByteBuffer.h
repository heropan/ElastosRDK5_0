#ifndef __ELASTOS_IO_READONLYHEAPBYTEBUFFER_H__
#define __ELASTOS_IO_READONLYHEAPBYTEBUFFER_H__

#include "HeapByteBuffer.h"

namespace Elastos {
namespace IO {

/**
 * HeapByteBuffer, ReadOnlyHeapByteBuffer and ReadOnlyHeapByteBuffer compose
 * the implementation of array based byte buffers.
 * <p>
 * ReadOnlyHeapByteBuffer extends HeapByteBuffer with all the write methods
 * throwing read only exception.
 * </p>
 * <p>
 * This class is marked final for runtime performance.
 * </p>
 *
 */
class ReadOnlyHeapByteBuffer
    : public HeapByteBuffer
{
public:
    CAR_INTERFACE_DECL()

    ReadOnlyHeapByteBuffer(
        /* [in] */ ArrayOf<Byte>* backingArray,
        /* [in] */ Int32 capacity,
        /* [in] */ Int32 arrayOffset);

    static CARAPI_(AutoPtr<ReadOnlyHeapByteBuffer>) Copy(
        /* [in] */ HeapByteBuffer* other,
        /* [in] */ Int32 markOfOther);

    CARAPI GetPrimitiveArray(
        /* [out] */ Handle32* arrayHandle);

    CARAPI GetArray(
        /* [out, callee] */ ArrayOf<Byte>** array);

    CARAPI GetElementSizeShift(
        /* [out] */ Int32* elementSizeShift);

    CARAPI GetEffectiveDirectAddress(
        /* [out] */ Int32* effectiveDirectAddress);

    CARAPI GetArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsCharBuffer(
        /* [out] */ ICharBuffer** buffer);

    CARAPI AsDoubleBuffer(
        /* [out] */ IDoubleBuffer** buffer);

    CARAPI AsFloatBuffer(
        /* [out] */ IFloatBuffer** buffer);

    CARAPI AsInt16Buffer(
        /* [out] */ IInt16Buffer** buffer);

    CARAPI AsInt32Buffer(
        /* [out] */ IInt32Buffer** buffer);

    CARAPI AsInt64Buffer(
        /* [out] */ IInt64Buffer** buffer);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IByteBuffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ IByteBuffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ IByteBuffer** buffer);

    CARAPI Get(
        /* [out] */ Byte* value);

    CARAPI Get(
        /* [in] */ Int32 index,
        /* [out] */ Byte* value);

    CARAPI Get(
        /* [out] */ ArrayOf<Byte>* dst);

    CARAPI Get(
        /* [out] */ ArrayOf<Byte>* dst,
        /* [in] */ Int32 off,
        /* [in] */ Int32 len);

    CARAPI GetChar(
        /* [out] */ Char32* value);

    CARAPI GetChar(
        /* [in] */ Int32 index,
        /* [out] */ Char32* value);

    CARAPI GetDouble(
        /* [out] */ Double* value);

    CARAPI GetDouble(
        /* [in] */ Int32 index,
        /* [out] */ Double* value);

    CARAPI GetFloat(
        /* [out] */ Float* value);

    CARAPI GetFloat(
        /* [in] */ Int32 index,
        /* [out] */ Float* value);

    CARAPI GetInt32(
        /* [out] */ Int32* value);

    CARAPI GetInt32(
        /* [in] */ Int32 index,
        /* [out] */ Int32* value);

    CARAPI GetInt64(
        /* [out] */ Int64* value);

    CARAPI GetInt64(
        /* [in] */ Int32 index,
        /* [out] */ Int64* value);

    CARAPI GetInt16(
        /* [out] */ Int16* value);

    CARAPI GetInt16(
        /* [in] */ Int32 index,
        /* [out] */ Int16* value);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI SetOrder(
        /* [in] */ ByteOrder byteOrder);

    CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Byte>** array);

    CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI ProtectedHasArray(
        /* [out] */ Boolean* hasArray);

    CARAPI Put(
        /* [in] */ Byte b);

    CARAPI Put(
        /* [in] */ Int32 index,
        /* [in] */ Byte b);

    CARAPI Put(
        /* [in] */ ArrayOf<Byte>* src);

    CARAPI Put(
        /* [in] */ ArrayOf<Byte>* src,
        /* [in] */ Int32 off,
        /* [in] */ Int32 len);

    CARAPI PutByteBuffer(
        /* [in] */ IByteBuffer* src);

    CARAPI PutChar(
        /* [in] */ Char32 value);

    CARAPI PutChar(
        /* [in] */ Int32 index,
        /* [in] */ Char32 value);

    CARAPI PutDouble(
        /* [in] */ Double value);

    CARAPI PutDouble(
        /* [in] */ Int32 index,
        /* [in] */ Double value);

    CARAPI PutFloat(
        /* [in] */ Float value);

    CARAPI PutFloat(
        /* [in] */ Int32 index,
        /* [in] */ Float value);

    CARAPI PutInt16(
        /* [in] */ Int16 value);

    CARAPI PutInt16(
        /* [in] */ Int32 index,
        /* [in] */ Int16 value);

    CARAPI PutInt32(
        /* [in] */ Int32 value);

    CARAPI PutInt32(
        /* [in] */ Int32 index,
        /* [in] */ Int32 value);

    CARAPI PutInt64(
        /* [in] */ Int64 value);

    CARAPI PutInt64(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value);

    CARAPI Slice(
        /* [out] */ IByteBuffer** buffer);

    CARAPI GetCapacity(
        /* [out] */ Int32* cap);

    CARAPI Clear();

    CARAPI Flip();

    CARAPI HasArray(
        /* [out] */ Boolean* hasArray);

    CARAPI HasRemaining(
        /* [out] */ Boolean* hasRemaining);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI IsReadOnly(
        /* [out] */ Boolean* isReadOnly);

    CARAPI GetLimit(
        /* [out] */ Int32* limit);

    CARAPI SetLimit(
        /* [in] */ Int32 newLimit);

    CARAPI Mark();

    CARAPI GetPosition(
        /* [out] */ Int32* pos);

    CARAPI SetPosition(
        /* [in] */ Int32 newPosition);

    CARAPI GetRemaining(
        /* [out] */ Int32* remaining);

    CARAPI Reset();

    CARAPI Rewind();

    virtual CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* isEquals);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    AutoPtr<ArrayOf<Byte> > mArrayTemp;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_READONLYHEAPBYTEBUFFER_H__
