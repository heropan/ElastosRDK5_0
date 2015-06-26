
#ifndef __ORG_APACHE_HTTP_CLIENT_PARAMS_CCLIENTPARAMBEAN_H_
#define __ORG_APACHE_HTTP_CLIENT_PARAMS_CCLIENTPARAMBEAN_H_

#include "_Org_Apache_Http_Client_Params_CClientParamBean.h"
#include "HttpAbstractParamBean.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;
using Elastos::Utility::ICollection;
using Org::Apache::Http::IHttpHost;
using Org::Apache::Http::Conn::IClientConnectionManagerFactory;
using Org::Apache::Http::Params::HttpAbstractParamBean;
using Org::Apache::Http::Params::IHttpParams;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Params {

CarClass(CClientParamBean)
    , public Object
    , public HttpAbstractParamBean
    , public IClientParamBean
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI SetConnectionManagerFactoryClassName(
        /* [in] */ const String& factory);

    CARAPI SetConnectionManagerFactory(
        /* [in] */ IClientConnectionManagerFactory factory);

    CARAPI SetHandleRedirects(
        /* [in] */ Boolean handle);

    CARAPI SetRejectRelativeRedirect(
        /* [in] */ Boolean reject);

    CARAPI SetMaxRedirects(
        /* [in] */ Int32 maxRedirects);

    CARAPI SetAllowCircularRedirects(
        /* [in] */ Boolean allow);

    CARAPI SetHandleAuthentication(
        /* [in] */ Boolean handle);

    CARAPI SetCookiePolicy(
        /* [in] */ String policy);

    CARAPI SetVirtualHost(
        /* [in] */ IHttpHost* host);

    CARAPI SetDefaultHeaders(
        /* [in] */ ICollection* headers);

    CARAPI SetDefaultHost(
        /* [in] */ IHttpHost* host);

    CARAPI constructor(
        /* [in] */ IHttpParams* params);
};

} // namespace Params
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_CLIENT_PARAMS_CCLIENTPARAMBEAN_H_
