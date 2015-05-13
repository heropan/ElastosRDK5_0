
#include "CNetworkPolicyListener.h"
#include "CConnectivityService.h"

using Elastos::Droid::Net::INetworkPolicyManager;
using Elastos::Droid::Net::CConnectivityManagerHelper;

namespace Elastos {
namespace Droid {
namespace Server {

ECode CNetworkPolicyListener::constructor(
    /* [in] */ Handle32 owner)
{
    mOwner = (CConnectivityService*)owner;
    return NOERROR;
}

ECode CNetworkPolicyListener::OnUidRulesChanged(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 uidRules)
{
    // caller is NPMS, since we only register with them
//    if (LOGD_RULES) {
//        Log("onUidRulesChanged(uid=" + String::FromInt32(uid) + ", uidRules=" + String::FromInt32(uidRules) + ")");
//    }

    {
        Mutex::Autolock lock(mOwner->mRulesLock);
        // skip update when we've already applied rules
        Int32 oldRules;
        HashMap<Int32, Int32>::Iterator iter = mOwner->mUidRules.Find(uid);
        if (iter != mOwner->mUidRules.End()) {
            oldRules = iter->mSecond;
        }
        else {
            oldRules = INetworkPolicyManager::RULE_ALLOW_ALL;
        }
        if (oldRules == uidRules) return NOERROR;

        mOwner->mUidRules[uid] = uidRules;
    }
    return NOERROR;
    // TODO: notify UID when it has requested targeted updates
}

ECode CNetworkPolicyListener::OnMeteredIfacesChanged(
    /* [in] */ ArrayOf<String>* meteredIfaces)
{
    // caller is NPMS, since we only register with them
//    if (LOGD_RULES) {
//        //Log("onMeteredIfacesChanged(ifaces=" + Arrays.toString(meteredIfaces) + ")");
//    }
    {
       Mutex::Autolock lock(mOwner->mRulesLock);
       mOwner->mMeteredIfaces.Clear();
       for (Int32 i = 0; i < meteredIfaces->GetLength(); i++) {
           mOwner->mMeteredIfaces.Insert((*meteredIfaces)[i]);
       }
    }
    return NOERROR;
}

ECode CNetworkPolicyListener::OnRestrictBackgroundChanged(
    /* [in] */ Boolean restrictBackground)
{
    // caller is NPMS, since we only register with them
//    if (LOGD_RULES) {
//        Log("onRestrictBackgroundChanged(restrictBackground=" + String::FromBoolean(restrictBackground) + ")");
//    }

    // kick off connectivity change broadcast for active network, since
    // global background policy change is radical.
    Int32 networkType = mOwner->mActiveDefaultNetwork;
    AutoPtr<IConnectivityManagerHelper> helper;
    CConnectivityManagerHelper::AcquireSingleton((IConnectivityManagerHelper**)&helper);
    Boolean isValid;
    if (helper->IsNetworkTypeValid(networkType, &isValid), isValid) {
        AutoPtr<INetworkStateTracker> tracker = (*(mOwner->mNetTrackers))[networkType];
        if (tracker != NULL) {
            AutoPtr<INetworkInfo> info;
            tracker->GetNetworkInfo((INetworkInfo**)&info);
            Boolean connected = FALSE;
            if (info != NULL && (info->IsConnected(&connected), connected)) {
                mOwner->SendConnectedBroadcast(info);
            }
        }
    }
    return NOERROR;
}

} // namespace Server
} // namespace Droid
} // namespace Elastos
