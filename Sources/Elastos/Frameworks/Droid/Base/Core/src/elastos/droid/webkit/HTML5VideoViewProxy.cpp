
#include "elastos/droid/graphics/CBitmapFactory.h"
#include "elastos/droid/graphics/CSurfaceTexture.h"
#include "elastos/droid/graphics/GraphicsNative.h"
#include "elastos/droid/os/CHandler.h"
#include "elastos/droid/os/CMessageHelper.h"
#include "elastos/droid/os/CLooperHelper.h"
//#include "elastos/droid/net/http/CRequestQueue.h"
#include "elastos/droid/webkit/HTML5VideoFullScreen.h"
#include "elastos/droid/webkit/HTML5VideoInline.h"
#include "elastos/droid/webkit/HTML5VideoViewProxy.h"
#include "elastos/droid/webkit/CWebViewCore.h"
#include "elastos/droid/webkit/CWebViewClassic.h"
#include <elastos/utility/logging/Logger.h>

#include <skia/core/SkBitmap.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Media::EIID_IMediaPlayerOnPreparedListener;
using Elastos::Droid::Media::EIID_IMediaPlayerOnCompletionListener;
using Elastos::Droid::Media::EIID_IMediaPlayerOnErrorListener;
using Elastos::Droid::Media::EIID_IMediaPlayerOnInfoListener;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Graphics::CSurfaceTexture;
using Elastos::Droid::Graphics::GraphicsNative;
using Elastos::Droid::View::EIID_IViewOnKeyListener;
//using Elastos::Droid::Net::Http::CRequestQueue;
using Elastos::Droid::Net::Http::EIID_IEventHandler;
using Elastos::Droid::App::EIID_IActivity;
using Elastos::Droid::App::IActivity;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::CLooperHelper;
using Elastos::Droid::Os::ILooperHelper;
using Elastos::IO::CByteArrayOutputStream;
using Elastos::Net::CURL;
using Elastos::Utility::CObjectStringMap;
using Elastos::Utility::IObjectStringMap;

namespace Elastos {

#ifdef __cplusplus
extern "C"
{
#endif

void Elastos_MediaPlayerPrivateAndroid_OnPrepared(Int32 duration, Int32 width, Int32 height, Int32 pointer);

void Elastos_MediaPlayerPrivateAndroid_OnEnded(Int32 pointer);

void Elastos_MediaPlayerPrivateAndroid_OnRequestPlay(Int32 pointer);

void Elastos_MediaPlayerPrivateAndroid_OnPaused(Int32 pointer);

void Elastos_MediaPlayerPrivateAndroid_OnPosterFetched(SkBitmap* posterNative, Int32 pointer);

void Elastos_MediaPlayerPrivateAndroid_OnBuffering(Int32 percent, Int32 pointer);

void Elastos_MediaPlayerPrivateAndroid_OnTimeupdate(Int32 position, Int32 pointer);

void Elastos_MediaPlayerPrivateAndroid_OnRestoreState(Int32 pointer);

Boolean Elastos_MediaPlayerPrivateAndroid_SendSurfaceTexture(android::sp<android::SurfaceTexture> texture,
                               Int32 baseLayer, Int32 videoLayerId,
                               Int32 textureName, Int32 playerState);

void Elastos_MediaPlayerPrivateAndroid_OnStopFullscreen(Int32 stillPlaying, Int32 pointer);

#ifdef __cplusplus
}
#endif

} //namespace Elastos

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//      HTML5VideoViewProxy::PosterDownloader::InnerRunnable
//===============================================================

HTML5VideoViewProxy::PosterDownloader::InnerRunnable::InnerRunnable(
    /* [in] */ PosterDownloader* owner)
    : mOwner(owner)
{
}

CAR_INTERFACE_IMPL_LIGHT(HTML5VideoViewProxy::PosterDownloader::InnerRunnable, IRunnable);

ECode HTML5VideoViewProxy::PosterDownloader::InnerRunnable::Run()
{
    if (mOwner->mRequestHandle != NULL) {
        String url;
        mOwner->mUrl->ToString(&url);
        AutoPtr<IObjectStringMap> map;
        CObjectStringMap::New((IObjectStringMap**)&map);
        Boolean result = FALSE;
        mOwner->mRequestHandle->SetupRedirect(url, mOwner->mStatusCode, map, &result);
    }

    return NOERROR;
}

ECode HTML5VideoViewProxy::PosterDownloader::InnerRunnable::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "HTML5VideoViewProxy::PosterDownloader::InnerRunnable";
    return NOERROR;
}

//===============================================================
//               HTML5VideoViewProxy::PosterDownloader
//===============================================================

// The request queue. This is static as we have one queue for all posters.
AutoPtr<IRequestQueue> HTML5VideoViewProxy::PosterDownloader::mRequestQueue;
Int32 HTML5VideoViewProxy::PosterDownloader::mQueueRefCount = 0;

HTML5VideoViewProxy::PosterDownloader::PosterDownloader(
    /* [in] */ const String& url,
    /* [in] */ HTML5VideoViewProxy* proxy)
       : mProxy(proxy)
{
    //try {
        CURL::New(url, (IURL**)&mUrl);
    //} catch (MalformedURLException e) {
    //    mUrl = null;
    //}

    CHandler::New((IHandler**)&mHandler);
}

CAR_INTERFACE_IMPL(HTML5VideoViewProxy::PosterDownloader, IEventHandler);

