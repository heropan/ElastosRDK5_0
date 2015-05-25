#include "cmdef.h"
#include "Certificate.h"
#include "CByteArrayInputStream.h"
#include "CCertificateFactoryHelper.h"

using Elastos::IO::CByteArrayInputStream;

#define FAIL_RETURN_WITH_ERROR_CAST(code, serr, derr) \
do { \
    ECode ec = code; \
    if(ec == serr) { \
        return derr; \
    } \
    FAIL_RETURN(ec) \
} while(0);

namespace Elastos {
namespace Security {
namespace Cert {

//{0907DFBE-9196-89D8-3CF9-C7D1DD52E4C5}
extern "C" const InterfaceID EIID_Certificate =
        { 0x0907DFBE, 0x9196, 0x89D8, { 0x3C, 0xF9, 0xC7, 0xD1, 0xDD, 0x52, 0xE4, 0xC5 } };

static const Int64 sSerialVersionUID = -3585440601605666277L;

Certificate::Certificate(
        /* [in] */ const String& type)
    : mType(type)
{}

CARAPI Certificate::GetType(
        /* [out] */ String* type) const
{
    VALIDATE_NOT_NULL(type)
    *type = mType;
    return NOERROR;
}

CARAPI Certificate::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* equal)

{
    VALIDATE_NOT_NULL(equal)
    // obj equal to itself
    if (this == reinterpret_cast<Certificate*>(other->Probe(EIID_Certificate))) {
        *equal = TRUE;
        return NOERROR;
    }
    if (ICertificate::Probe(other)) {
        // check that encoded forms match
        AutoPtr<ArrayOf<Byte> >  encode, oEncode;
        FAIL_RETURN_WITH_ERROR_CAST(GetEncoded((ArrayOf<Byte>**)&encode),
            E_CERTIFICATE_ENCODING_EXCEPTION, E_RUNTIME_EXCEPTION)
        FAIL_RETURN_WITH_ERROR_CAST(ICertificate::Probe(other)->GetEncoded((ArrayOf<Byte>**)&oEncode),
            E_CERTIFICATE_ENCODING_EXCEPTION, E_RUNTIME_EXCEPTION)
        *equal = encode->Equals(oEncode);
        return NOERROR;
    }
    *equal = FALSE;
    return NOERROR;
}

CARAPI Certificate::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    AutoPtr<ArrayOf<Byte> > encoded;
    FAIL_RETURN_WITH_ERROR_CAST(GetEncoded((ArrayOf<Byte>**)&encoded), E_CERTIFICATE_ENCODING_EXCEPTION, E_RUNTIME_EXCEPTION)
    Int32 hash = 0;
    for (Int32 i = 0; i < encoded->GetLength(); ++i) {
        hash += i * (*encoded)[i];
    }
    *hashCode = hash;
    return NOERROR;
}

CARAPI Certificate::WriteReplace(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    AutoPtr<ArrayOf<Byte> > encoded;
    FAIL_RETURN_WITH_ERROR_CAST(GetEncoded((ArrayOf<Byte>**)&encoded),
        E_CERTIFICATE_ENCODING_EXCEPTION, E_NOT_SERIALIZABLE_EXCEPTION)
    AutoPtr<CertificateRep> cr = new CertificateRep(mType, encoded);
    *object = cr.Get();
    INTERFACE_ADDREF(*object);
    return NOERROR;
}

CAR_INTERFACE_IMPL_LIGHT(Certificate::CertificateRep, ISerializable)

const Int64 Certificate::CertificateRep::sSerialVersionUID = -8563758940495660020L;
const AutoPtr<ArrayOf<IObjectStreamField*> > Certificate::CertificateRep::sSerialPersistentFields;

Certificate::CertificateRep::CertificateRep(
    /* [in] */ const String& type,
    /* [in] */ ArrayOf<Byte>* data)
    : mType(type)
    , mData(data)
{}

/**
 * Deserializes a {@code Certificate} from a serialized {@code
 * CertificateRep} object.
 *
 * @return the deserialized {@code Certificate}.
 * @throws ObjectStreamException
 *             if deserialization fails.
 */
CARAPI Certificate::CertificateRep::ReadResolve(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    AutoPtr<ICertificateFactoryHelper> helper;
    AutoPtr<ICertificateFactory> cf;
    CCertificateFactoryHelper::AcquireSingleton((ICertificateFactoryHelper**)&helper);
    helper->GetInstance(mType, (ICertificateFactory**)&cf);
    AutoPtr<IInputStream> is;
    CByteArrayInputStream::New(mData, (IInputStream**)&is);
    AutoPtr<ICertPath> cp;
    FAIL_RETURN(cf->GenerateCertPath(is, (ICertPath**)&cp))
    *object = cp.Get();
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

} // end Cert
} // end Security
} // end Elastos
