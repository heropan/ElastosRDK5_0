
#include "ext/frameworkext.h"
#include "net/CConnectivityManager.h"
// #include "net/NetworkUtils.h"
#include "os/CBinder.h"
#include <elastos/core/StringUtils.h>
// #include "CHashMap.h"
// #include "CArrayMap.h"
#include "AutoLock.h"

using Elastos::Core::StringUtils;
// using Elastos::Droid::Net::NetworkUtils;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::CBinder;
// using Elastos::Utility::CHashMap;
// using Elastos::Droid::Internal::Telephony::IPhoneConstants;

namespace Elastos {
namespace Droid {
namespace Net {

CAR_OBJECT_IMPL(CConnectivityManager)

CAR_INTERFACE_IMPL(CConnectivityManager, Object, IConnectivityManager)

AutoPtr<IHashMap> CreateHashMap()
{
    AutoPtr<IHashMap> rev;
#if 0 // TODO: Waiting for CHashMap
    CHashMap::New((IHashMap**)&rev);
#else
    assert(0);
#endif
    return rev;
}

const String CConnectivityManager::TAG("ConnectivityManager");
const Boolean CConnectivityManager::LEGACY_DBG = true;
AutoPtr<IHashMap> CConnectivityManager::sLegacyRequests = CreateHashMap();

CConnectivityManager::CConnectivityManager()
{
#if 0 // TODO: Waiting for CArrayMap
    CArrayMap::New((IArrayMap**)&mNetworkActivityListeners);
#else
    assert(0);
#endif
}

ECode CConnectivityManager::IsNetworkTypeValid(
    /* [in] */ Int32 networkType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = ((networkType >= 0) && (networkType <= MAX_NETWORK_TYPE));
    return NOERROR;
}

ECode CConnectivityManager::GetNetworkTypeName(
    /* [in] */ Int32 type,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    switch (type) {
        case TYPE_MOBILE:
            *result = "MOBILE";
            break;
        case TYPE_WIFI:
            *result = "WIFI";
            break;
        case TYPE_MOBILE_MMS:
            *result = "MOBILE_MMS";
            break;
        case TYPE_MOBILE_SUPL:
            *result = "MOBILE_SUPL";
            break;
        case TYPE_MOBILE_DUN:
            *result = "MOBILE_DUN";
            break;
        case TYPE_MOBILE_HIPRI:
            *result = String("MOBILE_HIPRI");
            break;
        case TYPE_WIMAX:
            *result = String("WIMAX");
            break;
        case TYPE_BLUETOOTH:
            *result = String("BLUETOOTH");
            break;
        case TYPE_DUMMY:
            *result = String("DUMMY");
            break;
        case TYPE_ETHERNET:
            *result = String("ETHERNET");
            break;
        case TYPE_MOBILE_FOTA:
            *result = String("MOBILE_FOTA");
            break;
        case TYPE_MOBILE_IMS:
            *result = String("MOBILE_IMS");
            break;
        case TYPE_MOBILE_CBS:
            *result = String("MOBILE_CBS");
            break;
        case TYPE_WIFI_P2P:
            *result = String("WIFI_P2P");
            break;
        case TYPE_MOBILE_IA:
            *result = String("MOBILE_IA");
            break;
        case TYPE_MOBILE_EMERGENCY:
            *result = String("MOBILE_EMERGENCY");
            break;
        case TYPE_PROXY:
            *result = String("PROXY");
            break;
        default:
            *result = StringUtils::ToString(type);
            break;
    }
    return NOERROR;
}

ECode CConnectivityManager::IsNetworkTypeMobile(
    /* [in] */ Int32 networkType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    switch (networkType) {
        case TYPE_MOBILE:
        case TYPE_MOBILE_MMS:
        case TYPE_MOBILE_SUPL:
        case TYPE_MOBILE_DUN:
        case TYPE_MOBILE_HIPRI:
        case TYPE_MOBILE_FOTA:
        case TYPE_MOBILE_IMS:
        case TYPE_MOBILE_CBS:
        case TYPE_MOBILE_IA:
        case TYPE_MOBILE_EMERGENCY:
            *result = TRUE;
            break;
        default:
            *result = FALSE;
            break;
    }
    return NOERROR;
}

ECode CConnectivityManager::IsNetworkTypeWifi(
    /* [in] */ Int32 networkType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    switch (networkType) {
        case TYPE_WIFI:
        case TYPE_WIFI_P2P:
            *result = TRUE;
            break;
        default:
            *result = FALSE;
            break;
    }
    return NOERROR;
}

ECode CConnectivityManager::SetNetworkPreference(
    /* [in] */ Int32 preference)
{
    return NOERROR;
}

ECode CConnectivityManager::GetNetworkPreference(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TYPE_NONE;

    return NOERROR;
}

ECode CConnectivityManager::GetActiveNetworkInfo(
    /* [out] */ INetworkInfo** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    AutoPtr<INetworkInfo> networkInfo;
    ECode ec = mService->GetActiveNetworkInfo((INetworkInfo**)&networkInfo);
    if (E_REMOTE_EXCEPTION == ec) {
        networkInfo = NULL;
    }
    *result = networkInfo;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetActiveNetworkInfoForUid(
    /* [in] */ Int32 uid,
    /* [out] */ INetworkInfo** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    AutoPtr<INetworkInfo> networkInfo;
    ECode ec = mService->GetActiveNetworkInfoForUid(uid, (INetworkInfo**)&networkInfo);
    if (E_REMOTE_EXCEPTION == ec) {
        networkInfo = NULL;
    }
    *result = networkInfo;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetNetworkInfo(
    /* [in] */ Int32 networkType,
    /* [out] */ INetworkInfo** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    AutoPtr<INetworkInfo> networkInfo;
    ECode ec = mService->GetNetworkInfo(networkType, (INetworkInfo**)&networkInfo);
    if (E_REMOTE_EXCEPTION == ec) {
        networkInfo = NULL;
    }
    *result = networkInfo;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetNetworkInfo(
    /* [in] */ INetwork* network,
    /* [out] */ INetworkInfo** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;
    VALIDATE_NOT_NULL(network)

    AutoPtr<INetworkInfo> networkInfo;
    ECode ec = mService->GetNetworkInfoForNetwork(network, (INetworkInfo**)&networkInfo);
    if (E_REMOTE_EXCEPTION == ec) {
        networkInfo = NULL;
    }
    *result = networkInfo;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetAllNetworkInfo(
    /* [out, callee] */ ArrayOf<INetworkInfo*>** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    AutoPtr<ArrayOf<INetworkInfo*> > outputarray;
    ECode ec = mService->GetAllNetworkInfo((ArrayOf<INetworkInfo*>**)&outputarray);
    if (E_REMOTE_EXCEPTION == ec) outputarray = NULL;
    *result = outputarray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetNetworkForType(
    /* [in] */ Int32 networkType,
    /* [out] */ INetwork** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    AutoPtr<INetwork> network;
    ECode ec = mService->GetNetworkForType(networkType, (INetwork**)&network);
    if (E_REMOTE_EXCEPTION == ec) network = NULL;
    *result = network;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetAllNetworks(
    /* [out] */ ArrayOf<INetwork*>** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;

    AutoPtr<ArrayOf<INetwork*> > outputarray;
    ECode ec = mService->GetAllNetworks((ArrayOf<INetwork*>**)&outputarray);
    if (E_REMOTE_EXCEPTION == ec) outputarray = NULL;
    *result = outputarray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetProvisioningOrActiveNetworkInfo(
    /* [out] */ INetworkInfo** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;

    AutoPtr<INetworkInfo> networkInfo;
    ECode ec = mService->GetProvisioningOrActiveNetworkInfo((INetworkInfo**)&networkInfo);
    if (E_REMOTE_EXCEPTION == ec) {
        networkInfo = NULL;
    }
    *result = networkInfo;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetActiveLinkProperties(
    /* [out] */ ILinkProperties** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    AutoPtr<ILinkProperties> linkp;
    ECode ec = mService->GetActiveLinkProperties((ILinkProperties**)&linkp);
    if (E_REMOTE_EXCEPTION == ec) linkp = NULL;
    *result = linkp;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetLinkProperties(
    /* [in] */ Int32 networkType,
    /* [out] */ ILinkProperties** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    AutoPtr<ILinkProperties> linkp;
    ECode ec = mService->GetLinkPropertiesForType(networkType, (ILinkProperties**)&linkp);
    if (E_REMOTE_EXCEPTION == ec) linkp = NULL;
    *result = linkp;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetLinkProperties(
    /* [in] */ INetwork* network,
    /* [out] */ ILinkProperties** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;
    VALIDATE_NOT_NULL(network)

    AutoPtr<ILinkProperties> linkp;
#if 0 // TODO: Waiting for IIConnectivityManager
    ECode ec = mService->GetLinkProperties(network, (ILinkProperties**)&linkp);
#else
    ECode ec;
    assert(0);
#endif
    if (E_REMOTE_EXCEPTION == ec) linkp = NULL;
    *result = linkp;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetNetworkCapabilities(
    /* [in] */ INetwork* network,
    /* [out] */ INetworkCapabilities** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;
    VALIDATE_NOT_NULL(network)

    AutoPtr<INetworkCapabilities> networkCapabilities;
#if 0 // TODO: Waiting for IIConnectivityManager
    ECode ec = mService->GetLinkProperties(network, (INetworkCapabilities**)&networkCapabilities);
#else
    ECode ec;
    assert(0);
#endif
    if (E_REMOTE_EXCEPTION == ec) networkCapabilities = NULL;
    *result = networkCapabilities;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::StartUsingNetworkFeature(
    /* [in] */ Int32 networkType,
    /* [in] */ const String& feature,
    /* [out] */ Int32* result)
{
#if 0
    VALIDATE_NOT_NULL(result);
    result = IPhoneConstants::APN_REQUEST_FAILED;

    AutoPtr<NetworkCapabilities> netCap
    AutoPtr<INetworkCapabilities> iNetCap;
    NetworkCapabilitiesForFeature(networkType, feature, (INetworkCapabilities**)&iNetCap);
    netCap = (NetworkCapabilities*) iNetCap;
    if (NULL == netCap) {
        // Log.d(TAG, "Can't satisfy startUsingNetworkFeature for " + networkType + ", " +
                // feature);
        Logger::D(TAG, (String("Can't satisfy startUsingNetworkFeature for ") + networkType + ", " + feature).string());
        *result = IPhoneConstants::APN_REQUEST_FAILED;
        return NOERROR;
    }

    AutoPtr<INetworkRequest> request = NULL;
    synchronized (sLegacyRequests) {
        if (LEGACY_DBG) {
            String s;
            iNetCap->ToString(&s);
            Int32 code;
            iNetCap->GetHashCode(&code);
            // Log.d(TAG, "Looking for legacyRequest for netCap with hash: " + netCap + " (" +
                    // netCap.hashCode() + ")");
            Logger::D(TAG, (String("Looking for legacyRequest for netCap with hash: ") + s + " (" + StringUtils::ToString(code) + ")").string());
            // Log.d(TAG, "sLegacyRequests has:");
            Logger::D(TAG, "sLegacyRequests has:");
            for (HashMap<NetworkCapabilities, Inner_LegacyRequest>::Iterator it = sLegacyRequests.Begin(); it != sLegacyRequests.End(); ++it) {
                it->mFirst->ToString(&s);
                it->mFirst->GetHashCode(&code);
                // Log.d(TAG, "  " + nc + " (" + nc.hashCode() + ")");
                Logger::D(TAG, (String("  ") + s + " (" + StringUtils::ToString(code) + ")").string());
            }
        }
        HashMap<NetworkCapabilities, Inner_LegacyRequest>::Iterator l = sLegacyRequests.Find(*netCap);
        if (l != sLegacyRequests.End()) {
            String s;
            l->mSecond.mNetworkRequest->ToString(&s);
            // Log.d(TAG, "renewing startUsingNetworkFeature request " + l.networkRequest);
            Logger::D(TAG, (String("renewing startUsingNetworkFeature request " + s).string()));
            RenewRequestLocked(l->mSecond);
            if (l->mSecond.mCurrentNetwork != NULL) {
                *result = IPhoneConstants::APN_ALREADY_ACTIVE;
                return NOERROR;
            } else {
                *result = IPhoneConstants::APN_REQUEST_STARTED;
                return NOERROR;
            }
        }

        RequestNetworkForFeatureLocked(iNetCap, &request);
    }
    if (request != NULL) {
        String s;
        request->ToString(&s);
        // Log.d(TAG, "starting startUsingNetworkFeature for request " + request);
        Logger::D(TAG, (String("starting startUsingNetworkFeature for request ") + s).string());
        *result = IPhoneConstants::APN_REQUEST_STARTED;
        return NOERROR;
    } else {
        // Log.d(TAG, " request Failed");
        Logger::D(TAG, " request Failed");
        *result = IPhoneConstants::APN_REQUEST_FAILED;
        return NOERROR;
    }
#endif
}

ECode CConnectivityManager::StopUsingNetworkFeature(
    /* [in] */ Int32 networkType,
    /* [in] */ const String& feature,
    /* [out] */ Int32* result)
{
#if 0
    VALIDATE_NOT_NULL(result);
    *result = -1;

    AutoPtr<INetworkCapabilities> netCap
    NetworkCapabilitiesForFeature(networkType, feature, (INetworkCapabilities**)&netCap);
    if (NULL == netCap) {
        // Log.d(TAG, "Can't satisfy stopUsingNetworkFeature for " + networkType + ", " +
                // feature);
        Logger::D(TAG, (String("Can't satisfy stopUsingNetworkFeature for ") + StringUtils::ToString(networkType) + ", " + feature).string());
        *result = -1;
        return NOERROR;
    }

    AutoPtr<INetworkCallback> networkCallback;
    RemoveRequestForFeature(netCap, (INetworkCallback**)&networkCallback);
    if (networkCallback != NULL) {
        // Log.d(TAG, "stopUsingNetworkFeature for " + networkType + ", " + feature);
        Logger::D(TAG, (String("stopUsingNetworkFeature for ") + StringUtils::ToString(networkType) + ", " + feature).string()));
        UnregisterNetworkCallback(networkCallback);
    }
    *result = 1;
#endif
    return NOERROR;
}

ECode CConnectivityManager::MaybeMarkCapabilitiesRestricted(
    /* [in] */ INetworkCapabilities* nc)
{
#if 0
    VALIDATE_NOT_NULL(nc)

    AutoPtr<ArrayOf<Int32> > cap;
    nc->getCapabilities((ArrayOf<Int32>**)&cap);
    for (Int32 i = 0; i < cap->GetLength(); ++i) {
        switch (cap[i]) {
            case INetworkCapabilities::NET_CAPABILITY_CBS:
            case INetworkCapabilities::NET_CAPABILITY_DUN:
            case INetworkCapabilities::NET_CAPABILITY_EIMS:
            case INetworkCapabilities::NET_CAPABILITY_FOTA:
            case INetworkCapabilities::NET_CAPABILITY_IA:
            case INetworkCapabilities::NET_CAPABILITY_IMS:
            case INetworkCapabilities::NET_CAPABILITY_RCS:
            case INetworkCapabilities::NET_CAPABILITY_XCAP:
            case INetworkCapabilities::NET_CAPABILITY_NOT_RESTRICTED: //there by default
                continue;
            default:
                // At least one capability usually provided by unrestricted
                // networks. Conclude that this network is unrestricted.
                return NOERROR;
        }
    }
    // All the capabilities are typically provided by restricted networks.
    // Conclude that this network is restricted.
    return nc->RemoveCapability(INetworkCapabilities::NET_CAPABILITY_NOT_RESTRICTED);
#endif
}

ECode CConnectivityManager::NetworkCapabilitiesForFeature(
    /* [in] */ Int32 networkType,
    /* [in] */ const String& feature,
    /* [out] */ INetworkCapabilities** result)
{
#if 0
    VALIDATE_NOT_NULL(*result)
    *result = NULL;

    if (TYPE_MOBILE == networkType) {
        Int32 cap = -1;
        if (feature.Equals("enableMMS")) {
            cap = INetworkCapabilities::NET_CAPABILITY_MMS;
        } else if (feature.Equals("enableSUPL")) {
            cap = INetworkCapabilities::NET_CAPABILITY_SUPL;
        } else if (feature.Equals("enableDUN") || feature.Equals("enableDUNAlways")) {
            cap = INetworkCapabilities::NET_CAPABILITY_DUN;
        } else if (feature.Equals("enableHIPRI")) {
            cap = INetworkCapabilities::NET_CAPABILITY_INTERNET;
        } else if (feature.Equals("enableFOTA")) {
            cap = INetworkCapabilities::NET_CAPABILITY_FOTA;
        } else if (feature.Equals("enableIMS")) {
            cap = INetworkCapabilities::NET_CAPABILITY_IMS;
        } else if (feature.Equals("enableCBS")) {
            cap = INetworkCapabilities::NET_CAPABILITY_CBS;
        } else {
            *result = NULL;
        }
        AutoPtr<INetworkCapabilities> netCap;
        CNetworkCapabilities::New((INetworkCapabilities**)&netCap);
        AutoPtr<INetworkCapabilities> tmp;
        netCap->AddTransportType(INetworkCapabilities::TRANSPORT_CELLULAR, (INetworkCapabilities**)&tmp);
        tmp->AddCapability(cap);
        MaybeMarkCapabilitiesRestricted(netCap);
        *result = netCap;
        REFCOUNT_ADD(*result)
        return NOERROR;
    } else if (TYPE_WIFI == networkType) {
        if (feature.Equals("p2p")) {
            AutoPtr<INetworkCapabilities> netCap;
            CNetworkCapabilities::New((INetworkCapabilities**)&netCap);
            netCap->AddTransportType(INetworkCapabilities::TRANSPORT_WIFI);
            netCap->AddCapability(INetworkCapabilities::NET_CAPABILITY_WIFI_P2P);
            MaybeMarkCapabilitiesRestricted(netCap);
            *result = netCap;
            REFCOUNT_ADD(*result)
            return NOERROR;
        }
    }
    *result = NULL;
#endif
    return NOERROR;
}

ECode CConnectivityManager::InferLegacyTypeForNetworkCapabilities(
    /* [in] */ INetworkCapabilities* netCap,
    /* [out] */ Int32* result)
{
#if 0
    if (NULL == netCap) {
        *result = TYPE_NONE;
        return NOERROR;
    }
    Boolean b;
    if (netCap->HasTransport(INetworkCapabilities::TRANSPORT_CELLULAR, &b), !b) {
        *result = TYPE_NONE;
        return NOERROR;
    }
    AutoPtr<INetworkCapabilities> networkCapabilities;
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_CBS, &b), b) {
        NetworkCapabilitiesForFeature(TYPE_MOBILE, String("enableCBS"), (INetworkCapabilities**)&networkCapabilities);
        if (netCap->Equals(networkCapabilities, &b), b) {
            *result = TYPE_MOBILE_CBS;
            return NOERROR;
        } else {
            *result = TYPE_NONE;
            return NOERROR;
        }
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_IMS)) {
        NetworkCapabilitiesForFeature(TYPE_MOBILE, String("enableIMS"), (INetworkCapabilities**)&networkCapabilities);
        if (netCap->Equals(networkCapabilities, &b), b) {
            *result = TYPE_MOBILE_IMS;
            return NOERROR;
        } else {
            *result = TYPE_NONE;
            return NOERROR;
        }
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_FOTA)) {
        NetworkCapabilitiesForFeature(TYPE_MOBILE, String("enableFOTA"), (INetworkCapabilities**)&networkCapabilities);
        if (netCap->Equals(networkCapabilities, &b), b) {
            *result = TYPE_MOBILE_FOTA;
            return NOERROR;
        } else {
            *result = TYPE_NONE;
            return NOERROR;
        }
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_DUN)) {
        NetworkCapabilitiesForFeature(TYPE_MOBILE, String("enableDUN"), (INetworkCapabilities**)&networkCapabilities);
        if (netCap->Equals(networkCapabilities, &b), b) {
            *result = TYPE_MOBILE_DUN;
            return NOERROR;
        } else {
            *result = TYPE_NONE;
            return NOERROR;
        }
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_SUPL)) {
        NetworkCapabilitiesForFeature(TYPE_MOBILE, String("enableSUPL"), (INetworkCapabilities**)&networkCapabilities);
        if (netCap->Equals(networkCapabilities, &b), b) {
            *result = TYPE_MOBILE_SUPL;
            return NOERROR;
        } else {
            *result = TYPE_NONE;
            return NOERROR;
        }
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_MMS)) {
        NetworkCapabilitiesForFeature(TYPE_MOBILE, String("enableMMS"), (INetworkCapabilities**)&networkCapabilities);
        if (netCap->Equals(networkCapabilities, &b), b) {
            *result = TYPE_MOBILE_MMS;
            return NOERROR;
        } else {
            *result = TYPE_NONE;
            return NOERROR;
        }
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_INTERNET)) {
        NetworkCapabilitiesForFeature(TYPE_MOBILE, String("enableHIPRI"), (INetworkCapabilities**)&networkCapabilities);
        if (netCap->Equals(networkCapabilities, &b), b) {
            *result = TYPE_MOBILE_HIPRI;
            return NOERROR;
        } else {
            *result = TYPE_NONE;
            return NOERROR;
        }
    }
    *result = TYPE_NONE;
#endif
    return NOERROR;
}

ECode CConnectivityManager::LegacyTypeForNetworkCapabilities(
    /* [in] */ INetworkCapabilities* netCap,
    /* [out] */ Int32* result)
{
#if 0
    VALIDATE_NOT_NULL(result)
    *result = TYPE_NONE;

    if (NULL == netCap) {
        *result = TYPE_NONE;
        return NOERROR;
    }
    Boolean b;
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_CBS, &b), b) {
       *result = TYPE_MOBILE_CBS;
       return NOERROR;
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_IMS, &b), b) {
       *result = TYPE_MOBILE_IMS;
       return NOERROR;
       }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_FOTA, &b), b) {
       *result = TYPE_MOBILE_FOTA;
       return NOERROR;
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_DUN, &b), b) {
       *result = TYPE_MOBILE_DUN;
       return NOERROR;
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_SUPL, &b), b) {
       *result = TYPE_MOBILE_SUPL;
       return NOERROR;
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_MMS, &b), b) {
       *result = TYPE_MOBILE_MMS;
       return NOERROR;
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_INTERNET, &b), b) {
       *result = TYPE_MOBILE_HIPRI;
       return NOERROR;
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_WIFI_P2P, &b), b) {
       *result = TYPE_WIFI_P2P;
       return NOERROR;
    }
   *result = TYPE_NONE;
