
#include "CContentInfoHelper.h"
#include "CContentInfo.h"
#include <cmdef.h>

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Pkcs7 {


ECode CContentInfoHelper::GetData(
    /* [out, callee] */ ArrayOf<Int32>** data)
{
    VALIDATE_NOT_NULL(data)
    *data = CContentInfo::DATA;
    INTERFACE_ADDREF(*data)
    return NOERROR;
}

ECode CContentInfoHelper::GetSignedData(
    /* [out, callee] */ ArrayOf<Int32>** signedData)
{
    VALIDATE_NOT_NULL(signedData)
    *signedData = CContentInfo::SIGNED_DATA;
    INTERFACE_ADDREF(*signedData)
    return NOERROR;
}

ECode CContentInfoHelper::GetEnvelopedData(
    /* [out, callee] */ ArrayOf<Int32>** envelopedData)
{
    VALIDATE_NOT_NULL(envelopedData)
    *envelopedData = CContentInfo::ENVELOPED_DATA;
    INTERFACE_ADDREF(*envelopedData)
    return NOERROR;
}

ECode CContentInfoHelper::GetSignedAndEnvelopedData(
    /* [out, callee] */ ArrayOf<Int32>** signedAndEnvelopedData)
{
    VALIDATE_NOT_NULL(signedAndEnvelopedData)
    *signedAndEnvelopedData = CContentInfo::SIGNED_AND_ENVELOPED_DATA;
    INTERFACE_ADDREF(*signedAndEnvelopedData)
    return NOERROR;
}

ECode CContentInfoHelper::GetDigestedData(
    /* [out, callee] */ ArrayOf<Int32>** digestedData)
{
    VALIDATE_NOT_NULL(digestedData)
    *digestedData = CContentInfo::DIGESTED_DATA;
    INTERFACE_ADDREF(*digestedData)
    return NOERROR;
}

ECode CContentInfoHelper::GetEncryptedData(
    /* [out, callee] */ ArrayOf<Int32>** encryptedData)
{
    VALIDATE_NOT_NULL(encryptedData)
    *encryptedData = CContentInfo::ENCRYPTED_DATA;
    INTERFACE_ADDREF(*encryptedData)
    return NOERROR;
}

ECode CContentInfoHelper::GetASN1(
    /* [out] */ IASN1Sequence** asn1)
{
    VALIDATE_NOT_NULL(asn1)
    *asn1 = CContentInfo::ASN1;
    INTERFACE_ADDREF(*asn1)
    return NOERROR;
}

} // namespace Pkcs7
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

