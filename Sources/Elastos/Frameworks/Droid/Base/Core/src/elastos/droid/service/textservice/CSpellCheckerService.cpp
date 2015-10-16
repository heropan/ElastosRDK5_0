
#include "elastos/droid/media/CSpellCheckerService.h"
#include <elastos/utility/logging/Logger.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;
using namespace Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace service {

ECode CSpellCheckerService::OnBind(
    /* [in] */ const IIntent* intent,
    /* [out] */ IBinder** result)
{
    VALIDATE_NOT_NULL(result);
    if (DBG) {
        Logger::W(TAG, "onBind");
    }
    *result = mBinder;
    return NOERROR;
}

ECode CSpellCheckerService::CreateSession(
    /* [out] */ ISession** result)
{
    return NOERROR;
}

ECode CSpellCheckerService::Session::SetInternalISpellCheckerSession(
    /* [in] */ IInternalISpellCheckerSession* session)
{
    mInternalSession = session;
    return NOERROR;
}

ECode CSpellCheckerService::Session::OnCreate()
{
    return NOERROR;
}

ECode CSpellCheckerService::Session::OnGetSuggestions(
    /* [in] */ ITextInfo* textInfo,
    /* [in] */ Int32 suggestionsLimit,
    /* [out] */ ISuggestionsInfo** result)
{
    return NOERROR;
}

ECode CSpellCheckerService::Session::OnGetSuggestionsMultiple(
    /* [in] */ ArrayOf<ITextInfo> textInfos,
    /* [in] */ Int32 suggestionsLimit,
    /* [in] */ Boolean sequentialWords,
    /* [out] */ ArrayOf<SuggestionsInfo*>* result)
{
    const Int32 length = textInfos::length;
    const AutoPtr< ArrayOf<ISuggestionsInfo> > retval = ArrayOf<ISuggestionsInfo>::Alloc(length);
    for (Int32 i = 0; i < length; ++i) {
        retval[i] = OnGetSuggestions(textInfos[i], suggestionsLimit);
        retval[i]->SetCookieAndSequence(
                textInfos[i]->GetCookie(), textInfos[i]->GetSequence());
    }
    *result = retval;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CSpellCheckerService::Session::OnGetSentenceSuggestionsMultiple(
    /* [in] */ ArrayOf<ITextInfo> textInfos,
    /* [in] */ Int32 suggestionsLimit,
    /* [out] */ ArrayOf<ISentenceSuggestionsInfo*>* result)
{
    VALIDATE_NOT_NULL(result);
    if (textInfos == NULL || textInfos::length == 0) {
        *result = SentenceLevelAdapter.EMPTY_SENTENCE_SUGGESTIONS_INFOS;
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
    if (DBG) {
        Logger::D(TAG, "onGetSentenceSuggestionsMultiple: + " + textInfos.length + ", " + suggestionsLimit);
    }
    if (mSentenceLevelAdapter == NULL) {
        {
            AutoLock lock(&this);
            if (mSentenceLevelAdapter == NULL) {
                const String localeStr = GetLocale();
                if (!TextUtils->IsEmpty(localeStr)) {
                    AutoPtr<Locale> locale = new Locale(localeStr);
                    mSentenceLevelAdapter = new SentenceLevelAdapter(locale);
                }
            }
        }
    }
    if (mSentenceLevelAdapter == NULL) {
        *result = SentenceLevelAdapter::EMPTY_SENTENCE_SUGGESTIONS_INFOS;
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
    const Int32 infosSize = textInfos::length;
    const AutoPtr< ArrayOf<ISentenceSuggestionsInfo> > retval =ArrayOf<ISentenceSuggestionsInfo>::Alloc(infosSize);
    for (Int32 i = 0; i < infosSize; ++i) {
        const SentenceLevelAdapter::SentenceTextInfoParams textInfoParams =
                mSentenceLevelAdapter->GetSplitWords(textInfos[i]);
        const ArrayList<SentenceLevelAdapter::SentenceWordItem> mItems =
                textInfoParams::mItems;
        const Int32 itemsSize = mItems->Size();
        const AutoPtr< ArrayOf<ITextInfo> > splitTextInfos = ArrayOf<ITextInfo>::Alloc(itemsSize);
        for (Int32 j = 0; j < itemsSize; ++j) {
            splitTextInfos[j] = mItems->Get(j).mTextInfo;
        }
        retval[i] = SentenceLevelAdapter->ReconstructSuggestions(
                textInfoParams, OnGetSuggestionsMultiple(splitTextInfos, suggestionsLimit, TRUE));
    }
    *result = retval;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CSpellCheckerService::Session::OnCancel()
{
    return NOERROR;
}

ECode CSpellCheckerService::Session::OnClose()
{
    return NOERROR;
}

ECode CSpellCheckerService::Session::GetLocale(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mInternalSession.getLocale();
    return NOERROR;
}

ECode CSpellCheckerService::Session::GetBundle(
    /* [out] */ IBundle** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mInternalSession.getBundle();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

CSpellCheckerService::InternalISpellCheckerSession::InternalISpellCheckerSession(
    /* [in] */ const String& locale,
    /* [in] */ IISpellCheckerSessionListener* listener,
    /* [in] */ IBundle* bundle,
    /* [in] */ ISession* session)
    : mListener(listener)
    , mSession(session)
    , mLocale(locale)
    , mBundle(bundle)
{
    session.setInternalISpellCheckerSession(this);
}

ECode CSpellCheckerService::InternalISpellCheckerSession::OnGetSuggestionsMultiple(
    /* [in] */ ArrayOf<ITextInfo> textInfos,
    /* [in] */ Int32 suggestionsLimit,
    /* [in] */ Boolean sequentialWords)
{
    Int32 pri = Process->GetThreadPriority(Process->MyTid());
    // try {
        Process->SetThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);
        mListener->OnGetSuggestions(
                mSession->OnGetSuggestionsMultiple(
                        textInfos, suggestionsLimit, sequentialWords));
    // } catch (RemoteException e) {
    // } finally {
        Process->SetThreadPriority(pri);
    // }
    return NOERROR;
}

ECode CSpellCheckerService::InternalISpellCheckerSession::OnGetSentenceSuggestionsMultiple(
    /* [in] */ ArrayOf<ITextInfo> textInfos,
    /* [in] */ Int32 suggestionsLimit)
{
    // try {
        mListener->OnGetSentenceSuggestions(
                mSession->OnGetSentenceSuggestionsMultiple(textInfos, suggestionsLimit));
    // } catch (RemoteException e) {
    // }
    return NOERROR;
}

ECode CSpellCheckerService::InternalISpellCheckerSession::OnCancel()
{
    Int32 pri = Process->GetThreadPriority(Process->MyTid());
    // try {
        Process->SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);
        mSession->OnCancel();
    // } finally {
        Process->SetThreadPriority(pri);
    // }
    return NOERROR;
}

ECode CSpellCheckerService::InternalISpellCheckerSession::OnClose()
{
    Int32 pri = Process->GetThreadPriority(Process->MyTid());
    // try {
        Process->SetThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);
        mSession->OnClose();
    // } finally {
        Process->SetThreadPriority(pri);
        mListener = NULL;
    // }
    return NOERROR;
}

ECode CSpellCheckerService::InternalISpellCheckerSession::GetLocale(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mLocale;
    return NOERROR;
}

ECode CSpellCheckerService::InternalISpellCheckerSession::GetBundle(
    /* [out] */ IBundle** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mBundle;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

CSpellCheckerService::SpellCheckerServiceBinder::SpellCheckerServiceBinder(
    /* [in] */ ISpellCheckerService* service)
{
    // mInternalServiceRef = new WeakReference<SpellCheckerService>(service);
}

ECode CSpellCheckerService::SpellCheckerServiceBinder::GetISpellCheckerSession(
    /* [in] */ const String& locale,
    /* [in] */ IISpellCheckerSessionListener* listener,
    /* [in] */ IBundle* bundle,
    /* [out] */ IISpellCheckerSession** result)
{
    VALIDATE_NOT_NULL(result);
    const AutoPtr<ISpellCheckerService> service = mInternalServiceRef->Get();
    if (service == NULL) return NULL;
    const Session session = service->CreateSession();
    const InternalISpellCheckerSession internalSession = new InternalISpellCheckerSession(locale, listener, bundle, session);
    session->OnCreate();
    *result = internalSession;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

CSpellCheckerService::SentenceLevelAdapter::SentenceLevelAdapter(
    /* [in] */ ILocale* locale)
{
    // mWordIterator = new WordIterator(locale);
    CWordIterator::New(locale,(IWordIterator**)&mWordIterator);
}

ECode CSpellCheckerService::SentenceLevelAdapter::SentenceWordItem::SentenceWordItem(
    /* [in] */ ITextInfo* ti,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
    : mTextInfo(ti)
    , mStart(start)
    , mLength(end - start)
{}

ECode CSpellCheckerService::SentenceLevelAdapter::SentenceTextInfoParams::SentenceTextInfoParams(
    /* [in] */ ITextInfo* ti,
    /* [in] */ ArrayList<SentenceWordItem> items)
    : mOriginalTextInfo(ti)
    , mItems(items)
{
    mSize = items->Size();
}

ISentenceTextInfoParams CSpellCheckerService::SentenceLevelAdapter::GetSplitWords(
    /* [in] */ ITextInfo* originalTextInfo)
{
    const WordIterator wordIterator = mWordIterator;
    const CharSequence originalText = originalTextInfo->GetText();
    const Int32 cookie = originalTextInfo->GetCookie();
    const Int32 start = 0;
    const Int32 end = originalText->GetLength();
    const ArrayList<SentenceWordItem> wordItems = new ArrayList<SentenceWordItem>();
    wordIterator->SetCharSequence(originalText, 0, originalText->GetLength());
    Int32 wordEnd = wordIterator->Following(start);
    Int32 wordStart = wordIterator->GetBeginning(wordEnd);
    if (DBG) {
        Logger::D(TAG, "iterator: break: ---- 1st word start = " + wordStart + ", end = "
                + wordEnd + "\n" + originalText);
    }
    while (wordStart <= end && wordEnd != IBreakIterator::DONE
            && wordStart != IBreakIterator::DONE) {
        if (wordEnd >= start && wordEnd > wordStart) {
            const String query = originalText->SubSequence(wordStart, wordEnd)->ToString();
            // const TextInfo ti = new TextInfo(query, cookie, query->GetHashCode());
            AutoPtr<ITextInfo> ti;
            CTextInfo::New(query, cookie, query->GetHashCode(), (ITextInfo**)&ti);
            AutoPtr<SentenceWordItem> item = new SentenceWordItem(ti, wordStart, wordEnd);
            wordItems->Add(item);
            if (DBG) {
                Logger::D(TAG, "Adapter: word (" + (wordItems.size() - 1) + ") " + query);
            }
        }
        wordEnd = wordIterator->Following(wordEnd);
        if (wordEnd == BreakIterator::DONE) {
            break;
        }
        wordStart = wordIterator->GetBeginning(wordEnd);
    }
    return new SentenceTextInfoParams(originalTextInfo, wordItems);
}

ECode CSpellCheckerService::SentenceLevelAdapter::ReconstructSuggestions(
    /* [in] */ ISentenceTextInfoParams* originalTextInfoParams,
    /* [in] */ ArrayOf<ISuggestionsInfo> results,
    /* [out] */ ISentenceSuggestionsInfo* result)
{
    if (results == NULL || results->GetLength() == 0) {
        *result = NULL;
        return NOERROR;
    }
    if (DBG) {
        Logger::W(TAG, "Adapter: onGetSuggestions: got " + results.length);
    }
    if (originalTextInfoParams == NULL) {
        if (DBG) {
            Logger::W(TAG, "Adapter: originalTextInfoParams is null.");
        }
        *result = NULL;
        return NOERROR;
    }
    const Int32 originalCookie = originalTextInfoParams::mOriginalTextInfo->GetCookie();
    const Int32 originalSequence =
            originalTextInfoParams::mOriginalTextInfo->GetSequence();

    const Int32 querySize = originalTextInfoParams::mSize;
    const AutoPtr< ArrayOf<Int32> > offsets = ArrayOf<Int32>::Alloc(querySize);
    const AutoPtr< ArrayOf<Int32> > lengths = ArrayOf<Int32>::Alloc(querySize);
    const AutoPtr< ArrayOf<ISuggestionsInfo> > reconstructedSuggestions = ArrayOf<SuggestionsInfo>::Alloc(querySize);
    for (Int32 i = 0; i < querySize; ++i) {
        const AutoPtr<SentenceWordItem> item = originalTextInfoParams::mItems->Get(i);
        SuggestionsInfo result = NULL;

        for (Int32 j = 0; j < results.length; ++j) {
            const AutoPtr<ISuggestionsInfo> cur = results[j];
            if (cur != NULL && cur->GetSequence() == item::mTextInfo->GetSequence()) {
                result = cur;
                result->SetCookieAndSequence(originalCookie, originalSequence);
                break;
            }
        }
        offsets[i] = item::mStart;
        lengths[i] = item::mLength;
        reconstructedSuggestions[i] = result != NULL ? result : EMPTY_SUGGESTIONS_INFO;
        if (DBG) {
            const Int32 size = reconstructedSuggestions[i]->GetSuggestionsCount();
            Logger::W(TAG, "reconstructedSuggestions(" + i + ")" + size + ", first = "
                    + (size > 0 ? reconstructedSuggestions[i].getSuggestionAt(0)
                            : "<none>") + ", offset = " + offsets[i] + ", length = "
                    + lengths[i]);
        }
    }
    // *result = new SentenceSuggestionsInfo(reconstructedSuggestions, offsets, lengths);
    CSentenceSuggestionsInfo::New(reconstructedSuggestions, offsets, lengths, (ISentenceSuggestionsInfo**)&result);
    return NOERROR;
}

} // namespace service
} // namepsace Droid
} // namespace Elastos
