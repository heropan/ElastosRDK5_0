
#include "content/CIntent.h"
#include "media/media/CMediaPlayer.h"
#include "net/Uri.h"
#include "webkit/HTML5VideoView.h"
#include "webkit/HTML5VideoViewProxy.h"
#include "webkit/CookieManager.h"
#include "webkit/CWebViewClassic.h"

using Elastos::Core::CStringWrapper;
using Elastos::Core::EIID_IRunnable;
using Elastos::Core::ICharSequence;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Media::CMediaPlayer;
using Elastos::Droid::Media::EIID_IMediaPlayerOnPreparedListener;
using Elastos::Utility::CTimer;
using Elastos::Utility::EIID_ITimerTask;
using Elastos::Utility::CObjectStringMap;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//               HTML5VideoView::TimeupdateTask
//===============================================================

HTML5VideoView::TimeupdateTask::TimeupdateTask(
    /* [in] */ HTML5VideoViewProxy* proxy)
        : mProxy(proxy)
{
}

ECode HTML5VideoView::TimeupdateTask::Run()
{
    mProxy->OnTimeupdate();
    return NOERROR;
}

ECode HTML5VideoView::TimeupdateTask::Cancel(
    /* [out] */ Boolean* value)
{
    return TimerTask::Cancel(value);
}

ECode HTML5VideoView::TimeupdateTask::ScheduledExecutionTime(
    /* [out] */ Int64* value)
{
    return TimerTask::ScheduledExecutionTime(value);
}

ECode HTML5VideoView::TimeupdateTask::GetWhen(
        /* [out] */ Int64* when)
{
    return TimerTask::GetWhen(when);
}

ECode HTML5VideoView::TimeupdateTask::SetWhen(
        /* [in] */ Int64 when)
{
    return TimerTask::SetWhen(when);
}

ECode HTML5VideoView::TimeupdateTask::IsScheduled(
        /* [out] */ Boolean* scheduled)
{
    return TimerTask::IsScheduled(scheduled);
}

ECode HTML5VideoView::TimeupdateTask::IsCancelled(
        /* [out] */ Boolean* cancelled)
{
    return TimerTask::IsCancelled(cancelled);
}

ECode HTML5VideoView::TimeupdateTask::GetPeriod(
        /* [out] */ Int64* period)
{
    return TimerTask::GetPeriod(period);
}

ECode HTML5VideoView::TimeupdateTask::SetPeriod(
        /* [in] */ Int64 period)
{
    return TimerTask::SetPeriod(period);
}

ECode HTML5VideoView::TimeupdateTask::IsFixedRate(
        /* [out] */ Boolean* fixed)
{
    return TimerTask::IsFixedRate(fixed);
}

ECode HTML5VideoView::TimeupdateTask::SetFixedRate(
        /* [in] */ Boolean fixed)
{
    return TimerTask::SetFixedRate(fixed);
}

ECode HTML5VideoView::TimeupdateTask::SetScheduledTime(
            /* [in] */ Int64 time)
{
    return TimerTask::SetScheduledTime(time);
}

ECode HTML5VideoView::TimeupdateTask::Lock()
{
    mLock.Lock();
    return NOERROR;
}

ECode HTML5VideoView::TimeupdateTask::Unlock()
{
    mLock.Unlock();
    return NOERROR;
}

Mutex* HTML5VideoView::TimeupdateTask::GetSelfLock()
{
    return &mLock;
}

//===============================================================
//                        HTML5VideoView
//===============================================================

const Int32 HTML5VideoView::STATE_INITIALIZED;
const Int32 HTML5VideoView::STATE_PREPARING;
const Int32 HTML5VideoView::STATE_PREPARED;
const Int32 HTML5VideoView::STATE_PLAYING;
const Int32 HTML5VideoView::STATE_RESETTED;
const Int32 HTML5VideoView::STATE_RELEASED;
const Int32 HTML5VideoView::TIMEUPDATE_PERIOD;  // ms

const String HTML5VideoView::LOGTAG("HTML5VideoView");
const String HTML5VideoView::COOKIE("Cookie");
const String HTML5VideoView::HIDE_URL_LOGS("x-hide-urls-from-log");

AutoPtr<IMediaPlayer> HTML5VideoView::mPlayer;
Int32 HTML5VideoView::mCurrentState = -1;
AutoPtr<ITimer> HTML5VideoView::mTimer;

HTML5VideoView::HTML5VideoView()
    : mPlayerBuffering(FALSE)
    , mPauseDuringPreparing(FALSE)
    , mSaveSeekTime(0)
    , mVideoLayerId(0)
    , mSkipPrepare(FALSE)
    , mStartWhenPrepared(FALSE)
{
}

CAR_INTERFACE_IMPL(HTML5VideoView, IMediaPlayerOnPreparedListener);

