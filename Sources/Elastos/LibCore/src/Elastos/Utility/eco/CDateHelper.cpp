
#include <cmdef.h>
#include "CDateHelper.h"
#include "CDate.h"

namespace Elastos {
namespace Utility {

ECode CDateHelper::Parse(
    /* [in] */ const String& string,
    /* [out] */ Int64* value)
{
    return CDate::Parse(string, value);
}

ECode CDateHelper::UTC(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 hour,
    /* [in] */ Int32 minute,
    /* [in] */ Int32 second,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    *value = CDate::UTC(year, month, day, hour, minute, second);
    return NOERROR;
}

} // Utility
} // Elastos