// Start the download. Called on WebCore thread.
void HTML5VideoViewProxy::PosterDownloader::Start()
{
    RetainQueue();

    if (mUrl == NULL) {
        return;
    }

    // Only support downloading posters over http/https.
    // FIXME: Add support for other schemes. WebKit seems able to load
    // posters over other schemes e.g. file://, but gets the dimensions wrong.
    String protocol;
    mUrl->GetProtocol(&protocol);
    if (protocol.Equals("http") || protocol.Equals("https")) {
        String str;
        mUrl->ToString(&str);
        mRequestHandle = NULL;
        mRequestQueue->QueueRequest(str, String("GET"), NULL,
                this, NULL, 0, (IRequestHandle**)&mRequestHandle);
    }
}

// Cancel the download if active and release the queue. Called on WebCore thread.
void HTML5VideoViewProxy::PosterDownloader::CancelAndReleaseQueue()
{
    if (mRequestHandle != NULL) {
        mRequestHandle->Cancel();
        mRequestHandle = NULL;
    }
    ReleaseQueue();
}

// EventHandler methods. Executed on the network thread.
ECode HTML5VideoViewProxy::PosterDownloader::Status(
    /* [in] */ Int32 major_version,
    /* [in] */ Int32 minor_version,
    /* [in] */ Int32 code,
    /* [in] */ const String& reason_phrase)
{
    mStatusCode = code;
    return NOERROR;
}

ECode HTML5VideoViewProxy::PosterDownloader::Headers(
    /* [in] */ IHeaders* headers)
{
    mHeaders = headers;
    return NOERROR;
}

ECode HTML5VideoViewProxy::PosterDownloader::Data(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 len)
{
    if (mPosterBytes == NULL) {
        CByteArrayOutputStream::New((IByteArrayOutputStream**)&mPosterBytes);
    }
    mPosterBytes->WriteBytes(data, 0, len);
    return NOERROR;
}

ECode HTML5VideoViewProxy::PosterDownloader::EndData()
{
    if (mStatusCode == 200) {
        Int32 size;
        mPosterBytes->GetSize(&size);
        if (size > 0) {
            AutoPtr<IBitmap> poster;
            AutoPtr<IBitmapFactory> factory;
            CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory);
            AutoPtr< ArrayOf<Byte> > array = ArrayOf<Byte>::Alloc(size);
            mPosterBytes->ToByteArray((ArrayOf<Byte>**)&array);
            factory->DecodeByteArray(*array, 0, size, (IBitmap**)&poster);
            mProxy->DoSetPoster(poster);
        }
        Cleanup();
    }
    else if (mStatusCode >= 300 && mStatusCode < 400) {
        // We have a redirect.
        //try {
            String location;
            mHeaders->GetLocation(&location);
            CURL::New(location, (IURL**)&mUrl);
        //} catch (MalformedURLException e) {
        //    mUrl = null;
        //}
        if (mUrl != NULL) {
            AutoPtr<IRunnable> runnable = new InnerRunnable(this);
            Boolean result = FALSE;
            mHandler->Post(runnable, &result);
        }
    }
}

ECode HTML5VideoViewProxy::PosterDownloader::Certificate(
    /* [in] */ ISslCertificate* certificate)
{
    // Don't care.
    return NOERROR;
}

ECode HTML5VideoViewProxy::PosterDownloader::Error(
    /* [in] */ Int32 id,
    /* [in] */ const String& description)
{
    Cleanup();
    return NOERROR;
}

ECode HTML5VideoViewProxy::PosterDownloader::HandleSslErrorRequest(
    /* [in] */ ISslError* error,
    /* [out] */ Boolean* result)
{
    // Don't care. If this happens, data() will never be called so
    // mPosterBytes will never be created, so no need to call cleanup.
    if (result) *result = FALSE;

    return NOERROR;
}

// Tears down the poster bytes stream. Called on network thread.
void HTML5VideoViewProxy::PosterDownloader::Cleanup()
{
    if (mPosterBytes != NULL) {
        //try {
            mPosterBytes->Close();
        //} catch (IOException ignored) {
            // Ignored.
        //} finally {
        //    mPosterBytes = null;
        //}
    }
}

// Queue management methods. Called on WebCore thread.
void HTML5VideoViewProxy::PosterDownloader::RetainQueue()
{
    Logger::D("HTML5VideoViewProxy", "TODO, no impl PosterDownloader::RetainQueue(depend on apache)");
    if (mRequestQueue == NULL) {
//        CRequestQueue::New(mProxy->GetContext(), (IRequestQueue**)&mRequestQueue);
    }
    mQueueRefCount++;
}

void HTML5VideoViewProxy::PosterDownloader::ReleaseQueue()
{
    if (mQueueRefCount == 0 || !mRequestQueue) {
        return;
    }
    if (--mQueueRefCount == 0) {
        mRequestQueue->Shutdown();
        mRequestQueue = NULL;
    }
}

ECode HTML5VideoViewProxy::PosterDownloader::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "HTML5VideoViewProxy::PosterDownloader";
    return NOERROR;
}

//===============================================================
//               HTML5VideoViewProxy::VideoPlayer
//===============================================================

AutoPtr<HTML5VideoViewProxy> HTML5VideoViewProxy::VideoPlayer::mCurrentProxy;
AutoPtr<HTML5VideoView> HTML5VideoViewProxy::VideoPlayer::mHTML5VideoView;
Boolean HTML5VideoViewProxy::VideoPlayer::isVideoSelfEnded = FALSE;