// common Video control FUNCTIONS:
ECode HTML5VideoView::Start()
{
    if (mCurrentState == STATE_PREPARED) {
        // When replaying the same video, there is no onPrepared call.
        // Therefore, the timer should be set up here.
        if (mTimer == NULL)
        {
            CTimer::New((ITimer**)&mTimer);
            AutoPtr<ITimerTask> task = new TimeupdateTask(mProxy);
            mTimer->Schedule(task, TIMEUPDATE_PERIOD,
                    TIMEUPDATE_PERIOD);
        }

        mPlayer->Start();
        SetPlayerBuffering(FALSE);
    }

    return NOERROR;
}

ECode HTML5VideoView::Pause()
{
    if (IsPlaying()) {
        mPlayer->Pause();
    }
    else if (mCurrentState == STATE_PREPARING) {
        mPauseDuringPreparing = TRUE;
    }
    // Delete the Timer to stop it since there is no stop call.
    if (mTimer != NULL) {
        Int32 value;
        mTimer->Purge(&value);
        mTimer->Cancel();
        mTimer = NULL;
    }

    return NOERROR;
}

Int32 HTML5VideoView::GetDuration()
{
    if (mCurrentState == STATE_PREPARED) {
        Int32 duration;
        mPlayer->GetDuration(&duration);
        return duration;
    }
    else {
        return -1;
    }
}

Int32 HTML5VideoView::GetCurrentPosition()
{
    if (mCurrentState == STATE_PREPARED) {
        Int32 postion;
        mPlayer->GetCurrentPosition(&postion);
        return postion;
    }

    return 0;
}

ECode HTML5VideoView::SeekTo(
    /* [in] */ Int32 pos)
{
    if (mCurrentState == STATE_PREPARED) {
        mPlayer->SeekTo(pos);
    }
    else {
        mSaveSeekTime = pos;
    }

    return NOERROR;
}

Boolean HTML5VideoView::IsPlaying()
{
    if (mCurrentState == STATE_PREPARED) {
        Boolean bFlag = FALSE;
        mPlayer->IsPlaying(&bFlag);
        return bFlag;
    }
    else {
        return FALSE;
    }
}

void HTML5VideoView::Reset()
{
    if (mCurrentState < STATE_RESETTED) {
        mPlayer->Reset();
    }
    mCurrentState = STATE_RESETTED;
}

void HTML5VideoView::StopPlayback()
{
    if (mCurrentState == STATE_PREPARED) {
        mPlayer->Stop();
    }
}

void HTML5VideoView::ReleaseResources()
{
    if (mPlayer != NULL && mCurrentState != STATE_RELEASED) {
        mPlayer->Release();
        mPlayer = NULL;
    }
    mCurrentState = STATE_RELEASED;
}

Boolean HTML5VideoView::IsReleased()
{
    return mCurrentState == STATE_RELEASED;
}

Boolean HTML5VideoView::GetPauseDuringPreparing()
{
    return mPauseDuringPreparing;
}

// Every time we start a new Video, we create a VideoView and a MediaPlayer
void HTML5VideoView::Init(
    /* [in] */ Int32 videoLayerId,
    /* [in] */ Int32 position,
    /* [in] */ Boolean skipPrepare)
{
    if (mPlayer == NULL) {
        CMediaPlayer::New((IMediaPlayer**)&mPlayer);
        mCurrentState = STATE_INITIALIZED;
    }
    mSkipPrepare = skipPrepare;
    // If we want to skip the prepare, then we keep the state.
    if (!mSkipPrepare) {
        mCurrentState = STATE_INITIALIZED;
    }
    mProxy = NULL;
    mVideoLayerId = videoLayerId;
    mSaveSeekTime = position;
    mTimer = NULL;
    mPauseDuringPreparing = FALSE;
}

void HTML5VideoView::SetVideoURI(
    /* [in] */ const String& uri,
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    // When switching players, surface texture will be reused.
    mUri = NULL;
    Uri::Parse(uri, (IUri**)&mUri);
    mHeaders = GenerateHeaders(uri, proxy);
}

// Listeners setup FUNCTIONS:
void HTML5VideoView::SetOnCompletionListener(
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    mPlayer->SetOnCompletionListener(proxy);
}

void HTML5VideoView::SetOnErrorListener(
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    mPlayer->SetOnErrorListener(proxy);
}

void HTML5VideoView::SetOnPreparedListener(
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    mProxy = proxy;
    mPlayer->SetOnPreparedListener(this);
}

void HTML5VideoView::SetOnInfoListener(
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    mPlayer->SetOnInfoListener(proxy);
}

