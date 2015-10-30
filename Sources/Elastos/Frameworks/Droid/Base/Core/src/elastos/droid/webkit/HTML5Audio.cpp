
#include "elastos/droid/webkit/CookieManager.h"
#include "elastos/droid/webkit/HTML5Audio.h"
#include "elastos/droid/webkit/CWebViewCore.h"
#include "elastos/droid/webkit/CWebViewClassic.h"
#include "elastos/droid/media/CMediaPlayer.h"
#include "elastos/droid/os/CHandler.h"
#include "elastos/droid/os/CMessageHelper.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::CStringWrapper;
using Elastos::Core::EIID_IRunnable;
using Elastos::Core::ICharSequence;
using Elastos::Droid::Media::IAudioManager;
using Elastos::Droid::Media::CMediaPlayer;
using Elastos::Droid::Media::EIID_IOnAudioFocusChangeListener;
using Elastos::Droid::Media::EIID_IMediaPlayerOnErrorListener;
using Elastos::Droid::Media::EIID_IMediaPlayerOnPreparedListener;
using Elastos::Droid::Media::EIID_IMediaPlayerOnCompletionListener;
using Elastos::Droid::Media::EIID_IMediaPlayerOnSeekCompleteListener;
using Elastos::Droid::Media::EIID_IMediaPlayerOnBufferingUpdateListener;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Utility::CObjectStringMap;
using Elastos::Utility::IObjectStringMap;
using Elastos::Utility::CTimer;
using Elastos::Utility::EIID_ITimerTask;
using Elastos::Utility::Logging::Logger;

#ifdef __cplusplus
extern "C"
{
#endif
    extern void Elastos_MediaPlayerPrivateAndroid_OnBuffering(Int32 percent, Int32 pointer);
    extern void Elastos_MediaPlayerPrivateAndroid_OnEnded(Int32 pointer);
    extern void Elastos_MediaPlayerPrivateAndroid_OnRequestPlay(Int32 pointer);
    extern void Elastos_MediaPlayerPrivateAndroid_OnPrepared(Int32 duration, Int32 width, Int32 height, Int32 pointer);
    extern void Elastos_MediaPlayerPrivateAndroid_OnTimeupdate(Int32 position, Int32 pointer);
#ifdef __cplusplus
}
#endif


namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//                  HTML5Audio::TimeupdateTask
//===============================================================

HTML5Audio::TimeupdateTask::TimeupdateTask(
    /* [in] */ HTML5Audio* owner)
    : mOwner(owner)
{
}

ECode HTML5Audio::TimeupdateTask::Run()
{
    AutoPtr<IMessage> msg;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(mOwner, TIMEUPDATE, (IMessage**)&msg);
    return msg->SendToTarget();
}

ECode HTML5Audio::TimeupdateTask::Cancel(
    /* [out] */ Boolean* value)
{
    return TimerTask::Cancel(value);
}

ECode HTML5Audio::TimeupdateTask::ScheduledExecutionTime(
    /* [out] */ Int64* value)
{
    return TimerTask::ScheduledExecutionTime(value);
}

ECode HTML5Audio::TimeupdateTask::GetWhen(
        /* [out] */ Int64* when)
{
    return TimerTask::GetWhen(when);
}

ECode HTML5Audio::TimeupdateTask::SetWhen(
        /* [in] */ Int64 when)
{
    return TimerTask::SetWhen(when);
}

ECode HTML5Audio::TimeupdateTask::IsScheduled(
        /* [out] */ Boolean* scheduled)
{
    return TimerTask::IsScheduled(scheduled);
}

ECode HTML5Audio::TimeupdateTask::IsCancelled(
        /* [out] */ Boolean* cancelled)
{
    return TimerTask::IsCancelled(cancelled);
}

ECode HTML5Audio::TimeupdateTask::GetPeriod(
        /* [out] */ Int64* period)
{
    return TimerTask::GetPeriod(period);
}

ECode HTML5Audio::TimeupdateTask::SetPeriod(
        /* [in] */ Int64 period)
{
    return TimerTask::SetPeriod(period);
}

ECode HTML5Audio::TimeupdateTask::IsFixedRate(
        /* [out] */ Boolean* fixed)
{
    return TimerTask::IsFixedRate(fixed);
}

ECode HTML5Audio::TimeupdateTask::SetFixedRate(
        /* [in] */ Boolean fixed)
{
    return TimerTask::SetFixedRate(fixed);
}

