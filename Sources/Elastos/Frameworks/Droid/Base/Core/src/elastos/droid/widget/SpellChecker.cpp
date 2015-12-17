
#include "Elastos.Droid.Widget.h"
#include "Elastos.Droid.View.h"
#include <Elastos.CoreLibrary.Text.h>
#include "elastos/droid/widget/SpellChecker.h"
#include "elastos/droid/widget/TextView.h"
#include "elastos/droid/text/method/CWordIterator.h"
#include "elastos/droid/text/Selection.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/text/style/CSpellCheckSpan.h"
#include "elastos/droid/text/style/CSuggestionSpan.h"
#include "elastos/droid/view/textservice/CTextInfo.h"
#include "elastos/droid/internal/utility/ArrayUtils.h"
#include "elastos/droid/internal/utility/GrowingArrayUtils.h"

#include <elastos/core/Math.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Internal::Utility::ArrayUtils;
using Elastos::Droid::Internal::Utility::GrowingArrayUtils;
using Elastos::Droid::Text::ISpannable;
using Elastos::Droid::Text::Selection;
using Elastos::Droid::Text::ISelectionPositionIterator;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::Method::CWordIterator;
using Elastos::Droid::Text::Style::CSpellCheckSpan;
using Elastos::Droid::Text::Style::CSuggestionSpan;
using Elastos::Droid::Text::Style::EIID_ISpellCheckSpan;
using Elastos::Droid::Text::Style::EIID_ISuggestionSpan;
using Elastos::Droid::Text::ISpannableStringBuilder;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::TextService::ISpellCheckerSubtype;
using Elastos::Droid::View::TextService::EIID_ISpellCheckerSessionListener;
using Elastos::Droid::View::TextService::CTextInfo;
using Elastos::Droid::View::TextService::ITextInfo;

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Widget {

const String SpellChecker::TAG("SpellChecker");
const Boolean SpellChecker::DBG = FALSE;
const Int32 SpellChecker::MAX_NUMBER_OF_WORDS;
const Int32 SpellChecker::AVERAGE_WORD_LENGTH;
const Int32 SpellChecker::WORD_ITERATOR_INTERVAL;
const Int32 SpellChecker::SPELL_PAUSE_DURATION;
const Int32 SpellChecker::MIN_SENTENCE_LENGTH;
const Int32 SpellChecker::USE_SPAN_RANGE;
const Int32 SpellChecker::SUGGESTION_SPAN_CACHE_SIZE;

CAR_INTERFACE_IMPL(SpellChecker::MySpellCheckerSessionListener, Object, ISpellCheckerSessionListener)

SpellChecker::MySpellCheckerSessionListener::MySpellCheckerSessionListener(
    /* [in] */ SpellChecker* host)
    : mHost(host)
{}

ECode SpellChecker::MySpellCheckerSessionListener::OnGetSuggestions(
    /* [in] */ ArrayOf<ISuggestionsInfo*>* results)
{
    AutoPtr<ICharSequence> csq;
    mHost->mTextView->GetText((ICharSequence**)&csq);
    AutoPtr<IEditable> editable = IEditable::Probe(csq);
    for (Int32 i = 0; i < results->GetLength(); ++i) {
        AutoPtr<ISpellCheckSpan> spellCheckSpan = mHost->OnGetSuggestionsInternal((*results)[i], USE_SPAN_RANGE, USE_SPAN_RANGE);
        if (spellCheckSpan) {
            ISpannable::Probe(editable)->RemoveSpan(spellCheckSpan);
        }
    }
    mHost->ScheduleNewSpellCheck();
    return NOERROR;
}

ECode SpellChecker::MySpellCheckerSessionListener::OnGetSentenceSuggestions(
    /* [in] */ ArrayOf<ISentenceSuggestionsInfo*>* results)
{
    AutoPtr<ICharSequence> csq;
    mHost->mTextView->GetText((ICharSequence**)&csq);
    AutoPtr<IEditable> editable = IEditable::Probe(csq);

    for (Int32 i = 0; i < results->GetLength(); ++i) {
        AutoPtr<ISentenceSuggestionsInfo> ssi = (*results)[i];
        if (!ssi) {
            continue;
        }
        AutoPtr<ISpellCheckSpan> spellCheckSpan;
        Int32 count = 0;
        ssi->GetSuggestionsCount(&count);
        for (Int32 j = 0; i < count; ++j) {
            AutoPtr<ISuggestionsInfo> suggestionsInfo;
            ssi->GetSuggestionsInfoAt(j, (ISuggestionsInfo**)&suggestionsInfo);
            if (!suggestionsInfo) {
                continue;
            }

            Int32 offset = 0, length = 0;
            ssi->GetOffsetAt(j, &offset);
            ssi->GetLengthAt(j, &length);
            AutoPtr<ISpellCheckSpan> scs = mHost->OnGetSuggestionsInternal(suggestionsInfo, offset, length);

            if (!spellCheckSpan && scs) {
                spellCheckSpan = scs;
            }
        }
        if (spellCheckSpan) {
            ISpannable::Probe(editable)->RemoveSpan(spellCheckSpan);
        }
    }
    mHost->ScheduleNewSpellCheck();
    return NOERROR;
}

CAR_INTERFACE_IMPL(SpellChecker, Object, ISpellChecker)

SpellChecker::SpellChecker ()
    : mIsSentenceSpellCheckSupported(FALSE)
    , mCookie(0)
    , mLength(0)
    , mSpanSequenceCounter(0)
    , mSuggestionSpanCache(SUGGESTION_SPAN_CACHE_SIZE)
{
    mSpellParsers = ArrayOf<SpellParser*>::Alloc(0);
}

ECode SpellChecker::constructor (
    /* [in] */ ITextView* textView)
{
    mTextView = textView;
    Int32 size = 1;
    mIds = ArrayUtils::NewUnpaddedInt32Array(size);
    mSpellCheckSpans = ArrayOf<ISpellCheckSpan*>::Alloc(mIds->GetLength());
    AutoPtr<ILocale> locale;
    mTextView->GetTextServicesLocale((ILocale**)&locale);
    SetLocale(locale);
    GetHashCode(&mCookie);
    return NOERROR;
}

ECode SpellChecker::CloseSession()
{
    if (mSpellCheckerSession != NULL) {
        mSpellCheckerSession->Close();
    }
    Int32 length = mSpellParsers->GetLength();
    for (Int32 i = 0; i < length; i++) {
        (*mSpellParsers)[i]->Stop();
    }
    if (mSpellRunnable) {
        Boolean result;
        IView::Probe(mTextView)->RemoveCallbacks(mSpellRunnable, &result);
    }
    return NOERROR;
}

ECode SpellChecker::OnSpellCheckSpanRemoved(
    /* [in] */ ISpellCheckSpan* spellCheckSpan)
{
    for (Int32 i = 0; i < mLength; i++) {
        if ((*mSpellCheckSpans)[i] == spellCheckSpan) {
            (*mIds)[i] = -1;
            return NOERROR;
        }
    }
    return NOERROR;
}

ECode SpellChecker::OnSelectionChanged()
{
    SpellCheck();
    return NOERROR;
}

ECode SpellChecker::SpellCheck(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<ILocale> locale;
    mTextView->GetTextServicesLocale((ILocale**)&locale);
    Boolean isSessionActive = IsSessionActive();
    Boolean equals = FALSE;
    mCurrentLocale->Equals(locale, &equals);
    if (!mCurrentLocale || !equals) {
        SetLocale(locale);
        start = 0;
        AutoPtr<ICharSequence> csq;
        mTextView->GetText((ICharSequence**)&csq);
        csq->GetLength(&end);
    } else {
        Boolean spellCheckerActivated = FALSE;
        mTextServicesManager->IsSpellCheckerEnabled(&spellCheckerActivated);
        if (isSessionActive != spellCheckerActivated) {
            ResetSession();
        }
    }

    if (!isSessionActive) return NOERROR;

    Int32 length = mSpellParsers->GetLength();
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<SpellParser> spellParser = (*mSpellParsers)[i];
        if (spellParser->IsFinished()) {
            spellParser->Parse(start, end);
            return NOERROR;
        }
    }

    AutoPtr< ArrayOf<SpellParser*> > newSpellParsers = ArrayOf<SpellParser*>::Alloc(length + 1);
    newSpellParsers->Copy(0, mSpellParsers, 0, length);
    mSpellParsers = newSpellParsers;

    AutoPtr<SpellParser> spellParser = new SpellParser(this);
    mSpellParsers->Set(length, spellParser);
    spellParser->Parse(start, end);
    return NOERROR;
}

