
#include "CHttpHost.h"
#include "CCharArrayBuffer.h"
#include "LangUtils.h"
#include <elastos/Logger.h>
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;
using Libcore::ICU::ILocale;
using Org::Apache::Http::Util::ICharArrayBuffer;
using Org::Apache::Http::Util::CCharArrayBuffer;
using Org::Apache::Http::Util::ILangUtils;

namespace Org {
namespace Apache {
namespace Http {

CAR_INTERFACE_IMPL_2(CHttpHost, Object, IHttpHost, ICloneable)

CAR_OBJECT_IMPL(CHttpHost)

ECode CHttpHost::GetHostName(
    /* [out] */ String* hostName)
{
    VALIDATE_NOT_NULL(hostName)
    *hostName = mHostName;
    return NOERROR;
}

ECode CHttpHost::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port)
    *port = mPort;
    return NOERROR;
}

ECode CHttpHost::GetSchemeName(
    /* [out] */ String* sehemeName)
{
    VALIDATE_NOT_NULL(sehemeName)
    *sehemeName = mSchemeName;
    return NOERROR;
}

ECode CHttpHost::ToURI(
    /* [out] */ String* uri)
{
    VALIDATE_NOT_NULL(uri)

    AutoPtr<ICharArrayBuffer> buffer;
    CCharArrayBuffer::New(32, (ICharArrayBuffer**)&buffer);
    buffer->Append(mSchemeName);
    buffer->Append(String("://"));
    buffer->Append(mHostname);
    if (mPort != -1) {
        buffer->Append(':');
        buffer->Append(StringUtils::Int32ToString(mPort));
    }
    return buffer->ToString(uri);
}

ECode CHttpHost::ToHostString(
    /* [out] */ String* hostString)
{
    VALIDATE_NOT_NULL(hostString)

    AutoPtr<ICharArrayBuffer> buffer;
    CCharArrayBuffer::New(32, (ICharArrayBuffer**)&buffer);
    buffer->Append(mHostname);
    if (mPort != -1) {
        buffer->Append(':');
        buffer->Append(StringUtils::Int32ToString(mPort));
    }
    return buffer->ToString(uri);
}

ECode CHttpHost::ToString(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string)
    return ToURI(string);
}

ECode CHttpHost::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* equals)
{
    VALIDATE_NOT_NULL(equals)

    if (obj == NULL) {
        *equals = FALSE;
        return NOERROR;
    }
    if (this == obj) {
        *equals = TRUE;
        return NOERROR;
    }
    if (IHttpHost::Probe(obj) != NULL) {
        AutoPtr<IHttpHost> that = IHttpHost::Probe(obj);
        String thatHostName;
        that->GetHostName(&thatHostName);
        Int32 thatPort;
        that->GetPort(&thatPort);
        String thatSchemeName;
        that->GetSchemeName(&thatSchemeName);
        *equals = mLcHostname.Equals(thatHostName.ToLowerCase())
                && mPort == thatPort
                && mSchemeName.Equals(thatSchemeName);
    }
    else {
        *equals = FALSE;
    }

    return NOERROR;
}

ECode CHttpHost::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)

    Int32 hash = ILangUtils::HASH_SEED;
    hash = LangUtils::HashCode(hash, mLcHostname);
    hash = LangUtils::HashCode(hash, mPort);
    hash = LangUtils::HashCode(hash, mSchemeName);

    *hashCode = hash;
    return NOERROR;
}

ECode CHttpHost::Clone(
    /* [out] */ IInterface** object)
{
    // return super.clone();
    return E_NOT_IMPLEMENTED;
}

ECode CHttpHost::constructor(
    /* [in] */ const String& hostName,
    /* [in] */ Int32 port,
    /* [in] */ const String& scheme)
{
    if (hostname.IsNull()) {
        // Logger::E("CHttpHost", "Host name may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mHostname   = hostname;
    mLcHostname = hostname.ToLowerCase(ILocale::ENGLISH);
    if (!scheme.IsNull()) {
        mSchemeName = scheme.ToLowerCase(ILocale::ENGLISH);
    }
    else {
        mSchemeName = DEFAULT_SCHEME_NAME;
    }
    mPort = port;
    return NOERROR;
}

ECode CHttpHost::constructor(
    /* [in] */ const String& hostName,
    /* [in] */ Int32 port)
{
    return constructor(hostName, port, String(NULL));
}

ECode CHttpHost::constructor(
    /* [in] */ const String& hostName)
{
    return constructor(hostName, -1, String(NULL));
}

ECode CHttpHost::constructor(
    /* [in] */ IHttpHost* httpHost)
{
    String hostname;
    httpHost->GetHostName(&hostname);
    Int32 port;
    httpHost->GetPort(&port);
    String schemeName;
    httpHost->GetSchemeName(&schemeName);
    return constructor(hostname, port, schemeName);
}

} // namespace Http
} // namespace Apache
} // namespace Org