// Every time webView setBaseLayer, this will be called.
// When we found the Video layer, then we set the Surface Texture to it.
// Otherwise, we may want to delete the Surface Texture to save memory.
void HTML5VideoViewProxy::VideoPlayer::SetBaseLayer(
    /* [in] */ Int32 layer)
{
    // Don't do this for full screen mode.
    if (mHTML5VideoView != NULL
        && !mHTML5VideoView->IsFullScreenMode()
        && !mHTML5VideoView->IsReleased()) {
        Int32 currentVideoLayerId = mHTML5VideoView->GetVideoLayerId();
        AutoPtr<ISurfaceTexture> surfTexture =
                HTML5VideoInline::GetSurfaceTexture(currentVideoLayerId);
        Int32 textureName = mHTML5VideoView->GetTextureName();

        if (layer != 0 && surfTexture != NULL && currentVideoLayerId != -1) {
            Int32 playerState = mHTML5VideoView->GetCurrentState();
            if (mHTML5VideoView->GetPlayerBuffering())
                playerState = HTML5VideoView::STATE_PREPARING;
            Boolean foundInTree = NativeSendSurfaceTexture(surfTexture,
                    layer, currentVideoLayerId, textureName,
                    playerState);
            if (playerState >= HTML5VideoView::STATE_PREPARED
                    && !foundInTree) {
                mHTML5VideoView->PauseAndDispatch(mCurrentProxy);
            }
        }
    }
}

// When a WebView is paused, we also want to pause the video in it.
void HTML5VideoViewProxy::VideoPlayer::PauseAndDispatch()
{
    if (mHTML5VideoView != NULL) {
        mHTML5VideoView->PauseAndDispatch(mCurrentProxy);
    }
}

void HTML5VideoViewProxy::VideoPlayer::EnterFullScreenVideo(
    /* [in] */ Int32 layerId,
    /* [in] */ const String& url,
    /* [in] */ HTML5VideoViewProxy* proxy,
    /* [in] */ CWebViewClassic* webView)
{
    // Save the inline video info and inherit it in the full screen
    Int32 savePosition = 0;
    Boolean canSkipPrepare = FALSE;
    Boolean forceStart = FALSE;
    if (mHTML5VideoView != NULL) {
        // We don't allow enter full screen mode while the previous
        // full screen video hasn't finished yet.
        if (!mHTML5VideoView->FullScreenExited() && mHTML5VideoView->IsFullScreenMode()) {
//            Log.w(LOGTAG, "Try to reenter the full screen mode");
            return;
        }
        Int32 playerState = mHTML5VideoView->GetCurrentState();
        // If we are playing the same video, then it is better to
        // save the current position.
        if (layerId == mHTML5VideoView->GetVideoLayerId()) {

            savePosition = mHTML5VideoView->GetCurrentPosition();
            canSkipPrepare = (playerState == HTML5VideoView::STATE_PREPARING
                    || playerState == HTML5VideoView::STATE_PREPARED
                    || playerState == HTML5VideoView::STATE_PLAYING)
                    && !mHTML5VideoView->IsFullScreenMode();
        }
        if (!canSkipPrepare) {
            mHTML5VideoView->Reset();
        } else {
            forceStart = playerState == HTML5VideoView::STATE_PREPARING
                    || playerState == HTML5VideoView::STATE_PLAYING;
        }
    }

    if (DISABLE_ROTATION) {
        AutoPtr<IActivity> act = (IActivity*)(proxy->GetContext()->Probe(EIID_IActivity));
        act->GetRequestedOrientation(&mSavedOrient);
        act->SetRequestedOrientation(IActivityInfo::SCREEN_ORIENTATION_LANDSCAPE);
    }

    mHTML5VideoView = new HTML5VideoFullScreen(proxy->GetContext(),
            layerId, savePosition, canSkipPrepare);
    mHTML5VideoView->SetStartWhenPrepared(TRUE/*forceStart*/);
    mCurrentProxy = proxy;
    mHTML5VideoView->SetVideoURI(url, mCurrentProxy);
    mHTML5VideoView->EnterFullScreenVideoState(layerId, proxy, webView);
}

void HTML5VideoViewProxy::VideoPlayer::ExitFullScreenVideo(
    /* [in] */ HTML5VideoViewProxy* proxy,
    /* [in] */ CWebViewClassic* webView)
{
    if (!mHTML5VideoView->FullScreenExited() && mHTML5VideoView->IsFullScreenMode()) {
        AutoPtr<IWebChromeClient> client;
        webView->GetWebChromeClient((IWebChromeClient**)&client);
        if (client != NULL) {
            client->OnHideCustomView();
        }
    }

    if (DISABLE_ROTATION) {
        AutoPtr<IActivity> act = (IActivity*)(proxy->GetContext()->Probe(EIID_IActivity));
        act->SetRequestedOrientation(IActivityInfo::SCREEN_ORIENTATION_SENSOR);
    }
}

