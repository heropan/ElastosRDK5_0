
#include "DefaultRedirectHandler.h"
#include "RedirectLocations.h"
#include "CHttpHost.h"
#include "utils/URIUtils.h"
#include <elastos/Logger.h>

using Elastos::Net::IURI;
using Elastos::Net::CURI;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IStatusLIine;
using Org::Apache::Http::IRequestLIine;
using Org::Apache::Http::IHttpStatus;
using Org::Apache::Http::IHttpRequest;
using Org::Apache::Http::IHeader;
using Org::Apache::Http::IHttpParams;
using Org::Apache::Http::IHttpHost;
using Org::Apache::Http::CHttpHost;
using Org::Apache::Http::Client::Params::IClientPNames;
using Org::Apache::Http::Client::Utils::URIUtils;
using Org::Apache::Http::Protocol::IExecutionContext;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Client {

const String DefaultRedirectHandler::REDIRECT_LOCATIONS("http.protocol.redirect-locations");

CAR_INTERFACE_IMPL(DefaultRedirectHandler, Object, IRedirectHandler)

ECode DefaultRedirectHandler::IsRedirectRequested(
    /* [in] */ IHttpResponse* response,
    /* [in] */ IHttpContext* context,
    /* [out] */ Boolean* redirected)
{
    VALIDATE_NOT_NULL(redirected)
    *redirected = FALSE;

    if (response == NULL) {
        Logger::E("DefaultRedirectHandler", "HTTP response may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IStatusLIine> statusLine;
    response->GetStatusLine((IStatusLIine**)&statusLine);
    Int32 status;
    statusLine->GetStatusCode(&status);
    switch (statusCode) {
        case IHttpStatus::SC_MOVED_TEMPORARILY:
        case IHttpStatus::SC_MOVED_PERMANENTLY:
        case IHttpStatus::SC_SEE_OTHER:
        case IHttpStatus::SC_TEMPORARY_REDIRECT:
            *redirected = TRUE;
        default:
            *redirected = FALSE;
    }
    return NOERROR;
}

ECode DefaultRedirectHandler::GetLocationURI(
    /* [in] */ IHttpResponse* response,
    /* [in] */ IHttpContext* context,
    /* [out] */ IURI** _uri)
{
    VALIDATE_NOT_NULL(_uri)
    *_uri = NULL;

    if (response == NULL) {
        Logger::E("DefaultRedirectHandler", "HTTP response may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    //get the location header to find out where to redirect to
    AutoPtr<IHeader> locationHeader;
    response->GetFirstHeader(String("location"), (IHeader**)&locationHeader);
    if (locationHeader == NULL) {
        // got a redirect response, but no location header
        AutoPtr<IStatusLIine> statusLine;
        response->GetStatusLine((IStatusLIine**)&statusLine);
        Logger::E("DefaultRedirectHandler", "Received redirect response %p  but no location header", statusLine.Get());
        return E_PROTOCOL_EXCEPTION;
    }
    String location;
    locationHeader->GetValue(&location);
    // if (this.log.isDebugEnabled()) {
    //     this.log.debug("Redirect requested to location '" + location + "'");
    // }

    AutoPtr<IURI> uri;
    // try {
    ECode ec = CURI::New(location, (IURI**)&uri);
    if (ec == (ECode)E_URI_SYNTAX_EXCEPTION) {
        Logger::E("DefaultRedirectHandler", "Invalid redirect URI: %s", location.string());
        return E_PROTOCOL_EXCEPTION;
    }
    // } catch (URISyntaxException ex) {
    //     throw new ProtocolException("Invalid redirect URI: " + location, ex);
    // }

    AutoPtr<IHttpParams> params;
    response->GetParams((IHttpParams**)&params);
    // rfc2616 demands the location value be a complete URI
    // Location       = "Location" ":" absoluteURI
    Boolean isAbsolute;
    if (uri->IsAbsolute(&isAbsolute), !isAbsolute) {
        Boolean isParameterTrue;
        if (params->IsParameterTrue(IClientPNames::REJECT_RELATIVE_REDIRECT, &isParameterTrue), isParameterTrue) {
            Logger::E("DefaultRedirectHandler", "Relative redirect location '%p' not allowed", uri.Get());
            return E_PROTOCOL_EXCEPTION;
        }
        // Adjust location URI
        AutoPtr<IInterface> attr;
        context->GetAttribute(IExecutionContext::HTTP_TARGET_HOST, (IInterface**)&attr);
        AutoPtr<IHttpHost> target = IHttpHost::Probe(attr);
        if (target == NULL) {
            Logger::E("DefaultRedirectHandler", "Target host not available in the HTTP context");
            return E_ILLEGAL_STATE_EXCEPTION;
        }

        AutoPtr<IInterface> requestAttr;
        context->GetAttribute(IExecutionContext::HTTP_REQUEST, (IInterface**)&requestAttr);
        AutoPtr<IHttpRequest> request = IHttpRequest::Probe(requestAttr);

        // try {
        AutoPtr<IRequestLIine> requestLine;
        request->GetRequestLine((IRequestLIine**)&requestLine);
        AutoPtr<IURI> u;
        requestLine->GetUri((IURI**)&u);
        AutoPtr<IURI> requestURI;
        if (FAILED(CURI::New(u, (IURI**)&requestURI))) {
            return E_PROTOCOL_EXCEPTION;
        }
        AutoPtr<IURI> absoluteRequestURI;
        if (FAILED(URIUtils::RewriteURI(requestURI, target, TRUE, (IURI**)&absoluteRequestURI))) {
            return E_PROTOCOL_EXCEPTION;
        }
        AutoPtr<IURI> temp = uri;
        uri = NULL;
        if (FAILED(URIUtils::Resolve(absoluteRequestURI, temp, (IURI**)&uri))) {
            return E_PROTOCOL_EXCEPTION;
        }
        // } catch (URISyntaxException ex) {
        //     throw new ProtocolException(ex.getMessage(), ex);
        // }
    }

    Boolean isParameterFalse;
    if (params->IsParameterFalse(IClientPNames::ALLOW_CIRCULAR_REDIRECTS, &isParameterFalse), isParameterFalse) {
        AutoPtr<IInterface> attr;
        context->GetAttribute(REDIRECT_LOCATIONS, (IInterface**)&attr);
        AutoPtr<RedirectLocations> redirectLocations;
        if (attr == NULL) {
            redirectLocations = new RedirectLocations();
            context->SetAttribute(REDIRECT_LOCATIONS, (IInterface*)redirectLocations);
        }
        else {
            redirectLocations = (RedirectLocations*)attr;
        }

        AutoPtr<IURI> redirectURI;
        String fragment;
        if (uri->GetFragment(&fragment), !fragment.IsNull()) {
            // try {
            String host, scheme;
            uri->GetHost(&host);
            uri->GetScheme(&scheme);
            Int32 port;
            uri->GetPort(&port);
            AutoPtr<IHttpHost> target;
            CHttpHost::New(host, port, scheme, (IHttpHost**)&target);
            if (FAILED(URIUtils::RewriteURI(uri, target, TRUE, (IURI**)&redirectURI))) {
                return E_PROTOCOL_EXCEPTION;
            }
            // } catch (URISyntaxException ex) {
            //     throw new ProtocolException(ex.getMessage(), ex);
            // }
        }
        else {
            redirectURI = uri;
        }

        Boolean contains;
        if (redirectLocations->Contains(redirectURI, &contains), contains) {
            Logger::E("DefaultRedirectHandler", "Circular redirect to '%p'", redirectURI.Get());
            return E_CIRCULAR_REDIRECT_EXCEPTION;
        }
        else {
            redirectLocations->Add(redirectURI);
        }
    }

    *_uri = uri;
    REFCOUNT_ADD(*_uri)
    return NOERROR;
}

} // namespace Client
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org