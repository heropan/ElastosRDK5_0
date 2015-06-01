
#ifndef __CNUMBERFORMATHELPER_H__
#define __CNUMBERFORMATHELPER_H__

#include "_CNumberFormatHelper.h"

using Libcore::ICU::ILocale;

namespace Elastos {
namespace Text {

CarClass(CNumberFormatHelper)
{
public:

    CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetCurrencyInstance(
        /* [out] */ INumberFormat** instance);

    CARAPI GetCurrencyInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    CARAPI GetIntegerInstance(
        /* [out] */ INumberFormat** instance);

    CARAPI GetIntegerInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    CARAPI GetInstance(
        /* [out] */ INumberFormat** instance);

    CARAPI GetInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    CARAPI GetNumberInstance(
        /* [out] */ INumberFormat** instance);

    CARAPI GetNumberInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    CARAPI GetPercentInstance(
        /* [out] */ INumberFormat** instance);

    CARAPI GetPercentInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

};

} // namespace Text
} // namespace Elastos

#endif // __CNUMBERFORMATHELPER_H__
