
#include "webkit/CWebView.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CWebView::constructor(
    /* [in] */ IContext * context)
{
    return WebView::Init(context);
}

ECode CWebView::constructor(
    /* [in] */ IContext * context,
    /* [in] */ IAttributeSet * attrs)
{
    return WebView::Init(context, attrs);
}

ECode CWebView::constructor(
    /* [in] */ IContext * context,
    /* [in] */ IAttributeSet * attrs,
    /* [in] */ Int32 defStyle)
{
    return WebView::Init(context, attrs, defStyle);
}

ECode CWebView::constructor(
    /* [in] */ IContext * context,
    /* [in] */ IAttributeSet * attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Boolean privateBrowsing)
{
    return WebView::Init(context, attrs, defStyle, privateBrowsing);
}

PInterface CWebView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CWebView::Probe(riid);
}

IVIEW_METHODS_IMPL(CWebView, WebView)
IVIEWGROUP_METHODS_IMPL(CWebView, WebView)
IVIEWPARENT_METHODS_IMPL(CWebView, WebView)
IVIEWMANAGER_METHODS_IMPL(CWebView, WebView)
IDRAWABLECALLBACK_METHODS_IMPL(CWebView, WebView)
IKEYEVENTCALLBACK_METHODS_IMPL(CWebView, WebView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CWebView, WebView)

ECode CWebView::SetHorizontalScrollbarOverlay(
    /* [in] */ Boolean overlay)
{
    return WebView::SetHorizontalScrollbarOverlay(overlay);
}

ECode CWebView::SetVerticalScrollbarOverlay(
    /* [in] */ Boolean overlay)
{
    return WebView::SetVerticalScrollbarOverlay(overlay);
}

ECode CWebView::OverlayHorizontalScrollbar(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebView::OverlayHorizontalScrollbar();
    return NOERROR;
}

ECode CWebView::OverlayVerticalScrollbar(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebView::OverlayVerticalScrollbar();
    return NOERROR;
}

ECode CWebView::GetVisibleTitleHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = WebView::GetVisibleTitleHeight();
    return NOERROR;
}

ECode CWebView::GetCertificate(
    /* [out] */ ISslCertificate** cer)
{
    VALIDATE_NOT_NULL(cer);
    AutoPtr<ISslCertificate> c = WebView::GetCertificate();
    *cer = c;
    INTERFACE_ADDREF(*cer);
    return NOERROR;
}

ECode CWebView::SetCertificate(
    /* [in] */ ISslCertificate* certificate)
{
    return WebView::SetCertificate(certificate);
}

ECode CWebView::SavePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    return WebView::SavePassword(host, username, password);
}

ECode CWebView::SetHttpAuthUsernamePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    return WebView::SetHttpAuthUsernamePassword(host, realm, username, password);
}

ECode CWebView::GetHttpAuthUsernamePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [out, callee] */ ArrayOf<String>** up)
{
    VALIDATE_NOT_NULL(up);
    AutoPtr< ArrayOf<String> > _up = WebView::GetHttpAuthUsernamePassword(host, realm);
    *up = _up.Get();
    INTERFACE_ADDREF(*up);
    return NOERROR;
}

ECode CWebView::Destroy()
{
    return WebView::Destroy();
}

ECode CWebView::SetNetworkAvailable(
    /* [in] */ Boolean networkUp)
{
    return WebView::SetNetworkAvailable(networkUp);
}

ECode CWebView::SaveState(
    /* [in] */ IBundle* outState,
    /* [out] */ IWebBackForwardList** wfl)
{
    VALIDATE_NOT_NULL(wfl);
    AutoPtr<IWebBackForwardList> _wfl = WebView::SaveState(outState);
    *wfl = _wfl.Get();
    INTERFACE_ADDREF(*wfl);
    return NOERROR;
}

ECode CWebView::SavePicture(
    /* [in] */ IBundle* b,
    /* [in] */ IFile* dest,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebView::SavePicture(b, dest);
    return NOERROR;
}

