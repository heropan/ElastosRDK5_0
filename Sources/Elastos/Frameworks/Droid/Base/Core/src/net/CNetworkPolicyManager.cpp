
#include "net/CNetworkPolicyManager.h"
#include "os/UserHandle.h"
#include "text/format/CTime.h"
#include "content/pm/PackageManager.h"

using Elastos::Droid::Text::Format::CTime;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Content::Pm::PackageManager;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::ISignature;
using Elastos::Droid::Content::Pm::IPackageInfo;
using Elastos::Core::CObjectContainer;

namespace Elastos {
namespace Droid {
namespace Net {

ECode CNetworkPolicyManager::constructor(
    /* [in] */ IINetworkPolicyManager* service)
{
    if (service == NULL) {
        //throw new IllegalArgumentException("missing INetworkPolicyManager");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mService = service;
    return NOERROR;
}

ECode CNetworkPolicyManager::From(
    /* [in] */ IContext* context,
    /* [out] */ INetworkPolicyManager** result)
{
    VALIDATE_NOT_NULL(result);
    ASSERT_SUCCEEDED(context->GetSystemService(IContext::NETWORK_POLICY_SERVICE, (IInterface**)result));
    return NOERROR;
}

/**
 * Set policy flags for specific UID.
 *
 * @param policy {@link #POLICY_NONE} or combination of flags like
 *            {@link #POLICY_REJECT_METERED_BACKGROUND}.
 */
ECode CNetworkPolicyManager::SetUidPolicy(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 policy)
{
    return mService->SetUidPolicy(uid, policy);
}

ECode CNetworkPolicyManager::GetUidPolicy(
    /* [in] */ Int32 uid,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec;
    ec = mService->GetUidPolicy(uid, result);
    if(FAILED(ec)) *result = POLICY_NONE;
    return NOERROR;
}

ECode CNetworkPolicyManager::GetUidsWithPolicy(
    /* [in] */ Int32 policy,
    /* [out, callee] */ ArrayOf<Int32>** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr< ArrayOf<Int32> > outputarray;
    ECode ec;
    ec = mService->GetUidsWithPolicy(policy, (ArrayOf<Int32>**)&outputarray);
    if(FAILED(ec)) outputarray = ArrayOf<Int32>::Alloc(0);
    *result = outputarray;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

ECode CNetworkPolicyManager::RegisterListener(
    /* [in] */ INetworkPolicyListener* listener)
{
    return mService->RegisterListener(listener);
}

ECode CNetworkPolicyManager::UnregisterListener(
    /* [in] */ INetworkPolicyListener* listener)
{
    return mService->UnregisterListener(listener);
}

ECode CNetworkPolicyManager::SetNetworkPolicies(
    /* [in] */ ArrayOf<INetworkPolicy*>* policies)
{
    return mService->SetNetworkPolicies(policies);
}

ECode CNetworkPolicyManager::GetNetworkPolicies(
    /* [out, callee] */ ArrayOf<INetworkPolicy*>** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr< ArrayOf<INetworkPolicy*> > outputarray;
    ECode ec;
    ec = mService->GetNetworkPolicies((ArrayOf<INetworkPolicy*>**)&outputarray);
    if(FAILED(ec)) outputarray = NULL;
    *result = outputarray;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

ECode CNetworkPolicyManager::SetRestrictBackground(
    /* [in] */ Boolean restrictBackground)
{
    return mService->SetRestrictBackground(restrictBackground);
}

ECode CNetworkPolicyManager::GetRestrictBackground(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec;
    ec = mService->GetRestrictBackground(result);
    if(FAILED(ec)) *result = FALSE;
    return NOERROR;
}

/**
 * Compute the last cycle boundary for the given {@link NetworkPolicy}. For
 * example, if cycle day is 20th, and today is June 15th, it will return May
 * 20th. When cycle day doesn't exist in current month, it snaps to the 1st
 * of following month.
 *
 * @hide
 */
ECode CNetworkPolicyManager::ComputeLastCycleBoundary(
    /* [in] */ Int64 currentTime,
    /* [in] */ INetworkPolicy* policy,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 cycleday;
    policy->GetCycleDay(&cycleday);
    if (cycleday == INetworkPolicy::CYCLE_NONE) {
        //throw new IllegalArgumentException("Unable to compute boundary without cycleDay");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String cycleTimezone;
    policy->GetCycleTimezone(&cycleTimezone);
    AutoPtr<ITime> now;
    CTime::New(cycleTimezone, (ITime**)&now);
    now->Set(currentTime);
    //first, find cycle boundary for current month
    AutoPtr<ITime> cycle;
    CTime::New(now, (ITime**)&cycle);
    cycle->SetHour(0);
    cycle->SetMinute(0);
    cycle->SetSecond(0);
    SnapToCycleDay(cycle, cycleday);
    Int32 compRet;
    if (CTime::Compare(cycle, now, &compRet), compRet >= 0) {
        //cycle boundary is beyond now, use last cycle boundary; start by
        //pushing ourselves squarely into last month.
        AutoPtr<ITime> lastMonth;
        CTime::New(now, (ITime**)&lastMonth);
        lastMonth->SetHour(0);
        lastMonth->SetMinute(0);
        lastMonth->SetSecond(0);
        lastMonth->SetMonthDay(1);
        Int32 month;
        lastMonth->GetMonth(&month);
        lastMonth->SetMonth(month - 1);
        Int64 ret;
        lastMonth->Normalize(TRUE, &ret);
        cycle->Set(lastMonth);
        SnapToCycleDay(cycle, cycleday);
    }
    cycle->ToMillis(TRUE, result);
    return NOERROR;
}

/** {@hide} */
ECode CNetworkPolicyManager::ComputeNextCycleBoundary(
    /* [in] */ Int64 currentTime,
    /* [in] */ INetworkPolicy* policy,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 cycleday;
    policy->GetCycleDay(&cycleday);
    if (cycleday == INetworkPolicy::CYCLE_NONE) {
        //throw new IllegalArgumentException("Unable to compute boundary without cycleDay");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<ITime> now;
    String cycleTimezone;
    policy->GetCycleTimezone(&cycleTimezone);
    CTime::New(cycleTimezone, (ITime**)&now);
    now->Set(currentTime);
    // // first, find cycle boundary for current month
    AutoPtr<ITime> cycle;
    CTime::New(now, (ITime**)&cycle);
    cycle->SetHour(0);
    cycle->SetMinute(0);
    cycle->SetSecond(0);

    SnapToCycleDay(cycle, cycleday);
    Int32 comRet;
    if (CTime::Compare(cycle, now, &comRet), comRet <= 0) {
        // cycle boundary is before now, use next cycle boundary; start by
        // pushing ourselves squarely into next month.
        AutoPtr<ITime> nextMonth;
        CTime::New(now, (ITime**)&nextMonth);
        nextMonth->SetHour(0);
        nextMonth->SetMinute(0);
        nextMonth->SetSecond(0);
        nextMonth->SetMonthDay(1);
        Int32 month;
        nextMonth->GetMonth(&month);
        nextMonth->SetMonth(month + 1);
        Int64 ret;
        nextMonth->Normalize(TRUE, &ret);
        cycle->Set(nextMonth);
        SnapToCycleDay(cycle, cycleday);
    }
    cycle->ToMillis(TRUE, result);
    return NOERROR;
}

/**
 * Snap to the cycle day for the current month given; when cycle day doesn't
 * exist, it snaps to last second of current month.
 *
 * @hide
 */
ECode CNetworkPolicyManager::SnapToCycleDay(
    /* [in] */ ITime* time,
    /* [in] */ Int32 cycleDay)
{
    Int32 maximum;
    time->GetActualMaximum(ITime::MONTH_DAY, &maximum);
    if (cycleDay > maximum) {
        // cycle day isn't valid this month; snap to last second of month
        Int32 month;
        time->GetMonth(&month);
        time->SetMonth(month + 1);
        time->SetMonthDay(1);
        Int32 second;
        time->GetSecond(&second);
        time->SetSecond(second - 1);
    }
    else {
        time->SetMonthDay(cycleDay);
    }
    Int64 ret;
    time->Normalize(TRUE, &ret);
    return NOERROR;
}

/**
 * Check if given UID can have a {@link #setUidPolicy(int, int)} defined,
 * usually to protect critical system services.
 */
//@Deprecated
ECode CNetworkPolicyManager::IsUidValidForPolicy(
    /* [in] */ IContext* context,
    /* [in] */ Int32 uid,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    //first, quick-reject non-applications
    if (!(UserHandle::IsApp(uid))) {
        *result = FALSE;
    }

    if (!ALLOW_PLATFORM_APP_POLICY) {
        AutoPtr<IPackageManager> pm;
        context->GetPackageManager((IPackageManager**)&pm);
        AutoPtr<IObjectContainer> systemSignature;
        CObjectContainer::New((IObjectContainer**)&systemSignature);
        //try {
        AutoPtr<IPackageInfo> packageinfo;
        pm->GetPackageInfo(String("android"), IPackageManager::GET_SIGNATURES, (IPackageInfo**)&packageinfo);
        AutoPtr< ArrayOf<ISignature*> > signatures;
        packageinfo->GetSignatures((ArrayOf<ISignature*>**)&signatures);

        for (Int32 i = 0; i< signatures->GetLength(); i++)
        {
            systemSignature->Add((*signatures)[i]);
        }
//      } catch (NameNotFoundException e) {
//          throw new RuntimeException("problem finding system signature", e);
//      }
//      reject apps signed with platform cert
        AutoPtr< ArrayOf<String> > uids;
        pm->GetPackagesForUid(uid, (ArrayOf<String>**)&uids);
        for (Int32 i = 0; i< uids->GetLength(); i++) {
            AutoPtr<IObjectContainer> packageSignature;
            CObjectContainer::New((IObjectContainer**)&packageSignature);
            AutoPtr<IPackageInfo> Uidspackageinfo;
            pm->GetPackageInfo((*uids)[i], IPackageManager::GET_SIGNATURES, (IPackageInfo**)&Uidspackageinfo);
            AutoPtr< ArrayOf<ISignature*> > Uidssignatures;
            packageinfo->GetSignatures((ArrayOf<ISignature*>**)&Uidssignatures);
            for (Int32 j = 0; j< Uidssignatures->GetLength(); j++) {
                packageSignature->Add((*Uidssignatures)[i]);
            }
            Boolean bol;
            packageSignature->ContainsAll(systemSignature, &bol);
            if (bol) {
                *result = FALSE;
                return NOERROR;
            }
        }

    }

    //nothing found above; we can apply policy to UID
    *result = TRUE;
    return NOERROR;

}

/** {@hide} */
ECode CNetworkPolicyManager::DumpPolicy(
    /* [in] */ IPrintWriter* fout,
    /* [in] */ Int32 policy)
{
    fout->WriteString(String("["));
    if ((policy & POLICY_REJECT_METERED_BACKGROUND) != 0) {
        fout->WriteString(String("REJECT_METERED_BACKGROUND"));
    }
    fout->WriteString(String("]"));
    return NOERROR;
}

/** {@hide} */
ECode CNetworkPolicyManager::DumpRules(
    /* [in] */ IPrintWriter* fout,
    /* [in] */ Int32 rules)
{
    fout->WriteString(String("["));
    if ((rules & RULE_REJECT_METERED) != 0) {
        fout->WriteString(String("REJECT_METERED"));
    }
    fout->WriteString(String("]"));
    return NOERROR;
}


} // namespace Net
} // namepsace Droid
} // namespace Elastos
