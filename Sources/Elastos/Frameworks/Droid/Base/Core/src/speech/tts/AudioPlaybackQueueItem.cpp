#include "speech/tts/AudioPlaybackQueueItem.h"
//#include "os/CConditionVariable.h"
#include <elastos/Logger.h>
#include "ext/frameworkext.h"
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::EIID_IRunnable;
using Elastos::Utility::Logging::Logger;
//using Elastos::Droid::media::EIID_IMediaPlayerOnErrorListener;
//using Elastos::Droid::media::EIID_IMediaPlayerOnCompletionListener;
using Elastos::Droid::Speech::Tts::ITextToSpeechServiceUtteranceProgressDispatcher;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/******************************AudioPlaybackQueueItem::MediaPlayerOnErrorListener*************************/
PInterface AudioPlaybackQueueItem::MediaPlayerOnErrorListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
//        return (IInterface*)(IMediaPlayerOnErrorListener*)this;
    }
//    else if (riid == EIID_IMediaPlayerOnErrorListener) {
//        return (IMediaPlayerOnErrorListener*)this;
//    }
    return NULL;
}

UInt32 AudioPlaybackQueueItem::MediaPlayerOnErrorListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AudioPlaybackQueueItem::MediaPlayerOnErrorListener::Release()
{
    return ElRefBase::Release();
}

ECode AudioPlaybackQueueItem::MediaPlayerOnErrorListener::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

//    if (Object == (IInterface*)(IMediaPlayerOnErrorListener*)this) {
//        *iID = EIID_IMediaPlayerOnErrorListener;
//    }
//    else {
        return E_INVALID_ARGUMENT;
//    }
    return NOERROR;
}

AudioPlaybackQueueItem::MediaPlayerOnErrorListener::MediaPlayerOnErrorListener(
    /* [in] */ AudioPlaybackQueueItem* apqi)
{
    mApqi = apqi;
}

ECode AudioPlaybackQueueItem::MediaPlayerOnErrorListener::OnError(
//    /* [in] */ IMediaPlayer* mp,
    /* [in] */ Int32 what,
    /* [in] */ Int32 extra,
    /* [out] */ Boolean* ret)
{
    //Java:    Log.w(TAG, "Audio playback error: " + what + ", " + extra);
    Logger::W(TAG, String("Audio playback error: ") + StringUtils::Int32ToString(what) + String(", ") + StringUtils::Int32ToString(extra) + String("\n"));
//    (mApqi->mDone)->Open();
    *ret = TRUE;
    return NOERROR;
}

/******************************AudioPlaybackQueueItem::MediaPlayerOnCompletionListener*************************/
PInterface AudioPlaybackQueueItem::MediaPlayerOnCompletionListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
//        return (IInterface*)(IMediaPlayerOnCompletionListener*)this;
    }
//    else if (riid == EIID_IMediaPlayerOnCompletionListener) {
//        return (IMediaPlayerOnCompletionListener*)this;
//    }
    return NULL;
}

UInt32 AudioPlaybackQueueItem::MediaPlayerOnCompletionListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AudioPlaybackQueueItem::MediaPlayerOnCompletionListener::Release()
{
    return ElRefBase::Release();
}

ECode AudioPlaybackQueueItem::MediaPlayerOnCompletionListener::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

//    if (Object == (IInterface*)(IMediaPlayerOnCompletionListener*)this) {
//        *iID = EIID_IMediaPlayerOnCompletionListener;
//    }
//    else {
        return E_INVALID_ARGUMENT;
//    }
    return NOERROR;
}

AudioPlaybackQueueItem::MediaPlayerOnCompletionListener::MediaPlayerOnCompletionListener(
    /* [in] */ AudioPlaybackQueueItem* apqi)
{
    mApqi = apqi;
}

ECode AudioPlaybackQueueItem::MediaPlayerOnCompletionListener::OnCompletion(
//    /* [in] */ IMediaPlayer* mp
    )
{
    mApqi->mFinished = TRUE;
//    (mApqi->mDone)->Open();
    return NOERROR;
}

/******************************AudioPlaybackQueueItem*************************/
const CString AudioPlaybackQueueItem::TAG = "TTS.AudioQueueItem";

PInterface AudioPlaybackQueueItem::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IRunnable*)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }
    return NULL;
}

UInt32 AudioPlaybackQueueItem::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AudioPlaybackQueueItem::Release()
{
    return ElRefBase::Release();
}

ECode AudioPlaybackQueueItem::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IRunnable*)this) {
        *iID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

AudioPlaybackQueueItem::AudioPlaybackQueueItem(
    /* [in] */ ITextToSpeechServiceUtteranceProgressDispatcher* dispatcher,
    /* [in] */ IInterface* callerIdentity,
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 streamType) : PlaybackQueueItem(dispatcher, callerIdentity)
{
    mContext = context;
    mUri = uri;
    mStreamType = streamType;

//    CConditionVariable::New((IConditionVariable**)&mDone);
//    mPlayer = NULL;
    mFinished = FALSE;
}

ECode AudioPlaybackQueueItem::Run()
{
    AutoPtr<ITextToSpeechServiceUtteranceProgressDispatcher> dispatcher = GetDispatcher();
    dispatcher->DispatchOnStart();
//    mPlayer = CMediaPlayer::Create(mContext, mUri);
    if (/*mPlayer == NULL*/FALSE) {
        dispatcher->DispatchOnError();
        return NOERROR;
    }

    //try {
        AutoPtr<MediaPlayerOnErrorListener> mpoer = new MediaPlayerOnErrorListener(this);
//        mPlayer->SetOnErrorListener(mpoer);
        AutoPtr<MediaPlayerOnCompletionListener> mpocl = new MediaPlayerOnCompletionListener(this);
//        mPlayer->SetOnCompletionListener(mpocl);
//        mPlayer->SetAudioStreamType(mStreamType);
//        mPlayer->Start();
//        mDone->Block();
        Finish();
    //} catch (IllegalArgumentException ex) {
        //Java:    Log.w(TAG, "MediaPlayer failed", ex);
        /*
        Logger::W(TAG, String("MediaPlayer failed\n"));
        mDone->Open();
        */
    //}

    if (mFinished) {
        dispatcher->DispatchOnDone();
    } else {
        dispatcher->DispatchOnError();
    }
    return NOERROR;
}

void AudioPlaybackQueueItem::Finish()
{
    //try {
//        mPlayer->Stop();
    //} catch (IllegalStateException ex) {
        // Do nothing, the player is already stopped
    //}
//    mPlayer->Release();
}

ECode AudioPlaybackQueueItem::Stop(
    /* [in] */ Boolean isError)
{
//    mDone->Open();
    return NOERROR;
}

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
