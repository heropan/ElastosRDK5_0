
#include "cmdef.h"
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
    INTERFACE_ADDREF(*localeData);

    return NOERROR;
}

} // namespace ICU
} // namespace Libcore
