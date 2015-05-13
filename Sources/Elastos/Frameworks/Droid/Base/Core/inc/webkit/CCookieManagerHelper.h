
#ifndef __CCOOKIEMANAGERHELPER_H__
#define __CCOOKIEMANAGERHELPER_H__

#include "_CCookieManagerHelper.h"

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

#endif // __CCOOKIEMANAGERHELPER_H__
