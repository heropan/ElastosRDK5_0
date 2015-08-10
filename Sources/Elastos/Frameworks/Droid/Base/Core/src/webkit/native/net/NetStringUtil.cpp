
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

String NetStringUtil::ConvertToUnicode(
	/* [in] */ ArrayOf<Byte>* text,
	/* [in] */ String charset_name)
{
	//try {
		AutoPtr<ICharset> charset;
		AutoPtr<ICharsetHelper> helper;
		CCharsetHelper::AcquireSingleton((ICharsetHelper**)&helper);
		helper->ForName(charset_name, (ICharset**)&charset);

		// question: interface ICharsetDecoder was deprecated in car file and
		// which can be used instead.
		AutoPtr<ICharsetDecoder> decoder;
		charset->NewDecoder(&decoder);
	    String result;
	    decoder->Decode(text, &result);
	    // On invalid characters, this will throw an exception.
	    return result;
	//} catch (Exception e) {
	//    return null;
	//}
}

String NetStringUtil::ConvertToUnicodeAndNormalize(
	/* [in] */ ArrayOf<Byte>* text,
	/* [in] */ String charset_name)
{
	String unicodeString = ConvertToUnicode(text, charset_name);
    if (unicodeString == String(""))
        return unicodeString;

	AutoPtr<INormalizer> normalizer;
	CNormalizer::New((INormalizer**)&normalizer);

	String result;
	// question: how do convert String to ICharSequence, Normalize function
	// first param need a variable of ICharSequence type.
	normalizer->Normalize(unicodeString, INormalizer::Form::NFC, &result);
    return result;
}

String NetStringUtil::ConvertToUnicodeWithSubstitutions(
	/* [in] */ ArrayOf<Byte>* text,
	/* [in] */ String charset_name)
{
	//try {
		AutoPtr<ICharset> charset;
		AutoPtr<ICharsetHelper> helper;
		CCharsetHelper::AcquireSingleton((ICharsetHelper**)&helper);
		helper->ForName(charset_name, (ICharset**)&charset);

        // TODO(mmenke):  Investigate if Charset.decode() can be used
        // instead.  The question is whether it uses the proper replace
        // character.  JDK CharsetDecoder docs say U+FFFD is the default,
        // but Charset.decode() docs say it uses the "charset's default
        // replacement byte array".
		AutoPtr<ICharsetDecoder> decoder;
		charset->NewDecoder(&decoder);
        decoder->OnMalformedInput(ICodingErrorAction::REPLACE);
        decoder->OnUnmappableCharacter(ICodingErrorAction::REPLACE);
        decoder->ReplaceWith(String("\uFFFD"));
        String result;
	    decoder->Decode(text, &result);
        return result;
    //} catch (Exception e) {
    //    return null;
    //}
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
