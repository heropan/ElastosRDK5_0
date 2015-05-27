#ifndef __CHARSEQUENCEADAPTER_H__
#define __CHARSEQUENCEADAPTER_H__

#include "CharBuffer.h"
#include <elastos.h>
#include <Elastos.CoreLibrary_server.h>

namespace Elastos {
namespace IO {

/**
 * This class wraps a char sequence to be a char buffer.
 * <p>
 * Implementation notice:
 * <ul>
 * <li>Char sequence based buffer is always readonly.</li>
 * </ul>
 * </p>
 *
 */
class CharSequenceAdapter
    : public ICharBuffer
    , public ICharSequence
    , public CharBuffer
{
public:
    CharSequenceAdapter(
        /* [in] */ Int32 capacity,
        /* [in] */ ICharSequence* chseq);

    static CARAPI_(AutoPtr<CharSequenceAdapter>) Copy(
        /* [in] */ CharSequenceAdapter* other);

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

    CARAPI GetChar(
        /* [in] */ Int32 index,
        /* [out] */ Char32* value);

    CARAPI GetChars(
        /* [out] */ ArrayOf<Char32>* dst);

    CARAPI GetChars(
        /* [out] */ ArrayOf<Char32>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 charCount);

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

    CARAPI PutChar(
        /* [in] */ Int32 index,
        /* [in] */ Char32 c);

    CARAPI PutChars(
        /* [in] */ const ArrayOf<Char32>& src);

    CARAPI PutChars(
        /* [in] */ const ArrayOf<Char32>& src,
        /* [in] */ Int32 srcOffset,
        /* [in] */ Int32 charCount);

    CARAPI PutCharBuffer(
        /* [in] */ ICharBuffer* src);

    CARAPI PutString(
        /* [in] */ const String& str);

    CARAPI PutString(
        /* [in] */ const String& str,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI Slice(
        /* [out] */ ICharBuffer** buffer);

    CARAPI AppendChar(
        /* [in] */ Char32 c);

    CARAPI AppendChars(
        /* [in] */ ICharSequence* csq);

    CARAPI AppendChars(
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
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* rst);

public:
    AutoPtr<ICharSequence> mSequence;

private:
    AutoPtr<ArrayOf<Char32> > mArrayTemp;
};

} // namespace IO
} // namespace Elastos

#endif // __CHARSEQUENCEADAPTER_H__
