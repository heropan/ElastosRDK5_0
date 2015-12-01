
#include "elastos/droid/wifi/p2p/nsd/CWifiP2pDnsSdServiceRequest.h"
#include "elastos/droid/wifi/p2p/nsd/CWifiP2pServiceRequest.h"
#include "elastos/droid/wifi/p2p/nsd/CWifiP2pDnsSdServiceInfoHelper.h"
#include "elastos/droid/wifi/p2p/nsd/CWifiP2pDnsSdServiceInfo.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Wifi {
namespace P2p {
namespace Nsd {

CAR_OBJECT_IMPL(CWifiP2pDnsSdServiceRequest)

ECode CWifiP2pDnsSdServiceRequest::constructor()
{
    return WifiP2pServiceRequest::constructor(
        IWifiP2pServiceInfo::SERVICE_TYPE_BONJOUR, String(NULL));
}


ECode CWifiP2pDnsSdServiceRequest::constructor(
    /* [in] */ const String& query)
{
    return WifiP2pServiceRequest::constructor(
        IWifiP2pServiceInfo::SERVICE_TYPE_BONJOUR, query);
}

ECode CWifiP2pDnsSdServiceRequest::constructor(
    /* [in] */ const String& dnsQuery,
    /* [in] */ Int32 dnsType,
    /* [in] */ Int32 versionNumber)
{
    AutoPtr<IWifiP2pDnsSdServiceInfoHelper> helper;
    FAIL_RETURN(CWifiP2pDnsSdServiceInfoHelper::AcquireSingleton(
        (IWifiP2pDnsSdServiceInfoHelper**)&helper));

    String temp;
    assert(0);
    // TODO
    // FAIL_RETURN(helper->CreateRequest(
    //     dnsQuery, dnsType, versionNumber, &temp));

    return WifiP2pServiceRequest::constructor(
        IWifiP2pServiceInfo::SERVICE_TYPE_BONJOUR, temp);
}

ECode CWifiP2pDnsSdServiceRequest::NewInstance(
    /* [out] */ IWifiP2pDnsSdServiceRequest** instance)
{
    VALIDATE_NOT_NULL(instance);
    *instance = NULL;

    AutoPtr<CWifiP2pDnsSdServiceRequest> obj;
    CWifiP2pDnsSdServiceRequest::NewByFriend((CWifiP2pDnsSdServiceRequest**)&obj);
    *instance = (IWifiP2pDnsSdServiceRequest*)obj.Get();
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode CWifiP2pDnsSdServiceRequest::NewInstance(
    /* [in] */ const String& serviceType,
    /* [out] */ IWifiP2pDnsSdServiceRequest** instance)
{
    VALIDATE_NOT_NULL(instance);
    *instance = NULL;

    if (serviceType.IsNull()) {
        Slogger::E("CWifiP2pDnsSdServiceRequest", "service type cannot be null!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<CWifiP2pDnsSdServiceRequest> obj;
    CWifiP2pDnsSdServiceRequest::NewByFriend(
        serviceType + String(".local."),
        IWifiP2pDnsSdServiceInfo::DNS_TYPE_PTR,
        IWifiP2pDnsSdServiceInfo::VERSION_1,
        (CWifiP2pDnsSdServiceRequest**)&obj);
    *instance = (IWifiP2pDnsSdServiceRequest*)obj.Get();
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode CWifiP2pDnsSdServiceRequest::NewInstance(
    /* [in] */ const String& instanceName,
    /* [in] */ const String& serviceType,
    /* [out] */ IWifiP2pDnsSdServiceRequest** instance)
{
    VALIDATE_NOT_NULL(instance);
    *instance = NULL;

    if (instanceName.IsNull() || serviceType.IsNull()) {
        Slogger::E("CWifiP2pDnsSdServiceRequest", "instance name or service type cannot be null!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    String fullDomainName =
        instanceName + String(".") + serviceType + String(".local.");

    AutoPtr<CWifiP2pDnsSdServiceRequest> obj;
    CWifiP2pDnsSdServiceRequest::NewByFriend(
        fullDomainName,
        IWifiP2pDnsSdServiceInfo::DNS_TYPE_TXT,
        IWifiP2pDnsSdServiceInfo::VERSION_1,
        (CWifiP2pDnsSdServiceRequest**)&obj);
    *instance = (IWifiP2pDnsSdServiceRequest*)obj.Get();
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode CWifiP2pDnsSdServiceRequest::GetTransactionId(
    /* [out] */ Int32* transactionId)
{
    return WifiP2pServiceRequest::GetTransactionId(transactionId);
}

ECode CWifiP2pDnsSdServiceRequest::SetTransactionId(
    /* [in] */ Int32 id)
{
    return WifiP2pServiceRequest::SetTransactionId(id);
}

ECode CWifiP2pDnsSdServiceRequest::GetSupplicantQuery(
    /* [out] */ String* supplicantQuery)
{
    return WifiP2pServiceRequest::GetSupplicantQuery(supplicantQuery);
}

ECode CWifiP2pDnsSdServiceRequest::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    return WifiP2pServiceRequest::Equals(obj, isEqual);
}

ECode CWifiP2pDnsSdServiceRequest::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return WifiP2pServiceRequest::GetHashCode(hashCode);
}

ECode CWifiP2pDnsSdServiceRequest::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return WifiP2pServiceRequest::ReadFromParcel(source);
}

ECode CWifiP2pDnsSdServiceRequest::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return WifiP2pServiceRequest::WriteToParcel(dest);
}

} // namespace Nsd
} // namespace P2p
} // namespace Wifi
} // namespace Droid
} // namespace Elastos
