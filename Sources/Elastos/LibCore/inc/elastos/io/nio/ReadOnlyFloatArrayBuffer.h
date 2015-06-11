#ifndef __ELASTOS_IO_READONLYFLOATARRAYBUFFER_H__
#define __ELASTOS_IO_READONLYFLOATARRAYBUFFER_H__

#include "FloatArrayBuffer.h"

namespace Elastos {
namespace IO {

/**
 * FloatArrayBuffer, ReadWriteFloatArrayBuffer and ReadOnlyFloatArrayBuffer
 * compose the implementation of array based float buffers.
 * <p>
 * ReadOnlyFloatArrayBuffer extends FloatArrayBuffer with all the write methods
 * throwing read only exception.
 * </p>
 * <p>
 * This class is marked final for runtime performance.
 * </p>
 *
 */
class ReadOnlyFloatArrayBuffer
    : public IFloatBuffer
    , public FloatArrayBuffer
{
public:
    static CARAPI_(AutoPtr<ReadOnlyFloatArrayBuffer>) Copy(
        /* [in] */ FloatArrayBuffer* other,
        /* [in] */ Int32 markOfOther);

    ReadOnlyFloatArrayBuffer(
        /* [in] */ Int32 capacity,
        /* [in] */ ArrayOf<Float>* backingArray,
        /* [in] */ Int32 arrayOffset);

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
        /* [out, callee] */ ArrayOf<Float>** array);

    CARAPI GetElementSizeShift(
        /* [out] */ Int32* elementSizeShift);

    CARAPI GetEffectiveDirectAddress(
        /* [out] */ Int32* effectiveDirectAddress);

    CARAPI GetArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IFloatBuffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ IFloatBuffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ IFloatBuffer** buffer);

    CARAPI GetFloat(
        /* [out] */ Float* value);

    CARAPI GetFloat(
        /* [in] */ Int32 index,
        /* [out] */ Float* value);

    CARAPI GetFloats(
        /* [out] */ ArrayOf<Float>* dst);

    CARAPI GetFloats(
        /* [out] */ ArrayOf<Float>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 floatCount);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI PutFloat(
        /* [in] */ Float d);

    CARAPI PutFloat(
        /* [in] */ Int32 index,
        /* [in] */ Float d);

    CARAPI PutFloats(
        /* [in] */ const ArrayOf<Float>& src);

    CARAPI PutFloats(
        /* [in] */ const ArrayOf<Float>& src,
        /* [in] */ Int32 srcOffset,
        /* [in] */ Int32 floatCount);

    CARAPI PutFloatBuffer(
        /* [in] */ IFloatBuffer* src);

    CARAPI Slice(
        /* [out] */ IFloatBuffer** buffer);

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
        /* [out, callee] */ ArrayOf<Float>** array);

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
    AutoPtr<ArrayOf<Float> > mArrayTemp;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_READONLYFLOATARRAYBUFFER_H__
