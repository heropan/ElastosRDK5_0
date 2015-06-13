
#include "CConnRouteParamBean.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Conn {
namespace Params {

CAR_INTERFACE_IMPL(CConnRouteParamBean, Object, IConnRouteParamBean)

CAR_OBJECT_IMPL(CConnRouteParamBean)

ECode CConnRouteParamBean::SetDefaultProxy(
    /* [in] */ IHttpHost* defaultProxy)
{
    return mParams->SetParameter(IConnRoutePNames::DEFAULT_PROXY, IObject::Probe(defaultProxy));
}

ECode CConnRouteParamBean::SetLocalAddress(
    /* [in] */ IInetAddress* address)
{
    return mParams->SetParameter(IConnRoutePNames::LOCAL_ADDRESS, IObject::Probe(address));
}

ECode CConnRouteParamBean::SetForcedRoute(
    /* [in] */ IHttpRoute* route)
{
    return mParams->SetParameter(IConnRoutePNames::FORCED_ROUTE, IObject::Probe(route));
}

ECode CConnRouteParamBean::constructor(
    /* [in] */ IHttpParams* params)
{
    return Init(params);
}

} // namespace Params
} // namespace Conn
} // namespace Http
} // namespace Apache
} // namespace Org
