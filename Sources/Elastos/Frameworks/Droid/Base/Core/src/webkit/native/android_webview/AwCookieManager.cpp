
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//       AwBrowserProcess::CookieCallback::InnerRunnable
//===============================================================
AwBrowserProcess::CookieCallback::InnerRunnable::InnerRunnable(
    /* [in] */ CookieCallback* owner,
    /* [in] */ IInterface* t)
    : mOwner(owner)
    , mT(t)
{
}

ECode AwBrowserProcess::CookieCallback::InnerRunnable::Run()
{
    return mOwner->mCallback->OnReceiveValue(mT);
}

//===============================================================
//               AwBrowserProcess::CookieCallback
//===============================================================

AwBrowserProcess::CookieCallback::CookieCallback(
    /* [in] */ IValueCallback* callback,
    /* [in] */ IHandler* handler)
    : mCallback(callback)
    , mHandler(handler)
{
}

AutoPtr<CookieCallback> AwBrowserProcess::CookieCallback::Convert(
    /* [in] */ IValueCallback* callback)
{
    if (callback == NULL) {
        return NULL;
    }

    // if (Looper.myLooper() == null) {
    //   throw new IllegalStateException(
    //       "CookieCallback.convert should be called on a thread with a running Looper.");
    // }

    AutoPtr<IHandler> handler;
    CHandler::New((IHandler**)&handler);
    AutoPtr<CookieCallback> cookieCallback = new CookieCallback(callback, handler);

    return cookieCallback;
}

void AwBrowserProcess::CookieCallback::OnReceiveValue(
    /* [in] */ const IInterface* t)
{
    AutoPtr<IRunnable> runnable = new InnerRunnable(this, t);
    mHandler->Post(runnable);
}

//===============================================================
//                      AwBrowserProcess
//===============================================================

// TODO(hjd): remove after landing android update to use new calls.
void AwBrowserProcess::RemoveExpiredCookie()
{
    removeExpiredCookies();
}

// TODO(hjd): remove after landing android update to use new calls.
void AwBrowserProcess::RemoveAllCookie()
{
    RemoveAllCookies();
}

// TODO(hjd): remove after landing android update to use new calls.
void AwBrowserProcess::RemoveSessionCookie()
{
    RemoveSessionCookies();
}

/**
 * Control whether cookie is enabled or disabled
 * @param accept TRUE if accept cookie
 */
void AwBrowserProcess::SetAcceptCookie(
    /* [in] */ Boolean accept)
{
    NativeSetShouldAcceptCookies(accept);
}

/**
 * Return whether cookie is enabled
 * @return TRUE if accept cookie
 */
Boolean void AwBrowserProcess::AcceptCookie()
{
    return NativeGetShouldAcceptCookies();
}

/**
 * Synchronous version of setCookie.
 */
void AwBrowserProcess::SetCookie(
    /* [in] */ String url,
    /* [in] */ String value)
{
    NativeSetCookieSync(url, value);
}

/**
 * Deprecated synchronous version of removeSessionCookies.
 */
void AwBrowserProcess::removeSessionCookies()
{
    NativeRemoveSessionCookiesSync();
}

/**
 * Deprecated synchronous version of removeAllCookies.
 */
void AwBrowserProcess::RemoveAllCookies()
{
    NativeRemoveAllCookiesSync();
}

/**
 * Set cookie for a given url. The old cookie with same host/path/name will
 * be removed. The new cookie will be added if it is not expired or it does
 * not have expiration which implies it is session cookie.
 * @param url The url which cookie is set for.
 * @param value The value for set-cookie: in http response header.
 * @param callback A callback called with the success status after the cookie is set.
 */
void AwBrowserProcess::SetCookie(
    /* [in] */ const String& url,
    /* [in] */ const String& value,
    /* [in] */ const IValueCallback* callback)
{
    //try {
        NativeSetCookie(url, value, CookieCallback->Convert(callback));
    //} catch (IllegalStateException e) {
    //    throw new IllegalStateException(
    //            "SetCookie must be called on a thread with a running Looper.");
    //}
}

/**
 * Get cookie(s) for a given url so that it can be set to "cookie:" in http
 * request header.
 * @param url The url needs cookie
 * @return The cookies in the format of NAME=VALUE [; NAME=VALUE]
 */
