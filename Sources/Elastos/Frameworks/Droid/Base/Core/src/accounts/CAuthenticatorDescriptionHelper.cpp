
#include "ext/frameworkdef.h"
#include "accounts/CAuthenticatorDescriptionHelper.h"
#include "accounts/CAuthenticatorDescription.h"

namespace Elastos {
namespace Droid {
namespace Accounts {

ECode CAuthenticatorDescriptionHelper::NewKey(
    /* [in] */ const String& type,
    /* [out] */ IAuthenticatorDescription** description)
{
    VALIDATE_NOT_NULL(description);
    return CAuthenticatorDescription::NewKey(type, description);
}

} // namespace Accounts
} // namespace Droid
} // namespace Elastos
