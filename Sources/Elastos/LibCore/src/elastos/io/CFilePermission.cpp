
#include "CFilePermission.h"
#include "AllPermissionCollection.h"

using Elastos::Security::AllPermissionCollection;
using Elastos::Security::EIID_IGuard;

namespace Elastos {
namespace IO {

CAR_OBJECT_IMPL(CFilePermission)

CAR_INTERFACE_IMPL_3(CFilePermission, Object, IFilePermission, IPermission, IGuard)

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
    REFCOUNT_ADD(*object)
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

ECode CFilePermission::CheckGuard(
    /* [in] */ IInterface* object)
{
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
