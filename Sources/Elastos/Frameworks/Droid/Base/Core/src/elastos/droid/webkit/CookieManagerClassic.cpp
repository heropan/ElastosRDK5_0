
#include "elastos/droid/webkit/CookieManagerClassic.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

AutoPtr<CookieManagerClassic> CookieManagerClassic::sRef;
const String CookieManagerClassic::LOGTAG("webkit");
Mutex CookieManagerClassic::sLock;

CookieManagerClassic::CookieManagerClassic()
    : mPendingCookieOperations(0)
{
}
/*
static void RemoveSessionCookies(
     WebCookieJar* cookieJar)
{
  net::CookieMonster* cookieMonster = cookieJar->cookieStore()->GetCookieMonster();
  net::CookieList cookies = cookieMonster->GetAllCookies();
  for (net::CookieList::const_iterator iter = cookies.begin(); iter != cookies.end(); ++iter) {
    if (iter->IsSessionCookie())
      cookieMonster->DeleteCanonicalCookie(*iter);
  }
}
*/

//synchronized
AutoPtr<CookieManagerClassic> CookieManagerClassic::GetInstance()
{
    AutoLock lock(sLock);

    if (sRef == NULL) {
        sRef = new CookieManagerClassic();
    }

    return sRef;
}

//synchronized
ECode CookieManagerClassic::SetAcceptCookie(
    /* [in] */ Boolean accept)
{
    AutoLock lock(this);
    NativeSetAcceptCookie(accept);
    return NOERROR;
}

//synchronized
ECode CookieManagerClassic::AcceptCookie(
    /* [out] */ Boolean* result)
{
    AutoLock lock(this);
    Boolean bFlag = NativeAcceptCookie();
    if (result) *result = bFlag;
    return NOERROR;
}

ECode CookieManagerClassic::SetCookie(
    /* [in] */ const String& url,
    /* [in] */ const String& value)
{
    AutoLock lock(this);
    SetCookie(url, value, FALSE);
    return NOERROR;
}

/**
 * See {@link #setCookie(String, String)}
 * @param url The URL for which the cookie is set
 * @param value The value of the cookie, as a string, using the format of
 *              the 'Set-Cookie' HTTP response header
 * @param privateBrowsing Whether to use the private browsing cookie jar
 */
void CookieManagerClassic::SetCookie(
    /* [in] */ const String& url,
    /* [in] */ const String& value,
    /* [in] */ Boolean privateBrowsing)
{
    assert(0);
//    WebAddress uri(url);
    //try {
    //    uri = new WebAddress(url);
    //} catch (ParseException ex) {
    //    Log.e(LOGTAG, "Bad address: " + url);
    //    return;
    //}

//    NativeSetCookie(uri.ToString(), value, privateBrowsing);
}

ECode CookieManagerClassic::GetCookie(
    /* [in] */ const String& url,
    /* [out] */ String* cookie)
{
    return GetCookie(url, FALSE, cookie);
}

ECode CookieManagerClassic::GetCookie(
    /* [in] */ const String& url,
    /* [in] */ Boolean privateBrowsing,
    /* [out] */ String* cookie)
{
    VALIDATE_NOT_NULL(cookie);

    assert(0);
    // TODO
    // WebAddress uri;
    // try {
    //     uri = new WebAddress(url);
    // } catch (ParseException ex) {
    //     Log.e(LOGTAG, "Bad address: " + url);
    //     return null;
    // }

    // return nativeGetCookie(uri.toString(), privateBrowsing);

    return E_NOT_IMPLEMENTED;
}

//synchronized
ECode CookieManagerClassic::GetCookie(
    /* [in] */ IWebAddress* uri,
    /* [out] */ String* cookie)
{
    AutoLock lock(this);
    VALIDATE_NOT_NULL(cookie);
    assert(0);
//    return NativeGetCookie(uri.toString(), false);
    return E_NOT_IMPLEMENTED;
}

/**
 * Waits for pending operations to completed.
 */
void CookieManagerClassic::WaitForCookieOperationsToComplete()
{
    assert(0);
#if 0
    // Note that this function is applicable for both the java
    // and native http stacks, and works correctly with either.
    synchronized (this) {
        while (mPendingCookieOperations > 0) {
            try {
                wait();
            } catch (InterruptedException e) { }
        }
    }
#endif
}

