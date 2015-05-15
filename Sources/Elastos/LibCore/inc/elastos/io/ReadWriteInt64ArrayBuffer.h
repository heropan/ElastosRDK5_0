#ifndef __READWRITEINT64ARRAYBUFFER_H__
#define __READWRITEINT64ARRAYBUFFER_H__

#include "Int64ArrayBuffer.h"

namespace Elastos {
namespace IO {

/**
 * LongArrayBuffer, ReadWriteLongArrayBuffer and ReadOnlyLongArrayBuffer compose
 * the implementation of array based long buffers.
 * <p>
 * ReadWriteLongArrayBuffer extends LongArrayBuffer with all the write methods.
 * </p>
 * <p>
 * This class is marked final for runtime performance.
 * </p>
 *
 */
class ReadWriteInt64ArrayBuffer
    : public IInt64Buffer
    , public Int64ArrayBuffer
{
public:
    ReadWriteInt64ArrayBuffer(
        /* [in] */ ArrayOf<Int64>* array);

    ReadWriteInt64ArrayBuffer(
        /* [in] */ Int32 capacity);

    ReadWriteInt64ArrayBuffer(
        /* [in] */ Int32 capacity,
        /* [in] */ ArrayOf<Int64>* backingArray,
        /* [in] */ Int32 arrayOffset);

    static CARAPI_(AutoPtr<ReadWriteInt64ArrayBuffer>) Copy(
        /* [in] */ Int64ArrayBuffer* other,
        /* [in] */ Int32 markOfOther);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetPrimitiveArray(
        /* [out] */ Handle32* arrayHandle);

    CARAPI GetArray(
        /* [out, callee] */ ArrayOf<Int64>** array);

    CARAPI GetElementSizeShift(
        /* [out] */ Int32* elementSizeShift);

    CARAPI GetEffectiveDirectAddress(
        /* [out] */ Int32* effectiveDirectAddress);

    CARAPI GetArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IInt64Buffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ IInt64Buffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ IInt64Buffer** buffer);

    CARAPI GetInt64(
        /* [out] */ Int64* value);

    CARAPI GetInt64Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int64* value);

    CARAPI GetInt64s(
        /* [out] */ ArrayOf<Int64>* dst);

    CARAPI GetInt64sEx(
        /* [out] */ ArrayOf<Int64>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 int64Count);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI PutInt64(
        /* [in] */ Int64 d);

    CARAPI PutInt64Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int64 d);

    CARAPI PutInt64s(
        /* [in] */ const ArrayOf<Int64>& src);

    CARAPI PutInt64sEx(
        /* [in] */ const ArrayOf<Int64>& src,
        /* [in] */ Int32 srcOffset,
        /* [in] */ Int32 int64Count);

    CARAPI PutInt64Buffer(
        /* [in] */ IInt64Buffer* src);

    CARAPI Slice(
        /* [out] */ IInt64Buffer** buffer);

    CARAPI GetCapacity(
        /* [out] */ Int32* cap);

    CARAPI Clear();

    CARAPI Flip();

    CARAPI HasArray(
        /* [out] */ Boolean* hasArray);

    CARAPI HasRemaining(
        /* [out] */ Boolean* hasRemaining);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI IsReadOnly(
        /* [out] */ Boolean* isReadOnly);

    CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Int64>** array);

    CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI ProtectedHasArray(
        /* [out] */ Boolean* result);

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

    CARAPI ToString(
        /* [out] */ String* str);

private:
    AutoPtr<ArrayOf<Int64> > mArrayTemp;
};

} // namespace IO
} // namespace Elastos

#endif // __READWRITEINT64ARRAYBUFFER_H__
