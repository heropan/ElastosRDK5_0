
#include "elastos/droid/net/CNetworkStatsServiceAlertObserber.h"
#include "CNetworkManagementService.h"
#include "elastos/droid/os/Handler.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

ECode CNetworkStatsServiceAlertObserber::InterfaceStatusChanged(
    /* [in] */ const String& iface,
    /* [in] */ Boolean up)
{
    return BaseNetworkObserver::InterfaceStatusChanged(iface, up);
}

ECode CNetworkStatsServiceAlertObserber::InterfaceLinkStateChanged(
    /* [in] */ const String& iface,
    /* [in] */ Boolean up)
{
    return BaseNetworkObserver::InterfaceLinkStateChanged(iface, up);
}

ECode CNetworkStatsServiceAlertObserber::InterfaceAdded(
    /* [in] */ const String& iface)
{
    return BaseNetworkObserver::InterfaceAdded(iface);
}

ECode CNetworkStatsServiceAlertObserber::InterfaceRemoved(
    /* [in] */ const String& iface)
{
    return BaseNetworkObserver::InterfaceRemoved(iface);
}

ECode CNetworkStatsServiceAlertObserber::LimitReached(
    /* [in] */ const String& limitName,
    /* [in] */ const String& iface)
{
    // only someone like NMS should be calling us
    FAIL_RETURN(mHost->mContext->EnforceCallingOrSelfPermission(
            String("android.permission.CONNECTIVITY_INTERNAL"), CNetworkStatsService::TAG));

    if (CNetworkManagementService::LIMIT_GLOBAL_ALERT.Equals(limitName)) {
        // kick off background poll to collect network stats; UID stats
        // are handled during normal polling interval.
        Int32 flags = CNetworkStatsService::FLAG_PERSIST_NETWORK;
        AutoPtr<IMessage> msg;
        mHost->mHandler->ObtainMessage(
            CNetworkStatsService::MSG_PERFORM_POLL, (IMessage**)&msg);
        msg->SetArg1(flags);
        Boolean result;
        mHost->mHandler->SendMessage(msg, &result);

        // re-arm global alert for next update
        mHost->mHandler->SendEmptyMessage(
            CNetworkStatsService::MSG_REGISTER_GLOBAL_ALERT, &result);
    }
    return NOERROR;
}

ECode CNetworkStatsServiceAlertObserber::InterfaceClassDataActivityChanged(
    /* [in] */ const String& label,
    /* [in] */ Boolean active)
{
    return BaseNetworkObserver::InterfaceClassDataActivityChanged(label, active);
}

ECode CNetworkStatsServiceAlertObserber::constructor(
    /* [in] */ Handle32 host)
{
    mHost = (CNetworkStatsService*)host;
    return NOERROR;
}

} // namespace Net
} // namespace Server
} // namepsace Droid
} // namespace Elastos
