
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/net/CConnectivityManager.h"
// #include "net/NetworkUtils.h"
#include "elastos/droid/os/CBinder.h"
#include <elastos/core/StringUtils.h>
// #include "CHashMap.h"
// #include "CArrayMap.h"
#include <elastos/core/AutoLock.h>
// #include "CAtomicInteger32.h"
// #include "CNetworkCapabilities.h"
#include "elastos/droid/R.h"
#include "elastos/droid/Manifest.h"
// #include "CHandlerThread.h"
#include "elastos/droid/os/CMessenger.h"

using Elastos::Core::StringUtils;
// using Elastos::Droid::Net::NetworkUtils;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::CBinder;
// using Elastos::Utility::CHashMap;
using Elastos::Droid::Internal::Telephony::IPhoneConstants;
// using Elastos::Utility::Concurrent::Atomic::CAtomicInteger32;
using Elastos::Utility::ISet;
using Elastos::Utility::IIterator;
using Elastos::Droid::Os::IServiceManager;
// using Elastos::Droid::Os::CServiceManager;
// using Elastos::Droid::Net::CNetworkUtilsHelper;
using Elastos::Droid::Internal::Telephony::IITelephony;
using Elastos::Droid::Os::INetworkActivityListener;
using Elastos::Utility::IMap;
using Elastos::Droid::Os::EIID_INetworkActivityListener;
using Elastos::Droid::Content::Res::IResources;
// using Elastos::Droid::Os::CHandlerThread;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Core::IThread;
using Elastos::Droid::Os::CMessenger;
using Elastos::Net::IInetAddressHelper;
using Elastos::Net::CInetAddressHelper;
// using Libcore::Net::Event::INetworkEventDispatcherHelper;

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

AutoPtr<IAtomicInteger32> CreateAtomicInterger32()
{
    AutoPtr<IAtomicInteger32> rev;
#if 0 // TODO: Waiting for CAtomicInteger32
    CAtomicInterger32::New(0, (IAtomicInteger32**)&rev);
#else
    assert(0);
#endif
    return rev;
}

const String CConnectivityManager::TAG = String("ConnectivityManager");
const Boolean CConnectivityManager::LEGACY_DBG = true;
const AutoPtr<IHashMap> CConnectivityManager::sLegacyRequests = CreateHashMap();
const Int32 CConnectivityManager::BASE = 0x00080000;
const Int32 CConnectivityManager::EXPIRE_LEGACY_REQUEST = BASE + 10;
const String CConnectivityManager::CallbackHandler::TAG = String("ConnectivityManager.CallbackHandler");
const AutoPtr<IHashMap> CConnectivityManager::sNetworkCallback = CreateHashMap();
const AutoPtr<IAtomicInteger32> CConnectivityManager::sCallbackRefCount = CreateAtomicInterger32();
AutoPtr<IHandler> CConnectivityManager::sCallbackHandler = NULL;
const Int32 CConnectivityManager::LISTEN  = 1;
const Int32 CConnectivityManager::REQUEST = 2;

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
            *result = String("MOBILE");
            break;
        case TYPE_WIFI:
            *result = String("WIFI");
            break;
        case TYPE_MOBILE_MMS:
            *result = String("MOBILE_MMS");
            break;
        case TYPE_MOBILE_SUPL:
            *result = String("MOBILE_SUPL");
            break;
        case TYPE_MOBILE_DUN:
            *result = String("MOBILE_DUN");
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
    VALIDATE_NOT_NULL(result);
#if 0 // TODO: Waiting for IPhoneConstants
    *result = IPhoneConstants::APN_REQUEST_FAILED;
#else
    assert(0);
#endif

    AutoPtr<INetworkCapabilities> netCap;
    FAIL_RETURN(NetworkCapabilitiesForFeature(networkType, feature, (INetworkCapabilities**)&netCap))
    if (NULL == netCap) {
        // Log.d(TAG, "Can't satisfy startUsingNetworkFeature for " + networkType + ", " +
                // feature);
        Logger::D(TAG, (String("Can't satisfy startUsingNetworkFeature for ") + StringUtils::ToString(networkType) + ", " + feature).string());
#if 0 // TODO: Waiting for IPhoneConstants
        *result = IPhoneConstants::APN_REQUEST_FAILED;
#else
        assert(0);
#endif
        return NOERROR;
    }

    AutoPtr<INetworkRequest> request = NULL;
    synchronized(sLegacyRequests) {
        if(LEGACY_DBG) {
            String s;
            netCap->ToString(&s);
            Int32 code;
            IObject::Probe(netCap)->GetHashCode(&code);
            // Log.d(TAG, "Looking for legacyRequest for netCap with hash: " + netCap + " (" +
                    // netCap.hashCode() + ")");
            Logger::D(TAG, (String("Looking for legacyRequest for netCap with hash: ") + s + " (" + StringUtils::ToString(code) + ")").string());
            // Log.d(TAG, "sLegacyRequests has:");
            Logger::D(TAG, "sLegacyRequests has:");
            AutoPtr<ISet> keySet;
            FAIL_RETURN(sLegacyRequests->GetKeySet((ISet**)&keySet))
            AutoPtr<IIterator> it;
            FAIL_RETURN(keySet->GetIterator((IIterator**)&it))
            Boolean hasNext = TRUE;
            AutoPtr<INetworkCapabilities> nc;
            AutoPtr<IInterface> tmp;
            while(it->HasNext(&hasNext), hasNext)
            {
                it->GetNext((IInterface**)&tmp);
                nc = INetworkCapabilities::Probe(tmp);
                IObject::Probe(nc)->GetHashCode(&code);
                IObject::Probe(nc)->ToString(&s);
                // Log.d(TAG, "Can't satisfy startUsingNetworkFeature for " + networkType + ", " +
                        // feature);
                Logger::D(TAG, (String("  ") + s + " (" + StringUtils::ToString(code) + ")").string());
            }
        }

        AutoPtr<IInterface> l;
        sLegacyRequests->Get(netCap, (IInterface**)&l);
        if(l != NULL) {
            LegacyRequest* lr = (LegacyRequest*) IObject::Probe(l.Get());
            String s;
            IObject::Probe(lr->mNetworkRequest)->ToString(&s);
            // Log.d(TAG, "renewing startUsingNetworkFeature request " + l.networkRequest);
            Logger::D(TAG, (String("renewing startUsingNetworkFeature request ") + s).string());
            FAIL_RETURN(RenewRequestLocked((LegacyRequest*)IObject::Probe(l.Get())))
            if (lr->mCurrentNetwork != NULL) {
#if 0 // TODO: Waiting for IPhoneConstants
                *result = IPhoneConstants::APN_ALREADY_ACTIVE;
#else
                assert(0);
#endif
                return NOERROR;
            } else {
#if 0 // TODO: Waiting for IPhoneConstants
                *result = IPhoneConstants::APN_REQUEST_STARTED;
#else
                assert(0);
#endif
                return NOERROR;
            }

            FAIL_RETURN(RequestNetworkForFeatureLocked(netCap, (INetworkRequest**)&request))
        }
    }

    if (request != NULL) {
        String s;
        IObject::Probe(request)->ToString(&s);
        // Log.d(TAG, "starting startUsingNetworkFeature for request " + request);
        Logger::D(TAG, (String("starting startUsingNetworkFeature for request ") + s).string());
#if 0 // TODO: Waiting for IPhoneConstants
        *result = IPhoneConstants::APN_REQUEST_STARTED;
#else
        assert(0);
#endif
        return NOERROR;
    } else {
        // Log.d(TAG, " request Failed");
        Logger::D(TAG, " request Failed");
#if 0 // TODO: Waiting for IPhoneConstants
        *result = IPhoneConstants::APN_REQUEST_FAILED;
#else
        assert(0);
#endif
        return NOERROR;
    }
}

