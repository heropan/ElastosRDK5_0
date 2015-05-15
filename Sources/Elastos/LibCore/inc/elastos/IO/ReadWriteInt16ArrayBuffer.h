#ifndef __READWRITEInt16ARRAYBUFFER_H__
#define __READWRITEInt16ARRAYBUFFER_H__

#include "Int16ArrayBuffer.h"

namespace Elastos {
namespace IO {

/**
 * ShortArrayBuffer, ReadWriteShortArrayBuffer and ReadOnlyShortArrayBuffer
 * compose the implementation of array based short buffers.
 * <p>
 * ReadWriteShortArrayBuffer extends ShortArrayBuffer with all the write
 * methods.
 * </p>
 * <p>
 * This class is marked final for runtime performance.
 * </p>
 *
 */
class ReadWriteInt16ArrayBuffer
    : public IInt16Buffer
    , public Int16ArrayBuffer
{
public:
    ReadWriteInt16ArrayBuffer(
        /* [in] */ ArrayOf<Int16>* array);

    ReadWriteInt16ArrayBuffer(
        /* [in] */ Int32 capacity);

    ReadWriteInt16ArrayBuffer(
        /* [in] */ Int32 capacity,
        /* [in] */ ArrayOf<Int16>* backingArray,
        /* [in] */ Int32 arrayOffset);

    static CARAPI_(AutoPtr<ReadWriteInt16ArrayBuffer>) Copy(
        /* [in] */ Int16ArrayBuffer* other,
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
        /* [out, callee] */ ArrayOf<Int16>** array);

    CARAPI GetElementSizeShift(
        /* [out] */ Int32* elementSizeShift);

    CARAPI GetEffectiveDirectAddress(
        /* [out] */ Int32* effectiveDirectAddress);

    CARAPI GetArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IInt16Buffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ IInt16Buffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ IInt16Buffer** buffer);

    CARAPI GetInt16(
        /* [out] */ Int16* value);

    CARAPI GetInt16Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int16* value);

    CARAPI GetInt16s(
        /* [out] */ ArrayOf<Int16>* dst);

    CARAPI GetInt16sEx(
        /* [out] */ ArrayOf<Int16>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 int16Count);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI PutInt16(
        /* [in] */ Int16 d);

    CARAPI PutInt16Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int16 d);

    CARAPI PutInt16s(
        /* [in] */ const ArrayOf<Int16>& src);

    CARAPI PutInt16sEx(
        /* [in] */ const ArrayOf<Int16>& src,
        /* [in] */ Int32 srcOffset,
        /* [in] */ Int32 int16Count);

    CARAPI PutInt16Buffer(
        /* [in] */ IInt16Buffer* src);

    CARAPI Slice(
        /* [out] */ IInt16Buffer** buffer);

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
        /* [out, callee] */ ArrayOf<Int16>** array);

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
    AutoPtr<ArrayOf<Int16> > mArrayTemp;
};

} // namespace IO
} // namespace Elastos

#endif // __READWRITEInt16ARRAYBUFFER_H__
