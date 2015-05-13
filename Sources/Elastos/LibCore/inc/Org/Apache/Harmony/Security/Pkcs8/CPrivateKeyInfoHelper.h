
#ifndef __CPRIVATEKEYINFOHELPER_H__
#define __CPRIVATEKEYINFOHELPER_H__

#include "_CPrivateKeyInfoHelper.h"

using Org::Apache::Harmony::Security::Asn1::IASN1Sequence;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Pkcs8 {

CarClass(CPrivateKeyInfoHelper)
{
public:
    CARAPI GetASN1(
        /* [out] */ IASN1Sequence** asn1);
};

} // namespace Pkcs8
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CPRIVATEKEYINFOHELPER_H__
