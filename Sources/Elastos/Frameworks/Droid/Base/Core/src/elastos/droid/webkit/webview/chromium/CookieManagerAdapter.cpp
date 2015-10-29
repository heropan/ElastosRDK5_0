
#include "elastos/droid/webkit/webview/chromium/CookieManagerAdapter.h"
//#include "elastos/droid/webkit/WebView.h"

//using Elastos::Droid::Webkit::WebView;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

//=====================================================================
//                         CookieManagerAdapter
//=====================================================================
const String CookieManagerAdapter::LOGTAG("CookieManager");

CookieManagerAdapter::CookieManagerAdapter(
    /* [in] */ AwCookieManager* chromeCookieManager)
    : mChromeCookieManager(chromeCookieManager)
{
    // ==================before translated======================
    // mChromeCookieManager = chromeCookieManager;
}

// synchronized
ECode CookieManagerAdapter::SetAcceptCookie(
    /* [in] */ Boolean accept)
{
    // ==================before translated======================
    // mChromeCookieManager.setAcceptCookie(accept);

    mChromeCookieManager->SetAcceptCookie(accept);
    return NOERROR;
}

// synchronized
Boolean CookieManagerAdapter::AcceptCookie()
{
    // ==================before translated======================
    // return mChromeCookieManager.acceptCookie();

    return mChromeCookieManager->AcceptCookie();
}

// synchronized
ECode CookieManagerAdapter::SetAcceptThirdPartyCookies(
    /* [in] */ WebView* webView,
    /* [in] */ Boolean accept)
{
    VALIDATE_NOT_NULL(webView);
    // ==================before translated======================
    // webView.getSettings().setAcceptThirdPartyCookies(accept);

    assert(0);
    //webView->GetSettings()->SetAcceptThirdPartyCookies(accept);
    return NOERROR;
}

// synchronized
Boolean CookieManagerAdapter::AcceptThirdPartyCookies(
    /* [in] */ WebView* webView)
{
    // ==================before translated======================
    // return webView.getSettings().getAcceptThirdPartyCookies();

    assert(0);
    //return webView->GetSettings()->GetAcceptThirdPartyCookies();
    return FALSE;
}

ECode CookieManagerAdapter::SetCookie(
    /* [in] */ const String& url,
    /* [in] */ const String& value)
{
    // ==================before translated======================
    // try {
    //     mChromeCookieManager.setCookie(fixupUrl(url), value);
    // } catch (ParseException e) {
    //     Log.e(LOGTAG, "Not setting cookie due to error parsing URL: " + url, e);
    // }

    // try {
        mChromeCookieManager->SetCookie(FixupUrl(url), value);
    // } catch (ParseException e) {
    //     Log.e(LOGTAG, "Not setting cookie due to error parsing URL: " + url, e);
    // }
    return NOERROR;
}

ECode CookieManagerAdapter::SetCookie(
    /* [in] */ const String& url,
    /* [in] */ const String& value,
    /* [in] */ IInterface/*IValueCallback*/* callback)
{
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // try {
    //     mChromeCookieManager.setCookie(fixupUrl(url), value, callback);
    // } catch (ParseException e) {
    //     Log.e(LOGTAG, "Not setting cookie due to error parsing URL: " + url, e);
    // }

    // try {
        mChromeCookieManager->SetCookie(FixupUrl(url), value, callback);
    // } catch (ParseException e) {
    //     Log.e(LOGTAG, "Not setting cookie due to error parsing URL: " + url, e);
    // }
    return NOERROR;
}

String CookieManagerAdapter::GetCookie(
    /* [in] */ const String& url)
{
    // ==================before translated======================
    // try {
    //     return mChromeCookieManager.getCookie(fixupUrl(url));
    // } catch (ParseException e) {
    //     Log.e(LOGTAG, "Unable to get cookies due to error parsing URL: " + url, e);
    //     return null;
    // }

    // try {
        return mChromeCookieManager->GetCookie(FixupUrl(url));
    // } catch (ParseException e) {
    //     Log.e(LOGTAG, "Unable to get cookies due to error parsing URL: " + url, e);
    //     return null;
    // }
    return String("");
}