String AwBrowserProcess::GetCookie(
    /* [in] */ const String& url)
{
    String cookie = NativeGetCookie(url.ToString());
    // Return null if the string is empty to match legacy behavior
    return cookie == NULL || cookie.Trim().IsEmpty() ? NULL : cookie;
}

/**
 * Remove all session cookies, the cookies without an expiration date.
 * The value of the callback is true iff at least one cookie was removed.
 * @param callback A callback called after the cookies (if any) are removed.
 */
void AwBrowserProcess::RemoveSessionCookies(
    /* [in] */ IValueCallback* callback)
{
    //try {
        NativeRemoveSessionCookies(CookieCallback::Convert(callback));
    //} catch (IllegalStateException e) {
    //    throw new IllegalStateException(
    //            "removeSessionCookies must be called on a thread with a running Looper.");
    //}
}

/**
 * Remove all cookies.
 * The value of the callback is true iff at least one cookie was removed.
 * @param callback A callback called after the cookies (if any) are removed.
 */
void AwBrowserProcess::RemoveAllCookies(
    /* [in] */ IValueCallback* callback)
{
    //try {
        NativeRemoveAllCookies(CookieCallback::Convert(callback));
    //} catch (IllegalStateException e) {
    //    throw new IllegalStateException(
    //            "removeAllCookies must be called on a thread with a running Looper.");
    //}
}

/**
 *  Return true if there are stored cookies.
 */
Boolean AwBrowserProcess::HasCookies()
{
    return NativeHasCookies();
}

/**
 * Remove all expired cookies
 */
void AwBrowserProcess::RemoveExpiredCookies()
{
    NativeRemoveExpiredCookies();
}

void AwBrowserProcess::FlushCookieStore()
{
    NativeFlushCookieStore();
}

/**
 * Whether cookies are accepted for file scheme URLs.
 */
Boolean AwBrowserProcess::AllowFileSchemeCookies()
{
    return NativeAllowFileSchemeCookies();
}

/**
 * Sets whether cookies are accepted for file scheme URLs.
 *
 * Use of cookies with file scheme URLs is potentially insecure. Do not use this feature unless
 * you can be sure that no unintentional sharing of cookie data can take place.
 * <p>
 * Note that calls to this method will have no effect if made after a WebView or CookieManager
 * instance has been created.
 */
void AwBrowserProcess::SetAcceptFileSchemeCookies(
    /* [in] */ Boolean accept)
{
    NativeSetAcceptFileSchemeCookies(accept);
}

//@CalledByNative
void AwBrowserProcess::InvokeBooleanCookieCallback(
    /* [in] */ CookieCallback* callback,
    /* [in] */ Boolean result)
{
    callback->OnReceiveValue(result);
}

void AwBrowserProcess::NativeSetShouldAcceptCookies(
    /* [in] */ Boolean accept)
{
}

Boolean AwBrowserProcess::NativeGetShouldAcceptCookies()
{
}

void AwBrowserProcess::NativeSetCookie(
    /* [in] */ String url,
    /* [in] */ String value,
    /* [in] */ CookieCallback* callback)
{
}

void AwBrowserProcess::NativeSetCookieSync(
    /* [in] */ String url,
    /* [in] */ String value)
{
}

String AwBrowserProcess::NativeGetCookie(
    /* [in] */ String url)
{
}

void AwBrowserProcess::NativeRemoveSessionCookies(
    /* [in] */ CookieCallback* callback)
{
}

void AwBrowserProcess::NativeRemoveSessionCookiesSync()
{
}

void AwBrowserProcess::NativeRemoveAllCookies(
    /* [in] */ CookieCallback* callback)
{
}

void AwBrowserProcess::NativeRemoveAllCookiesSync()
{
}

void AwBrowserProcess::NativeRemoveExpiredCookies()
{
}

void AwBrowserProcess::NativeFlushCookieStore()
{
}

Boolean AwBrowserProcess::NativeHasCookies()
{
}

Boolean AwBrowserProcess::NativeAllowFileSchemeCookies()
{
}

void AwBrowserProcess::NativeSetAcceptFileSchemeCookies(
    /* [in] */ Boolean accept)
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
