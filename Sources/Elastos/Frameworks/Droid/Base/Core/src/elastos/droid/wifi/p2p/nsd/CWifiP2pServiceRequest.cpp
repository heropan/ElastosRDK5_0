
#include "CWifiP2pServiceRequest.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Wifi {
namespace P2p {
namespace Nsd {

ECode CWifiP2pServiceRequest::constructor()
{
    return NOERROR;
}

ECode CWifiP2pServiceRequest::constructor(
    /* [in] */ Int32 protocolType,
    /* [in] */ const String& query)
{
    return WifiP2pServiceRequest::Init(protocolType, query);
}

ECode CWifiP2pServiceRequest::constructor(
    /* [in] */ Int32 serviceType,
    /* [in] */ Int32 length,
    /* [in] */ Int32 transId,
    /* [in] */ const String& query)
{
    return WifiP2pServiceRequest::Init(
        serviceType, length, transId, query);
}

PInterface CWifiP2pServiceRequest::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_WifiP2pServiceRequest) {
        return reinterpret_cast<PInterface>((WifiP2pServiceRequest*)this);
    }
    return _CWifiP2pServiceRequest::Probe(riid);
}

ECode CWifiP2pServiceRequest::GetTransactionId(
    /* [out] */ Int32* transactionId)
{
    return WifiP2pServiceRequest::GetTransactionId(transactionId);
}

ECode CWifiP2pServiceRequest::SetTransactionId(
    /* [in] */ Int32 id)
{
    return WifiP2pServiceRequest::SetTransactionId(id);
}

ECode CWifiP2pServiceRequest::GetSupplicantQuery(
    /* [out] */ String* supplicantQuery)
{
    return WifiP2pServiceRequest::GetSupplicantQuery(supplicantQuery);
}

ECode CWifiP2pServiceRequest::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    return WifiP2pServiceRequest::Equals(obj, isEqual);
}

ECode CWifiP2pServiceRequest::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return WifiP2pServiceRequest::GetHashCode(hashCode);
}

ECode CWifiP2pServiceRequest::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return WifiP2pServiceRequest::ReadFromParcel(source);
}

ECode CWifiP2pServiceRequest::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return WifiP2pServiceRequest::WriteToParcel(dest);
}

ECode CWifiP2pServiceRequest::NewInstance(
    /* [in] */ Int32 protocolType,
    /* [in] */ const String& queryData,
    /* [out] */ IWifiP2pServiceRequest** instance)
{
    VALIDATE_NOT_NULL(instance);
    AutoPtr<CWifiP2pServiceRequest> obj;
    CWifiP2pServiceRequest::NewByFriend(protocolType, queryData, (CWifiP2pServiceRequest**)&obj);
    *instance = obj;
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode CWifiP2pServiceRequest::NewInstance(
    /* [in] */ Int32 protocolType,
    /* [out] */ IWifiP2pServiceRequest** instance)
{
    return NewInstance(protocolType, String(NULL), instance);
}

}
}
}
}
}
