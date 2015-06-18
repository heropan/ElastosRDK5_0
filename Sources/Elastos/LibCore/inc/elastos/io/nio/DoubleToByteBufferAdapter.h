#ifndef __ELASTOS_IO_DOUBLETOBYTEBUFFERADAPTER_H__
#define __ELASTOS_IO_DOUBLETOBYTEBUFFERADAPTER_H__

#include "DoubleBuffer.h"

namespace Elastos {
namespace IO {

/**
 * This class wraps a byte buffer to be a double buffer.
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
class DoubleToByteBufferAdapter : public DoubleBuffer
{
public:
    static CARAPI AsDoubleBuffer(
        /* [in] */ IByteBuffer* byteBuffer,
        /* [out] */ IDoubleBuffer** doubleBuffer);

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
        /* [out, callee] */ ArrayOf<Double>** array);

    CARAPI GetElementSizeShift(
        /* [out] */ Int32* elementSizeShift);

    CARAPI GetEffectiveDirectAddress(
        /* [out] */ Int32* effectiveDirectAddress);

    CARAPI GetArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IDoubleBuffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ IDoubleBuffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ IDoubleBuffer** buffer);

    CARAPI GetDouble(
        /* [out] */ Double* value);

    CARAPI GetDouble(
        /* [in] */ Int32 index,
        /* [out] */ Double* value);

    CARAPI GetDoubles(
        /* [out] */ ArrayOf<Double>* dst);

    CARAPI GetDoubles(
        /* [out] */ ArrayOf<Double>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 doubleCount);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI PutDouble(
        /* [in] */ Double d);

    CARAPI PutDouble(
        /* [in] */ Int32 index,
        /* [in] */ Double d);

    CARAPI PutDoubles(
        /* [in] */ const ArrayOf<Double>& src);

    CARAPI PutDoubles(
        /* [in] */ const ArrayOf<Double>& src,
        /* [in] */ Int32 srcOffset,
        /* [in] */ Int32 doubleCount);

    CARAPI PutDoubleBuffer(
        /* [in] */ IDoubleBuffer* src);

    CARAPI Slice(
        /* [out] */ IDoubleBuffer** buffer);

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
        /* [out, callee] */ ArrayOf<Double>** array);

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

    virtual CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* rst);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    DoubleToByteBufferAdapter(
        /* [in] */ IByteBuffer* byteBuffer);

private:
    AutoPtr<IByteBuffer> mByteBuffer;
    AutoPtr<ArrayOf<Double> > mArrayTemp;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_DOUBLETOBYTEBUFFERADAPTER_H__
