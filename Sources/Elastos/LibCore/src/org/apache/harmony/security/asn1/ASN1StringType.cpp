
#include <ASN1StringType.h>
#include <Org.Apache.Harmony.h>
#include <cmdef.h>
#include <CBerInputStream.h>

using Elastos::Core::CString;
using Elastos::Core::ICharSequence;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

class ASN1StringType::ASN1StringTypeNull
        : public ASN1StringType
        , public ElRefBase
        , public IASN1Type
{
    public:
        CAR_INTERFACE_DECL()

        //////////////////////////////////////////
        CARAPI GetId(
            /* [out] */ Int32* id);

        CARAPI GetConstrId(
            /* [out] */ Int32* constrId);

        CARAPI Decode(
            /* [in] */ ArrayOf<Byte>* encoded,
            /* [out] */ IInterface** object);

        CARAPI DecodeEx(
            /* [in] */ ArrayOf<Byte>* encoded,
            /* [in] */ Int32 offset,
            /* [in] */ Int32 encodingLen,
            /* [out] */ IInterface** object);

        CARAPI DecodeEx2(
            /* [in] */ IInputStream* is,
            /* [out] */ IInterface** object);

        CARAPI Verify(
            /* [in] */ ArrayOf<Byte>* encoded);

        CARAPI VerifyEx(
            /* [in] */ IInputStream* is);

        CARAPI Encode(
            /* [in] */ IInterface* object,
            /* [out, callee] */ ArrayOf<Byte>** encode);

        CARAPI DecodeEx3(
            /* [in] */ IBerInputStream* bis,
            /* [out] */ IInterface** object);

        CARAPI CheckTag(
            /* [in] */ Int32 identifier,
            /* [out] */ Boolean* checkTag);

        CARAPI EncodeASN(
            /* [in] */ IBerOutputStream* bos);

        CARAPI EncodeContent(
            /* [in] */ IBerOutputStream* bos);

        CARAPI SetEncodingContent(
            /* [in] */ IBerOutputStream* bos);

        CARAPI GetEncodedLength(
            /* [in] */ IBerOutputStream* bos,
            /* [out] */ Int32* length);

        CARAPI ToString(
            /* [out] */ String* result);
        //////////////////////////////////////////

        ASN1StringTypeNull(
            /* [in] */ Int32 tagNumber);
};

class ASN1StringType::ASN1StringTypeOfDerived
    : public ASN1StringType
    , public ElRefBase
    , public IASN1Type
{
public:
    CAR_INTERFACE_DECL()

    ASN1StringTypeOfDerived(
        /* [in] */ Int32 tagNumber);
    CARAPI GetDecodedObject(
        /* [in] */ IBerInputStream* bis,
        /* [out] */ IInterface** object);
    CARAPI SetEncodingContent(
        /* [in] */ IBerOutputStream* bos);

    //////////////////////////////////////////////
    CARAPI GetId(
        /* [out] */ Int32* id);

    CARAPI GetConstrId(
        /* [out] */ Int32* constrId);

    CARAPI Decode(
        /* [in] */ ArrayOf<Byte>* encoded,
        /* [out] */ IInterface** object);

    CARAPI DecodeEx(
        /* [in] */ ArrayOf<Byte>* encoded,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 encodingLen,
        /* [out] */ IInterface** object);

    CARAPI DecodeEx2(
        /* [in] */ IInputStream* is,
        /* [out] */ IInterface** object);

    CARAPI Verify(
        /* [in] */ ArrayOf<Byte>* encoded);

    CARAPI VerifyEx(
        /* [in] */ IInputStream* is);

    CARAPI Encode(
        /* [in] */ IInterface* object,
        /* [out, callee] */ ArrayOf<Byte>** encode);

    CARAPI DecodeEx3(
        /* [in] */ IBerInputStream* bis,
        /* [out] */ IInterface** object);

    CARAPI CheckTag(
        /* [in] */ Int32 identifier,
        /* [out] */ Boolean* checkTag);

    CARAPI EncodeASN(
        /* [in] */ IBerOutputStream* bos);

    CARAPI EncodeContent(
        /* [in] */ IBerOutputStream* bos);

    CARAPI GetEncodedLength(
        /* [in] */ IBerOutputStream* bos,
        /* [out] */ Int32* length);

    CARAPI ToString(
        /* [out] */ String* result);
    //////////////////////////////////////////////
};

