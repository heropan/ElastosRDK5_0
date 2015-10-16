#include "elastos/droid/webkit/native/android_webview/AwCookieManager.h"
#include "elastos/droid/os/CHandler.h"
#include "elastos/droid/os/CLooperHelper.h"
#include "elastos/utility/logging/Logger.h"

using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::ILooperHelper;
using Elastos::Droid::Os::CLooperHelper;

using Elastos::Core::EIID_IRunnable;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//       AwCookieManager::CookieCallback::InnerRunnable
//===============================================================
CAR_INTERFACE_IMPL(AwCookieManager::CookieCallback::InnerRunnable, Object, IRunnable);

AwCookieManager::CookieCallback::InnerRunnable::InnerRunnable(
    /* [in] */ CookieCallback* owner,
    /* [in] */ IInterface* t)
    : mOwner(owner)
    , mT(t)
{
}

ECode AwCookieManager::CookieCallback::InnerRunnable::Run()
{
    //TODO return mOwner->mCallback->OnReceiveValue(mT);
    return NOERROR;//TODO remove this
}

//===============================================================
//               AwCookieManager::CookieCallback
//===============================================================

AwCookieManager::CookieCallback::CookieCallback(
    /* [in] */ /*TODO IValueCallback*/IInterface* callback,
    /* [in] */ IHandler* handler)
    : mCallback(callback)
    , mHandler(handler)
{
}

