#ifndef __ELASTOS_IO_MAPPEDBYTEBUFFERADAPTER_H__
#define __ELASTOS_IO_MAPPEDBYTEBUFFERADAPTER_H__

#include "MappedByteBuffer.h"

using Elastos::IO::Channels::FileChannelMapMode;

namespace Elastos {
namespace IO {

/**
 * Rather than duplicate all the code from ReadOnlyDirectByteBuffer and
 * ReadWriteDirectByteBuffer (and their superclasses), we delegate to one or the other.
 * The tricky part is that we need to keep our fields in sync with our delegate's fields.
 * There are lots of methods that access the fields directly.
 *
 * The main consequences of this implementation are:
 *
 * 1. we need to explicitly call wrapped.position(int) before any operation on our delegate
 * that makes use of the implicit position.
 *
 * 2. we need to explicitly update position after any operation on our delegate that makes
 * use of the implicit position.
 *
 * This means that, even more than usual, the implicit iteration
 * operations are more expensive than the indexed operations.
 *
 * But we save a ton of code, for classes that no-one really uses because the API's broken
 * by design (disallowing munmap(2) calls). Internally, we can use libcore.io.MemoryMappedFile
 * as a high-performance and more usable replacement for MappedByteBuffer.
 *
 * FIXME: harmony changed their implementation after we diverged, switching to a scheme
 * where DirectByteBuffer extends MappedByteBuffer and this class doesn't exist. That's
 * much better than their original implementation, fossilized here.
 */
class MappedByteBufferAdapter
    : public IMappedByteBuffer
    , public MappedByteBuffer
{
public:
    MappedByteBufferAdapter(
        /* [in] */ MemoryBlock* block,
        /* [in] */ Int32 capa,
        /* [in] */ Int32 offset,
        /* [in] */ FileChannelMapMode mode);

    CAR_INTERFACE_DECL()

    CARAPI IsLoaded(
        /* [out] */ Boolean* isLoaded);

    CARAPI Load();

    CARAPI Force();

    CARAPI SetLimitImpl(
        /* [in] */ Int32 newLimit);

    CARAPI SetPositionImpl(
        /* [in] */ Int32 newPosition);

    CARAPI AsCharBuffer(
        /* [out] */ ICharBuffer** buffer);

    CARAPI AsDoubleBuffer(
        /* [out] */ IDoubleBuffer** buffer);

    CARAPI AsFloatBuffer(
        /* [out] */ IFloatBuffer** buffer);

    CARAPI AsInt32Buffer(
        /* [out] */ IInt32Buffer** buffer);

    CARAPI AsInt64Buffer(
        /* [out] */ IInt64Buffer** buffer);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IByteBuffer** buffer);

    CARAPI AsInt16Buffer(
        /* [out] */ IInt16Buffer** buffer);

    CARAPI Compact();

    CARAPI Duplicate(
        /* [out] */ IByteBuffer** buffer);

    CARAPI Get(
        /* [out] */ Byte* value);

    CARAPI Get(
        /* [in] */ Int32 index,
        /* [out] */ Byte* value);

    CARAPI Get(
        /* [out] */ ArrayOf<Byte>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 byteCount);

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

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI IsReadOnly(
        /* [out] */ Boolean* isReadOnly);

    CARAPI SetOrderImpl(
        /* [in] */ ByteOrder byteOrder);

    CARAPI Put(
        /* [in] */ Byte b);

    CARAPI Put(
        /* [in] */ Int32 index,
        /* [in] */ Byte b);

    CARAPI Put(
        /* [in] */ ArrayOf<Byte>* src,
        /* [in] */ Int32 srcOffset,
        /* [in] */ Int32 byteCount);

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

    CARAPI PutInt16(
        /* [in] */ Int16 value);

    CARAPI PutInt16(
        /* [in] */ Int32 index,
        /* [in] */ Int16 value);

    CARAPI Slice(
       /* [out] */ IByteBuffer** buffer);

    CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Byte>** array);

    CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI ProtectedHasArray(
        /* [out] */ Boolean* hasArray);

    CARAPI_(void) Free();

    CARAPI GetArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI GetCapacity(
        /* [out] */ Int32* cap);

    CARAPI Clear();

    CARAPI Flip();

    CARAPI HasArray(
        /* [out] */ Boolean* hasArray);

    CARAPI HasRemaining(
        /* [out] */ Boolean* hasRemaining);

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

    CARAPI GetPrimitiveArray(
        /* [out] */ Handle32* arrayHandle);

    CARAPI GetArray(
        /* [out, callee] */ ArrayOf<Byte>** array);

    CARAPI GetElementSizeShift(
        /* [out] */ Int32* elementSizeShift);

    CARAPI GetEffectiveDirectAddress(
        /* [out] */ Int32* effectiveDirectAddress);

    CARAPI CompareTo(
        /* [in] */ IByteBuffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Get(
        /* [out] */ ArrayOf<Byte>* dst);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI SetOrder(
        /* [in] */ ByteOrder byteOrder);

    CARAPI Put(
        /* [in] */ ArrayOf<Byte>* src);

    CARAPI Put(
        /* [in] */ IByteBuffer* src);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    MappedByteBufferAdapter(
        /* [in] */ IByteBuffer* buffer);

private:
    AutoPtr<ArrayOf<Byte> > mArrayTemp;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_MAPPEDBYTEBUFFERADAPTER_H__