AutoPtr<IASN1Type> ASN1StringType::sBMPSTRING;

AutoPtr<IASN1Type> ASN1StringType::sIA5STRING;

AutoPtr<IASN1Type> ASN1StringType::sGENERALSTRING;

AutoPtr<IASN1Type> ASN1StringType::sPRINTABLESTRING;

AutoPtr<IASN1Type> ASN1StringType::sTELETEXSTRING;

AutoPtr<IASN1Type> ASN1StringType::sUTF8STRING;

AutoPtr<IASN1Type> ASN1StringType::sUNIVERSALSTRING = InitStatic();

AutoPtr<IASN1Type> ASN1StringType::InitStatic()
{
    sBMPSTRING = (IASN1Type*)(new ASN1StringTypeNull(IASN1Constants::TAG_BMPSTRING))->Probe(EIID_IASN1Type);
    sIA5STRING = (IASN1Type*)(new ASN1StringTypeNull(IASN1Constants::TAG_IA5STRING))->Probe(EIID_IASN1Type);
    sGENERALSTRING = (IASN1Type*)(new ASN1StringTypeNull(IASN1Constants::TAG_GENERALSTRING))->Probe(EIID_IASN1Type);
    sPRINTABLESTRING = (IASN1Type*)(new ASN1StringTypeNull(IASN1Constants::TAG_PRINTABLESTRING))->Probe(EIID_IASN1Type);
    sTELETEXSTRING = (IASN1Type*)(new ASN1StringTypeNull(IASN1Constants::TAG_TELETEXSTRING))->Probe(EIID_IASN1Type);
    sUTF8STRING = (IASN1Type*)(new ASN1StringTypeOfDerived(IASN1Constants::TAG_UTF8STRING))->Probe(EIID_IASN1Type);
    return (IASN1Type*)(new ASN1StringTypeNull(IASN1Constants::TAG_UNIVERSALSTRING))->Probe(EIID_IASN1Type);
}

CAR_INTERFACE_IMPL(ASN1StringType::ASN1StringTypeNull, IASN1Type)

////////////////////////////////////////////////////////////
ECode ASN1StringType::ASN1StringTypeNull::GetId(
    /* [out] */ Int32* id)
{
    return ASN1StringType::GetId(id);
}

ECode ASN1StringType::ASN1StringTypeNull::GetConstrId(
     /* [out] */ Int32* constrId)
{
    return ASN1StringType::GetConstrId(constrId);
}

ECode ASN1StringType::ASN1StringTypeNull::Decode(
     /* [in] */ ArrayOf<Byte>* encoded,
     /* [out] */ IInterface** object)
{
    return ASN1StringType::Decode(encoded, object);
}

ECode ASN1StringType::ASN1StringTypeNull::DecodeEx(
     /* [in] */ ArrayOf<Byte>* encoded,
     /* [in] */ Int32 offset,
     /* [in] */ Int32 encodingLen,
     /* [out] */ IInterface** object)
{
    return ASN1StringType::DecodeEx(encoded, offset
        ,encodingLen, object);
}

ECode ASN1StringType::ASN1StringTypeNull::DecodeEx2(
     /* [in] */ IInputStream* is,
     /* [out] */ IInterface** object)
{
    return ASN1StringType::DecodeEx2(is, object);
}

ECode ASN1StringType::ASN1StringTypeNull::Verify(
     /* [in] */ ArrayOf<Byte>* encoded)
{
    return ASN1StringType::Verify(encoded);
}

ECode ASN1StringType::ASN1StringTypeNull::VerifyEx(
     /* [in] */ IInputStream* is)
{
    return ASN1StringType::VerifyEx(is);
}

ECode ASN1StringType::ASN1StringTypeNull::Encode(
     /* [in] */ IInterface* object,
     /* [out, callee] */ ArrayOf<Byte>** encode)
{
    return ASN1StringType::Encode(object, encode);
}

ECode ASN1StringType::ASN1StringTypeNull::DecodeEx3(
     /* [in] */ IBerInputStream* bis,
     /* [out] */ IInterface** object)
{
    return ASN1StringType::DecodeEx3(bis, object);
}

ECode ASN1StringType::ASN1StringTypeNull::CheckTag(
     /* [in] */ Int32 identifier,
     /* [out] */ Boolean* checkTag)
{
    return ASN1StringType::CheckTag(identifier, checkTag);
}

