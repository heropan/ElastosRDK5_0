
#include "CAuthScope.h"
#include "LangUtils.h"
#include <StringBuilder.h>
#include <elastos/Logger.h>

using Elastos::Droid::StringBuilder;
using Elastos::Utility::Logging::Logger;
using Libcore::ICU::ILocale;
using Org::Apache::Http::Util::ILangUtils;

namespace Org {
namespace Apache {
namespace Http {
namespace Auth {

static AutoPtr<IAuthScope> InitANY()
{
    AutoPtr<CAuthScope> scope;
    CAuthScope::NewByFriend(IAuthScope::ANY_HOST, IAuthScope::ANY_PORT,
            IAuthScope::ANY_REALM, IAuthScope::ANY_SCHEME, (CAuthScope**)&scope);
    return (IAuthScope*)scope.Get();
}
const AutoPtr<IAuthScope> CAuthScope::ANY = InitANY();

CAR_INTERFACE_IMPL(CAuthScope, Object, IAuthScope)

CAR_OBJECT_IMPL(CAuthScope)

ECode CAuthScope::GetHost(
    /* [out] */ String* host)
{
    VALIDATE_NOT_NULL(host)
    *host = mHost;
    return NOERROR;
}

ECode CAuthScope::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port)
    *port = mPort;
    return NOERROR;
}

ECode CAuthScope::GetRealm(
    /* [out] */ String* realmName)
{
    VALIDATE_NOT_NULL(realmName)
    *realmName = mRealm;
    return NOERROR;
}

ECode CAuthScope::GetScheme(
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme)
    *scheme = mScheme;
    return NOERROR;
}

ECode CAuthScope::Match(
    /* [in] */ IAuthScope* that,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    Int32 factor = 0;

    String thatScheme;
    that->GetScheme(&thatScheme);
    if (LangUtils::Equals(mScheme, thatScheme)) {
        factor += 1;
    }
    else {
        if (mScheme != ANY_SCHEME && thatScheme != ANY_SCHEME) {
            *value = -1;
            return NOERROR;
        }
    }

    String thatRealm;
    that->GetRealm(&thatRealm);
    if (LangUtils::Equals(mRealm, thatRealm)) {
        factor += 2;
    }
    else {
        if (mRealm != ANY_REALM && thatRealm != ANY_REALM) {
            *value = -1;
            return NOERROR;
        }
    }

    Int32 thatPort;
    that->GetPort(&thatPort);
    if (mPort == thatPort) {
        factor += 4;
    }
    else {
        if (mPort != ANY_PORT && thatPort != ANY_PORT) {
            *value = -1;
            return NOERROR;
        }
    }

    String thatHost;
    that->GetHost(&thatHost);
    if (LangUtils::Equals(mHost, thatHost)) {
        factor += 8;
    }
    else {
        if (mHost != ANY_HOST && thatHost != ANY_HOST) {
            *value = -1;
            return NOERROR;
        }
    }

    *value = factor;
    return NOERROR;
}

ECode CAuthScope::Equals(
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
    if (IAuthScope::Probe(o) == NULL) {
        return Object::Equals(o, equals);
    }
    AutoPtr<IAuthScope> that = IAuthScope::Probe(o);
    String thatHost, thatRealm, thatScheme;
    that->GetHost(&thatHost);
    that->GetRealm(&thatRealm);
    that->GetScheme(&thatScheme);
    Boolean result;
    *equals = LangUtils::Equals(mHost, thatHost) &&
            mPort == thatPort && LangUtils::Equals(mRealm, thatRealm)
            && LangUtils::Equals(mScheme, thatScheme)
    return NOERROR;
}

ECode CAuthScope::ToString(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string)
    StringBuilder buffer;
    if (!mScheme.IsNull()) {
        buffer.AppendCStr(mScheme.ToUpperCase(ILocale::ENGLISH).string());
        buffer.AppendChar(' ');
    }
    if (!mRealm.IsNull()) {
        buffer.AppendChar('\'');
        buffer.AppendCStr(mRealm.string());
        buffer.AppendChar('\'');
    }
    else {
        buffer.Append("<any realm>");
    }
    if (!mHost.IsNull()) {
        buffer.AppendChar('@');
        buffer.AppendCStr(mHost.string());
        if (mPort >= 0) {
            buffer.AppendChar(':');
            buffer.AppendCStr(mPort.string());
        }
    }
    *string = buffer.ToString();
    return NOERROR;
}

ECode CAuthScope::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    Int32 hash = ILangUtils::HASH_SEED;
    hash = LangUtils::HashCode(hash, mHost);
    hash = LangUtils::HashCode(hash, mPort);
    hash = LangUtils::HashCode(hash, mRealm);
    hash = LangUtils::HashCode(hash, mScheme);
    *hashCode = hash;
    return NOERROR;
}

ECode CAuthScope::constructor(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& realm,
    /* [in] */ const String& scheme)
{
    mHost = host.IsNull() ? ANY_HOST: host.ToLowerCase(ILocale::ENGLISH);
    mPort = port < 0 ? ANY_PORT: port;
    mRealm = realm.IsNull() ? ANY_REALM: realm;
    mScheme = scheme.IsNull() ? ANY_SCHEME: scheme.ToUpperCase(ILocale::ENGLISH);
    return NOERROR;
}

ECode CAuthScope::constructor(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& realm)
{
    return constructor(host, port, realm, ANY_SCHEME);
}

ECode CAuthScope::constructor(
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
    return constructor(host, port, ANY_REALM, ANY_SCHEME);
}

ECode CAuthScope::constructor(
    /* [in] */ IAuthScope* authscope)
{
    if (authscope == NULL) {
        Logger::E("CAuthScope", "Scope may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    authscope->GetHost(&mHost);
    authscope->GetPort(&port);
    authscope->GetRealm(&realm);
    authscope->GetScheme(&scheme);
    return NOERROR;
}

} // namespace Auth
} // namespace Http
} // namespace Apache
} // namespace Org
