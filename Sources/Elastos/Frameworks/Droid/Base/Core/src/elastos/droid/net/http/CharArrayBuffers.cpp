
#include <Elastos.CoreLibrary.Apache.h>
#include "elastos/droid/net/http/CharArrayBuffers.h"
#include "elastos/droid/net/ReturnOutValue.h"

using Org::Apache::Http::Protocol::IHTTP;

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
    VALIDATE_NOT_NULL(result)

    Int32 len;
    buffer->GetLength(&len);
    AutoPtr<ArrayOf<Char32> > chars;
    buffer->GetBuffer((ArrayOf<Char32>**)&chars);
    AutoPtr<IHTTP> helper;
    // TODO: Waiting for CHTTP
    assert(0);
    // CHTTP::AcquireSingleton((IHTTP**)&helper);
    while (beginIndex < len) {
        Boolean isWhitespace;
        helper->IsWhitespace((*chars)[beginIndex], &isWhitespace);
        if (!isWhitespace)
            break;
        beginIndex++;
    }
    Int32 size = str.GetLength();
    Boolean ok = len >= beginIndex + size;
    for (Int32 j=0; ok && (j<size); j++) {
        Char32 a = (*chars)[beginIndex+j];
        Char32 b = str.GetChar(j);
        if (a != b) {
            a = ToLower(a);
            b = ToLower(b);
            ok = a == b;
        }
    }
    FUNC_RETURN(ok)
}

ECode CharArrayBuffers::SetLowercaseIndexOf(
    /* [in] */ ICharArrayBuffer* buffer,
    /* [in] */ Int32 ch,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    Int32 beginIndex = 0;
    Int32 endIndex;
    buffer->GetLength(&endIndex);
    AutoPtr<ArrayOf<Char32> > chars;
    buffer->GetBuffer((ArrayOf<Char32>**)&chars);
    for (Int32 i = beginIndex; i < endIndex; i++) {
        Char32 current = (*chars)[i];
        if (current == ch) {
            FUNC_RETURN(i);
        } else if (current >= 'A' && current <= 'Z'){
            // make lower case
            current += uppercaseAddon;
            (*chars)[i] = current;
        }
    }
    FUNC_RETURN(-1);
}

Char32 CharArrayBuffers::ToLower(
    /* [in] */ Char32 c)
{
    if (c >= 'A' && c <= 'Z'){
        c += uppercaseAddon;
    }
    return c;
}

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
