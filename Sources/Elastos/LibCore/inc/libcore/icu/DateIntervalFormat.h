
#ifndef __LIBCORE_ICU_DATEINTERVALFORMAT_H__
#define __LIBCORE_ICU_DATEINTERVALFORMAT_H__

#include "Object.h"
#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif
#include "utility/BasicLruCache.h"

using Libcore::Utility::BasicLruCache;
using Elastos::Utility::ILocale;
using Elastos::Utility::ITimeZone;
using Elastos::Utility::ICalendar;

namespace Libcore {
namespace ICU {

class DateIntervalFormat
{
public:
    class FormatterCache
        : public BasicLruCache
    {
    public:
        FormatterCache();

    protected:
        virtual CARAPI_(void) EntryEvicted(
            /* [in] */ IInterface* key,
            /* [in] */ IInterface* value);
    };

private:
    DateIntervalFormat(){}

public:
    static CARAPI_(String) FormatDateRange(
        /* [in] */ Int64 startMs,
        /* [in] */ Int64 endMs,
        /* [in] */ Int32 flags,
        /* [in] */ const String& olsonId);

    static CARAPI_(String) FormatDateRange(
        /* [in] */ ILocale* locale,
        /* [in] */ ITimeZone* tz,
        /* [in] */ Int64 startMs,
        /* [in] */ Int64 endMs,
        /* [in] */ Int32 flags);

private:
    static CARAPI_(Int64) GetFormatter(
        /* [in] */ const String& skeleton,
        /* [in] */ const String& localeName,
        /* [in] */ const String& tzName);

    static CARAPI_(String) ToSkeleton(
        /* [in] */ ICalendar* startCalendar,
        /* [in] */ ICalendar* endCalendar,
        /* [in] */ Int32 flags);

    static CARAPI_(Boolean) IsMidnight(
        /* [in] */ ICalendar* c);

    static CARAPI_(Boolean) OnTheHour(
        /* [in] */ ICalendar* c);

    static CARAPI_(Boolean) FallOnDifferentDates(
        /* [in] */ ICalendar* c1,
        /* [in] */ ICalendar* c2);

    static CARAPI_(Boolean) FallInSameMonth(
        /* [in] */ ICalendar* c1,
        /* [in] */ ICalendar* c2);

    static CARAPI_(Boolean) FallInSameYear(
        /* [in] */ ICalendar* c1,
        /* [in] */ ICalendar* c2);

    static CARAPI_(Boolean) IsThisYear(
        /* [in] */ ICalendar* c);

    static CARAPI_(Int32) DayDistance(
        /* [in] */ ICalendar* c1,
        /* [in] */ ICalendar* c2);

    static CARAPI_(Int32) JulianDay(
        /* [in] */ ICalendar* c);

    static CARAPI_(Int64) CreateDateIntervalFormat(
        /* [in] */ const String& skeleton,
        /* [in] */ const String& localeName,
        /* [in] */ const String& tzName);

    static CARAPI_(void) DestroyDateIntervalFormat(
        /* [in] */ Int64 address);

    static CARAPI_(String) FormatDateInterval(
        /* [in] */ Int64 address,
        /* [in] */ Int64 fromDate,
        /* [in] */ Int64 toDate);

private:
    static const Int32 DAY_IN_MS;
    static const Int32 EPOCH_JULIAN_DAY;
    static const AutoPtr<FormatterCache> CACHED_FORMATTERS;
};

} // ICU
} // Libcore
#endif // __LIBCORE_ICU_DATEINTERVALFORMAT_H__