
#ifndef __ORG_APACHE_HTTP_AUTH_PARAMS_CAUTHPARAMBEAN_H_
#define __ORG_APACHE_HTTP_AUTH_PARAMS_CAUTHPARAMBEAN_H_

#include "_Org_Apache_Http_Auth_CAuthParamBean.h"
#include "HttpAbstractParamBean.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Org {
namespace Apache {
namespace Http {
namespace Auth {
namespace Params {

CarClass(CAuthParamBean)
    , public Object
    , public IAuthParamBean
    , public HttpAbstractParamBean
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI SetCredentialCharset(
        /* [in] */ const String& charset);

    CARAPI constructor(
        /* [in] */ IHttpParams* params);

};

} // namespace Params
} // namespace Auth
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_AUTH_PARAMS_CAUTHPARAMBEAN_H_
