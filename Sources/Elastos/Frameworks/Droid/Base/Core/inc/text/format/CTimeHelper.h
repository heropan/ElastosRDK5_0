
#ifndef __CTIMEHELPER_H__
#define __CTIMEHELPER_H__

#include "_CTimeHelper.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Format {

CarClass(CTimeHelper)
{
public:
    CARAPI Compare(
        /* [in] */ ITime* a,
        /* [in] */ ITime* b,
        /* [out] */ Int32* ret);

    CARAPI GetCurrentTimezone(
        /* [out] */ String* ret);

    CARAPI GetJulianDay(
        /* [in] */ Int64 millis,
        /* [in] */ Int64 gmtoff,
        /* [out] */ Int32* ret);

    CARAPI GetJulianMondayFromWeeksSinceEpoch(
        /* [in] */ Int32 week,
        /* [out] */ Int32* ret);

    CARAPI GetWeeksSinceEpochFromJulianDay(
        /* [in] */ Int32 julianDay,
        /* [in] */ Int32 firstDayOfWeek,
        /* [out] */ Int32* ret);

    CARAPI IsEpoch(
        /* [in] */ ITime* time,
        /* [out] */ Boolean* ret);
};

} // namespace Format
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CTIMEHELPER_H__
