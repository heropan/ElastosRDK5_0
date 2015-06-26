
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//================================================================================
//                 DefaultVideoPosterRequestHandler::UIRunnable
//================================================================================

DefaultVideoPosterRequestHandler::UIRunnable::UIRunnable(
    /* [in] */ DefaultVideoPosterRequestHandler* owner,
    /* [in] */ IPipedOutputStream* outputStream,
    /* [in] */ AwContentsClient* contentClient)
    : mOwner(owner)
    , mOutputStream(outputStream)
    , mContentClient(contentClient)
{
}

ECode DefaultVideoPosterRequestHandler::UIRunnable::Run()
{
    AutoPtr<IBitmap> defaultVideoPoster = mContentClient->GetDefaultVideoPoster();
    if (defaultVideoPoster == NULL) {
        CloseOutputStream(mOutputStream);
        return;
    }

    AutoPtr<IRunnable> runnable =  new AsyncTaskRunnable(this, defaultVideoPoster, mOutputStream);
    AsyncTask::THREAD_POOL_EXECUTOR->Execute(runnable);

    return NOERROR;
}

//================================================================================
//               DefaultVideoPosterRequestHandler::AsyncTaskRunnable
//================================================================================

DefaultVideoPosterRequestHandler::AsyncTaskRunnable::AsyncTaskRunnable(
    /* [in] */ DefaultVideoPosterRequestHandler* owner,
    /* [in] */ IBitmap* defaultVideoPoster,
    /* [in] */ IPipedOutputStream* outputStream)
    : mOwner(owner)
    , mDefaultVideoPoster(defaultVideoPoster)
    , mOutputStream(outputStream)
{
}

ECode DefaultVideoPosterRequestHandler::AsyncTaskRunnable::Run()
{
    // try {
        mDefaultVideoPoster->Compress(BitmapCompressFormat_PNG, 100,
                mOutputStream);
        mOutputStream->Flush();
    // } catch (IOException e) {
    //     Log.e(TAG, null, e);
    // } finally {
    //     closeOutputStream(outputStream);
    // }

    return NOERROR;
}

//================================================================================
//                      DefaultVideoPosterRequestHandler
//================================================================================

const String DefaultVideoPosterRequestHandler::TAG("DefaultVideoPosterRequestHandler");

DefaultVideoPosterRequestHandler::DefaultVideoPosterRequestHandler(
    /* [in] */ AwContentsClient* contentClient)
    : mDefaultVideoPosterURL(GenerateDefaulVideoPosterURL())
    , mContentClient(contentClient)
{
}

AutoPtr<IInputStream> DefaultVideoPosterRequestHandler::GetInputStream(
    /* [in] */ const AwContentsClient* contentClient)
{
    AutoPtr<IPipedInputStream> inputStream;
    CPipedInputStream::New((IPipedInputStream**)&inputStream);
    AutoPtr<IPipedOutputStream> outputStream;
    CPipedOutputStream::New(inputStream, (IPipedOutputStream**)&outputStream);

    // Send the request to UI thread to callback to the client, and if it provides a
    // valid bitmap bounce on to the worker thread pool to compress it into the piped
    // input/output stream.
    AutoPtr<IRunnable> runnable = new UIRunnable(this, outputStream, contentClient);
    ThreadUtils::RunOnUiThread(runnable);

    return inputStream;
}

void DefaultVideoPosterRequestHandler::CloseOutputStream(
    /* [in] */ IOutputStream* outputStream) {
    // try {
        outputStream->Close();
    // } catch (IOException e) {
    //     Log.e(TAG, null, e);
    // }
}

/**
 * Used to get the image if the url is mDefaultVideoPosterURL.
 *
 * @param url the url requested
 * @return AwWebResourceResponse which caller can get the image if the url is
 * the default video poster URL, otherwise null is returned.
 */
AutoPtr<AwWebResourceResponse> DefaultVideoPosterRequestHandler::ShouldInterceptRequest(
    /* [in] */ const String& url)
{
    if (!mDefaultVideoPosterURL.Equals(url)) return NULL;

    // try {
        AutoPtr<AwWebResourceResponse> response = new AwWebResourceResponse(String("image/png"), NULL, GetInputStream(mContentClient));
        return response;
    // } catch (IOException e) {
    //     Log.e(TAG, null, e);
    //     return null;
    // }
}

String DefaultVideoPosterRequestHandler::GetDefaultVideoPosterURL()
{
    return mDefaultVideoPosterURL;
}

/**
 * @return a unique URL which has little chance to be used by application.
 */
String DefaultVideoPosterRequestHandler::GenerateDefaulVideoPosterURL()
{
    AutoPtr<IRandom> randomGenerator;
    CRandom::New((IRandom**)&randomGenerator);
    String path = String.valueOf(randomGenerator.nextLong());
    // The scheme of this URL should be kept in sync with kAndroidWebViewVideoPosterScheme
    // on the native side (see android_webview/common/url_constants.h)
    String strRet("android-webview-video-poster:default_video_poster/");
    strRet += path;
    return strRet;
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
