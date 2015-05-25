
#include "cmdef.h"
#include "CCookieHandlerHelper.h"

namespace Elastos {
namespace Net {

ECode CCookieHandlerHelper::GetDefault(
    /* [out] */ ICookieHandler** handler)
{
    VALIDATE_NOT_NULL(handler)

    *handler = mSystemWideCookieHandler;
    INTERFACE_ADDREF(*handler)
    return NOERROR;
}

ECode CCookieHandlerHelper::SetDefault(
    /* [in] */ ICookieHandler* handler)
{
    mSystemWideCookieHandler = handler;
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
