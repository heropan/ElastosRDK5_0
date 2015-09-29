
#ifndef __ORG_APACHE_HTTP_IMPL_COOKIE_RFC2965SPECFACTORY_H__
#define __ORG_APACHE_HTTP_IMPL_COOKIE_RFC2965SPECFACTORY_H__

#include "Object.h"

using Org::Apache::Http::Cookie::ICookieSpec;
using Org::Apache::Http::Cookie::ICookieSpecFactory;
using Org::Apache::Http::Params::IHttpParams;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Cookie {

/**
 *
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 *
 * @since 4.0
 */
class RFC2965SpecFactory
    : public Object
    , public ICookieSpecFactory
{
public:
    CAR_INTERFACE_DECL()

    CARAPI NewInstance(
        /* [in] */ IHttpParams* params,
        /* [out] */ ICookieSpec** spec);
};

} // namespace Cookie
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_IMPL_COOKIE_RFC2965SPECFACTORY_H__
