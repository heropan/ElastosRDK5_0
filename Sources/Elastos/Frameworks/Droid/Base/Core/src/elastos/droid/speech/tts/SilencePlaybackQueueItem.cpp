#include "speech/tts/SilencePlaybackQueueItem.h"
//#include "os/CConditionVariable.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

SilencePlaybackQueueItem::SilencePlaybackQueueItem(
    /* [in] */ ITextToSpeechServiceUtteranceProgressDispatcher* dispatcher,
    /* [in] */ IInterface* callerIdentity,
    /* [in] */ Int64 silenceDurationMs):PlaybackQueueItem(dispatcher, callerIdentity)
{
    mSilenceDurationMs = silenceDurationMs;
//    CConditionVariable::New((IConditionVariable**)&mCondVar);
}

ECode SilencePlaybackQueueItem::Run()
{
    GetDispatcher()->DispatchOnStart();
    if (mSilenceDurationMs > 0) {
        Boolean bBlock;
//        mCondVar->Block(mSilenceDurationMs, &bBlock);
    }
    GetDispatcher()->DispatchOnDone();
    return NOERROR;
}

ECode SilencePlaybackQueueItem::Stop(
    /* [in] */ Boolean isError)
{
//    mCondVar->Open();
    return NOERROR;
}

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos