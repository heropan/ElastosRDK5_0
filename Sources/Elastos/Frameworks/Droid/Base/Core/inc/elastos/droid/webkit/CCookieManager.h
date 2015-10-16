
#ifndef __ELASTOS_DROID_WEBKIT_CCOOKIEMANAGER_H__
#define __ELASTOS_DROID_WEBKIT_CCOOKIEMANAGER_H__

#include "_Elastos_Droid_Webkit_CCookieManager.h"
#include "elastos/droid/webkit/CookieManager.h"

using Elastos::Droid::Net::IWebAddress;

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CCookieManager), public CookieManager
{
public:
    CARAPI SetAcceptCookie(
        /* [in] */ Boolean accept);

    CARAPI AcceptCookie(
        /* [out] */ Boolean* result);

    CARAPI SetCookie(
        /* [in] */ const String& url,
        /* [in] */ const String& value);

    CARAPI GetCookie(
        /* [in] */ const String& url,
        /* [out] */ String* cookie);

    CARAPI GetCookie(
        /* [in] */ const String& url,
        /* [in] */ Boolean privateBrowsing,
        /* [out] */ String* cookie);

    CARAPI GetCookie(
        /* [in] */ IWebAddress* uri,
        /* [out] */ String* cookie);

    CARAPI RemoveSessionCookie();

    CARAPI RemoveAllCookie();

    CARAPI HasCookies(
        /* [out] */ Boolean* result);

    CARAPI HasCookies(
        /* [in] */ Boolean privateBrowsing,
        /* [out] */ Boolean* result);

    CARAPI RemoveExpiredCookie();
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CCOOKIEMANAGER_H__
