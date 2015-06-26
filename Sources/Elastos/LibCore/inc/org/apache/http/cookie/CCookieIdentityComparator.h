
#ifndef __ORG_APACHE_HTTP_COOKIE_CCOOKIEIDENTITYCOMPARATOR_H_
#define __ORG_APACHE_HTTP_COOKIE_CCOOKIEIDENTITYCOMPARATOR_H_

#include "_Org_Apache_Http_Cookie_CCookieIdentityComparator.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;
using Elastos::Net::IInetAddress;
using Elastos::Net::ISocket;
using Elastosx::Net::Ssl::IKeyManager;
using Elastosx::Net::Ssl::ITrustManager;
using Elastosx::Net::Ssl::IKeyStore;
using Elastosx::Net::Ssl::ISSLContext;
using Org::Apache::Http::Conn::Scheme::IHostNameResolver;
using Org::Apache::Http::Conn::Scheme::ILayeredSocketFactory;
using Org::Apache::Http::Conn::Scheme::ISocketFactory;
using Org::Apache::Http::Params::IHttpParams;

namespace Org {
namespace Apache {
namespace Http {
namespace Cookie {

/**
 * This cookie comparator can be used to compare identity of cookies.
 *
 * <p>
 *  Cookies are considered identical if their names are equal and
 *  their domain attributes match ignoring case.
 * </p>
 *
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 */
CarClass(CCookieIdentityComparator)
    , public Object
    , public IComparator
    , public ISerializable
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI Compare(
        /* [in] */ IInterface* lhs,
        /* [in] */ IInterface* rhs,
        /* [out] */ Int32* result);

private:
    static const Int64 serialVersionUID = 4466565437490631532L;

};

} // namespace Cookie
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_COOKIE_CCOOKIEIDENTITYCOMPARATOR_H_
