
#include "elastos/droid/webkit/native/net/NetStringUtil.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//=====================================================================
//                            NetStringUtil
//=====================================================================
String NetStringUtil::ConvertToUnicode(
    /* [in] */ IByteBuffer* text,
    /* [in] */ const String& charset_name)
{
    // ==================before translated======================
    // try {
    //     Charset charset = Charset.forName(charset_name);
    //     CharsetDecoder decoder = charset.newDecoder();
    //     // On invalid characters, this will throw an exception.
    //     return decoder.decode(text).toString();
    // } catch (Exception e) {
    //     return null;
    // }
    assert(0);
    return String("");
}

String NetStringUtil::ConvertToUnicodeAndNormalize(
    /* [in] */ IByteBuffer* text,
    /* [in] */ const String& charset_name)
{
    // ==================before translated======================
    // String unicodeString = convertToUnicode(text, charset_name);
    // if (unicodeString == null)
    //     return unicodeString;
    // return Normalizer.normalize(unicodeString, Normalizer.Form.NFC);
    assert(0);
    return String("");
}

String NetStringUtil::ConvertToUnicodeWithSubstitutions(
    /* [in] */ IByteBuffer* text,
    /* [in] */ const String& charset_name)
{
    // ==================before translated======================
    // try {
    //     Charset charset = Charset.forName(charset_name);
    //
    //     // TODO(mmenke):  Investigate if Charset.decode() can be used
    //     // instead.  The question is whether it uses the proper replace
    //     // character.  JDK CharsetDecoder docs say U+FFFD is the default,
    //     // but Charset.decode() docs say it uses the "charset's default
    //     // replacement byte array".
    //     CharsetDecoder decoder = charset.newDecoder();
    //     decoder.onMalformedInput(CodingErrorAction.REPLACE);
    //     decoder.onUnmappableCharacter(CodingErrorAction.REPLACE);
    //     decoder.replaceWith("\uFFFD");
    //     return decoder.decode(text).toString();
    // } catch (Exception e) {
    //     return null;
    // }
    assert(0);
    return String("");
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


