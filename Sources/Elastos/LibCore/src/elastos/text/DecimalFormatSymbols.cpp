
#include "DecimalFormatSymbols.h"
#include "CDecimalFormatSymbols.h"
#include "CLocaleHelper.h"
#include "StringBuilder.h"
#include "CLocaleDataHelper.h"
#include "ICUUtil.h"
#include "Currency.h"

using Elastos::Core::EIID_ICloneable;
using Elastos::Core::StringBuilder;
using Libcore::ICU::ICUUtil;
using Elastos::Utility::ILocaleHelper;
using Elastos::Utility::CLocaleHelper;
using Libcore::ICU::ILocaleData;
using Libcore::ICU::ILocaleDataHelper;
using Libcore::ICU::CLocaleDataHelper;
using Elastos::Utility::Currency;

namespace Elastos {
namespace Text {

CAR_INTERFACE_IMPL_2(DecimalFormatSymbols, Object, IDecimalFormatSymbols, ICloneable)

ECode DecimalFormatSymbols::constructor()
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return constructor(locale);
}

ECode DecimalFormatSymbols::constructor(
    /* [in] */ ILocale* locale)
{
    AutoPtr<ILocaleDataHelper> helper;
    CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&helper);
    AutoPtr<ILocaleData> localeData;
    FAIL_RETURN(helper->Get(locale, (ILocaleData**)&localeData));
    localeData->GetZeroDigit(&mZeroDigit);
    mDigit = '#';
    localeData->GetDecimalSeparator(&mDecimalSeparator);
    localeData->GetGroupingSeparator(&mGroupingSeparator);
    localeData->GetPatternSeparator(&mPatternSeparator);
    localeData->GetPercent(&mPercent);
    localeData->GetPerMill(&mPerMill);
    localeData->GetMonetarySeparator(&mMonetarySeparator);
    localeData->GetMinusSign(&mMinusSign);
    localeData->GetInfinity(&mInfinity);
    localeData->GetNaN(&mNaN);
    localeData->GetExponentSeparator(&mExponentSeparator);
    mLocale = locale;
    ECode ec = Currency::GetInstance(locale, (ICurrency**)&mCurrency);
    if (SUCCEEDED(ec)) {
        mCurrency->GetSymbol(locale, &mCurrencySymbol);
        mCurrency->GetCurrencyCode(&mIntlCurrencySymbol);
    }
    else {
        mCurrency = Currency::GetInstance(String("XXX"));
        localeData->GetCurrencySymbol(&mCurrencySymbol);
        localeData->GetInternationalCurrencySymbol(&mIntlCurrencySymbol);
    }
    return NOERROR;
}

ECode DecimalFormatSymbols::GetInstance(
    /* [out] */ IDecimalFormatSymbols** instance)
{
    VALIDATE_NOT_NULL(instance);

    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return GetInstance(locale, instance);
}

