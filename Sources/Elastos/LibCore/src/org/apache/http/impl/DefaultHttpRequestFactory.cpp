
#include "DefaultHttpRequestFactory.h"
#include "CBasicHttpRequest.h"
#include "CBasicHttpEntityEnclosingRequest.h"
#include <elastos/Logger.h>
#include <elastos/Core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::Message::IHttpEntityEnclosingRequest;
using Org::Apache::Http::Message::CBasicHttpEntityEnclosingRequest;
using Org::Apache::Http::Message::CBasicHttpRequest;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {

static AutoPtr< ArrayOf<String> > InitCommonMethods()
{
    AutoPtr< ArrayOf<String> > array = ArrayOf<String>::Alloc(1);
    (*array)[0] = String("GET");
    return array;
}
static const AutoPtr< ArrayOf<String> > RFC2616_COMMON_METHODS = InitCommonMethods();

static AutoPtr< ArrayOf<String> > InitEntityEncMethods()
{
    AutoPtr< ArrayOf<String> > array = ArrayOf<String>::Alloc(2);
    (*array)[0] = String("POST");
    (*array)[1] = String("PUT");
    return array;
}
static const AutoPtr< ArrayOf<String> > RFC2616_ENTITY_ENC_METHODS = InitEntityEncMethods();

static AutoPtr< ArrayOf<String> > InitSpecialMethods()
{
    AutoPtr< ArrayOf<String> > array = ArrayOf<String>::Alloc(4);
    (*array)[0] = String("HEAD");
    (*array)[1] = String("OPTIONS");
    (*array)[2] = String("DELETE");
    (*array)[3] = String("TRACE");
    return array;
}
static const AutoPtr< ArrayOf<String> > RFC2616_SPECIAL_METHODS = InitSpecialMethods;

DefaultHttpRequestFactory::DefaultHttpRequestFactory()
{}

CAR_INTERFACE_IMPL(DefaultHttpRequestFactory, Object, IHttpRequestFactory)

Boolean DefaultHttpRequestFactory::IsOneOf(
    /* [in] */ ArrayOf<String>* methods,
    /* [in] */ const String& method)
{
    for (Int32 i = 0; i < methods->GetLength(); i++) {
        if ((*methods)[i].EqualsIgnoreCase(method)) {
            return TRUE;
        }
    }
    return FALSE;
}

ECode DefaultHttpRequestFactory::NewHttpRequest(
    /* [in] */ IRequestLine* requestline,
    /* [out] */ IHttpRequest** request)
{
    VALIDATE_NOT_NULL(request)
    *request = NULL;

    if (requestline == NULL) {
        Logger::E("DefaultHttpRequestFactory", "Request line may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String method;
    requestline->GetMethod(&method);
    AutoPtr<IHttpRequest> req;
    if (IsOneOf(RFC2616_COMMON_METHODS, method)) {
        return CBasicHttpRequest::New(requestline, request);
    }
    else if (IsOneOf(RFC2616_ENTITY_ENC_METHODS, method)) {
        AutoPtr<IHttpEntityEnclosingRequest> enclosingRequest;
        CBasicHttpEntityEnclosingRequest::New(requestline, (IHttpEntityEnclosingRequest**)&enclosingRequest);
        *request = IHttpRequest::Probe(enclosingRequest);
        REFCOUNT_ADD(*request)
        return NOERROR;
    }
    else if (IsOneOf(RFC2616_SPECIAL_METHODS, method)) {
        return CBasicHttpRequest::New(requestline, request);
    }
    else {
        Logger::E("DefaultHttpRequestFactory", "%s method not supported", method.string());
        return E_METHOD_NOT_SUPPORTED_EXCEPTION;
    }
}

ECode DefaultHttpRequestFactory::NewHttpRequest(
    /* [in] */ const String& method,
    /* [in] */ const String& uri,
    /* [out] */ IHttpRequest** request)
{
    VALIDATE_NOT_NULL(request)
    *request = NULL;

    if (IsOneOf(RFC2616_COMMON_METHODS, method)) {
        return CBasicHttpRequest::New(method, uri, request);
    }
    else if (IsOneOf(RFC2616_ENTITY_ENC_METHODS, method)) {
        AutoPtr<IHttpEntityEnclosingRequest> enclosingRequest;
        CBasicHttpEntityEnclosingRequest::New(method, uri, (IHttpEntityEnclosingRequest**)&enclosingRequest);
        *request = IHttpRequest::Probe(enclosingRequest);
        REFCOUNT_ADD(*request)
        return NOERROR;
    }
    else if (IsOneOf(RFC2616_SPECIAL_METHODS, method)) {
        return CBasicHttpRequest::New(method, uri, request);
    }
    else {
        Logger::E("DefaultHttpRequestFactory", "%s method not supported", method.string());
        return E_METHOD_NOT_SUPPORTED_EXCEPTION;
    }
}

} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org