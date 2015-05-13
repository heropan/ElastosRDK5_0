
#include <ASN1Type.h>
#include <Org.Apache.Harmony_server.h>
#include <cmdef.h>
#include <CDerInputStream.h>
#include <CDerOutputStream.h>

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

ASN1Type::ASN1Type(
    /* [in] */ Int32 tagNumber)
{
    InitEx(IASN1Constants::CLASS_UNIVERSAL, tagNumber);
}

ASN1Type::ASN1Type(
    /* [in] */ Int32 tagClass,
    /* [in] */ Int32 tagNumber)
{
    InitEx(tagClass, tagNumber);
}

ASN1Type::Init(
    /* [in] */ Int32 tagNumber)
{
    InitEx(IASN1Constants::CLASS_UNIVERSAL, tagNumber);
}

ASN1Type::Init(
    /* [in] */ Int32 tagClass,
    /* [in] */ Int32 tagNumber)
{
    InitEx(tagClass, tagNumber);
}

ECode ASN1Type::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id)
    *id = mId;
    return NOERROR;
}

/** Integer representation of constructed identifier. */
ECode ASN1Type::GetConstrId(
    /* [out] */ Int32* constrId)
{
    VALIDATE_NOT_NULL(constrId)
    *constrId = mConstrId;
    return NOERROR;
}

ECode ASN1Type::Decode(
    /* [in] */ ArrayOf<Byte>* encoded,
    /* [out] */ IInterface** object)
{
    AutoPtr<IBerInputStream> bis;
    CDerInputStream::New(encoded, (IBerInputStream**)&bis);
    return DecodeEx3(bis, object);
}

ECode ASN1Type::DecodeEx(
    /* [in] */ ArrayOf<Byte>* encoded,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 encodingLen,
    /* [out] */ IInterface** object)
{
    AutoPtr<IBerInputStream> bis;
    CDerInputStream::New(encoded, offset, encodingLen, (IBerInputStream**)&bis);
    return DecodeEx3(bis, object);
}

ECode ASN1Type::DecodeEx2(
    /* [in] */ IInputStream* is,
    /* [out] */ IInterface** object)
{
    AutoPtr<IBerInputStream> bis;
    CDerInputStream::New(is, (IBerInputStream**)&bis);
    return DecodeEx3(bis, object);
}

ECode ASN1Type::Verify(
    /* [in] */ ArrayOf<Byte>* encoded)
{
    AutoPtr<IBerInputStream> bis;
    CDerInputStream::New(encoded, (IBerInputStream**)&bis);
    bis->SetVerify();
    AutoPtr<IInterface> object;
    return DecodeEx3(bis, (IInterface**)&object);
}

ECode ASN1Type::VerifyEx(
    /* [in] */ IInputStream* is)
{
    AutoPtr<IBerInputStream> decoder;
    CDerInputStream::New(is, (IBerInputStream**)&decoder);
    decoder->SetVerify();
    AutoPtr<IInterface> object;
    return DecodeEx3(decoder, (IInterface**)&object);
}

ECode ASN1Type::Encode(
    /* [in] */ IInterface* object,
    /* [out, callee] */ ArrayOf<Byte>** encode)
{
    AutoPtr<IBerOutputStream> output;
    CDerOutputStream::New(THIS_PROBE(IASN1Type), object, (IBerOutputStream**)&output);
    return output->GetEncoded(encode);
}

/**
 * Creates decoded object.
 *
 * Derived classes should override this method to provide creation for a
 * selected class of objects during decoding.
 *
 * The default implementation returns an object created by decoding stream.
*/
ECode ASN1Type::GetDecodedObject(
    /* [in] */ IBerInputStream* bis,
    /* [out] */ IInterface** object)
{
    return bis->GetContent(object);
}

ECode ASN1Type::GetEncodedLength(
    /* [in] */ IBerOutputStream* bos,
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length)
    //tag length
    Int32 len = 1; //FIXME tag length = 1. what about long form?
    //for (; tag > 0; tag = tag >> 8, len++);

    // length length :-)
    len++;
    Int32 outLen;
    bos->GetLength(&outLen);
    if (outLen > 127) {

        len++;
        for (Int32 cur = outLen >> 8; cur > 0; len++) {
            cur = cur >> 8;
        }
    }
    len += outLen;

    *length = len;
    return NOERROR;
}

ECode ASN1Type::ToString(
    /* [out] */ String* result)
{
    // TODO decide whether this method is necessary
    //return getClass().getName() + "(tag: 0x" + Integer.toHexString(0xff & this.id) + ")";
    return E_NOT_IMPLEMENTED;
}

ECode ASN1Type::Init(
    /* [in] */ Int32 tagClass,
    /* [in] */ Int32 tagNumber)
{
    if (tagNumber < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (tagClass != IASN1Constants::CLASS_UNIVERSAL
        && tagClass != IASN1Constants::CLASS_APPLICATION
        && tagClass != IASN1Constants::CLASS_CONTEXTSPECIFIC
        && tagClass != IASN1Constants::CLASS_PRIVATE) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (tagNumber < 31) {
        // short form
        mId = tagClass + tagNumber;
    } else {
        // long form
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mConstrId = mId + IASN1Constants::PC_CONSTRUCTED;
    return NOERROR;
}

PInterface ASN1Type::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org