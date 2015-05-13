
#ifndef __ELASTOS_DROID_NET_BASENETWORKSTATETRACKER_H__
#define __ELASTOS_DROID_NET_BASENETWORKSTATETRACKER_H__

#include "ext/frameworkdef.h"
#include "net/NetworkStateTracker.h"

using Elastos::Utility::Concurrent::Atomic::IAtomicBoolean;

namespace Elastos {
namespace Droid {
namespace Net {

class BaseNetworkStateTracker
    : public ElRefBase
    , public INetworkStateTracker
{
public:
    BaseNetworkStateTracker(
        /* [in] */ Int32 networkType);

    CAR_INTERFACE_DECL();

    CARAPI StartMonitoring(
        /* [in] */ IContext* context,
        /* [in] */ IHandler* target);

    CARAPI GetNetworkInfo(
        /* [out] */ INetworkInfo** info);

    CARAPI GetLinkProperties(
        /* [out] */ ILinkProperties** result);

    CARAPI GetLinkCapabilities(
        /* [out] */ ILinkCapabilities** result);

    CARAPI CaptivePortalCheckComplete();

    CARAPI SetRadio(
        /* [in] */ Boolean turnOn,
        /* [out] */ Boolean* result);

    CARAPI IsAvailable(
        /* [out] */ Boolean* result);

    CARAPI SetUserDataEnable(
        /* [in] */ Boolean enabled);

    CARAPI SetPolicyDataEnable(
        /* [in] */ Boolean enabled);

    CARAPI IsPrivateDnsRouteSet(
        /* [out] */ Boolean* result);

    CARAPI SetPrivateDnsRoute(
        /* [in] */ Boolean enabled);

    CARAPI IsDefaultRouteSet(
        /* [out] */ Boolean* result);

    CARAPI SetDefaultRoute(
        /* [in] */ Boolean enabled);

    CARAPI IsTeardownRequested(
        /* [out] */ Boolean* result);

    CARAPI SetTeardownRequested(
        /* [in] */ Boolean isRequested);

    CARAPI SetDependencyMet(
        /* [in] */ Boolean met);

protected:
    CARAPI_(AutoPtr<IHandler>) GetTargetHandler();

    CARAPI_(void) DispatchStateChanged();

    CARAPI_(void) DispatchConfigurationChanged();

    virtual CARAPI_(void) StartMonitoringInternal() = 0;

public:
    // TODO: better document threading expectations
    // TODO: migrate to make NetworkStateTracker abstract class
    static const String PROP_TCP_BUFFER_UNKNOWN;
    static const String PROP_TCP_BUFFER_WIFI;

protected:
    AutoPtr<IContext> mContext;
    AutoPtr<INetworkInfo> mNetworkInfo;
    AutoPtr<ILinkProperties> mLinkProperties;
    AutoPtr<ILinkCapabilities> mLinkCapabilities;

private:
    AutoPtr<IHandler> mTarget;
    AutoPtr<IAtomicBoolean> mTeardownRequested;
    AutoPtr<IAtomicBoolean> mPrivateDnsRouteSet;
    AutoPtr<IAtomicBoolean> mDefaultRouteSet;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_BASENETWORKSTATETRACKER_H__
