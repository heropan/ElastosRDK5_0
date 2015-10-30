
#ifndef __ELASTOS_DROID_WEBKIT_HTML5VIDEOVIEWPROXY_H__
#define __ELASTOS_DROID_WEBKIT_HTML5VIDEOVIEWPROXY_H__

#include "elastos/droid/ext/frameworkext.h"

#include "elastos/droid/os/HandlerBase.h"
#include "elastos/droid/webkit/HTML5VideoView.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IOnFrameAvailableListener;
using Elastos::Droid::Graphics::ISurfaceTexture;
using Elastos::Droid::Media::IMediaPlayer;
using Elastos::Droid::Media::IMediaPlayerOnCompletionListener;
using Elastos::Droid::Media::IMediaPlayerOnErrorListener;
using Elastos::Droid::Media::IMediaPlayerOnInfoListener;
using Elastos::Droid::Media::IMediaPlayerOnPreparedListener;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Net::Http::IEventHandler;
using Elastos::Droid::Net::Http::IHeaders;
using Elastos::Droid::Net::Http::IRequestHandle;
using Elastos::Droid::Net::Http::IRequestQueue;
using Elastos::Droid::Net::Http::ISslCertificate;
using Elastos::Droid::Net::Http::ISslError;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewOnKeyListener;
using Elastos::IO::IByteArrayOutputStream;
using Elastos::Net::IURL;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CWebViewCore;
class CWebViewClassic;

/**
 * <p>Proxy for HTML5 video views.
 */
