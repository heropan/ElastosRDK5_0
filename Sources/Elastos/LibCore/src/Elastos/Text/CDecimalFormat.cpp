
#include "CDecimalFormat.h"

namespace Elastos {
namespace Text {

NUMBERFORMAT_METHODS_IMPL(CDecimalFormat, DecimalFormat)

ECode CDecimalFormat::FormatDoubleEx(
    /* [in] */ Double value,
    /* [in] */ IStringBuffer *buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ IStringBuffer ** result)
{
    return DecimalFormat::FormatDoubleEx(value,buffer,field,result);
}

ECode CDecimalFormat::FormatInt64Ex(
    /* [in] */ Int64 value,
    /* [in] */ IStringBuffer * buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ IStringBuffer ** result)
{
    return DecimalFormat::FormatInt64Ex(value,buffer,field,result);
}

ECode CDecimalFormat::ParseEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ INumber** value)
{
    return DecimalFormat::ParseEx(string,position,value);
}

ECode CDecimalFormat::ApplyLocalizedPattern(
    /* [in] */ const String& pattern)
{
    return DecimalFormat::ApplyLocalizedPattern(pattern);
}

ECode CDecimalFormat::ApplyPattern(
    /* [in] */ const String& pattern)
{
    return DecimalFormat::ApplyPattern(pattern);
}

ECode CDecimalFormat::GetDecimalFormatSymbols(
    /* [out] */ IDecimalFormatSymbols** symbols)
{
    return DecimalFormat::GetDecimalFormatSymbols(symbols);
}

ECode CDecimalFormat::GetGroupingSize(
    /* [out] */ Int32* size)
{
    return DecimalFormat::GetGroupingSize(size);
}

ECode CDecimalFormat::GetMultiplier(
    /* [out] */ Int32* multiplier)
{
    return DecimalFormat::GetMultiplier(multiplier);
}

ECode CDecimalFormat::GetNegativePrefix(
    /* [out] */ String* prefix)
{
    return DecimalFormat::GetNegativePrefix(prefix);
}

ECode CDecimalFormat::GetNegativeSuffix(
    /* [out] */ String* suffix)
{
    return DecimalFormat::GetNegativeSuffix(suffix);
}

ECode CDecimalFormat::GetPositivePrefix(
    /* [out] */ String* prefix)
{
    return DecimalFormat::GetPositivePrefix(prefix);
}

ECode CDecimalFormat::GetPositiveSuffix(
    /* [out] */ String* suffix)
{
    return DecimalFormat::GetPositiveSuffix(suffix);
}

ECode CDecimalFormat::IsDecimalSeparatorAlwaysShown(
    /* [out] */ Boolean* shown)
{
    return DecimalFormat::IsDecimalSeparatorAlwaysShown(shown);
}

ECode CDecimalFormat::IsParseBigDecimal(
    /* [out] */ Boolean* isBigDecimal)
{
    return DecimalFormat::IsParseBigDecimal(isBigDecimal);
}

ECode CDecimalFormat::SetDecimalFormatSymbols(
    /* [in] */ IDecimalFormatSymbols* value)
{
    return DecimalFormat::SetDecimalFormatSymbols(value);
}

ECode CDecimalFormat::SetDecimalSeparatorAlwaysShown(
    /* [in] */ Boolean value)
{
    return DecimalFormat::SetDecimalSeparatorAlwaysShown(value);
}

ECode CDecimalFormat::SetGroupingSize(
    /* [in] */ Int32 value)
{
    return DecimalFormat::SetGroupingSize(value);
}

ECode CDecimalFormat::SetMultiplier(
    /* [in] */ Int32 value)
{
    return DecimalFormat::SetMultiplier(value);
}

ECode CDecimalFormat::SetNegativePrefix(
    /* [in] */ const String& value)
{
    return DecimalFormat::SetNegativePrefix(value);
}

ECode CDecimalFormat::SetNegativeSuffix(
    /* [in] */ const String& value)
{
    return DecimalFormat::SetNegativeSuffix(value);
}

ECode CDecimalFormat::SetPositivePrefix(
    /* [in] */ const String& value)
{
    return DecimalFormat::SetPositivePrefix(value);
}

ECode CDecimalFormat::SetPositiveSuffix(
    /* [in] */ const String& value)
{
    return DecimalFormat::SetPositiveSuffix(value);
}

ECode CDecimalFormat::SetParseBigDecimal(
    /* [in] */ Boolean newValue)
{
    return DecimalFormat::SetParseBigDecimal(newValue);
}

ECode CDecimalFormat::ToLocalizedPattern(
    /* [out] */ String* localizedPattern)
{
    return DecimalFormat::ToLocalizedPattern(localizedPattern);
}

ECode CDecimalFormat::ToPattern(
    /* [out] */ String* pattern)
{
    return DecimalFormat::ToPattern(pattern);
}

ECode CDecimalFormat::constructor()
{
    return DecimalFormat::Init();
}

ECode CDecimalFormat::constructor(
    /* [in] */ const String& pattern)
{
    return DecimalFormat::Init(pattern);
}

ECode CDecimalFormat::constructor(
    /* [in] */ const String& pattern,
    /* [in] */ IDecimalFormatSymbols* value)
{
    return DecimalFormat::Init(pattern,value);
}

ECode CDecimalFormat::constructor(
    /* [in] */ const String& pattern,
    /* [in] */ ILocale* locale)
{
    return DecimalFormat::Init(pattern,locale);
}

PInterface CDecimalFormat::Probe(
    /* [in] */ REIID riid)
{
    return _CDecimalFormat::Probe(riid);
}

ECode CDecimalFormat::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return DecimalFormat::Equals(object, result);
}

ECode CDecimalFormat::GetNdf(
    /* [out] */ INativeDecimalFormat** ndf)
{
    return DecimalFormat::GetNdf(ndf);
}

} // namespace Text
} // namespace Elastos
