
#include "ext/frameworkext.h"
#include "net/CNetworkPolicyManager.h"
#include "net/CNetworkPolicyManagerHelper.h"


namespace Elastos {
namespace Droid {
namespace Net {

ECode CNetworkPolicyManagerHelper::From(
    /* [in] */ IContext* context,
    /* [out] */ INetworkPolicyManager** result)
{
    VALIDATE_NOT_NULL(result);
    return CNetworkPolicyManager::From(context, result);
}

ECode CNetworkPolicyManagerHelper::ComputeLastCycleBoundary(
    /* [in] */ Int64 currentTime,
    /* [in] */ INetworkPolicy* policy,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    return CNetworkPolicyManager::ComputeLastCycleBoundary(currentTime, policy, result);
}

ECode CNetworkPolicyManagerHelper::ComputeNextCycleBoundary(
    /* [in] */ Int64 currentTime,
    /* [in] */ INetworkPolicy* policy,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    return CNetworkPolicyManager::ComputeNextCycleBoundary(currentTime, policy, result);
}

ECode CNetworkPolicyManagerHelper::SnapToCycleDay(
    /* [in] */ ITime* time,
    /* [in] */ Int32 cycleDay)
{
    return CNetworkPolicyManager::SnapToCycleDay(time, cycleDay);
}

ECode CNetworkPolicyManagerHelper::IsUidValidForPolicy(
    /* [in] */ IContext* context,
    /* [in] */ Int32 uid,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return CNetworkPolicyManager::IsUidValidForPolicy(context, uid, result);
}

ECode CNetworkPolicyManagerHelper::DumpPolicy(
    /* [in] */ IPrintWriter* fout,
    /* [in] */ Int32 policy)
{
    return CNetworkPolicyManager::DumpPolicy(fout, policy);
}

ECode CNetworkPolicyManagerHelper::DumpRules(
    /* [in] */ IPrintWriter* fout,
    /* [in] */ Int32 rules)
{
    return CNetworkPolicyManager::DumpRules(fout, rules);
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