String CookieManagerAdapter::GetCookie(
    /* [in] */ const String& url,
    /* [in] */ Boolean privateBrowsing)
{
    // ==================before translated======================
    // return getCookie(url);

    return GetCookie(url);
}

// synchronized
String CookieManagerAdapter::GetCookie(
    /* [in] */ WebAddress* uri)
{
    // ==================before translated======================
    // return mChromeCookieManager.getCookie(uri.toString());

    assert(0);
    return mChromeCookieManager->GetCookie(String("")/*uri->ToString()*/);
}

ECode CookieManagerAdapter::RemoveSessionCookie()
{
    // ==================before translated======================
    // mChromeCookieManager.removeSessionCookies();

    mChromeCookieManager->RemoveSessionCookies();
    return NOERROR;
}

ECode CookieManagerAdapter::RemoveSessionCookies(
    /* [in] */ IInterface/*IValueCallback*/* callback)
{
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // mChromeCookieManager.removeSessionCookies(callback);

    mChromeCookieManager->RemoveSessionCookies(callback);
    return NOERROR;
}

ECode CookieManagerAdapter::RemoveAllCookie()
{
    // ==================before translated======================
    // mChromeCookieManager.removeAllCookies();

    mChromeCookieManager->RemoveAllCookies();
    return NOERROR;
}

ECode CookieManagerAdapter::RemoveAllCookies(
    /* [in] */ IInterface/*IValueCallback*/* callback)
{
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // mChromeCookieManager.removeAllCookies(callback);

    mChromeCookieManager->RemoveAllCookies(callback);
    return NOERROR;
}

// synchronized
Boolean CookieManagerAdapter::HasCookies()
{
    // ==================before translated======================
    // return mChromeCookieManager.hasCookies();

    return mChromeCookieManager->HasCookies();
}

// synchronized
Boolean CookieManagerAdapter::HasCookies(
    /* [in] */ Boolean privateBrowsing)
{
    // ==================before translated======================
    // return mChromeCookieManager.hasCookies();

    return mChromeCookieManager->HasCookies();
}

ECode CookieManagerAdapter::RemoveExpiredCookie()
{
    // ==================before translated======================
    // mChromeCookieManager.removeExpiredCookie();

    mChromeCookieManager->RemoveExpiredCookie();
    return NOERROR;
}

ECode CookieManagerAdapter::FlushCookieStore()
{
    // ==================before translated======================
    // mChromeCookieManager.flushCookieStore();

    mChromeCookieManager->FlushCookieStore();
    return NOERROR;
}

Boolean CookieManagerAdapter::AllowFileSchemeCookiesImpl()
{
    // ==================before translated======================
    // return mChromeCookieManager.allowFileSchemeCookies();

    return mChromeCookieManager->AllowFileSchemeCookies();
}

ECode CookieManagerAdapter::SetAcceptFileSchemeCookiesImpl(
    /* [in] */ Boolean accept)
{
    // ==================before translated======================
    // mChromeCookieManager.setAcceptFileSchemeCookies(accept);

    mChromeCookieManager->SetAcceptFileSchemeCookies(accept);
    return NOERROR;
}

String CookieManagerAdapter::FixupUrl(
    /* [in] */ const String& url)
{
    // ==================before translated======================
    // // WebAddress is a private API in the android framework and a "quirk"
    // // of the Classic WebView implementation that allowed embedders to
    // // be relaxed about what URLs they passed into the CookieManager, so we
    // // do the same normalisation before entering the chromium stack.
    // return new WebAddress(url).toString();

    assert(0);
    //WebAddress webAddress(url);
    String result;
    // webAddress.ToString(&result);
    return result;
}

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


