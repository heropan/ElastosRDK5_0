
#include "text/format/CDateUtils.h"
#include "text/format/DateUtils.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Format {

ECode CDateUtils::Assign(
    /* [in] */ ICalendar* rval,
    /* [in] */ ICalendar* lval)
{
    DateUtils::Assign(rval, lval);
    return NOERROR;
}

ECode CDateUtils::FormatDateRange(
    /* [in] */ IContext* context,
    /* [in] */ Int64 startMillis,
    /* [in] */ Int64 endMillis,
    /* [in] */ Int32 flags,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = DateUtils::FormatDateRange(context, startMillis, endMillis, flags);
    return NOERROR;
}

ECode CDateUtils::FormatDateRangeEx(
    /* [in] */ IContext* context,
    /* [in] */ /*Elastos::Utility::IFormatter*/IInterface* formatter,
    /* [in] */ Int64 startMillis,
    /* [in] */ Int64 endMillis,
    /* [in] */ Int32 flags,
    /* [out] */ /*Elastos::Utility::IFormatter*/IInterface** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IInterface> formatterRet = DateUtils::FormatDateRange(context, formatter, startMillis, endMillis, flags);
    *ret = formatterRet;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CDateUtils::FormatDateRangeEx2(
    /* [in] */ IContext* context,
    /* [in] */ /*Elastos::Utility::IFormatter*/IInterface* formatter,
    /* [in] */ Int64 startMillis,
    /* [in] */ Int64 endMillis,
    /* [in] */ Int32 flags,
    /* [in] */ const String& timeZone,
    /* [out] */ /*Elastos::Utility::IFormatter*/IInterface** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IInterface> formatterRet = DateUtils::FormatDateRange(context, formatter, startMillis, endMillis, flags, timeZone);
    *ret = formatterRet;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CDateUtils::FormatDuration(
    /* [in] */ Int64 millis,
    /* [out] */ ICharSequence** rst)
{
    VALIDATE_NOT_NULL(rst);
    AutoPtr<ICharSequence> temp = DateUtils::FormatDuration(millis);
    *rst = temp;
    INTERFACE_ADDREF(*rst);
    return NOERROR;
}

ECode CDateUtils::FormatDateTime(
    /* [in] */ IContext* context,
    /* [in] */ Int64 millis,
    /* [in] */ Int32 flags,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=(DateUtils::FormatDateTime(context, millis, flags));
    return NOERROR;
}

ECode CDateUtils::FormatElapsedTime(
    /* [in] */ Int64 elapsedSeconds,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=(DateUtils::FormatElapsedTime(elapsedSeconds));
    return NOERROR;
}

ECode CDateUtils::FormatElapsedTimeEx(
    /* [in] */ const String& recycle,
    /* [in] */ Int64 elapsedSeconds,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=(DateUtils::FormatElapsedTime(recycle, elapsedSeconds));
    return NOERROR;
}

ECode CDateUtils::FormatSameDayTime(
    /* [in] */ Int64 then,
    /* [in] */ Int64 now,
    /* [in] */ Int32 dateStyle,
    /* [in] */ Int32 timeStyle,
    /* [out] */ ICharSequence** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ICharSequence> cRet = DateUtils::FormatSameDayTime(then, now, dateStyle, timeStyle);
    *ret = cRet;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CDateUtils::GetAMPMString(
    /* [in] */ Int32 ampm,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=(DateUtils::GetAMPMString(ampm));
    return NOERROR;
}

ECode CDateUtils::GetDayOfWeekString(
    /* [in] */ Int32 dayOfWeek,
    /* [in] */ Int32 abbrev,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=(DateUtils::GetDayOfWeekString(dayOfWeek, abbrev));
    return NOERROR;
}

ECode CDateUtils::GetMonthString(
    /* [in] */ Int32 month,
    /* [in] */ Int32 abbrev,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=(DateUtils::GetMonthString(month, abbrev));
    return NOERROR;
}

ECode CDateUtils::GetRelativeDateTimeString(
    /* [in] */ IContext* c,
    /* [in] */ Int64 time,
    /* [in] */ Int64 minResolution,
    /* [in] */ Int64 transitionResolution,
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ICharSequence> cRet = DateUtils::GetRelativeDateTimeString(c, time, minResolution, transitionResolution, flags);
    *ret = cRet;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CDateUtils::GetRelativeTimeSpanString(
    /* [in] */ Int64 startTime,
    /* [out] */ ICharSequence** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ICharSequence> cRet = DateUtils::GetRelativeTimeSpanString(startTime);
    *ret = cRet;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CDateUtils::GetRelativeTimeSpanStringEx(
    /* [in] */ Int64 time,
    /* [in] */ Int64 now,
    /* [in] */ Int64 minResolution,
    /* [out] */ ICharSequence** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ICharSequence> cRet = DateUtils::GetRelativeTimeSpanString(time, now, minResolution);
    *ret = cRet;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CDateUtils::GetRelativeTimeSpanStringEx2(
    /* [in] */ Int64 time,
    /* [in] */ Int64 now,
    /* [in] */ Int64 minResolution,
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ICharSequence> cRet = DateUtils::GetRelativeTimeSpanString(time, now, minResolution, flags);
    *ret = cRet;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CDateUtils::GetRelativeTimeSpanStringEx3(
    /* [in] */ IContext* c,
    /* [in] */ Int64 millis,
    /* [in] */ Boolean withPreposition,
    /* [out] */ ICharSequence** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ICharSequence> cRet = DateUtils::GetRelativeTimeSpanString(c, millis, withPreposition);
    *ret = cRet;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CDateUtils::GetRelativeTimeSpanStringEx4(
    /* [in] */ IContext* c,
    /* [in] */ Int64 millis,
    /* [out] */ ICharSequence** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ICharSequence> cRet = DateUtils::GetRelativeTimeSpanString(c, millis);
    *ret = cRet;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CDateUtils::GetSameMonthTable(
    /* [out, callee] */ ArrayOf<Int32>** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr< ArrayOf<Int32> > sameMT = DateUtils::GetSameMonthTable();
    *ret = sameMT;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CDateUtils::GetSameYearTable(
    /* [out, callee] */ ArrayOf<Int32>** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr< ArrayOf<Int32> > sameYT = DateUtils::GetSameYearTable();
    *ret = sameYT;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CDateUtils::GetStandaloneMonthString(
    /* [in] */ Int32 month,
    /* [in] */ Int32 abbrev,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=(DateUtils::GetStandaloneMonthString(month, abbrev));
    return NOERROR;
}

ECode CDateUtils::IsToday(
    /* [in] */ Int64 when,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=(DateUtils::IsToday(when));
    return NOERROR;
}

ECode CDateUtils::IsUTC(
    /* [in] */ const String& s,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=(DateUtils::IsUTC(s));
    return NOERROR;
}

ECode CDateUtils::NewCalendar(
    /* [in] */ Boolean zulu,
    /* [out] */ ICalendar** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ICalendar> cRet = DateUtils::NewCalendar(zulu);
    *ret = cRet;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CDateUtils::TimeString(
    /* [in] */ Int64 millis,
    /* [out] */ ICharSequence** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ICharSequence> cRet = DateUtils::TimeString(millis);
    *ret = cRet;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CDateUtils::WriteDateTime(
    /* [in] */ ICalendar* cal,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=(DateUtils::WriteDateTime(cal));
    return NOERROR;
}

ECode CDateUtils::WriteDateTimeEx(
    /* [in] */ ICalendar* cal,
    /* [in] */ Boolean zulu,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=(DateUtils::WriteDateTime(cal, zulu));
    return NOERROR;
}

ECode CDateUtils::WriteDateTimeEx2(
    /* [in] */ ICalendar* cal,
    /* [in,out] */ String* sb,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=(DateUtils::WriteDateTime(cal, sb));
    return NOERROR;
}

} // namespace Format
} // namespace Text
} // namepsace Droid
} // namespace Elastos

