
#include "elastos/droid/net/wifi/SupplicantStateHelper.h"

namespace Elastos {
namespace Droid {
namespace Wifi {

Boolean SupplicantStateHelper::IsHandshakeState(
    /* [in] */ SupplicantState state)
{
    switch(state) {
        case SupplicantState_AUTHENTICATING:
        case SupplicantState_ASSOCIATING:
        case SupplicantState_ASSOCIATED:
        case SupplicantState_FOUR_WAY_HANDSHAKE:
        case SupplicantState_GROUP_HANDSHAKE:
            return TRUE;
        case SupplicantState_COMPLETED:
        case SupplicantState_DISCONNECTED:
        case SupplicantState_INTERFACE_DISABLED:
        case SupplicantState_INACTIVE:
        case SupplicantState_SCANNING:
        case SupplicantState_DORMANT:
        case SupplicantState_UNINITIALIZED:
        case SupplicantState_INVALID:
            return FALSE;
        default:
            // throw new IllegalArgumentException("Unknown supplicant state");
            assert(0);
            return FALSE;
    }
}

Boolean SupplicantStateHelper::IsConnecting(
    /* [in] */ SupplicantState state)
{
    switch(state) {
        case SupplicantState_AUTHENTICATING:
        case SupplicantState_ASSOCIATING:
        case SupplicantState_ASSOCIATED:
        case SupplicantState_FOUR_WAY_HANDSHAKE:
        case SupplicantState_GROUP_HANDSHAKE:
        case SupplicantState_COMPLETED:
            return TRUE;
        case SupplicantState_DISCONNECTED:
        case SupplicantState_INTERFACE_DISABLED:
        case SupplicantState_INACTIVE:
        case SupplicantState_SCANNING:
        case SupplicantState_DORMANT:
        case SupplicantState_UNINITIALIZED:
        case SupplicantState_INVALID:
            return FALSE;
        default:
            // throw new IllegalArgumentException("Unknown supplicant state");
            assert(0);
            return FALSE;
    }
}

Boolean SupplicantStateHelper::IsDriverActive(
    /* [in] */ SupplicantState state)
{
    switch(state) {
        case SupplicantState_DISCONNECTED:
        case SupplicantState_DORMANT:
        case SupplicantState_INACTIVE:
        case SupplicantState_AUTHENTICATING:
        case SupplicantState_ASSOCIATING:
        case SupplicantState_ASSOCIATED:
        case SupplicantState_SCANNING:
        case SupplicantState_FOUR_WAY_HANDSHAKE:
        case SupplicantState_GROUP_HANDSHAKE:
        case SupplicantState_COMPLETED:
            return TRUE;
        case SupplicantState_INTERFACE_DISABLED:
        case SupplicantState_UNINITIALIZED:
        case SupplicantState_INVALID:
            return FALSE;
        default:
            // throw new IllegalArgumentException("Unknown supplicant state");
            assert(0);
            return FALSE;
    }
}

} // namespace Wifi
} // namespace Droid
} // namespace Elastos
