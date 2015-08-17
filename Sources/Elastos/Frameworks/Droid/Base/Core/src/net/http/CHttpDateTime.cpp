
#include "CHttpDateTime.h"
// #include "Pattern.h"
// #include "Matcher.h"
#include <elastos/core/Character.h>
#include "ext/frameworkext.h"

using namespace Elastos::Core;

using Elastos::Core::ICharSequence;
using Elastos::Core::ICalendar;
// using Elastos::Utility::Regex::IPattern;
using Elastos::Utility::Regex::IMatcher;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Internal {
namespace Http {

const String CHttpDateTime::HTTP_DATE_RFC_REGEXP =
    String("([0-9]{1,2})[- ]([A-Za-z]{3,9})[- ]([0-9]{2,4})[ ]")
    + String("([0-9]{1,2}:[0-9][0-9]:[0-9][0-9])");

const String CHttpDateTime::HTTP_DATE_ANSIC_REGEXP =
    String("[ ]([A-Za-z]{3,9})[ ]+([0-9]{1,2})[ ]")
    + String("([0-9]{1,2}:[0-9][0-9]:[0-9][0-9])[ ]([0-9]{2,4})");

const AutoPtr<IPattern> CHttpDateTime::HTTP_DATE_RFC_PATTERN;
const AutoPtr<IPattern> CHttpDateTime::HTTP_DATE_ANSIC_PATTERN;
void CHttpDateTime::InitStaticHttpDatePattern()
{
    // TODO:
    // Pattern::Compile(HTTP_DATE_RFC_REGEXP, (IPattern**)&HTTP_DATE_RFC_PATTERN);
    // Pattern::Compile(HTTP_DATE_ANSIC_REGEXP, (IPattern**)&HTTP_DATE_ANSIC_PATTERN);
}

CHttpDateTime::TimeOfDay::TimeOfDay(
    /* [in] */ Int32 h,
    /* [in] */  Int32 m,
    /* [in] */  Int32 s)
    : mHour(h)
    , mMinute(m)
    , mSecond(s)
{}

ECode CHttpDateTime::Parse(
    /* [in] */ const String& timeString,
    /* [out] */ Int64* time)
{
    Int32 date = 1;
    Int32 month = ICalendar::JANUARY;
    Int32 year = 1970;
    AutoPtr<TimeOfDay> timeOfDay;

    AutoPtr<IMatcher> rfcMatcher;
    AutoPtr<ICharSequence> cTimesString;
    CString::New(timeString, (ICharSequence**)&cTimesString);
    HTTP_DATE_RFC_PATTERN->Matcher(cTimesString, (IMatcher**)&rfcMatcher);
    Boolean isFind;
    rfcMatcher->Find(&isFind);
    if (isFind) {
        String sD;
        rfcMatcher->Group(1, &sD);
        date = GetDate(sD);

        String sM;
        rfcMatcher->Group(2, &sM);
        FAIL_RETURN(GetMonth(sM, &month));

        String sY;
        rfcMatcher->Group(3, &sY);
        year = GetYear(sY);

        String sT;
        rfcMatcher->Group(4, &sT);
        timeOfDay = GetTime(sT);
    } else {
        AutoPtr<IMatcher> ansicMatcher;
        HTTP_DATE_ANSIC_PATTERN->Matcher(cTimesString, (IMatcher**)&ansicMatcher);
        Boolean isFind;
        ansicMatcher->Find(&isFind);
        if (isFind) {
            String sM;
            ansicMatcher->Group(1, &sM);
            FAIL_RETURN(GetMonth(sM, &month));

            String sD;
            ansicMatcher->Group(2, &sD);
            date = GetDate(sD);

            String sT;
            ansicMatcher->Group(3, &sT);
            timeOfDay = GetTime(sT);

            String sY;
            ansicMatcher->Group(4, &sY);
            year = GetYear(sY);
        } else {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    // FIXME: Y2038 BUG!
    if (year >= 2038) {
        year = 2038;
        month = ICalendar::JANUARY;
        date = 1;
    }

    // TODO:
    // AutoPtr<ITime> itime;
    // CTime(ITime::TIMEZONE_UTC, (ITime**)&itime);
    // itime->Set(timeOfDay->mSecond, timeOfDay->mMinute, timeOfDay->mHour,
    //     date, month, year);

    // *time = itime->ToMillis(FALSE /* use isDst */);
    return NOERROR;
}

Int32 CHttpDateTime::GetDate(
    /* [in] */ const String& dateString)
{
    if (dateString.GetLength() == 2) {
        return (dateString.GetChar(0) - '0') * 10
                + (dateString.GetChar(1) - '0');
    }

    return (dateString.GetChar(0) - '0');
}

ECode CHttpDateTime::GetMonth(
    /* [in] */ const String& monthString,
    /* [out] */ Int32* month)
{
    VALIDATE_NOT_NULL(month);

    Int32 hash = Character::ToLowerCase(monthString.GetChar(0)) +
            Character::ToLowerCase(monthString.GetChar(1)) +
            Character::ToLowerCase(monthString.GetChar(2)) - 3 * 'a';
    switch (hash) {
        case 22:
            return ICalendar::JANUARY;
        case 10:
            return ICalendar::FEBRUARY;
        case 29:
            return ICalendar::MARCH;
        case 32:
            return ICalendar::APRIL;
        case 36:
            return ICalendar::MAY;
        case 42:
            return ICalendar::JUNE;
        case 40:
            return ICalendar::JULY;
        case 26:
            return ICalendar::AUGUST;
        case 37:
            return ICalendar::SEPTEMBER;
        case 35:
            return ICalendar::OCTOBER;
        case 48:
            return ICalendar::NOVEMBER;
        case 9:
            return ICalendar::DECEMBER;
        default:
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

Int32 CHttpDateTime::GetYear(
    /* [in] */ const String& yearString)
{
    if (yearString.GetLength() == 2) {
        Int32 year = (yearString.GetChar(0) - '0') * 10
                + (yearString.GetChar(1) - '0');
        if (year >= 70) {
            return year + 1900;
        } else {
            return year + 2000;
        }
    } else if (yearString.GetLength() == 3) {
        // According to RFC 2822, three digit years should be added to 1900.
        Int32 year = (yearString.GetChar(0) - '0') * 100
                + (yearString.GetChar(1) - '0') * 10
                + (yearString.GetChar(2) - '0');
        return year + 1900;
    } else if (yearString.GetLength() == 4) {
         return (yearString.GetChar(0) - '0') * 1000
                + (yearString.GetChar(1) - '0') * 100
                + (yearString.GetChar(2) - '0') * 10
                + (yearString.GetChar(3) - '0');
    }

    return 1970;
}

AutoPtr<CHttpDateTime::TimeOfDay> CHttpDateTime::GetTime(
    /* [in] */ const String& timeString)
{
    // HH might be H
    Int32 i = 0;
    Int32 hour = timeString.GetChar(i++) - '0';
    if (timeString.GetChar(i) != ':')
        hour = hour * 10 + (timeString.GetChar(i++) - '0');
    // Skip ':'
    i++;

    Int32 minute = (timeString.GetChar(i++) - '0') * 10
                + (timeString.GetChar(i++) - '0');
    // Skip ':'
    i++;

    Int32 second = (timeString.GetChar(i++) - '0') * 10
              + (timeString.GetChar(i++) - '0');

    AutoPtr<TimeOfDay> td = new TimeOfDay(hour, minute, second);
    return td;
}

}
}
}
}
}