ECode HTML5Audio::TimeupdateTask::SetScheduledTime(
            /* [in] */ Int64 time)
{
    return TimerTask::SetScheduledTime(time);
}

ECode HTML5Audio::TimeupdateTask::Lock()
{
    mLock.Lock();
    return NOERROR;
}

ECode HTML5Audio::TimeupdateTask::Unlock()
{
    mLock.Unlock();
    return NOERROR;
}

Object* HTML5Audio::TimeupdateTask::GetSelfLock()
{
    return &mLock;
}

ECode HTML5Audio::TimeupdateTask::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "HTML5Audio::TimeupdateTask";
    return NOERROR;
}

//========================================================================
//       HTML5Audio::IsPrivateBrowsingEnabledGetter::InnerRunnable
//========================================================================

HTML5Audio::IsPrivateBrowsingEnabledGetter::InnerRunnable::InnerRunnable(
    /* [in] */ IsPrivateBrowsingEnabledGetter* owner,
    /* [in] */ CWebViewClassic* webView)
    : mOwner(owner)
    , mWebView(webView)
{
}

CAR_INTERFACE_IMPL_LIGHT(HTML5Audio::IsPrivateBrowsingEnabledGetter::InnerRunnable, IRunnable);

ECode HTML5Audio::IsPrivateBrowsingEnabledGetter::InnerRunnable::Run()
{
    IsPrivateBrowsingEnabledGetter::AutoLock lock(mOwner);

    mWebView->IsPrivateBrowsingEnabled(&(mOwner->mIsPrivateBrowsingEnabled));
    mOwner->mIsReady = TRUE;
    mOwner->Notify();

    return NOERROR;
}

ECode HTML5Audio::IsPrivateBrowsingEnabledGetter::InnerRunnable::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "HTML5Audio::IsPrivateBrowsingEnabledGetter::InnerRunnable";
    return NOERROR;
}

//===============================================================
//           HTML5Audio::IsPrivateBrowsingEnabledGetter
//===============================================================

HTML5Audio::IsPrivateBrowsingEnabledGetter::IsPrivateBrowsingEnabledGetter(
    /* [in] */ HTML5Audio* owner,
    /* [in] */ ILooper* uiThreadLooper,
    /* [in] */ CWebViewClassic* webView)
    : mOwner(owner)
    , mIsReady(FALSE)
    , mIsPrivateBrowsingEnabled(FALSE)
{
    AutoPtr<IRunnable> runnable = new InnerRunnable(this, webView);
    AutoPtr<IHandler> handler;
    CHandler::New(uiThreadLooper, (IHandler**)&handler);
    Boolean result = FALSE;
    handler->Post(runnable, &result);
}

//synchronized
Boolean HTML5Audio::IsPrivateBrowsingEnabledGetter::Get()
{
    AutoLock lock(this);

    while (!mIsReady) {
//        try {
            Wait();
//        } catch (InterruptedException e) {
//        }
    }
    return mIsPrivateBrowsingEnabled;
}

ECode HTML5Audio::IsPrivateBrowsingEnabledGetter::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "HTML5Audio::IsPrivateBrowsingEnabledGetter";
    return NOERROR;
}

//===============================================================
//                         HTML5Audio
//===============================================================

const String HTML5Audio::LOGTAG("HTML5Audio");
const String HTML5Audio::COOKIE("Cookie");
const String HTML5Audio::HIDE_URL_LOGS("x-hide-urls-from-log");

Int32 HTML5Audio::IDLE        =  0;
Int32 HTML5Audio::INITIALIZED =  1;
Int32 HTML5Audio::PREPARED    =  2;
Int32 HTML5Audio::STARTED     =  4;
Int32 HTML5Audio::COMPLETE    =  5;
Int32 HTML5Audio::PAUSED      =  6;
Int32 HTML5Audio::STOPPED     = -2;
Int32 HTML5Audio::ERROR       = -1;

const Int32 HTML5Audio::TIMEUPDATE;
const Int32 HTML5Audio::TIMEUPDATE_PERIOD;

/**
 * @param nativePtr is the C++ pointer to the MediaPlayerPrivate object.
 */
