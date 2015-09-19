
#ifndef __ELASTOS_DROID_NET_CNETWORKPOLICYMANAGER_H__
#define __ELASTOS_DROID_NET_CNETWORKPOLICYMANAGER_H__

#include "_Elastos_Droid_Net_CNetworkPolicyManager.h"
#include "text/format/CTime.h"
#include "ext/frameworkext.h"

using Elastos::Droid::Text::Format::CTime;
using Elastos::Droid::Text::Format::ITime;
using Elastos::Droid::Content::IContext;
using Elastos::IO::IPrintWriter;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkPolicyManager)
{
public:
    CARAPI constructor(
        /* [in] */ IINetworkPolicyManager* service);

    CARAPI SetUidPolicy(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 policy);

    CARAPI GetUidPolicy(
        /* [in] */ Int32 uid,
        /* [out] */ Int32* result);

    CARAPI GetUidsWithPolicy(
        /* [in] */ Int32 policy,
        /* [out, callee] */ ArrayOf<Int32>** result);

    CARAPI RegisterListener(
        /* [in] */ INetworkPolicyListener* listener);

    CARAPI UnregisterListener(
        /* [in] */ INetworkPolicyListener* listener);

    CARAPI SetNetworkPolicies(
        /* [in] */ ArrayOf<INetworkPolicy*>* policies);

    CARAPI GetNetworkPolicies(
        /* [out, callee] */ ArrayOf<INetworkPolicy*>** result);

    CARAPI SetRestrictBackground(
        /* [in] */ Boolean restrictBackground);

    CARAPI GetRestrictBackground(
        /* [out] */ Boolean* result);

    static CARAPI From(
        /* [in] */ IContext* context,
        /* [out] */ INetworkPolicyManager** result);

    static CARAPI ComputeLastCycleBoundary(
        /* [in] */ Int64 currentTime,
        /* [in] */ INetworkPolicy* policy,
        /* [out] */ Int64* result);

    static CARAPI ComputeNextCycleBoundary(
        /* [in] */ Int64 currentTime,
        /* [in] */ INetworkPolicy* policy,
        /* [out] */ Int64* result);

    static CARAPI SnapToCycleDay(
        /* [in] */ ITime* time,
        /* [in] */ Int32 cycleDay);

    static CARAPI IsUidValidForPolicy(
        /* [in] */ IContext* context,
        /* [in] */ Int32 uid,
        /* [out] */ Boolean* result);

    static CARAPI DumpPolicy(
        /* [in] */ IPrintWriter* fout,
        /* [in] */ Int32 policy);

    static CARAPI DumpRules(
        /* [in] */ IPrintWriter* fout,
        /* [in] */ Int32 rules);

private:
    static const Boolean ALLOW_PLATFORM_APP_POLICY = TRUE;
    AutoPtr<IINetworkPolicyManager> mService;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CNETWORKPOLICYMANAGER_H__

