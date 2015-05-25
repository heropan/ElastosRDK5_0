
#include "CPKIXParameters.h"

namespace Elastos {
namespace Security {
namespace Cert {


ECode CPKIXParameters::GetTrustAnchors(
    /* [out] */ ISet **trustAnchors)
{
    return PKIXParameters::GetTrustAnchors(trustAnchors);
}

ECode CPKIXParameters::SetTrustAnchors(
    /* [in] */ ISet *trustAnchors)
{
    return PKIXParameters::SetTrustAnchors(trustAnchors);
}

ECode CPKIXParameters::IsAnyPolicyInhibited(
    /* [out] */ Boolean *anyPolicyInhibited)
{
    return PKIXParameters::IsAnyPolicyInhibited(anyPolicyInhibited);
}

ECode CPKIXParameters::SetAnyPolicyInhibited(
    /* [in] */ Boolean anyPolicyInhibited)
{
    return PKIXParameters::SetAnyPolicyInhibited(anyPolicyInhibited);
}

ECode CPKIXParameters::GetCertPathCheckers(
    /* [out] */ IList **certPathCheckers)
{
    return PKIXParameters::GetCertPathCheckers(certPathCheckers);
}

ECode CPKIXParameters::SetCertPathCheckers(
    /* [in] */ IList *certPathCheckers)
{
    return PKIXParameters::SetCertPathCheckers(certPathCheckers);
}

ECode CPKIXParameters::AddCertPathChecker(
    /* [in] */ IPKIXCertPathChecker *checker)
{
    return PKIXParameters::AddCertPathChecker(checker);
}

ECode CPKIXParameters::GetCertStores(
    /* [out] */ IList **certStores)
{
    return PKIXParameters::GetCertStores(certStores);
}

ECode CPKIXParameters::SetCertStores(
    /* [in] */ IList *certStores)
{
    return PKIXParameters::SetCertStores(certStores);
}

ECode CPKIXParameters::AddCertStore(
    /* [in] */ ICertStore *store)
{
    return PKIXParameters::AddCertStore(store);
}

ECode CPKIXParameters::GetDate(
    /* [out] */ IDate **date)
{
    return PKIXParameters::GetDate(date);
}

ECode CPKIXParameters::SetDate(
    /* [in] */ IDate *date)
{
    return PKIXParameters::SetDate(date);
}

ECode CPKIXParameters::IsExplicitPolicyRequired(
    /* [out] */ Boolean *explicitPolicyRequired)
{
    return PKIXParameters::IsExplicitPolicyRequired(explicitPolicyRequired);
}

ECode CPKIXParameters::SetExplicitPolicyRequired(
    /* [in] */ Boolean explicitPolicyRequired)
{
    return PKIXParameters::SetExplicitPolicyRequired(explicitPolicyRequired);
}

ECode CPKIXParameters::GetInitialPolicies(
    /* [out] */ ISet **initialPolicies)
{
    return PKIXParameters::GetInitialPolicies(initialPolicies);
}

ECode CPKIXParameters::SetInitialPolicies(
    /* [in] */ ISet *initialPolicies)
{
    return PKIXParameters::SetInitialPolicies(initialPolicies);
}

ECode CPKIXParameters::IsPolicyMappingInhibited(
    /* [out] */ Boolean *policyMappingInhibited)
{
    return PKIXParameters::IsPolicyMappingInhibited(policyMappingInhibited);
}

ECode CPKIXParameters::SetPolicyMappingInhibited(
    /* [in] */ Boolean policyMappingInhibited)
{
    return PKIXParameters::SetPolicyMappingInhibited(policyMappingInhibited);
}

ECode CPKIXParameters::GetPolicyQualifiersRejected(
    /* [out] */ Boolean *policyQualifiersRejected)
{
    return PKIXParameters::GetPolicyQualifiersRejected(policyQualifiersRejected);
}

ECode CPKIXParameters::SetPolicyQualifiersRejected(
    /* [in] */ Boolean policyQualifiersRejected)
{
    return PKIXParameters::SetPolicyQualifiersRejected(policyQualifiersRejected);
}

ECode CPKIXParameters::IsRevocationEnabled(
    /* [out] */ Boolean *revocationEnabled)
{
    return PKIXParameters::IsRevocationEnabled(revocationEnabled);
}

ECode CPKIXParameters::SetRevocationEnabled(
    /* [in] */ Boolean revocationEnabled)
{
    return PKIXParameters::SetRevocationEnabled(revocationEnabled);
}

ECode CPKIXParameters::GetSigProvider(
    /* [out] */ String *sigProvider)
{
    return PKIXParameters::GetSigProvider(sigProvider);
}

ECode CPKIXParameters::SetSigProvider(
    /* [in] */ const String& sigProvider)
{
    return PKIXParameters::SetSigProvider(sigProvider);
}

ECode CPKIXParameters::GetTargetCertConstraints(
    /* [out] */ ICertSelector **targetCertConstraints)
{
    return PKIXParameters::GetTargetCertConstraints(targetCertConstraints);
}

ECode CPKIXParameters::SetTargetCertConstraints(
    /* [in] */ ICertSelector *targetCertConstraints)
{
    return PKIXParameters::SetTargetCertConstraints(targetCertConstraints);
}

ECode CPKIXParameters::Clone(
    /* [out] */ IInterface **obj)
{
    return PKIXParameters::Clone(obj);
}

ECode CPKIXParameters::ToString(
    /* [out] */ String *str)
{
    return PKIXParameters::ToString(str);
}

ECode CPKIXParameters::constructor()
{
    return NOERROR;
}

ECode CPKIXParameters::constructor(
    /* [in] */ ISet *trustAnchors)
{
    return PKIXParameters::Init(trustAnchors);
}

ECode CPKIXParameters::constructor(
    /* [in] */ IKeyStore *keyStore)
{
    return PKIXParameters::Init(keyStore);
}

PInterface CPKIXParameters::Probe(
    /* [in] */ REIID riid)
{
    return _CPKIXParameters::Probe(riid);
}


}
}
}