ECode CWebView::RestorePicture(
    /* [in] */ IBundle* b,
    /* [in] */ IFile* src,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebView::RestorePicture(b, src);
    return NOERROR;
}

ECode CWebView::RestoreState(
    /* [in] */ IBundle* inState,
    /* [out] */ IWebBackForwardList ** wfl)
{
    VALIDATE_NOT_NULL(wfl);
    AutoPtr<IWebBackForwardList> _wfl = WebView::RestoreState(inState);
    *wfl = _wfl.Get();
    INTERFACE_ADDREF(*wfl);
    return NOERROR;
}

ECode CWebView::LoadUrlEx(
    /* [in] */ const String& url,
    /* [in] */ IMap* additionalHttpHeaders)
{
    VALIDATE_NOT_NULL(additionalHttpHeaders);
    return WebView::LoadUrl(url, additionalHttpHeaders);
}

ECode CWebView::LoadUrl(
    /* [in] */ const String& url)
{
    return WebView::LoadUrl(url);
}

ECode CWebView::PostUrl(
    /* [in] */ const String& url,
    /* [in] */ ArrayOf<Byte>* postData)
{
    return WebView::PostUrl(url, postData);
}

ECode CWebView::LoadData(
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding)
{
    return WebView::LoadData(data, mimeType, encoding);
}

ECode CWebView::LoadDataWithBaseURL(
    /* [in] */ const String& baseUrl,
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding,
    /* [in] */ const String& historyUrl)
{
    return WebView::LoadDataWithBaseURL(baseUrl, data, mimeType, encoding, historyUrl);
}

ECode CWebView::SaveWebArchive(
    /* [in] */ const String& filename)
{
    return WebView::SaveWebArchive(filename);
}

ECode CWebView::SaveWebArchiveEx(
    /* [in] */ const String& basename,
    /* [in] */ Boolean autoname,
    /* [in] */ IValueCallback* callBack)
{
    return WebView::SaveWebArchive(basename, autoname, callBack);
}

ECode CWebView::StopLoading()
{
    return WebView::StopLoading();
}

ECode CWebView::Reload()
{
    return WebView::Reload();
}

ECode CWebView::CanGoBack(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebView::CanGoBack();
    return NOERROR;
}

ECode CWebView::GoBack()
{
    return WebView::GoBack();
}

ECode CWebView::CanGoForward(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebView::CanGoForward();
    return NOERROR;
}

ECode CWebView::GoForward()
{
    return WebView::GoForward();
}

ECode CWebView::CanGoBackOrForward(
    /* [in] */ Int32 steps,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebView::CanGoBackOrForward(steps);
    return NOERROR;
}

ECode CWebView::GoBackOrForward(
    /* [in] */ Int32 steps)
{
    WebView::GoBackOrForward(steps);
    return NOERROR;
}

ECode CWebView::IsPrivateBrowsingEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = WebView::IsPrivateBrowsingEnabled();
    return NOERROR;
}

ECode CWebView::PageUp(
    /* [in] */ Boolean top,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebView::PageUp(top);
    return NOERROR;
}

ECode CWebView::PageDown(
    /* [in] */ Boolean bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebView::PageDown(bottom);
    return NOERROR;
}

ECode CWebView::ClearView()
{
    return WebView::ClearView();
}

ECode CWebView::CapturePicture(
    /* [out] */ IPicture** pic)
{
    VALIDATE_NOT_NULL(pic);
    AutoPtr<IPicture> p = WebView::CapturePicture();
    *pic = p.Get();
    INTERFACE_ADDREF(*pic);
    return NOERROR;
}

ECode CWebView::GetScale(
    /* [out] */ Float* scale)
{
    VALIDATE_NOT_NULL(scale);
    *scale = WebView::GetScale();
    return NOERROR;
}

ECode CWebView::SetInitialScale(
    /* [in] */ Int32 scaleInPercent)
{
    return WebView::SetInitialScale(scaleInPercent);
}

