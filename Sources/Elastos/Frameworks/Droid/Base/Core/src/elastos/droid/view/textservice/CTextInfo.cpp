
namespace Elastos {
namespace Droid {
namespace View {
namespace Textservice {

/**
 * Constructor.
 * @param text the text which will be input to TextService
 */
ECode CTextInfo::constructor(
    /* [in] */ const String& text)
{
    return constructor(text, 0, 0);
}

/**
 * Constructor.
 * @param text the text which will be input to TextService
 * @param cookie the cookie for this TextInfo
 * @param sequence the sequence number for this TextInfo
 */
ECode CTextInfo::constructor(
    /* [in] */ const String& text,
    /* [in] */ Int32 cookie,
    /* [in] */ Int32 sequence)
{
    //if (TextUtils.isEmpty(text)) {
    //    throw new IllegalArgumentException(text);
    //}
    assert(!TextUtils::IsEmpty(text));
    mText = text;
    mCookie = cookie;
    mSequence = sequence;

    return NOERROR;
}

ECode CTextInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadString(&mText);
    source->ReadInt32(&mCookie);
    source->ReadInt32(&mSequence);

    return NOERROR;
}

/**
 * Used to package this object into a {@link Parcel}.
 *
 * @param dest The {@link Parcel} to be written.
 * @param flags The flags used for parceling.
 */
//@Override
ECode CTextInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteString(mText);
    dest->WriteInt32(mCookie);
    dest->WriteInt32(mSequence);

    return NOERROR;
}

/**
 * @return the text which is an input of a text service
 */
ECode CTextInfo::GetText(
    /* [out] */ String* text)
{
    VALIDATE_NOT_NULL(text);
    *text = mText;
    return NOERROR;
}

/**
 * @return the cookie of TextInfo
 */
ECode CTextInfo::GetCookie(
    /* [out] */ Int32* cookie)
{
    VALIDATE_NOT_NULL(cookie);
    *cookie = mCookie;
    return NOERROR;
}

/**
 * @return the sequence of TextInfo
 */
ECode CTextInfo::GetSequence(
    /* [out] */ Int32* sequence)
{
    VALIDATE_NOT_NULL(sequence);
    *sequence = mSequence;
    return NOERROR;
}

}   //namespace Textservice
}   //namespace View
}   //namespace Droid
}   //namespace Elastos