ECode CConnectivityManager::StopUsingNetworkFeature(
    /* [in] */ Int32 networkType,
    /* [in] */ const String& feature,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = -1;

    AutoPtr<INetworkCapabilities> netCap;
    FAIL_RETURN(NetworkCapabilitiesForFeature(networkType, feature, (INetworkCapabilities**)&netCap))
    if (NULL == netCap) {
        // Log.d(TAG, "Can't satisfy stopUsingNetworkFeature for " + networkType + ", " +
                // feature);
        Logger::D(TAG, (String("Can't satisfy stopUsingNetworkFeature for ") + StringUtils::ToString(networkType) + ", " + feature).string());
        *result = -1;
        return NOERROR;
    }

    AutoPtr<IConnectivityManagerNetworkCallback> networkCallback;
    FAIL_RETURN(RemoveRequestForFeature(netCap, (IConnectivityManagerNetworkCallback**)&networkCallback))
    if (networkCallback != NULL) {
        // Log.d(TAG, "stopUsingNetworkFeature for " + networkType + ", " + feature);
        Logger::D(TAG, (String("stopUsingNetworkFeature for ") + StringUtils::ToString(networkType) + ", " + feature).string());
        FAIL_RETURN(UnregisterNetworkCallback(networkCallback))
    }
    *result = 1;
    return NOERROR;
}

ECode CConnectivityManager::MaybeMarkCapabilitiesRestricted(
    /* [in] */ INetworkCapabilities* nc)
{
    VALIDATE_NOT_NULL(nc)

    AutoPtr<ArrayOf<Int32> > cap;
    FAIL_RETURN(nc->GetCapabilities((ArrayOf<Int32>**)&cap))
    for (Int32 i = 0; i < cap->GetLength(); ++i) {
        switch ((*cap)[i]) {
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
    AutoPtr<INetworkCapabilities> tmp;
    return nc->RemoveCapability(INetworkCapabilities::NET_CAPABILITY_NOT_RESTRICTED, (INetworkCapabilities**)&tmp);
}

ECode CConnectivityManager::NetworkCapabilitiesForFeature(
    /* [in] */ Int32 networkType,
    /* [in] */ const String& feature,
    /* [out] */ INetworkCapabilities** result)
{
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
#if 0 // TODO: Waiting for CNetworkCapabilities
        CNetworkCapabilities::New((INetworkCapabilities**)&netCap);
#else
        assert(0);
#endif
        AutoPtr<INetworkCapabilities> tmp;
        FAIL_RETURN(netCap->AddTransportType(INetworkCapabilities::TRANSPORT_CELLULAR, (INetworkCapabilities**)&tmp))
        FAIL_RETURN(tmp->AddCapability(cap, (INetworkCapabilities**)&netCap))
        FAIL_RETURN(MaybeMarkCapabilitiesRestricted(netCap))
        *result = netCap;
        REFCOUNT_ADD(*result)
        return NOERROR;
    } else if (TYPE_WIFI == networkType) {
        if (feature.Equals("p2p")) {
            AutoPtr<INetworkCapabilities> netCap;
#if 0 // TODO: Waiting for CNetworkCapabilities
            CNetworkCapabilities::New((INetworkCapabilities**)&netCap);
#else
            assert(0);
#endif
            AutoPtr<INetworkCapabilities> tmp;
            FAIL_RETURN(netCap->AddTransportType(INetworkCapabilities::TRANSPORT_WIFI, (INetworkCapabilities**)&tmp))
            FAIL_RETURN(netCap->AddCapability(INetworkCapabilities::NET_CAPABILITY_WIFI_P2P, (INetworkCapabilities**)&tmp))
            FAIL_RETURN(MaybeMarkCapabilitiesRestricted(netCap))
            *result = netCap;
            REFCOUNT_ADD(*result)
            return NOERROR;
        }
    }
    *result = NULL;
    return NOERROR;
}

ECode CConnectivityManager::InferLegacyTypeForNetworkCapabilities(
    /* [in] */ INetworkCapabilities* netCap,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = TYPE_NONE;

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
        FAIL_RETURN(NetworkCapabilitiesForFeature(TYPE_MOBILE, String("enableCBS"), (INetworkCapabilities**)&networkCapabilities))
        if (IObject::Probe(netCap)->Equals(networkCapabilities, &b), b) {
            *result = TYPE_MOBILE_CBS;
            return NOERROR;
        } else {
            *result = TYPE_NONE;
            return NOERROR;
        }
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_IMS, &b), b) {
        FAIL_RETURN(NetworkCapabilitiesForFeature(TYPE_MOBILE, String("enableIMS"), (INetworkCapabilities**)&networkCapabilities))
        if (IObject::Probe(netCap)->Equals(networkCapabilities, &b), b) {
            *result = TYPE_MOBILE_IMS;
            return NOERROR;
        } else {
            *result = TYPE_NONE;
            return NOERROR;
        }
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_FOTA, &b), b) {
        FAIL_RETURN(NetworkCapabilitiesForFeature(TYPE_MOBILE, String("enableFOTA"), (INetworkCapabilities**)&networkCapabilities))
        if (IObject::Probe(netCap)->Equals(networkCapabilities, &b), b) {
            *result = TYPE_MOBILE_FOTA;
            return NOERROR;
        } else {
            *result = TYPE_NONE;
            return NOERROR;
        }
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_DUN, &b), b) {
        FAIL_RETURN(NetworkCapabilitiesForFeature(TYPE_MOBILE, String("enableDUN"), (INetworkCapabilities**)&networkCapabilities))
        if (IObject::Probe(netCap)->Equals(networkCapabilities, &b), b) {
            *result = TYPE_MOBILE_DUN;
            return NOERROR;
        } else {
            *result = TYPE_NONE;
            return NOERROR;
        }
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_SUPL, &b), b) {
        FAIL_RETURN(NetworkCapabilitiesForFeature(TYPE_MOBILE, String("enableSUPL"), (INetworkCapabilities**)&networkCapabilities))
        if (IObject::Probe(netCap)->Equals(networkCapabilities, &b), b) {
            *result = TYPE_MOBILE_SUPL;
            return NOERROR;
        } else {
            *result = TYPE_NONE;
            return NOERROR;
        }
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_MMS, &b), b) {
        FAIL_RETURN(NetworkCapabilitiesForFeature(TYPE_MOBILE, String("enableMMS"), (INetworkCapabilities**)&networkCapabilities))
        if (IObject::Probe(netCap)->Equals(networkCapabilities, &b), b) {
            *result = TYPE_MOBILE_MMS;
            return NOERROR;
        } else {
            *result = TYPE_NONE;
            return NOERROR;
        }
    }
    if (netCap->HasCapability(INetworkCapabilities::NET_CAPABILITY_INTERNET, &b), b) {
        FAIL_RETURN(NetworkCapabilitiesForFeature(TYPE_MOBILE, String("enableHIPRI"), (INetworkCapabilities**)&networkCapabilities))
        if (IObject::Probe(netCap)->Equals(networkCapabilities, &b), b) {
            *result = TYPE_MOBILE_HIPRI;
            return NOERROR;
        } else {
            *result = TYPE_NONE;
            return NOERROR;
        }
    }
    *result = TYPE_NONE;
    return NOERROR;
}

ECode CConnectivityManager::LegacyTypeForNetworkCapabilities(
    /* [in] */ INetworkCapabilities* netCap,
    /* [out] */ Int32* result)
{
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
   return NOERROR;
}

