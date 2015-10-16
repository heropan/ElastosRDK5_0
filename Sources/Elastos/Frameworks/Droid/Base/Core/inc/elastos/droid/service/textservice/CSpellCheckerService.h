
#ifndef __ELASTOS_DROID_SERVICE_CSPELLCHECKERSERVICE_H__
#define __ELASTOS_DROID_SERVICE_CSPELLCHECKERSERVICE_H__

#include "_Elastos_Droid_service_CSpellCheckerService.h"
#include "elastos/droid/ext/frameworkext.h"


namespace Elastos {
namespace Droid {
namespace service {

CarClass(CSpellCheckerService), public IService
{
public:
    /**
     * This abstract class should be overridden by a concrete implementation of a spell checker.
     */
    /*static abstract*/
    class Session
    {
    public:
        /**
         * @hide
         */
        const CARAPI SetInternalISpellCheckerSession(
            /* [in] */ IInternalISpellCheckerSession* session);

        /**
         * This is called after the class is initialized, at which point it knows it can call
         * getLocale() etc...
         */
        /*abstract*/ CARAPI OnCreate();

        /**
         * Get suggestions for specified text in TextInfo.
         * This function will run on the incoming IPC thread.
         * So, this is not called on the main thread,
         * but will be called in series on another thread.
         * @param textInfo the text metadata
         * @param suggestionsLimit the maximum number of suggestions to be returned
         * @return SuggestionsInfo which contains suggestions for textInfo
         */
        /*abstract*/ CARAPI OnGetSuggestions(
            /* [in] */ ITextInfo* textInfo,
            /* [in] */ Int32 suggestionsLimit,
            /* [out] */ ISuggestionsInfo** result);

        /**
         * A batch process of onGetSuggestions.
         * This function will run on the incoming IPC thread.
         * So, this is not called on the main thread,
         * but will be called in series on another thread.
         * @param textInfos an array of the text metadata
         * @param suggestionsLimit the maximum number of suggestions to be returned
         * @param sequentialWords true if textInfos can be treated as sequential words.
         * @return an array of {@link SentenceSuggestionsInfo} returned by
         * {@link SpellCheckerService.Session#onGetSuggestions(TextInfo, int)}
         */
        CARAPI OnGetSuggestionsMultiple(
            /* [in] */ ArrayOf<ITextInfo> textInfos,
            /* [in] */ Int32 suggestionsLimit,
            /* [in] */ Boolean sequentialWords,
            /* [out] */ ArrayOf<SuggestionsInfo*>* result);

        /**
         * Get sentence suggestions for specified texts in an array of TextInfo.
         * The default implementation splits the input text to words and returns
         * {@link SentenceSuggestionsInfo} which contains suggestions for each word.
         * This function will run on the incoming IPC thread.
         * So, this is not called on the main thread,
         * but will be called in series on another thread.
         * When you override this method, make sure that suggestionsLimit is applied to suggestions
         * that share the same start position and length.
         * @param textInfos an array of the text metadata
         * @param suggestionsLimit the maximum number of suggestions to be returned
         * @return an array of {@link SentenceSuggestionsInfo} returned by
         * {@link SpellCheckerService.Session#onGetSuggestions(TextInfo, int)}
         */
        CARAPI OnGetSentenceSuggestionsMultiple(
            /* [in] */ ArrayOf<ITextInfo> textInfos,
            /* [in] */ Int32 suggestionsLimit,
            /* [out] */ ArrayOf<ISentenceSuggestionsInfo*>* result);

        /**
         * Request to abort all tasks executed in SpellChecker.
         * This function will run on the incoming IPC thread.
         * So, this is not called on the main thread,
         * but will be called in series on another thread.
         */
        CARAPI OnCancel();

        /**
         * Request to close this session.
         * This function will run on the incoming IPC thread.
         * So, this is not called on the main thread,
         * but will be called in series on another thread.
         */
        CARAPI OnClose();

        /**
         * @return Locale for this session
         */
        CARAPI GetLocale(
            /* [out] */ String* result);

        /**
         * @return Bundle for this session
         */
        CARAPI GetBundle(
            /* [out] */ IBundle** result);

    private:
        AutoPtr<IInternalISpellCheckerSession> mInternalSession;
        /*volatile*/ AutoPtr<ISentenceLevelAdapter> mSentenceLevelAdapter;
    };
    /**
     * Implement to return the implementation of the internal spell checker
     * service interface. Subclasses should not override.
     */
    const CARAPI OnBind(
        /* [in] */ const IIntent* intent,
        /* [out] */ IBinder** result);

    /**
     * Factory method to create a spell checker session impl
     * @return SpellCheckerSessionImpl which should be overridden by a concrete implementation.
     */
    /*abstract*/ CARAPI CreateSession(
        /* [out] */ ISession** result);

private:
    // Preventing from exposing ISpellCheckerSession.aidl, create an internal class.
    /*static*/
    class InternalISpellCheckerSession extends ISpellCheckerSessionStub
    {
    public:
        InternalISpellCheckerSession(
            /* [in] */ const String& locale,
            /* [in] */ IISpellCheckerSessionListener* listener,
            /* [in] */ IBundle* bundle,
            /* [in] */ ISession* session);

        CARAPI OnGetSuggestionsMultiple(
            /* [in] */ ArrayOf<ITextInfo> textInfos,
            /* [in] */ Int32 suggestionsLimit,
            /* [in] */ Boolean sequentialWords);

        CARAPI OnGetSentenceSuggestionsMultiple(
            /* [in] */ ArrayOf<ITextInfo> textInfos,
            /* [in] */ Int32 suggestionsLimit);

        CARAPI OnCancel();

        CARAPI OnClose();

        CARAPI GetLocale(
            /* [out] */ String* result);

        CARAPI GetBundle(
            /* [out] */ IBundle** result);

    private:
        AutoPtr<IISpellCheckerSessionListener> mListener;
        const AutoPtr<ISession> mSession;
        const String mLocale;
        const AutoPtr<IBundle> mBundle;
    };

    /*static*/
    class SpellCheckerServiceBinder extends ISpellCheckerServiceStub
    {
    public:
        SpellCheckerServiceBinder(
            /* [in] */ ISpellCheckerService* service);

        CARAPI GetISpellCheckerSession(
            /* [in] */ const String& locale,
            /* [in] */ IISpellCheckerSessionListener* listener,
            /* [in] */ IBundle* bundle,
            /* [out] */ IISpellCheckerSession** result);
    private:
        // final WeakReference<SpellCheckerService> mInternalServiceRef;
    };


    /**
     * Adapter class to accommodate word level spell checking APIs to sentence level spell checking
     * APIs used in
     * {@link SpellCheckerService.Session#onGetSuggestionsMultiple(TextInfo[], int, boolean)}
     */
    /*static*/
    class SentenceLevelAdapter
    {
    public:
        /**
         * Container for split TextInfo parameters
         */
        /*static*/
        class SentenceWordItem
        {
        public:
            SentenceWordItem(
                /* [in] */ ITextInfo* ti,
                /* [in] */ Int32 start,
                /* [in] */ Int32 end);

        public:
            const AutoPtr<ITextInfo> mTextInfo;
            const Int32 mStart;
            const Int32 mLength;
        };

        /**
         * Container for originally queried TextInfo and parameters
         */
        /*static*/
        class SentenceTextInfoParams
        {
        public:
            SentenceTextInfoParams(
                /* [in] */ ITextInfo* ti,
                /* [in] */ ArrayList<SentenceWordItem> items);
        private:
            const AutoPtr<ITextInfo> mOriginalTextInfo;
            const ArrayList<SentenceWordItem> mItems;
            const Int32 mSize;

        };

        SentenceLevelAdapter(
            /* [in] */ ILocale* locale);

        static CARAPI ReconstructSuggestions(
            /* [in] */ ISentenceTextInfoParams* originalTextInfoParams,
            /* [in] */ ArrayOf<ISuggestionsInfo> results,
            /* [out] */ ISentenceSuggestionsInfo* result);

    private:
        CARAPI_(ISentenceTextInfoParams) GetSplitWords(
            /* [in] */ ITextInfo* originalTextInfo);

    private:
        static const ArrayOf<ISentenceSuggestionsInfo> EMPTY_SENTENCE_SUGGESTIONS_INFOS ; // = new SentenceSuggestionsInfo[] {};
        static const AutoPtr<ISuggestionsInfo> EMPTY_SUGGESTIONS_INFO; // = new SuggestionsInfo(0, null);
        const AutoPtr<IWordIterator> mWordIterator;

    };
private:
    static const String TAG; // = SpellCheckerService.class.getSimpleName();
    static const Boolean DBG; // = false;
    const AutoPtr<ISpellCheckerServiceBinder> mBinder; // = new SpellCheckerServiceBinder(this);
};

} // namespace service
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVICE_CSPELLCHECKERSERVICE_H__