#endif
   return NOERROR;
}

ECode CConnectivityManager::FindRequestForFeature(
    /* [in] */ INetworkCapabilities* netCap,
    /* [out] */ INetworkRequest** result)
{
#if 0
    VALIDATE_NOT_NULL(*result)
    *result = NULL;
    VALIDATE_NOT_NULL(netCap)

    synchronized (sLegacyRequests) {
        HashMap<INetworkCapabilities*, Inner_LegacyRequest>::Iterator l = sLegacyRequests.Find(netCap);
        if (l != NULL)
        {
            *result = l->mSecond.mNetworkRequest;
        }
    }
    return null;
#endif
}

ECode CConnectivityManager::RenewRequestLocked(
    /* [in] */ Inner_LegacyRequest& l)
{
#if 0
    l.mExpireSequenceNumber++;
    // Log.d(TAG, "renewing request to seqNum " + l.expireSequenceNumber);
    SendExpireMsgForFeature(l.mNetworkCapabilities, l.mExpireSequenceNumber, l.mDelay);
#endif
}

ECode CConnectivityManager::ExpireRequest(
    /* [in] */ INetworkCapabilities* netCap,
    /* [in] */ Int32 sequenceNum)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Waiting for implementing.
        int ourSeqNum = -1;
        synchronized (sLegacyRequests) {
            LegacyRequest l = sLegacyRequests.get(netCap);
            if (l == null) return;
            ourSeqNum = l.expireSequenceNumber;
            if (l.expireSequenceNumber == sequenceNum) {
                unregisterNetworkCallback(l.networkCallback);
                sLegacyRequests.remove(netCap);
            }
        }
        Log.d(TAG, "expireRequest with " + ourSeqNum + ", " + sequenceNum);
