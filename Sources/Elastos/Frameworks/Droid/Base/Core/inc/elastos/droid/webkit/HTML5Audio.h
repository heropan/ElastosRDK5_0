
#ifndef __ELASTOS_DROID_WEBKIT_HTML5AUDIO_H__
#define __ELASTOS_DROID_WEBKIT_HTML5AUDIO_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/HandlerBase.h"
#include <elastos/TimerTask.h>

using Elastos::Core::IRunnable;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Media::IMediaPlayer;
using Elastos::Droid::Media::IMediaPlayerOnBufferingUpdateListener;
using Elastos::Droid::Media::IMediaPlayerOnCompletionListener;
using Elastos::Droid::Media::IMediaPlayerOnErrorListener;
using Elastos::Droid::Media::IMediaPlayerOnPreparedListener;
using Elastos::Droid::Media::IMediaPlayerOnSeekCompleteListener;
using Elastos::Droid::Media::IOnAudioFocusChangeListener;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::ILooper;
using Elastos::Utility::ITimer;
using Elastos::Utility::ITimerTask;
using Elastos::Utility::TimerTask;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CWebViewCore;
class CWebViewClassic;

/**
 * HTML5 support class for Audio.
 *
 * This class runs almost entirely on the WebCore thread. The exception is when
 * accessing the WebView object to determine whether private browsing is
 * enabled.
 */