// This is on the UI thread.
// When native tell Java to play, we need to check whether or not it is
// still the same video by using videoLayerId and treat it differently.
void HTML5VideoViewProxy::VideoPlayer::Play(
    /* [in] */ const String& url,
    /* [in] */ Int32 time,
    /* [in] */ HTML5VideoViewProxy* proxy,
    /* [in] */ IWebChromeClient* client,
    /* [in] */ Int32 videoLayerId)
{
    Int32 currentVideoLayerId = -1;
    Boolean backFromFullScreenMode = FALSE;
    if (mHTML5VideoView != NULL) {
        currentVideoLayerId = mHTML5VideoView->GetVideoLayerId();
        backFromFullScreenMode = mHTML5VideoView->FullScreenExited();

        // When playing video back to back in full screen mode,
        // javascript will switch the src and call play.
        // In this case, we can just reuse the same full screen view,
        // and play the video after prepared.
        if (mHTML5VideoView->IsFullScreenMode()
            && !backFromFullScreenMode
            && currentVideoLayerId != videoLayerId
            && mCurrentProxy != proxy) {
            mCurrentProxy = proxy;
            mHTML5VideoView->SetStartWhenPrepared(true);
            mHTML5VideoView->SetVideoURI(url, proxy);
            mHTML5VideoView->ReprepareData(proxy);
            return;
        }
    }

    Boolean skipPrepare = FALSE;
    Boolean createInlineView = FALSE;
    if (backFromFullScreenMode
        && currentVideoLayerId == videoLayerId
        && !mHTML5VideoView->IsReleased()) {
        skipPrepare = TRUE;
        createInlineView = TRUE;
    }
    else if(backFromFullScreenMode
        || currentVideoLayerId != videoLayerId
        || HTML5VideoInline::SurfaceTextureDeleted()) {
        // Here, we handle the case when switching to a new video,
        // either inside a WebView or across WebViews
        // For switching videos within a WebView or across the WebView,
        // we need to pause the old one and re-create a new media player
        // inside the HTML5VideoView.
        if (mHTML5VideoView != NULL) {
            if (!backFromFullScreenMode) {
                mHTML5VideoView->PauseAndDispatch(mCurrentProxy);
            }
            mHTML5VideoView->Reset();
        }
        createInlineView = TRUE;
    }
    if (createInlineView) {
        mCurrentProxy = proxy;
        mHTML5VideoView = new HTML5VideoInline(videoLayerId, time, skipPrepare);

        mHTML5VideoView->SetVideoURI(url, mCurrentProxy);
        mHTML5VideoView->PrepareDataAndDisplayMode(proxy);
        return;
    }
    if (mCurrentProxy == proxy) {
        // Here, we handle the case when we keep playing with one video
        if (!mHTML5VideoView->IsPlaying()) {
            mHTML5VideoView->SeekTo(time);
            mHTML5VideoView->Start();
        }
    }
    else if (mCurrentProxy != NULL) {
        // Some other video is already playing. Notify the caller that
        // its playback ended.
        proxy->DispatchOnEnded();
    }
}

Boolean HTML5VideoViewProxy::VideoPlayer::IsPlaying(
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    return (mCurrentProxy == proxy && mHTML5VideoView != NULL
                    && mHTML5VideoView->IsPlaying());
}

Int32 HTML5VideoViewProxy::VideoPlayer::GetCurrentPosition()
{
    Int32 currentPosMs = 0;
    if (mHTML5VideoView != NULL) {
        currentPosMs = mHTML5VideoView->GetCurrentPosition();
    }
    return currentPosMs;
}

void HTML5VideoViewProxy::VideoPlayer::Seek(
    /* [in] */ Int32 time,
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    if (mCurrentProxy == proxy && time >= 0 && mHTML5VideoView != NULL) {
        mHTML5VideoView->SeekTo(time);
    }
}

void HTML5VideoViewProxy::VideoPlayer::Pause(
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    if (mCurrentProxy == proxy && mHTML5VideoView != NULL) {
        mHTML5VideoView->Pause();
    }
}

void HTML5VideoViewProxy::VideoPlayer::OnPrepared()
{
    if (!mHTML5VideoView->IsFullScreenMode()) {
        mHTML5VideoView->Start();
    }
}

void HTML5VideoViewProxy::VideoPlayer::End()
{
    mHTML5VideoView->ShowControllerInFullScreen();
    if (mCurrentProxy != NULL) {
        if (isVideoSelfEnded) {
            mCurrentProxy->DispatchOnEnded();
            mCurrentProxy->ExitFullScreenVideo();    //add by Bevis, whenever a video ended, exit fullscreen.
        }
        else {
            mCurrentProxy->DispatchOnPaused();
        }
    }
    isVideoSelfEnded = FALSE;
}

void HTML5VideoViewProxy::VideoPlayer::SetPlayerBuffering(
    /* [in] */ Boolean playerBuffering)
{
    mHTML5VideoView->SetPlayerBuffering(playerBuffering);
}

ECode HTML5VideoViewProxy::VideoPlayer::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "HTML5VideoViewProxy::VideoPlayer";
    return NOERROR;
}

//===============================================================
//              HTML5VideoViewProxy::InnerHandler
//===============================================================

HTML5VideoViewProxy::InnerHandler::InnerHandler(
    HTML5VideoViewProxy* owner)
    : mOwner(owner)
{
}

ECode HTML5VideoViewProxy::InnerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case PREPARED: {
            AutoPtr<IObjectStringMap> map;
            msg->GetObj((IInterface**)&map);
            AutoPtr<IInteger32> iDuration, iWidth, iHeight;
            map->Get(String("dur"), (IInterface**)&iDuration);
            map->Get(String("width"), (IInterface**)&iWidth);
            map->Get(String("height"), (IInterface**)&iHeight);
            Int32 duration, width, height;
            iDuration->GetValue(&duration);
            iWidth->GetValue(&width);
            iHeight->GetValue(&height);
            mOwner->NativeOnPrepared(duration, width, height, mOwner->mNativePointer);
            break;
        }
        case ENDED:
            mOwner->mSeekPosition = 0;
            mOwner->NativeOnEnded(mOwner->mNativePointer);
            break;
        case PAUSED:
            mOwner->NativeOnPaused(mOwner->mNativePointer);
            break;
        case POSTER_FETCHED: {
                AutoPtr<IBitmap> poster;
                msg->GetObj((IInterface**)&poster);
                mOwner->NativeOnPosterFetched(poster, mOwner->mNativePointer);
            }
            break;
        case TIMEUPDATE: {
                Int32 arg1;
                msg->GetArg1(&arg1);
                mOwner->NativeOnTimeupdate(arg1, mOwner->mNativePointer);
            }
            break;
        case STOPFULLSCREEN:
            {
                Int32 arg1;
                msg->GetArg1(&arg1);
                mOwner->NativeOnStopFullscreen(arg1, mOwner->mNativePointer);
            }
            break;
        case RESTORESTATE:
            mOwner->NativeOnRestoreState(mOwner->mNativePointer);
            break;
    }

    return NOERROR;
}

