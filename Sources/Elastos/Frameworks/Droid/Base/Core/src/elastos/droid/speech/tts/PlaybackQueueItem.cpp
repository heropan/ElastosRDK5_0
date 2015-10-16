#include "elastos/droid/speech/tts/PlaybackQueueItem.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

PlaybackQueueItem::PlaybackQueueItem(
    /* [in] */ ITextToSpeechServiceUtteranceProgressDispatcher* dispatcher,
    /* [in] */ IInterface* callerIdentity)
{
    mDispatcher = dispatcher;
    mCallerIdentity = callerIdentity;
}

AutoPtr<IInterface> PlaybackQueueItem::GetCallerIdentity()
{
    return mCallerIdentity;
}

AutoPtr<ITextToSpeechServiceUtteranceProgressDispatcher> PlaybackQueueItem::GetDispatcher()
{
    return mDispatcher;
}

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos