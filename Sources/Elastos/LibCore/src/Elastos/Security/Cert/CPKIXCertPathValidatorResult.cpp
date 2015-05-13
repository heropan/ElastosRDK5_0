
#include "CPKIXCertPathValidatorResult.h"
#include "cmdef.h"

namespace Elastos {
namespace Security {
namespace Cert {


ECode CPKIXCertPathValidatorResult::Clone(
    /* [out] */ IInterface **object)
{
    return PKIXCertPathValidatorResult::Clone(object);
}

ECode CPKIXCertPathValidatorResult::GetPolicyTree(
    /* [out] */ IPolicyNode **policyTree)
{
    return PKIXCertPathValidatorResult::GetPolicyTree(policyTree);
}

ECode CPKIXCertPathValidatorResult::GetPublicKey(
    /* [out] */ IPublicKey **key)
{
    return PKIXCertPathValidatorResult::GetPublicKey(key);
}

ECode CPKIXCertPathValidatorResult::GetTrustAnchor(
    /* [out] */ ITrustAnchor **anchor)
{
    return PKIXCertPathValidatorResult::GetTrustAnchor(anchor);
}

ECode CPKIXCertPathValidatorResult::ToString(
    /* [out] */ String *str)
{
    return PKIXCertPathValidatorResult::ToString(str);
}

ECode CPKIXCertPathValidatorResult::constructor(
    /* [in] */ ITrustAnchor *trustAnchor,
    /* [in] */ IPolicyNode *policyTree,
    /* [in] */ IPublicKey *subjectPublicKey)
{
    return PKIXCertPathValidatorResult::Init(trustAnchor, policyTree, subjectPublicKey);
}

PInterface CPKIXCertPathValidatorResult::Probe(
    /* [in] */ REIID riid)
{
    return _CPKIXCertPathValidatorResult::Probe(riid);
}

}
}
}

