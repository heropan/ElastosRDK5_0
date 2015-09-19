
#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_DEFAULTVIDEOPOSTERREQUESTHANDLER_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_DEFAULTVIDEOPOSTERREQUESTHANDLER_H__

// import android.graphics.Bitmap;
// import android.os.AsyncTask;
// import android.util.Log;

// import org.chromium.base.ThreadUtils;

// import java.io.IOException;
// import java.io.InputStream;
// import java.io.OutputStream;
// import java.io.PipedInputStream;
// import java.io.PipedOutputStream;
// import java.util.Random;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * This class takes advantage of shouldInterceptRequest(), returns the bitmap from
 * WebChromeClient.getDefaultVidoePoster() when the mDefaultVideoPosterURL is requested.
 *
 * The shouldInterceptRequest is used to get the default video poster, if the url is
 * the mDefaultVideoPosterURL.
 */
class DefaultVideoPosterRequestHandler
{
private:
    class UIRunnable
        : public Object
        , public IRunnable
    {
    public:
        UIRunnable(
            /* [in] */ DefaultVideoPosterRequestHandler* owner,
            /* [in] */ IPipedOutputStream* outputStream,
            /* [in] */ AwContentsClient* contentClient);

        CARAPI Run();

    private:
        DefaultVideoPosterRequestHandler* mOwner;
        IPipedOutputStream* mOutputStream;
        AwContentsClient* mContentClient;
    };

    class AsyncTaskRunnable
        : public Object
        , public IRunnable
    {
    public:
        AsyncTaskRunnable(
            /* [in] */ DefaultVideoPosterRequestHandler* owner,
            /* [in] */ IBitmap* defaultVideoPoster,
            /* [in] */ IPipedOutputStream* outputStream);

        CARAPI Run();

    private:
        DefaultVideoPosterRequestHandler* mOwner;
        AutoPtr<IBitmap> mDefaultVideoPoster;
        AutoPtr<IPipedOutputStream> mOutputStream;
    };

public:
    DefaultVideoPosterRequestHandler(
        /* [in] */ AwContentsClient* contentClient);

    /**
     * Used to get the image if the url is mDefaultVideoPosterURL.
     *
     * @param url the url requested
     * @return AwWebResourceResponse which caller can get the image if the url is
     * the default video poster URL, otherwise null is returned.
     */
    CARAPI_(AutoPtr<AwWebResourceResponse>) ShouldInterceptRequest(
        /* [in] */ const String& url);

    CARAPI_(String) GetDefaultVideoPosterURL();

private:
    static CARAPI_(AutoPtr<IInputStream>) GetInputStream(
        /* [in] */ const AwContentsClient* contentClient);

    static CARAPI_(void) CloseOutputStream(
        /* [in] */ IOutputStream* outputStream);

    /**
     * @return a unique URL which has little chance to be used by application.
     */
    static CARAPI_(String) GenerateDefaulVideoPosterURL();

private:
    static const String TAG;
    String mDefaultVideoPosterURL;
    AutoPtr<AwContentsClient> mContentClient;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_DEFAULTVIDEOPOSTERREQUESTHANDLER_H__
