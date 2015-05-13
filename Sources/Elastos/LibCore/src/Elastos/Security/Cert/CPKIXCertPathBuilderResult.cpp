
#include "CPKIXCertPathBuilderResult.h"
#include "cmdef.h"
#include "StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Security {
namespace Cert {


ECode CPKIXCertPathBuilderResult::Clone(
    /* [out] */ IInterface **object)
{
    return PKIXCertPathValidatorResult::Clone(object);
}

ECode CPKIXCertPathBuilderResult::GetPolicyTree(
    /* [out] */ IPolicyNode **policyTree)
{
    return PKIXCertPathValidatorResult::GetPolicyTree(policyTree);
}

ECode CPKIXCertPathBuilderResult::GetPublicKey(
    /* [out] */ IPublicKey **key)
{
    return PKIXCertPathValidatorResult::GetPublicKey(key);
}

ECode CPKIXCertPathBuilderResult::GetTrustAnchor(
    /* [out] */ ITrustAnchor **anchor)
{
    return PKIXCertPathValidatorResult::GetTrustAnchor(anchor);
}

ECode CPKIXCertPathBuilderResult::ToString(
    /* [out] */ String *str)
{
    FAIL_RETURN(PKIXCertPathValidatorResult::ToString(str))
    StringBuilder sb(*str);
    sb.AppendCStr("\n Certification Path: ");
    String certPath;
    mCertPath->ToString(&certPath);
    sb.AppendString(certPath);
    sb.AppendCStr("\n]");
    return sb.ToString(str);
}

ECode CPKIXCertPathBuilderResult::GetCertPath(
    /* [out] */ ICertPath **certPath)
{
    VALIDATE_NOT_NULL(certPath)
    *certPath = mCertPath;
    INTERFACE_ADDREF(*certPath)
    return NOERROR;
}

ECode CPKIXCertPathBuilderResult::constructor(
    /* [in] */ ICertPath *certPath,
    /* [in] */ ITrustAnchor *trustAnchor,
    /* [in] */ IPolicyNode *policyTree,
    /* [in] */ IPublicKey *subjectPublicKey)
{
    PKIXCertPathValidatorResult::Init(trustAnchor, policyTree, subjectPublicKey);
    if (!certPath) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mCertPath = certPath;
    return NOERROR;
}

PInterface CPKIXCertPathBuilderResult::Probe(
    /* [in] */ REIID riid)
{
    return _CPKIXCertPathBuilderResult::Probe(riid);
}

}
}
}

