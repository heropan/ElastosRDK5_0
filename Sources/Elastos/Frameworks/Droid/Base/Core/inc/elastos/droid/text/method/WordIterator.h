#ifndef __ELASTOS_DROID_TEXT_METHOD_WordIterator_H__
#define __ELASTOS_DROID_TEXT_METHOD_WordIterator_H__

#include "Elastos.Droid.Core_server.h"

using Libcore::ICU::ILocale;
using Elastos::Core::ICharSequence;
//using Elastos::Text::IBreakIterator;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

/**
 * Walks through cursor positions at word boundaries. Internally uses
 * {@link BreakIterator#getWordInstance()}, and caches {@link CharSequence}
 * for performance reasons.
 *
 * Also provides methods to determine word boundaries.
 * {@hide}
 */
//public
class WordIterator
{
public:
    /**
     * Constructs a WordIterator using the default locale.
     */
    WordIterator();

    /**
     * Constructs a new WordIterator for the specified locale.
     * @param locale The locale to be used when analysing the text.
     */
    WordIterator(
        /* [in] */ ILocale* locale);

    CARAPI_(void) Init(
        /* [in] */ ILocale* locale);

    CARAPI/*CARAPI_(void)*/ SetCharSequence(
        /* [in] */ ICharSequence* charSequence,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /** {@inheritDoc} */
    CARAPI_(Int32) Preceding(
        /* [in] */ Int32 offset);

    /** {@inheritDoc} */
    CARAPI_(Int32) Following(
        /* [in] */ Int32 offset);

    /** If <code>offset</code> is within a word, returns the index of the first character of that
     * word, otherwise returns BreakIterator.DONE.
     *
     * The offsets that are considered to be part of a word are the indexes of its characters,
     * <i>as well as</i> the index of its last character plus one.
     * If offset is the index of a low surrogate character, BreakIterator.DONE will be returned.
     *
     * Valid range for offset is [0..textLength] (note the inclusive upper bound).
     * The returned value is within [0..offset] or BreakIterator.DONE.
     *
     * @throws IllegalArgumentException is offset is not valid.
     */
    CARAPI_(Int32) GetBeginning(
        /* [in] */ Int32 offset);

    /** If <code>offset</code> is within a word, returns the index of the last character of that
     * word plus one, otherwise returns BreakIterator.DONE.
     *
     * The offsets that are considered to be part of a word are the indexes of its characters,
     * <i>as well as</i> the index of its last character plus one.
     * If offset is the index of a low surrogate character, BreakIterator.DONE will be returned.
     *
     * Valid range for offset is [0..textLength] (note the inclusive upper bound).
     * The returned value is within [offset..textLength] or BreakIterator.DONE.
     *
     * @throws IllegalArgumentException is offset is not valid.
     */
    CARAPI_(Int32) GetEnd(
        /* [in] */ Int32 offset);

private:
    CARAPI_(Boolean) IsAfterLetterOrDigit(
        /* [in] */ Int32 shiftedOffset);

    CARAPI_(Boolean) IsOnLetterOrDigit(
        /* [in] */ Int32 shiftedOffset);

    CARAPI_(void) CheckOffsetIsValid(
        /* [in] */ Int32 shiftedOffset);

private:
    // Size of the window for the word iterator, should be greater than the longest word's length
    static const Int32 WINDOW_WIDTH;// = 50;

    String mString;
    Int32 mOffsetShift;

    AutoPtr</*IBreakIterator*/IInterface> mIterator;

};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_WordIterator_H__
