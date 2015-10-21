
#ifndef __ORG_APACHE_HTTP_PROTOCOL_CHTTPREQUESTHANDLERREGISTRY_H_
#define __ORG_APACHE_HTTP_PROTOCOL_CHTTPREQUESTHANDLERREGISTRY_H_

#include "_Org_Apache_Http_Protocol_CHttpRequestHandlerRegistry.h"
#include "org/apache/http/protocol/HttpRequestHandlerRegistry.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Protocol {

/**
 * Maintains a map of HTTP request handlers keyed by a request URI pattern.
 * {@link HttpRequestHandler} instances can be looked up by request URI
 * using the {@link HttpRequestHandlerResolver} interface.<br/>
 * Patterns may have three formats:
 * <ul>
 *   <li><code>*</code></li>
 *   <li><code>*&lt;uri&gt;</code></li>
 *   <li><code>&lt;uri&gt;*</code></li>
 * </ul>
 *
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 *
 * @version $Revision: 630662 $
 */
CarClass(CHttpRequestHandlerRegistry) , public HttpRequestHandlerRegistry
{
public:
    CAR_OBJECT_DECL()

    CARAPI constructor();
};

} // namespace Protocol
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_PROTOCOL_CHTTPREQUESTHANDLERREGISTRY_H_
