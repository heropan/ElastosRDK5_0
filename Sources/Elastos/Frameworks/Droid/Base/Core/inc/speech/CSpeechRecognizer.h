
#ifndef __CSPEECHRECOGNIZER_H__
#define __CSPEECHRECOGNIZER_H__

#include "_CSpeechRecognizer.h"

#include "speech/SpeechRecognizer.h"

namespace Elastos {
namespace Droid {
namespace Speech {

CarClass(CSpeechRecognizer) , public SpeechRecognizer
{
public:
    CARAPI SetRecognitionListener(
        /* [in] */ IRecognitionListener * pListener);

    CARAPI StartListening(
        /* [in] */ IIntent * pRecognizerIntent);

    CARAPI StopListening();

    CARAPI Cancel();

    CARAPI Destroy();

    /**
     * Factory method to create a new {@code SpeechRecognizer}. Please note that
     * {@link #setRecognitionListener(RecognitionListener)} should be called before dispatching any
     * command to the created {@code SpeechRecognizer}, otherwise no notifications will be
     * received.
     *
     * @param context in which to create {@code SpeechRecognizer}
     * @return a new {@code SpeechRecognizer}
     */
    //public
    static CARAPI_(AutoPtr<ISpeechRecognizer>) CreateSpeechRecognizer(
        /* [in] */ IContext* context);

    /**
     * Factory method to create a new {@code SpeechRecognizer}. Please note that
     * {@link #setRecognitionListener(RecognitionListener)} should be called before dispatching any
     * command to the created {@code SpeechRecognizer}, otherwise no notifications will be
     * received.
     *
     * Use this version of the method to specify a specific service to direct this
     * {@link SpeechRecognizer} to. Normally you would not use this; use
     * {@link #createSpeechRecognizer(Context)} instead to use the system default recognition
     * service.
     *
     * @param context in which to create {@code SpeechRecognizer}
     * @param serviceComponent the {@link ComponentName} of a specific service to direct this
     *        {@code SpeechRecognizer} to
     * @return a new {@code SpeechRecognizer}
     */
    //public
    static CARAPI_(AutoPtr<ISpeechRecognizer>) CreateSpeechRecognizer(
        /* [in] */ IContext* context,
        /* [in] */ IComponentName* serviceComponent);

private:
    // TODO: Add your private member variables here.
};

}//namespace Speech
}//namespace Droid
}//namespace Elastos

#endif // __CSPEECHRECOGNIZER_H__
