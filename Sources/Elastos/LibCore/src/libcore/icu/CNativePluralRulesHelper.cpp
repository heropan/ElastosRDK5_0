
#include "CNativePluralRulesHelper.h"
#include "CNativePluralRules.h"

namespace Libcore {
namespace ICU {

ECode CNativePluralRulesHelper::ForLocale(
    /* [in] */ ILocale * locale,
    /* [out] */ INativePluralRules ** outrules)
{
    VALIDATE_NOT_NULL(outrules)

    AutoPtr<INativePluralRules> ans = CNativePluralRules::ForLocale(locale);
    *outrules = ans;
    REFCOUNT_ADD(*outrules);
    return NOERROR;
}

} // namespace ICU
} // namespace Libcore
