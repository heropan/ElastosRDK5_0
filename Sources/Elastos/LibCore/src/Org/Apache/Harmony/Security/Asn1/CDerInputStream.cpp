
#include "CDerInputStream.h"
#include <cmdef.h>

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

AutoPtr<ArrayOf<Byte> > CDerInputStream::UNUSED_BITS_MASK = InitStatic();

AutoPtr<ArrayOf<Byte> > CDerInputStream::InitStatic()
{
    Byte[] tmp = { 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
    AutoPtr<ArrayOf<Byte> > ret = ArrayOf<Byte>::Alloc(sizeof(tmp)/sizeof(tmp[0]));
    for (Int32 i = 0; i < sizeof(tmp)/sizeof(tmp[0]); i++) {
        (*ret)[i] = tmp[i];
    }
    return ret;
}

ECode CDerInputStream::SetTag(
    /* [in] */ Int32 tag)
{
    return BerInputStream::SetTag(tag);
}

ECode CDerInputStream::GetTag(
    /* [out] */ Int32* tag)
{
    return BerInputStream::GetTag(tag);
}

ECode CDerInputStream::SetContent(
    /* [in] */ IInterface* content)
{
    return BerInputStream::SetContent(content);
}

ECode CDerInputStream::GetContent(
    /* [out] */ IInterface** content)
{
    return BerInputStream::GetContent(content);
}

ECode CDerInputStream::Reset(
    /* [in] */ ArrayOf<Byte>* encoded)
{
    return BerInputStream::Reset(encoded);
}

ECode CDerInputStream::Next(
    /* [out] */ Int32* next)
{
    VALIDATE_NOT_NULL(next)
    Int32 tag;
    BerInputStream::Next(&tag);

    if (mLength == INDEFINIT_LENGTH) {
        //throw new ASN1Exception("DER: only definite length encoding MUST be used");
        return E_ASN1_EXCEPTION;
    }

    // FIXME add check: length encoding uses minimum number of octets
    *next = tag;
    return NOERROR;
}

ECode CDerInputStream::ReadBitString()
{
    if (mTag == IASN1Constants::TAG_C_BITSTRING) {
        //throw new ASN1Exception("ASN.1 bitstring: constructed identifier at [" + tagOffset
        //        + "]. Not valid for DER.");
        return E_ASN1_EXCEPTION;
    }

    BerInputStream::ReadBitString();

    //check: unused bits values - MUST be 0
    if (mLength > 1
            && (*mBuffer)[mContentOffset] != 0
            && ((*mBuffer)[mOffset - 1] & (*UNUSED_BITS_MASK)[(*mBuffer)[mContentOffset] - 1]) != 0) {
        /*throw new ASN1Exception("ASN.1 bitstring: wrong content at [" + contentOffset
                + "]. DER requires zero unused bits in final octet.");*/
        return E_ASN1_EXCEPTION;
    }
    return NOERROR;
}

ECode CDerInputStream::ReadEnumerated()
{
    return BerInputStream::ReadEnumerated();
}

ECode CDerInputStream::ReadBoolean()
{
    BerInputStream::ReadBoolean();

    // check encoded content
    if ((*mBuffer)[mContentOffset] != 0 && (*mBuffer)[mContentOffset] != (Byte) 0xFF) {
        /*throw new ASN1Exception("ASN.1 boolean: wrong content at [" + contentOffset
                + "]. DER allows only 0x00 or 0xFF values");*/
        return NOERROR;
    }
    return NOERROR;
}

ECode CDerInputStream::SetChoiceIndex(
    /* [in] */ Int32 choiceIndex)
{
    return BerInputStream::SetChoiceIndex(choiceIndex);
}

ECode CDerInputStream::GetChoiceIndex(
    /* [out] */ Int32* choiceIndex)
{
    return BerInputStream::GetChoiceIndex(choiceIndex);
}

ECode CDerInputStream::SetTimes(
    /* [in] */ ArrayOf<Int32>* times)
{
    return BerInputStream::SetTimes(times);
}

ECode CDerInputStream::GetTimes(
    /* [out, callee] */ ArrayOf<Int32>** times)
{
    return BerInputStream::GetTimes(times);
}

ECode CDerInputStream::ReadGeneralizedTime()
{
    if (mTag == IASN1Constants::TAG_C_GENERALIZEDTIME) {
        // It is a string type and it can be encoded as primitive or constructed.
        /*throw new ASN1Exception("ASN.1 GeneralizedTime: constructed identifier at ["
                + tagOffset + "]. Not valid for DER.");*/
        return E_ASN1_EXCEPTION;
    }

    return BerInputStream::ReadGeneralizedTime();
}

ECode CDerInputStream::ReadUTCTime()
{
    if (mTag == IASN1Constants::TAG_C_UTCTIME) {
        // It is a string type and it can be encoded as primitive or constructed.
        /*throw new ASN1Exception("ASN.1 UTCTime: constructed identifier at [" + tagOffset
                + "]. Not valid for DER.");*/
        return E_ASN1_EXCEPTION;
    }

    // check format: DER uses YYMMDDHHMMSS'Z' only
    if (mLength != IASN1UTCTime::UTC_HMS) {
        /*throw new ASN1Exception("ASN.1 UTCTime: wrong format for DER, identifier at ["
                + tagOffset + "]");*/
        return E_ASN1_EXCEPTION;
    }

    return BerInputStream::ReadUTCTime();
}

ECode CDerInputStream::ReadInteger()
{
    return BerInputStream::ReadInteger();
}

ECode CDerInputStream::ReadOctetString()
{
    if (mTag == IASN1Constants::TAG_C_OCTETSTRING) {
        /*throw new ASN1Exception("ASN.1 octetstring: constructed identifier at [" + tagOffset
                + "]. Not valid for DER.");*/
        return E_ASN1_EXCEPTION;
    }
    return BerInputStream::ReadOctetString();
}

ECode CDerInputStream::SetOidElement(
    /* [in] */ Int32 oidElement)
{
    return BerInputStream::SetOidElement(oidElement);
}

ECode CDerInputStream::GetOidElement(
    /* [out] */ Int32* oidElement)
{
    return BerInputStream::GetOidElement(oidElement);
}

ECode CDerInputStream::ReadOID()
{
    return BerInputStream::ReadOID();
}

ECode CDerInputStream::ReadSequence(
    /* [out] */ IASN1Sequence** sequence)
{
    //
    // According to ASN.1 DER spec. sequence MUST not include
    // any encoding which value is equal to its default value
    //
    // Verification of this assertion is not implemented
    //
    return BerInputStream::ReadSequence(sequence);
}

ECode CDerInputStream::ReadSequenceOf(
    /* [in] */ IASN1SequenceOf* sequenceOf)
{
    return BerInputStream::ReadSequenceOf(sequenceOf);
}

ECode CDerInputStream::ReadSet(
    /* [in] */ IASN1Set* set)
{
    return BerInputStream::ReadSet(set);
}

ECode CDerInputStream::ReadSetOf(
    /* [in] */ IASN1SetOf* setOf)
{
    //
    // According to ASN.1 DER spec. set of MUST appear in
    // ascending order (short component are padded for comparison)
    //
    // Verification of this assertion is not implemented
    //
    return BerInputStream::ReadSetOf(setOf);
}

ECode CDerInputStream::ReadString(
    /* [in] */ IASN1StringType* type)
{
    Int32 id;
    type->GetConstrId(&id);
    if (mTag == id) {
        /*
        throw new ASN1Exception("ASN.1 string: constructed identifier at [" + tagOffset
                + "]. Not valid for DER.");*/
        return E_ASN1_EXCEPTION;
    }
    return BerInputStream::ReadString(type);
}

ECode CDerInputStream::GetEncoded(
    /* [out, callee] */ ArrayOf<Byte>** encoded)
{
    return BerInputStream::GetEncoded(encoded);
}

ECode CDerInputStream::GetBuffer(
    /* [out, callee] */ ArrayOf<Byte>** buffer)
{
    return BerInputStream::GetBuffer(buffer);
}

ECode CDerInputStream::GetLength(
    /* [out] */ Int32* length)
{
    return BerInputStream::GetLength(length);
}

ECode CDerInputStream::GetOffset(
    /* [out] */ Int32* offset)
{
    return BerInputStream::GetOffset(offset);
}

ECode CDerInputStream::GetEndOffset(
    /* [out] */ Int32* endOffset)
{
    return BerInputStream::GetEndOffset(endOffset);
}

ECode CDerInputStream::GetTagOffset(
    /* [out] */ Int32* tagOffset)
{
    return BerInputStream::GetTagOffset(tagOffset);
}

ECode CDerInputStream::SetVerify()
{
    return BerInputStream::SetVerify();
}

ECode CDerInputStream::ReadContent()
{
    return BerInputStream::ReadContent();
}

ECode CDerInputStream::CompactBuffer()
{
    return BerInputStream::CompactBuffer();
}

ECode CDerInputStream::Put(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* entry)
{
    return BerInputStream::Put(key, entry);
}

ECode CDerInputStream::Get(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** entry)
{
    return BerInputStream::Get(key, entry);
}

ECode CDerInputStream::constructor(
    /* [in] */ ArrayOf<Byte>* encoded)
{
    return BerInputStream::constructor(encoded, 0, encoded->GetLength());
}

ECode CDerInputStream::constructor(
    /* [in] */ ArrayOf<Byte>* encoded,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 encodingLen)
{
    return BerInputStream::constructor(encoded, offset, encodingLen);
}

ECode CDerInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return BerInputStream::constructor(is);
}

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

