
#ifndef __CACCESSCONTROLCONTEXT_H__
#define __CACCESSCONTROLCONTEXT_H__

#include "_CAccessControlContext.h"

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

#endif // __CACCESSCONTROLCONTEXT_H__
