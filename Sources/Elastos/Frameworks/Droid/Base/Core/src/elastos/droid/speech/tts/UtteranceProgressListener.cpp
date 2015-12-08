#include "elastos/droid/speech/tts/UtteranceProgressListener.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/******************
 * UtteranceProgressListener::UtteranceProgressListenerStand
 *******************************************************************************************************/

CAR_OBJECT_IMPL(UtteranceProgressListener::UtteranceProgressListenerStand);

UtteranceProgressListener::UtteranceProgressListenerStand::UtteranceProgressListenerStand()
{}

UtteranceProgressListener::UtteranceProgressListenerStand::~UtteranceProgressListenerStand()
{}

ECode UtteranceProgressListener::UtteranceProgressListenerStand::constructor()
{
    return NOERROR;
}


ECode UtteranceProgressListener::UtteranceProgressListenerStand::constructor(
    /* [in] */ ITextToSpeechOnUtteranceCompletedListener* listener)
{
    mListener = listener;

    return NOERROR;
}

ECode UtteranceProgressListener::UtteranceProgressListenerStand::OnDone(
    /* [in] */ const String& utteranceId)
{
    AutoLock lock(mMutex);
    mListener->OnUtteranceCompleted(utteranceId);
    return NOERROR;
}

ECode UtteranceProgressListener::UtteranceProgressListenerStand::OnError(
    /* [in] */ const String& utteranceId)
{
    mListener->OnUtteranceCompleted(utteranceId);
    return NOERROR;
}

ECode UtteranceProgressListener::UtteranceProgressListenerStand::OnError(
    /* [in] */ const String& utteranceId,
    /* [in] */ Int32 errorCode)
{
    OnError(utteranceId);
    return NOERROR;
}

ECode UtteranceProgressListener::UtteranceProgressListenerStand::OnStart(
    /* [in] */ const String& utteranceId)
{
    // Left unimplemented, has no equivalent in the old
    // API.
    return NOERROR;
}

/******************
 * UtteranceProgressListener
 *******************************************************************************************************/

AutoPtr<IUtteranceProgressListener> UtteranceProgressListener::From(
    /* [in] */ ITextToSpeechOnUtteranceCompletedListener* listener)
{
    AutoPtr<IUtteranceProgressListener> ret = new UtteranceProgressListenerStand(listener);
    return ret;
}

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
