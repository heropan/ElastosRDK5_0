
#ifndef __UTILITY_CCURRENCYHELPER_H__
#define __UTILITY_CCURRENCYHELPER_H__

#include "_CCurrencyHelper.h"

using Libcore::ICU::ILocale;

namespace Elastos{
namespace Utility{

CarClass(CCurrencyHelper)
{
public:
    CARAPI GetInstance(
        /* [in] */ const String& currencyCode,
        /* [out] */ ICurrency** outcur);

    CARAPI GetInstanceEx(
        /* [in] */ ILocale* locale,
        /* [out] */ ICurrency** outcur);

    CARAPI GetAvailableCurrencies(
        /* [out] */ ISet** currencies);
};

} // namespace Utility
} // namespace Elastos

#endif //__UTILITY_CCURRENCYHELPER_H__
