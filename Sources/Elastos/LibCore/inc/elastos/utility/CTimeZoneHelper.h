
#ifndef __UTILITY_CTIMEZONEHELPER_H__
#define __UTILITY_CTIMEZONEHELPER_H__

#include "_CTimeZoneHelper.h"

namespace Elastos {
namespace Utility {

CarClass(CTimeZoneHelper)
{
public:
    CARAPI GetAvailableIDs(
        /* [out, callee] */ ArrayOf<String>** ppArray);

    CARAPI GetAvailableIDs(
        /* [in] */ Int32 offsetMillis,
        /* [out, callee] */ ArrayOf<String>** ppArray);

    CARAPI GetDefault(
        /* [out] */ ITimeZone** ppTimezone);

    CARAPI GetTimeZone(
        /* [in] */ const String& id,
        /* [out] */ ITimeZone** ppTimezone);

    CARAPI SetDefault(
        /* [in] */ ITimeZone* pTimeZone);

    CARAPI GetGMT(
        /* [out] */ ITimeZone** timezone);

    CARAPI GetUTC(
        /* [out] */ ITimeZone** timezone);
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_CTIMEZONEHELPER_H__
