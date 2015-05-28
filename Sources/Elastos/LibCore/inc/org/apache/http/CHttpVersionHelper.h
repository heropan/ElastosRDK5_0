
#ifndef __CHTTPVERSIONHELPER_H__
#define __CHTTPVERSIONHELPER_H__

#include "_Org_Apache_Http_CHttpVersionHelper.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Org {
namespace Apache {
namespace Http {

/**
 * Represents an HTTP version, as specified in RFC 2616.
 *
 * @author <a href="mailto:oleg@ural.ru">Oleg Kalnichevski</a>
 *
 * @version $Revision: 609106 $ $Date: 2008-01-05 01:15:42 -0800 (Sat, 05 Jan 2008) $
 */
CarClass(CHttpVersionHelper)
    , public Object
    , public IHttpVersionHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI GerHttpVersion09(
        /* [out] */ IHttpVersion** ver);

    CARAPI GerHttpVersion10(
        /* [out] */ IHttpVersion** ver);

    CARAPI GerHttpVersion11(
        /* [out] */ IHttpVersion** ver);
};

} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __CHTTPVERSION_H__