ECode CWebView::InvokeZoomPicker()
{
    return WebView::InvokeZoomPicker();
}

ECode CWebView::GetHitTestResult(
    /* [out] */ IWebViewHitTestResult** tr)
{
    VALIDATE_NOT_NULL(tr);
    AutoPtr<IWebViewHitTestResult> _tr = WebView::GetHitTestResult();
    *tr = _tr.Get();
    INTERFACE_ADDREF(*tr);
    return NOERROR;
}

ECode CWebView::RequestFocusNodeHref(
    /* [in] */ IMessage* hrefMsg)
{
    return WebView::RequestFocusNodeHref(hrefMsg);
}

ECode CWebView::RequestImageRef(
    /* [in] */ IMessage* msg)
{
    return WebView::RequestImageRef(msg);
}

ECode CWebView::GetUrl(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    *url = WebView::GetUrl();
    return NOERROR;
}

ECode CWebView::GetOriginalUrl(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    *url = WebView::GetOriginalUrl();
    return NOERROR;
}

ECode CWebView::GetTitle(
    /* [out] */ String* title)
{
    VALIDATE_NOT_NULL(title);
    *title = WebView::GetTitle();
    return NOERROR;
}

ECode CWebView::GetFavicon(
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    AutoPtr<IBitmap> _bitmap = WebView::GetFavicon();
    *bitmap = _bitmap.Get();
    INTERFACE_ADDREF(*bitmap);
    return NOERROR;
}

ECode CWebView::GetTouchIconUrl(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    *url = WebView::GetTouchIconUrl();
    return NOERROR;
}

ECode CWebView::GetProgress(
    /* [out] */ Int32* progress)
{
    VALIDATE_NOT_NULL(progress);
    *progress = WebView::GetProgress();
    return NOERROR;
}

ECode CWebView::GetContentHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = WebView::GetContentHeight();
    return NOERROR;
}

ECode CWebView::GetContentWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = WebView::GetContentWidth();
    return NOERROR;
}

ECode CWebView::PauseTimers()
{
    return WebView::PauseTimers();
}

ECode CWebView::ResumeTimers()
{
    return WebView::ResumeTimers();
}

ECode CWebView::OnPause()
{
    return WebView::OnPause();
}

ECode CWebView::OnResume()
{
    return WebView::OnResume();
}

ECode CWebView::IsPaused(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebView::IsPaused();
    return NOERROR;
}

ECode CWebView::FreeMemory()
{
    return WebView::FreeMemory();
}

ECode CWebView::ClearCache(
    /* [in] */ Boolean includeDiskFiles)
{
    return WebView::ClearCache(includeDiskFiles);
}

ECode CWebView::ClearFormData()
{
    return WebView::ClearFormData();
}

ECode CWebView::ClearHistory()
{
    return WebView::ClearHistory();
}

ECode CWebView::ClearSslPreferences()
{
    return WebView::ClearSslPreferences();
}

ECode CWebView::CopyBackForwardList(
    /* [out] */ IWebBackForwardList** wfl)
{
    VALIDATE_NOT_NULL(wfl);
    AutoPtr<IWebBackForwardList> _wfl = WebView::CopyBackForwardList();
    *wfl = _wfl.Get();
    INTERFACE_ADDREF(*wfl);
    return NOERROR;
}

ECode CWebView::SetFindListener(
    /* [in] */ IWebViewFindListener* listener)
{
    return WebView::SetFindListener(listener);
}

ECode CWebView::FindNext(
    /* [in] */ Boolean forward)
{
    WebView::FindNext(forward);
    return NOERROR;
}

ECode CWebView::FindAll(
    /* [in] */ const String& find,
    /* [out] */ Int32* all)
{
    VALIDATE_NOT_NULL(all);
    *all = WebView::FindAll(find);
    return NOERROR;
}

ECode CWebView::FindAllAsync(
    /* [in] */ const String& find)
{
    return WebView::FindAllAsync(find);
}

