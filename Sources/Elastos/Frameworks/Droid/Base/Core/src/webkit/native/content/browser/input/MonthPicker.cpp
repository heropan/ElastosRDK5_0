
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

const Int32 MonthPicker::MONTHS_NUMBER;

MonthPicker::MonthPicker(
    /* [in] */ IContext* context,
    /* [in] */ Double minValue,
    /* [in] */ Double maxValue)
    : TwoFieldDatePicker(context, minValue, maxValue);
{
    GetPositionInYearSpinner()->SetContentDescription(
            GetResources()->GetString(R::string::accessibility_date_picker_month));

    // initialization based on locale
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    AutoPtr<IDateFormatSymbolsHelper> dfsHelper;
    CDateFormatSymbolsHelper::AcquireSingleton((IDateFormatSymbolsHelper**)&dfsHelper);
    AutoPtr<IDateFormatSymbols> symbols;
    dfsHelper->GetInstance(locale, (IDateFormatSymbols**)&symbols);
    symbols->GetShortMonths((ArrayOf<String>**)&mShortMonths);

    // initialize to current date
    AutoPtr<ICalendarHelper> calendarHelper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&calendarHelper);
    AutoPtr<ICalendar> cal;
    calendarHelper->GetInstance(TimeZone::GetTimeZone(String("UTC")), (ICalendar**)&cal);
    Int32 year, month;
    cal->Get(ICalendar::YEAR, &year);
    cal->Get(ICalendar::MONTH, &month);
    Init(year, month, NULL);
}

/**
 * Creates a date object from the |value| which is months since epoch.
 */
AutoPtr<ICalendar> MonthPicker::CreateDateFromValue(
    /* [in] */ Double value)
{
    Int32 year = (Int32) Math::Min(value / 12 + 1970, Integer.MAX_VALUE);
    Int32 month = (Int32) (value % 12);
    AutoPtr<ICalendarHelper> calendarHelper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&calendarHelper);
    AutoPtr<ICalendar> cal;
    calendarHelper->GetInstance(TimeZone::GetTimeZone(String("UTC")), (ICalendar**)&cal);
    cal->Clear();
    cal->Set(year, month, 1);
    return cal;
}

//@Override
AutoPtr<ICalendar> MonthPicker::GetDateForValue(
    /* [in] */ Double value)
{
    return MonthPicker::CreateDateFromValue(value);
}

//@Override
void MonthPicker::SetCurrentDate(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month)
{
    AutoPtr<ICalendarHelper> calendarHelper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&calendarHelper);
    AutoPtr<ICalendar> date;
    calendarHelper->GetInstance(TimeZone::GetTimeZone(String("UTC")), (ICalendar**)&date);
    date->Set(year, month, 1);
    Boolean bFlag;
    if (date->Before(GetMinDate(), &bFlag), bFlag) {
        SetCurrentDate(GetMinDate());
    }
    else if (date->After(GetMaxDate(), &bFlag), bFlag) {
        SetCurrentDate(GetMaxDate());
    }
    else {
        SetCurrentDate(date);
    }
}

//@Override
void MonthPicker::UpdateSpinners()
{
    TwoFieldDatePicker::UpdateSpinners();

    // make sure the month names are a zero based array
    // with the months in the month spinner
    AutoPtr< ArrayOf<String> > displayedValues;
    Arrays<String>::CopyOfRange(mShortMonths,
            GetPositionInYearSpinner()->GetMinValue(),
            GetPositionInYearSpinner()->GetMaxValue() + 1,
            (ArrayOf<String>**)&displayedValues);
    GetPositionInYearSpinner()->SetDisplayedValues(displayedValues);
}

/**
 * @return The selected month.
 */
Int32 MonthPicker::GetMonth()
{
    Int32 month;
    GetCurrentDate()->Get(ICalendar::MONTH, &month);
    return month;
}

//@Override
Int32 MonthPicker::GetPositionInYear()
{
    return GetMonth();
}

//@Override
Int32 MonthPicker::GetMaxYear()
{
    Int32 year;
    GetMaxDate()->Get(ICalendar::YEAR, &year);
    return year;
}

//@Override
Int32 MonthPicker::GetMinYear()
{
    Int32 year;
    GetMinDate()->Get(ICalendar::YEAR, &year);
}

//@Override
Int32 MonthPicker::GetMaxPositionInYear(
    /* [in] */ Int32 year)
{
    Int32 cYear;
    GetMaxDate()->Get(ICalendar::YEAR, cYear);
    if (year == cYear) {
        Int32 month;
        GetMaxDate()->Get(ICalendar::MONTH, &month);
        return month;
    }

    return MONTHS_NUMBER - 1;
}

//@Override
Int32 MonthPicker::GetMinPositionInYear(
    /* [in] */ Int32 year)
{
    Int32 cYear;
    GetMinDate()->Get(ICalendar::YEAR, &cYear);
    if (year == cYear) {
        Int32 month;
        GetMinDate()->Get(ICalendar::MONTH, &month);
        return month;
    }

    return 0;
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