void SpellChecker::ResetSession()
{
    CloseSession();
    AutoPtr<IContext> context;
    IView::Probe(mTextView)->GetContext((IContext**)&context);
    AutoPtr<IInterface> tmp;
    context->GetSystemService(IContext::TEXT_SERVICES_MANAGER_SERVICE, (IInterface**)&tmp);
    mTextServicesManager = ITextServicesManager::Probe(tmp);

    Boolean enabled = FALSE;
    mTextServicesManager->IsSpellCheckerEnabled(&enabled);
    AutoPtr<ISpellCheckerSubtype> type;
    mTextServicesManager->GetCurrentSpellCheckerSubtype(TRUE, (ISpellCheckerSubtype**)&type);
    if (!enabled || !type) {
        mSpellCheckerSession = NULL;
    } else {
        AutoPtr<ISpellCheckerSessionListener> l = new MySpellCheckerSessionListener(this);
        mTextServicesManager->NewSpellCheckerSession(NULL, mCurrentLocale,
            l, FALSE, (ISpellCheckerSession**)&mSpellCheckerSession);
        mIsSentenceSpellCheckSupported = TRUE;
    }

    for (Int32 i = 0; i < mLength; i++) {
        (*mIds)[i] = -1;
    }
    mLength = 0;
    AutoPtr<ICharSequence> csq;
    mTextView->GetText((ICharSequence**)&csq);
    AutoPtr<IEditable> editable = IEditable::Probe(csq);
    ((TextView*)mTextView.Get())->RemoveMisspelledSpans(ISpannable::Probe(editable));
    mSuggestionSpanCache.EvictAll();
}

