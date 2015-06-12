#ifndef __ELASTOS_TEXT_NUMBERFORMAT_H__
#define __ELASTOS_TEXT_NUMBERFORMAT_H__

#include "Elastos.CoreLibrary_server.h"
#include <elastos/core/StringBuffer.h>
#include "Format.h"
#include "CLocaleHelper.h"
// #include "CICUHelper.h"
// #include "CLocaleDataHelper.h"
#include "CFieldPosition.h"
#include "CParsePosition.h"

using Elastos::Utility::ILocale;
using Elastos::Utility::ILocaleHelper;
using Elastos::Utility::CLocaleHelper;
using Libcore::ICU::IICUHelper;
// using Libcore::ICU::CICUHelper;
using Libcore::ICU::ILocaleData;
using Libcore::ICU::ILocaleDataHelper;
// using Libcore::ICU::CLocaleDataHelper;
using Elastos::Text::IFieldPosition;
using Elastos::Text::IParsePosition;
using Elastos::Text::CFieldPosition;
using Elastos::Text::CParsePosition;
using Elastos::Utility::ICurrency;
using Elastos::Core::INumber;
using Elastos::Core::IStringBuffer;
using Elastos::Math::RoundingMode;

namespace Elastos {
namespace Text {

class NumberFormat : public Format
{
public:
    class Field : public Format::Field
    {
    protected:
        CARAPI Init(
            /* [in] */ const String& fn);

    public:
        const static AutoPtr<INumberFormatField> SIGN;
        const static AutoPtr<INumberFormatField> INTEGER;
        const static AutoPtr<INumberFormatField> FRACTION;
        const static AutoPtr<INumberFormatField> EXPONENT;
        const static AutoPtr<INumberFormatField> EXPONENT_SIGN;
        const static AutoPtr<INumberFormatField> EXPONENT_SYMBOL;
        const static AutoPtr<INumberFormatField> DECIMAL_SEPARATOR;
        const static AutoPtr<INumberFormatField> GROUPING_SEPARATOR;
        const static AutoPtr<INumberFormatField> PERCENT;
        const static AutoPtr<INumberFormatField> PERMILLE;
        const static AutoPtr<INumberFormatField> CURRENCY;
    };

public:
    CARAPI FormatDouble(
        /* [in] */ Double value,
        /* [out] */ String* result);

    virtual CARAPI FormatDouble(
        /* [in] */ Double value,
        /* [in] */ IStringBuffer * inbuffer,
        /* [in] */ IFieldPosition * field,
        /* [out] */ IStringBuffer ** outbuffer) = 0;

    CARAPI FormatInt64(
        /* [in] */ Int64 value,
        /* [out] */ String* result);

    virtual CARAPI FormatInt64(
        /* [in] */ Int64 value,
        /* [in] */ IStringBuffer * inbuffer,
        /* [in] */ IFieldPosition * field ,
        /* [out] */ IStringBuffer ** outbuffer) = 0;

    //@Override
    CARAPI FormatObject(
        /* [in] */ IInterface* object,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ IStringBuffer ** value);

    static CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    virtual CARAPI GetCurrency(
        /* [out] */ ICurrency** currency);

    static CARAPI GetCurrencyInstance(
        /* [out] */ INumberFormat** instance);

    static CARAPI GetCurrencyInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    static CARAPI GetIntegerInstance(
        /* [out] */ INumberFormat** instance);

    static CARAPI GetIntegerInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    static CARAPI GetInstance(
        /* [out] */ INumberFormat** instance);

    static CARAPI GetInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    virtual CARAPI GetMaximumFractionDigits(
        /* [out] */ Int32* number);

    virtual CARAPI GetMaximumIntegerDigits(
        /* [out] */ Int32* number);

    virtual CARAPI GetMinimumFractionDigits(
        /* [out] */ Int32* number);

    virtual CARAPI GetMinimumIntegerDigits(
        /* [out] */ Int32* number);

    static CARAPI GetNumberInstance(
        /* [out] */ INumberFormat** instance);

    static CARAPI GetNumberInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

    static CARAPI GetPercentInstance(
        /* [out] */ INumberFormat** instance);

    static CARAPI GetPercentInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

//    @Override
//    public int hashCode();

    virtual CARAPI IsGroupingUsed(
        /* [out] */ Boolean* isGroupingUsed);

    virtual CARAPI IsParseIntegerOnly(
        /* [out] */ Boolean* isParseIntegerOnly);

    virtual CARAPI Parse(
        /* [in] */ const String& string,
        /* [out] */ INumber** value);

    virtual CARAPI Parse(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** value) = 0;

    //@Override
    CARAPI ParseObject(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IInterface** object);

    virtual CARAPI SetCurrency(
        /* [in] */ ICurrency* currency);

    virtual CARAPI SetGroupingUsed(
        /* [in] */ Boolean value);

    virtual CARAPI SetMaximumFractionDigits(
        /* [in] */ Int32 value);

    virtual CARAPI SetMaximumIntegerDigits(
        /* [in] */ Int32 value);

    virtual CARAPI SetMinimumFractionDigits(
        /* [in] */ Int32 value);

    virtual CARAPI SetMinimumIntegerDigits(
        /* [in] */ Int32 value);

    virtual CARAPI SetParseIntegerOnly(
        /* [in] */ Boolean value);

    virtual CARAPI GetRoundingMode(
        /* [out] */ RoundingMode* roundingMode);

    virtual CARAPI SetRoundingMode(
        /* [in] */ RoundingMode roundingMode);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

protected:
    NumberFormat();

private:
    static CARAPI GetInstance(
        /* [in] */ const String& pattern,
        /* [in] */ ILocale* locale,
        /* [out] */ INumberFormat** instance);

//    private static final ObjectStreamField[] serialPersistentFields = {
//            new ObjectStreamField("groupingUsed", Boolean.class),
//            new ObjectStreamField("maxFractionDigits", Byte.class),
//            new ObjectStreamField("maximumFractionDigits", Integer.class),
//            new ObjectStreamField("maximumIntegerDigits", Integer.class),
//            new ObjectStreamField("maxIntegerDigits", Byte.class),
//            new ObjectStreamField("minFractionDigits", Byte.class),
//            new ObjectStreamField("minimumFractionDigits", Integer.class),
//            new ObjectStreamField("minimumIntegerDigits", Integer.class),
//            new ObjectStreamField("minIntegerDigits", Byte.class),
//            new ObjectStreamField("parseIntegerOnly", Boolean.class),
//            new ObjectStreamField("serialVersionOnStream", Integer.class),
//    };

//    private void writeObject(ObjectOutputStream stream);

//    private void readObject(ObjectInputStream stream);

private:
    Boolean mGroupingUsed;
    Boolean mParseIntegerOnly;

    Int32 mMaximumIntegerDigits;
    Int32 mMinimumIntegerDigits;
    Int32 mMaximumFractionDigits;
    Int32 mMinimumFractionDigits;
};

} // namespace Text
} // namespace Elastos

#endif //__ELASTOS_TEXT_NUMBERFORMAT_H__
