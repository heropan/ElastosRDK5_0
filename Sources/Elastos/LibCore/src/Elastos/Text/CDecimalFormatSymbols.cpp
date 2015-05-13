
#include "CDecimalFormatSymbols.h"

namespace Elastos {
namespace Text {

PInterface CDecimalFormatSymbols::Probe(
        /* [in] */ REIID riid)
{
    return _CDecimalFormatSymbols::Probe(riid);
}

ECode CDecimalFormatSymbols::GetCurrency(
    /* [out] */ ICurrency** currency)
{
    return DecimalFormatSymbols::GetCurrency(currency);
}

ECode CDecimalFormatSymbols::GetInternationalCurrencySymbol(
    /* [out] */ String* symbol)
{
    return DecimalFormatSymbols::GetInternationalCurrencySymbol(symbol);
}

ECode CDecimalFormatSymbols::GetCurrencySymbol(
    /* [out] */ String* symbol)
{
    return DecimalFormatSymbols::GetCurrencySymbol(symbol);
}

ECode CDecimalFormatSymbols::GetDecimalSeparator(
    /* [out] */ Char32* separator)
{
    return DecimalFormatSymbols::GetDecimalSeparator(separator);
}

ECode CDecimalFormatSymbols::GetDigit(
    /* [out] */ Char32* digit)
{
    return DecimalFormatSymbols::GetDigit(digit);
}

ECode CDecimalFormatSymbols::GetGroupingSeparator(
    /* [out] */ Char32* separator)
{
    return DecimalFormatSymbols::GetGroupingSeparator(separator);
}

ECode CDecimalFormatSymbols::GetInfinity(
    /* [out] */ String* infinity)
{
    return DecimalFormatSymbols::GetInfinity(infinity);
}

ECode CDecimalFormatSymbols::GetMinusSign(
    /* [out] */ Char32* sign)
{
    return DecimalFormatSymbols::GetMinusSign(sign);
}

ECode CDecimalFormatSymbols::GetMonetaryDecimalSeparator(
    /* [out] */ Char32* separator)
{
    return DecimalFormatSymbols::GetMonetaryDecimalSeparator(separator);
}

ECode CDecimalFormatSymbols::GetNaN(
    /* [out] */ String* NaN)
{
    return DecimalFormatSymbols::GetNaN(NaN);
}

ECode CDecimalFormatSymbols::GetPatternSeparator(
    /* [out] */ Char32* separator)
{
    return DecimalFormatSymbols::GetPatternSeparator(separator);
}

ECode CDecimalFormatSymbols::GetPercent(
    /* [out] */ Char32* percent)
{
    return DecimalFormatSymbols::GetPercent(percent);
}

ECode CDecimalFormatSymbols::GetPerMill(
    /* [out] */ Char32* perMill)
{
    return DecimalFormatSymbols::GetPerMill(perMill);
}

ECode CDecimalFormatSymbols::GetZeroDigit(
    /* [out] */ Char32* zeroDigit)
{
    return DecimalFormatSymbols::GetZeroDigit(zeroDigit);
}

ECode CDecimalFormatSymbols::GetExponentSeparator(
    /* [out] */ String* separator)
{
    return DecimalFormatSymbols::GetExponentSeparator(separator);
}

ECode CDecimalFormatSymbols::SetCurrency(
    /* [in] */ ICurrency* currency)
{
    return DecimalFormatSymbols::SetCurrency(currency);
}

ECode CDecimalFormatSymbols::SetInternationalCurrencySymbol(
    /* [in] */ const String& value)
{
    return DecimalFormatSymbols::SetInternationalCurrencySymbol(value);
}

ECode CDecimalFormatSymbols::SetCurrencySymbol(
    /* [in] */ const String& value)
{
    return DecimalFormatSymbols::SetCurrencySymbol(value);
}

ECode CDecimalFormatSymbols::SetDecimalSeparator(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetDecimalSeparator(value);
}

ECode CDecimalFormatSymbols::SetDigit(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetDigit(value);
}

ECode CDecimalFormatSymbols::SetGroupingSeparator(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetGroupingSeparator(value);
}

ECode CDecimalFormatSymbols::SetInfinity(
    /* [in] */ const String& value)
{
    return DecimalFormatSymbols::SetInfinity(value);
}

ECode CDecimalFormatSymbols::SetMinusSign(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetMinusSign(value);
}

ECode CDecimalFormatSymbols::SetMonetaryDecimalSeparator(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetMonetaryDecimalSeparator(value);
}

ECode CDecimalFormatSymbols::SetNaN(
    /* [in] */ const String& value)
{
    return DecimalFormatSymbols::SetNaN(value);
}

ECode CDecimalFormatSymbols::SetPatternSeparator(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetPatternSeparator(value);
}

ECode CDecimalFormatSymbols::SetPercent(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetPercent(value);
}

ECode CDecimalFormatSymbols::SetPerMill(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetPerMill(value);
}

ECode CDecimalFormatSymbols::SetZeroDigit(
    /* [in] */ Char32 value)
{
    return DecimalFormatSymbols::SetZeroDigit(value);
}

ECode CDecimalFormatSymbols::SetExponentSeparator(
    /* [in] */ const String& value)
{
    return DecimalFormatSymbols::SetExponentSeparator(value);
}

ECode CDecimalFormatSymbols::Clone(
    /* [out] */ IDecimalFormatSymbols** obj)
{
    return DecimalFormatSymbols::Clone(obj);
}

ECode CDecimalFormatSymbols::constructor()
{
    return DecimalFormatSymbols::Init();
}

ECode CDecimalFormatSymbols::constructor(
    /* [in] */ ILocale* locale)
{
    return DecimalFormatSymbols::Init(locale);
}

ECode CDecimalFormatSymbols::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* res)
{
    return DecimalFormatSymbols::Equals(object, res);
}

} // namespace Text
} // namespace Elastos

