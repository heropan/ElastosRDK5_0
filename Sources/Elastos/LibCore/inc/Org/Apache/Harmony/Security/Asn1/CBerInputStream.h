
#ifndef __CBERINPUTSTREAM_H__
#define __CBERINPUTSTREAM_H__

#include "_CBerInputStream.h"
#include <BerInputStream.h>

using Elastos::IO::IInputStream;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CBerInputStream), public BerInputStream
{
public:
    static CARAPI GetLength(
        /* [in] */ ArrayOf<Byte>* encoding,
        /* [out] */ Int32* length);

    CARAPI SetTag(
        /* [in] */ Int32 tag);

    CARAPI GetTag(
        /* [out] */ Int32* tag);

    CARAPI SetLength(
        /* [in] */ Int32 length);

    CARAPI SetContent(
        /* [in] */ IInterface* content);

    CARAPI GetContent(
        /* [out] */ IInterface** content);

    CARAPI GetTagOffSet(
        /* [out] */ Int32* tagOffset);

    CARAPI SetTagOffSet(
        /* [in] */ Int32 tagOffset);

    CARAPI GetContentOffset(
        /* [out] */ Int32* contentOffset);

    CARAPI SetContentOffset(
        /* [in] */ Int32 contentOffset);

    CARAPI Reset(
        /* [in] */ ArrayOf<Byte>* encoded);

    CARAPI Next(
        /* [out] */ Int32* next);

    CARAPI ReadBitString();

    CARAPI ReadEnumerated();

    CARAPI ReadBoolean();

    CARAPI SetChoiceIndex(
        /* [in] */ Int32 choiceIndex);

    CARAPI GetChoiceIndex(
        /* [out] */ Int32* choiceIndex);

    CARAPI SetTimes(
        /* [in] */ ArrayOf<Int32>* times);

    CARAPI GetTimes(
        /* [out, callee] */ ArrayOf<Int32>** times);

    CARAPI ReadGeneralizedTime();

    CARAPI ReadUTCTime();

    CARAPI ReadInteger();

    CARAPI ReadOctetString();

    CARAPI SetOidElement(
        /* [in] */ Int32 oidElement);

    CARAPI GetOidElement(
        /* [out] */ Int32* oidElement);

    CARAPI ReadOID();

    CARAPI ReadSequence(
        /* [out] */ IASN1Sequence** sequence);

    CARAPI ReadSequenceOf(
        /* [in] */ IASN1SequenceOf* sequenceOf);

    CARAPI ReadSet(
        /* [in] */ IASN1Set* set);

    CARAPI ReadSetOf(
        /* [in] */ IASN1SetOf* setOf);

    CARAPI ReadString(
        /* [in] */ IASN1Type* type);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte>** encoded);

    CARAPI GetBuffer(
        /* [out, callee] */ ArrayOf<Byte>** buffer);

    CARAPI GetLength(
        /* [out] */ Int32* length);

    CARAPI GetOffset(
        /* [out] */ Int32* offset);

    CARAPI GetEndOffset(
        /* [out] */ Int32* endOffset);

    CARAPI GetTagOffset(
        /* [out] */ Int32* tagOffset);

    CARAPI SetVerify();

    CARAPI ReadContent();

    CARAPI CompactBuffer();

    CARAPI Put(
        /* [in] */ IInterface* key,
        /* [in] */ IInterface* entry);

    CARAPI Get(
        /* [in] */ IInterface* key,
        /* [out] */ IInterface** entry);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* encoded);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* encoded,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 expectedLength);

    CARAPI constructor(
        /* [in] */ IInputStream* is);

    CARAPI constructor(
        /* [in] */ IInputStream* is,
        /* [in] */ Int32 initialSize);
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CBERINPUTSTREAM_H__
