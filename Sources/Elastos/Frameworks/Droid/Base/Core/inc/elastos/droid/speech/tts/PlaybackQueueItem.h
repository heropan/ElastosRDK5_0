#ifndef __ELASTOS_DROID_SPEECH_TTS_PlaybackQueueItem_H__
#define __ELASTOS_DROID_SPEECH_TTS_PlaybackQueueItem_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

//abstract
class PlaybackQueueItem
    : public ElRefBase
//    , public IRunnable
{
public:
    PlaybackQueueItem(
        /* [in] */ ITextToSpeechServiceUtteranceProgressDispatcher* dispatcher,
        /* [in] */ IInterface* callerIdentity);

    CARAPI_(AutoPtr<IInterface>) GetCallerIdentity();

public:
    virtual CARAPI Run() = 0;
    virtual CARAPI Stop(
        /* [in] */ Boolean isError) = 0;

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
