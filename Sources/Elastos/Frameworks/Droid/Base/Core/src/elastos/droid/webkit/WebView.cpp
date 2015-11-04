
#include "elastos/droid/os/Looper.h"
#include "elastos/droid/webkit/WebView.h"
#include "elastos/droid/webkit/WebViewFactory.h"
#include "elastos/droid/webkit/CPluginList.h"
#include "elastos/droid/R.h"
#include <elastos/core/AutoLock.h>

using Elastos::Droid::Os::Looper;
using Elastos::Droid::View::Accessibility::IAccessibilityRecord;
using Elastos::Core::AutoLock;
using Elastos::Core::CString;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Webkit {

//=================================================================
//     WebView::WebViewTransport
//=================================================================

CAR_INTERFACE_IMPL(WebView::WebViewTransport, Object, IWebViewTransport);

ECode WebView::WebViewTransport::constructor()
{
    return NOERROR;
}

ECode WebView::WebViewTransport::SetWebView(
    /* [in] */ IWebView* webview)
{
    AutoLock lock(this);
    mWebview = webview;
    return NOERROR;
}

ECode WebView::WebViewTransport::GetWebView(
    /* [out] */ IWebView** webView)
{
    AutoLock lock(this);
    *webView = mWebview;
    REFCOUNT_ADD(*webView);
    return NOERROR;
}

ECode WebView::WebViewTransport::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "WebView::WebViewTransport";
    return NOERROR;
}

//=================================================================
//     WebView::PrivateAccess
//=================================================================

CAR_INTERFACE_IMPL(WebView::PrivateAccess, Object, IWebViewPrivateAccess)

