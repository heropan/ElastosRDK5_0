
#include "CSqlDate.h"
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Sql {

const String CSqlDate::PADDING = String("0000");

SQLDATE_METHODS_IMPL(CSqlDate, Date)

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
    Date::SetTime(NormalizeTime(milliseconds));
    return NOERROR;
}

ECode CSqlDate::ToString(
    /* [out] */ String * str)
{
    AutoPtr<StringBuilder> sb = new StringBuilder(10);

    Format((Date::GetYear() + 1900), 4, sb);
    sb->AppendChar('-');
    Format((Date::GetMonth() + 1), 2, sb);
    sb->AppendChar('-');
    Format(Date::GetDate(), 2, sb);
    sb->ToString(str);
    return NOERROR;
}

ECode CSqlDate::constructor(
    /* [in] */ Int32 theYear,
    /* [in] */ Int32 theMonth,
    /* [in] */ Int32 theDay)
{
    Init(theYear, theMonth, theDay);
    return NOERROR;
}

ECode CSqlDate::constructor(
    /* [in] */ Int64 theDate)
{
    Init(NormalizeTime(theDate));
    return NOERROR;
}

AutoPtr<IDate> CSqlDate::ValueOf(
    /* [in] */ const String& dateString)
{
    if (dateString.IsNull()) {
        return NULL;
    }
    Int32 firstIndex = dateString.IndexOf('-');
    Int32 secondIndex = dateString.IndexOf('-', firstIndex + 1);
    // secondIndex == -1 means none or only one separator '-' has been
    // found.
    // The string is separated into three parts by two separator characters,
    // if the first or the third part is null string, we should throw
    // IllegalArgumentException to follow RI
    if (secondIndex == -1 || firstIndex == 0
            || secondIndex + 1 == dateString.GetLength()) {
        return NULL;
    }
    // parse each part of the string
    Int32 year = StringUtils::ParseInt32(dateString.Substring(0, firstIndex));
    Int32 month = StringUtils::ParseInt32(dateString.Substring(firstIndex + 1, secondIndex));
    Int32 day = StringUtils::ParseInt32(dateString.Substring(secondIndex + 1, dateString.GetLength()));
    AutoPtr<IDate> thedate;
    CSqlDate::New(year - 1900, month - 1, day, (IDate **)&thedate);
    return thedate;
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
    String str = StringUtils::Int32ToString(date);
    if (digits - str.GetLength() > 0) {
        sb->AppendString(PADDING.Substring(0, digits - str.GetLength()));
    }
    sb->AppendString(str);
}

} // namespace Sql
} // namespace Elastos
