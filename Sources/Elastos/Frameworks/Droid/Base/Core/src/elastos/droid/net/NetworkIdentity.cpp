
#include "elastos/droid/net/NetworkIdentity.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(NetworkIdentity, Object, INetworkIdentity)

ECode NetworkIdentity::constructor(
    /* [in] */ Int32 type,
    /* [in] */ Int32 subType,
    /* [in] */ const String& subscriberId,
    /* [in] */ const String& networkId,
    /* [in] */ Boolean roaming)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    mType = nType;
    mSubType = COMBINE_SUBTYPE_ENABLED ? SUBTYPE_COMBINED : subType;
    mSubscriberId = subscriberId;
    mNetworkId = networkId;
    mRoaming = roaming;
    return NOERROR;
#endif
}

ECode NetworkIdentity::HashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return Objects.hash(mType, mSubType, mSubscriberId, mNetworkId, mRoaming);

#endif
}

ECode NetworkIdentity::Equals(
    /* [in] */ IObject* obj,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (obj instanceof NetworkIdentity) {
            final NetworkIdentity ident = (NetworkIdentity) obj;
            return mType == ident.mType && mSubType == ident.mSubType && mRoaming == ident.mRoaming
                    && Objects.equals(mSubscriberId, ident.mSubscriberId)
                    && Objects.equals(mNetworkId, ident.mNetworkId);
        }
        return false;

#endif
}

ECode NetworkIdentity::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode NetworkIdentity::GetType(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mType;
    return NOERROR;
#endif
}

ECode NetworkIdentity::GetSubType(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mSubType;
    return NOERROR;
#endif
}

ECode NetworkIdentity::GetSubscriberId(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mSubscriberId;
    return NOERROR;
#endif
}

ECode NetworkIdentity::GetNetworkId(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mNetworkId;
    return NOERROR;
#endif
}

ECode NetworkIdentity::GetRoaming(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mRoaming;
    return NOERROR;
#endif
}

ECode NetworkIdentity::ScrubSubscriberId(
    /* [in] */ const String& subscriberId,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode NetworkIdentity::BuildNetworkIdentity(
    /* [in] */ IContext* context,
    /* [in] */ INetworkState* state,
    /* [out] */ INetworkIdentity** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos

#if 0 // old CNetworkIdentity.cpp
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

ECode CNetworkIdentity::Equals(
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
#endif