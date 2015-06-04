#ifndef __ELASTOS_IO_CHARARRAYBUFFER_H__
#define __ELASTOS_IO_CHARARRAYBUFFER_H__

#include "CharBuffer.h"

namespace Elastos {
namespace IO {

/**
 * CharArrayBuffer, ReadWriteCharArrayBuffer and ReadOnlyCharArrayBuffer compose
 * the implementation of array based char buffers.
 * <p>
 * CharArrayBuffer implements all the shared readonly methods and is extended by
 * the other two classes.
 * </p>
 * <p>
 * All methods are marked final for runtime performance.
 * </p>
 *
 */
class CharArrayBuffer
    : public CharBuffer
{
public:
    CharArrayBuffer(
        /* [in] */ ArrayOf<Char32>* array);

    CharArrayBuffer(
        /* [in] */ Int32 capacity);

    CharArrayBuffer(
        /* [in] */ Int32 capacity,
        /* [in] */ ArrayOf<Char32>* backingArray,
        /* [in] */ Int32 offset);

    CARAPI Get(
        /* [out] */ Char32* value);

    CARAPI Get(
        /* [in] */ Int32 index,
        /* [out] */ Char32* value);

    CARAPI Get(
        /* [out] */ ArrayOf<Char32>* dst);

    CARAPI Get(
        /* [out] */ ArrayOf<Char32>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 charCount);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI SubSequence(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ICharSequence** csq);

    CARAPI ToString(
        /* [out] */ String* str);

public:
    AutoPtr< ArrayOf<Char32> > mBackingArray;

    Int32 mOffset;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_CHARARRAYBUFFER_H__