ECode CConnectivityManager::FindRequestForFeature(
    /* [in] */ INetworkCapabilities* netCap,
    /* [out] */ INetworkRequest** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;
    VALIDATE_NOT_NULL(netCap)

    synchronized(sLegacyRequests) {
        AutoPtr<IInterface> l;
        sLegacyRequests->Get(netCap, (IInterface**)&l);
        if (l != NULL) {
            LegacyRequest* lr = (LegacyRequest*) IObject::Probe(l.Get());
            *result = lr->mNetworkRequest;
            REFCOUNT_ADD(*result);
            return NOERROR;
        }
    }
    *result = NULL;
    return NOERROR;
}

ECode CConnectivityManager::RenewRequestLocked(
    /* [in] */ LegacyRequest* l)
{
    VALIDATE_NOT_NULL(l)

    l->mExpireSequenceNumber++;
    // Log.d(TAG, "renewing request to seqNum " + l.expireSequenceNumber);
    Logger::D(TAG, (String("renewing request to seqNum ") + StringUtils::ToString(l->mExpireSequenceNumber)).string());
    FAIL_RETURN(SendExpireMsgForFeature(l->mNetworkCapabilities, l->mExpireSequenceNumber, l->mDelay))
    return NOERROR;
}

ECode CConnectivityManager::ExpireRequest(
    /* [in] */ INetworkCapabilities* netCap,
    /* [in] */ Int32 sequenceNum)
{
    VALIDATE_NOT_NULL(netCap)

    Int32 ourSeqNum = -1;
    synchronized(sLegacyRequests) {
        AutoPtr<IInterface> l;
        sLegacyRequests->Get(netCap, (IInterface**)&l);
        LegacyRequest* lr = (LegacyRequest*) IObject::Probe(l.Get());
        if (NULL == lr)
            return NOERROR;
        ourSeqNum = lr->mExpireSequenceNumber;
        if (lr->mExpireSequenceNumber == sequenceNum) {
            FAIL_RETURN(UnregisterNetworkCallback(lr->mNetworkCallback))
            sLegacyRequests->Remove(netCap);
        }
    }
    // Log.d(TAG, "expireRequest with " + ourSeqNum + ", " + sequenceNum);
    Logger::D(TAG, (String("expireRequest with ") + StringUtils::ToString(ourSeqNum) + ", " + StringUtils::ToString(sequenceNum)).string());
    return NOERROR;
}

ECode CConnectivityManager::RequestNetworkForFeatureLocked(
    /* [in] */ INetworkCapabilities* netCap,
    /* [out] */ INetworkRequest** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;
    VALIDATE_NOT_NULL(netCap)

    Int32 delay = -1;
    Int32 type;
    FAIL_RETURN(LegacyTypeForNetworkCapabilities(netCap, &type))
    // try {
        // delay = mService.getRestoreDefaultNetworkDelay(type);
    // } catch (RemoteException e) {}
    ECode ec = mService->GetRestoreDefaultNetworkDelay(type, &delay);
    if (FAILED(ec))
    {
        if (ec != E_REMOTE_EXCEPTION)
            return ec;
    }
    LegacyRequest* pl = new LegacyRequest;
    AutoPtr<IInterface> l = pl->Probe(EIID_IObject);
    pl->mNetworkCapabilities = netCap;
    pl->mDelay = delay;
    pl->mExpireSequenceNumber = 0;
    FAIL_RETURN(SendRequestForNetwork(netCap, pl->mNetworkCallback, 0, REQUEST, type, (INetworkRequest**)&(pl->mNetworkRequest)))
    if (NULL == pl->mNetworkRequest) {
        *result = NULL;
        return NOERROR;
    }
    sLegacyRequests->Put(netCap, l);
    FAIL_RETURN(SendExpireMsgForFeature(netCap, pl->mExpireSequenceNumber, delay))
    *result = pl->mNetworkRequest;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CConnectivityManager::SendExpireMsgForFeature(
    /* [in] */ INetworkCapabilities* netCap,
    /* [in] */ Int32 seqNum,
    /* [in] */ Int32 delay)
{
    VALIDATE_NOT_NULL(netCap)

    if (delay >= 0) {
        // Log.d(TAG, "sending expire msg with seqNum " + seqNum + " and delay " + delay);
        Logger::D(TAG, (String("sending expire msg with seqNum ") + StringUtils::ToString(seqNum) + " and delay " + StringUtils::ToString(delay)).string());
        AutoPtr<IMessage> msg;
        FAIL_RETURN(sCallbackHandler->ObtainMessage(EXPIRE_LEGACY_REQUEST, seqNum, 0, netCap, (IMessage**)&msg))
        Boolean b;
        FAIL_RETURN(sCallbackHandler->SendMessageDelayed(msg, delay, &b))
    }
    return NOERROR;
}

ECode CConnectivityManager::RemoveRequestForFeature(
    /* [in] */ INetworkCapabilities* netCap,
    /* [out] */ IConnectivityManagerNetworkCallback** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;
    VALIDATE_NOT_NULL(netCap)

    synchronized (sLegacyRequests) {
        AutoPtr<IInterface> pl;
        FAIL_RETURN(sLegacyRequests->Remove(netCap, (IInterface**)&pl))
        LegacyRequest* l = (LegacyRequest*) IObject::Probe(pl);
        if (NULL == l) {
            *result = NULL;
            return NOERROR;
        }
        *result = l->mNetworkCallback;
        REFCOUNT_ADD(*result)
    }
    return NOERROR;
}

ECode CConnectivityManager::RequestRouteToHost(
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 hostAddress,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    AutoPtr<IInetAddress> inetAddress;
    AutoPtr<INetworkUtilsHelper> networkUtilsHelper;
#if 0 // TODO: Waiting for CNetworkUtilsHelper
    CNetworkUtilsHelper::AcquireSingleton((INetworkUtilsHelper**)&networkUtilsHelper);
#else
    assert(0);
#endif
    FAIL_RETURN(networkUtilsHelper->IntToInetAddress(hostAddress, (IInetAddress**)&inetAddress))
    return RequestRouteToHostAddress(networkType, inetAddress, result);
}

ECode CConnectivityManager::RequestRouteToHostAddress(
    /* [in] */ Int32 networkType,
    /* [in] */ IInetAddress* hostAddress,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(hostAddress)

    // try {
    //     return mService.requestRouteToHostAddress(networkType, hostAddress.getAddress());
    // } catch (RemoteException e) {
    //     return false;
    // }
    AutoPtr<ArrayOf<Byte> > address;
    hostAddress->GetAddress((ArrayOf<Byte>**)&address);
    ECode ec = mService->RequestRouteToHostAddress(networkType, address, result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = FALSE;
        return NOERROR;
    }
    return ec;
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

    AutoPtr<IServiceManager> serviceManager;
#if 0 // TODO: Waiting for CServiceManager, IServiceManager
    CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager);
#else
    assert(0);
#endif
    AutoPtr<IBinder> b;
    AutoPtr<IInterface> itmp;
    FAIL_RETURN(serviceManager->GetService(IContext::TELEPHONY_SERVICE, (IInterface**)&itmp))
    b = IBinder::Probe(itmp);
    if (b != NULL) {
        // try {
        //     ITelephony it = ITelephony.Stub.asInterface(b);
        //     return it.getDataEnabled();
        // } catch (RemoteException e) { }
        AutoPtr<IITelephony> it = IITelephony::Probe(b);
#if 0 // TODO: Waiting for IITelephony
        ECode ec = it->GetDataEnabled(result);
        if (E_REMOTE_EXCEPTION == ec) {
            return NOERROR;
        }
        return ec;
#else
        assert(0);
#endif
    }
    *result = FALSE;
    return NOERROR;
}

