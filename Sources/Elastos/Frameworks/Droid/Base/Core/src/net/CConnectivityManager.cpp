
#include "ext/frameworkext.h"
#include "net/CConnectivityManager.h"
#include "net/NetworkUtils.h"
#include "os/CBinder.h"
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Droid::Net::NetworkUtils;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::CBinder;

namespace Elastos {
namespace Droid {
namespace Net {

const String CConnectivityManager::TAG("ConnectivityManager");

ECode CConnectivityManager::IsNetworkTypeValid(
    /* [in] */ Int32 networkType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ((networkType >= 0) && (networkType <= MAX_NETWORK_TYPE));
    return NOERROR;
}

/** {@hide} */
ECode CConnectivityManager::GetNetworkTypeName(
    /* [in] */ Int32 type,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    switch (type) {
        case TYPE_MOBILE:
            *result = "MOBILE";
            break;
        case TYPE_WIFI:
            *result = "WIFI";
            break;
        case TYPE_MOBILE_MMS:
            *result = "MOBILE_MMS";
            break;
        case TYPE_MOBILE_SUPL:
            *result = "MOBILE_SUPL";
            break;
        case TYPE_MOBILE_DUN:
            *result = "MOBILE_DUN";
            break;
        case TYPE_MOBILE_HIPRI:
            *result = String("MOBILE_HIPRI");
            break;
        case TYPE_WIMAX:
            *result = String("WIMAX");
            break;
        case TYPE_BLUETOOTH:
            *result = String("BLUETOOTH");
            break;
        case TYPE_DUMMY:
            *result = String("DUMMY");
            break;
        case TYPE_ETHERNET:
            *result = String("ETHERNET");
            break;
        case TYPE_MOBILE_FOTA:
            *result = String("MOBILE_FOTA");
            break;
        case TYPE_MOBILE_IMS:
            *result = String("MOBILE_IMS");
            break;
        case TYPE_MOBILE_CBS:
            *result = String("MOBILE_CBS");
            break;
        case TYPE_WIFI_P2P:
            *result = String("WIFI_P2P");
            break;
        default:
            *result = StringUtils::Int32ToString(type);
            break;
    }
    return NOERROR;
}

/** {@hide} */
ECode CConnectivityManager::IsNetworkTypeMobile(
    /* [in] */ Int32 networkType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    switch (networkType) {
        case TYPE_MOBILE:
        case TYPE_MOBILE_MMS:
        case TYPE_MOBILE_SUPL:
        case TYPE_MOBILE_DUN:
        case TYPE_MOBILE_HIPRI:
        case TYPE_MOBILE_FOTA:
        case TYPE_MOBILE_IMS:
        case TYPE_MOBILE_CBS:
            *result = TRUE;
            break;
        default:
            *result = FALSE;
            break;
    }
    return NOERROR;
}

ECode CConnectivityManager::SetNetworkPreference(
    /* [in] */ Int32 preference)
{
    mService->SetNetworkPreference(preference);
    return NOERROR;
}

ECode CConnectivityManager::GetNetworkPreference(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mService->GetNetworkPreference(result);
    if (FAILED(ec)) *result = -1;
    return NOERROR;
}

/**
 * Returns details about the currently active data network. When connected,
 * this network is the default route for outgoing connections. You should
 * always check {@link NetworkInfo#isConnected()} before initiating network
 * traffic. This may return {@code NULL} when no networks are available.
 * <p>This method requires the caller to hold the permission
 * {@link android.Manifest.permission#ACCESS_NETWORK_STATE}.
 */
ECode CConnectivityManager::GetActiveNetworkInfo(
    /* [out] */ INetworkInfo** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<INetworkInfo> networkInfo;
    ECode ec = mService->GetActiveNetworkInfo((INetworkInfo**)&networkInfo);
    if (FAILED(ec)) networkInfo = NULL;
    *result = networkInfo;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/** {@hide} */
ECode CConnectivityManager::GetActiveNetworkInfoForUid(
    /* [in] */ Int32 uid,
    /* [out] */ INetworkInfo** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<INetworkInfo> networkInfo;
    ECode ec = mService->GetActiveNetworkInfoForUid(uid, (INetworkInfo**)&networkInfo);
    if (FAILED(ec)) networkInfo = NULL;
    *result = networkInfo;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetNetworkInfo(
    /* [in] */ Int32 networkType,
    /* [out] */ INetworkInfo** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<INetworkInfo> networkInfo;
    ECode ec = mService->GetNetworkInfo(networkType, (INetworkInfo**)&networkInfo);
    if (FAILED(ec)) networkInfo = NULL;
    *result = networkInfo;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetAllNetworkInfo(
    /* [out, callee] */ ArrayOf<INetworkInfo*>** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr< ArrayOf<INetworkInfo*> > outputarray;
    ECode ec = mService->GetAllNetworkInfo((ArrayOf<INetworkInfo*>**)&outputarray);
    if (FAILED(ec)) outputarray = NULL;
    *result = outputarray;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

/** {@hide} */
ECode CConnectivityManager::GetActiveLinkProperties(
    /* [out] */ ILinkProperties** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ILinkProperties> linkp;
    ECode ec = mService->GetActiveLinkProperties((ILinkProperties**)&linkp);
    if (FAILED(ec)) linkp = NULL;
    *result = linkp;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/** {@hide} */
ECode CConnectivityManager::GetLinkProperties(
    /* [in] */ Int32 networkType,
    /* [out] */ ILinkProperties** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ILinkProperties> linkp;
    ECode ec = mService->GetLinkProperties(networkType, (ILinkProperties**)&linkp);
    if (FAILED(ec)) linkp = NULL;
    *result = linkp;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/** {@hide} */
ECode CConnectivityManager::SetRadios(
    /* [in] */ Boolean turnOn,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mService->SetRadios(turnOn, result);
    if (FAILED(ec)) *result = FALSE;
    return NOERROR;
}

/** {@hide} */
ECode CConnectivityManager::SetRadio(
    /* [in] */ Int32 networkType,
    /* [in] */ Boolean turnOn,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mService->SetRadio(networkType, turnOn, result);
    if (FAILED(ec)) *result = FALSE;
    return NOERROR;
}

/**
 * Tells the underlying networking system that the caller wants to
 * begin using the named feature. The interpretation of {@code feature}
 * is completely up to each networking implementation.
 * <p>This method requires the caller to hold the permission
 * {@link android.Manifest.permission#CHANGE_NETWORK_STATE}.
 * @param networkType specifies which network the request pertains to
 * @param feature the name of the feature to be used
 * @return an integer value representing the outcome of the request.
 * The interpretation of this value is specific to each networking
 * implementation+feature combination, except that the value {@code -1}
 * always indicates failure.
 */
ECode CConnectivityManager::StartUsingNetworkFeature(
    /* [in] */ Int32 networkType,
    /* [in] */ const String& feature,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IBinder> binder;
    CBinder::New((IBinder**)&binder);
    ECode ec = mService->StartUsingNetworkFeature(networkType, feature, binder, result);
    if (FAILED(ec)) *result = -1;
    return NOERROR;
}

/**
 * Tells the underlying networking system that the caller is finished
 * using the named feature. The interpretation of {@code feature}
 * is completely up to each networking implementation.
 * <p>This method requires the caller to hold the permission
 * {@link android.Manifest.permission#CHANGE_NETWORK_STATE}.
 * @param networkType specifies which network the request pertains to
 * @param feature the name of the feature that is no longer needed
 * @return an integer value representing the outcome of the request.
 * The interpretation of this value is specific to each networking
 * implementation+feature combination, except that the value {@code -1}
 * always indicates failure.
 */
ECode CConnectivityManager::StopUsingNetworkFeature(
    /* [in] */ Int32 networkType,
    /* [in] */ const String& feature,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mService->StopUsingNetworkFeature(networkType, feature, result);
    if (FAILED(ec)) *result = -1;
    return NOERROR;
}

/**
 * Ensure that a network route exists to deliver traffic to the specified
 * host via the specified network interface. An attempt to add a route that
 * already exists is ignored, but treated as successful.
 * <p>This method requires the caller to hold the permission
 * {@link android.Manifest.permission#CHANGE_NETWORK_STATE}.
 * @param networkType the type of the network over which traffic to the specified
 * host is to be routed
 * @param hostAddress the IP address of the host to which the route is desired
 * @return {@code TRUE} on success, {@code FALSE} on failure
 */
ECode CConnectivityManager::RequestRouteToHost(
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 hostAddress,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IInetAddress> inetAddress;
    NetworkUtils::Int32ToInetAddress(hostAddress,(IInetAddress**)&inetAddress);
    if (inetAddress == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    return RequestRouteToHostAddress(networkType, inetAddress, result);
}

/**
 * Ensure that a network route exists to deliver traffic to the specified
 * host via the specified network interface. An attempt to add a route that
 * already exists is ignored, but treated as successful.
 * @param networkType the type of the network over which traffic to the specified
 * host is to be routed
 * @param hostAddress the IP address of the host to which the route is desired
 * @return {@code TRUE} on success, {@code FALSE} on failure
 * @hide
 */
ECode CConnectivityManager::RequestRouteToHostAddress(
    /* [in] */ Int32 networkType,
    /* [in] */ IInetAddress* hostAddress,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr< ArrayOf<Byte> > address;
    hostAddress->GetAddress((ArrayOf<Byte>**)&address);
    ECode ec = mService->RequestRouteToHostAddress(networkType, address, result);
    if (FAILED(ec)) *result = FALSE;
    return NOERROR;
}

/**
 * Returns the value of the setting for background data usage. If FALSE,
 * applications should not use the network if the application is not in the
 * foreground. Developers should respect this setting, and check the value
 * of this before performing any background data operations.
 * <p>
 * All applications that have background services that use the network
 * should listen to {@link #ACTION_BACKGROUND_DATA_SETTING_CHANGED}.
 * <p>
 * @deprecated As of {@link VERSION_CODES#ICE_CREAM_SANDWICH}, availability of
 * background data depends on several combined factors, and this method will
 * always return {@code TRUE}. Instead, when background data is unavailable,
 * {@link #getActiveNetworkInfo()} will now appear disconnected.
 *
 * @return Whether background data usage is allowed.
 */
//@Deprecated
ECode CConnectivityManager::GetBackgroundDataSetting(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

/**
 * Sets the value of the setting for background data usage.
 *
 * @param allowBackgroundData Whether an application should use data while
 *            it is in the background.
 *
 * @attr ref android.Manifest.permission#CHANGE_BACKGROUND_DATA_SETTING
 * @see #getBackgroundDataSetting()
 * @hide
 */
//@Deprecated
ECode CConnectivityManager::SetBackgroundDataSetting(
    /* [in] */ Boolean allowBackgroundData)
{
    return NOERROR;
}

/**
 * Return quota status for the current active network, or {@code NULL} if no
 * network is active. Quota status can change rapidly, so these values
 * shouldn't be cached.
 *
 * @hide
 */
ECode CConnectivityManager::GetActiveNetworkQuotaInfo(
    /* [out] */ INetworkQuotaInfo** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<INetworkQuotaInfo> quotaInfo;
    ECode ec = mService->GetActiveNetworkQuotaInfo((INetworkQuotaInfo**)&quotaInfo);
    if (FAILED(ec)) quotaInfo = NULL;
    *result = quotaInfo;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/**
 * Gets the value of the setting for enabling Mobile data.
 *
 * @return Whether mobile data is enabled.
 * @hide
 */
ECode CConnectivityManager::GetMobileDataEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mService->GetMobileDataEnabled(result);
    if (FAILED(ec)) *result = TRUE;
    return NOERROR;
}

/**
 * Sets the persisted value for enabling/disabling Mobile data.
 *
 * @param enabled Whether the mobile data connection should be
 *            used or not.
 * @hide
 */
ECode CConnectivityManager::SetMobileDataEnabled(
    /* [in] */ Boolean enabled)
{
    mService->SetMobileDataEnabled(enabled);
    return NOERROR;
}

/**
 * {@hide}
 */
ECode CConnectivityManager::constructor(
    /* [in] */ IIConnectivityManager* service)
{
    VALIDATE_NOT_NULL(service);
    mService = service;
    return NOERROR;
}

/** {@hide} */
ECode CConnectivityManager::From(
    /* [in] */ IContext* ctx,
    /*[out] */ IConnectivityManager** result)
{
   VALIDATE_NOT_NULL(result);
   ASSERT_SUCCEEDED(ctx->GetSystemService(IContext::CONNECTIVITY_SERVICE, (IInterface**)&result));
   return NOERROR;
}

/**
 * {@hide}
 */
ECode CConnectivityManager::GetTetherableIfaces(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr <ArrayOf<String> > outputArray;
    ECode ec = mService->GetTetherableIfaces((ArrayOf<String>**)&outputArray);
    if (FAILED(ec)) outputArray = ArrayOf<String>::Alloc(0);
    *result = outputArray;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

/**
 * {@hide}
 */
ECode CConnectivityManager::GetTetheredIfaces(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr <ArrayOf<String> > outputArray;
    ECode ec = mService->GetTetheredIfaces((ArrayOf<String>**)&outputArray);
    if (FAILED(ec)) outputArray = ArrayOf<String>::Alloc(0);
    *result = outputArray;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

/**
 * {@hide}
 */
ECode CConnectivityManager::GetTetheringErroredIfaces(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr <ArrayOf<String> > outputArray;
    ECode ec = mService->GetTetheringErroredIfaces((ArrayOf<String>**)&outputArray);
    if (FAILED(ec)) outputArray = ArrayOf<String>::Alloc(0);
    *result = outputArray;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

/**
 * @return error A TETHER_ERROR value indicating success or failure type
 * {@hide}
 */
ECode  CConnectivityManager::Tether(
    /* [in] */ const String& iface,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mService->Tether(iface, result);
    if (FAILED(ec)) *result = TETHER_ERROR_SERVICE_UNAVAIL;
    return NOERROR;
}

/**
 * @return error A TETHER_ERROR value indicating success or failure type
 * {@hide}
 */
ECode CConnectivityManager::Untether(
    /* [in] */ const String& iface,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mService->Untether(iface, result);
    if (FAILED(ec)) *result = TETHER_ERROR_SERVICE_UNAVAIL;
    return NOERROR;
}

/**
 * {@hide}
 */

ECode CConnectivityManager::IsTetheringSupported(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mService->IsTetheringSupported(result);
    if (FAILED(ec)) *result = FALSE;
    return NOERROR;
}

/**
 * {@hide}
 */
ECode CConnectivityManager::GetTetherableUsbRegexs(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr <ArrayOf<String> > outputArray;
    ECode ec = mService->GetTetherableUsbRegexs((ArrayOf<String>**)&outputArray);
    if (FAILED(ec)) outputArray = ArrayOf<String>::Alloc(0);
    *result = outputArray;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

/**
 * {@hide}
 */
ECode CConnectivityManager::GetTetherableWifiRegexs(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr <ArrayOf<String> > outputArray;
    ECode ec = mService->GetTetherableWifiRegexs((ArrayOf<String>**)&outputArray);
    if (FAILED(ec)) outputArray = ArrayOf<String>::Alloc(0);
    *result = outputArray;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

/**
 * {@hide}
 */
ECode CConnectivityManager::GetTetherableBluetoothRegexs(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr <ArrayOf<String> > outputArray;
    ECode ec = mService->GetTetherableBluetoothRegexs((ArrayOf<String>**)&outputArray);
    if (FAILED(ec)) outputArray = ArrayOf<String>::Alloc(0);
    *result = outputArray;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

/**
 * {@hide}
 */
ECode CConnectivityManager::SetUsbTethering(
    /* [in] */ Boolean enabled,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mService->SetUsbTethering(enabled, result);
    if (FAILED(ec)) *result = TETHER_ERROR_SERVICE_UNAVAIL;
    return NOERROR;
}

/**
 * @param iface The name of the interface we're interested in
 * @return error The error code of the last error tethering or untethering the named
 *               interface
 * {@hide}
 */
ECode CConnectivityManager::GetLastTetherError(
    /* [in] */ const String& iface,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mService->GetLastTetherError(iface, result);
    if (FAILED(ec)) *result = TETHER_ERROR_SERVICE_UNAVAIL;
    return NOERROR;
}

/**
 * Ensure the device stays awake until we connect with the next network
 * @param forWhome The name of the network going down for logging purposes
 * @return {@code TRUE} on success, {@code FALSE} on failure
 * {@hide}
 */
ECode CConnectivityManager::RequestNetworkTransitionWakelock(
    /* [in] */ const String& forWhom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mService->RequestNetworkTransitionWakelock(forWhom);
    if (FAILED(ec)) *result = FALSE;
    else *result = TRUE;
    return NOERROR;
}

/**
 * @param networkType The type of network you want to report on
 * @param percentage The quality of the connection 0 is bad, 100 is good
 * {@hide}
 */
ECode CConnectivityManager::ReportInetCondition(
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 percentage)
{
    ECode ec = mService->ReportInetCondition(networkType, percentage);
    if (FAILED(ec)) {}
    return NOERROR;
}

/**
 * @param proxyProperties The definition for the new global http proxy
 * {@hide}
 */
ECode CConnectivityManager::SetGlobalProxy(
    /* [in] */ IProxyProperties* proxyp)
{
    ECode ec = mService->SetGlobalProxy(proxyp);
    if (FAILED(ec)) {}
    return NOERROR;
}

/**
 * @return proxyProperties for the current global proxy
 * {@hide}
 */
ECode CConnectivityManager::GetGlobalProxy(
    /* [out] */ IProxyProperties** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IProxyProperties> proxyp;
    ECode ec = mService->GetGlobalProxy((IProxyProperties**)&proxyp);
    if (FAILED(ec)) proxyp = NULL;
    *result = proxyp;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/**
 * @return proxyProperties for the current proxy (global if set, network specific if not)
 * {@hide}
 */
ECode CConnectivityManager::GetProxy(
    /* [out] */ IProxyProperties** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IProxyProperties> proxyp;
    ECode ec = mService->GetProxy((IProxyProperties**)&proxyp);
    if (FAILED(ec)) proxyp = NULL;
    *result = proxyp;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/**
 * @param networkType The network who's dependence has changed
 * @param met Boolean - TRUE if network use is ok, FALSE if not
 * {@hide}
 */
ECode CConnectivityManager::SetDataDependency(
    /* [in] */ Int32 networkType,
    /* [in] */ Boolean met)
{
    ECode ec = mService->SetDataDependency(networkType, met);
    if (FAILED(ec)) {}
    return NOERROR;
}

/**
 * Returns TRUE if the hardware supports the given network type
 * else it returns FALSE.  This doesn't indicate we have coverage
 * or are authorized onto a network, just whether or not the
 * hardware supports it.  For example a gsm phone without a sim
 * should still return TRUE for mobile data, but a wifi only tablet
 * would return FALSE.
 * @param networkType The nework type we'd like to check
 * @return TRUE if supported, else FALSE
 * @hide
 */
ECode CConnectivityManager::IsNetworkSupported(
    /* [in] */ Int32 networkType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mService->IsNetworkSupported(networkType, result);
    if (FAILED(ec)) *result = FALSE;
    return NOERROR;
}

/**
 * Returns if the currently active data network is metered. A network is
 * classified as metered when the user is sensitive to heavy data usage on
 * that connection. You should check this before doing large data transfers,
 * and warn the user or delay the operation until another network is
 * available.
 */
ECode CConnectivityManager::IsActiveNetworkMetered(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mService->IsActiveNetworkMetered(result);
    if (FAILED(ec)) *result = FALSE;
    return NOERROR;
}


ECode CConnectivityManager::UpdateLockdownVpn(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mService->UpdateLockdownVpn(result);
    if (FAILED(ec)) *result = FALSE;
    return NOERROR;
}

/**
 * {@hide}
 */
ECode CConnectivityManager::CaptivePortalCheckComplete(
    /* [in] */ INetworkInfo* info)
{
    mService->CaptivePortalCheckComplete(info);
    return NOERROR;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