// ---- Access to super-class methods ----
ECode WebView::PrivateAccess::Super_getScrollBarStyle(
    /* [out] */ Int32* style)
{
    VALIDATE_NOT_NULL(style);
    assert(0);
    // TODO
    // *style = mHost->AbsoluteLayout::GetScrollBarStyle();
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::Super_scrollTo(
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY)
{
    assert(0);
    // TODO
    // mHost->AbsoluteLayout::ScrollTo(scrollX, scrollY);
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::Super_computeScroll()
{
    assert(0);
    // TODO
    // mHost->AbsoluteLayout::ComputeScroll();
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::Super_onHoverEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    assert(0);
    // TODO
    // *result = mHost->AbsoluteLayout::OnHoverEvent(event);
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::Super_performAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    assert(0);
    // TODO
    // *result = mHost->AbsoluteLayout::PerformAccessibilityAction(action, arguments);
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::Super_performLongClick(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    assert(0);
    // TODO
    // *result = mHost->AbsoluteLayout::PerformLongClick();
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::Super_setFrame(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    assert(0);
    // TODO
    // *result = mHost->AbsoluteLayout::SetFrame(left, top, right, bottom);
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::Super_dispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    assert(0);
    // TODO
    // *result = mHost->AbsoluteLayout::DispatchKeyEvent(event);
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::Super_onGenericMotionEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    assert(0);
    // TODO
    // *result = mHost->AbsoluteLayout::OnGenericMotionEvent(event);
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::Super_requestFocus(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    assert(0);
    // TODO
    // *result = mHost->AbsoluteLayout::RequestFocus(direction, previouslyFocusedRect);
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::Super_setLayoutParams(
    /* [in] */ IViewGroupLayoutParams* params)
{
    assert(0);
    // TODO
    // mHost->AbsoluteLayout::SetLayoutParams(params);
    return E_NOT_IMPLEMENTED;
}

// ---- Access to non-public methods ----
ECode WebView::PrivateAccess::OverScrollBy(
    /* [in] */ Int32 deltaX,
    /* [in] */ Int32 deltaY,
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Int32 scrollRangeX,
    /* [in] */ Int32 scrollRangeY,
    /* [in] */ Int32 maxOverScrollX,
    /* [in] */ Int32 maxOverScrollY,
    /* [in] */ Boolean isTouchEvent)
{
    assert(0);
    // TODO
    // mHost->OverScrollBy(deltaX, deltaY, scrollX, scrollY, scrollRangeX, scrollRangeY,
    //                 maxOverScrollX, maxOverScrollY, isTouchEvent);
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::AwakenScrollBars(
    /* [in] */ Int32 duration)
{
    assert(0);
    // TODO
    // mHost->AwakenScrollBars(duration);
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::AwakenScrollBars(
    /* [in] */ Int32 duration,
    /* [in] */ Boolean invalidate)
{
    assert(0);
    // TODO
    // mHost->AwakenScrollBars(duration, invalidate);
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::GetVerticalScrollFactor(
    /* [out] */ Float* factor)
{
    VALIDATE_NOT_NULL(factor);
    assert(0);
    // TODO
    // *factor = mHost->GetVerticalScrollFactor();
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::GetHorizontalScrollFactor(
    /* [out] */ Float* factor)
{
    VALIDATE_NOT_NULL(factor);
    assert(0);
    // TODO
    // *factor = mHost->GetHorizontalScrollFactor();
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::SetMeasuredDimension(
    /* [in] */ Int32 measuredWidth,
    /* [in] */ Int32 measuredHeight)
{
    assert(0);
    // TODO
    // mHost->SetMeasuredDimension(measuredWidth, measuredHeight);
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::OnScrollChanged(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 oldl,
    /* [in] */ Int32 oldt)
{
    assert(0);
    // TODO
    // mHost->OnScrollChanged(l, t, oldl, oldt);
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::GetHorizontalScrollbarHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    assert(0);
    // TODO
    // *height = mHost->GetHorizontalScrollbarHeight();
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::Super_onDrawVerticalScrollBar(
            /* [in] */ ICanvas* canvas,
            /* [in] */ IDrawable* scrollBar,
            /* [in] */ Int32 l,
            /* [in] */ Int32 t,
            /* [in] */ Int32 r,
            /* [in] */ Int32 b)
{
    assert(0);
    // TODO
    return E_NOT_IMPLEMENTED;
}

// ---- Access to (non-public) fields ----
/** Raw setter for the scroll X value, without invoking onScrollChanged handlers etc. */
ECode WebView::PrivateAccess::SetScrollXRaw(
    /* [in] */ Int32 scrollX)
{
    assert(0);
    // TODO
    // mHost->mScrollX = scrollX;
    return E_NOT_IMPLEMENTED;
}

/** Raw setter for the scroll Y value, without invoking onScrollChanged handlers etc. */
ECode WebView::PrivateAccess::SetScrollYRaw(
    /* [in] */ Int32 scrollY)
{
    assert(0);
    // TODO
    // mHost->mScrollY = scrollY;
    return E_NOT_IMPLEMENTED;
}

ECode WebView::PrivateAccess::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "WebView::PrivateAccess";
    return NOERROR;
}

//=================================================================
//     WebView
//=================================================================

const String WebView::LOGTAG("WebView_proxy");
/**
 * URI scheme for telephone number.
 */
const String WebView::SCHEME_TEL("tel:");
/**
 * URI scheme for email address.
 */
const String WebView::SCHEME_MAILTO("mailto:");
/**
 * URI scheme for map address.
 */
const String WebView::SCHEME_GEO("geo:0,0?q=");

Object WebView::sLock;

CAR_INTERFACE_IMPL(WebView, Object, IWebView);

WebView::WebView()
{
}

/**
 * Constructs a new WebView with a Context object.
 *
 * @param context a Context object used to access application assets
 */
WebView::WebView(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(Init(context, NULL));
}

/**
 * Constructs a new WebView with layout parameters.
 *
 * @param context a Context object used to access application assets
 * @param attrs an AttributeSet passed to our parent
 */
WebView::WebView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(Init(context, attrs, R::attr::webViewStyle));
}

/**
 * Constructs a new WebView with layout parameters and a default style.
 *
 * @param context a Context object used to access application assets
 * @param attrs an AttributeSet passed to our parent
 * @param defStyle the default style resource ID
 */
WebView::WebView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(Init(context, attrs, defStyle, FALSE));
}

/**
 * Constructs a new WebView with layout parameters and a default style.
 *
 * @param context a Context object used to access application assets
 * @param attrs an AttributeSet passed to our parent
 * @param defStyle the default style resource ID
 * @param privateBrowsing whether this WebView will be initialized in
 *                        private mode
 *
 * @deprecated Private browsing is no longer supported directly via
 * WebView and will be removed in a future release. Prefer using
 * {@link WebSettings}, {@link WebViewDatabase}, {@link CookieManager}
 * and {@link WebStorage} for fine-grained control of privacy data.
 */
WebView::WebView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Boolean privateBrowsing)
{
    ASSERT_SUCCEEDED(Init(context, attrs, defStyle, NULL, privateBrowsing));
}

/**
 * Constructs a new WebView with layout parameters, a default style and a set
 * of custom Javscript interfaces to be added to this WebView at initialization
 * time. This guarantees that these interfaces will be available when the JS
 * context is initialized.
 *
 * @param context a Context object used to access application assets
 * @param attrs an AttributeSet passed to our parent
 * @param defStyle the default style resource ID
 * @param javaScriptInterfaces a Map of interface names, as keys, and
 *                             object implementing those interfaces, as
 *                             values
 * @param privateBrowsing whether this WebView will be initialized in
 *                        private mode
 * @hide This is used internally by dumprendertree, as it requires the javaScript interfaces to
 *       be added synchronously, before a subsequent loadUrl call takes effect.
 */
WebView::WebView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ IMap* javaScriptInterfaces,
    /* [in] */ Boolean privateBrowsing)
{
    ASSERT_SUCCEEDED(Init(context, attrs, defStyle, javaScriptInterfaces,
            privateBrowsing));
}

ECode WebView::constructor(
    /* [in] */ IContext * context)
{
    return Init(context);
}

ECode WebView::constructor(
    /* [in] */ IContext * context,
    /* [in] */ IAttributeSet * attrs)
{
    return Init(context, attrs);
}

ECode WebView::constructor(
    /* [in] */ IContext * context,
    /* [in] */ IAttributeSet * attrs,
    /* [in] */ Int32 defStyle)
{
    return Init(context, attrs, defStyle);
}

ECode WebView::constructor(
    /* [in] */ IContext * context,
    /* [in] */ IAttributeSet * attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Boolean privateBrowsing)
{
    return Init(context, attrs, defStyle, privateBrowsing);
}

//////////////////////////////////////////////////////////////
/**
 * Constructs a new WebView with a Context object.
 *
 * @param context a Context object used to access application assets
 */
ECode WebView::Init(
    /* [in] */ IContext* context)
{
    return Init(context, NULL);
}

/**
 * Constructs a new WebView with layout parameters.
 *
 * @param context a Context object used to access application assets
 * @param attrs an AttributeSet passed to our parent
 */
ECode WebView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs, R::attr::webViewStyle);
}

/**
 * Constructs a new WebView with layout parameters and a default style.
 *
 * @param context a Context object used to access application assets
 * @param attrs an AttributeSet passed to our parent
 * @param defStyle the default style resource ID
 */
ECode WebView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return Init(context, attrs, defStyle, FALSE);
}

/**
 * Constructs a new WebView with layout parameters and a default style.
 *
 * @param context a Context object used to access application assets
 * @param attrs an AttributeSet passed to our parent
 * @param defStyle the default style resource ID
 * @param privateBrowsing whether this WebView will be initialized in
 *                        private mode
 *
 * @deprecated Private browsing is no longer supported directly via
 * WebView and will be removed in a future release. Prefer using
 * {@link WebSettings}, {@link WebViewDatabase}, {@link CookieManager}
 * and {@link WebStorage} for fine-grained control of privacy data.
 */
ECode WebView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Boolean privateBrowsing)
{
    return Init(context, attrs, defStyle, NULL, privateBrowsing);
}

ECode WebView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ IMap* javaScriptInterfaces,
    /* [in] */ Boolean privateBrowsing)
{
    if (context == NULL) {
        //throw new IllegalArgumentException("Invalid context argument");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    assert(0);
    // TODO
    // FAIL_RETURN(AbsoluteLayout::Init(context, attrs, defStyle));
    CheckThread();

    EnsureProviderCreated();
    return mProvider->Init(javaScriptInterfaces, privateBrowsing);
}

/**
 * Specifies whether the horizontal scrollbar has overlay style.
 *
 * @param overlay true if horizontal scrollbar should have overlay style
 */
ECode WebView::SetHorizontalScrollbarOverlay(
    /* [in] */ Boolean overlay)
{
    CheckThread();
    return mProvider->SetHorizontalScrollbarOverlay(overlay);
}

/**
 * Specifies whether the vertical scrollbar has overlay style.
 *
 * @param overlay true if vertical scrollbar should have overlay style
 */
ECode WebView::SetVerticalScrollbarOverlay(
    /* [in] */ Boolean overlay)
{
    CheckThread();
    return mProvider->SetVerticalScrollbarOverlay(overlay);
}

/**
 * Gets whether horizontal scrollbar has overlay style.
 *
 * @return true if horizontal scrollbar has overlay style
 */
ECode WebView::OverlayHorizontalScrollbar(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    CheckThread();
    return mProvider->OverlayHorizontalScrollbar(result);
}

/**
 * Gets whether vertical scrollbar has overlay style.
 *
 * @return true if vertical scrollbar has overlay style
 */
ECode WebView::OverlayVerticalScrollbar(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    CheckThread();
    return mProvider->OverlayVerticalScrollbar(result);
}

/**
 * Gets the visible height (in pixels) of the embedded title bar (if any).
 *
 * @deprecated This method is now obsolete.
 * @hide Since API level {@link android.os.Build.VERSION_CODES#JELLY_BEAN_MR1}
 */
ECode WebView::GetVisibleTitleHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    CheckThread();
    return mProvider->GetVisibleTitleHeight(height);
}

/**
 * Gets the SSL certificate for the main top-level page or null if there is
 * no certificate (the site is not secure).
 *
 * @return the SSL certificate for the main top-level page
 */
ECode WebView::GetCertificate(
    /* [out] */ ISslCertificate** cert)
{
    VALIDATE_NOT_NULL(cert);
    CheckThread();
    return mProvider->GetCertificate(cert);
}

/**
 * Sets the SSL certificate for the main top-level page.
 *
 * @deprecated Calling this function has no useful effect, and will be
 * ignored in future releases.
 */
//@Deprecated
ECode WebView::SetCertificate(
    /* [in] */ ISslCertificate* certificate)
{
    CheckThread();
    return mProvider->SetCertificate(certificate);
}

//-------------------------------------------------------------------------
// Methods called by activity
//-------------------------------------------------------------------------

/**
 * Sets a username and password pair for the specified host. This data is
 * used by the WebView to autocomplete username and password fields in web
 * forms. Note that this is unrelated to the credentials used for HTTP
 * authentication.
 *
 * @param host the host that required the credentials
 * @param username the username for the given host
 * @param password the password for the given host
 * @see WebViewDatabase#clearUsernamePassword
 * @see WebViewDatabase#hasUsernamePassword
 */
ECode WebView::SavePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    CheckThread();
    return mProvider->SavePassword(host, username, password);
}

/**
 * Stores HTTP authentication credentials for a given host and realm. This
 * method is intended to be used with
 * {@link WebViewClient#onReceivedHttpAuthRequest}.
 *
 * @param host the host to which the credentials apply
 * @param realm the realm to which the credentials apply
 * @param username the username
 * @param password the password
 * @see getHttpAuthUsernamePassword
 * @see WebViewDatabase#hasHttpAuthUsernamePassword
 * @see WebViewDatabase#clearHttpAuthUsernamePassword
 */
ECode WebView::SetHttpAuthUsernamePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    CheckThread();
    return mProvider->SetHttpAuthUsernamePassword(host, realm, username, password);
}

/**
 * Retrieves HTTP authentication credentials for a given host and realm.
 * This method is intended to be used with
 * {@link WebViewClient#onReceivedHttpAuthRequest}.
 *
 * @param host the host to which the credentials apply
 * @param realm the realm to which the credentials apply
 * @return the credentials as a String array, if found. The first element
 *         is the username and the second element is the password. Null if
 *         no credentials are found.
 * @see setHttpAuthUsernamePassword
 * @see WebViewDatabase#hasHttpAuthUsernamePassword
 * @see WebViewDatabase#clearHttpAuthUsernamePassword
 */
ECode WebView::GetHttpAuthUsernamePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [out, callee] */ ArrayOf<String>** up)
{
    VALIDATE_NOT_NULL(up);
    CheckThread();
    return mProvider->GetHttpAuthUsernamePassword(host, realm, up);
}

