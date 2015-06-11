#ifndef __ELASTOS_IO_INT32ARRAYBUFFER_H__
#define __ELASTOS_IO_INT32ARRAYBUFFER_H__

#include "Int32Buffer.h"

namespace Elastos {
namespace IO {

/**
 * IntArrayBuffer, ReadWriteIntArrayBuffer and ReadOnlyIntArrayBuffer compose
 * the implementation of array based int buffers.
 * <p>
 * IntArrayBuffer implements all the shared readonly methods and is extended by
 * the other two classes.
 * </p>
 * <p>
 * All methods are marked final for runtime performance.
 * </p>
 *
 */
class Int32ArrayBuffer
    : public Int32Buffer
{
public:
    Int32ArrayBuffer(
        /* [in] */ ArrayOf<Int32>* array);

    Int32ArrayBuffer(
        /* [in] */ Int32 capacity);

    Int32ArrayBuffer(
        /* [in] */ Int32 capacity,
        /* [in] */ ArrayOf<Int32>* backingArray,
        /* [in] */ Int32 offset);

    CARAPI GetInt32(
        /* [out] */ Int32* value);

    CARAPI GetInt32(
        /* [in] */ Int32 index,
        /* [out] */ Int32* value);

    CARAPI GetInt32s(
        /* [out] */ ArrayOf<Int32>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 int32Count);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

public:
    AutoPtr< ArrayOf<Int32> > mBackingArray;

    Int32 mOffset;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_INT32ARRAYBUFFER_H__
