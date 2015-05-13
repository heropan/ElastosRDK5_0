
#ifndef __CCERTIFICATEPOLICIES_H__
#define __CCERTIFICATEPOLICIES_H__

#include "_CCertificatePolicies.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CCertificatePolicies)
{
public:
    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncode);

    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb,
        /* [in] */ const String& prefix);

    CARAPI DumpValueEx(
        /* [in] */ Elastos::Core::IStringBuilder * pSb);

    CARAPI GetPolicyInformations(
        /* [out] */ Elastos::Utility::IList ** ppInfomations);

    CARAPI AddPolicyInformation(
        /* [in] */ Org::Apache::Harmony::Security::X509::IPolicyInformation * pPolicyInformation,
        /* [out] */ Org::Apache::Harmony::Security::X509::ICertificatePolicies ** ppPolicies);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CCERTIFICATEPOLICIES_H__
