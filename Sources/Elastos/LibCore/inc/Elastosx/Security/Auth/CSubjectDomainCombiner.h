
#ifndef __CSUBJECTDOMAINCOMBINER_H__
#define __CSUBJECTDOMAINCOMBINER_H__

#include "_CSubjectDomainCombiner.h"

using Elastos::Security::IProtectionDomain;

namespace Elastosx {
namespace Security {
namespace Auth {

CarClass(CSubjectDomainCombiner)
{
public:
    CARAPI Combine(
        /* [in] */ ArrayOf<IProtectionDomain*> *current,
        /* [in] */ ArrayOf<IProtectionDomain*> *assigned,
        /* [out, callee] */ ArrayOf<IProtectionDomain*> **result);

    CARAPI GetSubject(
        /* [out] */ ISubject **subject);

    CARAPI constructor(
        /* [in] */ ISubject *sub);
};

}
}
}

#endif // __CSUBJECTDOMAINCOMBINER_H__