ECode AwCookieManager::CookieCallback::Convert(
    /* [in] */ /*TODO IValueCallback*/IInterface* callback,
    /* [out] */ CookieCallback** cookieCallback)
{
    VALIDATE_NOT_NULL(cookieCallback);
    *cookieCallback = NULL;
    if (callback == NULL) {
        return NOERROR;
    }

    // if (Looper.myLooper() == null) {
    //   throw new IllegalStateException(
    //       "CookieCallback.convert should be called on a thread with a running Looper.");
    // }
    AutoPtr<ILooper> looper;
    AutoPtr<ILooperHelper> looperHelper;
    CLooperHelper::AcquireSingleton((ILooperHelper**)&looperHelper);
    looperHelper->GetMyLooper((ILooper**)&looper);
    if (looper == NULL)
    {
        Logger::E("AwCookieManager", "AwCookieManager::CookieCallback::Convert");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<IHandler> handler;
    CHandler::New((IHandler**)&handler);
    AutoPtr<CookieCallback> cookieCB= new CookieCallback(callback, handler);

    *cookieCallback = cookieCB;
    REFCOUNT_ADD(*cookieCallback);
    return NOERROR;
}

void AwCookieManager::CookieCallback::OnReceiveValue(
    /* [in] */ IInterface* t)
{
    AutoPtr<IRunnable> runnable = new InnerRunnable(this, t);
    Boolean result;
    mHandler->Post(runnable, &result);
}

//===============================================================
//                      AwCookieManager
//===============================================================

// TODO(hjd): remove after landing android update to use new calls.
void AwCookieManager::RemoveExpiredCookie()
{
    RemoveExpiredCookies();
}

// TODO(hjd): remove after landing android update to use new calls.
void AwCookieManager::RemoveAllCookie()
{
    RemoveAllCookies();
}

// TODO(hjd): remove after landing android update to use new calls.
void AwCookieManager::RemoveSessionCookie()
{
    RemoveSessionCookies();
}

/**
 * Control whether cookie is enabled or disabled
 * @param accept TRUE if accept cookie
 */
void AwCookieManager::SetAcceptCookie(
    /* [in] */ Boolean accept)
{
    NativeSetShouldAcceptCookies(accept);
}

/**
 * Return whether cookie is enabled
 * @return TRUE if accept cookie
 */
Boolean AwCookieManager::AcceptCookie()
{
    return NativeGetShouldAcceptCookies();
}

/**
 * Synchronous version of setCookie.
 */
void AwCookieManager::SetCookie(
    /* [in] */ const String& url,
    /* [in] */ const String& value)
{
    NativeSetCookieSync(url, value);
}

/**
 * Deprecated synchronous version of removeSessionCookies.
 */
void AwCookieManager::RemoveSessionCookies()
{
    NativeRemoveSessionCookiesSync();
}

/**
 * Deprecated synchronous version of removeAllCookies.
 */
void AwCookieManager::RemoveAllCookies()
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
ECode AwCookieManager::SetCookie(
    /* [in] */ const String& url,
    /* [in] */ const String& value,
    /* [in] */ /*TODO IValueCallback*/IInterface* callback)
{
    //try {
    AutoPtr<CookieCallback> cookieCallback;
    ECode ecode = CookieCallback::Convert(callback, (CookieCallback**)&cookieCallback);
    if (FAILED(ecode))
    {
        Logger::E("AwCookieManager", "AwCookieManager::SetCookie");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    NativeSetCookie(url, value, cookieCallback);
    return NOERROR;
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
String AwCookieManager::GetCookie(
    /* [in] */ const String& url)
{
    String cookie = NativeGetCookie(url);
    // Return null if the string is empty to match legacy behavior
    //return cookie == NULL || cookie.Trim().IsEmpty() ? NULL : cookie;
    if (cookie.IsNullOrEmpty())
    {
        return String(NULL);
    }
    if (cookie.Trim().IsEmpty())
    {
        return String(NULL);
    }
    return cookie;
}

/**
 * Remove all session cookies, the cookies without an expiration date.
 * The value of the callback is true iff at least one cookie was removed.
 * @param callback A callback called after the cookies (if any) are removed.
 */
ECode AwCookieManager::RemoveSessionCookies(
    /* [in] */ /*TODO IValueCallback*/IInterface* callback)
{
    //try {
    AutoPtr<CookieCallback> cookieCallback;
    ECode ecode = CookieCallback::Convert(callback, (CookieCallback**)&cookieCallback);
    if (FAILED(ecode))
    {
        Logger::E("AwCookieManager", "AwCookieManager::RemoveSessionCookies");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    NativeRemoveSessionCookies(cookieCallback);
    return NOERROR;
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
ECode AwCookieManager::RemoveAllCookies(
    /* [in] */ /*TODO IValueCallback*/IInterface* callback)
{
    //try {
    AutoPtr<CookieCallback> cookieCallback;
    ECode ecode = CookieCallback::Convert(callback, (CookieCallback**)&cookieCallback);
    if (FAILED(ecode))
    {
        Logger::E("AwCookieManager", "AwCookieManager::RemoveSessionCookies");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    NativeRemoveAllCookies(cookieCallback);
    return NOERROR;
    //} catch (IllegalStateException e) {
    //    throw new IllegalStateException(
    //            "removeAllCookies must be called on a thread with a running Looper.");
    //}
}

/**
 *  Return true if there are stored cookies.
 */
Boolean AwCookieManager::HasCookies()
{
    return NativeHasCookies();
}

/**
 * Remove all expired cookies
 */
void AwCookieManager::RemoveExpiredCookies()
{
    NativeRemoveExpiredCookies();
}

void AwCookieManager::FlushCookieStore()
{
    NativeFlushCookieStore();
}

/**
 * Whether cookies are accepted for file scheme URLs.
 */
Boolean AwCookieManager::AllowFileSchemeCookies()
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
void AwCookieManager::SetAcceptFileSchemeCookies(
    /* [in] */ Boolean accept)
{
    NativeSetAcceptFileSchemeCookies(accept);
}

//@CalledByNative
void AwCookieManager::InvokeBooleanCookieCallback(
    /* [in] */ IInterface* callback,
    /* [in] */ Boolean result)
{
    AutoPtr<CookieCallback> cookieCB = (CookieCallback*)IObject::Probe(callback);
    AutoPtr<IBoolean> res;
    CBoolean::New(result, (IBoolean**)&res);
    cookieCB->OnReceiveValue(res);
}

void AwCookieManager::NativeSetShouldAcceptCookies(
    /* [in] */ Boolean accept)
{
}

Boolean AwCookieManager::NativeGetShouldAcceptCookies()
{
    return FALSE;
}

void AwCookieManager::NativeSetCookie(
    /* [in] */ const String& url,
    /* [in] */ const String& value,
    /* [in] */ CookieCallback* callback)
{
}

void AwCookieManager::NativeSetCookieSync(
    /* [in] */ const String& url,
    /* [in] */ const String& value)
{
}

String AwCookieManager::NativeGetCookie(
    /* [in] */ const String& url)
{
    return String(NULL);
}

void AwCookieManager::NativeRemoveSessionCookies(
    /* [in] */ CookieCallback* callback)
{
}

void AwCookieManager::NativeRemoveSessionCookiesSync()
{
}

void AwCookieManager::NativeRemoveAllCookies(
    /* [in] */ CookieCallback* callback)
{
}

void AwCookieManager::NativeRemoveAllCookiesSync()
{
}

void AwCookieManager::NativeRemoveExpiredCookies()
{
}

void AwCookieManager::NativeFlushCookieStore()
{
}

Boolean AwCookieManager::NativeHasCookies()
{
    return FALSE;
}

Boolean AwCookieManager::NativeAllowFileSchemeCookies()
{
    return FALSE;
}

void AwCookieManager::NativeSetAcceptFileSchemeCookies(
    /* [in] */ Boolean accept)
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
