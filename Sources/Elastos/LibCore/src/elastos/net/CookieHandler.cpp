
#include "cmdef.h"
#include "CookieHandler.h"

namespace Elastos {
namespace Net {

AutoPtr<ICookieHandler> CookieHandler::sSystemWideCookieHandler;

ECode CookieHandler::GetDefault(
    /* [out] */ ICookieHandler** handler)
{
    VALIDATE_NOT_NULL(handler);

    // SecurityManager sm = System.getSecurityManager();
    // if (null != sm) {
    //     sm.checkPermission(getCookieHandlerPermission);
    // }
    *handler = sSystemWideCookieHandler;
    INTERFACE_ADDREF(*handler);

    return NOERROR;
}

/**
 * Sets the system-wide cookie handler.
 *
 * @param cHandler
 *            a cookie handler to set as the system-wide default handler.
 */
ECode CookieHandler::SetDefault(
    /* [in] */ ICookieHandler* cHandler)
{
    // SecurityManager sm = System.getSecurityManager();
    // if (null != sm) {
    //     sm.checkPermission(setCookieHandlerPermission);
    // }
    sSystemWideCookieHandler = cHandler;
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
