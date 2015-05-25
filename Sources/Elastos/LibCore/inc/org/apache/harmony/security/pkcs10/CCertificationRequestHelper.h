
#ifndef __CCERTIFICATIONREQUESTHELPER_H__
#define __CCERTIFICATIONREQUESTHELPER_H__

#include "_CCertificationRequestHelper.h"

using Org::Apache::Harmony::Security::Asn1::IASN1Sequence;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Pkcs10 {

CarClass(CCertificationRequestHelper)
{
public:
    CARAPI GetASN1(
        /* [out] */ IASN1Sequence** asn1);
};

} // namespace Pkcs10
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CCERTIFICATIONREQUESTHELPER_H__
