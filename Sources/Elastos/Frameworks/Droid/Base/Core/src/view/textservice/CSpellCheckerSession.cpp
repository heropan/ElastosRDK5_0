
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

/**
 * @return true if the connection to a text service of this session is disconnected and not
 * alive.
 */
ECode CSpellCheckerSession::IsSessionDisconnected(
    /* [out]*/ Boolean* bFlag)
{
    VALIDATE_NOT_NULL(bFlag);
    *bFlag = SpellCheckerSession::IsSessionDisconnected();
    return NOERROR;
}

/**
 * Get the spell checker service info this spell checker session has.
 * @return SpellCheckerInfo for the specified locale.
 */
ECode CSpellCheckerSession::GetSpellChecker(
    /* [out] */ ISpellCheckerInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = SpellCheckerSession::GetSpellChecker();
    return NOERROR;
}

/**
 * Cancel pending and running spell check tasks
 */
ECode CSpellCheckerSession::Cancel()
{
    SpellCheckerSession::Cancel();
    return NOERROR;
}

/**
 * Finish this session and allow TextServicesManagerService to disconnect the bound spell
 * checker.
 */
ECode CSpellCheckerSession::Close()
{
    SpellCheckerSession::Close();
    return NOERROR;
}

/**
 * Get suggestions from the specified sentences
 * @param textInfos an array of text metadata for a spell checker
 * @param suggestionsLimit the maximum number of suggestions that will be returned
 */
ECode CSpellCheckerSession::GetSentenceSuggestions(
    /* [in] */ ArrayOf<ITextInfo*>* textInfos,
    /* [in] */ Int32 suggestionsLimit)
{
    SpellCheckerSession::GetSentenceSuggestions(textInfos, suggestionsLimit);
    return NOERROR;
}

/**
 * Get candidate strings for a substring of the specified text.
 * @param textInfo text metadata for a spell checker
 * @param suggestionsLimit the maximum number of suggestions that will be returned
 * @deprecated use {@link SpellCheckerSession#getSentenceSuggestions(TextInfo[], int)} instead
 */
//@Deprecated
ECode CSpellCheckerSession::GetSuggestions(
    /* [in] */ ITextInfo* textInfo,
    /* [in] */ Int32 suggestionsLimit)
{
    SpellCheckerSession::GetSuggestions(textInfo, suggestionsLimit);
    return NOERROR;
}

/**
 * A batch process of getSuggestions
 * @param textInfos an array of text metadata for a spell checker
 * @param suggestionsLimit the maximum number of suggestions that will be returned
 * @param sequentialWords true if textInfos can be treated as sequential words.
 * @deprecated use {@link SpellCheckerSession#getSentenceSuggestions(TextInfo[], int)} instead
 */
//@Deprecated
ECode CSpellCheckerSession::GetSuggestions(
    /* [in] */ ArrayOf<ITextInfo*>* textInfos,
    /* [in] */ Int32 suggestionsLimit,
    /* [in] */ Boolean sequentialWords)
{
    SpellCheckerSession::GetSuggestions(textInfos, suggestionsLimit, sequentialWords);
    return NOERROR;
}

/**
 * @hide
 */
ECode CSpellCheckerSession::GetTextServicesSessionListener(
    /* [out] */ ITextServicesSessionListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    AutoPtr<ITextServicesSessionListener> temp = SpellCheckerSession::GetTextServicesSessionListener();
    *listener = temp;
    INTERFACE_ADDREF(*listener);
    return NOERROR;
}

/**
 * @hide
 */
ECode CSpellCheckerSession::GetSpellCheckerSessionListener(
    /* [out]*/ ISpellCheckerSessionListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    AutoPtr<ISpellCheckerSessionListener> temp = SpellCheckerSession::GetSpellCheckerSessionListener();
    *listener = temp;
    INTERFACE_ADDREF(*listener);
    return NOERROR;
}

}   //namespace Textservice
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
