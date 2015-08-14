
#ifndef __ELASTOS_DROID_VIEW_TEXTSERVICE_SPELLCHECKERSESSION_H__
#define __ELASTOS_DROID_VIEW_TEXTSERVICE_SPELLCHECKERSESSION_H__

namespace Elastos {
namespace Droid {
namespace View {
namespace Textservice {

/**
 * The SpellCheckerSession interface provides the per client functionality of SpellCheckerService.
 *
 *
 * <a name="Applications"></a>
 * <h3>Applications</h3>
 *
 * <p>In most cases, applications that are using the standard
 * {@link android.widget.TextView} or its subclasses will have little they need
 * to do to work well with spell checker services.  The main things you need to
 * be aware of are:</p>
 *
 * <ul>
 * <li> Properly set the {@link android.R.attr#inputType} in your editable
 * text views, so that the spell checker will have enough context to help the
 * user in editing text in them.
 * </ul>
 *
 * <p>For the rare people amongst us writing client applications that use the spell checker service
 * directly, you will need to use {@link #getSuggestions(TextInfo, int)} or
 * {@link #getSuggestions(TextInfo[], int, boolean)} for obtaining results from the spell checker
 * service by yourself.</p>
 *
 * <h3>Security</h3>
 *
 * <p>There are a lot of security issues associated with spell checkers,
 * since they could monitor all the text being sent to them
 * through, for instance, {@link android.widget.TextView}.
 * The Android spell checker framework also allows
 * arbitrary third party spell checkers, so care must be taken to restrict their
 * selection and interactions.</p>
 *
 * <p>Here are some key points about the security architecture behind the
 * spell checker framework:</p>
 *
 * <ul>
 * <li>Only the system is allowed to directly access a spell checker framework's
 * {@link android.service.textservice.SpellCheckerService} interface, via the
 * {@link android.Manifest.permission#BIND_TEXT_SERVICE} permission.  This is
 * enforced in the system by not binding to a spell checker service that does
 * not require this permission.
 *
 * <li>The user must explicitly enable a new spell checker in settings before
 * they can be enabled, to confirm with the system that they know about it
 * and want to make it available for use.
 * </ul>
 *
 */
class SpellCheckerSession
{
public:
    /**
     * Constructor
     * @hide
     */
    SpellCheckerSession(
        /* [in] */ ISpellCheckerInfo* info,
        /* [in] */ ITextServicesManager* tsm,
        /* [in] */ ISpellCheckerSessionListener* listener,
        /* [in] */ ISpellCheckerSubtype* subtype);

    /**
     * @return true if the connection to a text service of this session is disconnected and not
     * alive.
     */
    CARAPI_(Boolean) IsSessionDisconnected();

    /**
     * Get the spell checker service info this spell checker session has.
     * @return SpellCheckerInfo for the specified locale.
     */
    CARAPI_(AutoPtr<ISpellCheckerInfo>) GetSpellChecker();

    /**
     * Cancel pending and running spell check tasks
     */
    CARAPI_(void) Cancel();

    /**
     * Finish this session and allow TextServicesManagerService to disconnect the bound spell
     * checker.
     */
    CARAPI_(void) Close();

    /**
     * Get suggestions from the specified sentences
     * @param textInfos an array of text metadata for a spell checker
     * @param suggestionsLimit the maximum number of suggestions that will be returned
     */
    CARAPI_(void) GetSentenceSuggestions(
        /* [in] */ ArrayOf<ITextInfo*>* textInfos,
        /* [in] */ Int32 suggestionsLimit);

    /**
     * Get candidate strings for a substring of the specified text.
     * @param textInfo text metadata for a spell checker
     * @param suggestionsLimit the maximum number of suggestions that will be returned
     * @deprecated use {@link SpellCheckerSession#getSentenceSuggestions(TextInfo[], int)} instead
     */
    //@Deprecated
    CARAPI_(void) GetSuggestions(
        /* [in] */ ITextInfo* textInfo,
        /* [in] */ Int32 suggestionsLimit);

    /**
     * A batch process of getSuggestions
     * @param textInfos an array of text metadata for a spell checker
     * @param suggestionsLimit the maximum number of suggestions that will be returned
     * @param sequentialWords true if textInfos can be treated as sequential words.
     * @deprecated use {@link SpellCheckerSession#getSentenceSuggestions(TextInfo[], int)} instead
     */
    //@Deprecated
    CARAPI_(void) GetSuggestions(
        /* [in] */ ArrayOf<ITextInfo*>* textInfos,
        /* [in] */ Int32 suggestionsLimit,
        /* [in] */ Boolean sequentialWords);

    /**
     * @hide
     */
    CARAPI_(AutoPtr<ITextServicesSessionListener>) GetTextServicesSessionListener();

    /**
     * @hide
     */
    CARAPI_(AutoPtr<ISpellCheckerSessionListener>) GetSpellCheckerSessionListener();

protected:
    CARAPI_(void) Init(
        /* [in] */ ISpellCheckerInfo* info,
        /* [in] */ ITextServicesManager* tsm,
        /* [in] */ ISpellCheckerSessionListener* listener,
        /* [in] */ ISpellCheckerSubtype* subtype);

/*    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        if (mIsUsed) {
            Log.e(TAG, "SpellCheckerSession was not finished properly." +
                    "You should call finishShession() when you finished to use a spell checker.");
            close();
        }
    }*/

private:

