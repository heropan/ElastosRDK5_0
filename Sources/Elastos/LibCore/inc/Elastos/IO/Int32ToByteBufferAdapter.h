#ifndef __INT32TOBYTEBUFFERADAPTER_H__
#define __INT32TOBYTEBUFFERADAPTER_H__

#include "Int32Buffer.h"

namespace Elastos {
namespace IO {

/**
 * This class wraps a byte buffer to be a int buffer.
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
class Int32ToByteBufferAdapter
    : public IInt32Buffer
    , public Int32Buffer
{
public:
    static CARAPI AsInt32Buffer(
        /* [in] */ IByteBuffer* byteBuffer,
        /* [out] */ IInt32Buffer** int32Buffer);

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
        /* [out, callee] */ ArrayOf<Int32>** array);

    CARAPI GetElementSizeShift(
        /* [out] */ Int32* elementSizeShift);

    CARAPI GetEffectiveDirectAddress(
        /* [out] */ Int32* effectiveDirectAddress);

    CARAPI GetArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IInt32Buffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ IInt32Buffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ IInt32Buffer** buffer);

    CARAPI GetInt32(
        /* [out] */ Int32* value);

    CARAPI GetInt32Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int32* value);

    CARAPI GetInt32s(
        /* [out] */ ArrayOf<Int32>* dst);

    CARAPI GetInt32sEx(
        /* [out] */ ArrayOf<Int32>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 int32Count);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI PutInt32(
        /* [in] */ Int32 d);

    CARAPI PutInt32Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int32 d);

    CARAPI PutInt32s(
        /* [in] */ const ArrayOf<Int32>& src);

    CARAPI PutInt32sEx(
        /* [in] */ const ArrayOf<Int32>& src,
        /* [in] */ Int32 srcOffset,
        /* [in] */ Int32 int32Count);

    CARAPI PutInt32Buffer(
        /* [in] */ IInt32Buffer* src);

    CARAPI Slice(
        /* [out] */ IInt32Buffer** buffer);

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
        /* [out, callee] */ ArrayOf<Int32>** array);

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
    Int32ToByteBufferAdapter(
        /* [in] */ IByteBuffer* byteBuffer);

private:
    AutoPtr<IByteBuffer> mByteBuffer;
    AutoPtr<ArrayOf<Int32> > mArrayTemp;
};

} // namespace IO
} // namespace Elastos

#endif // __INT32TOBYTEBUFFERADAPTER_H__