ECode ASN1StringType::ASN1StringTypeNull::EncodeASN(
     /* [in] */ IBerOutputStream* bos)
{
    return ASN1StringType::EncodeASN(bos);
}

ECode ASN1StringType::ASN1StringTypeNull::EncodeContent(
     /* [in] */ IBerOutputStream* bos)
{
    return ASN1StringType::EncodeContent(bos);
}

ECode ASN1StringType::ASN1StringTypeNull::SetEncodingContent(
     /* [in] */ IBerOutputStream* bos)
{
    return ASN1StringType::SetEncodingContent(bos);
}

ECode ASN1StringType::ASN1StringTypeNull::GetEncodedLength(
     /* [in] */ IBerOutputStream* bos,
     /* [out] */ Int32* length)
{
    return ASN1StringType::GetEncodedLength(bos, length);
}

ECode ASN1StringType::ASN1StringTypeNull::ToString(
     /* [out] */ String* result)
{
    return ASN1StringType::ToString(result);
}
////////////////////////////////////////////////////////////

ASN1StringType::ASN1StringTypeNull::ASN1StringTypeNull(
    /* [in] */ Int32 tagNumber)
    : ASN1StringType(tagNumber){}

CAR_INTERFACE_IMPL(ASN1StringType::ASN1StringTypeOfDerived, IASN1Type)

////////////////////////////////////////////////////////////////////
ECode ASN1StringType::ASN1StringTypeOfDerived::GetId(
    /* [out] */ Int32* id)
{
    return ASN1StringType::GetId(id);
}

ECode ASN1StringType::ASN1StringTypeOfDerived::GetConstrId(
     /* [out] */ Int32* constrId)
{
    return ASN1StringType::GetConstrId(constrId);
}

ECode ASN1StringType::ASN1StringTypeOfDerived::Decode(
     /* [in] */ ArrayOf<Byte>* encoded,
     /* [out] */ IInterface** object)
{
    return ASN1StringType::Decode(encoded, object);
}

ECode ASN1StringType::ASN1StringTypeOfDerived::DecodeEx(
     /* [in] */ ArrayOf<Byte>* encoded,
     /* [in] */ Int32 offset,
     /* [in] */ Int32 encodingLen,
     /* [out] */ IInterface** object)
{
    return ASN1StringType::DecodeEx(encoded, offset
        ,encodingLen, object);
}

ECode ASN1StringType::ASN1StringTypeOfDerived::DecodeEx2(
     /* [in] */ IInputStream* is,
     /* [out] */ IInterface** object)
{
    return ASN1StringType::DecodeEx2(is, object);
}

ECode ASN1StringType::ASN1StringTypeOfDerived::Verify(
     /* [in] */ ArrayOf<Byte>* encoded)
{
    return ASN1StringType::Verify(encoded);
}

ECode ASN1StringType::ASN1StringTypeOfDerived::VerifyEx(
     /* [in] */ IInputStream* is)
{
    return ASN1StringType::VerifyEx(is);
}

ECode ASN1StringType::ASN1StringTypeOfDerived::Encode(
     /* [in] */ IInterface* object,
     /* [out, callee] */ ArrayOf<Byte>** encode)
{
    return ASN1StringType::Encode(object, encode);
}

ECode ASN1StringType::ASN1StringTypeOfDerived::DecodeEx3(
     /* [in] */ IBerInputStream* bis,
     /* [out] */ IInterface** object)
{
    return ASN1StringType::DecodeEx3(bis, object);
}

ECode ASN1StringType::ASN1StringTypeOfDerived::CheckTag(
     /* [in] */ Int32 identifier,
     /* [out] */ Boolean* checkTag)
{
    return ASN1StringType::CheckTag(identifier, checkTag);
}

ECode ASN1StringType::ASN1StringTypeOfDerived::EncodeASN(
     /* [in] */ IBerOutputStream* bos)
{
    return ASN1StringType::EncodeASN(bos);
}

ECode ASN1StringType::ASN1StringTypeOfDerived::EncodeContent(
     /* [in] */ IBerOutputStream* bos)
{
    return ASN1StringType::EncodeContent(bos);
}

ECode ASN1StringType::ASN1StringTypeOfDerived::ToString(
     /* [out] */ String* result)
{
    return ASN1StringType::ToString(result);
}
///////////////////////////////////////////////////////

