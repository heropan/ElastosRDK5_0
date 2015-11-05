
#include "elastos/droid/net/http/CharArrayBuffers.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

const Char32 CharArrayBuffers::uppercaseAddon = 'a' - 'A';

ECode CharArrayBuffers::ContainsIgnoreCaseTrimmed(
    /* [in] */ ICharArrayBuffer* buffer,
    /* [in] */ Int32 beginIndex,
    /* [in] */ const String& str,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        int len = buffer.length();
        char[] chars = buffer.buffer();
        while (beginIndex < len && HTTP.isWhitespace(chars[beginIndex])) {
            beginIndex++;
        }
        int size = str.length();
        boolean ok = len >= beginIndex + size;
        for (int j=0; ok && (j<size); j++) {
            char a = chars[beginIndex+j];
            char b = str.charAt(j);
            if (a != b) {
                a = toLower(a);
                b = toLower(b);
                ok = a == b;
            }
        }
        return ok;

#endif
}

ECode CharArrayBuffers::SetLowercaseIndexOf(
    /* [in] */ ICharArrayBuffer* buffer,
    /* [in] */ Int32 ch,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        int beginIndex = 0;
        int endIndex = buffer.length();
        char[] chars = buffer.buffer();
        for (int i = beginIndex; i < endIndex; i++) {
            char current = chars[i];
            if (current == ch) {
                return i;
            } else if (current >= 'A' && current <= 'Z'){
                // make lower case
                current += uppercaseAddon;
                chars[i] = current;
            }
        }
        return -1;

#endif
}

ECode CharArrayBuffers::ToLower(
    /* [in] */ Char32 c,
    /* [out] */ Char32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (c >= 'A' && c <= 'Z'){
            c += uppercaseAddon;
        }
        return c;

#endif
}


} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
