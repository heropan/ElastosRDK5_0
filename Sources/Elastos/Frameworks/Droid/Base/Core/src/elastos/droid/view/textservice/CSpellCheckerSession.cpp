
namespace Elastos {
namespace Droid {
namespace View {
namespace Textservice {

ECode CSpellCheckerSession::constructor(
    /* [in] */ ISpellCheckerInfo* info,
    /* [in] */ ITextServicesManager* tsm,
    /* [in] */ ISpellCheckerSessionListener* listener,
    /* [in] */ ISpellCheckerSubtype* subtype)
{
    SpellCheckerSession::Init(info, tsm, listener, subtype);
    return NOERROR;
}

ECode CSpellCheckerSession::IsSessionDisconnected(
    /* [out]*/ Boolean* bFlag)
{
    VALIDATE_NOT_NULL(bFlag);
    *bFlag = SpellCheckerSession::IsSessionDisconnected();
    return NOERROR;
}

ECode CSpellCheckerSession::GetSpellChecker(
    /* [out] */ ISpellCheckerInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = SpellCheckerSession::GetSpellChecker();
    return NOERROR;
}

ECode CSpellCheckerSession::Cancel()
{
    SpellCheckerSession::Cancel();
    return NOERROR;
}

ECode CSpellCheckerSession::Close()
{
    SpellCheckerSession::Close();
    return NOERROR;
}

ECode CSpellCheckerSession::GetSentenceSuggestions(
    /* [in] */ ArrayOf<ITextInfo*>* textInfos,
    /* [in] */ Int32 suggestionsLimit)
{
    SpellCheckerSession::GetSentenceSuggestions(textInfos, suggestionsLimit);
    return NOERROR;
}

ECode CSpellCheckerSession::GetSuggestions(
    /* [in] */ ITextInfo* textInfo,
    /* [in] */ Int32 suggestionsLimit)
{
    SpellCheckerSession::GetSuggestions(textInfo, suggestionsLimit);
    return NOERROR;
}

ECode CSpellCheckerSession::GetSuggestions(
    /* [in] */ ArrayOf<ITextInfo*>* textInfos,
    /* [in] */ Int32 suggestionsLimit,
    /* [in] */ Boolean sequentialWords)
{
    SpellCheckerSession::GetSuggestions(textInfos, suggestionsLimit, sequentialWords);
    return NOERROR;
}

ECode CSpellCheckerSession::GetTextServicesSessionListener(
    /* [out] */ ITextServicesSessionListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    AutoPtr<ITextServicesSessionListener> temp = SpellCheckerSession::GetTextServicesSessionListener();
    *listener = temp;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

ECode CSpellCheckerSession::GetSpellCheckerSessionListener(
    /* [out]*/ ISpellCheckerSessionListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    AutoPtr<ISpellCheckerSessionListener> temp = SpellCheckerSession::GetSpellCheckerSessionListener();
    *listener = temp;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

}   //namespace Textservice
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
