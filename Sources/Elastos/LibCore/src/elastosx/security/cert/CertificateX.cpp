
#include "CertificateX.h"
#include "Arrays.h"

using Elastos::Utility::Arrays;

namespace Elastosx {
namespace Security {
namespace Cert {

CAR_INTERFACE_IMPL(Certificate, Object, ICertificate)

// {E2B9C46C-4362-7D9B-4DF1-84C0669BE14D}
extern "C" const InterfaceID EIID_XCertificate =
    { 0xE2B9C46C, 0x4362, 0x7D9B, { 0x4D, 0xF1, 0x84, 0xC0, 0x66, 0x9B, 0xE1, 0x4D } };

ECode Certificate::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual)
    *isEqual = FALSE;
    VALIDATE_NOT_NULL(obj)

    if (obj->Probe(EIID_XCertificate) == this->Probe(EIID_XCertificate)) {
        *isEqual = TRUE;
        return NOERROR;
    }
    if (NULL == obj->Probe(EIID_XCertificate)) {
        return NOERROR;
    }
    AutoPtr<ICertificate> object = (ICertificate*) obj->Probe(EIID_XCertificate);
    AutoPtr<ArrayOf<Byte> > encode, oEncode;
    ECode ec;
    FAIL_GOTO(ec = GetEncoded((ArrayOf<Byte>**)&encode), ERROR_PROCESS)
    FAIL_GOTO(ec =((ICertificate*) obj->Probe(EIID_XCertificate))->GetEncoded((ArrayOf<Byte>**)&oEncode), ERROR_PROCESS)
    *isEqual = Arrays::Equals(encode, oEncode);
    return NOERROR;
ERROR_PROCESS:
    if(E_CERTIFICATE_ENCODING_EXCEPTION == ec) {
        *isEqual = FALSE;
        return NOERROR;
    }
    return ec;
}

ECode Certificate::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    *hashCode = 0;

    Int32 res = 0;
    AutoPtr<ArrayOf<Byte> > encode;
    ECode ec;
    FAIL_GOTO(ec = GetEncoded((ArrayOf<Byte>**)&encode), ERROR_PROCESS)
    for (Int32 i = 0; i < encode->GetLength(); ++i) {
        res += (*encode)[i];
    }
ERROR_PROCESS:
    if (E_CERTIFICATE_ENCODING_EXCEPTION == ec) {
        ;
    }
    else {
        FAIL_RETURN(ec)
    }
    *hashCode = res;
    return ec;
}

} // end Cert
} // end Security
} // end Elastosx