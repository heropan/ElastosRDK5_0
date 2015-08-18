
#ifndef __ELASTOS_DROID_NET_CUrlQuerySanitizer_H__
#define __ELASTOS_DROID_NET_CUrlQuerySanitizer_H__

#include "_Elastos_Droid_Net_CUrlQuerySanitizer.h"

using Elastos::Droid::Net::IValueSanitizer;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CUrlQuerySanitizerHelper)
{
public:
    /**
     * Return a value sanitizer that does not allow any special characters,
     * and also does not allow script URLs.
     * @return a value sanitizer
     */
    CARAPI GetAllIllegal(
        /* [out] */ IValueSanitizer** result);

    /**
     * Return a value sanitizer that allows everything except Nul ('\0')
     * characters. Script URLs are allowed.
     * @return a value sanitizer
     */
    CARAPI GetAllButNulLegal(
        /* [out] */ IValueSanitizer** result);

    /**
     * Return a value sanitizer that allows everything except Nul ('\0')
     * characters, space (' '), and other whitespace characters.
     * Script URLs are allowed.
     * @return a value sanitizer
     */
    CARAPI GetAllButWhitespaceLegal(
        /* [out] */ IValueSanitizer** result);

    /**
     * Return a value sanitizer that allows all the characters used by
     * encoded URLs. Does not allow script URLs.
     * @return a value sanitizer
     */
    CARAPI GetUrlLegal(
        /* [out] */ IValueSanitizer** result);

    /**
     * Return a value sanitizer that allows all the characters used by
     * encoded URLs and allows spaces, which are not technically legal
     * in encoded URLs, but commonly appear anyway.
     * Does not allow script URLs.
     * @return a value sanitizer
     */
    CARAPI GetUrlAndSpaceLegal(
        /* [out] */ IValueSanitizer** result);

    /**
     * Return a value sanitizer that does not allow any special characters
     * except ampersand ('&'). Does not allow script URLs.
     * @return a value sanitizer
     */
    CARAPI GetAmpLegal(
        /* [out] */ IValueSanitizer** result);

    /**
     * Return a value sanitizer that does not allow any special characters
     * except ampersand ('&') and space (' '). Does not allow script URLs.
     * @return a value sanitizer
     */
    CARAPI GetAmpAndSpaceLegal(
        /* [out] */ IValueSanitizer** result);

    /**
     * Return a value sanitizer that does not allow any special characters
     * except space (' '). Does not allow script URLs.
     * @return a value sanitizer
     */
    CARAPI GetSpaceLegal(
        /* [out] */ IValueSanitizer** result);

    /**
     * Return a value sanitizer that allows any special characters
     * except angle brackets ('<' and '>') and Nul ('\0').
     * Allows script URLs.
     * @return a value sanitizer
     */
    CARAPI GetAllButNulAndAngleBracketsLegal(
        /* [out] */ IValueSanitizer** result);

};




} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CUrlQuerySanitizer_H__
