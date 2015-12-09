
#ifndef __ORG_APACHE_HTTP_CLIENT_METHODS_CHTTPOPTIONS_H_
#define __ORG_APACHE_HTTP_CLIENT_METHODS_CHTTPOPTIONS_H_

#include "Elastos.CoreLibrary.Utility.Concurrent.h"
#include "_Org_Apache_Http_Client_Methods_CHttpOptions.h"
#include "org/apache/http/client/methods/HttpRequestBase.h"

using Elastos::Utility::ISet;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Methods {

/**
 * HTTP OPTIONS method.
 * <p>
 * The HTTP OPTIONS method is defined in section 9.2 of
 * <a href="http://www.ietf.org/rfc/rfc2616.txt">RFC2616</a>:
 * <blockquote>
 *  The OPTIONS method represents a request for information about the
 *  communication options available on the request/response chain
 *  identified by the Request-URI. This method allows the client to
 *  determine the options and/or requirements associated with a resource,
 *  or the capabilities of a server, without implying a resource action
 *  or initiating a resource retrieval.
 * </blockquote>
 * </p>
 *
 * @version $Revision: 664505 $
 *
 * @since 4.0
 */
CarClass(CHttpOptions)
    , public HttpRequestBase
    , public IHttpOptions
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI GetMethod(
        /* [out] */ String* method);

    CARAPI GetAllowedMethods(
        /* [in] */ IHttpResponse* response,
        /* [out] */ ISet** methods);

    CARAPI Clone(
        /* [out] */ IInterface** obj);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IURI* uri);

    CARAPI constructor(
        /* [in] */ const String& uri);
};

} // Methods
} // Client
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_CLIENT_METHODS_CHTTPOPTIONS_H_
