
#include "net/wifi/CWifiConfigurationHelper.h"
#include "net/wifi/CWifiConfiguration.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

ECode CWifiConfigurationHelper::GetWepKeyVarName(
    /* [in] */ Int32 index,
    /* [out] */ String* wepKeyVarName)
{
    return CWifiConfiguration::GetWepKeyVarName(index, wepKeyVarName);
}

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos