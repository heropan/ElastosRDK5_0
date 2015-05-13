
#include "CAccessControlContext.h"

namespace Elastos {
namespace Security {


ECode CAccessControlContext::CheckPermission(
    /* [in] */ IPermission *perm)
{
    return NOERROR;
}

ECode CAccessControlContext::GetDomainCombiner(
    /* [out] */ IDomainCombiner **domCombiner)
{
    return NOERROR;
}

ECode CAccessControlContext::constructor(
    /* [in] */ IAccessControlContext *acc,
    /* [in] */ IDomainCombiner *combiner)
{
    return NOERROR;
}

ECode CAccessControlContext::constructor(
    /* [in] */ ArrayOf<IProtectionDomain*> *context)
{
    return NOERROR;
}

} // namespace Security
} // namespace Elastos

