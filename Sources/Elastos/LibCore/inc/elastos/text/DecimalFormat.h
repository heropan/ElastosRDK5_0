#ifndef __ELASTOS_TEXT_DECIMALFORMAT__
#define __ELASTOS_TEXT_DECIMALFORMAT__

#include "NumberFormat.h"

using Libcore::ICU::INativeDecimalFormat;
using Elastos::Text::IAttributedCharacterIterator;;
using Elastos::Text::IDecimalFormatSymbols;
using Elastos::Text::IFieldPosition;
using Elastos::Text::IParsePosition;
using Elastos::Utility::ICurrency;
using Elastos::Core::IDouble;

namespace Elastos {
namespace Text {

class DecimalFormat
    : public NumberFormat
    , public IDecimalFormat
{
public:
    CAR_INTERFACE_DECL()

    DecimalFormat();

    virtual CARAPI ApplyLocalizedPattern(
        /* [in] */ const String& pattern);

    virtual CARAPI ApplyPattern(
        /* [in] */ const String& pattern);

    //@Override
    CARAPI Clone(
        /* [out] */ IInterface** outobj);

    CARAPI CloneImpl(
        /* [in] */ IDecimalFormat* outobj);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI FormatToCharacterIterator(
        /* [in] */ IInterface* object,
        /* [out] */ IAttributedCharacterIterator** characterIterator);

    //@Override
    CARAPI Format(
        /* [in] */ Double value,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* field);

    //@Override
    CARAPI Format(
        /* [in] */ Int64 value,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* field);

    //@Override
    CARAPI Format(
        /* [in] */ IInterface* object,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* field);

    virtual CARAPI GetDecimalFormatSymbols(
        /* [out] */ IDecimalFormatSymbols** symbols);

    // @Override
    virtual CARAPI GetCurrency(
        /* [out] */ ICurrency** currency);

    virtual CARAPI GetGroupingSize(
        /* [out] */ Int32* size);

    virtual CARAPI GetMultiplier(
        /* [out] */ Int32* multiplier);

    virtual CARAPI GetNegativePrefix(
        /* [out] */ String* prefix);

    virtual CARAPI GetNegativeSuffix(
        /* [out] */ String* suffix);

    virtual CARAPI GetPositivePrefix(
        /* [out] */ String* prefix);

    virtual CARAPI GetPositiveSuffix(
        /* [out] */ String* suffix);

    //@Override
    //public int hashCode();

    virtual CARAPI IsDecimalSeparatorAlwaysShown(
        /* [out] */ Boolean* alwaysShown);

    virtual CARAPI IsParseBigDecimal(
        /* [out] */ Boolean* isBigDecimal);

    //@Override
    CARAPI SetParseIntegerOnly(
        /* [in] */ Boolean value);

    //@Override
    CARAPI IsParseIntegerOnly(
        /* [out] */ Boolean* isParseIntegerOnly);

    //@Override
    CARAPI Parse(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** value);

    virtual CARAPI SetDecimalFormatSymbols(
        /* [in] */ IDecimalFormatSymbols* value);

    //@Override
    CARAPI SetCurrency(
        /* [in] */ ICurrency* currency);

    virtual CARAPI SetDecimalSeparatorAlwaysShown(
        /* [in] */ Boolean value);

    virtual CARAPI SetGroupingSize(
        /* [in] */ Int32 value);

    //@Override
    CARAPI SetGroupingUsed(
        /* [in] */ Boolean value);

    //@Override
    CARAPI IsGroupingUsed(
        /* [out] */ Boolean* isGroupingUsed);

    //@Override
    CARAPI SetMaximumFractionDigits(
        /* [in] */ Int32 value);

    //@Override
    CARAPI SetMaximumIntegerDigits(
        /* [in] */ Int32 value);

    //@Override
    CARAPI SetMinimumFractionDigits(
        /* [in] */ Int32 value);

    //@Override
    CARAPI SetMinimumIntegerDigits(
        /* [in] */ Int32 value);

    virtual CARAPI SetMultiplier(
        /* [in] */ Int32 value);

    virtual CARAPI SetNegativePrefix(
        /* [in] */ const String& value);

    virtual CARAPI SetNegativeSuffix(
        /* [in] */ const String& value);

    virtual CARAPI SetPositivePrefix(
        /* [in] */ const String& value);

    virtual CARAPI SetPositiveSuffix(
        /* [in] */ const String& value);

    virtual CARAPI SetParseBigDecimal(
        /* [in] */ Boolean newValue);

    virtual CARAPI ToLocalizedPattern(
        /* [out] */ String* pattern);

    virtual CARAPI ToPattern(
        /* [out] */ String* pattern);

    //override
    CARAPI GetRoundingMode(
        /* [out] */ RoundingMode* mode);

    //override
    CARAPI SetRoundingMode(
        /* [in] */ RoundingMode mode);

    CARAPI GetNdf(
        /* [out] */ INativeDecimalFormat** ndf);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& pattern);

    CARAPI constructor(
        /* [in] */ const String& pattern,
        /* [in] */ IDecimalFormatSymbols* value);

    CARAPI constructor(
        /* [in] */ const String& pattern,
        /* [in] */ ILocale* locale);

private:
    CARAPI InitNative(
        /* [in] */ const String& pattern);

    CARAPI CheckBufferAndFieldPosition(
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* position);

    // @SuppressWarnings("nls")
    // private void writeObject(ObjectOutputStream stream);


    // private void readObject(ObjectInputStream stream);

private:

    mutable AutoPtr<IDecimalFormatSymbols> mSymbols;

    mutable AutoPtr<INativeDecimalFormat> mNdf;

    mutable RoundingMode mRoundingMode;

    static const AutoPtr<IDouble> NEGATIVE_ZERO_DOUBLE;

/*
    private static final ObjectStreamField[] serialPersistentFields = {
            new ObjectStreamField("positivePrefix", String.class),
            new ObjectStreamField("positiveSuffix", String.class),
            new ObjectStreamField("negativePrefix", String.class),
            new ObjectStreamField("negativeSuffix", String.class),
            new ObjectStreamField("posPrefixPattern", String.class),
            new ObjectStreamField("posSuffixPattern", String.class),
            new ObjectStreamField("negPrefixPattern", String.class),
            new ObjectStreamField("negSuffixPattern", String.class),
            new ObjectStreamField("multiplier", int.class),
            new ObjectStreamField("groupingSize", byte.class),
            new ObjectStreamField("groupingUsed", boolean.class),
            new ObjectStreamField("decimalSeparatorAlwaysShown", boolean.class),
            new ObjectStreamField("parseBigDecimal", boolean.class),
            new ObjectStreamField("roundingMode", RoundingMode.class),
            new ObjectStreamField("symbols", DecimalFormatSymbols.class),
            new ObjectStreamField("useExponentialNotation", boolean.class),
            new ObjectStreamField("minExponentDigits", byte.class),
            new ObjectStreamField("maximumIntegerDigits", int.class),
            new ObjectStreamField("minimumIntegerDigits", int.class),
            new ObjectStreamField("maximumFractionDigits", int.class),
            new ObjectStreamField("minimumFractionDigits", int.class),
            new ObjectStreamField("serialVersionOnStream", int.class), };
*/
};

} // namespace Text
} // namespace Elastos

#endif //__ELASTOS_TEXT_DECIMALFORMAT__