ECode CWebView::ShowFindDialog(
    /* [in] */ const String& text,
    /* [in] */ Boolean showIme,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebView::ShowFindDialog(text, showIme);
    return NOERROR;
}

ECode CWebView::ClearMatches()
{
    return WebView::ClearMatches();
}

ECode CWebView::DocumentHasImages(
    /* [in] */ IMessage* response)
{
    return WebView::DocumentHasImages(response);
}

ECode CWebView::SetWebViewClient(
    /* [in] */ IWebViewClient* client)
{
    return WebView::SetWebViewClient(client);
}

ECode CWebView::SetDownloadListener(
    /* [in] */ IDownloadListener* listener)
{
    return WebView::SetDownloadListener(listener);
}

ECode CWebView::SetWebChromeClient(
    /* [in] */ IWebChromeClient* client)
{
    return WebView::SetWebChromeClient(client);
}

ECode CWebView::SetPictureListener(
    /* [in] */ IWebViewPictureListener* listener)
{
    return WebView::SetPictureListener(listener);
}

ECode CWebView::AddJavascriptInterface(
    /* [in] */ IInterface* object,
    /* [in] */ const String& name)
{
    return WebView::AddJavascriptInterface(object, name);
}

ECode CWebView::RemoveJavascriptInterface(
    /* [in] */ const String& name)
{
    return WebView::RemoveJavascriptInterface(name);
}

ECode CWebView::GetSettings(
    /* [out] */ IWebSettings** webSetting)
{
    VALIDATE_NOT_NULL(webSetting);
    AutoPtr<IWebSettings> _webSetting = WebView::GetSettings();
    *webSetting = _webSetting.Get();
    INTERFACE_ADDREF(*webSetting);
    return NOERROR;
}

ECode CWebView::RefreshPlugins(
    /* [in] */ Boolean reloadOpenPages)
{
    return WebView::RefreshPlugins(reloadOpenPages);
}

ECode CWebView::EmulateShiftHeld()
{
    return WebView::EmulateShiftHeld();
}

ECode CWebView::SetMapTrackballToArrowKeys(
    /* [in] */ Boolean setMap)
{
    return WebView::SetMapTrackballToArrowKeys(setMap);
}

ECode CWebView::FlingScroll(
    /* [in] */ Int32 vx,
    /* [in] */ Int32 vy)
{
    return WebView::FlingScroll(vx, vy);
}

ECode CWebView::GetZoomControls(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> _view = WebView::GetZoomControls();
    *view = _view.Get();
    INTERFACE_ADDREF(*view);
    return NOERROR;
}

ECode CWebView::CanZoomIn(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebView::CanZoomIn();
    return NOERROR;
}

ECode CWebView::CanZoomOut(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebView::CanZoomOut();
    return NOERROR;
}

ECode CWebView::ZoomIn(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebView::ZoomIn();
    return NOERROR;
}

ECode CWebView::ZoomOut(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebView::ZoomOut();
    return NOERROR;
}

ECode CWebView::DebugDump()
{
    return WebView::DebugDump();
}

ECode CWebView::GetWebViewProvider(
    /* [out] */ IWebViewProvider** provider)
{
    VALIDATE_NOT_NULL(provider);
    AutoPtr<IWebViewProvider> _provider = WebView::GetWebViewProvider();
    *provider = _provider.Get();
    INTERFACE_ADDREF(*provider);
    return NOERROR;
}

ECode CWebView::OnGlobalFocusChanged(
    /* [in] */ IView* oldFocus,
    /* [in] */ IView* newFocus)
{
    return WebView::OnGlobalFocusChanged(oldFocus, newFocus);
}

ECode CWebView::OnChildViewAdded(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    return WebView::OnChildViewAdded(parent, child);
}

ECode CWebView::OnChildViewRemoved(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    return WebView::OnChildViewRemoved(parent, child);
}
void CWebView::PostInvalidate(
    /* [in] */ IInterface* obj)
{
    ((CWebView*)IWebView::Probe(obj))->PostInvalidate();
}

} // Webkit
} // Droid
} // Elastos
