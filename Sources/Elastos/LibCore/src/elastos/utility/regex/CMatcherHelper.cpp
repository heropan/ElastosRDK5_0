
#include "CMatcherHelper.h"
#include "CMatcher.h"

namespace Elastos {
namespace Utility {
namespace Regex {


ECode CMatcherHelper::QuoteReplacement(
    /* [in] */ const String& s,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = CMatcher::QuoteReplacement(s);
    return NOERROR;
}

} // namespace Regex
} // namespace Utility
} // namespace Elastos
