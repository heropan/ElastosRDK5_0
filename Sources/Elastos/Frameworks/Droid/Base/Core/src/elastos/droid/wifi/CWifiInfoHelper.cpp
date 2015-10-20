
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/net/wifi/CWifiInfoHelper.h"
#include "elastos/droid/net/wifi/CWifiInfo.h"

namespace Elastos {
namespace Droid {
namespace Wifi {

ECode CWifiInfoHelper::GetDetailedStateOf(
    /* [in] */ SupplicantState suppState,
    /* [out] */ NetworkInfoDetailedState* detailedState)
{
    VALIDATE_NOT_NULL(detailedState);
    *detailedState = CWifiInfo::GetDetailedStateOf(suppState);
    return NOERROR;
}

ECode CWifiInfoHelper::RemoveDoubleQuotes(
    /* [in] */ const String& str,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    CWifiInfo::RemoveDoubleQuotes(str, value);
    return NOERROR;
}

} // namespace Wifi
} // namespace Droid
} // namespace Elastos