/**
 * Destroys the internal state of this WebView. This method should be called
 * after this WebView has been removed from the view system. No other
 * methods may be called on this WebView after destroy.
 */
ECode WebView::Destroy()
{
    CheckThread();
    return mProvider->Destroy();
}

/**
 * Enables platform notifications of data state and proxy changes.
 * Notifications are enabled by default.
 *
 * @deprecated This method is now obsolete.
 * @hide Since API level {@link android.os.Build.VERSION_CODES#JELLY_BEAN_MR1}
 */
void WebView::EnablePlatformNotifications()
{
    CheckThread();
    AutoPtr<IWebViewFactoryProvider::IStatics> starts;
    GetFactory()->GetStatics((IWebViewFactoryProvider::IStatics**)&starts);
    starts->SetPlatformNotificationsEnabled(TRUE);
}

/**
 * Disables platform notifications of data state and proxy changes.
 * Notifications are enabled by default.
 *
 * @deprecated This method is now obsolete.
 * @hide Since API level {@link android.os.Build.VERSION_CODES#JELLY_BEAN_MR1}
 */
void WebView::DisablePlatformNotifications()
{
    CheckThread();
    AutoPtr<IWebViewFactoryProvider::IStatics> starts;
    GetFactory()->GetStatics((IWebViewFactoryProvider::IStatics**)&starts);
    starts->SetPlatformNotificationsEnabled(FALSE);
}

/**
 * Informs WebView of the network state. This is used to set
 * the JavaScript property window.navigator.isOnline and
 * generates the online/offline event as specified in HTML5, sec. 5.7.7
 *
 * @param networkUp a boolean indicating if network is available
 */
ECode WebView::SetNetworkAvailable(
    /* [in] */ Boolean networkUp)
{
    CheckThread();
    return mProvider->SetNetworkAvailable(networkUp);
}

/**
 * Saves the state of this WebView used in
 * {@link android.app.Activity#onSaveInstanceState}. Please note that this
 * method no longer stores the display data for this WebView. The previous
 * behavior could potentially leak files if {@link #restoreState} was never
 * called.
 *
 * @param outState the Bundle to store this WebView's state
 * @return the same copy of the back/forward list used to save the state. If
 *         saveState fails, the returned list will be null.
 */
ECode WebView::SaveState(
    /* [in] */ IBundle* outState,
    /* [out] */ IWebBackForwardList** wfl)
{
    VALIDATE_NOT_NULL(wfl);
    CheckThread();
    return mProvider->SaveState(outState, wfl);
}

/**
 * Saves the current display data to the Bundle given. Used in conjunction
 * with {@link #saveState}.
 * @param b a Bundle to store the display data
 * @param dest the file to store the serialized picture data. Will be
 *             overwritten with this WebView's picture data.
 * @return true if the picture was successfully saved
 * @deprecated This method is now obsolete.
 * @hide Since API level {@link android.os.Build.VERSION_CODES#JELLY_BEAN_MR1}
 */
ECode WebView::SavePicture(
    /* [in] */ IBundle* b,
    /* [in] */ IFile* dest,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    CheckThread();
    return mProvider->SavePicture(b, dest, result);
}

/**
 * Restores the display data that was saved in {@link #savePicture}. Used in
 * conjunction with {@link #restoreState}. Note that this will not work if
 * this WebView is hardware accelerated.
 *
 * @param b a Bundle containing the saved display data
 * @param src the file where the picture data was stored
 * @return true if the picture was successfully restored
 * @deprecated This method is now obsolete.
 * @hide Since API level {@link android.os.Build.VERSION_CODES#JELLY_BEAN_MR1}
 */
ECode WebView::RestorePicture(
    /* [in] */ IBundle* b,
    /* [in] */ IFile* src,
    /* [out] */ Boolean* result)
{
    CheckThread();
    return mProvider->RestorePicture(b, src, result);
}

/**
 * Restores the state of this WebView from the given Bundle. This method is
 * intended for use in {@link android.app.Activity#onRestoreInstanceState}
 * and should be called to restore the state of this WebView. If
 * it is called after this WebView has had a chance to build state (load
 * pages, create a back/forward list, etc.) there may be undesirable
 * side-effects. Please note that this method no longer restores the
 * display data for this WebView.
 *
 * @param inState the incoming Bundle of state
 * @return the restored back/forward list or null if restoreState failed
 */
ECode WebView::RestoreState(
    /* [in] */ IBundle* inState,
    /* [out] */ IWebBackForwardList** wfl)
{
    VALIDATE_NOT_NULL(wfl);
    CheckThread();
    return mProvider->RestoreState(inState, wfl);
}

/**
 * Loads the given URL with the specified additional HTTP headers.
 *
 * @param url the URL of the resource to load
 * @param additionalHttpHeaders the additional headers to be used in the
 *            HTTP request for this URL, specified as a map from name to
 *            value. Note that if this map contains any of the headers
 *            that are set by default by this WebView, such as those
 *            controlling caching, accept types or the User-Agent, their
 *            values may be overriden by this WebView's defaults.
 */
ECode WebView::LoadUrl(
    /* [in] */ const String& url,
    /* [in] */ IMap* additionalHttpHeaders)
{
    CheckThread();
    return mProvider->LoadUrl(url, additionalHttpHeaders);
}

/**
 * Loads the given URL.
 *
 * @param url the URL of the resource to load
 */
ECode WebView::LoadUrl(
    /* [in] */ const String& url)
{
    CheckThread();
    return mProvider->LoadUrl(url);
}

/**
 * Loads the URL with postData using "POST" method into this WebView. If url
 * is not a network URL, it will be loaded with {link
 * {@link #loadUrl(String)} instead.
 *
 * @param url the URL of the resource to load
 * @param postData the data will be passed to "POST" request
 */
ECode WebView::PostUrl(
    /* [in] */ const String& url,
    /* [in] */ ArrayOf<Byte>* postData)
{
    CheckThread();
    return mProvider->PostUrl(url, postData);
}

/**
 * Loads the given data into this WebView using a 'data' scheme URL.
 * <p>
 * Note that JavaScript's same origin policy means that script running in a
 * page loaded using this method will be unable to access content loaded
 * using any scheme other than 'data', including 'http(s)'. To avoid this
 * restriction, use {@link
 * #loadDataWithBaseURL(String,String,String,String,String)
 * loadDataWithBaseURL()} with an appropriate base URL.
 * <p>
 * The encoding parameter specifies whether the data is base64 or URL
 * encoded. If the data is base64 encoded, the value of the encoding
 * parameter must be 'base64'. For all other values of the parameter,
 * including null, it is assumed that the data uses ASCII encoding for
 * octets inside the range of safe URL characters and use the standard %xx
 * hex encoding of URLs for octets outside that range. For example, '#',
 * '%', '\', '?' should be replaced by %23, %25, %27, %3f respectively.
 * <p>
 * The 'data' scheme URL formed by this method uses the default US-ASCII
 * charset. If you need need to set a different charset, you should form a
 * 'data' scheme URL which explicitly specifies a charset parameter in the
 * mediatype portion of the URL and call {@link #loadUrl(String)} instead.
 * Note that the charset obtained from the mediatype portion of a data URL
 * always overrides that specified in the HTML or XML document itself.
 *
 * @param data a String of data in the given encoding
 * @param mimeType the MIME type of the data, e.g. 'text/html'
 * @param encoding the encoding of the data
 */
ECode WebView::LoadData(
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding)
{
    CheckThread();
    return mProvider->LoadData(data, mimeType, encoding);
}

/**
 * Loads the given data into this WebView, using baseUrl as the base URL for
 * the content. The base URL is used both to resolve relative URLs and when
 * applying JavaScript's same origin policy. The historyUrl is used for the
 * history entry.
 * <p>
 * Note that content specified in this way can access local device files
 * (via 'file' scheme URLs) only if baseUrl specifies a scheme other than
 * 'http', 'https', 'ftp', 'ftps', 'about' or 'javascript'.
 * <p>
 * If the base URL uses the data scheme, this method is equivalent to
 * calling {@link #loadData(String,String,String) loadData()} and the
 * historyUrl is ignored.
 *
 * @param baseUrl the URL to use as the page's base URL. If null defaults to
 *                'about:blank'.
 * @param data a String of data in the given encoding
 * @param mimeType the MIMEType of the data, e.g. 'text/html'. If null,
 *                 defaults to 'text/html'.
 * @param encoding the encoding of the data
 * @param historyUrl the URL to use as the history entry. If null defaults
 *                   to 'about:blank'.
 */
