
#include "DateFormat.h"
#include <elastos/StringBuffer.h>
#include "AttributedCharacterIteratorAttribute.h"
#include "CSimpleDateFormat.h"
#include "CDate.h"
#include "CICUHelper.h"
#include "CLocaleHelper.h"
#include "CStringWrapper.h"
#include "CDouble.h"
#include "CLocaleDataHelper.h"
// #include "CLocaleData.h"
#include "CFieldPosition.h"
#include "CParsePosition.h"

using Elastos::Utility::EIID_IDate;
using Elastos::Utility::CDate;
using Elastos::Core::INumber;
using Elastos::Core::EIID_INumber;
using Libcore::ICU::IICUHelper;
// using Libcore::ICU::CICUHelper;
using Elastos::Utility::ILocaleHelper;
using Elastos::Utility::CLocaleHelper;
using Libcore::ICU::ILocaleDataHelper;
using Libcore::ICU::CLocaleDataHelper;
using Libcore::ICU::ILocaleData;
// using Libcore::ICU::CLocaleData;
using Elastos::Text::CFieldPosition;
using Elastos::Text::CParsePosition;

namespace Elastos {
namespace Text {

static AutoPtr<IDateFormatField> sInit(const String& name, Int32 value)
{
    AutoPtr<DateFormat::Field> f  = new DateFormat::Field();
    f->Init(name, value);
    AutoPtr<IDateFormatField> field = (IDateFormatField*)f;
    return field;
}

const AutoPtr<IDateFormatField> DateFormat::Field::ERA
    = sInit(String("era"), ICalendar::ERA);
const AutoPtr<IDateFormatField> DateFormat::Field::YEAR
    = sInit(String("year"), ICalendar::YEAR);
const AutoPtr<IDateFormatField> DateFormat::Field::MONTH
    = sInit(String("month"), ICalendar::MONTH);
const AutoPtr<IDateFormatField> DateFormat::Field::HOUR_OF_DAY0
    = sInit(String("hour of day"), ICalendar::HOUR_OF_DAY);
const AutoPtr<IDateFormatField> DateFormat::Field::HOUR_OF_DAY1
    = sInit(String("hour of day 1"), -1);
const AutoPtr<IDateFormatField> DateFormat::Field::MINUTE
    = sInit(String("minute"), ICalendar::MINUTE);
const AutoPtr<IDateFormatField> DateFormat::Field::SECOND
    = sInit(String("second"), ICalendar::SECOND);
const AutoPtr<IDateFormatField> DateFormat::Field::MILLISECOND
    = sInit(String("millisecond"), ICalendar::MILLISECOND);
const AutoPtr<IDateFormatField> DateFormat::Field::DAY_OF_WEEK
    = sInit(String("day of week"), ICalendar::DAY_OF_WEEK);
const AutoPtr<IDateFormatField> DateFormat::Field::DAY_OF_MONTH
    = sInit(String("day of month"), ICalendar::DAY_OF_MONTH);
const AutoPtr<IDateFormatField> DateFormat::Field::DAY_OF_YEAR
    = sInit(String("day of year"), ICalendar::DAY_OF_YEAR);
const AutoPtr<IDateFormatField> DateFormat::Field::DAY_OF_WEEK_IN_MONTH
    = sInit(String("day of week in month"), ICalendar::DAY_OF_WEEK_IN_MONTH);
const AutoPtr<IDateFormatField> DateFormat::Field::WEEK_OF_YEAR
    = sInit(String("week of year"), ICalendar::WEEK_OF_YEAR);
const AutoPtr<IDateFormatField> DateFormat::Field::WEEK_OF_MONTH
    = sInit(String("week of month"), ICalendar::WEEK_OF_MONTH);
const AutoPtr<IDateFormatField> DateFormat::Field::AM_PM
    = sInit(String("am pm"), ICalendar::AM_PM);
const AutoPtr<IDateFormatField> DateFormat::Field::HOUR0
    = sInit(String("hour"), ICalendar::HOUR);
const AutoPtr<IDateFormatField> DateFormat::Field::HOUR1
    = sInit(String("hour 1"), -1);
const AutoPtr<IDateFormatField> DateFormat::Field::TIME_ZONE
    = sInit(String("time zone"), -1);

HashMap<Int32, AutoPtr<IDateFormatField> > DateFormat::Field::sTable(11);

TEXTATTRIBUITEDCHARACTERITERATORATTRIBUTE_METHODS_IMPL(DateFormat::Field, AttributedCharacterIteratorAttribute)

CAR_INTERFACE_IMPL(DateFormat::Field, IDateFormatField)

DateFormat::Field::Field()
    : mCalendarField(-1)
{ }

DateFormat::Field::Init(
    /* [in] */ const String& fieldName,
    /* [in] */ Int32 calendarField)
{
    FAIL_RETURN(Format::Field::Init(fieldName));
    mCalendarField = calendarField;
    // if (calendarField != -1 && sTable.Find(calendarField) == sTable.End()) {
    if (calendarField != -1 && sTable[calendarField] == NULL) {
        sTable[calendarField] = (IDateFormatField*)this->Probe(EIID_IDateFormatField);
    }
    return NOERROR;
}

// ECode DateFormat::Field::GetName(
//     /* [out] */ String* name)
// {
//     return Format::Field::GetName(name);
// }

ECode DateFormat::Field::GetCalendarField(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mCalendarField;
    return NOERROR;
}

ECode DateFormat::Field::OfCalendarField(
    /* [in] */ Int32 calendarField,
    /* [out] */ IDateFormatField** field)
{
    VALIDATE_NOT_NULL(field);
    if (calendarField < 0 || calendarField >= ICalendar::FIELD_COUNT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    HashMap<Int32, AutoPtr<IDateFormatField> >::Iterator it = sTable.Find(calendarField);
    if (it != sTable.End()) {
        *field = it->mSecond;
        REFCOUNT_ADD(*field);
    }
    return NOERROR;
}

DateFormat::DateFormat()
{}

ECode DateFormat::FormatObject(
    /* [in] */ IInterface* object,
    /* [in] */ IStringBuffer * buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ IStringBuffer ** value)
{
    VALIDATE_NOT_NULL(object)
    VALIDATE_NOT_NULL(buffer)
    VALIDATE_NOT_NULL(field)
    VALIDATE_NOT_NULL(value)

    IDate* date = IDate::Probe(object);
    if (date != NULL) {
        return FormatDate(date, buffer, field, value);
    }

    INumber* number = INumber::Probe(object) ;
    if (number != NULL) {
        Int64 v;
        number->Int64Value(&v);
        AutoPtr<IDate> dateObj;
        CDate::New(v, (IDate**)&dateObj);
        return FormatDate(dateObj, buffer, field, value);
    }

    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode DateFormat::FormatDate(
    /* [in] */ IDate* date,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);

    AutoPtr<IFieldPosition> field;
    CFieldPosition::New(0, (IFieldPosition**)&field);
    AutoPtr<IStringBuffer> sb = new StringBuffer();
    AutoPtr<IStringBuffer> outsb;
    FormatDate(date, sb, field, (IStringBuffer **)&outsb);
    return outsb->ToString(value);
}

ECode DateFormat::GetAvailableLocales(
    /* [out] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);
    AutoPtr<IICUHelper> ICUHelper;
    FAIL_RETURN(CICUHelper::AcquireSingleton((IICUHelper**)&ICUHelper));
    return ICUHelper->GetAvailableDateFormatLocales(locales);
}

ECode DateFormat::GetCalendar(
    /* [out] */ ICalendar** calendar)
{
    VALIDATE_NOT_NULL(calendar);
    *calendar = mCalendar;
    REFCOUNT_ADD(*calendar);
    return NOERROR;
}

ECode DateFormat::GetDateInstance(
    /* [out] */ IDateFormat** instance)
{
    return GetDateInstance(IDateFormat::DEFAULT, instance);
}

ECode DateFormat::GetDateInstance(
    /* [in] */ Int32 style,
    /* [out] */ IDateFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    FAIL_RETURN(CheckDateStyle(style));

    AutoPtr<ILocaleHelper> localeHelper;
    FAIL_RETURN(CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper));
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));

