
#include "CBerInputStream.h"
#include <cmdef.h>

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

ECode CBerInputStream::GetLength(
        /* [in] */ ArrayOf<Byte>* encoding,
        /* [out] */ Int32* ret)
{
    return BerInputStream::GetLength(encoding, ret);
}

ECode CBerInputStream::SetTag(
    /* [in] */ Int32 tag)
{
    return BerInputStream::SetTag(tag);
}

ECode CBerInputStream::GetTag(
    /* [out] */ Int32* tag)
{
    return BerInputStream::GetTag(tag);
}

ECode CBerInputStream::GetLength(
    /* [out] */ Int32* length)
{
    return BerInputStream::GetLength(length);
}

ECode CBerInputStream::SetLength(
    /* [in] */ Int32 length)
{
    return BerInputStream::SetLength(length);
}

ECode CBerInputStream::SetContent(
    /* [in] */ IInterface* content)
{
    return BerInputStream::SetContent(content);
}

ECode CBerInputStream::GetContent(
    /* [out] */ IInterface** content)
{
    return BerInputStream::GetContent(content);
}

ECode CBerInputStream::GetTagOffSet(
    /* [out] */ Int32* tagOffset)
{
    return BerInputStream::GetTagOffset(tagOffset);
}
ECode CBerInputStream::SetTagOffSet(
    /* [in] */ Int32 tagOffset)
{
    return BerInputStream::SetTagOffSet(tagOffset);
}

ECode CBerInputStream::GetContentOffset(
    /* [out] */ Int32* contentOffset)
{
    return BerInputStream::GetContentOffset(contentOffset);
}

ECode CBerInputStream::SetContentOffset(
    /* [in] */ Int32 contentOffset)
{
    return BerInputStream::SetContentOffset(contentOffset);
}

ECode CBerInputStream::Reset(
    /* [in] */ ArrayOf<Byte>* encoded)
{
    return BerInputStream::Reset(encoded);
}

ECode CBerInputStream::Next(
    /* [out] */ Int32* next)
{
    return BerInputStream::Next(next);
}

ECode CBerInputStream::ReadBitString()
{
    return BerInputStream::ReadBitString();
}

ECode CBerInputStream::ReadEnumerated()
{
    return BerInputStream::ReadEnumerated();
}

ECode CBerInputStream::ReadBoolean()
{
    return BerInputStream::ReadBoolean();
}

ECode CBerInputStream::SetChoiceIndex(
    /* [in] */ Int32 choiceIndex)
{
    return BerInputStream::SetChoiceIndex(choiceIndex);
}

ECode CBerInputStream::GetChoiceIndex(
    /* [out] */ Int32* choiceIndex)
{
    return BerInputStream::GetChoiceIndex(choiceIndex);
}

ECode CBerInputStream::SetTimes(
    /* [in] */ ArrayOf<Int32>* times)
{
    return BerInputStream::SetTimes(times);
}

ECode CBerInputStream::GetTimes(
    /* [out, callee] */ ArrayOf<Int32>** times)
{
    return BerInputStream::GetTimes(times);
}

ECode CBerInputStream::ReadGeneralizedTime()
{
    return BerInputStream::ReadGeneralizedTime();
}

ECode CBerInputStream::ReadUTCTime()
{
    return BerInputStream::ReadUTCTime();
}

ECode CBerInputStream::ReadInteger()
{
    return BerInputStream::ReadInteger();
}

ECode CBerInputStream::ReadOctetString()
{
    return BerInputStream::ReadOctetString();
}

ECode CBerInputStream::SetOidElement(
    /* [in] */ Int32 oidElement)
{
    return BerInputStream::SetOidElement(oidElement);
}

ECode CBerInputStream::GetOidElement(
    /* [out] */ Int32* oidElement)
{
    return BerInputStream::GetOidElement(oidElement);
}

ECode CBerInputStream::ReadOID()
{
    return BerInputStream::ReadOID();
}

ECode CBerInputStream::ReadSequence(
    /* [out] */ IASN1Sequence** sequence)
{
    return BerInputStream::ReadSequence(sequence);
}

ECode CBerInputStream::ReadSequenceOf(
    /* [in] */ IASN1SequenceOf* sequenceOf)
{
    return BerInputStream::ReadSequenceOf(sequenceOf);
}

ECode CBerInputStream::ReadSet(
    /* [in] */ IASN1Set* set)
{
    return BerInputStream::ReadSet(set);
}

ECode CBerInputStream::ReadSetOf(
    /* [in] */ IASN1SetOf* setOf)
{
    return BerInputStream::ReadSetOf(setOf);
}

ECode CBerInputStream::ReadString(
    /* [in] */ IASN1StringType* type)
{
    return BerInputStream::ReadString(type);
}

ECode CBerInputStream::GetEncoded(
    /* [out, callee] */ ArrayOf<Byte>** encoded)
{
    return BerInputStream::GetEncoded(encoded);
}

ECode CBerInputStream::GetBuffer(
    /* [out, callee] */ ArrayOf<Byte>** buffer)
{
    return BerInputStream::GetBuffer(buffer);
}

ECode CBerInputStream::GetOffset(
    /* [out] */ Int32* offset)
{
    return BerInputStream::GetOffset(offset);
}

ECode CBerInputStream::GetEndOffset(
    /* [out] */ Int32* endOffset)
{
    return BerInputStream::GetEndOffset(endOffset);
}

ECode CBerInputStream::GetTagOffset(
    /* [out] */ Int32* tagOffset)
{
    return BerInputStream::GetTagOffset(tagOffset);
}

ECode CBerInputStream::SetVerify()
{
    return BerInputStream::SetVerify();
}

ECode CBerInputStream::ReadContent()
{
    return BerInputStream::ReadContent();
}

ECode CBerInputStream::CompactBuffer()
{
    return BerInputStream::CompactBuffer();
}

ECode CBerInputStream::Put(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* entry)
{
    return BerInputStream::Put(key, entry);
}

ECode CBerInputStream::Get(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** entry)
{
    return BerInputStream::Get(key, entry);
}

ECode CBerInputStream::constructor(
    /* [in] */ ArrayOf<Byte>* encoded)
{
    return BerInputStream::constructor(encoded);
}

ECode CBerInputStream::constructor(
    /* [in] */ ArrayOf<Byte>* encoded,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 expectedLength)
{
    return BerInputStream::constructor(encoded, offset, expectedLength);
}

ECode CBerInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return BerInputStream::constructor(is);
}

ECode CBerInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ Int32 initialSize)
{
    return BerInputStream::constructor(is, initialSize);
}

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