ECode WebView::LoadDataWithBaseURL(
    /* [in] */ const String& baseUrl,
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding,
    /* [in] */ const String& historyUrl)
{
    CheckThread();
    return mProvider->LoadDataWithBaseURL(baseUrl, data, mimeType, encoding, historyUrl);
}

ECode WebView::EvaluateJavascript(
    /* [in] */ const String& script,
    /* [in] */ IValueCallback* resultCallback)
{
    assert(0);
    // TODO
    return E_NOT_IMPLEMENTED;
}

/**
 * Saves the current view as a web archive.
 *
 * @param filename the filename where the archive should be placed
 */
ECode WebView::SaveWebArchive(
    /* [in] */ const String& filename)
{
    CheckThread();
    return mProvider->SaveWebArchive(filename);
}

/**
 * Saves the current view as a web archive.
 *
 * @param basename the filename where the archive should be placed
 * @param autoname if false, takes basename to be a file. If true, basename
 *                 is assumed to be a directory in which a filename will be
 *                 chosen according to the URL of the current page.
 * @param callback called after the web archive has been saved. The
 *                 parameter for onReceiveValue will either be the filename
 *                 under which the file was saved, or null if saving the
 *                 file failed.
 */
ECode WebView::SaveWebArchive(
    /* [in] */ const String& basename,
    /* [in] */ Boolean autoname,
    /* [in] */ IValueCallback* callback)
{
    CheckThread();
    return mProvider->SaveWebArchive(basename, autoname, callback);
}

/**
 * Stops the current load.
 */
ECode WebView::StopLoading()
{
    CheckThread();
    return mProvider->StopLoading();
}

/**
 * Reloads the current URL.
 */
ECode WebView::Reload()
{
    CheckThread();
    return mProvider->Reload();
}

/**
 * Gets whether this WebView has a back history item.
 *
 * @return true iff this WebView has a back history item
 */
ECode WebView::CanGoBack(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    CheckThread();
    return mProvider->CanGoBack(result);
}

/**
 * Goes back in the history of this WebView.
 */
ECode WebView::GoBack()
{
    CheckThread();
    return mProvider->GoBack();
}

/**
 * Gets whether this WebView has a forward history item.
 *
 * @return true iff this WebView has a forward history item
 */
ECode WebView::CanGoForward(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    CheckThread();
    return mProvider->CanGoForward(result);
}

/**
 * Goes forward in the history of this WebView.
 */
ECode WebView::GoForward()
{
    CheckThread();
    return mProvider->GoForward();
}

/**
 * Gets whether the page can go back or forward the given
 * number of steps.
 *
 * @param steps the negative or positive number of steps to move the
 *              history
 */
ECode WebView::CanGoBackOrForward(
    /* [in] */ Int32 steps,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    CheckThread();
    return mProvider->CanGoBackOrForward(steps, result);
}

/**
 * Goes to the history item that is the number of steps away from
 * the current item. Steps is negative if backward and positive
 * if forward.
 *
 * @param steps the number of steps to take back or forward in the back
 *              forward list
 */
ECode WebView::GoBackOrForward(
    /* [in] */ Int32 steps)
{
    CheckThread();
    return mProvider->GoBackOrForward(steps);
}

/**
 * Gets whether private browsing is enabled in this WebView.
 */
ECode WebView::IsPrivateBrowsingEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    CheckThread();
    return mProvider->IsPrivateBrowsingEnabled(result);
}

/**
 * Scrolls the contents of this WebView up by half the view size.
 *
 * @param top true to jump to the top of the page
 * @return true if the page was scrolled
 */
ECode WebView::PageUp(
    /* [in] */ Boolean top,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    CheckThread();
    return mProvider->PageUp(top, result);
}

/**
 * Scrolls the contents of this WebView down by half the page size.
 *
 * @param bottom true to jump to bottom of page
 * @return true if the page was scrolled
 */
ECode WebView::PageDown(
    /* [in] */ Boolean bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    CheckThread();
    return mProvider->PageDown(bottom, result);
}

/**
 * Clears this WebView so that onDraw() will draw nothing but white background,
 * and onMeasure() will return 0 if MeasureSpec is not MeasureSpec.EXACTLY.
 */
ECode WebView::ClearView()
{
    CheckThread();
    return mProvider->ClearView();
}

/**
 * Gets a new picture that captures the current contents of this WebView.
 * The picture is of the entire document being displayed, and is not
 * limited to the area currently displayed by this WebView. Also, the
 * picture is a static copy and is unaffected by later changes to the
 * content being displayed.
 * <p>
 * Note that due to internal changes, for API levels between
 * {@link android.os.Build.VERSION_CODES#HONEYCOMB} and
 * {@link android.os.Build.VERSION_CODES#ICE_CREAM_SANDWICH} inclusive, the
 * picture does not include fixed position elements or scrollable divs.
 *
 * @return a picture that captures the current contents of this WebView
 */
ECode WebView::CapturePicture(
    /* [out] */ IPicture** picture)
{
    VALIDATE_NOT_NULL(picture);
    CheckThread();
    return mProvider->CapturePicture(picture);
}

/**
 * Gets the current scale of this WebView.
 *
 * @return the current scale
 *
 * @deprecated This method is prone to inaccuracy due to race conditions
 * between the web rendering and UI threads; prefer
 * {@link WebViewClient#onScaleChanged}.
 */
ECode WebView::GetScale(
    /* [out] */ Float* scale)
{
    VALIDATE_NOT_NULL(scale);
    CheckThread();
    return mProvider->GetScale(scale);
}

/**
 * Sets the initial scale for this WebView. 0 means default. If
 * {@link WebSettings#getUseWideViewPort()} is true, it zooms out all the
 * way. Otherwise it starts with 100%. If initial scale is greater than 0,
 * WebView starts with this value as initial scale.
 * Please note that unlike the scale properties in the viewport meta tag,
 * this method doesn't take the screen density into account.
 *
 * @param scaleInPercent the initial scale in percent
 */
ECode WebView::SetInitialScale(
    /* [in] */ Int32 scaleInPercent)
{
    CheckThread();
    return mProvider->SetInitialScale(scaleInPercent);
}

/**
 * Invokes the graphical zoom picker widget for this WebView. This will
 * result in the zoom widget appearing on the screen to control the zoom
 * level of this WebView.
 */
ECode WebView::InvokeZoomPicker()
{
    CheckThread();
    return mProvider->InvokeZoomPicker();
}

/**
 * Gets a HitTestResult based on the current cursor node. If a HTML::a
 * tag is found and the anchor has a non-JavaScript URL, the HitTestResult
 * type is set to SRC_ANCHOR_TYPE and the URL is set in the "extra" field.
 * If the anchor does not have a URL or if it is a JavaScript URL, the type
 * will be UNKNOWN_TYPE and the URL has to be retrieved through
 * {@link #requestFocusNodeHref} asynchronously. If a HTML::img tag is
 * found, the HitTestResult type is set to IMAGE_TYPE and the URL is set in
 * the "extra" field. A type of
 * SRC_IMAGE_ANCHOR_TYPE indicates an anchor with a URL that has an image as
 * a child node. If a phone number is found, the HitTestResult type is set
 * to PHONE_TYPE and the phone number is set in the "extra" field of
 * HitTestResult. If a map address is found, the HitTestResult type is set
 * to GEO_TYPE and the address is set in the "extra" field of HitTestResult.
 * If an email address is found, the HitTestResult type is set to EMAIL_TYPE
 * and the email is set in the "extra" field of HitTestResult. Otherwise,
 * HitTestResult type is set to UNKNOWN_TYPE.
 */
ECode WebView::GetHitTestResult(
    /* [out] */ IWebViewHitTestResult** result)
{
    VALIDATE_NOT_NULL(result);
    CheckThread();
    return mProvider->GetHitTestResult(result);
}

