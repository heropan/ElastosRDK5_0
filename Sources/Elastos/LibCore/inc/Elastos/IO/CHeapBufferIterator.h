#ifndef __IO_HEAP_BUFFER_ITERATOR_H__
#define __IO_HEAP_BUFFER_ITERATOR_H__

#include "_CHeapBufferIterator.h"

namespace Elastos {
namespace IO {

CarClass(CHeapBufferIterator)
{
public:
    CARAPI constructor(
        /* [in/out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount,
        /* [in] */ ByteOrder order);

    /**
     * Seeks to the absolute position {@code offset}, measured in bytes from the start.
     */
    CARAPI Seek(
        /* [in] */ Int32 offset);

    /**
     * Skips forwards or backwards {@code byteCount} bytes from the current position.
     */
    CARAPI Skip(
        /* [in] */ Int32 byteCount);

    /**
     * Copies {@code byteCount} bytes from the current position into {@code dst}, starting at
     * {@code dstOffset}, and advances the current position {@code byteCount} bytes.
     */
    CARAPI ReadByteArray(
        /* [out] */ ArrayOf<Byte>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 byteCount);

    /**
     * Returns the byte at the current position, and advances the current position one byte.
     */
    CARAPI ReadByte(
        /* [out] */ Byte* result);

    /**
     * Returns the 32-bit int at the current position, and advances the current position four bytes.
     */
    CARAPI ReadInt32(
        /* [out] */ Int32* result);

    /**
     * Copies {@code intCount} 32-bit ints from the current position into {@code dst}, starting at
     * {@code dstOffset}, and advances the current position {@code 4 * intCount} bytes.
     */
    CARAPI ReadInt32Array(
        /* [out] */ ArrayOf<Int32>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 intCount);

    /**
     * Returns the 16-bit short at the current position, and advances the current position two bytes.
     */
    CARAPI ReadInt16(
        /* [out] */ Int16* result);

private:
    AutoPtr< ArrayOf<Byte> > mBuffer;
    Int32 mOffset;
    Int32 mByteCount;
    ByteOrder mOrder;
    Int32 mPosition;
};

} // namespace IO
} // namespace Elastos

#endif //__IO_HEAP_BUFFER_ITERATOR_H__