#endif
}

ECode CConnectivityManager::RequestNetworkForFeatureLocked(
    /* [in] */ INetworkCapabilities* netCap,
    /* [out] */ INetworkRequest** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Waiting for implementing.
        int delay = -1;
        int type = legacyTypeForNetworkCapabilities(netCap);
        try {
            delay = mService.getRestoreDefaultNetworkDelay(type);
        } catch (RemoteException e) {}
        LegacyRequest l = new LegacyRequest();
        l.networkCapabilities = netCap;
        l.delay = delay;
        l.expireSequenceNumber = 0;
        l.networkRequest = sendRequestForNetwork(netCap, l.networkCallback, 0,
                REQUEST, type);
        if (l.networkRequest == null) return null;
        sLegacyRequests.put(netCap, l);
        sendExpireMsgForFeature(netCap, l.expireSequenceNumber, delay);
        return l.networkRequest;
#endif
}

ECode CConnectivityManager::SendExpireMsgForFeature(
    /* [in] */ INetworkCapabilities* netCap,
    /* [in] */ Int32 seqNum,
    /* [in] */ Int32 delay)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Waiting for implementing.
        if (delay >= 0) {
            Log.d(TAG, "sending expire msg with seqNum " + seqNum + " and delay " + delay);
            Message msg = sCallbackHandler.obtainMessage(EXPIRE_LEGACY_REQUEST, seqNum, 0, netCap);
            sCallbackHandler.sendMessageDelayed(msg, delay);
        }