ECode CConnectivityManager::GetNetworkManagementService(
    /* [out] */ INetworkManagementService** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    synchronized(this) {
        if (mNMService != NULL) {
            *result = mNMService;
            return NOERROR;
        }
        AutoPtr<IBinder> b;
        AutoPtr<IServiceManager> sm;
#if 0 // TODO: Waiting for CServiceManager
        CServiceManager::AcquireSingleton((IServiceManager**)&sm);
#else
        assert(0);
#endif
        AutoPtr<IInterface> itmp;
        FAIL_RETURN(sm->GetService(IContext::NETWORKMANAGEMENT_SERVICE, (IInterface**)&itmp))
        b = IBinder::Probe(itmp);
        mNMService = INetworkManagementService::Probe(b);
        *result = mNMService;
    }
    return NOERROR;
}

ECode CConnectivityManager::AddDefaultNetworkActiveListener(
    /* [in] */ IConnectivityManagerOnNetworkActiveListener* l)
{
    VALIDATE_NOT_NULL(l)

    class InnerSub_INetworkActivityListener
        : public Object
        , public INetworkActivityListener
    {
    public:
        CAR_INNER_INTERFACE_IMPL(Object, INetworkActivityListener)

        InnerSub_INetworkActivityListener(IConnectivityManagerOnNetworkActiveListener* l)
            : mListener(l)
        {}

        // @Override
        ECode OnNetworkActive()
        {
            mListener->OnNetworkActive();
            return NOERROR;
        }
    private:
        IConnectivityManagerOnNetworkActiveListener* mListener;
    };

    AutoPtr<INetworkActivityListener> rl = new InnerSub_INetworkActivityListener(l);

    // try {
    //     getNetworkManagementService().registerNetworkActivityListener(rl);
    //     mNetworkActivityListeners.put(l, rl);
    // } catch (RemoteException e) {
    // }
    AutoPtr<INetworkManagementService> nms;
    ECode ec = GetNetworkManagementService((INetworkManagementService**)&nms);
    if (FAILED(ec)) {
        if (E_REMOTE_EXCEPTION == ec)
            return NOERROR;
        return ec;
    }
    ec = nms->RegisterNetworkActivityListener(rl);
    if (FAILED(ec)) {
        if (E_REMOTE_EXCEPTION == ec)
            return NOERROR;
        return ec;
    }
    ec = IMap::Probe(mNetworkActivityListeners)->Put(l, rl);
    if (E_REMOTE_EXCEPTION == ec)
        return NOERROR;
    return ec;
}

