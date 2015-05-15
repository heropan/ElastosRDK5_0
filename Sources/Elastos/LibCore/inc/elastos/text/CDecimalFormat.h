
#ifndef __CDECIMALFORMAT_H__
#define __CDECIMALFORMAT_H__

#include "_CDecimalFormat.h"
#include <cmdef.h>
#include "NumberFormatMacro.h"
#include "DecimalFormat.h"

using Libcore::ICU::ILocale;
using Elastos::Text::IFieldPosition;
using Elastos::Utility::ICurrency;
using Elastos::Text::IDecimalFormatSymbols;
using Elastos::Text::IParsePosition;

namespace Elastos {
namespace Text {

CarClass(CDecimalFormat) , public DecimalFormat
{
public:
    NUMBERFORMAT_METHODS_DECL()

    CARAPI FormatDoubleEx(
        /* [in] */ Double value,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ IStringBuffer ** result);

    CARAPI FormatInt64Ex(
        /* [in] */ Int64 value,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ IStringBuffer ** result);

    CARAPI ParseEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** value);

    CARAPI ApplyLocalizedPattern(
        /* [in] */ const String& pattern);

    CARAPI ApplyPattern(
        /* [in] */ const String& pattern);

    CARAPI GetDecimalFormatSymbols(
        /* [out] */ IDecimalFormatSymbols** symbols);

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
        /* [out] */ Boolean* shown);

    CARAPI IsParseBigDecimal(
        /* [out] */ Boolean* isBigDecimal);

    CARAPI SetDecimalFormatSymbols(
        /* [in] */ IDecimalFormatSymbols* value);

    CARAPI SetDecimalSeparatorAlwaysShown(
        /* [in] */ Boolean value);

    CARAPI SetGroupingSize(
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
        /* [in] */ Boolean newValue);

    CARAPI ToLocalizedPattern(
        /* [out] */ String* localizedPattern);

    CARAPI ToPattern(
        /* [out] */ String* pattern);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& pattern);

    CARAPI constructor(
        /* [in] */ const String& pattern,
        /* [in] */ IDecimalFormatSymbols* value);

    CARAPI constructor(
        /* [in] */ const String& pattern,
        /* [in] */ ILocale* locale);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI GetNdf(
        /* [out] */ INativeDecimalFormat** ndf);
};

} // namespace Text
} // namespace Elastos

#endif // __CDECIMALFORMAT_H__
