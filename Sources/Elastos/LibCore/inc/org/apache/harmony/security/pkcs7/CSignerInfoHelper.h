
#ifndef __CSIGNERINFOHELPER_H__
#define __CSIGNERINFOHELPER_H__

#include "_CSignerInfoHelper.h"

using Org::Apache::Harmony::Security::Asn1::IASN1Sequence;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Pkcs7 {

CarClass(CSignerInfoHelper)
{
public:
    CARAPI GetIssuerAndSerialNumber(
        /* [out] */ IASN1Sequence** issuerAndSerialNumber);

    CARAPI GetAsn1(
        /* [out] */ IASN1Sequence** asn1);
};

} // namespace Pkcs7
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CSIGNERINFOHELPER_H__