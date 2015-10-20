
#include "elastos/droid/net/wifi/CWifiConfigurationHelper.h"
#include "elastos/droid/net/wifi/CWifiConfiguration.h"

namespace Elastos {
namespace Droid {
namespace Wifi {

ECode CWifiConfigurationHelper::GetWepKeyVarName(
    /* [in] */ Int32 index,
    /* [out] */ String* wepKeyVarName)
{
    return CWifiConfiguration::GetWepKeyVarName(index, wepKeyVarName);
}

} // namespace Wifi
} // namespace Droid
} // namespace Elastos