void HTML5VideoView::PrepareDataCommon(
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    if (!mSkipPrepare) {
        //try {
            mPlayer->Reset();
            mPlayer->SetDataSource(proxy->GetContext(), mUri, mHeaders);
            mPlayer->PrepareAsync();
        //} catch (IllegalArgumentException e) {
        //    e.printStackTrace();
        //} catch (IllegalStateException e) {
        //    e.printStackTrace();
        //} catch (IOException e) {
        //    e.printStackTrace();
        //}
        mCurrentState = STATE_PREPARING;
    }
    else {
        // If we skip prepare and the onPrepared happened in inline mode, we
        // don't need to call prepare again, we just need to call onPrepared
        // to refresh the state here.
        if (mCurrentState >= STATE_PREPARED) {
            OnPrepared(mPlayer);
        }
        mSkipPrepare = FALSE;
    }
}

void HTML5VideoView::ReprepareData(
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    mPlayer->Reset();
    PrepareDataCommon(proxy);
}

// Normally called immediately after setVideoURI. But for full screen,
// this should be after surface holder created
void HTML5VideoView::PrepareDataAndDisplayMode(
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    // SurfaceTexture will be created lazily here for inline mode
    DecideDisplayMode();

    SetOnCompletionListener(proxy);
    SetOnPreparedListener(proxy);
    SetOnErrorListener(proxy);
    SetOnInfoListener(proxy);

    PrepareDataCommon(proxy);
}

// Common code
Int32 HTML5VideoView::GetVideoLayerId()
{
    return mVideoLayerId;
}

Int32 HTML5VideoView::GetCurrentState()
{
    if (IsPlaying()) {
        return STATE_PLAYING;
    }
    else {
        return mCurrentState;
    }
}

ECode HTML5VideoView::OnPrepared(
    /* [in] */ IMediaPlayer* mp)
{
    mCurrentState = STATE_PREPARED;
    SeekTo(mSaveSeekTime);
    if (mProxy != NULL) {
        mProxy->OnPrepared(mp);
    }
    if (mPauseDuringPreparing) {
        PauseAndDispatch(mProxy);
        mPauseDuringPreparing = FALSE;
    }
}

// Pause the play and update the play/pause button
void HTML5VideoView::PauseAndDispatch(
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    Pause();
    if (proxy != NULL) {
        proxy->DispatchOnPaused();
    }
}

// Below are functions that are different implementation on inline and full-
// screen mode. Some are specific to one type, but currently are called
// directly from the proxy.
void HTML5VideoView::EnterFullScreenVideoState(
    /* [in] */ Int32 layerId,
    /* [in] */ HTML5VideoViewProxy* proxy,
    /* [in] */ CWebViewClassic* webView)
{
}

Boolean HTML5VideoView::IsFullScreenMode()
{
    return FALSE;
}

void HTML5VideoView::DecideDisplayMode()
{
}

Boolean HTML5VideoView::GetReadyToUseSurfTex()
{
    return FALSE;
}

void HTML5VideoView::DeleteSurfaceTexture()
{
}

Int32 HTML5VideoView::GetTextureName()
{
    return 0;
}

Boolean HTML5VideoView::GetPlayerBuffering()
{
    return mPlayerBuffering;
}

void HTML5VideoView::SetPlayerBuffering(
    /* [in] */ Boolean playerBuffering)
{
    mPlayerBuffering = playerBuffering;
    SwitchProgressView(playerBuffering);
}

Boolean HTML5VideoView::FullScreenExited()
{
    // Only meaningful for HTML5VideoFullScreen
    return FALSE;
}

void HTML5VideoView::SetStartWhenPrepared(
    /* [in] */ Boolean willPlay)
{
    mStartWhenPrepared  = willPlay;
}

Boolean HTML5VideoView::GetStartWhenPrepared()
{
    return mStartWhenPrepared;
}

void HTML5VideoView::ShowControllerInFullScreen()
{
}

AutoPtr<IObjectStringMap> HTML5VideoView::GenerateHeaders(
    /* [in] */ const String& url,
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    Boolean isPrivate = FALSE;
    proxy->GetWebView()->IsPrivateBrowsingEnabled(&isPrivate);
    AutoPtr<ICookieManager> cm = CookieManager::GetInstance();
    String cookieValue;
    cm->GetCookie(url, isPrivate, &cookieValue);
    AutoPtr<IObjectStringMap> headers;
    CObjectStringMap::New((IObjectStringMap**)&headers);
    if (cookieValue != NULL) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(cookieValue, (ICharSequence**)&cs);
        headers->Put(COOKIE, cs);
    }
    if (isPrivate) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(String("true"), (ICharSequence**)&cs);
        headers->Put(HIDE_URL_LOGS, cs);
    }

    return headers;
}

void HTML5VideoView::SwitchProgressView(
    /* [in] */ Boolean playerBuffering)
{
    // Only used in HTML5VideoFullScreen
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos