
#ifndef __ORG_APACHE_HTTP_AUTH_CAUTHSCOPEHELPER_H_
#define __ORG_APACHE_HTTP_AUTH_CAUTHSCOPEHELPER_H_

#include "_Org_Apache_Http_Auth_CAuthScopeHelper.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Org {
namespace Apache {
namespace Http {
namespace Auth {

CarClass(CAuthScopeHelper)
    , public Object
    , public IAuthScopeHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI GetDefaultScope(
        /* [out] */ IAuthScope** scope);

};

} // namespace Auth
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_AUTH_CAUTHSCOPEHELPER_H_
