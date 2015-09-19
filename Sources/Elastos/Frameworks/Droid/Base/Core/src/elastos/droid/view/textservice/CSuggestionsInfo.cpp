

#ifndef __CSUGGESTIONSINFO_H__
#define __CSUGGESTIONSINFO_H__

namespace Elastos {
namespace Droid {
namespace View {
namespace Textservice {

AutoPtr<ArrayOf<String> > CSuggestionsInfo::EMPTY;// = ArrayUtils.emptyArray(String.class);

/**
 * Constructor.
 * @param suggestionsAttributes from the text service
 * @param suggestions from the text service
 */
ECode CSuggestionsInfo::constructor(
    /* [in] */ Int32 suggestionsAttributes,
    /* [in] */ ArrayOf<String>* suggestions)
{
    return constructor(suggestionsAttributes, suggestions, 0, 0);
}

/**
 * Constructor.
 * @param suggestionsAttributes from the text service
 * @param suggestions from the text service
 * @param cookie the cookie of the input TextInfo
 * @param sequence the cookie of the input TextInfo
 */
ECode CSuggestionsInfo::constructor(
    /* [in] */ Int32 suggestionsAttributes,
    /* [in] */ ArrayOf<String>* suggestions,
    /* [in] */ Int32 cookie,
    /* [in] */ Int32 sequence)
{
    if (suggestions == NULL) {
        mSuggestions = EMPTY;
        mSuggestionsAvailable = FALSE;
    } else {
        mSuggestions = suggestions;
        mSuggestionsAvailable = TRUE;
    }
    mSuggestionsAttributes = suggestionsAttributes;
    mCookie = cookie;
    mSequence = sequence;

    return NOERROR;
}

ECode CSuggestionsInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt32(&mSuggestionsAttributes);
//    source->ReadStringArray(&mSuggestions);
    source->ReadInt32(&mCookie);
    source->ReadInt32(&mSequence);
    Int32 temp;
    source->ReadInt32(&temp);
    mSuggestionsAvailable = temp == 1;

    return NOERROR;
}

/**
 * Used to package this object into a {@link Parcel}.
 *
 * @param dest The {@link Parcel} to be written.
 * @param flags The flags used for parceling.
 */
//@Override
ECode CSuggestionsInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mSuggestionsAttributes);
//    dest->WriteStringArray(mSuggestions);
    dest->WriteInt32(mCookie);
    dest->WriteInt32(mSequence);
    dest->WriteInt32(mSuggestionsAvailable ? 1 : 0);

    return NOERROR;
}

/**
 * Set the cookie and the sequence of SuggestionsInfo which are set to TextInfo from a client
 * application
 * @param cookie the cookie of an input TextInfo
 * @param sequence the cookie of an input TextInfo
 */
ECode CSuggestionsInfo::SetCookieAndSequence(
    /* [in] */ Int32 cookie,
    /* [in] */ Int32 sequence)
{
    mCookie = cookie;
    mSequence = sequence;
}

/**
 * @return the cookie which may be set by a client application
 */
ECode CSuggestionsInfo::GetCookie(
    /* [out] */ Int32* cookie)
{
    VALIDATE_NOT_NULL(cookie);
    *cookie = mCookie;
    return NOERROR;
}

/**
 * @return the sequence which may be set by a client application
 */
ECode CSuggestionsInfo::GetSequence(
    /* [out] */ Int32* sequence)
{
    VALIDATE_NOT_NULL(sequence);
    *sequence = mSequence;
    return NOERROR;
}

/**
 * @return the attributes of suggestions. This includes whether the spell checker has the word
 * in its dictionary or not and whether the spell checker has confident suggestions for the
 * word or not.
 */
ECode CSuggestionsInfo::GetSuggestionsAttributes(
    /* [out] */ Int32* attr)
{
    VALIDATE_NOT_NULL(attr);
    *attr = mSuggestionsAttributes;
    return NOERROR;
}

/**
 * @return the count of the suggestions. If there's no suggestions at all, this method returns
 * -1. Even if this method returns 0, it doesn't necessarily mean that there are no suggestions
 * for the requested word. For instance, the caller could have been asked to limit the maximum
 * number of suggestions returned.
 */
ECode CSuggestionsInfo::GetSuggestionsCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    if (!mSuggestionsAvailable) {
        *count = -1;
        return NOERROR;
    }

    *count = mSuggestions.GetLength();

    return NOERROR;
}

/**
 * @param i the id of suggestions
 * @return the suggestion at the specified id
 */
ECode CSuggestionsInfo::GetSuggestionAt(
    /* [in] */ Int32 i,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = mSuggestions[i];
    return NOERROR;
}

}   //namespace Textservice
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
