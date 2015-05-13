#ifndef __READONLYCHARARRAYBUFFER_H__
#define __READONLYCHARARRAYBUFFER_H__

#include "CharArrayBuffer.h"

namespace Elastos {
namespace IO {

/**
 * CharArrayBuffer, ReadWriteCharArrayBuffer and ReadOnlyCharArrayBuffer compose
 * the implementation of array based char buffers.
 * <p>
 * ReadOnlyCharArrayBuffer extends CharArrayBuffer with all the write methods
 * throwing read only exception.
 * </p>
 * <p>
 * This class is marked final for runtime performance.
 * </p>
 *
 */
class ReadOnlyCharArrayBuffer
    : public ICharBuffer
    , public ICharSequence
    , public CharArrayBuffer
{
public:
    ReadOnlyCharArrayBuffer(
        /* [in] */ Int32 capacity,
        /* [in] */ ArrayOf<Char32>* backingArray,
        /* [in] */ Int32 arrayOffset);

    static CARAPI_(AutoPtr<ReadOnlyCharArrayBuffer>) Copy(
        /* [in] */ CharArrayBuffer* other,
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
        /* [out, callee] */ ArrayOf<Char32>** array);

    CARAPI GetElementSizeShift(
        /* [out] */ Int32* elementSizeShift);

    CARAPI GetEffectiveDirectAddress(
        /* [out] */ Int32* effectiveDirectAddress);

    CARAPI GetArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ ICharBuffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ ICharBuffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ ICharBuffer** buffer);

    CARAPI GetChar(
        /* [out] */ Char32* value);

    CARAPI GetCharEx(
        /* [in] */ Int32 index,
        /* [out] */ Char32* value);

    CARAPI GetChars(
        /* [out] */ ArrayOf<Char32>* dst);

    CARAPI GetCharsEx(
        /* [out] */ ArrayOf<Char32>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 byteCount);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* order);

    CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Char32>** array);

    CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI ProtectedHasArray(
        /* [out] */ Boolean* result);

    CARAPI PutChar(
        /* [in] */ Char32 c);

    CARAPI PutCharEx(
        /* [in] */ Int32 index,
        /* [in] */ Char32 c);

    CARAPI PutChars(
        /* [in] */ const ArrayOf<Char32>& src);

    CARAPI PutCharsEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Char32>& src);

    CARAPI PutCharBuffer(
        /* [in] */ ICharBuffer* src);

    CARAPI PutString(
        /* [in] */ const String& str);

    CARAPI PutStringEx(
        /* [in] */ const String& str,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI Slice(
        /* [out] */ ICharBuffer** buffer);

    CARAPI AppendChar(
        /* [in] */ Char32 c);

    CARAPI AppendChars(
        /* [in] */ ICharSequence* csq);

    CARAPI AppendCharsEx(
        /* [in] */ ICharSequence* csq,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI Read(
        /* [in] */ ICharBuffer* target,
        /* [out] */ Int32* number);

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

    CARAPI GetLength(
        /* [out] */ Int32* number);

    CARAPI GetCharAt(
        /* [in] */ Int32 index,
        /* [out] */ Char32* c);

    CARAPI SubSequence(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ICharSequence** csq);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* rst);

    CARAPI PutCharsEx(
        /* [in] */ const ArrayOf<Char32>& src,
        /* [in] */ Int32 off,
        /* [in] */ Int32 len);

private:
    AutoPtr<ArrayOf<Char32> > mArrayTemp;
};

} // namespace IO
} // namespace Elastos

#endif // __READONLYCHARARRAYBUFFER_H__
