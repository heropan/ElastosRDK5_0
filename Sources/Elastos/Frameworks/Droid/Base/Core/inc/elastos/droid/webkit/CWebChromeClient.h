
#ifndef __ELASTOS_DROID_WEBKIT_CWEBCHROMECLIENT_H__
#define __ELASTOS_DROID_WEBKIT_CWEBCHROMECLIENT_H__

#include "_Elastos_Droid_Webkit_CWebChromeClient.h"
#include "elastos/droid/webkit/WebChromeClient.h"

using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebChromeClient), public WebChromeClient
{
public:
    CARAPI OnProgressChanged(
        /* [in] */ IWebView* view,
        /* [in] */ Int32 newProgress);

    CARAPI OnReceivedTitle(
        /* [in] */ IWebView* view,
        /* [in] */ const String& title);

    CARAPI OnReceivedIcon(
        /* [in] */ IWebView* view,
        /* [in] */ IBitmap* icon);

    CARAPI OnReceivedTouchIconUrl(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [in] */ Boolean precomposed);

    CARAPI OnShowCustomView(
        /* [in] */ IView* view,
        /* [in] */ IWebChromeClientCustomViewCallback* callBack);

    CARAPI OnShowCustomView(
        /* [in] */ IView* view,
        /* [in] */ Int32 requestedOrientation,
        /* [in] */ IWebChromeClientCustomViewCallback* callBack);

    CARAPI OnHideCustomView();

    CARAPI OnCreateWindow(
        /* [in] */ IWebView* view,
        /* [in] */ Boolean isDialog,
        /* [in] */ Boolean isUserGesture,
        /* [in] */ IMessage* resultMsg,
        /* [out] */ Boolean* result);

    CARAPI OnRequestFocus(
        /* [in] */ IWebView* view);

    CARAPI OnCloseWindow(
        /* [in] */ IWebView* window);

    CARAPI OnJsAlert(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [in] */ IJsResult* result,
        /* [out] */ Boolean* flag);

    CARAPI OnJsConfirm(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [in] */ IJsResult* result,
        /* [out] */ Boolean* flag);

    CARAPI OnJsPrompt(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [in] */ const String& defaultValue,
        /* [in] */ IJsPromptResult* result,
        /* [out] */ Boolean* flag);

    CARAPI OnJsBeforeUnload(
        /* [in] */ IWebView* view,
        /* [in] */ const String& url,
        /* [in] */ const String& message,
        /* [in] */ IJsResult* result,
        /* [out] */ Boolean* flag);

    CARAPI OnExceededDatabaseQuota(
        /* [in] */ const String& url,
        /* [in] */ const String& databaseIdentifier,
        /* [in] */ Int64 quota,
        /* [in] */ Int64 estimatedDatabaseSize,
        /* [in] */ Int64 totalQuota,
        /* [in] */ IWebStorageQuotaUpdater* quotaUpdater);

    CARAPI OnReachedMaxAppCacheSize(
        /* [in] */ Int64 requiredStorage,
        /* [in] */ Int64 quota,
        /* [in] */ IWebStorageQuotaUpdater* quotaUpdater);

    CARAPI OnGeolocationPermissionsShowPrompt(
        /* [in] */ const String& origin,
        /* [in] */ IGeolocationPermissionsCallback* callBack);

    CARAPI OnGeolocationPermissionsHidePrompt();

    CARAPI OnJsTimeout(
        /* [out] */ Boolean* result);

    CARAPI OnConsoleMessage(
        /* [in] */ const String& message,
        /* [in] */ Int32 lineNumber,
        /* [in] */ const String& sourceID);

    CARAPI OnConsoleMessage(
        /* [in] */ IConsoleMessage* consoleMessage,
        /* [out] */ Boolean* result);

    CARAPI GetDefaultVideoPoster(
        /* [out] */ IBitmap** bitmap);

    CARAPI GetVideoLoadingProgressView(
        /* [out] */ IView** view);

    CARAPI GetVisitedHistory(
        /* [in] */ IValueCallback* callBack);

    CARAPI OpenFileChooser(
        /* [in] */ IValueCallback* uploadFile,
        /* [in] */ const String& acceptType,
        /* [in] */ const String& capture);

    CARAPI SetupAutoFill(
        /* [in] */ IMessage* msg);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CWEBCHROMECLIENT_H__
