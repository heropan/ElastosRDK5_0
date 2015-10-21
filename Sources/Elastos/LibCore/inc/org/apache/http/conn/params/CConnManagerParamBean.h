
#ifndef __ORG_APACHE_HTTP_CONN_PARAMS_CCONNMANAGERPARAMBEAN_H_
#define __ORG_APACHE_HTTP_CONN_PARAMS_CCONNMANAGERPARAMBEAN_H_

#include "_Org_Apache_Http_Conn_Params_CConnManagerParamBean.h"
#include "org/apache/http/params/HttpAbstractParamBean.h"

using Org::Apache::Http::Params::HttpAbstractParamBean;
using Org::Apache::Http::Params::IHttpParams;

namespace Org {
namespace Apache {
namespace Http {
namespace Conn {
namespace Params {

/**
 * Allows for setting parameters relating to connection routes on
 * {@link HttpParams}.  This class ensures that the values set on the params
 * are type-safe.
 */
CarClass(CConnManagerParamBean)
    , public IConnManagerParamBean
    , public HttpAbstractParamBean
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI SetTimeout(
        /* [in] */ Int64 timeout);

    /** @see ConnManagerPNames#MAX_TOTAL_CONNECTIONS */
    CARAPI SetMaxTotalConnections(
        /* [in] */ Int32 maxConnections);

    /** @see ConnManagerPNames#MAX_CONNECTIONS_PER_ROUTE */
    CARAPI SetConnectionsPerRoute(
        /* [in] */ IConnPerRouteBean* connPerRoute);

    CARAPI constructor(
        /* [in] */ IHttpParams* params);

};

} // namespace Params
} // namespace Conn
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_CONN_PARAMS_CCONNMANAGERPARAMBEAN_H_
