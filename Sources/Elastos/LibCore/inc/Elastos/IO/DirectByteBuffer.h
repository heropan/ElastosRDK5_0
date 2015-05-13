#ifndef __DIRECTBYTEBUFFER_H__
#define __DIRECTBYTEBUFFER_H__

#include "BaseByteBuffer.h"
#include "MemoryBlock.h"

namespace Elastos {
namespace IO {

extern "C" const InterfaceID EIID_DirectByteBuffer;

/**
 * DirectByteBuffer, ReadWriteDirectByteBuffer and ReadOnlyDirectByteBuffer
 * compose the implementation of platform memory based byte buffers.
 * <p>
 * DirectByteBuffer implements all the shared readonly methods and is extended
 * by the other two classes.
 * </p>
 * <p>
 * All methods are marked final for runtime performance.
 * </p>
 *
 */
class DirectByteBuffer
    : public BaseByteBuffer
{
    friend class ReadWriteDirectByteBuffer;
    friend class ReadOnlyDirectByteBuffer;

public:
    CARAPI GetBytesEx(
        /* [out] */ ArrayOf<Byte>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 byteCount);

    CARAPI GetCharsEx(
        /* [out] */ ArrayOf<Char32>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 charCount);

    CARAPI GetDoublesEx(
        /* [out] */ ArrayOf<Double>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 doubleCount);

    CARAPI GetFloatsEx(
        /* [out] */ ArrayOf<Float>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 floatCount);

    CARAPI GetInt32sEx(
        /* [out] */ ArrayOf<Int32>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 int32Count);

    CARAPI GetInt64sEx(
        /* [out] */ ArrayOf<Int64>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 int64Count);

    CARAPI GetInt16sEx(
        /* [out] */ ArrayOf<Int16>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 int16Count);

    CARAPI GetByte(
        /* [out] */ Byte* value);

    CARAPI GetByteEx(
        /* [in] */ Int32 index,
        /* [out] */ Byte* value);

    CARAPI GetChar(
        /* [out] */ Char32* value);

    CARAPI GetCharEx(
        /* [in] */ Int32 index,
        /* [out] */ Char32* value);

    CARAPI GetDouble(
        /* [out] */ Double* value);

    CARAPI GetDoubleEx(
        /* [in] */ Int32 index,
        /* [out] */ Double* value);

    CARAPI GetFloat(
        /* [out] */ Float* value);

    CARAPI GetFloatEx(
        /* [in] */ Int32 index,
        /* [out] */ Float* value);

    CARAPI GetInt32(
        /* [out] */ Int32* value);

    CARAPI GetInt32Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int32* value);

    CARAPI GetInt64(
        /* [out] */ Int64* value);

    CARAPI GetInt64Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int64* value);

    CARAPI GetInt16(
        /* [out] */ Int16* value);

    CARAPI GetInt16Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int16* value);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI Free();

    CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Byte>** array);

    CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI ProtectedHasArray(
        /* [out] */ Boolean* hasArray);

protected:
    DirectByteBuffer(
        /* [in] */ MemoryBlock* block,
        /* [in] */ Int32 capacity,
        /* [in] */ Int32 offset);

protected:
    // This is the offset into {@code Buffer.block} at which this buffer logically starts.
    // TODO: rewrite this so we set 'block' to an OffsetMemoryBlock?
    Int32 mOffset;
};

} // namespace IO
} // namespace Elastos

#endif // __DIRECTBYTEBUFFER_H__
