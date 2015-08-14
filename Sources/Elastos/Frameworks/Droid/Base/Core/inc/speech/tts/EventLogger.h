#ifndef __ELASTOS_DROID_SPEECH_TTS_EventLogger_H__
#define __ELASTOS_DROID_SPEECH_TTS_EventLogger_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/**
 * Writes data about a given speech synthesis request to the event logs.
 * The data that is logged includes the calling app, length of the utterance,
 * speech rate / pitch and the latency and overall time taken.
 *
 * Note that {@link EventLogger#onStopped()} and {@link EventLogger#onError()}
 * might be called from any thread, but on {@link EventLogger#onAudioDataWritten()} and
 * {@link EventLogger#onComplete()} must be called from a single thread
 * (usually the audio playback thread}
 */
class EventLogger
    : public ElRefBase
{
public:
    EventLogger(
        /* [in] */ ISynthesisRequest* request,
        /* [in] */ Int32 callerUid,
        /* [in] */ Int32 callerPid,
        /* [in] */ const String& serviceApp);

    /**
     * Notifies the logger that this request has been selected from
     * the processing queue for processing. Engine latency / total time
     * is measured from this baseline.
     */
    CARAPI_(void) OnRequestProcessingStart();

    /**
     * Notifies the logger that a chunk of data has been received from
     * the engine. Might be called multiple times.
     */
    CARAPI_(void) OnEngineDataReceived();

    /**
     * Notifies the logger that the engine has finished processing data.
     * Will be called exactly once.
     */
    CARAPI_(void) OnEngineComplete();

    /**
     * Notifies the logger that audio playback has started for some section
     * of the synthesis. This is normally some amount of time after the engine
     * has synthesized data and varies depending on utterances and
     * other audio currently in the queue.
     */
    CARAPI_(void) OnAudioDataWritten();

    /**
     * Notifies the logger that the current synthesis was stopped.
     * Latency numbers are not reported for stopped syntheses.
     */
    CARAPI_(void) OnStopped();

    /**
     * Notifies the logger that the current synthesis resulted in
     * an error. This is logged using {@link EventLogTags#writeTtsSpeakFailure}.
     */
    CARAPI_(void) OnError();

    /**
     * Notifies the logger that the current synthesis has completed.
     * All available data is not logged.
     */
    CARAPI_(void) OnWriteData();

private:
    /**
     * @return the length of the utterance for the given synthesis, 0
     *          if the utterance was {@code null}.
     */
    CARAPI_(Int32) GetUtteranceLength();

    /**
     * Returns a formatted locale string from the synthesis params of the
     * form lang-country-variant.
     */
    CARAPI_(String) GetLocaleString();

private:
    AutoPtr<ISynthesisRequest> mRequest;// = NULL;
    String mServiceApp;
    Int32 mCallerUid;
    Int32 mCallerPid;
    Int64 mReceivedTime;
    Int64 mPlaybackStartTime;// = -1;
    volatile Int64 mRequestProcessingStartTime;// = -1;
    volatile Int64 mEngineStartTime;// = -1;
    volatile Int64 mEngineCompleteTime;// = -1;

    volatile Boolean mError;// = FALSE;
    volatile Boolean mStopped;// = FALSE;
    Boolean mLogWritten;// = FALSE;
};

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_EventLogger_H__
