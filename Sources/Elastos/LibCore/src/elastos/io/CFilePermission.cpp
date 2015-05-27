
#include "coredef.h"
#include "CFilePermission.h"
#include "AllPermissionCollection.h"

using Elastos::Security::AllPermissionCollection;

namespace Elastos {
namespace IO {

ECode CFilePermission::constructor(
    /* [in] */ const String& path,
    /* [in] */ const String& actions)
{
    return NOERROR;
}

ECode CFilePermission::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = NULL;
    return NOERROR;
}

ECode CFilePermission::NewPermissionCollection(
    /* [out] */ IPermissionCollection** object)
{
    VALIDATE_NOT_NULL(object);

    AutoPtr<IPermissionCollection> res = (IPermissionCollection*) new AllPermissionCollection();
    *object = res;
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

ECode CFilePermission::GetActions(
    /* [out] */ String* actions)
{
    VALIDATE_NOT_NULL(actions);

    *actions = NULL;
    return NOERROR;
}

ECode CFilePermission::Implies(
    /* [in] */ IPermission* p,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = TRUE;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
