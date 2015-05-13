
#include "connectivity/CVpnObserver.h"
#include "connectivity/Vpn.h"
#include "os/Binder.h"

using Elastos::Droid::Net::NetworkInfoDetailedState_DISCONNECTED;
using Elastos::Droid::Os::Binder;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Connectivity {

ECode CVpnObserver::constructor(
    /* [in] */ Handle32 owner)
{
    mOwner = (Vpn*)owner;
    return NOERROR;
}

ECode CVpnObserver::InterfaceStatusChanged(
    /* [in] */ const String& interfaze,
    /* [in] */ Boolean up)
{
    Mutex::Autolock lock(mOwner->mLock);
    if (!up && mOwner->mLegacyVpnRunner != NULL) {
        mOwner->mLegacyVpnRunner->Check(interfaze);
    }
    return NOERROR;
}

ECode CVpnObserver::InterfaceLinkStateChanged(
    /* [in] */ const String& interfaze,
    /* [in] */ Boolean up)
{
    return NOERROR;
}

ECode CVpnObserver::InterfaceAdded(
    /* [in] */ const String& interfaze)
{
    return NOERROR;
}

ECode CVpnObserver::InterfaceRemoved(
    /* [in] */ const String& interfaze)
{
    {
        Mutex::Autolock lock(mOwner->mLock);
        if (interfaze.Equals(mOwner->mInterface) && mOwner->NativeCheck(interfaze) == 0) {
            Int64 token = Binder::ClearCallingIdentity();
            // try {
            mOwner->mCallback->Restore();
            mOwner->HideNotification();
            // } finally {
            Binder::RestoreCallingIdentity(token);
            // }
            mOwner->mInterface = NULL;
            if (mOwner->mConnection != NULL) {
                mOwner->mContext->UnbindService(mOwner->mConnection);
                mOwner->mConnection = NULL;
                mOwner->UpdateState(NetworkInfoDetailedState_DISCONNECTED, String("interfaceRemoved"));
            }
            else if (mOwner->mLegacyVpnRunner != NULL) {
                mOwner->mLegacyVpnRunner->Exit();
                mOwner->mLegacyVpnRunner = NULL;
            }
        }
    }
    return NOERROR;
}

/**
 * A networking quota limit has been reached. The quota might not
 * be specific to an interface.
 *
 * @param limitName The name of the limit that triggered.
 * @param iface The interface on which the limit was detected.
 */
ECode CVpnObserver::LimitReached(
    /* [in] */ const String& limitName,
    /* [in] */ const String& interfaze)
{
    return NOERROR;
}

/**
 * Interface data activity status is changed.
 *
 * @param iface The interface.
 * @param active  True if the interface is actively transmitting data, false if it is idle.
 */
ECode CVpnObserver::InterfaceClassDataActivityChanged(
    /* [in] */ const String& label,
    /* [in] */ Boolean active)
{
    return NOERROR;
}

ECode CVpnObserver::ToString(
    /* [out] */ String* str)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

} // Connectivity
} // Server
} // Droid
} // Elastos
