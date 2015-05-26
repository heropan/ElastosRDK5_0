
#ifndef __CSQLPERMISSION_H__
#define __CSQLPERMISSION_H__

#include "_Elastos_Sql_CSQLPermission.h"

namespace Elastos {
namespace Sql {

CarClass(CSQLPermission)
    , public BasicPermission
    , public ISQLPermission
{
public:
    CAR_OBJECT_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& actions);

    CARAPI GetActions(
        /* [out] */ String * str);

    CARAPI Implies(
        /* [in] */ IPermission* permission,
        /* [out] */ Boolean * value);

};

} // namespace Sql
} // namespace Elastos

#endif // __CSQLPERMISSION_H__
