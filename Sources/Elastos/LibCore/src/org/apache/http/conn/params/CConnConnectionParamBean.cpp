
#include "CConnConnectionParamBean.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Conn {
namespace Params {

CAR_INTERFACE_IMPL(CConnConnectionParamBean, Object, IConnConnectionParamBean)

CAR_OBJECT_IMPL(CConnConnectionParamBean)

ECode CConnConnectionParamBean::SetMaxStatusLineGarbage(
    /* [in] */ Int32 maxStatusLineGarbage)
{
    return mParams->SetInt32Parameter(IConnConnectionPNames::MAX_STATUS_LINE_GARBAGE, maxStatusLineGarbage);
}

ECode CConnConnectionParamBean::constructor(
    /* [in] */ IHttpParams* params)
{
    return Init(params);
}

} // namespace Params
} // namespace Conn
} // namespace Http
} // namespace Apache
} // namespace Org
