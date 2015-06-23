#ifndef __UtteranceProgressListener_H__
#define __UtteranceProgressListener_H__


#include "Elastos.Droid.Core_server.h"
namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/**
 * Listener for events relating to the progress of an utterance through
 * the synthesis queue. Each utterance is associated with a call to
 * {@link TextToSpeech#speak} or {@link TextToSpeech#synthesizeToFile} with an
 * associated utterance identifier, as per {@link TextToSpeech.Engine#KEY_PARAM_UTTERANCE_ID}.
 *
 * The callbacks specified in this method can be called from multiple threads.
 */
//public abstract class
class UtteranceProgressListener
    : public ElRefBase
{
private:
    class UtteranceProgressListenerStand
        : public ElRefBase
        , public IUtteranceProgressListener
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* Object,
            /* [out] */ InterfaceID* iID);

    public:
        //@Override
        //public synchronized void
        CARAPI OnDone(
            /* [in] */ const String & utteranceId);

        //@Override
        //public void
        CARAPI OnError(
            /* [in] */ const String & utteranceId);

        //@Override
        //public void
        CARAPI OnStart(
            /* [in] */ const String & utteranceId);

    public:
        UtteranceProgressListenerStand(
        /* [in] */ ITextToSpeechOnUtteranceCompletedListener* listener);
    private:
        AutoPtr<ITextToSpeechOnUtteranceCompletedListener> mListener;
        Mutex mMutex;
    };

public:
    /**
     * Called when an utterance "starts" as perceived by the caller. This will
     * be soon before audio is played back in the case of a {@link TextToSpeech#speak}
     * or before the first bytes of a file are written to storage in the case
     * of {@link TextToSpeech#synthesizeToFile}.
     *
     * @param utteranceId the utterance ID of the utterance.
     */
    //public abstract
    virtual CARAPI OnStart(
        /* [in] */ const String & utteranceId) = 0;

    /**
     * Called when an utterance has successfully completed processing.
     * All audio will have been played back by this point for audible output, and all
     * output will have been written to disk for file synthesis requests.
     *
     * This request is guaranteed to be called after {@link #onStart(String)}.
     *
     * @param utteranceId the utterance ID of the utterance.
     */
    //public abstract
    virtual CARAPI OnDone(
        /* [in] */ const String & utteranceId) = 0;

    /**
     * Called when an error has occurred during processing. This can be called
     * at any point in the synthesis process. Note that there might be calls
     * to {@link #onStart(String)} for specified utteranceId but there will never
     * be a call to both {@link #onDone(String)} and {@link #onError(String)} for
     * the same utterance.
     *
     * @param utteranceId the utterance ID of the utterance.
     */
    //public abstract
    virtual CARAPI OnError(
        /* [in] */ const String & utteranceId) = 0;

    /**
     * Wraps an old deprecated OnUtteranceCompletedListener with a shiny new
     * progress listener.
     *
     * @hide
     */
    static AutoPtr<IUtteranceProgressListener> From(
        /* [in] */ ITextToSpeechOnUtteranceCompletedListener* listener);
};


} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __UtteranceProgressListener_H__
