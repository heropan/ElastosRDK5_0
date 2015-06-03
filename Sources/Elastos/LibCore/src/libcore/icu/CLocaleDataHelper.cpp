
#include "CLocaleDataHelper.h"
#include "LocaleData.h"

namespace Libcore {
namespace ICU {

ECode CLocaleDataHelper::Get(
    /* [in] */ ILocale* locale,
    /* [out] */ ILocaleData** localeData)
{
    VALIDATE_NOT_NULL(localeData);

    AutoPtr<ILocaleData> temp = LocaleData::Get(locale);
    *localeData = temp;
    REFCOUNT_ADD(*localeData);

    return NOERROR;
}

} // namespace ICU
} // namespace Libcore
