
#ifndef __WEBSETTINGSCLASSIC_H__
#define __WEBSETTINGSCLASSIC_H__

#include "ext/frameworkext.h"
#include "os/HandlerBase.h"
#include "webkit/BrowserFrame.h"
#include "webkit/WebSettings.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Libcore::ICU::ILocale;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Net::Http::IEventHandler;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::IHandler;

#ifdef __cplusplus
extern "C"
{
#endif
        void Elastos_WebSettings_nativeSync(IInterface* obj, Int32 frame);
        void Elastos_WebSettings_InitCallback(Int32 cb);
#ifdef __cplusplus
}
#endif

namespace Elastos {
namespace Droid {
namespace Webkit {

struct ElaWebSettingsCallback
{
    Int32 (*getLayoutAlgorithm)(IInterface* obj);//return the value from Enum ordinal
    Int32 (*getTextSize)(IInterface* obj);
    Elastos::String (*getStandardFontFamily)(IInterface* obj);
    Elastos::String (*getFixedFontFamily)(IInterface* obj);
    Elastos::String (*getSansSerifFontFamily)(IInterface* obj);
    Elastos::String (*getSerifFontFamily)(IInterface* obj);
    Elastos::String (*getCursiveFontFamily)(IInterface* obj);
    Elastos::String (*getFantasyFontFamily)(IInterface* obj);
    Elastos::String (*getDefaultTextEncoding)(IInterface* obj);
    Elastos::String (*getUserAgentString)(IInterface* obj);//method
    Elastos::String (*getAcceptLanguage)(IInterface* obj);//method
    Int32 (*getMinimumFontSize)(IInterface* obj);
    Int32 (*getMinimumLogicalFontSize)(IInterface* obj);
    Int32 (*getDefaultFontSize)(IInterface* obj);
    Int32 (*getDefaultFixedFontSize)(IInterface* obj);
    Boolean (*getLoadsImagesAutomatically)(IInterface* obj);

    Boolean (*getBlockNetworkImage)(IInterface* obj);

    Boolean (*getBlockNetworkLoads)(IInterface* obj);
    Boolean (*getJavaScriptEnabled)(IInterface* obj);
    Boolean (*getAllowUniversalAccessFromFileURLs)(IInterface* obj);
    Boolean (*getAllowFileAccessFromFileURLs)(IInterface* obj);
    Int32 (*getPluginState)(IInterface* obj);//return the value from Enum ordinal

    Boolean (*getAppCacheEnabled)(IInterface* obj);
    Elastos::String (*getAppCachePath)(IInterface* obj);
    Int64 (*getAppCacheMaxSize)(IInterface* obj);

    Boolean (*getWorkersEnabled)(IInterface* obj);

    Boolean (*getJavaScriptCanOpenWindowsAutomatically)(IInterface* obj);
    Boolean (*getUseWideViewport)(IInterface* obj);
    Boolean (*getSupportMultipleWindows)(IInterface* obj);
    Boolean (*getShrinksStandaloneImagesToFit)(IInterface* obj);
    Int64 (*getMaximumDecodedImageSize)(IInterface* obj);
    Boolean (*getPrivateBrowsingEnabled)(IInterface* obj);
    Boolean (*getSyntheticLinksEnabled)(IInterface* obj);
    Boolean (*getUseDoubleTree)(IInterface* obj);
    Int32 (*getPageCacheCapacity)(IInterface* obj);

    // Ordinal() method and value field for enums
    //jmethodID mOrdinal;
    //jfieldID  mTextSizeValue;

    Boolean (*getDatabaseEnabled)(IInterface* obj);

    Boolean (*getDomStorageEnabled)(IInterface* obj);

    Boolean (*getGeolocationEnabled)(IInterface* obj);
    Elastos::String (*getGeolocationDatabasePath)(IInterface* obj);
    Boolean (*getXSSAuditorEnabled)(IInterface* obj);

    Boolean (*getLinkPrefetchEnabled)(IInterface* obj);

    Elastos::String (*getDatabasePath)(IInterface* obj);
    Boolean (*getDatabasePathHasBeenSet)(IInterface* obj);

    Boolean (*getAutoFillEnabled)(IInterface* obj);
    AutoPtr<IInterface> (*getAutoFillProfile)(IInterface* obj);
    Elastos::String (*getAutoFillProfileFullName)(IInterface* obj);
    Elastos::String (*getAutoFillProfileEmailAddress)(IInterface* obj);
    Elastos::String (*getAutoFillProfileCompanyName)(IInterface* obj);
    Elastos::String (*getAutoFillProfileAddressLine1)(IInterface* obj);
    Elastos::String (*getAutoFillProfileAddressLine2)(IInterface* obj);
    Elastos::String (*getAutoFillProfileCity)(IInterface* obj);
    Elastos::String (*getAutoFillProfileState)(IInterface* obj);
    Elastos::String (*getAutoFillProfileZipCode)(IInterface* obj);
    Elastos::String (*getAutoFillProfileCountry)(IInterface* obj);
    Elastos::String (*getAutoFillProfilePhoneNumber)(IInterface* obj);

