
#include "ext/frameworkdef.h"
#include "net/CConnectivityManagerHelper.h"
#include "net/CConnectivityManager.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CConnectivityManagerHelper::IsNetworkTypeValid(
    /* [in] */ Int32 networkType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return CConnectivityManager::IsNetworkTypeValid(networkType, result);
}

ECode CConnectivityManagerHelper::GetNetworkTypeName(
    /* [in] */ Int32 networkType,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    return CConnectivityManager::GetNetworkTypeName(networkType, result);
}

ECode CConnectivityManagerHelper::IsNetworkTypeMobile(
    /* [in] */ Int32 networkType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return CConnectivityManager::IsNetworkTypeMobile(networkType, result);
}

ECode CConnectivityManagerHelper::From(
    /* [in] */ IContext* ctx,
    /* [out] */ IConnectivityManager** result)
{
    VALIDATE_NOT_NULL(result);
    return CConnectivityManager::From(ctx, result);
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
