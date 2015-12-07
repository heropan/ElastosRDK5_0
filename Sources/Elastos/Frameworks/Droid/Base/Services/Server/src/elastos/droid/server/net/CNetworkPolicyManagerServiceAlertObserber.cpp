
#include "elastos/droid/net/CNetworkPolicyManagerServiceAlertObserber.h"
#include "CNetworkManagementService.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

ECode CNetworkPolicyManagerServiceAlertObserber::InterfaceStatusChanged(
    /* [in] */ const String& iface,
    /* [in] */ Boolean up)
{
    return BaseNetworkObserver::InterfaceStatusChanged(iface, up);
}

ECode CNetworkPolicyManagerServiceAlertObserber::InterfaceLinkStateChanged(
    /* [in] */ const String& iface,
    /* [in] */ Boolean up)
{
    return BaseNetworkObserver::InterfaceLinkStateChanged(iface, up);
}

ECode CNetworkPolicyManagerServiceAlertObserber::InterfaceAdded(
    /* [in] */ const String& iface)
{
    return BaseNetworkObserver::InterfaceAdded(iface);
}

ECode CNetworkPolicyManagerServiceAlertObserber::InterfaceRemoved(
    /* [in] */ const String& iface)
{
    return BaseNetworkObserver::InterfaceRemoved(iface);
}

ECode CNetworkPolicyManagerServiceAlertObserber::LimitReached(
    /* [in] */ const String& limitName,
    /* [in] */ const String& iface)
{
    // only someone like NMS should be calling us
    FAIL_RETURN(mHost->mContext->EnforceCallingOrSelfPermission(
            String("android.permission.CONNECTIVITY_INTERNAL"), CNetworkPolicyManagerService::TAG));

    if (!CNetworkManagementService::LIMIT_GLOBAL_ALERT.Equals(limitName)) {
        AutoPtr<ICharSequence> seq;
        CString::New(iface, (ICharSequence**)&seq);
        AutoPtr<IMessage> msg;
        mHost->mHandler->ObtainMessage(
            CNetworkPolicyManagerService::MSG_LIMIT_REACHED,
            (IMessage**)&msg);
        msg->SetObj(seq);
        Boolean result;
        return mHost->mHandler->SendMessage(msg, &result);
    }
    return NOERROR;
}

ECode CNetworkPolicyManagerServiceAlertObserber::InterfaceClassDataActivityChanged(
    /* [in] */ const String& label,
    /* [in] */ Boolean active)
{
    return BaseNetworkObserver::InterfaceClassDataActivityChanged(label, active);
}

ECode CNetworkPolicyManagerServiceAlertObserber::constructor(
    /* [in] */ Handle32 host)
{
    mHost = (CNetworkPolicyManagerService*)host;
    return NOERROR;
}

} // namespace Net
} // namespace Server
} // namepsace Droid
} // namespace Elastos
