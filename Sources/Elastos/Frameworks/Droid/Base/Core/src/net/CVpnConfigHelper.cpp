
#include "net/CVpnConfigHelper.h"
#include "net/CVpnConfig.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Net {

ECode CVpnConfigHelper::GetIntentForConfirmation(
    /* [out] */ IIntent** intent)
{
    return CVpnConfig::GetIntentForConfirmation(intent);
}

ECode CVpnConfigHelper::GetIntentForStatusPanel(
    /* [in] */ IContext* context,
    /* [in] */ IVpnConfig* config,
    /* [out] */ IPendingIntent** intent)
{
    return CVpnConfig::GetIntentForStatusPanel(context, config, intent);
}

} // namespace Net
} // namespace Internal
} // namespace Droid
} // namespace Elastos
