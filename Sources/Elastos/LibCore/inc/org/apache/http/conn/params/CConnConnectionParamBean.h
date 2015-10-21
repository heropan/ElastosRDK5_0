
#ifndef __ORG_APACHE_HTTP_CONN_PARAMS_CCONNCONNECTIONPARAMBEAN_H_
#define __ORG_APACHE_HTTP_CONN_PARAMS_CCONNCONNECTIONPARAMBEAN_H_

#include "_Org_Apache_Http_Conn_Params_CConnConnectionParamBean.h"
#include "org/apache/http/params/HttpAbstractParamBean.h"

using Org::Apache::Http::Params::HttpAbstractParamBean;
using Org::Apache::Http::Params::IHttpParams;

namespace Org {
namespace Apache {
namespace Http {
namespace Conn {
namespace Params {

/**
 * Allows for setting parameters relating to connections on
 * {@link HttpParams}.  This class ensures that the values set on the params
 * are type-safe.
 */
CarClass(CConnConnectionParamBean)
    , public IConnConnectionParamBean
    , public HttpAbstractParamBean
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI SetMaxStatusLineGarbage(
        /* [in] */ Int32 maxStatusLineGarbage);

    CARAPI constructor(
        /* [in] */ IHttpParams* params);

};

} // namespace Params
} // namespace Conn
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_CONN_PARAMS_CCONNCONNECTIONPARAMBEAN_H_
