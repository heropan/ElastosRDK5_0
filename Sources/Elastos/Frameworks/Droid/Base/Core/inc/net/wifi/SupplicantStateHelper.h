
#ifndef __SUPPLICANTSTATEHELPER_H__
#define __SUPPLICANTSTATEHELPER_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

class SupplicantStateHelper
{
public:
    /** Supplicant associating or authenticating is considered a handshake state {@hide} */
    static CARAPI_(Boolean) IsHandshakeState(
        /* [in] */ SupplicantState state);

    static CARAPI_(Boolean) IsConnecting(
        /* [in] */ SupplicantState state);

    static CARAPI_(Boolean) IsDriverActive(
        /* [in] */ SupplicantState state);
};

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //__SUPPLICANTSTATEHELPER_H__