HTML5Audio::HTML5Audio(
    /* [in] */ CWebViewCore* webViewCore,
    /* [in] */ Int32 nativePtr)
    : mNativePointer(nativePtr)
    , mContext(webViewCore->GetContext())
    , mState(IDLE)
    , mAskToPlay(FALSE)
    , mLoopEnabled(FALSE)
    , mProcessingOnEnd(FALSE)
{
    ResetMediaPlayer();
    AutoPtr<IContext> context = webViewCore->GetContext();
    AutoPtr<ILooper> looper;
    context->GetMainLooper((ILooper**)&looper);
    mIsPrivateBrowsingEnabledGetter = new IsPrivateBrowsingEnabledGetter(this,
            looper, webViewCore->GetWebViewClassic());
}

UInt32 HTML5Audio::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 HTML5Audio::Release()
{
    return ElRefBase::Release();
}

ECode HTML5Audio::GetInterfaceID(
    /* [in] */ IInterface *Object,
    /* [out] */ InterfaceID *IID)
{
    VALIDATE_NOT_NULL(IID);

    if (Object == (IInterface*)(IHandler*)this) {
        *IID = EIID_IHandler;
    }
    else if (Object == (IInterface*)(IMediaPlayerOnBufferingUpdateListener*)this) {
        *IID = EIID_IMediaPlayerOnBufferingUpdateListener;
    }
    else if (Object == (IInterface*)(IMediaPlayerOnCompletionListener*)this) {
        *IID = EIID_IMediaPlayerOnCompletionListener;
    }
    else if (Object == (IInterface*)(IMediaPlayerOnErrorListener*)this) {
        *IID = EIID_IMediaPlayerOnErrorListener;
    }
    else if (Object == (IInterface*)(IMediaPlayerOnPreparedListener*)this) {
        *IID = EIID_IMediaPlayerOnPreparedListener;
    }
    else if (Object == (IInterface*)(IMediaPlayerOnSeekCompleteListener*)this) {
        *IID = EIID_IMediaPlayerOnSeekCompleteListener;
    }
    else if (Object == (IInterface*)(IOnAudioFocusChangeListener*)this) {
        *IID = EIID_IOnAudioFocusChangeListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

PInterface HTML5Audio::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IHandler) {
        return (IHandler*)this;
    }
    else if (riid == EIID_IMediaPlayerOnBufferingUpdateListener) {
        return (IMediaPlayerOnBufferingUpdateListener*)this;
    }
    else if (riid == EIID_IMediaPlayerOnCompletionListener) {
        return (IMediaPlayerOnCompletionListener*)this;
    }
    else if (riid == EIID_IMediaPlayerOnErrorListener) {
        return (IMediaPlayerOnErrorListener*)this;
    }
    else if (riid == EIID_IMediaPlayerOnPreparedListener) {
        return (IMediaPlayerOnPreparedListener*)this;
    }
    else if (riid == EIID_IMediaPlayerOnSeekCompleteListener) {
        return (IMediaPlayerOnSeekCompleteListener*)this;
    }
    else if (riid == EIID_IOnAudioFocusChangeListener) {
        return (IOnAudioFocusChangeListener*)this;
    }

    return NULL;
}

ECode HTML5Audio::HandleMessage(
        /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    Int32 what;
    msg->GetWhat(&what);
    switch (what)
    {
        case TIMEUPDATE: {
            Boolean isPlaying = FALSE;
            if (mState != ERROR && mMediaPlayer->IsPlaying(&isPlaying), isPlaying) {
                Int32 position;
                mMediaPlayer->GetCurrentPosition(&position);
                NativeOnTimeupdate(position, mNativePointer);
            }
        }
    }

    return NOERROR;
}

// MediaPlayer.OnBufferingUpdateListener
ECode HTML5Audio::OnBufferingUpdate(
    /* [in] */ IMediaPlayer* mp,
    /* [in] */ Int32 percent)
{
    NativeOnBuffering(percent, mNativePointer);

    return NOERROR;
}

// MediaPlayer.OnCompletionListener;
ECode HTML5Audio::OnCompletion(
    /* [in] */ IMediaPlayer* mp)
{
    mState = COMPLETE;
    mProcessingOnEnd = TRUE;
    NativeOnEnded(mNativePointer);
    mProcessingOnEnd = FALSE;
    if (mLoopEnabled == TRUE) {
        NativeOnRequestPlay(mNativePointer);
        mLoopEnabled = FALSE;
    }

    return NOERROR;
}

