
#ifndef __ORG_APACHE_HTTP_CLIENT_METHODS_CHTTPHEAD_H_
#define __ORG_APACHE_HTTP_CLIENT_METHODS_CHTTPHEAD_H_

#include "Elastos.CoreLibrary.Utility.Concurrent.h"
#include "_Org_Apache_Http_Client_Methods_CHttpHead.h"
#include "org/apache/http/client/methods/HttpRequestBase.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Methods {

/**
 * HTTP HEAD method.
 * <p>
 * The HTTP HEAD method is defined in section 9.4 of
 * <a href="http://www.ietf.org/rfc/rfc2616.txt">RFC2616</a>:
 * <blockquote>
 * The HEAD method is identical to GET except that the server MUST NOT
 * return a message-body in the response. The metainformation contained
 * in the HTTP headers in response to a HEAD request SHOULD be identical
 * to the information sent in response to a GET request. This method can
 * be used for obtaining metainformation about the entity implied by the
 * request without transferring the entity-body itself. This method is
 * often used for testing hypertext links for validity, accessibility,
 * and recent modification.
 * </blockquote>
 * </p>
 *
 * @version $Revision: 664505 $
 *
 * @since 4.0
 */
CarClass(CHttpHead)
    , public HttpRequestBase
    , public IHttpHead
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI GetMethod(
        /* [out] */ String* method);

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

#endif // __ORG_APACHE_HTTP_CLIENT_METHODS_CHTTPHEAD_H_
