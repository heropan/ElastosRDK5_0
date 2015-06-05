
#include "CASN1OctetString.h"
#include <cmdef.h>

using Elastos::Utility::Arrays;
using Elastos::Core::CArrayOf;
using Elastos::Core::IArrayOf;
using Elastos::Core::CByte;
using Elastos::Core::IByte;
using Elastos::Core::EIID_IByte;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

static AutoPtr<IASN1Type> ASN1;

AutoPtr<IASN1Type> CASN1OctetString::InitStatic()
{
    return new CASN1OctetString();
}

ECode CASN1OctetString::GetInstance(
    /* [out] */ IASN1Type** instance)
{
    VALIDATE_NOT_NULL(instance)
    *instance = ASN1;
    REFCOUNT_ADD(*instance)
    return NOERROR;
}

ECode CASN1OctetString::GetId(
    /* [out] */ Int32* id)
{
    return ASN1StringType::GetId(id);
}

ECode CASN1OctetString::GetConstrId(
    /* [out] */ Int32* constrId)
{
    return ASN1StringType::GetConstrId(constrId);
}

ECode CASN1OctetString::Decode(
    /* [in] */ ArrayOf<Byte>* encoded,
    /* [out] */ IInterface** object)
{
    return ASN1StringType::Decode(encoded, object);
}

ECode CASN1OctetString::DecodeEx(
    /* [in] */ ArrayOf<Byte>* encoded,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 encodingLen,
    /* [out] */ IInterface** object)
{
    return ASN1StringType::DecodeEx(encoded, offset, encodingLen, object);
}

ECode CASN1OctetString::DecodeEx2(
    /* [in] */ IInputStream* is,
    /* [out] */ IInterface** object)
{
    return ASN1StringType::DecodeEx2(is, object);
}

ECode CASN1OctetString::Verify(
    /* [in] */ ArrayOf<Byte>* encoded)
{
    return ASN1StringType::Verify(encoded);
}

ECode CASN1OctetString::VerifyEx(
    /* [in] */ IInputStream* is)
{
    return ASN1StringType::VerifyEx(is);
}

ECode CASN1OctetString::Encode(
    /* [in] */ IInterface* object,
    /* [out, callee] */ ArrayOf<Byte>** encode)
{
    return ASN1StringType::Encode(object, encode);
}

ECode CASN1OctetString::DecodeEx3(
    /* [in] */ IBerInputStream* bis,
    /* [out] */ IInterface** object)
{
    bis->ReadOctetString();

    if (((CBerInputStream*)bis)->mIsVerify) {
        return NOERROR;
    }
    return GetDecodedObject(bis, object);
}

ECode CASN1OctetString::CheckTag(
    /* [in] */ Int32 identifier,
    /* [out] */ Boolean* checkTag)
{
    return ASN1StringType::CheckTag(checkTag);
}

ECode CASN1OctetString::GetDecodedObject(
    /* [in] */ IBerInputStream* bis,
    /* [out] */ IInterface** object)
{
    Int32 contentOffset, length;
    AutoPtr<ArrayOf<Byte> > buffer, tmp;
    bis->GetContentOffset(&contentOffset);
    bis->GetLength(&length);
    AutoPtr<IArrays> arr;
    CArrays::Acquiresingleton((IArrays**)&arr);
    arr->CopyOfRangeByte(buffer, contentOffset, contentOffset + length, tmp);
    AutoPtr<IArrayOf> arrayOf;
    CArrayOf::New(EIID_IByte, tmp->GetLength(), (IArrayOf**)&arrayOf);
    for (Int32 i = 0; i < tmp->GetLength(); i++) {
        AutoPtr<IByte> bt;
        CByte::New((*tmp)[i], (IByte**)&bt);
        arrayOf->Put(i, bt.Get());
    }
    *object = arrayOf.Get();
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode CASN1OctetString::EncodeASN(
    /* [in] */ IBerOutputStream* bos)
{
    return ASN1StringType::EncodeASN(bos);
}

ECode CASN1OctetString::EncodeContent(
    /* [in] */ IBerOutputStream* bos)
{
    return bos->EncodeOctetString();
}

ECode CASN1OctetString::SetEncodingContent(
    /* [in] */ IBerOutputStream* bos)
{
    AutoPtr<IInterface> content;
    bos->GetContent((IInterface**)&content);
    Int32 length;
    IArrayOf::Probe(content)->GetLength(&length);
    return bos->SetLength(length);
}

ECode CASN1OctetString::GetEncodedLength(
    /* [in] */ IBerOutputStream* bos,
    /* [out] */ Int32* length)
{
    return ASN1StringType::GetEncodedLength(bos, length);
}

ECode CASN1OctetString::ToString(
    /* [out] */ String* result)
{
    return ASN1StringType::ToString(result);
}

ECode CASN1OctetString::constructor()
{
    return ASN1StringType::Init(IASN1Constants::TAG_OCTETSTRING);
}

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

