
#ifndef __ELASTOS_DROID_WEBKIT_COOKIEMANAGER_H__
#define __ELASTOS_DROID_WEBKIT_COOKIEMANAGER_H__

#include "ext/frameworkext.h"
#include <elastos/Core/Object.h>

using Elastos::Core::Object;
using Elastos::Droid::Net::IWebAddress;

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Manages the cookies used by an application's {@link WebView} instances.
 * Cookies are manipulated according to RFC2109.
 */
class CookieManager
    : public ElRefBase
    , public Object
    , public ICookieManager
{
public:
    CAR_INTERFACE_DECL();

    /**
     * Gets the singleton CookieManager instance. If this method is used
     * before the application instantiates a {@link WebView} instance,
     * {@link CookieSyncManager#createInstance CookieSyncManager.createInstance(Context)}
     * must be called first.
     *
     * @return the singleton CookieManager instance
     */
    //synchronized
    static CARAPI_(AutoPtr<ICookieManager>) GetInstance();

    /**
     * Sets whether the application's {@link WebView} instances should send and
     * accept cookies.
     *
     * @param accept whether {@link WebView} instances should send and accept
     *               cookies
     */
    //synchronized
    virtual CARAPI SetAcceptCookie(
        /* [in] */ Boolean accept);

    /**
     * Gets whether the application's {@link WebView} instances send and accept
     * cookies.
     *
     * @return true if {@link WebView} instances send and accept cookies
     */
    //synchronized
    virtual CARAPI_(Boolean) AcceptCookie();

     /**
     * Sets a cookie for the given URL. Any existing cookie with the same host,
     * path and name will be replaced with the new cookie. The cookie being set
     * must not have expired and must not be a session cookie, otherwise it
     * will be ignored.
     *
     * @param url the URL for which the cookie is set
     * @param value the cookie as a string, using the format of the 'Set-Cookie'
     *              HTTP response header
     */
    virtual CARAPI SetCookie(
        /* [in] */ const String& url,
        /* [in] */ const String& value);

    /**
     * Gets the cookies for the given URL.
     *
     * @param url the URL for which the cookies are requested
     * @return value the cookies as a string, using the format of the 'Cookie'
     *               HTTP request header
     */
    virtual CARAPI GetCookie(
        /* [in] */ const String& url,
        /* [out] */ String* cookie);

    /**
     * See {@link #getCookie(String)}.
     *
     * @param url the URL for which the cookies are requested
     * @param privateBrowsing whether to use the private browsing cookie jar
     * @return value the cookies as a string, using the format of the 'Cookie'
     *               HTTP request header
     * @hide Used by Browser, no intention to publish.
     */
    virtual CARAPI GetCookie(
        /* [in] */ const String& url,
        /* [in] */ Boolean privateBrowsing,
        /* [out] */ String* cookie);

    /**
     * Gets cookie(s) for a given uri so that it can be set to "cookie:" in http
     * request header.
     *
     * @param uri the WebAddress for which the cookies are requested
     * @return value the cookies as a string, using the format of the 'Cookie'
     *               HTTP request header
     * @hide Used by RequestHandle, no intention to publish.
     */
    //synchronized
    virtual CARAPI GetCookie(
        /* [in] */ IWebAddress* uri,
        /* [out] */ String* cookie);

    /**
     * Removes all session cookies, which are cookies without an expiration
     * date.
     */
    virtual CARAPI RemoveSessionCookie();

    /**
     * Removes all cookies.
     */
    virtual CARAPI RemoveAllCookie();

    /**
     * Gets whether there are stored cookies.
     *
     * @return true if there are stored cookies
     */
    //synchronized
    virtual CARAPI HasCookies(
        /* [out] */ Boolean* result);

    /**
     * See {@link #hasCookies()}.
     *
     * @param privateBrowsing whether to use the private browsing cookie jar
     * @hide Used by Browser, no intention to publish.
     */
    //synchronized
    virtual CARAPI HasCookies(
        /* [in] */ Boolean privateBrowsing,
        /* [out] */ Boolean* result);

    /**
     * Removes all expired cookies.
     */
    virtual CARAPI RemoveExpiredCookie();

    /**
     * Gets whether the application's {@link WebView} instances send and accept
     * cookies for file scheme URLs.
     *
     * @return true if {@link WebView} instances send and accept cookies for
     *         file scheme URLs
     */
    // Static for backward compatibility.
    static CARAPI_(Boolean) AllowFileSchemeCookies();

    /**
     * Sets whether the application's {@link WebView} instances should send and
     * accept cookies for file scheme URLs.
     * Use of cookies with file scheme URLs is potentially insecure. Do not use
     * this feature unless you can be sure that no unintentional sharing of
     * cookie data can take place.
     * <p>
     * Note that calls to this method will have no effect if made after a
     * {@link WebView} or CookieManager instance has been created.
     */
    // Static for backward compatibility.
    static CARAPI_(void) SetAcceptFileSchemeCookies(
        /* [in] */ Boolean accept);

    /**
     * Flushes all cookies managed by the Chrome HTTP stack to flash.
     *
     * @hide Package level api, called from CookieSyncManager
     */
    virtual CARAPI_(void) FlushCookieStore();

protected:
    /**
     * @hide Only for use by WebViewProvider implementations
     */
    CookieManager();

    /**
     * Implements {@link #setAcceptFileSchemeCookies(boolean)}.
     *
     * @hide Only for use by WebViewProvider implementations
     */
    virtual CARAPI_(void) SetAcceptFileSchemeCookiesImpl(
        /* [in] */ Boolean accept);


    //@Override
    virtual CARAPI_(IInterface*) Clone();

    /**
     * Implements {@link #allowFileSchemeCookies()}.
     *
     * @hide Only for use by WebViewProvider implementations
     */
    virtual CARAPI_(Boolean) AllowFileSchemeCookiesImpl();

protected:
    static Object sLock;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_COOKIEMANAGER_H__