#endif
}

ECode CConnectivityManager::RemoveRequestForFeature(
    /* [in] */ INetworkCapabilities* netCap,
    /* [out] */ INetworkCallback** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Waiting for implementing.
        synchronized (sLegacyRequests) {
            LegacyRequest l = sLegacyRequests.remove(netCap);
            if (l == null) return null;
            return l.networkCallback;
        }
#endif
}

ECode CConnectivityManager::RequestRouteToHost(
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 hostAddress,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

#if 0
    AutoPtr<ArrayOf<Byte> > address;
    hostAddress->GetAddress((ArrayOf<Byte>**)&address);
    ECode ec = RequestRouteToHostAddress(networkType, address, result);
    if (E_REMOTE_EXCEPTION == ec)
    {
        *result = FALSE;
        return NOERROR;
    }
    return ec;
#endif
}

ECode CConnectivityManager::RequestRouteToHostAddress(
    /* [in] */ Int32 networkType,
    /* [in] */ IInetAddress* hostAddress,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    AutoPtr<ArrayOf<Byte> > address;
    hostAddress->GetAddress((ArrayOf<Byte>**)&address);
    ECode ec = mService->RequestRouteToHostAddress(networkType, address, result);
    if (E_REMOTE_EXCEPTION == ec) *result = FALSE;
    return NOERROR;
}

