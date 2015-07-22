
#ifndef __ORG_APACHE_HTTP_CLIENT_METHODS_HTTPENTITYENCLOSINGREQUESTBASE_H__
#define __ORG_APACHE_HTTP_CLIENT_METHODS_HTTPENTITYENCLOSINGREQUESTBASE_H__

#include "HttpRequestBase.h"

using Org::Apache::Http::IHttpEntity;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Methods {

/**
 * Basic implementation of an HTTP request that can be modified.
 *
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 *
 * @version $Revision: 674186 $
 *
 * @since 4.0
 */
class HttpEntityEnclosingRequestBase
    : public HttpRequestBase
    , public IHttpEntityEnclosingRequest
{
public:
    HttpEntityEnclosingRequestBase();

    virtual ~HttpEntityEnclosingRequestBase() = 0;

    CAR_INTERFACE_DECL()

    CARAPI SetEntity(
        /* [in] */ IHttpEntity* entity);

    CARAPI GetEntity(
        /* [out] */ IHttpEntity** entity);

    CARAPI ExpectContinue(
        /* [out] */ Boolean* result);

protected:
    virtual CARAPI CloneImpl(
        /* [in] */ IHttpEntityEnclosingRequest* dst);

private:
    AutoPtr<IHttpEntity> mEntity;
};

} // namespace Methods
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_CLIENT_METHODS_HTTPENTITYENCLOSINGREQUESTBASE_H__
