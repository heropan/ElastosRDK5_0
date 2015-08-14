
#ifndef __ELASTOS_SECURITY_CACCESSCONTROLCONTEXT_H__
#define __ELASTOS_SECURITY_CACCESSCONTROLCONTEXT_H__

#include "_Elastos_Security_CAccessControlContext.h"

namespace Elastos {
namespace Security {

CarClass(CAccessControlContext)
{
public:
    CARAPI CheckPermission(
        /* [in] */ IPermission *perm);

    CARAPI GetDomainCombiner(
        /* [out] */ IDomainCombiner **domCombiner);

    CARAPI constructor(
        /* [in] */ IAccessControlContext *acc,
        /* [in] */ IDomainCombiner *combiner);

    CARAPI constructor(
        /* [in] */ ArrayOf<IProtectionDomain*> *context);
};

}
}

#endif // __ELASTOS_SECURITY_CACCESSCONTROLCONTEXT_H__