    return GetDateInstance(style, locale, instance);
}

ECode DateFormat::GetDateInstance(
    /* [in] */ Int32 style,
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    FAIL_RETURN(CheckDateStyle(style));

    AutoPtr<ILocaleDataHelper> localeDataHelper;
    FAIL_RETURN(CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&localeDataHelper));
    AutoPtr<ILocaleData> localeData;
    FAIL_RETURN(localeDataHelper->Get(locale, (ILocaleData**)&localeData));
    String format;
    localeData->GetDateFormat(style, &format);

    AutoPtr<ISimpleDateFormat> f;
    FAIL_RETURN(CSimpleDateFormat::New(format, locale, (ISimpleDateFormat**)&f));
    *instance = IDateFormat::Probe(f.Get());
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode DateFormat::GetDateTimeInstance(
    /* [out] */ IDateFormat** instance)
{
    return GetDateTimeInstance(IDateFormat::DEFAULT, IDateFormat::DEFAULT, instance);
}

ECode DateFormat::GetDateTimeInstance(
    /* [in] */ Int32 dateStyle,
    /* [in] */ Int32 timeStyle,
    /* [out] */ IDateFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    FAIL_RETURN(CheckTimeStyle(timeStyle));
    FAIL_RETURN(CheckDateStyle(dateStyle));

    AutoPtr<ILocaleHelper> localeHelper;
    FAIL_RETURN(CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper));
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);

    return GetDateTimeInstance(dateStyle, timeStyle, locale, instance);
}

ECode DateFormat::GetDateTimeInstance(
    /* [in] */ Int32 dateStyle,
    /* [in] */ Int32 timeStyle,
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    FAIL_RETURN(CheckTimeStyle(timeStyle));
    FAIL_RETURN(CheckDateStyle(dateStyle));

    AutoPtr<ILocaleDataHelper> localeDataHelper;
    FAIL_RETURN(CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&localeDataHelper));
    AutoPtr<ILocaleData> localeData;
    FAIL_RETURN(localeDataHelper->Get(locale, (ILocaleData**)&localeData));
    String dateFormat;
    String timeFormat;
    localeData->GetDateFormat(dateStyle, &dateFormat);
    localeData->GetTimeFormat(timeStyle, &timeFormat);
    String pattern = dateFormat + " " + timeFormat;

    AutoPtr<ISimpleDateFormat> f;
    FAIL_RETURN(CSimpleDateFormat::New(pattern, locale, (ISimpleDateFormat**)&f));
    *instance = IDateFormat::Probe(f);
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode DateFormat::GetInstance(
    /* [out] */ IDateFormat** instance)
{
    return GetDateTimeInstance(IDateFormat::SHORT, IDateFormat::SHORT, instance);
}

ECode DateFormat::GetNumberFormat(
    /* [out] */ INumberFormat** numberFormat)
{
    VALIDATE_NOT_NULL(numberFormat);
    *numberFormat = mNumberFormat;
    REFCOUNT_ADD(*numberFormat);
    return NOERROR;
}

ECode DateFormat::GetTimeInstance(
    /* [out] */ IDateFormat** instance)
{
    return GetTimeInstance(IDateFormat::DEFAULT, instance);
}

ECode DateFormat::GetTimeInstance(
    /* [in] */ Int32 style,
    /* [out] */ IDateFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    FAIL_RETURN(CheckTimeStyle(style));
    AutoPtr<ILocaleHelper> localeHelper;
    FAIL_RETURN(CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper));
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));

    return GetTimeInstance(style, locale, instance);
}