ECode CookieManagerClassic::RemoveSessionCookie()
{
    assert(0);
#if 0
    signalCookieOperationsStart();
    new AsyncTask<Void, Void, Void>() {
        @Override
        protected Void doInBackground(Void... none) {
            nativeRemoveSessionCookie();
            signalCookieOperationsComplete();
            return null;
        }
    }.execute();
#endif
    return NOERROR;
}

ECode CookieManagerClassic::RemoveAllCookie()
{
    NativeRemoveAllCookie();
    return NOERROR;
}

//synchronized
ECode CookieManagerClassic::HasCookies(
    /* [out] */ Boolean* result)
{
    AutoLock lock(this);
    return HasCookies(FALSE, result);
}

//synchronized
ECode CookieManagerClassic::HasCookies(
    /* [in] */ Boolean privateBrowsing,
    /* [out] */ Boolean* result)
{
    AutoLock lock(this);
    VALIDATE_NOT_NULL(result);
    *result = NativeHasCookies(privateBrowsing);
    return NOERROR;
}

ECode CookieManagerClassic::RemoveExpiredCookie()
{
    NativeRemoveExpiredCookie();
    return NOERROR;
}

void CookieManagerClassic::FlushCookieStore()
{
    NativeFlushCookieStore();
}

Boolean CookieManagerClassic::AllowFileSchemeCookiesImpl()
{
    return NativeAcceptFileSchemeCookies();
}

void CookieManagerClassic::SetAcceptFileSchemeCookiesImpl(
    /* [in] */ Boolean accept)
{
    NativeSetAcceptFileSchemeCookies(accept);
}

//synchronized
void CookieManagerClassic::SignalCookieOperationsComplete()
{
    AutoLock lock(this);
    mPendingCookieOperations--;
    assert(mPendingCookieOperations > -1);
//    notify();
}

//synchronized
void CookieManagerClassic::SignalCookieOperationsStart()
{
    AutoLock lock(this);
    mPendingCookieOperations++;
}

// Native functions
Boolean CookieManagerClassic::NativeAcceptCookie()
{
    return Elastos_CookieManager_nativeAcceptCookie();
}

String CookieManagerClassic::NativeGetCookie(
    /* [in] */ const String& url,
    /* [in] */ Boolean privateBrowsing)
{
    return Elastos_CookieManager_nativeGetCookie(url, privateBrowsing);
}

Boolean CookieManagerClassic::NativeHasCookies(
    /* [in] */ Boolean privateBrowsing)
{
    return Elastos_CookieManager_nativeHasCookies(privateBrowsing);
}

void CookieManagerClassic::NativeRemoveAllCookie()
{
    Elastos_CookieManager_nativeRemoveAllCookie();
}

void CookieManagerClassic::NativeRemoveExpiredCookie()
{
    Elastos_CookieManager_nativeRemoveExpiredCookie();
}

void CookieManagerClassic::NativeRemoveSessionCookie()
{
    Elastos_CookieManager_nativeRemoveSessionCookie();
}

void CookieManagerClassic::NativeSetAcceptCookie(
    /* [in] */ Boolean accept)
{
    Elastos_CookieManager_nativeSetAcceptCookie(accept);
}

void CookieManagerClassic::NativeSetCookie(
    /* [in] */ const String& url,
    /* [in] */ const String& value,
    /* [in] */ Boolean privateBrowsing)
{
    Elastos_CookieManager_nativeSetCookie(url, value, privateBrowsing);
}

void CookieManagerClassic::NativeFlushCookieStore()
{
    Elastos_CookieManager_nativeFlushCookieStore();
}

Boolean CookieManagerClassic::NativeAcceptFileSchemeCookies()
{
    return Elastos_CookieManager_nativeAcceptFileSchemeCookies();
}

void CookieManagerClassic::NativeSetAcceptFileSchemeCookies(
    /* [in] */ Boolean accept)
{
    Elastos_CookieManager_nativeSetAcceptFileSchemeCookies(accept);
}

ECode CookieManagerClassic::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "CookieManagerClassic";
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
