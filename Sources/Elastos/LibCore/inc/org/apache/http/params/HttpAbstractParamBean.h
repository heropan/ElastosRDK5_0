
#ifndef __ORG_APACHE_HTTP_PARAMS_HTTPABSTRACTPARAMBEAN_H_
#define __ORG_APACHE_HTTP_PARAMS_HTTPABSTRACTPARAMBEAN_H_

#include "Object.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Params {

class HttpAbstractParamBean : public Object
{
public:
    virtual ~HttpAbstractParamBean() = 0;

protected:
    CARAPI Init(
        /* [in] */ IHttpParams* params);

protected:
    AutoPtr<IHttpParams> mParams;
};

} // namespace Params
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_PARAMS_HTTPABSTRACTPARAMBEAN_H_
