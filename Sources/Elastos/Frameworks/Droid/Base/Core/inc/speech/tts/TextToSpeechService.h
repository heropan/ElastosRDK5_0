#ifndef __ELASTOS_DROID_SPEECH_TTS_TextToSpeechService_H__
#define __ELASTOS_DROID_SPEECH_TTS_TextToSpeechService_H__


#include "app/Service.h"
#include "speech/tts/AudioPlaybackHandler.h"
//#include "os/HandlerThread.h"
#include "os/Looper.h"
#include "os/Runnable.h"
#include "os/HandlerBase.h"
#include "speech/tts/AbstractSynthesisCallback.h"
#include "speech/tts/EventLogger.h"
#include "speech/tts/AudioPlaybackQueueItem.h"
//#include "os/RemoteCallbackList.h"
#include <elastos/utility/etl/HashMap.h>
#include <elastos/Set.h>


#include "Elastos.Droid.Core_server.h"
using Elastos::Droid::Os::IRemoteCallbackList;
//using Elastos::Droid::Os::RemoteCallbackList;
using Elastos::Droid::Os::IIdleHandler;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Speech::Tts::IITextToSpeechService;
using Elastos::Droid::App::Service;
//using Elastos::Droid::Provider::ISettingsSecure;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Set;
namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/**
 * Abstract base class for TTS engine implementations. The following methods
 * need to be implemented.
 *
 * <ul>
 *   <li>{@link #onIsLanguageAvailable}</li>
 *   <li>{@link #onLoadLanguage}</li>
 *   <li>{@link #onGetLanguage}</li>
 *   <li>{@link #onSynthesizeText}</li>
 *   <li>{@link #onStop}</li>
 * </ul>
 *
 * The first three deal primarily with language management, and are used to
 * query the engine for it's support for a given language and indicate to it
 * that requests in a given language are imminent.
 *
 * {@link #onSynthesizeText} is central to the engine implementation. The
 * implementation should synthesize text as per the request parameters and
 * return synthesized data via the supplied callback. This class and its helpers
 * will then consume that data, which might mean queueing it for playback or writing
 * it to a file or similar. All calls to this method will be on a single
 * thread, which will be different from the main thread of the service. Synthesis
 * must be synchronous which means the engine must NOT hold on the callback or call
 * any methods on it after the method returns
 *
 * {@link #onStop} tells the engine that it should stop all ongoing synthesis, if
 * any. Any pending data from the current synthesis will be discarded.
 *
 */
//public abstract class
class TextToSpeechService
    : public Elastos::Droid::App::Service
{
private:
    /**
     * Synthesizer thread. This thread is used to run {@link SynthHandler}.
     */
    //private class
    class SynthThread
        : public ElRefBase
//        , public HandlerThread
        , public IIdleHandler
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
        //public
        SynthThread(
            /* [in] */ TextToSpeechService* ttss);
    public:
        //@Override
        //public boolean
        CARAPI QueueIdle(
            /* [out] */ Boolean* result);
    protected:
        //@Override
        //protected
        CARAPI_(void) OnLooperPrepared();
    private:
        //private
        CARAPI_(void) BroadcastTtsQueueProcessingCompleted();

    private:
        Boolean mFirstIdle;// = TRUE;
        AutoPtr<TextToSpeechService> mTtss;
    };