ECode HTML5VideoViewProxy::InnerHandler::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "HTML5VideoViewProxy::InnerHandler";
    return NOERROR;
}

//===============================================================
//                    HTML5VideoViewProxy
//===============================================================

const String HTML5VideoViewProxy::LOGTAG("HTML5VideoViewProxy");

// Message Ids for WebCore thread -> UI thread communication.
const Int32 HTML5VideoViewProxy::PLAY;
const Int32 HTML5VideoViewProxy::SEEK;
const Int32 HTML5VideoViewProxy::PAUSE;
const Int32 HTML5VideoViewProxy::HVVP_ERROR;
const Int32 HTML5VideoViewProxy::LOAD_DEFAULT_POSTER;
const Int32 HTML5VideoViewProxy::BUFFERING_START;
const Int32 HTML5VideoViewProxy::BUFFERING_END;
const Int32 HTML5VideoViewProxy::ENTER_FULLSCREEN;

// Message Ids to be handled on the WebCore thread
const Int32 HTML5VideoViewProxy::PREPARED;
const Int32 HTML5VideoViewProxy::ENDED;
const Int32 HTML5VideoViewProxy::POSTER_FETCHED;
const Int32 HTML5VideoViewProxy::PAUSED;
const Int32 HTML5VideoViewProxy::STOPFULLSCREEN;
const Int32 HTML5VideoViewProxy::RESTORESTATE;
const Boolean HTML5VideoViewProxy::DISABLE_ROTATION  = FALSE;
Int32 HTML5VideoViewProxy::mSavedOrient = 0;

// Timer thread -> UI thread
const Int32 HTML5VideoViewProxy::TIMEUPDATE;

// This handler is for the main (UI) thread.
static AutoPtr<ILooper> _GetMainLooper()
{
    AutoPtr<ILooperHelper> looperHelper;
    CLooperHelper::AcquireSingleton((ILooperHelper**)&looperHelper);
    AutoPtr<ILooper> looper;
    looperHelper->GetMainLooper((ILooper**)&looper);
    return looper;
}

/**
 * Private constructor.
 * @param webView is the WebView that hosts the video.
 * @param nativePtr is the C++ pointer to the MediaPlayerPrivate object.
 */
HTML5VideoViewProxy::HTML5VideoViewProxy(
    /* [in] */ CWebViewClassic* webView,
    /* [in] */ Int32 nativePtr)
       : mWebView(webView) // Save the WebView object.
       , mNativePointer(nativePtr) // Save the native ptr
       , HandlerBase(_GetMainLooper())
{
    // Pass Proxy into webview, such that every time we have a setBaseLayer
    // call, we tell this Proxy to call the native to update the layer tree
    // for the Video Layer's surface texture info
    mWebView->SetHTML5VideoViewProxy(this);

    // create the message handler for this thread
    CreateWebCoreHandler();
}

UInt32 HTML5VideoViewProxy::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 HTML5VideoViewProxy::Release()
{
    return ElRefBase::Release();
}

ECode HTML5VideoViewProxy::GetInterfaceID(
    /* [in] */ IInterface *Object,
    /* [out] */ InterfaceID *IID)
{
    VALIDATE_NOT_NULL(IID);

    if (Object == (IInterface*)(IHandler*)this) {
        *IID = EIID_IHandler;
    }
    if (Object == (IInterface*)(IMediaPlayerOnPreparedListener*)this) {
        *IID = EIID_IMediaPlayerOnPreparedListener;
    }
    if (Object == (IInterface*)(IMediaPlayerOnCompletionListener*)this) {
        *IID = EIID_IMediaPlayerOnCompletionListener;
    }
    if (Object == (IInterface*)(IMediaPlayerOnErrorListener*)this) {
        *IID = EIID_IMediaPlayerOnErrorListener;
    }
    if (Object == (IInterface*)(IMediaPlayerOnInfoListener*)this) {
        *IID = EIID_IMediaPlayerOnInfoListener;
    }
    if (Object == (IInterface*)(IOnFrameAvailableListener*)this) {
        *IID = EIID_IOnFrameAvailableListener;
    }
    if (Object == (IInterface*)(IViewOnKeyListener*)this) {
        *IID = EIID_IViewOnKeyListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

PInterface HTML5VideoViewProxy::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IHandler) {
        return (IHandler*)this;
    }
    else if (riid == EIID_IMediaPlayerOnPreparedListener) {
        return (IMediaPlayerOnPreparedListener*)this;
    }
    else if (riid == EIID_IMediaPlayerOnCompletionListener) {
        return (IMediaPlayerOnCompletionListener*)this;
    }
    else if (riid == EIID_IMediaPlayerOnErrorListener) {
        return (IMediaPlayerOnErrorListener*)this;
    }
    else if (riid == EIID_IMediaPlayerOnInfoListener) {
        return (IMediaPlayerOnInfoListener*)this;
    }
    else if (riid == EIID_IOnFrameAvailableListener) {
        return (IOnFrameAvailableListener*)this;
    }
    else if (riid == EIID_IViewOnKeyListener) {
        return (IViewOnKeyListener*)this;
    }

    return NULL;
}

