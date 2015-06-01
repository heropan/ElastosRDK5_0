#ifndef __ELASTOS_IO_INT64TOBYTEBUFFERADAPTER_H__
#define __ELASTOS_IO_INT64TOBYTEBUFFERADAPTER_H__

#include "Int64Buffer.h"

namespace Elastos {
namespace IO {

/**
 * This class wraps a byte buffer to be a long buffer.
 * <p>
 * Implementation notice:
 * <ul>
 * <li>After a byte buffer instance is wrapped, it becomes privately owned by
 * the adapter. It must NOT be accessed outside the adapter any more.</li>
 * <li>The byte buffer's position and limit are NOT linked with the adapter.
 * The adapter extends Buffer, thus has its own position and limit.</li>
 * </ul>
 * </p>
 *
 */
class Int64ToByteBufferAdapter
    : public IInt64Buffer
    , public Int64Buffer
{
public:
    static CARAPI AsInt64Buffer(
        /* [in] */ IByteBuffer* byteBuffer,
        /* [out] */ IInt64Buffer** int64Buffer);

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

    CARAPI GetInt64(
        /* [in] */ Int32 index,
        /* [out] */ Int64* value);

    CARAPI GetInt64s(
        /* [out] */ ArrayOf<Int64>* dst);

    CARAPI GetInt64s(
        /* [out] */ ArrayOf<Int64>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 int64Count);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI PutInt64(
        /* [in] */ Int64 d);

    CARAPI PutInt64(
        /* [in] */ Int32 index,
        /* [in] */ Int64 d);

    CARAPI PutInt64s(
        /* [in] */ const ArrayOf<Int64>& src);

    CARAPI PutInt64s(
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
    Int64ToByteBufferAdapter(
        /* [in] */ IByteBuffer* byteBuffer);

private:
    AutoPtr<IByteBuffer> mByteBuffer;
    AutoPtr<ArrayOf<Int64> > mArrayTemp;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_INT64TOBYTEBUFFERADAPTER_H__
