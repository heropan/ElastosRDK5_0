
#include "webkit/CWebChromeClient.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CWebChromeClient::OnProgressChanged(
    /* [in] */ IWebView* view,
    /* [in] */ Int32 newProgress)
{
    return WebChromeClient::OnProgressChanged(view, newProgress);
}

ECode CWebChromeClient::OnReceivedTitle(
    /* [in] */ IWebView* view,
    /* [in] */ const String& title)
{
    return WebChromeClient::OnReceivedTitle(view, title);
}

ECode CWebChromeClient::OnReceivedIcon(
    /* [in] */ IWebView* view,
    /* [in] */ IBitmap* icon)
{
    return WebChromeClient::OnReceivedIcon(view, icon);
}

ECode CWebChromeClient::OnReceivedTouchIconUrl(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [in] */ Boolean precomposed)
{
    return WebChromeClient::OnReceivedTouchIconUrl(view, url, precomposed);
}

ECode CWebChromeClient::OnShowCustomView(
    /* [in] */ IView* view,
    /* [in] */ IWebChromeClientCustomViewCallback* callBack)
{
    return WebChromeClient::OnShowCustomView(view, callBack);
}

ECode CWebChromeClient::OnShowCustomViewEx(
    /* [in] */ IView* view,
    /* [in] */ Int32 requestedOrientation,
    /* [in] */ IWebChromeClientCustomViewCallback* callBack)
{
    return WebChromeClient::OnShowCustomView(view, requestedOrientation, callBack);
}

ECode CWebChromeClient::OnHideCustomView()
{
    return WebChromeClient::OnHideCustomView();
}

ECode CWebChromeClient::OnCreateWindow(
    /* [in] */ IWebView* view,
    /* [in] */ Boolean isDialog,
    /* [in] */ Boolean isUserGesture,
    /* [in] */ IMessage* resultMsg,
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

ECode CWebChromeClient::OnRequestFocus(
    /* [in] */ IWebView* view)
{
    return WebChromeClient::OnRequestFocus(view);
}

ECode CWebChromeClient::OnCloseWindow(
    /* [in] */ IWebView* window)
{
    return WebChromeClient::OnCloseWindow(window);
}

ECode CWebChromeClient::OnJsAlert(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [in] */ IJsResult* result,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    *flag = WebChromeClient::OnJsAlert(view, url, message, result);
    return NOERROR;
}

ECode CWebChromeClient::OnJsConfirm(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [in] */ IJsResult* result,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    *flag = WebChromeClient::OnJsConfirm(view, url, message, result);
    return NOERROR;
}

ECode CWebChromeClient::OnJsPrompt(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [in] */ const String& defaultValue,
    /* [in] */ IJsPromptResult * result,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    *flag = WebChromeClient::OnJsPrompt(view, url, message, defaultValue, result);
    return NOERROR;
}

ECode CWebChromeClient::OnJsBeforeUnload(
    /* [in] */ IWebView* view,
    /* [in] */ const String& url,
    /* [in] */ const String& message,
    /* [in] */ IJsResult* result,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    *flag = WebChromeClient::OnJsBeforeUnload(view, url, message, result);
    return NOERROR;
}

ECode CWebChromeClient::OnExceededDatabaseQuota(
    /* [in] */ const String& url,
    /* [in] */ const String& databaseIdentifier,
    /* [in] */ Int64 quota,
    /* [in] */ Int64 estimatedDatabaseSize,
    /* [in] */ Int64 totalQuota,
    /* [in] */ IWebStorageQuotaUpdater* quotaUpdater)
{
    return WebChromeClient::OnExceededDatabaseQuota(url, databaseIdentifier, quota, estimatedDatabaseSize,
            totalQuota, quotaUpdater);
}

ECode CWebChromeClient::OnReachedMaxAppCacheSize(
    /* [in] */ Int64 requiredStorage,
    /* [in] */ Int64 quota,
    /* [in] */ IWebStorageQuotaUpdater* quotaUpdater)
{
    return WebChromeClient::OnReachedMaxAppCacheSize(requiredStorage, quota, quotaUpdater);
}

ECode CWebChromeClient::OnGeolocationPermissionsShowPrompt(
    /* [in] */ const String& origin,
    /* [in] */ IGeolocationPermissionsCallback* callBack)
{
    return WebChromeClient::OnGeolocationPermissionsShowPrompt(origin, callBack);
}

ECode CWebChromeClient::OnGeolocationPermissionsHidePrompt()
{
    return WebChromeClient::OnGeolocationPermissionsHidePrompt();
}

ECode CWebChromeClient::OnJsTimeout(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebChromeClient::OnJsTimeout();
    return NOERROR;
}

ECode CWebChromeClient::OnConsoleMessage(
    /* [in] */ const String& message,
    /* [in] */ Int32 lineNumber,
    /* [in] */ const String& sourceID)
{
    return WebChromeClient::OnConsoleMessage(message, lineNumber, sourceID);
}

ECode CWebChromeClient::OnConsoleMessageEx(
    /* [in] */ IConsoleMessage* consoleMessage,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = WebChromeClient::OnConsoleMessage(consoleMessage);
    return NOERROR;
}

ECode CWebChromeClient::GetDefaultVideoPoster(
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    AutoPtr<IBitmap> _bitmap = WebChromeClient::GetDefaultVideoPoster();
    *bitmap = _bitmap.Get();
    INTERFACE_ADDREF(*bitmap);
    return NOERROR;
}

ECode CWebChromeClient::GetVideoLoadingProgressView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> _view = WebChromeClient::GetVideoLoadingProgressView();
    *view = _view.Get();
    INTERFACE_ADDREF(*view);
    return NOERROR;
}

ECode CWebChromeClient::GetVisitedHistory(
    /* [in] */ IValueCallback* callBack)
{
    return WebChromeClient::GetVisitedHistory(callBack);
}

ECode CWebChromeClient::OpenFileChooser(
    /* [in] */ IValueCallback* uploadFile,
    /* [in] */ const String& acceptType,
    /* [in] */ const String& capture)
{
    return WebChromeClient::OpenFileChooser(uploadFile, acceptType, capture);
}

ECode CWebChromeClient::SetupAutoFill(
    /* [in] */ IMessage* msg)
{
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
