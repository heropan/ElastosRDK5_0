#ifndef __INT16ARRAYBUFFER_H__
#define __INT16ARRAYBUFFER_H__

#include "Int16Buffer.h"

namespace Elastos {
namespace IO {

/**
 * ShortArrayBuffer, ReadWriteShortArrayBuffer and ReadOnlyShortArrayBuffer
 * compose the implementation of array based short buffers.
 * <p>
 * ShortArrayBuffer implements all the shared readonly methods and is extended
 * by the other two classes.
 * </p>
 * <p>
 * All methods are marked final for runtime performance.
 * </p>
 *
 */
class Int16ArrayBuffer
    : public Int16Buffer
{
public:
    Int16ArrayBuffer(
        /* [in] */ ArrayOf<Int16>* array);

    Int16ArrayBuffer(
        /* [in] */ Int32 capacity);

    Int16ArrayBuffer(
        /* [in] */ Int32 capacity,
        /* [in] */ ArrayOf<Int16>* backingArray,
        /* [in] */ Int32 offset);

    CARAPI GetInt16(
        /* [out] */ Int16* value);

    CARAPI GetInt16Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int16* value);

    CARAPI GetInt16sEx(
        /* [out] */ ArrayOf<Int16>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 int16Count);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

public:
    AutoPtr< ArrayOf<Int16> > mBackingArray;

    Int32 mOffset;
};

} // namespace IO
} // namespace Elastos

#endif // __INT16ARRAYBUFFER_H__
