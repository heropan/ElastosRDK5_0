
#include "CClientParamBean.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Params {

CAR_INTERFACE_IMPL(CClientParamBean, HttpAbstractParamBean, IClientParamBean)

CAR_OBJECT_IMPL(CClientParamBean)

ECode CClientParamBean::SetConnectionManagerFactoryClassName(
    /* [in] */ const String& factory)
{
    return mParams->SetParameter(IClientPNames::CONNECTION_MANAGER_FACTORY_CLASS_NAME, factory);
}

ECode CClientParamBean::SetConnectionManagerFactory(
    /* [in] */ IClientConnectionManagerFactory factory)
{
    mParams->SetParameter(IClientPNames::CONNECTION_MANAGER_FACTORY, factory);
}

ECode CClientParamBean::SetHandleRedirects(
    /* [in] */ Boolean handle)
{
    mParams->SetBooleanParameter(IClientPNames::HANDLE_REDIRECTS, handle);
}

ECode CClientParamBean::SetRejectRelativeRedirect(
    /* [in] */ Boolean reject)
{
    mParams->SetBooleanParameter(IClientPNames::REJECT_RELATIVE_REDIRECT, reject);
}

ECode CClientParamBean::SetMaxRedirects(
    /* [in] */ Int32 maxRedirects)
{
    mParams->SetIntParameter(IClientPNames::MAX_REDIRECTS, maxRedirects);
}

ECode CClientParamBean::SetAllowCircularRedirects(
    /* [in] */ Boolean allow)
{
    mParams->SetBooleanParameter(IClientPNames::ALLOW_CIRCULAR_REDIRECTS, allow);
}

ECode CClientParamBean::SetHandleAuthentication(
    /* [in] */ Boolean handle)
{
    mParams->SetBooleanParameter(IClientPNames::HANDLE_AUTHENTICATION, handle);
}

ECode CClientParamBean::SetCookiePolicy(
    /* [in] */ String policy)
{
    mParams->SetParameter(IClientPNames::COOKIE_POLICY, policy);
}

ECode CClientParamBean::SetVirtualHost(
    /* [in] */ IHttpHost* host)
{
    mParams->SetParameter(IClientPNames::VIRTUAL_HOST, host);
}

ECode CClientParamBean::SetDefaultHeaders(
    /* [in] */ ICollection* headers)
{
    mParams->SetParameter(IClientPNames::DEFAULT_HEADERS, headers);
}

ECode CClientParamBean::SetDefaultHost(
    /* [in] */ IHttpHost* host)
{
    mParams->SetParameter(IClientPNames::DEFAULT_HOST, host);
}

ECode CClientParamBean::constructor(
    /* [in] */ IHttpParams* params)
{
    return Init(params);
}

} // namespace Params
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org