// A bunch event listeners for our VideoView
// MediaPlayer.OnPreparedListener
ECode HTML5VideoViewProxy::OnPrepared(
    /* [in] */ IMediaPlayer* mp)
{
    VALIDATE_NOT_NULL(mp);

    VideoPlayer::OnPrepared();
    AutoPtr<IMessage> msg;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(mWebCoreHandler, PREPARED, (IMessage**)&msg);
    AutoPtr<IObjectStringMap> map;
    CObjectStringMap::New((IObjectStringMap**)&map);
    Int32 duration, width, height;
    mp->GetDuration(&duration);
    mp->GetVideoWidth(&width);
    mp->GetVideoHeight(&height);
    AutoPtr<IInteger32> iDuration, iWidth, iHeight;
    CInteger32::New(duration, (IInteger32**)&iDuration);
    CInteger32::New(width, (IInteger32**)&iWidth);
    CInteger32::New(height, (IInteger32**)&iHeight);
    map->Put(String("dur"), iDuration);
    map->Put(String("width"), iWidth);
    map->Put(String("height"), iHeight);
    msg->SetObj(map);
    Boolean result = FALSE;
    return mWebCoreHandler->SendMessage(msg, &result);
}

// MediaPlayer.OnCompletionListener;
ECode HTML5VideoViewProxy::OnCompletion(
    /* [in] */ IMediaPlayer* mp)
{
    // The video ended by itself, so we need to
    // send a message to the UI thread to dismiss
    // the video view and to return to the WebView.
    // arg1 == 1 means the video ends by itself.
    AutoPtr<IMessage> msg;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(this, ENDED, 1, 0, (IMessage**)&msg);
    Boolean result = FALSE;
    SendMessage(msg, &result);

    return NOERROR;
}

// MediaPlayer.OnErrorListener
ECode HTML5VideoViewProxy::OnError(
    /* [in] */ IMediaPlayer* mp,
    /* [in] */ Int32 what,
    /* [in] */ Int32 extra,
    /* [out] */ Boolean* result)
{
    AutoPtr<IMessage> msg;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(this, ERROR, (IMessage**)&msg);
    Boolean flag = FALSE;
    SendMessage(msg, &flag);

    if(result) *result = FALSE;

    return NOERROR;
}

void HTML5VideoViewProxy::DispatchOnEnded()
{
    AutoPtr<IMessage> msg;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(mWebCoreHandler, ENDED, (IMessage**)&msg);
    Boolean result = FALSE;
    mWebCoreHandler->SendMessage(msg, &result);
}

void HTML5VideoViewProxy::DispatchOnPaused()
{
    AutoPtr<IMessage> msg;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(mWebCoreHandler, PAUSED, (IMessage**)&msg);
    Boolean result = FALSE;
    mWebCoreHandler->SendMessage(msg, &result);
}

void HTML5VideoViewProxy::DispatchOnStopFullScreen(
    /* [in] */ Boolean stillPlaying)
{
    AutoPtr<IMessage> msg;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(mWebCoreHandler, STOPFULLSCREEN, (IMessage**)&msg);
    msg->SetArg1(stillPlaying ? 1 : 0);
    Boolean result = FALSE;
    mWebCoreHandler->SendMessage(msg, &result);
}

void HTML5VideoViewProxy::DispatchOnRestoreState()
{
    AutoPtr<IMessage> msg;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(mWebCoreHandler, RESTORESTATE, (IMessage**)&msg);
    Boolean result = FALSE;
    mWebCoreHandler->SendMessage(msg, &result);
}

void HTML5VideoViewProxy::OnTimeupdate()
{
    AutoPtr<IMessage> msg;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(this, TIMEUPDATE, (IMessage**)&msg);
    Boolean result = FALSE;
    SendMessage(msg, &result);
}

// When there is a frame ready from surface texture, we should tell WebView
// to refresh.
ECode HTML5VideoViewProxy::OnFrameAvailable(
    /* [in] */ ISurfaceTexture* surfaceTexture)
{
    // TODO: This should support partial invalidation too.
    mWebView->Invalidate();

    return NOERROR;
}

// Handler for the messages from WebCore or Timer thread to the UI thread.
ECode HTML5VideoViewProxy::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    // This executes on the UI thread.
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case PLAY: {
            AutoPtr<ICharSequence> _url;
            msg->GetObj((IInterface**)&_url);
            String url;
            _url->ToString(&url);
            AutoPtr<IWebChromeClient> client;
            mWebView->GetWebChromeClient((IWebChromeClient**)&client);
            Int32 videoLayerID;
            msg->GetArg1(&videoLayerID);
            if (client != NULL) {
                VideoPlayer::Play(url, mSeekPosition, this, client, videoLayerID);
            }
            break;
        }
        case ENTER_FULLSCREEN:{
            AutoPtr<ICharSequence> _url;
            msg->GetObj((IInterface**)&_url);
            String url;
            _url->ToString(&url);
            AutoPtr<IWebChromeClient> client;
            mWebView->GetWebChromeClient((IWebChromeClient**)&client);
            Int32 videoLayerID;
            msg->GetArg1(&videoLayerID);
            if (client != NULL) {
                VideoPlayer::EnterFullScreenVideo(videoLayerID, url, this, mWebView);
            }
            break;
        }
        case SEEK: {
            AutoPtr<IInteger32> _time;
            msg->GetObj((IInterface**)&_time);
            Int32 time;
            _time->GetValue(&time);
            mSeekPosition = time;
            VideoPlayer::Seek(mSeekPosition, this);
            break;
        }
        case PAUSE: {
            VideoPlayer::Pause(this);
            break;
        }
        case ENDED:
            Int32 arg1;
            msg->GetArg1(&arg1);
            if (arg1 == 1)
                VideoPlayer::isVideoSelfEnded = true;
            VideoPlayer::End();
            break;
        case ERROR: {
            AutoPtr<IWebChromeClient> client;
            mWebView->GetWebChromeClient((IWebChromeClient**)&client);
            if (client != NULL) {
                client->OnHideCustomView();
            }
            break;
        }
        case LOAD_DEFAULT_POSTER: {
            AutoPtr<IWebChromeClient> client;
            mWebView->GetWebChromeClient((IWebChromeClient**)&client);
            if (client != NULL) {
                AutoPtr<IBitmap> bitmap;
                client->GetDefaultVideoPoster((IBitmap**)&bitmap);
                DoSetPoster(bitmap);
            }
            break;
        }
        case TIMEUPDATE: {
            if (VideoPlayer::IsPlaying(this)) {
                SendTimeupdate();
            }
            break;
        }
        case BUFFERING_START: {
            VideoPlayer::SetPlayerBuffering(TRUE);
            break;
        }
        case BUFFERING_END: {
            VideoPlayer::SetPlayerBuffering(FALSE);
            break;
        }
    }

    return NOERROR;
}

