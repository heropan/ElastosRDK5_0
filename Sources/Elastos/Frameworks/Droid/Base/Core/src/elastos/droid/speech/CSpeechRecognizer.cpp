
#include "elastos/droid/speech/CSpeechRecognizer.h"

#include <elastos/utility/logging/Logger.h>
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Speech {

ECode CSpeechRecognizer::SetRecognitionListener(
    /* [in] */ IRecognitionListener * pListener)
{
    SpeechRecognizer::SetRecognitionListener(pListener);
    return NOERROR;
}

ECode CSpeechRecognizer::StartListening(
    /* [in] */ IIntent * pRecognizerIntent)
{
    SpeechRecognizer::StartListening(pRecognizerIntent);
    return NOERROR;
}

ECode CSpeechRecognizer::StopListening()
{
    SpeechRecognizer::StopListening();
    return NOERROR;
}

ECode CSpeechRecognizer::Cancel()
{
    SpeechRecognizer::Cancel();
    return NOERROR;
}

ECode CSpeechRecognizer::Destroy()
{
    SpeechRecognizer::Destroy();
    return NOERROR;
}

AutoPtr<ISpeechRecognizer> CSpeechRecognizer::CreateSpeechRecognizer(
    /* [in] */ IContext* context)
{
    return CreateSpeechRecognizer(context, NULL);
}

AutoPtr<ISpeechRecognizer> CSpeechRecognizer::CreateSpeechRecognizer(
    /* [in] */ IContext* context,
    /* [in] */ IComponentName* serviceComponent)
{
    if (context == NULL) {
        //Java:    throw new IllegalArgumentException("Context cannot be null)");
        Logger::E(TAG, String("Context cannot be null\n"));
        return NULL;
    }
    CheckIsCalledFromMainThread();

    AutoPtr<ISpeechRecognizer> sr;
    CSpeechRecognizer::New( (ISpeechRecognizer**)&sr );
    ((CSpeechRecognizer*)(sr.Get())) -> Init(context, serviceComponent);

    return sr;
}

}//namespace Speech
}//namespace Droid
}//namespace Elastos

