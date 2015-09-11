
#ifndef __ORG_APACHE_HTTP_CLIENT_UTILS_CLONEUTILS_H__
#define __ORG_APACHE_HTTP_CLIENT_UTILS_CLONEUTILS_H__

#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif
#include <elastos/coredef.h>

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Utils {

/**
 * A collection of utilities to workaround limitations of Java clone framework.
 */
class CloneUtils
{
private:
    /**
     * This class should not be instantiated.
     */
    CloneUtils() {}

public:
    static CARAPI Clone(
        /* [in] */ IObject* obj,
        /* [out] */ IObject** cloneObj);
};

} // namespace Utils
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_CLIENT_UTILS_CLONEUTILS_H__
