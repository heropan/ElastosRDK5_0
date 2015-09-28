
#include "webkit/native/content/browser/ContentViewCore.h"
#include "webkit/native/content/browser/DownloadController.h"
#include "webkit/native/content/browser/DownloadInfo.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

const String DownloadController::LOGTAG("DownloadController");
AutoPtr<DownloadController> DownloadController::sInstance;
AutoPtr<DownloadController::DownloadNotificationService> DownloadController::sDownloadNotificationService;

DownloadController::DownloadController()
{
    NativeInit();
}

//@CalledByNative
AutoPtr<DownloadController> DownloadController::GetInstance()
{
    if (sInstance == NULL) {
        sInstance = new DownloadController();
    }

    return sInstance;
}

AutoPtr<ContentViewDownloadDelegate> DownloadController::DownloadDelegateFromView(
    /* [in] */ ContentViewCore* view)
{
    return view->GetDownloadDelegate();
}

void DownloadController::SetDownloadNotificationService(
    /* [in] */ DownloadController::DownloadNotificationService* service)
{
    sDownloadNotificationService = service;
}

/**
 * Notifies the download delegate of a new GET download and passes all the information
 * needed to download the file.
 *
 * The download delegate is expected to handle the download.
 */
//@CalledByNative
void DownloadController::NewHttpGetDownload(
    /* [in] */ ContentViewCore* view,
    /* [in] */ const String& url,
    /* [in] */ const String& userAgent,
    /* [in] */ const String& contentDisposition,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& cookie,
    /* [in] */ const String& referer,
    /* [in] */ const String& filename,
    /* [in] */ Int64 contentLength)
{
    AutoPtr<ContentViewDownloadDelegate> downloadDelegate = DownloadDelegateFromView(view);

    if (downloadDelegate != NULL) {
        AutoPtr<DownloadInfo::Builder> builder = new DownloadInfo::Builder();
        builder->SetUrl(url);
        builder->SetUserAgent(userAgent);
        builder->SetContentDisposition(contentDisposition);
        builder->SetMimeType(mimeType);
        builder->SetCookie(cookie);
        builder->SetReferer(referer);
        builder->SetFileName(filename);
        builder->SetContentLength(contentLength);
        builder->SetIsGETRequest(TRUE);
        AutoPtr<DownloadInfo> downloadInfo = builder->Build();
        downloadDelegate->RequestHttpGetDownload(downloadInfo);
    }
}

/**
 * Notifies the download delegate that a new download has started. This can
 * be either a POST download or a GET download with authentication.
 * @param view ContentViewCore associated with the download item.
 * @param filename File name of the downloaded file.
 * @param mimeType Mime of the downloaded item.
 */
//@CalledByNative
void DownloadController::OnDownloadStarted(
    /* [in] */ ContentViewCore* view,
    /* [in] */ const String& filename,
    /* [in] */ const String& mimeType)
{
    AutoPtr<ContentViewDownloadDelegate> downloadDelegate = DownloadDelegateFromView(view);

    if (downloadDelegate != NULL) {
        downloadDelegate->OnDownloadStarted(filename, mimeType);
    }
}

/**
 * Notifies the download delegate that a download completed and passes along info about the
 * download. This can be either a POST download or a GET download with authentication.
 */
//@CalledByNative
void DownloadController::OnDownloadCompleted(
    /* [in] */ IContext* context,
    /* [in] */ const String& url,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& filename,
    /* [in] */ const String& path,
    /* [in] */ Int64 contentLength,
    /* [in] */ Boolean successful,
    /* [in] */ Int32 downloadId)
{
    if (sDownloadNotificationService != NULL) {
        AutoPtr<DownloadInfo::Builder> builder = new DownloadInfo::Builder();
        builder->SetUrl(url);
        builder->SetMimeType(mimeType);
        builder->SetFileName(filename);
        builder->SetFilePath(path);
        builder->SetContentLength(contentLength);
        builder->SetIsSuccessful(successful);
        builder->SetDescription(filename);
        builder->SetDownloadId(downloadId);
        builder->SetHasDownloadId(TRUE);
        AutoPtr<DownloadInfo> downloadInfo = builder->Build();
        sDownloadNotificationService->OnDownloadCompleted(downloadInfo);
    }
}

/**
 * Notifies the download delegate about progress of a download. Downloads that use Chrome
 * network stack use custom notification to display the progress of downloads.
 */
//@CalledByNative
void DownloadController::OnDownloadUpdated(
    /* [in] */ IContext* context,
    /* [in] */ const String& url,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& filename,
    /* [in] */ const String& path,
    /* [in] */ Int64 contentLength,
    /* [in] */ Boolean successful,
    /* [in] */ Int32 downloadId,
    /* [in] */ Int32 percentCompleted,
    /* [in] */ Int64 timeRemainingInMs)
{
    if (sDownloadNotificationService != NULL) {
        AutoPtr<DownloadInfo::Builder> builder = new DownloadInfo::Builder();
        builder->SetUrl(url);
        builder->SetMimeType(mimeType);
        builder->SetFileName(filename);
        builder->SetFilePath(path);
        builder->SetContentLength(contentLength);
        builder->SetIsSuccessful(successful);
        builder->SetDescription(filename);
        builder->SetDownloadId(downloadId);
        builder->SetHasDownloadId(TRUE);
        builder->SetPercentCompleted(percentCompleted);
        builder->SetTimeRemainingInMillis(timeRemainingInMs);
        AutoPtr<DownloadInfo> downloadInfo = builder->Build();
        sDownloadNotificationService->OnDownloadUpdated(downloadInfo);
    }
}

/**
 * Notifies the download delegate that a dangerous download started.
 */
//@CalledByNative
void DownloadController::OnDangerousDownload(
    /* [in] */ ContentViewCore* view,
    /* [in] */ const String& filename,
    /* [in] */ Int32 downloadId)
{
    AutoPtr<ContentViewDownloadDelegate> downloadDelegate = DownloadDelegateFromView(view);
    if (downloadDelegate != NULL) {
        downloadDelegate->OnDangerousDownload(filename, downloadId);
    }
}

// native methods
void DownloadController::NativeInit()
{
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
