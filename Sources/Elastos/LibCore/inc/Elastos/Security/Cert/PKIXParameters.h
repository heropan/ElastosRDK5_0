
#ifndef __PKIXPARAMETERS_H__
#define __PKIXPARAMETERS_H__

#ifdef ELASTOS_CORE
#include "Elastos.Core_server.h"
#else
#include "Elastos.Core.h"
#endif

using Elastos::Utility::ISet;
using Elastos::Utility::IList;
using Elastos::Utility::IDate;
using Elastos::Security::IKeyStore;

namespace Elastos {
namespace Security {
namespace Cert {

class PKIXParameters
{
public:
    PKIXParameters();

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

    CARAPI Init(
        /* [in] */ ISet *trustAnchors);

    CARAPI Init(
        /* [in] */ IKeyStore *keyStore);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

private:
    /**
     * Checks that {@code trustAnchors} contains only {@code
     * TrustAnchor} instances.
     *
     * @throws InvalidAlgorithmParameterException if trustAnchors set is empty.
     */
    CheckTrustAnchors(
        /* [in] */ ISet *trustAnchors);

private:
    // Set of trust anchors - most trusted CAs
    AutoPtr<ISet> mTrustAnchors;
    // Set of acceptable initial policy identifiers (OID strings)
    AutoPtr<ISet> mInitialPolicies;
    // List of cert stores that used to find certificates and CRLs
    AutoPtr<IList> mCertStores;
    // Time for which the validity of the certification
    // patch should be determined
    AutoPtr<IDate> mDate;
    // List of certification patch checkers (PKIXCertPathChecker)
    AutoPtr<IList> mCertPathCheckers;
    // Preferred signature provider name
    String mSigProvider;
    // Required constraints on the target certificate
    AutoPtr<ICertSelector> mTargetCertConstraints;
    // Indicates whether cert revocation is enabled or not
    Boolean mRevocationEnabled;
    // Indicates whether explicit policy required or not
    Boolean mExplicitPolicyRequired;
    // Indicates whether policy mapping inhibited or not
    Boolean mPolicyMappingInhibited;
    // Indicates whether any policy inhibited or not
    Boolean mAnyPolicyInhibited;
    // Indicates whether certificates that include policy
    // qualifiers in a certificate policies extension that
    // is marked critical must be rejected or not
    Boolean mPolicyQualifiersRejected;
};

}
}
}

#endif // __CPKIXPARAMETERS_H__
