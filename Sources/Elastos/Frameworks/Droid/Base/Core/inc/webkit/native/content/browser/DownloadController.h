
#ifndef __DOWNLOADCONTROLLER_H__
#define __DOWNLOADCONTROLLER_H__

// import android.content.Context;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
 * Java counterpart of android DownloadController.
 *
 * Its a singleton class instantiated by the C++ DownloadController.
 */
//@JNINamespace("content")
class DownloadController
{
public:
    /**
     * Class for notifying the application that download has completed.
     */
    class DownloadNotificationService
    {
    public:
        /**
         * Notify the host application that a download is finished.
         * @param downloadInfo Information about the completed download.
         */
        virtual CARAPI_(void) OnDownloadCompleted(
            /* [in] */ const DownloadInfo* downloadInfo) = 0;

        /**
         * Notify the host application that a download is in progress.
         * @param downloadInfo Information about the in-progress download.
         */
        virtual CARAPI_(void) OnDownloadUpdated(
            /* [in] */ const DownloadInfo* downloadInfo) = 0;
    };

public:
    //@CalledByNative
    static CARAPI_(AutoPtr<DownloadController>) GetInstance();

    static CARAPI_(void) SetDownloadNotificationService(
        /* [in] */ DownloadNotificationService* service);

    /**
     * Notifies the download delegate of a new GET download and passes all the information
     * needed to download the file.
     *
     * The download delegate is expected to handle the download.
     */
    //@CalledByNative
    CARAPI_(void) NewHttpGetDownload(
        /* [in] */ ContentViewCore* view,
        /* [in] */ String url,
        /* [in] */ String userAgent,
        /* [in] */ String contentDisposition,
        /* [in] */ String mimeType,
        /* [in] */ String cookie,
        /* [in] */ String referer,
        /* [in] */ String filename,
        /* [in] */ Int64 contentLength);

    /**
     * Notifies the download delegate that a new download has started. This can
     * be either a POST download or a GET download with authentication.
     * @param view ContentViewCore associated with the download item.
     * @param filename File name of the downloaded file.
     * @param mimeType Mime of the downloaded item.
     */
    //@CalledByNative
    CARAPI_(void) OnDownloadStarted(
        /* [in] */ ContentViewCore* view,
        /* [in] */ String filename,
        /* [in] */ String mimeType);

    /**
     * Notifies the download delegate that a download completed and passes along info about the
     * download. This can be either a POST download or a GET download with authentication.
     */
    //@CalledByNative
    CARAPI_(void) OnDownloadCompleted(
        /* [in] */ IContext* context,
        /* [in] */ String url,
        /* [in] */ String mimeType,
        /* [in] */ String filename,
        /* [in] */ String path,
        /* [in] */ Int64 contentLength,
        /* [in] */ Boolean successful,
        /* [in] */ Int32 downloadId);

    /**
     * Notifies the download delegate about progress of a download. Downloads that use Chrome
     * network stack use custom notification to display the progress of downloads.
     */
    //@CalledByNative
    CARAPI_(void) OnDownloadUpdated(
        /* [in] */ IContext* context,
        /* [in] */ String url,
        /* [in] */ String mimeType,
        /* [in] */ String filename,
        /* [in] */ String path,
        /* [in] */ Int64 contentLength,
        /* [in] */ Boolean successful,
        /* [in] */ Int32 downloadId,
        /* [in] */ Int32 percentCompleted,
        /* [in] */ Int64 timeRemainingInMs);

    /**
     * Notifies the download delegate that a dangerous download started.
     */
    //@CalledByNative
    CARAPI_(void) OnDangerousDownload(
        /* [in] */ ContentViewCore* view,
        /* [in] */ String filename,
        /* [in] */ Int32 downloadId);

private:
    DownloadController();

    static CARAPI_(AutoPtr<ContentViewDownloadDelegate>) DownloadDelegateFromView(
        /* [in] */ ContentViewCore* view);

    // native methods
    CARAPI_(void) NativeInit();

private:
    static const String LOGTAG;
    static AutoPtr<DownloadController> sInstance;

    static AutoPtr<DownloadNotificationService> sDownloadNotificationService;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__DOWNLOADCONTROLLER_H__
