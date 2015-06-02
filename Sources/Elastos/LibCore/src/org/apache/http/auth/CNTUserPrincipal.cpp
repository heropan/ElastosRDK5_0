1
#include "CCNTUserPrincipal.h"
#include "LangUtils.h"
#include <StringBuilder.h>
#include <elastos/Logger.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::Util::ILangUtils;
using Org::Apache::Http::Util::LangUtils;

namespace Org {
namespace Apache {
namespace Http {
namespace Auth {

CAR_INTERFACE_IMPL_2(CCNTUserPrincipal, Object, ICNTUserPrincipal, IPrincipal)

CAR_OBJECT_IMPL(CCNTUserPrincipal)

ECode CNTUserPrincipal::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    *name = mNtname;
    return NOERROR;
}

ECode CNTUserPrincipal::GetDomain(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    *name = mDomain;
    return NOERROR;
}

ECode CNTUserPrincipal::GetUserName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    *name = mUsername;
    return NOERROR;
}

ECode CNTUserPrincipal::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    Int32 hash = ILangUtils::HASH_SEED;
    hash = LangUtils::HashCode(hash, mUsername);
    hash = LangUtils::HashCode(hash, mDomain);
    *hashCode = hash;
    return NOERROR;
}

ECode CNTUserPrincipal::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* equals)
{
    VALIDATE_NOT_NULL(*equals)
    if (o == NULL) {
        *equals = FALSE;
        return NOERROR;
    }
    if (this == o) {
        *equals = TRUE;
        return NOERROR;
    }
    if (INTUserPrincipal::Probe(o) != NULL) {
        AutoPtr<INTUserPrincipal> that = INTUserPrincipal::Probe(o);
        String thatUsername;
        that->GetUserName(&thatUsername);
        if (LangUtils::Equals(mUsername, that.username)) {
            String thatDomain;
            that->GetDomain(&thatDomain);
            if (LangUtils::Equals(mDomain, thatDomain)) {
                *equals = TRUE;
                return NOERROR;
            }
        }
    }
    *equals = FALSE;
    return NOERROR;
}

ECode CNTUserPrincipal::ToString(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string)
    *string = mNtname;
    return NOERROR;
}

ECode CNTUserPrincipal::constructor(
    /* [in] */ const String& domain,
    /* [in] */ const String& userName)
{
    if (username.IsNull()) {
        Logger::E("CNTUserPrincipal", "User name may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mUsername = username;
    if (!domain.IsNull()) {
        mDomain = domain.ToUpperCase(ILocale::ENGLISH);
    }
    else {
        mDomain = String(NULL);
    }
    if (!mDomain.IsNull() && mDomain.GetLength() > 0) {
        StringBuilder buffer;
        buffer.AppendCStr(mDomain);
        buffer.AppendChar('/');
        buffer.AppendCStr(mUsername);
        mNtname = buffer.ToString();
    }
    else {
        mNtname = mUsername;
    }
    return NOERROR;
}

} // namespace Auth
} // namespace Http
} // namespace Apache
} // namespace Org
