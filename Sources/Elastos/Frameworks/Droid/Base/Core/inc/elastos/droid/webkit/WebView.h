
#ifndef __ELASTOS_DROID_WEBKIT_WEBVIEW_H__
#define __ELASTOS_DROID_WEBKIT_WEBVIEW_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/webkit/WebViewFactoryProvider.h"
// TODO #include "elastos/droid/widget/AbsoluteLayout.h"

using Elastos::IO::IBufferedWriter;
using Elastos::IO::IFile;
using Elastos::Utility::IMap;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::IPicture;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Net::Http::ISslCertificate;
// TODO using Elastos::Droid::Widget::AbsoluteLayout;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
using Elastos::Droid::View::InputMethod::IEditorInfo;
using Elastos::Droid::View::InputMethod::IInputConnection;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::Utility::IAttributeSet;

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * <p>A View that displays web pages. This class is the basis upon which you
 * can roll your own web browser or simply display some online content within your Activity.
 * It uses the WebKit rendering engine to display
 * web pages and includes methods to navigate forward and backward
 * through a history, zoom in and out, perform text searches and more.</p>
 * <p>To enable the built-in zoom, set
 * {@link #getSettings() WebSettings}.{@link WebSettings#setBuiltInZoomControls(boolean)}
 * (introduced in API level {@link android.os.Build.VERSION_CODES#CUPCAKE}).
 * <p>Note that, in order for your Activity to access the Internet and load web pages
 * in a WebView, you must add the {@code INTERNET} permissions to your
 * Android Manifest file:</p>
 * <pre>&lt;uses-permission android:name="android.permission.INTERNET" /></pre>
 *
 * <p>This must be a child of the <a
 * href="{@docRoot}guide/topics/manifest/manifest-element.html">{@code <manifest>}</a>
 * element.</p>
 *
 * <p>For more information, read
 * <a href="{@docRoot}guide/webapps/webview.html">Building Web Apps in WebView</a>.</p>
 *
 * <h3>Basic usage</h3>
 *
 * <p>By default, a WebView provides no browser-like widgets, does not
 * enable JavaScript and web page errors are ignored. If your goal is only
 * to display some HTML as a part of your UI, this is probably fine;
 * the user won't need to interact with the web page beyond reading
 * it, and the web page won't need to interact with the user. If you
 * actually want a full-blown web browser, then you probably want to
 * invoke the Browser application with a URL Intent rather than show it
 * with a WebView. For example:
 * <pre>
 * Uri uri = Uri.parse("http://www.example.com");
 * Intent intent = new Intent(Intent.ACTION_VIEW, uri);
 * startActivity(intent);
 * </pre>
 * <p>See {@link android.content.Intent} for more information.</p>
 *
 * <p>To provide a WebView in your own Activity, include a {@code <WebView>} in your layout,
 * or set the entire Activity window as a WebView during {@link
 * android.app.Activity#onCreate(Bundle) onCreate()}:</p>
 * <pre class="prettyprint">
 * WebView webview = new WebView(this);
 * setContentView(webview);
 * </pre>
 *
 * <p>Then load the desired web page:</p>
 * <pre>
 * // Simplest usage: note that an exception will NOT be thrown
 * // if there is an error loading this page (see below).
 * webview.loadUrl("http://slashdot.org/");
 *
 * // OR, you can also load from an HTML string:
 * String summary = "&lt;html>&lt;body>You scored &lt;b>192&lt;/b> points.&lt;/body>&lt;/html>";
 * webview.loadData(summary, "text/html", null);
 * // ... although note that there are restrictions on what this HTML can do.
 * // See the JavaDocs for {@link #loadData(String,String,String) loadData()} and {@link
 * #loadDataWithBaseURL(String,String,String,String,String) loadDataWithBaseURL()} for more info.
 * </pre>
 *
 * <p>A WebView has several customization points where you can add your
 * own behavior. These are:</p>
 *
 * <ul>
 *   <li>Creating and setting a {@link android.webkit.WebChromeClient} subclass.
 *       This class is called when something that might impact a
 *       browser UI happens, for instance, progress updates and
 *       JavaScript alerts are sent here (see <a
 * href="{@docRoot}guide/developing/debug-tasks.html#DebuggingWebPages">Debugging Tasks</a>).
 *   </li>
 *   <li>Creating and setting a {@link android.webkit.WebViewClient} subclass.
 *       It will be called when things happen that impact the
 *       rendering of the content, eg, errors or form submissions. You
 *       can also intercept URL loading here (via {@link
 * android.webkit.WebViewClient#shouldOverrideUrlLoading(WebView,String)
 * shouldOverrideUrlLoading()}).</li>
 *   <li>Modifying the {@link android.webkit.WebSettings}, such as
 * enabling JavaScript with {@link android.webkit.WebSettings#setJavaScriptEnabled(boolean)
 * setJavaScriptEnabled()}. </li>
 *   <li>Injecting Java objects into the WebView using the
 *       {@link android.webkit.WebView#addJavascriptInterface} method. This
 *       method allows you to inject Java objects into a page's JavaScript
 *       context, so that they can be accessed by JavaScript in the page.</li>
 * </ul>
 *
 * <p>Here's a more complicated example, showing error handling,
 *    settings, and progress notification:</p>
 *
 * <pre class="prettyprint">
 * // Let's display the progress in the activity title bar, like the
 * // browser app does.
 * getWindow().requestFeature(Window.FEATURE_PROGRESS);
 *
 * webview.getSettings().setJavaScriptEnabled(true);
 *
 * final Activity activity = this;
 * webview.setWebChromeClient(new WebChromeClient() {
 *   CARAPI_(void) onProgressChanged(WebView view, Int32 progress) {
 *     // Activities and WebViews measure progress with different scales.
 *     // The progress meter will automatically disappear when we reach 100%
 *     activity.setProgress(progress * 1000);
 *   }
 * });
 * webview.setWebViewClient(new WebViewClient() {
 *   CARAPI_(void) onReceivedError(WebView view, Int32 errorCode, String description, String failingUrl) {
 *     Toast.makeText(activity, "Oh no! " + description, Toast.LENGTH_SHORT).show();
 *   }
 * });
 *
 * webview.loadUrl("http://slashdot.org/");
 * </pre>
 *
 * <h3>Cookie and window management</h3>
 *
 * <p>For obvious security reasons, your application has its own
 * cache, cookie store etc.&mdash;it does not share the Browser
 * application's data.
 * </p>
 *
 * <p>By default, requests by the HTML to open new windows are
 * ignored. This is true whether they be opened by JavaScript or by
 * the target attribute on a link. You can customize your
 * {@link WebChromeClient} to provide your own behaviour for opening multiple windows,
 * and render them in whatever manner you want.</p>
 *
 * <p>The standard behavior for an Activity is to be destroyed and
 * recreated when the device orientation or any other configuration changes. This will cause
 * the WebView to reload the current page. If you don't want that, you
 * can set your Activity to handle the {@code orientation} and {@code keyboardHidden}
 * changes, and then just leave the WebView alone. It'll automatically
 * re-orient itself as appropriate. Read <a
 * href="{@docRoot}guide/topics/resources/runtime-changes.html">Handling Runtime Changes</a> for
 * more information about how to handle configuration changes during runtime.</p>
 *
 *
 * <h3>Building web pages to support different screen densities</h3>
 *
 * <p>The screen density of a device is based on the screen resolution. A screen with low density
 * has fewer available pixels per inch, where a screen with high density
 * has more &mdash; sometimes significantly more &mdash; pixels per inch. The density of a
 * screen is important because, other things being equal, a UI element (such as a button) whose
 * height and width are defined in terms of screen pixels will appear larger on the lower density
 * screen and smaller on the higher density screen.
 * For simplicity, Android collapses all actual screen densities into three generalized densities:
 * high, medium, and low.</p>
 * <p>By default, WebView scales a web page so that it is drawn at a size that matches the default
 * appearance on a medium density screen. So, it applies 1.5x scaling on a high density screen
 * (because its pixels are smaller) and 0.75x scaling on a low density screen (because its pixels
 * are bigger).
 * Starting with API level {@link android.os.Build.VERSION_CODES#ECLAIR}, WebView supports DOM, CSS,
 * and meta tag features to help you (as a web developer) target screens with different screen
 * densities.</p>
 * <p>Here's a summary of the features you can use to handle different screen densities:</p>
 * <ul>
 * <li>The {@code window.devicePixelRatio} DOM property. The value of this property specifies the
 * default scaling factor used for the current device. For example, if the value of {@code
 * window.devicePixelRatio} is "1.0", then the device is considered a medium density (mdpi) device
 * and default scaling is not applied to the web page; if the value is "1.5", then the device is
 * considered a high density device (hdpi) and the page content is scaled 1.5x; if the
 * value is "0.75", then the device is considered a low density device (ldpi) and the content is
 * scaled 0.75x. However, if you specify the {@code "target-densitydpi"} meta property
 * (discussed below), then you can stop this default scaling behavior.</li>
 * <li>The {@code -webkit-device-pixel-ratio} CSS media query. Use this to specify the screen
 * densities for which this style sheet is to be used. The corresponding value should be either
 * "0.75", "1", or "1.5", to indicate that the styles are for devices with low density, medium
 * density, or high density screens, respectively. For example:
 * <pre>
 * &lt;link rel="stylesheet" media="screen and (-webkit-device-pixel-ratio:1.5)" href="hdpi.css" /&gt;</pre>
 * <p>The {@code hdpi.css} stylesheet is only used for devices with a screen pixel ration of 1.5,
 * which is the high density pixel ratio.</p>
 * </li>
 * <li>The {@code target-densitydpi} property for the {@code viewport} meta tag. You can use
 * this to specify the target density for which the web page is designed, using the following
 * values:
 * <ul>
 * <li>{@code device-dpi} - Use the device's native dpi as the target dpi. Default scaling never
 * occurs.</li>
 * <li>{@code high-dpi} - Use hdpi as the target dpi. Medium and low density screens scale down
 * as appropriate.</li>
 * <li>{@code medium-dpi} - Use mdpi as the target dpi. High density screens scale up and
 * low density screens scale down. This is also the default behavior.</li>
 * <li>{@code low-dpi} - Use ldpi as the target dpi. Medium and high density screens scale up
 * as appropriate.</li>
 * <li><em>{@code <value>}</em> - Specify a dpi value to use as the target dpi (accepted
 * values are 70-400).</li>
 * </ul>
 * <p>Here's an example meta tag to specify the target density:</p>
 * <pre>&lt;meta name="viewport" content="target-densitydpi=device-dpi" /&gt;</pre></li>
 * </ul>
 * <p>If you want to modify your web page for different densities, by using the {@code
 * -webkit-device-pixel-ratio} CSS media query and/or the {@code
 * window.devicePixelRatio} DOM property, then you should set the {@code target-densitydpi} meta
 * property to {@code device-dpi}. This stops Android from performing scaling in your web page and
 * allows you to make the necessary adjustments for each density via CSS and JavaScript.</p>
 *
 * <h3>HTML5 Video support</h3>
 *
 * <p>In order to support inline HTML5 video in your application, you need to have hardware
 * acceleration turned on, and set a {@link android.webkit.WebChromeClient}. For full screen support,
 * implementations of {@link WebChromeClient#onShowCustomView(View, WebChromeClient.CustomViewCallback)}
 * and {@link WebChromeClient#onHideCustomView()} are required,
 * {@link WebChromeClient#getVideoLoadingProgressView()} is optional.
 * </p>
 */
// Implementation notes.
// The WebView is a thin API class that delegates its public API to a backend WebViewProvider
// class instance. WebView extends {@link AbsoluteLayout} for backward compatibility reasons.
// Methods are delegated to the provider implementation: all public API methods introduced in this
// file are fully delegated, whereas public and protected methods from the View base classes are
// only delegated where a specific need exists for them to do so.
//@Widget
class WebView // TODO: public AbsoluteLayout
    : public Object
    , public IWebView
{
    friend class PrivateAccess;
public:
    /**
     *  Transportation object for returning WebView across thread boundaries.
     */
    class WebViewTransport : public Object
    {
    public:
        /**
         * Sets the WebView to the transportation object.
         *
         * @param webview the WebView to transport
         */
        virtual CARAPI SetWebView(
            /* [in] */ IWebView* webview);

        /**
         * Gets the WebView object.
         *
         * @return the transported WebView object
         */
        virtual CARAPI GetWebView(
            /* [out] */ IWebView** webView);

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        AutoPtr<IWebView> mWebview;
        Object mLock;
    };

    /**
     * Callback interface, allows the provider implementation to access non-public methods
     * and fields, and make super-class calls in this WebView instance.
     * @hide Only for use by WebViewProvider implementations
     */
    class PrivateAccess
        : public Object
        , public IWebViewPrivateAccess
    {
    public:
        PrivateAccess(
            /* [in] */ WebView* host)
            : mHost(host)
        {}

        CAR_INTERFACE_DECL()

        // ---- Access to super-class methods ----
        CARAPI Super_getScrollBarStyle(
            /* [out] */ Int32* style);

        CARAPI Super_scrollTo(
            /* [in] */ Int32 scrollX,
            /* [in] */ Int32 scrollY);

        CARAPI Super_computeScroll();

        CARAPI Super_onHoverEvent(
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

        CARAPI Super_performAccessibilityAction(
            /* [in] */ Int32 action,
            /* [in] */ IBundle* arguments,
            /* [out] */ Boolean* result);

        CARAPI Super_performLongClick(
            /* [out] */ Boolean* result);

        CARAPI Super_setFrame(
            /* [in] */ Int32 left,
            /* [in] */ Int32 top,
            /* [in] */ Int32 right,
            /* [in] */ Int32 bottom,
            /* [out] */ Boolean* result);

        CARAPI Super_dispatchKeyEvent(
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

        CARAPI Super_onGenericMotionEvent(
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

        CARAPI Super_requestFocus(
            /* [in] */ Int32 direction,
            /* [in] */ IRect* previouslyFocusedRect,
            /* [out] */ Boolean* result);

        CARAPI Super_setLayoutParams(
            /* [in] */ IViewGroupLayoutParams* params);

        // ---- Access to non-public methods ----
        CARAPI OverScrollBy(
            /* [in] */ Int32 deltaX,
            /* [in] */ Int32 deltaY,
            /* [in] */ Int32 scrollX,
            /* [in] */ Int32 scrollY,
            /* [in] */ Int32 scrollRangeX,
            /* [in] */ Int32 scrollRangeY,
            /* [in] */ Int32 maxOverScrollX,
            /* [in] */ Int32 maxOverScrollY,
            /* [in] */ Boolean isTouchEvent);

        CARAPI AwakenScrollBars(
            /* [in] */ Int32 duration);

        CARAPI AwakenScrollBars(
            /* [in] */ Int32 duration,
            /* [in] */ Boolean invalidate);

        CARAPI GetVerticalScrollFactor(
            /* [out] */ Float* factor);

        CARAPI GetHorizontalScrollFactor(
            /* [out] */ Float* factor);

        CARAPI SetMeasuredDimension(
            /* [in] */ Int32 measuredWidth,
            /* [in] */ Int32 measuredHeight);

        CARAPI OnScrollChanged(
            /* [in] */ Int32 l,
            /* [in] */ Int32 t,
            /* [in] */ Int32 oldl,
            /* [in] */ Int32 oldt);

        CARAPI GetHorizontalScrollbarHeight(
            /* [out] */ Int32* height);

        // ---- Access to (non-public) fields ----
        /** Raw setter for the scroll X value, without invoking onScrollChanged handlers etc. */
        CARAPI SetScrollXRaw(
            /* [in] */ Int32 scrollX);

        /** Raw setter for the scroll Y value, without invoking onScrollChanged handlers etc. */
        CARAPI SetScrollYRaw(
            /* [in] */ Int32 scrollY);

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        WebView* mHost;
    };

public:
    CAR_INTERFACE_DECL()

    /**
     * Constructs a new WebView with a Context object.
     *
     * @param context a Context object used to access application assets
     */
    WebView(
        /* [in] */ IContext* context);

    /**
     * Constructs a new WebView with layout parameters.
     *
     * @param context a Context object used to access application assets
     * @param attrs an AttributeSet passed to our parent
     */
    WebView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    /**
     * Constructs a new WebView with layout parameters and a default style.
     *
     * @param context a Context object used to access application assets
     * @param attrs an AttributeSet passed to our parent
     * @param defStyle the default style resource ID
     */
    WebView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

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
    WebView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle,
        /* [in] */ Boolean privateBrowsing);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle,
        /* [in] */ Boolean privateBrowsing);

    /**
     * Specifies whether the horizontal scrollbar has overlay style.
     *
     * @param overlay true if horizontal scrollbar should have overlay style
     */
    virtual CARAPI SetHorizontalScrollbarOverlay(
        /* [in] */ Boolean overlay);

    /**
     * Specifies whether the vertical scrollbar has overlay style.
     *
     * @param overlay true if vertical scrollbar should have overlay style
     */
    virtual CARAPI SetVerticalScrollbarOverlay(
        /* [in] */ Boolean overlay);

    /**
     * Gets whether horizontal scrollbar has overlay style.
     *
     * @return true if horizontal scrollbar has overlay style
     */
    virtual CARAPI_(Boolean) OverlayHorizontalScrollbar();

    /**
     * Gets whether vertical scrollbar has overlay style.
     *
     * @return true if vertical scrollbar has overlay style
     */
    virtual CARAPI_(Boolean) OverlayVerticalScrollbar();

    /**
     * Gets the visible height (in pixels) of the embedded title bar (if any).
     *
     * @deprecated This method is now obsolete.
     * @hide Since API level {@link android.os.Build.VERSION_CODES#JELLY_BEAN_MR1}
     */
    virtual CARAPI_(Int32) GetVisibleTitleHeight();

    /**
     * Gets the SSL certificate for the main top-level page or null if there is
     * no certificate (the site is not secure).
     *
     * @return the SSL certificate for the main top-level page
     */
    virtual CARAPI_(AutoPtr<ISslCertificate>) GetCertificate();

    /**
     * Sets the SSL certificate for the main top-level page.
     *
     * @deprecated Calling this function has no useful effect, and will be
     * ignored in future releases.
     */
    virtual CARAPI SetCertificate(
        /* [in] */ ISslCertificate* certificate);

    //-------------------------------------------------------------------------
    // Methods called by activity
    //-------------------------------------------------------------------------

    /**
     * Sets a username and password pair for the specified host. This data is
     * used by the Webview to autocomplete username and password fields in web
     * forms. Note that this is unrelated to the credentials used for HTTP
     * authentication.
     *
     * @param host the host that required the credentials
     * @param username the username for the given host
     * @param password the password for the given host
     * @see WebViewDatabase#clearUsernamePassword
     * @see WebViewDatabase#hasUsernamePassword
     */
    virtual CARAPI SavePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

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
    virtual CARAPI SetHttpAuthUsernamePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& realm,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

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
    virtual CARAPI_(AutoPtr< ArrayOf<String> >) GetHttpAuthUsernamePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& realm);

    /**
     * Destroys the internal state of this WebView. This method should be called
     * after this WebView has been removed from the view system. No other
     * methods may be called on this WebView after destroy.
     */
    virtual CARAPI Destroy();

    /**
     * Enables platform notifications of data state and proxy changes.
     * Notifications are enabled by default.
     *
     * @deprecated This method is now obsolete.
     * @hide Since API level {@link android.os.Build.VERSION_CODES#JELLY_BEAN_MR1}
     */
    static CARAPI_(void) EnablePlatformNotifications();

    /**
     * Disables platform notifications of data state and proxy changes.
     * Notifications are enabled by default.
     *
     * @deprecated This method is now obsolete.
     * @hide Since API level {@link android.os.Build.VERSION_CODES#JELLY_BEAN_MR1}
     */
    static CARAPI_(void) DisablePlatformNotifications();

    /**
     * Informs WebView of the network state. This is used to set
     * the JavaScript property window.navigator.isOnline and
     * generates the online/offline event as specified in HTML5, sec. 5.7.7
     *
     * @param networkUp a boolean indicating if network is available
     */
    virtual CARAPI SetNetworkAvailable(
        /* [in] */ Boolean networkUp);

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
    virtual CARAPI_(AutoPtr<IWebBackForwardList>) SaveState(
        /* [in] */ IBundle* outState);

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
    virtual CARAPI_(Boolean) SavePicture(
        /* [in] */ IBundle* b,
        /* [in] */ IFile* dest);

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
    virtual CARAPI_(Boolean) RestorePicture(
        /* [in] */ IBundle* b,
        /* [in] */ IFile* src);

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
    virtual CARAPI_(AutoPtr<IWebBackForwardList>) RestoreState(
        /* [in] */ IBundle* inState);

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
    virtual CARAPI LoadUrl(
        /* [in] */ const String& url,
        /* [in] */ IMap* additionalHttpHeaders);

    /**
     * Loads the given URL.
     *
     * @param url the URL of the resource to load
     */
    virtual CARAPI LoadUrl(
        /* [in] */ const String& url);

    /**
     * Loads the URL with postData using "POST" method into this WebView. If url
     * is not a network URL, it will be loaded with {link
     * {@link #loadUrl(String)} instead.
     *
     * @param url the URL of the resource to load
     * @param postData the data will be passed to "POST" request
     */
    virtual CARAPI PostUrl(
        /* [in] */ const String& url,
        /* [in] */ ArrayOf<Byte>* postData);

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
    virtual CARAPI LoadData(
        /* [in] */ const String& data,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding);

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
    virtual CARAPI LoadDataWithBaseURL(
        /* [in] */ const String& baseUrl,
        /* [in] */ const String& data,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding,
        /* [in] */ const String& historyUrl);

    /**
     * Asynchronously evaluates JavaScript in the context of the currently displayed page.
     * If non-null, |resultCallback| will be invoked with any result returned from that
     * execution. This method must be called on the UI thread and the callback will
     * be made on the UI thread.
     *
     * @param script the JavaScript to execute.
     * @param resultCallback A callback to be invoked when the script execution
     *                       completes with the result of the execution (if any).
     *                       May be null if no notificaion of the result is required.
     */
    virtual CARAPI EvaluateJavascript(
        /* [in] */ const String& script,
        /* [in] */ IValueCallback* resultCallback);

    /**
     * Saves the current view as a web archive.
     *
     * @param filename the filename where the archive should be placed
     */
    virtual CARAPI SaveWebArchive(
        /* [in] */ const String& filename);

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
    virtual CARAPI SaveWebArchive(
        /* [in] */ const String& basename,
        /* [in] */ Boolean autoname,
        /* [in] */ IValueCallback* callback);

    /**
     * Stops the current load.
     */
    virtual CARAPI StopLoading();

    /**
     * Reloads the current URL.
     */
    virtual CARAPI Reload();

    /**
     * Gets whether this WebView has a back history item.
     *
     * @return true iff this WebView has a back history item
     */
    virtual CARAPI_(Boolean) CanGoBack();

    /**
     * Goes back in the history of this WebView.
     */
    virtual CARAPI GoBack();

    /**
     * Gets whether this WebView has a forward history item.
     *
     * @return true iff this Webview has a forward history item
     */
    virtual CARAPI_(Boolean) CanGoForward();

    /**
     * Goes forward in the history of this WebView.
     */
    virtual CARAPI GoForward();

    /**
     * Gets whether the page can go back or forward the given
     * number of steps.
     *
     * @param steps the negative or positive number of steps to move the
     *              history
     */
    virtual CARAPI_(Boolean) CanGoBackOrForward(
        /* [in] */ Int32 steps);

    /**
     * Goes to the history item that is the number of steps away from
     * the current item. Steps is negative if backward and positive
     * if forward.
     *
     * @param steps the number of steps to take back or forward in the back
     *              forward list
     */
    virtual CARAPI GoBackOrForward(
        /* [in] */ Int32 steps);

    /**
     * Gets whether private browsing is enabled in this WebView.
     */
    virtual CARAPI_(Boolean) IsPrivateBrowsingEnabled();

    /**
     * Scrolls the contents of this WebView up by half the view size.
     *
     * @param top true to jump to the top of the page
     * @return true if the page was scrolled
     */
    virtual CARAPI_(Boolean) PageUp(
        /* [in] */ Boolean top);

    /**
     * Scrolls the contents of this WebView down by half the page size.
     *
     * @param bottom true to jump to bottom of page
     * @return true if the page was scrolled
     */
    virtual CARAPI_(Boolean) PageDown(
        /* [in] */ Boolean bottom);

    /**
     * Clears this WebView so that onDraw() will draw nothing but white background,
     * and onMeasure() will return 0 if MeasureSpec is not MeasureSpec.EXACTLY.
     */
    virtual CARAPI ClearView();

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
    virtual CARAPI_(AutoPtr<IPicture>) CapturePicture();

    /**
     * Gets the current scale of this WebView.
     *
     * @return the current scale
     *
     * @deprecated This method is prone to inaccuracy due to race conditions
     * between the web rendering and UI threads; prefer
     * {@link WebViewClient#onScaleChanged}.
     */
    virtual CARAPI_(Float) GetScale();

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
    virtual CARAPI SetInitialScale(
        /* [in] */ Int32 scaleInPercent);

    /**
     * Invokes the graphical zoom picker widget for this WebView. This will
     * result in the zoom widget appearing on the screen to control the zoom
     * level of this WebView.
     */
    virtual CARAPI InvokeZoomPicker();

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
    virtual CARAPI_(AutoPtr<IWebViewHitTestResult>) GetHitTestResult();

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
    virtual CARAPI RequestFocusNodeHref(
        /* [in] */ IMessage* hrefMsg);

    /**
     * Requests the URL of the image last touched by the user. msg will be sent
     * to its target with a String representing the URL as its object.
     *
     * @param msg the message to be dispatched with the result of the request
     *            as the data member with "url" as key. The result can be null.
     */
    virtual CARAPI RequestImageRef(
        /* [in] */ IMessage* msg);

    /**
     * Gets the URL for the current page. This is not always the same as the URL
     * passed to WebViewClient.onPageStarted because although the load for
     * that URL has begun, the current page may not have changed.
     *
     * @return the URL for the current page
     */
    virtual CARAPI_(String) GetUrl();

    /**
     * Gets the original URL for the current page. This is not always the same
     * as the URL passed to WebViewClient.onPageStarted because although the
     * load for that URL has begun, the current page may not have changed.
     * Also, there may have been redirects resulting in a different URL to that
     * originally requested.
     *
     * @return the URL that was originally requested for the current page
     */
    virtual CARAPI_(String) GetOriginalUrl();

    /**
     * Gets the title for the current page. This is the title of the current page
     * until WebViewClient.onReceivedTitle is called.
     *
     * @return the title for the current page
     */
    virtual CARAPI_(String) GetTitle();

    /**
     * Gets the favicon for the current page. This is the favicon of the current
     * page until WebViewClient.onReceivedIcon is called.
     *
     * @return the favicon for the current page
     */
    virtual CARAPI_(AutoPtr<IBitmap>) GetFavicon();

    /**
     * Gets the touch icon URL for the apple-touch-icon <link> element, or
     * a URL on this site's server pointing to the standard location of a
     * touch icon.
     *
     * @hide
     */
    virtual CARAPI_(String) GetTouchIconUrl();

    /**
     * Gets the progress for the current page.
     *
     * @return the progress for the current page between 0 and 100
     */
    virtual CARAPI_(Int32) GetProgress();

    /**
     * Gets the height of the HTML content.
     *
     * @return the height of the HTML content
     */
    virtual CARAPI_(Int32) GetContentHeight();

    /**
     * Gets the width of the HTML content.
     *
     * @return the width of the HTML content
     * @hide
     */
    virtual CARAPI_(Int32) GetContentWidth();

    /**
     * Pauses all layout, parsing, and JavaScript timers for all WebViews. This
     * is a global requests, not restricted to just this WebView. This can be
     * useful if the application has been paused.
     */
    virtual CARAPI PauseTimers();

    /**
     * Resumes all layout, parsing, and JavaScript timers for all WebViews.
     * This will resume dispatching all timers.
     */
    virtual CARAPI ResumeTimers();

    /**
     * Pauses any extra processing associated with this WebView and its
     * associated DOM, plugins, JavaScript etc. For example, if this WebView is
     * taken offscreen, this could be called to reduce unnecessary CPU or
     * network traffic. When this WebView is again "active", call onResume().
     * Note that this differs from pauseTimers(), which affects all WebViews.
     */
    virtual CARAPI OnPause();

    /**
     * Resumes a WebView after a previous call to onPause().
     */
    virtual CARAPI OnResume();

    /**
     * Gets whether this WebView is paused, meaning onPause() was called.
     * Calling onResume() sets the paused state back to false.
     *
     * @hide
     */
    virtual CARAPI_(Boolean) IsPaused();

    /**
     * Informs this WebView that memory is low so that it can free any available
     * memory.
     */
    virtual CARAPI FreeMemory();

    /**
     * Clears the resource cache. Note that the cache is per-application, so
     * this will clear the cache for all WebViews used.
     *
     * @param includeDiskFiles if false, only the RAM cache is cleared
     */
    virtual CARAPI ClearCache(
        /* [in] */ Boolean includeDiskFiles);

    /**
     * Removes the autocomplete popup from the currently focused form field, if
     * present. Note this only affects the display of the autocomplete popup,
     * it does not remove any saved form data from this WebView's store. To do
     * that, use {@link WebViewDatabase#clearFormData}.
     */
    virtual CARAPI ClearFormData();

    /**
     * Tells this WebView to clear its internal back/forward list.
     */
    virtual CARAPI ClearHistory();

    /**
     * Clears the SSL preferences table stored in response to proceeding with
     * SSL certificate errors.
     */
    virtual CARAPI ClearSslPreferences();

    /**
     * Gets the WebBackForwardList for this WebView. This contains the
     * back/forward list for use in querying each item in the history stack.
     * This is a copy of the private WebBackForwardList so it contains only a
     * snapshot of the current state. Multiple calls to this method may return
     * different objects. The object returned from this method will not be
     * updated to reflect any new state.
     */
    virtual CARAPI_(AutoPtr<IWebBackForwardList>) CopyBackForwardList();

    /**
     * Registers the listener to be notified as find-on-page operations
     * progress. This will replace the current listener.
     *
     * @param listener an implementation of {@link FindListener}
     */
    virtual CARAPI SetFindListener(
        /* [in] */ IWebViewFindListener* listener);

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
    virtual CARAPI FindNext(
        /* [in] */ Boolean forward);

    /**
     * Finds all instances of find on the page and highlights them.
     * Notifies any registered {@link FindListener}.
     *
     * @param find the string to find
     * @return the number of occurances of the String "find" that were found
     * @deprecated {@link #findAllAsync} is preferred.
     * @see #setFindListener
     */
    virtual CARAPI_(Int32) FindAll(
        /* [in] */ const String& find);

    /**
     * Finds all instances of find on the page and highlights them,
     * asynchronously. Notifies any registered {@link FindListener}.
     * Successive calls to this will cancel any pending searches.
     *
     * @param find the string to find.
     * @see #setFindListener
     */
    virtual CARAPI FindAllAsync(
        /* [in] */ const String& find);

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
    virtual CARAPI_(Boolean) ShowFindDialog(
        /* [in] */ const String& text,
        /* [in] */ Boolean showIme);

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
    static CARAPI_(String) FindAddress(
        /* [in] */ const String& addr);

    /**
     * Clears the highlighting surrounding text matches created by
     * {@link #findAllAsync}.
     */
    virtual CARAPI ClearMatches();

    /**
     * Queries the document to see if it contains any image references. The
     * message object will be dispatched with arg1 being set to 1 if images
     * were found and 0 if the document does not reference any images.
     *
     * @param response the message that will be dispatched with the result
     */
    virtual CARAPI DocumentHasImages(
        /* [in] */ IMessage* response);

    /**
     * Sets the WebViewClient that will receive various notifications and
     * requests. This will replace the current handler.
     *
     * @param client an implementation of WebViewClient
     */
    virtual CARAPI SetWebViewClient(
        /* [in] */ IWebViewClient* client);

    /**
     * Registers the interface to be used when content can not be handled by
     * the rendering engine, and should be downloaded instead. This will replace
     * the current handler.
     *
     * @param listener an implementation of DownloadListener
     */
    virtual CARAPI SetDownloadListener(
        /* [in] */ IDownloadListener* listener);

    /**
     * Sets the chrome handler. This is an implementation of WebChromeClient for
     * use in handling JavaScript dialogs, favicons, titles, and the progress.
     * This will replace the current handler.
     *
     * @param client an implementation of WebChromeClient
     */
    virtual CARAPI SetWebChromeClient(
        /* [in] */ IWebChromeClient* client);

    /**
     * Sets the Picture listener. This is an interface used to receive
     * notifications of a new Picture.
     *
     * @param listener an implementation of WebView.PictureListener
     * @deprecated This method is now obsolete.
     */
    virtual CARAPI SetPictureListener(
        /* [in] */ IWebViewPictureListener* listener);

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
     *    CARAPI_(String) toString() { return "injectedObject"; }
     * }
     * webView.addJavascriptInterface(new JsObject(), "injectedObject");
     * webView.loadData("<!DOCTYPE html><title></title>", "text/html", null);
     * webView.loadUrl("javascript:alert(injectedObject.toString())");</pre>
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
    virtual CARAPI AddJavascriptInterface(
        /* [in] */ IInterface* object,
        /* [in] */ const String& name);

    /**
     * Removes a previously injected Java object from this WebView. Note that
     * the removal will not be reflected in JavaScript until the page is next
     * (re)loaded. See {@link #addJavascriptInterface}.
     *
     * @param name the name used to expose the object in JavaScript
     */
    virtual CARAPI RemoveJavascriptInterface(
        /* [in] */ const String& name);

    /**
     * Gets the WebSettings object used to control the settings for this
     * WebView.
     *
     * @return a WebSettings object that can be used to control this WebView's
     *         settings
     */
    virtual CARAPI_(AutoPtr<IWebSettings>) GetSettings();

    /**
     * Gets the list of currently loaded plugins.
     *
     * @return the list of currently loaded plugins
     * @deprecated This was used for Gears, which has been deprecated.
     * @hide
     */
    static CARAPI_(AutoPtr<IPluginList>) GetPluginList();

    /**
     * @deprecated This was used for Gears, which has been deprecated.
     * @hide
     */
    virtual CARAPI RefreshPlugins(
        /* [in] */ Boolean reloadOpenPages);

    /**
     * Puts this WebView into text selection mode. Do not rely on this
     * functionality; it will be deprecated in the future.
     *
     * @deprecated This method is now obsolete.
     * @hide Since API level {@link android.os.Build.VERSION_CODES#JELLY_BEAN_MR1}
     */
    virtual CARAPI EmulateShiftHeld();

    /**
     * @deprecated WebView no longer needs to implement
     * ViewGroup.OnHierarchyChangeListener.  This method does nothing now.
     */
    //@Override
    // Cannot add @hide as this can always be accessed via the interface.
    CARAPI OnChildViewAdded(
        /* [in] */ IView* parent,
        /* [in] */ IView* child);

    /**
     * @deprecated WebView no longer needs to implement
     * ViewGroup.OnHierarchyChangeListener.  This method does nothing now.
     */
    //@Override
    // Cannot add @hide as this can always be accessed via the interface.
    CARAPI OnChildViewRemoved(
        /* [in] */ IView* p,
        /* [in] */ IView* child);

    /**
     * @deprecated WebView should not have implemented
     * ViewTreeObserver.OnGlobalFocusChangeListener. This method does nothing now.
     */
    //@Override
    // Cannot add @hide as this can always be accessed via the interface.
    CARAPI OnGlobalFocusChanged(
        /* [in] */ IView* oldFocus,
        /* [in] */ IView* newFocus);

    /**
     * @deprecated Only the default case, true, will be supported in a future version.
     */
    virtual CARAPI SetMapTrackballToArrowKeys(
        /* [in] */ Boolean setMap);

    virtual CARAPI FlingScroll(
        /* [in] */ Int32 vx,
        /* [in] */ Int32 vy);

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
    virtual CARAPI_(AutoPtr<IView>) GetZoomControls();

    /**
     * Gets whether this WebView can be zoomed in.
     *
     * @return true if this WebView can be zoomed in
     *
     * @deprecated This method is prone to inaccuracy due to race conditions
     * between the web rendering and UI threads; prefer
     * {@link WebViewClient#onScaleChanged}.
     */
    virtual CARAPI_(Boolean) CanZoomIn();

    /**
     * Gets whether this WebView can be zoomed out.
     *
     * @return true if this WebView can be zoomed out
     *
     * @deprecated This method is prone to inaccuracy due to race conditions
     * between the web rendering and UI threads; prefer
     * {@link WebViewClient#onScaleChanged}.
     */
    virtual CARAPI_(Boolean) CanZoomOut();

    /**
     * Performs a zoom operation in this WebView.
     *
     * @param zoomFactor the zoom factor to apply. The zoom factor will be clamped to the Webview's
     * zoom limits. This value must be in the range 0.01 to 100.0 inclusive.
     */
    virtual CARAPI ZoomBy(
        /* [in] */ Float zoomFactor);

    /**
     * Performs zoom in in this WebView.
     *
     * @return true if zoom in succeeds, false if no zoom changes
     */
    virtual CARAPI_(Boolean) ZoomIn();

    /**
     * Performs zoom out in this WebView.
     *
     * @return true if zoom out succeeds, false if no zoom changes
     */
    virtual CARAPI_(Boolean) ZoomOut();

    /**
     * @deprecated This method is now obsolete.
     * @hide Since API level {@link android.os.Build.VERSION_CODES#JELLY_BEAN_MR1}
     */
    virtual CARAPI DebugDump();

    /**
     * See {@link ViewDebug.HierarchyHandler#dumpViewHierarchyWithProperties(BufferedWriter, int)}
     * @hide
     */
    virtual CARAPI DumpViewHierarchyWithProperties(
        /* [in] */ IBufferedWriter* out,
        /* [in] */ Int32 level);

    /**
     * See {@link ViewDebug.HierarchyHandler#findHierarchyView(String, int)}
     * @hide
     */
    virtual CARAPI_(AutoPtr<IView>) FindHierarchyView(
        /* [in] */ const String& className,
        /* [in] */ Int32 hashCode);

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
    virtual CARAPI_(AutoPtr<IWebViewProvider>) GetWebViewProvider();

    //-------------------------------------------------------------------------
    // Override View methods
    //-------------------------------------------------------------------------

    // TODO: Add a test that enumerates all methods in ViewDelegte & ScrollDelegate, and ensures
    // there's a corresponding override (or better, caller) for each of them in here.

    CARAPI SetLayoutParams(
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI SetOverScrollMode(
        /* [in] */ Int32 mode);

    CARAPI SetScrollBarStyle(
        /* [in] */ Int32 style);

    CARAPI ComputeScroll();

    CARAPI_(Boolean) OnHoverEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI_(Boolean) OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) OnKeyMultiple(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 repeatCount,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) ShouldDelayChildPressedState();

    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI_(Boolean) PerformAccessibilityAction(
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments);

    CARAPI_(Boolean) PerformLongClick();

    CARAPI_(AutoPtr<IInputConnection>) OnCreateInputConnection(
        /* [in] */ IEditorInfo* outAttrs);

    CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus);

    CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) RequestFocus(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    CARAPI_(Boolean) RequestChildRectangleOnScreen(
        /* [in] */ IView* child,
        /* [in] */ IRect* rect,
        /* [in] */ Boolean immediate);

    CARAPI SetBackgroundColor(
        /* [in] */ Int32 color);

    CARAPI SetLayerType(
        /* [in] */ Int32 layerType,
        /* [in] */ IPaint* paint);

    CARAPI ToString(
        /* [out] */ String* info);

protected:
    WebView();

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
    WebView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle,
        /* [in] */ IMap* javaScriptInterfaces,
        /* [in] */ Boolean privateBrowsing);

    /**
     * Constructs a new WebView with a Context object.
     *
     * @param context a Context object used to access application assets
     */
    CARAPI Init(
        /* [in] */ IContext* context);

    /**
     * Constructs a new WebView with layout parameters.
     *
     * @param context a Context object used to access application assets
     * @param attrs an AttributeSet passed to our parent
     */
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    /**
     * Constructs a new WebView with layout parameters and a default style.
     *
     * @param context a Context object used to access application assets
     * @param attrs an AttributeSet passed to our parent
     * @param defStyle the default style resource ID
     */
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

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
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle,
        /* [in] */ Boolean privateBrowsing);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle,
        /* [in] */ IMap* javaScriptInterfaces,
        /* [in] */ Boolean privateBrowsing);

    CARAPI OnAttachedToWindow();

    CARAPI OnDetachedFromWindow();

    CARAPI_(Int32) ComputeHorizontalScrollRange();

    CARAPI_(Int32) ComputeHorizontalScrollOffset();

    CARAPI_(Int32) ComputeVerticalScrollRange();

    CARAPI_(Int32) ComputeVerticalScrollOffset();

    CARAPI_(Int32) ComputeVerticalScrollExtent();

    /** @hide */
    CARAPI_(void) OnDrawVerticalScrollBar(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IDrawable* scrollBar,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI_(void) OnOverScrolled(
        /* [in] */ Int32 scrollX,
        /* [in] */ Int32 scrollY,
        /* [in] */ Boolean clampedX,
        /* [in] */ Boolean clampedY);

    CARAPI_(void) OnWindowVisibilityChanged(
        /* [in] */ Int32 visibility);

    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI_(void) OnVisibilityChanged(
        /* [in] */ IView* changedView,
        /* [in] */ Int32 visibility);

    CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    /** @hide */
    CARAPI_(Boolean) SetFrame(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 ow,
        /* [in] */ Int32 oh);

    CARAPI_(void) OnScrollChanged(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 oldl,
        /* [in] */ Int32 oldt);

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

private:
    CARAPI_(void) EnsureProviderCreated();

    static CARAPI_(AutoPtr<IWebViewFactoryProvider>) GetFactory();

    static CARAPI_(void) CheckThread();

public:
    /**
     * URI scheme for telephone number.
     */
    static const String SCHEME_TEL;
    /**
     * URI scheme for email address.
     */
    static const String SCHEME_MAILTO;
    /**
     * URI scheme for map address.
     */
    static const String SCHEME_GEO;

private:
    static const String LOGTAG;

    //-------------------------------------------------------------------------
    // Private internal stuff
    //-------------------------------------------------------------------------

    AutoPtr<IWebViewProvider> mProvider;

    static Object sLock;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBVIEW_H__
