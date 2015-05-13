
#ifndef __CRFC822TOKENIZER_H__
#define __CRFC822TOKENIZER_H__

#include "_CRfc822Tokenizer.h"
#include <elastos/StringBuilder.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Utility {

CarClass(CRfc822Tokenizer)
{
public:
    /**
     * This constructor will try to take a string like
     * "Foo Bar (something) &lt;foo\@google.com&gt;,
     * blah\@google.com (something)"
     * and convert it into one or more Rfc822Tokens, output into the supplied
     * collection.
     *
     * It does *not* decode MIME encoded-words; charset conversion
     * must already have taken place if necessary.
     * It will try to be tolerant of broken syntax instead of
     * returning an error.
     *
     */
    static CARAPI Tokenize(
        /* [in] */ ICharSequence* text,
        /* [in, out] */ IObjectContainer* out);

    /**
     * This method will try to take a string like
     * "Foo Bar (something) &lt;foo\@google.com&gt;,
     * blah\@google.com (something)"
     * and convert it into one or more Rfc822Tokens.
     * It does *not* decode MIME encoded-words; charset conversion
     * must already have taken place if necessary.
     * It will try to be tolerant of broken syntax instead of
     * returning an error.
     */
    static CARAPI TokenizeEx(
        /* [in] */ ICharSequence* text,
        /* [out] */ ArrayOf<IRfc822Token*>** ret);

    CARAPI FindTokenStart(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 cursor,
        /* [out] */ Int32* ret);

    CARAPI FindTokenEnd(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 cursor,
        /* [out] */ Int32* ret);

    /**
     * Terminates the specified address with a comma and space.
     * This assumes that the specified text already has valid syntax.
     * The Adapter subclass's convertToString() method must make that
     * guarantee.
     */
    CARAPI TerminateToken(
        /* [in] */ ICharSequence* text,
        /* [out] */ ICharSequence** ret);

private:
    static CARAPI_(void) Crunch(
        /* [in] */ StringBuilder& sb);

};

} //namespace Utility
} //namespace Text
} //namespace Droid
} //namespace Elastos

#endif // __CRFC822TOKENIZER_H__
