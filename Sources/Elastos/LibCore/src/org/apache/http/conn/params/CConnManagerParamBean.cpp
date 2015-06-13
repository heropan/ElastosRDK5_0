
#include "CConnManagerParamBean.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Conn {
namespace Params {

CAR_INTERFACE_IMPL(CConnManagerParamBean, Object, IConnManagerParamBean)

CAR_OBJECT_IMPL(CConnManagerParamBean)

ECode CConnManagerParamBean::SetTimeout(
    /* [in] */ Int64 timeout)
{
    return params->SetInt64Parameter(IConnManagerPNames::TIMEOUT, timeout);
}

ECode CConnManagerParamBean::SetMaxTotalConnections(
    /* [in] */ Int32 maxConnections)
{
    return params->SetInt32Parameter(IConnManagerPNames::MAX_TOTAL_CONNECTIONS, maxConnections);
}

ECode CConnManagerParamBean::SetConnectionsPerRoute(
    /* [in] */ IConnPerRouteBean* connPerRoute)
{
    return params->SetParameter(IConnManagerPNames::MAX_CONNECTIONS_PER_ROUTE, IObject::Probe(connPerRoute));
}

ECode CConnManagerParamBean::constructor(
    /* [in] */ IHttpParams* params)
{
    return Init(params);
}

} // namespace Params
} // namespace Conn
} // namespace Http
} // namespace Apache
} // namespace Org