private:
    /**
     * An item in the synth thread queue.
     */
    //private abstract class
    class SpeechItem
        : public ElRefBase
        , public ITextToSpeechServiceUtteranceProgressDispatcher
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
        //public
        SpeechItem(
            /* [in] */ IInterface* caller,
            /* [in] */ Int32 callerUid,
            /* [in] */ Int32 callerPid,
            /* [in] */ IBundle* params,
            /* [in] */ TextToSpeechService* ttss);

        //public
        CARAPI_(AutoPtr<IInterface>) GetCallerIdentity();

        /**
         * Checker whether the item is valid. If this method returns false, the item should not
         * be played.
         */
        //public abstract
        virtual CARAPI_(Boolean) IsValid() = 0;

        /**
         * Plays the speech item. Blocks until playback is finished.
         * Must not be called more than once.
         *
         * Only called on the synthesis thread.
         *
         * @return {@link TextToSpeech#SUCCESS} or {@link TextToSpeech#ERROR}.
         */
        //public
        CARAPI_(Int32) Play();

        /**
         * Stops the speech item.
         * Must not be called more than once.
         *
         * Can be called on multiple threads,  but not on the synthesis thread.
         */
        //public
        CARAPI_(void) Stop();

        //@Override
        //public
        CARAPI DispatchOnDone();

        //@Override
        //public
        CARAPI DispatchOnStart();

        //@Override
        //public
        CARAPI DispatchOnError();

        //public
        CARAPI_(Int32) GetCallerUid();

        //public
        CARAPI_(Int32) GetCallerPid();

    public:
        //public
        CARAPI_(Int32) GetStreamType();

        //public
        CARAPI_(Float) GetVolume();

        //public
        CARAPI_(Float) GetPan();

        //public
        CARAPI_(String) GetUtteranceId();

    protected:
        //protected synchronized
        CARAPI_(Boolean) IsStopped();

        //protected abstract
        virtual CARAPI_(Int32) PlayImpl() = 0;

        //protected abstract
        virtual CARAPI_(void) StopImpl() = 0;

    protected:
        //protected
        CARAPI_(String) GetStringParam(
            /* [in] */ const String& key,
            /* [in] */ const String& defaultValue);

        //protected
        CARAPI_(Int32) GetIntParam(
            /* [in] */ const String& key,
            /* [in] */ Int32 defaultValue);

        //protected
        CARAPI_(Float) GetFloatParam(
            /* [in] */ const String& key,
            /* [in] */ Float defaultValue);

    protected:
        //protected final
        AutoPtr<IBundle> mParams;
    protected:
        //private final
        AutoPtr<IInterface> mCallerIdentity;
    protected:
        //private final
        Int32 mCallerUid;
        //private final
        Int32 mCallerPid;
        //private
        Boolean mStarted;// = FALSE;
        //private
        Boolean mStopped;// = FALSE;
        Object mLock;
        AutoPtr<TextToSpeechService> mTtss;
    };

public:
    //class
    class SynthesisSpeechItem
        : public SpeechItem
    {
    public:
        //public
        SynthesisSpeechItem(
            /* [in] */ IInterface* callerIdentity,
            /* [in] */ Int32 callerUid,
            /* [in] */ Int32 callerPid,
            /* [in] */ IBundle* params,
            /* [in] */ const String& text,
            /* [in] */ TextToSpeechService* ttss);

        //public
        String GetText();

        //@Override
        //public
        Boolean IsValid();

    public:
        //public
        String GetLanguage();

    protected:
        //@Override
        //protected
        Int32 PlayImpl();

        //protected
        AutoPtr<AbstractSynthesisCallback> CreateSynthesisCallback();

    protected:
        //@Override
        //protected
        void StopImpl();

    private:
        //private
        void SetRequestParams(
            /* [in] */ ISynthesisRequest* request);

    private:
        //private
        Boolean HasLanguage();

        //private
        String GetCountry();

        //private
        String GetVariant();

        //private
        Int32 GetSpeechRate();

        //private
        Int32 GetPitch();

    private:
        // Never null.
        //private final
        String mText;
        //private final
        AutoPtr<ISynthesisRequest> mSynthesisRequest;
        //private final
        AutoPtr< ArrayOf<String> > mDefaultLocale;
        // Non null after synthesis has started, and all accesses
        // guarded by 'this'.
        //private
        AutoPtr<AbstractSynthesisCallback> mSynthesisCallback;
        //private final
        AutoPtr<EventLogger> mEventLogger;
    };

private:
    //private class
    class SynthesisToFileSpeechItem
        : public SynthesisSpeechItem
    {
    public:
        //public
        SynthesisToFileSpeechItem(
            /* [in] */ IInterface* callerIdentity,
            /* [in] */ Int32 callerUid,
            /* [in] */ Int32 callerPid,
            /* [in] */ IBundle* params,
            /* [in] */ const String& text,
            /* [in] */ IFile* file,
            /* [in] */ TextToSpeechService* ttss);

    protected:
        //@Override
        //protected
        AutoPtr<AbstractSynthesisCallback> CreateSynthesisCallback();

        //@Override
        //protected
        Int32 PlayImpl();

    private:
        //private final
        AutoPtr<IFile> mFile;
    };

    //private class
    class AudioSpeechItem
        : public SpeechItem
    {
    public:
        //public
        AudioSpeechItem(
            /* [in] */ IInterface* callerIdentity,
            /* [in] */ Int32 callerUid,
            /* [in] */ Int32 callerPid,
            /* [in] */ IBundle* params,
            /* [in] */ IUri* uri,
            /* [in] */ TextToSpeechService* ttss);

        //@Override
        //public
        Boolean IsValid();

    protected:
        //@Override
        //protected
        Int32 PlayImpl();

        //@Override
        //protected
        void StopImpl();

    private:
        //private final
        AutoPtr<AudioPlaybackQueueItem> mItem;
    };

    //private class
    class SilenceSpeechItem
        : public SpeechItem
    {
    public:
        //public
        SilenceSpeechItem(
            /* [in] */ IInterface* callerIdentity,
            /* [in] */ Int32 callerUid,
            /* [in] */ Int32 callerPid,
            /* [in] */ IBundle* params,
            /* [in] */ Int64 duration,
            /* [in] */ TextToSpeechService* ttss);

        //@Override
        //public
        Boolean IsValid();

    protected:
        //@Override
        //protected
        Int32 PlayImpl();

        //@Override
        //protected
        void StopImpl();

    private:
        //private final
        Int64 mDuration;
    };

