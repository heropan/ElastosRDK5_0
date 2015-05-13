
#include <cmdef.h>
#include "CTimestamp.h"
#include <elastos/StringUtils.h>
#include "CPatternHelper.h"
#include "CParsePosition.h"

using Elastos::Core::StringUtils;
using Elastos::Utility::Regex::IPatternHelper;
using Elastos::Utility::Regex::CPatternHelper;
// using Elastos::Text::ISimpleDateFormat;
// using Elastos::Text::CSimpleDateFormat;
using Elastos::Text::IParsePosition;
using Elastos::Text::CParsePosition;

namespace Elastos {
namespace Sql {

const String CTimestamp::TIME_FORMAT_REGEX = String("[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}.*");

const String CTimestamp::PADDING = String("000000000");

SQLTIMESTAMP_METHODS_IMPL(CTimestamp, Date)

ECode CTimestamp::CompareTo(
    /* [in] */ Elastos::Utility::IDate* date,
    /* [out] */ Int32* result)
{
    CompareToEx((ITimestamp *)date , result);
    return NOERROR;
}

ECode CTimestamp::GetTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time)

    Int64 theTime = Date::GetTime();
    theTime = theTime + (nanos / 1000000);
    *time = theTime;
    return NOERROR;
}

ECode CTimestamp::SetTime(
    /* [in] */ Int64 milliseconds)
{
    SetTimeImpl(milliseconds);
    return NOERROR;
}

ECode CTimestamp::ToString(
    /* [out] */ String* localeStr)
{
    AutoPtr<StringBuilder> sb = new StringBuilder(29);

    Format((Date::GetYear() + 1900), 4, sb);
    sb->AppendChar('-');
    Format((Date::GetMonth() + 1), 2, sb);
    sb->AppendChar('-');
    Format(Date::GetDate(), 2, sb);
    sb->AppendChar(' ');
    Format(Date::GetHours(), 2, sb);
    sb->AppendChar(':');
    Format(Date::GetMinutes(), 2, sb);
    sb->AppendChar(':');
    Format(Date::GetSeconds(), 2, sb);
    sb->AppendChar('.');
    if (nanos == 0) {
        sb->AppendChar('0');
    } else {
        Format(nanos, 9, sb);
        while (sb->GetChar(sb->GetLength() - 1) == '0') {
            // sb->SetLength(sb->GetLength() - 1);
        }
    }

    sb->ToString(localeStr);
    return NOERROR;
}

ECode CTimestamp::After(
    /* [in] */ ITimestamp * theTimestamp,
    /* [out] */ Boolean * value)
{
    Int64 thisTime = 0;
    GetTime(&thisTime);
    Int64 compareTime = 0;
    theTimestamp->GetTime(&compareTime);

    Int32 thisNano = 0;
    GetNanos(&thisNano);
    Int32 comparenano = 0;
    theTimestamp->GetNanos(&comparenano);

    // If the time value is later, the timestamp is later
    if (thisTime > compareTime) {
        *value = TRUE;
    }
    // If the time value is earlier, the timestamp is not later
    else if (thisTime < compareTime) {
        *value = FALSE;
    }
    /*
     * Otherwise the time values are equal in which case the nanoseconds
     * value determines whether this timestamp is later...
     */
    else if (thisNano > comparenano) {
        *value = TRUE;
    } else {
        *value = FALSE;
    }
    return NOERROR;
}

ECode CTimestamp::Before(
    /* [in] */ ITimestamp * theTimestamp,
    /* [out] */ Boolean * value)
{
    Int64 thisTime = 0;
    GetTime(&thisTime);
    Int64 compareTime = 0;
    theTimestamp->GetTime(&compareTime);

    Int32 thisNano = 0;
    GetNanos(&thisNano);
    Int32 comparenano = 0;
    theTimestamp->GetNanos(&comparenano);

    // If the time value is later, the timestamp is later
    if (thisTime < compareTime) {
        *value = TRUE;
    }
    // If the time value is earlier, the timestamp is not later
    else if (thisTime > compareTime) {
        *value = FALSE;
    }
    /*
     * Otherwise the time values are equal in which case the nanoseconds
     * value determines whether this timestamp is later...
     */
    else if (thisNano < comparenano) {
        *value = TRUE;
    } else {
        *value = FALSE;
    }
    return NOERROR;
}

ECode CTimestamp::CompareToEx(
    /* [in] */ ITimestamp * theTimestamp,
    /* [out] */ Int32 * value)
{
    Int32 result = Date::CompareTo(theTimestamp);
    if (result == 0) {
        Int32 thisNano = 0;
        GetNanos(&thisNano);
        Int32 thatNano = 0;
        theTimestamp->GetNanos(&thatNano);
        if (thisNano > thatNano) {
            *value = 1;
        } else if (thisNano == thatNano) {
            *value = 0;
        } else {
            *value = -1;
        }
    }
    *value = result;
    return NOERROR;
}

ECode CTimestamp::Equals(
    /* [in] */ IInterface * pTheObject,
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(value);
    *value = FALSE;
    VALIDATE_NOT_NULL(pTheObject);

    if (ITimestamp::Probe(pTheObject) == NULL) return NOERROR;

    return EqualsEx(ITimestamp::Probe(pTheObject), value);
}

ECode CTimestamp::EqualsEx(
    /* [in] */ ITimestamp * theTimestamp,
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(value);
    *value = FALSE;
    VALIDATE_NOT_NULL(theTimestamp);

    Int64 atime = 0;
    Int64 btime = 0;
    Int32 ananos = 0;
    Int32 bnanos = 0;
    GetTime(&atime);
    theTimestamp->GetTime(&btime);
    GetNanos(&ananos);
    theTimestamp->GetNanos(&bnanos);
    *value = (atime == btime) && (ananos == bnanos);
    return NOERROR;
}


ECode CTimestamp::GetNanos(
    /* [out] */ Int32 * pNano)
{
    *pNano = nanos;
    return NOERROR;
}

ECode CTimestamp::SetNanos(
    /* [in] */ Int32 n)
{
    if ((n < 0) || (n > 999999999)) {
        return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    nanos = n;
    return NOERROR;
}

ECode CTimestamp::constructor(
    /* [in] */ Int32 theYear,
    /* [in] */ Int32 theMonth,
    /* [in] */ Int32 theDate,
    /* [in] */ Int32 theHour,
    /* [in] */ Int32 theMinute,
    /* [in] */ Int32 theSecond,
    /* [in] */ Int32 theNano)
{
    InitEx2(theYear, theMonth, theDate, theHour, theMinute, theSecond);
    if (theNano < 0 || theNano > 999999999) {
        return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    nanos = theNano;
    return NOERROR;
}

ECode CTimestamp::constructor(
    /* [in] */ Int64 theTime)
{
    InitEx(theTime);
    /*
     * Now set the time for this Timestamp object - which deals with the
     * nanosecond value as well as the base time
     */
    SetTimeImpl(theTime);
    return NOERROR;
}

AutoPtr<ITimestamp> CTimestamp::ValueOf(const String& str)
{
    if (str.IsNull()) {
        return NULL;
    }

    // omit trailing whitespace
    String s = str.Trim();
    AutoPtr<IPatternHelper> pat;
    CPatternHelper::AcquireSingleton((IPatternHelper **)&pat);
    Boolean ispat = FALSE;
    pat->Matches(TIME_FORMAT_REGEX, s, &ispat);
    if (!ispat) {
        return NULL;
    }

    // AutoPtr<ISimpleDateFormat> df;
    // CSimpleDateFormat::New(String("yyyy-MM-dd HH:mm:ss"), (ISimpleDateFormat **)&df);
    // AutoPtr<IParsePosition> pp;
    // CParsePosition::New(0, (IParsePosition **)&pp);

    /*
     * First parse out the yyyy-MM-dd HH:mm:ss component of the String into
     * a Date object using the SimpleDateFormat. This should stop after the
     * seconds value, according to the definition of SimpleDateFormat.parse,
     * with the ParsePosition indicating the index of the "." which should
     * precede the nanoseconds value
     */
    // AutoPtr<IDate> theDate;

    // ECode ec = df->ParseEx(s, pp,(IDate **)&theDate);
    // if (ec != NOERROR ) {
    //     return NULL;
    // }

    // if (theDate == NULL) {
    //     return NULL;
    // }

    /*
     * If we get here, the Date part of the string was OK - now for the
     * nanoseconds value. Strictly, this requires the remaining part of the
     * String to look like ".nnnnnnnnn". However, we accept anything with a
     * '.' followed by 1 to 9 digits - we also accept nothing (no fractions
     * of a second). Anything else is interpreted as incorrect format which
     * will generate an IllegalArgumentException
     */
    Int32 position = 0;
    // pp->GetIndex(&position);
    Int32 remaining = s.GetLength() - position;
    Int32 theNanos = 0;

    if (remaining == 0) {
        // First, allow for the case where no fraction of a second is given:
        theNanos = 0;
    } else {
        /*
         * Case where fraction of a second is specified: Require 1 character
         * plus the "." in the remaining part of the string...
         */
        if ((s.GetLength() - position) < String(".n").GetLength()) {
            return NULL;
        }

        /*
         * If we're strict, we should not allow any EXTRA characters after
         * the 9 digits
         */
        if ((s.GetLength() - position) > String(".nnnnnnnnn").GetLength()) {
            return NULL;
        }

        // Require the next character to be a "."
        if (s.GetChar(position) != '.') {
            return NULL;
        }
        // Get the length of the number string - need to account for the '.'
        Int32 nanoLength = s.GetLength() - position - 1;

        // Get the 9 characters following the "." as an integer
        String theNanoString = s.Substring(position + 1, position + 1 + nanoLength);
        /*
         * We must adjust for the cases where the nanos String was not 9
         * characters long by padding out with zeros
         */
        theNanoString = theNanoString + String("000000000");
        theNanoString = theNanoString.Substring(0, 9);

        theNanos = StringUtils::ParseInt32(theNanoString);
    }

    if (theNanos < 0 || theNanos > 999999999) {
        return NULL;
    }

    AutoPtr<ITimestamp> theTimestamp;
    Int64 outtime = 0;
    // theDate->GetTime(&outtime);
    CTimestamp::New(outtime, (ITimestamp **)&theTimestamp);
    theTimestamp->SetNanos(theNanos);

    return theTimestamp;
}

ECode CTimestamp::BadTimestampString(
    /* [in] */ const String& s)
{
    return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
}

void CTimestamp::Format(
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

void CTimestamp::SetTimeImpl(
    /* [in] */ Int64 theTime)
{
    Int32 milliseconds = (Int32) (theTime % 1000);
    theTime = theTime - milliseconds;
    if (milliseconds < 0) {
        theTime = theTime - 1000;
        milliseconds = 1000 + milliseconds;
    }
    Date::SetTime(theTime);
    SetNanos(milliseconds * 1000000);
}

} // namespace Sql
} // namespace Elastos
