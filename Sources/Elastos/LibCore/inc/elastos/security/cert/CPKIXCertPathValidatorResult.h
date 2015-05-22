
#ifndef __CPKIXCERTPATHVALIDATORRESULT_H__
#define __CPKIXCERTPATHVALIDATORRESULT_H__

#include "_CPKIXCertPathValidatorResult.h"
#include "PKIXCertPathValidatorResult.h"
#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

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

#endif // __CPKIXCERTPATHVALIDATORRESULT_H__