/**
 * Requests the anchor or image element URL at the last tapped point.
 * If hrefMsg is null, this method returns immediately and does not
 * dispatch hrefMsg to its target. If the tapped point hits an image,
 * an anchor, or an image in an anchor, the message associates
 * strings in named keys in its data. The value paired with the key
 * may be an empty string.
 *
 * @param hrefMsg the message to be dispatched with the result of the
 *                request. The message data contains three keys. "url"
 *                returns the anchor's href attribute. "title" returns the
 *                anchor's text. "src" returns the image's src attribute.
 */
ECode WebView::RequestFocusNodeHref(
    /* [in] */ IMessage* hrefMsg)
{
    CheckThread();
    return mProvider->RequestFocusNodeHref(hrefMsg);
}

/**
 * Requests the URL of the image last touched by the user. msg will be sent
 * to its target with a String representing the URL as its object.
 *
 * @param msg the message to be dispatched with the result of the request
 *            as the data member with "url" as key. The result can be null.
 */
ECode WebView::RequestImageRef(
    /* [in] */ IMessage* msg)
{
    CheckThread();
    return mProvider->RequestImageRef(msg);
}

/**
 * Gets the URL for the current page. This is not always the same as the URL
 * passed to WebViewClient.onPageStarted because although the load for
 * that URL has begun, the current page may not have changed.
 *
 * @return the URL for the current page
 */
ECode WebView::GetUrl(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    CheckThread();
    return mProvider->GetUrl(url);
}

/**
 * Gets the original URL for the current page. This is not always the same
 * as the URL passed to WebViewClient.onPageStarted because although the
 * load for that URL has begun, the current page may not have changed.
 * Also, there may have been redirects resulting in a different URL to that
 * originally requested.
 *
 * @return the URL that was originally requested for the current page
 */
ECode WebView::GetOriginalUrl(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    CheckThread();
    return mProvider->GetOriginalUrl(url);
}

/**
 * Gets the title for the current page. This is the title of the current page
 * until WebViewClient.onReceivedTitle is called.
 *
 * @return the title for the current page
 */
ECode WebView::GetTitle(
    /* [out] */ String* title)
{
    VALIDATE_NOT_NULL(title);
    CheckThread();
    return mProvider->GetTitle(title);
}

/**
 * Gets the favicon for the current page. This is the favicon of the current
 * page until WebViewClient.onReceivedIcon is called.
 *
 * @return the favicon for the current page
 */
ECode WebView::GetFavicon(
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    CheckThread();
    return mProvider->GetFavicon(bitmap);
}

/**
 * Gets the touch icon URL for the apple-touch-icon <link> element, or
 * a URL on this site's server pointing to the standard location of a
 * touch icon.
 *
 * @hide
 */
ECode WebView::GetTouchIconUrl(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    return mProvider->GetTouchIconUrl(url);
}

/**
 * Gets the progress for the current page.
 *
 * @return the progress for the current page between 0 and 100
 */
ECode WebView::GetProgress(
    /* [out] */ Int32* progress)
{
    VALIDATE_NOT_NULL(progress);
    CheckThread();
    return mProvider->GetProgress(progress);
}

/**
 * Gets the height of the HTML content.
 *
 * @return the height of the HTML content
 */
ECode WebView::GetContentHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    CheckThread();
    return mProvider->GetContentHeight(height);
}

/**
 * Gets the width of the HTML content.
 *
 * @return the width of the HTML content
 * @hide
 */
ECode WebView::GetContentWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    return mProvider->GetContentWidth(width);
}

/**
 * Pauses all layout, parsing, and JavaScript timers for all WebViews. This
 * is a global requests, not restricted to just this WebView. This can be
 * useful if the application has been paused.
 */
ECode WebView::PauseTimers()
{
    CheckThread();
    return mProvider->PauseTimers();
}

/**
 * Resumes all layout, parsing, and JavaScript timers for all WebViews.
 * This will resume dispatching all timers.
 */
ECode WebView::ResumeTimers()
{
    CheckThread();
    return mProvider->ResumeTimers();
}

/**
 * Pauses any extra processing associated with this WebView and its
 * associated DOM, plugins, JavaScript etc. For example, if this WebView is
 * taken offscreen, this could be called to reduce unnecessary CPU or
 * network traffic. When this WebView is again "active", call onResume().
 * Note that this differs from pauseTimers(), which affects all WebViews.
 */
ECode WebView::OnPause()
{
    CheckThread();
    return mProvider->OnPause();
}

/**
 * Resumes a WebView after a previous call to onPause().
 */
ECode WebView::OnResume()
{
    CheckThread();
    return mProvider->OnResume();
}

/**
 * Gets whether this WebView is paused, meaning onPause() was called.
 * Calling onResume() sets the paused state back to false.
 *
 * @hide
 */
ECode WebView::IsPaused(
    /* [out] */ Boolean* isPaused)
{
    VALIDATE_NOT_NULL(isPaused);
    return mProvider->IsPaused(isPaused);
}

/**
 * Informs this WebView that memory is low so that it can free any available
 * memory.
 */
ECode WebView::FreeMemory()
{
    CheckThread();
    return mProvider->FreeMemory();
}

/**
 * Clears the resource cache. Note that the cache is per-application, so
 * this will clear the cache for all WebViews used.
 *
 * @param includeDiskFiles if false, only the RAM cache is cleared
 */
ECode WebView::ClearCache(
    /* [in] */ Boolean includeDiskFiles)
{
    CheckThread();
    return mProvider->ClearCache(includeDiskFiles);
}

/**
 * Removes the autocomplete popup from the currently focused form field, if
 * present. Note this only affects the display of the autocomplete popup,
 * it does not remove any saved form data from this WebView's store. To do
 * that, use {@link WebViewDatabase#clearFormData}.
 */
ECode WebView::ClearFormData()
{
    CheckThread();
    return mProvider->ClearFormData();
}

/**
 * Tells this WebView to clear its internal back/forward list.
 */
ECode WebView::ClearHistory()
{
    CheckThread();
    return mProvider->ClearHistory();
}

/**
 * Clears the SSL preferences table stored in response to proceeding with
 * SSL certificate errors.
 */
ECode WebView::ClearSslPreferences()
{
    CheckThread();
    return mProvider->ClearSslPreferences();
}

/**
 * Gets the WebBackForwardList for this WebView. This contains the
 * back/forward list for use in querying each item in the history stack.
 * This is a copy of the private WebBackForwardList so it contains only a
 * snapshot of the current state. Multiple calls to this method may return
 * different objects. The object returned from this method will not be
 * updated to reflect any new state.
 */
ECode WebView::CopyBackForwardList(
    /* [out] */ IWebBackForwardList** wfl)
{
    VALIDATE_NOT_NULL(wfl);
    CheckThread();
    return mProvider->CopyBackForwardList(wfl);
}

/**
 * Registers the listener to be notified as find-on-page operations
 * progress. This will replace the current listener.
 *
 * @param listener an implementation of {@link FindListener}
 */
ECode WebView::SetFindListener(
    /* [in] */ IWebViewFindListener* listener)
{
    CheckThread();
    return mProvider->SetFindListener(listener);
}

/**
 * Highlights and scrolls to the next match found by
 * {@link #findAllAsync}, wrapping around page boundaries as necessary.
 * Notifies any registered {@link FindListener}. If {@link #findAllAsync(String)}
 * has not been called yet, or if {@link #clearMatches} has been called since the
 * last find operation, this function does nothing.
 *
 * @param forward the direction to search
 * @see #setFindListener
 */
ECode WebView::FindNext(
    /* [in] */ Boolean forward)
{
    CheckThread();
    return mProvider->FindNext(forward);
}

/**
 * Finds all instances of find on the page and highlights them.
 * Notifies any registered {@link FindListener}.
 *
 * @param find the string to find
 * @return the number of occurances of the String "find" that were found
 * @deprecated {@link #findAllAsync} is preferred.
 * @see #setFindListener
 */
ECode WebView::FindAll(
    /* [in] */ const String& find,
    /* [out] */ Int32* all)
{
    VALIDATE_NOT_NULL(all);
    CheckThread();
//    StrictMode.noteSlowCall("findAll blocks UI: prefer findAllAsync");
    return mProvider->FindAll(find, all);
}

