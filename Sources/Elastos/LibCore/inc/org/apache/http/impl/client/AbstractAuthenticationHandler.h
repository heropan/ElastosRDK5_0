
#ifndef __ORG_APACHE_HTTP_IMPL_CLIENT_ABSTRACTAUTHENTICATIONHANDLER_H__
#define __ORG_APACHE_HTTP_IMPL_CLIENT_ABSTRACTAUTHENTICATIONHANDLER_H__

#include "elastos/core/Object.h"

using Elastos::Utility::IMap;
using Elastos::Utility::IList;
using Org::Apache::Http::IHeader;
using Org::Apache::Http::IHttpResponse;
using Org::Apache::Http::Auth::IAuthScheme;
using Org::Apache::Http::Client::IAuthenticationHandler;
using Org::Apache::Http::Protocol::IHttpContext;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Client {

/**
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 */
class AbstractAuthenticationHandler
    : public Object
    , public IAuthenticationHandler
{
public:
    AbstractAuthenticationHandler() {}

    CAR_INTERFACE_DECL()

    CARAPI SelectScheme(
        /* [in] */ IMap* challenges,
        /* [in] */ IHttpResponse* response,
        /* [in] */ IHttpContext* context,
        /* [out] */ IAuthScheme** scheme);

protected:
    CARAPI ParseChallenges(
        /* [in] */ ArrayOf<IHeader*>* headers,
        /* [out] */ IMap** challenges);

    CARAPI_(AutoPtr<IList>) GetAuthPreferences();

private:
    static const AutoPtr<IList> DEFAULT_SCHEME_PRIORITY;
};

} // namespace Client
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_IMPL_CLIENT_ABSTRACTAUTHENTICATIONHANDLER_H__