// MediaPlayer.OnErrorListener
ECode HTML5Audio::OnError(
    /* [in] */ IMediaPlayer* mp,
    /* [in] */ Int32 what,
    /* [in] */ Int32 extra,
    /* [out] */ Boolean* result)
{
    mState = ERROR;
    ResetMediaPlayer();
    mState = IDLE;

    if (result) *result = FALSE;

    return NOERROR;
}

// MediaPlayer.OnPreparedListener
ECode HTML5Audio::OnPrepared(
    /* [in] */ IMediaPlayer* mp)
{
    mState = PREPARED;
    if (mTimer != NULL) {
        AutoPtr<ITimerTask> task = new TimeupdateTask(this);
        mTimer->Schedule(task, TIMEUPDATE_PERIOD, TIMEUPDATE_PERIOD);
    }

    Int32 duration;
    mp->GetDuration(&duration);
    NativeOnPrepared(duration, 0, 0, mNativePointer);
    if (mAskToPlay) {
        mAskToPlay = FALSE;
        Play();
    }

    return NOERROR;
}

// MediaPlayer.OnSeekCompleteListener
ECode HTML5Audio::OnSeekComplete(
    /* [in] */ IMediaPlayer* mp)
{
    Int32 position;
    mp->GetCurrentPosition(&position);
    NativeOnTimeupdate(position, mNativePointer);

    return NOERROR;
}

ECode HTML5Audio::OnAudioFocusChange(
    /* [in] */ Int32 focusChange)
{
    Boolean bFlag = FALSE;
    mMediaPlayer->IsPlaying(&bFlag);
    switch (focusChange) {
    case IAudioManager::AUDIOFOCUS_GAIN:
        // resume playback
        if (mMediaPlayer == NULL) {
            ResetMediaPlayer();
        }
        else if (mState != ERROR && !bFlag) {
            mMediaPlayer->Start();
            mState = STARTED;
        }
        break;

    case IAudioManager::AUDIOFOCUS_LOSS:
        // Lost focus for an unbounded amount of time: stop playback.
        if (mState != ERROR && bFlag) {
            mMediaPlayer->Stop();
            mState = STOPPED;
        }
        break;

    case IAudioManager::AUDIOFOCUS_LOSS_TRANSIENT:
    case IAudioManager::AUDIOFOCUS_LOSS_TRANSIENT_CAN_DUCK:
        // Lost focus for a short time, but we have to stop
        // playback.
        if (mState != ERROR && bFlag) Pause();
        break;
    }

    return NOERROR;
}

void HTML5Audio::ResetMediaPlayer()
{
    if (mMediaPlayer == NULL) {
        CMediaPlayer::New((IMediaPlayer**)&mMediaPlayer);
    }
    else {
        mMediaPlayer->Reset();
    }

    mMediaPlayer->SetOnBufferingUpdateListener(this);
    mMediaPlayer->SetOnCompletionListener(this);
    mMediaPlayer->SetOnErrorListener(this);
    mMediaPlayer->SetOnPreparedListener(this);
    mMediaPlayer->SetOnSeekCompleteListener(this);

    if (mTimer != NULL) {
        mTimer->Cancel();
    }
    mTimer = NULL;
    CTimer::New((ITimer**)&mTimer);

    mState = IDLE;
}

void HTML5Audio::SetDataSource(
    /* [in] */ const String& url)
{
    mUrl = url;
    //try {
        if (mState != IDLE) {
            ResetMediaPlayer();
        }

        String cookieValue;
        CookieManager::GetInstance()->GetCookie(
                url, mIsPrivateBrowsingEnabledGetter->Get(), &cookieValue);
        AutoPtr<IObjectStringMap> headers;
        CObjectStringMap::New((IObjectStringMap**)&headers);
        if (cookieValue != NULL) {
            AutoPtr<ICharSequence> cookie;
            CStringWrapper::New(cookieValue, (ICharSequence**)&cookie);
            headers->Put(COOKIE, cookie);
        }
        if (mIsPrivateBrowsingEnabledGetter->Get()) {
            AutoPtr<ICharSequence> value;
            CStringWrapper::New(String("true"), (ICharSequence**)&value);
            headers->Put(HIDE_URL_LOGS, value);
        }

        mMediaPlayer->SetDataSource(url, headers);
        mState = INITIALIZED;
        mMediaPlayer->PrepareAsync();
    //} catch (IOException e) {
    //    String debugUrl = url.length() > 128 ? url.substring(0, 128) + "..." : url;
    //    Log.e(LOGTAG, "couldn't load the resource: "+ debugUrl +" exc: " + e);
    //    resetMediaPlayer();
    //}
}

