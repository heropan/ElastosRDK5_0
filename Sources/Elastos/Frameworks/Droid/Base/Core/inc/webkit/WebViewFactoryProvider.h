
#ifndef __WEBVIEWFACTORYPROVIDER_H__
#define __WEBVIEWFACTORYPROVIDER_H__

#include "ext/frameworkext.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * This is the main entry-point into the WebView back end implementations, which the WebView
 * proxy class uses to instantiate all the other objects as needed. The backend must provide an
 * implementation of this interface, and make it available to the WebView via mechanism TBD.
 * @hide
 */
interface IWebViewFactoryProvider : public IInterface
{
public:
    /**
     * This Interface provides glue for implementing the backend of WebView static methods which
     * cannot be implemented in-situ in the proxy class.
     */
    interface IStatics : public IInterface
    {
    public:
        /**
         * Implements the API method:
         * {@link android.webkit.WebView#findAddress(String)}
         */
        virtual CARAPI FindAddress(
            /* [in] */ const String& addr,
            /* [out] */ String* addrOut) = 0;

        /**
         * Implements the API methods:
         * {@link android.webkit.WebView#enablePlatformNotifications()}
         * {@link android.webkit.WebView#disablePlatformNotifications()}
         */
        virtual CARAPI SetPlatformNotificationsEnabled(
            /* [in] */ Boolean enable) = 0;

        /**
         * Implements the API method:
         * {@link android.webkit.WebSettings#getDefaultUserAgent(Context) }
         */
        virtual CARAPI GetDefaultUserAgent(
            /* [in] */ IContext* context,
            /* [out] */ String* userAgent) = 0;
    };

public:
    virtual CARAPI GetStatics(
        /* [out] */ IStatics** statics) = 0;

    /**
     * Construct a new WebViewProvider.
     * @param webView the WebView instance bound to this implementation instance. Note it will not
     * necessarily be fully constructed at the point of this call: defer real initialization to
     * WebViewProvider.init().
     * @param privateAccess provides access into WebView internal methods.
     */
    virtual CARAPI CreateWebView(
        /* [in] */ IWebView* webView,
        /* [in] */ IWebViewPrivateAccess* privateAccess,
        /* [out] */ IWebViewProvider** webViewOut) = 0;

    /**
     * Gets the singleton GeolocationPermissions instance for this WebView implementation. The
     * implementation must return the same instance on subsequent calls.
     * @return the single GeolocationPermissions instance.
     */
    virtual CARAPI GetGeolocationPermissions(
        /* [out] */ IGeolocationPermissions** geolocationPermissions) = 0;

    /**
     * Gets the singleton CookieManager instance for this WebView implementation. The
     * implementation must return the same instance on subsequent calls.
     *
     * @return the singleton CookieManager instance
     */
    virtual CARAPI GetCookieManager(
        /* [out] */ ICookieManager** cookieManager) = 0;

    /**
     * Gets the singleton WebIconDatabase instance for this WebView implementation. The
     * implementation must return the same instance on subsequent calls.
     *
     * @return the singleton WebIconDatabase instance
     */
    virtual CARAPI GetWebIconDatabase(
        /* [out] */ IWebIconDatabase** database) = 0;

    /**
     * Gets the singleton WebStorage instance for this WebView implementation. The
     * implementation must return the same instance on subsequent calls.
     *
     * @return the singleton WebStorage instance
     */
    virtual CARAPI GetWebStorage(
        /* [out] */ IWebStorage** webStorage) = 0;

    /**
     * Gets the singleton WebViewDatabase instance for this WebView implementation. The
     * implementation must return the same instance on subsequent calls.
     *
     * @return the singleton WebViewDatabase instance
     */
    virtual CARAPI GetWebViewDatabase(
        /* [in] */ IContext* context,
        /* [out] */ IWebViewDatabase** database) = 0;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__WEBVIEWFACTORYPROVIDER_H__
