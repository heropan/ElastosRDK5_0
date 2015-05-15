
#ifndef __CSQLPERMISSION_H__
#define __CSQLPERMISSION_H__

#include "_CSQLPermission.h"

namespace Elastos {
namespace Sql {

CarClass(CSQLPermission)
{
public:
    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& actions);

    CARAPI GetActions(
        /* [out] */ String * str);

    // CARAPI Implies(
    //     /* [in] */ IPermission permission,
    //     /* [out] */ Boolean * value);

};

} // namespace Sql
} // namespace Elastos

#endif // __CSQLPERMISSION_H__
