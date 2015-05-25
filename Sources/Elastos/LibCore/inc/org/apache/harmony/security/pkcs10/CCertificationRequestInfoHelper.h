
#ifndef __CCERTIFICATIONREQUESTINFOHELPER_H__
#define __CCERTIFICATIONREQUESTINFOHELPER_H__

#include "_CCertificationRequestInfoHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Pkcs10 {

CarClass(CCertificationRequestInfoHelper)
{
public:
    CARAPI GetASN1(
        /* [out] */ IASN1Sequence** asn1);

private:
    // TODO: Add your private member variables here.
};

} // namespace Pkcs10
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CCERTIFICATIONREQUESTINFOHELPER_H__