ASN1StringType::ASN1StringTypeOfDerived::ASN1StringTypeOfDerived(
    /* [in] */ Int32 tagNumber)
    : ASN1StringType(tagNumber){}

ECode ASN1StringType::ASN1StringTypeOfDerived::GetDecodedObject(
    /* [in] */ IBerInputStream* bis,
    /* [out] */ IInterface** object)
{
    //return new String(in.buffer, in.contentOffset, in.length, Charsets.UTF_8);
    VALIDATE_NOT_NULL(object)
    AutoPtr<ArrayOf<Byte> > buffer;
    bis->GetBuffer((ArrayOf<Byte>**)&buffer);
    Int32 contentOffset, length;
    bis->GetContentOffset(&contentOffset);
    bis->GetLength(&length);
    //Todo
    //following code needed to transfer, comment just now
    //String str(reinterpret_cast<ArrayOf<Char32>* >(buffer.Get()), contentOffset, length);
    String str;
    AutoPtr<ICharSequence> cs;
    CString::New(str, (ICharSequence**)&cs);
    *object = cs.Get();
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode ASN1StringType::ASN1StringTypeOfDerived::SetEncodingContent(
    /* [in] */ IBerOutputStream* bos)
{
    AutoPtr<IInterface> content;
    bos->GetContent((IInterface**)&content);
    String str;
    ICharSequence::Probe(content)->ToString(&str);
    AutoPtr<ArrayOf<Byte> > bytes = str.GetBytes();
    AutoPtr<ICharSequence> cs;
    CString::New(str, (ICharSequence**)&cs);
    bos->SetContent(cs.Get());
    bos->SetLength(bytes->GetLength());
    return NOERROR;
}
//////////////////////////////////////////////////////////////////////////

ASN1StringType::ASN1StringType(
    /* [in] */ Int32 tagNumber)
    : ASN1Type(tagNumber)
{}

ECode ASN1StringType::Init(
    /* [in] */ Int32 tagNumber)
{
    ASN1Type::Init(tagNumber);
}

ECode ASN1StringType::CheckTag(
    /* [in] */ Int32 identifier,
    /* [ou] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mId == identifier || mConstrId == identifier;
    return NOERROR;
}

ECode ASN1StringType::DecodeEx3(
    /* [in] */ IBerInputStream* bis,
    /* [out] */ IInterface** object)
{
    bis->ReadString(THIS_PROBE(IASN1Type));

    if (((CBerInputStream*)bis)->mIsVerify) {
        return NOERROR;
    }

    return GetDecodedObject(bis, object);
}

ECode ASN1StringType::GetDecodedObject(
    /* [in] */ IBerInputStream* bis,
    /* [out] */ IInterface** object)
{
    /* To ensure we get the correct encoding on non-ASCII platforms, specify
           that we wish to convert from ASCII to the default platform encoding */
    //return new String(in.buffer, in.contentOffset, in.length, Charsets.ISO_8859_1);
    VALIDATE_NOT_NULL(object)
    AutoPtr<ArrayOf<Byte> > buffer;
    bis->GetBuffer((ArrayOf<Byte>**)&buffer);
    Int32 contentOffset, length;
    bis->GetContentOffset(&contentOffset);
    bis->GetLength(&length);
    //Todo
    //String str(buffer.Get(), contentOffset, length);
    String str;
    AutoPtr<ICharSequence> cs;
    CString::New(str, (ICharSequence**)&cs);
    *object = cs.Get();
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode ASN1StringType::EncodeASN(
    /* [in] */ IBerOutputStream* bos)
{
    bos->EncodeTag(mId);
    return EncodeContent(bos);
}

ECode ASN1StringType::EncodeContent(
    /* [in] */ IBerOutputStream* bos)
{
    return bos->EncodeString();
}

ECode ASN1StringType::SetEncodingContent(
    /* [in] */ IBerOutputStream* bos)
{
    AutoPtr<IInterface> content;
    bos->GetContent((IInterface**)&content);
    String str;
    ICharSequence::Probe(content)->ToString(&str);
    AutoPtr<ArrayOf<Byte> > bytes = str.GetBytes();
    AutoPtr<ICharSequence> cs;
    CString::New(str, (ICharSequence**)&cs);
    bos->SetContent(cs.Get());
    bos->SetLength(bytes->GetLength());
    return NOERROR;
}

PInterface ASN1StringType::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org