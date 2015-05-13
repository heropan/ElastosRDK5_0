
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

    CARAPI GetCurrencyInstanceEx(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    CARAPI GetIntegerInstance(
        /* [out] */ INumberFormat** instance);

    CARAPI GetIntegerInstanceEx(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    CARAPI GetInstance(
        /* [out] */ INumberFormat** instance);

    CARAPI GetInstanceEx(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    CARAPI GetNumberInstance(
        /* [out] */ INumberFormat** instance);

    CARAPI GetNumberInstanceEx(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    CARAPI GetPercentInstance(
        /* [out] */ INumberFormat** instance);

    CARAPI GetPercentInstanceEx(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

};

} // namespace Text
} // namespace Elastos

#endif // __CNUMBERFORMATHELPER_H__
