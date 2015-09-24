
#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTSIOTHREADCLIENT_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTSIOTHREADCLIENT_H__

// import android.util.ArrayMap;

// import org.chromium.android_webview.AwContentsClient;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * Delegate for handling callbacks. All methods are called on the IO thread.
 *
 * You should create a separate instance for every WebContents that requires the
 * provided functionality.
 */
//@JNINamespace("android_webview")
class AwContentsIoThreadClient
{
public:
    //@CalledByNative
    virtual CARAPI_(Int32) GetCacheMode() = 0;

    //@CalledByNative
    virtual CARAPI_(Boolean) ShouldBlockContentUrls() = 0;

    //@CalledByNative
    virtual CARAPI_(Boolean) ShouldBlockFileUrls() = 0;

    //@CalledByNative
    virtual CARAPI_(Boolean) ShouldBlockNetworkLoads() = 0;

    //@CalledByNative
    virtual CARAPI_(Boolean) ShouldAcceptThirdPartyCookies() = 0;

    //@CalledByNative
    virtual CARAPI_(void) OnDownloadStart(
        /* [in] */ const String& url,
        /* [in] */ const String& userAgent,
        /* [in] */ const String& contentDisposition,
        /* [in] */ const String& mimeType,
        /* [in] */ Int64 contentLength) = 0;

    //@CalledByNative
    virtual CARAPI_(void) NewLoginRequest(
        /* [in] */ const String& realm,
        /* [in] */ const String& account,
        /* [in] */ const String& args) = 0;

    virtual CARAPI_(AutoPtr<AwWebResourceResponse>) ShouldInterceptRequest(
        /* [in] */ AwContentsClient::ShouldInterceptRequestParams params) = 0;

protected:
    // Protected methods ---------------------------------------------------------------------------

    //@CalledByNative
    virtual CARAPI_(AutoPtr<AwWebResourceResponse>) ShouldInterceptRequest(
        /* [in] */ const String& url,
        /* [in] */ Boolean isMainFrame,
        /* [in] */ Boolean hasUserGesture,
        /* [in] */ const String& method,
        /* [in] */ ArrayOf<String>* requestHeaderNames,
        /* [in] */ ArrayOf<String>* requestHeaderValues);
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTSIOTHREADCLIENT_H__
