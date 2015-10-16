#ifndef __ELASTOS_DROID_SPEECH_RecognitionService_H__
#define __ELASTOS_DROID_SPEECH_RecognitionService_H__

#include "elastos/droid/app/Service.h"
#include "elastos/droid/os/HandlerBase.h"
#include "Elastos.Droid.Core_server.h"

using Elastos::Droid::App::Service;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::HandlerBase;

namespace Elastos {
namespace Droid {
namespace Speech {

/**
 * This class provides a base class for recognition service implementations. This class should be
 * extended only in case you wish to implement a new speech recognizer. Please note that the
 * implementation of this service is stateless.
 */
//public abstract class
class RecognitionService
    : public Elastos::Droid::App::Service
{
public:
    /**
     * This class receives callbacks from the speech recognition service and forwards them to the
     * user. An instance of this class is passed to the
     * {@link RecognitionService#onStartListening(Intent, Callback)} method. Recognizers may call
     * these methods on any thread.
     */
    //public
    class RecognitionServiceCallback
        : public ElRefBase
        , public IRecognitionServiceCallback
    {
        friend class RecognitionService;
    public:
        CAR_INTERFACE_DECL()

    public:
        /**
         * The service should call this method when the user has started to speak.
         */
        //public
        CARAPI BeginningOfSpeech();

        /**
         * The service should call this method when sound has been received. The purpose of this
         * function is to allow giving feedback to the user regarding the captured audio.
         *
         * @param buffer a buffer containing a sequence of big-endian 16-bit integers representing a
         *        single channel audio stream. The sample rate is implementation dependent.
         */
        //public
        CARAPI BufferReceived(
            /* [in]  */ ArrayOf<Byte>* buffer);

        /**
         * The service should call this method after the user stops speaking.
         */
        //public
        CARAPI EndOfSpeech();

        /**
         * The service should call this method when a network or recognition error occurred.
         *
         * @param error code is defined in {@link SpeechRecognizer}
         */
        //public
        CARAPI Error(
            /* [in]  */ Int32 error);

        /**
         * The service should call this method when partial recognition results are available. This
         * method can be called at any time between {@link #beginningOfSpeech()} and
         * {@link #results(Bundle)} when partial results are ready. This method may be called zero,
         * one or multiple times for each call to {@link SpeechRecognizer#startListening(Intent)},
         * depending on the speech recognition service implementation.
         *
         * @param partialResults the returned results. To retrieve the results in
         *        ArrayList&lt;String&gt; format use {@link Bundle#getStringArrayList(String)} with
         *        {@link SpeechRecognizer#RESULTS_RECOGNITION} as a parameter
         */
        //public
        CARAPI PartialResults(
            /* [in]  */ IBundle* partialResults);

        /**
         * The service should call this method when the endpointer is ready for the user to start
         * speaking.
         *
         * @param params parameters set by the recognition service. Reserved for future use.
         */
        //public
        CARAPI ReadyForSpeech(
            /* [in]  */ IBundle* params);

        /**
         * The service should call this method when recognition results are ready.
         *
         * @param results the recognition results. To retrieve the results in {@code
         *        ArrayList&lt;String&gt;} format use {@link Bundle#getStringArrayList(String)} with
         *        {@link SpeechRecognizer#RESULTS_RECOGNITION} as a parameter
         */
        //public
        CARAPI Results(
            /* [in]  */ IBundle* results);

        /**
         * The service should call this method when the sound level in the audio stream has changed.
         * There is no guarantee that this method will be called.
         *
         * @param rmsdB the new RMS dB value
         */
        //public
        CARAPI RmsChanged(
            /* [in]  */ Float rmsdB);

    private:
        //private
        RecognitionServiceCallback(
            /* [in]  */ IIRecognitionListener* listener,
            /* [in]  */ RecognitionService* rs);
    private:
        //private final
        AutoPtr<IIRecognitionListener> mListener;
        AutoPtr<RecognitionService> mHost;
    };

private:
    class RecognitionServiceStartListeningArgs
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()
        RecognitionServiceStartListeningArgs(
            /* [in]  */ IIntent* intent,
            /* [in]  */ IIRecognitionListener* listener);

    public:
        AutoPtr<IIntent> mIntent;

        AutoPtr<IIRecognitionListener> mListener;
    };

private:
    class RecognitionServiceHandler
        : public HandlerBase
    {
    public:
        RecognitionServiceHandler(
            /* [in] */ RecognitionService* host)
            : mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        AutoPtr<RecognitionService> mHost;
    };

private:
    /** Binder of the recognition service */
    //private static
    class RecognitionServiceBinder
        : public ElRefBase
        , public IIRecognitionService
    {
    public:
        CAR_INTERFACE_DECL()

    public:
        //public
        RecognitionServiceBinder(
            /* [in]  */ RecognitionService* service);

        //public
        CARAPI StartListening(
            /* [in]  */ IIntent* recognizerIntent,
            /* [in]  */ IIRecognitionListener* listener);

        //public
        CARAPI StopListening(
            /* [in]  */ IIRecognitionListener* listener);

        //public
        CARAPI Cancel(
            /* [in]  */ IIRecognitionListener* listener);

        //public
        CARAPI ClearReference();

    private:
        //private
        AutoPtr<RecognitionService> mInternalService;
    };

public:
    friend class RecognitionServiceCallback;
    friend class RecognitionServiceBinder;

public:
    //@Override
    //public final
    CARAPI_(AutoPtr</*IIBinder*/IBinder>) OnBind(
        /* [in]  */ IIntent* intent);

    //@Override
    //public void
    CARAPI OnDestroy();

public:
    RecognitionService();

protected:
    /**
     * Notifies the service that it should start listening for speech.
     *
     * @param recognizerIntent contains parameters for the recognition to be performed. The intent
     *        may also contain optional extras, see {@link RecognizerIntent}. If these values are
     *        not set explicitly, default values should be used by the recognizer.
     * @param listener that will receive the service's callbacks
     */
    //protected abstract
    virtual CARAPI_(void) OnStartListening(
        /* [in]  */ IIntent* recognizerIntent,
        /* [in]  */ RecognitionServiceCallback* listener) = 0;

    /**
     * Notifies the service that it should cancel the speech recognition.
     */
    //protected abstract
    virtual CARAPI_(void) OnCancel(
        /* [in]  */ RecognitionServiceCallback* listener) = 0;

    /**
     * Notifies the service that it should stop listening for speech. Speech captured so far should
     * be recognized as if the user had stopped speaking at this point. This method is only called
     * if the application calls it explicitly.
     */
    //protected abstract
    virtual CARAPI_(void) OnStopListening(
        /* [in]  */ RecognitionServiceCallback* listener) = 0;

private:
    //private
    CARAPI_(void) DispatchStartListening(
        /* [in]  */ IIntent* intent,
        /* [in]  */ IIRecognitionListener* listener);

    //private
    CARAPI_(void) DispatchStopListening(
        /* [in]  */ IIRecognitionListener* listener);

    //private
    CARAPI_(void) DispatchCancel(
        /* [in]  */ IIRecognitionListener* listener);

    //private
    CARAPI_(void) DispatchClearCallback();

    /**
     * Checks whether the caller has sufficient permissions
     *
     * @param listener to send the error message to in case of error
     * @return {@code true} if the caller has enough permissions, {@code false} otherwise
     */
    //private
    CARAPI_(Boolean) CheckPermissions(
        /* [in]  */ IIRecognitionListener* listener);

public:
    /**
     * The {@link Intent} that must be declared as handled by the service.
     */
    //@SdkConstant(SdkConstantType.SERVICE_ACTION)
    //public
    static const CString SERVICE_INTERFACE;// = "android.speech.RecognitionService";

    /**
     * Name under which a RecognitionService component publishes information about itself.
     * This meta-data should reference an XML resource containing a
     * <code>&lt;{@link android.R.styleable#RecognitionService recognition-service}&gt;</code> tag.
     */
    //public
    static const CString SERVICE_META_DATA;// = "android.speech";

private:
    /** Log messages identifier */
    //private
    static const CString TAG;// = "RecognitionService";

    /** Debugging flag */
    //private
    static const Boolean DBG;// = FALSE;

    /** Binder of the recognition service */
    //private
    AutoPtr<RecognitionServiceBinder> mBinder;// = new RecognitionServiceBinder(this);

    /**
     * The current callback of an application that invoked the
     * {@link RecognitionService#onStartListening(Intent, Callback)} method
     */
    //private
    AutoPtr<RecognitionServiceCallback> mCurrentCallback;// = NULL;

    //private
    static const Int32 MSG_START_LISTENING;// = 1;

    //private
    static const Int32 MSG_STOP_LISTENING;// = 2;

    //private
    static const Int32 MSG_CANCEL;// = 3;

    //private
    static const Int32 MSG_RESET;// = 4;

    //private final
    AutoPtr<IHandler>  mHandler;// = new RecognitionServiceHandler(this);

};



} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_RecognitionService_H__