private:
    //private class
    class SynthHandler
        : public HandlerBase
    {
    private:
        class RunnableSynthHandlerEnqueueSpeechItem
            : public Runnable
        {
        public:
            CARAPI Run();
        public:
            RunnableSynthHandlerEnqueueSpeechItem(
                /* [in] */ SynthHandler* sh,
                /* [in] */ SpeechItem* si);
        private:
            AutoPtr<SynthHandler> mSh;
            AutoPtr<SpeechItem> mSi;
        };

    public:
        CARAPI HandleMessage(
            /* [in] */ IMessage* message);

    public:
        //public
        SynthHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ TextToSpeechService* ttss);

    public:
        //public
        CARAPI_(Boolean) IsSpeaking();

        //public
        CARAPI_(void) Quit();

        /**
         * Adds a speech item to the queue.
         *
         * Called on a service binder thread.
         */
        //public
        CARAPI_(Int32) EnqueueSpeechItem(
            /* [in] */ Int32 queueMode,
            /* [in] */ SpeechItem* speechItem);

        /**
         * Stops all speech output and removes any utterances still in the queue for
         * the calling app.
         *
         * Called on a service binder thread.
         */
        //public
        CARAPI_(Int32) StopForApp(
            /* [in] */ IInterface* callerIdentity);

        //public
        CARAPI_(Int32) StopAll();

    private:
        //private synchronized
        CARAPI_(AutoPtr<SpeechItem>) GetCurrentSpeechItem();

        //private synchronized
        CARAPI_(AutoPtr<SpeechItem>) SetCurrentSpeechItem(
            /* [in] */ SpeechItem* speechItem);

        //private synchronized
        CARAPI_(AutoPtr<SpeechItem>) MaybeRemoveCurrentSpeechItem(
            /* [in] */ IInterface* callerIdentity);

    private:
        //private
        AutoPtr<SpeechItem> mCurrentSpeechItem;// = NULL;
        Object mLock;
        AutoPtr<TextToSpeechService> mTtss;
    };

