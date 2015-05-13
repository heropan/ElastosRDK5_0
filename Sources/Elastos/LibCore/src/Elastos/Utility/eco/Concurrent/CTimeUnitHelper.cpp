
#include "cmdef.h"
#include "CTimeUnitHelper.h"
#include "TimeUnit.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

ECode CTimeUnitHelper::GetNANOSECONDS(
    /* [out] */ ITimeUnit** unit)
{
    VALIDATE_NOT_NULL(unit);
    *unit = new NANOSECONDS();
    INTERFACE_ADDREF(*unit);
    return NOERROR;
}

ECode CTimeUnitHelper::GetMICROSECONDS(
    /* [out] */ ITimeUnit** unit)
{
    VALIDATE_NOT_NULL(unit);
    *unit = new MICROSECONDS();
    INTERFACE_ADDREF(*unit);
    return NOERROR;
}

ECode CTimeUnitHelper::GetMILLISECONDS(
    /* [out] */ ITimeUnit** unit)
{
    VALIDATE_NOT_NULL(unit);
    *unit = new MILLISECONDS();
    INTERFACE_ADDREF(*unit);
    return NOERROR;
}

ECode CTimeUnitHelper::GetSECONDS(
    /* [out] */ ITimeUnit** unit)
{
    VALIDATE_NOT_NULL(unit);
    *unit = new SECONDS();
    INTERFACE_ADDREF(*unit);
    return NOERROR;
}

ECode CTimeUnitHelper::GetMINUTES(
    /* [out] */ ITimeUnit** unit)
{
    VALIDATE_NOT_NULL(unit);
    *unit = new MINUTES();
    INTERFACE_ADDREF(*unit);
    return NOERROR;
}

ECode CTimeUnitHelper::GetHOURS(
    /* [out] */ ITimeUnit** unit)
{
    VALIDATE_NOT_NULL(unit);
    *unit = new HOURS();
    INTERFACE_ADDREF(*unit);
    return NOERROR;
}

ECode CTimeUnitHelper::GetDAYS(
    /* [out] */ ITimeUnit** unit)
{
    VALIDATE_NOT_NULL(unit);
    *unit = new DAYS();
    INTERFACE_ADDREF(*unit);
    return NOERROR;
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
