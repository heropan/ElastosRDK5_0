
#ifndef __ICU_CLOCALEDATAHELPER_H__
#define __ICU_CLOCALEDATAHELPER_H__

#include "_CLocaleDataHelper.h"

namespace Libcore {
namespace ICU {

CarClass(CLocaleDataHelper)
{
public:
    /**
     * Returns a shared LocaleData for the given locale.
     */
    CARAPI Get(
        /* [in] */ ILocale* locale,
        /* [out] */ ILocaleData** localeData);
};

} // namespace ICU
} // namespace Libcore

#endif // __ICU_CLOCALEDATAHELPER_H__
