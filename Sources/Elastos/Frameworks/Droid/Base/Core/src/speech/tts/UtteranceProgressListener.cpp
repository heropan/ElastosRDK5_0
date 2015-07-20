#include "speech/tts/UtteranceProgressListener.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/******************************UtteranceProgressListener::UtteranceProgressListenerStand*************************/
PInterface UtteranceProgressListener::UtteranceProgressListenerStand::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IUtteranceProgressListener*)this;
    }
    else if (riid == EIID_IUtteranceProgressListener) {
        return (IUtteranceProgressListener*)this;
    }
    return NULL;
}

UInt32 UtteranceProgressListener::UtteranceProgressListenerStand::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 UtteranceProgressListener::UtteranceProgressListenerStand::Release()
{
    return ElRefBase::Release();
}

ECode UtteranceProgressListener::UtteranceProgressListenerStand::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IUtteranceProgressListener*)this) {
        *iID = EIID_IUtteranceProgressListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

UtteranceProgressListener::UtteranceProgressListenerStand::UtteranceProgressListenerStand(
    /* [in] */ ITextToSpeechOnUtteranceCompletedListener* listener)
{
    mListener = listener;
}

ECode UtteranceProgressListener::UtteranceProgressListenerStand::OnDone(
    /* [in] */ const String & utteranceId)
{
    AutoLock lock(mMutex);
    mListener->OnUtteranceCompleted(utteranceId);
    return NOERROR;
}

ECode UtteranceProgressListener::UtteranceProgressListenerStand::OnError(
    /* [in] */ const String & utteranceId)
{
    mListener->OnUtteranceCompleted(utteranceId);
    return NOERROR;
}

ECode UtteranceProgressListener::UtteranceProgressListenerStand::OnStart(
    /* [in] */ const String & utteranceId)
{
    // Left unimplemented, has no equivalent in the old
    // API.
    return NOERROR;
}

/******************************UtteranceProgressListener*************************/
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