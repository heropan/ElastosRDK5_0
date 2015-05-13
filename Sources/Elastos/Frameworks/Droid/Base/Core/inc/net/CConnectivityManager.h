
#ifndef __ELASTOS_DROID_NET_CCONNECTIVITYMANAGER_H__
#define __ELASTOS_DROID_NET_CCONNECTIVITYMANAGER_H__

#include "ext/frameworkext.h"
#include "_CConnectivityManager.h"

using Elastos::Net::IInetAddress;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CConnectivityManager)
{
public:
    static CARAPI IsNetworkTypeValid(
        /* [in] */ Int32 networkType,
        /* [out] */ Boolean* result);

    static CARAPI  GetNetworkTypeName(
        /* [in] */ Int32 networkType,
        /* [out] */ String* result);

    static CARAPI  IsNetworkTypeMobile(
        /* [in] */ Int32 networkType,
        /* [out] */ Boolean* result);

    CARAPI constructor(
        /* [in] */ IIConnectivityManager* service);

    CARAPI  SetNetworkPreference(
        /* [in] */ Int32 preference);

    CARAPI  GetNetworkPreference(
        /* [out] */ Int32* result);

    CARAPI  GetActiveNetworkInfo(
        /* [out] */ INetworkInfo** result);

    CARAPI  GetActiveNetworkInfoForUid(
        /* [in] */ Int32 uid,
        /* [out] */ INetworkInfo** result);

    CARAPI  GetNetworkInfo(
        /* [in] */ Int32 networkType,
        /* [out] */ INetworkInfo** result);

    CARAPI  GetAllNetworkInfo(
        /* [out, callee] */ ArrayOf<INetworkInfo*>** result);

    CARAPI  GetActiveLinkProperties(
        /* [out] */ ILinkProperties** result);

    CARAPI  GetLinkProperties(
        /* [in] */ Int32 networkType,
        /* [out] */ ILinkProperties** result);

    CARAPI  SetRadios(
        /* [in] */ Boolean turnOn,
        /* [out] */ Boolean* result);

    CARAPI  SetRadio(
        /* [in] */ Int32 networkType,
        /* [in] */ Boolean turnOn,
        /* [out] */ Boolean* result);

    CARAPI  StartUsingNetworkFeature(
        /* [in] */ Int32 networkType,
        /* [in] */ const String& feature,
        /* [out] */ Int32* result);

    CARAPI  StopUsingNetworkFeature(
        /* [in] */ Int32 networkType,
        /* [in] */ const String& feature,
        /* [out] */ Int32* result);

    CARAPI  RequestRouteToHost(
        /* [in] */ Int32 networkType,
        /* [in] */ Int32 hostAddress,
        /* [out] */ Boolean* result);

    CARAPI  RequestRouteToHostAddress(
        /* [in] */ Int32 networkType,
        /* [in] */ IInetAddress* hostAddress,
        /* [out] */ Boolean* result);

    CARAPI  GetBackgroundDataSetting(
        /* [out] */ Boolean* result);

    CARAPI  SetBackgroundDataSetting(
        /* [in] */ Boolean allowBackgroundData);

    CARAPI  GetActiveNetworkQuotaInfo(
        /* [out] */ INetworkQuotaInfo** result);

    CARAPI  GetMobileDataEnabled(
        /* [out] */ Boolean* result);

    CARAPI  SetMobileDataEnabled(
        /* [in] */ Boolean enabled);

    static CARAPI From(
        /* [in] */ IContext* ctx,
        /* [out] */ IConnectivityManager** result);

    CARAPI  GetTetherableIfaces(
        /* [out, callee] */ ArrayOf<String>** result);

    CARAPI  GetTetheredIfaces(
        /* [out, callee] */ ArrayOf<String>** result);

    CARAPI  GetTetheringErroredIfaces(
        /* [out, callee] */ ArrayOf<String>** result);

    CARAPI   Tether(
        /* [in] */ const String& iface,
        /* [out] */ Int32* result);

    CARAPI  Untether(
        /* [in] */ const String& iface,
        /* [out] */ Int32* result);

   CARAPI  IsTetheringSupported(
        /* [out] */ Boolean* result);

    CARAPI  GetTetherableUsbRegexs(
        /* [out, callee] */ ArrayOf<String>** result);

    CARAPI  GetTetherableWifiRegexs(
        /* [out, callee] */ ArrayOf<String>** result);

    CARAPI  GetTetherableBluetoothRegexs(
        /* [out, callee] */ ArrayOf<String>** result);

    CARAPI  SetUsbTethering(
        /* [in] */ Boolean enabled,
        /* [out] */ Int32* result);

    CARAPI  GetLastTetherError(
        /* [in] */ const String& iface,
        /* [out] */ Int32* result);

    CARAPI  RequestNetworkTransitionWakelock(
        /* [in] */ const String& forWhom,
        /* [out] */ Boolean* result);

    CARAPI  ReportInetCondition(
        /* [in] */ Int32 networkType,
        /* [in] */ Int32 percentage);

    CARAPI  SetGlobalProxy(
        /* [in] */ IProxyProperties* p);

    CARAPI  GetGlobalProxy(
        /* [out] */ IProxyProperties** p);

    CARAPI  GetProxy(
        /* [out] */ IProxyProperties** p);

    CARAPI  SetDataDependency(
        /* [in] */ Int32 networkType,
        /* [in] */ Boolean met);

    CARAPI  IsNetworkSupported(
        /* [in] */ Int32 networkType,
        /* [out] */ Boolean* result);

    CARAPI  IsActiveNetworkMetered(
        /* [out] */ Boolean* result);

    //CARAPI  SendMessage(
    //    /* [in] */ Int32 what,
    //   /* [in] */ INetworkInfo* info);

    CARAPI  UpdateLockdownVpn(
        /* [out] */ Boolean* result);

    CARAPI  CaptivePortalCheckComplete(
        /* [in] */ INetworkInfo* info);

private:
    static const String TAG;
    AutoPtr<IIConnectivityManager> mService;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CCONNECTIVITYMANAGER_H__