ECode CConnectivityManager::GetBackgroundDataSetting(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode CConnectivityManager::SetBackgroundDataSetting(
    /* [in] */ Boolean allowBackgroundData)
{
    return NOERROR;
}

ECode CConnectivityManager::GetActiveNetworkQuotaInfo(
    /* [out] */ INetworkQuotaInfo** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    AutoPtr<INetworkQuotaInfo> quotaInfo;
    ECode ec = mService->GetActiveNetworkQuotaInfo((INetworkQuotaInfo**)&quotaInfo);
    if (E_REMOTE_EXCEPTION == ec) quotaInfo = NULL;
    *result = quotaInfo;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetMobileDataEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Waiting for implementing.
        IBinder b = ServiceManager.getService(Context.TELEPHONY_SERVICE);
        if (b != null) {
            try {
                ITelephony it = ITelephony.Stub.asInterface(b);
                return it.getDataEnabled();
            } catch (RemoteException e) { }
        }
        return false;
#endif
}

ECode CConnectivityManager::GetNetworkManagementService(
    /* [in] */ INetworkManagementService* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Waiting for implementing.
        synchronized (this) {
            if (mNMService != null) {
                return mNMService;
            }
            IBinder b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
            mNMService = INetworkManagementService.Stub.asInterface(b);
            return mNMService;
        }
#endif
}

ECode CConnectivityManager::AddDefaultNetworkActiveListener(
    /* [in] */ IOnNetworkActiveListener* l)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Waiting for implementing.
        INetworkActivityListener rl = new INetworkActivityListener.Stub() {
            @Override
            public void onNetworkActive() throws RemoteException {
                l.onNetworkActive();
            }
        };

        try {
            getNetworkManagementService().registerNetworkActivityListener(rl);
            mNetworkActivityListeners.put(l, rl);
        } catch (RemoteException e) {
        }
#endif
}

ECode CConnectivityManager::RemoveDefaultNetworkActiveListener(
    /* [in] */ IOnNetworkActiveListener* l)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Waiting for implementing.
        INetworkActivityListener rl = mNetworkActivityListeners.get(l);
        if (rl == null) {
            throw new IllegalArgumentException("Listener not registered: " + l);
        }
        try {
            getNetworkManagementService().unregisterNetworkActivityListener(rl);
        } catch (RemoteException e) {
        }
#endif
}

ECode CConnectivityManager::IsDefaultNetworkActive(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Waiting for implementing.
        try {
            return getNetworkManagementService().isNetworkActive();
        } catch (RemoteException e) {
        }
        return false;
#endif
}

ECode CConnectivityManager::constructor(
    /* [in] */ IIConnectivityManager* service)
{
    VALIDATE_NOT_NULL(service);

    mService = service;
    return NOERROR;
}

ECode CConnectivityManager::From(
    /* [in] */ IContext* ctx,
    /* [out] */ IConnectivityManager** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

   ASSERT_SUCCEEDED(ctx->GetSystemService(IContext::CONNECTIVITY_SERVICE, (IInterface**)&result));
   return NOERROR;
}

ECode CConnectivityManager::EnforceTetherChangePermission(
    /* [in] */ IContext* context)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Waiting for implementing.
        if (context.getResources().getStringArray(
                com.android.internal.R.array.config_mobile_hotspot_provision_app).length == 2) {
            // Have a provisioning app - must only let system apps (which check this app)
            // turn on tethering
            context.enforceCallingOrSelfPermission(
                    android.Manifest.permission.CONNECTIVITY_INTERNAL, "ConnectivityService");
        } else {
            context.enforceCallingOrSelfPermission(
                    android.Manifest.permission.CHANGE_NETWORK_STATE, "ConnectivityService");
        }
#endif
}

