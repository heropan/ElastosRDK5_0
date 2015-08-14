
#ifndef __ORG_APACHE_HARMONY_SECURITY_PKCS7_CCONTENTINFOHELPER_H__
#define __ORG_APACHE_HARMONY_SECURITY_PKCS7_CCONTENTINFOHELPER_H__

#include "_CContentInfoHelper.h"

using Org::Apache::Harmony::Security::Asn1::IASN1Sequence;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Pkcs7 {

CarClass(CContentInfoHelper)
{
public:
    CARAPI GetData(
        /* [out, callee] */ ArrayOf<Int32>** data);

    CARAPI GetSignedData(
        /* [out, callee] */ ArrayOf<Int32>** signedData);

    CARAPI GetEnvelopedData(
        /* [out, callee] */ ArrayOf<Int32>** envelopedData);

    CARAPI GetSignedAndEnvelopedData(
        /* [out, callee] */ ArrayOf<Int32>** signedAndEnvelopedData);

    CARAPI GetDigestedData(
        /* [out, callee] */ ArrayOf<Int32>** digestedData);

    CARAPI GetEncryptedData(
        /* [out, callee] */ ArrayOf<Int32>** encryptedData);

    CARAPI GetASN1(
        /* [out] */ IASN1Sequence** asn1);
};

} // namespace Pkcs7
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HARMONY_SECURITY_PKCS7_CCONTENTINFOHELPER_H__
