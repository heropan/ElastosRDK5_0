#ifndef __UTILITY_CZONEINFODB_H__
#define __UTILITY_CZONEINFODB_H__

#include "_CZoneInfoDB.h"

namespace Elastos{
namespace Utility{

/**
 * Our concrete TimeZone implementation, backed by zoneinfo data.
 *
 * @hide - used to implement TimeZone
 */
CarClass(CZoneInfoDB)
{
public:
    CARAPI constructor();

    CARAPI GetAvailableIDs(
        /* [out, callee] */ ArrayOf<String>** array);

    CARAPI GetAvailableIDs(
        /* [in] */ Int32 rawOffset,
        /* [out, callee] */ ArrayOf<String>** array);

    CARAPI GetSystemDefault(
        /* [out] */ ITimeZone** tz);

    CARAPI GetTimeZone(
        /* [in] */ const String& id,
        /* [out] */ ITimeZone** tz);

    CARAPI GetVersion(
        /* [out] */ String* version);

    CARAPI MakeTimeZone(
        /* [in] */  const String& id,
        /* [out] */ ITimeZone** tz);
};

} // namespace Utility
} // namespace Elastos

#endif //__UTILITY_CZONEINFODB_H__
