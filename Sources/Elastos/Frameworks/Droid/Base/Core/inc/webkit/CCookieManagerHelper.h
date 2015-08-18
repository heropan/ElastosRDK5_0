
#ifndef __ELASTOS_DROID_WEBKIT_CCOOKIEMANAGERHELPER_H__
#define __ELASTOS_DROID_WEBKIT_CCOOKIEMANAGERHELPER_H__

#include "_Elastos_Droid_Webkit_CCookieManagerHelper.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CCookieManagerHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ ICookieManager** cookieManager);

    CARAPI AllowFileSchemeCookies(
        /* [out] */ Boolean* result);

    CARAPI SetAcceptFileSchemeCookies(
        /* [in] */ Boolean accept);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CCOOKIEMANAGERHELPER_H__
