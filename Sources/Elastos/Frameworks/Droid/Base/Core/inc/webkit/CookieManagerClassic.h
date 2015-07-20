
#ifndef __COOKIEMANAGERCLASSIC_H__
#define __COOKIEMANAGERCLASSIC_H__

#include "ext/frameworkext.h"

#include "webkit/CookieManager.h"

#ifdef __cplusplus
extern "C"
{
#endif
    extern Boolean Elastos_CookieManager_nativeAcceptCookie();
    extern Elastos::String Elastos_CookieManager_nativeGetCookie(const Elastos::String& url, Boolean privateBrowsing);
    extern Boolean Elastos_CookieManager_nativeHasCookies(Boolean privateBrowsing);
    extern void Elastos_CookieManager_nativeRemoveAllCookie();
    extern void Elastos_CookieManager_nativeRemoveExpiredCookie();
    extern void Elastos_CookieManager_nativeRemoveSessionCookie();
    extern void Elastos_CookieManager_nativeSetAcceptCookie(Boolean);
    extern void Elastos_CookieManager_nativeSetCookie(const Elastos::String& url,
              const Elastos::String& value, Boolean privateBrowsing);
    extern void Elastos_CookieManager_nativeFlushCookieStore();
    extern Boolean Elastos_CookieManager_nativeAcceptFileSchemeCookies();
    extern void Elastos_CookieManager_nativeSetAcceptFileSchemeCookies(Boolean);
#ifdef __cplusplus
}
#endif

using Elastos::Droid::Net::IWebAddress;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CookieManagerClassic : public CookieManager
{
public:
    static CARAPI_(AutoPtr<CookieManagerClassic>) GetInstance();

    CARAPI SetAcceptCookie(
        /* [in] */ Boolean accept);

    CARAPI AcceptCookie(
        /* [out] */ Boolean* result);

    CARAPI SetCookie(
        /* [in] */ const String& url,
        /* [in] */ const String& value);

    /**
     * See {@link #setCookie(String, String)}
     * @param url The URL for which the cookie is set
     * @param value The value of the cookie, as a string, using the format of
     *              the 'Set-Cookie' HTTP response header
     * @param privateBrowsing Whether to use the private browsing cookie jar
     */
    virtual CARAPI_(void) SetCookie(
        /* [in] */ const String& url,
        /* [in] */ const String& value,
        /* [in] */ Boolean privateBrowsing);

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

    /**
     * Waits for pending operations to completed.
     */
    virtual CARAPI_(void) WaitForCookieOperationsToComplete();

    CARAPI RemoveSessionCookie();

    CARAPI RemoveAllCookie();

    CARAPI HasCookies(
        /* [out] */ Boolean* result);

    CARAPI HasCookies(
        /* [in] */ Boolean privateBrowsing,
        /* [out] */ Boolean* result);

    CARAPI RemoveExpiredCookie();

protected:
    CARAPI_(void) FlushCookieStore();

    CARAPI_(Boolean) AllowFileSchemeCookiesImpl();

    CARAPI_(void) SetAcceptFileSchemeCookiesImpl(
        /* [in] */ Boolean accept);

private:
    CookieManagerClassic();

    CARAPI_(void) SignalCookieOperationsComplete();

    CARAPI_(void) SignalCookieOperationsStart();

    // Native functions
    static CARAPI_(Boolean) NativeAcceptCookie();

    static CARAPI_(String) NativeGetCookie(
        /* [in] */ const String& url,
        /* [in] */ Boolean privateBrowsing);

    static CARAPI_(Boolean) NativeHasCookies(
        /* [in] */ Boolean privateBrowsing);

    static CARAPI_(void) NativeRemoveAllCookie();

    static CARAPI_(void) NativeRemoveExpiredCookie();

    static CARAPI_(void) NativeRemoveSessionCookie();

    static CARAPI_(void) NativeSetAcceptCookie(
        /* [in] */ Boolean accept);

    static CARAPI_(void) NativeSetCookie(
        /* [in] */ const String& url,
        /* [in] */ const String& value,
        /* [in] */ Boolean privateBrowsing);

    static CARAPI_(void) NativeFlushCookieStore();

    static CARAPI_(Boolean) NativeAcceptFileSchemeCookies();

    static CARAPI_(void) NativeSetAcceptFileSchemeCookies(
        /* [in] */ Boolean accept);

private:
    static AutoPtr<CookieManagerClassic> sRef;
    static const String LOGTAG;
    static Object sLock;

    Int32 mPendingCookieOperations;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__COOKIEMANAGERCLASSIC_H__
