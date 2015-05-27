#ifndef __DOUBLEARRAYBUFFER_H__
#define __DOUBLEARRAYBUFFER_H__

#include "DoubleBuffer.h"

namespace Elastos {
namespace IO {

/**
 * DoubleArrayBuffer, ReadWriteDoubleArrayBuffer and ReadOnlyDoubleArrayBuffer
 * compose the implementation of array based double buffers.
 * <p>
 * DoubleArrayBuffer implements all the shared readonly methods and is extended
 * by the other two classes.
 * </p>
 * <p>
 * All methods are marked final for runtime performance.
 * </p>
 *
 */
class DoubleArrayBuffer
    : public DoubleBuffer
{
public:
    DoubleArrayBuffer(
        /* [in] */ ArrayOf<Double>* array);

    DoubleArrayBuffer(
        /* [in] */ Int32 capacity);

    DoubleArrayBuffer(
        /* [in] */ Int32 capacity,
        /* [in] */ ArrayOf<Double>* backingArray,
        /* [in] */ Int32 offset);

    CARAPI GetDouble(
        /* [out] */ Double* value);

    CARAPI GetDouble(
        /* [in] */ Int32 index,
        /* [out] */ Double* value);

    CARAPI GetDoubles(
        /* [out] */ ArrayOf<Double>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 doubleCount);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

public:
    AutoPtr< ArrayOf<Double> > mBackingArray;

    Int32 mOffset;
};

} // namespace IO
} // namespace Elastos

#endif // __DOUBLEARRAYBUFFER_H__