ECode DateFormat::GetTimeInstance(
    /* [in] */ Int32 style,
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    FAIL_RETURN(CheckTimeStyle(style));

    AutoPtr<ILocaleDataHelper> localeDataHelper;
    FAIL_RETURN(CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&localeDataHelper));
    AutoPtr<ILocaleData> localeData;
    FAIL_RETURN(localeDataHelper->Get(locale, (ILocaleData**)&localeData));
    String timeFormat;
    localeData->GetTimeFormat(style, &timeFormat);

    AutoPtr<ISimpleDateFormat> f;
    FAIL_RETURN(CSimpleDateFormat::New(timeFormat, locale, (ISimpleDateFormat**)&f));
    *instance = IDateFormat::Probe(f.Get());
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode DateFormat::GetTimeZone(
    /* [out] */ ITimeZone** tz)
{
    return mCalendar->GetTimeZone(tz);
}

ECode DateFormat::IsLenient(
    /* [out] */ Boolean* isLenient)
{
    return mCalendar->IsLenient(isLenient);
}

ECode DateFormat::Parse(
    /* [in] */ const String& string,
    /* [out] */ IDate** date)
{
    VALIDATE_NOT_NULL(date);

    AutoPtr<IParsePosition> position;
    CParsePosition::New(0, (IParsePosition**)&position);
    FAIL_RETURN(Parse(string, position, date));
    REFCOUNT_ADD(*date);
    Int32 index;
    position->GetIndex(&index);
    if (index == 0) {
        return E_PARSE_EXCEPTION;
    }

    return NOERROR;
}

ECode DateFormat::ParseObject(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ IInterface** object)
{
    return Parse(string, position, (IDate**)object);
}

ECode DateFormat::SetCalendar(
    /* [in] */ ICalendar* cal)
{
    mCalendar = cal;
    return NOERROR;
}

ECode DateFormat::SetLenient(
    /* [in] */ Boolean value)
{
    return mCalendar->SetLenient(value);
}

ECode DateFormat::SetNumberFormat(
   /* [in] */ INumberFormat* format)
{
    mNumberFormat = format;
    return NOERROR;
}

ECode DateFormat::SetTimeZone(
    /* [in] */ ITimeZone* timezone)
{
    return mCalendar->SetTimeZone(timezone);
}

ECode DateFormat::CheckDateStyle(
    /* [in] */ Int32 style)
{
    if (!(style == IDateFormat::SHORT
            || style == IDateFormat::MEDIUM
            || style == IDateFormat::LONG
            || style == IDateFormat::FULL
            || style == IDateFormat::DEFAULT)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode DateFormat::CheckTimeStyle(
    /* [in] */ Int32 style)
{
    if (!(style == IDateFormat::SHORT
            || style == IDateFormat::MEDIUM
            || style == IDateFormat::LONG
            || style == IDateFormat::FULL
            || style == IDateFormat::DEFAULT)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode DateFormat::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    if (this->Probe(EIID_IDateFormat) == IDateFormat::Probe(object)) {
        *result = TRUE;
        return NOERROR;
    }
    if (object->Probe(EIID_IDateFormat) == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    AutoPtr<IDateFormat> dateFormat = (IDateFormat*)object;
    Boolean res1 = FALSE, res2 = FALSE;
    AutoPtr<INumberFormat> nf;
    dateFormat->GetNumberFormat((INumberFormat**)&nf);
    mNumberFormat->Equals((INumberFormat*)nf, &res1);

    AutoPtr<ITimeZone> timezone1, timezone2;
    mCalendar->GetTimeZone((ITimeZone**)&timezone1);

    AutoPtr<ICalendar> cal;
    dateFormat->GetCalendar((ICalendar**)&cal);

    cal->GetTimeZone((ITimeZone**)&timezone2);
    timezone1->Equals(timezone2, &res2);

    Int32 fdw1, fdw2;
    mCalendar->GetFirstDayOfWeek(&fdw1);
    cal->GetFirstDayOfWeek(&fdw2);

    Int32 mdfw1, mdfw2;
    mCalendar->GetMinimalDaysInFirstWeek(&mdfw1);
    cal->GetMinimalDaysInFirstWeek(&mdfw2);

    Boolean l1, l2;
    mCalendar->IsLenient(&l1);
    cal->IsLenient(&l2);

    *result = res1 && res2 && fdw1 == fdw2 && mdfw1 == mdfw2 && l1 == l2;
    return NOERROR;
}

} // namespace Text
} // namespace Elastos
