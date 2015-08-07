
#ifndef __CONTENTVIEWDOWNLOADDELEGATE_H__
#ifndef __CONTENTVIEWDOWNLOADDELEGATE_H__

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
 * Interface to be implemented by the embedder to handle file downloads.
 */
class ContentViewDownloadDelegate
{
public:
    /**
    * Notify the host application that a file should be downloaded. Replaces
    * onDownloadStart from DownloadListener.
    * @param downloadInfo Information about the requested download.
    */
    virtual CARAPI_(void) RequestHttpGetDownload(
        /* [in] */ DownloadInfo* downloadInfo) = 0;

    /**
     * Notify the host application that a download is started.
     * @param filename File name of the downloaded file.
     * @param mimeType Mime of the downloaded item.
     */
    virtual CARAPI_(void) OnDownloadStarted(
        /* [in] */ String filename,
        /* [in] */ String mimeType) = 0;

    /**
     * Notify the host application that a download has an extension indicating
     * a dangerous file type.
     * @param filename File name of the downloaded file.
     * @param downloadId The download id.
     */
    virtual CARAPI_(void) OnDangerousDownload(
        /* [in] */ String filename,
        /* [in] */ Int32 downloadId) = 0;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__CONTENTVIEWDOWNLOADDELEGATE_H__
