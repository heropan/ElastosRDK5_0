#ifndef __ELASTOS_DROID_SPEECH_TTS_PlaybackSynthesisCallback_H__
#define __ELASTOS_DROID_SPEECH_TTS_PlaybackSynthesisCallback_H__

#include "elastos/droid/speech/tts/AbstractSynthesisCallback.h"
#include "elastos/droid/speech/tts/SynthesisPlaybackQueueItem.h"
#include "elastos/droid/speech/tts/EventLogger.h"
#include "elastos/droid/speech/tts/AudioPlaybackHandler.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/**
 * Speech synthesis request that plays the audio as it is received.
 */
class PlaybackSynthesisCallback
    : public AbstractSynthesisCallback
{
public:
    PlaybackSynthesisCallback(
        /* [in] */ Int32 streamType,
        /* [in] */ Float volume,
        /* [in] */ Float pan,
        /* [in] */ AudioPlaybackHandler* audioTrackHandler,
        /* [in] */ ITextToSpeechServiceUtteranceProgressDispatcher* dispatcher,
        /* [in] */ IInterface* callerIdentity,
        /* [in] */ EventLogger* logger);

    //@Override
    CARAPI_(void) Stop();

    CARAPI_(void) StopImpl(
        /* [in] */ Boolean wasError);

    //@Override
    CARAPI_(Int32) GetMaxBufferSize();

    //@Override
    CARAPI_(Boolean) IsDone();

    //@Override
    CARAPI_(Int32) Start(
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 channelCount);

    //@Override
    CARAPI_(Int32) AudioAvailable(
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    //@Override
    CARAPI_(Int32) Done();

    //@Override
    CARAPI Error();

private:
    static const String TAG;            // = "PlaybackSynthesisRequest";
    static const Boolean DBG;           // = FALSE;

    static const Int32 MIN_AUDIO_BUFFER_SIZE;       // = 8192;

    /**
     * Audio stream type. Must be one of the STREAM_ contants defined in
     * {@link android.media.AudioManager}.
     */
    Int32 mStreamType;                    // = 0;

    /**
     * Volume, in the range [0.0f, 1.0f]. The default value is
     * {@link TextToSpeech.Engine#DEFAULT_VOLUME} (1.0f).
     */
    Float mVolume;                        // = 0.0;

    /**
     * Left/right position of the audio, in the range [-1.0f, 1.0f].
     * The default value is {@link TextToSpeech.Engine#DEFAULT_PAN} (0.0f).
     */
    Float mPan;// = 0.0;

    /**
     * Guards {@link #mAudioTrackHandler}, {@link #mItem} and {@link #mStopped}.
     */
    Object mStateLock;

    // Handler associated with a thread that plays back audio requests.
    AutoPtr<AudioPlaybackHandler> mAudioTrackHandler;
    // A request "token", which will be non null after start() has been called.
    AutoPtr<SynthesisPlaybackQueueItem> mItem;// = NULL;
    // Whether this request has been stopped. This is useful for keeping
    // track whether stop() has been called before start(). In all other cases,
    // a non-null value of mItem will provide the same information.
    Boolean mStopped;// = FALSE;

    volatile Boolean mDone;             // = FALSE;

    AutoPtr<ITextToSpeechServiceUtteranceProgressDispatcher> mDispatcher;
    AutoPtr<IInterface> mCallerIdentity;
    AutoPtr<EventLogger> mLogger;
};

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_PlaybackSynthesisCallback_H__
