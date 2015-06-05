
#include "CResponseProcessCookies.h"
#include <elastos/Logger.h>

using Elastos::Utility::ICollection;
using Elastos::Utility::IIterable;
using Elastos::Utility::IIterator;
using Elastos::Utility::IList;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHeader;
using Org::Apache::Http::Cookie::ISM;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Protocol {

CAR_INTERFACE_IMPL(CResponseProcessCookies, Object, IHttpResponseInterceptor)

CAR_OBJECT_IMPL(CResponseProcessCookies)

ECode CResponseProcessCookies::Process(
    /* [in] */ IHttpResponse* response,
    /* [in] */ IHttpContext* contexT)
{
    if (response == NULL) {
        Logger::E("CResponseProcessCookies", "HTTP response may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (context == NULL) {
        Logger::E("CResponseProcessCookies", "HTTP context may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // Obtain cookie store
    AutoPtr<IObject> o;
    context->GetAttribute(IClientContext::COOKIE_STORE, (IObject**)&o);
    AutoPtr<ICookieStore> cookieStore = ICookieStore::Probe(o);
    if (cookieStore == NULL) {
        Logger::E("CResponseProcessCookies", "Cookie store not available in HTTP context");
        // this.log.info("Cookie store not available in HTTP context");
        return NOERROR;
    }
    // Obtain actual CookieSpec instance
    AutoPtr<IObject> o;
    context->GetAttribute(IClientContext::COOKIE_SPEC, (IObject**)&o);
    AutoPtr<ICookieSpec> cookieSpec = ICookieSpec::Probe(o);
    if (cookieSpec == NULL) {
        Logger::E("CResponseProcessCookies", "CookieSpec not available in HTTP context");
        // this.log.info("CookieSpec not available in HTTP context");
        return NOERROR;
    }
    // Obtain actual CookieOrigin instance
    AutoPtr<IObject> o;
    context->GetAttribute(IClientContext::COOKIE_ORIGIN, (IObject**)&o);
    AutoPtr<ICookieOrigin> cookieOrigin = ICookieOrigin::Probe(o);
    if (cookieOrigin == NULL) {
        Logger::E("CResponseProcessCookies", "CookieOrigin not available in HTTP context");
        // this.log.info("CookieOrigin not available in HTTP context");
        return NOERROR;
    }
    AutoPtr<IHeaderIterator> it;
    response->HeaderIterator(ISM::SET_COOKIE, (IHeaderIterator**)&it);
    ProcessCookies(it, cookieSpec, cookieOrigin, cookieStore);

    // see if the cookie spec supports cookie versioning.
    Int32 ver;
    if (cookieSpec->GetVersion(&ver), ver > 0) {
        // process set-cookie2 headers.
        // Cookie2 will replace equivalent Cookie instances
        it = NULL;
        response->HeaderIterator(ISM::SET_COOKIE2, (IHeaderIterator**)&it);
        ProcessCookies(it, cookieSpec, cookieOrigin, cookieStore);
    }
    return NOERROR;
}

void CResponseProcessCookies::ProcessCookies(
    /* [in] */ IHeaderIterator* iterator,
    /* [in] */ ICookieSpec* cookieSpec,
    /* [in] */ ICookieOrigin* cookieOrigin,
    /* [in] */ ICookieStore* cookieStore)
{
    Boolean hasNext;
    while(iterator->HasNext(&hasNext), hasNext) {
        AutoPtr<IHeader> header;
        iterator->NextHeader((IHeader**)&header);
        // try {
        AutoPtr<IList> cookies;
        cookieSpec->Parse(header, cookieOrigin, (IList**)&cookies);
        AutoPtr<IIterable> iterable = IIterable::Probe(cookies);
        AutoPtr<IIterator> it;
        iterable->GetIterator((IIterator**)&it);
        Boolean result;
        while(it->HasNext(&result), result) {
            AutoPtr<ICookie> cookie;
            it->Next((IInterface**)&cookie);
            // try {
            if (FAILED(cookieSpec->Validate(cookie, cookieOrigin))) {
                Logger::E("CResponseProcessCookies", "Cookie rejected: ");
                return NOERROR;
            }
            cookieStore->AddCookie(cookie);

            // if (this.log.isDebugEnabled()) {
            //     // BEGIN android-changed
            //     this.log.debug("Cookie accepted: \""
            //             + cookieToString(cookie) + "\". ");
            //     // END android-changed
            // }
            // } catch (MalformedCookieException ex) {
            //     if (this.log.isWarnEnabled()) {
            //         // BEGIN android-changed
            //         this.log.warn("Cookie rejected: \""
            //                 + cookieToString(cookie) + "\". " + ex.getMessage());
            //         // END android-changed
            //     }
            // }
        }
        // } catch (MalformedCookieException ex) {
        //     if (this.log.isWarnEnabled()) {
        //         this.log.warn("Invalid cookie header: \""
        //                 + header + "\". " + ex.getMessage());
        //     }
        // }
    }
}

String CResponseProcessCookies::CookieToString(
    /* [in] */ ICookie* cookie)
{
    return String(NULL);
    // return cookie.getClass().getSimpleName()
    //         + "[version=" + cookie.getVersion()
    //         + ",name=" + cookie.getName()
    //         + ",domain=" + cookie.getDomain()
    //         + ",path=" + cookie.getPath()
    //         + ",expiry=" + cookie.getExpiryDate()
    //         + "]";
}

ECode CResponseProcessCookies::constructor()
{
    return NOERROR;
}

} // namespace Protocol
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org