/**
 * Finds all instances of find on the page and highlights them,
 * asynchronously. Notifies any registered {@link FindListener}.
 * Successive calls to this will cancel any pending searches.
 *
 * @param find the string to find.
 * @see #setFindListener
 */
ECode WebView::FindAllAsync(
    /* [in] */ const String& find)
{
    CheckThread();
    return mProvider->FindAllAsync(find);
}

/**
 * Starts an ActionMode for finding text in this WebView.  Only works if this
 * WebView is attached to the view system.
 *
 * @param text if non-null, will be the initial text to search for.
 *             Otherwise, the last String searched for in this WebView will
 *             be used to start.
 * @param showIme if true, show the IME, assuming the user will begin typing.
 *                If false and text is non-null, perform a find all.
 * @return true if the find dialog is shown, false otherwise
 */
ECode WebView::ShowFindDialog(
    /* [in] */ const String& text,
    /* [in] */ Boolean showIme,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    CheckThread();
    return mProvider->ShowFindDialog(text, showIme, result);
}

/**
 * Gets the first substring consisting of the address of a physical
 * location. Currently, only addresses in the United States are detected,
 * and consist of:
 * <ul>
 *   <li>a house number</li>
 *   <li>a street name</li>
 *   <li>a street type (Road, Circle, etc), either spelled out or
 *       abbreviated</li>
 *   <li>a city name</li>
 *   <li>a state or territory, either spelled out or two-letter abbr</li>
 *   <li>an optional 5 digit or 9 digit zip code</li>
 * </ul>
 * All names must be correctly capitalized, and the zip code, if present,
 * must be valid for the state. The street type must be a standard USPS
 * spelling or abbreviation. The state or territory must also be spelled
 * or abbreviated using USPS standards. The house number may not exceed
 * five digits.
 *
 * @param addr the string to search for addresses
 * @return the address, or if no address is found, null
 */
String WebView::FindAddress(
    /* [in] */ const String& addr)
{
    AutoPtr<IWebViewFactoryProvider::IStatics> starts;
    GetFactory()->GetStatics((IWebViewFactoryProvider::IStatics**)&starts);
    String str;
    starts->FindAddress(addr, &str);
    return str;
}

/**
 * Clears the highlighting surrounding text matches created by
 * {@link #findAllAsync}.
 */
ECode WebView::ClearMatches()
{
    CheckThread();
    return mProvider->ClearMatches();
}

/**
 * Queries the document to see if it contains any image references. The
 * message object will be dispatched with arg1 being set to 1 if images
 * were found and 0 if the document does not reference any images.
 *
 * @param response the message that will be dispatched with the result
 */
ECode WebView::DocumentHasImages(
   /* [in] */ IMessage* response)
{
    CheckThread();
    return mProvider->DocumentHasImages(response);
}

/**
 * Sets the WebViewClient that will receive various notifications and
 * requests. This will replace the current handler.
 *
 * @param client an implementation of WebViewClient
 */
ECode WebView::SetWebViewClient(
    /* [in] */ IWebViewClient* client)
{
    CheckThread();
    return mProvider->SetWebViewClient(client);
}

/**
 * Registers the interface to be used when content can not be handled by
 * the rendering engine, and should be downloaded instead. This will replace
 * the current handler.
 *
 * @param listener an implementation of DownloadListener
 */
ECode WebView::SetDownloadListener(
    /* [in] */ IDownloadListener* listener)
{
    CheckThread();
    return mProvider->SetDownloadListener(listener);
}

/**
 * Sets the chrome handler. This is an implementation of WebChromeClient for
 * use in handling JavaScript dialogs, favicons, titles, and the progress.
 * This will replace the current handler.
 *
 * @param client an implementation of WebChromeClient
 */
ECode WebView::SetWebChromeClient(
    /* [in] */ IWebChromeClient* client)
{
    CheckThread();
    return mProvider->SetWebChromeClient(client);
}

/**
 * Sets the Picture listener. This is an interface used to receive
 * notifications of a new Picture.
 *
 * @param listener an implementation of WebView.PictureListener
 * @deprecated This method is now obsolete.
 */
ECode WebView::SetPictureListener(
    /* [in] */ IWebViewPictureListener* listener)
{
    CheckThread();
    return mProvider->SetPictureListener(listener);
}

/**
 * Injects the supplied Java object into this WebView. The object is
 * injected into the JavaScript context of the main frame, using the
 * supplied name. This allows the Java object's methods to be
 * accessed from JavaScript. For applications targeted to API
 * level {@link android.os.Build.VERSION_CODES#JELLY_BEAN_MR1}
 * and above, only public methods that are annotated with
 * {@link android.webkit.JavascriptInterface} can be accessed from JavaScript.
 * For applications targeted to API level {@link android.os.Build.VERSION_CODES#JELLY_BEAN} or below,
 * all public methods (including the inherited ones) can be accessed, see the
 * important security note below for implications.
 * <p> Note that injected objects will not
 * appear in JavaScript until the page is next (re)loaded. For example:
 * <pre>
 * class JsObject {
 *    {@literal @}JavascriptInterface
 *    String WebView::toString() { return "injectedObject"; }
 * }
 * WebView.addJavascriptInterface(new JsObject(), "injectedObject");
 * WebView.loadData("<!DOCTYPE html><title></title>", "text/html", null);
 * WebView.loadUrl("javascript:alert(injectedObject.toString())");</pre>
 * <p>
 * <strong>IMPORTANT:</strong>
 * <ul>
 * <li> This method can be used to allow JavaScript to control the host
 * application. This is a powerful feature, but also presents a security
 * risk for applications targeted to API level
 * {@link android.os.Build.VERSION_CODES#JELLY_BEAN} or below, because
 * JavaScript could use reflection to access an
 * injected object's public fields. Use of this method in a WebView
 * containing untrusted content could allow an attacker to manipulate the
 * host application in unintended ways, executing Java code with the
 * permissions of the host application. Use extreme care when using this
 * method in a WebView which could contain untrusted content.</li>
 * <li> JavaScript interacts with Java object on a private, background
 * thread of this WebView. Care is therefore required to maintain thread
 * safety.</li>
 * <li> The Java object's fields are not accessible.</li>
 * </ul>
 *
 * @param object the Java object to inject into this WebView's JavaScript
 *               context. Null values are ignored.
 * @param name the name used to expose the object in JavaScript
 */
ECode WebView::AddJavascriptInterface(
    /* [in] */ IInterface* object,
    /* [in] */ const String& name)
{
    CheckThread();
    return mProvider->AddJavascriptInterface(object, name);
}

/**
 * Removes a previously injected Java object from this WebView. Note that
 * the removal will not be reflected in JavaScript until the page is next
 * (re)loaded. See {@link #addJavascriptInterface}.
 *
 * @param name the name used to expose the object in JavaScript
 */
ECode WebView::RemoveJavascriptInterface(
    /* [in] */ const String& name)
{
    CheckThread();
    return mProvider->RemoveJavascriptInterface(name);
}

/**
 * Gets the WebSettings object used to control the settings for this
 * WebView.
 *
 * @return a WebSettings object that can be used to control this WebView's
 *         settings
 */
ECode WebView::GetSettings(
    /* [out] */ IWebSettings** webSettings)
{
    VALIDATE_NOT_NULL(webSettings);
    CheckThread();
    return mProvider->GetSettings(webSettings);
}

/**
 * Gets the list of currently loaded plugins.
 *
 * @return the list of currently loaded plugins
 * @deprecated This was used for Gears, which has been deprecated.
 * @hide
 */
AutoPtr<IPluginList> WebView::GetPluginList()
{
    AutoLock lock(sLock);
    CheckThread();
    AutoPtr<IPluginList> pl;
    CPluginList::New((IPluginList**)&pl);
    return pl;
}

/**
 * @deprecated This was used for Gears, which has been deprecated.
 * @hide
 */
ECode WebView::RefreshPlugins(
    /* [in] */ Boolean reloadOpenPages)
{
    CheckThread();
    return NOERROR;
}

/**
 * Puts this WebView into text selection mode. Do not rely on this
 * functionality; it will be deprecated in the future.
 *
 * @deprecated This method is now obsolete.
 * @hide Since API level {@link android.os.Build.VERSION_CODES#JELLY_BEAN_MR1}
 */
ECode WebView::EmulateShiftHeld()
{
    CheckThread();
    return NOERROR;
}

