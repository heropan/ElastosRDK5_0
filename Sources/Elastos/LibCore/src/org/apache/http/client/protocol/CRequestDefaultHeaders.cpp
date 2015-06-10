
#include "CRequestDefaultHeaders.h"
#include <elastos/Logger.h>

using Elastos::Utility::ICollection;
using Elastos::Utility::IIterable;
using Elastos::Utility::IIterator;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHeader;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Protocol {

CAR_INTERFACE_IMPL(CRequestDefaultHeaders, Object, IHttpRequestInterceptor)

CAR_OBJECT_IMPL(CRequestDefaultHeaders)

ECode CRequestDefaultHeaders::Process(
    /* [in] */ IHttpRequest* request,
    /* [in] */ IHttpContext* contexT)
{
    if (request == NULL) {
        Logger::E("CRequestDefaultHeaders", "HTTP request may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // Add default headers
    AutoPtr<IHttpParams> params;
    request->GetParams((IHttpParams**)&params);
    AutoPtr<IObject> o;
    params->GetParameter(IClientPNames::DEFAULT_HEADERS, (IObject**)&o);
    AutoPtr<ICollection> defHeaders = ICollection::Probe(o);
    if (defHeaders != NULL) {
        AutoPtr<IIterable> iterable = IIterable::Probe(defHeaders);
        AutoPtr<IIterator> it;
        iterable->GetIterator((IIterator**)&it);
        Boolean hasNext;
        while (it->HasNext(&hasNext), hasNext) {
            AutoPtr<IObject> defHeader;
            it->GetNext((IInterface**)&defHeader);
            request->AddHeader(IHeader::Probe(defHeader));
        }
    }
    return NOERROR;
}

ECode CRequestDefaultHeaders::constructor()
{
    return NOERROR;
}

} // namespace Protocol
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org