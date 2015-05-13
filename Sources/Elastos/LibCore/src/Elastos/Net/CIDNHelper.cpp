
#include "cmdef.h"
#include "CIDNHelper.h"
#include "IDN.h"

namespace Elastos {
namespace Net {

ECode CIDNHelper::ToASCIIEx(
    /* [in] */ const String& input,
    /* [in] */ Int32 flags,
    /* [out] */ String* result)
{
    return IDN::ToASCII(input, flags, result);
}

/**
 * Equivalent to {@code toASCII(input, 0)}.
 *
 * @param input the Unicode name
 * @return the ACE name
 * @throws IllegalArgumentException if {@code input} does not conform to RFC 3490
 */
ECode CIDNHelper::ToASCII(
    /* [in] */ const String& input,
    /* [out] */ String* result)
{
    return ToASCIIEx(input, 0, result);
}

 /**
 * Translates a string from ASCII Compatible Encoding (ACE) to Unicode
 * according to the algorithm defined in RFC 3490.
 *
 * <p>Unlike {@code toASCII}, this transformation cannot fail.
 *
 * <p>This method can handle either an individual label or an entire domain name.
 * In the latter case, the separators are: U+002E (full stop), U+3002 (ideographic full stop),
 * U+FF0E (fullwidth full stop), and U+FF61 (halfwidth ideographic full stop).
 *
 * @param input the ACE name
 * @return the Unicode name
 * @param flags 0, {@code ALLOW_UNASSIGNED}, {@code USE_STD3_ASCII_RULES},
 *         or {@code ALLOW_UNASSIGNED | USE_STD3_ASCII_RULES}
 */
ECode CIDNHelper::ToUnicodeEx(
    /* [in] */ const String& input,
    /* [in] */ Int32 flags,
    /* [out] */ String* result)
{
   return IDN::ToUnicode(input, flags, result);
}

/**
 * Equivalent to {@code toUnicode(input, 0)}.
 *
 * @param input the ACE name
 * @return the Unicode name
 */
ECode CIDNHelper::ToUnicode(
    /* [in] */ const String& input,
    /* [out] */ String* result)
{
    return ToUnicodeEx(input, 0, result);
}

} // namespace Net
} // namespace Elastos
