
#ifndef __BROWSERDOWNLOADLISTENER_H__
#define __BROWSERDOWNLOADLISTENER_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * An abstract download listener that allows passing extra information as
 * part of onDownloadStart callback.
 * @hide
 */
class BrowserDownloadListener
{
public:
    /**
     * Notify the host application that a file should be downloaded
     * @param url The full url to the content that should be downloaded
     * @param userAgent the user agent to be used for the download.
     * @param contentDisposition Content-disposition http header, if
     *                           present.
     * @param mimetype The mimetype of the content reported by the server
     * @param contentLength The file size reported by the server
     */
    CARAPI OnDownloadStart(
        /* [in] */ const String& url,
        /* [in] */ const String& userAgent,
        /* [in] */ const String& contentDisposition,
        /* [in] */ const String& mimetype,
        /* [in] */ Int64 contentLength) {

        return OnDownloadStartEx(url, userAgent, contentDisposition, mimetype, String(NULL),
                      contentLength);
    }
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__BROWSERDOWNLOADLISTENER_H__
