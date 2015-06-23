
#include "widget/SpellChecker.h"

namespace Elastos {
namespace Droid {
namespace Widget {

const String SpellChecker::TAG = String("SpellChecker");
const Boolean SpellChecker::DBG = FALSE;
const Int32 SpellChecker::MAX_NUMBER_OF_WORDS;
const Int32 SpellChecker::AVERAGE_WORD_LENGTH;
const Int32 SpellChecker::WORD_ITERATOR_INTERVAL;
const Int32 SpellChecker::SPELL_PAUSE_DURATION;
const Int32 SpellChecker::MIN_SENTENCE_LENGTH;
const Int32 SpellChecker::USE_SPAN_RANGE;
const Int32 SpellChecker::SUGGESTION_SPAN_CACHE_SIZE;

SpellChecker::SpellChecker ()
    : mSpanSequenceCounter(0)
{
    mSpellParsers = ArrayOf<SpellParser>::Alloc(0);
}

SpellChecker::SpellChecker (
    /* [in] */ ITextView* textView)
    : mSpanSequenceCounter(0)
{
    /*mSpellParsers = ArrayOf<SpellParser>::Alloc(0);
    mTextView = textView;
    Int32 size = ArrayUtils::IdealObjectArraySize(1);
    mIds = ArrayOf<Int32>::Alloc(size);
    mSpellCheckSpans = ArrayOf<ISpellCheckSpan>::Alloc(size);
    AutoPtr<ILocale> locale;
    mTextView->GetTextServicesLocale((ILocale**)&locale);
    SetLocale(locale);*/
    //mCookie = GetHashCode();
}

void SpellChecker::CloseSession()
{
    /*if (mSpellCheckerSession) {
        mSpellCheckerSession->OnClose();
    }
    Int32 length = mSpellParsers->GetLength();
    for (Int32 i = 0; i < length; i++) {
        mSpellParsers[i]->Stop();
    }
    if (mSpellRunnable) {
        mTextView->RemoveCallbacks(mSpellRunnable);
    }*/
}

void SpellChecker::OnSpellCheckSpanRemoved(
    /* [in] */ ISpellCheckSpan* spellCheckSpan)
{
    /*for (Int32 i = 0; i < mLength; i++) {
        if ((*mSpellCheckSpans)[i] == spellCheckSpan) {
            (*mIds)[i] = -1;
            return;
        }
    }*/
}

void SpellChecker::OnSelectionChanged()
{
    //SpellCheck();
}

void SpellChecker::SpellCheck(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    /*AutoPtr<ILocale> locale;
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

    if (!isSessionActive) return;

    Int32 length = mSpellParsers->GetLength();
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<SpellParser> spellParser = (*mSpellParsers)[i];
        if (spellParser->IsFinished()) {
            spellParser->Parse(start, end);
            return;
        }
    }

    AutoPtr< ArrayOf<SpellParser> > newSpellParsers = ArrayOf<SpellParser>::Alloc(length + 1);
    mSpellParsers->Copy(0, newSpellParsers, 0, length);
    mSpellParsers = newSpellParsers;

    AutoPtr<SpellParser> spellParser = new SpellParser();
    (*mSpellParsers)[length] = spellParser;
    spellParser->Parse(start, end);*/
}

void SpellChecker::OnGetSuggestions(
    /* [in] */ ArrayOf<ISuggestionsInfo*>* results)
{
    /*AutoPtr<ICharSequence> csq;
    mTextView->GetText((ICharSequence**)&csq);
    AutoPtr<IEditable> editable = IEditable::Probe(csq);
    for (Int32 i = 0; i < results->GetLength(); ++i) {
        AutoPtr<ISpellCheckSpan> spellCheckSpan = OnGetSuggestionsInternal((*results)[i], USE_SPAN_RANGE, USE_SPAN_RANGE);
        if (spellCheckSpan) {
            AutoPtr<IInterface> interface = spellCheckSpan->Probe(EIID_IInterface);
            editable->RemoveSpan(interface);
        }
    }
    ScheduleNewSpellCheck();*/
}

void SpellChecker::onGetSentenceSuggestions(
    /* [in] */ ArrayOf<ISentenceSuggestionsInfo*>* results)
{
    /*AutoPtr<ICharSequence> csq;
    mTextView->GetText((ICharSequence**)&csq);
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
            AutoPtr<ISpellCheckSpan> scs = OnGetSuggestionsInternal(suggestionsInfo, offset, length);

            if (!spellCheckSpan && scs) {
                spellCheckSpan = scs;
            }
        }
        if (spellCheckSpan) {
            AutoPtr<IInterface> interface = spellCheckSpan->Probe(EIID_IInterface);
            editable->RemoveSpan(interface);
        }
    }
    ScheduleNewSpellCheck();*/
}

void SpellChecker::ResetSession()
{
    /*CloseSession();
    AutoPtr<IContext> context;
    mTextView->GetContext((IContext**)&context);
    AutoPtr<IInterface> interface;
    context->GetSystemService(IContext::TEXT_SERVICES_MANAGER_SERVICE, (IInterface**)&interface);
    mTextServicesManager = ITextServicesManager::Probe(interface);

    Boolean enabled = FALSE;
    mTextServicesManager->IsSpellCheckerEnabled(&enabled);
    AutoPtr<ISpellCheckerSubtype> type;
    mTextServicesManager->GetCurrentSpellCheckerSubtype(true, (ISpellCheckerSubtype**)&type);
    if (!enabled || !type) {
        mSpellCheckerSession = NULL;
    } else {
        mTextServicesManager->NewSpellCheckerSession(NULL, mCurrentLocale,
            (ISpellCheckerSessionListener*)this->Probe(EIID_ISpellCheckerSessionListener), FALSE, (ISpellCheckerSession**)&mSpellCheckerSession);
        mIsSentenceSpellCheckSupported = TRUE;
    }

    for (Int32 i = 0; i < mLength; i++) {
        mIds[i] = -1;
    }
    mLength = 0;
    AutoPtr<ICharSequence> csq;
    mTextView->GetText((ICharSequence**)&csq);
    AutoPtr<IEditable> editable = IEditable::Probe(csq);
    mTextView->RemoveMisspelledSpans(editable);*/
    //mSuggestionSpanCache.evictAll();
}

void SpellChecker::SetLocale(
    /* [in] */ ILocale* locale)
{
    /*mCurrentLocale = locale;
    ResetSession();

    CWordIterator::New(locale, (IWordIterator**)&mWordIterator);
    mTextView->OnLocaleChanged();*/
}

Boolean SpellChecker::IsSessionActive()
{
    //return mSpellCheckerSession != NULL;
}

Int32 SpellChecker::NextSpellCheckSpanIndex()
{
    /*for (Int32 i = 0; i < mLength; i++) {
        if ((*mIds)[i] < 0) return i;
    }

    if (mLength == mSpellCheckSpans->GetLength()) {
        Int32 newSize = mLength * 2;
        AutoPtr<ArrayOf<Int32> > newIds = ArrayOf<Int32>::Alloc(newSize);
        AutoPtr< ArrayOf<ISpellCheckSpan> > newSpellCheckSpans = ArrayOf<ISpellCheckSpan>::Alloc(newSize);
        mIds->Copy(0, newIds, 0, mLength);
        mSpellCheckSpans->Copy(0, newSpellCheckSpans, 0, mLength);
        mIds = newIds;
        mSpellCheckSpans = newSpellCheckSpans;
    }
    CSpellCheckSpan::New((ISpellCheckSpan**)&((*mSpellCheckSpans)[mLength]));
    mLength++;
    return mLength - 1;*/

}

void SpellChecker::AddSpellCheckSpan(
    /* [in] */ IEditable* editable,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    /*Int32 index = NextSpellCheckSpanIndex();
    AutoPtr<ISpellCheckSpan> spellCheckSpan = (*mSpellCheckSpans)[index];
    AutoPtr<IInterface> interface = spellCheckSpan->Probe(EIID_IInterface);
    editable->SetSpan(interface, start, end, ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
    spellCheckSpan->SetSpellCheckInProgress(FALSE);
    (*mIds)[index] = mSpanSequenceCounter++;*/
}

void SpellChecker::SpellCheck()
{
    /*if (!mSpellCheckerSession) return;
    AutoPtr<ICharSequence> csq;
    mTextView->GetText((ICharSequence**)&csq);
    AutoPtr<IEditable> editable = IEditable::Probe(csq);
    Int32 selectionStart = Selection::GetSelectionStart(editable);
    Int32 selectionEnd = Selection::GetSelectionEnd(editable);

    AutoPtr< ArrayOf<ITextInfo> > textInfos = ArrayOf<ITextInfo>::Alloc(mLength);
    Int32 textInfosCount = 0;

    for (Int32 i = 0; i < mLength; i++) {
        AutoPtr<ISpellCheckSpan> spellCheckSpan = (*mSpellCheckSpans)[i];
        Boolean res = FALSE;
        spellCheckSpan->IsSpellCheckInProgress(&res);
        if ((*mIds)[i] < 0 || res) continue;

        AutoPtr<IInterface> interface = spellCheckSpan->Probe(EIID_IInterface);
        Int32 start = 0, end = 0;
        editable->GetSpanStart(interface, &start);
        editable->GetSpanEnd(interface, &end);
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
                sb->SubString(start, end, &word);
            } else {
                AutoPtr<ICharSequence> chars;
                editable->SubSequence(start, end, (ICharSequence**)&chars);
                chars->ToString(&word);
            }

            spellCheckSpan->SetSpellCheckInProgress(TRUE);
            CTextInfo::New(word, mCookie, (*mIds)[i], (ITextInfo**)&((*textInfos)[textInfosCount++]));
        }
    }

    if (textInfosCount > 0) {
        if (textInfosCount < textInfos->GetLength()) {
            AutoPtr< ArrayOf<ITextInfo> > textInfosCopy = ArrayOf<ITextInfo>::Alloc(textInfosCount);
            textInfos->Copy(0, textInfosCopy, 0, textInfosCount);
            textInfos = textInfosCopy;
        }

        if (mIsSentenceSpellCheckSupported) {
            mSpellCheckerSession->GetSentenceSuggestions(textInfos, ISuggestionSpan::SUGGESTIONS_MAX_SIZE);
        } else {
            mSpellCheckerSession->GetSuggestions(textInfos, ISuggestionSpan::SUGGESTIONS_MAX_SIZE, FALSE);
        }
    }*/
}

AutoPtr<ISpellCheckSpan> SpellChecker::OnGetSuggestionsInternal(
    /* [in] */ ISuggestionsInfo* suggestionsInfo,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    /*Int32 cookie = 0;
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
                    AutoPtr<IInterface> interface = spellCheckSpan->Probe(EIID_IInterface);
                    editable->GetSpanStart(interface, &spellCheckSpanStart);
                    editable->GetSpanEnd(interface, &spellCheckSpanEnd);

                    if (offset != USE_SPAN_RANGE && length != USE_SPAN_RANGE) {
                        start = spellCheckSpanStart + offset;
                        end = start + length;
                    } else {
                        start = spellCheckSpanStart;
                        end = spellCheckSpanEnd;
                    }

                    if (spellCheckSpanStart >= 0 && spellCheckSpanEnd > spellCheckSpanStart && end > start) {
                        Int64 key = TextUtils::PackRangeInInt64(start, end);
                        AutoPtr<ISuggestionSpan> tempSuggestionSpan ;//= mSuggestionSpanCache.get(key);

                        if (tempSuggestionSpan) {
                            AutoPtr<IInterface> i = tempSuggestionSpan->Probe(EIID_IInterface);
                            editable->RemoveSpan(i);
                            //mSuggestionSpanCache.remove(key);
                        }
                    }
                }
            }
            return spellCheckSpan;
        }
    }*/
    return NULL;
}

void SpellChecker::ScheduleNewSpellCheck()
{
    /*if (!mSpellRunnable) {
        mSpellRunnable = new SpellCheckerRunnable();
    } else {
        mTextView->RemoveCallbacks(mSpellRunnable);
    }
    Boolean res = FALSE;
    mTextView->PostDelayed(mSpellRunnable, SPELL_PAUSE_DURATION, &res);*/
}

void SpellChecker::CreateMisspelledSuggestionSpan(
    /* [in] */ IEditable* editable,
    /* [in] */ ISuggestionsInfo* suggestionsInfo,
    /* [in] */ ISpellCheckSpan* spellCheckSpan,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    /*AutoPtr<IInterface> interface = spellCheckSpan->Probe(EIID_IInterface);
    Int32 spellCheckSpanStart = 0, spellCheckSpanEnd = 0;
    editable->GetSpanStart(interface, &spellCheckSpanStart);
    editable->GetSpanEnd(interface, &spellCheckSpanEnd);

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
        assert(0 && "TODO");
        //suggestions = ArrayUtils.emptyArray(String.class);
    }

    AutoPtr<IContext> context;
    mTextView->GetContext((IContext**)&context);
    AutoPtr<ISuggestionSpan> suggestionSpan;
    CSuggestionSpan::New(context, suggestions, ISuggestionSpan::FLAG_EASY_CORRECT | ISuggestionSpan::FLAG_MISSPELLED, (ISuggestionSpan**)&suggestionSpan);

    if (mIsSentenceSpellCheckSupported) {
        Int64 key == TextUtils::PackRangeInInt64(start, end);
        AutoPtr<ISuggestionSpan> tempSuggestionSpan;// = mSuggestionSpanCache.get(key);
        if (tempSuggestionSpan) {
            AutoPtr<IInterface> i = tempSuggestionSpan->Probe(EIID_IInterface);
            editable->RemoveSpan(i);
        }
        //mSuggestionSpanCache.put(key, suggestionSpan);
    }
    AutoPtr<IInterface> in = suggestionSpan->Probe(EIID_IInterface);
    editable->SetSpan(in, start, end, ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
    mTextView->InvalidateRegion(start, end, FALSE);*/
}

//=====================================================================
//                SpellChecker::SpellCheckerRunnable
//=====================================================================
SpellChecker::SpellCheckerRunnable::SpellCheckerRunnable(
    /* [in] */ SpellChecker* host)
    : mHost(host)
{

}

ECode SpellChecker::SpellCheckerRunnable::Run()
{
    /*Int32 length = mHost->mSpellParsers->GetLength();
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<SpellParser> spellParser = (*mSpellParsers)[i];
        if (!spellParser->IsFinished()) {
            spellParser->Parse();
            break;
        }
    }*/
}


//=====================================================================
//                SpellChecker::SpellParser
//=====================================================================
SpellChecker::SpellParser::SpellParser(
    /* [in] */ SpellChecker* host)
    : mHost(host)
{
    //mRange = new Object();
}

void SpellChecker::SpellParser::Parse(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    /*Int32 max = 0;
    mHost->mTextView->GetLength(&max);
    Int32 parseEnd = 0;
    if (end > max) {
        parseEnd = max;
    } else {
        parseEnd = end;
    }

    if (parseEnd > start) {
        AutoPtr<ICharSequence> csq;
        mHost->GetText((ICharSequence**)&csq);
        SetRangeSpan(IEditable::Probe(csq), start, parseEnd);
        Parse();
    }*/
}

Boolean SpellChecker::SpellParser::IsFinished()
{
    /*AutoPtr<ICharSequence> csq;
    mHost->GetText((ICharSequence**)&csq);
    AutoPtr<IEditable> editable = IEditable::Probe(csq);
    Int32 start = 0;
    editable->GetSpanStart(mRange, &start);
    return start < 0;*/
}

void SpellChecker::SpellParser::Stop()
{
    /*AutoPtr<ICharSequence> csq;
    mHost->GetText((ICharSequence**)&csq);
    RemoveRangeSpan(IEditable::Probe(csq));*/
}

void SpellChecker::SpellParser::Parse()
{
    /*AutoPtr<ICharSequence> csq;
    mHost->GetText((ICharSequence**)&csq);
    AutoPtr<IEditable> editable = IEditable::Probe(csq);
    Int32 start = 0;
    if (mHost->mIsSentenceSpellCheckSupported) {
        Int32 span = 0;
        editable->GetSpanStart(mRange, &span);
        start = Elastos::Core::Math::Max(0, span - mHost->MIN_SENTENCE_LENGTH);
    } else {
        start = span;
    }

    Int32 end = 0;
    editable->GetSpanEnd(mRange, &end);

    Int32 wordIteratorWindowEnd = Elastos::Core::Math::Min(end, start + mHost->WORD_ITERATOR_INTERVAL);
    mHost->mWordIterator->SetCharSequence(editable, start, wordIteratorWindowEnd);

    Int32 wordStart = 0, wordEnd = 0;
    mHost->mWordIterator->Preceding(start, &wordStart);

    if (wordStart == IBreakIterator::DONE) {
        mHost->mWordIterator->Following(start, &wordEnd);
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

    AutoPtr< ArrayOf<ISpellCheckSpan> > spellCheckSpans;
    Editable->GetSpans(start - 1, end + 1, EIID_SpellCheckSpan, (ArrayOf<ISpellCheckSpan>**)&spellCheckSpan);
    AutoPtr< ArrayOf<ISuggestionSpan> > suggestionSpans;
    Editable->GetSpans(start - 1, end + 1, EIID_SuggestionSpan, (ArrayOf<ISpellCheckSpan>**)&suggestionSpans);

    Int32 wordCount = 0;
    Boolean scheduleOtherSpellCheck = FALSE;

    if (mHost->mIsSentenceSpellCheckSupported) {
        if (wordIteratorWindowEnd < end) {
            scheduleOtherSpellCheck = TRUE;
        }
        Int32 spellCheckEnd = 0;
        mHost->mWordIterator->Preceding(wordIteratorWindowEnd, &spellCheckEnd);
        Boolean correct = spellCheckEnd != IBreakIterator::DONE;
        if (correct) {
            mWordIterator ->GetEnd(spellCheckEnd, &spellCheckEnd);
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
                AutoPtr<ISpellCheckSpan> spellCheckSpan = (*mHost->mSpellCheckSpans)[i];
                Boolean inProgress = FALSE;
                spellCheckSpan->IsSpellCheckInProgress(&inProgress);
                if ((*mHost->mIds)[i] < 0 || inProgress) {
                    continue;
                }
                Int32 spanStart = 0, spanEnd = 0;
                editable->GetSpanStart(spellCheckSpan->Probe(EIID_IInterface), &spanStart);
                editable->GetSpanEnd(spellCheckSpan->Probe(EIID_IInterface), &spanEnd);
                if (spanEnd < spellCheckStart || spellCheckEnd < spanStart) {
                    continue;
                }
                if (spanStart <= spellCheckStart && spellCheckEnd <= spanEnd) {
                    createSpellCheckSpan = FALSE;
                    break;
                }

                editable->RemoveSpan(spellCheckSpan->Probe(EIID_IInterface));
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
                AddSpellCheckSpan(editable, spellCheckStart, spellCheckEnd);
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
                    RemoveSpansAt(editable, start, spellCheckSpans);
                    RemoveSpansAt(editable, start, suggestionSpans);
                }

                if (wordStart < end && wordEnd > end) {
                    RemoveSpansAt(editable, end, spellCheckSpans);
                    RemoveSpansAt(editable, end, suggestionSpans);
                }

                Boolean createSpellCheckSpan = TRUE;
                if (wordEnd == start) {
                    for (Int32 i = 0; i < spellCheckSpans->GetLength(); i++) {
                        Int32 spanEnd = 0;
                        editable->GetSpanEnd(((*spellCheckSpans)[i])->Probe(EIID_IInterface), &spanEnd);
                        if (spanEnd == start) {
                            createSpellCheckSpan = FALSE;
                            break;
                        }
                    }
                }

                if (wordStart == end) {
                    for (Int32 i = 0; i < spellCheckSpans->GetLength(); i++) {
                        Int32 spanStart = 0;
                        editable->GetSpanStart(((*spellCheckSpans)[i])->Probe(EIID_IInterface), &spanStart);
                        if (spanStart == end) {
                            createSpellCheckSpan = FALSE;
                            break;
                        }
                    }
                }
                if (createSpellCheckSpan) {
                    AddSpellCheckSpan(editable, wordStart, wordEnd);
                }
                wordCount++;
            }
            Int32 originalWordEnd = wordEnd;
            mHost->mWordIterator->Following(wordEnd, &wordEnd);
            if ((wordIteratorWindowEnd < end) &&
                (wordEnd == IBreakIterator::DONE ||wordEnd >= wordIteratorWindowEnd)) {
                wordIteratorWindowEnd = Elastos::Core::Math::Min(end, originalWordEnd + mHost->WORD_ITERATOR_INTERVAL);
                mHost->mWordIterator->SetCharSequence(ICharSequence::Probe(editable), originalWordEnd, wordIteratorWindowEnd);
                mHost->mWordIterator->Following(originalWordEnd, &wordEnd);
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

    mHost->SpellCheck();*/
}

void SpellChecker::SpellParser::SetRangeSpan(
    /* [in] */ IEditable* editable,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    //editable->SetSpan(mRange, start, end, ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
}

void SpellChecker::SpellParser::RemoveRangeSpan (
    /* [in] */ IEditable* editable)
{
    //editable->RemoveSpan(mRange);
}

void SpellChecker::SpellParser::RemoveSpansAt(
    /* [in] */ IEditable* editable,
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<IInterface*>* spans)
{
    assert(0 && "TODO");
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos


