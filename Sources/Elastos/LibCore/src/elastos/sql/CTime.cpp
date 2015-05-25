
#include "CTime.h"
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Sql {

const String CTime::PADDING = String("00");

SQLTIME_METHODS_IMPL(CTime, Date)

ECode CTime::GetDate(
    /* [out] */ Int32 * pDate)
{
    return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CTime::GetDay(
    /* [out] */ Int32 * pDay)
{
    return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CTime::GetMonth(
    /* [out] */ Int32 * pMonth)
{
    return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CTime::GetYear(
    /* [out] */ Int32 * pYear)
{
    return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CTime::SetDate(
    /* [in] */ Int32 day)
{
    return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CTime::SetMonth(
    /* [in] */ Int32 month)
{
    return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CTime::SetTime(
    /* [in] */ Int64 milliseconds)
{
    Date::SetTime(milliseconds);
    return NOERROR;
}

ECode CTime::SetYear(
    /* [in] */ Int32 year)
{
    return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CTime::ToString(
    /* [out] */ String * pLocaleStr)
{
    AutoPtr<StringBuilder> sb = new StringBuilder(8);

    Format(Date::GetHours(), 2, sb);
    sb->AppendChar(':');
    Format(Date::GetMinutes(), 2, sb);
    sb->AppendChar(':');
    Format(Date::GetSeconds(), 2, sb);

    sb->ToString(pLocaleStr);
    return NOERROR;
}

ECode CTime::constructor(
    /* [in] */ Int32 theHour,
    /* [in] */ Int32 theMinute,
    /* [in] */ Int32 theSecond)
{
    Init(70, 0, 1, theHour, theMinute, theSecond);
    return NOERROR;
}

ECode CTime::constructor(
    /* [in] */ Int64 theTime)
{
    Init(theTime);
    return NOERROR;
}

ITime * CTime::ValueOf(const String& timeString)
{
    if (timeString.IsNull()) {
            return NULL;
    }
    Int32 firstIndex = timeString.IndexOf(':');
    Int32 secondIndex = timeString.IndexOf(':', firstIndex + 1);
    // secondIndex == -1 means none or only one separator '-' has been
    // found.
    // The string is separated into three parts by two separator characters,
    // if the first or the third part is null string, we should throw
    // IllegalArgumentException to follow RI
    if (secondIndex == -1 || firstIndex == 0
        || secondIndex + 1 == timeString.GetLength()) {
        return NULL;
    }
    // parse each part of the string
    Int32 hour = StringUtils::ParseInt32(timeString.Substring(0, firstIndex));
    Int32 minute = StringUtils::ParseInt32(timeString.Substring(firstIndex + 1, secondIndex));
    Int32 second = StringUtils::ParseInt32(timeString.Substring(secondIndex + 1, timeString.GetLength()));
    AutoPtr<ITime> theTime;
    CTime::New(hour, minute, second, (ITime **)&theTime);
    return theTime;

}

void CTime::Format(
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