AutoPtr<IContext> HTML5VideoViewProxy::GetContext()
{
    return mWebView->GetContext();
}

// The public methods below are all called from WebKit only.
/**
 * Play a video stream.
 * @param url is the URL of the video stream.
 */
void HTML5VideoViewProxy::Play(
    /* [in] */ const String& url,
    /* [in] */ Int32 position,
    /* [in] */ Int32 videoLayerID)
{
    if (url.IsNullOrEmpty()) {
        return;
    }

    if (position > 0) {
        Seek(position);
    }

    AutoPtr<IMessage> message;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(this, PLAY, (IMessage**)&message);
    message->SetArg1(videoLayerID);
    AutoPtr<ICharSequence> _url;
    CStringWrapper::New(url, (ICharSequence**)&_url);
    message->SetObj(_url);
    Boolean result = FALSE;
    SendMessage(message, &result);
}

/**
 * Play a video stream in full screen mode.
 * @param url is the URL of the video stream.
 */
void HTML5VideoViewProxy::EnterFullscreenForVideoLayer(
    /* [in] */ const String& url,
    /* [in] */ Int32 videoLayerID)
{
    if (url.IsNullOrEmpty()) {
        return;
    }

    AutoPtr<IMessage> message;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(this, ENTER_FULLSCREEN, (IMessage**)&message);
    message->SetArg1(videoLayerID);
    AutoPtr<ICharSequence> _url;
    CStringWrapper::New(url, (ICharSequence**)&_url);
    message->SetObj(_url);
    Boolean result = FALSE;
    SendMessage(message, &result);
}

/**
 * Seek into the video stream.
 * @param  time is the position in the video stream.
 */
void HTML5VideoViewProxy::Seek(
    /* [in] */ Int32 time)
{
    AutoPtr<IMessage> message;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(this, SEEK, (IMessage**)&message);
    AutoPtr<IInteger32> iTime;
    CInteger32::New(time, (IInteger32**)&iTime);
    message->SetObj(iTime);
    Boolean result = FALSE;
    SendMessage(message, &result);
}

/**
 * Pause the playback.
 */
void HTML5VideoViewProxy::Pause()
{
    AutoPtr<IMessage> message;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(this, PAUSE, (IMessage**)&message);
    Boolean result = FALSE;
    SendMessage(message, &result);
}

/**
 * Tear down this proxy object.
 */
void HTML5VideoViewProxy::Teardown()
{
    // This is called by the C++ MediaPlayerPrivate dtor.
    // Cancel any active poster download.
    if (mPosterDownloader != NULL) {
        mPosterDownloader->CancelAndReleaseQueue();
    }
    mNativePointer = 0;
}

/**
 * Load the poster image.
 * @param url is the URL of the poster image.
 */
void HTML5VideoViewProxy::LoadPoster(
    /* [in] */ const String& url)
{
    if (url == NULL) {
        AutoPtr<IMessageHelper> mh;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
        AutoPtr<IMessage> message;
        mh->Obtain(this, LOAD_DEFAULT_POSTER, (IMessage**)&message);
        Boolean result = FALSE;
        SendMessage(message, &result);
        return;
    }
    // Cancel any active poster download.
    if (mPosterDownloader != NULL) {
        mPosterDownloader->CancelAndReleaseQueue();
    }
    // Load the poster asynchronously
    mPosterDownloader = new PosterDownloader(url, this);
    mPosterDownloader->Start();
}

// These three function are called from UI thread only by WebView.
void HTML5VideoViewProxy::SetBaseLayer(
    /* [in] */ Int32 layer)
{
    VideoPlayer::SetBaseLayer(layer);
}

void HTML5VideoViewProxy::PauseAndDispatch()
{
    VideoPlayer::PauseAndDispatch();
}

void HTML5VideoViewProxy::EnterFullScreenVideo(
    /* [in] */ Int32 layerId,
    /* [in] */ const String& url)
{
    VideoPlayer::EnterFullScreenVideo(layerId, url, this, mWebView);
}

void HTML5VideoViewProxy::ExitFullScreenVideo()
{
    VideoPlayer::ExitFullScreenVideo(this, mWebView);
}

/**
 * The factory for HTML5VideoViewProxy instances.
 * @param webViewCore is the WebViewCore that is requesting the proxy.
 *
 * @return a new HTML5VideoViewProxy object.
 */