void SpellChecker::SetLocale(
    /* [in] */ ILocale* locale)
{
    mCurrentLocale = locale;
    ResetSession();

    CWordIterator::New(locale, (IWordIterator**)&mWordIterator);
    mTextView->OnLocaleChanged();
}

Boolean SpellChecker::IsSessionActive()
{
    return mSpellCheckerSession != NULL;
}

Int32 SpellChecker::NextSpellCheckSpanIndex()
{
    for (Int32 i = 0; i < mLength; i++) {
        if ((*mIds)[i] < 0) return i;
    }

    mIds = GrowingArrayUtils::Append(mIds, mLength, 0);
    AutoPtr<ISpellCheckSpan> tmp;
    CSpellCheckSpan::New((ISpellCheckSpan**)&tmp);
    mSpellCheckSpans = GrowingArrayUtils::Append(
            mSpellCheckSpans, mLength, tmp.Get());
    mLength++;
    return mLength - 1;

}

void SpellChecker::AddSpellCheckSpan(
    /* [in] */ IEditable* editable,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Int32 index = NextSpellCheckSpanIndex();
    AutoPtr<ISpellCheckSpan> spellCheckSpan = (*mSpellCheckSpans)[index];
    ISpannable::Probe(editable)->SetSpan(spellCheckSpan, start, end, ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
    spellCheckSpan->SetSpellCheckInProgress(FALSE);
    (*mIds)[index] = mSpanSequenceCounter++;
}

void SpellChecker::SpellCheck()
{
    if (!mSpellCheckerSession) return;
    AutoPtr<ICharSequence> csq;
    mTextView->GetText((ICharSequence**)&csq);
    AutoPtr<IEditable> editable = IEditable::Probe(csq);
    Int32 selectionStart = Selection::GetSelectionStart(ICharSequence::Probe(editable));
    Int32 selectionEnd = Selection::GetSelectionEnd(ICharSequence::Probe(editable));

    AutoPtr< ArrayOf<ITextInfo*> > textInfos = ArrayOf<ITextInfo*>::Alloc(mLength);
    Int32 textInfosCount = 0;

    for (Int32 i = 0; i < mLength; i++) {
        AutoPtr<ISpellCheckSpan> spellCheckSpan = (*mSpellCheckSpans)[i];
        Boolean res = FALSE;
        spellCheckSpan->IsSpellCheckInProgress(&res);
        if ((*mIds)[i] < 0 || res) continue;

        Int32 start = 0, end = 0;
        ISpanned::Probe(editable)->GetSpanStart(spellCheckSpan, &start);
        ISpanned::Probe(editable)->GetSpanEnd(spellCheckSpan, &end);
        Boolean isEditing = FALSE;
        if (mIsSentenceSpellCheckSupported) {
            isEditing = selectionEnd <= start || selectionStart > end;
        } else {
            isEditing = selectionEnd < start || selectionStart > end;
        }

        if (start >= 0 && end > start && isEditing) {
            String word = String(NULL);
            if (ISpannableStringBuilder::Probe(editable)) {
                AutoPtr<ISpannableStringBuilder> sb = ISpannableStringBuilder::Probe(editable);
                sb->Substring(start, end, &word);
            } else {
                AutoPtr<ICharSequence> chars;
                ICharSequence::Probe(editable)->SubSequence(start, end, (ICharSequence**)&chars);
                chars->ToString(&word);
            }

            spellCheckSpan->SetSpellCheckInProgress(TRUE);
            AutoPtr<ITextInfo> info;
            CTextInfo::New(word, mCookie, (*mIds)[i], (ITextInfo**)&info);
            textInfos->Set(textInfosCount++, info);
        }
    }

    if (textInfosCount > 0) {
        if (textInfosCount < textInfos->GetLength()) {
            AutoPtr< ArrayOf<ITextInfo*> > textInfosCopy = ArrayOf<ITextInfo*>::Alloc(textInfosCount);
            textInfosCopy->Copy(0, textInfos, 0, textInfosCount);
            textInfos = textInfosCopy;
        }

        if (mIsSentenceSpellCheckSupported) {
            mSpellCheckerSession->GetSentenceSuggestions(textInfos, ISuggestionSpan::SUGGESTIONS_MAX_SIZE);
        } else {
            mSpellCheckerSession->GetSuggestions(textInfos, ISuggestionSpan::SUGGESTIONS_MAX_SIZE, FALSE);
        }
    }
}

AutoPtr<ISpellCheckSpan> SpellChecker::OnGetSuggestionsInternal(
    /* [in] */ ISuggestionsInfo* suggestionsInfo,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    Int32 cookie = 0;
    suggestionsInfo->GetCookie(&cookie);
    if (!suggestionsInfo || cookie != mCookie) {
        return NULL;
    }
    AutoPtr<ICharSequence> csq;
    mTextView->GetText((ICharSequence**)&csq);
    AutoPtr<IEditable> editable = IEditable::Probe(csq);
    Int32 sequenceNumber = 0;
    suggestionsInfo->GetSequence(&sequenceNumber);

    for(Int32 k = 0; k < mLength; ++k) {
        if (sequenceNumber == (*mIds)[k]) {
            Int32 attributes = 0;
            suggestionsInfo->GetSuggestionsAttributes(&attributes);
            Boolean isInDictionary = ((attributes & ISuggestionsInfo::RESULT_ATTR_IN_THE_DICTIONARY) > 0);
            Boolean looksLikeTypo = ((attributes & ISuggestionsInfo::RESULT_ATTR_LOOKS_LIKE_TYPO) > 0);

            AutoPtr<ISpellCheckSpan> spellCheckSpan = (*mSpellCheckSpans)[k];
            if (!isInDictionary && looksLikeTypo) {
                CreateMisspelledSuggestionSpan(editable, suggestionsInfo, spellCheckSpan, offset, length);
            } else {
                if (mIsSentenceSpellCheckSupported) {
                    Int32 spellCheckSpanStart = 0, spellCheckSpanEnd = 0, start = 0, end = 0;
                    ISpanned::Probe(editable)->GetSpanStart(spellCheckSpan, &spellCheckSpanStart);
                    ISpanned::Probe(editable)->GetSpanEnd(spellCheckSpan, &spellCheckSpanEnd);

                    if (offset != USE_SPAN_RANGE && length != USE_SPAN_RANGE) {
                        start = spellCheckSpanStart + offset;
                        end = start + length;
                    } else {
                        start = spellCheckSpanStart;
                        end = spellCheckSpanEnd;
                    }

                    if (spellCheckSpanStart >= 0 && spellCheckSpanEnd > spellCheckSpanStart && end > start) {
                        Int64 key = TextUtils::PackRangeInInt64(start, end);
                        AutoPtr<ISuggestionSpan> tempSuggestionSpan = mSuggestionSpanCache.Get(key);

                        if (tempSuggestionSpan) {
                            ISpannable::Probe(editable)->RemoveSpan(tempSuggestionSpan);
                            mSuggestionSpanCache.Remove(key);
                        }
                    }
                }
            }
            return spellCheckSpan;
        }
    }
    return NULL;
}

void SpellChecker::ScheduleNewSpellCheck()
{
    Boolean res = FALSE;
    if (!mSpellRunnable) {
        mSpellRunnable = new SpellCheckerRunnable(this);
    } else {
        IView::Probe(mTextView)->RemoveCallbacks(mSpellRunnable, &res);
    }
    IView::Probe(mTextView)->PostDelayed(mSpellRunnable, SPELL_PAUSE_DURATION, &res);
}

void SpellChecker::CreateMisspelledSuggestionSpan(
    /* [in] */ IEditable* editable,
    /* [in] */ ISuggestionsInfo* suggestionsInfo,
    /* [in] */ ISpellCheckSpan* spellCheckSpan,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    Int32 spellCheckSpanStart = 0, spellCheckSpanEnd = 0;
    ISpanned::Probe(editable)->GetSpanEnd(spellCheckSpan, &spellCheckSpanStart);
    ISpanned::Probe(editable)->GetSpanEnd(spellCheckSpan, &spellCheckSpanEnd);

    if (spellCheckSpanStart < 0 || spellCheckSpanEnd <= spellCheckSpanStart) return;

    Int32 start = 0, end =0;
    if (offset != USE_SPAN_RANGE && length != USE_SPAN_RANGE) {
        start = spellCheckSpanStart + offset;
        end = start + length;
    } else {
        start = spellCheckSpanStart;
        end = spellCheckSpanEnd;
    }

    Int32 suggestionsCount = 0;
    suggestionsInfo->GetSuggestionsCount(&suggestionsCount);
    AutoPtr< ArrayOf<String> > suggestions;
    if (suggestionsCount > 0) {
        suggestions = ArrayOf<String>::Alloc(suggestionsCount);
        for (Int32 i = 0; i < suggestionsCount; i++) {
            suggestionsInfo->GetSuggestionAt(i, &((*suggestions)[i]));
        }
    } else {
        suggestions = ArrayOf<String>::Alloc(0);
    }

    AutoPtr<IContext> context;
    IView::Probe(mTextView)->GetContext((IContext**)&context);
    AutoPtr<ISuggestionSpan> suggestionSpan;
    CSuggestionSpan::New(context, suggestions, ISuggestionSpan::FLAG_EASY_CORRECT | ISuggestionSpan::FLAG_MISSPELLED, (ISuggestionSpan**)&suggestionSpan);

    if (mIsSentenceSpellCheckSupported) {
        Int64 key = TextUtils::PackRangeInInt64(start, end);
        AutoPtr<ISuggestionSpan> tempSuggestionSpan = mSuggestionSpanCache.Get(key);
        if (tempSuggestionSpan) {
            ISpannable::Probe(editable)->RemoveSpan(tempSuggestionSpan);
        }
        //mSuggestionSpanCache.put(key, suggestionSpan);
    }
    ISpannable::Probe(editable)->SetSpan(suggestionSpan, start, end, ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
    mTextView->InvalidateRegion(start, end, FALSE);
}

//=====================================================================
//                SpellChecker::SpellCheckerRunnable
//=====================================================================
SpellChecker::SpellCheckerRunnable::SpellCheckerRunnable(
    /* [in] */ SpellChecker* host)
    : mHost(host)
{}

ECode SpellChecker::SpellCheckerRunnable::Run()
{
    AutoPtr<ArrayOf<SpellParser*> > parsers = mHost->mSpellParsers;
    Int32 length = parsers->GetLength();
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<SpellParser> spellParser = (*parsers)[i];
        if (!spellParser->IsFinished()) {
            spellParser->Parse();
            break; // run one spell parser at a time to bound running time
        }
    }
    return NOERROR;
}


//=====================================================================
//                SpellChecker::SpellParser
//=====================================================================
SpellChecker::SpellParser::SpellParser(
    /* [in] */ SpellChecker* host)
    : mHost(host)
{
    mRange = new Object();
}

void SpellChecker::SpellParser::Parse(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Int32 max = 0;
    mHost->mTextView->GetLength(&max);
    Int32 parseEnd = 0;
    if (end > max) {
        parseEnd = max;
    } else {
        parseEnd = end;
    }

    if (parseEnd > start) {
        AutoPtr<ICharSequence> csq;
        mHost->mTextView->GetText((ICharSequence**)&csq);
        SetRangeSpan(IEditable::Probe(csq), start, parseEnd);
        Parse();
    }
}

Boolean SpellChecker::SpellParser::IsFinished()
{
    AutoPtr<ICharSequence> csq;
    mHost->mTextView->GetText((ICharSequence**)&csq);
    AutoPtr<IEditable> editable = IEditable::Probe(csq);
    Int32 start = 0;
    ISpanned::Probe(editable)->GetSpanStart(mRange, &start);
    return start < 0;
}

void SpellChecker::SpellParser::Stop()
{
    AutoPtr<ICharSequence> csq;
    mHost->mTextView->GetText((ICharSequence**)&csq);
    RemoveRangeSpan(IEditable::Probe(csq));
}

void SpellChecker::SpellParser::Parse()
{
    AutoPtr<ICharSequence> csq;
    mHost->mTextView->GetText((ICharSequence**)&csq);
    AutoPtr<IEditable> editable = IEditable::Probe(csq);
    Int32 start = 0;
    ISpanned::Probe(editable)->GetSpanStart(mRange, &start);
    if (mHost->mIsSentenceSpellCheckSupported) {
        start = Elastos::Core::Math::Max(0, start - mHost->MIN_SENTENCE_LENGTH);
    }

    Int32 end = 0;
    ISpanned::Probe(editable)->GetSpanEnd(mRange, &end);

    Int32 wordIteratorWindowEnd = Elastos::Core::Math::Min(end, start + mHost->WORD_ITERATOR_INTERVAL);
    mHost->mWordIterator->SetCharSequence(ICharSequence::Probe(editable), start, wordIteratorWindowEnd);

    Int32 wordStart = 0, wordEnd = 0;
    ISelectionPositionIterator::Probe(mHost->mWordIterator)->Preceding(start, &wordStart);

    if (wordStart == IBreakIterator::DONE) {
        ISelectionPositionIterator::Probe(mHost->mWordIterator)->Following(start, &wordEnd);
        if (wordEnd != IBreakIterator::DONE) {
            mHost->mWordIterator->GetBeginning(wordEnd, &wordStart);
        }
    } else {
        mHost->mWordIterator->GetEnd(wordStart, &wordEnd);
    }

    if (wordEnd == IBreakIterator::DONE) {
        RemoveRangeSpan(editable);
        return;
    }

    AutoPtr< ArrayOf<ISpellCheckSpan*> > spellCheckSpans;
    ISpanned::Probe(editable)->GetSpans(start - 1, end + 1, EIID_ISpellCheckSpan, (ArrayOf<IInterface*>**)&spellCheckSpans);
    AutoPtr< ArrayOf<ISuggestionSpan*> > suggestionSpans;
    ISpanned::Probe(editable)->GetSpans(start - 1, end + 1, EIID_ISuggestionSpan, (ArrayOf<IInterface*>**)&suggestionSpans);

    Int32 wordCount = 0;
    Boolean scheduleOtherSpellCheck = FALSE;

    if (mHost->mIsSentenceSpellCheckSupported) {
        if (wordIteratorWindowEnd < end) {
            scheduleOtherSpellCheck = TRUE;
        }
        Int32 spellCheckEnd = 0;
        ISelectionPositionIterator::Probe(mHost->mWordIterator)->Preceding(wordIteratorWindowEnd, &spellCheckEnd);
        Boolean correct = spellCheckEnd != IBreakIterator::DONE;
        if (correct) {
            mHost->mWordIterator->GetEnd(spellCheckEnd, &spellCheckEnd);
            correct = spellCheckEnd != IBreakIterator::DONE;
        }

        if (!correct) {
            RemoveRangeSpan(editable);
            return;
        }

        do {
            Int32 spellCheckStart = wordStart;
            Boolean createSpellCheckSpan = TRUE;
            for (Int32 i = 0; i < mHost->mLength; ++i) {
                AutoPtr<ArrayOf<ISpellCheckSpan*> > spans = mHost->mSpellCheckSpans;
                AutoPtr<ISpellCheckSpan> spellCheckSpan = (*spans)[i];
                Boolean inProgress = FALSE;
                spellCheckSpan->IsSpellCheckInProgress(&inProgress);
                if ((*mHost->mIds)[i] < 0 || inProgress) {
                    continue;
                }
                Int32 spanStart = 0, spanEnd = 0;
                ISpanned::Probe(editable)->GetSpanStart(spellCheckSpan, &spanStart);
                ISpanned::Probe(editable)->GetSpanEnd(spellCheckSpan, &spanEnd);
                if (spanEnd < spellCheckStart || spellCheckEnd < spanStart) {
                    continue;
                }
                if (spanStart <= spellCheckStart && spellCheckEnd <= spanEnd) {
                    createSpellCheckSpan = FALSE;
                    break;
                }

                ISpannable::Probe(editable)->RemoveSpan(spellCheckSpan);
                spellCheckStart = Elastos::Core::Math::Min(spanStart, spellCheckStart);
                spellCheckEnd = Elastos::Core::Math::Max(spanEnd, spellCheckEnd);
            }

            if (spellCheckEnd < start) {
                break;
            }

            if (spellCheckEnd <= spellCheckStart) {
                break;
            }
            if (createSpellCheckSpan) {
                mHost->AddSpellCheckSpan(editable, spellCheckStart, spellCheckEnd);
            }
        } while(FALSE);
        wordStart = spellCheckEnd;
    } else {
        while (wordStart <= end) {
            if (wordEnd >= start && wordEnd > wordStart) {
                if (wordCount >= mHost->MAX_NUMBER_OF_WORDS) {
                    scheduleOtherSpellCheck = TRUE;
                    break;
                }
                if (wordStart < start && wordEnd > start) {
                    RemoveSpansAt(editable, start, (ArrayOf<IInterface*>*)spellCheckSpans.Get());
                    RemoveSpansAt(editable, start, (ArrayOf<IInterface*>*)suggestionSpans.Get());
                }

                if (wordStart < end && wordEnd > end) {
                    RemoveSpansAt(editable, end, (ArrayOf<IInterface*>*)spellCheckSpans.Get());
                    RemoveSpansAt(editable, end, (ArrayOf<IInterface*>*)suggestionSpans.Get());
                }

                Boolean createSpellCheckSpan = TRUE;
                if (wordEnd == start) {
                    for (Int32 i = 0; i < spellCheckSpans->GetLength(); i++) {
                        Int32 spanEnd = 0;
                        ISpanned::Probe(editable)->GetSpanEnd((*spellCheckSpans)[i], &spanEnd);
                        if (spanEnd == start) {
                            createSpellCheckSpan = FALSE;
                            break;
                        }
                    }
                }

                if (wordStart == end) {
                    for (Int32 i = 0; i < spellCheckSpans->GetLength(); i++) {
                        Int32 spanStart = 0;
                        ISpanned::Probe(editable)->GetSpanStart((*spellCheckSpans)[i], &spanStart);
                        if (spanStart == end) {
                            createSpellCheckSpan = FALSE;
                            break;
                        }
                    }
                }
                if (createSpellCheckSpan) {
                    mHost->AddSpellCheckSpan(editable, wordStart, wordEnd);
                }
                wordCount++;
            }
            Int32 originalWordEnd = wordEnd;
            ISelectionPositionIterator::Probe(mHost->mWordIterator)->Following(wordEnd, &wordEnd);
            if ((wordIteratorWindowEnd < end) &&
                (wordEnd == IBreakIterator::DONE ||wordEnd >= wordIteratorWindowEnd)) {
                wordIteratorWindowEnd = Elastos::Core::Math::Min(end, originalWordEnd + mHost->WORD_ITERATOR_INTERVAL);
                mHost->mWordIterator->SetCharSequence(ICharSequence::Probe(editable), originalWordEnd, wordIteratorWindowEnd);
                ISelectionPositionIterator::Probe(mHost->mWordIterator)->Following(originalWordEnd, &wordEnd);
            }

            if (wordEnd == IBreakIterator::DONE) break;
            mHost->mWordIterator->GetBeginning(wordEnd, &wordStart);
            if (wordStart == IBreakIterator::DONE) break;
        }
    }

    if (scheduleOtherSpellCheck) {
        SetRangeSpan(editable, wordStart, end);
    } else {
        RemoveRangeSpan(editable);
    }

    mHost->SpellCheck();
}

void SpellChecker::SpellParser::SetRangeSpan(
    /* [in] */ IEditable* editable,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    ISpannable::Probe(editable)->SetSpan(mRange, start, end, ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
}

void SpellChecker::SpellParser::RemoveRangeSpan (
    /* [in] */ IEditable* editable)
{
    ISpannable::Probe(editable)->RemoveSpan(mRange);
}

void SpellChecker::SpellParser::RemoveSpansAt(
    /* [in] */ IEditable* editable,
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<IInterface*>* spans)
{
    Int32 length = spans->GetLength();
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IInterface> span = (*spans)[i];
        Int32 start;
        ISpanned::Probe(editable)->GetSpanStart(span, &start);
        if (start > offset) continue;
        Int32 end;
        ISpanned::Probe(editable)->GetSpanEnd(span, &end);
        if (end < offset) continue;
        ISpannable::Probe(editable)->RemoveSpan(span);
    }
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos


