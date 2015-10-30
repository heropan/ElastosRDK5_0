
#include "elastos/droid/webkit/DateSorter.h"
#include <elastos/core/Math.h>
#include "elastos/droid/R.h"

using Elastos::Utility::ILocale;
using Elastos::Utility::CLocaleHelper;
using Elastos::Utility::ILocaleHelper;
using Libcore::ICU::ILocaleData;
using Libcore::ICU::CLocaleDataHelper;
using Libcore::ICU::ILocaleDataHelper;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Utility::ICalendar;
using Elastos::Utility::CCalendarHelper;
using Elastos::Utility::ICalendarHelper;

namespace Elastos {
namespace Droid {
namespace Webkit {

const String DateSorter::LOGTAG("webkit");
const Int32 DateSorter::NUM_DAYS_AGO;

DateSorter::DateSorter()
{
}

/**
 * @param context Application context
 */
DateSorter::DateSorter(
    /* [in] */ IContext* context)
{
    Init(context);
}

/**
 * @param context Application context
 */
void DateSorter::Init(
    /* [in] */ IContext* context)
{
    mBins = ArrayOf<Int64>::Alloc(IDateSorter::DAY_COUNT-1);
    mLabels = ArrayOf<String>::Alloc(IDateSorter::DAY_COUNT);

    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);

    AutoPtr<ICalendar> c;
    AutoPtr<ICalendarHelper> calendarHelper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&calendarHelper);
    calendarHelper->GetInstance((ICalendar**)&c);
    BeginningOfDay(c);

    // Create the bins
    c->GetTimeInMillis(&((*mBins)[0])); // Today
    c->Add(ICalendar::DAY_OF_YEAR, -1);
    c->GetTimeInMillis(&((*mBins)[1]));  // Yesterday
    c->Add(ICalendar::DAY_OF_YEAR, -(NUM_DAYS_AGO - 1));
    c->GetTimeInMillis(&((*mBins)[2]));  // Five days ago
    c->Add(ICalendar::DAY_OF_YEAR, NUM_DAYS_AGO); // move back to today
    c->Add(ICalendar::MONTH, -1);
    c->GetTimeInMillis(&((*mBins)[3]));  // One month ago

    // build labels
    AutoPtr<IConfiguration> configuration;
    resources->GetConfiguration((IConfiguration**)&configuration);
    AutoPtr<ILocale> locale;
    configuration->GetLocale((ILocale**)&locale);
    if (locale == NULL) {
        AutoPtr<ILocaleHelper> localeHelper;
        CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
        localeHelper->GetDefault((ILocale**)&locale);
    }
    AutoPtr<ILocaleDataHelper> localeDataHelper;
    CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&localeDataHelper);
    AutoPtr<ILocaleData> localeData;
    localeDataHelper->Get(locale, (ILocaleData**)&localeData);
    String today, yesterday;
    localeData->GetToday(&today);
    localeData->GetYesterday(&yesterday);
    (*mLabels)[0] = today;
    (*mLabels)[1] = yesterday;

    Int32 resId = R::plurals::last_num_days;
    String format;
    resources->GetQuantityString(resId, NUM_DAYS_AGO, &format);
    (*mLabels)[2].AppendFormat((const char*)format, NUM_DAYS_AGO);

    context->GetString(R::string::last_month, &((*mLabels)[3]));
    context->GetString(R::string::older, &((*mLabels)[4]));
}

/**
 * @param time time since the Epoch in milliseconds, such as that
 * returned by Calendar.getTimeInMillis()
 * @return an index from 0 to (DAY_COUNT - 1) that identifies which
 * date bin this date belongs to
 */
Int32 DateSorter::GetIndex(
    /* [in] */ Int64 time)
{
    Int32 lastDay = IDateSorter::DAY_COUNT - 1;
    for (Int32 i = 0; i < lastDay; i++) {
        if (time > (*mBins)[i]) return i;
    }

    return lastDay;
}

/**
 * @param index date bin index as returned by getIndex()
 * @return string label suitable for display to user
 */
String DateSorter::GetLabel(
    /* [in] */ Int32 index)
{
    if (index < 0 || index >= IDateSorter::DAY_COUNT) return String("");
    return (*mLabels)[index];
}

/**
 * @param index date bin index as returned by getIndex()
 * @return date boundary at given index
 */
Int64 DateSorter::GetBoundary(
    /* [in] */ Int32 index)
{
    Int32 lastDay = IDateSorter::DAY_COUNT - 1;
    // Error case
    if (index < 0 || index > lastDay) index = 0;
    // Since this provides a lower boundary on dates that will be included
    // in the given bin, provide the smallest value
    if (index == lastDay) return Elastos::Core::Math::INT64_MIN_VALUE;
    return (*mBins)[index];
}

/**
 * Calcuate 12:00am by zeroing out hour, minute, second, millisecond
 */
void DateSorter::BeginningOfDay(
    /* [in] */ ICalendar* c)
{
    c->Set(ICalendar::HOUR_OF_DAY, 0);
    c->Set(ICalendar::MINUTE, 0);
    c->Set(ICalendar::SECOND, 0);
    c->Set(ICalendar::MILLISECOND, 0);
}

ECode DateSorter::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "DateSorter";
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos