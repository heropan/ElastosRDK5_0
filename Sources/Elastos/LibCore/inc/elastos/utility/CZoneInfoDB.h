#ifndef __ELASTOS_UTILITY_CZONEINFODB_H__
#define __ELASTOS_UTILITY_CZONEINFODB_H__

#include "_Elastos_Utility_CZoneInfoDB.h"
#include "Singleton.h"

using Elastos::Core::Singleton;

namespace Elastos{
namespace Utility{

/**
 * Our concrete TimeZone implementation, backed by zoneinfo data.
 *
 * @hide - used to implement TimeZone
 */
CarClass(CZoneInfoDB)
    , public Singleton
    , public IZoneInfoDB
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

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

#endif //__ELASTOS_UTILITY_CZONEINFODB_H__
