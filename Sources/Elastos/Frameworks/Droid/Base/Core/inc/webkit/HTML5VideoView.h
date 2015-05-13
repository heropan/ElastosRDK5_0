
#ifndef __HTML5VIDEOVIEW_H__
#define __HTML5VIDEOVIEW_H__

#include "ext/frameworkext.h"
#include <elastos/Mutex.h>
#include <elastos/TimerTask.h>

using Elastos::Core::Threading::Mutex;
using Elastos::Droid::Media::IMediaPlayer;
using Elastos::Droid::Media::IMediaPlayerOnPreparedListener;
using Elastos::Droid::Net::IUri;
using Elastos::Utility::IObjectStringMap;
using Elastos::Utility::ITimer;
using Elastos::Utility::ITimerTask;
using Elastos::Utility::TimerTask;

namespace Elastos {
namespace Droid {
namespace Webkit {

class HTML5VideoViewProxy;
class CWebViewClassic;

/**
 * @hide This is only used by the browser
 */
class HTML5VideoView
    : public ElRefBase
    , public IMediaPlayerOnPreparedListener
{
private:
    class TimeupdateTask
        : public TimerTask
    {
    public:
        TimeupdateTask(
            /* [in] */ HTML5VideoViewProxy* proxy);

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
        CARAPI_(Mutex*) GetSelfLock();

    private:
        Mutex mLock;
        AutoPtr<HTML5VideoViewProxy> mProxy;
    };

public:
    // For handling the seekTo before prepared, we need to know whether or not
    // the video is prepared. Therefore, we differentiate the state between
    // prepared and not prepared.
    // When the video is not prepared, we will have to save the seekTo time,
    // and use it when prepared to play.
    // NOTE: these values are in sync with VideoLayerAndroid.h in webkit side.
    // Please keep them in sync when changed.
    static const Int32 STATE_INITIALIZED        = 0;
    static const Int32 STATE_PREPARING          = 1;
    static const Int32 STATE_PREPARED           = 2;
    static const Int32 STATE_PLAYING            = 3;
    static const Int32 STATE_RESETTED           = 4;
    static const Int32 STATE_RELEASED           = 5;

public:
    CAR_INTERFACE_DECL();

    // common Video control FUNCTIONS:
    virtual CARAPI Start();

    virtual CARAPI Pause();

    virtual CARAPI_(Int32) GetDuration();

    virtual CARAPI_(Int32) GetCurrentPosition();

    virtual CARAPI SeekTo(
        /* [in] */ Int32 pos);

    virtual CARAPI_(Boolean) IsPlaying();

    virtual CARAPI_(void) Reset();

    virtual CARAPI_(void) StopPlayback();

    static CARAPI_(void) ReleaseResources();

    virtual CARAPI_(Boolean) IsReleased();

    virtual CARAPI_(Boolean) GetPauseDuringPreparing();

    // Every time we start a new Video, we create a VideoView and a MediaPlayer
    virtual CARAPI_(void) Init(
        /* [in] */ Int32 videoLayerId,
        /* [in] */ Int32 position,
        /* [in] */ Boolean skipPrepare);

    virtual CARAPI_(void) SetVideoURI(
        /* [in] */ const String& uri,
        /* [in] */ HTML5VideoViewProxy* proxy);

    // Listeners setup FUNCTIONS:
    virtual CARAPI_(void) SetOnCompletionListener(
        /* [in] */ HTML5VideoViewProxy* proxy);

    virtual CARAPI_(void) SetOnErrorListener(
        /* [in] */ HTML5VideoViewProxy* proxy);

    virtual CARAPI_(void) SetOnPreparedListener(
        /* [in] */ HTML5VideoViewProxy* proxy);

    virtual CARAPI_(void) SetOnInfoListener(
        /* [in] */ HTML5VideoViewProxy* proxy);

    virtual CARAPI_(void) PrepareDataCommon(
        /* [in] */ HTML5VideoViewProxy* proxy);

    virtual CARAPI_(void) ReprepareData(
        /* [in] */ HTML5VideoViewProxy* proxy);

    // Normally called immediately after setVideoURI. But for full screen,
    // this should be after surface holder created
    virtual CARAPI_(void) PrepareDataAndDisplayMode(
        /* [in] */ HTML5VideoViewProxy* proxy);

    // Common code
    virtual CARAPI_(Int32) GetVideoLayerId();


    virtual CARAPI_(Int32) GetCurrentState();

    ECode OnPrepared(
        /* [in] */ IMediaPlayer* mp);

    // Pause the play and update the play/pause button
    virtual CARAPI_(void) PauseAndDispatch(
        /* [in] */ HTML5VideoViewProxy* proxy);

    // Below are functions that are different implementation on inline and full-
    // screen mode. Some are specific to one type, but currently are called
    // directly from the proxy.
    virtual CARAPI_(void) EnterFullScreenVideoState(
        /* [in] */ Int32 layerId,
        /* [in] */ HTML5VideoViewProxy* proxy,
        /* [in] */ CWebViewClassic* webView);

    virtual CARAPI_(Boolean) IsFullScreenMode();

    virtual CARAPI_(void) DecideDisplayMode();

    virtual CARAPI_(Boolean) GetReadyToUseSurfTex();

    virtual CARAPI_(void) DeleteSurfaceTexture();

    virtual CARAPI_(Int32) GetTextureName();

    virtual CARAPI_(Boolean) GetPlayerBuffering();

    virtual CARAPI_(void) SetPlayerBuffering(
        /* [in] */ Boolean playerBuffering);

    virtual CARAPI_(Boolean) FullScreenExited();

    virtual CARAPI_(void) SetStartWhenPrepared(
        /* [in] */ Boolean willPlay);

    virtual CARAPI_(Boolean) GetStartWhenPrepared();

    virtual CARAPI_(void) ShowControllerInFullScreen();

public:
    // This is true only when the player is buffering and paused
    Boolean mPlayerBuffering;

protected:
    HTML5VideoView();

    static CARAPI_(AutoPtr<IObjectStringMap>) GenerateHeaders(
        /* [in] */ const String& url,
        /* [in] */ HTML5VideoViewProxy* proxy);

    virtual CARAPI_(void) SwitchProgressView(
        /* [in] */ Boolean playerBuffering);

protected:
    static const String LOGTAG;

    static const String COOKIE;
    static const String HIDE_URL_LOGS;

    AutoPtr<HTML5VideoViewProxy> mProxy;

    // Save the seek time when not prepared. This can happen when switching
    // video besides initial load.
    Int32 mSaveSeekTime;

    // This is used to find the VideoLayer on the native side.
    Int32 mVideoLayerId;

    // Given the fact we only have one SurfaceTexture, we cannot support multiple
    // player at the same time. We may recreate a new one and abandon the old
    // one at transition time.
    static AutoPtr<IMediaPlayer> mPlayer;
    static Int32 mCurrentState;

    // We need to save such info.
    AutoPtr<IUri> mUri;
    AutoPtr<IObjectStringMap> mHeaders;

    // The timer for timeupate events.
    // See http://www.whatwg.org/specs/web-apps/current-work/#event-media-timeupdate
    static AutoPtr<ITimer> mTimer;

    Boolean mPauseDuringPreparing;

private:
    // The spec says the timer should fire every 250 ms or less.
    static const Int32 TIMEUPDATE_PERIOD = 250;  // ms
    Boolean mSkipPrepare;
    Boolean mStartWhenPrepared;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__HTML5VIDEOVIEW_H__
