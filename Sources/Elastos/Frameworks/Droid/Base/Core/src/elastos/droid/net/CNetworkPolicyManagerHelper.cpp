
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/net/CNetworkPolicyManager.h"
#include "elastos/droid/net/CNetworkPolicyManagerHelper.h"


namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(CNetworkPolicyManagerHelper, Singleton, INetworkPolicyManagerHelper)

CAR_SINGLETON_IMPL(CNetworkPolicyManagerHelper)

ECode CNetworkPolicyManagerHelper::From(
    /* [in] */ IContext* context,
    /* [out] */ INetworkPolicyManager** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;
    VALIDATE_NOT_NULL(context)

    return CNetworkPolicyManager::From(context, result);
}

ECode CNetworkPolicyManagerHelper::ComputeLastCycleBoundary(
    /* [in] */ Int64 currentTime,
    /* [in] */ INetworkPolicy* policy,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)
    *result = 0;
    VALIDATE_NOT_NULL(policy)

    return CNetworkPolicyManager::ComputeLastCycleBoundary(currentTime, policy, result);
}

ECode CNetworkPolicyManagerHelper::ComputeNextCycleBoundary(
    /* [in] */ Int64 currentTime,
    /* [in] */ INetworkPolicy* policy,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)
    *result = 0;
    VALIDATE_NOT_NULL(policy)

    return CNetworkPolicyManager::ComputeNextCycleBoundary(currentTime, policy, result);
}

ECode CNetworkPolicyManagerHelper::SnapToCycleDay(
    /* [in] */ ITime* time,
    /* [in] */ Int32 cycleDay)
{
    VALIDATE_NOT_NULL(time)

    return CNetworkPolicyManager::SnapToCycleDay(time, cycleDay);
}

ECode CNetworkPolicyManagerHelper::IsUidValidForPolicy(
    /* [in] */ IContext* context,
    /* [in] */ Int32 uid,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    VALIDATE_NOT_NULL(context)

    return CNetworkPolicyManager::IsUidValidForPolicy(context, uid, result);
}

ECode CNetworkPolicyManagerHelper::DumpPolicy(
    /* [in] */ IPrintWriter* fout,
    /* [in] */ Int32 policy)
{
    VALIDATE_NOT_NULL(fout)

    return CNetworkPolicyManager::DumpPolicy(fout, policy);
}

ECode CNetworkPolicyManagerHelper::DumpRules(
    /* [in] */ IPrintWriter* fout,
    /* [in] */ Int32 rules)
{
    VALIDATE_NOT_NULL(fout)

    return CNetworkPolicyManager::DumpRules(fout, rules);
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