private:
    class TextToSpeechServiceStub
        : public ElRefBase
        , public IITextToSpeechService
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
        //public int
        CARAPI Speak(
            /* [in] */ /*IIBinder*/IBinder* caller,
            /* [in] */ const String & text,
            /* [in] */ Int32 queueMode,
            /* [in] */ IBundle* params,
            /* [out] */ Int32* result);

        //@Override
        //public int
        CARAPI SynthesizeToFile(
            /* [in] */ /*IIBinder*/IBinder* caller,
            /* [in] */ const String & text,
            /* [in] */ const String & filename,
            /* [in] */ IBundle* params,
            /* [out] */ Int32* result);

        //@Override
        //public int
        CARAPI PlayAudio(
            /* [in] */ /*IIBinder*/IBinder* caller,
            /* [in] */ IUri* audioUri,
            /* [in] */ Int32 queueMode,
            /* [in] */ IBundle* params,
            /* [out] */ Int32* result);

        //@Override
        //public int
        CARAPI PlaySilence(
            /* [in] */ /*IIBinder*/IBinder* caller,
            /* [in] */ Int64 duration,
            /* [in] */ Int32 queueMode,
            /* [in] */ IBundle* params,
            /* [out] */ Int32* result);

        //@Override
        //public boolean
        CARAPI IsSpeaking(
            /* [out] */ Boolean* result);

        //@Override
        //public int
        CARAPI Stop(
            /* [in] */ /*IIBinder*/IBinder* caller,
            /* [out] */ Int32* result);

        //@Override
        //public String []
        CARAPI GetLanguage(
            /* [out] */ ArrayOf<String>** result);

        /*
         * If defaults are enforced, then no language is "available" except
         * perhaps the default language selected by the user.
         */
        //@Override
        //public int
        CARAPI IsLanguageAvailable(
            /* [in] */ const String & lang,
            /* [in] */ const String & country,
            /* [in] */ const String & variant,
            /* [out] */ Int32* result);

        //@Override
        //public String[]
        CARAPI GetFeaturesForLanguage(
            /* [in] */ const String & lang,
            /* [in] */ const String & country,
            /* [in] */ const String & variant,
            /* [out] */ ArrayOf<String>** result);

        /*
         * There is no point loading a non default language if defaults
         * are enforced.
         */
        //@Override
        //public int
        CARAPI LoadLanguage(
            /* [in] */ const String & lang,
            /* [in] */ const String & country,
            /* [in] */ const String & variant,
            /* [out] */ Int32* result);

        //@Override
        //public void
        CARAPI SetCallback(
            /* [in] */ /*IIBinder*/IBinder* caller,
            /* [in] */ IITextToSpeechCallback* cb);

    public:
        TextToSpeechServiceStub(
            /* [in] */ TextToSpeechService* ttss);

    private:
        //private
        String Intern(
            /* [in] */ const String& in);

        //private
        Boolean CheckNonNull(
            /* [in] */ ArrayOf<IInterface*>* args);
    private:
        AutoPtr<TextToSpeechService> mTtss;
    };

private:
    //private class
    class CallbackMap
        : public ElRefBase
//        , public IRemoteCallbackList
//        , public RemoteCallbackList
    {
    public:
        //public void
        void SetCallback(
            /* [in] */ /*IIBinder*/IBinder* caller,
            /* [in] */ IITextToSpeechCallback* cb);

        //public void
        void DispatchOnDone(
            /* [in] */ IInterface* callerIdentity,
            /* [in] */ const String& utteranceId);

        //public void
        void DispatchOnStart(
            /* [in] */ IInterface* callerIdentity,
            /* [in] */ const String& utteranceId);

        //public void
        void DispatchOnError(
            /* [in] */ IInterface* callerIdentity,
            /* [in] */ const String& utteranceId);

        //@Override
        //public void
        void OnCallbackDied(
            /* [in] */ IITextToSpeechCallback* callback,
            /* [in] */ IInterface* cookie);

        //@Override
        //public
        void Kill();
    public:
        CallbackMap(
            /* [in] */ TextToSpeechService* ttss);
    private:
        //private
        AutoPtr<IITextToSpeechCallback> GetCallbackFor(
            /* [in] */ IInterface* caller);

    private:
        //private final
        HashMap< AutoPtr</*IIBinder*/IBinder>, AutoPtr<IITextToSpeechCallback> > mCallerToCallback;
        Object mCallerToCallbackLock;
        AutoPtr<TextToSpeechService> mTtss;
    };

public:
    TextToSpeechService();

public:
    //@Override
    //public
    CARAPI OnCreate();

    //@Override
    //public
    CARAPI OnDestroy();

public:
    //@Override
    //public
    AutoPtr</*IIBinder*/IBinder> OnBind(
        /* [in] */ IIntent* intent);

