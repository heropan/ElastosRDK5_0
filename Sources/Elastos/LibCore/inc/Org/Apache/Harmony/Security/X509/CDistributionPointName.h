
#ifndef __CDISTRIBUTIONPOINTNAME_H__
#define __CDISTRIBUTIONPOINTNAME_H__

#include "_CDistributionPointName.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CDistributionPointName)
{
public:
    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb,
        /* [in] */ const String& prefix);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Security::X509::IGeneralNames * pFullName);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Security::X501::IName * pNameRelativeToCRLIssuer);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CDISTRIBUTIONPOINTNAME_H__
