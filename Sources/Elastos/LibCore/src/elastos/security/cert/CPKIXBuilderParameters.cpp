
#include "CPKIXBuilderParameters.h"
#include "StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Security {
namespace Cert {

CPKIXBuilderParameters::CPKIXBuilderParameters()
    : mMaxPathLength(5)
{}

ECode CPKIXBuilderParameters::GetTrustAnchors(
    /* [out] */ ISet **trustAnchors)
{
    return PKIXParameters::GetTrustAnchors(trustAnchors);
}

ECode CPKIXBuilderParameters::SetTrustAnchors(
    /* [in] */ ISet *trustAnchors)
{
    return PKIXParameters::SetTrustAnchors(trustAnchors);
}

ECode CPKIXBuilderParameters::IsAnyPolicyInhibited(
    /* [out] */ Boolean *anyPolicyInhibited)
{
    return PKIXParameters::IsAnyPolicyInhibited(anyPolicyInhibited);
}

ECode CPKIXBuilderParameters::SetAnyPolicyInhibited(
    /* [in] */ Boolean anyPolicyInhibited)
{
    return PKIXParameters::SetAnyPolicyInhibited(anyPolicyInhibited);
}

ECode CPKIXBuilderParameters::GetCertPathCheckers(
    /* [out] */ IList **certPathCheckers)
{
    return PKIXParameters::GetCertPathCheckers(certPathCheckers);
}

ECode CPKIXBuilderParameters::SetCertPathCheckers(
    /* [in] */ IList *certPathCheckers)
{
    return PKIXParameters::SetCertPathCheckers(certPathCheckers);
}

ECode CPKIXBuilderParameters::AddCertPathChecker(
    /* [in] */ IPKIXCertPathChecker *checker)
{
    return PKIXParameters::AddCertPathChecker(checker);
}

ECode CPKIXBuilderParameters::GetCertStores(
    /* [out] */ IList **certStores)
{
    return PKIXParameters::GetCertStores(certStores);
}

ECode CPKIXBuilderParameters::SetCertStores(
    /* [in] */ IList *certStores)
{
    return PKIXParameters::SetCertStores(certStores);
}

ECode CPKIXBuilderParameters::AddCertStore(
    /* [in] */ ICertStore *store)
{
    return PKIXParameters::AddCertStore(store);
}

ECode CPKIXBuilderParameters::GetDate(
    /* [out] */ IDate **date)
{
    return PKIXParameters::GetDate(date);
}

ECode CPKIXBuilderParameters::SetDate(
    /* [in] */ IDate *date)
{
    return PKIXParameters::SetDate(date);
}

ECode CPKIXBuilderParameters::IsExplicitPolicyRequired(
    /* [out] */ Boolean *explicitPolicyRequired)
{
    return PKIXParameters::IsExplicitPolicyRequired(explicitPolicyRequired);
}

ECode CPKIXBuilderParameters::SetExplicitPolicyRequired(
    /* [in] */ Boolean explicitPolicyRequired)
{
    return PKIXParameters::SetExplicitPolicyRequired(explicitPolicyRequired);
}

ECode CPKIXBuilderParameters::GetInitialPolicies(
    /* [out] */ ISet **initialPolicies)
{
    return PKIXParameters::GetInitialPolicies(initialPolicies);
}

ECode CPKIXBuilderParameters::SetInitialPolicies(
    /* [in] */ ISet *initialPolicies)
{
    return PKIXParameters::SetInitialPolicies(initialPolicies);
}

ECode CPKIXBuilderParameters::IsPolicyMappingInhibited(
    /* [out] */ Boolean *policyMappingInhibited)
{
    return PKIXParameters::IsPolicyMappingInhibited(policyMappingInhibited);
}

ECode CPKIXBuilderParameters::SetPolicyMappingInhibited(
    /* [in] */ Boolean policyMappingInhibited)
{
    return PKIXParameters::SetPolicyMappingInhibited(policyMappingInhibited);
}

ECode CPKIXBuilderParameters::GetPolicyQualifiersRejected(
    /* [out] */ Boolean *policyQualifiersRejected)
{
    return PKIXParameters::GetPolicyQualifiersRejected(policyQualifiersRejected);
}

ECode CPKIXBuilderParameters::SetPolicyQualifiersRejected(
    /* [in] */ Boolean policyQualifiersRejected)
{
    return PKIXParameters::SetPolicyQualifiersRejected(policyQualifiersRejected);
}

ECode CPKIXBuilderParameters::IsRevocationEnabled(
    /* [out] */ Boolean *revocationEnabled)
{
    return PKIXParameters::IsRevocationEnabled(revocationEnabled);
}

ECode CPKIXBuilderParameters::SetRevocationEnabled(
    /* [in] */ Boolean revocationEnabled)
{
    return PKIXParameters::SetRevocationEnabled(revocationEnabled);
}

ECode CPKIXBuilderParameters::GetSigProvider(
    /* [out] */ String *sigProvider)
{
    return PKIXParameters::GetSigProvider(sigProvider);
}

ECode CPKIXBuilderParameters::SetSigProvider(
    /* [in] */ const String& sigProvider)
{
    return PKIXParameters::SetSigProvider(sigProvider);
}

ECode CPKIXBuilderParameters::GetTargetCertConstraints(
    /* [out] */ ICertSelector **targetCertConstraints)
{
    return PKIXParameters::GetTargetCertConstraints(targetCertConstraints);
}

ECode CPKIXBuilderParameters::SetTargetCertConstraints(
    /* [in] */ ICertSelector *targetCertConstraints)
{
    return PKIXParameters::SetTargetCertConstraints(targetCertConstraints);
}

ECode CPKIXBuilderParameters::Clone(
    /* [out] */ IInterface **obj)
{
    return PKIXParameters::Clone(obj);
}

ECode CPKIXBuilderParameters::ToString(
    /* [out] */ String *str)
{
    StringBuilder sb("[\n");
    String base;
    PKIXParameters::ToString(&base);
    sb.AppendString(base);
    sb.AppendCStr(" Max Path Length: ");
    sb.AppendInt32(mMaxPathLength);
    sb.AppendCStr("\n]");
    return sb.ToString(str);

}

ECode CPKIXBuilderParameters::GetMaxPathLength(
    /* [out] */ Int32 *length)
{
    VALIDATE_NOT_NULL(length)
    *length = mMaxPathLength;
    return NOERROR;
}

ECode CPKIXBuilderParameters::SetMaxPathLength(
    /* [in] */ Int32 maxPathLength)
{
    if (mMaxPathLength < -1) {
        return E_INVALID_PARAMETER_EXCEPTION;
    }
    mMaxPathLength = maxPathLength;
    return NOERROR;
}

ECode CPKIXBuilderParameters::constructor()
{
    return NOERROR;
}

ECode CPKIXBuilderParameters::constructor(
    /* [in] */ ISet *trustAnchors,
    /* [in] */ ICertSelector *targetConstraints)
{
    PKIXParameters::Init(trustAnchors);
    return PKIXParameters::SetTargetCertConstraints(targetConstraints);
}

ECode CPKIXBuilderParameters::constructor(
    /* [in] */ IKeyStore *keyStore,
    /* [in] */ ICertSelector *targetConstraints)
{
    PKIXParameters::Init(keyStore);
    return PKIXParameters::SetTargetCertConstraints(targetConstraints);
}

PInterface CPKIXBuilderParameters::Probe(
    /* [in] */ REIID riid)
{
    return _CPKIXBuilderParameters::Probe(riid);
}

}
}
}

