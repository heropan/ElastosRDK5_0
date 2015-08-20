
#include "ext/frameworkdef.h"
#include "net/CConnectivityManagerHelper.h"
#include "net/CConnectivityManager.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(CConnectivityManagerHelper, Singleton, IConnectivityManagerHelper)

CAR_SINGLETON_IMPL(CConnectivityManagerHelper)

ECode CConnectivityManagerHelper::IsNetworkTypeValid(
    /* [in] */ Int32 networkType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    return CConnectivityManager::IsNetworkTypeValid(networkType, result);
}

ECode CConnectivityManagerHelper::GetNetworkTypeName(
    /* [in] */ Int32 networkType,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    return CConnectivityManager::GetNetworkTypeName(networkType, result);
}

ECode CConnectivityManagerHelper::IsNetworkTypeMobile(
    /* [in] */ Int32 networkType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    return CConnectivityManager::IsNetworkTypeMobile(networkType, result);
}

ECode CConnectivityManagerHelper::IsNetworkTypeWifi(
        /* [in] */ int networkType,
        /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    return CConnectivityManager::IsNetworkTypeWifi(networkType, result);
}

ECode CConnectivityManagerHelper::MaybeMarkCapabilitiesRestricted(
    /* [in] */ INetworkCapabilities* nc)
{
    VALIDATE_NOT_NULL(nc)

    return CConnectivityManager::MaybeMarkCapabilitiesRestricted(nc);
}

ECode CConnectivityManagerHelper::From(
    /* [in] */ IContext* ctx,
    /* [out] */ IConnectivityManager** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;
    VALIDATE_NOT_NULL(ctx)

    return CConnectivityManager::From(ctx, result);
}

ECode CConnectivityManagerHelper::EnforceTetherChangePermission(
    /* [in] */ IContext* context)
{
    VALIDATE_NOT_NULL(context)

    return CConnectivityManager::EnforceTetherChangePermission(context);
}

ECode CConnectivityManagerHelper::SetProcessDefaultNetwork(
    /* [in] */ INetwork* network,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    VALIDATE_NOT_NULL(network)

    return CConnectivityManager::SetProcessDefaultNetwork(network, result);
}

ECode CConnectivityManagerHelper::GetProcessDefaultNetwork(
        /* [out] */ INetwork** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    return CConnectivityManager::GetProcessDefaultNetwork(result);
}

ECode CConnectivityManagerHelper::SetProcessDefaultNetworkForHostResolution(
    /* [in] */ INetwork* network,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    VALIDATE_NOT_NULL(network)

    return CConnectivityManager::SetProcessDefaultNetworkForHostResolution(network, result);
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