    Int32 (*getOverrideCacheMode)(IInterface* obj);
    Boolean (*getPasswordEchoEnabled)(IInterface* obj);
    Boolean (*getMediaPlaybackRequiresUserGesture)(IInterface* obj);
};

class CWebViewClassic;

/**
 * WebSettings implementation for the WebViewClassic implementation of WebView.
 * @hide
 */
class WebSettingsClassic
    : public ElRefBase
    , public WebSettings
    , public IWebSettingsClassic
{
private:
    // Class to handle messages before WebCore is ready.
    class EventHandler : public ElRefBase
    {
        friend class WebSettingsClassic;

    private:
        class InnerHandler : public HandlerBase
        {
        public:
            InnerHandler(
                /* [in] */ EventHandler* owner);

            CARAPI HandleMessage(
                /* [in] */ IMessage* msg);

        private:
            EventHandler* mOwner;
        };

    public:
        // Message id for syncing
        static const Int32 SYNC = 0;
        // Message id for setting priority
        static const Int32 PRIORITY = 1;
        // Message id for writing double-tap toast count
        static const Int32 SET_DOUBLE_TAP_TOAST_COUNT = 2;

    public:
        EventHandler(
            /* [in] */ WebSettingsClassic* owner);

    private:
        CARAPI_(void) CreateHandler();

        CARAPI_(void) SetRenderPriority();

        /**
         * Send a message to the private queue or handler.
         */
        CARAPI_(Boolean) SendMessage(
            /* [in] */ IMessage* msg);

    private:
        // Actual WebCore thread handler
        AutoPtr<IHandler> mHandler;
        WebSettingsClassic* mOwner;
        Object mLock;
    };

public:
    /**
     * Package constructor to prevent clients from creating a new settings
     * instance.
     */
    WebSettingsClassic(
        /* [in] */ IContext* context,
        /* [in] */ CWebViewClassic* webview);

    CAR_INTERFACE_DECL();

    /**
     * Returns the default User-Agent used by a WebView.
     * An instance of WebView could use a different User-Agent if a call
     * is made to {@link WebSettings#setUserAgent(int)} or
     * {@link WebSettings#setUserAgentString(String)}.
     *
     * @param context a Context object used to access application assets
     * @param locale The Locale to use in the User-Agent string.
     * @see WebViewFactoryProvider#getDefaultUserAgent(Context)
     * @see WebView#getDefaultUserAgent(Context)
     */
    static CARAPI_(String) GetDefaultUserAgentForLocale(
        /* [in] */  IContext* context,
        /* [in] */  ILocale* locale);

    /**
     * @see android.webkit.WebSettings#setNavDump(boolean)
     */
    CARAPI SetNavDump(
        /* [in] */ Boolean enabled);

    /**
     * @see android.webkit.WebSettings#getNavDump()
     */
    CARAPI_(Boolean) GetNavDump();

    /**
     * @see android.webkit.WebSettings#setSupportZoom(boolean)
     */
    CARAPI SetSupportZoom(
        /* [in] */ Boolean support);

    /**
     * @see android.webkit.WebSettings#supportZoom()
     */
    CARAPI_(Boolean) SupportZoom();

    /**
     * @see android.webkit.WebSettings#setMediaPlaybackRequiresUserGesture(boolean)
     */
    CARAPI SetMediaPlaybackRequiresUserGesture(
        /* [in] */ Boolean support);

    /**
     * @see android.webkit.WebSettings#getMediaPlaybackRequiresUserGesture()
     */
    CARAPI_(Boolean) GetMediaPlaybackRequiresUserGesture();

    /**
     * @see android.webkit.WebSettings#setBuiltInZoomControls(boolean)
     */
    CARAPI SetBuiltInZoomControls(
        /* [in] */ Boolean enabled);

    /**
     * @see android.webkit.WebSettings#getBuiltInZoomControls()
     */
    CARAPI_(Boolean) GetBuiltInZoomControls();

    /**
     * @see android.webkit.WebSettings#setDisplayZoomControls(boolean)
     */
    CARAPI SetDisplayZoomControls(
        /* [in] */ Boolean enabled);

    /**
     * @see android.webkit.WebSettings#getDisplayZoomControls()
     */
    CARAPI_(Boolean) GetDisplayZoomControls();

    /**
     * @see android.webkit.WebSettings#setAllowFileAccess(boolean)
     */
    CARAPI SetAllowFileAccess(
        /* [in] */ Boolean allow);

    /**
     * @see android.webkit.WebSettings#getAllowFileAccess()
     */
    CARAPI_(Boolean) GetAllowFileAccess();

    /**
     * @see android.webkit.WebSettings#setAllowContentAccess(boolean)
     */
    CARAPI SetAllowContentAccess(
        /* [in] */ Boolean allow);

    /**
     * @see android.webkit.WebSettings#getAllowContentAccess()
     */
    CARAPI_(Boolean) GetAllowContentAccess();

    /**
     * @see android.webkit.WebSettings#setLoadWithOverviewMode(boolean)
     */
    CARAPI SetLoadWithOverviewMode(
        /* [in] */ Boolean overview);

    /**
     * @see android.webkit.WebSettings#getLoadWithOverviewMode()
     */
    CARAPI_(Boolean) GetLoadWithOverviewMode();

    /**
     * @see android.webkit.WebSettings#setEnableSmoothTransition(boolean)
     */
    CARAPI SetEnableSmoothTransition(
        /* [in] */ Boolean enable);

    /**
     * @see android.webkit.WebSettings#enableSmoothTransition()
     */
    CARAPI_(Boolean) EnableSmoothTransition();

    /**
     * @see android.webkit.WebSettings#setUseWebViewBackgroundForOverscrollBackground(boolean)
     */
    CARAPI SetUseWebViewBackgroundForOverscrollBackground(
        /* [in] */ Boolean view);

    /**
     * @see android.webkit.WebSettings#getUseWebViewBackgroundForOverscrollBackground()
     */
    CARAPI_(Boolean) GetUseWebViewBackgroundForOverscrollBackground();

    /**
     * @see android.webkit.WebSettings#setSaveFormData(boolean)
     */
    CARAPI SetSaveFormData(
        /* [in] */ Boolean save);

    /**
     * @see android.webkit.WebSettings#getSaveFormData()
     */
    CARAPI_(Boolean) GetSaveFormData();

    /**
     * @see android.webkit.WebSettings#setSavePassword(boolean)
     */
    CARAPI SetSavePassword(
        /* [in] */ Boolean save);

    /**
     * @see android.webkit.WebSettings#getSavePassword()
     */
    CARAPI_(Boolean) GetSavePassword();

    /**
     * @see android.webkit.WebSettings#setTextZoom(int)
     */
    CARAPI SetTextZoom(
        /* [in] */ Int32 textZoom);

    /**
     * @see android.webkit.WebSettings#getTextZoom()
     */
    CARAPI_(Int32) GetTextZoom();

    /**
     * Set the double-tap zoom of the page in percent. Default is 100.
     * @param doubleTapZoom A percent value for increasing or decreasing the double-tap zoom.
     */
    CARAPI SetDoubleTapZoom(
        /* [in] */ Int32 doubleTapZoom);

    /**
     * Get the double-tap zoom of the page in percent.
     * @return A percent value describing the double-tap zoom.
     */
    CARAPI GetDoubleTapZoom(
        /* [out] */ Int32* zoom);

    /**
     * @see android.webkit.WebSettings#setDefaultZoom(android.webkit.WebSettingsClassic.ZoomDensity)
     */
    CARAPI SetDefaultZoom(
        /* [in] */ ZoomDensity zoom);

    /**
     * @see android.webkit.WebSettings#getDefaultZoom()
     */
    CARAPI_(ZoomDensity) GetDefaultZoom();

    /**
     * @see android.webkit.WebSettings#setLightTouchEnabled(boolean)
     */
    CARAPI SetLightTouchEnabled(
        /* [in] */ Boolean enabled);

    /**
     * @see android.webkit.WebSettings#getLightTouchEnabled()
     */
    CARAPI_(Boolean) GetLightTouchEnabled();

    /**
     * @see android.webkit.WebSettings#setUseDoubleTree(boolean)
     */
    CARAPI SetUseDoubleTree(
        /* [in] */ Boolean use);

    /**
     * @see android.webkit.WebSettings#getUseDoubleTree()
     */
    CARAPI_(Boolean) GetUseDoubleTree();

    /**
     * @see android.webkit.WebSettings#setUserAgent(int)
     */
    CARAPI SetUserAgent(
        /* [in] */ Int32 ua);

    /**
     * @see android.webkit.WebSettings#getUserAgent()
     */
    CARAPI_(Int32) GetUserAgent();

    /**
     * @see android.webkit.WebSettings#setUseWideViewPort(boolean)
     */
    CARAPI SetUseWideViewPort(
        /* [in] */ Boolean use);

    /**
     * @see android.webkit.WebSettings#getUseWideViewPort()
     */
    CARAPI_(Boolean) GetUseWideViewPort();

    /**
     * @see android.webkit.WebSettings#setSupportMultipleWindows(boolean)
     */
    CARAPI SetSupportMultipleWindows(
        /* [in] */ Boolean support);

    /**
     * @see android.webkit.WebSettings#supportMultipleWindows()
     */
    CARAPI_(Boolean) SupportMultipleWindows();

    /**
     * @see android.webkit.WebSettings#setLayoutAlgorithm(android.webkit.WebSettingsClassic.LayoutAlgorithm)
     */
    CARAPI SetLayoutAlgorithm(
        /* [in] */ LayoutAlgorithm l);

    /**
     * @see android.webkit.WebSettings#getLayoutAlgorithm()
     */
    CARAPI_(LayoutAlgorithm) GetLayoutAlgorithm();

    /**
     * @see android.webkit.WebSettings#setStandardFontFamily(java.lang.String)
     */
    CARAPI SetStandardFontFamily(
        /* [in] */ const String& font);

    /**
     * @see android.webkit.WebSettings#getStandardFontFamily()
     */
    CARAPI_(String) GetStandardFontFamily();

    /**
     * @see android.webkit.WebSettings#setFixedFontFamily(java.lang.String)
     */
    CARAPI SetFixedFontFamily(
        /* [in] */ const String& font);

    /**
     * @see android.webkit.WebSettings#getFixedFontFamily()
     */
    CARAPI_(String) GetFixedFontFamily();

    /**
     * @see android.webkit.WebSettings#setSansSerifFontFamily(java.lang.String)
     */
    CARAPI SetSansSerifFontFamily(
        /* [in] */  const String& font);

    /**
     * @see android.webkit.WebSettings#getSansSerifFontFamily()
     */
    CARAPI_(String) GetSansSerifFontFamily();

    /**
     * @see android.webkit.WebSettings#setSerifFontFamily(java.lang.String)
     */
    CARAPI SetSerifFontFamily(
        /* [in] */  const String& font);

    /**
     * @see android.webkit.WebSettings#getSerifFontFamily()
     */
    CARAPI_(String) GetSerifFontFamily();

    /**
     * @see android.webkit.WebSettings#setCursiveFontFamily(java.lang.String)
     */
    CARAPI SetCursiveFontFamily(
        /* [in] */ const String& font);

    /**
     * @see android.webkit.WebSettings#getCursiveFontFamily()
     */
    CARAPI_(String) GetCursiveFontFamily();

    /**
     * @see android.webkit.WebSettings#setFantasyFontFamily(java.lang.String)
     */
    CARAPI SetFantasyFontFamily(
        /* [in] */ const String& font);

    /**
     * @see android.webkit.WebSettings#getFantasyFontFamily()
     */
    CARAPI_(String) GetFantasyFontFamily();

    /**
     * @see android.webkit.WebSettings#setMinimumFontSize(int)
     */
    CARAPI SetMinimumFontSize(
        /* [in] */ Int32 size);

    /**
     * @see android.webkit.WebSettings#getMinimumFontSize()
     */
    CARAPI_(Int32) GetMinimumFontSize();

    /**
     * @see android.webkit.WebSettings#setMinimumLogicalFontSize(int)
     */
    CARAPI SetMinimumLogicalFontSize(
        /* [in] */ Int32 size);

    /**
     * @see android.webkit.WebSettings#getMinimumLogicalFontSize()
     */
    CARAPI_(Int32) GetMinimumLogicalFontSize();

    /**
     * @see android.webkit.WebSettings#setDefaultFontSize(int)
     */
    CARAPI SetDefaultFontSize(
        /* [in] */ Int32 size);

    /**
     * @see android.webkit.WebSettings#getDefaultFontSize()
     */
    CARAPI_(Int32) GetDefaultFontSize();

    /**
     * @see android.webkit.WebSettings#setDefaultFixedFontSize(int)
     */
    CARAPI SetDefaultFixedFontSize(
        /* [in] */ Int32 size);

    /**
     * @see android.webkit.WebSettings#getDefaultFixedFontSize()
     */
    CARAPI_(Int32) GetDefaultFixedFontSize();

    /**
     * Set the number of pages cached by the WebKit for the history navigation.
     * @param size A non-negative integer between 0 (no cache) and 20 (max).
     */
    CARAPI SetPageCacheCapacity(
        /* [in] */ Int32 size);

    /**
     * @see android.webkit.WebSettings#setLoadsImagesAutomatically(boolean)
     */
    CARAPI SetLoadsImagesAutomatically(
        /* [in] */ Boolean flag);

    /**
     * @see android.webkit.WebSettings#getLoadsImagesAutomatically()
     */
    CARAPI_(Boolean) GetLoadsImagesAutomatically();

    /**
     * @see android.webkit.WebSettings#setBlockNetworkImage(boolean)
     */
    CARAPI SetBlockNetworkImage(
        /* [in] */ Boolean flag);

    /**
     * @see android.webkit.WebSettings#getBlockNetworkImage()
     */
    CARAPI_(Boolean) GetBlockNetworkImage();

    /**
     * @see android.webkit.WebSettings#setBlockNetworkLoads(boolean)
     */
    CARAPI SetBlockNetworkLoads(
        /* [in] */ Boolean flag);

    /**
     * @see android.webkit.WebSettings#getBlockNetworkLoads()
     */
    CARAPI_(Boolean) GetBlockNetworkLoads();

    /**
     * @see android.webkit.WebSettings#setJavaScriptEnabled(boolean)
     */
    CARAPI SetJavaScriptEnabled(
        /* [in] */ Boolean flag);

    /**
     * @see android.webkit.WebSettings#setAllowUniversalAccessFromFileURLs
     */
    CARAPI SetAllowUniversalAccessFromFileURLs(
        /* [in] */ Boolean flag);

    /**
     * @see android.webkit.WebSettings#setAllowFileAccessFromFileURLs
     */
    CARAPI SetAllowFileAccessFromFileURLs(
        /* [in] */ Boolean flag);

    /**
     * Tell the WebView to use Skia's hardware accelerated rendering path
     * @param flag True if the WebView should use Skia's hw-accel path
     */
    CARAPI SetHardwareAccelSkiaEnabled(
        /* [in] */ Boolean flag);

    /**
     * @return True if the WebView is using hardware accelerated skia
     */
    CARAPI GetHardwareAccelSkiaEnabled(
        /* [out] */ Boolean* enabled);

    /**
     * Tell the WebView to show the visual indicator
     * @param flag True if the WebView should show the visual indicator
     */
    CARAPI SetShowVisualIndicator(
        /* [in] */ Boolean flag);

    /**
     * @return True if the WebView is showing the visual indicator
     */
    CARAPI GetShowVisualIndicator(
        /* [out] */ Boolean* isShowing);

    /**
     * @see android.webkit.WebSettings#setPluginsEnabled(boolean)
     */
    CARAPI SetPluginsEnabled(
        /* [in] */ Boolean flag);

    /**
     * @see android.webkit.WebSettings#setPluginState(android.webkit.WebSettingsClassic.PluginState)
     */
    CARAPI SetPluginState(
        /* [in] */ PluginState state);

    /**
     * @see android.webkit.WebSettings#setPluginsPath(java.lang.String)
     */
    CARAPI SetPluginsPath(
        /* [in] */ const String& pluginsPath);

    /**
     * @see android.webkit.WebSettings#setDatabasePath(java.lang.String)
     */
    CARAPI SetDatabasePath(
        /* [in] */ const String& databasePath);

    /**
     * @see android.webkit.WebSettings#setGeolocationDatabasePath(java.lang.String)
     */
    CARAPI SetGeolocationDatabasePath(
        /* [in] */ const String& databasePath);

    /**
     * @see android.webkit.WebSettings#setAppCacheEnabled(boolean)
     */
    CARAPI SetAppCacheEnabled(
        /* [in] */ Boolean flag);

    /**
     * @see android.webkit.WebSettings#setAppCachePath(java.lang.String)
     */
    CARAPI SetAppCachePath(
        /* [in] */ const String& path);

    /**
     * @see android.webkit.WebSettings#setAppCacheMaxSize(long)
     */
    CARAPI SetAppCacheMaxSize(
        /* [in] */ Int64 appCacheMaxSize);

    /**
     * @see android.webkit.WebSettings#setDatabaseEnabled(boolean)
     */
    CARAPI SetDatabaseEnabled(
        /* [in] */ Boolean flag);

    /**
     * @see android.webkit.WebSettings#setDomStorageEnabled(boolean)
     */
    CARAPI SetDomStorageEnabled(
        /* [in] */ Boolean flag);

    /**
     * @see android.webkit.WebSettings#getDomStorageEnabled()
     */
    CARAPI_(Boolean) GetDomStorageEnabled();

    /**
     * @see android.webkit.WebSettings#getDatabasePath()
     */
    CARAPI_(String) GetDatabasePath();

    /**
     * @see android.webkit.WebSettings#getDatabaseEnabled()
     */
    CARAPI_(Boolean) GetDatabaseEnabled();

    /**
     * Tell the WebView to enable WebWorkers API.
     * @param flag True if the WebView should enable WebWorkers.
     * Note that this flag only affects V8. JSC does not have
     * an equivalent setting.
     */
    CARAPI SetWorkersEnabled(
        /* [in] */ Boolean flag);

    /**
     * @see android.webkit.WebSettings#setGeolocationEnabled(boolean)
     */
    CARAPI SetGeolocationEnabled(
        /* [in] */ Boolean flag);

    /**
     * Sets whether XSS Auditor is enabled.
     * Only used by LayoutTestController.
     * @param flag Whether XSS Auditor should be enabled.
     */
    CARAPI SetXSSAuditorEnabled(
        /* [in] */ Boolean flag);

    /**
     * Enables/disables HTML5 link "prefetch" parameter.
     */
    CARAPI SetLinkPrefetchEnabled(
        /* [in] */ Boolean flag);

    /**
     * @see android.webkit.WebSettings#getJavaScriptEnabled()
     */
    CARAPI_(Boolean) GetJavaScriptEnabled();

    /**
     * @see android.webkit.WebSettings#getAllowUniversalFileAccessFromFileURLs
     */
    CARAPI_(Boolean) GetAllowUniversalAccessFromFileURLs();

    /**
     * @see android.webkit.WebSettings#getAllowFileAccessFromFileURLs
     */
    CARAPI_(Boolean) GetAllowFileAccessFromFileURLs();

    /**
     * @see android.webkit.WebSettings#getPluginsEnabled()
     */
    CARAPI_(Boolean) GetPluginsEnabled();

    /**
     * @see android.webkit.WebSettings#getPluginState()
     */
    CARAPI_(PluginState) GetPluginState();

    /**
     * @see android.webkit.WebSettings#getPluginsPath()
     */
    CARAPI_(String) GetPluginsPath();

    /**
     * @see android.webkit.WebSettings#setJavaScriptCanOpenWindowsAutomatically(boolean)
     */
    CARAPI SetJavaScriptCanOpenWindowsAutomatically(
        /* [in] */ Boolean flag);

    /**
     * @see android.webkit.WebSettings#getJavaScriptCanOpenWindowsAutomatically()
     */
    CARAPI_(Boolean) GetJavaScriptCanOpenWindowsAutomatically();

    /**
     * @see android.webkit.WebSettings#setDefaultTextEncodingName(java.lang.String)
     */
    CARAPI SetDefaultTextEncodingName(
        /* [in] */ const String& encoding);

    /**
     * @see android.webkit.WebSettings#getDefaultTextEncodingName()
     */
    CARAPI_(String) GetDefaultTextEncodingName();

    /**
     * @see android.webkit.WebSettings#setUserAgentString(java.lang.String)
     */
    CARAPI SetUserAgentString(
        /* [in] */ const String& ua);

    /**
     * @see android.webkit.WebSettings#getUserAgentString()
     */
    CARAPI_(String) GetUserAgentString();

    /* package api to grab the Accept Language string. */
    CARAPI_(String) GetAcceptLanguage();

    /* package */
    CARAPI_(Boolean) IsNarrowColumnLayout();

    /**
     * @see android.webkit.WebSettings#setNeedInitialFocus(boolean)
     */
    CARAPI SetNeedInitialFocus(
        /* [in] */ Boolean flag);

    /* Package api to get the choice whether it needs to set initial focus. */
    CARAPI_(Boolean) GetNeedInitialFocus();

    /**
     * @see android.webkit.WebSettings#setRenderPriority(android.webkit.WebSettingsClassic.RenderPriority)
     */
    CARAPI SetRenderPriority(
        /* [in] */ RenderPriority priority);

    /**
     * @see android.webkit.WebSettings#setCacheMode(int)
     */
    CARAPI SetCacheMode(
        /* [in] */ Int32 mode);

    /**
     * @see android.webkit.WebSettings#getCacheMode()
     */
    CARAPI_(Int32) GetCacheMode();

    /**
     * If set, webkit alternately shrinks and expands images viewed outside
     * of an HTML page to fit the screen. This conflicts with attempts by
     * the UI to zoom in and out of an image, so it is set false by default.
     * @param shrink Set true to let webkit shrink the standalone image to fit.
     */
    CARAPI SetShrinksStandaloneImagesToFit(
        /* [in] */ Boolean shrink);

    /**
     * Specify the maximum decoded image size. The default is
     * 2 megs for small memory devices and 8 megs for large memory devices.
     * @param size The maximum decoded size, or zero to set to the default.
     */
    CARAPI SetMaximumDecodedImageSize(
        /* [in] */ Int64 size);

    /**
     * Returns whether to use fixed viewport.  Use fixed viewport
     * whenever wide viewport is on.
     */
    CARAPI_(Boolean) GetUseFixedViewport();

    /**
     * Returns whether private browsing is enabled.
     */
    CARAPI_(Boolean) IsPrivateBrowsingEnabled();

    /**
     * Sets whether private browsing is enabled.
     * @param flag Whether private browsing should be enabled.
     */
    CARAPI SetPrivateBrowsingEnabled(
        /* [in] */ Boolean flag);

    /**
     * Returns whether the viewport metatag can disable zooming
     */
    CARAPI ForceUserScalable(
        /* [out] */ Boolean* result);

    /**
     * Sets whether viewport metatag can disable zooming.
     * @param flag Whether or not to forceably enable user scalable.
     */
    CARAPI SetForceUserScalable(
        /* [in] */ Boolean flag);

    CARAPI SetSyntheticLinksEnabled(
        /* [in] */ Boolean flag);

    CARAPI SetAutoFillEnabled(
        /* [in] */ Boolean enabled);

    CARAPI GetAutoFillEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetAutoFillProfile(
        /* [in] */ IAutoFillProfile* profile);

    CARAPI GetAutoFillProfile(
        /* [out] */ IAutoFillProfile** profile);

    CARAPI GetDoubleTapToastCount(
        /* [out] */ Int32* count);

    CARAPI SetDoubleTapToastCount(
        /* [in] */ Int32 count);

    CARAPI SetProperty(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    CARAPI GetProperty(
        /* [in] */ const String& key,
        /* [out] */ String* property);

    /**
     * Transfer messages from the queue to the new WebCoreThread. Called from
     * WebCore thread.
     */
    CARAPI_(void) SyncSettingsAndCreateHandler(
        /* [in] */ BrowserFrame* frame);

    /**
     * Let the Settings object know that our owner is being destroyed.
     */
    CARAPI_(void) OnDestroyed();

    CARAPI GetNavDump(
        /* [out] */ Boolean* result);

    CARAPI SupportZoom(
        /* [out] */ Boolean* result);

    CARAPI GetMediaPlaybackRequiresUserGesture(
        /* [out] */ Boolean* result);

    CARAPI GetBuiltInZoomControls(
        /* [out] */ Boolean* result);

    CARAPI GetDisplayZoomControls(
        /* [out] */ Boolean* result);

    CARAPI GetAllowFileAccess(
        /* [out] */ Boolean* result);

    CARAPI GetAllowContentAccess(
        /* [out] */ Boolean* result);

    CARAPI GetLoadWithOverviewMode(
        /* [out] */ Boolean* result);

    CARAPI EnableSmoothTransition(
        /* [out] */ Boolean* result);

    CARAPI GetUseWebViewBackgroundForOverscrollBackground(
        /* [out] */ Boolean* result);

    CARAPI GetSaveFormData(
        /* [out] */ Boolean* result);

    CARAPI GetSavePassword(
        /* [out] */ Boolean* result);

    CARAPI GetTextZoom(
        /* [out] */ Int32* textZoom);

    CARAPI SetTextSize(
        /* [in] */ TextSize t);

    CARAPI GetTextSize(
        /* [out] */ TextSize* textSize);

    CARAPI GetDefaultZoom(
        /* [out] */ ZoomDensity* zoom);

    CARAPI GetLightTouchEnabled(
        /* [out] */ Boolean* result);

    CARAPI GetUseDoubleTree(
        /* [out] */ Boolean* result);

    CARAPI GetUserAgent(
        /* [out] */ Int32* ua);

    CARAPI GetUseWideViewPort(
        /* [out] */ Boolean* result);

    CARAPI SupportMultipleWindows(
        /* [out] */ Boolean* result);

    CARAPI GetLayoutAlgorithm(
        /* [out] */ LayoutAlgorithm* l);

    CARAPI GetStandardFontFamily(
        /* [out] */ String* font);

    CARAPI GetFixedFontFamily(
        /* [out] */ String* font);

    CARAPI GetSansSerifFontFamily(
        /* [out] */ String* font);

    CARAPI GetSerifFontFamily(
        /* [out] */ String* font);

    CARAPI GetCursiveFontFamily(
        /* [out] */ String* font);

    CARAPI GetFantasyFontFamily(
        /* [out] */ String* font);

    CARAPI GetMinimumFontSize(
        /* [out] */ Int32* size);

    CARAPI GetMinimumLogicalFontSize(
        /* [out] */ Int32* size);

    CARAPI GetDefaultFontSize(
        /* [out] */ Int32* size);

    CARAPI GetDefaultFixedFontSize(
        /* [out] */ Int32* size);

    CARAPI GetLoadsImagesAutomatically(
        /* [out] */ Boolean* flag);

    CARAPI GetBlockNetworkImage(
        /* [out] */ Boolean* flag);

    CARAPI GetBlockNetworkLoads(
        /* [out] */ Boolean* flag);

    CARAPI GetDomStorageEnabled(
        /* [out] */ Boolean* flag);

    CARAPI GetDatabasePath(
        /* [out] */ String* path);

    CARAPI GetDatabaseEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI GetJavaScriptEnabled(
        /* [out] */ Boolean* flag);

    CARAPI GetAllowUniversalAccessFromFileURLs(
        /* [out] */ Boolean* result);

    CARAPI GetAllowFileAccessFromFileURLs(
        /* [out] */ Boolean* result);

    CARAPI GetPluginsEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI GetPluginState(
        /* [out] */ PluginState* state);

    CARAPI GetPluginsPath(
        /* [out] */ String* path);

    CARAPI GetJavaScriptCanOpenWindowsAutomatically(
        /* [out] */ Boolean* flag);

    CARAPI GetDefaultTextEncodingName(
        /* [out] */ String* encoding);

    CARAPI GetUserAgentString(
        /* [out] */ String* ua);

    CARAPI GetCacheMode(
        /* [out] */ Int32* mode);

private:
    /**
     * Looks at sLocale and returns current AcceptLanguage String.
     * @return Current AcceptLanguage String.
     */
    CARAPI_(String) GetCurrentAcceptLanguage();

    /**
     * Convert obsolete language codes, including Hebrew/Indonesian/Yiddish,
     * to new standard.
     */
    static CARAPI_(String) ConvertObsoleteLanguageCodeToNew(
        /* [in] */ const String& langCode);

    static CARAPI_(void) AddLocaleToHttpAcceptLanguage(
        /* [in] */ StringBuilder& builder,
        /* [in] */ ILocale* locale);

    /**
     * Looks at sLocale and mContext and returns current UserAgent String.
     * @return Current UserAgent String.
     */
    CARAPI_(String) GetCurrentUserAgent();

    CARAPI VerifyNetworkAccess();

    CARAPI_(Int32) Pin(
        /* [in] */ Int32 size);

    /* Post a SYNC message to handle syncing the native settings. */
    CARAPI_(void) PostSync();

    // Synchronize the native and java settings.
    CARAPI_(void) NativeSync(
        /* [in] */ Int32 nativeFrame);

    //begin callback definition
    //return the value from Enum ordinal
    static Int32 GetLayoutAlgorithm(
            /* [in] */ IInterface* obj);

    static Int32 GetTextSize(
            /* [in] */ IInterface* obj);

    static Elastos::String GetStandardFontFamily(
            /* [in] */ IInterface* obj);

    static Elastos::String GetFixedFontFamily(
            /* [in] */ IInterface* obj);

    static Elastos::String GetSansSerifFontFamily(
            /* [in] */ IInterface* obj);

    static Elastos::String GetSerifFontFamily(
            /* [in] */ IInterface* obj);

    static Elastos::String GetCursiveFontFamily(
            /* [in] */ IInterface* obj);

    static Elastos::String GetFantasyFontFamily(
            /* [in] */ IInterface* obj);

    static Elastos::String GetDefaultTextEncoding(
            /* [in] */ IInterface* obj);

    static Elastos::String GetUserAgentString(
            /* [in] */ IInterface* obj);//method

    static Elastos::String GetAcceptLanguage(
            /* [in] */ IInterface* obj);//method

    static Int32 GetMinimumFontSize(
            /* [in] */ IInterface* obj);

    static Int32 GetMinimumLogicalFontSize(
            /* [in] */ IInterface* obj);

    static Int32 GetDefaultFontSize(
            /* [in] */ IInterface* obj);

    static Int32 GetDefaultFixedFontSize(
            /* [in] */ IInterface* obj);

    static Boolean GetLoadsImagesAutomatically(
            /* [in] */ IInterface* obj);


    static Boolean GetBlockNetworkImage(
            /* [in] */ IInterface* obj);


    static Boolean GetBlockNetworkLoads(
            /* [in] */ IInterface* obj);

    static Boolean GetJavaScriptEnabled(
            /* [in] */ IInterface* obj);

    static Boolean GetAllowUniversalAccessFromFileURLs(
            /* [in] */ IInterface* obj);

    static Boolean GetAllowFileAccessFromFileURLs(
            /* [in] */ IInterface* obj);

    static Int32 GetPluginState(
            /* [in] */ IInterface* obj);//return the value from Enum ordinal


    static Boolean GetAppCacheEnabled(
            /* [in] */ IInterface* obj);

    static Elastos::String GetAppCachePath(
            /* [in] */ IInterface* obj);

    static Int64 GetAppCacheMaxSize(
            /* [in] */ IInterface* obj);

    static Boolean GetWorkersEnabled(
            /* [in] */ IInterface* obj);

    static Boolean GetJavaScriptCanOpenWindowsAutomatically(
            /* [in] */ IInterface* obj);

    static Boolean GetUseWideViewport(
            /* [in] */ IInterface* obj);

    static Boolean GetSupportMultipleWindows(
            /* [in] */ IInterface* obj);

    static Boolean GetShrinksStandaloneImagesToFit(
            /* [in] */ IInterface* obj);

    static Int64 GetMaximumDecodedImageSize(
            /* [in] */ IInterface* obj);

    static Boolean GetPrivateBrowsingEnabled(
            /* [in] */ IInterface* obj);

    static Boolean GetSyntheticLinksEnabled(
            /* [in] */ IInterface* obj);

    static Boolean GetUseDoubleTree(
            /* [in] */ IInterface* obj);

    static Int32 GetPageCacheCapacity(
            /* [in] */ IInterface* obj);

    static Boolean GetDatabaseEnabled(
            /* [in] */ IInterface* obj);

    static Boolean GetDomStorageEnabled(
            /* [in] */ IInterface* obj);

    static Boolean GetGeolocationEnabled(
            /* [in] */ IInterface* obj);

    static Elastos::String GetGeolocationDatabasePath(
            /* [in] */ IInterface* obj);

    static Boolean GetXSSAuditorEnabled(
            /* [in] */ IInterface* obj);

    static Boolean GetLinkPrefetchEnabled(
            /* [in] */ IInterface* obj);

    static Elastos::String GetDatabasePath(
            /* [in] */ IInterface* obj);

    static Boolean GetDatabasePathHasBeenSet(
            /* [in] */ IInterface* obj);

    static Boolean GetAutoFillEnabled(
            /* [in] */ IInterface* obj);

    static AutoPtr<IInterface> GetAutoFillProfile(
            /* [in] */ IInterface* obj);

    static Elastos::String GetAutoFillProfileFullName(
            /* [in] */ IInterface* obj);

    static Elastos::String GetAutoFillProfileEmailAddress(
            /* [in] */ IInterface* obj);

    static Elastos::String GetAutoFillProfileCompanyName(
            /* [in] */ IInterface* obj);

    static Elastos::String GetAutoFillProfileAddressLine1(
            /* [in] */ IInterface* obj);

    static Elastos::String GetAutoFillProfileAddressLine2(
            /* [in] */ IInterface* obj);

    static Elastos::String GetAutoFillProfileCity(
            /* [in] */ IInterface* obj);

    static Elastos::String GetAutoFillProfileState(
            /* [in] */ IInterface* obj);

    static Elastos::String GetAutoFillProfileZipCode(
            /* [in] */ IInterface* obj);

    static Elastos::String GetAutoFillProfileCountry(
            /* [in] */ IInterface* obj);

    static Elastos::String GetAutoFillProfilePhoneNumber(
            /* [in] */ IInterface* obj);

    static Int32 GetOverrideCacheMode(
            /* [in] */ IInterface* obj);

    static Boolean GetPasswordEchoEnabled(
            /* [in] */ IInterface* obj);

    static Boolean GetMediaPlaybackRequiresUserGesture(
            /* [in] */ IInterface* obj);

    //end callback definition
    static ElaWebSettingsCallback* Init_WebSettingsCallback();

private:
    static const String LOGTAG;
    // TODO: Keep this up to date
    static const String PREVIOUS_VERSION;

    // WebView associated with this WebSettings.
    CWebViewClassic* mWebView;
    // BrowserFrame used to access the native frame pointer.
    AutoPtr<BrowserFrame> mBrowserFrame;
    // Flag to prevent multiple SYNC messages at one time.
    Boolean mSyncPending;
    // Custom handler that queues messages until the WebCore thread is active.
    AutoPtr<EventHandler> mEventHandler;

    // Private settings so we don't have to go into native code to
    // retrieve the values. After setXXX, postSync() needs to be called.
    //
    // The default values need to match those in WebSettings.cpp
    // If the defaults change, please also update the JavaDocs so developers
    // know what they are.
    LayoutAlgorithm mLayoutAlgorithm;
    AutoPtr<IContext> mContext;
    Int32 mTextSize;
    String mStandardFontFamily;
    String mFixedFontFamily;
    String mSansSerifFontFamily;
    String mSerifFontFamily;
    String mCursiveFontFamily;
    String mFantasyFontFamily;
    String mDefaultTextEncoding;
    String mUserAgent;
    Boolean mUseDefaultUserAgent;
    String mAcceptLanguage;
    Int32 mMinimumFontSize;
    Int32 mMinimumLogicalFontSize;
    Int32 mDefaultFontSize;
    Int32 mDefaultFixedFontSize;
    Int32 mPageCacheCapacity;
    Boolean mLoadsImagesAutomatically;
    Boolean mBlockNetworkImage;
    Boolean mBlockNetworkLoads;
    Boolean mJavaScriptEnabled;
    Boolean mAllowUniversalAccessFromFileURLs;
    Boolean mAllowFileAccessFromFileURLs;
    Boolean mHardwareAccelSkia;
    Boolean mShowVisualIndicator;
    PluginState mPluginState;
    Boolean mJavaScriptCanOpenWindowsAutomatically;
    Boolean mUseDoubleTree;
    Boolean mUseWideViewport;
    Boolean mSupportMultipleWindows;
    Boolean mShrinksStandaloneImagesToFit;
    Int64 mMaximumDecodedImageSize; // 0 means default
    Boolean mPrivateBrowsingEnabled;
    Boolean mSyntheticLinksEnabled;
    // HTML5 API flags
    Boolean mAppCacheEnabled;
    Boolean mDatabaseEnabled;
    Boolean mDomStorageEnabled;
    Boolean mWorkersEnabled;  // only affects V8.
    Boolean mGeolocationEnabled;
    Boolean mXSSAuditorEnabled;
    Boolean mLinkPrefetchEnabled;
    // HTML5 configuration parameters
    Int64 mAppCacheMaxSize;
    String mAppCachePath;
    String mDatabasePath;
    // The WebCore DatabaseTracker only allows the database path to be set
    // once. Keep track of when the path has been set.
    Boolean mDatabasePathHasBeenSet;
    String mGeolocationDatabasePath;
    // Don't need to synchronize the get/set methods as they
    // are basic types, also none of these values are used in
    // native WebCore code.
    ZoomDensity mDefaultZoom;
    RenderPriority mRenderPriority;
    Int32 mOverrideCacheMode;
    Int32 mDoubleTapZoom;
    Boolean mSaveFormData;
    Boolean mAutoFillEnabled;
    Boolean mSavePassword;
    Boolean mLightTouchEnabled;
    Boolean mNeedInitialFocus;
    Boolean mNavDump;
    Boolean mSupportZoom;
    Boolean mMediaPlaybackRequiresUserGesture;
    Boolean mBuiltInZoomControls;
    Boolean mDisplayZoomControls;
    Boolean mAllowFileAccess;
    Boolean mAllowContentAccess;
    Boolean mLoadWithOverviewMode;
    Boolean mEnableSmoothTransition;
    Boolean mForceUserScalable;
    Boolean mPasswordEchoEnabled;

    AutoPtr<IAutoFillProfile> mAutoFillProfile;

    Boolean mUseWebViewBackgroundForOverscroll;

    // private WebSettings, not accessible by the host activity
    static Int32 sDoubleTapToastCount;

    static const String PREF_FILE;
    static const String DOUBLE_TAP_TOAST_COUNT;

    // User agent strings.
    static const String DESKTOP_USERAGENT;
    static const String IPHONE_USERAGENT;
    static AutoPtr<ILocale> sLocale;
    static Object sLockForLocaleSettings;

    static const String ACCEPT_LANG_FOR_US_LOCALE;
    static struct ElaWebSettingsCallback* sElaWebSettingsCallback;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__WEBSETTINGSCLASSIC_H__
