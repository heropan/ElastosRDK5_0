#include "speech/tts/PlaybackSynthesisCallback.h"
#include "speech/tts/BlockingAudioTrack.h"
#include "speech/tts/SynthesisPlaybackQueueItem.h"
#include <elastos/Logger.h>
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::Threading::Mutex;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

const CString PlaybackSynthesisCallback::TAG = "PlaybackSynthesisRequest";
const Boolean PlaybackSynthesisCallback::DBG = FALSE;

const Int32 PlaybackSynthesisCallback::MIN_AUDIO_BUFFER_SIZE = 8192;

PlaybackSynthesisCallback::PlaybackSynthesisCallback(
    /* [in] */ Int32 streamType,
    /* [in] */ Float volume,
    /* [in] */ Float pan,
    /* [in] */ AudioPlaybackHandler* audioTrackHandler,
    /* [in] */ ITextToSpeechServiceUtteranceProgressDispatcher* dispatcher,
    /* [in] */ IInterface* callerIdentity,
    /* [in] */ EventLogger* logger)
{
    mStreamType = streamType;
    mVolume = volume;
    mPan = pan;
    mAudioTrackHandler = audioTrackHandler;
    mDispatcher = dispatcher;
    mCallerIdentity = callerIdentity;
    mLogger = logger;

    mStopped = FALSE;
    mDone = FALSE;
}

void PlaybackSynthesisCallback::Stop()
{
    StopImpl(FALSE);
}

void PlaybackSynthesisCallback::StopImpl(
    /* [in] */ Boolean wasError)
{
    if (DBG){
        //Java:    Log.d(TAG, "stop()");
        Logger::D(TAG, String("stop()\n"));
    }

    // Note that mLogger.mError might be true too at this point.
    mLogger->OnStopped();

    AutoPtr<SynthesisPlaybackQueueItem> item;
    {
        Mutex::Autolock lock(mStateLock);
        if (mStopped) {
            //Java:    Log.w(TAG, "stop() called twice");
            Logger::W(TAG, String("stop() called twice\n"));
            return;
        }
        item = mItem;
        mStopped = TRUE;
    }

    if (item != NULL) {
        // This might result in the synthesis thread being woken up, at which
        // point it will write an additional buffer to the item - but we
        // won't worry about that because the audio playback queue will be cleared
        // soon after (see SynthHandler#stop(String).
        item->Stop(wasError);
    }
    else {
        // This happens when stop() or error() were called before start() was.

        // In all other cases, mAudioTrackHandler.stop() will
        // result in onSynthesisDone being called, and we will
        // write data there.
        mLogger->OnWriteData();

        if (wasError) {
            // We have to dispatch the error ourselves.
            mDispatcher->DispatchOnError();
        }
    }
}

Int32 PlaybackSynthesisCallback::GetMaxBufferSize()
{
    // The AudioTrack buffer will be at least MIN_AUDIO_BUFFER_SIZE, so that should always be
    // a safe buffer size to pass in.
    return MIN_AUDIO_BUFFER_SIZE;
}

Boolean PlaybackSynthesisCallback::IsDone()
{
    return mDone;
}

Int32 PlaybackSynthesisCallback::Start(
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 channelCount)
{
    if (DBG) {
        //Java:    Log.d(TAG, "start(" + sampleRateInHz + "," + audioFormat + "," + channelCount + ")");
        String strOut = String("start(") + StringUtils::Int32ToString(sampleRateInHz) + String(",") + StringUtils::Int32ToString(audioFormat) + String(",") + StringUtils::Int32ToString(channelCount) + String(")\n");
        Logger::D(TAG, strOut);
    }

    Int32 channelConfig = BlockingAudioTrack::GetChannelConfig(channelCount);
    if (channelConfig == 0) {
        //Java:    Log.e(TAG, "Unsupported number of channels :" + channelCount);
        String strOut = String("Unsupported number of channels :") + StringUtils::Int32ToString(channelCount) + String("\n");
        Logger::E(TAG, strOut);
        return ITextToSpeech::TTS_ERROR;
    }

    {
        Mutex::Autolock lock(mStateLock);
        if (mStopped) {
            if (DBG) {
                //Java:    Log.d(TAG, "stop() called before start(), returning.");
                Logger::D(TAG, String("stop() called before start(), returning.\n"));
            }
            return ITextToSpeech::TTS_ERROR;
        }
        AutoPtr<SynthesisPlaybackQueueItem> item = new SynthesisPlaybackQueueItem(
                    mStreamType, sampleRateInHz, audioFormat, channelCount, mVolume, mPan,
                    mDispatcher, mCallerIdentity, mLogger);
        mAudioTrackHandler->Enqueue(item);
        mItem = item;
    }

    return ITextToSpeech::TTS_SUCCESS;
}

Int32 PlaybackSynthesisCallback::AudioAvailable(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    if (DBG) {
        //Java:    Log.d(TAG, "audioAvailable(byte[" + buffer.length + "]," + offset + "," + length + ")");
        String strOut = String("audioAvailable(byte[") + StringUtils::Int32ToString(buffer->GetLength()) + String("],") + StringUtils::Int32ToString(offset) + String(",") + StringUtils::Int32ToString(length) + String(")\n");
        Logger::D(TAG, strOut);
    }
    if (length > GetMaxBufferSize() || length <= 0) {
        //Java:    throw new IllegalArgumentException("buffer is too large or of zero length (" + length + " bytes)");
        String strOut = String("buffer is too large or of zero length (") + StringUtils::Int32ToString(length) + String(" bytes)\n");
        Logger::E(TAG, strOut);
        return 0;// E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<SynthesisPlaybackQueueItem> item = NULL;

    {
        Mutex::Autolock lock(mStateLock);
        if (mItem == NULL || mStopped) {
            return ITextToSpeech::TTS_ERROR;
        }
        item = mItem;
    }

    // Sigh, another copy.
    AutoPtr< ArrayOf<Byte> > bufferCopy = ArrayOf<Byte>::Alloc(length);
    bufferCopy->Copy(buffer, offset, length);    //Java:    System.arraycopy(buffer, offset, bufferCopy, 0, length);

    // Might block on mItem.this, if there are too many buffers waiting to
    // be consumed.
    //try {
        item->Put(bufferCopy);
    //} catch (InterruptedException ie) {
        //return ITextToSpeech::TTS_ERROR;
    //}

    mLogger->OnEngineDataReceived();

    return ITextToSpeech::TTS_SUCCESS;
}

Int32 PlaybackSynthesisCallback::Done()
{
    if (DBG) {
        //Java:    Log.d(TAG, "done()");
        Logger::D(TAG, String("done()\n") );
    }

    AutoPtr<SynthesisPlaybackQueueItem> item = NULL;

    {
        Mutex::Autolock lock(mStateLock);
        if (mDone) {
            //Java:    Log.w(TAG, "Duplicate call to done()");
            Logger::W(TAG, String("Duplicate call to done()\n") );
            return ITextToSpeech::TTS_ERROR;
        }

        mDone = TRUE;

        if (mItem == NULL) {
            return ITextToSpeech::TTS_ERROR;
        }

        item = mItem;
    }

    item->Done();
    mLogger->OnEngineComplete();

    return ITextToSpeech::TTS_SUCCESS;
}

ECode PlaybackSynthesisCallback::Error()
{
    if (DBG) {
        //Java:    Log.d(TAG, "error() [will call stop]");
        Logger::D(TAG, String("error() [will call stop]\n") );
    }
    // Currently, this call will not be logged if error( ) is called
    // before start.
    mLogger->OnError();
    StopImpl(TRUE);
    return NOERROR;
}

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
