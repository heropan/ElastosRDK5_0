
#include "elastos/droid/net/NetworkTemplate.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(NetworkTemplate, Object, IParcelable, INetworkTemplate)

Boolean NetworkTemplate::sForceAllNetworkTypes = FALSE;

const AutoPtr<ArrayOf<Int32> > NetworkTemplate::DATA_USAGE_NETWORK_TYPES = CreateDataUsageNetworkTypes();

AutoPtr<ArrayOf<Int32> > NetworkTemplate::CreateDataUsageNetworkTypes()
{
    AutoPtr<ArrayOf<Int32> > rev;
#if 0 // TODO: Translate codes below
    static {
        DATA_USAGE_NETWORK_TYPES = Resources.getSystem().getIntArray(
                com.android.internal.R.array.config_data_usage_network_types);
    }
#endif
    return rev;
}

NetworkTemplate::NetworkTemplate()
{}

ECode NetworkTemplate::ForceAllNetworkTypes()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    sForceAllNetworkTypes = TRUE;
    return NOERROR;
#endif
}

ECode NetworkTemplate::BuildTemplateMobileAll(
    /* [in] */ const String& subscriberId,
    /* [out] */ INetworkTemplate** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    String str;
    AutoPtr<INetworkTemplate> networktemplate;
    CNetworkTemplate::New(MATCH_MOBILE_ALL, subscriberId, str, (INetworkTemplate**)&networktemplate);
    *result = networktemplate;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

ECode NetworkTemplate::BuildTemplateMobile3gLower(
    /* [in] */ const String& subscriberId,
    /* [out] */ INetworkTemplate** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    String str;
    AutoPtr<INetworkTemplate> networktemplate;
    CNetworkTemplate::New(MATCH_MOBILE_3G_LOWER, subscriberId, str, (INetworkTemplate**)&networktemplate);
    *result = networktemplate;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

ECode NetworkTemplate::BuildTemplateMobile4g(
    /* [in] */ const String& subscriberId,
    /* [out] */ INetworkTemplate** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    String str;
    AutoPtr<INetworkTemplate> networktemplate;
    CNetworkTemplate::New(MATCH_MOBILE_4G, subscriberId, str, (INetworkTemplate**)&networktemplate);
    *result = networktemplate;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

ECode NetworkTemplate::BuildTemplateMobileWildcard(
    /* [out] */ INetworkTemplate** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    String str;
    AutoPtr<INetworkTemplate> networktemplate;
    CNetworkTemplate::New(MATCH_MOBILE_WILDCARD, str, str, (INetworkTemplate**)&networktemplate);
    *result = networktemplate;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

ECode NetworkTemplate::BuildTemplateWifiWildcard(
    /* [out] */ INetworkTemplate** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    String str;
    AutoPtr<INetworkTemplate> networktemplate;
    CNetworkTemplate::New(MATCH_WIFI_WILDCARD, str, str, (INetworkTemplate**)&networktemplate);
    *result = networktemplate;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

ECode NetworkTemplate::BuildTemplateWifi(
    /* [out] */ INetworkTemplate** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    return BuildTemplateWifiWildcard(result);
#endif
}

ECode NetworkTemplate::BuildTemplateWifi(
    /* [in] */ const String& networkId,
    /* [out] */ INetworkTemplate** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    String str;
    AutoPtr<INetworkTemplate> networktemplate;
    CNetworkTemplate::New(MATCH_WIFI, str, networkId, (INetworkTemplate**)&networktemplate);
    *result = networktemplate;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

ECode NetworkTemplate::BuildTemplateEthernet(
    /* [out] */ INetworkTemplate** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    String str;
    AutoPtr<INetworkTemplate> networktemplate;
    CNetworkTemplate::New(MATCH_ETHERNET, str, str, (INetworkTemplate**)&networktemplate);
    *result = networktemplate;
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

ECode NetworkTemplate::constructor(
    /* [in] */ Int32 matchRule,
    /* [in] */ const String& subscriberId,
    /* [in] */ const String& networkId)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    mMatchRule = matchRule;
    mSubscriberId = subscriberId;
    mNetworkId = networkId;
    CResources::GetSystem()->GetIntArray(R::array::config_data_usage_network_types, (ArrayOf<Int32>**)&DATA_USAGE_NETWORK_TYPES);
    return NOERROR;
#endif
}

NetworkTemplate::NetworkTemplate(
    /* [in] */ IParcel* in)
{
#if 0 // TODO: Translate codes below
        mMatchRule = in.readInt();
        mSubscriberId = in.readString();
        mNetworkId = in.readString();

#endif
}

ECode NetworkTemplate::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);

    StringBuilder builder("NetworkTemplate: ");
    builder += String("matchRule=");
    builder += GetMatchRuleName(mMatchRule);
    if (!(mSubscriberId.IsNull())) {
        builder += String(", subscriberId=");
        String subscriberId;
        CNetworkIdentity::ScrubSubscriberId(mSubscriberId, &subscriberId);
        builder += subscriberId;
    }
    if (!(mNetworkId.IsNull())) {
        builder += String(", networkId=");
        builder += mNetworkId;
    }
    *result = builder.ToString();
    return NOERROR;
#endif
}

ECode NetworkTemplate::GetHashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(result);
    Int32 hashCode = 1;
    hashCode = 31 * hashCode + mMatchRule;
    hashCode = 31 * hashCode + mSubscriberId.GetHashCode();
    hashCode = 31 * hashCode + mNetworkId.GetHashCode();
    *result = hashCode;
    return NOERROR;
#endif
}

ECode NetworkTemplate::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    return Equals(INetworkTemplate::Probe(obj), result);
#endif
}

ECode NetworkTemplate::GetMatchRule(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mMatchRule;
    return NOERROR;
#endif
}

ECode NetworkTemplate::GetSubscriberId(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mSubscriberId;
    return NOERROR;
#endif
}

ECode NetworkTemplate::GetNetworkId(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mNetworkId;
    return NOERROR;
#endif
}

ECode NetworkTemplate::Matches(
    /* [in] */ INetworkIdentity* ident,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);

    switch (mMatchRule) {
        case MATCH_MOBILE_ALL:
            *result = MatchesMobile(ident);
        case MATCH_MOBILE_3G_LOWER:
            *result = MatchesMobile3gLower(ident);
        case MATCH_MOBILE_4G:
            *result = MatchesMobile4g(ident);
        case MATCH_WIFI:
            *result = MatchesWifi(ident);
        case MATCH_ETHERNET:
            *result = MatchesEthernet(ident);
        case MATCH_MOBILE_WILDCARD:
            *result = MatchesMobileWildcard(ident);
        case MATCH_WIFI_WILDCARD:
            *result = MatchesWifiWildcard(ident);
        default:
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
//            throw new IllegalArgumentException("unknown network template");
    }
    return NOERROR;
#endif
}

ECode NetworkTemplate::MatchesMobile(
    /* [in] */ INetworkIdentity* ident,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    Int32 type;
    ident->GetType(&type);
    if (type == IConnectivityManager::TYPE_WIMAX) {
        // TODO: consider matching against WiMAX subscriber identity
        return TRUE;
    } else {
        Boolean bol = ArrayUtils::Contains(DATA_USAGE_NETWORK_TYPES, type);
        String subscriberid;
        ident->GetSubscriberId(&subscriberid);
        return ((sForceAllNetworkTypes || bol) && mSubscriberId.Equals(subscriberid));
    }
#endif
}

ECode NetworkTemplate::MatchesMobile3gLower(
    /* [in] */ INetworkIdentity* ident,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    EnsureSubtypeAvailable();
    Int32 type;
    ident->GetType(&type);
    if (type == IConnectivityManager::TYPE_WIMAX) {
        return FALSE;
    } else if (MatchesMobile(ident)) {
        Int32 subtype;
        ident->GetSubType(&subtype);
        Int32 type;
        CTelephonyManager::GetNetworkClass(subtype, &type);
        switch (type) {
            case ITelephonyManager::NETWORK_CLASS_UNKNOWN:
            case ITelephonyManager::NETWORK_CLASS_2_G:
            case ITelephonyManager::NETWORK_CLASS_3_G:
            return TRUE;
        }
    }
    return FALSE;
#endif
}

ECode NetworkTemplate::MatchesMobile4g(
    /* [in] */ INetworkIdentity* ident,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    EnsureSubtypeAvailable();
    Int32 type;
    ident->GetType(&type);
    if (type == IConnectivityManager::TYPE_WIMAX) {
        // TODO: consider matching against WiMAX subscriber identity
        return TRUE;
    } else if (MatchesMobile(ident)) {
        Int32 subtype;
        ident->GetSubType(&subtype);
        Int32 type;
        CTelephonyManager::GetNetworkClass(subtype, &type);
        switch (type) {
            case ITelephonyManager::NETWORK_CLASS_4_G:
            return TRUE;
        }
    }
    return FALSE;
#endif
}

ECode NetworkTemplate::MatchesWifi(
    /* [in] */ INetworkIdentity* ident,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    Int32 type;
    ident->GetType(&type);
    switch (type) {
        case IConnectivityManager::TYPE_WIFI: {
            String IdenNetworkid;
            ident->GetNetworkId(&IdenNetworkid);
            return mNetworkId.Equals(IdenNetworkid);
        }
        default:
            return FALSE;
    }
#endif
}

ECode NetworkTemplate::MatchesEthernet(
    /* [in] */ INetworkIdentity* ident,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    Int32 type;
    ident->GetType(&type);
    if (type == IConnectivityManager::TYPE_ETHERNET) {
        return TRUE;
    }
    return FALSE;
#endif
}

ECode NetworkTemplate::MatchesMobileWildcard(
    /* [in] */ INetworkIdentity* ident,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    Int32 type;
    ident->GetType(&type);
    if (type == IConnectivityManager::TYPE_WIMAX) {
        return TRUE;
    } else {
        Boolean bol = ArrayUtils::Contains(DATA_USAGE_NETWORK_TYPES, type);
        return sForceAllNetworkTypes || bol;
    }
#endif
}

ECode NetworkTemplate::MatchesWifiWildcard(
    /* [in] */ INetworkIdentity* ident,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    Int32 type;
    ident->GetType(&type);
    switch (type) {
        case IConnectivityManager::TYPE_WIFI:
        case IConnectivityManager::TYPE_WIFI_P2P:
            return TRUE;
        default:
            return FALSE;
    }
#endif
}

ECode NetworkTemplate::GetMatchRuleName(
    /* [in] */ Int32 matchRule,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    switch (matchRule) {
        case MATCH_MOBILE_3G_LOWER:
            return String("MOBILE_3G_LOWER");
        case MATCH_MOBILE_4G:
            return String("MOBILE_4G");
        case MATCH_MOBILE_ALL:
            return String("MOBILE_ALL");
        case MATCH_WIFI:
            return String("WIFI");
        case MATCH_ETHERNET:
            return String("ETHERNET");
        case MATCH_MOBILE_WILDCARD:
            return String("MOBILE_WILDCARD");
        case MATCH_WIFI_WILDCARD:
            return String("WIFI_WILDCARD");
        default:
            return String("UNKNOWN");
    }
#endif
}

ECode NetworkTemplate::EnsureSubtypeAvailable()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (INetworkIdentity::COMBINE_SUBTYPE_ENABLED) {
        // throw new IllegalArgumentException(
        //         "Unable to enforce 3G_LOWER template on combined data.");
        Slogger::E("NetworkTemplate", "Unable to enforce 3G_LOWER template on combined data.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
#endif
}

ECode NetworkTemplate::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(dest);
    dest->WriteInt32(mMatchRule);
    dest->WriteString(mSubscriberId);
    dest->WriteString(mNetworkId);
    return NOERROR;
#endif
}

ECode NetworkTemplate::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(source);
    source->ReadInt32(&mMatchRule);
    source->ReadString(&mSubscriberId);
    source->ReadString(&mNetworkId);
    return NOERROR;
#endif
}

} // namespace Net
} // namespace Droid
} // namespace Elastos

#if 0 // old CNetworkTemplate.cpp
ECode CNetworkTemplate::Equals(
    /* [in] */ INetworkTemplate* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (INetworkTemplate::Probe(obj) != NULL) {
        AutoPtr<INetworkTemplate> other = obj;
        Int32 matchrule;
        other->GetMatchRule(&matchrule);
        String subscriberId;
        other->GetSubscriberId(&subscriberId);
        String networkId;
        other->GetNetworkId(&networkId);
        *result = mMatchRule == matchrule &&
                  mSubscriberId.Equals(subscriberId) &&
                  mNetworkId.Equals(networkId);
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}
#endif