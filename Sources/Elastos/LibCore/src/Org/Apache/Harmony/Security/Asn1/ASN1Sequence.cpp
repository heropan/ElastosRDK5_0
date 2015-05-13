
#include "ASN1Sequence.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {


ECode ASN1Sequence::GetId(
    /* [out] */ Int32* id)
{
    return ASN1TypeCollection::GetId(id);
}

ECode ASN1Sequence::GetConstrId(
    /* [out] */ Int32* constrId)
{
    return ASN1TypeCollection::GetConstrId(constrId);
}

ECode ASN1Sequence::Decode(
    /* [in] */ ArrayOf<Byte>* encoded,
    /* [out] */ IInterface** object)
{
    return ASN1TypeCollection::Decode(encoded, object);
}

ECode ASN1Sequence::DecodeEx(
    /* [in] */ ArrayOf<Byte>* encoded,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 encodingLen,
    /* [out] */ IInterface** object)
{
    return ASN1TypeCollection::DecodeEx(encoded, offset, encodingLen, object);
}

ECode ASN1Sequence::DecodeEx2(
    /* [in] */ IInputStream* is,
    /* [out] */ IInterface** object)
{
    return ASN1TypeCollection::DecodeEx2(is, object);
}

ECode ASN1Sequence::Verify(
    /* [in] */ ArrayOf<Byte>* encoded)
{
    return ASN1TypeCollection::Verify(encoded);
}

ECode ASN1Sequence::VerifyEx(
    /* [in] */ IInputStream* is)
{
    return ASN1TypeCollection::VerifyEx(is);
}

ECode ASN1Sequence::Encode(
    /* [in] */ IInterface* object,
    /* [out, callee] */ ArrayOf<Byte>** encode)
{
    return ASN1TypeCollection::Encode(object, encode);
}

ECode ASN1Sequence::DecodeEx3(
    /* [in] */ IBerInputStream* bis,
    /* [out] */ IInterface** object)
{
    bis->ReadSequence(this);

    if (((CBerInputStream*)bis)->mIsVerify) {
        return NOERROR;
    }
    return GetDecodedObject(bis, object);
}

ECode ASN1Sequence::CheckTag(
    /* [in] */ Int32 identifier,
    /* [out] */ Boolean* checkTag)
{
    return ASN1TypeCollection::CheckTag(identifier, checkTag);
}

ECode ASN1Sequence::GetDecodedObject(
    /* [in] */ IBerInputStream* bis,
    /* [out] */ IInterface** object)
{
    return ASN1TypeCollection::GetDecodedObject(bis, object);
}

ECode ASN1Sequence::EncodeASN(
    /* [in] */ IBerOutputStream* bos)
{
    return ASN1TypeCollection::EncodeASN(bos);
}

ECode ASN1Sequence::EncodeContent(
    /* [in] */ IBerOutputStream* bos)
{
    return bos->EncodeSequence(this);
}

ECode ASN1Sequence::SetEncodingContent(
    /* [in] */ IBerOutputStream* bos)
{
    return bos->GetSequenceLength(this);
}

ECode ASN1Sequence::GetEncodedLength(
    /* [in] */ IBerOutputStream* bos,
    /* [out] */ Int32* length)
{
    return ASN1TypeCollection::GetEncodedLength(bos, length);
}

ECode ASN1Sequence::ToString(
    /* [out] */ String* result)
{
    return ASN1TypeCollection::ToString(result);
}

ECode ASN1Sequence::SetType(
    /* [in] */ ArrayOf<IASN1Type *>* type)
{
    return ASN1TypeCollection::SetType(type);
}

ECode ASN1Sequence::GetType(
    /* [out, callee] */ ArrayOf<IASN1Type*>** type)
{
    return ASN1TypeCollection::GetType(type);
}

ECode ASN1Sequence::SetOPTIONAL(
    /* [in] */ ArrayOf<Boolean>* optional)
{
    return ASN1TypeCollection::SetOptional(optional);
}

ECode ASN1Sequence::GetOPTIONAL(
    /* [out, callee] */ ArrayOf<Boolean>** optional)
{
    return ASN1TypeCollection::GetOptional(optional);
}

ECode ASN1Sequence::SetDEFAULT(
    /* [in] */ ArrayOf<IInterface*>* def)
{
    return ASN1TypeCollection::SetDefault(def);
}

ECode ASN1Sequence::GetDEFAULT(
    /* [out, callee] */ ArrayOf<IInterface*>** def)
{
    return ASN1TypeCollection::GetDefault(def);
}

ECode ASN1Sequence::GetValues(
    /* [in] */ IInterface* object,
    /* [in] */ ArrayOf<IInterface*>* values)
{
    return ASN1TypeCollection::GetValues(object, values);
}

ECode ASN1Sequence::Init(
    /* [in] */ ArrayOf<IASN1Type *>* type)
{
    return ASN1TypeCollection::Init(IASN1Constants::TAG_SEQUENCE, type);
}

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