/**
 * @deprecated WebView no longer needs to implement
 * ViewGroup.OnHierarchyChangeListener.  This method does nothing now.
 */
// Cannot add @hide as this can always be accessed via the interface.
ECode WebView::OnChildViewAdded(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    return NOERROR;
}

/**
 * @deprecated WebView no longer needs to implement
 * ViewGroup.OnHierarchyChangeListener.  This method does nothing now.
 */
// Cannot add @hide as this can always be accessed via the interface.
ECode WebView::OnChildViewRemoved(
    /* [in] */ IView* p,
    /* [in] */ IView* child)
{
    return NOERROR;
}

/**
 * @deprecated WebView should not have implemented
 * ViewTreeObserver.OnGlobalFocusChangeListener. This method does nothing now.
 */
// Cannot add @hide as this can always be accessed via the interface.
ECode WebView::OnGlobalFocusChanged(
    /* [in] */ IView* oldFocus,
    /* [in] */ IView* newFocus)
{
    return NOERROR;
}

/**
 * @deprecated Only the default case, true, will be supported in a future version.
 */
ECode WebView::SetMapTrackballToArrowKeys(
    /* [in] */ Boolean setMap)
{
    CheckThread();
    return mProvider->SetMapTrackballToArrowKeys(setMap);
}

ECode WebView::FlingScroll(
    /* [in] */ Int32 vx,
    /* [in] */ Int32 vy)
{
    CheckThread();
    return mProvider->FlingScroll(vx, vy);
}

/**
 * Gets the zoom controls for this WebView, as a separate View. The caller
 * is responsible for inserting this View into the layout hierarchy.
 * <p/>
 * API level {@link android.os.Build.VERSION_CODES#CUPCAKE} introduced
 * built-in zoom mechanisms for the WebView, as opposed to these separate
 * zoom controls. The built-in mechanisms are preferred and can be enabled
 * using {@link WebSettings#setBuiltInZoomControls}.
 *
 * @deprecated the built-in zoom mechanisms are preferred
 * @hide Since API level {@link android.os.Build.VERSION_CODES#JELLY_BEAN}
 */
ECode WebView::GetZoomControls(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    CheckThread();
    return mProvider->GetZoomControls(view);
}

/**
 * Gets whether this WebView can be zoomed in.
 *
 * @return true if this WebView can be zoomed in
 *
 * @deprecated This method is prone to inaccuracy due to race conditions
 * between the web rendering and UI threads; prefer
 * {@link WebViewClient#onScaleChanged}.
 */
ECode WebView::CanZoomIn(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    CheckThread();
    return mProvider->CanZoomIn(result);
}

/**
 * Gets whether this WebView can be zoomed out.
 *
 * @return true if this WebView can be zoomed out
 *
 * @deprecated This method is prone to inaccuracy due to race conditions
 * between the web rendering and UI threads; prefer
 * {@link WebViewClient#onScaleChanged}.
 */
ECode WebView::CanZoomOut(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    CheckThread();
    return mProvider->CanZoomOut(result);
}

ECode WebView::ZoomBy(
        /* [in] */ Float zoomFactor)
{
    assert(0);
    // TODO
    return E_NOT_IMPLEMENTED;
}

/**
 * Performs zoom in in this WebView.
 *
 * @return true if zoom in succeeds, false if no zoom changes
 */
ECode WebView::ZoomIn(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    CheckThread();
    return mProvider->ZoomIn(result);
}

/**
 * Performs zoom out in this WebView.
 *
 * @return true if zoom out succeeds, false if no zoom changes
 */
ECode WebView::ZoomOut(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    CheckThread();
    return mProvider->ZoomOut(result);
}

/**
 * @deprecated This method is now obsolete.
 * @hide Since API level {@link android.os.Build.VERSION_CODES#JELLY_BEAN_MR1}
 */
ECode WebView::DebugDump()
{
    CheckThread();
    return NOERROR;
}

/**
 * See {@link ViewDebug.HierarchyHandler#dumpViewHierarchyWithProperties(BufferedWriter, int)}
 * @hide
 */
ECode WebView::DumpViewHierarchyWithProperties(
    /* [in] */ IBufferedWriter* out,
    /* [in] */ Int32 level)
{
    return mProvider->DumpViewHierarchyWithProperties(out, level);
}

/**
 * See {@link ViewDebug.HierarchyHandler#findHierarchyView(String, int)}
 * @hide
 */
AutoPtr<IView> WebView::FindHierarchyView(
    /* [in] */ const String& className,
    /* [in] */ Int32 hashCode)
{
    AutoPtr<IView> view;
    mProvider->FindHierarchyView(className, hashCode, (IView**)&view);
    return view;
}

//-------------------------------------------------------------------------
// Interface for WebView providers
//-------------------------------------------------------------------------

/**
 * Gets the WebViewProvider. Used by providers to obtain the underlying
 * implementation, e.g. when the appliction responds to
 * WebViewClient.onCreateWindow() request.
 *
 * @hide WebViewProvider is not public API.
 */
ECode WebView::GetWebViewProvider(
    /* [out] */ IWebViewProvider** provider)
{
    VALIDATE_NOT_NULL(provider);
    *provider = mProvider;
    return NOERROR;
}

//-------------------------------------------------------------------------
// Private internal stuff
//-------------------------------------------------------------------------

void WebView::EnsureProviderCreated()
{
    CheckThread();
    if (mProvider == NULL) {
        // As this can get called during the base class constructor chain, pass the minimum
        // number of dependencies here; the rest are deferred to init().
        AutoPtr<IWebViewPrivateAccess> pa = new PrivateAccess(this);
        IWebView* wv = (IWebView*)this->Probe(EIID_IWebView);
        assert(wv != NULL);
        GetFactory()->CreateWebView(wv, pa, (IWebViewProvider**)&mProvider);
    }
}

AutoPtr<IWebViewFactoryProvider> WebView::GetFactory()
{
    AutoLock lock(sLock);

    // For now the main purpose of this function (and the factory abstration) is to keep
    // us honest and minimize usage of WebViewClassic internals when binding the proxy.
    CheckThread();
    return WebViewFactory::GetProvider();
}

void WebView::CheckThread()
{
    if (Looper::GetMyLooper() != Looper::GetMainLooper()) {
        assert(0);
//        Throwable throwable = new Throwable(
//                "Warning: A WebView method was called on thread '" +
//                Thread.currentThread().getName() + "'. " +
//                "All WebView methods must be called on the UI thread. " +
//                "Future versions of WebView may not support use on other threads.");
//        Log.w(LOGTAG, Log.getStackTraceString(throwable));
//        StrictMode.onWebViewMethodCalledOnWrongThread(throwable);
    }
}

//-------------------------------------------------------------------------
// Override View methods
//-------------------------------------------------------------------------

// TODO: Add a test that enumerates all methods in ViewDelegte & ScrollDelegate, and ensures
// there's a corresponding override (or better, caller) for each of them in here.

ECode WebView::OnAttachedToWindow()
{
    assert(0);
    // TODO
    // FAIL_RETURN(AbsoluteLayout::OnAttachedToWindow());
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    return viewDelegate->OnAttachedToWindow();
}

ECode WebView::OnDetachedFromWindow()
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->OnDetachedFromWindow();
    assert(0);
    // TODO
    // return AbsoluteLayout::OnDetachedFromWindow();
    return E_NOT_IMPLEMENTED;
}

ECode WebView::SetLayoutParams(
    /* [in] */ IViewGroupLayoutParams* params)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->SetLayoutParams(params);
    return NOERROR;
}

ECode WebView::SetOverScrollMode(
    /* [in] */ Int32 mode)
{
    assert(0);
    // TODO
    // FAIL_RETURN(AbsoluteLayout::SetOverScrollMode(mode));

    // This method may called in the constructor chain, before the WebView provider is
    // created. (Fortunately, this is the only method we override that can get called by
    // any of the base class constructors).
    EnsureProviderCreated();
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->SetOverScrollMode(mode);
    return E_NOT_IMPLEMENTED;
}

ECode WebView::SetScrollBarStyle(
    /* [in] */ Int32 style)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->SetScrollBarStyle(style);
    assert(0);
    // TODO
    // return AbsoluteLayout::SetScrollBarStyle(style);
    return E_NOT_IMPLEMENTED;
}