AutoPtr<HTML5VideoViewProxy> HTML5VideoViewProxy::GetInstance(
    /* [in] */ CWebViewCore* webViewCore,
    /* [in] */ Int32 nativePtr)
{
    return new HTML5VideoViewProxy(webViewCore->GetWebViewClassic(), nativePtr);
}

/* package */
AutoPtr<CWebViewClassic> HTML5VideoViewProxy::GetWebView()
{
    return mWebView;
}

ECode HTML5VideoViewProxy::OnInfo(
    /* [in] */ IMediaPlayer* mp,
    /* [in] */ Int32 what,
    /* [in] */ Int32 extra,
    /* [out] */ Boolean* result)
{
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    if (what == IMediaPlayer::MEDIA_INFO_BUFFERING_START) {
        AutoPtr<IMessage> msg;
        Boolean result = FALSE;
        mh->Obtain(this, BUFFERING_START, what, extra, (IMessage**)&msg);
        SendMessage(msg, &result);
    }
    else if (what == IMediaPlayer::MEDIA_INFO_BUFFERING_END) {
        AutoPtr<IMessage> msg;
        Boolean result = FALSE;
        mh->Obtain(this, BUFFERING_END, what, extra, (IMessage**)&msg);
        SendMessage(msg, &result);
    }

    if (result) *result = FALSE;

    return NOERROR;
}

ECode HTML5VideoViewProxy::OnKey(
    /* [in] */ IView* v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (keyCode == IKeyEvent::KEYCODE_BACK) {
        Int32 action;
        Boolean bFlag = FALSE;
        event->GetAction(&action);
        event->IsCanceled(&bFlag);
        if (action == IKeyEvent::ACTION_DOWN) {
            *result = TRUE;
            return NOERROR;
        }
        else if (action == IKeyEvent::ACTION_UP && !bFlag) {
            VideoPlayer::ExitFullScreenVideo(this, mWebView);
            *result = TRUE;
            return NOERROR;
        }
    }

    *result = FALSE;

    return NOERROR;
}

void HTML5VideoViewProxy::CreateWebCoreHandler()
{
    mWebCoreHandler = new InnerHandler(this);
}

void HTML5VideoViewProxy::DoSetPoster(
    /* [in] */ IBitmap* poster)
{
    if (poster == NULL) {
        return;
    }

    // Save a ref to the bitmap and send it over to the WebCore thread.
    mPoster = poster;
    AutoPtr<IMessage> msg;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(mWebCoreHandler, POSTER_FETCHED, (IMessage**)&msg);
    msg->SetObj(poster);
    Boolean result = FALSE;
    mWebCoreHandler->SendMessage(msg, &result);
}

void HTML5VideoViewProxy::SendTimeupdate()
{
    AutoPtr<IMessage> msg;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(mWebCoreHandler, TIMEUPDATE, (IMessage**)&msg);
    msg->SetArg1(VideoPlayer::GetCurrentPosition());
    Boolean result = FALSE;
    mWebCoreHandler->SendMessage(msg, &result);
}

void HTML5VideoViewProxy::NativeOnPrepared(
    /* [in] */ Int32 duration,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 pointer)
{
    Elastos_MediaPlayerPrivateAndroid_OnPrepared(duration, width, height, pointer);
}

void HTML5VideoViewProxy::NativeOnEnded(
    /* [in] */ Int32 pointer)
{
    Elastos_MediaPlayerPrivateAndroid_OnEnded(pointer);
}

void HTML5VideoViewProxy::NativeOnPaused(
    /* [in] */ Int32 pointer)
{
    Elastos_MediaPlayerPrivateAndroid_OnPaused(pointer);
}

void HTML5VideoViewProxy::NativeOnPosterFetched(
    /* [in] */ IBitmap* poster,
    /* [in] */ Int32 pointer)
{
    SkBitmap* posterNative = GraphicsNative::GetNativeBitmap(poster);
    Elastos_MediaPlayerPrivateAndroid_OnPosterFetched(posterNative, pointer);
}

void HTML5VideoViewProxy::NativeOnTimeupdate(
    /* [in] */ Int32 position,
    /* [in] */ Int32 pointer)
{
    Elastos_MediaPlayerPrivateAndroid_OnTimeupdate(position, pointer);
}

void HTML5VideoViewProxy::NativeOnStopFullscreen(
    /* [in] */ Int32 stillPlaying,
    /* [in] */ Int32 pointer)
{
    Elastos_MediaPlayerPrivateAndroid_OnStopFullscreen(stillPlaying, pointer);
}

void HTML5VideoViewProxy::NativeOnRestoreState(
    /* [in] */ Int32 pointer)
{
    Elastos_MediaPlayerPrivateAndroid_OnRestoreState(pointer);
}

Boolean HTML5VideoViewProxy::NativeSendSurfaceTexture(
    /* [in] */ ISurfaceTexture* surfTex,
    /* [in] */ Int32 baseLayer,
    /* [in] */ Int32 videoLayerId,
    /* [in] */ Int32 textureName,
    /* [in] */ Int32 playerState)
{
    AutoPtr<CSurfaceTexture> cSurfTex = (CSurfaceTexture*)surfTex;
    android::sp<android::SurfaceTexture> texture = SurfaceTexture_getSurfaceTexture(cSurfTex);
    return Elastos_MediaPlayerPrivateAndroid_SendSurfaceTexture(texture, baseLayer, videoLayerId, textureName, playerState);
}

ECode HTML5VideoViewProxy::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "HTML5VideoViewProxy";
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
