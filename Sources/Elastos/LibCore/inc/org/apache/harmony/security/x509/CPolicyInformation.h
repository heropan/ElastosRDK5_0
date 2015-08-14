
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CPOLICYINFORMATION_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CPOLICYINFORMATION_H__

#include "_CPolicyInformation.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CPolicyInformation)
{
public:
    CARAPI GetPolicyIdentifier(
        /* [out] */ String * pPolicyIdentifier);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncoded);

    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb);

    CARAPI constructor(
        /* [in] */ const String& policyIdentifier);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CPOLICYINFORMATION_H__