ECode CConnectivityManager::GetTetherableIfaces(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    AutoPtr <ArrayOf<String> > outputArray;
    ECode ec = mService->GetTetherableIfaces((ArrayOf<String>**)&outputArray);
    if (E_REMOTE_EXCEPTION == ec) outputArray = ArrayOf<String>::Alloc(0);
    *result = outputArray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetTetheredIfaces(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    AutoPtr <ArrayOf<String> > outputArray;
    ECode ec = mService->GetTetheredIfaces((ArrayOf<String>**)&outputArray);
    if (E_REMOTE_EXCEPTION == ec) outputArray = ArrayOf<String>::Alloc(0);
    *result = outputArray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetTetheringErroredIfaces(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    AutoPtr <ArrayOf<String> > outputArray;
    ECode ec = mService->GetTetheringErroredIfaces((ArrayOf<String>**)&outputArray);
    if (E_REMOTE_EXCEPTION == ec) outputArray = ArrayOf<String>::Alloc(0);
    *result = outputArray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetTetheredDhcpRanges(
    /* [out] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Waiting for implementing.
        try {
            return mService.getTetheredDhcpRanges();
        } catch (RemoteException e) {
            return new String[0];
        }
#endif
}

ECode  CConnectivityManager::Tether(
    /* [in] */ const String& iface,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TETHER_ERROR_UNSUPPORTED;

    ECode ec = mService->Tether(iface, result);
    if (E_REMOTE_EXCEPTION == ec) *result = TETHER_ERROR_SERVICE_UNAVAIL;
    return NOERROR;
}

ECode CConnectivityManager::Untether(
    /* [in] */ const String& iface,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TETHER_ERROR_UNSUPPORTED;

    ECode ec = mService->Untether(iface, result);
    if (E_REMOTE_EXCEPTION == ec) *result = TETHER_ERROR_SERVICE_UNAVAIL;
    return NOERROR;
}

ECode CConnectivityManager::IsTetheringSupported(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    result = FALSE;

    ECode ec = mService->IsTetheringSupported(result);
    if (E_REMOTE_EXCEPTION == ec) *result = FALSE;
    return NOERROR;
}

ECode CConnectivityManager::GetTetherableUsbRegexs(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    AutoPtr <ArrayOf<String> > outputArray;
    ECode ec = mService->GetTetherableUsbRegexs((ArrayOf<String>**)&outputArray);
    if (E_REMOTE_EXCEPTION == ec) outputArray = ArrayOf<String>::Alloc(0);
    *result = outputArray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetTetherableWifiRegexs(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    AutoPtr <ArrayOf<String> > outputArray;
    ECode ec = mService->GetTetherableWifiRegexs((ArrayOf<String>**)&outputArray);
    if (E_REMOTE_EXCEPTION == ec) outputArray = ArrayOf<String>::Alloc(0);
    *result = outputArray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::GetTetherableBluetoothRegexs(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    AutoPtr <ArrayOf<String> > outputArray;
    ECode ec = mService->GetTetherableBluetoothRegexs((ArrayOf<String>**)&outputArray);
    if (E_REMOTE_EXCEPTION == ec) outputArray = ArrayOf<String>::Alloc(0);
    *result = outputArray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CConnectivityManager::SetUsbTethering(
    /* [in] */ Boolean enabled,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TETHER_ERROR_UNSUPPORTED;

    ECode ec = mService->SetUsbTethering(enabled, result);
    if (E_REMOTE_EXCEPTION == ec) *result = TETHER_ERROR_SERVICE_UNAVAIL;
    return NOERROR;
}

ECode CConnectivityManager::GetLastTetherError(
    /* [in] */ const String& iface,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TETHER_ERROR_UNSUPPORTED;

    ECode ec = mService->GetLastTetherError(iface, result);
    if (E_REMOTE_EXCEPTION == ec) *result = TETHER_ERROR_SERVICE_UNAVAIL;
    return NOERROR;
}

ECode CConnectivityManager::ReportInetCondition(
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 percentage)
{
    ECode ec = mService->ReportInetCondition(networkType, percentage);
    if (E_REMOTE_EXCEPTION == ec) {}
    return NOERROR;
}

ECode CConnectivityManager::ReportBadNetwork(
    /* [in] */ INetwork* network)
{
    VALIDATE_NOT_NULL(network)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Waiting for implementing.
        try {
            mService.reportBadNetwork(network);
        } catch (RemoteException e) {
        }
#endif
}

ECode CConnectivityManager::SetGlobalProxy(
    /* [in] */ IProxyInfo* proxyp)
{
    VALIDATE_NOT_NULL(proxyp)

#if 0
    ECode ec = mService->SetGlobalProxy(proxyp);
    if (E_REMOTE_EXCEPTION == ec) {}
#endif
    return NOERROR;
}

ECode CConnectivityManager::GetGlobalProxy(
    /* [out] */ IProxyInfo** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

#if 0
    AutoPtr<IProxyProperties> proxyp;
    ECode ec = mService->GetGlobalProxy((IProxyProperties**)&proxyp);
    if (E_REMOTE_EXCEPTION == ec) proxyp = NULL;
    *result = proxyp;
    REFCOUNT_ADD(*result);
#endif
    return NOERROR;
}

ECode CConnectivityManager::GetProxy(
    /* [out] */ IProxyInfo** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

#if 0
    AutoPtr<IProxyInfo> proxyp;
    ECode ec = mService->GetProxy((IProxyInfo**)&proxyp);
    if (E_REMOTE_EXCEPTION == ec) proxyp = NULL;
    *result = proxyp;
    REFCOUNT_ADD(*result);
#endif
    return NOERROR;
}

ECode CConnectivityManager::SetDataDependency(
    /* [in] */ Int32 networkType,
    /* [in] */ Boolean met)
{
    ECode ec = mService->SetDataDependency(networkType, met);
    if (E_REMOTE_EXCEPTION == ec) {}
    return NOERROR;
}

ECode CConnectivityManager::IsNetworkSupported(
    /* [in] */ Int32 networkType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    ECode ec = mService->IsNetworkSupported(networkType, result);
    if (E_REMOTE_EXCEPTION == ec) *result = FALSE;
    return NOERROR;
}

ECode CConnectivityManager::IsActiveNetworkMetered(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    ECode ec = mService->IsActiveNetworkMetered(result);
    if (E_REMOTE_EXCEPTION == ec) *result = FALSE;
    return NOERROR;
}


ECode CConnectivityManager::UpdateLockdownVpn(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    ECode ec = mService->UpdateLockdownVpn(result);
    if (E_REMOTE_EXCEPTION == ec) *result = FALSE;
    return NOERROR;
}

ECode CConnectivityManager::CaptivePortalCheckCompleted(
    /* [in] */ INetworkInfo* info,
    /* [in] */ Boolean isCaptivePortal)
{
    VALIDATE_NOT_NULL(info)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Waiting for implementing.
        try {
            mService.captivePortalCheckCompleted(info, isCaptivePortal);
        } catch (RemoteException e) {
        }
#endif
}

ECode CConnectivityManager::SupplyMessenger(
    /* [in] */ Int32 networkType,
    /* [in] */ IMessenger* messenger)
{
    VALIDATE_NOT_NULL(messenger)

    // try {
    ECode ec = mService->SupplyMessenger(networkType, messenger);
    //} catch (RemoteException e) {
    //}
    if (E_REMOTE_EXCEPTION == ec)
        return NOERROR;
    return ec;
}

ECode CConnectivityManager::CheckMobileProvisioning(
    /* [in] */ Int32 suggestedTimeOutMs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    Int32 timeOutMs = -1;
    // try {
    ECode ec = mService->CheckMobileProvisioning(suggestedTimeOutMs, &timeOutMs);
    // } catch (RemoteException e) {
    // }
    if (E_REMOTE_EXCEPTION == ec)
        return NOERROR;
    *result = timeOutMs;
    return ec;
}

ECode CConnectivityManager::GetMobileProvisioningUrl(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = String(NULL);

    // try {
    ECode ec = mService->GetMobileProvisioningUrl(result);
    // } catch (RemoteException e) {
    // }
    if (FAILED(ec)) {
        *result = String(NULL);
    }
    if (E_REMOTE_EXCEPTION == ec) {
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::GetMobileRedirectedProvisioningUrl(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = String(NULL);

    // try {
    ECode ec = mService->GetMobileRedirectedProvisioningUrl(result);
    // } catch (RemoteException e) {
    // }
    if (FAILED(ec)) {
        *result = String(NULL);
    }
    if (E_REMOTE_EXCEPTION == ec) {
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::GetLinkQualityInfo(
    /* [in] */ Int32 networkType,
    /* [out] */ ILinkQualityInfo** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;

#if 0
    // try {
    AutoPtr<ILinkQualityInfo> li;
    ECode ec = mService->GetLinkQualityInfo(networkType, &li);
    if (SUCCEEDED(ec)) {
        *result = li;
        REFCOUNT_ADD(*result)
        return NOERROR;
    }
    // } catch (RemoteException e) {
        // return null;
    // }
    if (E_REMOTE_EXCEPTION == ec) {
        return NOERROR;
    }
    return ec;
#endif
}

ECode CConnectivityManager::GetActiveLinkQualityInfo(
    /* [out] */ ILinkQualityInfo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0
        try {
            LinkQualityInfo li = mService.getActiveLinkQualityInfo();
            return li;
        } catch (RemoteException e) {
            return null;
        }
#endif
}

ECode CConnectivityManager::GetAllLinkQualityInfo(
    /* [out, callee] */ ArrayOf<ILinkQualityInfo>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0
        try {
            LinkQualityInfo[] li = mService.getAllLinkQualityInfo();
            return li;
        } catch (RemoteException e) {
            return null;
        }
#endif
}

ECode CConnectivityManager::SetProvisioningNotificationVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Int32 networkType,
    /* [in] */ const String& action)
{
    return E_NOT_IMPLEMENTED;
#if 0
        try {
            mService.setProvisioningNotificationVisible(visible, networkType, action);
        } catch (RemoteException e) {
        }
#endif
}

ECode CConnectivityManager::SetAirplaneMode(
        /* [in] */ Boolean enable)
{
    return E_NOT_IMPLEMENTED;
#if 0
        try {
            mService.setAirplaneMode(enable);
        } catch (RemoteException e) {
        }
#endif
}

ECode CConnectivityManager::RegisterNetworkFactory(
    /* [in] */ IMessenger* messenger,
    /* [in] */ const String& name)
{
    return E_NOT_IMPLEMENTED;
#if 0
        try {
            mService.registerNetworkFactory(messenger, name);
        } catch (RemoteException e) { }
#endif
}

ECode CConnectivityManager::UnregisterNetworkFactory(
    /* [in] */ IMessenger* messenger)
{
    return E_NOT_IMPLEMENTED;
#if 0
        try {
            mService.unregisterNetworkFactory(messenger);
        } catch (RemoteException e) { }
#endif
}

ECode CConnectivityManager::RegisterNetworkAgent(
    /* [in] */ IMessenger* messenger,
    /* [in] */ INetworkInfo* ni,
    /* [in] */ ILinkProperties* lp,
    /* [in] */ INetworkCapabilities* nc,
    /* [in] */ Int32 score,
    /* [in] */ INetworkMisc* misc)
{
    return E_NOT_IMPLEMENTED;
#if 0
        try {
            mService.registerNetworkAgent(messenger, ni, lp, nc, score, misc);
        } catch (RemoteException e) { }
#endif
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
