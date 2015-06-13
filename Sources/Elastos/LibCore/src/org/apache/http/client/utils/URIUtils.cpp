
#include "URIUtils.h"
#include <StringBuilder.h>
#include <elastos/Logger.h>

using Elastos::Core::StringBuilder;
using Elastos::Net::CURI;
using Elastos::Net::IURIHelper;
using Elastos::Net::CURIHelper;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Utils {

ECode URIUtils::CreateURI(
    /* [in] */ const String& scheme,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& path,
    /* [in] */ const String& query,
    /* [in] */ const String& fragment,
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri)

    StringBuilder buffer;
    if (!host.IsNull()) {
        if (!scheme.IsNull()) {
            buffer.Append(scheme);
            buffer.Append("://");
        }
        buffer.Append(host);
        if (port > 0) {
            buffer.AppendChar(':');
            buffer.Append(port);
        }
    }
    if (path.IsNull() || !path.StartsWith("/")) {
        buffer.AppendChar('/');
    }
    if (!path.IsNull()) {
        buffer.Append(path);
    }
    if (!query.IsNull()) {
        buffer.AppendChar('?');
        buffer.Append(query);
    }
    if (!fragment.IsNull()) {
        buffer.AppendChar('#');
        buffer.Append(fragment);
    }
    return CURI::New(buffer.ToString(), uri);
}

ECode URIUtils::RewriteURI(
    /* [in] */ IURI* inUri,
    /* [in] */ IHttpHost* target,
    /* [in] */ Boolean dropFragment,
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri)
    if (inUri == NULL) {
        Logger::E("URIUtils", "URI may nor be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (target != NULL) {
        String schemeName, hostName, path, query;
        target->GetSchemeName(&schemeName);
        target->GetHostName(&hostName);
        inUri->GetRawPaht(&path);
        inUri->GetRawQuery(&query);
        Int32 port;
        target->GetPort(&port);
        String fragment(NULL);
        if (!dropFragment) {
            inUri->GetRawFragment(&fragment);
        }
        return CreateURI(schemeName, hostName, port, path, query, fragment, uri);
    }
    else {
        String path, query;
        inUri->GetRawPaht(&path);
        inUri->GetRawQuery(&query);
        String fragment(NULL);
        if (!dropFragment) {
            inUri->GetRawFragment(&fragment);
        }
        return CreateURI(String(NULL), String(NULL), -1, path, query, fragment, uri);
    }
}

ECode URIUtils::RewriteURI(
    /* [in] */ IURI* inUri,
    /* [in] */ IHttpHost* target,
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri)
    return RewriteURI(inUri, target, FALSE, uri);
}

ECode URIUtils::Resolve(
    /* [in] */ IURI* baseURI,
    /* [in] */ const String& reference,
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri)
    AutoPtr<IURIHelper> helper;
    CURIHelper::AcquireSingleton((IURIHelper**)&helper);
    AutoPtr<IURI> iuri;
    helper->Create(reference, (IURI**)&iuri);
    return Resolve(baseURI, iuri, uri);
}

ECode URIUtils::Resolve(
    /* [in] */ IURI* baseURI,
    /* [in] */ IURI* reference,
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri)
    if (baseURI == NULL) {
        Logger::E("URIUtils", "Base URI may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (reference == NULL) {
        Logger::E("URIUtils", "Reference URI may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String referenceStr;
    IObject::Probe(reference)->ToString(&referenceStr);
    Boolean emptyReference = referenceStr.GetLength() == 0;
    if (emptyReference) {
        AutoPtr<IURIHelper> helper;
        CURIHelper::AcquireSingleton((IURIHelper**)&helper);
        reference = NULL;
        helper->Create(String("#"), (IURI**)&reference);
    }
    AutoPtr<IURI> resolved;
    baseURI->Resolve(reference, (IURI**)&resolved);
    if (emptyReference) {
        String resolvedString;
        IObject::Probe(resolved)->ToString(&resolvedString);
        AutoPtr<IURIHelper> helper;
        CURIHelper::AcquireSingleton((IURIHelper**)&helper);
        resolved = NULL;
        helper->Create(resolvedString.Substring(0, resolvedString.IndexOf('#')), (IURI**)&resolved);
    }
    *uri = resolved;
    REFCOUNT_ADD(*uri)
    return NOERROR;
}

} // namespace Utils
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org