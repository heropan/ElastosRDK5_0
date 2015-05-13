
#include "ext/frameworkdef.h"
#include "os/CUserManagerHelper.h"
#include "os/CUserManager.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CUserManagerHelper::SupportsMultipleUsers(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = CUserManager::SupportsMultipleUsers();
    return NOERROR;
}

ECode CUserManagerHelper::GetMaxSupportedUsers(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = CUserManager::GetMaxSupportedUsers();
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
