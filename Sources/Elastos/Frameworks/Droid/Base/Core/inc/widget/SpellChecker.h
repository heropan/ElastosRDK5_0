
#ifndef __SPELLCHECKER_H__
#define __SPELLCHECKER_H__

#include "ext/frameworkext.h"

using Libcore::ICU::ILocale;
using Elastos::Droid::Text::IEditable;
using Elastos::Droid::Text::Style::ISpellCheckSpan;
using Elastos::Droid::View::Textservice::ISuggestionsInfo;
using Elastos::Droid::View::Textservice::ISentenceSuggestionsInfo;
using Elastos::Droid::View::Textservice::ISpellCheckerSession;
using Elastos::Droid::View::Textservice::ITextServicesManager;
using Elastos::Droid::Text::Method::IWordIterator;

namespace Elastos {
namespace Droid {
namespace Widget {

class SpellChecker
{
private:
    class SpellCheckerRunnable
     : public Runnable
    {
    public:
        SpellCheckerRunnable(
            /* [in] */ SpellChecker* host);

        CARAPI Run();

    private:
        SpellChecker* mHost;
    };

    class SpellParser
        : public ElRefBase
    {
    public:
        SpellParser(
            /* [in] */ SpellChecker* host);

        CARAPI_(void) Parse(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end);

        CARAPI_(Boolean) IsFinished();

        CARAPI_(void) Stop();



        CARAPI_(void) Parse();

    private:
        CARAPI_(void) SetRangeSpan(
            /* [in] */ IEditable* editable,
            /* [in] */ Int32 start,
            /* [in] */ Int32 end);

        CARAPI_(void) RemoveRangeSpan (
            /* [in] */ IEditable* editable);

        CARAPI_(void) RemoveSpansAt(
            /* [in] */ IEditable* editable,
            /* [in] */ Int32 offset,
            /* [in] */ ArrayOf<IInterface*>* spans);
        //private <T> void removeSpansAt(Editable editable, int offset, T[] spans)
    private:
        AutoPtr<IInterface> mRange;
        SpellChecker* mHost;
    };

public:
    SpellChecker (
        /* [in] */ ITextView* textView);

    CARAPI_(void) CloseSession();

    CARAPI_(void) OnSpellCheckSpanRemoved(
        /* [in] */ ISpellCheckSpan* spellCheckSpan);

    CARAPI_(void) OnSelectionChanged();

    CARAPI_(void) SpellCheck(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(void) OnGetSuggestions(
        /* [in] */ ArrayOf<ISuggestionsInfo*>* results);

    CARAPI_(void) onGetSentenceSuggestions(
        /* [in] */ ArrayOf<ISentenceSuggestionsInfo*>* results);

protected:
    SpellChecker();

private:
    CARAPI_(void) ResetSession();

    CARAPI_(void) SetLocale(
        /* [in] */ ILocale* locale);

    CARAPI_(Boolean) IsSessionActive();

    CARAPI_(Int32) NextSpellCheckSpanIndex();

    CARAPI_(void) AddSpellCheckSpan(
        /* [in] */ IEditable* editable,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(void) SpellCheck();

    CARAPI_(AutoPtr<ISpellCheckSpan>) OnGetSuggestionsInternal(
        /* [in] */ ISuggestionsInfo* suggestionsInfo,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    CARAPI_(void) ScheduleNewSpellCheck();

    CARAPI_(void) CreateMisspelledSuggestionSpan(
        /* [in] */ IEditable* editable,
        /* [in] */ ISuggestionsInfo* suggestionsInfo,
        /* [in] */ ISpellCheckSpan* spellCheckSpan,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

private:
    static const String TAG;
    static const Boolean DBG;
    static const Int32 MAX_NUMBER_OF_WORDS = 50;
    static const Int32 AVERAGE_WORD_LENGTH = 7;
    static const Int32 WORD_ITERATOR_INTERVAL = MAX_NUMBER_OF_WORDS * AVERAGE_WORD_LENGTH;
    static const Int32 SPELL_PAUSE_DURATION = 400;
    static const Int32 MIN_SENTENCE_LENGTH = 50;
    static const Int32 USE_SPAN_RANGE = -1;
    static const Int32 SUGGESTION_SPAN_CACHE_SIZE = 10;

    AutoPtr<ITextView> mTextView;
    AutoPtr<ISpellCheckerSession> mSpellCheckerSession;

    Boolean mIsSentenceSpellCheckSupported;
    Int32 mCookie;
    AutoPtr< ArrayOf<Int32> > mIds;
    AutoPtr< ArrayOf<ISpellCheckSpan> > mSpellCheckSpans;
    Int32 mLength;
    AutoPtr< ArrayOf<SpellParser> > mSpellParsers;
    Int32 mSpanSequenceCounter;
    AutoPtr<ILocale> mCurrentLocale;
    AutoPtr<IWordIterator> mWordIterator;
    AutoPtr<ITextServicesManager> mTextServicesManager;
    AutoPtr<SpellCheckerRunnable> mSpellRunnable;

    /*private final LruCache<Long, SuggestionSpan> mSuggestionSpanCache =
            new LruCache<Long, SuggestionSpan>(SUGGESTION_SPAN_CACHE_SIZE);*/
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__SPELLCHECKER_H__

