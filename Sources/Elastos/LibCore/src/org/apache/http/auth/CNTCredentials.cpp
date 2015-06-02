1
#include "CNTCredentials.h"
#include "CNTUserPrincipal.h"
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

CAR_INTERFACE_IMPL_2(CNTCredentials, Object, INTCredentials, ICredentials)

CAR_OBJECT_IMPL(CNTCredentials)

ECode CNTCredentials::GetUserPrincipal(
    /* [out] */ IPrincipal** principal)
{
    VALIDATE_NOT_NULL(principal)
    *principal = IPrincipal::Probe(mPrincipal);
    REFCOUNT_ADD(*principal)
    return NOERROR;
}

ECode CNTCredentials::GetPassword(
    /* [out] */ String* password)
{
    VALIDATE_NOT_NULL(password)
    *password = mPassword;
    return NOERROR;
}

ECode CNTCredentials::GetUserName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    return mPrincipal->GetUserName(name);
}

ECode CNTCredentials::GetDomain(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    return mPrincipal->GetDomain(name);
}

ECode CNTCredentials::GetWorkstation(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    *name = mWorkstation;
    return NOERROR;
}

ECode CNTCredentials::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    Int32 hash = ILangUtils::HASH_SEED;
    hash = LangUtils::HashCode(hash, mPrincipal);
    hash = LangUtils::HashCode(hash, mWorkstation);
    *hashCode = hash;
    return NOERROR;
}

ECode CNTCredentials::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* equals)
{
    VALIDATE_NOT_NULL(equals)
    if (o == NULL) {
        *equals = FALSE
        return NOERROR;
    }
    if (o == this) {
        *equals = TRUE;
        return NOERROR;
    }
    if (INTCredentials::Probe(o) != NULL) {
        AutoPtr<INTCredentials> that = INTCredentials::Probe(o);
        AutoPtr<IPrincipal> thatPrincipal;
        that->GetUserPrincipal((IPrincipal**)&thatPrincipal);
        if (LangUtils::Equals(mPrincipal, thatPrincipal)) {
            String thatWorkstation;
            that->GetWorkstation(&thatWorkstation);
            if (LangUtils::Equals(mWorkstation, thatWorkstation)) {
                *equals = TRUE;
                return NOERROR;
            }
        }
    }
    *equals = FALSE;
    return NOERROR;
}

ECode CNTCredentials::ToString(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string)
    StringBuilder buffer;
    buffer.AppendCStr("[principal: ");
    buffer.AppendInterface(mPrincipal);
    buffer.AppendCStr("][workstation: ");
    buffer.AppendCStr(mWorkstation.string());
    buffer.AppendCStr("]");
    *string = buffer.ToString();
    return NOERROR;
}

ECode CNTCredentials::constructor(
    /* [in] */ const String& usernamePassword)
{
    if (usernamePassword.IsNull()) {
        Logger::E("CNTCredentials", "Username:password string may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String username;
    Int32 atColon = usernamePassword.IndexOf(':');
    if (atColon >= 0) {
        username = usernamePassword.Substring(0, atColon);
        mPassword = usernamePassword.Substring(atColon + 1);
    }
    else {
        username = usernamePassword;
        mPassword = STring(NULL);
    }
    Int32 atSlash = username.IndexOf('/');
    if (atSlash >= 0) {
        CNTUserPrincipal::New(username.Substring(0, atSlash).ToUpperCase(ILocale::ENGLISH),
                username.Substring(atSlash + 1), (INTUserPrincipal**)&mPrincipal);
    }
    else {
        CNTUserPrincipal::New(String()NULL, username.Substring(atSlash + 1), (INTUserPrincipal**)&mPrincipal);
    }
    mWorkstation = String(NULL);
    return NOERROR;
}

ECode CNTCredentials::constructor(
    /* [in] */ const String& userName,
    /* [in] */ const String& password,
    /* [in] */ const String& workstation,
    /* [in] */ const String& domain)
{
    if (userName.IsNull()) {
        Logger::E("CNTCredentials", "User name may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    CNTUserPrincipal::New(domain, userName, (INTUserPrincipal**)&mPrincipal);
    mPassword = password;
    if (!workstation.IsNull()) {
        mWorkstation = workstation.ToUpperCase(ILocale::ENGLISH);
    }
    else {
        mWorkstation = String(NULL);
    }
    return NOERROR;
}

} // namespace Auth
} // namespace Http
} // namespace Apache
} // namespace Org
