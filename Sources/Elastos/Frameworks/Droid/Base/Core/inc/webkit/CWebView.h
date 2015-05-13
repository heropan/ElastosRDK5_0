
#ifndef __CWEBVIEW_H__
#define __CWEBVIEW_H__

#include "_CWebView.h"
#include "webkit/WebView.h"
#include "view/ViewMacro.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebView), public WebView
{
public:
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

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI SetHorizontalScrollbarOverlay(
        /* [in] */ Boolean overlay);

    CARAPI SetVerticalScrollbarOverlay(
        /* [in] */ Boolean overlay);

    CARAPI OverlayHorizontalScrollbar(
        /* [out] */ Boolean* result);

    CARAPI OverlayVerticalScrollbar(
        /* [out] */ Boolean* result);

    CARAPI GetVisibleTitleHeight(
        /* [out] */ Int32* height);

    CARAPI GetCertificate(
        /* [out] */ ISslCertificate** cer);

    CARAPI SetCertificate(
        /* [in] */ ISslCertificate* certificate);

    CARAPI SavePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    CARAPI SetHttpAuthUsernamePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& realm,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    CARAPI GetHttpAuthUsernamePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& realm,
        /* [out, callee] */ ArrayOf<String>** up);

    CARAPI Destroy();

    CARAPI SetNetworkAvailable(
        /* [in] */ Boolean networkUp);

    CARAPI SaveState(
        /* [in] */ IBundle* outState,
        /* [out] */ IWebBackForwardList** wfl);

    CARAPI SavePicture(
        /* [in] */ IBundle* b,
        /* [in] */ IFile* dest,
        /* [out] */ Boolean* result);

    CARAPI RestorePicture(
        /* [in] */ IBundle* b,
        /* [in] */ IFile * src,
        /* [out] */ Boolean* result);

    CARAPI RestoreState(
        /* [in] */ IBundle* inState,
        /* [out] */ IWebBackForwardList** wfl);

    CARAPI LoadUrlEx(
        /* [in] */ const String& url,
        /* [in] */ IMap* additionalHttpHeaders);

    CARAPI LoadUrl(
        /* [in] */ const String& url);

    CARAPI PostUrl(
        /* [in] */ const String& url,
        /* [in] */ ArrayOf<Byte>* postData);

    CARAPI LoadData(
        /* [in] */ const String& data,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding);

    CARAPI LoadDataWithBaseURL(
        /* [in] */ const String& baseUrl,
        /* [in] */ const String& data,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding,
        /* [in] */ const String& historyUrl);

    CARAPI SaveWebArchive(
        /* [in] */ const String& filename);

    CARAPI SaveWebArchiveEx(
        /* [in] */ const String& basename,
        /* [in] */ Boolean autoname,
        /* [in] */ IValueCallback* callBack);

    CARAPI StopLoading();

    CARAPI Reload();

    CARAPI CanGoBack(
        /* [out] */ Boolean* result);

    CARAPI GoBack();

    CARAPI CanGoForward(
        /* [out] */ Boolean* result);

    CARAPI GoForward();

    CARAPI CanGoBackOrForward(
        /* [in] */ Int32 steps,
        /* [out] */ Boolean* result);

    CARAPI GoBackOrForward(
        /* [in] */ Int32 steps);

    CARAPI IsPrivateBrowsingEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI PageUp(
        /* [in] */ Boolean top,
        /* [out] */ Boolean* result);

    CARAPI PageDown(
        /* [in] */ Boolean bottom,
        /* [out] */ Boolean* result);

    CARAPI ClearView();

    CARAPI CapturePicture(
        /* [out] */ IPicture** pic);

    CARAPI GetScale(
        /* [out] */ Float* scale);

    CARAPI SetInitialScale(
        /* [in] */ Int32 scaleInPercent);

    CARAPI InvokeZoomPicker();

    CARAPI GetHitTestResult(
        /* [out] */ IWebViewHitTestResult** tr);

    CARAPI RequestFocusNodeHref(
        /* [in] */ IMessage* hrefMsg);

    CARAPI RequestImageRef(
        /* [in] */ IMessage* msg);

    CARAPI GetUrl(
        /* [out] */ String* url);

    CARAPI GetOriginalUrl(
        /* [out] */ String* url);

    CARAPI GetTitle(
        /* [out] */ String* title);

    CARAPI GetFavicon(
        /* [out] */ IBitmap** bitmap);

    CARAPI GetTouchIconUrl(
        /* [out] */ String* url);

    CARAPI GetProgress(
        /* [out] */ Int32* progress);

    CARAPI GetContentHeight(
        /* [out] */ Int32* height);

    CARAPI GetContentWidth(
        /* [out] */ Int32* width);

    CARAPI PauseTimers();

    CARAPI ResumeTimers();

    CARAPI OnPause();

    CARAPI OnResume();

    CARAPI IsPaused(
        /* [out] */ Boolean* result);

    CARAPI FreeMemory();

    CARAPI ClearCache(
        /* [in] */ Boolean includeDiskFiles);

    CARAPI ClearFormData();

    CARAPI ClearHistory();

    CARAPI ClearSslPreferences();

    CARAPI CopyBackForwardList(
        /* [out] */ IWebBackForwardList** wfl);

    CARAPI SetFindListener(
        /* [in] */ IWebViewFindListener* listener);

    CARAPI FindNext(
        /* [in] */ Boolean forward);

    CARAPI FindAll(
        /* [in] */ const String& find,
        /* [out] */ Int32* all);

    CARAPI FindAllAsync(
        /* [in] */ const String& find);

    CARAPI ShowFindDialog(
        /* [in] */ const String& text,
        /* [in] */ Boolean showIme,
        /* [out] */ Boolean* result);

    CARAPI ClearMatches();

    CARAPI DocumentHasImages(
        /* [in] */ IMessage* response);

    CARAPI SetWebViewClient(
        /* [in] */ IWebViewClient* client);

    CARAPI SetDownloadListener(
        /* [in] */ IDownloadListener* listener);

    CARAPI SetWebChromeClient(
        /* [in] */ IWebChromeClient* client);

    CARAPI SetPictureListener(
        /* [in] */ IWebViewPictureListener* listener);

    CARAPI AddJavascriptInterface(
        /* [in] */ IInterface* object,
        /* [in] */ const String& name);

    CARAPI RemoveJavascriptInterface(
        /* [in] */ const String& name);

    CARAPI GetSettings(
        /* [out] */ IWebSettings** webSetting);

    CARAPI RefreshPlugins(
        /* [in] */ Boolean reloadOpenPages);

    CARAPI EmulateShiftHeld();

    CARAPI SetMapTrackballToArrowKeys(
        /* [in] */ Boolean setMap);

    CARAPI FlingScroll(
        /* [in] */ Int32 vx,
        /* [in] */ Int32 vy);

    CARAPI GetZoomControls(
        /* [out] */ IView** view);

    CARAPI CanZoomIn(
        /* [out] */ Boolean* result);

    CARAPI CanZoomOut(
        /* [out] */ Boolean* result);

    CARAPI ZoomIn(
        /* [out] */ Boolean* result);

    CARAPI ZoomOut(
        /* [out] */ Boolean* result);

    CARAPI DebugDump();

    CARAPI GetWebViewProvider(
        /* [out] */ IWebViewProvider** provider);

    CARAPI OnGlobalFocusChanged(
        /* [in] */ IView* oldFocus,
        /* [in] */ IView* newFocus);

    CARAPI OnChildViewAdded(
        /* [in] */ IView* parent,
        /* [in] */ IView* child);

    CARAPI OnChildViewRemoved(
        /* [in] */ IView* parent,
        /* [in] */ IView* child);

    static void PostInvalidate(
        /* [in] */ IInterface* obj);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __CWEBVIEW_H__