class HTML5VideoViewProxy
    : public HandlerBase
    , public IMediaPlayerOnPreparedListener
    , public IMediaPlayerOnCompletionListener
    , public IMediaPlayerOnErrorListener
    , public IMediaPlayerOnInfoListener
    , public IOnFrameAvailableListener
    , public IViewOnKeyListener
{
private:
    // Everything below this comment executes on the WebCore thread, except for
    // the EventHandler methods, which are called on the network thread.

    // A helper class that knows how to download posters
    class PosterDownloader
        : public Object
        , public IEventHandler
    {
    private:
        class InnerRunnable
            : public Object
            , public IRunnable
        {
        public:
            InnerRunnable(
                /* [in] */ PosterDownloader* owner);

            CAR_INTERFACE_DECL();

            CARAPI Run();

            CARAPI ToString(
                /* [out] */ String* info);

        private:
            PosterDownloader* mOwner;
        };

    public:
        PosterDownloader(
            /* [in] */ const String& url,
            /* [in] */ HTML5VideoViewProxy* proxy);

        CAR_INTERFACE_DECL();

        // Start the download. Called on WebCore thread.
        virtual CARAPI_(void) Start();

        // Cancel the download if active and release the queue. Called on WebCore thread.
        virtual CARAPI_(void) CancelAndReleaseQueue();

        // EventHandler methods. Executed on the network thread.
        CARAPI Status(
            /* [in] */ Int32 major_version,
            /* [in] */ Int32 minor_version,
            /* [in] */ Int32 code,
            /* [in] */ const String& reason_phrase);

        CARAPI Headers(
            /* [in] */ IHeaders* headers);

        CARAPI Data(
            /* [in] */ const ArrayOf<Byte>& data,
            /* [in] */ Int32 len);

        CARAPI EndData();

        CARAPI Certificate(
            /* [in] */ ISslCertificate* certificate);

        CARAPI Error(
            /* [in] */ Int32 id,
            /* [in] */ const String& description);

        CARAPI HandleSslErrorRequest(
            /* [in] */ ISslError* error,
            /* [out] */ Boolean* result);

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        // Tears down the poster bytes stream. Called on network thread.
        CARAPI_(void) Cleanup();

        // Queue management methods. Called on WebCore thread.
        CARAPI_(void) RetainQueue();

        CARAPI_(void) ReleaseQueue();

    private:
        // The request queue. This is static as we have one queue for all posters.
        static AutoPtr<IRequestQueue> mRequestQueue;
        static Int32 mQueueRefCount;
        // The poster URL
        AutoPtr<IURL> mUrl;
        // The proxy we're doing this for.
        const AutoPtr<HTML5VideoViewProxy> mProxy;
        // The poster bytes. We only touch this on the network thread.
        AutoPtr<IByteArrayOutputStream> mPosterBytes;
        // The request handle. We only touch this on the WebCore thread.
        AutoPtr<IRequestHandle> mRequestHandle;
        // The response status code.
        Int32 mStatusCode;
        // The response headers.
        AutoPtr<IHeaders> mHeaders;
        // The handler to handle messages on the WebCore thread.
        AutoPtr<IHandler> mHandler;
    };

    // A helper class to control the playback. This executes on the UI thread!
    class VideoPlayer
    {
        friend class HTML5VideoViewProxy;
    public:
        // Every time webView setBaseLayer, this will be called.
        // When we found the Video layer, then we set the Surface Texture to it.
        // Otherwise, we may want to delete the Surface Texture to save memory.
        static CARAPI_(void) SetBaseLayer(
            /* [in] */ Int32 layer);

        // When a WebView is paused, we also want to pause the video in it.
        static CARAPI_(void) PauseAndDispatch();

        static CARAPI_(void) EnterFullScreenVideo(
            /* [in] */ Int32 layerId,
            /* [in] */ const String& url,
            /* [in] */ HTML5VideoViewProxy* proxy,
            /* [in] */ CWebViewClassic* webView);

        static CARAPI_(void) ExitFullScreenVideo(
            /* [in] */ HTML5VideoViewProxy* proxy,
            /* [in] */ CWebViewClassic* webView);

        // This is on the UI thread.
        // When native tell Java to play, we need to check whether or not it is
        // still the same video by using videoLayerId and treat it differently.
        static CARAPI_(void) Play(
            /* [in] */ const String& url,
            /* [in] */ Int32 time,
            /* [in] */ HTML5VideoViewProxy* proxy,
            /* [in] */ IWebChromeClient* client,
            /* [in] */ Int32 videoLayerId);

        static CARAPI_(Boolean) IsPlaying(
            /* [in] */ HTML5VideoViewProxy* proxy);

        static CARAPI_(Int32) GetCurrentPosition();

        static CARAPI_(void) Seek(
            /* [in] */ Int32 time,
            /* [in] */ HTML5VideoViewProxy* proxy);

        static CARAPI_(void) Pause(
            /* [in] */ HTML5VideoViewProxy* proxy);

        static CARAPI_(void) OnPrepared();

        static CARAPI_(void) End();

    private:
        static CARAPI_(void) SetPlayerBuffering(
            /* [in] */ Boolean playerBuffering);

        // The proxy that is currently playing (if any).
        static AutoPtr<HTML5VideoViewProxy> mCurrentProxy;
        // The VideoView instance. This is a singleton for now, at least until
        // http://b/issue?id=1973663 is fixed.
        static AutoPtr<HTML5VideoView> mHTML5VideoView;

        static Boolean isVideoSelfEnded;
    };

    class InnerHandler : public HandlerBase
    {
    public:
        InnerHandler(
            HTML5VideoViewProxy* owner);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        HTML5VideoViewProxy* mOwner;
    };

public:
    CAR_INTERFACE_DECL();

    // A bunch event listeners for our VideoView
    // MediaPlayer.OnPreparedListener
    CARAPI OnPrepared(
        /* [in] */ IMediaPlayer* mp);

    // MediaPlayer.OnCompletionListener;
    CARAPI OnCompletion(
        /* [in] */ IMediaPlayer* mp);

    // MediaPlayer.OnErrorListener
    CARAPI OnError(
        /* [in] */ IMediaPlayer* mp,
        /* [in] */ Int32 what,
        /* [in] */ Int32 extra,
        /* [out] */ Boolean* result);

    virtual CARAPI_(void) DispatchOnEnded();

    virtual CARAPI_(void) DispatchOnPaused();

    virtual CARAPI_(void) DispatchOnStopFullScreen(
        /* [in] */ Boolean stillPlaying);

    virtual CARAPI_(void) DispatchOnRestoreState();

    virtual CARAPI_(void) OnTimeupdate();

    // When there is a frame ready from surface texture, we should tell WebView
    // to refresh.
    CARAPI OnFrameAvailable(
        /* [in] */ ISurfaceTexture* surfaceTexture);

    // Handler for the messages from WebCore or Timer thread to the UI thread.
    CARAPI HandleMessage(
        /* [in] */ IMessage* msg);

    virtual CARAPI_(AutoPtr<IContext>) GetContext();

    // The public methods below are all called from WebKit only.
    /**
     * Play a video stream.
     * @param url is the URL of the video stream.
     */
    virtual CARAPI_(void) Play(
        /* [in] */ const String& url,
        /* [in] */ Int32 position,
        /* [in] */ Int32 videoLayerID);

    /**
     * Play a video stream in full screen mode.
     * @param url is the URL of the video stream.
     */
    virtual CARAPI_(void) EnterFullscreenForVideoLayer(
        /* [in] */ const String& url,
        /* [in] */ Int32 videoLayerID);

    /**
     * Seek into the video stream.
     * @param  time is the position in the video stream.
     */
    virtual CARAPI_(void) Seek(
        /* [in] */ Int32 time);

    /**
     * Pause the playback.
     */
    virtual CARAPI_(void) Pause();

    /**
     * Tear down this proxy object.
     */
    virtual CARAPI_(void) Teardown();

    /**
     * Load the poster image.
     * @param url is the URL of the poster image.
     */
    virtual CARAPI_(void) LoadPoster(
        /* [in] */ const String& url);

    // These three function are called from UI thread only by WebView.
    virtual CARAPI_(void) SetBaseLayer(
        /* [in] */ Int32 layer);

    virtual CARAPI_(void) PauseAndDispatch();

    virtual CARAPI_(void) EnterFullScreenVideo(
        /* [in] */ Int32 layerId,
        /* [in] */ const String& url);

    virtual CARAPI_(void) ExitFullScreenVideo();

    /**
     * The factory for HTML5VideoViewProxy instances.
     * @param webViewCore is the WebViewCore that is requesting the proxy.
     *
     * @return a new HTML5VideoViewProxy object.
     */
    static CARAPI_(AutoPtr<HTML5VideoViewProxy>) GetInstance(
        /* [in] */ CWebViewCore* webViewCore,
        /* [in] */ Int32 nativePtr);

    /* package */
    virtual CARAPI_(AutoPtr<CWebViewClassic>) GetWebView();

    CARAPI OnInfo(
        /* [in] */ IMediaPlayer* mp,
        /* [in] */ Int32 what,
        /* [in] */ Int32 extra,
        /* [out] */ Boolean* result);

    CARAPI OnKey(
        /* [in] */ IView* v,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI ToString(
        /* [out] */ String* info);

public:
    // The C++ MediaPlayerPrivateAndroid object.
    Int32 mNativePointer;

private:
    /**
     * Private constructor.
     * @param webView is the WebView that hosts the video.
     * @param nativePtr is the C++ pointer to the MediaPlayerPrivate object.
     */
    HTML5VideoViewProxy(
        /* [in] */ CWebViewClassic* webView,
        /* [in] */ Int32 nativePtr);

    CARAPI_(void) CreateWebCoreHandler();

    CARAPI_(void) DoSetPoster(
        /* [in] */ IBitmap* poster);

    CARAPI_(void) SendTimeupdate();

    CARAPI_(void) NativeOnPrepared(
        /* [in] */ Int32 duration,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 nativePointer);

    CARAPI_(void) NativeOnEnded(
        /* [in] */ Int32 nativePointer);

    CARAPI_(void) NativeOnPaused(
        /* [in] */ Int32 nativePointer);

    CARAPI_(void) NativeOnPosterFetched(
        /* [in] */ IBitmap* poster,
        /* [in] */ Int32 nativePointer);

    CARAPI_(void) NativeOnTimeupdate(
        /* [in] */ Int32 position,
        /* [in] */ Int32 nativePointer);

    CARAPI_(void) NativeOnStopFullscreen(
        /* [in] */ Int32 stillPlaying,
        /* [in] */ Int32 nativePointer);

    CARAPI_(void) NativeOnRestoreState(
        /* [in] */ Int32 nativePointer);

    static CARAPI_(Boolean) NativeSendSurfaceTexture(
        /* [in] */ ISurfaceTexture* texture,
        /* [in] */ Int32 baseLayer,
        /* [in] */ Int32 videoLayerId,
        /* [in] */ Int32 textureName,
        /* [in] */ Int32 playerState);

private:
    // Logging tag.
    static const String LOGTAG;

    // Message Ids for WebCore thread -> UI thread communication.
    static const Int32 PLAY                = 100;
    static const Int32 SEEK                = 101;
    static const Int32 PAUSE               = 102;
    static const Int32 HVVP_ERROR          = 103;
    static const Int32 LOAD_DEFAULT_POSTER = 104;
    static const Int32 BUFFERING_START     = 105;
    static const Int32 BUFFERING_END       = 106;
    static const Int32 ENTER_FULLSCREEN    = 107;

    // Message Ids to be handled on the WebCore thread
    static const Int32 PREPARED          = 200;
    static const Int32 ENDED             = 201;
    static const Int32 POSTER_FETCHED    = 202;
    static const Int32 PAUSED            = 203;
    static const Int32 STOPFULLSCREEN    = 204;
    static const Int32 RESTORESTATE      = 205;
    static const Boolean DISABLE_ROTATION;
    static Int32 mSavedOrient;

    // Timer thread -> UI thread
    static const Int32 TIMEUPDATE = 300;

    // The handler for WebCore thread messages;
    AutoPtr<IHandler> mWebCoreHandler;
    // The WebViewClassic instance that created this view.
    AutoPtr<CWebViewClassic> mWebView;
    // The poster image to be shown when the video is not playing.
    // This ref prevents the bitmap from being GC'ed.
    AutoPtr<IBitmap> mPoster;
    // The poster downloader.
    AutoPtr<PosterDownloader> mPosterDownloader;
    // The seek position.
    Int32 mSeekPosition;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_HTML5VIDEOVIEWPROXY_H__
