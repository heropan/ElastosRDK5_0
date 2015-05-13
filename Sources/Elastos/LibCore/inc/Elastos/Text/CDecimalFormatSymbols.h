
#ifndef __CDECIMALFORMATSYMBOLS_H__
#define __CDECIMALFORMATSYMBOLS_H__

#include "_CDecimalFormatSymbols.h"
#include <cmdef.h>
#include "DecimalFormatSymbols.h"

namespace Elastos {
namespace Text {

CarClass(CDecimalFormatSymbols) , public DecimalFormatSymbols
{
public:
    CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

    CARAPI GetCurrency(
        /* [out] */ ICurrency** currency);

    CARAPI GetInternationalCurrencySymbol(
        /* [out] */ String* symbol);

    CARAPI GetCurrencySymbol(
        /* [out] */ String* symbol);

    CARAPI GetDecimalSeparator(
        /* [out] */ Char32* separator);

    CARAPI GetDigit(
        /* [out] */ Char32* digit);

    CARAPI GetGroupingSeparator(
        /* [out] */ Char32* separator);

    CARAPI GetInfinity(
        /* [out] */ String* infinity);

    CARAPI GetMinusSign(
        /* [out] */ Char32* sign);

    CARAPI GetMonetaryDecimalSeparator(
        /* [out] */ Char32* separator);

    CARAPI GetNaN(
        /* [out] */ String* NaN);

    CARAPI GetPatternSeparator(
        /* [out] */ Char32* separator);

    CARAPI GetPercent(
        /* [out] */ Char32* percent);

    CARAPI GetPerMill(
        /* [out] */ Char32* perMill);

    CARAPI GetZeroDigit(
        /* [out] */ Char32* zeroDigit);

    CARAPI GetExponentSeparator(
        /* [out] */ String* separator);

    CARAPI SetCurrency(
        /* [in] */ ICurrency* currency);

    CARAPI SetInternationalCurrencySymbol(
        /* [in] */ const String& value);

    CARAPI SetCurrencySymbol(
        /* [in] */ const String& value);

    CARAPI SetDecimalSeparator(
        /* [in] */ Char32 value);

    CARAPI SetDigit(
        /* [in] */ Char32 value);

    CARAPI SetGroupingSeparator(
        /* [in] */ Char32 value);

    CARAPI SetInfinity(
        /* [in] */ const String& value);

    CARAPI SetMinusSign(
        /* [in] */ Char32 value);

    CARAPI SetMonetaryDecimalSeparator(
        /* [in] */ Char32 value);

    CARAPI SetNaN(
        /* [in] */ const String& value);

    CARAPI SetPatternSeparator(
        /* [in] */ Char32 value);

    CARAPI SetPercent(
        /* [in] */ Char32 value);

    CARAPI SetPerMill(
        /* [in] */ Char32 value);

    CARAPI SetZeroDigit(
        /* [in] */ Char32 value);

    CARAPI SetExponentSeparator(
        /* [in] */ const String& value);

    CARAPI Clone(
        /* [out] */ IDecimalFormatSymbols** obj);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ILocale* locale);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* res);
};

} // namespace Text
} // namespace Elastos

#endif // __CDECIMALFORMATSYMBOLS_H__
