#include <cmdef.h>
#include "CTimeZoneHelper.h"
#include "TimeZone.h"

namespace Elastos {
namespace Utility {

ECode CTimeZoneHelper::GetAvailableIDs(
    /* [out, callee] */ ArrayOf<String> ** array)
{
    VALIDATE_NOT_NULL(array);
    AutoPtr<ArrayOf<String> > ids = TimeZone::GetAvailableIDs();
    *array = ids;
    INTERFACE_ADDREF(*array)
    return NOERROR;
}

ECode CTimeZoneHelper::GetAvailableIDsEx(
    /* [in] */ Int32 offsetMillis,
    /* [out, callee] */ ArrayOf<String> ** array)
{
    VALIDATE_NOT_NULL(array);
    AutoPtr<ArrayOf<String> > ids = TimeZone::GetAvailableIDs(offsetMillis);
    *array = ids;
    INTERFACE_ADDREF(*array)
    return NOERROR;
}

ECode CTimeZoneHelper::GetDefault(
    /* [out] */ ITimeZone ** timezone)
{
    VALIDATE_NOT_NULL(timezone);
    AutoPtr<ITimeZone> tz = TimeZone::GetDefault();
    *timezone = tz;
    INTERFACE_ADDREF(*timezone);

    return NOERROR;
}

ECode CTimeZoneHelper::GetTimeZone(
    /* [in] */ const String& id,
    /* [out] */ ITimeZone ** timezone)
{
    return TimeZone::GetTimeZone(id, timezone);
}

ECode CTimeZoneHelper::SetDefault(
    /* [in] */ ITimeZone* timeZone)
{
    TimeZone::SetDefault(timeZone);
    return NOERROR;
}

ECode CTimeZoneHelper::GetGMT(
    /* [out] */ ITimeZone** timezone)
{
    VALIDATE_NOT_NULL(timezone);
    AutoPtr<ITimeZone> tz = TimeZone::GMT();
    *timezone = tz;
    INTERFACE_ADDREF(*timezone);
    return NOERROR;
}

ECode CTimeZoneHelper::GetUTC(
    /* [out] */ ITimeZone** timezone)
{
    VALIDATE_NOT_NULL(timezone);
    AutoPtr<ITimeZone> tz = TimeZone::UTC();
    *timezone = tz;
    INTERFACE_ADDREF(*timezone);
    return NOERROR;
}


} // Utility
} // Elastos
