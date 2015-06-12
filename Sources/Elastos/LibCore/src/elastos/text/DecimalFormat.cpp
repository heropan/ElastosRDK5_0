
#include "DecimalFormat.h"
#include <elautoptr.h>
#include <elastos/core/Math.h>
#include <unistd.h>
#include <elastos/core/Character.h>
#include "CDouble.h"
#include "CInteger64.h"
#include "CDecimalFormatSymbols.h"
// #include "CNativeDecimalFormat.h"
// #include "CBigDecimal.h"
// #include "Currency.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::Character;
using Elastos::Core::CDouble;
using Elastos::Core::IDouble;
using Elastos::Core::EIID_IInteger64;
using Elastos::Core::IInteger64;
using Elastos::Core::CInteger64;
using Elastos::Core::EIID_IDouble;
using Elastos::Core::EIID_INumber;
using Elastos::Text::IDecimalFormatSymbols;
using Elastos::Text::CDecimalFormatSymbols;
// using Libcore::ICU::CNativeDecimalFormat;
using Elastos::Math::IBigInteger;
using Elastos::Math::EIID_IBigInteger;
using Elastos::Math::IBigDecimal;
using Elastos::Math::EIID_IBigDecimal;
// using Elastos::Math::CBigDecimal;
// using Elastos::Utility::Currency;

namespace Elastos {
namespace Text {

static AutoPtr<IDouble>  IntNEGATIVE_ZERO_DOUBLE()
{
    AutoPtr<CDouble> out;
    CDouble::NewByFriend(-0.0, (CDouble **)&out);
    return (IDouble*)out->Probe(EIID_IDouble);
}

const AutoPtr<IDouble> DecimalFormat::NEGATIVE_ZERO_DOUBLE = IntNEGATIVE_ZERO_DOUBLE();

DecimalFormat::DecimalFormat()
    : mRoundingMode(Elastos::Math::RoundingMode_HALF_EVEN)
{}

ECode DecimalFormat::constructor()
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    CDecimalFormatSymbols::New(locale, (IDecimalFormatSymbols**)&mSymbols);
    AutoPtr<ILocaleDataHelper> helper;
    assert(0 && "TODO");
    // CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&helper);
    AutoPtr<ILocaleData> localeData;
    FAIL_RETURN(helper->Get(locale, (ILocaleData**)&localeData));
    String numberPattern;
    localeData->GetNumberPattern(&numberPattern);
    InitNative(numberPattern);
    return NOERROR;
}

ECode DecimalFormat::constructor(
    /* [in] */ const String& pattern)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return constructor(pattern, locale);
}

ECode DecimalFormat::constructor(
    /* [in] */ const String& pattern,
    /* [in] */ IDecimalFormatSymbols* value)
{
    if (value == NULL) return E_NULL_POINTER_EXCEPTION;
    ICloneable::Probe(value)->Clone((IDecimalFormatSymbols**)&mSymbols);
    return InitNative(pattern);
}

ECode DecimalFormat::constructor(
    /* [in] */ const String& pattern,
    /* [in] */ ILocale* locale)
{
    FAIL_RETURN(CDecimalFormatSymbols::New(locale, (IDecimalFormatSymbols**)&mSymbols));
    return InitNative(pattern);
}

ECode DecimalFormat::InitNative(
    /* [in] */ const String& pattern)
{
    //try {
    assert(0 && "TODO");
    // FAIL_RETURN(CNativeDecimalFormat::New(pattern, mSymbols, (INativeDecimalFormat**)&mNdf));
    //} catch (IllegalArgumentException ex) {
    //    throw new IllegalArgumentException(pattern);
    //}
    Int32 maxFractionDigits(0);
    Int32 maxIntegerDigits(0);
    Int32 minFractionDigits(0);
    Int32 minIntegerDigits(0);
    mNdf->GetMaximumFractionDigits(&maxFractionDigits);
    mNdf->GetMaximumIntegerDigits(&maxIntegerDigits);
    mNdf->GetMinimumFractionDigits(&minFractionDigits);
    mNdf->GetMinimumIntegerDigits(&minIntegerDigits);
    NumberFormat::SetMaximumFractionDigits(maxFractionDigits);
    NumberFormat::SetMaximumIntegerDigits(maxIntegerDigits);
    NumberFormat::SetMinimumFractionDigits(minFractionDigits);
    NumberFormat::SetMinimumIntegerDigits(minIntegerDigits);
    return NOERROR;
}

