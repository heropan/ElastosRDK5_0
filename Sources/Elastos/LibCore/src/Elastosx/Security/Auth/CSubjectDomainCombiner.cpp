
#include "CSubjectDomainCombiner.h"

namespace Elastosx {
namespace Security {
namespace Auth {


ECode CSubjectDomainCombiner::Combine(
    /* [in] */ ArrayOf<IProtectionDomain*> *current,
    /* [in] */ ArrayOf<IProtectionDomain*> *assigned,
    /* [out, callee] */ ArrayOf<IProtectionDomain*> **result)
{
    return NOERROR;
}

ECode CSubjectDomainCombiner::GetSubject(
    /* [out] */ ISubject **subject)
{
    return NOERROR;
}

ECode CSubjectDomainCombiner::constructor(
    /* [in] */ ISubject *sub)
{}

}
}
}

