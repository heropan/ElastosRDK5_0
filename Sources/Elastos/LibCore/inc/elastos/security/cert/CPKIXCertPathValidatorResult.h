
#ifndef __ELASTOS_SECURITY_CERT_CPKIXCERTPATHVALIDATORRESULT_H__
#define __ELASTOS_SECURITY_CERT_CPKIXCERTPATHVALIDATORRESULT_H__

#include "_Elastos_Security_Cert_CPKIXCertPathValidatorResult.h"
#include "PKIXCertPathValidatorResult.h"


using Elastos::Security::IPublicKey;

namespace Elastos {
namespace Security {
namespace Cert {

CarClass(CPKIXCertPathValidatorResult), public PKIXCertPathValidatorResult
{
public:
    CARAPI Clone(
        /* [out] */ IInterface **object);

    CARAPI GetPolicyTree(
        /* [out] */ IPolicyNode **policyTree);

    CARAPI GetPublicKey(
        /* [out] */ IPublicKey **key);

    CARAPI GetTrustAnchor(
        /* [out] */ ITrustAnchor **anchor);

    CARAPI ToString(
        /* [out] */ String *str);

    CARAPI constructor(
        /* [in] */ ITrustAnchor *trustAnchor,
        /* [in] */ IPolicyNode *policyTree,
        /* [in] */ IPublicKey *subjectPublicKey);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

}
}
}

#endif // __ELASTOS_SECURITY_CERT_CPKIXCERTPATHVALIDATORRESULT_H__