ECode CConnectivityManager::RemoveDefaultNetworkActiveListener(
    /* [in] */ IConnectivityManagerOnNetworkActiveListener* l)
{
    AutoPtr<IInterface> itmp;
    IMap::Probe(mNetworkActivityListeners)->Get(l, (IInterface**)&itmp);
    AutoPtr<INetworkActivityListener> rl = INetworkActivityListener::Probe(itmp);
    if (NULL == rl) {
        // throw new IllegalArgumentException("Listener not registered: " + l);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // try {
    //     getNetworkManagementService().unregisterNetworkActivityListener(rl);
    // } catch (RemoteException e) {
    // }
    AutoPtr<INetworkManagementService> nms;
    ECode ec = GetNetworkManagementService((INetworkManagementService**)&nms);
    if (FAILED(ec)) {
        if (E_REMOTE_EXCEPTION == ec)
            return NOERROR;
        return ec;
    }
    ec = nms->RegisterNetworkActivityListener(rl);
    if (E_REMOTE_EXCEPTION == ec)
        return NOERROR;
    return ec;
}

ECode CConnectivityManager::IsDefaultNetworkActive(
    /* [out] */ Boolean* result)
{
    // try {
    //     return getNetworkManagementService().isNetworkActive();
    // } catch (RemoteException e) {
    // }
    AutoPtr<INetworkManagementService> nms;
    ECode ec = GetNetworkManagementService((INetworkManagementService**)&nms);
    if (FAILED(ec)) {
        if (E_REMOTE_EXCEPTION != ec) {
            return ec;
        }
        *result = false;
        return NOERROR;
    }
    ec = nms->IsNetworkActive(result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = false;
        return NOERROR;
    }
    return ec;
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
    VALIDATE_NOT_NULL(context)

    AutoPtr<IResources> res;
    FAIL_RETURN(context->GetResources((IResources**)&res))
    AutoPtr<ArrayOf<String> > stringArray;
    FAIL_RETURN(res->GetStringArray(Elastos::Droid::R::array::config_mobile_hotspot_provision_app, (ArrayOf<String>**)&stringArray))
    if(stringArray->GetLength() == 2) {
        // Have a provisioning app - must only let system apps (which check this app)
        // turn on tethering
#if 0 // TODO: Waiting for Manifest.cpp add to sources
        FAIL_RETURN(context->EnforceCallingOrSelfPermission(
                Elastos::Droid::Manifest::Permission::CONNECTIVITY_INTERNAL, String("ConnectivityService")))
#else
        assert(0);
#endif
    } else {
#if 0 // TODO: Waiting for Manifest.cpp add to sources
        FAIL_RETURN(context->EnforceCallingOrSelfPermission(
                Elastos::Droid::Manifest::Permission::CHANGE_NETWORK_STATE, String("ConnectivityService")))
#else
        assert(0);
#endif
    }
    return NOERROR;
}

ECode CConnectivityManager::GetTetherableIfaces(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    ECode ec = mService->GetTetherableIfaces(result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = ArrayOf<String>::Alloc(0);
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::GetTetheredIfaces(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    AutoPtr <ArrayOf<String> > outputArray;
    ECode ec = mService->GetTetheredIfaces(result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = ArrayOf<String>::Alloc(0);
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::GetTetheringErroredIfaces(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    ECode ec = mService->GetTetheringErroredIfaces(result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = ArrayOf<String>::Alloc(0);
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::GetTetheredDhcpRanges(
    /* [out] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;

    ECode ec = mService->GetTetheredDhcpRanges((ArrayOf<String>**)&result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = ArrayOf<String>::Alloc(0);
        REFCOUNT_ADD(*result)
        return NOERROR;
    }
    return ec;
}

ECode  CConnectivityManager::Tether(
    /* [in] */ const String& iface,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TETHER_ERROR_UNSUPPORTED;

    ECode ec = mService->Tether(iface, result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = TETHER_ERROR_SERVICE_UNAVAIL;
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::Untether(
    /* [in] */ const String& iface,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TETHER_ERROR_UNSUPPORTED;

    ECode ec = mService->Untether(iface, result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = TETHER_ERROR_SERVICE_UNAVAIL;
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::IsTetheringSupported(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    result = FALSE;

    ECode ec = mService->IsTetheringSupported(result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = FALSE;
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::GetTetherableUsbRegexs(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    ECode ec = mService->GetTetherableUsbRegexs(result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = ArrayOf<String>::Alloc(0);
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::GetTetherableWifiRegexs(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    ECode ec = mService->GetTetherableWifiRegexs(result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = ArrayOf<String>::Alloc(0);
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::GetTetherableBluetoothRegexs(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    ECode ec = mService->GetTetherableBluetoothRegexs(result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = ArrayOf<String>::Alloc(0);
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::SetUsbTethering(
    /* [in] */ Boolean enabled,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TETHER_ERROR_UNSUPPORTED;

    ECode ec = mService->SetUsbTethering(enabled, result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = TETHER_ERROR_SERVICE_UNAVAIL;
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::GetLastTetherError(
    /* [in] */ const String& iface,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TETHER_ERROR_UNSUPPORTED;

    ECode ec = mService->GetLastTetherError(iface, result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = TETHER_ERROR_SERVICE_UNAVAIL;
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::ReportInetCondition(
    /* [in] */ Int32 networkType,
    /* [in] */ Int32 percentage)
{
    ECode ec = mService->ReportInetCondition(networkType, percentage);
    if (E_REMOTE_EXCEPTION == ec) {
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::ReportBadNetwork(
    /* [in] */ INetwork* network)
{
    VALIDATE_NOT_NULL(network)

    ECode ec = mService->ReportBadNetwork(network);
    if (E_REMOTE_EXCEPTION == ec) {
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::SetGlobalProxy(
    /* [in] */ IProxyInfo* proxyp)
{
    VALIDATE_NOT_NULL(proxyp)

    ECode ec = mService->SetGlobalProxy(proxyp);
    if (E_REMOTE_EXCEPTION == ec) {
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::GetGlobalProxy(
    /* [out] */ IProxyInfo** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    ECode ec = mService->GetGlobalProxy(result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = NULL;
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::GetProxy(
    /* [out] */ IProxyInfo** result)
{
    VALIDATE_NOT_NULL(*result);
    *result = NULL;

    ECode ec = mService->GetProxy(result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = NULL;
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::SetDataDependency(
    /* [in] */ Int32 networkType,
    /* [in] */ Boolean met)
{
    ECode ec = mService->SetDataDependency(networkType, met);
    if (E_REMOTE_EXCEPTION == ec) {
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::IsNetworkSupported(
    /* [in] */ Int32 networkType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    ECode ec = mService->IsNetworkSupported(networkType, result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = FALSE;
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::IsActiveNetworkMetered(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    ECode ec = mService->IsActiveNetworkMetered(result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = FALSE;
        return NOERROR;
    }
    return ec;
}


ECode CConnectivityManager::UpdateLockdownVpn(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    ECode ec = mService->UpdateLockdownVpn(result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = FALSE;
        return NOERROR;
    }
    return NOERROR;
}

ECode CConnectivityManager::CaptivePortalCheckCompleted(
    /* [in] */ INetworkInfo* info,
    /* [in] */ Boolean isCaptivePortal)
{
    VALIDATE_NOT_NULL(info)

    // try {
    //     mService.captivePortalCheckCompleted(info, isCaptivePortal);
    // } catch (RemoteException e) {
    // }
    ECode ec = mService->CaptivePortalCheckCompleted(info, isCaptivePortal);
    if (E_REMOTE_EXCEPTION == ec) {
        return NOERROR;
    }
    return ec;
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

    // try {
    //     LinkQualityInfo li = mService.getLinkQualityInfo(networkType);
    //     return li;
    // } catch (RemoteException e) {
    //     return NULL;
    // }

    ECode ec = mService->GetLinkQualityInfo(networkType, result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = NULL;
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::GetActiveLinkQualityInfo(
    /* [out] */ ILinkQualityInfo** result)
{
    // try {
    //     LinkQualityInfo li = mService.getActiveLinkQualityInfo();
    //     return li;
    // } catch (RemoteException e) {
    //     return NULL;
    // }
    ECode ec = mService->GetActiveLinkQualityInfo(result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = NULL;
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::GetAllLinkQualityInfo(
    /* [out, callee] */ ArrayOf<ILinkQualityInfo*>** result)
{
    // try {
    //     LinkQualityInfo[] li = mService.getAllLinkQualityInfo();
    //     return li;
    // } catch (RemoteException e) {
    //     return NULL;
    // }
    ECode ec = mService->GetAllLinkQualityInfo(result);
    if (E_REMOTE_EXCEPTION == ec) {
        *result = NULL;
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::SetProvisioningNotificationVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Int32 networkType,
    /* [in] */ const String& action)
{
    // try {
    //     mService.setProvisioningNotificationVisible(visible, networkType, action);
    // } catch (RemoteException e) {
    // }
    ECode ec = mService->SetProvisioningNotificationVisible(visible, networkType, action);
    if (E_REMOTE_EXCEPTION == ec) {
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::SetAirplaneMode(
        /* [in] */ Boolean enable)
{
    // try {
    //     mService.setAirplaneMode(enable);
    // } catch (RemoteException e) {
    // }
    ECode ec = mService->SetAirplaneMode(enable);
    if (E_REMOTE_EXCEPTION == ec) {
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::RegisterNetworkFactory(
    /* [in] */ IMessenger* messenger,
    /* [in] */ const String& name)
{
    // try {
    //     mService.registerNetworkFactory(messenger, name);
    // } catch (RemoteException e) { }
    ECode ec = mService->RegisterNetworkFactory(messenger, name);
    if (E_REMOTE_EXCEPTION == ec) {
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::UnregisterNetworkFactory(
    /* [in] */ IMessenger* messenger)
{
    // try {
    //     mService.unregisterNetworkFactory(messenger);
    // } catch (RemoteException e) { }
    ECode ec = mService->UnregisterNetworkFactory(messenger);
    if (E_REMOTE_EXCEPTION == ec) {
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::RegisterNetworkAgent(
    /* [in] */ IMessenger* messenger,
    /* [in] */ INetworkInfo* ni,
    /* [in] */ ILinkProperties* lp,
    /* [in] */ INetworkCapabilities* nc,
    /* [in] */ Int32 score,
    /* [in] */ INetworkMisc* misc)
{
    // try {
    //     mService.registerNetworkAgent(messenger, ni, lp, nc, score, misc);
    // } catch (RemoteException e) { }
    ECode ec = mService->RegisterNetworkAgent(messenger, ni, lp, nc, score, misc);
    if (E_REMOTE_EXCEPTION == ec) {
        return NOERROR;
    }
    return ec;
}

//================================================================================
// CConnectivityManager::ConnectivityManagerNetworkCallback
//================================================================================
CAR_INTERFACE_IMPL(CConnectivityManager::ConnectivityManagerNetworkCallback, Object, IConnectivityManagerNetworkCallback)

ECode CConnectivityManager::ConnectivityManagerNetworkCallback::constructor()
{
    return NOERROR;
}

ECode CConnectivityManager::ConnectivityManagerNetworkCallback::OnPreCheck(
    /* [in] */ INetwork* network)
{
    VALIDATE_NOT_NULL(network)

    return NOERROR;
}


ECode CConnectivityManager::ConnectivityManagerNetworkCallback::OnAvailable(
            /* [in] */ INetwork* network)
{
    VALIDATE_NOT_NULL(network)

    return NOERROR;
}

ECode CConnectivityManager::ConnectivityManagerNetworkCallback::OnLosing(
    /* [in] */ INetwork* network,
    /* [in] */ Int32 maxMsToLive)
{
    VALIDATE_NOT_NULL(network)

    return NOERROR;
}

ECode CConnectivityManager::ConnectivityManagerNetworkCallback::OnLost(
    /* [in] */ INetwork* network)
{
    VALIDATE_NOT_NULL(network)

    return NOERROR;
}

ECode CConnectivityManager::ConnectivityManagerNetworkCallback::OnUnavailable()
{
    return NOERROR;
}

ECode CConnectivityManager::ConnectivityManagerNetworkCallback::OnCapabilitiesChanged(
    /* [in] */ INetwork* network,
    /* [in] */ INetworkCapabilities* networkCapabilities)
{
    VALIDATE_NOT_NULL(network)
    VALIDATE_NOT_NULL(networkCapabilities)

    return NOERROR;
}

ECode CConnectivityManager::ConnectivityManagerNetworkCallback::OnLinkPropertiesChanged(
    /* [in] */ INetwork* network,
    /* [in] */ ILinkProperties* linkProperties)
{
    VALIDATE_NOT_NULL(network)
    VALIDATE_NOT_NULL(linkProperties)

    return NOERROR;
}

ECode CConnectivityManager::IncCallbackHandlerRefCount()
{
    synchronized(sCallbackRefCount) {
        Int32 num;
        FAIL_RETURN(sCallbackRefCount->IncrementAndGet(&num))
        if (1 == num) {
            // TODO - switch this over to a ManagerThread or expire it when done
            AutoPtr<IHandlerThread> callbackThread;
#if 0 // TODO: Waiting for CHandlerThread
            CHandlerThread::New("ConnectivityManager", (IHandlerThread**)&callbackThread);
#else
            assert(0);
#endif
            FAIL_RETURN(IThread::Probe(callbackThread)->Start())
            AutoPtr<ILooper> looper;
            FAIL_RETURN(callbackThread->GetLooper((ILooper**)&looper))
            sCallbackHandler = new CallbackHandler(looper, sNetworkCallback, sCallbackRefCount, this);
        }
    }
    return NOERROR;
}

ECode CConnectivityManager::DecCallbackHandlerRefCount()
{
    synchronized(sCallbackRefCount) {
        Int32 num;
        FAIL_RETURN(sCallbackRefCount->DecrementAndGet(&num))
        if (0 == num) {
            AutoPtr<IMessage> msg;
            FAIL_RETURN(sCallbackHandler->ObtainMessage(CALLBACK_EXIT, (IMessage**)&msg))
            FAIL_RETURN(msg->SendToTarget())
            sCallbackHandler = NULL;
        }
    }
    return NOERROR;
}

ECode CConnectivityManager::SendRequestForNetwork(
        /* [in] */ INetworkCapabilities* need,
        /* [in] */ IConnectivityManagerNetworkCallback* networkCallback,
        /* [in] */ Int32 timeoutSec,
        /* [in] */ Int32 action,
        /* [in] */ Int32 legacyType,
        /* [out] */ INetworkRequest** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;

    if (NULL == networkCallback) {
        // throw new IllegalArgumentException("NULL NetworkCallback");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (NULL == need) {
        // throw new IllegalArgumentException("NULL NetworkCapabilities");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // try {
    ECode ec = IncCallbackHandlerRefCount();
    if (FAILED(ec) && ec != E_REMOTE_EXCEPTION) {
        return ec;
    }
    synchronized(sNetworkCallback) {
        AutoPtr<IMessenger> msg;
        AutoPtr<IBinder> binder;
        CMessenger::New(sCallbackHandler, (IMessenger**)&msg);
        CBinder::New((IBinder**)&binder);
        if (LISTEN == action) {
            ec = mService->ListenForNetwork(need, msg, binder, (INetworkRequest**)&(((ConnectivityManagerNetworkCallback*)networkCallback)->mNetworkRequest));
            if (FAILED(ec) && ec != E_REMOTE_EXCEPTION) {
                return ec;
            }
        } else {
            ec = mService->RequestNetwork(need, msg, timeoutSec, binder, legacyType, (INetworkRequest**)&(((ConnectivityManagerNetworkCallback*)networkCallback)->mNetworkRequest));
            if (FAILED(ec) && ec != E_REMOTE_EXCEPTION) {
                return ec;
            }
        }
        if (((ConnectivityManagerNetworkCallback*)networkCallback)->mNetworkRequest != NULL) {
            sNetworkCallback->Put(((ConnectivityManagerNetworkCallback*)networkCallback)->mNetworkRequest, networkCallback);
        }
    }
    // } catch (RemoteException e) {}
    if (NULL == ((ConnectivityManagerNetworkCallback*)networkCallback)->mNetworkRequest) {
        FAIL_RETURN(DecCallbackHandlerRefCount())
    }
    *result = ((ConnectivityManagerNetworkCallback*)networkCallback)->mNetworkRequest;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CConnectivityManager::RequestNetwork(
    /* [in] */ INetworkRequest* request,
    /* [in] */ IConnectivityManagerNetworkCallback* networkCallback)
{
    VALIDATE_NOT_NULL(request)
    VALIDATE_NOT_NULL(networkCallback)

    AutoPtr<INetworkCapabilities> nc;
    request->GetNetworkCapabilities((INetworkCapabilities**)&nc);
    Int32 type;
    FAIL_RETURN(InferLegacyTypeForNetworkCapabilities(nc, &type))
    AutoPtr<INetworkRequest> nr;
    FAIL_RETURN(SendRequestForNetwork(nc, networkCallback, 0,
            REQUEST, type, (INetworkRequest**)&nr))
    return NOERROR;
}

ECode CConnectivityManager::RequestNetwork(
        /* [in] */ INetworkRequest* request,
        /* [in] */ IConnectivityManagerNetworkCallback* networkCallback,
        /* [in] */ Int32 timeoutMs)
{
    VALIDATE_NOT_NULL(request)
    VALIDATE_NOT_NULL(networkCallback)

    AutoPtr<INetworkCapabilities> nc;
    request->GetNetworkCapabilities((INetworkCapabilities**)&nc);
    Int32 type;
    FAIL_RETURN(InferLegacyTypeForNetworkCapabilities(nc, &type))
    AutoPtr<INetworkRequest> nr;
    FAIL_RETURN(SendRequestForNetwork(nc, networkCallback, timeoutMs,
            REQUEST, type, (INetworkRequest**)&nr))
    return NOERROR;
}

ECode CConnectivityManager::RequestNetwork(
        /* [in] */ INetworkRequest* request,
        /* [in] */ IPendingIntent* operation)
{
    VALIDATE_NOT_NULL(request)
    VALIDATE_NOT_NULL(operation)

    // try {
    //     mService.pendingRequestForNetwork(request.networkCapabilities, operation);
    // } catch (RemoteException e) {}
    AutoPtr<INetworkCapabilities> nc;
    request->GetNetworkCapabilities((INetworkCapabilities**)&nc);
    AutoPtr<INetworkRequest> nr;
    ECode ec = mService->PendingRequestForNetwork(nc, operation, (INetworkRequest**)&nr);
    if (E_REMOTE_EXCEPTION == ec) {
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::RegisterNetworkCallback(
    /* [in] */ INetworkRequest* request,
    /* [in] */ IConnectivityManagerNetworkCallback* networkCallback)
{
    VALIDATE_NOT_NULL(request)
    VALIDATE_NOT_NULL(networkCallback)

    AutoPtr<INetworkCapabilities> nc;
    FAIL_RETURN(request->GetNetworkCapabilities((INetworkCapabilities**)&nc))
    AutoPtr<INetworkRequest> nr;
    return SendRequestForNetwork(nc, networkCallback, 0, LISTEN, TYPE_NONE, (INetworkRequest**)&nr);
}

ECode CConnectivityManager::UnregisterNetworkCallback(
    /* [in] */ IConnectivityManagerNetworkCallback* networkCallback)
{
    if (NULL == networkCallback) {
        // throw new IllegalArgumentException("Invalid NetworkCallback");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    } else {
        if (NULL == ((ConnectivityManagerNetworkCallback*)networkCallback)->mNetworkRequest) {
            // throw new IllegalArgumentException("Invalid NetworkCallback");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        } else {
            Int32 requestId;
            ((ConnectivityManagerNetworkCallback*)networkCallback)->mNetworkRequest->GetRequestId(&requestId);
            if (REQUEST_ID_UNSET == requestId) {
                // throw new IllegalArgumentException("Invalid NetworkCallback");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
        }
    }
    // try {
    //     mService.releaseNetworkRequest(networkCallback.networkRequest);
    // } catch (RemoteException e) {}
    ECode ec = mService->ReleaseNetworkRequest(((ConnectivityManagerNetworkCallback*)networkCallback)->mNetworkRequest);
    if (E_REMOTE_EXCEPTION == ec) {
        return NOERROR;
    }
    return ec;
}

ECode CConnectivityManager::SetProcessDefaultNetwork(
        /* [in] */ INetwork* network,
        /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    Int32 netId;
    if (NULL == network) {
        netId = NETID_UNSET;
    } else {
        network->GetNetId(&netId);
    }
    Int32 netBound;
    AutoPtr<INetworkUtilsHelper> networkUtilsHelper;
#if 0 // TODO: Waiting for CNetworkUtilsHelper
    CNetworkUtilsHelper::AcquireSingleton((INetworkUtilsHelper**)&networkUtilsHelper);
#else
    assert(0);
#endif
    FAIL_RETURN(networkUtilsHelper->GetNetworkBoundToProcess(&netBound))
    if (netBound == netId) {
        *result = TRUE;
        return NOERROR;
    }
    Boolean b;
    FAIL_RETURN(networkUtilsHelper->BindProcessToNetwork(netId, &b))
    if (b) {
        // Must flush DNS cache as new network may have different DNS resolutions.
        AutoPtr<IInetAddressHelper> inetAddressHelper;
#if 0 // TODO: Waiting for CInetAddressHelper
        CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&inetAddressHelper);
#else
        assert(0);
#endif
        FAIL_RETURN(inetAddressHelper->ClearDnsCache())
#if 0 // TODO: Waiting for NetworkEventDispatcher
        AutoPtr<INetworkEventDispatcherHelper>  networkEventDispatcherHelper;
        CNetworkEventDispatcherHelper::AcquireSingleton((INetworkEventDispatcherHelper**)&networkEventDispatcherHelper);
        AutoPtr<INetworkEventDispatcher> networkEventDispatcher;
        FAIL_RETURN(networkEventDispatcherHelper->GetInstance((INetworkEventDispatcher**)&networkEventDispatcher))
        FAIL_RETURN(networkEventDispatcher->OnNetworkConfigurationChanged())
#else
        assert(0);
#endif
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CConnectivityManager::GetProcessDefaultNetwork(
    /* [out] */ INetwork** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;

    AutoPtr<INetworkUtilsHelper> networkUtilsHelper;
#if 0 // TODO: Waiting for CNetworkUtilsHelper
    CNetworkUtilsHelper::AcquireSingleton((INetworkUtilsHelper**)&networkUtilsHelper);
#else
    assert(0);
#endif
    Int32 netId;
    networkUtilsHelper->GetNetworkBoundToProcess(&netId);
    if (NETID_UNSET == netId) {
        *result = NULL;
        return NOERROR;
    }
#if 0 // TODO: Waiting for CNetwork
    CNetwork::New(netId, result);
#else
    assert(0);
#endif
    return NOERROR;
}

ECode CConnectivityManager::SetProcessDefaultNetworkForHostResolution(
    /* [in] */ INetwork* network,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    Int32 netId = NETID_UNSET;
    if (NULL == network) {
        network->GetNetId(&netId);
    }
    AutoPtr<INetworkUtilsHelper> networkUtilsHelper;
#if 0 // TODO: Waiting for CNetworkUtilsHelper
    CNetworkUtilsHelper::AcquireSingleton((INetworkUtilsHelper**)&networkUtilsHelper);
#else
    assert(0);
#endif
    networkUtilsHelper->BindProcessToNetworkForHostResolution(netId, result);
    return NOERROR;
}

CConnectivityManager::LegacyRequest::LegacyRequest()
    : mDelay(-1)
    , mNetworkCallback(new InnerSub_ConnectivityManagerNetworkCallback(this))
{}

CConnectivityManager::CallbackHandler::CallbackHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ IHashMap* callbackMap,
    /* [in] */ IAtomicInteger32* refCount,
    /* [in] */ CConnectivityManager* cm)
    : Handler(looper)
    , mCallbackMap(callbackMap)
    , mRefCount(refCount)
    , mCm(cm)
{}

ECode CConnectivityManager::CallbackHandler::HandleMessage(
    /* [in] */ IMessage* message)
{
    VALIDATE_NOT_NULL(message)

    // Log.d(TAG, "CM callback handler got msg " + message.what);
    Int32 what;
    message->GetWhat(&what);
    Logger::D(TAG, (String("CM callback handler got msg ") + StringUtils::ToString(what)).string());
    switch (what) {
        case CALLBACK_PRECHECK: {
            AutoPtr<IInterface> request;
#if 0 // TODO: Waiting for NetworkRequest
            GetObject(message, ECLSID_NetworkRequest, (IInterface**)&request);
#else
            assert(0);
#endif
            AutoPtr<IConnectivityManagerNetworkCallback> callbacks;
            GetCallbacks(INetworkRequest::Probe(request), (IConnectivityManagerNetworkCallback**)&callbacks);
            if (callbacks != NULL) {
                AutoPtr<IInterface> network;
#if 0 // TODO: Waiting for class Network
                GetObject(message, ECLSID_Network, (IInterface**)&network);
#else
                assert(0);
#endif
                callbacks->OnPreCheck(INetwork::Probe(network));
            } else {
                // Log.e(TAG, "callback not found for PRECHECK message");
                Logger::E(TAG, String("callback not found for PRECHECK message").string());
            }
            break;
        }
        case CALLBACK_AVAILABLE: {
            AutoPtr<IInterface> request;
#if 0 // TODO: Waiting for NetworkRequest
            GetObject(message, ECLSID_NetworkRequest, (IInterface**)&request);
#else
            assert(0);
#endif
            AutoPtr<IConnectivityManagerNetworkCallback> callbacks;
            GetCallbacks(INetworkRequest::Probe(request), (IConnectivityManagerNetworkCallback**)&callbacks);
            if (callbacks != NULL) {
                AutoPtr<IInterface> network;
#if 0 // TODO: Waiting for class Network
                GetObject(message, ECLSID_Network, (IInterface**)&network);
#else
                assert(0);
#endif
                callbacks->OnAvailable(INetwork::Probe(network));
            } else {
                // Log.e(TAG, "callback not found for AVAILABLE message");
                Logger::E(TAG, String("callback not found for AVAILABLE message").string());
            }
            break;
        }
        case CALLBACK_LOSING: {
            AutoPtr<IInterface> request;
#if 0 // TODO: Waiting for NetworkRequest
            GetObject(message, ECLSID_NetworkRequest, (IInterface**)&request);
#else
            assert(0);
#endif

            AutoPtr<IConnectivityManagerNetworkCallback> callbacks;
            GetCallbacks(INetworkRequest::Probe(request), (IConnectivityManagerNetworkCallback**)&callbacks);
            if (callbacks != NULL) {
                AutoPtr<IInterface> network;
#if 0 // TODO: Waiting for class Network
                GetObject(message, ECLSID_Network, (IInterface**)&network);
#else
                assert(0);
#endif
                Int32 arg1;
                message->GetArg1(&arg1);
                callbacks->OnLosing(INetwork::Probe(network), arg1);
            } else {
                // Log.e(TAG, "callback not found for LOSING message");
                Logger::E(TAG, String("callback not found for LOSING message").string());
            }
            break;
        }
        case CALLBACK_LOST: {
            AutoPtr<IInterface> request;
#if 0 // TODO: Waiting for NetworkRequest
            GetObject(message, ECLSID_NetworkRequest, (IInterface**)&request);
#else
            assert(0);
#endif
            AutoPtr<IConnectivityManagerNetworkCallback> callbacks;
            GetCallbacks(INetworkRequest::Probe(request), (IConnectivityManagerNetworkCallback**)&callbacks);
            if (callbacks != NULL) {
                AutoPtr<IInterface> network;
#if 0 // TODO: Waiting for class Network
                GetObject(message, ECLSID_Network, (IInterface**)&network);
#else
                assert(0);
#endif
                callbacks->OnLost(INetwork::Probe(network));
            } else {
                // Log.e(TAG, "callback not found for LOST message");
                Logger::E(TAG, String("callback not found for LOST message").string());
            }
            break;
        }
        case CALLBACK_UNAVAIL: {
            AutoPtr<IInterface> request;
#if 0 // TODO: Waiting for NetworkRequest
            GetObject(message, ECLSID_NetworkRequest, (IInterface**)&request);
#else
            assert(0);
#endif
            AutoPtr<IInterface> callbacks = NULL;
            synchronized(mCallbackMap) {
                mCallbackMap->Get(request, (IInterface**)&callbacks);
            }
            if (callbacks != NULL) {
                AutoPtr<IInterface> network;
#if 0 // TODO: Waiting for class Network
                GetObject(message, ECLSID_Network, (IInterface**)&network);
#else
                assert(0);
#endif
                IConnectivityManagerNetworkCallback::Probe(callbacks)->OnUnavailable();
            } else {
                // Log.e(TAG, "callback not found for UNAVAIL message");
                Logger::E(TAG, String("callback not found for UNAVAIL message").string());
            }
            break;
        }
        case CALLBACK_CAP_CHANGED: {
            AutoPtr<IInterface> request;
#if 0 // TODO: Waiting for NetworkRequest
            GetObject(message, ECLSID_NetworkRequest, (IInterface**)&request);
#else
            assert(0);
#endif
            AutoPtr<IConnectivityManagerNetworkCallback> callbacks;
            GetCallbacks(INetworkRequest::Probe(request), (IConnectivityManagerNetworkCallback**)&callbacks);
            if (callbacks != NULL) {
                AutoPtr<IInterface> network;
#if 0 // TODO: Waiting for Network
                GetObject(message, ECLSID_Network, (IInterface**)&network);
#else
                assert(0);
#endif
                AutoPtr<IInterface> cap;
#if 0 // TODO: Waiting for NetworkCapabilities
                GetObject(message, ECLSID_NetworkCapabilities, (IInterface**)&cap);
#else
            assert(0);
#endif
                callbacks->OnCapabilitiesChanged(INetwork::Probe(network), INetworkCapabilities::Probe(cap));
            } else {
                // Log.e(TAG, "callback not found for CAP_CHANGED message");
                Logger::E(TAG, String("callback not found for CAP_CHANGED message").string());
            }
            break;
        }
        case CALLBACK_IP_CHANGED: {
            AutoPtr<IInterface> request;
#if 0 // TODO: Waiting for NetworkRequest
            GetObject(message, ECLSID_NetworkRequest, (IInterface**)&request);
#else
            assert(0);
#endif
            AutoPtr<IConnectivityManagerNetworkCallback> callbacks;
            GetCallbacks(INetworkRequest::Probe(request), (IConnectivityManagerNetworkCallback**)&callbacks);
            if (callbacks != NULL) {
                AutoPtr<IInterface> network;
#if 0 // TODO: Waiting for class Network
                GetObject(message, ECLSID_Network, (IInterface**)&network);
#else
                assert(0);
#endif
                AutoPtr<IInterface> lp;
#if 0 // TODO: Waiting for LinkProperties
                GetObject(message, ECLSID_LinkProperties, (IInterface**)&lp);
#else
                assert(0);
#endif

                callbacks->OnLinkPropertiesChanged(INetwork::Probe(network), ILinkProperties::Probe(lp));
            } else {
                // Log.e(TAG, "callback not found for IP_CHANGED message");
                Logger::E(TAG, String("callback not found for IP_CHANGED message").string());
            }
            break;
        }
        case CALLBACK_RELEASED: {
            AutoPtr<IInterface> req;
#if 0 // TODO: Waiting for NetworkRequest
            GetObject(message, ECLSID_NetworkRequest, (IInterface**)&req)
#else
            assert(0);
#endif
            AutoPtr<IInterface> callbacks = NULL;
            synchronized(mCallbackMap) {
                mCallbackMap->Remove(req, (IInterface**)&callbacks);
            }
            if (callbacks != NULL) {
                synchronized(mRefCount) {
                    Int32 count;
                    mRefCount->DecrementAndGet(&count);
                    if (0 == count) {
                        AutoPtr<ILooper> looper;
                        GetLooper((ILooper**)&looper);
                        looper->Quit();
                    }
                }
            } else {
                // Log.e(TAG, "callback not found for CANCELED message");
                Logger::E(TAG, String("callback not found for CANCELED message").string());
            }
            break;
        }
        case CALLBACK_EXIT: {
            // Log.d(TAG, "Listener quiting");
            Logger::D(TAG, String("Listener quiting").string());
            AutoPtr<ILooper> looper;
            GetLooper((ILooper**)&looper);
            looper->Quit();
            break;
        }
        case EXPIRE_LEGACY_REQUEST: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            Int32 arg1;
            message->GetArg1(&arg1);
            mCm->ExpireRequest(INetworkCapabilities::Probe(obj), arg1);
            break;
        }
    }
    return NOERROR;
}

CConnectivityManager::LegacyRequest::InnerSub_ConnectivityManagerNetworkCallback::InnerSub_ConnectivityManagerNetworkCallback(LegacyRequest* const host)
    :   mHost(host)
{}

ECode CConnectivityManager::LegacyRequest::InnerSub_ConnectivityManagerNetworkCallback::OnAvailable(
    /* [in] */ INetwork* network)
{
    mHost->mCurrentNetwork = network;
    // Log.d(TAG, "startUsingNetworkFeature got Network:" + network);
    String s;
    IObject::Probe(network)->ToString(&s);
    Logger::D(TAG, (String("startUsingNetworkFeature got Network:") + s).string());
    Boolean b;
    CConnectivityManager::SetProcessDefaultNetworkForHostResolution(network, &b);
    return NOERROR;
}

ECode CConnectivityManager::LegacyRequest::InnerSub_ConnectivityManagerNetworkCallback::OnLost(
    /* [in] */ INetwork* network)
{
    VALIDATE_NOT_NULL(network)

    Boolean b;
    if (IObject::Probe(network)->Equals(mHost->mCurrentNetwork, &b), b) {
        mHost->mCurrentNetwork = NULL;
        SetProcessDefaultNetworkForHostResolution(NULL, &b);
    }
    String s;
    IObject::Probe(network)->ToString(&s);
    Logger::D(TAG, (String("startUsingNetworkFeature lost Network:") + s).string());
    return NOERROR;
}

ECode CConnectivityManager::CallbackHandler::GetObject(
    /* [in] */ IMessage* msg,
    /* [in] */ ClassID c,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;
    VALIDATE_NOT_NULL(msg)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Waiting for the method to get name from classid
    return msg.getData().getParcelable(c.getSimpleName());
#endif
}

ECode CConnectivityManager::CallbackHandler::GetCallbacks(
    /* [in] */ INetworkRequest* req,
   /* [out] */ IConnectivityManagerNetworkCallback** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;
    VALIDATE_NOT_NULL(req)

    synchronized(mCallbackMap) {
        AutoPtr<IInterface> value;
        mCallbackMap->Get(req, (IInterface**)&value);
        *result = IConnectivityManagerNetworkCallback::Probe(value);
        REFCOUNT_ADD(*result)
    }
    return NOERROR;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