ECode DecimalFormat::ApplyLocalizedPattern(
    /* [in] */ const String& pattern)
{
   return mNdf->ApplyLocalizedPattern(pattern);
}

ECode DecimalFormat::ApplyPattern(
    /* [in] */ const String& pattern)
{
    return mNdf->ApplyPattern(pattern);
}

ECode DecimalFormat::FormatToCharacterIterator(
    /* [in] */ IInterface* object,
    /* [out] */ IAttributedCharacterIterator** characterIterator)
{
    if (object == NULL) {
        //throw new NullPointerException("object == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    return mNdf->FormatToCharacterIterator(object, characterIterator);
}

ECode DecimalFormat::CheckBufferAndFieldPosition(
    /* [in] */ IStringBuffer * buffer,
    /* [in] */ IFieldPosition* position)
{
    if (buffer == NULL)
    {
        return E_NULL_POINTER_EXCEPTION;
    }
    if (position == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    return NOERROR;
}

ECode DecimalFormat::FormatDouble(
    /* [in] */ Double value,
    /* [in] */ IStringBuffer * buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ IStringBuffer ** result)
{
    VALIDATE_NOT_NULL(result)

    FAIL_RETURN(CheckBufferAndFieldPosition(buffer, field));
    // All float/double/Float/Double formatting ends up here...
    if (mRoundingMode == Elastos::Math::RoundingMode_UNNECESSARY) {
        // ICU4C doesn't support this rounding mode, so we have to fake it.
        //try {
        SetRoundingMode(Elastos::Math::RoundingMode_UP);
        AutoPtr<IFieldPosition> fp;
        CFieldPosition::New(0, (IFieldPosition**)&fp);
        AutoPtr<IStringBuffer> sb = new StringBuffer();
        AutoPtr<IStringBuffer> outsb;
        FormatDouble(value, sb, fp, (IStringBuffer **)&outsb);
        String upResult;
        ICharSequence::Probe(outsb)->ToString(&upResult);

        SetRoundingMode(Elastos::Math::RoundingMode_DOWN);
        AutoPtr<IFieldPosition> fpx;
        CFieldPosition::New(0, (IFieldPosition**)&fpx);
        String downResult;
        FormatDouble(value, sb, fp, (IStringBuffer **)&outsb);
        ICharSequence::Probe(outsb)->ToString(&downResult);
        if (!upResult.Equals(downResult)) {
            //throw new ArithmeticException("rounding mode UNNECESSARY but rounding required");
            SetRoundingMode(Elastos::Math::RoundingMode_UNNECESSARY);
            return E_ARITHMETIC_EXCEPTION;
        }
        //} finally {
        SetRoundingMode(Elastos::Math::RoundingMode_UNNECESSARY);
        //}
    }
    AutoPtr< ArrayOf<Char32> > v;
    FAIL_RETURN(mNdf->FormatDouble(value, field, (ArrayOf<Char32>**)&v));
    for (Int32 i = 0; i < v->GetLength(); ++i) {
        buffer->AppendChar((*v.Get())[i]);
    }
    *result = buffer;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode DecimalFormat::FormatInt64(
    /* [in] */ Int64 value,
    /* [in] */ IStringBuffer * buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ IStringBuffer ** result)
{
    VALIDATE_NOT_NULL(result);
    FAIL_RETURN(CheckBufferAndFieldPosition(buffer, field));

    AutoPtr< ArrayOf<Char32> > v;
    FAIL_RETURN(mNdf->FormatInt64(value, field, (ArrayOf<Char32>**)&v));

    if (v) {
        for (Int32 i = 0; i < v->GetLength(); ++i) {
            buffer->AppendChar((*v)[i]);
        }
    }

    *result = buffer;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode DecimalFormat::FormatObject(
    /* [in] */ IInterface* object,
    /* [in] */ IStringBuffer * buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ IStringBuffer ** value)
{
    VALIDATE_NOT_NULL(object)
    VALIDATE_NOT_NULL(value)

    FAIL_RETURN(CheckBufferAndFieldPosition(buffer, field));
    AutoPtr<IBigInteger> bigInteger = (IBigInteger *) object->Probe(EIID_IBigInteger);
    if (bigInteger) {
        AutoPtr<ArrayOf<Char32> > chars = NULL;
        Int32 bitlen(0);
        bigInteger->BitLength(&bitlen);
        if (bitlen < 64)
        {
            Int64 result(0);
            INumber::Probe(bigInteger)->Int64Value(&result);
            mNdf->FormatInt64(result, field , (ArrayOf<Char32> **)&chars);
        } else {
            mNdf->FormatBigInteger(bigInteger, field ,(ArrayOf<Char32> **)&chars);
        }
        for (Int32 i = 0; i < chars->GetLength(); ++i) {
            buffer->AppendChar((*chars.Get())[i]);
        }
        *value = buffer;
        REFCOUNT_ADD(*value);
        return NOERROR;
    } else if (object->Probe(EIID_IBigDecimal)) {
        AutoPtr<ArrayOf<Char32> > chars;
        AutoPtr<IBigDecimal> ibd = (IBigDecimal *)object->Probe(EIID_IBigDecimal);
        mNdf->FormatBigDecimal(ibd, field ,(ArrayOf<Char32> **)&chars);
        for (Int32 i = 0; i < chars->GetLength(); ++i) {
            buffer->AppendChar((*chars.Get())[i]);
        }
        *value = buffer;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    return NumberFormat::FormatObject(object, buffer, field ,value);
}

ECode DecimalFormat::GetDecimalFormatSymbols(
    /* [out] */ IDecimalFormatSymbols** symbols)
{
    return ICloneable::Probe(mSymbols)->Clone(symbols);
}

ECode DecimalFormat::GetCurrency(
    /* [out] */ ICurrency** currency)
{
    return mSymbols->GetCurrency(currency);
}

ECode DecimalFormat::GetGroupingSize(
    /* [out] */ Int32* size)
{
    return mNdf->GetGroupingSize(size);
}

ECode DecimalFormat::GetMultiplier(
    /* [out] */ Int32* multiplier)
{
    return mNdf->GetMultiplier(multiplier);
}

ECode DecimalFormat::GetNegativePrefix(
    /* [out] */ String* prefix)
{
    return mNdf->GetNegativePrefix(prefix);
}

ECode DecimalFormat::GetNegativeSuffix(
    /* [out] */ String* suffix)
{
    return mNdf->GetNegativeSuffix(suffix);
}

ECode DecimalFormat::GetPositivePrefix(
    /* [out] */ String* prefix)
{
    return mNdf->GetPositivePrefix(prefix);
}

ECode DecimalFormat::GetPositiveSuffix(
    /* [out] */ String* suffix)
{
    return mNdf->GetPositiveSuffix(suffix);
}

ECode DecimalFormat::IsDecimalSeparatorAlwaysShown(
    /* [out] */ Boolean* isAlwaysShown)
{
    return mNdf->IsDecimalSeparatorAlwaysShown(isAlwaysShown);
}

ECode DecimalFormat::IsParseBigDecimal(
    /* [out] */ Boolean* isParseBigDecimal)
{
    return mNdf->IsParseBigDecimal(isParseBigDecimal);
}

ECode DecimalFormat::SetParseIntegerOnly(
    /* [in] */ Boolean value)
{
    // In this implementation, NativeDecimalFormat is wrapped to
    // fulfill most of the format and parse feature. And this method is
    // delegated to the wrapped instance of NativeDecimalFormat.
    return mNdf->SetParseIntegerOnly(value);
}

ECode DecimalFormat::IsParseIntegerOnly(
    /* [out] */ Boolean* isParseIntegerOnly)
{
    return mNdf->IsParseIntegerOnly(isParseIntegerOnly);
}

ECode DecimalFormat::Parse(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ INumber** value)
{
    AutoPtr<INumber> number;
    FAIL_RETURN(mNdf->Parse(string, position, (INumber**)&number));
    if (number == NULL) {
        *value = NULL;
        return NOERROR;
    }
    Boolean isParseBigDecimal = FALSE;

    if (IsParseBigDecimal(&isParseBigDecimal), isParseBigDecimal) {
        if (number->Probe(EIID_IInteger64))
        {
            Int64 lnum(0);
            number->Int64Value(&lnum);
            AutoPtr<IBigDecimal> Inum;
            assert(0 && "TODO");
            // FAIL_RETURN(CBigDecimal::New(lnum,(IBigDecimal **)&Inum));
            *value = (INumber *)Inum->Probe(EIID_INumber);
            REFCOUNT_ADD(*value);
            return NOERROR;
        }
        AutoPtr<IDouble> Idou = (IDouble *)number->Probe(EIID_IDouble);
        Boolean flagdouble = FALSE;
        if (Idou && (Idou->IsInfinite(&flagdouble) , !flagdouble) && (Idou->IsNaN(&flagdouble), !flagdouble)) {
            String str;
            IObject::Probe(number)->ToString(&str);
            AutoPtr<IBigDecimal> Inum;
            assert(0 && "TODO");
            // FAIL_RETURN(CBigDecimal::New(str,(IBigDecimal **)&Inum));
            *value = (INumber *)Inum->Probe(EIID_INumber);
            REFCOUNT_ADD(*value);
            return NOERROR;

        }
        if (number->Probe(EIID_IBigInteger))
        {
            String str;
            IObject::Probe(number)->ToString(&str);
            AutoPtr<IBigDecimal> Inum;
            assert(0 && "TODO");
            // FAIL_RETURN(CBigDecimal::New(str,(IBigDecimal **)&Inum));
            *value = (INumber *)Inum->Probe(EIID_INumber);
            REFCOUNT_ADD(*value);
            return NOERROR;
        }
        *value = number;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    if (number->Probe(EIID_IBigDecimal) || number->Probe(EIID_IBigInteger))
    {
        Double lnum(0);
        number->DoubleValue(&lnum);
        AutoPtr<IDouble> Inum;
        FAIL_RETURN(CDouble::New(lnum,(IDouble **)&Inum));
        *value = INumber::Probe(Inum);
        REFCOUNT_ADD(*value);
        return NOERROR;
    }

    Boolean isParseIntegerOnly = FALSE , nagflag = FALSE;
    this->IsParseIntegerOnly(&isParseIntegerOnly);

    if (isParseIntegerOnly && number->Probe(EIID_IDouble) ) {
        if (IObject::Probe(number)->Equals(NEGATIVE_ZERO_DOUBLE ,&nagflag) , nagflag)
        {
            AutoPtr<IInteger64> Inum;
            FAIL_RETURN(CInteger64::New(0L,(IInteger64 **)&Inum));
            *value = INumber::Probe(Inum);
            REFCOUNT_ADD(*value);
            return NOERROR;
        }
    }
    *value = number;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode DecimalFormat::SetDecimalFormatSymbols(
    /* [in] */ IDecimalFormatSymbols* value)
{
    if (value != NULL) {
        mSymbols = NULL;
        ICloneable::Probe(value)->Clone((IDecimalFormatSymbols**)&mSymbols);
        String exponentSeparator;
        mSymbols->GetExponentSeparator(&exponentSeparator);
        mNdf->SetDecimalFormatSymbols(mSymbols);
    }
    return NOERROR;
}

ECode DecimalFormat::SetCurrency(
    /* [in] */ ICurrency* currency)
{
    String currencyCode;
    currency->GetCurrencyCode(&currencyCode);
    assert(0 && "TODO");
    AutoPtr<ICurrency> autoCurrency; // = Currency::GetInstance(currencyCode);
    // FAIL_RETURN(mNdf->SetCurrency(autoCurrency));
    return mSymbols->SetCurrency(currency);
}

ECode DecimalFormat::SetDecimalSeparatorAlwaysShown(
    /* [in] */ Boolean value)
{
    return mNdf->SetDecimalSeparatorAlwaysShown(value);
}

ECode DecimalFormat::SetGroupingSize(
    /* [in] */ Int32 value)
{
    return mNdf->SetGroupingSize(value);
}

ECode DecimalFormat::SetGroupingUsed(
    /* [in] */ Boolean value)
{
    return mNdf->SetGroupingUsed(value);
}

ECode DecimalFormat::IsGroupingUsed(
    /* [out] */ Boolean* isGroupingUsed)
{
    return mNdf->IsGroupingUsed(isGroupingUsed);
}

ECode DecimalFormat::SetMaximumFractionDigits(
    /* [in] */ Int32 value)
{
    NumberFormat::SetMaximumFractionDigits(value);
    Int32 maxFractionDigits;
    GetMaximumFractionDigits(&maxFractionDigits);
    mNdf->SetMaximumFractionDigits(maxFractionDigits);
    // Changing the maximum fraction digits needs to update ICU4C's rounding configuration.
    SetRoundingMode(mRoundingMode);
    return NOERROR;
}

ECode DecimalFormat::SetMaximumIntegerDigits(
    /* [in] */ Int32 value)
{
    NumberFormat::SetMaximumIntegerDigits(value);
    Int32 maxIntegerDigits;
    GetMaximumIntegerDigits(&maxIntegerDigits);
    mNdf->GetMaximumIntegerDigits(&maxIntegerDigits);
    return NOERROR;
}

ECode DecimalFormat::SetMinimumFractionDigits(
    /* [in] */ Int32 value)
{
    NumberFormat::SetMinimumFractionDigits(value);
    Int32 minFractionDigits;
    GetMinimumFractionDigits(&minFractionDigits);
    mNdf->SetMinimumFractionDigits(minFractionDigits);
    return NOERROR;
}

ECode DecimalFormat::SetMinimumIntegerDigits(
    /* [in] */ Int32 value)
{
    NumberFormat::SetMinimumIntegerDigits(value);
    Int32 minIntegerDigits;
    GetMinimumIntegerDigits(&minIntegerDigits);
    mNdf->SetMinimumIntegerDigits(minIntegerDigits);
    return NOERROR;
}

ECode DecimalFormat::SetMultiplier(
    /* [in] */ Int32 value)
{
    return mNdf->SetMultiplier(value);
}

ECode DecimalFormat::SetNegativePrefix(
    /* [in] */ const String& value)
{
    return mNdf->SetNegativePrefix(value);
}

ECode DecimalFormat::SetNegativeSuffix(
    /* [in] */ const String& value)
{
    return mNdf->SetNegativeSuffix(value);
}

ECode DecimalFormat::SetPositivePrefix(
    /* [in] */ const String& value)
{
    return mNdf->SetPositivePrefix(value);
}

ECode DecimalFormat::SetPositiveSuffix(
    /* [in] */ const String& value)
{
    return mNdf->SetPositiveSuffix(value);
}

ECode DecimalFormat::SetParseBigDecimal(
    /* [in] */ Boolean newValue)
{
    return mNdf->SetParseBigDecimal(newValue);
}

ECode DecimalFormat::ToLocalizedPattern(
    /* [out] */ String* pattern)
{
    return mNdf->ToLocalizedPattern(pattern);
}

ECode DecimalFormat::ToPattern(
    /* [out] */ String* pattern)
{
    return mNdf->ToPattern(pattern);
}

ECode DecimalFormat::GetRoundingMode(
    /* [out] */ RoundingMode* mode)
{
    *mode = mRoundingMode;
    return NOERROR;
}

ECode DecimalFormat::SetRoundingMode(
    /* [in] */ RoundingMode mode)
{
    if (mode == NULL) {
        //throw new NullPointerException("roundingmode == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    mRoundingMode = mode;
    if (mRoundingMode != Elastos::Math::RoundingMode_UNNECESSARY) { // ICU4C doesn't support UNNECESSARY.
        Int32 maxFractionDigits;
        GetMaximumFractionDigits(&maxFractionDigits);
        Double roundingIncrement = 1.0 / Elastos::Core::Math::Pow(10, Elastos::Core::Math::Max(0, maxFractionDigits));
        mNdf->SetRoundingMode(mode, roundingIncrement);
    }
    return NOERROR;
}

ECode DecimalFormat::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (this->Probe(EIID_IDecimalFormat) == IDecimalFormat::Probe(object)) {
        *result = TRUE;
        return NOERROR;
    }
    if (object->Probe(EIID_IDecimalFormat) == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    AutoPtr<IDecimalFormat> other = (IDecimalFormat*) object;
    AutoPtr<INativeDecimalFormat> ndf;
    other->GetNdf((INativeDecimalFormat**)&ndf);

    Boolean res1= FALSE, res2 = FALSE;
    mNdf->Equals(ndf, &res1);

    AutoPtr<IDecimalFormatSymbols> dfs1, dfs2;
    GetDecimalFormatSymbols((IDecimalFormatSymbols**)&dfs1);
    other->GetDecimalFormatSymbols((IDecimalFormatSymbols**)&dfs2);

    IObject::Probe(dfs1)->Equals(dfs2, &res2);

    *result = (mNdf == NULL ? ndf == NULL : res1) && res2;

    return NOERROR;
}

ECode DecimalFormat::GetNdf(
    /* [out] */ INativeDecimalFormat** ndf)
{
    VALIDATE_NOT_NULL(ndf)
    *ndf = mNdf;
    REFCOUNT_ADD(*ndf);
    return NOERROR;
}

ECode DecimalFormat::Clone(
    /* [out] */ IInterface** outobj)
{
    assert(0 && "TODO");
    return NOERROR;
}

} // namespace Text
} // namespace Elastos
