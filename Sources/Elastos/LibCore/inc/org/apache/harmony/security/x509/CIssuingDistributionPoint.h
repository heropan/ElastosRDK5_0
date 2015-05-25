
#ifndef __CISSUINGDISTRIBUTIONPOINT_H__
#define __CISSUINGDISTRIBUTIONPOINT_H__

#include "_CIssuingDistributionPoint.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CIssuingDistributionPoint)
{
public:
    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncode);

    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb,
        /* [in] */ const String& prefix);

    CARAPI DumpValueEx(
        /* [in] */ Elastos::Core::IStringBuilder * pSb);

    CARAPI SetOnlyContainsUserCerts(
        /* [in] */ Boolean onlyContainsUserCerts);

    CARAPI SetOnlyContainsCACerts(
        /* [in] */ Boolean onlyContainsCACerts);

    CARAPI SetIndirectCRL(
        /* [in] */ Boolean indirectCRL);

    CARAPI SetOnlyContainsAttributeCerts(
        /* [in] */ Boolean onlyContainsAttributeCerts);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Security::X509::IDistributionPointName * pDistributionPoint,
        /* [in] */ Org::Apache::Harmony::Security::X509::IReasonFlags * pOnlySomeReasons);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CISSUINGDISTRIBUTIONPOINT_H__
