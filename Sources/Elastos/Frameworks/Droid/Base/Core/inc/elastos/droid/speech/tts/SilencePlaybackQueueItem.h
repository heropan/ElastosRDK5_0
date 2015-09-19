#ifndef __ELASTOS_DROID_SPEECH_TTS_SilencePlaybackQueueItem_H__
#define __ELASTOS_DROID_SPEECH_TTS_SilencePlaybackQueueItem_H__

#include "speech/tts/PlaybackQueueItem.h"
#include "Elastos.Droid.Core_server.h"
//
namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

class SilencePlaybackQueueItem
    : public PlaybackQueueItem
{
public:
    SilencePlaybackQueueItem(
        /* [in] */ ITextToSpeechServiceUtteranceProgressDispatcher* dispatcher,
        /* [in] */ IInterface* callerIdentity,
        /* [in] */ Int64 silenceDurationMs);

    //@Override
    CARAPI Run();

    //@Override
    CARAPI Stop(
        /* [in] */ Boolean isError);

private:
//    AutoPtr<IConditionVariable> mCondVar;// = new ConditionVariable();
    Int64 mSilenceDurationMs;
};

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_SilencePlaybackQueueItem_H__
