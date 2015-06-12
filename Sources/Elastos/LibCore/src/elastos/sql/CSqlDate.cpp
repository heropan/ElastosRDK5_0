
#include "CSqlDate.h"
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Sql {

const String CSqlDate::PADDING = String("0000");

CAR_INTERFACE_IMPL(CSqlDate, Date, ISQLDate);
CAR_OBJECT_IMPL(CSqlDate);

ECode CSqlDate::GetHours(
    /* [out] */ Int32 * value)
{
    return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CSqlDate::GetMinutes(
    /* [out] */ Int32 * value)
{
    return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CSqlDate::GetSeconds(
    /* [out] */ Int32 * value)
{
    return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CSqlDate::SetHours(
    /* [in] */ Int32 hour)
{
    return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CSqlDate::SetMinutes(
    /* [in] */ Int32 minute)
{
    return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CSqlDate::SetSeconds(
    /* [in] */ Int32 second)
{
    return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CSqlDate::SetTime(
    /* [in] */ Int64 milliseconds)
{
    return Date::SetTime(NormalizeTime(milliseconds));
}

ECode CSqlDate::ToString(
    /* [out] */ String * str)
{
    AutoPtr<StringBuilder> sb = new StringBuilder(10);

    Int32 value = 0;
    Date::GetYear(&value);
    Format(value + 1900, 4, sb);
    sb->AppendChar('-');

    Date::GetMonth(&value);
    Format(value + 1, 2, sb);
    sb->AppendChar('-');

    Date::GetDate(&value);
    Format(value, 2, sb);
    sb->ToString(str);
    return NOERROR;
}

ECode CSqlDate::constructor(
    /* [in] */ Int32 theYear,
    /* [in] */ Int32 theMonth,
    /* [in] */ Int32 theDay)
{
    Date::constructor(theYear, theMonth, theDay);
    return NOERROR;
}

ECode CSqlDate::constructor(
    /* [in] */ Int64 theDate)
{
    Date::constructor(NormalizeTime(theDate));
    return NOERROR;
}

AutoPtr<ISQLDate> CSqlDate::ValueOf(
    /* [in] */ const String& dateString)
{
    if (dateString == NULL) {
        // throw new IllegalArgumentException("dateString == null");
        // return E_ILLEGAL_ARGUMENT_EXCEPTION;
        return NULL;
    }

    if (dateString.GetLength() > 10) {
        // early fail to avoid parsing huge invalid strings
        // throw new IllegalArgumentException();
        // return E_ILLEGAL_ARGUMENT_EXCEPTION;
        return NULL;
    }

    AutoPtr<ArrayOf<String> > parts;
    StringUtils::Split(dateString, "-", (ArrayOf<String>**)&parts);
    if (parts->GetLength() != 3) {
        // throw new IllegalArgumentException();
        // return E_ILLEGAL_ARGUMENT_EXCEPTION;
        return NULL;
    }

    Int64 value = 0;
    StringUtils::ParsePositiveInt64((*parts)[0], &value);
    Int32 year = value;
    StringUtils::ParsePositiveInt64((*parts)[1], &value);
    Int32 month = value;
    StringUtils::ParsePositiveInt64((*parts)[2], &value);
    Int32 day = value;

    AutoPtr<ISQLDate> date;
    CSqlDate::New(year - 1900, month - 1, day, (ISQLDate**)&date);
    return date;
}

Int64 CSqlDate::NormalizeTime(
    /* [in] */ Int64 theTime)
{
    return theTime;
}

void CSqlDate::Format(
    /* [in] */ Int32 date,
    /* [in] */ Int32 digits,
    /* [in] */ AutoPtr<StringBuilder> sb)
{
    String str = StringUtils::ToString(date);
    if (digits - str.GetLength() > 0) {
        sb->Append(PADDING.Substring(0, digits - str.GetLength()));
    }
    sb->Append(str);
}

} // namespace Sql
} // namespace Elastos
