
#include "Permission.h"
#include "cmdef.h"
#include "AllPermissionCollection.h"

namespace Elastos {
namespace Security {

ECode Permission::Init(
        /* [in] */ const String& name)
{
    return NOERROR;
}

ECode Permission::GetName(
    /* [out] */ String *name) const
{
    VALIDATE_NOT_NULL(name)
    *name = String();
    return NOERROR;
}

ECode Permission::CheckGuard(
    /* [in] */ IInterface *obj)
{
    return NOERROR;
}

ECode Permission::NewPermissionCollection(
    /* [out] */ IPermissionCollection **coll)
{
    VALIDATE_NOT_NULL(coll)
    *coll = new AllPermissionCollection;
    INTERFACE_ADDREF(*coll)
    return NOERROR;
}

} // namespace Security
} // namespace Elastos
