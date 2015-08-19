
#include "X509CRLEntry.h"

namespace Elastos {
namespace Security {
namespace Cert {

// {9CB9E71A-34C3-BEAC-DC56-6E5F9B9101FC}
extern "C" const InterfaceID EIID_X509CRLEntry =
    { 0x9CB9E71A, 0x34C3, 0xBEAC, { 0xDC, 0x56, 0x6E, 0x5F, 0x9B, 0x91, 0x01, 0xFC } };

CAR_INTERFACE_IMPL_2(X509CRLEntry, Object, IX509CRLEntry, IX509Extension);
ECode X509CRLEntry::GetInterfaceID(
    /* [in] */ IInterface *object,
    /* [out] */ InterfaceID *pIID)
{
    if (object == (IInterface*)(IX509Extension *)this) {
        *pIID = EIID_IX509Extension;
    } else if (object == reinterpret_cast<PInterface>(this)) {
        *pIID = EIID_X509CRLEntry;
    } else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

PInterface X509CRLEntry::Probe(
    /* [in] */ REIID riid)
{
    if(riid == EIID_X509CRLEntry) {
        return reinterpret_cast<PInterface>(this);
    } else if (riid == EIID_IInterface) {
        return (IInterface*)(IX509Extension*)this;
    } else if (riid == EIID_IX509Extension) {
        return (PInterface)(IX509Extension*)this;
    } else {
        return NULL;
    }
}

X509CRLEntry::X509CRLEntry()
{}

ECode X509CRLEntry::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual)
    if (reinterpret_cast<X509CRLEntry*>(other->Probe(EIID_X509CRLEntry)) == this) {
        *isEqual = TRUE;
        return NOERROR;
    }
    if (other == NULL ||
        !other->Probe(EIID_X509CRLEntry)) {
        *isEqual = FALSE;
        return NOERROR;
    }
    AutoPtr<X509CRLEntry> obj = reinterpret_cast<X509CRLEntry*>(other->Probe(EIID_X509CRLEntry));
    AutoPtr<ArrayOf<Byte> > encode, oEncode;
    ECode ec = GetEncoded((ArrayOf<Byte>**)&encode);
    FAIL_GOTO(ec, FAIL_PROCESS)
    ec = obj->GetEncoded((ArrayOf<Byte>**)&oEncode);
    FAIL_GOTO(ec, FAIL_PROCESS)
    return encode->Equals(oEncode);

FAIL_PROCESS:
    if (ec == (ECode)E_CRL_EXCEPTION) {
        *isEqual = FALSE;
        return NOERROR;
    }
    return ec;
}

ECode X509CRLEntry::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    Int32 res = 0;
    AutoPtr<ArrayOf<Byte> > array;
    ECode ec = GetEncoded((ArrayOf<Byte>**)&array);
    FAIL_GOTO(ec, FAIL_PROCESS)
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        res += (*array)[i] & 0xFF;
    }
FAIL_PROCESS:
    if (ec == (ECode)E_CRL_EXCEPTION) {
        ;
    }
    else
    {
        FAIL_RETURN(ec)
    }
    *hashCode = res;
    return NOERROR;
}

ECode X509CRLEntry::GetCertificateIssuer(
    /* [out] */ IX500Principal** issuer)
{
    return NOERROR;
}

} // end Cert
} // end Security
} // end Elastos