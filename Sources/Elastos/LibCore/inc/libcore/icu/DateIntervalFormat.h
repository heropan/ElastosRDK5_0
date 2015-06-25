
#ifndef __LIBCORE_ICU_DATEINTERVALFORMAT_H__
#define __LIBCORE_ICU_DATEINTERVALFORMAT_H__

#include "Object.h"
#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

using Elastos::Utility::ILocale;
using Elastos::Utility::ITimeZone;
using Elastos::Utility::ICalendar;
using Elastos::Core::Object;

namespace Libcore {
namespace ICU {

class DateIntervalFormat
{
public:
	class FormatterCache
		: public Object
		// , public BasicLruCache<String, Int64>
	{
	public:
		FormatterCache()/*:BasicLruCache<String, Int64>(8)*/{/*TODO: Wait for BasicLruCache*/assert(0);}

	protected:
		void EntryEvicted(
			const String& key,
			Int64 value);
	};

private:
	DateIntervalFormat(){}

public:
	static String FormatDateRange(
		Int64 startMs,
		Int64 endMs,
		Int32 flags,
		String& olsonId);

	static String FormatDateRange(
		ILocale* locale,
		ITimeZone* tz,
		Int64 startMs,
		Int64 endMs,
		Int32 flags);

private:
	static Int64 GetFormatter(
		const String& skeleton,
		const String& localeName,
		const String& tzName);

	static String ToSkeleton(
		ICalendar* startCalendar,
		ICalendar* endCalendar,
		Int32 flags);

	static Boolean IsMidnight(
		ICalendar* c);

	static Boolean OnTheHour(
		ICalendar* c);

	static Boolean FallOnDifferentDates(
		ICalendar* c1,
		ICalendar* c2);

	static Boolean FallInSameMonth(
		ICalendar* c1,
		ICalendar* c2);

	static Boolean FallInSameYear(
		ICalendar* c1,
		ICalendar* c2);

	static Boolean IsThisYear(
		ICalendar* c);

	static Int32 DayDistance(
		ICalendar* c1,
		ICalendar* c2);

	static Int32 JulianDay(
		ICalendar* c);

	static Int64 CreateDateIntervalFormat(
		const String& skeleton,
		const String& localeName,
		const String& tzName);

    static void DestroyDateIntervalFormat(
    	Int64 address);

    static String FormatDateInterval(
    	Int64 address,
    	Int64 fromDate,
    	Int64 toDate);

private:
	static const Int32 DAY_IN_MS;
	static const Int32 EPOCH_JULIAN_DAY;
	static const AutoPtr<FormatterCache> CACHED_FORMATTERS;
};

} // ICU
} // Libcore
#endif // __LIBCORE_ICU_DATEINTERVALFORMAT_H__