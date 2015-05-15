#ifndef __UTILITY_CZONEINFO_H__
#define __UTILITY_CZONEINFO_H__

#include "_CZoneInfo.h"
#include "TimeZone.h"

namespace Elastos {
namespace Utility {

/**
 * Our concrete TimeZone implementation, backed by zoneinfo data.
 *
 * @hide - used to implement TimeZone
 */
CarClass(CZoneInfo), public TimeZone
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Int32>* transitions,
        /* [in] */ ArrayOf<Byte>* type,
        /* [in] */ ArrayOf<Int32>* gmtoff,
        /* [in] */ ArrayOf<Byte>* isdst);

    /**
     * Returns the offset in milliseconds from UTC for this time zone at {@code
     * time}. The offset includes daylight savings time if the specified
     * date is within the daylight savings time period.
     *
     * @param time the date in milliseconds since January 1, 1970 00:00:00 UTC
     */
    CARAPI GetOffset(
        /* [in] */ Int64 time,
        /* [out] */ Int32* pOffset);

    /**
     * Returns the offset in milliseconds from UTC for this time zone at {@code
     * time}. The offset includes daylight savings time if the specified
     * date is within the daylight savings time period.
     *
     * @param time the date in milliseconds since January 1, 1970 00:00:00 UTC
     */
    CARAPI GetOffsetEx(
            /* [in] */ Int32 era,
            /* [in] */ Int32 year,
            /* [in] */ Int32 month,
            /* [in] */ Int32 day,
            /* [in] */ Int32 dayOfWeek,
            /* [in] */ Int32 timeOfDayMillis,
            /* [out] */ Int32* pOffset);

    /**
     * Returns the offset in milliseconds from UTC of this time zone's standard
     * time.
     */
    CARAPI GetRawOffset(
            /* [out] */ Int32* pRawOffset);

    /**
     * Sets the offset in milliseconds from UTC of this time zone's standard
     * time.
     */
    CARAPI SetRawOffset(
        /* [in] */ Int32 offsetMillis);

    CARAPI GetDSTSavings(
        /* [out] */ Int32* pSavings);

    /**
     * Returns true if {@code time} is in a daylight savings time period for
     * this time zone.
     */
    CARAPI InDaylightTime(
        /* [in] */ IDate *when,
        /* [out] */ Boolean* value);

    /**
     * Returns true if this time zone has a future transition to or from
     * daylight savings time.
     *
     * <p><strong>Warning:</strong> this returns false for time zones like
     * {@code Asia/Kuala_Lumpur} that have previously used DST but do not
     * currently. A hypothetical country that has never observed daylight
     * savings before but plans to start next year would return true.
     *
     * <p><strong>Warning:</strong> this returns true for time zones that use
     * DST, even when it is not active.
     *
     * <p>Use {@link #inDaylightTime} to find out whether daylight savings is
     * in effect at a specific time.
     *
     * <p>Most applications should not use this method.
     */
    CARAPI UseDaylightTime(
        /* [out] */ Boolean* value);

    /**
     * Returns true if {@code timeZone} has the same rules as this time zone.
     *
     * <p>The base implementation returns true if both time zones have the same
     * raw offset.
     */
    CARAPI HasSameRules(
        /* [in] */ ITimeZone* timeZone,
        /* [out] */ Boolean* value);

    CARAPI Clone(
            /* [out] */ ITimeZone** newObj);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* isEqual);

    CARAPI SetID(
        /* [in] */ const String& id);

    CARAPI GetID(
        /* [out] */ String* id);

    CARAPI GetDisplayName(
        /* [out] */ String* name);

    CARAPI GetDisplayNameEx(
        /* [in] */ ILocale* locale,
        /* [out] */ String* name);

    CARAPI GetDisplayNameEx2(
        /* [in] */ Boolean daylightTime,
        /* [in] */ Int32 style,
        /* [out] */ String* name);

    CARAPI GetDisplayNameEx3(
        /* [in] */ Boolean daylightTime,
        /* [in] */ Int32 style,
        /* [in] */ ILocale* locale,
        /* [out] */ String* name);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI Clone(
        /* [out] */ IInterface** newObj);

private:
    static String FormatTime(
        /* [in] */ Int32 s,
        /* [in] */ ITimeZone* tz);

private:
    static const Int64 MILLISECONDS_PER_DAY = 24 * 60 * 60 * 1000;
    static const Int64 MILLISECONDS_PER_400_YEARS =
            MILLISECONDS_PER_DAY * (400 * 365 + 100 - 3);

    static const Int64 UNIX_OFFSET = 62167219200000LL;

    static const Int32 NORMAL[];
    static const Int32 LEAP[];

private:
    Int32 mRawOffset;
    Int32 mEarliestRawOffset;

    AutoPtr<ArrayOf<Int32> > mTransitions;
    AutoPtr<ArrayOf<Int32> > mOffsets;
    AutoPtr<ArrayOf<Byte> > mTypes;
    AutoPtr<ArrayOf<Byte> > mIsDsts;
    Boolean mUseDst;
    Int32 mDstSavings; // Implements TimeZone.getDSTSavings.
};

} // namespace Utility
} // namespace Elastos

#endif //__UTILITY_CZONEINFO_H__