ECode DecimalFormatSymbols::GetInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ IDecimalFormatSymbols** instance)
{
    VALIDATE_NOT_NULL(instance);

    if (locale == NULL) {
        //throw new NullPointerException("locale == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    return CDecimalFormatSymbols::New(locale, instance);
}

ECode DecimalFormatSymbols::GetAvailableLocales(
    /* [out] */ ArrayOf<ILocale*>** locales)
{
    return ICUUtil::GetAvailableDecimalFormatSymbolsLocales(locales);
}

ECode DecimalFormatSymbols::Clone(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    AutoPtr<IDecimalFormatSymbols> outdec ;
    FAIL_RETURN(CDecimalFormatSymbols::New(mLocale, (IDecimalFormatSymbols **)&outdec));

    CloneImpl(outdec);

    *object = outdec;
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode DecimalFormatSymbols::CloneImpl(
    /* [in] */ IDecimalFormatSymbols* object)
{
    assert(object);

    object->SetExponentSeparator(mExponentSeparator);
    object->SetCurrency(mCurrency);
    object->SetInternationalCurrencySymbol(mIntlCurrencySymbol);
    object->SetCurrencySymbol(mCurrencySymbol);
    object->SetDecimalSeparator(mDecimalSeparator);
    object->SetDigit(mDigit);
    object->SetGroupingSeparator(mGroupingSeparator);
    object->SetInfinity(mInfinity);
    // object->SetMinusSign(mMinusSign);
    object->SetMonetaryDecimalSeparator(mMonetarySeparator);
    object->SetNaN(mNaN);
    object->SetPatternSeparator(mPatternSeparator);
    object->SetPercent(mPercent);
    object->SetPerMill(mPerMill);
    object->SetZeroDigit(mZeroDigit);

    return NOERROR;
}

ECode DecimalFormatSymbols::GetCurrency(
    /* [out] */ ICurrency** currency)
{
    VALIDATE_NOT_NULL(currency)
    *currency = mCurrency;
    REFCOUNT_ADD(*currency);
    return NOERROR;
}

ECode DecimalFormatSymbols::GetInternationalCurrencySymbol(
    /* [out] */ String* symbol)
{
    VALIDATE_NOT_NULL(symbol)
    *symbol = mIntlCurrencySymbol;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetCurrencySymbol(
    /* [out] */ String* symbol)
{
    VALIDATE_NOT_NULL(symbol)
    *symbol = mCurrencySymbol;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetDecimalSeparator(
    /* [out] */ Char32* separator)
{
    VALIDATE_NOT_NULL(separator)
    *separator = mDecimalSeparator;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetDigit(
    /* [out] */ Char32* digit)
{
    VALIDATE_NOT_NULL(digit)
    *digit = mDigit;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetGroupingSeparator(
    /* [out] */ Char32* separator)
{
    VALIDATE_NOT_NULL(separator)
    *separator = mGroupingSeparator;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetInfinity(
    /* [out] */ String* infinity)
{
    VALIDATE_NOT_NULL(infinity)
    *infinity = mInfinity;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetMinusSign(
    /* [out] */ Char32* minusSign)
{
    VALIDATE_NOT_NULL(minusSign)

    if (mMinusSign.GetLength() == 1) {
        *minusSign  = mMinusSign.GetChar(0);
        return NOERROR;
    }

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
    // throw new UnsupportedOperationException(
    //         "Minus sign spans multiple characters: " + minusSign);
}

ECode DecimalFormatSymbols::GetMonetaryDecimalSeparator(
    /* [out] */ Char32* separator)
{
    VALIDATE_NOT_NULL(separator)
    *separator = mMonetarySeparator;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetNaN(
    /* [out] */ String* infinity)
{
    VALIDATE_NOT_NULL(infinity)
    *infinity = mNaN;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetPatternSeparator(
    /* [out] */ Char32* separator)
{
    VALIDATE_NOT_NULL(separator)
    *separator = mPatternSeparator;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetPercent(
    /* [out] */ Char32* percent)
{
    VALIDATE_NOT_NULL(percent)
    *percent = mPercent;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetPerMill(
    /* [out] */ Char32* perMill)
{
    VALIDATE_NOT_NULL(perMill)
    *perMill = mPerMill;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetZeroDigit(
    /* [out] */ Char32* zeroDigit)
{
    VALIDATE_NOT_NULL(zeroDigit)
    *zeroDigit = mZeroDigit;
    return NOERROR;
}

ECode DecimalFormatSymbols::GetExponentSeparator(
    /* [out] */ String* separator)
{
    VALIDATE_NOT_NULL(separator)
    *separator = mExponentSeparator;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetCurrency(
    /* [in] */ ICurrency* currency)
{
    if (currency == NULL) {
        //throw new NullPointerException("currency == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (currency == mCurrency) {
        return NOERROR;
    }
    mCurrency = currency;
    currency->GetCurrencyCode(&mIntlCurrencySymbol);
    currency->GetSymbol(mLocale, &mCurrencySymbol);
    return NOERROR;
}

ECode DecimalFormatSymbols::SetInternationalCurrencySymbol(
    /* [in] */ const String& value)
{
    if (value.IsNull()) {
        mCurrency = NULL;
        mIntlCurrencySymbol = NULL;
        return NOERROR;
    }

    if (value.Equals(mIntlCurrencySymbol)) {
        return NOERROR;
    }

    mCurrency = Currency::GetInstance(value);
    ECode ec = mCurrency->GetSymbol(mLocale, &mCurrencySymbol);
    if (FAILED(ec)) {
        mCurrency = NULL;
    }
    mIntlCurrencySymbol = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetCurrencySymbol(
    /* [in] */ const String& value)
{
    mCurrencySymbol = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetDecimalSeparator(
    /* [in] */ Char32 value)
{
    mDecimalSeparator = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetDigit(
    /* [in] */ Char32 value)
{
    mDigit = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetGroupingSeparator(
    /* [in] */ Char32 value)
{
    mGroupingSeparator = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetInfinity(
    /* [in] */ const String& value)
{
    mInfinity = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetMinusSign(
    /* [in] */ Char32 value)
{
    StringBuilder sb;
    sb.AppendChar(value);
    mMinusSign = sb.ToString();
    return NOERROR;
}

ECode DecimalFormatSymbols::SetMonetaryDecimalSeparator(
    /* [in] */ Char32 value)
{
    mMonetarySeparator = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetNaN(
    /* [in] */ const String& value)
{
    mNaN = value;
    return NOERROR;
}
ECode DecimalFormatSymbols::SetPatternSeparator(
    /* [in] */ Char32 value)
{
    mPatternSeparator = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetPercent(
    /* [in] */ Char32 value)
{
    mPercent = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetPerMill(
    /* [in] */ Char32 value)
{
    mPerMill = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetZeroDigit(
    /* [in] */ Char32 value)
{
    mZeroDigit = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::SetExponentSeparator(
    /* [in] */ const String& value)
{
    if (value.IsNull()) {
        //throw new NullPointerException("value == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    mExponentSeparator = value;
    return NOERROR;
}

ECode DecimalFormatSymbols::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if (this->Probe(EIID_IDecimalFormatSymbols) == IDecimalFormatSymbols::Probe(object)) {
        *res = TRUE;
        return NOERROR;
    }
    if (object->Probe(EIID_IDecimalFormatSymbols) == NULL) {
        *res = FALSE;
        return NOERROR;
    }
    AutoPtr<IDecimalFormatSymbols> obj = (IDecimalFormatSymbols*) object;

    AutoPtr<ICurrency> currency;
    obj->GetCurrency((ICurrency**)&currency);
    String currencySymbol;
    obj->GetCurrencySymbol(&currencySymbol);
    Char32 decimalSeparator;
    obj->GetDecimalSeparator(&decimalSeparator);
    Char32 digit;
    obj->GetDigit(&digit);
    String exponentSeparator;
    obj->GetExponentSeparator(&exponentSeparator);
    Char32 groupingSeparator;
    obj->GetGroupingSeparator(&groupingSeparator);
    String infinity;
    obj->GetInfinity(&infinity);
    String intlCurrencySymbol;
    obj->GetInternationalCurrencySymbol(&intlCurrencySymbol);
    Char32 minusSign;
    obj->GetMinusSign(&minusSign);
    Char32 monetarySeparator;
    obj->GetMonetaryDecimalSeparator(&monetarySeparator);
    String NaN;
    obj->GetNaN(&NaN);
    Char32 patternSeparator;
    obj->GetPatternSeparator(&patternSeparator);
    Char32 perMill;
    obj->GetPerMill(&perMill);
    Char32 percent;
    obj->GetPercent(&percent);
    Char32 zeroDigit;
    obj->GetZeroDigit(&zeroDigit);
    *res = mCurrency == currency &&
            mCurrencySymbol.Equals(currencySymbol) &&
            mDecimalSeparator == decimalSeparator &&
            mDigit == digit &&
            mExponentSeparator.Equals(exponentSeparator) &&
            mGroupingSeparator == groupingSeparator &&
            mInfinity.Equals(infinity) &&
            mIntlCurrencySymbol.Equals(intlCurrencySymbol) &&
            mMinusSign.GetChar(0) == minusSign &&
            mMonetarySeparator == monetarySeparator &&
            mNaN.Equals(NaN) &&
            mPatternSeparator == patternSeparator &&
            mPerMill == perMill &&
            mPercent == percent &&
            mZeroDigit == zeroDigit;
    return NOERROR;
}

} // namespace Text
} // namespace Elastos
