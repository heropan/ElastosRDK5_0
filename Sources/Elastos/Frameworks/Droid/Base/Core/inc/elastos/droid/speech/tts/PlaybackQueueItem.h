#ifndef __ELASTOS_DROID_SPEECH_TTS_PlaybackQueueItem_H__
#define __ELASTOS_DROID_SPEECH_TTS_PlaybackQueueItem_H__

#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/core/Object.h"

using Elastos::Core::IRunnable;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

//abstract
class PlaybackQueueItem
    : public Object
    , public IRunnable
{
public:
    PlaybackQueueItem(
        /* [in] */ ITextToSpeechServiceUtteranceProgressDispatcher* dispatcher,
        /* [in] */ IInterface* callerIdentity);

    CARAPI_(AutoPtr<IInterface>) GetCallerIdentity();

public:
    virtual CARAPI Run() = 0;

    /**
     * Stop the playback.
     *
     * @param errorCode Cause of the stop. Can be either one of the error codes from
     *         {@link android.speech.tts.TextToSpeech} or
     *         {@link android.speech.tts.TextToSpeech#STOPPED}
     *         if stopped on a client request.
     */
     virtual CARAPI Stop(
        /* [in] */ Int32 errorCode) = 0;

protected:
    CARAPI_(AutoPtr<ITextToSpeechServiceUtteranceProgressDispatcher>) GetDispatcher();

private:
    AutoPtr<ITextToSpeechServiceUtteranceProgressDispatcher> mDispatcher;
    AutoPtr<IInterface> mCallerIdentity;
};

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_PlaybackQueueItem_H__