class HTML5Audio
    : public HandlerBase
    , public IMediaPlayerOnBufferingUpdateListener
    , public IMediaPlayerOnCompletionListener
    , public IMediaPlayerOnErrorListener
    , public IMediaPlayerOnPreparedListener
    , public IMediaPlayerOnSeekCompleteListener
    , public IOnAudioFocusChangeListener
{
private:
    class TimeupdateTask
        : public TimerTask
    {
    public:
        TimeupdateTask(
            /* [in] */ HTML5Audio* owner);

        CARAPI Run();

        CARAPI Cancel(
                /* [out] */ Boolean* value);

        CARAPI ScheduledExecutionTime(
                /* [out] */ Int64* value);

        CARAPI GetWhen(
                /* [out] */ Int64* when);

        CARAPI SetWhen(
                /* [in] */ Int64 when);

        CARAPI IsScheduled(
                /* [out] */ Boolean* scheduled);

        CARAPI IsCancelled(
                /* [out] */ Boolean* cancelled);

        CARAPI GetPeriod(
                /* [out] */ Int64* period);

        CARAPI SetPeriod(
                /* [in] */ Int64 period);

        CARAPI IsFixedRate(
                /* [out] */ Boolean* fixed);

        CARAPI SetFixedRate(
                /* [in] */ Boolean fixed);

        CARAPI Lock();

        CARAPI Unlock();

        CARAPI SetScheduledTime(
                /* [in] */ Int64 time);
    private:
        CARAPI_(Object*) GetSelfLock();

    private:
        Object mLock;
        HTML5Audio* mOwner;
    };

    // Helper class to determine whether private browsing is enabled in the
    // given WebView. Queries the WebView on the UI thread. Calls to get()
    // block until the data is available.
    class IsPrivateBrowsingEnabledGetter
        : public ElLightRefBase
        , public Object
    {
    private:
        class InnerRunnable
            : public ElLightRefBase
            , public IRunnable
        {
        public:
            InnerRunnable(
                /* [in] */ IsPrivateBrowsingEnabledGetter* owner,
                /* [in] */ CWebViewClassic* webView);

            CAR_INTERFACE_DECL();

            CARAPI Run();

        private:
            IsPrivateBrowsingEnabledGetter* mOwner;
            CWebViewClassic* mWebView;
        };

    public:
        IsPrivateBrowsingEnabledGetter(
            /* [in] */ HTML5Audio* owner,
            /* [in] */ ILooper* uiThreadLooper,
            /* [in] */ CWebViewClassic* webView);

        //synchronized
        virtual CARAPI_(Boolean) Get();

    private:
        HTML5Audio* mOwner;
        Boolean mIsReady;
        Boolean mIsPrivateBrowsingEnabled;
    };

public:
    /**
     * @param nativePtr is the C++ pointer to the MediaPlayerPrivate object.
     */
    HTML5Audio(
        /* [in] */ CWebViewCore* webViewCore,
        /* [in] */ Int32 nativePtr);

    CAR_INTERFACE_DECL();

    CARAPI HandleMessage(
        /* [in] */ IMessage* msg);

    // event listeners for MediaPlayer
    // Those are called from the same thread we created the MediaPlayer
    // (i.e. the webviewcore thread here)

    // MediaPlayer.OnBufferingUpdateListener
    CARAPI OnBufferingUpdate(
        /* [in] */ IMediaPlayer* mp,
        /* [in] */ Int32 percent);

    // MediaPlayer.OnCompletionListener;
    CARAPI OnCompletion(
        /* [in] */ IMediaPlayer* mp);

    // MediaPlayer.OnErrorListener
    CARAPI OnError(
        /* [in] */ IMediaPlayer* mp,
        /* [in] */ Int32 what,
        /* [in] */ Int32 extra,
        /* [out] */ Boolean* result);

    // MediaPlayer.OnPreparedListener
    CARAPI OnPrepared(
        /* [in] */ IMediaPlayer* mp);

    // MediaPlayer.OnSeekCompleteListener
    CARAPI OnSeekComplete(
        /* [in] */ IMediaPlayer* mp);

    CARAPI OnAudioFocusChange(
        /* [in] */ Int32 focusChange);

    CARAPI_(void) SetDataSource(
        /* [in] */ const String& url);

    CARAPI_(void) Play();

    CARAPI_(void) Pause();

    CARAPI_(void) Seek(
        /* [in] */ Int32 msec);

    /**
     * Called only over JNI when WebKit is happy to
     * destroy the media player.
     */
    CARAPI_(void) Teardown();

    CARAPI_(Float) GetMaxTimeSeekable();

private:
    CARAPI_(void) ResetMediaPlayer();


    CARAPI_(void) NativeOnBuffering(
        /* [in] */ Int32 percent,
        /* [in] */ Int32 nativePointer);

    CARAPI_(void) NativeOnEnded(
        /* [in] */ Int32 nativePointer);

    CARAPI_(void) NativeOnRequestPlay(
        /* [in] */ Int32 nativePointer);

    CARAPI_(void) NativeOnPrepared(
        /* [in] */ Int32 duration,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 nativePointer);

    CARAPI_(void) NativeOnTimeupdate(
        /* [in] */ Int32 position,
        /* [in] */ Int32 nativePointer);

private:
    // Logging tag.
    static const String LOGTAG;

    AutoPtr<IMediaPlayer> mMediaPlayer;

    // The C++ MediaPlayerPrivateAndroid object.
    Int32 mNativePointer;
    // The private status of the view that created this player
    AutoPtr<IsPrivateBrowsingEnabledGetter> mIsPrivateBrowsingEnabledGetter;

    static Int32 IDLE;
    static Int32 INITIALIZED;
    static Int32 PREPARED;
    static Int32 STARTED;
    static Int32 COMPLETE;
    static Int32 PAUSED;
    static Int32 STOPPED;
    static Int32 ERROR;

    Int32 mState;

    String mUrl;
    Boolean mAskToPlay;
    Boolean mLoopEnabled;
    Boolean mProcessingOnEnd;
    AutoPtr<IContext> mContext;

    // Timer thread -> UI thread
    static const Int32 TIMEUPDATE = 100;

    static const String COOKIE;
    static const String HIDE_URL_LOGS;

    // The spec says the timer should fire every 250 ms or less.
    static const Int32 TIMEUPDATE_PERIOD = 250;  // ms
    // The timer for timeupate events.
    // See http://www.whatwg.org/specs/web-apps/current-work/#event-media-timeupdate
    AutoPtr<ITimer> mTimer;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_HTML5AUDIO_H__
