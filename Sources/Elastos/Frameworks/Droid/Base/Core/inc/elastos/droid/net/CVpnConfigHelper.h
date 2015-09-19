
#ifndef __ELASTOS_DROID_INTERNAL_NET_CVPNCONFIGHELPER_H__
#define __ELASTOS_DROID_INTERNAL_NET_CVPNCONFIGHELPER_H__

#include "_Elastos_Droid_Internal_Net_CVpnConfigHelper.h"

using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Net {

CarClass(CVpnConfigHelper)
{
public:
    CARAPI GetIntentForConfirmation(
        /* [out] */ IIntent** intent);

    CARAPI GetIntentForStatusPanel(
        /* [in] */ IContext* context,
        /* [in] */ IVpnConfig* config,
        /* [out] */ IPendingIntent** intent);
};

} // namespace Net
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_NET_CVPNCONFIGHELPER_H__
