
#include "ext/frameworkext.h"
#include "R.h"
#include "content/res/CResources.h"
#include "net/CNetworkTemplate.h"
#include "net/CNetworkIdentity.h"
#include "telephony/CTelephonyManagerHelper.h"
#include "util/ArrayUtils.h"
#include <elastos/StringBuilder.h>
#include <elastos/Slogger.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Content::Res::CResources;
using Elastos::Droid::Internal::Utility::ArrayUtils;
using Elastos::Droid::Net::CNetworkIdentity;
using Elastos::Droid::Telephony::ITelephonyManagerHelper;
using Elastos::Droid::Telephony::CTelephonyManagerHelper;

namespace Elastos {
namespace Droid {
namespace Net {

Boolean CNetworkTemplate::sForceAllNetworkTypes = FALSE;
AutoPtr< ArrayOf<Int32> > CNetworkTemplate::DATA_USAGE_NETWORK_TYPES = NULL;

ECode CNetworkTemplate::constructor()
{
    return NOERROR;
}

ECode CNetworkTemplate::constructor(
    /* [in] */ Int32 matchRule,
    /* [in] */ const String& subscriberId,
    /* [in] */ const String& networkId)
{
    mMatchRule = matchRule;
    mSubscriberId = subscriberId;
    mNetworkId = networkId;
    CResources::GetSystem()->GetIntArray(R::array::config_data_usage_network_types, (ArrayOf<Int32>**)&DATA_USAGE_NETWORK_TYPES);
    return NOERROR;
}

// @VisibleForTesting
ECode CNetworkTemplate::ForceAllNetworkTypes()
{
    sForceAllNetworkTypes = TRUE;
    return NOERROR;
}

/**
 * Template to match {@link ConnectivityManager#TYPE_MOBILE} networks with
 * the given IMSI.
 */
ECode CNetworkTemplate::BuildTemplateMobileAll(
    /* [in] */ const String& subscriberId,
    /* [out] */ INetworkTemplate** result)
{
    String str;
    AutoPtr<INetworkTemplate> networktemplate;
    CNetworkTemplate::New(MATCH_MOBILE_ALL, subscriberId, str, (INetworkTemplate**)&networktemplate);
    *result = networktemplate;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/**
 * Template to match {@link ConnectivityManager#TYPE_MOBILE} networks with
 * the given IMSI that roughly meet a "3G" definition, or lower.
 */
//@Deprecated
ECode CNetworkTemplate::BuildTemplateMobile3gLower(
    /* [in] */ const String& subscriberId,
    /* [out] */ INetworkTemplate** result)
{
    String str;
    AutoPtr<INetworkTemplate> networktemplate;
    CNetworkTemplate::New(MATCH_MOBILE_3G_LOWER, subscriberId, str, (INetworkTemplate**)&networktemplate);
    *result = networktemplate;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/**
 * Template to match {@link ConnectivityManager#TYPE_MOBILE} networks with
 * the given IMSI that roughly meet a "4G" definition.
 */
//@Deprecated
ECode CNetworkTemplate::BuildTemplateMobile4g(
    /* [in] */ const String& subscriberId,
    /* [out] */ INetworkTemplate** result)
{
    String str;
    AutoPtr<INetworkTemplate> networktemplate;
    CNetworkTemplate::New(MATCH_MOBILE_4G, subscriberId, str, (INetworkTemplate**)&networktemplate);
    *result = networktemplate;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/**
 * Template to match {@link ConnectivityManager#TYPE_MOBILE} networks,
 * regardless of IMSI.
 */
ECode CNetworkTemplate::BuildTemplateMobileWildcard(
    /* [out] */ INetworkTemplate** result)
{
    String str;
    AutoPtr<INetworkTemplate> networktemplate;
    CNetworkTemplate::New(MATCH_MOBILE_WILDCARD, str, str, (INetworkTemplate**)&networktemplate);
    *result = networktemplate;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/**
 * Template to match all {@link ConnectivityManager#TYPE_WIFI} networks,
 * regardless of SSID.
 */
ECode CNetworkTemplate::BuildTemplateWifiWildcard(
    /* [out] */ INetworkTemplate** result)
{
    String str;
    AutoPtr<INetworkTemplate> networktemplate;
    CNetworkTemplate::New(MATCH_WIFI_WILDCARD, str, str, (INetworkTemplate**)&networktemplate);
    *result = networktemplate;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

//@Deprecated
ECode CNetworkTemplate::BuildTemplateWifi(
    /* [out] */ INetworkTemplate** result)
{
    return BuildTemplateWifiWildcard(result);
}

/**
 * Template to match {@link ConnectivityManager#TYPE_WIFI} networks with the
 * given SSID.
 */
ECode CNetworkTemplate::BuildTemplateWifiEx(
    /* [in] */ const String& networkId,
    /* [out] */ INetworkTemplate** result)
{
    String str;
    AutoPtr<INetworkTemplate> networktemplate;
    CNetworkTemplate::New(MATCH_WIFI, str, networkId, (INetworkTemplate**)&networktemplate);
    *result = networktemplate;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

/**
 * Template to combine all {@link ConnectivityManager#TYPE_ETHERNET} style
 * networks together.
 */
ECode CNetworkTemplate::BuildTemplateEthernet(
    /* [out] */ INetworkTemplate** result)
{
    String str;
    AutoPtr<INetworkTemplate> networktemplate;
    CNetworkTemplate::New(MATCH_ETHERNET, str, str, (INetworkTemplate**)&networktemplate);
    *result = networktemplate;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}


//@Override
ECode CNetworkTemplate::ToString(
    /* [out] */ String* result)
{
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
}

//@Override
ECode CNetworkTemplate::GetHashCode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 hashCode = 1;
    hashCode = 31 * hashCode + mMatchRule;
    hashCode = 31 * hashCode + mSubscriberId.GetHashCode();
    hashCode = 31 * hashCode + mNetworkId.GetHashCode();
    *result = hashCode;
    return NOERROR;
}

//@Override
ECode CNetworkTemplate::EqualsEx(
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

ECode CNetworkTemplate::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result )
{
    VALIDATE_NOT_NULL(result);
    return EqualsEx(INetworkTemplate::Probe(obj), result);
}

ECode CNetworkTemplate::GetMatchRule(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mMatchRule;
    return NOERROR;
}

ECode CNetworkTemplate::GetSubscriberId(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mSubscriberId;
    return NOERROR;
}

ECode CNetworkTemplate::GetNetworkId(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mNetworkId;
    return NOERROR;
}

/**
 * Test if given {@link NetworkIdentity} matches this template.
 */
ECode CNetworkTemplate::Matches(
    /* [in] */ INetworkIdentity* ident,
    /* [out] */ Boolean* result)
{
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
}

/**
 * Check if mobile network with matching IMSI.
 */
Boolean CNetworkTemplate::MatchesMobile(
    /* [in] */ INetworkIdentity* ident)
{
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
}

/**
 * Check if mobile network classified 3G or lower with matching IMSI.
 */
Boolean CNetworkTemplate::MatchesMobile3gLower(
    /* [in] */ INetworkIdentity* ident)
{
    EnsureSubtypeAvailable();
    Int32 type;
    ident->GetType(&type);
    if (type == IConnectivityManager::TYPE_WIMAX) {
        return FALSE;
    } else if (MatchesMobile(ident)) {
        Int32 subtype;
        ident->GetSubType(&subtype);
        AutoPtr<ITelephonyManagerHelper> helper;
        CTelephonyManagerHelper::AcquireSingleton((ITelephonyManagerHelper**)&helper);
        Int32 type;
        helper->GetNetworkClass(subtype, &type);
        switch (type) {
            case ITelephonyManager::NETWORK_CLASS_UNKNOWN:
            case ITelephonyManager::NETWORK_CLASS_2_G:
            case ITelephonyManager::NETWORK_CLASS_3_G:
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Check if mobile network classified 4G with matching IMSI.
 */
Boolean CNetworkTemplate::MatchesMobile4g(
    /* [in] */ INetworkIdentity* ident)
{
    EnsureSubtypeAvailable();
    Int32 type;
    ident->GetType(&type);
    if (type == IConnectivityManager::TYPE_WIMAX) {
        // TODO: consider matching against WiMAX subscriber identity
        return TRUE;
    } else if (MatchesMobile(ident)) {
        Int32 subtype;
        ident->GetSubType(&subtype);
        AutoPtr<ITelephonyManagerHelper> helper;
        CTelephonyManagerHelper::AcquireSingleton((ITelephonyManagerHelper**)&helper);
        Int32 type;
        helper->GetNetworkClass(subtype, &type);
        switch (type) {
            case ITelephonyManager::NETWORK_CLASS_4_G:
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Check if matches Wi-Fi network template.
 */
Boolean CNetworkTemplate::MatchesWifi(
    /* [in] */ INetworkIdentity* ident)
{
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
}

/**
 * Check if matches Ethernet network template.
 */
Boolean CNetworkTemplate::MatchesEthernet(
    /* [in] */ INetworkIdentity* ident)
{
    Int32 type;
    ident->GetType(&type);
    if (type == IConnectivityManager::TYPE_ETHERNET) {
        return TRUE;
    }
    return FALSE;
}

Boolean CNetworkTemplate::MatchesMobileWildcard(
    /* [in] */ INetworkIdentity* ident)
{
    Int32 type;
    ident->GetType(&type);
    if (type == IConnectivityManager::TYPE_WIMAX) {
        return TRUE;
    } else {
        Boolean bol = ArrayUtils::Contains(DATA_USAGE_NETWORK_TYPES, type);
        return sForceAllNetworkTypes || bol;
    }
}

Boolean CNetworkTemplate::MatchesWifiWildcard(
    /* [in] */ INetworkIdentity* ident)
{
    Int32 type;
    ident->GetType(&type);
    switch (type) {
        case IConnectivityManager::TYPE_WIFI:
        case IConnectivityManager::TYPE_WIFI_P2P:
            return TRUE;
        default:
            return FALSE;
    }
}

String CNetworkTemplate::GetMatchRuleName(
    /* [in] */ Int32 matchRule)
{
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
}

ECode CNetworkTemplate::EnsureSubtypeAvailable()
{
    if (INetworkIdentity::COMBINE_SUBTYPE_ENABLED) {
        // throw new IllegalArgumentException(
        //         "Unable to enforce 3G_LOWER template on combined data.");
        Slogger::E("NetworkTemplate", "Unable to enforce 3G_LOWER template on combined data.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

//@Override
ECode CNetworkTemplate::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);
    dest->WriteInt32(mMatchRule);
    dest->WriteString(mSubscriberId);
    dest->WriteString(mNetworkId);
    return NOERROR;
}

ECode CNetworkTemplate::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);
    source->ReadInt32(&mMatchRule);
    source->ReadString(&mSubscriberId);
    source->ReadString(&mNetworkId);
    return NOERROR;
}


} // namespace Net
} // namepsace Droid
} // namespace Elastos
