#ifndef __HEAPBYTEBUFFER_H__
#define __HEAPBYTEBUFFER_H__

#include "BaseByteBuffer.h"

namespace Elastos {
namespace IO {

extern "C" const InterfaceID EIID_HeapByteBuffer;

/**
 * HeapByteBuffer, ReadWriteHeapByteBuffer and ReadOnlyHeapByteBuffer compose
 * the implementation of array based byte buffers.
 * <p>
 * HeapByteBuffer implements all the shared readonly methods and is extended by
 * the other two classes.
 * </p>
 * <p>
 * All methods are marked final for runtime performance.
 * </p>
 *
 */
class HeapByteBuffer
    : public BaseByteBuffer
{
protected:
    HeapByteBuffer(
        /* [in] */ ArrayOf<Byte>* backingArray);

    HeapByteBuffer(
        /* [in] */ Int32 capacity);

    HeapByteBuffer(
        /* [in] */ ArrayOf<Byte>* backingArray,
        /* [in] */ Int32 capacity,
        /* [in] */ Int32 offset);

public:
    CARAPI GetBytes(
        /* [out] */ ArrayOf<Byte>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 byteCount);

    CARAPI GetChars(
        /* [out] */ ArrayOf<Char32>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 charCount);

    CARAPI GetDoubles(
        /* [out] */ ArrayOf<Double>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 doubleCount);

    CARAPI GetFloats(
        /* [out] */ ArrayOf<Float>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 floatCount);

    CARAPI GetInt16s(
        /* [out] */ ArrayOf<Int16>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 int16Count);

    CARAPI GetInt32s(
        /* [out] */ ArrayOf<Int32>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 int32Count);

    CARAPI GetInt64s(
        /* [out] */ ArrayOf<Int64>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 int64Count);

    CARAPI GetByte(
        /* [out] */ Byte* value);

    CARAPI GetByte(
        /* [in] */ Int32 index,
        /* [out] */ Byte* value);

    CARAPI GetChar32(
        /* [out] */ Char32* c);

    CARAPI GetChar32(
        /* [in] */ Int32 index,
        /* [out] */ Char32* c);

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

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

public:
    AutoPtr< ArrayOf<Byte> > mBackingArray;
    Int32 mOffset;
};

} // namespace IO
} // namespace Elastos

#endif // __HEAPBYTEBUFFER_H__