    class SpellCheckerSessionListenerImpl;// extends ISpellCheckerSessionListener.Stub
    {
    public:
        SpellCheckerSessionListenerImpl(
            /* [in] */ Handler handler);

        /* synchronized */
        CARAPI_(void) OnServiceConnected(
            /* [in] */ ISpellCheckerSession* session);

        CARAPI_(void) Cancel();

        CARAPI_(void) GetSuggestionsMultiple(
            /* [in] */ ArrayOf<ITextInfo*>* textInfos,
            /* [in] */ Int32 suggestionsLimit,
            /* [in] */ Boolean sequentialWords);

        CARAPI_(void) GetSentenceSuggestionsMultiple(
            /* [in] */ ArrayOf<ITextInfo*>* textInfos,
            /* [in] */ Int32 suggestionsLimit);

        CARAPI_(void) Close();

        CARAPI_(Boolean) IsDisconnected();

        //@Override
        CARAPI_(void) OnGetSuggestions(
            /* [in] */ ArrayOf<ISuggestionsInfo*>* results);

        //@Override
        CARAPI_(void) OnGetSentenceSuggestions(
            /* [in] */ ArrayOf<ISentenceSuggestionsInfo*>* results);

    private:

        class SpellCheckerParams
        {
        public:
            SpellCheckerParams(
                /* [in] */ Int32 what,
                /* [in] */ ArrayOf<ITextInfo*>* textInfos,
                /* [in] */ Int32 suggestionsLimit,
                /* [in] */ Boolean sequentialWords)
            {
                mWhat = what;
                mTextInfos = textInfos;
                mSuggestionsLimit = suggestionsLimit;
                mSequentialWords = sequentialWords;
            }

        public:

            Int32 mWhat;
            AutoPtr<ArrayOf<ITextInfo*> > mTextInfos;
            Int32 mSuggestionsLimit;
            Boolean mSequentialWords;
            AutoPtr<ISpellCheckerSession> mSession;
        };

    private:

        static const Int32 TASK_CANCEL = 1;
        static const Int32 TASK_GET_SUGGESTIONS_MULTIPLE = 2;
        static const Int32 TASK_CLOSE = 3;
        static const Int32 TASK_GET_SUGGESTIONS_MULTIPLE_FOR_SENTENCE = 4;

    private:
        CARAPI_(void) ProcessTask(
            /* [in] */ ISpellCheckerSession* session,
            /* [in] */ SpellCheckerParams* scp,
            /* [in] */ Boolean async);

        CARAPI_(void) ProcessOrEnqueueTask(
            /* [in] */ SpellCheckerParams* scp);

    private:

//        private final Queue<SpellCheckerParams> mPendingTasks =
//                new LinkedList<SpellCheckerParams>();
        Handler mHandler;

        Boolean mOpened;
        AutoPtr<ISpellCheckerSession> mISpellCheckerSession;
        HandlerThread mThread;
        Handler mAsyncHandler;

        Object mLock;
    };

    class InternalListener //extends ITextServicesSessionListener.Stub
    {
    public:

        InternalListener(
            /* [in] */ SpellCheckerSessionListenerImpl* spellCheckerSessionListenerImpl);

        //@Override
        CARAPI_(void) OnServiceConnected(
            /* [in] */ ISpellCheckerSession* session);

    private:
        SpellCheckerSessionListenerImpl* mParentSpellCheckerSessionListenerImpl;
    };

private:
    CARAPI_(void) HandleOnGetSuggestionsMultiple(
        /* [in] */ ArrayOf<ISuggestionsInfo*>* suggestionInfos);

    CARAPI_(void) HandleOnGetSentenceSuggestionsMultiple(
        /* [in] */ ArrayOf<ISentenceSuggestionsInfo*>* suggestionInfos);

private:

    static const String TAG;// = SpellCheckerSession.class.getSimpleName();
    static const Boolean DBG = FALSE;

    static const Int32 MSG_ON_GET_SUGGESTION_MULTIPLE = 1;
    static const Int32 MSG_ON_GET_SUGGESTION_MULTIPLE_FOR_SENTENCE = 2;

    InternalListener* mInternalListener;
    AutoPtr<ITextServicesManager> mTextServicesManager;
    AutoPtr<ISpellCheckerInfo> mSpellCheckerInfo;
    AutoPtr<ISpellCheckerSessionListenerImpl> mSpellCheckerSessionListenerImpl;
    AutoPtr<ISpellCheckerSubtype> mSubtype;

    Boolean mIsUsed;
    AutoPtr<ISpellCheckerSessionListener> mSpellCheckerSessionListener;

    /** Handler that will execute the main tasks */
    Handler mHandler;/* = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MSG_ON_GET_SUGGESTION_MULTIPLE:
                    handleOnGetSuggestionsMultiple((SuggestionsInfo[]) msg.obj);
                    break;
                case MSG_ON_GET_SUGGESTION_MULTIPLE_FOR_SENTENCE:
                    handleOnGetSentenceSuggestionsMultiple((SentenceSuggestionsInfo[]) msg.obj);
                    break;
            }
        }
    };*/
};

}   //namespace Textservice
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__ELASTOS_DROID_VIEW_TEXTSERVICE_SPELLCHECKERSESSION_H__
