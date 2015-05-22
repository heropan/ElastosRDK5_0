
#ifndef __CTIMESTAMP_H__
#define __CTIMESTAMP_H__

#include "_CTimestamp.h"
#include "Date.h"
#include "TimestampMacro.h"

namespace Elastos {
namespace Sql {

CarClass(CTimestamp) , public Date
{
public:
    SQLTIMESTAMP_METHODS_DECL()

    CARAPI CompareTo(
        /* [in] */ Elastos::Utility::IDate* date,
        /* [out] */ Int32* result);

    CARAPI GetTime(
        /* [out] */ Int64* time);

    CARAPI SetTime(
        /* [in] */ Int64 milliseconds);

    CARAPI ToString(
        /* [out] */ String* localeStr);

    CARAPI After(
        /* [in] */ ITimestamp * theTimestamp,
        /* [out] */ Boolean * value);

    CARAPI Before(
        /* [in] */ ITimestamp * theTimestamp,
        /* [out] */ Boolean * value);

    CARAPI CompareTo(
        /* [in] */ ITimestamp * theTimestamp,
        /* [out] */ Int32 * value);

    CARAPI Equals(
        /* [in] */ IInterface * pTheObject,
        /* [out] */ Boolean * value);

    CARAPI Equals(
        /* [in] */ ITimestamp * theTimestamp,
        /* [out] */ Boolean * value);

    CARAPI GetNanos(
        /* [out] */ Int32 * pNano);

    CARAPI SetNanos(
        /* [in] */ Int32 n);

    CARAPI constructor(
        /* [in] */ Int32 theYear,
        /* [in] */ Int32 theMonth,
        /* [in] */ Int32 theDate,
        /* [in] */ Int32 theHour,
        /* [in] */ Int32 theMinute,
        /* [in] */ Int32 theSecond,
        /* [in] */ Int32 theNano);

    CARAPI constructor(
        /* [in] */ Int64 theTime);

public:
    static CARAPI_(AutoPtr<ITimestamp>) ValueOf(
        /* [in] */ const String& str);

private:
    CARAPI_(void) Format(
        /* [in] */ Int32 date,
        /* [in] */ Int32 digits,
        /* [in] */ AutoPtr<StringBuilder> sb);

    CARAPI_(void) SetTimeImpl(
        /* [in] */ Int64 theTime);

private:
    static CARAPI BadTimestampString(
        /* [in] */ const String& s);

private:
    Int32 nanos;

    const static String TIME_FORMAT_REGEX;

    const static String PADDING;
};

} // namespace Sql
} // namespace Elastos

#endif // __CTIMESTAMP_H__
