#include "CZoneInfoDB.h"
#include "cmdef.h"
#include "ZoneInfoDB.h"

namespace Elastos{
namespace Utility{

ECode CZoneInfoDB::constructor()
{
    return ZoneInfoDB::Init();
}

ECode CZoneInfoDB::GetAvailableIDs(
    /* [out, callee] */ ArrayOf<String>** array)
{
    VALIDATE_NOT_NULL(array);
    AutoPtr<ArrayOf<String> > tmp = ZoneInfoDB::GetAvailableIDs();
    *array = tmp;
    INTERFACE_ADDREF(*array);
    return NOERROR;
}

ECode CZoneInfoDB::GetAvailableIDsEx(
    /* [in] */ Int32 rawOffset,
    /* [out, callee] */ ArrayOf<String>** array)
{
    VALIDATE_NOT_NULL(array);
    AutoPtr<ArrayOf<String> > tmp = ZoneInfoDB::GetAvailableIDs(rawOffset);
    *array = tmp;
    INTERFACE_ADDREF(*array);
    return NOERROR;
}

ECode CZoneInfoDB::GetSystemDefault(
    /* [out] */ ITimeZone** tz)
{
    VALIDATE_NOT_NULL(tz);
    AutoPtr<ITimeZone> tmp = ZoneInfoDB::GetSystemDefault();
    *tz = tmp;
    INTERFACE_ADDREF(*tz);
    return NOERROR;
}

ECode CZoneInfoDB::GetTimeZone(
    /* [in] */ const String& id,
    /* [out] */ ITimeZone** tz)
{
    VALIDATE_NOT_NULL(tz);
    AutoPtr<ITimeZone> tmp = ZoneInfoDB::GetTimeZone(id);
    *tz = tmp;
    INTERFACE_ADDREF(*tz);
    return NOERROR;
}

ECode CZoneInfoDB::GetVersion(
    /* [out] */ String* version)
{
    VALIDATE_NOT_NULL(version);
    *version = ZoneInfoDB::GetVersion();
    return NOERROR;
}

ECode CZoneInfoDB::MakeTimeZone(
    /* [in] */  const String& id,
    /* [out] */ ITimeZone** tz)
{
    VALIDATE_NOT_NULL(tz);
    AutoPtr<ITimeZone> tmp = ZoneInfoDB::MakeTimeZone(id);
    *tz = tmp;
    INTERFACE_ADDREF(*tz);
    return NOERROR;
}

} // Utility
} // Elastos