void HTML5Audio::Play()
{
    if (mState == COMPLETE && mLoopEnabled == TRUE) {
        // Play it again, Sam
        mMediaPlayer->Start();
        mState = STARTED;
        return;
    }

    if (((mState >= ERROR && mState < PREPARED)) && !mUrl.IsNullOrEmpty()) {
        ResetMediaPlayer();
        SetDataSource(mUrl);
        mAskToPlay = TRUE;
    }

    if (mState >= PREPARED) {
        AutoPtr<IInterface> audioService;
        mContext->GetSystemService(IContext::AUDIO_SERVICE, (IInterface**)&audioService);
        AutoPtr<IAudioManager> audioManager = IAudioManager::Probe(audioService);

        Int32 result;
        audioManager->RequestAudioFocus(this, IAudioManager::STREAM_MUSIC,
            IAudioManager::AUDIOFOCUS_GAIN, &result);

        if (result == IAudioManager::AUDIOFOCUS_REQUEST_GRANTED) {
            mMediaPlayer->Start();
            mState = STARTED;
        }
    }
}

void HTML5Audio::Pause()
{
    if (mState == STARTED) {
        if (mTimer != NULL) {
            Int32 value;
            mTimer->Purge(&value);
        }
        mMediaPlayer->Pause();
        mState = PAUSED;
    }
}

void HTML5Audio::Seek(
    /* [in] */ Int32 msec)
{
    if (mProcessingOnEnd == TRUE && mState == COMPLETE && msec == 0) {
        mLoopEnabled = TRUE;
    }
    if (mState >= PREPARED) {
        mMediaPlayer->SeekTo(msec);
    }
}

/**
 * Called only over JNI when WebKit is happy to
 * destroy the media player.
 */
void HTML5Audio::Teardown()
{
    if (mTimer != NULL) {
        mTimer->Cancel();
    }
    mTimer = NULL;

    mMediaPlayer->ReleaseResources();
    mMediaPlayer = NULL;
    mState = ERROR;
    mNativePointer = 0;
}

Float HTML5Audio::GetMaxTimeSeekable()
{
    if (mState >= PREPARED) {
        Int32 duration;
        mMediaPlayer->GetDuration(&duration);
        return duration / 1000.0f;
    }
    else {
        return 0.0f;
    }
}

void HTML5Audio::NativeOnBuffering(
    /* [in] */ Int32 percent,
    /* [in] */ Int32 pointer)
{
    if (pointer) {
        Elastos_MediaPlayerPrivateAndroid_OnBuffering(percent, pointer);
    }
}

void HTML5Audio::NativeOnEnded(
    /* [in] */ Int32 pointer)
{
    //Logger::V(LOGTAG, "HTML5Audio::NativeOnEnded, point:0x%x", pointer);
    if (pointer) {
        Elastos_MediaPlayerPrivateAndroid_OnEnded(pointer);
    }
}

void HTML5Audio::NativeOnRequestPlay(
    /* [in] */ Int32 pointer)
{
    //Logger::V(LOGTAG, "HTML5Audio::NativeOnRequestPlay, point:0x%x", pointer);
    if (pointer) {
        Elastos_MediaPlayerPrivateAndroid_OnRequestPlay(pointer);
    }
}

void HTML5Audio::NativeOnPrepared(
    /* [in] */ Int32 duration,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 pointer)
{
    //Logger::V(LOGTAG, "HTML5Audio::NativeOnPrepared, duration:%d,w:%d,h:%d,point:0x%x", duration, width, height,pointer);
    if (pointer) {
        Elastos_MediaPlayerPrivateAndroid_OnPrepared(duration, width, height, pointer);
    }
}

void HTML5Audio::NativeOnTimeupdate(
    /* [in] */ Int32 position,
    /* [in] */ Int32 pointer)
{
    //Logger::V(LOGTAG, "HTML5Audio::NativeOnTimeupdate, position:%d,point:0x%x", position,pointer);
    if (pointer) {
        Elastos_MediaPlayerPrivateAndroid_OnTimeupdate(position, pointer);
    }
}

ECode HTML5Audio::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "HTML5Audio";
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
