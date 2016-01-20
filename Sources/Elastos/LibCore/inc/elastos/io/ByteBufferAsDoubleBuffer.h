
#ifndef __ELASTOS_IO_BYTEBUFFERASDOUBLEBUFFER_H__
#define __ELASTOS_IO_BYTEBUFFERASDOUBLEBUFFER_H__

#include "DoubleBuffer.h"
#include "ByteBuffer.h"

namespace Elastos {
namespace IO {


/**
 * A buffer for bytes.
 * <p>
 * A byte buffer can be created in either one of the following ways:
 * <ul>
 * <li>{@link #allocate(int) Allocate} a new byte array and create a buffer
 * based on it;</li>
 * <li>{@link #allocateDirect(int) Allocate} a memory block and create a direct
 * buffer based on it;</li>
 * <li>{@link #wrap(byte[]) Wrap} an existing byte array to create a new
 * buffer.</li>
 * </ul>
 *
 */
class ByteBufferAsDoubleBuffer
    : public DoubleBuffer
{
private:
    ByteBufferAsDoubleBuffer();

    CARAPI constructor(
        /* [in] */ ByteBuffer* byteBuffer);

public:
    static CARAPI_(AutoPtr<IDoubleBuffer>) AsDoubleBuffer(
        /* [in] */ ByteBuffer* byteBuffer);

    CARAPI GetPrimitiveArray(
        /* [out] */ Handle64* arrayHandle);

    // @Override
    CARAPI AsReadOnlyBuffer(
        /* [out] */ IDoubleBuffer** buffer);

    // @Override
    CARAPI Compact();

    // @Override
    CARAPI Duplicate(
        /* [out] */ IDoubleBuffer** buffer);

    // @Override
    CARAPI Get(
        /* [out] */ Double* value);

    // @Override
    CARAPI Get(
        /* [in] */ Int32 index,
        /* [out] */ Double* value);

    // @Override
    CARAPI Get(
        /* [out] */ ArrayOf<Double>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 charCount);

    // @Override
    CARAPI IsDirect(
        /* [out] */ Boolean* rst);

    // @Override
    CARAPI IsReadOnly(
        /* [out] */ Boolean* rst);

    // @Override
    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI Put(
        /* [in] */ Double c);

    CARAPI Put(
        /* [in] */ Int32 index,
        /* [in] */ Double c);

    CARAPI Put(
        /* [in] */ ArrayOf<Double>* src,
        /* [in] */ Int32 srcOffset,
        /* [in] */ Int32 charCount);

    CARAPI Slice(
        /* [out] */ IDoubleBuffer** buffer);

protected:
    CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Double>** array);

    /**
     * Child class implements this method to realize {@code arrayOffset()}.
     *
     * @see #arrayOffset()
     */
    CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset);

    /**
     * Child class implements this method to realize {@code hasArray()}.
     *
     * @see #hasArray()
     */
    CARAPI ProtectedHasArray(
        /* [out] */ Boolean* result);

private:
    AutoPtr<ByteBuffer> mByteBuffer;
    Int32 mCap;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_BYTEBUFFERASDOUBLEBUFFER_H__
