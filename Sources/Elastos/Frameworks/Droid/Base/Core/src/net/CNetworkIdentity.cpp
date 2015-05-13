
#include "ext/frameworkext.h"
#include "net/CNetworkIdentity.h"
#include "net/CConnectivityManager.h"
#include "telephony/CTelephonyManagerHelper.h"
#include <elastos/StringBuilder.h>
#include <elastos/Math.h>

using Elastos::Core::Math;
using Elastos::Core::StringBuilder;
using Elastos::Droid::Net::CConnectivityManager;
using Elastos::Droid::Net::Wifi::IWifiInfo;
using Elastos::Droid::Net::Wifi::IWifiManager;
using Elastos::Droid::Telephony::ITelephonyManager;
using Elastos::Droid::Telephony::ITelephonyManagerHelper;
using Elastos::Droid::Telephony::CTelephonyManagerHelper;

namespace Elastos {
namespace Droid {
namespace Net {

ECode CNetworkIdentity::constructor(
    /* [in] */ Int32 nType,
    /* [in] */ Int32 subType,
    /* [in] */ const String& subscriberId,
    /* [in] */ const String& networkId,
    /* [in] */ Boolean roaming)
{
    mType = nType;
    mSubType = COMBINE_SUBTYPE_ENABLED ? SUBTYPE_COMBINED : subType;
    mSubscriberId = subscriberId;
    mNetworkId = networkId;
    mRoaming = roaming;
    return NOERROR;
}

//@Override
ECode CNetworkIdentity::GetHashCode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 hashCode = 1;
    hashCode = 31 * hashCode + mType;
    hashCode = 31 * hashCode + mSubType;
    hashCode = 31 * hashCode + mSubscriberId.GetHashCode();
    hashCode = 31 * hashCode + mNetworkId.GetHashCode();
    hashCode = 31 * hashCode + (mRoaming ? 1231 : 1237);
    *result = hashCode;
    return NOERROR;
}

//@Override
ECode CNetworkIdentity::EqualsEx(
    /* [in] */ INetworkIdentity* obj,
    /* [out] */ Boolean* result )
{
    VALIDATE_NOT_NULL(result);
    if (INetworkIdentity::Probe(obj) != NULL) {
        const AutoPtr<INetworkIdentity> ident = obj;
        Int32 type;
        ident->GetType(&type);
        Int32 subType;
        ident->GetSubType(&subType);
        String subscriberId;
        ident->GetSubscriberId(&subscriberId);
        String networkId;
        ident->GetNetworkId(&networkId);
        Boolean roaming;
        ident->GetRoaming(&roaming);
        *result = mType == type &&
                  mSubType == subType &&
                  mRoaming == roaming &&
                  mSubscriberId.Equals(subscriberId) &&
                  mNetworkId.Equals(networkId);
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CNetworkIdentity::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result )
{
    VALIDATE_NOT_NULL(result);
    return EqualsEx(INetworkIdentity::Probe(obj), result);
}

//@Override
ECode CNetworkIdentity::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    StringBuilder builder("[");
    builder += String("type=");
    String typeName;
    CConnectivityManager::GetNetworkTypeName(mType, &typeName);
    builder += String("type=");
    builder += typeName;
    builder += String(", subType=");
    Boolean bol;
    CConnectivityManager::IsNetworkTypeMobile(mType, &bol);
    if (COMBINE_SUBTYPE_ENABLED) {
        builder += String("COMBINED");
    }
    else if (bol) {
        AutoPtr<ITelephonyManagerHelper> helper;
        CTelephonyManagerHelper::AcquireSingleton((ITelephonyManagerHelper**)&helper);
        String name;
        helper->GetNetworkTypeName(mSubType, &name);
        builder.AppendString(name);
    }
    else {
        builder += mSubType;
    }
    if (!(mSubscriberId.IsNull())) {
        builder += String(", subscriberId=");
        builder += mSubscriberId;
    }
    if (!(mNetworkId.IsNull())) {
        builder += String(", networkId=");
        builder += mNetworkId;
    }
    if (mRoaming) {
        builder += String(", ROAMING");
    }
    builder += String("]");
    *result = builder.ToString();
    return NOERROR;
}

ECode CNetworkIdentity::GetType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mType;
    return NOERROR;
}

ECode CNetworkIdentity::GetSubType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mSubType;
    return NOERROR;
}

ECode CNetworkIdentity::GetSubscriberId(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mSubscriberId;
    return NOERROR;
}

ECode CNetworkIdentity::GetNetworkId(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mNetworkId;
    return NOERROR;
}

ECode CNetworkIdentity::GetRoaming(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mRoaming;
    return NOERROR;
}

/**
 * Scrub given IMSI on production builds.
 */
ECode CNetworkIdentity::ScrubSubscriberId(
    /* [in] */ const String& subscriberId,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    using Elastos::Core::Math;

    if (String("eng").Equals(Build::TYPE)) {
        *result = subscriberId;
    }
    else if (!(subscriberId.IsNull())) {
        // TODO: parse this as MCC+MNC instead of hard-coding
        *result = subscriberId.Substring(0, Math::Min(6, subscriberId.GetLength())) + String("...");
    }
    else {
        *result = String("NULL");
    }
    return NOERROR;
}

/**
 * Build a {@link NetworkIdentity} from the given {@link NetworkState},
 * assuming that any mobile networks are using the current IMSI.
 */
ECode CNetworkIdentity::BuildNetworkIdentity(
    /* [in] */ IContext* context,
    /* [in] */ INetworkState* state,
    /* [out] */ INetworkIdentity** result)
{
    VALIDATE_NOT_NULL(result);

    Int32 type;
    AutoPtr<INetworkInfo> networkinfo;
    state->GetNetworkInfo((INetworkInfo**)&networkinfo);
    networkinfo->GetType(&type);
    Int32 subType;
    networkinfo->GetSubtype(&subType);

    // TODO: consider moving subscriberId over to LinkCapabilities, so it
    // comes from an authoritative source.

    String subscriberId;
    String networkId;
    Boolean roaming = FALSE;
    Boolean bol;
    CConnectivityManager::IsNetworkTypeMobile(type, &bol);
    if (bol) {
        AutoPtr<IInterface> service;
        context->GetSystemService(IContext::TELEPHONY_SERVICE, (IInterface**)&service);
        AutoPtr<ITelephonyManager> telephony = ITelephonyManager::Probe(service);
        telephony->IsNetworkRoaming(&roaming);
        String sId;
        state->GetSubscriberId(&sId);
        if (!sId.IsNull()) {
            subscriberId = sId;
        } else {
            telephony->GetSubscriberId(&subscriberId);
        }
    } else if (type == IConnectivityManager::TYPE_WIFI) {
        String netId;
        state->GetNetworkId(&netId);
        if (!netId.IsNull()) {
            networkId = netId;
        } else {
            AutoPtr<IInterface> service;
            context->GetSystemService(IContext::WIFI_SERVICE, (IInterface**)&service);
            AutoPtr<IWifiManager> wifi = IWifiManager::Probe(service);
            AutoPtr<IWifiInfo> info;
            wifi->GetConnectionInfo((IWifiInfo**)&info);
            if(info != NULL) {
                info->GetSSID(&networkId);
            } else {
                networkId = String(NULL);
            }
        }
    }
    CNetworkIdentity::New(type, subType, subscriberId, networkId, roaming, result);
    return NOERROR;
}


} // namespace Net
} // namepsace Droid
} // namespace Elastos
