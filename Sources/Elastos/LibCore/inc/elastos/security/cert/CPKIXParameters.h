
#ifndef __ELASTOS_SECURITY_CERT_CPKIXPARAMETERS_H__
#define __ELASTOS_SECURITY_CERT_CPKIXPARAMETERS_H__

#include "_CPKIXParameters.h"
#include "PKIXParameters.h"

namespace Elastos {
namespace Security {
namespace Cert {

CarClass(CPKIXParameters), public PKIXParameters
{
public:
    CARAPI GetTrustAnchors(
        /* [out] */ ISet **trustAnchors);

    CARAPI SetTrustAnchors(
        /* [in] */ ISet *trustAnchors);

    CARAPI IsAnyPolicyInhibited(
        /* [out] */ Boolean *anyPolicyInhibited);

    CARAPI SetAnyPolicyInhibited(
        /* [in] */ Boolean anyPolicyInhibited);

    CARAPI GetCertPathCheckers(
        /* [out] */ IList **certPathCheckers);

    CARAPI SetCertPathCheckers(
        /* [in] */ IList *certPathCheckers);

    CARAPI AddCertPathChecker(
        /* [in] */ IPKIXCertPathChecker *checker);

    CARAPI GetCertStores(
        /* [out] */ IList **certStores);

    CARAPI SetCertStores(
        /* [in] */ IList *certStores);

    CARAPI AddCertStore(
        /* [in] */ ICertStore *store);

    CARAPI GetDate(
        /* [out] */ IDate **date);

    CARAPI SetDate(
        /* [in] */ IDate *date);

    CARAPI IsExplicitPolicyRequired(
        /* [out] */ Boolean *explicitPolicyRequired);

    CARAPI SetExplicitPolicyRequired(
        /* [in] */ Boolean explicitPolicyRequired);

    CARAPI GetInitialPolicies(
        /* [out] */ ISet **initialPolicies);

    CARAPI SetInitialPolicies(
        /* [in] */ ISet *initialPolicies);

    CARAPI IsPolicyMappingInhibited(
        /* [out] */ Boolean *policyMappingInhibited);

    CARAPI SetPolicyMappingInhibited(
        /* [in] */ Boolean policyMappingInhibited);

    CARAPI GetPolicyQualifiersRejected(
        /* [out] */ Boolean * policyQualifiersRejected);

    CARAPI SetPolicyQualifiersRejected(
        /* [in] */ Boolean policyQualifiersRejected);

    CARAPI IsRevocationEnabled(
        /* [out] */ Boolean *revocationEnabled);

    CARAPI SetRevocationEnabled(
        /* [in] */ Boolean revocationEnabled);

    CARAPI GetSigProvider(
        /* [out] */ String *sigProvider);

    CARAPI SetSigProvider(
        /* [in] */ const String& sigProvider);

    CARAPI GetTargetCertConstraints(
        /* [out] */ ICertSelector **targetCertConstraints);

    CARAPI SetTargetCertConstraints(
        /* [in] */ ICertSelector *targetCertConstraints);

    CARAPI Clone(
    /* [out] */ IInterface **obj);

    CARAPI ToString(
        /* [out] */ String *str);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ISet *trustAnchors);

    CARAPI constructor(
        /* [in] */ IKeyStore *keyStore);

    PInterface Probe(
    /* [in] */ REIID riid);
};

}
}
}

#endif // __ELASTOS_SECURITY_CERT_CPKIXPARAMETERS_H__
