
#ifndef __ELASTOS_SECURITY_CERT_CPKIXCERTPATHBUILDERRESULT_H__
#define __ELASTOS_SECURITY_CERT_CPKIXCERTPATHBUILDERRESULT_H__

#include "_CPKIXCertPathBuilderResult.h"
#include "PKIXCertPathValidatorResult.h"

using Elastos::Security::IPublicKey;

namespace Elastos {
namespace Security {
namespace Cert {

CarClass(CPKIXCertPathBuilderResult), public PKIXCertPathValidatorResult
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
        /* [out] */ String * pStr);

    CARAPI GetCertPath(
        /* [out] */ ICertPath **certPath);

    CARAPI constructor(
        /* [in] */ ICertPath *certPath,
        /* [in] */ ITrustAnchor *trustAnchor,
        /* [in] */ IPolicyNode *policyTree,
        /* [in] */ IPublicKey *subjectPublicKey);

    CARAPI_(PInterface) Probe(
    /* [in] */ REIID riid);

private:
    // Built and validated certification path
    AutoPtr<ICertPath> mCertPath;
};

}
}
}

#endif // __ELASTOS_SECURITY_CERT_CPKIXCERTPATHBUILDERRESULT_H__
