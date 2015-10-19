#ifndef __CHARSEQUENCES_H__
#define __CHARSEQUENCES_H__

#include "ext/frameworkext.h"

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Utility {

/**
 * {@link CharSequence} utility methods.
 */
class CharSequences
{
public:
    /**
     * Adapts {@link CharSequence} to an array of ASCII (7-bits per character)
     * bytes.
     *
     * @param bytes ASCII bytes
     */
    static AutoPtr<ICharSequence> ForAsciiBytes(
         /* [in] */ ArrayOf<Byte>* bytes);

    /**
     * Adapts {@link CharSequence} to an array of ASCII (7-bits per character)
     * bytes.
     *
     * @param bytes ASCII bytes
     * @param start index, inclusive
     * @param end index, exclusive
     *
     * @throws IndexOutOfBoundsException if start or end are negative, if end
     *  is greater than length(), or if start is greater than end
     */
    static AutoPtr<ICharSequence> ForAsciiBytes(
        /* [in] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI Validate(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 length);

    /**
     * Compares two character sequences for equality.
     */
    static Boolean Equals(
         /* [in] */ ICharSequence* a,
         /* [in] */ ICharSequence* b);

        /**
     * Compares two character sequences with API like {@link Comparable#compareTo}.
     *
     * @param me The CharSequence that receives the compareTo call.
     * @param another The other CharSequence.
     * @return See {@link Comparable#compareTo}.
     */
    static Int32 CompareToIgnoreCase(
         /* [in] */ ICharSequence* a,
         /* [in] */ ICharSequence* b);
};

} // namespace Utility
} // namespace Droid
} // namespace Elastos

#endif //__CHARSEQUENCES_H__
