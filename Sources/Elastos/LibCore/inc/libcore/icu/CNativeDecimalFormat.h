
#ifndef __CNATIVEDECIMALFORMAT_H__
#define __CNATIVEDECIMALFORMAT_H__

#include "_CNativeDecimalFormat.h"
#include <Elastos.CoreLibrary_server.h>
#include <elastos/StringBuffer.h>

using Elastos::Math::IBigDecimal;
using Elastos::Math::IBigInteger;
using Elastos::Math::RoundingMode;
using Elastos::Core::INumber;
using Elastos::Core::StringBuffer;
using Elastos::Text::IFieldPosition;
using Elastos::Text::IParsePosition;
using Elastos::Text::IDecimalFormatSymbols;
using Elastos::Text::IAttributedCharacterIterator;
using Elastos::Text::IAttributedCharacterIteratorAttribute;
using Elastos::Utility::ICurrency;

namespace Libcore {
namespace ICU {

CarClass(CNativeDecimalFormat)
{
public:
    // Utility to get information about field positions from native (ICU) code.
    class FieldPositionIterator : public Object
    {
        friend class NativeDecimalFormat;

    public:
        static CARAPI_(AutoPtr<FieldPositionIterator>) ForFieldPosition(
            /* [in] */ IFieldPosition* fp);

        CARAPI Next(
            /* [out] */ Boolean* next);

        CARAPI FieldId(
            /* [out] */ Int32* fieldId);

        CARAPI Field(
            /* [out] */ IAttributedCharacterIteratorAttribute** field);

        CARAPI Start(
            /* [out] */ Int32* start);

        CARAPI Limit(
            /* [out] */ Int32* limit);

        // called by native
        CARAPI_(void) SetData(
            /* [in] */ ArrayOf<Int32>* data);

    public:
        FieldPositionIterator();

        static CARAPI_(Int32) GetNativeFieldPositionId(
            /* [in] */ IFieldPosition* fp);

    public:
        static CARAPI SetFieldPosition(
            /* [in] */ FieldPositionIterator* fpi,
            /* [in] */ IFieldPosition* fp);

        CARAPI CheckValid();

    private:
        AutoPtr< ArrayOf<Int32> > mData;
        Int32 mPos; // so first call to next() leaves pos at 0
        static AutoPtr< ArrayOf<IAttributedCharacterIteratorAttribute *> > sFields;
    };

public:
    CNativeDecimalFormat();

    ~CNativeDecimalFormat();

    //synchronized
    CARAPI Close();

    /**
     * Copies the DecimalFormatSymbols settings into our native peer in bulk.
     */
    CARAPI SetDecimalFormatSymbols(
        /* [in] */ IDecimalFormatSymbols* dfs);

    CARAPI SetDecimalFormatSymbols(
        /* [in] */ ILocaleData* localeData);

    CARAPI FormatBigDecimal(
        /* [in] */ IBigDecimal* value,
        /* [in] */ IFieldPosition* field,
        /* [out, callee] */ ArrayOf<Char32>** result);

    CARAPI FormatBigInteger(
        /* [in] */ IBigInteger* value,
        /* [in] */ IFieldPosition* field,
        /* [out, callee] */ ArrayOf<Char32>** result);

    CARAPI FormatInt64(
        /* [in] */ Int64 value,
        /* [in] */ IFieldPosition* field,
        /* [out, callee] */ ArrayOf<Char32>** array);

    CARAPI FormatDouble(
        /* [in] */ Double value,
        /* [in] */ IFieldPosition* field,
        /* [out, callee] */ ArrayOf<Char32>** array);

    CARAPI ApplyLocalizedPattern(
        /* [in] */ const String& pattern);

    CARAPI ApplyPattern(
        /* [in] */ const String& pattern);

    CARAPI FormatToCharacterIterator(
        /* [in] */ IInterface* object,
        /* [out] */ IAttributedCharacterIterator** iter);

    CARAPI ToLocalizedPattern(
        /* [out] */ String* localizedPattern);

    CARAPI ToPattern(
        /* [out] */ String* pattern);

    CARAPI Parse(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** number);

    // start getter and setter
    CARAPI GetMaximumFractionDigits(
        /* [out] */ Int32* maxFractionDigits);

    CARAPI GetMaximumIntegerDigits(
        /* [out] */ Int32* maxIntegerDigits);

    CARAPI GetMinimumFractionDigits(
        /* [out] */ Int32* minFractionDigits);

    CARAPI GetMinimumIntegerDigits(
        /* [out] */ Int32* minIntegerDigits);

    CARAPI GetGroupingSize(
        /* [out] */ Int32* size);

    CARAPI GetMultiplier(
        /* [out] */ Int32* multiplier);

    CARAPI GetNegativePrefix(
        /* [out] */ String* prefix);

    CARAPI GetNegativeSuffix(
        /* [out] */ String* suffix);

    CARAPI GetPositivePrefix(
        /* [out] */ String* prefix);

    CARAPI GetPositiveSuffix(
        /* [out] */ String* suffix);

    CARAPI IsDecimalSeparatorAlwaysShown(
        /* [out] */ Boolean* isAlwaysShown);

    CARAPI IsParseBigDecimal(
        /* [out] */ Boolean* isParseBigDecimal);

    CARAPI IsParseIntegerOnly(
        /* [out] */ Boolean* isParseIntegerOnly);

    CARAPI IsGroupingUsed(
        /* [out] */ Boolean* isGroupingUsed);

    CARAPI SetDecimalSeparatorAlwaysShown(
        /* [in] */ Boolean value);

    CARAPI SetCurrency(
        /* [in] */ ICurrency* currency);

    CARAPI SetGroupingSize(
        /* [in] */ Int32 value);

    CARAPI SetGroupingUsed(
        /* [in] */ Boolean value);

    CARAPI SetMaximumFractionDigits(
        /* [in] */ Int32 value);

    CARAPI SetMaximumIntegerDigits(
        /* [in] */ Int32 value);

    CARAPI SetMinimumFractionDigits(
        /* [in] */ Int32 value);

    CARAPI SetMinimumIntegerDigits(
        /* [in] */ Int32 value);

    CARAPI SetMultiplier(
        /* [in] */ Int32 value);

    CARAPI SetNegativePrefix(
        /* [in] */ const String& value);

    CARAPI SetNegativeSuffix(
        /* [in] */ const String& value);

    CARAPI SetPositivePrefix(
        /* [in] */ const String& value);

    CARAPI SetPositiveSuffix(
        /* [in] */ const String& value);

    CARAPI SetParseBigDecimal(
        /* [in] */ Boolean value);

    CARAPI SetParseIntegerOnly(
        /* [in] */ Boolean value);

    CARAPI SetRoundingMode(
        /* [in] */ RoundingMode mode,
        /* [in] */ Double roundingIncrement);

    CARAPI GetAddr(
        /* [out] */ Int32* value);

    CARAPI GetLastPattern(
        /* [out] */ String* value);

    CARAPI GetNegPrefNull(
        /* [out] */ Boolean* value);

    CARAPI GetNegSuffNull(
        /* [out] */ Boolean* value);

    CARAPI GetPosPrefNull(
        /* [out] */ Boolean* value);

    CARAPI GetPosSuffNull(
        /* [out] */ Boolean* value);

    CARAPI constructor(
        /* [in] */ const String& pattern,
        /* [in] */ IDecimalFormatSymbols* dfs);

    // Used so java.util.Formatter doesn't need to allocate DecimalFormatSymbols instances.
    CARAPI constructor(
        /* [in] */ const String& pattern,
        /* [in] */ ILocaleData* data);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

private:
    CARAPI_(Int32) MakeScalePositive(
        /* [in] */ Int32 scale,
        /* [in] */ StringBuffer& val);

    static CARAPI ApplyPattern(
        /* [in] */ Int32 addr,
        /* [in] */ Boolean localized,
        /* [in] */ const String& pattern);

    static CARAPI ApplyPatternImpl(
        /* [in] */ Int32 addr,
        /* [in] */ Boolean localized,
        /* [in] */ const String& pattern);

    static CARAPI_(Int32) CloneImpl(
        /* [in] */ Int32 addr);

    static CARAPI_(void) Close(
        /* [in] */ Int32 addr);

    static CARAPI_(AutoPtr<ArrayOf<Char32> >) FormatInt64(
        /* [in] */ Int32 addr,
        /* [in] */ Int64 value,
        /* [in] */ FieldPositionIterator* iter);

    static CARAPI_(AutoPtr<ArrayOf<Char32> >) FormatDouble(
        /* [in] */ Int32 addr,
        /* [in] */ Double value,
        /* [in] */ FieldPositionIterator* iter);

    static CARAPI_(AutoPtr<ArrayOf<Char32> >) FormatDigitList(
        /* [in] */ Int32 addr,
        /* [in] */ const String& value,
        /* [in] */ FieldPositionIterator* iter);

    static CARAPI_(Int32) GetAttribute(
        /* [in] */ Int32 addr,
        /* [in] */ Int32 symbol);

    static CARAPI GetTextAttribute(
        /* [in] */ Int32 addr,
        /* [in] */ Int32 symbol,
        /* [out] */ String* _attr);

    static CARAPI Open(
        /* [in] */ const String& pattern,
        /* [in] */ const String& currencySymbol,
        /* [in] */ Char32 decimalSeparator,
        /* [in] */ Char32 digit,
        /* [in] */ const String& exponentSeparator,
        /* [in] */ Char32 groupingSeparator,
        /* [in] */ const String& infinity,
        /* [in] */ const String& internationalCurrencySymbol,
        /* [in] */ Char32 minusSign,
        /* [in] */ Char32 monetaryDecimalSeparator,
        /* [in] */ const String& nan,
        /* [in] */ Char32 patternSeparator,
        /* [in] */ Char32 percent,
        /* [in] */ Char32 perMill,
        /* [in] */ Char32 zeroDigit,
        /* [in] */ Int32* result);

    static CARAPI_(AutoPtr<INumber>) Parse(
        /* [in] */ Int32 addr,
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [in] */ Boolean parseBigDecimal);

    static CARAPI_(void) SetDecimalFormatSymbols(
        /* [in] */ Int32 addr,
        /* [in] */ const String& currencySymbol,
        /* [in] */ Char32 decimalSeparator,
        /* [in] */ Char32 digit,
        /* [in] */ const String& exponentSeparator,
        /* [in] */ Char32 groupingSeparator,
        /* [in] */ const String& infinity,
        /* [in] */ const String& internationalCurrencySymbol,
        /* [in] */ Char32 minusSign,
        /* [in] */ Char32 monetaryDecimalSeparator,
        /* [in] */ const String& nan,
        /* [in] */ Char32 patternSeparator,
        /* [in] */ Char32 percent,
        /* [in] */ Char32 perMill,
        /* [in] */ Char32 zeroDigit);

    static CARAPI SetSymbol(
        /* [in] */ Int32 addr,
        /* [in] */ Int32 symbol,
        /* [in] */ const String& str);

    static CARAPI_(void) SetAttribute(
        /* [in] */ Int32 addr,
        /* [in] */ Int32 symbol,
        /* [in] */ Int32 i);

    static CARAPI_(void) SetRoundingMode(
        /* [in] */ Int32 addr,
        /* [in] */ Int32 roundingMode,
        /* [in] */ Double roundingIncrement);

    static CARAPI SetTextAttribute(
        /* [in] */ Int32 addr,
        /* [in] */ Int32 symbol,
        /* [in] */ const String& str);

    static CARAPI_(String) ToPatternImpl(
        /* [in] */ Int32 addr,
        /* [in] */ Boolean localized);

private:
    /**
     * Constants corresponding to the native type UNumberFormatSymbol, for setSymbol.
     */
    const static Int32 UNUM_DECIMAL_SEPARATOR_SYMBOL;
    const static Int32 UNUM_GROUPING_SEPARATOR_SYMBOL;
    const static Int32 UNUM_PATTERN_SEPARATOR_SYMBOL;
    const static Int32 UNUM_PERCENT_SYMBOL;
    const static Int32 UNUM_ZERO_DIGIT_SYMBOL;
    const static Int32 UNUM_DIGIT_SYMBOL;
    const static Int32 UNUM_MINUS_SIGN_SYMBOL;
    const static Int32 UNUM_PLUS_SIGN_SYMBOL;
    const static Int32 UNUM_CURRENCY_SYMBOL;
    const static Int32 UNUM_INTL_CURRENCY_SYMBOL;
    const static Int32 UNUM_MONETARY_SEPARATOR_SYMBOL;
    const static Int32 UNUM_EXPONENTIAL_SYMBOL;
    const static Int32 UNUM_PERMILL_SYMBOL;
    const static Int32 UNUM_PAD_ESCAPE_SYMBOL;
    const static Int32 UNUM_INFINITY_SYMBOL;
    const static Int32 UNUM_NAN_SYMBOL;
    const static Int32 UNUM_SIGNIFICANT_DIGIT_SYMBOL;
    const static Int32 UNUM_MONETARY_GROUPING_SEPARATOR_SYMBOL;
    const static Int32 UNUM_FORMAT_SYMBOL_COUNT;

    /**
     * Constants corresponding to the native type UNumberFormatAttribute, for
     * getAttribute/setAttribute.
     */
    const static Int32 UNUM_PARSE_INT_ONLY;
    const static Int32 UNUM_GROUPING_USED;
    const static Int32 UNUM_DECIMAL_ALWAYS_SHOWN;
    const static Int32 UNUM_MAX_INTEGER_DIGITS;
    const static Int32 UNUM_MIN_INTEGER_DIGITS;
    const static Int32 UNUM_INTEGER_DIGITS;
    const static Int32 UNUM_MAX_FRACTION_DIGITS;
    const static Int32 UNUM_MIN_FRACTION_DIGITS;
    const static Int32 UNUM_FRACTION_DIGITS;
    const static Int32 UNUM_MULTIPLIER;
    const static Int32 UNUM_GROUPING_SIZE;
    const static Int32 UNUM_ROUNDING_MODE;
    const static Int32 UNUM_ROUNDING_INCREMENT;
    const static Int32 UNUM_FORMAT_WIDTH;
    const static Int32 UNUM_PADDING_POSITION;
    const static Int32 UNUM_SECONDARY_GROUPING_SIZE;
    const static Int32 UNUM_SIGNIFICANT_DIGITS_USED;
    const static Int32 UNUM_MIN_SIGNIFICANT_DIGITS;
    const static Int32 UNUM_MAX_SIGNIFICANT_DIGITS;
    const static Int32 UNUM_LENIENT_PARSE;

    /**
     * Constants corresponding to the native type UNumberFormatTextAttribute, for
     * getTextAttribute/setTextAttribute.
     */
    const static Int32 UNUM_POSITIVE_PREFIX;
    const static Int32 UNUM_POSITIVE_SUFFIX;
    const static Int32 UNUM_NEGATIVE_PREFIX;
    const static Int32 UNUM_NEGATIVE_SUFFIX;
    const static Int32 UNUM_PADDING_CHARACTER;
    const static Int32 UNUM_CURRENCY_CODE;
    const static Int32 UNUM_DEFAULT_RULESET;
    const static Int32 UNUM_PUBLIC_RULESETS;

    /**
     * The address of the ICU DecimalFormat* on the native heap.
     */
    Int32 mAddress;

    /**
     * The last pattern we gave to ICU, so we can make repeated applications cheap.
     * This helps in cases like String.format("%.2f,%.2f\n", x, y) where the DecimalFormat is
     * reused.
     */
    String mLastPattern;

    // TODO: store all these in DecimalFormat instead!
    Boolean mNegPrefNull;
    Boolean mNegSuffNull;
    Boolean mPosPrefNull;
    Boolean mPosSuffNull;
    Boolean mParseBigDecimal;

    /**
     * Cache the BigDecimal form of the multiplier. This is null until we've
     * formatted a BigDecimal (with a multiplier that is not 1), or the user has
     * explicitly called {@link #setMultiplier(int)} with any multiplier.
     */
    AutoPtr<IBigDecimal> mMultiplierBigDecimal;
};

} // namespace ICU
} // namespace Libcore

#endif //__CNATIVEDECIMALFORMAT_H__