Int32 WebView::ComputeHorizontalScrollRange()
{
    AutoPtr<IWebViewProviderScrollDelegate> scrollDelegate;
    mProvider->GetScrollDelegate((IWebViewProviderScrollDelegate**)&scrollDelegate);
    Int32 range;
    scrollDelegate->ComputeHorizontalScrollRange(&range);
    return range;
}

Int32 WebView::ComputeHorizontalScrollOffset()
{
    AutoPtr<IWebViewProviderScrollDelegate> scrollDelegate;
    mProvider->GetScrollDelegate((IWebViewProviderScrollDelegate**)&scrollDelegate);
    Int32 offset;
    scrollDelegate->ComputeHorizontalScrollOffset(&offset);
    return offset;
}

Int32 WebView::ComputeVerticalScrollRange()
{
    AutoPtr<IWebViewProviderScrollDelegate> scrollDelegate;
    mProvider->GetScrollDelegate((IWebViewProviderScrollDelegate**)&scrollDelegate);
    Int32 range;
    scrollDelegate->ComputeVerticalScrollRange(&range);
    return range;
}

Int32 WebView::ComputeVerticalScrollOffset()
{
    AutoPtr<IWebViewProviderScrollDelegate> scrollDelegate;
    mProvider->GetScrollDelegate((IWebViewProviderScrollDelegate**)&scrollDelegate);
    Int32 offset;
    scrollDelegate->ComputeVerticalScrollOffset(&offset);
    return offset;
}

Int32 WebView::ComputeVerticalScrollExtent()
{
    AutoPtr<IWebViewProviderScrollDelegate> scrollDelegate;
    mProvider->GetScrollDelegate((IWebViewProviderScrollDelegate**)&scrollDelegate);
    Int32 extent;
    scrollDelegate->ComputeVerticalScrollExtent(&extent);
    return extent;
}

ECode WebView::ComputeScroll()
{
    AutoPtr<IWebViewProviderScrollDelegate> scrollDelegate;
    mProvider->GetScrollDelegate((IWebViewProviderScrollDelegate**)&scrollDelegate);
    scrollDelegate->ComputeScroll();
    return NOERROR;
}

Boolean WebView::OnHoverEvent(
    /* [in] */ IMotionEvent* event)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    Boolean result;
    viewDelegate->OnHoverEvent(event, &result);
    return result;
}

Boolean WebView::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    Boolean result;
    viewDelegate->OnTouchEvent(event, &result);
    return result;
}

Boolean WebView::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    Boolean result;
    viewDelegate->OnGenericMotionEvent(event, &result);
    return result;
}

Boolean WebView::OnTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    Boolean result;
    viewDelegate->OnTrackballEvent(event, &result);
    return result;
}

Boolean WebView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    Boolean result;
    viewDelegate->OnKeyDown(keyCode, event, &result);
    return result;
}

Boolean WebView::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    Boolean result;
    viewDelegate->OnKeyUp(keyCode, event, &result);
    return result;
}

Boolean WebView::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 repeatCount,
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    Boolean result;
    viewDelegate->OnKeyMultiple(keyCode, repeatCount, event, &result);
    return result;
}

Boolean WebView::ShouldDelayChildPressedState()
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    Boolean result;
    viewDelegate->ShouldDelayChildPressedState(&result);
    return result;
}

ECode WebView::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    assert(0);
    // TODO
    // FAIL_RETURN(AbsoluteLayout::OnInitializeAccessibilityNodeInfo(info));
    AutoPtr<ICharSequence> cs;
    CString::New(String("Elastos::Droid::Webkit::CWebView")/*WebView.class.getName()*/, (ICharSequence**)&cs);
    info->SetClassName(cs);
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->OnInitializeAccessibilityNodeInfo(info);
    return E_NOT_IMPLEMENTED;
}

ECode WebView::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    assert(0);
    // TODO
    // FAIL_RETURN(AbsoluteLayout::OnInitializeAccessibilityEvent(event));
    AutoPtr<ICharSequence> cs;
    CString::New(String("Elastos::Droid::Webkit::CWebView")/*WebView.class.getName()*/, (ICharSequence**)&cs);
    IAccessibilityRecord::Probe(event)->SetClassName(cs);
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->OnInitializeAccessibilityEvent(event);
    return E_NOT_IMPLEMENTED;
}

Boolean WebView::PerformAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    Boolean result;
    viewDelegate->PerformAccessibilityAction(action, arguments, &result);
    return result;
}

/** @hide */
void WebView::OnDrawVerticalScrollBar(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IDrawable* scrollBar,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->OnDrawVerticalScrollBar(canvas, scrollBar, l, t, r, b);
}

void WebView::OnOverScrolled(
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Boolean clampedX,
    /* [in] */ Boolean clampedY)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->OnOverScrolled(scrollX, scrollY, clampedX, clampedY);
}

void WebView::OnWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    assert(0);
    // TODO
    // AbsoluteLayout::OnWindowVisibilityChanged(visibility);
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->OnWindowVisibilityChanged(visibility);
}

void WebView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->OnDraw(canvas);
}

Boolean WebView::PerformLongClick()
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    Boolean result = FALSE;
    viewDelegate->PerformLongClick(&result);
    return result;
}

void WebView::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->OnConfigurationChanged(newConfig);
}

AutoPtr<IInputConnection> WebView::OnCreateInputConnection(
    /* [in] */ IEditorInfo* outAttrs)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    AutoPtr<IInputConnection> conn;
    viewDelegate->OnCreateInputConnection(outAttrs, (IInputConnection**)&conn);
    return conn;
}

void WebView::OnVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    assert(0);
    // TODO
    // AbsoluteLayout::OnVisibilityChanged(changedView, visibility);
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->OnVisibilityChanged(changedView, visibility);
}

ECode WebView::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->OnWindowFocusChanged(hasWindowFocus);
    assert(0);
    // TODO
    // AbsoluteLayout::OnWindowFocusChanged(hasWindowFocus);
    return E_NOT_IMPLEMENTED;
}

void WebView::OnFocusChanged(
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->OnFocusChanged(focused, direction, previouslyFocusedRect);
    assert(0);
    // TODO
    // AbsoluteLayout::OnFocusChanged(focused, direction, previouslyFocusedRect);
}

/** @hide */
Boolean WebView::SetFrame(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    Boolean result = FALSE;
    viewDelegate->SetFrame(left, top, right, bottom, &result);
    return result;
}

void WebView::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 ow,
    /* [in] */ Int32 oh)
{
    assert(0);
    // TODO
    // AbsoluteLayout::OnSizeChanged(w, h, ow, oh);
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->OnSizeChanged(w, h, ow, oh);
}

void WebView::OnScrollChanged(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 oldl,
    /* [in] */ Int32 oldt)
{
    assert(0);
    // TODO
    // AbsoluteLayout::OnScrollChanged(l, t, oldl, oldt);
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->OnScrollChanged(l, t, oldl, oldt);
}

Boolean WebView::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    Boolean result;
    viewDelegate->DispatchKeyEvent(event, &result);
    return result;
}

Boolean WebView::RequestFocus(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    Boolean result;
    viewDelegate->RequestFocus(direction, previouslyFocusedRect, &result);
    return result;
}

void WebView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    assert(0);
    // TODO
    // AbsoluteLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    viewDelegate->OnMeasure(widthMeasureSpec, heightMeasureSpec);
}

Boolean WebView::RequestChildRectangleOnScreen(
    /* [in] */ IView* child,
    /* [in] */ IRect* rect,
    /* [in] */ Boolean immediate)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    Boolean result;
    viewDelegate->RequestChildRectangleOnScreen(child, rect, immediate, &result);
    return result;
}

ECode WebView::SetBackgroundColor(
    /* [in] */ Int32 color)
{
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    return viewDelegate->SetBackgroundColor(color);
}

ECode WebView::SetLayerType(
    /* [in] */ Int32 layerType,
    /* [in] */ IPaint* paint)
{
    assert(0);
    // TODO
    // AbsoluteLayout::SetLayerType(layerType, paint);
    AutoPtr<IWebViewProviderViewDelegate> viewDelegate;
    mProvider->GetViewDelegate((IWebViewProviderViewDelegate**)&viewDelegate);
    return viewDelegate->SetLayerType(layerType, paint);
}

ECode WebView::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "WebView";
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
