
#include "CBasicUserPrincipal.h"
#include "LangUtils.h"
#include <StringBuilder.h>
#include <elastos/Logger.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::Util::ILangUtils;

namespace Org {
namespace Apache {
namespace Http {
namespace Auth {

CAR_INTERFACE_IMPL_2(CBasicUserPrincipal, Object, IBasicUserPrincipal, IPrincipal)

CAR_OBJECT_IMPL(CBasicUserPrincipal)

ECode CBasicUserPrincipal::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    *name = mUsername;
    return NOERROR;
}

ECode CBasicUserPrincipal::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    Int32 hash = LangUtils.HASH_SEED;
    hash = LangUtils::HashCode(hash, mUsername);
    *hashCode = hash;
    return NOERROR;
}

ECode CBasicUserPrincipal::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* equals)
{
    VALIDATE_NOT_NULL(equals)

    if (o == NULL) {
        *equals = FALSE;
        return NOERROR;
    }
    if (this == o) {
        *equals = TRUE;
        return NOERROR;
    }
    if (IBasicUserPrincipal::Probe(o) != NULL) {
        AutoPtr<IBasicUserPrincipal> that = IBasicUserPrincipal::Probe(o);
        String thatName;
        that->GetName(&thatName);
        if (LangUtils::Equals(mUsername, thatName)) {
            *equals = TRUE;
            return NOERROR;
        }
    }
    *equals = FALSE;
    return NOERROR;
}

ECode CBasicUserPrincipal::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buffer;
    buffer.AppendCStr("[principal: ");
    buffer.AppendCStr(mUsername.string());
    buffer.AppendCStr("]");
    *str = buffer.ToString();
    return NOERROR;
}

ECode CBasicUserPrincipal::constructor(
    /* [in] */ const String& username)
{
    if (username.IsNull()) {
        Logger::E("CBasicUserPrincipal", "User name may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mUsername = username;
    return NOERROR;
}

} // namespace Auth
} // namespace Http
} // namespace Apache
} // namespace Org
