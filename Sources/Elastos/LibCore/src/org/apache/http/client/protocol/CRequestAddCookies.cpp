
#include "CRequestAddCookies.h"
#include "CCookieOrigin.h"
#include "params/HttpClientParams.h"
#include <elastos/Logger.h>

using Elastos::Net::IURI;
using Elastos::Net::CURI;
using Elastos::Utility::ICollection;
using Elastos::Utility::IList;
using Elastos::Utility::CArrayList;
using Elastos::Utility::IArrayList;
using Elastos::Utility::IIterable;
using Elastos::Utility::IIterator;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHeader;
using Org::Apache::Http::IHttpHost;
using Org::Apache::Http::Client::ICookieStore;
using Org::Apache::Http::Client::Methods::IHttpUriRequest;
using Org::Apache::Http::Client::Params::HttpClientParams;
using Org::Apache::Http::Conn::IManagedClientConnection;
using Org::Apache::Http::Cookie::ICookie;
using Org::Apache::Http::Cookie::ICookieOrigin;
using Org::Apache::Http::Cookie::CCookieOrigin;
using Org::Apache::Http::Cookie::ICookieSpec;
using Org::Apache::Http::Cookie::ICookieSpecRegistry;
using Org::Apache::Http::Protocol::IExecutionContext;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Protocol {

CAR_INTERFACE_IMPL(CRequestAddCookies, Object, IHttpRequestInterceptor)

CAR_OBJECT_IMPL(CRequestAddCookies)

ECode CRequestAddCookies::Process(
    /* [in] */ IHttpRequest* request,
    /* [in] */ IHttpContext* contexT)
{
    if (request == NULL) {
        Logger::E("CRequestAddCookies", "HTTP request may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (context == NULL) {
        Logger::E("CRequestAddCookies", "HTTP context may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // Obtain cookie store
    AutoPtr<IObject> o;
    context->GetAttribute(IClientContext::COOKIE_STORE, (IObject**)&o);
    AutoPtr<ICookieStore> cookieStore = ICookieStore::Probe(o);
    if (cookieStore == NULL) {
        Logger::E("CRequestAddCookies", "Cookie store not available in HTTP context");
        // this.log.info("Cookie store not available in HTTP context");
        return NOERROR;
    }

    // Obtain the registry of cookie specs
    o = NULL;
    context->GetAttribute(IClientContext::COOKIESPEC_REGISTRY, (IObject**)&o);
    AutoPtr<ICookieSpecRegistry> registry = ICookieSpecRegistry::Probe(o);
    if (registry == NULL) {
        Logger::E("CRequestAddCookies", "CookieSpec registry not available in HTTP context");
        // this.log.info("CookieSpec registry not available in HTTP context");
        return NOERROR;
    }

    // Obtain the target host (required)
    o = NULL;
    context->GetAttribute(IClientContext::HTTP_TARGET_HOST, (IObject**)&o);
    AutoPtr<IHttpHost> targetHost = IHttpHost::Probe(o);
    if (targetHost == NULL) {
        Logger::E("CRequestAddCookies", "Target host not specified in HTTP context");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // Obtain the client connection (required)
    o = NULL;
    context->GetAttribute(IClientContext::HTTP_CONNECTION, (IObject**)&o);
    AutoPtr<IManagedClientConnection> conn = IManagedClientConnection::Probe(o);
    if (conn == NULL) {
        Logger::E("CRequestAddCookies", "Client connection not specified in HTTP context");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IHttpParams> params;
    request->GetParams((IHttpParams**)&params);
    String policy;
    HttpClientParams::GetCookiePolicy(params, &policy);
    // if (this.log.isDebugEnabled()) {
    //     this.log.debug("CookieSpec selected: " + policy);
    // }

    AutoPtr<IURI> requestURI;
    if (IHttpUriRequest::Probe(request) != NULL) {
        IHttpUriRequest::Probe(request)->GetURI((IURI**)&requestURI);
    }
    else {
        // try {
        AutoPtr<IRequestLine> requestLine;
        request->GetRequestLine((IRequestLine**)&requestLine);
        String uri;
        requestLine->GetUri(&uri);
        if (FAILED(CURI::New(uri, (IURI**)&requestURI))) {
            Logger::E("CRequestAddCookies", "Invalid request URI: %s", uri.string());
            return E_PROTOCOL_EXCEPTION;
        }
        // } catch (URISyntaxException ex) {
        //     throw new ProtocolException("Invalid request URI: " +
        //             request.getRequestLine().getUri(), ex);
        // }
    }

    String hostName;
    targetHost->GetHostName(&hostName);
    Int32 port;
    targetHost->GetPort(&port);
    if (port < 0) {
        conn->GetRemotePort(&port);
    }

    String path;
    requestURI->GetPath(&path);
    Boolean isSecure;
    conn->IsSecure(&isSecure);
    AutoPtr<ICookieOrigin> cookieOrigin;
    CCookieOrigin::New(hostName, port, path, isSecure, (ICookieOrigin**)&cookieOrigin);

    // Get an instance of the selected cookie policy
    AutoPtr<IHttpParams> p;
    request->GetParams((IHttpParams**)&p);
    AutoPtr<ICookieSpec> cookieSpec;
    registry->GetCookieSpec(policy, p, (ICookieSpec**)&cookieSpec);

    // Get all cookies available in the HTTP state
    AutoPtr<IList> cs;
    cookieStore->GetCookies((IList**)&cs);
    AutoPtr<IArrayList> cookies;
    CArrayList::New(ICollection::Probe(cs), (IArrayList**)&cookies);

    // Find cookies matching the given origin
    AutoPtr<IArrayList> matchedCookies;
    CArrayList::New((IArrayList**)&matchedCookies);
    AutoPtr<ICollection> matchedCol = ICollection::Probe(matchedCookies);

    AutoPtr<IIterable> iterable = IIterable::Probe(cookies);
    AutoPtr<IIterator> it;
    iterable->GetIterator((IIterator**)&it);
    Boolean hasNext;
    while(it->HasNext(&hasNext), hasNext) {
        AutoPtr<ICookie> cookie;
        it->Next((IInterface**)&cookie);
        Boolean match;
        if (cookieSpec->Match(cookie, cookieOrigin, &match), match) {
            // if (this.log.isDebugEnabled()) {
            //     this.log.debug("Cookie " + cookie + " match " + cookieOrigin);
            // }
            matchedCol->Add(cookie);
        }
    }
    // Generate Cookie request headers
    Boolean isEmpty;
    if (matchedCol->IsEmpty(&isEmpty), !isEmpty) {
        AutoPtr<IList> headers;
        cookieSpec->FormatCookies(matchedCookies, (IList**)&headers);
        AutoPtr<IIterable> able = IIterable::Probe(headers);
        AutoPtr<IIterator> headersIt;
        able->GetIterator((IIterator**)&headersIt);
        while(headersIt->HasNext(&hasNext), hasNext) {
            AutoPtr<IHeader> header;
            headersIt->Next((IInterface**)&header);
            request->AddHeader(header);
        }
    }

    Int32 ver;
    cookieSpec->GetVersion(&ver);
    if (ver > 0) {
        Boolean needVersionHeader = FALSE;
        AutoPtr<IIterable> able = IIterable::Probe(matchedCookies);
        AutoPtr<IIterator> matchedIt;
        able->GetIterator((IIterator**)&matchedIt);
        while(matchedIt->HasNext(&hasNext), hasNext) {
            AutoPtr<ICookie> cookie;
            matchedIt->Next((IInterface**)&cookie);
            Int32 cookieVer;
            if (cookie->GetVersion(&cookieVer), ver != cookieVer) {
                needVersionHeader = TRUE;
            }
        }

        if (needVersionHeader) {
            AutoPtr<IHeader> header;
            cookieSpec->GetVersionHeader((IHeader**)&header);
            if (header != NULL) {
                // Advertise cookie version support
                request->AddHeader(header);
            }
        }
    }

    // Stick the CookieSpec and CookieOrigin instances to the HTTP context
    // so they could be obtained by the response interceptor
    context->SetAttribute(IClientContext::COOKIE_SPEC, IObject::Probe(cookieSpec));
    context->SetAttribute(IClientContext::COOKIE_ORIGIN, IObject::Probe(cookieOrigin));
    return NOERROR;
}

ECode CRequestAddCookies::constructor()
{
    return NOERROR;
}

} // namespace Protocol
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org