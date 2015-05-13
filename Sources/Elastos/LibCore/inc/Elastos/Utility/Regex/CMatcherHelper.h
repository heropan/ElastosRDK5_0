
#ifndef __REGEX_CMATCHERHELPER_H__
#define __REGEX_CMATCHERHELPER_H__

#include "_CMatcherHelper.h"

namespace Elastos {
namespace Utility {
namespace Regex {

/**
 * The result of applying a {@code Pattern} to a given input. See {@link Pattern} for
 * example uses.
 */
CarClass(CMatcherHelper)
{
public:
    /**
     * Returns a replacement string for the given one that has all backslashes
     * and dollar signs escaped.
     *
     * @param s
     *            the input string.
     * @return the input string, with all backslashes and dollar signs having
     *         been escaped.
     */
    CARAPI QuoteReplacement(
        /* [in] */ const String& s,
        /* [out] */ String* input);
};

} // namespace Regex
} // namespace Utility
} // namespace Elastos

#endif //__REGEX_CMATCHERHELPER_H__
