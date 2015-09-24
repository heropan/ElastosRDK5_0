
#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTSCLIENTCALLBACKHELPER_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTSCLIENTCALLBACKHELPER_H__

// import android.graphics.Picture;
// import android.os.Handler;
// import android.os.Looper;
// import android.os.Message;
// import android.os.SystemClock;

// import com.google.common.annotations.VisibleForTesting;

// import java.util.concurrent.Callable;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * This class is responsible for calling certain client callbacks on the UI thread.
 *
 * Most callbacks do no go through here, but get forwarded to AwContentsClient directly. The
 * messages processed here may originate from the IO or UI thread.
 */
 class AwContentsClientCallbackHelper
 {
 private:
    // TODO(boliu): Consider removing DownloadInfo and LoginRequestInfo by using native
    // MessageLoop to post directly to AwContents.

    class DownloadInfo : public Object
    {
    public:
        DownloadInfo(
            /* [in] */ const String& url,
            /* [in] */ const String& userAgent,
            /* [in] */ const String& contentDisposition,
            /* [in] */ const String& mimeType,
            /* [in] */ Int64 contentLength);

    public:
        const String mUrl;
        const String mUserAgent;
        const String mContentDisposition;
        const String mMimeType;
        const Int64 mContentLength;
    };

    class LoginRequestInfo
    {
    public:
        LoginRequestInfo(
            /* [in] */ const String& realm,
            /* [in] */ const String& account,
            /* [in] */ const String& args);

    public:
        const String mRealm;
        const String mAccount;
        const String mArgs;
    };

    class OnReceivedErrorInfo
    {
    public:
        OnReceivedErrorInfo(
            /* [in] */ Int32 errorCode,
            /* [in] */ const String& description,
            /* [in] */ const String& failingUrl);

    public:
        const Int32 mErrorCode;
        const String mDescription;
        const String mFailingUrl;
    };

    class MyHandler : public Handler
    {
    public:
        //@Override
        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        MyHandler(
            /* [in] */ AwContentsClientCallbackHelper* owner,
            /* [in] */ ILooper* looper);

        AwContentsClientCallbackHelper* owner;
    };

public:
    AwContentsClientCallbackHelper(
        /* [in] */ ILooper* looper,
        /* [in] */ AwContentsClient* contentsClient);

    virtual CARAPI_(void) PostOnLoadResource(
        /* [in] */ const String& url);

    virtual CARAPI_(void) PostOnPageStarted(
        /* [in] */ const String& url);

    virtual CARAPI_(void) PostOnDownloadStart(
        /* [in] */ const String& url,
        /* [in] */ const String& userAgent,
        /* [in] */ const String& contentDisposition,
        /* [in] */ const String& mimeType,
        /* [in] */ Int64 contentLength);

    virtual CARAPI_(void) PostOnReceivedLoginRequest(
        /* [in] */ const String& realm,
        /* [in] */ const String& account,
        /* [in] */ const String& args);

    virtual CARAPI_(void) PostOnReceivedError(
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& description,
        /* [in] */ const String& failingUrl);

    virtual CARAPI_(void) PostOnNewPicture(
        /* [in] */ ICallable* pictureProvider);

    virtual CARAPI_(void) PostOnScaleChangedScaled(
        /* [in] */ Float oldScale,
        /* [in] */ Float newScale);

private:
    static const Int32 MSG_ON_LOAD_RESOURCE = 1;
    static const Int32 MSG_ON_PAGE_STARTED = 2;
    static const Int32 MSG_ON_DOWNLOAD_START = 3;
    static const Int32 MSG_ON_RECEIVED_LOGIN_REQUEST = 4;
    static const Int32 MSG_ON_RECEIVED_ERROR = 5;
    static const Int32 MSG_ON_NEW_PICTURE = 6;
    static const Int32 MSG_ON_SCALE_CHANGED_SCALED = 7;

    // Minimum period allowed between consecutive onNewPicture calls, to rate-limit the callbacks.
    static const Int64 ON_NEW_PICTURE_MIN_PERIOD_MILLIS = 500;
    // Timestamp of the most recent onNewPicture callback.
    Int64 mLastPictureTime;
    // True when a onNewPicture callback is currenly in flight.
    Boolean mHasPendingOnNewPicture;

    const AutoPtr<AwContentsClient> mContentsClient;

    const AutoPtr<IHandler> mHandler;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTSCLIENTCALLBACKHELPER_H__
