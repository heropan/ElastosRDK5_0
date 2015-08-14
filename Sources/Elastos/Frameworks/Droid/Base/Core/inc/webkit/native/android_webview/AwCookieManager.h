
#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCOOKIEMANAGER_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCOOKIEMANAGER_H__

// import android.os.Handler;
// import android.os.Looper;
// import android.webkit.ValueCallback;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * AwCookieManager manages cookies according to RFC2109 spec.
 *
 * Methods in this class are thread safe.
 */
//@JNINamespace("android_webview")
class AwCookieManager
{
private:
    /**
     * CookieCallback is a bridge that knows how to call a ValueCallback on its original thread.
     * We need to arrange for the users ValueCallback#onReceiveValue to be called on the original
     * thread after the work is done. When the API is called we construct a CookieCallback which
     * remembers the handler of the current thread. Later the native code uses
     * invokeBooleanCookieCallback to call CookieCallback#onReceiveValue which posts a Runnable
     * on the handler of the original thread which in turn calls ValueCallback#onReceiveValue.
     */
    class CookieCallback
    {
    private:
        class InnerRunnable
            : public Object
            , public IRunnable
        {
        public:
            InnerRunnable(
                /* [in] */ CookieCallback* owner,
                /* [in] */ IInterface* t);

            CARAPI Run();

        private:
            CookieCallback* mOwner;
            IInterface* mT;
        };

    public:
        CookieCallback(
            /* [in] */ IValueCallback* callback,
            /* [in] */ IHandler* handler);

        static CARAPI_(AutoPtr<CookieCallback>) Convert(
            /* [in] */ IValueCallback* callback);

        CARAPI_(void) OnReceiveValue(
            /* [in] */ const IInterface* t);

    private:
        AutoPtr<IValueCallback> mCallback;
        AutoPtr<IHandler> mHandler;
    };

public:
    // TODO(hjd): remove after landing android update to use new calls.
    CARAPI_(void) RemoveExpiredCookie();

    // TODO(hjd): remove after landing android update to use new calls.
    CARAPI_(void) RemoveAllCookie();

    // TODO(hjd): remove after landing android update to use new calls.
    CARAPI_(void) RemoveSessionCookie();

    /**
     * Control whether cookie is enabled or disabled
     * @param accept TRUE if accept cookie
     */
    CARAPI_(void) SetAcceptCookie(
        /* [in] */ Boolean accept);

    /**
     * Return whether cookie is enabled
     * @return TRUE if accept cookie
     */
    CARAPI_(Boolean) AcceptCookie();

    /**
     * Synchronous version of setCookie.
     */
    CARAPI_(void) SetCookie(
        /* [in] */ String url,
        /* [in] */ String value);

    /**
     * Deprecated synchronous version of removeSessionCookies.
     */
    CARAPI_(void) RemoveSessionCookies();

    /**
     * Deprecated synchronous version of removeAllCookies.
     */
    CARAPI_(void) RemoveAllCookies();

    /**
     * Set cookie for a given url. The old cookie with same host/path/name will
     * be removed. The new cookie will be added if it is not expired or it does
     * not have expiration which implies it is session cookie.
     * @param url The url which cookie is set for.
     * @param value The value for set-cookie: in http response header.
     * @param callback A callback called with the success status after the cookie is set.
     */
    CARAPI_(void) SetCookie(
        /* [in] */ const String& url,
        /* [in] */ const String& value,
        /* [in] */ const IValueCallback* callback);

    /**
     * Get cookie(s) for a given url so that it can be set to "cookie:" in http
     * request header.
     * @param url The url needs cookie
     * @return The cookies in the format of NAME=VALUE [; NAME=VALUE]
     */
    CARAPI_(String) GetCookie(
        /* [in] */ const String& url);

    /**
     * Remove all session cookies, the cookies without an expiration date.
     * The value of the callback is true iff at least one cookie was removed.
     * @param callback A callback called after the cookies (if any) are removed.
     */
    CARAPI_(void) RemoveSessionCookies(
        /* [in] */ IValueCallback* callback);

    /**
     * Remove all cookies.
     * The value of the callback is true iff at least one cookie was removed.
     * @param callback A callback called after the cookies (if any) are removed.
     */
    CARAPI_(void) RemoveAllCookies(
        /* [in] */ IValueCallback* callback);

    /**
     *  Return true if there are stored cookies.
     */
    CARAPI_(Boolean) HasCookies();

    /**
     * Remove all expired cookies
     */
    CARAPI_(void) RemoveExpiredCookies();

    CARAPI_(void) FlushCookieStore();

    /**
     * Whether cookies are accepted for file scheme URLs.
     */
    CARAPI_(Boolean) AllowFileSchemeCookies();

    /**
     * Sets whether cookies are accepted for file scheme URLs.
     *
     * Use of cookies with file scheme URLs is potentially insecure. Do not use this feature unless
     * you can be sure that no unintentional sharing of cookie data can take place.
     * <p>
     * Note that calls to this method will have no effect if made after a WebView or CookieManager
     * instance has been created.
     */
    CARAPI_(void) SetAcceptFileSchemeCookies(
        /* [in] */ Boolean accept);

    //@CalledByNative
    static CARAPI_(void) InvokeBooleanCookieCallback(
        /* [in] */ ICookieCallback* callback,
        /* [in] */ Boolean result);

private:
    CARAPI_(void) NativeSetShouldAcceptCookies(
        /* [in] */ Boolean accept);

    CARAPI_(Boolean) NativeGetShouldAcceptCookies();

    CARAPI_(void) NativeSetCookie(
        /* [in] */ String url,
        /* [in] */ String value,
        /* [in] */ CookieCallback* callback);

    CARAPI_(void) NativeSetCookieSync(
        /* [in] */ String url,
        /* [in] */ String value);

    CARAPI_(String) NativeGetCookie(
        /* [in] */ String url);

    CARAPI_(void) NativeRemoveSessionCookies(
        /* [in] */ CookieCallback* callback);

    CARAPI_(void) NativeRemoveSessionCookiesSync();

    CARAPI_(void) NativeRemoveAllCookies(
        /* [in] */ CookieCallback* callback);

    CARAPI_(void) NativeRemoveAllCookiesSync();

    CARAPI_(void) NativeRemoveExpiredCookies();

    CARAPI_(void) NativeFlushCookieStore();

    CARAPI_(Boolean) NativeHasCookies();

    CARAPI_(Boolean) NativeAllowFileSchemeCookies();

    CARAPI_(void) NativeSetAcceptFileSchemeCookies(
        /* [in] */ Boolean accept);
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCOOKIEMANAGER_H__