protected:
    /**
     * Checks whether the engine supports a given language.
     *
     * Can be called on multiple threads.
     *
     * @param lang ISO-3 language code.
     * @param country ISO-3 country code. May be empty or null.
     * @param variant Language variant. May be empty or null.
     * @return Code indicating the support status for the locale.
     *         One of {@link TextToSpeech#LANG_AVAILABLE},
     *         {@link TextToSpeech#LANG_COUNTRY_AVAILABLE},
     *         {@link TextToSpeech#LANG_COUNTRY_VAR_AVAILABLE},
     *         {@link TextToSpeech#LANG_MISSING_DATA}
     *         {@link TextToSpeech#LANG_NOT_SUPPORTED}.
     */
    //protected abstract
    virtual CARAPI_(Int32) OnIsLanguageAvailable(
        /* [in] */ const String& lang,
        /* [in] */ const String& country,
        /* [in] */ const String& variant) = 0;

    /**
     * Returns the language, country and variant currently being used by the TTS engine.
     *
     * Can be called on multiple threads.
     *
     * @return A 3-element array, containing language (ISO 3-letter code),
     *         country (ISO 3-letter code) and variant used by the engine.
     *         The country and variant may be {@code ""}. If country is empty, then variant must
     *         be empty too.
     * @see Locale#getISO3Language()
     * @see Locale#getISO3Country()
     * @see Locale#getVariant()
     */
    //protected abstract
    virtual CARAPI_(AutoPtr< ArrayOf<String> >) OnGetLanguage() = 0;

    /**
     * Notifies the engine that it should load a speech synthesis language. There is no guarantee
     * that this method is always called before the language is used for synthesis. It is merely
     * a hint to the engine that it will probably get some synthesis requests for this language
     * at some point in the future.
     *
     * Can be called on multiple threads.
     *
     * @param lang ISO-3 language code.
     * @param country ISO-3 country code. May be empty or null.
     * @param variant Language variant. May be empty or null.
     * @return Code indicating the support status for the locale.
     *         One of {@link TextToSpeech#LANG_AVAILABLE},
     *         {@link TextToSpeech#LANG_COUNTRY_AVAILABLE},
     *         {@link TextToSpeech#LANG_COUNTRY_VAR_AVAILABLE},
     *         {@link TextToSpeech#LANG_MISSING_DATA}
     *         {@link TextToSpeech#LANG_NOT_SUPPORTED}.
     */
    //protected abstract
    virtual CARAPI_(Int32) OnLoadLanguage(
        /* [in] */ const String& lang,
        /* [in] */ const String& country,
        /* [in] */ const String& variant) = 0;

    /**
     * Notifies the service that it should stop any in-progress speech synthesis.
     * This method can be called even if no speech synthesis is currently in progress.
     *
     * Can be called on multiple threads, but not on the synthesis thread.
     */
    //protected abstract
    virtual CARAPI_(void) OnStop() = 0;

    /**
     * Tells the service to synthesize speech from the given text. This method should
     * block until the synthesis is finished.
     *
     * Called on the synthesis thread.
     *
     * @param request The synthesis request.
     * @param callback The callback the the engine must use to make data available for
     *         playback or for writing to a file.
     */
    //protected abstract
    virtual CARAPI_(void) OnSynthesizeText(
        /* [in] */ ISynthesisRequest* request,
        /* [in] */ ISynthesisCallback* callback) = 0;

    /**
     * Queries the service for a set of features supported for a given language.
     *
     * @param lang ISO-3 language code.
     * @param country ISO-3 country code. May be empty or null.
     * @param variant Language variant. May be empty or null.
     * @return A list of features supported for the given language.
     */
    //protected
    CARAPI_(AutoPtr< Set<String> >) OnGetFeaturesForLanguage(
        /* [in] */ const String& lang,
        /* [in] */ const String& country,
        /* [in] */ const String& variant);

private:
    //private
    CARAPI_(Int32) GetDefaultSpeechRate();

    //private
    CARAPI_(AutoPtr< ArrayOf<String> >) GetSettingsLocale();

    //private
    CARAPI_(Int32) GetSecureSettingInt(
        /* [in] */ const String& name,
        /* [in] */ Int32 defaultValue);

private:
    //private
    static const Boolean DBG;// = FALSE;
    //private
    static const CString TAG;// = "TextToSpeechService";

    //private
    static const Int32 MAX_SPEECH_ITEM_CHAR_LENGTH;// = 4000;
    //private
    static const CString SYNTH_THREAD_NAME;// = "SynthThread";

    //private
    AutoPtr<SynthHandler> mSynthHandler;
    // A thread and it's associated handler for playing back any audio
    // associated with this TTS engine. Will handle all requests except synthesis
    // to file requests, which occur on the synthesis thread.
    //private
    AutoPtr<AudioPlaybackHandler> mAudioPlaybackHandler;
    //private
    AutoPtr<ITtsEngines> mEngineHelper;

    //private
    AutoPtr<CallbackMap> mCallbacks;
    //private
    String mPackageName;

private:
    /**
     * Binder returned from {@code #onBind(Intent)}. The methods in this class can be
     * called called from several different threads.
     */
    // NOTE: All calls that are passed in a calling app are interned so that
    // they can be used as message objects (which are tested for equality using ==).
    //private final
    AutoPtr<IITextToSpeechService> mBinder;// = (IITextToSpeechService*)(new TextToSpeechServiceStub());

};



} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_TextToSpeechService_H__
