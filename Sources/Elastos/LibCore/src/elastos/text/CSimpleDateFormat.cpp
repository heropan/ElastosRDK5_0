
#include "CSimpleDateFormat.h"

namespace Elastos {
namespace Text {

DATEFORMAT_METHODS_IMPL(CSimpleDateFormat,DateFormat)

ECode CSimpleDateFormat::FormatObject(
    /* [in] */ IInterface* object,
    /* [in] */ IStringBuffer * buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ IStringBuffer ** value)
{
    return SimpleDateFormat::FormatObject(object,buffer,field,value);
}

ECode CSimpleDateFormat::ParseObject(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ IInterface** object)
{
    return SimpleDateFormat::ParseObject(string,position,object);
}

ECode CSimpleDateFormat::FormatDate(
    /* [in] */ IDate* date,
    /* [in] */ IStringBuffer * buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ IStringBuffer ** result)
{
    return SimpleDateFormat::FormatDate(date,buffer,field,result);
}

ECode CSimpleDateFormat::Parse(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ IDate** date)
{
    return SimpleDateFormat::Parse(string,position,date);
}

ECode CSimpleDateFormat::ApplyLocalizedPattern(
    /* [in] */ const String& tem)
{
    return SimpleDateFormat::ApplyLocalizedPattern(tem);
}

ECode CSimpleDateFormat::ApplyPattern(
    /* [in] */ const String& tem)
{
    return SimpleDateFormat::ApplyPattern(tem);
}

ECode CSimpleDateFormat::Get2DigitYearStart(
    /* [out] */ IDate** date)
{
    return SimpleDateFormat::Get2DigitYearStart(date);
}

ECode CSimpleDateFormat::GetDateFormatSymbols(
    /* [out] */ IDateFormatSymbols** dateFormatSymbols)
{
    return SimpleDateFormat::GetDateFormatSymbols(dateFormatSymbols);
}

ECode CSimpleDateFormat::Set2DigitYearStart(
    /* [in] */ IDate* date)
{
    return SimpleDateFormat::Set2DigitYearStart(date);
}

ECode CSimpleDateFormat::SetDateFormatSymbols(
    /* [in] */ IDateFormatSymbols* value)
{
    return SimpleDateFormat::SetDateFormatSymbols(value);
}

ECode CSimpleDateFormat::ToLocalizedPattern(
    /* [out] */ String* pattern)
{
    return SimpleDateFormat::ToLocalizedPattern(pattern);
}

ECode CSimpleDateFormat::ToPattern(
    /* [out] */ String* pattern)
{
    return SimpleDateFormat::ToPattern(pattern);
}

ECode CSimpleDateFormat::constructor()
{
    return SimpleDateFormat::Init();
}

ECode CSimpleDateFormat::constructor(
    /* [in] */ const String& pattern)
{
    return SimpleDateFormat::Init(pattern);
}

ECode CSimpleDateFormat::constructor(
    /* [in] */ const String& temp,
    /* [in] */ IDateFormatSymbols* value)
{
    return SimpleDateFormat::Init(temp,value);
}

ECode CSimpleDateFormat::constructor(
    /* [in] */ const String& temp,
    /* [in] */ ILocale* locale)
{
    return SimpleDateFormat::Init(temp,locale);
}

PInterface CSimpleDateFormat::Probe(
    /* [in] */ REIID riid)
{
    return _CSimpleDateFormat::Probe(riid);
}

ECode CSimpleDateFormat::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return SimpleDateFormat::Equals(object, result);
}

} // namespace Text
} // namespace Elastos
