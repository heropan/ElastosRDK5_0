#ifndef __ELASTOS_IO_INT64ARRAYBUFFER_H__
#define __ELASTOS_IO_INT64ARRAYBUFFER_H__

#include "Int64Buffer.h"

namespace Elastos {
namespace IO {

/**
 * LongArrayBuffer, ReadWriteLongArrayBuffer and ReadOnlyLongArrayBuffer compose
 * the implementation of array based long buffers.
 * <p>
 * LongArrayBuffer implements all the shared readonly methods and is extended by
 * the other two classes.
 * </p>
 * <p>
 * All methods are marked final for runtime performance.
 * </p>
 *
 */
class Int64ArrayBuffer
    : public Int64Buffer
{
public:
    Int64ArrayBuffer(
        /* [in] */ ArrayOf<Int64>* array);

    Int64ArrayBuffer(
        /* [in] */ Int32 capacity,
        /* [in] */ ArrayOf<Int64>* backingArray,
        /* [in] */ Int32 offset,
        /* [in] */ Boolean isReadOnly);

    CARAPI GetInt64(
        /* [out] */ Int64* value);

    CARAPI GetInt64(
        /* [in] */ Int32 index,
        /* [out] */ Int64* value);

    CARAPI GetInt64s(
        /* [out] */ ArrayOf<Int64>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 int64Count);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI GetOrder(
       /* [out] */ ByteOrder* byteOrder);

public:
    AutoPtr< ArrayOf<Int64> > mBackingArray;

    Int32 mOffset;

    Boolean mIsReadOnly;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_INT64ARRAYBUFFER_H__
